#include "Serial-solution.h"
#include "Structures.h"
#include "Verify.h"
#include <cassert>
#include <iostream>


void SanityTest(const CadPolygon &polygon, float width, float extent)
{
	bool inside = false;
	CadPt2 pt(5.f, 5.f);

	for (int i = 0; i < 9; ++i)
	{
		assert(inside == PointInPolySeriallyEx(pt, polygon));
		Verify(pt, width, extent, inside);

		pt.x += 10.f;
		pt.y += 10.f;
		inside = !inside;
	}

	pt.x = 10.1f;
	pt.y = 20.1f;
	assert(false == PointInPolySeriallyEx(pt, polygon));
	Verify(pt, width, extent, false);
}

void PointInPolySerially(const std::vector<CadPt2> &points, const CadPolygon &polygon, float width, float extent)
{
	for (auto pt : points)
	{
		bool inside = PointInPolySeriallyEx(pt, polygon);

		if (settings.Verify)
			Verify(pt, width, extent, inside);
	}
}

bool PointInPolySeriallyEx(const CadPt2 &pt, const CadPolygon &polygon)
{
	int inout = 0;

	for (auto edge : polygon)
	{
		auto fst = edge.first;
		auto snd = edge.second;

		if ((Float::IsLessThanOrEqual(fst.y, pt.y) && Float::IsLessThan(pt.y, snd.y)) ||
			(Float::IsLessThanOrEqual(snd.y, pt.y) && Float::IsLessThan(pt.y, fst.y)))
		{
			float tdbl1 = Float::Divide(pt.y - fst.y, snd.y - fst.y);
			float tdbl2 = snd.x - fst.x;
			if (Float::IsGreaterThanOrEqual(fst.x + (tdbl2 * tdbl1), pt.x))
				inout++;
		}
	}
	return (0 == inout ? false : (bool)(0 != inout % 2));
}
