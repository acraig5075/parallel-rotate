#include "AMP-solution.h"
#include "Structures.h"
#include "Verify.h"
#include <numeric>

using namespace concurrency;
using namespace graphics;

void MultiplyUsingAMP(const std::vector<float> &a, const std::vector<float> &b)
{
	const std::vector<float> result = MultiplyUsingAMPEx(a, b);

	if (settings.Verify)
		Verify(result);
}

std::vector<float> MultiplyUsingAMPEx(const std::vector<float> &a, const std::vector<float> &b)
{
	auto side = static_cast<size_t>(std::sqrt(a.size()));

	std::vector<float> result(a.size());

	array_view<const float, 2> av(side, side, a);
	array_view<const float, 2> bv(side, side, b);
	array_view<float, 2> resultv(side, side, result);
	resultv.discard_data();

	parallel_for_each(resultv.extent, [=](index<2> idx) restrict(amp)
	{
		size_t r = idx[0];
		size_t c = idx[1];

		float sum = 0.f;
		for (size_t k = 0; k < side; ++k)
		{
			sum += av[r][k] + bv[k][c];
		}

		resultv[r][c] = sum;
	});

	resultv.synchronize();
	return result;
}


