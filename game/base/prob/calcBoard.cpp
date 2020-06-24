#include "calcBoard.hpp"
#include <cassert>

template<class T>
template<class... Args>
PBoard<T>::PBoard (
    PBoard::UInt size,
    Args... args)
    :       _size(size),
            _evaluation(size, Vector(size)),
            _extraLayer(_evaluation),
            _searchMode(TargetMode::Hunt)
{
    (assert(args > 0 && args <= 5), ...);
    (++_boatSizes[args], ...);

    for (auto[size, count] : _boatSizes)
        _addBoat(size);
}

template<class T>
bool
PBoard<T>::inRange (
    Point<T> c)
{
    return 0 <= c.y && c.y < _size &&
           0 <= c.x && c.x < _size;
}

template<class T>
typename PBoard<T>::UInt
PBoard<T>::size () const
{
    return _size;
}

template<class T>
std::optional<T>
PBoard<T>::at (
    const Point<T> & c,
    const bool & huntMode)
{
    if (!inRange(c))
        return std::nullopt;
    else if (huntMode)
        return std::make_optional(_evaluation[c.y][c.x]);
    T nVal = 0.25 * _evaluation[c.y][c.x] + _extraLayer[c.y][c.x];
    return std::make_optional(nVal);
}

template<class T>
UpdateMessage
PBoard<T>::checkImpact (
    const Point<T> & coordinate)
{
    char input;
    std::cin >> input;
    switch (input)
    {
        case 's':
            return UpdateMessage::Sunk ;
        case 'h':
            return UpdateMessage::Hit;
        default:
            return UpdateMessage::Miss;
    }
}

template<class T>
void
PBoard<T>::propagate (
    const Point<T> & coordinate,
    UpdateMessage update,
    const UInt & removed)
{
    using namespace Direction;

    if (update == UpdateMessage::Sunk)
    {
        removeBoat(removed);
        changeMode(TargetMode::Hunt);
    }
    else if (update == UpdateMessage::Hit &&
             _searchMode != TargetMode::Target)
    {
        changeMode(TargetMode::Target);
    }

    for (auto&[size, count] : _boatSizes)
    {
        _huntPropagate(
            coordinate, Up, Down, size);

        _huntPropagate(
            coordinate, Left, Right, size);
    }
}

template<class T>
void
PBoard<T>::print ()
{
    for (const auto & line : _evaluation)
        std::cout << line << std::endl;
    std::cout << std::endl;
}

template<class T>
void
PBoard<T>::removeBoat (
    PBoard::UInt boatSize)
{
    assert(_boatSizes.find(boatSize) != _boatSizes.end());
    _boatSizes[boatSize] -= 1;
    _recalculate();
}

template<class T>
void PBoard<T>::clearUpdates ()
{
    _updates.clear();
}

template<class T>
void
PBoard<T>::_overlap (
    PBoard::Matrix & base,
    const PBoard::Matrix & overlay)
{
    for (unsigned i = 0; i < _size; ++i)
        for (unsigned j = 0; j < _size; ++j)
            base[i][j] += overlay[i][j];
}

template<class T>
void
PBoard<T>::_addBoat (
    const PBoard::UInt & boatSize)
{
    if (_boatProbabilities.find(boatSize) != _boatProbabilities.end())
    {
        _overlap(_evaluation, _boatProbabilities[boatSize]);
        return;
    }

    auto count = _boatSizes[boatSize];
    Matrix verticalMatrix;
    Vector horizontalLine;

    for (UInt i = 0; i < _size; ++i)
    {
        UInt value = count * min(i + 1, _size - i, boatSize);
        verticalMatrix.push_back(Vector(_size, value));
        horizontalLine.push_back(value);
    }
    Matrix layer(_size, horizontalLine);
    _overlap(layer, verticalMatrix);

    _overlap(_evaluation, layer);
    for (auto & row : layer)
        for (auto & value : row)
            value /= count;

    _boatProbabilities.emplace(boatSize, layer);
}

template<class T>
Point<T>
PBoard<T>::_findBoundary (
    const Point<T> & current,
    const Direction::type & direction,
    const PBoard::UInt & boatSize,
    const PBoard::UInt & depth)
{
    auto next = current + direction;
    auto val = this->at(next);
    if (!val.has_value() || *val == 0 || depth == boatSize)
        return current;
    return _findBoundary(next, direction, boatSize, depth + 1);
}

template<class T>
typename PBoard<T>::template Container<int>
PBoard<T>::_makeWindow (
    const Point<T> & coordinate,
    const Point<T> & bound1,
    const Point<T> & bound2,
    const Direction::type & direction,
    const PBoard::UInt & size)
{
    auto dist = bound2 - bound1;
    if (dist.originDistance() < size - 1)
        return {};

    Container<int> window(size, 0);

    window[0] = 1;
    auto temp = bound1;
    auto temp2 = bound1 + Point<T>(direction) * (size - 1);

    for (int i = 1; temp != coordinate && temp2 != bound2; ++i)
    {
        temp = temp + direction;
        temp2 = temp2 + direction;
        window[i] = 1;
        window.push_back(-1);
    }
    return window;
}

template<class T>
void
PBoard<T>::_huntPropagate (
    const Point<T> & coordinate,
    const Direction::type & firstDirection,
    const Direction::type & endDirection,
    const PBoard::UInt & size)
{
    auto firstBoundary =
        _findBoundary(
            coordinate, firstDirection, size);

    auto endBoundary =
        _findBoundary(
            coordinate, endDirection, size);

    auto window =
        _makeWindow(
            coordinate, firstBoundary, endBoundary, endDirection, size);

    int accumulated = 0;
    for (const auto & i : window)
    {
        accumulated += i;
        auto&[x, y] = firstBoundary;
        auto count = _boatSizes[size];

        _evaluation[y][x] -= count * accumulated;
        _boatProbabilities[size][y][x] -= accumulated;

        _updates.emplace(firstBoundary);
        firstBoundary = firstBoundary + endDirection;
    }
//    _evaluation[coordinate.y][coordinate.x] = 0;

}

template<class T>
void
PBoard<T>::_recalculate ()
{
    Matrix localEvaluation(_evaluation.size(), Vector(_evaluation.size(), 0));

    for (auto&[size, count] : _boatSizes)
    {
        auto layer = _boatProbabilities[size];
        for (auto & row : layer)
            for (auto & value : row)
                value *= count;

        _overlap(localEvaluation, layer);
    }
    for (unsigned i = 0; i < _evaluation.size(); ++i)
        for (unsigned j = 0; j < _evaluation.size(); ++j)
            if (_evaluation[i][j] != localEvaluation[i][j])
                _updates.emplace(i, j);

    _evaluation = localEvaluation;
}

