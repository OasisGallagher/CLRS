#include "Algorithm/Exercise/ClrsExercise/Chapter13 (Red-Black Trees)/redblacktree.hpp"
#include "Utilities/vector2.hpp"
#include "Utilities/segment.hpp"
#include <vector>
#include <algorithm>
#include <cassert>

typedef vector2<float> point;

struct point_wrapper {
	bool left;
	size_t seg_index;
	point p;
};

typedef std::vector<point_wrapper> point_container;
typedef segment<float> mysegment;
typedef std::vector<mysegment> segment_container;

struct point_sorter : public std::binary_function<point_wrapper, point_wrapper, bool> {
	bool operator()(const point_wrapper& lhs, const point_wrapper& rhs) const {
		if (lhs.p.x != rhs.p.x)
			return lhs.p.x < rhs.p.x;

		if (lhs.left != rhs.left)
			return lhs.left;

		return lhs.p.y < rhs.p.y;
	}
};

void split(point_container& pc, const segment_container& c) {
	for (segment_container::size_type i = 0; i < c.size(); ++i) {
		point_wrapper p;
		p.left = true;
		p.p = c[i].left;
		p.seg_index = i;
		pc.push_back(p);

		p.left = false;
		p.p = c[i].right;
		p.seg_index = i;
		pc.push_back(p);
	}

	std::sort(pc.begin(), pc.end(), point_sorter());
}

struct segment_sorter : public std::binary_function<mysegment, mysegment, bool> {
	bool operator()(const mysegment& lhs, const mysegment& rhs) const {
		// 如果二者比较, 那么, 二者必然都在树中或者其中之一即将被加入到树中.
		// 且在二者中最大的x值处, 二者可比较.
		float max_x = std::max(lhs.left.x, lhs.left.x);
		return order_at(lhs, rhs, max_x);
	}

	bool order_at(const mysegment& lhs, const mysegment& rhs, float x) const {
		point p1 = at(lhs, x), p2 = at(rhs, x);
		return p1.cross(p2) > 0.f;
	}

	point at(const mysegment& seg, float x) const {
		assert(x >= std::min(seg.left.x, seg.right.x) || x <= std::max(seg.left.x, seg.right.x));
		point result;
		result.x = x;
		result.y = (x - seg.left.x) / (seg.right.x - seg.left.x) * (seg.right.y - seg.left.y) + seg.left.y;
		return result;
	}
};

bool intersect(const mysegment& lhs, const mysegment& rhs) {
	return false;
}

// 当监测到线段A和B相交时, 二者必然都在树中, 且相邻.
bool intersect(const segment_container& c) {
	point_container pc;
	split(pc, c);

	// mapped_type这里没有用到.
	rb_tree<mysegment, int, segment_sorter> tree;

	for (point_container::size_type i = 0; i < pc.size(); ++i) {
		point_wrapper& curpoint = pc[i];
		const segment_container::value_type& seg = c[curpoint.seg_index];

		if (curpoint.left) {
			mysegment other;
			tree.insert(seg, i);
			if (tree.predecessor(seg, &other) && intersect(seg, other))
				return true;

			if (tree.successor(seg, &other) && intersect(seg, other))
				return true;
		}
		else {
			mysegment above, below;
			if (tree.predecessor(seg, &above) && tree.successor(seg, &below) && intersect(above, below))
				return true;
			tree.erase(seg);
		}
	}

	return false;
}

int main() {
	segment_container c;
	bool b = intersect(c);
}
