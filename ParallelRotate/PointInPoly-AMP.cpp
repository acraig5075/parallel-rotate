#include "AMP-solution.h"
#include "Structures.h"
#include "Verify.h"
#include <algorithm>
#include <cassert>
#include <numeric>

using namespace concurrency;
using namespace graphics;


std::vector<float_4> TranslateFromPolygon(const CadPolygon &polygon)
{
	std::vector<float_4> ret;
	ret.reserve(polygon.size());

	for (auto edge : polygon)
	{
		float_4 f(edge.first.x, edge.first.y, edge.second.x, edge.second.y);
		ret.push_back(f);
	}

	return ret;
}

void SanityTest(const CadPolygon &polygon, float width, float extent, array_view<const float_4, 1> &aPolygon)
{
	bool inside = false;
	CadPt2 pt(5.f, 5.f);

	for (int i = 0; i < 9; ++i)
	{
		assert(inside == PointInPolyAMPEx(pt, polygon, aPolygon));
		Verify(pt, width, extent, inside);

		pt.x += 10.f;
		pt.y += 10.f;
		inside = !inside;
	}

	pt.x = 10.1f;
	pt.y = 20.1f;
	assert(false == PointInPolyAMPEx(pt, polygon, aPolygon));
	Verify(pt, width, extent, false);
}

void PointInPolyAMP(const std::vector<CadPt2> &points, const CadPolygon &polygon, float width, float extent)
{
	return; // fixme, not working

	auto vPolygon = TranslateFromPolygon(polygon);
	array_view<const float_4, 1> av(polygon.size(), vPolygon);

	for (auto pt : points)
	{
		bool inside = PointInPolyAMPEx(pt, polygon, av);

		if (settings.Verify)
			Verify(pt, width, extent, inside);
	}
}

bool PointInPolyAMPEx(const CadPt2 &pt, const CadPolygon &polygon, array_view<const float_4, 1> &aPolygon)
{
	std::vector<int> vInout(polygon.size());
	array_view<int, 1> aInout(polygon.size(), vInout);
	aInout.discard_data();

	parallel_for_each(aInout.extent, [=](index<1> idx) restrict(amp)
	{
		int i = idx[0];
		auto edge = aPolygon[i];
		float fst[] = { edge.r, edge.g };
		float snd[] = { edge.b, edge.a };

		if (((fst[1] <= pt.y) && (pt.y < snd[1])) ||
			((snd[1] <= pt.y) && (pt.y < fst[1])))
		{
			float tdbl1;
			if (snd[1] == fst[1])
				tdbl1 = (pt.y - fst[1] / 0.0000001f);
			else
				tdbl1 = (pt.y - fst[1] / snd[1] - fst[1]);
			float tdbl2 = snd[0] - fst[0];
			if ((fst[0] + (tdbl2 * tdbl1) >= pt.x))
				aInout[i] = 1;
			else
				aInout[i] = 0;
		}
	});

	aInout.synchronize();
	int inout = 0;
	std::accumulate(vInout.begin(), vInout.end(), inout);
	return (0 == inout ? false : (bool)(0 != inout % 2));
}
