#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include "Utilities/vector2.hpp"
typedef vector2<int> point;
typedef std::vector<point> array;

#pragma region(SortPolar)
bool less_than(const point& lhs, const point& rhs) {
	bool b1 = lhs.cross(point(1, 0)) > 0;
	bool b2 = rhs.cross(point(1, 0)) > 0;

	// ���һ������1/2����, ��һ����3/4����.
	if (b1 != b2)
		return b2;

	// ͬ��1/2����2/4����.
	return lhs.cross(rhs) > 0;
}

int partition(array& buffer, int i, int j) {
	array::value_type& last = buffer[j - 1];
	int pos = i - 1;
	for (; i < j - 1; ++i) {
		if (less_than(buffer[i], last))
			std::swap(buffer[++pos], buffer[i]);
	}

	std::swap(buffer[++pos], last);
	return pos;
}

void sort_polar(array& buffer, int first, int last) {
	if (first < last) {
		int p = partition(buffer, first, last);
		sort_polar(buffer, first, p);
		sort_polar(buffer, p + 1, last);
	}
}

// ���ռ��ǽ�������(O(nlog(n)).
void sort_polar(array& buffer, point origin) {
	std::transform(buffer.begin(), buffer.end(), buffer.begin(), std::bind2nd(std::minus<point>(), origin));
	sort_polar(buffer, 0, buffer.size());
}
#pragma endregion(SortPolar)

bool collinear(const point& lhs, const point& rhs) {
	return lhs.cross(rhs) == 0;
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

	// ���p, q, r���㹲��, ��ô����(q - p)��(r - p)ƽ��.

	// O(n)
	for (array::iterator ite = points.begin(); ite != points.end(); ++ite) {
		// O(n*log(n)).
		point pivot = *ite;
		sort_polar(points, pivot);

		array::iterator pos = std::adjacent_find(points.begin(), points.end(), collinear);
		std::transform(points.begin(), points.end(), points.begin(), std::bind2nd(std::plus<point>(), pivot));

		if (pos != points.end()) {
			printf("%s %s %s", pivot.tostring().c_str(), pos->tostring().c_str(), (pos + 1)->tostring().c_str());
			break;
		}
	} // O(n*n*log(n)).
}
