// depth-determination problem.
#include <map>
#include <cassert>

// 如果使用actuallydistance, 即在d中记录每个节点的实际深度, 需要将find_depth
// 修改为path_compression之后, 直接返回d[v].
// path_compression也需要进行修改, 因为之前是没有将root的d值计算在内的, 而是在
// find_depth时在另行添加d[r], 修改为仅当x为root时, 不进行压缩, 如果x的父节点是root,
// 依然对x进行压缩.
// 这样会出现问题, 当继续下一次压缩时, 树中存在一个根和若干个子节点, 就会重复累加d值
// 的过程.
// 而使用pseudodistance, 就可以避免, 因为当树中仅存在一个根和若干个该根的子节点
// 时, 函数不会进行任何操作, 而是直接返回x的父节点.
//

template <class Element>
class DepthDetermination {
	// for simplicity.
	typedef std::map<Element, Element> ParentContainer;
	typedef std::map<Element, unsigned> RankContainer;
	typedef std::map<Element, int> PseudoDistanceContainer;
	ParentContainer parents;
	RankContainer ranks;
	PseudoDistanceContainer dists;

	//
	Element path_compression(const Element& x) {
		Element pa = parents[x];
		// 如果x是root, 不进行压缩.
		// 如果x是root的孩子, 依然不进行压缩. 压缩的过程中, 非root节点会都变为root的
		// 子节点, 并且, 其dists会被累加. 操作过后, dist[x]+dist[root]即为x的depth.
		if(pa != parents[pa]) {
			parents[x] = path_compression(pa);
			dists[x] += dists[pa];
		}

		return parents[x];
	}

	Element find_root(const Element& x) {
		return path_compression(x);
	}
public:
	DepthDetermination& make_set(const Element& x) {
		assert(parents.find(x) == parents.end());
		parents[x] = x;
		ranks[x] = 0;
		dists[x] = 0;
		return *this;
	}

	int find_depth(const Element& x) {
		path_compression(x);
		if(parents[x] == x)
			return dists[x];
		// 非root节点, 必然是root的子节点, 那么, 累加root和该节点的dist, 得到的就是
		// 该节点的depth.
		return dists[x] + dists[parents[x]];
	}

	// See Problems 21-2: Depth determination:
	// "Note that the root of a set Si is not necessarily the
	//  root of the corresponding tree Ti".
	// T为实际操作时, 期望形成的树. 比如执行graft(x, y), 是将x附到y上, 作为y所在树的一个子树.
	// 而在算法中, 可能y的rank比较低, 而将y附到x上(数据结构上的树在集合S中).
	void graft(const Element& r, const Element& v) {
		// r is assumed to be the root of a tree in T, instead of S.
		Element r2 = find_root(r), v2 = find_root(v);
		int dv = find_depth(v);
		if(ranks[r2] < ranks[v2]) {
			// 将r附做v的子节点. 并压缩路径, 使r为v2的子节点.
			parents[r2] = v2;
			// 需要注意的是, 根节点的dist不一定为0.
			// 因此, 当r作为v2的子节点时, 需要将其dist减去v2的dist, 这样, 在
			// find_path查找之前的以r为根的树中的节点时, 才能正确处理.
			dists[r2] += dv + 1 - dists[v2];
		}
		else {
			// graft的目的是将r附做v的子节点. 但是, v所在的树v2的rank, 比
			// r2还要少, 需要将v2附做r2的子树.
			parents[v2] = r2;
			// 虽然数据结构上, v2是r2的子树, 但是, 实际高度上, r2确实v的子树. 
			// 这是graft的功能.
			// 所有之前的r2为根的子树的depth都增加dv+1.
			dists[r2] += dv + 1;
			// 当r2作为v2的子节点之后, 以它为根的子树的depth应该是不变的, 所以
			// v2的dist减去r2的dist, 以保证在计算v2为根的子树的depth时, 结果正确.
			dists[v2] -= dists[r2];

			if(ranks[r2] == ranks[v2])
				++ranks[r2];
		}
	}
};

#include <iostream>

#define EXPECT_EQ(expr, x)	expect_eq(__LINE__, expr, x)
void expect_eq(unsigned line, int x, int y) {
	if(x != y) {
		printf("line: %u test failed (%d == %d)\n", line, x, y);
		abort();
	}
}

int main() {
	DepthDetermination<int> d;
	d.make_set(0).make_set(1);
	EXPECT_EQ(d.find_depth(0), 0);
	EXPECT_EQ(d.find_depth(1), 0);

	d.graft(0, 1);
	EXPECT_EQ(d.find_depth(1), 0);
	EXPECT_EQ(d.find_depth(0), 1);

	d.make_set(2).make_set(3).make_set(4);

	d.graft(2, 3);
	EXPECT_EQ(d.find_depth(3), 0);
	EXPECT_EQ(d.find_depth(2), 1);

	d.graft(3, 4);
	EXPECT_EQ(d.find_depth(4), 0);
	EXPECT_EQ(d.find_depth(3), 1);
	EXPECT_EQ(d.find_depth(2), 2);

	d.graft(1, 3);
	EXPECT_EQ(d.find_depth(1), 2);
	EXPECT_EQ(d.find_depth(0), 3);

	return 0;
}
