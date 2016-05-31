#include "graph.hpp"
#include <map>
#include <iostream>

typedef unsigned Color;

enum { Undiscovered, Finished };

typedef std::vector<size_t> Sequence;

class Table {
	typedef size_t EdgeBegin;
	typedef size_t EdgeEnd;
	typedef std::pair<EdgeBegin, EdgeEnd> Edge;
	typedef std::map<Edge, Color> Container;
	typedef size_t Vertex;
	Container cont;
public:
	Color& operator()(EdgeBegin beg, EdgeEnd end) {
		return cont[std::make_pair(beg, end)];
	}
	Color& operator()(Vertex x) {
		return cont[std::make_pair(x, x)];
	}
};

bool dfs_visit(size_t vertex, Sequence& seq, adj_table& adj, Table& edgeColor) {
	adj_table::Iterators ites = adj(vertex);
	
	if(seq.size() == adj.count_edges() + 1)
		return true;

	for(adj_table::NodeIterator it = ites.first; it != ites.second; ++it) {
		if(edgeColor(vertex, it->connected) == Undiscovered) {
			seq.push_back(it->connected);
			edgeColor(vertex, it->connected) = Finished;
			if(dfs_visit(it->connected, seq, adj, edgeColor))
				return true;
			edgeColor(vertex, it->connected) = Undiscovered;
			seq.pop_back();
		}
	}

	return false;
}

void dfs(adj_table& adj) {
	Sequence visitSequence(1, 0);
	Table edgeColor;

	if(dfs_visit(0, visitSequence, adj, edgeColor)) {
		visitSequence.pop_back();
		std::copy(visitSequence.begin(), visitSequence.end(),
			std::ostream_iterator<unsigned>(std::cout, " -> "));

		std::cout << "0\n";
	}
}

adj_table prepare_test_case() {
	adj_table adj(6);
	adj.insert(0, 2, 1, 3);
	adj.insert(1, 3, 0, 3, 4);
	adj.insert(2, 2, 4, 5);
	adj.insert(3, 3, 0, 1, 4);
	adj.insert(4, 3, 1, 2, 3);
	adj.insert(5, 1, 2);
	return adj;
}

int main() {
	adj_table adj = prepare_test_case();
	dfs(adj);
}

// 上述方法可以遍历无向图G中的所有边两次(u->v和v->u).
// 如果在迷宫之中, 可以按照上述的路径走完迷宫的所有点, 从而找到出口.
