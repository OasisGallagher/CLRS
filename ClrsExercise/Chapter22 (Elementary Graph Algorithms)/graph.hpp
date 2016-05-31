#pragma once
#include <vector>
#include <algorithm>
#include <iterator>
#include <queue>
#include <cstdarg>
#include <stack>
#include <functional>
#include <cassert>
#include <map>

struct graph_node {
	typedef int value_type;
	value_type connected;
};

std::ostream& operator << (std::ostream& os, const graph_node& node) {
	return os << int(node.connected);
}

typedef std::vector<graph_node> line;
typedef std::vector<line> table;

class adj_matrix {
	table cont;
public:
	adj_matrix(size_t row) {
		cont.resize(row);
		for (table::iterator ite = cont.begin(); ite != cont.end(); ++ite)
			ite->resize(row);
	}

	void print(std::ostream& dest) const {
		for (table::const_iterator ite = cont.begin(); ite != cont.end(); ++ite) {
			std::copy(ite->begin(), ite->end(), std::ostream_iterator<graph_node>(dest, " "));
			dest << std::endl;
		}
	}

	size_t size() const {
		return cont.size();
	}

	typedef graph_node::value_type value_type;
	value_type& operator()(size_t row, size_t col) {
		return cont[row][col].connected;
	}
	const value_type& operator()(size_t row, size_t col) const {
		return cont[row][col].connected;
	}

	adj_matrix operator * (const adj_matrix& other) const {
		size_t sz = cont.size();
		adj_matrix result(sz);

		for (size_t i = 0; i < sz; ++i) {
			for (size_t j = 0; j < sz; ++j) {
				size_t tmp = 0;
				for (size_t k = 0; k < sz; ++k) {
					tmp += cont[i][k].connected * other.cont[k][j].connected;
				}
				result(i, j) = graph_node::value_type(tmp != 0);
			}
		}

		return result;
	}
};

#include <list>

class linear_time_sort {
	typedef std::pair<size_t, size_t> Pair;
	typedef std::vector<Pair> Container;
public:
	// sort the input with radix sort.
	static void radix_sort_pair(Container& input, size_t max_1, size_t max_2) {
		// sort the day part, month part and year part one by one.
		count_sort(max_1, input.begin(), input.end(), second);
		count_sort(max_2, input.begin(), input.end(), first);
	}

private:
	// sort component returned by cmp.
	typedef unsigned(*Traits)(const Pair&);
	// upper: greatest value + 1.
	static void count_sort(unsigned upper, Container::iterator beg, Container::iterator end, Traits traits) {
		std::vector<size_t> counter(upper);

		size_t size = std::distance(beg, end);
		for (unsigned i = 0; i < size; ++i)
			++counter[traits(beg[i])];

		for (unsigned i = 1; i < upper; ++i)
			counter[i] += counter[i - 1];

		Container result(size);

		for (; size != 0;) {
			--size;
			size_t pos = traits(beg[size]);
			result[counter[pos] - 1] = beg[size];
			--counter[pos];
		}

		std::copy(result.begin(), result.end(), beg);
	}

	// returns the key to be compared.
	static unsigned first(const Pair& p) {
		return p.first;
	}
	static unsigned second(const Pair& p) {
		return p.second;
	}
};

class adj_table {
public:
	typedef std::pair<size_t, size_t> Edge;
private:
	typedef std::vector<size_t> ColorContainer;
	enum { Black, Gray, White };
	class UndirectedGraphComparer : public std::binary_function<Edge, Edge, bool>
	{
	public:
		bool operator () (const Edge& lhs, const Edge& rhs) const {
			if (lhs.first == rhs.second && lhs.second == rhs.first)
				return false;
			if (lhs.second == rhs.first && lhs.first == rhs.second)
				return false;
			if (lhs == rhs)
				return false;
			return lhs < rhs;
		}
	};
public:
	enum EdgeType { InvalidEdgeType, TreeEdge, BackEdge, FwdEdge, CrossEdge, };
	typedef std::pair<size_t, size_t> Edge;
	typedef std::vector<Edge> EdgeContainer;
	typedef std::vector<size_t> ComponentContainer;

