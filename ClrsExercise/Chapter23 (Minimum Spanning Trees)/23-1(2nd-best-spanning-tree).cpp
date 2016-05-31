#include <map>
#include <list>
#include <vector>
#include <queue>
#include <string>
#include <functional>
#include <iostream>
#include <cassert>
#include "Algorithm/Exercise/ClrsExercise/Chapter6 (Heapsort)/minimumheap.hpp"

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

static const Edge InvalidEdge(NULL, NULL);

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

class MaxWeightEdgesContainer {
	std::map<Edge, Edge> table;
public:
	Edge& operator()(Edge::first_type first, Edge::second_type second) {
		return table[std::make_pair(first, second)];
	}
};

class Graph {
public:
	void Set(VertexPtr from, const VertexContainer& list) {
		VertexContainer& adjList = graphContainer[from];
		adjList.assign(list.begin(), list.end());
	}

	GraphContainer SecondBestSpanningTree() {
		VertexContainer vertices = V(graphContainer);
		size_t w = 1;
		for (VertexContainer::iterator ite = vertices.begin(); ite != vertices.end(); ++ite) {
			for (VertexContainer::iterator ite2 = vertices.begin(); ite2 != vertices.end(); ++ite2) {
				if(ite == ite2) continue;
				++w;
				// undirected graph.
				weight[std::make_pair(*ite, *ite2)] = w;
				weight[std::make_pair(*ite2, *ite)] = w;
			}
		}

		return SecondBestSpanningTree(graphContainer, weight);
	}
private:
	// All vertices.
	VertexContainer V(const GraphContainer& g) const;

	// All edges.
	EdgeContainer E(const GraphContainer& g) const;

	void ClearVertices(VertexContainer& vertices) const {
		std::for_each(vertices.begin(), vertices.end(), std::mem_fun(&Vertex::Clear));
	}

	GraphContainer MstPrim(GraphContainer& g, WeightContainer& w) const;

	// clrs 22-1
	// (a) 对w值互异的G, MST唯一. 因为取V-1个w值, 只能得到唯一的最小值. 
	//     同理, 次MST不一定唯一. 因为取V-1个w值, 可能得到多个次最小值. 
	// (b) T上的任意两点, u和v, 在T中存在连接u, v两顶点的唯一路径.
	//     由于G的E >= V, 所以E-T上, 依然存在若干条边, 加入到T中, 即可使T形成环.
	//     依次添加E-T上的边(x,y), 并寻找(x,y)为起末点的环. 此通路
	//     上, w(x,y)必然是该路径上最大的(否则会形成更优的MST), 去掉该路径中w值
	//     次最大的, 再加上w(x,y), 就会得到新的总权重, 记录替换后最小的总权重
	//     时被移除的边以及(x,y), 就可以得到次最小生成树.
	// (c)
	void FindAllEdgesWithMaxWeight(MaxWeightEdgesContainer& result, GraphContainer& spanningTree, WeightContainer& w) const;
	void BfsFindAllEdgesWithMaxWeight(MaxWeightEdgesContainer& result, GraphContainer& spanningTree, WeightContainer& w) const;
	void DfsFindAllEdgesWithMaxWeight(MaxWeightEdgesContainer& result, GraphContainer& spanningTree, WeightContainer& w) const;
	void DfsFindAllEdgesWithMaxWeight(VertexPtr u, VertexPtr x, MaxWeightEdgesContainer& result, GraphContainer& spanningTree, WeightContainer& w) const;
	// (d)
	GraphContainer SecondBestSpanningTree(GraphContainer& g, WeightContainer& w) const;

	GraphContainer Edges2Graph(EdgeContainer& edges) const;
	
	GraphContainer graphContainer;

	WeightContainer weight;
};

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

