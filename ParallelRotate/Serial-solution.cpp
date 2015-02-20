#include "Serial-solution.h"
#include "Structures.h"
#include "Verify.h"
#include <cassert>

void RotateSerially(const std::vector<CadPt3> &points, float step)
{
	for (float d = step; d <= 360.f; d += step)
		RotateSeriallyEx(points, degToRad(d));
}

void RotateSeriallyEx(const std::vector<CadPt3> &points, float radians)
{
	auto rotation = CadMatrix3::MakeRotationZ(radians);

	for (auto it = points.begin(); it != points.end(); ++it)
	{
		CadPt3 pt = rotation.Multiply(*it);
		
		if (kVerify)
			Verify(*it, radians, pt);
	}
}

void MultiplySerially(const std::vector<float> &a, const std::vector<float> &b)
{
	std::vector<float> result = MultiplySeriallyEx(a, b);

	if (kVerify)
		Verify(result);
}

std::vector<float> MultiplySeriallyEx(const std::vector<float> &a, const std::vector<float> &b)
{
	auto side = static_cast<size_t>(std::sqrt(a.size()));

	std::vector<float> result(a.size());

	for (size_t r = 0; r < side; ++r)
	{
		for (size_t c = 0; c < side; ++c)
		{
			float sum = 0.f;
			for (size_t k = 0; k < side; ++k)
			{
				auto ai = r * side + k;
				auto bi = k * side + c;
				sum += a.at(ai) + b.at(bi);
			}

			auto index = r * side + c;
			result.at(index) = sum;
		}
	}

	return result;
}

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

void PointInPolySerially(const CadPolygon &polygon, float width, float extent)
{
	if (kVerify)
		SanityTest(polygon, width, extent);

	CadPt2 pt;
	int xcount = 1;
	for (pt.x = 0.1f; pt.x < extent; pt.x += .1f, ++xcount)
	{
		if (xcount == 10)
		{
			xcount = 0;
			continue;
		}

		int ycount = 1;
		for (pt.y = 0.1f; pt.y < extent; pt.y += .1f, ++ycount)
		{
			if (ycount == 10)
			{
				ycount = 0;
				continue;
			}

			bool inside = PointInPolySeriallyEx(pt, polygon);

			if (kVerify)
				Verify(pt, width, extent, inside);
		}
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

void CheckDuplicatesSerially(const std::vector<CadPt2ID> &points, int gridSize)
{
	std::vector<std::pair<int, int>> duplicates;

	for (auto outerPt : points)
	{
		for (auto innerPt : points)
		{
			if (innerPt.id != outerPt.id && innerPt.pt == outerPt.pt) // coordinates are the same, but id's are different
			{
				auto smaller = std::min<int>(innerPt.id, outerPt.id);
				auto larger = std::max<int>(innerPt.id, outerPt.id);
				duplicates.push_back(std::make_pair(smaller, larger));
			}
		}
	}

	// Sort vector to get pairs ordered adjacent to each other
	std::sort(duplicates.begin(), duplicates.end(), &ComparePairs);

	// The vector now contains a,b entries as well as b,a. Ensure uniqueness.
	duplicates.erase(std::unique(duplicates.begin(), duplicates.end()), duplicates.end());

	if (kVerify)
		Verify(duplicates, gridSize);
}