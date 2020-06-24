
#include "utils.hpp"
#include <iomanip>

template<class T, class... Args>
T min (
    const T & single)
{
    return single;
}

template<class T, class... Args>
T min (
    const T & first,
    const Args & ... args)
{
    auto current_min = min(args...);
    if (first < current_min)
        return first;
    return current_min;
}

template<class T, class... Args>
T max (
    const T & single)
{
    return single;
}

template<class T, class... Args>
T max (
    const T & first,
    const Args & ... args)
{
    auto current_max = max(args...);
    if (first > current_max)
        return first;
    return current_max;
}

template<class T, class U>
std::vector<T> operator + (
    std::vector<T> first,
    const std::vector<U> & second)
{
    for (unsigned i = 0; i < first.size(); ++i)
        first[i] += second[i];
    return first;
}

template<class T>
std::ostream & operator << (
    std::ostream & os,
    std::vector<T> container)
{
#if DEBUG
    os << "[";
    auto it = container.begin();
    if (container.empty())
    {
        os << "]";
        return os;
    }

    for (; it != std::end(container) - 1; ++it)
        os << std::setfill(' ') << std::setw(3) << *it << ", ";
    os << std::setfill(' ') << std::setw(3) << *it << "]";
#endif
    return os;
}


