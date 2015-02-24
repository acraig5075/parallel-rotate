#include "Serial-solution.h"
#include "Structures.h"
#include "Verify.h"
#include <cassert>
#include <iostream>

void CheckDuplicatesSerially(const std::vector<CadPt2ID> &points, int gridSize)
{
	std::vector<std::pair<int, int>> duplicates;

	for (auto outerPt : points)
	{
		for (auto innerPt : points)
		{
			if (innerPt.id != outerPt.id && innerPt.pt == outerPt.pt) // coordinates are the same, but id's are different
			{
				duplicates.push_back(std::make_pair(innerPt.id, outerPt.id));
			}
		}
	}

	// Ensure first member of pair is less than second
	std::transform(duplicates.begin(), duplicates.end(), duplicates.begin(), &OrderPair);

	// Sort vector to get pairs ordered adjacent to each other
	std::sort(duplicates.begin(), duplicates.end(), &ComparePairs);

	// The vector now contains a,b entries as well as b,a. Ensure uniqueness.
	duplicates.erase(std::unique(duplicates.begin(), duplicates.end()), duplicates.end());

	if (settings.Verify)
		Verify(duplicates, gridSize);
}