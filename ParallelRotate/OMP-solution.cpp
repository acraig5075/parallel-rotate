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

}
//error C3016 : 'i' : index variable in OpenMP 'for' statement must have signed integral type