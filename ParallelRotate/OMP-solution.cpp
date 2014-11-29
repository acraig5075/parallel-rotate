#include "OMP-solution.h"
#include "Structures.h"
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

		if (kVerify)
			Verify(p, radians, r);
	}
}

void MultiplyUsingOMP(const std::vector<float> &a, const std::vector<float> &b)
{
	std::vector<float> result = MultiplyUsingOMPEx(a, b);

	if (kVerify)
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

void PointInPolyOMP(const CadPolygon &polygon, float width, float extent)
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

			bool inside = PointInPolyOMPEx(pt, polygon);

			if (kVerify)
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

//error C3016 : 'i' : index variable in OpenMP 'for' statement must have signed integral type