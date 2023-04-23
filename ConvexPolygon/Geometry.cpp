#include "Geometry.hpp"

#include <functional>

namespace Geometry
{
    bool Point::operator== (const Point& otherPoint) const
    {
        return this->x == otherPoint.x && this->y == otherPoint.y;
    }

    size_t Point::HashFunction::operator() (const Point& point) const
    {
        const size_t xHash = std::hash<int> ()(point.x);
        const size_t yHash = std::hash<int> ()(point.y) << 1;
        return xHash ^ yHash;
    }
}
