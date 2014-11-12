#pragma once

#include <vector>

extern const bool kVerify;

struct CadPt3;

bool Verify(const CadPt3 &before, float radians, const CadPt3 &after);
bool Verify(const std::vector<CadPt3> &input, float angle, const std::vector<CadPt3> &output);
