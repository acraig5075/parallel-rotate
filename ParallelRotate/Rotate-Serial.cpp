#include "Serial-solution.h"
#include "Structures.h"
#include "Verify.h"

void RotateSerially(const std::vector<CadPt3> &points, float step)
{
	for (float d = step; d <= 360.f; d += step)
		RotateSeriallyEx(points, degToRad(d));
}

void RotateSeriallyEx(const std::vector<CadPt3> &points, float radians)
{
	auto rotation = CadMatrix3::MakeRotationZ(radians);

	for (auto p : points)
	{
		CadPt3 r = rotation.Multiply(p);
		
		if (settings.Verify)
			Verify(p, radians, r);
	}
}

