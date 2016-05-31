#include <vector>
#include <map>
#include <cstdio>

template <class Ty>
class disjoint_sets {
public:
	typedef Ty value_type;
	typedef unsigned rank_type;
	typedef std::map<value_type, value_type> ParentContainer;
	typedef std::map<value_type, rank_type> RankContainer;
	void make_set(const value_type& x) {
		setparent(x, x);
		setrank(x, 1);
	}
	value_type find_set(const value_type& x) {
		return path_compression(x);
	}
	void union_set(const value_type& x, const value_type& y) {
		link(find_set(x), find_set(y));
	}
	size_t count_sets() const {
		size_t r = 0;
		for (ParentContainer::const_iterator ite = parent.begin();
			ite != parent.end(); ++ite) {
			r += (size_t)(ite->first == ite->second);
		}

		return r;
	}
	void clear() {
		parent.clear();
		rank.clear();
	}
private:
	void link(const value_type& x, const value_type& y) {
		if (x == y) return;
		rank_type rx = getrank(x), ry = getrank(y);
		if (rx < ry)
			setparent(x, y);
		else
			setparent(y, x);

		if (rx == ry)
			setrank(x, getrank(x) + 1);
	}

	value_type path_compression(const value_type& x) {
		if (getparent(x) != x)
			setparent(x, path_compression(getparent(x)));
		return getparent(x);
	}

	void setparent(const value_type& x, const value_type y) {
		parent[x] = y;
	}

	const value_type& getparent(const value_type& x) {
		return parent[x];
	}

	void setrank(const value_type& x, rank_type r) {
		rank[x] = r;
	}

	rank_type getrank(const value_type& x) {
		return rank[x];
	}

	ParentContainer parent;
	RankContainer rank;
};

template <class Vertex>
class TarjansOfflineLeastCommonAncestors {
	typedef disjoint_sets<Vertex> DisjointSets;
	DisjointSets sets;
	typedef std::map<Vertex, Vertex> AncestorContainer;
	AncestorContainer ancestors;
	typedef std::map<Vertex, bool> ColorContainer;
	ColorContainer colors;
	static const bool white = false;
	static const bool black = true;
	void __output_ancestors(Vertex from, Vertex const u, Vertex const v) {
		sets.make_set(from);
		ancestors[sets.find_set(from)] = from;
		VertexContainer child = children(from);
		for (VertexContainer::iterator ite = child.begin(); ite != child.end(); ++ite) {
			__output_ancestors(*ite, u, v);
			sets.union_set(from, *ite);
			ancestors[sets.find_set(from)] = from;
		}
		colors[from] = black;
		if (u == from && colors[v] == black) {
			printf("The least common ancestor of %d and %d is %d.\n",
				query_dat(u), query_dat(v), query_dat(ancestors[sets.find_set(v)]));
		}
		else if (v == from && colors[u] == black) {
			printf("The least common ancestor of %d and %d is %d.\n",
				query_dat(v), query_dat(u), query_dat(ancestors[sets.find_set(u)]));
		}
	}
	void reset() {
		ancestors.clear();
		colors.clear();
		sets.clear();
	}
public:
	typedef std::vector<Vertex> VertexContainer;
	void output_ancestors(Vertex from, Vertex const u, Vertex const v) {
		reset();
		__output_ancestors(from, u, v);
	}
};

struct Node {
	int dat;
	Node* lchild;
	Node* rchild;
	Node() : dat(-1), lchild(NULL), rchild(NULL) { }
};

typedef TarjansOfflineLeastCommonAncestors<Node*> Algo;

Algo::VertexContainer children(Node* p) {
	Algo::VertexContainer cont;
	if (p != NULL) {
		if (p->lchild != NULL)
			cont.push_back(p->lchild);
		if (p->rchild != NULL)
			cont.push_back(p->rchild);
	}

	return cont;
}

int query_dat(Node* p) {
	return p->dat;
}

int main() {
	Node* nodes[5];
	for (int i = 0; i < 5; ++i) {
		nodes[i] = new Node;
		nodes[i]->dat = i;
	}
	/*
	      0
	  1       3
    2   4
	*/
	nodes[0]->lchild = nodes[1];
	nodes[0]->rchild = nodes[3];
	nodes[1]->lchild = nodes[2];
	nodes[1]->rchild = nodes[4];

	TarjansOfflineLeastCommonAncestors<Node*> obj;
	obj.output_ancestors(nodes[0], nodes[2], nodes[4]);

	obj.output_ancestors(nodes[0], nodes[2], nodes[1]);

	obj.output_ancestors(nodes[0], nodes[3], nodes[4]);

	return 0;
}
