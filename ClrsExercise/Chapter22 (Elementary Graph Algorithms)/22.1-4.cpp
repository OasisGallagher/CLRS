#include "graph.hpp"
#include <boost/dynamic_bitset.hpp>
// - Given an adjacency-list representation of a multigraph 
//   G = (V, E), describe an O(V + E)-time algorithm to
//	 compute the adjacency-list representation of the
//	 "equivalent" undirected graph G' = (V, E'), where E'
//	 consists of the edges in E with all the multiple edges
//	 between two vertices replaced by a single edge with all
//	 the self-loops removed.
void simplify(adj_table& table) {
	size_t sz = table.size();
	adj_table result = table;

	boost::dynamic_bitset<> bitset(sz);

	// to undirected.	
	for(size_t i = 0; i < sz; ++i) {
		adj_table::Iterators ites = table(i);
		for(adj_table::NodeIterator it = ites.first; it != ites.second; ++it) {
			result.append(it->connected, i);
		}
	}

	for(size_t i = 0; i < sz; ++i) {
		// remove self-loops.
		bitset.set(i);
		adj_table::NodeList& cont = result.list(i);
		for(adj_table::NodeList::iterator it = cont.begin(); it != cont.end(); ) {
			if(!bitset.test(it->connected)) {
				bitset.set(it->connected);
				++it;
			}
			else {
				it = cont.erase(it);
			}
		}

		bitset.reset();
	}

	table = result;
}

int main() {
	adj_table table(2);
	table.append(0, 1);
	table.append(0, 0);
	table.append(1, 1);
	simplify(table);
}

