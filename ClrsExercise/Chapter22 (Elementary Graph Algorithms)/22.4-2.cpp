#include "Algorithm/Exercise/ClrsExercise/Chapter22 (Elementary Graph Algorithms)/graph.hpp"
#include <cstdio>
#include <cassert>

adj_table prepare_test_case() {
	adj_table adj(2);
	adj.insert(0, 1, 1);
// 	adj.insert(0, 2, 1, 3);
// 	adj.insert(1, 1, 4);
// 	adj.insert(2, 2, 4, 5);
// 	adj.insert(3, 1, 1);
// 	adj.insert(4, 1, 3);
// 	adj.insert(5, 1, 5);
	return adj;
}

void dbg_time(adj_table::TimeContainer& discovered, adj_table::TimeContainer& finished) {
	for (size_t i = 0; i < discovered.size(); ++i) {
		printf("node (%u): <%u, %u>.\n",i,  discovered[i], finished[i]);
	}
}

size_t count_paths(const adj_table& adj, const adj_table::TopologicalOrderContainer& order, size_t from, size_t to) {
	assert(from < to);

	typedef adj_table::TopologicalOrderContainer Container;
	Container::const_iterator first = order.begin();
	std::advance(first, from);

	Container::const_iterator last = order.begin();
	std::advance(last, to);

	std::vector<size_t> p(adj.size(), 0);
	p[from] = 1;

	// 对于拓扑排序产生的序列, 从first到last进行遍历.
	// 对于单个的节点i, 以及所有的v, 满足(v, i) belongs to E,
	// p[i] += p[v].
	for (; first != last; ++first) {
		adj_table::ConstIterators iters = adj(*first);
		for (adj_table::NodeConstIterator i = iters.first; i != iters.second; ++i) {
			p[i->connected] += p[*first];
		}
	}

	return p[to];
}

int main() {
	adj_table adj = prepare_test_case();

	adj_table::TopologicalOrderContainer order;
	adj.topological_sort(order);

	const size_t from = 0, to = 1;
	printf("%u path(s) from %u to %u.\n", count_paths(adj, order, from, to), from, to);
}

//
// 另外一种方法:
// 不对点的颜色进行判断, 从s开始调用DFS_VISIT, 遇到v时, 计数器+1.
// 最终计数器的值, 就是s->v的路径数.
// 这样,在for循环的执行次数为|Adj(u)| = O(E), 由于递归, 效率是O(E^2), 而
// 不是线性的方法.
//
