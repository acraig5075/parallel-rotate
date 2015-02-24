#include "PPL-solution.h"
#include "Structures.h"
#include "Serial-solution.h"
#include "Verify.h"
#include <algorithm>
#include <ppl.h>
#include <concurrent_vector.h>


void MultiplyUsingPPL(const std::vector<float> &a, const std::vector<float> &b)
{
	std::vector<float> result = MultiplyUsingPPLEx(a, b);

	if (settings.Verify)
		Verify(result);
}

std::vector<float> MultiplyUsingPPLEx(const std::vector<float> &a, const std::vector<float> &b)
{
	auto side = static_cast<size_t>(std::sqrt(a.size()));

	std::vector<float> result(a.size());

	concurrency::parallel_for(size_t(0), side, [&](size_t r)
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
	});
	return result;
}

