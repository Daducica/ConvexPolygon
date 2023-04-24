#include "UnitTest.hpp"

#include <cassert>
#include <memory>

#include "Geometry.hpp"

namespace Test
{
	const double eps = 0.00001;

	void RunTests ()
	{
		using namespace Geometry;

		{ // line with whole parameters
			const Point point1 {0,3};
			const Point point2 {1,5};
			const MathematicalLine line {point1, point2};
			assert (line.slope - 2 < eps);
			assert (line.yOffset - 3 < eps);
		}

		{ // line with fractional slope
			const Point point1 {0,0};
			const Point point2 {3,1};
			const MathematicalLine line {point1, point2};
			assert (line.slope - ((double)1/3) < eps);
			assert (line.yOffset - 0 < eps);
		}

		{ // line with fractional slope and yOffset
			const Point point1 {-1,0};
			const Point point2 {2,1};
			const MathematicalLine line {point1, point2};
			assert (line.slope - ((double)1/3) < eps);
			assert (line.yOffset - ((double)1 / 3) < eps);
		}

		{ // constant line
			const Point point1 {4,-1};
			const Point point2 {-3,-1};
			const MathematicalLine line {point1, point2};
			assert (line.slope - 0 < eps);
			assert (line.yOffset - (-1) < eps);
		}

		{ // line with negative slope
			const Point point1 {-2,-1};
			const Point point2 {4,-2};
			const MathematicalLine line {point1, point2};
			assert (line.slope - ((double)1/6) < eps);
			assert (line.yOffset - (-1 - (double)2/6) < eps);
		}

		{ // vertical line
			const Point point1 {-2,-1};
			const Point point2 {-2,-7};
			const std::unique_ptr<GeneralLine> line = GeneralLine::CreateLine (point1, point2);
			const VerticalLine* verticalLine = dynamic_cast<VerticalLine*>(line.get ());
			assert (verticalLine != nullptr);
			assert (verticalLine->x == -2);
		}

		{ // mathematical line
			const Point point1 {-2,-1};
			const Point point2 {4,-2};
			const std::unique_ptr<GeneralLine> line = GeneralLine::CreateLine (point1, point2);
			const MathematicalLine* mathematicalLine = dynamic_cast<MathematicalLine*>(line.get ());
			assert (mathematicalLine != nullptr);
			assert (mathematicalLine->slope - ((double)1 / 6) < eps);
			assert (mathematicalLine->yOffset - (-1 - (double)2 / 6) < eps);
		}

		{ // find leftmost point - one-point set
			const PointSet points = { {-2,-1} };
			const Point leftMostPoint = FindLeftMostPoint (points);
			assert (leftMostPoint == Point(-2,-1));
		}

		{ // find leftmost point - simple set
			const PointSet points = {{-2,-1}, {12,-2}, {0,5}};
			const Point leftMostPoint = FindLeftMostPoint (points);
			assert (leftMostPoint == Point (-2,-1));
		}

		{ // find leftmost point - multiple point with the same x coord
			const PointSet points = {{-2,3}, {-2,-1}, {-2,-2}, {0,5}};
			const Point leftMostPoint = FindLeftMostPoint (points);
			assert (leftMostPoint == Point (-2,-2));
		}

		{ // find leftmost point - multiple point with the same x coord (different order)
			const PointSet points = {{-2,-1}, {-2,-2}, {-2,3}, {0,5}};
			const Point leftMostPoint = FindLeftMostPoint (points);
			assert (leftMostPoint == Point (-2, -2));
		}

		{ // find point with smallest slope - easy case
			const Point startPoint {0, 0};
			const PointSet points = {{1,-1}, {2,-1}, {3,-2}, {0,5}, {3,-4}};
			const Point leftMostPoint = FindNextPointInBoundingPolygon (points, startPoint);
			assert (leftMostPoint == Point (3,-4));
		}

		{ // find point with smallest slope - easy case (different order)
			const Point startPoint{0, 0};
			const PointSet points = {{3,-4}, {1,-1}, {2,-1}, {3,-2}, {0,5}};
			const Point leftMostPoint = FindNextPointInBoundingPolygon (points, startPoint);
			assert (leftMostPoint == Point (3, -4));
		}

		{ // find point with smallest slope - multiple points with same slope
			const Point startPoint {0, 0};
			const PointSet points = {{1,-1}, {2,-1}, {2,-2}, {0,5}};
			const Point leftMostPoint = FindNextPointInBoundingPolygon (points, startPoint);
			assert (leftMostPoint == Point (2,-2));
		}

		{ // find point with smallest slope - startPoint is the leftmost point
			const Point startPoint {3,-1};
			const PointSet points = {{1,-1}, {2,-1}, {2,-2}, {0,5}};
			const Point leftMostPoint = FindNextPointInBoundingPolygon (points, startPoint);
			assert (leftMostPoint == Point (0,5));
		}

		{ // find next point for bounding polygon - next point would be through a vertical line
			const Point startPoint {3,-1};
			const PointSet points = {{1,-1}, {2,-1}, {3,2}, {0,5}};
			const Point leftMostPoint = FindNextPointInBoundingPolygon (points, startPoint);
			assert (leftMostPoint == Point (3,2));
		}

		{ // find next point for bounding polygon - next point would be through a vertical line, several points on the line
			const Point startPoint{3,-1};
			const PointSet points = {{1,-1}, {2,-1}, {3,2}, {0,5}, {3,-2}, {3,1}, {3,8}};
			const Point leftMostPoint = FindNextPointInBoundingPolygon (points, startPoint);
			assert (leftMostPoint == Point (3,8));
		}

		{ // find point with smallest slope - multiple points with same slope - left direction
			const Point startPoint {3,-1};
			const PointSet points = {{1,-1}, {2,-1}, {2,1}, {0,5}, {1,3}};
			const Point leftMostPoint = FindNextPointInBoundingPolygon (points, startPoint);
			assert (leftMostPoint == Point (0,5));
		}

		{ // find next point for bounding polygon - next point would be through a vertical line, left side
			const Point startPoint {0,5};
			const PointSet points = {{1,-1}, {2,-1}, {3,-2}, {0,0}, {3,-4}};
			const Point leftMostPoint = FindNextPointInBoundingPolygon (points, startPoint);
			assert (leftMostPoint == Point (0, 0));
		}
	}
}