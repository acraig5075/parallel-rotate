#pragma once

#include "Structures.h"
#include <amp.h>
#include <amp_short_vectors.h>


extern const bool kVerify;

void AMPRuntimeWarmup();
void RotateUsingAMP(const std::vector<CadPt3> &points, float step);
void RotateUsingAMPEx(const std::vector<CadPt3> &points, float radians);

void MultiplyUsingAMP(const std::vector<float> &a, const std::vector<float> &b); 
std::vector<float> MultiplyUsingAMPEx(const std::vector<float> &a, const std::vector<float> &b);

void PointInPolyAMP(const CadPolygon &polygon, float width, float extent);
bool PointInPolyAMPEx(const CadPt2 &pt, const CadPolygon &polygon, concurrency::array_view<const concurrency::graphics::float_4, 1> &aPolygon);

void CheckDuplicatesUsingAMP(const std::vector<CadPt2ID> &points, int gridSize);