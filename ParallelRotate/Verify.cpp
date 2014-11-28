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

// Verify that a matrix multiplied with it's transpose is symmetric
bool Verify(const std::vector<float> &matrix)
{
	auto side = static_cast<size_t>(std::sqrt(matrix.size()));

	for (size_t r = 0; r < side; ++r)
	{
		for (size_t c = r + 1; c < side; ++c)
		{
			auto iUpper = r * side + c;
			auto iLower = c * side + r;

			auto upper = matrix.at(iUpper);
			auto lower = matrix.at(iLower);

			if (Float::AreEqual(upper, lower))
				continue;
			else
			{
				assert(false);
				return false;
			}
		}
	}

	return true;
}

bool Verify(const CadPt2 &pt, float width, float extent, bool test)
{
	assert(static_cast<int>(0.1f / 10.f) == 0);
	assert(static_cast<int>(4.1f / 10.f) == 0);
	assert(static_cast<int>(5.1f / 10.f) == 0);
	assert(static_cast<int>(6.1f / 10.f) == 0);
	assert(static_cast<int>(9.1f / 10.f) == 0);
	assert(static_cast<int>(10.1f / 10.f) == 1);

	auto row = static_cast<int>(pt.y / width);
	auto col = static_cast<int>(pt.x / width);

	bool inside = true;
	if (col == 0 || col == 9 || row == 0)
	{
		inside = false;
	}
	else if (row % 2 == 1)
	{
		if (col == 1)
			inside = (row != 2 || row != 6);
		else if (col == 8)
			inside = (row != 4 || row != 8);
		else
			inside = true;
	}
	else if (row == 2 || row == 6)
	{
		inside = col == 8;
	}
	else if (row == 4 || row == 8)
	{
		inside = col == 1;
	}

	if (inside == test)
	{
		return true;
	}
	else
	{
		assert(false);
		return false;
	}
}
