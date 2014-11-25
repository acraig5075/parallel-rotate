#pragma once

#include "Structures.h"

extern const bool kVerify;

void RotateUsingPPL(const std::vector<CadPt3> &points, float step);
void RotateUsingPPLEx(const std::vector<CadPt3> &points, float radians);

void MultiplyUsingPPL(const std::vector<float> &a, const std::vector<float> &b); 
std::vector<float> MultiplyUsingPPLEx(const std::vector<float> &a, const std::vector<float> &b);

void PointInPolyPPL(const CadPolygon &polygon, float width, float extent);
bool PointInPolyPPLEx(const CadPt2 &pt, const CadPolygon &polygon);
