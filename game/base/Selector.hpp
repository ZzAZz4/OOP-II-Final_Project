#ifndef UNTITLED1_SELECTOR_HPP
#define UNTITLED1_SELECTOR_HPP


#include "../../resources/geometry/Geometry.hpp"
#include "prob/calcBoard.hpp"
#include <set>

struct Selector
{
    enum class State
    {
        Hunt, Target
    };
    struct BoardPosition;

    using MSet = std::multiset<BoardPosition>;
    using Board = PBoard<>;

    Board board;
    MSet priorities;

public:
    template<class ... Args>
    explicit Selector (uint32_t size, Args ... boat_sizes);

    void printChoices ();

    void printTop ();

    void printBoard ();

    Point<> getMove ();

    void makeUpdate (
        Point<> choice,
        UpdateMessage update,
        unsigned boatSize = 0);

    void updatePriority ();

};

struct Selector::BoardPosition
{
    Point<> coordinate;
    PBoard<> * board;

    bool operator < (BoardPosition other) const;

    bool operator == (BoardPosition other);

    BoardPosition (
        const int & x,
        const int & y,
        PBoard<> & pBoard);

    BoardPosition (
        const Point<> & position,
        PBoard<> & pBoard);

    Point<> pos () const;
};

#include "prob/Selector.cpp"

#endif //UNTITLED1_SELECTOR_HPP
