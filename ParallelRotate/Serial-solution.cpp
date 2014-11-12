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