	typedef std::map<Edge, EdgeType, UndirectedGraphComparer> EdgeTypeContainer;

	typedef std::list<size_t> TopologicalOrderContainer;
	// item.
	typedef std::list<graph_node> NodeList;
	// 
	typedef std::vector<NodeList> Container;
	adj_table(size_t row) {
		cont.resize(row);
	}

	size_t size() const {
		return cont.size();
	}

	size_t count_edges() const {
		size_t count = 0;
		for (Container::const_iterator ite = cont.begin(); ite != cont.end(); ++ite)
			count += ite->size();

		return count;
	}

	typedef NodeList::iterator NodeIterator;
	typedef std::pair<NodeIterator, NodeIterator> Iterators;
	Iterators operator()(size_t i) {
		return iterators(i);
	}

	void append(size_t pos, graph_node::value_type val) {
		graph_node node;
		node.connected = val;
		cont[pos].push_back(node);
	}

	void insert(size_t pos, size_t count, ...) {
		va_list ap;
		va_start(ap, count);
		for (size_t i = 0; i < count; ++i) {
			append(pos, va_arg(ap, graph_node::value_type));
		}

		va_end(ap);
	}

	NodeList& list(size_t pos) {
		return cont[pos];
	}

	typedef NodeList::const_iterator NodeConstIterator;
	typedef std::pair<NodeConstIterator, NodeConstIterator> ConstIterators;
	ConstIterators operator()(size_t i) const {
		return iterators(i);
	}

	typedef std::vector<size_t> ParentContainer;
	typedef std::vector<size_t> DepthContainer;
	static const size_t noparent = 0xffffffffui32;
	static const size_t invalid_node = 0xffffffffui32;
	// breadth-first search.
	void bfs(size_t from, ParentContainer* out_parent, DepthContainer* out_depth) {
		size_t sz = cont.size();
		ParentContainer parent(sz, noparent);
		DepthContainer depth(sz, std::numeric_limits<graph_node::value_type>::max());

		ColorContainer colors(sz, White);
		depth[from] = 0;
		colors[from] = Gray;

		typedef std::queue<size_t> Q;
		Q q;
		q.push(from);
		for (; !q.empty();) {
			size_t vertex = q.front();
			q.pop();

			Iterators ites = iterators(vertex);
			for (NodeIterator it = ites.first; it != ites.second; ++it) {
				if (colors[it->connected] == White) {
					colors[it->connected] = Gray;
					depth[it->connected] = depth[vertex] + 1;
					parent[it->connected] = vertex;
					q.push(it->connected);
				}
			}

			colors[vertex] = Black;
		}

		if (out_parent != NULL)
			out_parent->assign(parent.begin(), parent.end());

		if (out_depth != NULL)
			out_depth->assign(depth.begin(), depth.end());
	}

	typedef std::vector<size_t> TimeContainer;

	void topological_sort(TopologicalOrderContainer& order)
	{
		size_t sz = cont.size();
		ColorContainer color(sz, White);

		// for each vertex ...
		for (size_t i = 0; i < sz; ++i) {
			if (color[i] == White)
				topological_sort(i, color, order);
			//dfs_visit2(i, color, discovered, parent, finished, time);

			//Iterators ites = iterators(i);
			//for (NodeIterator it = ites.first; it != ites.second; ++it) {
			//	if (color[it->connected] == White)
			//		topological_sort(it->connected, color, order);
			//		dfs_visit2(it->connected, color, discovered, parent, finished, time);
			//}
		}
	}

