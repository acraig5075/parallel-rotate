#include "Structures.h"
#include <cmath>
#include <cstring>


float kPI = 3.141592653589793f;
float degToRad(float d) { return d * kPI / 180.f; }

const float Float::Accuracy = 0.0001f;

bool Float::AreEqual(float a, float b)
{
	return fabs(a - b) <= (Accuracy + Accuracy * fabs(b));
}

bool Float::IsLessThanOrEqualsZero(float a)
{
	return a <= -Accuracy;
}

bool Float::IsZero(double a)
{
	return fabs(a) <= Accuracy;
}

bool CadPt3::operator== (const CadPt3 &rhs) const
{
	return (Float::AreEqual(x, rhs.x) && Float::AreEqual(y, rhs.y) && Float::AreEqual(z, rhs.z));
}

bool CadPt3::operator!= (const CadPt3 &rhs) const
{
	return !(*this == rhs);
}

CadMatrix3 CadMatrix3::MakeRotationZ(float radians)
{
	float cosa = cos(radians);
	float sina = sin(radians);

	CadMatrix3 ret;
	memset(ret.m[0], 0, 4 * sizeof(float));
	memset(ret.m[1], 0, 4 * sizeof(float));
	memset(ret.m[2], 0, 4 * sizeof(float));
	memset(ret.m[3], 0, 4 * sizeof(float));

	ret.m[0][0] = cosa;
	ret.m[0][1] = -sina;
	ret.m[1][0] = sina;
	ret.m[1][1] = cosa;
	ret.m[2][2] = 1.f;
	ret.m[3][3] = 1.f;
	return ret;
}

CadPt3 CadMatrix3::Multiply(const CadPt3 &p) const
{
	auto x = m[0][0] * p.x + m[0][1] * p.y + m[0][2] * p.z + m[0][3];
	auto y = m[1][0] * p.x + m[1][1] * p.y + m[1][2] * p.z + m[1][3];
	auto z = m[2][0] * p.x + m[2][1] * p.y + m[2][2] * p.z + m[2][3];
	return{ x, y, z };
}
