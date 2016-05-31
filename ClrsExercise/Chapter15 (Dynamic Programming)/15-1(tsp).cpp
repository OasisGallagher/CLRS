#include <map>
#include <vector>
#include <iostream>
#include <algorithm>
#include <limits>

template <class value_type>
class table {
	typedef std::pair<unsigned, unsigned> key_type;
	typedef std::map<key_type, value_type> container;
	container cont;
public:
	value_type& operator()(key_type::first_type key1, key_type::second_type key2) {
		return cont[std::make_pair(key1, key2)];
	}
};

struct point {
	typedef unsigned coordinate_type;
	point(coordinate_type cx, coordinate_type cy) : x(cx), y(cy) {

	}
	coordinate_type x;
	coordinate_type y;
	bool operator < (const point& other) const {
		return x < other.x;
	}
};

typedef std::vector<point> point_container;

float distance(const point& x, const point& y) {
	unsigned cx = y.x - x.x;
	unsigned cy = y.y - x.y;
	return sqrtf(float(cx * cx + cy * cy));
}

float bitonic_tsp(point_container& cont, table<unsigned>* processer) {
	// sort the points by their x coordinate.
	std::sort(cont.begin(), cont.end());

	// start at p(i), goes strictly left to p(1) and then goes strictly right to p(j).
	// b(i, j) is the minimum length squared.
	table<float> b;
	// immediate processer.
	table<unsigned> ip;

	// 1的下一个节点一定是2.
	b(1, 2) = distance(cont[1], cont[2]);

	const point_container::size_type n = cont.size() - 1;

	// 无论哪种情况, 在j的直接前驱x处, 直接按照p(x)->p(j)就是二者的最短距离(线段最短).
	for (unsigned j = 3; j <= n; ++j) {
		// case 1: p(j)的直接前驱是p(j-1).
		// 如果在p(j-1)向右走, 那么, 起点一定在j-1左边的点, 否则, 比如索引为j-1的点
		// 它一定是向左的.
		// 这种情况下, 从起点到1, 再从1到j-1, 再从j-1直接到j即可.
		for (unsigned i = 1; i < j - 1; ++i) {
			b(i, j) = b(i, j - 1) + distance(cont[j - 1], cont[j]);
			ip(i, j) = j - 1;
		}

		// case 2: p(j)的直接前驱是j-1之前的一个点, k.
		// 如果在p(j-1)向左走, 那么起点一定就是j-1.
		// 此时的p(j)一定有一个索引小于j-1的直接前驱, 那么仿照第一种情况, 
		// 从这个先从j-1到这个前驱, 再从这个前驱直接到p(j)即可.
		// 由于table的两个key, 按照递增的顺序存储, 所以记为b(k, j-1).
		b(j - 1, j) = std::numeric_limits<float>::max();
		// p(k) is an immediate predecessor of p(j).
		for (unsigned k = 1; k < j - 1; ++k) {
			float t = b(k, j - 1) + distance(cont[k], cont[j]);
			if (t < b(j - 1, j)) {
				b(j - 1, j) = t;
				ip(j - 1, j) = k;
			}
		}
	}
	*processer = ip;
	return (b(n, n) = b(n - 1, n) + distance(cont[n - 1], cont[n]));
}

void print_point(unsigned index) {
	std::cout << "P" << index << std::endl;
}

void solution(table<unsigned>& ip, unsigned from, unsigned to) {
	// case 1:
	// -------------------(to)
	// |
	// -----------(from)
	// from < to, k = ip(from, to)是to的前驱, 由于是从n到n-1的顺序输出, 那么, 要先输出
	// k再输出ip(from, k).
	if (from < to) {
		unsigned k = ip(from, to);
		print_point(k);
		if(k > 1)
			solution(ip, from, k);
	}
	// case 2:
	// -------(to)
	// |
	// -------------(from)
	// from >= to, k = ip(to, from)是from的前驱, 由于是从n到n-1的顺序输出, 那么, 要先输出
	// ip(from, k)再输出k.
	else {
		unsigned k = ip(to, from);
		if (k > 1) {
			solution(ip, k, to);
			print_point(k);
		}
	}
}

void solution(table<unsigned>& ip, unsigned n) {
	// n的前一个节点必然是n-1.
	print_point(n);
	print_point(n - 1);
	unsigned k = ip(n - 1, n);
	solution(ip, k, n - 1);
	print_point(k);
}

int main() {
	unsigned points[] = { 0, 6, 1, 0, 2, 3, 5, 4, 6, 1, 7, 5, 8, 2 };

	point_container cont;
	// place holder.
	cont.push_back(point(0, 0));
	for (unsigned i = 0; i < _countof(points); i += 2)
		cont.push_back(point(points[i], points[i + 1]));

	table<unsigned> ip;
	std::cout << bitonic_tsp(cont, &ip) << std::endl;

	solution(ip, cont.size() - 1);
}
