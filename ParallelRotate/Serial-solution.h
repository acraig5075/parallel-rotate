#pragma once

#include <vector>

extern const bool kVerify;

struct CadPt3;

void RotateSerially(const std::vector<CadPt3> &points, float step);
void RotateSeriallyEx(const std::vector<CadPt3> &points, float radians);

void MultiplySerially(const std::vector<float> &a, const std::vector<float> &b); 
std::vector<float> MultiplySeriallyEx(const std::vector<float> &a, const std::vector<float> &b);