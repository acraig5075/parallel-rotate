#pragma once

#include "Structures.h"
#include "Settings.h"

extern Settings settings;

void RotateUsingOMP(const std::vector<CadPt3> &points, float step);
void RotateUsingOMPEx(const std::vector<CadPt3> &points, float radians);

void MultiplyUsingOMP(const std::vector<float> &a, const std::vector<float> &b); 
std::vector<float> MultiplyUsingOMPEx(const std::vector<float> &a, const std::vector<float> &b);

void PointInPolyOMP(const CadPolygon &polygon, float width, float extent);
bool PointInPolyOMPEx(const CadPt2 &pt, const CadPolygon &polygon);

void CheckDuplicatesUsingOMP(const std::vector<CadPt2ID> &points, int gridSize);