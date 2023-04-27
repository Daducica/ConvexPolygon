#include "Logic.hpp"

#include <cassert>

namespace Logic
{
	static Geometry::PointSet ConvertUICoordsToLogicalCoords (const Geometry::PointSet& uiPoints)
	{
		Geometry::PointSet logicalCoords;
		for (const Geometry::Point& point : uiPoints) {
			Geometry::Point reversedPoint = point;
			reversedPoint.y = -reversedPoint.y;
			logicalCoords.insert (reversedPoint);
		}
		return logicalCoords;
	}


	static void TransformToUICoords (Geometry::Polygon& logicalCoords)
	{
		for (Geometry::Point& point : logicalCoords) {
			point.y = -point.y;
		}
	}


	Geometry::Polygon CalculateBoundingPolygon (const Geometry::PointSet& points)
	{
		if (points.size () < 3 || Geometry::AreAllPointsInOneLine (points))
			return std::vector<Geometry::Point> ();

		Geometry::PointSet logicalCoords = ConvertUICoordsToLogicalCoords (points);
		Geometry::Polygon polygonPoints = Geometry::CalculateBoundingPolygon (logicalCoords);
		TransformToUICoords (polygonPoints);

		return polygonPoints;
	}
}