GraphContainer Graph::MstPrim(GraphContainer& g, WeightContainer& w) const {
	// Cut G into Q and V - Q.
	VertexContainer vertices = V(g);
	ClearVertices(vertices);

	vertices.front()->k = 0;

	EdgeContainer result;

	for (minimum_heap<VertexPtr, KValueComparer> heap(vertices.begin(), vertices.end()); !heap.empty();) {
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
	
	return Edges2Graph(result);
}

void Graph::FindAllEdgesWithMaxWeight(MaxWeightEdgesContainer& result, GraphContainer& spanningTree, WeightContainer& w) const {
	BfsFindAllEdgesWithMaxWeight(result, spanningTree, w);
}

void Graph::BfsFindAllEdgesWithMaxWeight(MaxWeightEdgesContainer& result, GraphContainer& spanningTree, WeightContainer& w) const {
	VertexContainer vertices = V(spanningTree);
	ClearVertices(vertices);
	typedef std::queue<VertexPtr> Queue;

	// O(V).
	for (VertexContainer::iterator ite = vertices.begin(); ite != vertices.end(); ++ite) {
		VertexPtr u = *ite;
		// O(V).
		for (VertexContainer::iterator ite2 = vertices.begin(); ite2 != vertices.end(); ++ite2)
			result(u, *ite2) = InvalidEdge;

		// O(E+V), for a spanning tree, E=V-1, O(E+V)=O(V).
		Queue q;
		q.push(u);

		// 通过判断result(u, v) ?= InvalidEdge, 从而判断(u, v)是否被访问过, 从而
		// 避免(u, v)被访问多次.
		for (; !q.empty();) {
			VertexPtr x = q.front();
			q.pop();

			VertexContainer adjList = spanningTree[x];
			for (VertexContainer::iterator ite2 = adjList.begin(); ite2 != adjList.end(); ++ite2) {
				VertexPtr v = *ite2;

				if(u == v || result(u, v) != InvalidEdge) continue;

				if (u == x || w[std::make_pair(x, v)] > w[result(u, x)])
					result(u, v) = std::make_pair(x, v);
				else
					result(u, v) = result(u, x);

				q.push(v);
			}
		}
	}
}

void Graph::DfsFindAllEdgesWithMaxWeight(MaxWeightEdgesContainer& result, GraphContainer& spanningTree, WeightContainer& w) const {
	VertexContainer vertices = V(spanningTree);
	for (VertexContainer::iterator ite = vertices.begin(); ite != vertices.end(); ++ite) {
		VertexPtr u = *ite;
		for (VertexContainer::iterator ite2 = vertices.begin(); ite2 != vertices.end(); ++ite2)
			result(u, *ite2) = InvalidEdge;

		DfsFindAllEdgesWithMaxWeight(u, u, result, spanningTree, w);
	}
}

void Graph::DfsFindAllEdgesWithMaxWeight(VertexPtr u, VertexPtr x, MaxWeightEdgesContainer& result, GraphContainer& spanningTree, WeightContainer& w) const {
	VertexContainer adjList = spanningTree[x];
	for (VertexContainer::iterator ite = adjList.begin(); ite != adjList.end(); ++ite) {
		VertexPtr v = *ite;

		if(u == v || result(u, v) != InvalidEdge) continue;

		if (u == x || w[std::make_pair(x, v)] > w[result(u, x)])
			result(u, v) = std::make_pair(x, v);
		else
			result(u, v) = result(u, x);

		DfsFindAllEdgesWithMaxWeight(u, v, result, spanningTree, w);
	}
}

GraphContainer Graph::Edges2Graph(EdgeContainer& edges) const {
	GraphContainer g;
	for (EdgeContainer::iterator ite = edges.begin(); ite != edges.end(); ++ite) {
		Edge& e = *ite;
		g[e.first].push_back(e.second);
		g[e.second].push_back(e.first);
	}

	return g;
}

GraphContainer Graph::SecondBestSpanningTree(GraphContainer& g, WeightContainer& w) const {
	// Compute minimum spanning tree with prim's algorithm.
	GraphContainer mst = MstPrim(g, w);

	MaxWeightEdgesContainer maxWeightEdgesContainer;
	FindAllEdgesWithMaxWeight(maxWeightEdgesContainer, mst, w);
	
	EdgeContainer edges = E(g), stEdges = E(mst);

	std::sort(stEdges.begin(), stEdges.end());

	unsigned secondMinWeight = std::numeric_limits<unsigned>::max();
	Edge edgeNew = InvalidEdge, edgeOld = InvalidEdge;

	// 检查所有在原图中, 但是不在MST中的边.
	for (EdgeContainer::iterator ite = edges.begin(); ite != edges.end(); ++ite) {
		if (std::binary_search(stEdges.begin(), stEdges.end(), *ite))
			continue;

		// 寻找增减一条边之后, 使得总权值增量最小的边.
		Edge& maxWeightEdge = maxWeightEdgesContainer(ite->first, ite->second);
		assert(w[std::make_pair(ite->first, ite->second)] >= w[maxWeightEdge]);

		unsigned deltaWeight = w[std::make_pair(ite->first, ite->second)] - w[maxWeightEdge];
		if (deltaWeight < secondMinWeight) {
			secondMinWeight = deltaWeight;
			edgeNew = std::make_pair(ite->first, ite->second);
			edgeOld = maxWeightEdge;
		}
	}

	edges = E(mst);
	edges.erase(std::find(edges.begin(), edges.end(), edgeOld));
	edges.push_back(edgeNew);

	return Edges2Graph(edges);
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

	g.SecondBestSpanningTree();
	return 0;
}
