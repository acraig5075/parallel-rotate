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

}
