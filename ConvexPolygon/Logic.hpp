#ifndef LOGIC_HPP
#define LOGIC_HPP

#include <unordered_set>
#include <vector>

#include "Geometry.hpp"

namespace Logic
{
	std::vector<Geometry::Point> CalculateBoundingPolygon (const Geometry::PointSet& points);
}


#endif