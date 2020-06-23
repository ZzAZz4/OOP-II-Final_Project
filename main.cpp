#include <iostream>
#include <vector>
#include <cassert>
#include <iomanip>
#include <set>
#include <optional>
#include <queue>
#include <unordered_map>
#include <map>
#include <cmath>
#include <unordered_set>


#include "utility/functions.h"

namespace Direction
{

    enum class type
    {
        Up, Right, Down, Left, None
    };

    constexpr auto Up = type::Up;
    constexpr auto Right = type::Right;
    constexpr auto Down = type::Down;
    constexpr auto Left = type::Left;
    constexpr auto None = type::None;
}

template<class T = int>
struct p2Vector
{
    T x, y;

    p2Vector (T x, T y)
        : x(x), y(y)
    {}

    explicit p2Vector (
        Direction::type dir)
    {
        switch (dir)
        {
            case Direction::Up:
                x = 0;
                y = -1;
                break;
            case Direction::Right:
                x = 1;
                y = 0;
                break;
            case Direction::Down:
                x = 0;
                y = 1;
                break;
            case Direction::Left:
                x = -1;
                y = 0;
                break;
            case Direction::None:
                x = 0;
                y = 0;
                break;
        }

    }

    constexpr
    p2Vector
    operator +
        (p2Vector other) const
    {
        other.x += x;
        other.y += y;
        return other;
    }

    constexpr
    p2Vector
    operator +
        (const Direction::type & direction) const
    {
        return *this + p2Vector(direction);
    }

    constexpr
    p2Vector
    operator -
        (p2Vector other) const
    {
        auto r = *this;
        r.x -= other.x;
        r.y -= other.y;
        return r;
    }

    constexpr
    p2Vector
    operator *
        (const T & scalar) const
    {
        auto r = *this;
        r.x *= scalar;
        r.y *= scalar;
        return r;
    }

    constexpr
    p2Vector
    operator /
        (const T & scalar) const
    {
        auto r = *this;
        r.x /= scalar;
        r.y /= scalar;
        return r;
    }

    constexpr
    bool
    operator ==
        (const p2Vector & other) const
    { return x == other.x && y == other.y; }

    constexpr
    bool
    operator !=
        (const p2Vector & other) const
    { return x != other.x || y != other.y; }

    [[nodiscard]]
    constexpr
    double modulus () const
    {
        return sqrt(x * x + y * y);
    }

    constexpr
    bool operator < (p2Vector other) const
    {
        return modulus() < other.modulus();
    }
};

namespace std
{
    template<typename T>
    struct hash<p2Vector<T>>
    {
        inline std::size_t operator ()
            (const p2Vector<T> & p) const
        {
            return p.y << 16 ^ p.x;
        }
    };
}


template<class T = int>
class PBoard
{
    template<class U>
    using Container = std::vector<U>;
    using Vector = Container<T>;
    using Matrix = Container<Container<T>>;
    using UInt = uint32_t;
    using CountMap = std::map<UInt, UInt, std::greater<>>;
    using BoardMap = std::map<UInt, Matrix>;
    using USet = std::unordered_set<p2Vector<T>>;

    UInt _size{};
    Matrix _evaluation;
    CountMap _boatSizes;
    BoardMap _boatProbabilities = {};

    void
    _append (
        Matrix & base,
        const Matrix & overlay)
    {
        for (unsigned i = 0; i < _size; ++i)
            for (unsigned j = 0; j < _size; ++j)
                base[i][j] += overlay[i][j];
    }

