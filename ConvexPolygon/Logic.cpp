#include "Logic.hpp"

#include <cassert>

namespace Logic
{
	Geometry::PointSet ConvertUIPointsToLogicalPoints (const Model::UIPointSet& uiPoints)
	{
		Geometry::PointSet logicalPoints;
		for (const wxPoint& point : uiPoints) {
			Geometry::Point reversedPoint {point.x, -point.y};
			logicalPoints.insert (reversedPoint);
		}
		return logicalPoints;
	}


	Model::UIPolygon ConvertLogicalPointsToUIPoints (Geometry::Polygon& logicalPoints)
	{
		Model::UIPolygon uiPoints;
		for (Geometry::Point& point : logicalPoints) {
			wxPoint reversedPoint {point.x, -point.y};
			uiPoints.push_back (reversedPoint);
		}
		return uiPoints;
	}


	Model::UIPolygon CalculateBoundingPolygon (const Model::UIPointSet& points)
	{
		Geometry::PointSet logicalPoints = ConvertUIPointsToLogicalPoints (points);
		if (points.size () < 3 || Geometry::AreAllPointsInOneLine (logicalPoints))
			return Model::UIPolygon ();

		Geometry::Polygon polygonPoints = Geometry::CalculateBoundingPolygon (logicalPoints);

		assert (polygonPoints.size () > 2);
		assert (Geometry::CheckIfPolygonContainsAllPoints (polygonPoints, logicalPoints));

		polygonPoints.push_back (polygonPoints[0]);
		return ConvertLogicalPointsToUIPoints (polygonPoints);
	}
}