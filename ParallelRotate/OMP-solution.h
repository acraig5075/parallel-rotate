#pragma once

#include <vector>

extern const bool kVerify;

struct CadPt3;

void RotateUsingOMP(const std::vector<CadPt3> &points, float step);
void RotateUsingOMPEx(const std::vector<CadPt3> &points, float radians);