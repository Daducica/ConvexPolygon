#include "Geometry.hpp"

#include <cassert>
#include <functional>

namespace Geometry
{
    bool Point::operator== (const Point& otherPoint) const
    {
        return this->x == otherPoint.x && this->y == otherPoint.y;
    }
    

    bool Point::operator!= (const Point& otherPoint) const
    {
        return !operator== (otherPoint);
    }


    size_t Point::HashFunction::operator() (const Point& point) const
    {
        const size_t xHash = std::hash<int> ()(point.x);
        const size_t yHash = std::hash<int> ()(point.y) << 1;
        return xHash ^ yHash;
    }


    GeneralLine::~GeneralLine () = default;


    std::unique_ptr<GeneralLine> GeneralLine::CreateLine (const Point& point1, const Point& point2)
    {
        assert (point1 != point2);
        if (point2.x == point1.x)
            return std::make_unique<VerticalLine> (point1.x);
        else
            return std::make_unique<MathematicalLine> (point1, point2);
    }


    MathematicalLine::MathematicalLine (const Point& point1, const Point& point2)
    {
        assert (point1 != point2);
        assert (point2.x != point1.x);

        slope = (double)(point2.y - point1.y) / (point2.x - point1.x);
        yOffset = point1.y - slope * point1.x;
    }


    // if multiple points have the same x coord, return the bottom one
    Point FindLeftMostPoint (const PointSet& points)
    {
        assert (points.size () > 0);

        Point leftMostPoint = *points.begin ();
        for (const Point& point : points) {
            if (leftMostPoint.x > point.x) {
                leftMostPoint = point;
            } else if (leftMostPoint.x == point.x) {
                if (leftMostPoint.y > point.y)
                    leftMostPoint = point;
            }
        }
        return leftMostPoint;
    }
}
