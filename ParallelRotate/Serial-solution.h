#pragma once

#include <vector>

extern const bool kVerify;

struct CadPt3;

void RotateSerially(const std::vector<CadPt3> &points, float step);
void RotateSeriallyEx(const std::vector<CadPt3> &points, float radians);