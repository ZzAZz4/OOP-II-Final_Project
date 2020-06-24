#include "../Selector.hpp"

template<class... Args>
Selector::Selector (uint32_t size, Args... boat_sizes)
    : board(size, boat_sizes...)
{
    for (unsigned x = 0; x < board.size(); ++x)
        for (unsigned y = 0; y < board.size(); ++y)
            priorities.emplace(x, y, board);
}

void Selector::printChoices ()
{
    for (auto value : priorities)
        std::cout << value.pos().x << ' '
                  << value.pos().y << ' '
                  << *board.at(value.pos()) << '\n';
}

void Selector::printTop ()
{
    std::cout << priorities.begin()->pos().x << ' '
              << priorities.begin()->pos().y << ' '
              << *board.at(priorities.begin()->pos()) << '\n';
}

void Selector::printBoard ()
{
    board.print();
}

Point<> Selector::getMove ()
{
    return priorities.begin()->pos();
}

void Selector::makeUpdate (
    Point<> choice,
    UpdateMessage update,
    unsigned boatSize)
{
    board.propagate(choice, update);

    printBoard();
    updatePriority();
    printTop();
}


void Selector::updatePriority ()
{
    std::vector<MSet::iterator> removes;
    for (auto it = begin(priorities); it != end(priorities); ++it)
    {
        if (board._updates.count(it->pos()) > 0)
            removes.emplace_back(it);
    }
    for (auto it : removes)
        priorities.erase(it);

    for (auto update : board._updates)
        priorities.emplace(update, board);
    board.clearUpdates();
}


bool Selector::BoardPosition::operator < (
    Selector::BoardPosition other) const
{
    auto f = *board->at(coordinate);
    auto s = *board->at(other.coordinate);
    if (f == s)
        return coordinate.originDistance() < other.coordinate.originDistance();

    return f >= s;
}

bool Selector::BoardPosition::operator == (
    Selector::BoardPosition other)
{
    return coordinate == other.coordinate;
}

Selector::BoardPosition::BoardPosition (
    const int & x,
    const int & y,
    PBoard<> & pBoard)
    : coordinate(x, y), board(&pBoard)
{}

Selector::BoardPosition::BoardPosition (
    const Point<> & position,
    PBoard<> & pBoard)
    : coordinate(position.x, position.y), board(&pBoard)
{}

[[nodiscard]]
Point<> Selector::BoardPosition::pos () const
{
    return coordinate;
}
