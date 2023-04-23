#ifndef GEMOTERY_HPP
#define GEOMETRY_HPP

#include <memory>

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

        struct HashFunction
        {
            size_t operator() (const Point& point) const;
        };
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
}



#endif