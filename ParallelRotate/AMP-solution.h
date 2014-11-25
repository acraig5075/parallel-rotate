#pragma once

#include "Structures.h"

extern const bool kVerify;

void AMPRuntimeWarmup();
void RotateUsingAMP(const std::vector<CadPt3> &points, float step);
void RotateUsingAMPEx(const std::vector<CadPt3> &points, float radians);

void MultiplyUsingAMP(const std::vector<float> &a, const std::vector<float> &b); 
std::vector<float> MultiplyUsingAMPEx(const std::vector<float> &a, const std::vector<float> &b);