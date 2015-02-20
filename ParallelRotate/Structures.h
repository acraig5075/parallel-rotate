#pragma once;

#include <vector>
#include <algorithm>
#include <amp_math.h>

extern float kPI;
extern float degToRad(float d);


struct Float 
{
	static const float Float::Accuracy;

	static bool Float::AreEqual(float a, float b);
	static bool Float::IsLessThanOrEqualsZero(float a);
	static bool Float::IsZero(float a);
	static bool Float::IsLessThan(float a, float b);
	static bool Float::IsGreaterThan(float a, float b);
	static bool Float::IsLessThanOrEqual(float a, float b);
	static bool Float::IsGreaterThanOrEqual(float a, float b);

	static float Float::Divide(float a, float b);
};

struct CadPt2
{
	float x = 0.f;
	float y = 0.f;

	CadPt2() = default;
	CadPt2(float x, float y)
		: x(x)
		, y(y)
	{}

	CadPt2 operator+ (const CadPt2 &rhs);
	bool operator== (const CadPt2 &rhs) const;
};

struct CadPt3
{
	float x = 0.f;
	float y = 0.f;
	float z = 0.f;

	CadPt3() = default;
	CadPt3(float x, float y, float z)
		: x(x)
		, y(y)
		, z(z)
	{}

	bool operator== (const CadPt3 &rhs) const;
	bool operator!= (const CadPt3 &rhs) const;
};

struct CadMatrix3
{
	float m[4][4];

	static CadMatrix3 MakeRotationZ(float radians);
	CadPt3 Multiply(const CadPt3 &p) const;
};

struct CadPt2ID
{
	CadPt2 pt;
	int id = 0;
};

bool ComparePairs(const std::pair<int, int> &a, const std::pair<int, int> &b);

using CadEdge = std::pair < CadPt2, CadPt2 >;
using CadPolygon = std::vector < CadEdge > ;