#pragma once

#include "Structures.h"

extern const bool kVerify;

void RotateUsingOMP(const std::vector<CadPt3> &points, float step);
void RotateUsingOMPEx(const std::vector<CadPt3> &points, float radians);

void MultiplyUsingOMP(const std::vector<float> &a, const std::vector<float> &b); 
std::vector<float> MultiplyUsingOMPEx(const std::vector<float> &a, const std::vector<float> &b);