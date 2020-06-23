#ifndef UNTITLED1_FUNCTIONS_H
#define UNTITLED1_FUNCTIONS_H

#define DEBUG 1

template<class T, class ... Args>
T min
        (const T &single)
{
    return single;
}

template<class T, class ... Args>
T min
        (const T &first,
         const Args &... args)
{
    auto current_min = min(args...);
    if (first < current_min)
        return first;
    return current_min;
}

template<class T, class ... Args>
T max
        (const T &single)
{
    return single;
}

template<class T, class ... Args>
T max
        (const T &first,
         const Args &... args)
{
    auto current_max = max(args...);
    if (first > current_max)
        return first;
    return current_max;
}

#if DEBUG
template<class T>
std::ostream &
operator<<
        (std::ostream &os,
         std::vector<T> container)
{
    os << "[";
    auto it = container.begin();
    if (container.empty())
    {
        os << "]";
        return os;
    }

    for (; it != std::end(container) - 1; ++it)
        os << std::setfill(' ') << std::setw(2) << *it << ", ";
    os << std::setfill(' ') << std::setw(2) << *it << "]";
    return os;
}
#endif

#endif //UNTITLED1_FUNCTIONS_H
