#include <tuple>

template<class T>
constexpr
Segment<T>::Segment (
    Segment::Point_t begin,
    Segment::Point_t end)
    : _begin(begin), _end(end)
{}

template<class T>
constexpr
const typename Segment<T>::Point_t &
Segment<T>::begin () const
{
    return _begin;
}

template<class T>
constexpr
typename Segment<T>::Point_t
Segment<T>::end () const
{
    return _end;
}

template<class T>
constexpr bool
Segment<T>::intersects (
    const Segment & other) const
{
    auto P_0s = begin(), P_1s = other.begin();
    auto P_0e = end(), P_1e = other.end();

    auto v_0 = P_0e - P_0s;
    auto v_1 = P_1e - P_1s;

    auto D = v_0.x * v_1.y - v_0.y * v_1.x;

    if (D == 0)
    {
        if (P_0s == P_1s || P_0s == P_1e || P_0e == P_1s || P_0e == P_1e)
            return true;

        auto dAse = P_1e - P_0s;
        auto dAss = P_1s - P_0s;
        auto dAes = P_1s - P_0e;
        auto dAee = P_1e - P_0e;

        if (dAse.x * dAss.y != dAse.y * dAss.y)
            return false;

        return dAss.y * dAse.y + dAss.x * dAse.x < 0 ||
               dAes.y * dAee.y + dAes.x * dAee.x < 0 ||
               dAss.y * dAes.y + dAss.x * dAes.x < 0 ||
               dAee.y * dAse.y + dAee.x * dAse.x < 0;
    }

    auto DPs = P_1s - P_0s;
    Point_t AVec = {v_1.y * DPs.x - v_1.x * DPs.y,
                    v_0.y * DPs.x - v_0.x * DPs.y};

    auto pt = AVec.x, ps = AVec.y;
    auto[t, s, m] = D < 0 ? std::tuple(-pt, -ps, -D) : std::tuple(pt, ps, D);

    return 0 <= t && t <= m && 0 <= s && s <= m;
}
