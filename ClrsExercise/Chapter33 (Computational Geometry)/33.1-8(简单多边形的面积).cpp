#include "Utilities/polygon.hpp"

typedef polygon<int> mypolygon;

// see
// http://www.mathopenref.com/coordpolygonarea2.html
// or
// ./AreaOfSimplePolygons.html
float area_of_simple_polygon(const mypolygon& p) {
	int result = 0;
	for (int i = 0, j = (p.size() - 1); i < p.size(); ++i) {
		// 累加的是(p, q), 以及(0, p), (q, 0)组成的梯形的面积:
		// (上底 + 下底) * 高 / 2).
		// 由于y值的顺序, 梯形面积为矢量, 累加这些矢量, 可以裁剪出p表示的
		// 多边形的面积.
		result += (p[i].x + p[j].x) * (p[i].y - p[j].y);
		j = i;
	}

	return abs(result) / 2.f;
}

int main() {
	mypolygon p;
	p.resize(3);
	p[0].x = p[0].y = 0;
	p[1].x = -2;
	p[1].y = 0;
	p[2].x = p[2].y = -1;
	
	float a = area_of_simple_polygon(p);
}

