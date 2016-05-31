#include "Algorithm/Exercise/ClrsExercise/Chapter22 (Elementary Graph Algorithms)/graph.hpp"
#include <iostream>
#include <functional>

// O(|V|*log(|V|)) + O(|E| + |V|)
// 可以在dfs发现某个节点置Gray和置Black时分别打印, 那么, 就可以在O(|E| + |V|)内完成.
void parenthesis_structure(const adj_table::TimeContainer& discovered, const adj_table::TimeContainer& finished) {
	struct St {
		size_t vertex;
		size_t time;
		bool finished;
		bool operator < (const St& other) const {
			return time < other.time;
		}
		St(size_t v, size_t t, bool f) { 
			vertex = v, time = t, finished = f;
		}
	};

	typedef std::vector<St> Container;
	Container cont;
	for(adj_table::TimeContainer::size_type i = 0; i < discovered.size(); ++i)
		cont.push_back(St(i, discovered[i], false));
	for(adj_table::TimeContainer::size_type i = 0; i < finished.size(); ++i)
		cont.push_back(St(i, finished[i], true));

	std::sort(cont.begin(), cont.end());

	for(Container::iterator ite = cont.begin(); ite != cont.end(); ++ite) {
		const char* fmt = (ite->finished) ? " %d) " : " (%d ";
		printf(fmt, ite->vertex);
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
	adj_table::TimeContainer discovered, finished;
	adj_table::ParentContainer p;
	adj.dfs(&p, &discovered, &finished);
	parenthesis_structure(discovered, finished);
}
