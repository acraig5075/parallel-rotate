#pragma once

#include <vector>

extern const bool kVerify;

struct CadPt3;

void AMPRuntimeWarmup();
void RotateUsingAMP(const std::vector<CadPt3> &points, float step);
void RotateUsingAMPEx(const std::vector<CadPt3> &points, float radians);