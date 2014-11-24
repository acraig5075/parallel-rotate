// ParallelRotate.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Structures.h"
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


const int kSize = 10000;
const bool kVerify = false;
const float kStep = 1.f;
const int kSquare = 400;


__int64 TimeFunction(const std::function<void(const std::vector<CadPt3> &, float)> &func, const std::vector<CadPt3> &p)
{
	auto start = std::chrono::high_resolution_clock::now();

	func(p, kStep);

	auto end = std::chrono::high_resolution_clock::now();

	return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

__int64 TimeFunction(const std::function<void(const std::vector<float> &, const std::vector<float> &)> &func, const std::vector<float> &a, const std::vector<float> &b)
{
	auto start = std::chrono::high_resolution_clock::now();

	func(a, b);

	auto end = std::chrono::high_resolution_clock::now();

	return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

__int64 TimeFunction(const std::function<void(const CadPolygon &, float, float)> &func, const CadPolygon &poly, float width, float extent)
{
	auto start = std::chrono::high_resolution_clock::now();

	func(poly, width, extent);

	auto end = std::chrono::high_resolution_clock::now();

	return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

std::vector<CadPt3> GetInputForRotation()
{
	std::vector<CadPt3> points(kSize);

	float x = 0.f;
	float y = 0.f;
	float z = 0.f;

	for (int i = 0; i < kSize; ++i)
	{
		points.at(i) = { x, y, z };
		x += 1.f;
		z += 1.f;
	}

	return points;
}

std::vector<float> GetInputforMultiplication()
{
	std::vector<float> matrix;
	matrix.reserve(kSquare * kSquare);

	std::default_random_engine generator;
	std::uniform_real_distribution<float> distribution(0.f, 1.f);

	for (int r = 0; r < kSquare; ++r)
	{
		for (int c = 0; c < kSquare; ++c)
		{
			float value = distribution(generator);
			matrix.push_back(value);
		}
	}

	return matrix;
}

std::vector<float> TransformSquareMatrix(std::vector<float> &matrix, int size = kSquare)
{
	std::vector<float> transform(size * size);

	int c = 0;
	int r = 0;
	for (auto it = matrix.cbegin(); it != matrix.cend(); ++it)
	{
		auto index = r * size + c;
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

void Rotation()
{
	std::vector<CadPt3> points = GetInputForRotation();

	AMPRuntimeWarmup();

	__int64 duration1 = TimeFunction(&RotateSerially, points);
	__int64 duration2 = TimeFunction(&RotateUsingPPL, points);
	__int64 duration3 = TimeFunction(&RotateUsingOMP, points);
	__int64 duration4 = TimeFunction(&RotateUsingAMP, points);

	std::cout << "Rotating " << kSize << " points\n";
	std::cout << duration1 << "\n";
	std::cout << duration2 << "\n";
	std::cout << duration3 << "\n";
	std::cout << duration4 << "\n";
	std::cout << std::endl;

	points.clear();
}

void Multiplication()
{
	std::vector<float> matrix = GetInputforMultiplication();
	std::vector<float> transform = TransformSquareMatrix(matrix);

	__int64 duration1 = TimeFunction(&MultiplySerially, matrix, transform);
	__int64 duration2 = TimeFunction(&MultiplyUsingPPL, matrix, transform);
	__int64 duration3 = TimeFunction(&MultiplyUsingOMP, matrix, transform);
	__int64 duration4 = TimeFunction(&MultiplyUsingAMP, matrix, transform);

	std::cout << "Multiplying " << kSquare << "x" << kSquare << " matrices\n";
	std::cout << duration1 << "\n";
	std::cout << duration2 << "\n";
	std::cout << duration3 << "\n";
	std::cout << duration4 << "\n";
	std::cout << std::endl;

	matrix.clear();
	transform.clear();
}

void PointInPoly()
{
	float width = 10.f;
	float extent = 100.f;
	CadPolygon polygon = MakePolygon(0, width, extent);

	__int64 duration1 = TimeFunction(&PointInPolySerially, polygon, width, extent);

	std::cout << "Point in polygon\n";
	std::cout << duration1 << "\n";
	std::cout << std::endl;
}

int _tmain(int argc, _TCHAR* argv[])
{
	Rotation();
	Multiplication();
	PointInPoly();

	return 0;
}

