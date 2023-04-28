#ifndef LOGIC_HPP
#define LOGIC_HPP

#include <unordered_set>

#include "Geometry.hpp"
#include "Model.hpp"

namespace Logic
{
	Geometry::PointSet ConvertUIPointsToLogicalPoints (const Model::UIPointSet& uiPoints);
	Model::UIPolygon ConvertLogicalPointsToUIPoints (Geometry::Polygon& logicalPoints);

	Model::UIPolygon CalculateBoundingPolygon (const Model::UIPointSet& points);
}


#endif