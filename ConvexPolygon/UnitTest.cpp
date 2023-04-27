#include "UnitTest.hpp"

#include <cassert>
#include <chrono>
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
			const Point leftMostPoint = FindNextPointInBoundingPolygon (points, startPoint, SearchDirection::Right);
			assert (leftMostPoint == Point (3,-4));
		}

		{ // find point with smallest slope - easy case (different order)
			const Point startPoint{0, 0};
			const PointSet points = {{3,-4}, {1,-1}, {2,-1}, {3,-2}, {0,5}};
			const Point leftMostPoint = FindNextPointInBoundingPolygon (points, startPoint, SearchDirection::Right);
			assert (leftMostPoint == Point (3, -4));
		}

		{ // find point with smallest slope - multiple points with same slope
			const Point startPoint {0, 0};
			const PointSet points = {{1,-1}, {2,-1}, {2,-2}, {0,5}};
			const Point leftMostPoint = FindNextPointInBoundingPolygon (points, startPoint, SearchDirection::Right);
			assert (leftMostPoint == Point (2,-2));
		}

		{ // find point with smallest slope - startPoint is the leftmost point
			const Point startPoint {3,-1};
			const PointSet points = {{1,-1}, {2,-1}, {2,-2}, {0,5}};
			const Point leftMostPoint = FindNextPointInBoundingPolygon (points, startPoint, SearchDirection::Left);
			assert (leftMostPoint == Point (0,5));
		}

		{ // find next point for bounding polygon - next point would be through a vertical line
			const Point startPoint {3,-1};
			const PointSet points = {{1,-1}, {2,-1}, {3,2}, {0,5}};
			const Point leftMostPoint = FindNextPointInBoundingPolygon (points, startPoint, SearchDirection::Right);
			assert (leftMostPoint == Point (3,2));
		}

		{ // find next point for bounding polygon - next point would be through a vertical line, several points on the line
			const Point startPoint{3,-1};
			const PointSet points = {{1,-1}, {2,-1}, {3,2}, {0,5}, {3,-2}, {3,1}, {3,8}};
			const Point leftMostPoint = FindNextPointInBoundingPolygon (points, startPoint, SearchDirection::Right);
			assert (leftMostPoint == Point (3,8));
		}

		{ // find point with smallest slope - multiple points with same slope - left direction
			const Point startPoint {3,-1};
			const PointSet points = {{1,-1}, {2,-1}, {2,1}, {0,5}, {1,3}};
			const Point leftMostPoint = FindNextPointInBoundingPolygon (points, startPoint, SearchDirection::Left);
			assert (leftMostPoint == Point (0,5));
		}

		{ // find next point for bounding polygon - next point would be through a vertical line, left side
			const Point startPoint {0,5};
			const PointSet points = {{1,-1}, {2,-1}, {3,-2}, {0,0}, {3,-4}};
			const Point leftMostPoint = FindNextPointInBoundingPolygon (points, startPoint, SearchDirection::Right);
			assert (leftMostPoint == Point (0, 0));
		}

		{ // two points
			const PointSet points = {{0,5}, {0,1}};
			assert (AreAllPointsInOneLine (points) == true);
		}

		{ // same x coords
			const PointSet points = {{0,5}, {0,1}, {0,-2}, {0,0}};
			assert (AreAllPointsInOneLine (points) == true);
		}

		{ // one point with different x coord
			const PointSet points = {{0,5}, {0,1}, {1,-1}, {0,-2}, {0,0}};
			assert (AreAllPointsInOneLine (points) == false);
		}

		{ // same y coors
			const PointSet points = {{5,0}, {1,0}, {-2,0}, {0,0}};
			assert (AreAllPointsInOneLine (points) == true);
		}

		{ // one point with different y coord
			const PointSet points = {{5,0}, {1,0}, {-2,-2}, {0,0}};
			assert (AreAllPointsInOneLine (points) == false);
		}

		{ // calculate polygon - simple triangle
			const PointSet points = {{0,0}, {2,0}, {1,2}};
			std::vector<Point> boundingPoints = CalculateBoundingPolygon (points);
			assert (boundingPoints.size () == points.size () + 1);
			assert (boundingPoints[0] == Point (0,0));
			assert (boundingPoints[1] == Point (2,0));
			assert (boundingPoints[2] == Point (1,2));
			assert (boundingPoints[3] == Point (0,0));
		}

		{ // calculate polygon - all points needed, contains vertical lines
			const PointSet points = {{0,0}, {0,2}, {2,0}, {1,2}, {2,1}};
			std::vector<Point> boundingPoints = CalculateBoundingPolygon (points);
			assert (boundingPoints.size () == points.size () + 1);
			assert (boundingPoints[0] == Point (0,0));
			assert (boundingPoints[1] == Point (2,0));
			assert (boundingPoints[2] == Point (2,1));
			assert (boundingPoints[3] == Point (1,2));
			assert (boundingPoints[4] == Point (0,2));
			assert (boundingPoints[5] == Point (0,0));
		}

		{ // calculate polygon - not every point is needed
			const PointSet points = {{1,2}, {1,1}, {1,0}, {2,0}, {0,0}};
			std::vector<Point> boundingPoints = CalculateBoundingPolygon (points);
			assert (boundingPoints.size () == 4);
			assert (boundingPoints[0] == Point (0,0));
			assert (boundingPoints[1] == Point (2,0));
			assert (boundingPoints[2] == Point (1,2));
			assert (boundingPoints[3] == Point (0,0));
		}


		{ // calculate polygon - not every point is needed
			const PointSet points = {{0,-1}, {1,1}, {-1,1}};
			std::vector<Point> boundingPoints = CalculateBoundingPolygon (points);
			assert (boundingPoints.size () == 4);
			assert (boundingPoints[0] == Point (-1,1));
			assert (boundingPoints[1] == Point (0,-1));
			assert (boundingPoints[2] == Point (1,1));
			assert (boundingPoints[3] == Point (-1,1));
		}

		{ // calculate polygon - not every point is needed
			const PointSet points = {{1,1}, {4,0}, {2,3}, {5,2}};
			std::vector<Point> boundingPoints = CalculateBoundingPolygon (points);
			assert (boundingPoints.size () == 5);
			assert (boundingPoints[0] == Point (1,1));
			assert (boundingPoints[1] == Point (4,0));
			assert (boundingPoints[2] == Point (5,2));
			assert (boundingPoints[3] == Point (2,3));
			assert (boundingPoints[4] == Point (1,1));
		}

		{ // large amount of points
			PointSet points;
			const int lowerBound = 0;
			const int upperBound = 500;
			for (int x = lowerBound; x <= upperBound; x++) {
				for (int y = lowerBound; y <= upperBound; y++) {
					points.insert (Point (x,y));
				}
			}
			auto start = std::chrono::high_resolution_clock::now ();
			std::vector<Point> boundingPoints = CalculateBoundingPolygon (points);
			auto stop = std::chrono::high_resolution_clock::now ();
			auto duration = std::chrono::duration_cast<std::chrono::seconds>(stop - start);
			assert (duration.count () < 10);

			assert (boundingPoints.size () == 5);
			assert (boundingPoints[0] == Point (lowerBound, lowerBound));
			assert (boundingPoints[1] == Point (upperBound, lowerBound));
			assert (boundingPoints[2] == Point (upperBound, upperBound));
			assert (boundingPoints[3] == Point (lowerBound, upperBound));
			assert (boundingPoints[4] == Point (lowerBound, lowerBound));
		}
	}
}