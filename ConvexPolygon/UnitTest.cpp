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
	}
}