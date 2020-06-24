#include "Geometry.hpp"

template<class T>
Point<T>::Point (
    const T & _x,
    const T & _y
)
    : x(_x), y(_y)
{}

template<class T>
Point<T>::Point (
    Direction::type dir)
{
    switch (dir)
    {
        case Direction::Up:
            x = 0, y = -1;
            break;
        case Direction::Right:
            x = 1, y = 0;
            break;
        case Direction::Down:
            x = 0, y = 1;
            break;
        case Direction::Left:
            x = -1, y = 0;
            break;
        case Direction::None:
            x = 0, y = 0;
            break;
    }
}

template<class T>
constexpr Point<T>
Point<T>::operator + (
    Point other) const
{
    other.x += x;
    other.y += y;
    return other;
}

template<class T>
constexpr Point<T>
Point<T>::operator + (
    const Direction::type & direction) const
{
    return *this + Point(direction);
}

template<class T>
constexpr Point<T>
Point<T>::operator - (
    Point other) const
{
    auto r = *this;
    r.x -= other.x;
    r.y -= other.y;
    return r;
}

template<class T>
constexpr Point<T>
Point<T>::operator * (
    const T & scalar) const
{
    auto r = *this;
    r.x *= scalar;
    r.y *= scalar;
    return r;
}

template<class T>
constexpr Point<T>
Point<T>::operator / (
    const T & scalar) const
{
    auto r = *this;
    r.x /= scalar;
    r.y /= scalar;
    return r;
}

template<class T>
constexpr bool
Point<T>::operator == (
    const Point & other) const
{
    return x == other.x &&
           y == other.y;
}

template<class T>
constexpr bool
Point<T>::operator != (
    const Point & other) const
{
    return x != other.x || y != other.y;
}

template<class T>
constexpr unsigned
Point<T>::originDistance () const
{
    return x + y;
}

template<class T>
constexpr bool
Point<T>::operator < (
    Point other) const
{
    return originDistance() < other.originDistance();
}

