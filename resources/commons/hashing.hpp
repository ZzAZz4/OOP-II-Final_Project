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


//template<class T>
//constexpr inline
//void
//hash_combine(std::size_t &seed, const T &v)
//{
//    std::hash<T> THash{};
//    seed ^= THash(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
//}

//
//template<typename C>
//struct hash_container
//{
//    typedef typename C::value_type value_type;
//
//    inline size_t operator()(const C &c) const
//    {
//        size_t seed = 0;
//        for (typename C::const_iterator it = c.begin(), end = c.end(); it != end; ++it)
//            hash_combine<value_type>(seed, *it);
//
//        return seed;
//    }
//};

#endif //UNTITLED1_HASHING_HPP
