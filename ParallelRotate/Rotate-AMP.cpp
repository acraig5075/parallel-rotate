#include "AMP-solution.h"
#include "Structures.h"
#include "Verify.h"
#include <cassert>
#include <numeric>

using namespace concurrency;
using namespace graphics;

std::vector<float> TranslateFromCadPt3(const std::vector<CadPt3> &points);
std::vector<CadPt3> TranslateToCadPt3(const std::vector<float> &ordinates);


float RowMultiply(float col1, float col2, float col3, float x, float y, float z) restrict(amp)
{
	return x * col1 + y * col2 + z * col3;
}

void RotateUsingAMP(const std::vector<CadPt3> &points, float step)
{
	for (float d = step; d <= 360.f; d += step)
		RotateUsingAMPEx(points, degToRad(d));
}

void RotateUsingAMPEx(const std::vector<CadPt3> &points, float radians)
{
	// rotation matrix and equivalent AMP grid
	CadMatrix3 rotation = CadMatrix3::MakeRotationZ(radians);
	array_view<const float, 2> ampRotation(4, 4, rotation.m[0]);

	// input coordinates as sequential floats, and equivalent AMP grid
	std::vector<float> input = TranslateFromCadPt3(points);
	array_view<const float, 2> ampInput(points.size(), 3, input);

	// output coordinates as sequential floats, and equivalent AMP grid
	std::vector<float> output(input.size());
	array_view<float, 2> ampOutput(points.size(), 3, output);
	//ampOutput.discard_data();

	parallel_for_each(ampInput.extent, [=](index<2> idx) restrict(amp)
	{
		int row = idx[0];
		int col = idx[1];
		ampOutput[row][col] += RowMultiply(ampRotation[col][0], ampRotation[col][1], ampRotation[col][2], ampInput[row][0], ampInput[row][1], ampInput[row][2]);
	});

	ampOutput.synchronize();
	std::vector<CadPt3> result = TranslateToCadPt3(output);

	if (settings.Verify)
	{
		for (size_t i = 0; i < points.size(); ++i)
		{
			Verify(points.at(i), radians, result.at(i));
		}
	}
}


std::vector<float> TranslateFromCadPt3(const std::vector<CadPt3> &points)
{
	std::vector<float> ordinates;
	ordinates.reserve(3 * points.size());

	std::for_each(points.begin(), points.end(), [&](const CadPt3 &p)
	{
		ordinates.push_back(p.x);
		ordinates.push_back(p.y);
		ordinates.push_back(p.z);
	});

	return ordinates;
}

std::vector<CadPt3> TranslateToCadPt3(const std::vector<float> &ordinates)
{
	std::vector<CadPt3> points;
	points.reserve(ordinates.size() / 3);

	for (auto it = ordinates.begin(); it != ordinates.end(); ++it)
	{
		float x = *it;
		++it;
		float y = *it;
		++it;
		float z = *it;
		points.push_back({ x, y, z });
	}

	return points;
}


void AMPRuntimeWarmup()
{
	std::vector<CadPt3> points = { { 0.f, 0.f, 0.f } };
	RotateUsingAMPEx(points, 0.f);
}
