#pragma once;

extern float kPI;
extern float degToRad(float d);


struct Float
{
	static const float Accuracy;
	static bool AreEqual(float a, float b);
	static bool IsLessThanOrEqualsZero(float a);
	static bool Float::IsZero(double a);
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
//
//bool operator== (const CadPt3 &lhs, const CadPt3 &rhs)
//{
//	return (Float::AreEqual(lhs.x, rhs.x) && Float::AreEqual(lhs.y, rhs.y) && Float::AreEqual(lhs.z, rhs.z));
//}
//
//bool operator!= (const CadPt3 &lhs, const CadPt3 &rhs)
//{
//	return !(lhs == rhs);
//}

struct CadMatrix3
{
	float m[4][4];

	static CadMatrix3 MakeRotationZ(float radians);
	CadPt3 Multiply(const CadPt3 &p) const;
};
