#include <map>
#include <list>
#include <vector>
#include <queue>
#include <string>
#include <functional>
#include <numeric>
#include <iostream>
#include <cassert>
#include "Algorithm/Exercise/ClrsExercise/Chapter6 (Heapsort)/minimumheap.hpp"
#include "Algorithm/Exercise/ClrsExercise/Chapter21 (Data Structures for Disjoint Sets)/disjoint_sets.hpp"

enum Color {
	White,
	Gray,
	Black,
};

int DbgVertexNumber = 0;

typedef struct Vertex {
	Color color;
	Vertex* parent;
	unsigned d;

	int dbgVertexNumber;
	Vertex() :color(White) {
		Clear();
		dbgVertexNumber = DbgVertexNumber++;
	}

	void Clear() {
		color = White;
		parent = NULL;
		d = std::numeric_limits<unsigned>::max();
	}
} *VertexPtr;

typedef std::pair<VertexPtr, VertexPtr> Edge;
typedef std::vector<Edge> EdgeContainer;
typedef std::map<Edge, int> WeightContainer;
typedef std::map<VertexPtr, unsigned> PERTWeightContainer;
typedef std::list<VertexPtr> VertexContainer;
typedef std::map<VertexPtr, VertexContainer> GraphContainer;
typedef std::map<Edge, int> ConstraintContainer;

struct EdgeWeightComparer : public std::binary_function<Edge, Edge, bool> {
	WeightContainer& weight;
	EdgeWeightComparer(WeightContainer& w) : weight(w) {
	}
	bool operator()(const Edge& lhs, const Edge& rhs) const {
		return weight[lhs] < weight[rhs];
	}
};

// minimum-heap.
struct KValueComparer : public std::binary_function<VertexPtr, VertexPtr, bool> {
	bool operator()(VertexPtr lhs, VertexPtr rhs) const {
		return lhs->d > rhs->d;
	}
};

class Graph {
public:
	void Set(VertexPtr from, const VertexContainer& list) {
		VertexContainer& adjList = graphContainer[from];
		adjList.assign(list.begin(), list.end());
	}

	VertexContainer V() {
		return V(graphContainer);
	}

	VertexContainer OutEdges(VertexPtr vertex) {
		return graphContainer[vertex];
	}

	bool SingleSourceShortestPath(VertexPtr src) {
		InitializeSingleSource(graphContainer, src);
		return BellmanFord(graphContainer, weight, src);
	}
private:
	VertexContainer TopologicalSort(GraphContainer& g) const;

	void TopologicalSort(GraphContainer& g, VertexPtr u, VertexContainer* result) const;

	bool BellmanFord(GraphContainer& g, WeightContainer& w, VertexPtr src) const;

	// assert: w[edge] >= 0 for each edge in g.
	void Dijkstra(GraphContainer& g, WeightContainer& w, VertexPtr src) const;

	void DifferenceConstraints(ConstraintContainer& cont) const;

	void DagShortestPaths(GraphContainer& g, WeightContainer& w, VertexPtr src) const;

	unsigned DagCountPaths(GraphContainer& g, WeightContainer& w) const;

	void DagPERT(GraphContainer& g, PERTWeightContainer& w, VertexPtr src) const;

	bool Relax(VertexPtr u, VertexPtr v, WeightContainer& w) const;

	void InitializeSingleSource(GraphContainer& g, VertexPtr src) const;

	GraphContainer Transpose(const GraphContainer& g) const;

	VertexContainer V(const GraphContainer& g) const;

	EdgeContainer E(const GraphContainer& g) const;

	GraphContainer graphContainer;

	WeightContainer weight;
};

VertexContainer Graph::TopologicalSort(GraphContainer& g) const {
	VertexContainer vertices = V(g);
	std::for_each(vertices.begin(), vertices.end(), &Vertex::Clear);

	VertexContainer result;
	for (VertexContainer::iterator ite = vertices.begin(); ite != vertices.end(); ++ite) {
		VertexPtr u = *ite;
		if (u->color == White)
			TopologicalSort(g, u, &result);
	}
}

void Graph::TopologicalSort(GraphContainer& g, VertexPtr u, VertexContainer* result) const {
	u->color = Gray;
	VertexContainer adj = g[u];
	for (VertexContainer::iterator ite = adj.begin(); ite != adj.end(); ++ite) {
		VertexPtr v = *ite;
		if (v->color == White)
			TopologicalSort(g, v, result);
	}

	u->color = Black;
	result->push_front(u);
}

// cont(u, v) = w, 表示u.d - v.d <= w.
void Graph::DifferenceConstraints(ConstraintContainer& cont) const {
	GraphContainer g;
	WeightContainer w; // w可以为负, 因为后续使用的是BellmanFord方法.
	// u.d <= v.d + w, 表示v->u的边, 权值为w.
	// 从cont构造g.
	for (ConstraintContainer::iterator ite = cont.begin(); ite != cont.end(); ++ite) {
		Edge edge = ite->first;
		g[edge.second].push_back(edge.first);
		w[std::make_pair(edge.second, edge.first)] = ite->second;
	}

	// 添加v0点, 使v0可达任意其他顶点, 且权值为0.
	// 保证存在一个起点, 可以访问到g中的每一个点.
	VertexPtr v0 = new Vertex;
	VertexContainer vertices = V(g);
	for (VertexContainer::iterator ite = vertices.begin(); ite != vertices.end(); ++ite) {
		g[v0].push_back(*ite);
		w[std::make_pair(v0, *ite)] = 0;
	}

	BellmanFord(g, w, v0);

	delete v0;
}

