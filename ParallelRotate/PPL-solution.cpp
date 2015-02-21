#include "PPL-solution.h"
#include "Structures.h"
#include "Verify.h"
#include <algorithm>
#include <ppl.h>
#include <concurrent_vector.h>


void RotateUsingPPL(const std::vector<CadPt3> &points, float step)
{
	for (float d = step; d <= 360.f; d += step)
		RotateUsingPPLEx(points, degToRad(d));
}

void RotateUsingPPLEx(const std::vector<CadPt3> &points, float radians)
{
	std::vector<CadPt3> result;
	result.reserve(points.size());

	CadMatrix3 rotation = CadMatrix3::MakeRotationZ(radians);

	concurrency::parallel_for_each(points.begin(), points.end(), 
	[&](const CadPt3 &p)
	{
		CadPt3 r = rotation.Multiply(p);

		if (kVerify)
			Verify(p, radians, r);
	});
}

void MultiplyUsingPPL(const std::vector<float> &a, const std::vector<float> &b)
{
	std::vector<float> result = MultiplyUsingPPLEx(a, b);

	if (kVerify)
		Verify(result);
}

std::vector<float> MultiplyUsingPPLEx(const std::vector<float> &a, const std::vector<float> &b)
{
	auto side = static_cast<size_t>(std::sqrt(a.size()));

	std::vector<float> result(a.size());

	concurrency::parallel_for(size_t(0), side, [&](size_t r)
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
	});
	return result;
}


void PointInPolyPPL(const CadPolygon &polygon, float width, float extent)
{
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

			bool inside = PointInPolyPPLEx(pt, polygon);

			if (kVerify)
				Verify(pt, width, extent, inside);
		}
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

void CheckDuplicatesUsingPPL(const std::vector<CadPt2ID> &points, int gridSize)
{
	concurrency::concurrent_vector<std::pair<int, int>> cvDuplicates;

	concurrency::parallel_for_each(points.begin(), points.end(), [&](const CadPt2ID &outerPt)
	{
		for (auto innerPt : points)
		{
			if (innerPt.id != outerPt.id && innerPt.pt == outerPt.pt) // coordinates are the same, but id's are different
			{
				cvDuplicates.push_back(std::make_pair(innerPt.id, outerPt.id));
			}
		}
	});

	// Copy from concurrent_vector to std::vector
	std::vector<std::pair<int, int>> duplicates;
	duplicates.insert(duplicates.begin(), cvDuplicates.begin(), cvDuplicates.end());

	// Ensure first member of pair is less than second
	std::transform(duplicates.begin(), duplicates.end(), duplicates.begin(), &OrderPair);

	// Sort vector to get pairs ordered adjacent to each other
	std::sort(duplicates.begin(), duplicates.end(), &ComparePairs);

	// The vector now contains a,b entries as well as b,a. Ensure uniqueness.
	duplicates.erase(std::unique(duplicates.begin(), duplicates.end()), duplicates.end());

	if (kVerify)
		Verify(duplicates, gridSize);
}