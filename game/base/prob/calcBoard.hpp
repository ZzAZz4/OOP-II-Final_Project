#ifndef UNTITLED1_CALCBOARD_HPP
#define UNTITLED1_CALCBOARD_HPP

#include "../../../resources/geometry/Geometry.hpp"
#include "../../../resources/commons/utils.hpp"
#include <optional>
#include <map>
#include <unordered_set>

enum class UpdateMessage
{
    Hit, Miss, Sunk
};
enum class TargetMode
{
    Hunt, Target
};

template<class T = int>
class PBoard
{
public:
    template<class U>
    using Container = std::vector<U>;
    using Vector = Container<T>;
    using Matrix = Container<Container<T>>;
    using UInt = uint32_t;
    using CountMap = std::map<UInt, UInt, std::greater<>>;
    using BoardMap = std::map<UInt, Matrix>;
    using USet = std::unordered_set<Point<T>>;

private:
    UInt _size{};
    Matrix _evaluation;
    CountMap _boatSizes{};
    BoardMap _boatProbabilities{};

public:
    USet _updates{};
    TargetMode _searchMode;

    template<class ... Args>
    explicit
    PBoard (
        UInt size,
        Args ... args);

    [[nodiscard]]
    UInt
    size () const;

    bool
    inRange (
        Point<T> c);

    std::optional<T>
    at (
        const Point<T> & c,
        const bool & huntMode = true);

    UpdateMessage checkImpact (
        const Point<T> & coordinate);

    void propagate (
        const Point<T> & coordinate,
        UpdateMessage update,
        const UInt & removed = 0);

    void
    print ();

    void
    removeBoat (
        UInt boatSize);

    void
    clearUpdates ();

    void
    changeMode (TargetMode mode)
    {
        _searchMode = mode;
    }

private:
    void
    _overlap (
        Matrix & base,
        const Matrix & overlay);

    void
    _addBoat (
        const UInt & boatSize);

    Point<T>
    _findBoundary (
        const Point<T> & current,
        const Direction::type & direction,
        const UInt & boatSize,
        const UInt & depth = 1);

    Container<int>
    _makeWindow (
        const Point<T> & coordinate,
        const Point<T> & bound1,
        const Point<T> & bound2,
        const Direction::type & direction,
        const UInt & size);

    void
    _huntPropagate (
        const Point<T> & coordinate,
        const Direction::type & firstDirection,
        const Direction::type & endDirection,
        const UInt & size);

    void
    _recalculate ();
};

#include "calcBoard.cpp"

#endif //UNTITLED1_CALCBOARD_HPP
