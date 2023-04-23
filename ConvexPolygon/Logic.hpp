#ifndef LOGIC_HPP
#define LOGIC_HPP

#include <unordered_set>

#include "Geometry.hpp"

namespace Logic
{
	void CalculateBoundingPolygon (const std::unordered_set<Geometry::Point>& points);
}


#endif