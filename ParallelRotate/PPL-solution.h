#pragma once

#include <vector>

extern const bool kVerify;

struct CadPt3;

void RotateUsingPPL(const std::vector<CadPt3> &points, float step);
void RotateUsingPPLEx(const std::vector<CadPt3> &points, float radians);

void MultiplyUsingPPL(const std::vector<float> &a, const std::vector<float> &b); 
std::vector<float> MultiplyUsingPPLEx(const std::vector<float> &a, const std::vector<float> &b);