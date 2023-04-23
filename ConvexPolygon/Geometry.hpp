#ifndef GEMOTERY_HPP
#define GEOMETRY_HPP

namespace Geometry
{
    struct Point
    {
        int x;
        int y;

        Point () = default;
        Point (int x, int y) : x (x), y (y) {}

        bool operator== (const Point& otherPoint) const;

        struct HashFunction
        {
            size_t operator() (const Point& point) const;
        };
    };
}



#endif