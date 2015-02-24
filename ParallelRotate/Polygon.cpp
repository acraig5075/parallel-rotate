#include "Polygon.h"
#include <iostream>

CadPolygon MakePolygon(float width)
{
	CadPt2 p0 = { width, width };
	CadPt2 p1 = p0 + CadPt2(80.f, 0.f);
	CadPt2 p2 = p1 + CadPt2(0.f, 30.f);
	CadPt2 p3 = p2 + CadPt2(-70.f, 0.f);
	CadPt2 p4 = p3 + CadPt2(0.f, 10.f);
	CadPt2 p5 = p4 + CadPt2(70.f, 0.f);
	CadPt2 p6 = p5 + CadPt2(0.f, 30.f);
	CadPt2 p7 = p6 + CadPt2(-70.f, 0.f);
	CadPt2 p8 = p7 + CadPt2(0.f, 10.f);
	CadPt2 p9 = p8 + CadPt2(70.f, 0.f);
	CadPt2 p10 = p9 + CadPt2(0.f, 10.f);
	CadPt2 p11 = p10 + CadPt2(-80.f, 0.f);
	CadPt2 p12 = p11 + CadPt2(0.f, -30.f);
	CadPt2 p13 = p12 + CadPt2(70.f, 0.f);
	CadPt2 p14 = p13 + CadPt2(0.f, -10.f);
	CadPt2 p15 = p14 + CadPt2(-70.f, 0.f);
	CadPt2 p16 = p15 + CadPt2(0.f, -30.f);
	CadPt2 p17 = p16 + CadPt2(70.f, 0.f);
	CadPt2 p18 = p17 + CadPt2(0.f, -10.f);
	CadPt2 p19 = p18 + CadPt2(-70.f, 0.f);
	CadPt2 p20 = p0;

	CadPolygon ret;
	ret.push_back(std::make_pair(p0, p1));
	ret.push_back(std::make_pair(p1, p2));
	ret.push_back(std::make_pair(p2, p3));
	ret.push_back(std::make_pair(p3, p4));
	ret.push_back(std::make_pair(p4, p5));
	ret.push_back(std::make_pair(p5, p6));
	ret.push_back(std::make_pair(p6, p7));
	ret.push_back(std::make_pair(p7, p8));
	ret.push_back(std::make_pair(p8, p9));
	ret.push_back(std::make_pair(p9, p10));
	ret.push_back(std::make_pair(p10, p11));
	ret.push_back(std::make_pair(p11, p12));
	ret.push_back(std::make_pair(p12, p13));
	ret.push_back(std::make_pair(p13, p14));
	ret.push_back(std::make_pair(p14, p15));
	ret.push_back(std::make_pair(p15, p16));
	ret.push_back(std::make_pair(p16, p17));
	ret.push_back(std::make_pair(p17, p18));
	ret.push_back(std::make_pair(p18, p19));
	ret.push_back(std::make_pair(p19, p20));
	return ret;
}