	// returns whether the directed graph is acyclic.
	bool topological_sort2(TopologicalOrderContainer& order) {
		const size_t sz = size();
		size_t* in_degree = new size_t[sz];
		std::fill(in_degree, in_degree + sz, 0);

		// O(V + E).
		for (size_t i = 0; i < sz; ++i) {
			Iterators ites = iterators(i);
			for (NodeIterator it = ites.first; it != ites.second; ++it)
				++in_degree[it->connected];
		}

		// no matter which container.
		typedef std::queue<size_t> Queue;
		Queue q;

		// O(V).
		for (size_t i = 0; i < sz; ++i) {
			if (in_degree[i] == 0)
				q.push(i);
		}

		// O(V + E).
		for (; !q.empty();) {
			size_t u = q.front();
			q.pop();

			order.push_back(u);

			Iterators ites = iterators(u);
			
			// O(E) totally.
			for (NodeIterator it = ites.first; it != ites.second; ++it) {
				assert(in_degree[it->connected] > 0);
				if (--in_degree[it->connected] == 0) {
					q.push(it->connected);
				}
			}
		}

		// O(E).
		size_t* pos = std::find_if(in_degree, in_degree + sz, std::not1(std::bind2nd(std::equal_to<size_t>(), 0)));

		delete[] in_degree;

		assert(pos == in_degree + sz);

		return pos == in_degree + sz;
	}

	void transpose() {
		size_t sz = size();

		adj_table other(sz);
		for (size_t i = 0; i < sz; ++i) {
			adj_table::Iterators iters = iterators(i);
			for (adj_table::NodeIterator it = iters.first; it != iters.second; ++it) {
				other.append(it->connected, i);
			}
		}

		*this = other;
	}

	// components保存的值, components[i] = j, 顶点i属于第j个强连通分支.
	// edge_results包含: 索引 => (SCC ID I, SCC ID II), 表示两个SCC之间有边.
	size_t strongly_connected_component(ComponentContainer& components, EdgeContainer& edge_results) {
		TimeContainer finished;
		dfs(NULL, NULL, &finished);

		size_t V = size();

		std::vector<size_t> sequence(V);

		// sort vetex by decreasing finish time.
		for (size_t i = 0; i < V; ++i) {
			sequence[V - finished[i]] = i;
		}

		transpose();
		ColorContainer color(V, White);
		size_t time = 0;

		// mapping vertex: component_id.
		components.resize(V, noparent);

		size_t component_id = 0;
		for (size_t i = 0; i < V; ++i) {
			size_t vertex = sequence[i];
			if (color[vertex] == White) {
				components[vertex] = component_id;
				dfs_visit(vertex, color, time, components, component_id);
				++component_id;
			}
		}

		EdgeContainer edge_container;
		// O(E).
		// for each edge ...
		for (size_t i = 0; i < V; ++i) {
			Iterators ites = iterators(i);
			size_t comp1 = components[i];
			for (NodeIterator it = ites.first; it != ites.second; ++it) {
				size_t comp2 = components[it->connected];
				if (comp1 != comp2) {
					// an edge from comp1 to comp2.
					edge_container.push_back(std::make_pair(comp1, comp2));
				}
			}
		}

		// use radix sort to perform a O(V) sort.
		linear_time_sort::radix_sort_pair(edge_container, V, V);

		// foreach pair in edge_result:
		//     (pair.first, pair.second) denotes edge from component_id_1 to component_id_2.

		// remove duplicate edges.
		if (!edge_container.empty()) {
			edge_results.push_back(edge_container.front());
			EdgeContainer::iterator pos = edge_container.begin();
			for (; ;) {
				pos = std::find_if(pos, edge_container.end(), std::bind1st(std::not_equal_to<Edge>(), *pos));
				if (pos == edge_container.end())
					break;
				edge_results.push_back(*pos);
			}
		}

		// count of SCC.
		return component_id;
	}

	bool semiconnected() {
		// compute strongly_connected_component.
		ComponentContainer componets;
		EdgeContainer edge_container;
		size_t count = strongly_connected_component(componets, edge_container);

		// topological sort the strongly connected components(say C1, C2 ... Cn).
		adj_table scc_adj(count);
		for (EdgeContainer::const_iterator ite = edge_container.begin(); ite != edge_container.end(); ++ite) {
			scc_adj.append(ite->first, ite->second);
		}

		TopologicalOrderContainer order;
		scc_adj.topological_sort(order);

		if (order.empty())
			return true;

		// return true is for all i belongs to (1, n-1), (Ci, C(i+1)) is exists.
		// otherwise, return false.
		size_t V = order.size();
		for (TopologicalOrderContainer::size_type i = 0; i != V - 1; ++i) {
			TopologicalOrderContainer::size_type next = i + 1;
			if (componets[i] != componets[next])
				return false;
		}

		return true;
	}

