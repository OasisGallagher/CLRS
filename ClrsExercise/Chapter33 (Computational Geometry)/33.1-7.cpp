#include <iostream>
#include <vector>
#include "Utilities/polygon.hpp"

typedef vector2<int> point;
typedef std::vector<point> array;
typedef polygon<int> mypolygon;
typedef segment<int> line_segment;

bool cross_horizontal_ray(point r, line_segment ls);

// ��p0����ƽ����x�������(���ⷽ������߶�����), ������p�ཻ�ߵĸ���Ϊ����.
bool contains(const mypolygon& p, const point& p0) {
	mypolygon::boundary_container b = p.boundaries();
	int count = 0;
	for (mypolygon::boundary_container::iterator ite = b.begin(); ite != b.end(); ++ite) {
		if (cross_horizontal_ray(p0, *ite))
			++count;
	}

	return (count & 1) != 0;
}

int main() {
	array points;	
	points.push_back(point(2, 1));
	points.push_back(point(2, 2));
	points.push_back(point(-1, 1));
	points.push_back(point(1, -1));
	points.push_back(point(3, 3));
	points.push_back(point(-1, -1));
	points.push_back(point(1, 1));

	mypolygon p(points.begin(), points.end());
}
