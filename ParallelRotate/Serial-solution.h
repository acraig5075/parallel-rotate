#pragma once

#include "Structures.h"
#include "Settings.h"

extern Settings settings;

void RotateSerially(const std::vector<CadPt3> &points, float step);
void RotateSeriallyEx(const std::vector<CadPt3> &points, float radians);

void MultiplySerially(const std::vector<float> &a, const std::vector<float> &b); 
std::vector<float> MultiplySeriallyEx(const std::vector<float> &a, const std::vector<float> &b);

void PointInPolySerially(const CadPolygon &polygon, float width, float extent);
bool PointInPolySeriallyEx(const CadPt2 &pt, const CadPolygon &polygon);

void CheckDuplicatesSerially(const std::vector<CadPt2ID> &points, int gridSize);