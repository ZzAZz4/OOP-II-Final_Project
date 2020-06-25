//
// Created by esteb on 6/24/2020.
//

#ifndef UNTITLED1_HASHING_HPP
#define UNTITLED1_HASHING_HPP

#include "../geometry/Geometry.hpp"
#include <functional>

namespace std
{
    template<typename T>
    struct hash<Point<T>>
    {
        inline
        std::size_t
        operator () (
            const Point<T> & p) const
        {
            return p.y << 16 ^ p.x;
        }
    };
}


#endif //UNTITLED1_HASHING_HPP