	// depth-first search.
	void dfs(ParentContainer* p, TimeContainer* tm_discovered, TimeContainer* tm_finished)
	{
		size_t sz = cont.size();
		if (p != NULL) p->resize(sz, noparent);
		if (tm_discovered != NULL) tm_discovered->resize(sz, 0);
		if (tm_finished != NULL) tm_finished->resize(sz, 0);

		ColorContainer color(sz, White);

		TopologicalOrderContainer order;

		size_t time = 0;

		// for each vertex ...
		for (size_t i = 0; i < sz; ++i) {
			if (color[i] == White)
				dfs_visit(i, color, time, p, tm_discovered, tm_finished);
				//topological_sort(i, color, order);
				//dfs_visit2(i, color, discovered, parent, finished, time);

			//Iterators ites = iterators(i);
			//for (NodeIterator it = ites.first; it != ites.second; ++it) {
			//	if (color[it->connected] == White)
			//		topological_sort(it->connected, color, order);
			//		dfs_visit2(it->connected, color, discovered, parent, finished, time);
			//}
		}
	}

	// CLRS 22-2(c).
	typedef std::vector<size_t> LowValueContainer;
	void dfs_22_2_c(LowValueContainer& low) {
		size_t V = cont.size();
		ColorContainer color(V, White);
		low.resize(V, std::numeric_limits<size_t>::max());

		DepthContainer depth_container(V);
		EdgeTypeContainer edge_type;

		for (size_t i = 0; i < V; ++i) {
			if (color[i] == White) {
				depth_container[i] = 0;
				dfs_visit_22_2_c(i, color, edge_type, low, depth_container, 1);
			}
		}
	}

	void euler_tour() {
		struct TourNode {
			size_t node;
			typedef std::vector<size_t> OutEdgeContainer;
			OutEdgeContainer out_edges;
		};
		typedef std::list<TourNode> TourNodeContainer;
		TourNodeContainer tour_node_container;
		typedef TourNodeContainer::iterator TourNodeIterator;
		// initialize tour_node_container.
		// search a tour from u to u, and remove all in/out edges traversed in this tour.
		// repeat this procedure, until all edges are traversed.
	}
private:
	Iterators iterators(size_t i) {
		return std::make_pair(cont[i].begin(), cont[i].end());
	}

	ConstIterators iterators(size_t i) const {
		return std::make_pair(cont[i].begin(), cont[i].end());
	}

	// time -> [1, 2|V|].
	// [0, discovered[u])			-> White
	// [discovered[u], finished[u]) -> Gray
	// [finished[u], inf)			-> Black
	void dfs_visit(size_t u, ColorContainer& color, size_t& time, 
		ParentContainer* parent, TimeContainer* discovered, TimeContainer* finished) {
		color[u] = Gray;
		if (discovered != NULL)
			discovered->at(u) = ++time;
		Iterators ites = iterators(u);

		for (NodeIterator it = ites.first; it != ites.second; ++it) {
			// explore edge (u, it->connected).
			// we can classify edges here.
			if (color[it->connected] == White) {
				if (parent != NULL)
					parent->at(it->connected) = u;
				dfs_visit(it->connected, color, time, parent, discovered, finished);
			}
		}

		color[u] = Black;
		if (finished != NULL)
			finished->at(u) = ++time;
	}

	// for an undirected graph.
	size_t dfs_visit_22_2_c(size_t u, ColorContainer& color, EdgeTypeContainer& edge_type,
			LowValueContainer& low, DepthContainer& depth_container, unsigned depth) {
		color[u] = Gray;
		Iterators ites = iterators(u);
		for (NodeIterator it = ites.first; it != ites.second; ++it) {
			if (color[it->connected] == White) {
				edge_type[std::make_pair(u, it->connected)] = TreeEdge;
				depth_container[it->connected] = depth;
				low[u] = std::min(dfs_visit_22_2_c(it->connected, color, edge_type, low, depth_container, depth + 1), low[u]);
			}
			// back edge.
			else if (color[it->connected] == Gray && edge_type[std::make_pair(it->connected, u)] != TreeEdge) {
				edge_type[std::make_pair(u, it->connected)] = BackEdge;
				low[u] = std::min(low[u], depth_container[it->connected]);
			}
		}

		color[u] = Black;

		return low[u];
	}

