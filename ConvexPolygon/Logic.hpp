#ifndef LOGIC_HPP
#define LOGIC_HPP

#include <unordered_set>

#include "Geometry.hpp"

namespace Logic
{
	Geometry::Polygon CalculateBoundingPolygon (const Geometry::PointSet& points);
}


#endif