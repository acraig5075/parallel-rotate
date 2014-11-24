#pragma once

#include <vector>

extern const bool kVerify;

struct CadPt2;
struct CadPt3;

bool Verify(const CadPt3 &before, float radians, const CadPt3 &after);
bool Verify(const std::vector<CadPt3> &input, float angle, const std::vector<CadPt3> &output);
bool Verify(const std::vector<float> &matrix);
bool Verify(const CadPt2 &pt, float width, float extent, bool inside);
