#include "AMP-solution.h"
#include "Structures.h"
#include "Verify.h"
#include <algorithm>
#include <cassert>
#include <numeric>

using namespace concurrency;
using namespace graphics;

float RowMultiply(float col1, float col2, float col3, float x, float y, float z) restrict(amp);
std::vector<float> TranslateFromCadPt3(const std::vector<CadPt3> &points);
std::vector<CadPt3> TranslateToCadPt3(const std::vector<float> &ordinates);
std::vector<float_4> TranslateFromPolygon(const CadPolygon &polygon);


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

	if (kVerify)
	{
		for (size_t i = 0; i < points.size(); ++i)
		{
			Verify(points.at(i), radians, result.at(i));
		}
	}
}


void AMPRuntimeWarmup()
{
	std::vector<CadPt3> points = { { 0.f, 0.f, 0.f } };
	RotateUsingAMPEx(points, 0.f);
}


float RowMultiply(float col1, float col2, float col3, float x, float y, float z) restrict(amp)
{
	return x * col1 + y * col2 + z * col3;
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

void MultiplyUsingAMP(const std::vector<float> &a, const std::vector<float> &b)
{
	const std::vector<float> result = MultiplyUsingAMPEx(a, b);

	if (kVerify)
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

void SanityTest(const CadPolygon &polygon, float width, float extent, array_view<const float_4, 1> &aPolygon)
{
	bool inside = false;
	CadPt2 pt(5.f, 5.f);

	for (int i = 0; i < 9; ++i)
	{
		assert(inside == PointInPolyAMPEx(pt, polygon, aPolygon));
		Verify(pt, width, extent, inside);

		pt.x += 10.f;
		pt.y += 10.f;
		inside = !inside;
	}

	pt.x = 10.1f;
	pt.y = 20.1f;
	assert(false == PointInPolyAMPEx(pt, polygon, aPolygon));
	Verify(pt, width, extent, false);
}

void PointInPolyAMP(const CadPolygon &polygon, float width, float extent)
{
	auto vPolygon = TranslateFromPolygon(polygon);
	array_view<const float_4, 1> av(polygon.size(), vPolygon);

	if (kVerify)
		SanityTest(polygon, width, extent, av);

	CadPt2 pt;
	int xcount = 1;
	for (pt.x = 0.1f; pt.x < extent; pt.x += .1f, ++xcount)
	{
		if (xcount == 10)
		{
			xcount = 0;
			continue;
		}

		int ycount = 1;
		for (pt.y = 0.1f; pt.y < extent; pt.y += .1f, ++ycount)
		{
			if (ycount == 10)
			{
				ycount = 0;
				continue;
			}

			bool inside = PointInPolyAMPEx(pt, polygon, av);

			if (kVerify)
				Verify(pt, width, extent, inside);
		}
	}
}

bool PointInPolyAMPEx(const CadPt2 &pt, const CadPolygon &polygon, array_view<const float_4, 1> &aPolygon)
{
	std::vector<int> vInout(polygon.size());
	array_view<int, 1> aInout(polygon.size(), vInout);
	aInout.discard_data();

	parallel_for_each(aInout.extent, [=](index<1> idx) restrict(amp)
	{
		auto edge = aPolygon[idx[0]];
		float fst[] = { edge.r, edge.g };
		float snd[] = { edge.b, edge.a };

		if (((fst[1] <= pt.y) && (pt.y < snd[1])) ||
			((snd[1] <= pt.y) && (pt.y < fst[1])))
		{
			float tdbl1;
			if (snd[1] == fst[1])
				tdbl1 = (pt.y - fst[1] / 0.0000001f);
			else
				tdbl1 = (pt.y - fst[1] / snd[1] - fst[1]);
			float tdbl2 = snd[0] - fst[0];
			if ((fst[0] + (tdbl2 * tdbl1) >= pt.x))
				aInout[idx[0]] = 1;
			else
				aInout[idx[0]] = 0;
		}
	});

	aInout.synchronize();
	int inout = 0;
	std::accumulate(vInout.begin(), vInout.end(), inout);
	return (0 == inout ? false : (bool)(0 != inout % 2));
}

std::vector<float_4> TranslateFromPolygon(const CadPolygon &polygon)
{
	std::vector<float_4> ret;
	ret.reserve(polygon.size());

	for (auto edge : polygon)
	{
		float_4 f(edge.first.x, edge.first.y, edge.second.x, edge.second.y);
		ret.push_back(f);
	}

	return ret;
}
