#pragma once;

#include <vector>
#include <algorithm>

extern float kPI;
extern float degToRad(float d);


struct Float
{
	static const float Accuracy;
	static bool AreEqual(float a, float b);
	static bool IsLessThanOrEqualsZero(float a);
	static bool IsZero(float a);
	static bool IsLessThan(float a, float b);
	static bool IsLessThanOrEqual(float a, float b);
	static bool IsGreaterThan(float a, float b);
	static bool IsGreaterThanOrEqual(float a, float b);
	static float Divide(float a, float b);
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

using CadEdge = std::pair < CadPt2, CadPt2 >;
using CadPolygon = std::vector < CadEdge > ;