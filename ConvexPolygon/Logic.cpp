#include "Logic.hpp"

#include <cassert>

namespace Logic
{
	std::vector<Geometry::Point> CalculateBoundingPolygon (const Geometry::PointSet& points)
	{
		if (points.size () > 2 || Geometry::AreAllPointsInOneLine (points))
			return std::vector<Geometry::Point> ();

		return Geometry::CalculateBoundingPolygon (points);
	}
}