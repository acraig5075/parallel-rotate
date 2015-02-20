#include "Structures.h"
#include <cmath>
#include <cstring>


float kPI = 3.141592653589793f;
float degToRad(float d) { return d * kPI / 180.f; }

const float Float::Accuracy = 0.0001f;

bool Float::AreEqual(float a, float b)
{
	return concurrency::precise_math::fabs(a - b) <= (Accuracy + Accuracy * concurrency::precise_math::fabs(b));
}
bool Float::IsLessThanOrEqualsZero(float a)
{
	return a <= -Accuracy;
}
bool Float::IsZero(float a)
{
	return concurrency::precise_math::fabs(a) <= Accuracy;
}
bool Float::IsLessThan(float a, float b)
{
	return a < (b - (Accuracy + Accuracy * concurrency::precise_math::fabs(b)));
}
bool Float::IsGreaterThan(float a, float b)
{
	return a >(b + (Accuracy + Accuracy * concurrency::precise_math::fabs(b)));
}
bool Float::IsLessThanOrEqual(float a, float b)
{
	return a <= (b + (Accuracy + Accuracy * concurrency::precise_math::fabs(b)));
}
bool Float::IsGreaterThanOrEqual(float a, float b)
{
	return a >= (b - (Accuracy + Accuracy * concurrency::precise_math::fabs(b)));
}

float Float::Divide(float a, float b)
{
	if (IsZero(b))
		b = 0.0000001f;
	return (a / b);
}

CadPt2 CadPt2::operator+ (const CadPt2 &rhs)
{
	return{ this->x + rhs.x, this->y + rhs.y };
}

bool CadPt2::operator== (const CadPt2 &rhs) const
{
	return (Float::AreEqual(x, rhs.x) && Float::AreEqual(y, rhs.y));
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

bool ComparePairs(const std::pair<int, int> &a, const std::pair<int, int> &b)
{
	if (a.first == b.first)
		return a.second < b.second;
	else
		return a.first < b.first;
}

