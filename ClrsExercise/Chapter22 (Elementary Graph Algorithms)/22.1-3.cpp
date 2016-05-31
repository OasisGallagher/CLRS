#include "graph.hpp"

// O(|E| + |V|).
void transpose(adj_table& table) {
	size_t sz = table.size();

	adj_table other(sz);
	for (size_t i = 0; i < sz; ++i) {
		adj_table::Iterators iters = table(i);
		for (adj_table::NodeIterator it = iters.first; it != iters.second; ++it) {
			other.append(it->connected, i);
		}
	}

	table = other;
}

// O(|E|^2)
void transpose(adj_matrix& matrix) {
	size_t sz = matrix.size();
	for (size_t i = 0; i < sz; ++i) {
		for (size_t j = i; j < sz; ++j) {
			std::swap(matrix(i, j), matrix(j, i));
		}
	}
}

int main() {
	adj_table table(2);
	table.append(0, 1);
	table.append(0, 0);
	table.append(1, 1);

	transpose(table);
}
