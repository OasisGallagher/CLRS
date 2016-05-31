// depth-determination problem.
#include <map>
#include <cassert>

// ���ʹ��actuallydistance, ����d�м�¼ÿ���ڵ��ʵ�����, ��Ҫ��find_depth
// �޸�Ϊpath_compression֮��, ֱ�ӷ���d[v].
// path_compressionҲ��Ҫ�����޸�, ��Ϊ֮ǰ��û�н�root��dֵ�������ڵ�, ������
// find_depthʱ���������d[r], �޸�Ϊ����xΪrootʱ, ������ѹ��, ���x�ĸ��ڵ���root,
// ��Ȼ��x����ѹ��.
// �������������, ��������һ��ѹ��ʱ, ���д���һ���������ɸ��ӽڵ�, �ͻ��ظ��ۼ�dֵ
// �Ĺ���.
// ��ʹ��pseudodistance, �Ϳ��Ա���, ��Ϊ�����н�����һ���������ɸ��ø����ӽڵ�
// ʱ, ������������κβ���, ����ֱ�ӷ���x�ĸ��ڵ�.
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
		// ���x��root, ������ѹ��.
		// ���x��root�ĺ���, ��Ȼ������ѹ��. ѹ���Ĺ�����, ��root�ڵ�ᶼ��Ϊroot��
		// �ӽڵ�, ����, ��dists�ᱻ�ۼ�. ��������, dist[x]+dist[root]��Ϊx��depth.
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
		// ��root�ڵ�, ��Ȼ��root���ӽڵ�, ��ô, �ۼ�root�͸ýڵ��dist, �õ��ľ���
		// �ýڵ��depth.
		return dists[x] + dists[parents[x]];
	}

	// See Problems 21-2: Depth determination:
	// "Note that the root of a set Si is not necessarily the
	//  root of the corresponding tree Ti".
	// TΪʵ�ʲ���ʱ, �����γɵ���. ����ִ��graft(x, y), �ǽ�x����y��, ��Ϊy��������һ������.
	// �����㷨��, ����y��rank�Ƚϵ�, ����y����x��(���ݽṹ�ϵ����ڼ���S��).
	void graft(const Element& r, const Element& v) {
		// r is assumed to be the root of a tree in T, instead of S.
		Element r2 = find_root(r), v2 = find_root(v);
		int dv = find_depth(v);
		if(ranks[r2] < ranks[v2]) {
			// ��r����v���ӽڵ�. ��ѹ��·��, ʹrΪv2���ӽڵ�.
			parents[r2] = v2;
			// ��Ҫע�����, ���ڵ��dist��һ��Ϊ0.
			// ���, ��r��Ϊv2���ӽڵ�ʱ, ��Ҫ����dist��ȥv2��dist, ����, ��
			// find_path����֮ǰ����rΪ�������еĽڵ�ʱ, ������ȷ����.
			dists[r2] += dv + 1 - dists[v2];
		}
		else {
			// graft��Ŀ���ǽ�r����v���ӽڵ�. ����, v���ڵ���v2��rank, ��
			// r2��Ҫ��, ��Ҫ��v2����r2������.
			parents[v2] = r2;
			// ��Ȼ���ݽṹ��, v2��r2������, ����, ʵ�ʸ߶���, r2ȷʵv������. 
			// ����graft�Ĺ���.
			// ����֮ǰ��r2Ϊ����������depth������dv+1.
			dists[r2] += dv + 1;
			// ��r2��Ϊv2���ӽڵ�֮��, ����Ϊ����������depthӦ���ǲ����, ����
			// v2��dist��ȥr2��dist, �Ա�֤�ڼ���v2Ϊ����������depthʱ, �����ȷ.
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
