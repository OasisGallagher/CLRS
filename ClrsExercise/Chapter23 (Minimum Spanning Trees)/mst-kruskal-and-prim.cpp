#include <map>
#include <list>
#include <vector>
#include <queue>
#include <string>
#include <functional>
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
	unsigned k;

	int dbgVertexNumber;
	Vertex() :color(White) {
		Clear();
		dbgVertexNumber = DbgVertexNumber++;
	}

	void Clear() {
		color = White;
		parent = NULL;
		k = std::numeric_limits<unsigned>::max();
	}
} *VertexPtr;

typedef std::pair<VertexPtr, VertexPtr> Edge;
typedef std::vector<Edge> EdgeContainer;
typedef std::map<Edge, unsigned> WeightContainer;
typedef std::list<VertexPtr> VertexContainer;
typedef std::map<VertexPtr, VertexContainer> GraphContainer;

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
		return lhs->k > rhs->k;
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

	EdgeContainer MinimumSpanningTrees() {
		return MstPrim(graphContainer, weight);
	}
private:
	GraphContainer Transpose(const GraphContainer& g) const;

	VertexContainer V(const GraphContainer& g) const;

	EdgeContainer E(const GraphContainer& g) const;

	EdgeContainer MstKruskal(GraphContainer& g, WeightContainer& w) const;

	EdgeContainer MstPrim(GraphContainer& g, WeightContainer& w) const;

	GraphContainer graphContainer;

	WeightContainer weight;
};

GraphContainer Graph::Transpose(const GraphContainer& g) const {
	GraphContainer result;

	for(GraphContainer::const_iterator ite = g.begin(); ite != g.end(); ++ite) {
		const VertexContainer& adjList = ite->second;
		for(VertexContainer::const_iterator ite2 = adjList.begin(); ite2 != adjList.end(); ++ite2) {
			result[*ite2].push_back(ite->first);
		}
	}

	return result;
}

VertexContainer Graph::V(const GraphContainer& g) const {
	VertexContainer result;
	for(GraphContainer::const_iterator ite = g.begin(); ite != g.end(); ++ite) {
		result.push_back(ite->first);
	}
	return result;
}

EdgeContainer Graph::E(const GraphContainer& g) const {
	EdgeContainer edges;
	for(GraphContainer::const_iterator ite = g.begin(); ite != g.end(); ++ite) {
		for(VertexContainer::const_iterator ite2 = ite->second.begin(); ite2 != ite->second.end(); ++ite2) {
			edges.push_back(std::make_pair(ite->first, *ite2));
		}
	}
	return edges;
}

EdgeContainer Graph::MstKruskal(GraphContainer& g, WeightContainer& w) const {
	VertexContainer vertices = V(g);
	disjoint_sets<VertexPtr> sets;

	// O(V).
	for (VertexContainer::const_iterator ite = vertices.begin(); ite != vertices.end(); ++ite) {
		// O(1).
		sets.make_set(*ite);
	}

	EdgeContainer edges = E(g);
	// O(E*log(E)).
	std::sort(edges.begin(), edges.end(), EdgeWeightComparer(w));

	EdgeContainer result;

	// O((E + V)*a(V)) for all disjoint set operations.
	for (EdgeContainer::const_iterator ite = edges.begin(); ite != edges.end(); ++ite) {
		if (sets.find_set(ite->first) != sets.find_set(ite->second)) {
			result.push_back(*ite);
			sets.union_set(ite->first, ite->second);
		}
	}

	return result;
}

EdgeContainer Graph::MstPrim(GraphContainer& g, WeightContainer& w) const {
	// Cut G into Q and V - Q.
	VertexContainer vertices = V(g);
	std::for_each(vertices.begin(), vertices.end(), std::mem_fun(&Vertex::Clear));

	vertices.front()->k = 0;

	EdgeContainer result;

	for (minimumheap<VertexPtr, KValueComparer> heap(vertices.begin(), vertices.end()); !heap.empty();) {
		VertexPtr u = heap.top();
		heap.pop();

		VertexContainer adjList = g[u];

		for (VertexContainer::iterator ite = adjList.begin(); ite != adjList.end(); ++ite) {
			VertexPtr v = *ite;
			unsigned weight = w[std::make_pair(u, v)];
			if (heap.contains(v) && weight < v->k) {
				v->parent = u;

				// decrease key.
				v->k = weight;
				heap.rearrange(v);
			}
		}

		// add (u, parent[u])(the light edge from Q to (V - Q))into result container.
		if (u->parent != NULL)
			result.push_back(std::make_pair(u->parent, u));
	}

	assert(result.size() == vertices.size() - 1);
	return result;
}

int main() {
	Graph g;
	const unsigned VertexCount = 6;
	VertexPtr* vertices = new VertexPtr[VertexCount];
	for (size_t i = 0; i < VertexCount; ++i)
		vertices[i] = new Vertex();

#pragma region Prepare
	VertexContainer container;

	container.push_back(vertices[1]); 
	container.push_back(vertices[3]);
	container.push_back(vertices[2]);
	g.Set(vertices[0], container);

	container.clear();
	container.push_back(vertices[3]);
	container.push_back(vertices[0]);
	container.push_back(vertices[2]);
	g.Set(vertices[1], container);

	container.clear();
	container.push_back(vertices[1]);
	container.push_back(vertices[3]);
	container.push_back(vertices[0]);
	g.Set(vertices[2], container);

	container.clear();
	container.push_back(vertices[2]);
	container.push_back(vertices[1]);
	container.push_back(vertices[0]);
	g.Set(vertices[3], container);

	container.clear();
	container.push_back(vertices[4]);
	g.Set(vertices[3], container);

	container.clear();
	container.push_back(vertices[5]);
	g.Set(vertices[4], container);

	container.clear();
	container.push_back(vertices[4]);
	g.Set(vertices[5], container);
#pragma endregion

	g.MinimumSpanningTrees();
	return 0;
}
