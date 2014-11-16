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
//error C3016 : 'i' : index variable in OpenMP 'for' statement must have signed integral type