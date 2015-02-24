#include "PPL-solution.h"
#include "Structures.h"
#include "Verify.h"
#include <ppl.h>
#include <concurrent_vector.h>

void CheckDuplicatesUsingPPL(const std::vector<CadPt2ID> &points, int gridSize)
{
	concurrency::concurrent_vector<std::pair<int, int>> cvDuplicates;

	concurrency::parallel_for_each(points.begin(), points.end(), [&](const CadPt2ID &outerPt)
	{
		for (auto innerPt : points)
		{
			if (innerPt.id != outerPt.id && innerPt.pt == outerPt.pt) // coordinates are the same, but id's are different
			{
				cvDuplicates.push_back(std::make_pair(innerPt.id, outerPt.id));
			}
		}
	});

	// Copy from concurrent_vector to std::vector
	std::vector<std::pair<int, int>> duplicates;
	duplicates.insert(duplicates.begin(), cvDuplicates.begin(), cvDuplicates.end());

	// Ensure first member of pair is less than second
	std::transform(duplicates.begin(), duplicates.end(), duplicates.begin(), &OrderPair);

	// Sort vector to get pairs ordered adjacent to each other
	std::sort(duplicates.begin(), duplicates.end(), &ComparePairs);

	// The vector now contains a,b entries as well as b,a. Ensure uniqueness.
	duplicates.erase(std::unique(duplicates.begin(), duplicates.end()), duplicates.end());

	if (settings.Verify)
		Verify(duplicates, gridSize);
}