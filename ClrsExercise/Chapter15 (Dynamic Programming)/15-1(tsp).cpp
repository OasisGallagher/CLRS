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

	// 1����һ���ڵ�һ����2.
	b(1, 2) = distance(cont[1], cont[2]);

	const point_container::size_type n = cont.size() - 1;

	// �����������, ��j��ֱ��ǰ��x��, ֱ�Ӱ���p(x)->p(j)���Ƕ��ߵ���̾���(�߶����).
	for (unsigned j = 3; j <= n; ++j) {
		// case 1: p(j)��ֱ��ǰ����p(j-1).
		// �����p(j-1)������, ��ô, ���һ����j-1��ߵĵ�, ����, ��������Ϊj-1�ĵ�
		// ��һ���������.
		// ���������, ����㵽1, �ٴ�1��j-1, �ٴ�j-1ֱ�ӵ�j����.
		for (unsigned i = 1; i < j - 1; ++i) {
			b(i, j) = b(i, j - 1) + distance(cont[j - 1], cont[j]);
			ip(i, j) = j - 1;
		}

		// case 2: p(j)��ֱ��ǰ����j-1֮ǰ��һ����, k.
		// �����p(j-1)������, ��ô���һ������j-1.
		// ��ʱ��p(j)һ����һ������С��j-1��ֱ��ǰ��, ��ô���յ�һ�����, 
		// ������ȴ�j-1�����ǰ��, �ٴ����ǰ��ֱ�ӵ�p(j)����.
		// ����table������key, ���յ�����˳��洢, ���Լ�Ϊb(k, j-1).
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
	// from < to, k = ip(from, to)��to��ǰ��, �����Ǵ�n��n-1��˳�����, ��ô, Ҫ�����
	// k�����ip(from, k).
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
	// from >= to, k = ip(to, from)��from��ǰ��, �����Ǵ�n��n-1��˳�����, ��ô, Ҫ�����
	// ip(from, k)�����k.
	else {
		unsigned k = ip(to, from);
		if (k > 1) {
			solution(ip, k, to);
			print_point(k);
		}
	}
}

void solution(table<unsigned>& ip, unsigned n) {
	// n��ǰһ���ڵ��Ȼ��n-1.
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
