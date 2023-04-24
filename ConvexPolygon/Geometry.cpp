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

        slope = ((double)point2.y - point1.y) / (point2.x - point1.x);
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


    struct NextPointAnalysisCache
    {
        const Point& startPoint;
        std::unordered_map<Point, double, Point::HashFunction> slopeValuesForMathematicalPoints;
        PointSet verticalLinePointSet;
        int maxXCoord;
        int minXCoord;
    };


    static NextPointAnalysisCache DoPreprocessingForNextPointSearch (const PointSet& points, const Point& startPoint)
    {
        NextPointAnalysisCache nextPointChache {startPoint};
        nextPointChache.maxXCoord = startPoint.x;
        nextPointChache.minXCoord = startPoint.x;

        for (const Point& point : points) {
            if (point.x > nextPointChache.maxXCoord)
                nextPointChache.maxXCoord = point.x;
            if (point.x < nextPointChache.minXCoord)
                nextPointChache.minXCoord = point.x;

            const std::unique_ptr<GeneralLine> line = GeneralLine::CreateLine (startPoint, point);
            const MathematicalLine* mathematicalLine = dynamic_cast<MathematicalLine*>(line.get ());
            if (mathematicalLine != nullptr) {
                nextPointChache.slopeValuesForMathematicalPoints[point] = mathematicalLine->slope;
            } else {
                nextPointChache.verticalLinePointSet.insert (point);
            }
        }
        return nextPointChache;
    }


    static Point FindPointWithLowestYCoord (const PointSet& points, const Point& startPoint)
    {
        int minYCoord = startPoint.y;
        Point pointWithLowestYCoord = startPoint;
        for (const Point& point : points) {
            if (point.y < minYCoord) {
                minYCoord = point.y;
                pointWithLowestYCoord = point;
            }
        }
        return pointWithLowestYCoord;
    }


    static Point FindPointWithHighestYCoord (const PointSet& points, const Point& startPoint)
    {
        int maxYCoord = startPoint.y;
        Point pointWithHighestYCoord = startPoint;
        for (const Point& point : points) {
            if (point.y > maxYCoord) {
                maxYCoord = point.y;
                pointWithHighestYCoord = point;
            }
        }
        return pointWithHighestYCoord;
    }


    static bool IsLineVerticalToNextPoint (const NextPointAnalysisCache& nextPointCache)
    {
        const bool isStartPointOnRightEdge = nextPointCache.maxXCoord == nextPointCache.startPoint.x;
        if (isStartPointOnRightEdge) {
            Point highestYPoint = FindPointWithHighestYCoord (nextPointCache.verticalLinePointSet, nextPointCache.startPoint);
            return highestYPoint.y > nextPointCache.startPoint.y;
        }       
        const bool isStartPointOnLeftEdge = nextPointCache.minXCoord == nextPointCache.startPoint.x;
        if (isStartPointOnLeftEdge) {
            Point lowestYPoint = FindPointWithLowestYCoord (nextPointCache.verticalLinePointSet, nextPointCache.startPoint);
            return lowestYPoint.y < nextPointCache.startPoint.y;
        }
        return false;
    }


    static Point HandleVerticalLinesOnEdges (const NextPointAnalysisCache& nextPointCache)
    {
        const bool rightSide = nextPointCache.maxXCoord == nextPointCache.startPoint.x;
        if (rightSide) { // search upwards
            return FindPointWithHighestYCoord (nextPointCache.verticalLinePointSet, nextPointCache.startPoint);
        } else { // left side, search downwards
            return FindPointWithLowestYCoord (nextPointCache.verticalLinePointSet, nextPointCache.startPoint);
        }
    }


    // if multiple points result in the same slope, choose the farthest point
    static Point FindPointWithSmallestSlope (const NextPointAnalysisCache& nextPointCache)
    {
        assert (nextPointCache.slopeValuesForMathematicalPoints.size () > 0);

        Point pointWithSmallestSlope = nextPointCache.slopeValuesForMathematicalPoints.begin ()->first;
        double smallestSlope = nextPointCache.slopeValuesForMathematicalPoints.begin ()->second;
        for (const auto& [point, slope] : nextPointCache.slopeValuesForMathematicalPoints) {
            if (slope < smallestSlope) {
                smallestSlope = slope;
                pointWithSmallestSlope = point;
            } else if (slope == smallestSlope) {
                if (nextPointCache.maxXCoord > nextPointCache.startPoint.x) { // search in the right direction
                    if (point.y < pointWithSmallestSlope.y)
                        pointWithSmallestSlope = point;
                } else { // search in the left direction
                    if (point.y > pointWithSmallestSlope.y)
                        pointWithSmallestSlope = point;
                }
            }
        }
        return pointWithSmallestSlope;
    }


    // assumes that the startPoint is correct
    Point FindNextPointInBoundingPolygon (const PointSet& points, const Point& startPoint)
    {
        assert (points.size () > 0);
        assert (points.find (startPoint) == points.end ());

        NextPointAnalysisCache nextPointCache = DoPreprocessingForNextPointSearch (points, startPoint);

        if (IsLineVerticalToNextPoint (nextPointCache))
            return HandleVerticalLinesOnEdges (nextPointCache);

        return FindPointWithSmallestSlope (nextPointCache);
    }
}
