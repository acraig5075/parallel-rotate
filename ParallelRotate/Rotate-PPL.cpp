#include "PPL-solution.h"
#include "Structures.h"
#include "Verify.h"
#include <ppl.h>
#include <concurrent_vector.h>


void RotateUsingPPL(const std::vector<CadPt3> &points, float step)
{
	for (float d = step; d <= 360.f; d += step)
		RotateUsingPPLEx(points, degToRad(d));
}

void RotateUsingPPLEx(const std::vector<CadPt3> &points, float radians)
{
	auto rotation = CadMatrix3::MakeRotationZ(radians);

	concurrency::parallel_for_each(points.begin(), points.end(), 
	[&](const CadPt3 &p)
	{
		CadPt3 r = rotation.Multiply(p);

		if (settings.Verify)
			Verify(p, radians, r);
	});
}
