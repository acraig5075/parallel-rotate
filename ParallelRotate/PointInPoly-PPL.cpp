#include "PPL-solution.h"
#include "Structures.h"
#include "Serial-solution.h"
#include "Verify.h"
#include <algorithm>
#include <ppl.h>
#include <concurrent_vector.h>


void PointInPolyPPL(const std::vector<CadPt2> &points, const CadPolygon &polygon, float width, float extent)
{
	if (settings.PointInPolyWhichLoop == Settings::ParalleliseInnerLoop)
	{
		// parallelise inner loop, point-in-point test
		for (auto pt : points)
		{
			bool inside = PointInPolyPPLEx(pt, polygon);

			if (settings.Verify)
				Verify(pt, width, extent, inside);
		}
	}
	else
	{
		// parallelise outer loop, serial point-in-point test
		concurrency::parallel_for_each(points.begin(), points.end(),
			[&](const CadPt2 &pt)
		{
			bool inside = PointInPolySeriallyEx(pt, polygon);

			if (settings.Verify)
				Verify(pt, width, extent, inside);
		});
	}
}

bool PointInPolyPPLEx(const CadPt2 &pt, const CadPolygon &polygon)
{
	concurrency::combinable<int> inout([](){ return 0; }); // thread-local storage, with 0 initializer

	concurrency::parallel_for_each(polygon.begin(), polygon.end(), [&](const CadEdge &edge)
	{
		auto fst = edge.first;
		auto snd = edge.second;

		if ((Float::IsLessThanOrEqual(fst.y, pt.y) && Float::IsLessThan(pt.y, snd.y)) ||
			(Float::IsLessThanOrEqual(snd.y, pt.y) && Float::IsLessThan(pt.y, fst.y)))
		{
			float tdbl1 = Float::Divide(pt.y - fst.y, snd.y - fst.y);
			float tdbl2 = snd.x - fst.x;
			if (Float::IsGreaterThanOrEqual(fst.x + (tdbl2 * tdbl1), pt.x))
				inout.local()++;
		}
	});

	auto val = inout.combine(std::plus<int>());
	return (0 == val ? false : (bool)(0 != val % 2));
}