bool Graph::BellmanFord(GraphContainer& g, WeightContainer& w, VertexPtr src) const {
	size_t vertexCount = V(g).size();
	EdgeContainer edges = E(g);
	for (size_t i = 1; i < vertexCount; ++i) { // V-1 iterations.
		for (EdgeContainer::iterator ite = edges.begin(); ite != edges.end(); ++ite) {
			Relax(ite->first, ite->second, w);
		}
	}

	// test for negative cycle reachable from src.
	for (EdgeContainer::iterator ite = edges.begin(); ite != edges.end(); ++ite) {
		if (ite->first->d + w[std::make_pair(ite->first, ite->second)] < ite->second->d)
			return false;
	}

	return true;
}

void Graph::Dijkstra(GraphContainer& g, WeightContainer& w, VertexPtr src) const {
	VertexContainer vertices = V(g);
	minimum_heap<VertexPtr, KValueComparer> heap(vertices.begin(), vertices.end());

	for (; !heap.empty();) {
		VertexPtr u = heap.top();
		heap.pop();

		VertexContainer adj = g[u];

		for (VertexContainer::iterator ite = adj.begin(); ite != adj.end(); ++ite) {
			VertexPtr v = *ite;
			if (Relax(u, v, w))
				heap.rearrange(v);
		}
	}
}

static void set(VertexPtr v, unsigned value) {
	v->d = value;
}

static unsigned get(VertexPtr v) {
	return v->d;
}

void Graph::DagShortestPaths(GraphContainer& g, WeightContainer& w, VertexPtr src) const {
	VertexContainer sequence = TopologicalSort(g);
	InitializeSingleSource(g, src);

	for (VertexContainer::iterator ite = sequence.begin(); ite != sequence.end(); ++ite) {
		VertexPtr u = *ite;
		VertexContainer adj = g[u];
		for (VertexContainer::iterator adjIter = adj.begin(); adjIter != adj.end(); ++adjIter)
			Relax(u, *adjIter, w);
	}
}

unsigned Graph::DagCountPaths(GraphContainer& g, WeightContainer& w) const {
	assert(!g.empty());
	VertexContainer sequence = TopologicalSort(g);
	std::map<VertexPtr, unsigned> buffer;
	++buffer[sequence.back()];
	// O(V+E).
	for (VertexContainer::reverse_iterator ite = ++sequence.rbegin(); ite != sequence.rend(); ++ite) {
		VertexPtr u = *ite;
		VertexContainer adj = g[u];
		for (VertexContainer::iterator adjIter = adj.begin(); adjIter != adj.end(); ++adjIter) {
			VertexPtr v = *adjIter;
			buffer[u] += buffer[v];
		}
	}

	return std::accumulate(buffer.begin(), buffer.end(), 0);
}

void Graph::DagPERT(GraphContainer& g, PERTWeightContainer& w, VertexPtr src) const {
	WeightContainer weight;

	// 对于入度为0的节点, 加入新的顶点s, 并连接s和该点.
	VertexContainer vertices = V(g);
	for (VertexContainer::iterator ite = vertices.begin(); ite != vertices.end(); ++ite) {
		//
	}

	EdgeContainer edges = E(g);
	// 令边(u, v)的权值 = -w[v].
	for (EdgeContainer::iterator ite = edges.begin(); ite != edges.end(); ++ite) {
		weight[*ite] = -w[ite->second];
	}

	DagShortestPaths(g, weight, src);
}

void Graph::InitializeSingleSource(GraphContainer& g, VertexPtr src) const {
	VertexContainer vertices = V(g);
	for (VertexContainer::iterator v = vertices.begin(); v != vertices.end(); ++v) {
		VertexPtr vertex = *v;
		vertex->d = std::numeric_limits<unsigned>::max();
		vertex->parent = NULL;
	}

	src->d = 0;
}

bool Graph::Relax(VertexPtr u, VertexPtr v, WeightContainer& w) const {
	if (v->d > u->d + w[std::make_pair(u, v)]) {
		v->d = u->d + w[std::make_pair(u, v)];
		v->parent = u;
		return true;
	}

	return false;
}

GraphContainer Graph::Transpose(const GraphContainer& g) const {
	GraphContainer result;

	for (GraphContainer::const_iterator ite = g.begin(); ite != g.end(); ++ite) {
		const VertexContainer& adjList = ite->second;
		for (VertexContainer::const_iterator ite2 = adjList.begin(); ite2 != adjList.end(); ++ite2) {
			result[*ite2].push_back(ite->first);
		}
	}

	return result;
}

VertexContainer Graph::V(const GraphContainer& g) const {
	VertexContainer result;
	for (GraphContainer::const_iterator ite = g.begin(); ite != g.end(); ++ite) {
		result.push_back(ite->first);
	}
	return result;
}

EdgeContainer Graph::E(const GraphContainer& g) const {
	EdgeContainer edges;
	for (GraphContainer::const_iterator ite = g.begin(); ite != g.end(); ++ite) {
		for (VertexContainer::const_iterator ite2 = ite->second.begin(); ite2 != ite->second.end(); ++ite2) {
			edges.push_back(std::make_pair(ite->first, *ite2));
		}
	}
	return edges;
}

int main() {
	Graph g;
	const unsigned VertexCount = 6;
	VertexPtr* vertices = new VertexPtr[VertexCount];
	for (size_t i = 0; i < VertexCount; ++i)
		vertices[i] = new Vertex();

	return 0;
}

