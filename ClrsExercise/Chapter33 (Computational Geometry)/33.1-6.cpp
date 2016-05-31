#include "Utilities/segment.hpp"
typedef vector2<int> point;
typedef segment<int> line_segment;
bool cross_horizontal_ray(point r, line_segment ls) {
	point ray(r.x + 1, r.y);
	ls.left -= r, ls.right -= r, ray -= r;

	float f1 = ray.cross(ls.left);
	float f2 = ray.cross(ls.right);

	// 线段端点在射线两侧.
	if (f1 * f2 < 0)
		return std::max(ls.left.x, ls.right.x) >= 0;
	else if (f1 * f2 == 0)	// 端点在射线上.
		return ls.left.x >= 0 || ls.right.x >= 0;

	return false;
}

