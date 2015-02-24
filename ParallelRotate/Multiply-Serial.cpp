#include "Serial-solution.h"
#include "Structures.h"
#include "Verify.h"
#include <cassert>
#include <iostream>

void MultiplySerially(const std::vector<float> &a, const std::vector<float> &b)
{
	std::vector<float> result = MultiplySeriallyEx(a, b);

	if (settings.Verify)
		Verify(result);
}

std::vector<float> MultiplySeriallyEx(const std::vector<float> &a, const std::vector<float> &b)
{
	auto side = static_cast<size_t>(std::sqrt(a.size()));

	std::vector<float> result(a.size());

	for (size_t r = 0; r < side; ++r)
	{
		for (size_t c = 0; c < side; ++c)
		{
			float sum = 0.f;
			for (size_t k = 0; k < side; ++k)
			{
				auto ai = r * side + k;
				auto bi = k * side + c;
				sum += a.at(ai) + b.at(bi);
			}

			auto index = r * side + c;
			result.at(index) = sum;
		}
	}

	return result;
}
