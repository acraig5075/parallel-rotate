// ParallelRotate.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Structures.h"
#include "Settings.h"
#include "Polygon.h"
#include "Serial-solution.h" // not parallel
#include "PPL-solution.h"    // Parallel Patterns Library
#include "OMP-solution.h"    // OpenMP
#include "AMP-solution.h"    // Accelerated Massive Parallelism

#include <vector>
#include <functional>
#include <chrono>
#include <iostream>
#include <random>


Settings settings;


// rotation
__int64 TimeFunction(const std::function<void(const std::vector<CadPt3> &, float)> &func, const std::vector<CadPt3> &p)
{
	auto start = std::chrono::high_resolution_clock::now();

	func(p, settings.RotationDegreeStep);

	auto end = std::chrono::high_resolution_clock::now();

	return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

// multiplication
__int64 TimeFunction(const std::function<void(const std::vector<float> &, const std::vector<float> &)> &func, const std::vector<float> &a, const std::vector<float> &b)
{
	auto start = std::chrono::high_resolution_clock::now();

	func(a, b);

	auto end = std::chrono::high_resolution_clock::now();

	return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

// point-in-poly
__int64 TimeFunction(const std::function<void(const CadPolygon &, float, float)> &func, const CadPolygon &poly, float width, float extent)
{
	auto start = std::chrono::high_resolution_clock::now();

	func(poly, width, extent);

	auto end = std::chrono::high_resolution_clock::now();

	return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

// duplicates
__int64 TimeFunction(const std::function<void(const std::vector<CadPt2ID> &, int)> &func, const std::vector<CadPt2ID> &points)
{
	auto start = std::chrono::high_resolution_clock::now();

	func(points, settings.DuplicatesGridSize);

	auto end = std::chrono::high_resolution_clock::now();

	return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

}

std::vector<CadPt3> GetInputForRotation()
{
	size_t size = settings.RotationNumPoints;

	std::vector<CadPt3> points(size);

	float x = 0.f;
	float y = 0.f;
	float z = 0.f;

	for (size_t i = 0; i < size; ++i)
	{
		points.at(i) = { x, y, z };
		x += 1.f;
		z += 1.f;
	}

	return points;
}

std::vector<float> GetInputforMultiplication()
{
	size_t square = settings.MultiplicationMatSize;

	std::vector<float> matrix;
	matrix.reserve(square * square);

	std::default_random_engine generator;
	std::uniform_real_distribution<float> distribution(0.f, 1.f);

	for (size_t r = 0; r < square; ++r)
	{
		for (size_t c = 0; c < square; ++c)
		{
			float value = distribution(generator);
			matrix.push_back(value);
		}
	}

	return matrix;
}

std::vector<float> TransformSquareMatrix(std::vector<float> &matrix, int size = settings.MultiplicationMatSize)
{
	std::vector<float> transform(size * size);

	int c = 0;
	int r = 0;
	for (auto it = matrix.cbegin(); it != matrix.cend(); ++it)
	{
		auto index = c * size + r;
		transform.at(index) = *it;

		++c;
		if (c == size)
		{
			c = 0;
			++r;
		}
	}

	return transform;
}

std::vector<CadPt2ID> GetInputForDuplicates()
{
	std::vector<CadPt2ID> points;

	int id = 1000;
	int duplicateID = 100000;

	CadPt2 origin(50000.f, 50000.f);
	CadPt2ID p;
	p.pt = origin;
	p.id = id;
	float spacing = 25.f;
	int grid = settings.DuplicatesGridSize;

	for (int row = 0; row < grid; ++row)
	{
		for (int col = 0; col < grid; ++col)
		{
			points.push_back(p);

			// manufactured duplicate
			if (col == row)
			{
				int oldID = p.id;
				p.id = duplicateID;
				points.push_back(p);
				duplicateID++;
				p.id = oldID;
			}

			p.pt.x += spacing;
			p.id++;
		}

		p.pt.x = origin.x;
		p.pt.y += spacing;
	}

	return points;
}

void Rotation()
{
	std::vector<CadPt3> points = GetInputForRotation();

	AMPRuntimeWarmup();

	__int64 duration1 = !settings.UseSerial ? 0 : TimeFunction(&RotateSerially, points);
	__int64 duration2 = !settings.UsePpl    ? 0 : TimeFunction(&RotateUsingPPL, points);
	__int64 duration3 = !settings.UseOmp    ? 0 : TimeFunction(&RotateUsingOMP, points);
	__int64 duration4 = !settings.UseAmp    ? 0 : TimeFunction(&RotateUsingAMP, points);

	std::cout << "Rotating " << settings.RotationNumPoints << " points\n"
		<< duration1 << "\n"
		<< duration2 << "\n"
		<< duration3 << "\n"
		<< duration4 << "\n"
		<< std::endl;

	points.clear();
}

void Multiplication()
{
	std::vector<float> matrix = GetInputforMultiplication();
	std::vector<float> transform = TransformSquareMatrix(matrix);
	size_t square = settings.MultiplicationMatSize;

	__int64 duration1 = !settings.UseSerial ? 0 : TimeFunction(&MultiplySerially, matrix, transform);
	__int64 duration2 = !settings.UsePpl    ? 0 : TimeFunction(&MultiplyUsingPPL, matrix, transform);
	__int64 duration3 = !settings.UseOmp    ? 0 : TimeFunction(&MultiplyUsingOMP, matrix, transform);
	__int64 duration4 = !settings.UseAmp    ? 0 : TimeFunction(&MultiplyUsingAMP, matrix, transform);

	std::cout << "Multiplying " << square << "x" << square << " matrices\n"
		<< duration1 << "\n"
		<< duration2 << "\n"
		<< duration3 << "\n"
		<< duration4 << "\n"
		<< std::endl;

	matrix.clear();
	transform.clear();
}

void PointInPoly()
{
	float width = 10.f;
	float extent = 100.f;
	CadPolygon polygon = MakePolygon(0, width, extent);

	__int64 duration1 = !settings.UseSerial ? 0 : TimeFunction(&PointInPolySerially, polygon, width, extent);
	__int64 duration2 = !settings.UsePpl    ? 0 : TimeFunction(&PointInPolyPPL, polygon, width, extent);
	__int64 duration3 = !settings.UseOmp    ? 0 : TimeFunction(&PointInPolyOMP, polygon, width, extent);
	__int64 duration4 = !settings.UseAmp    ? 0 : TimeFunction(&PointInPolyAMP, polygon, width, extent);

	std::cout << "Point in polygon\n"
		<< duration1 << "\n"
		<< duration2 << "\n"
		<< duration3 << "\n"
		<< duration4 << "\n"
		<< std::endl;
}

void Duplicates()
{
	std::vector<CadPt2ID> points = GetInputForDuplicates();

	__int64 duration1 = !settings.UseSerial ? 0 : TimeFunction(&CheckDuplicatesSerially, points);
	__int64 duration2 = !settings.UsePpl    ? 0 : TimeFunction(&CheckDuplicatesUsingPPL, points);
	__int64 duration3 = !settings.UseOmp    ? 0 : TimeFunction(&CheckDuplicatesUsingOMP, points);
	__int64 duration4 = !settings.UseAmp    ? 0 : TimeFunction(&CheckDuplicatesUsingAMP, points);

	std::cout << "Removing duplicates in " << points.size() << " points\n"
		<< duration1 << "\n"
		<< duration2 << "\n"
		<< duration3 << "\n"
		<< duration4 << "\n"
		<< std::endl;
}


int _tmain(int argc, _TCHAR* argv[])
{
	if (settings.HasFile())
	{
		settings.EverythingOff();
		settings.Read();
	}
	else
	{
		settings.EverythingOn();
		settings.Write();
	}

	if (settings.DoRotation)
		Rotation();

	if (settings.DoMultiplication)
		Multiplication();

	if (settings.DoPointInPoly)
		PointInPoly();

	if (settings.DoDuplicates)
		Duplicates();

	return 0;
}

