#ifndef UNTITLED1_GEOMETRY_HPP
#define UNTITLED1_GEOMETRY_HPP

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
struct Point
{
    T x, y;

    Point (
        const T & _x,
        const T & _y);

    explicit Point (
        Direction::type dir);

    constexpr
    Point<T> operator +
        (Point other) const;

    constexpr
    Point<T> operator +
        (const Direction::type & direction) const;

    constexpr
    Point<T> operator -
        (Point other) const;

    constexpr
    Point<T> operator *
        (const T & scalar) const;

    constexpr
    Point<T> operator /
        (const T & scalar) const;

    constexpr
    bool operator ==
        (const Point & other) const;

    constexpr
    bool operator !=
        (const Point & other) const;

    [[nodiscard]]
    constexpr
    unsigned originDistance () const;

    constexpr
    bool operator < (Point other) const;
};

template<class T>
struct Segment
{
    using Point_t = Point<T>;
    Point_t _begin;
    Point_t _end;

public:
    constexpr
    Segment
        (Point_t begin, Point_t end);

    constexpr
    const Point_t &
    begin () const;

    constexpr
    Point_t
    end () const;

    constexpr
    bool intersects
        (const Segment & other) const;

};

#include "Point.cpp"
#include "Segment.cpp"
#include "../commons/hashing.hpp"

#endif //UNTITLED1_GEOMETRY_HPP
