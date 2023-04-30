#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include <memory>
#include <unordered_set>
#include <vector>

namespace Geometry
{
    struct Point
    {
        int x;
        int y;

        Point () = default;
        Point (int x, int y) : x (x), y (y) {}

        bool operator== (const Point& otherPoint) const;
        bool operator!= (const Point& otherPoint) const;
    };


    template <typename PointTpye>
    struct PointHashFunction
    {
        size_t operator() (const PointTpye& point) const
        {
            const size_t xHash = std::hash<int> ()(point.x);
            const size_t yHash = std::hash<int> ()(point.y) << 1;
            return xHash ^ yHash;
        }
    };


    typedef PointHashFunction<Point> GeometryPointHashFunction;
    typedef std::unordered_set<Point, GeometryPointHashFunction> PointSet;
    typedef std::vector<Point> Polygon;

    enum class SearchDirection
    {
        Right,
        Left
    };

    struct GeneralLine
    {
        static std::unique_ptr<GeneralLine> CreateLine (const Point& point1, const Point& point2);
        virtual ~GeneralLine ();
    };


    struct MathematicalLine : GeneralLine
    {
        double yOffset;
        double slope;

        MathematicalLine (double yOffset, double slope) : yOffset (yOffset), slope (slope) {}
        MathematicalLine (const Point& point1, const Point& point2);
    };


    struct VerticalLine : GeneralLine
    {
        int x;

        VerticalLine (int x) : x (x) {}
    };


    Point FindLeftMostPoint (const PointSet& points);
    Point FindNextPointInBoundingPolygon (const PointSet& points, const Point& startPoint, SearchDirection searchDirection);
    bool AreAllPointsInOneLine (const PointSet& points);
    std::vector<Point> CalculateBoundingPolygon (const PointSet& points);
    bool CheckIfPolygonContainsAllPoints (const std::vector<Point>& polygon, const PointSet& points);
}



#endif