#include "PPL-solution.h"
#include "Structures.h"
#include "Verify.h"
#include <ppl.h>

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