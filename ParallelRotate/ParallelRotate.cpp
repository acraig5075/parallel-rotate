// ParallelRotate.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Structures.h"
#include "Serial-solution.h" // not parallel
#include "PPL-solution.h"    // Parallel Patterns Library
#include "OMP-solution.h"    // OpenMP
#include "AMP-solution.h"    // Accelerated Massive Parallelism

#include <vector>
#include <functional>
#include <chrono>
#include <iostream>


const int kSize = 10000;
const bool kVerify = false;
const float kStep = 1.f;


__int64 TimeFunction(const std::function<void(const std::vector<CadPt3> &, float)> &func, const std::vector<CadPt3> &p)
{
	auto start = std::chrono::high_resolution_clock::now();

	func(p, kStep);

	auto end = std::chrono::high_resolution_clock::now();

	return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

int _tmain(int argc, _TCHAR* argv[])
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

	AMPRuntimeWarmup();

	__int64 duration1 = TimeFunction(&RotateSerially, points);
	__int64 duration2 = TimeFunction(&RotateUsingPPL, points);
	__int64 duration3 = TimeFunction(&RotateUsingOMP, points);
	__int64 duration4 = TimeFunction(&RotateUsingAMP, points);

	std::cout << duration1 << "\n";
	std::cout << duration2 << "\n";
	std::cout << duration3 << "\n";
	std::cout << duration4 << "\n";
	return 0;
}

