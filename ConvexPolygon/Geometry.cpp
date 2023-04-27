#include "Geometry.hpp"

#include <cassert>
#include <functional>
#include <optional>

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
        const bool isStartPointOnLeftEdge = nextPointCache.minXCoord == nextPointCache.startPoint.x;

        if (isStartPointOnRightEdge && isStartPointOnLeftEdge)
            return true;

        if (isStartPointOnRightEdge) {
            Point highestYPoint = FindPointWithHighestYCoord (nextPointCache.verticalLinePointSet, nextPointCache.startPoint);
            return highestYPoint.y > nextPointCache.startPoint.y;
        }       
        if (isStartPointOnLeftEdge) {
            Point lowestYPoint = FindPointWithLowestYCoord (nextPointCache.verticalLinePointSet, nextPointCache.startPoint);
            return lowestYPoint.y < nextPointCache.startPoint.y;
        }
        return false;
    }


    static Point HandleVerticalLinesOnEdges (const NextPointAnalysisCache& nextPointCache)
    {
        const bool leftSide = nextPointCache.minXCoord == nextPointCache.startPoint.x;
        if (leftSide) { // search upwards
            return FindPointWithLowestYCoord (nextPointCache.verticalLinePointSet, nextPointCache.startPoint);
        } else { // left side, search downwards
            return FindPointWithHighestYCoord (nextPointCache.verticalLinePointSet, nextPointCache.startPoint);
        }
    }


    // if multiple points result in the same slope, choose the farthest point
    static Point FindPointWithSmallestSlope (const NextPointAnalysisCache& nextPointCache, SearchDirection searchDirection)
    {
        assert (nextPointCache.slopeValuesForMathematicalPoints.size () > 0);

        std::optional<Point> pointWithSmallestSlope;
        std::optional<double> smallestSlope;
        for (const auto& [point, slope] : nextPointCache.slopeValuesForMathematicalPoints) {
            if (searchDirection == SearchDirection::Right && point.x < nextPointCache.startPoint.x)
                continue;
            if (searchDirection == SearchDirection::Left && point.x > nextPointCache.startPoint.x)
                continue;

            if (!smallestSlope.has_value ()) {
                smallestSlope = slope;
                pointWithSmallestSlope = point;
                continue;
            }

            if (slope < smallestSlope.value ()) {
                smallestSlope = slope;
                pointWithSmallestSlope = point;
            } else if (slope == smallestSlope.value ()) {
                if (searchDirection == SearchDirection::Right) {
                    if (point.x > pointWithSmallestSlope.value ().x)
                        pointWithSmallestSlope = point;
                } else {
                    if (point.x < pointWithSmallestSlope.value ().x)
                        pointWithSmallestSlope = point;
                }
            }
        }
        assert (pointWithSmallestSlope.has_value ());
        return pointWithSmallestSlope.value ();
    }


    // assumes that the startPoint is correct
    Point FindNextPointInBoundingPolygon (const PointSet& points, const Point& startPoint, SearchDirection searchDirection)
    {
        assert (points.size () > 0);
        assert (points.find (startPoint) == points.end ());

        NextPointAnalysisCache nextPointCache = DoPreprocessingForNextPointSearch (points, startPoint);

        if (IsLineVerticalToNextPoint (nextPointCache))
            return HandleVerticalLinesOnEdges (nextPointCache);

        return FindPointWithSmallestSlope (nextPointCache, searchDirection);
    }


    bool AreAllPointsInOneLine (const PointSet& points)
    {
        if (points.size () < 3)
            return true;

        const Point point = *points.begin ();
        const int y = point.y;
        const int x = point.x;

        bool sameYCoords = true;
        bool sameXCoords = true;

        for (const Point& point : points) {
            if (sameYCoords && point.y != y)
                sameYCoords = false;
            if (sameXCoords && point.x != x)
                sameXCoords = false;
        }

        return sameYCoords || sameXCoords;
    }


    static int FindMaxXCoord (const PointSet& points)
    {
        assert (points.size () > 0);

        int maxXCoord = (*points.begin ()).x;
        for (const Point& point : points) {
            if (point.x > maxXCoord) {
                maxXCoord = point.x;
            }
        }
        return maxXCoord;
    }


    std::vector<Point> CalculateBoundingPolygon (const PointSet& points)
    {
        assert (points.size () > 2);
        assert (!Geometry::AreAllPointsInOneLine (points));

        const int maxXCoord = FindMaxXCoord (points);
        SearchDirection searchDirection = SearchDirection::Right;
        std::vector<Point> boundingPoints;
        PointSet unusedPoints = points;

        Point leftMostPoint = FindLeftMostPoint (points);
        unusedPoints.erase (leftMostPoint);
        Point nextPoint = FindNextPointInBoundingPolygon (unusedPoints, leftMostPoint, searchDirection);
        if (nextPoint.x == maxXCoord)
            searchDirection = SearchDirection::Left;
        unusedPoints.insert (leftMostPoint);

        boundingPoints.push_back (leftMostPoint);
        boundingPoints.push_back (nextPoint);


        while (nextPoint != leftMostPoint)
        {
            unusedPoints.erase (nextPoint);
            nextPoint = FindNextPointInBoundingPolygon (unusedPoints, nextPoint, searchDirection);
            if (nextPoint.x == maxXCoord)
                searchDirection = SearchDirection::Left;
            boundingPoints.push_back (nextPoint);
        }

        return boundingPoints;
    }
}
