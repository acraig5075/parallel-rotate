#include "Verify.h"
#include "Structures.h"
#include <cassert>
#include <cmath>


bool Verify(const CadPt3 &before, float radians, const CadPt3 &after)
{
	if (before == CadPt3{ 0.f, 0.f, 0.f })
		return true;

	auto angle = atan2(after.y, after.x);
	while (Float::IsLessThanOrEqualsZero(angle) || Float::IsZero(angle))
		angle += kPI + kPI;

	if (Float::AreEqual(angle, radians))
	{
		return true;
	}
	else
	{
		assert(false);
		return false;
	}
}

bool Verify(const std::vector<CadPt3> &input, float angle, const std::vector<CadPt3> &output)
{
	assert(input.size() == output.size());
	if (input.size() != output.size())
		return false;

	auto i = std::begin(input);
	auto j = std::begin(output);
	auto radians = degToRad(angle);
	auto cosa = cos(radians);
	auto sina = sin(radians);

	while (i != std::end(input))
	{
		CadPt3 expect = { i->x * cosa - i->y * sina, i->x * sina + i->y * cosa, i->z };
		assert(*j == expect);
		if (*j != expect)
			return false;

		++i;
		++j;
	}

	return true;
}