	void dfs_visit(size_t u, ColorContainer& color, size_t& time, ComponentContainer& component, size_t component_id) {
		color[u] = Gray;
		Iterators ites = iterators(u);

		for (NodeIterator it = ites.first; it != ites.second; ++it) {
			if (color[it->connected] == White) {
				component[it->connected] = component_id;
				dfs_visit(it->connected, color, time, component, component_id);
			}
		}

		color[u] = Black;
	}

	void dfs_visit_classify_edge_type(size_t u, ColorContainer& color) {
		color[u] = Gray;
		Iterators ites = iterators(u);
		for (NodeIterator it = ites.first; it != ites.second; ++it) {
#if DIRECTED_GRAPH
			switch(color[it->connected]) {
				case White:
					// edge[u, it->connected] is a tree edge.
					break;
				case Black:
					// edge[u, it->connected] is a foward or cross edge.
					break;
				case Gray:
					// edge[u, it->connected] is a back edge.
					break;
				default:
					break;
			}
#else	// only tree edge or back edge is possible.
			/*
			for each edge in E[G]:
				edge_type[edge] = Invalid;
			*/
			switch (color[it->connected]) {
			case White:
				// edge[u, it->connected] is a tree edge.
				/*
				edge_type[u, it->connected] = TreeEdge;
				*/
				break;
			case Black:
				// impossible.
				break;
			case Gray:
				// the edge is classified as the first type in the
				// classification list that applies.
				/*
				if edge_type[u, it->connected] != TreeEdge {
					edge_type[u, it->connected] = BackEdge;
				}
				*/
				break;
			default:
				break;
			}
#endif
			if (color[it->connected] == White)  {
				dfs_visit_classify_edge_type(it->connected, color);
			}
		}
	}

	struct WhiteFinder : public std::binary_function<graph_node, ColorContainer, bool>
	{
		bool operator()(graph_node& n, ColorContainer c) const {
			return c[n.connected] == White;
		}
	};
	
	std::binder2nd<WhiteFinder> white_finder(ColorContainer& color) {
		return std::bind2nd(WhiteFinder(), color);
	}

	// rewrite the procedure DFS, using a stack to eliminate recursion.
	void dfs_visit2(size_t u, ColorContainer& color, TimeContainer& discovered,
		ParentContainer& parent, TimeContainer& finished, size_t& time) {
		typedef std::stack<size_t> Stack;
		Stack s;

		for (; !s.empty() || u != invalid_node;) {
			for (; u != invalid_node;) {
				s.push(u);
				color[u] = Gray;
				discovered[u] = ++time;

				Iterators ites = iterators(u);
				NodeIterator pos = std::find_if(ites.first, ites.second, white_finder(color));
				if (pos == ites.second)
					break;
				
				parent[pos->connected] = u;
				u = pos->connected;
			}

			if (!s.empty()) {
				size_t v = s.top();
				s.pop();
				// use v.
				Iterators ites = iterators(v);
				NodeIterator pos = std::find_if(ites.first, ites.second, white_finder(color));
				u = (pos == ites.second) ? invalid_node : pos->connected;
				if (u != invalid_node)
					parent[u] = v;
				else {
					color[v] = Black;
					finished[v] = ++time;
				}
			}
		}
	}

	void topological_sort(size_t u, ColorContainer& color, TopologicalOrderContainer& order) {
		color[u] = Gray;
		Iterators iters = iterators(u);
		for (NodeIterator i = iters.first; i != iters.second; ++i) {
			if (color[i->connected] == White)
				topological_sort(i->connected, color, order);
		}

		color[u] = Black;
		order.push_front(u);
	}

	Container cont;
};