    void
    _addBoat (
        const UInt & boatSize)
    {
        if (_boatProbabilities.find(boatSize) != _boatProbabilities.end())
        {
            _append(_evaluation, _boatProbabilities[boatSize]);
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
        _append(layer, verticalMatrix);

        _append(_evaluation, layer);
        for (auto & row : layer)
            for (auto & value : row)
                value /= count;

        _boatProbabilities.emplace(boatSize, layer);
    }

    p2Vector<T>
    _findBoundary (
        const p2Vector<T> & current,
        const Direction::type & direction,
        const UInt & boatSize,
        const UInt & depth = 1)
    {
        auto next = current + direction;
        auto val = this->at(next);
        if (!val.has_value() || *val == 0 || depth == boatSize)
            return current;
        return _findBoundary(next, direction, boatSize, depth + 1);
    }

    Container<int>
    _makeWindow (
        const p2Vector<T> & coordinate,
        const p2Vector<T> & bound1,
        const p2Vector<T> & bound2,
        const Direction::type & direction,
        const UInt & size)
    {
        if ((bound1 - bound2).modulus() < size - 1)
            return {};

        Container<int> window(size, 0);

        window[0] = 1;
        auto temp = bound1;
        auto temp2 = bound1 + p2Vector<T>(direction) * (size - 1);

        for (int i = 1; temp != coordinate && temp2 != bound2; ++i)
        {
            temp = temp + direction;
            temp2 = temp2 + direction;
            window[i] = 1;
            window.push_back(-1);
        }
        return window;
    };

    void
    _updateNeighbors (
        const p2Vector<T> & coordinate,
        const Direction::type & firstDirection,
        const Direction::type & endDirection,
        const UInt & size)
    {
        {
            auto firstBoundary =
                _findBoundary(
                    coordinate,
                    firstDirection,
                    size);

            auto endBoundary =
                _findBoundary(
                    coordinate,
                    endDirection,
                    size);

            auto windowY =
                _makeWindow(
                    coordinate,
                    firstBoundary,
                    endBoundary,
                    endDirection,
                    size);

            int accumulated = 0;
            for (const auto & i : windowY)
            {
                accumulated += i;
                auto&[x, y] = firstBoundary;
                auto count = _boatSizes[size];

                _evaluation[y][x] -= count * accumulated;
                _boatProbabilities[size][y][x] -= accumulated;

                _updates.emplace(firstBoundary);
                firstBoundary = firstBoundary + endDirection;
            }
        }
    }


    void
    _recalculate ()
    {
        Matrix localEvaluation;
        localEvaluation.resize(_evaluation.size());
        for (auto & row : localEvaluation)
            row.resize(_evaluation.size());

        for (auto&[size, count] : _boatSizes)
        {
            auto layer = _boatProbabilities[size];
            for (auto & row : layer)
                for (auto & value : row)
                    value *= count;

            _append(localEvaluation, layer);
        }
        for (unsigned i = 0; i < _evaluation.size(); ++i)
            for (unsigned j = 0; j < _evaluation.size(); ++j)
                if (_evaluation[i][j] != localEvaluation[i][j])
                    _updates.emplace(i, j);

        _evaluation = localEvaluation;
    }

public:
    USet _updates = {};

    template<class ... Args>
    explicit
    PBoard (
        UInt size,
        Args ... args)
        :       _size(size),
                _evaluation(size, Container<T>(size))
    {
        (assert(args > 0 && args <= 5), ...);
        (++_boatSizes[args], ...);

        for (auto[size, count] : _boatSizes)
            _addBoat(size);
    }

    [[nodiscard]]
    UInt
    size () const
    {
        return _size;
    }

    bool
    inRange
        (p2Vector<T> c)
    {
        return 0 <= c.y && c.y < _size &&
               0 <= c.x && c.x < _size;
    }

    std::optional<T>
    at
        (p2Vector<T> c)
    {
        if (!inRange(c))
            return std::nullopt;
        return std::make_optional(_evaluation[c.y][c.x]);
    }


    void shootPosition
        (p2Vector<T> coordinate)
    {
        using namespace Direction;
        for (auto&[size, count] : _boatSizes)
        {
            _updateNeighbors(coordinate, Up, Down, size);
            _updateNeighbors(coordinate, Left, Right, size);
        }
    }

    void
    print ()
    {
        for (const auto & line : _evaluation)
            std::cout << line << std::endl;
        std::cout << std::endl;
    }

    void
    sinkBoat (UInt boatSize)
    {
        assert(_boatSizes.find(boatSize) != _boatSizes.end());
        _boatSizes[boatSize] -= 1;
        _recalculate();
    }

    void
    clearUpdates ()
    {
        _updates.clear();
    }
};

struct AiPlayer
{
    enum class State
    {
        Hunt, Target
    };

    struct BoardPosition
    {
        p2Vector<> coordinate;
        PBoard<> * board;

        bool operator < (BoardPosition other) const
        {
            auto f = *board->at(coordinate);
            auto s = *board->at(other.coordinate);
            if (f == s)
            {
                return coordinate.modulus() < other.coordinate.modulus();
            }
            return f >= s;
        }

        bool operator == (BoardPosition other)
        {
            return coordinate == other.coordinate;
        }


        BoardPosition (
            int x,
            int y,
            PBoard<> & pBoard)
            : coordinate(x, y), board(&pBoard)
        {}

        BoardPosition (
            p2Vector<> position,
            PBoard<> & pBoard)
            : coordinate(position.x, position.y), board(&pBoard)
        {}

        [[nodiscard]]
        p2Vector<> pos () const
        {
            return coordinate;
        };
    };

    using MSet = std::multiset<BoardPosition>;
    using Board = PBoard<>;

    Board board;
    MSet priorities;
    State state;


public:
    template<class ... Args>
    explicit AiPlayer (uint32_t size, Args ... boat_sizes)
        : board(size, boat_sizes...), state(State::Hunt)
    {
        for (unsigned x = 0; x < board.size(); ++x)
            for (unsigned y = 0; y < board.size(); ++y)
                priorities.emplace(x, y, board);
    }

    void printChoices ()
    {
        for (auto value : priorities)
            std::cout << value.pos().x << ' ' << value.pos().y << ' ' << *board.at(value.pos()) << '\n';
    }

    void printBoard ()
    {
        board.print();
    }

    void chooseBestMove ()
    {
        auto choice = (*priorities.begin());
        board.shootPosition(choice.coordinate);
        updatePriority();
        printBoard();
    }

    void updatePriority ()
    {
        //priorities = std::multiset<BoardPosition>(begin(priorities), end(priorities));
        std::vector<std::multiset<BoardPosition>::iterator> removes;
        for (auto it = begin(priorities); it != end(priorities); ++it)
        {
            if (board._updates.count(it->pos()) > 0)
                removes.emplace_back(it);
        }
        for (auto it : removes)
            priorities.erase(it);

        for (auto update : board._updates)
            priorities.emplace(update, board);
        printChoices();
        board.clearUpdates();
//
    }
};

int main ()
{
    AiPlayer s(10, 5, 4, 4, 3, 3, 3, 2, 2, 2, 2, 1, 1, 1, 1, 1);
    for (auto i = 0; i < 19; ++i)
        s.chooseBestMove();
}