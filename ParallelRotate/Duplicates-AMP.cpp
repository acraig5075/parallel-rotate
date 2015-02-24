#include "AMP-solution.h"
#include "Structures.h"
#include "Verify.h"
#include <numeric>

using namespace concurrency;
using namespace graphics;

auto TranslateFromCadPt2ID(const std::vector<CadPt2ID> &points) -> std::vector<float_3>
{
	std::vector<float_3> v;
	std::transform(points.begin(), points.end(), std::back_inserter(v), [](const CadPt2ID &p)
	{
		return float_3(p.pt.x, p.pt.y, static_cast<float>(p.id) + 0.1F);
	});
	return v;
}

auto TranslateToIntPairs(const std::vector<int_2> &duplicates) -> std::vector<std::pair<int, int>>
{
	std::vector<std::pair<int, int>> v;
	std::transform(duplicates.begin(), duplicates.end(), std::back_inserter(v), [](const int_2 &p)
	{
		return std::make_pair(p.x, p.y);
	});
	return v;

}

void CheckDuplicatesUsingAMP(const std::vector<CadPt2ID> &points, int gridSize)
{
	size_t nPoints = points.size();

	std::vector<int_2> stdDuplicates(nPoints);
	concurrency::array_view<int_2, 1> ampDuplicates(nPoints, stdDuplicates);

	std::vector<float_3> stdPoints = TranslateFromCadPt2ID(points);
	concurrency::array_view<const float_3, 1> ampPoints(nPoints, stdPoints);

	parallel_for_each(ampPoints.extent, [=](index<1> idx) restrict(amp)
	{
		float_3 outerPt = ampPoints[idx[0]];
		int outerID = static_cast<int>(outerPt.b);

		for (unsigned int i = 0; i < ampPoints.extent.size(); ++i)
		{
			float_3 innerPt = ampPoints[i];
			int innerID = static_cast<int>(innerPt.b);

			if (innerID != outerID && (innerPt.x == outerPt.x && innerPt.y == outerPt.y)) // coordinates are the same, but id's are different
			{
				ampDuplicates[idx[0]].x = innerID;
				ampDuplicates[idx[0]].y = outerID;
			}
		}
	});

	ampDuplicates.synchronize();

	auto duplicates = TranslateToIntPairs(stdDuplicates);

	// Remove default zeroed entries
	auto emptyPair = std::make_pair(0, 0);
	duplicates.erase(std::remove(duplicates.begin(), duplicates.end(), emptyPair), duplicates.end());

	// Ensure first member of pair is less than second
	std::transform(duplicates.begin(), duplicates.end(), duplicates.begin(), &OrderPair);

	// Sort vector to get pairs ordered adjacent to each other
	std::sort(duplicates.begin(), duplicates.end(), &ComparePairs);

	// The vector now contains a,b entries as well as b,a. Ensure uniqueness.
	duplicates.erase(std::unique(duplicates.begin(), duplicates.end()), duplicates.end());

	if (settings.Verify)
		Verify(duplicates, gridSize);
}