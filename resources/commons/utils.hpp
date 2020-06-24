#ifndef UNTITLED1_UTILS_HPP
#define UNTITLED1_UTILS_HPP

#define DEBUG 1
#include <vector>
#include <iostream>

template<class T, class ... Args>
T min (
    const T & single);

template<class T, class ... Args>
T min (
    const T & first,
    const Args & ... args);

template<class T, class ... Args>
T max
    (const T & single);

template<class T, class ... Args>
T max (
    const T & first,
    const Args & ... args);

template<class T, class U>
std::vector<T>
operator + (
    std::vector<T> first,
    const std::vector<U> & second);

template<class T>
std::ostream &
operator << (
    std::ostream & os,
    std::vector<T> container);

#include "utils.cpp"

#endif //UNTITLED1_UTILS_HPP
