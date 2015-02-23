#include "OMP-solution.h"
#include "Structures.h"
#include "Serial-solution.h"
#include "Verify.h"
#include <cassert>

void RotateUsingOMP(const std::vector<CadPt3> &points, float step)
{
	for (float d = step; d <= 360.f; d += step)
		RotateUsingOMPEx(points, degToRad(d));
}

void RotateUsingOMPEx(const std::vector<CadPt3> &points, float radians)
{
	CadMatrix3 rotation = CadMatrix3::MakeRotationZ(radians);

	int size = static_cast<int>(points.size());

	#pragma omp parallel for
	for (int i = 0; i < size; ++i)
	{
		CadPt3 p = points[i];
		CadPt3 r = rotation.Multiply(p);

		if (settings.Verify)
			Verify(p, radians, r);
	}
}

void MultiplyUsingOMP(const std::vector<float> &a, const std::vector<float> &b)
{
	std::vector<float> result = MultiplyUsingOMPEx(a, b);

	if (settings.Verify)
		Verify(result);
}

std::vector<float> MultiplyUsingOMPEx(const std::vector<float> &a, const std::vector<float> &b)
{
	auto side = static_cast<int>(std::sqrt(a.size()));

	std::vector<float> result(a.size());

	#pragma omp parallel for
	for (int r = 0; r < side; ++r)
	{
		for (int c = 0; c < side; ++c)
		{
			float sum = 0.f;
			for (int k = 0; k < side; ++k)
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

void PointInPolyOMP(const std::vector<CadPt2> &points, const CadPolygon &polygon, float width, float extent)
{
	if (settings.PointInPolyWhichLoop == Settings::ParalleliseInnerLoop)
	{
		// parallelise inner loop, point-in-point test
		for (auto pt : points)
		{
			bool inside = PointInPolyOMPEx(pt, polygon);

			if (settings.Verify)
				Verify(pt, width, extent, inside);
		}
	}
	else
	{
		// parallelise outer loop, serial point-in-point test
		int size = points.size();

		#pragma omp parallel for
		for (int i = 0; i < size; ++i)
		{
			CadPt2 pt = points[i];
			bool inside = PointInPolySeriallyEx(pt, polygon);

			if (settings.Verify)
				Verify(pt, width, extent, inside);
		}
	}
}

bool PointInPolyOMPEx(const CadPt2 &pt, const CadPolygon &polygon)
{
	int inout = 0;

	#pragma omp parallel for reduction(+:inout)
	for (int i = 0; i < (int)polygon.size(); ++i)
	{
		CadEdge edge = polygon.at(i);
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

void CheckDuplicatesUsingOMP(const std::vector<CadPt2ID> &points, int gridSize)
{
	auto emptyPair = std::make_pair(0, 0);
	std::vector<std::pair<int, int>> duplicates(points.size(), emptyPair);

	#pragma omp parallel for
	for (int i = 0; i < (int)points.size(); ++i)
	{
		auto outerPt = points.at(i);

		for (int j = 0; j < (int)points.size(); ++j)
		{
			auto innerPt = points.at(j);

			if (innerPt.id != outerPt.id && innerPt.pt == outerPt.pt) // coordinates are the same, but id's are different
			{
				duplicates.at(i) = std::make_pair(innerPt.id, outerPt.id);
			}
		}
	}

	// Remove default zeroed entries
	duplicates.erase(std::remove(duplicates.begin(), duplicates.end(), emptyPair), duplicates.end());

	// Ensure first member of pair is less than second
	std::transform(duplicates.begin(), duplicates.end(), duplicates.begin(), &OrderPair);

	// Sort vector to get pairs ordered adjacent to each other
	std::sort(duplicates.begin(), duplicates.end(), &ComparePairs);

	// The vector now contains a,b entries as well as b,a. Ensure uniqueness.
	duplicates.erase(std::unique(duplicates.begin(), duplicates.end()), duplicates.end());

	if (settings.Verify)
		Verify(duplicates, gridSize);
}
//error C3016 : 'i' : index variable in OpenMP 'for' statement must have signed integral type