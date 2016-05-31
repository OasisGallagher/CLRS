#include <map>
#include <list>
#include <vector>
#include <queue>
#include <string>
#include <functional>
#include <iostream>

// 缺少22-2(h)...

enum Color {
	White,
	Gray,
	Black,
};

int DbgVertexNumber = 0;

typedef struct Vertex {
	Color color;
	Vertex* parent;
	unsigned childrenCount;
	unsigned d, f;

	int dbgVertexNumber;
	Vertex() :color(White) {
		Clear();
		dbgVertexNumber = DbgVertexNumber++;
	}

	void Clear() {
		color = White;
		parent = NULL;
		d = f = childrenCount = 0;
	}
} *VertexPtr;

typedef std::list<VertexPtr> VertexContainer;
typedef std::pair<VertexPtr, VertexPtr> Edge;
typedef std::vector<Edge> EdgeContainer;
typedef std::map<VertexPtr, VertexContainer> GraphContainer;

class Graph {
public:
	void Set(VertexPtr from, const VertexContainer& list) {
		VertexContainer& adjList = graphContainer[from];
		adjList.assign(list.begin(), list.end());
	}

	// 欧拉回路. 只有当每个顶点的出/入度相等时才存在.
	void EulerTour() {
		if(!graphContainer.empty()) {
			try {
				eulerTour(graphContainer);
			}
			catch(const std::exception& exp) {
				std::cout << std::string("Error: ") + exp.what() << std::endl;
			}
		}
	}

	VertexContainer V() {
		return V(graphContainer);
	}

	VertexContainer OutEdges(VertexPtr vertex) {
		return graphContainer[vertex];
	}

	VertexContainer ArticulationPoints() {
		BioConnectedComponent bcc(*this);
		return bcc.ArticulationPoints();
	}

	EdgeContainer Bridges() {
		BioConnectedComponent bcc(*this);
		return bcc.Bridges();
	}

	void GetBioConnectedComponent() {
		BioConnectedComponent bcc(*this);
		bcc.Get();
	}
private:
	friend class BioConnectedComponent;
	class BioConnectedComponent {
	public:
		BioConnectedComponent(Graph& G) :graph(G) {
		}
		VertexContainer ArticulationPoints();
		EdgeContainer Bridges();
		void Get();
	private:
		typedef std::map<VertexPtr, unsigned> LowValueContainer;
		LowValueContainer ComputeLowValue(VertexPtr& root);
		void DfsVisit(LowValueContainer& lowValueContainer, unsigned& time, VertexPtr from);
		void DfsVisitOn(GraphContainer& graphContainer, VertexPtr from);
		void MarkBioConnectedComponent();

		Graph& graph;
	};

	GraphContainer Transpose(const GraphContainer& graph) const;

	VertexContainer V(const GraphContainer& g) const;

	struct ListNode {
		ListNode* previous, *next;
		VertexPtr vertex;
		ListNode() : previous(NULL), next(NULL), vertex(NULL) {
		}
	};

	struct Position {
		VertexPtr vertex;
		ListNode* pos;
	};

	typedef std::map<VertexPtr, VertexContainer> OutDegreeContainer;

	// 计算并打印图graph上的欧拉回路.
	void eulerTour(const GraphContainer& graph);

	// 从from开始, 寻找一个回路.
	// 由于假定该图存在欧拉回路, 那么, 从每个点发起的探寻, 一定能找到回路.
	std::pair<ListNode*, ListNode*> eulerTourVisit(VertexPtr from, OutDegreeContainer& outDegree, std::queue<Position>& positionQueue);

	GraphContainer graphContainer;
};

GraphContainer Graph::Transpose(const GraphContainer& graph) const {
	GraphContainer result;

	for(GraphContainer::const_iterator ite = graph.begin(); ite != graph.end(); ++ite) {
		const VertexContainer& adjList = ite->second;
		for(VertexContainer::const_iterator ite2 = adjList.begin(); ite2 != adjList.end(); ++ite2) {
			result[*ite2].push_back(ite->first);
		}
	}

	return result;
}

std::pair<Graph::ListNode*, Graph::ListNode*> Graph::eulerTourVisit(VertexPtr from, OutDegreeContainer& outDegree, std::queue<Position>& positionQueue) {
	ListNode* first = NULL, *last = NULL;

	VertexPtr start = from;

	for(VertexContainer* od = &outDegree[from]; !od->empty(); od = &outDegree[from]) {
		VertexPtr next = od->front();
		od->pop_front();

		ListNode* currentNode = new ListNode();
		currentNode->vertex = from;

		if(first == NULL)
			first = last = currentNode;
		else {
			last->next = currentNode;
			currentNode->previous = last;
		}

		if(!outDegree[from].empty()) {
			Position position;
			position.pos = currentNode;
			position.vertex = from;
			positionQueue.push(position);
		}

		last = currentNode;

		from = next;
	}

	if(start != from) {
		throw std::runtime_error("该图上不存在欧拉回路");
	}

	return std::make_pair(first, last);
}

void Graph::eulerTour(const GraphContainer& graph) {
	VertexContainer allVertices = V(graph);
	VertexPtr first = allVertices.front();
	OutDegreeContainer outDegree = graph;

	ListNode* listHead = NULL;
	std::queue<Position> q;
	Position pos;
	pos.vertex = first, pos.pos = NULL;
	q.push(pos);

	for(; !q.empty();) {
		pos = q.front();
		q.pop();

		std::pair<ListNode*, ListNode*> visitResult = eulerTourVisit(pos.vertex, outDegree, q);
		if(pos.pos == NULL)
			listHead = visitResult.first;
		else if(visitResult.first != NULL) {
			// 第一次调用eulerTourVisit时, 使得第一个节点的出度为0, 因此后续
			// 的节点不会插入到第一个节点前面, 也就是说, pos.pos->previous一定不会为NULL.
			pos.pos->previous->next = visitResult.first;
			visitResult.first->previous = pos.pos->previous;

			visitResult.second->next = pos.pos;
			pos.pos->previous = visitResult.second;
		}
	}

	for(ListNode* current = listHead; current != NULL; current = current->next)
		std::cout << current->vertex->dbgVertexNumber << " -> ";

	std::cout << listHead->vertex->dbgVertexNumber << std::endl;

	for(ListNode* current = listHead; current != NULL;) {
		ListNode* next = current->next;
		delete current;
		current = next;
	}
}

VertexContainer Graph::V(const GraphContainer& g) const {
	VertexContainer result;
	for(GraphContainer::const_iterator ite = g.begin(); ite != g.end(); ++ite) {
		result.push_back(ite->first);
	}
	return result;
}

void Graph::BioConnectedComponent::Get() {
	MarkBioConnectedComponent();
}

VertexContainer Graph::BioConnectedComponent::ArticulationPoints() {
	VertexPtr root = NULL;
	VertexContainer articulationPoints;
	LowValueContainer lowValueContainer = ComputeLowValue(root);
	for(LowValueContainer::iterator ite = lowValueContainer.begin(); ite != lowValueContainer.end(); ++ite) {
		VertexPtr vertex = ite->first;
		if(vertex == root && vertex->childrenCount >= 2)
			articulationPoints.push_back(vertex);

		if (vertex != root && lowValueContainer[vertex] > vertex->d)
			articulationPoints.push_back(vertex);
	}

	return articulationPoints;
}

EdgeContainer Graph::BioConnectedComponent::Bridges() {
	VertexPtr root = NULL;
	EdgeContainer bridges;
	LowValueContainer lowValueContainer = ComputeLowValue(root);
	for (LowValueContainer::iterator ite = lowValueContainer.begin(); ite != lowValueContainer.end(); ++ite) {
		VertexPtr vertex = ite->first;
		if (vertex->parent == NULL) continue;

		VertexPtr parent = vertex->parent;
		if (lowValueContainer[vertex] > parent->d)
			bridges.push_back(std::make_pair(parent, vertex));
	}

	return bridges;
}

Graph::BioConnectedComponent::LowValueContainer
Graph::BioConnectedComponent::ComputeLowValue(VertexPtr& root) {
	VertexContainer vertices = graph.V();
	std::for_each(vertices.begin(), vertices.end(), std::mem_fun(&Vertex::Clear));

	LowValueContainer lowValueContainer;
	unsigned time = 0;

	for(VertexContainer::iterator v = vertices.begin(); v != vertices.end(); ++v) {
		VertexPtr vertex = *v;
		if(vertex->color == White) {
			DfsVisit(lowValueContainer, time, vertex);
		}
	}

	root = vertices.front();

	return lowValueContainer;
}

void Graph::BioConnectedComponent::DfsVisit(LowValueContainer& lowValueContainer, unsigned& time, VertexPtr from) {
	from->d = lowValueContainer[from] = time++;
	from->color = Gray;
	VertexContainer outEdges = graph.OutEdges(from);

	for(VertexContainer::iterator ite = outEdges.begin(); ite != outEdges.end(); ++ite) {
		VertexPtr vertex = *ite;
		if(vertex->color == White) {
			vertex->parent = from;
			++from->childrenCount;
			DfsVisit(lowValueContainer, time, vertex);
			lowValueContainer[from] = std::min(lowValueContainer[from], lowValueContainer[vertex]);
		}
		else if(vertex->color == Gray)
			lowValueContainer[from] = std::min(lowValueContainer[from], vertex->d);
	}

	// finish time.
	from->f = time++;
	from->color = Black;
}

void Graph::BioConnectedComponent::DfsVisitOn(GraphContainer& graphContainer, VertexPtr from) {
	from->color = Gray;
	VertexContainer outEdges = graphContainer[from];
	for (VertexContainer::iterator ite = outEdges.begin(); ite != outEdges.end(); ++ite) {
		VertexPtr vertex = *ite;
		if (vertex->color == White) {
			DfsVisitOn(graphContainer, vertex);
		}
	}
}

void Graph::BioConnectedComponent::MarkBioConnectedComponent() {
	EdgeContainer bridges = Bridges();
	
	GraphContainer graphContainer = graph.graphContainer;

	// remove all bridges from graph.
	for (EdgeContainer::const_iterator ite = bridges.begin(); ite != bridges.end(); ++ite) {
		graphContainer[ite->first].remove(ite->second);
	}

	// reset vertex.
	for (GraphContainer::iterator ite = graphContainer.begin(); ite != graphContainer.end(); ++ite) {
		ite->first->Clear();
	}

	for (GraphContainer::iterator ite = graphContainer.begin(); ite != graphContainer.end(); ++ite) {
		VertexPtr vertex = ite->first;
		if (vertex->color == White)
			DfsVisitOn(graphContainer, vertex);
	}
	/*
	MODIFIED-DFS-VISIT(u, nbr)
	color[u] = GRAY
	for each v in Adj[u]
	do if color[v] = WHITE
	then
	if u is cut vertex
	then
	nbr = nbr + 1
	bcc[v] = nbr
	MODIFIED-DFS-VISIT(u, nbr)
	color[u] = BLACK
	*/
}

int main() {
	Graph g;
	const unsigned VertexCount = 6;
	VertexPtr* vertices = new VertexPtr[VertexCount];
	for (size_t i = 0; i < VertexCount; ++i)
		vertices[i] = new Vertex();

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

	//g.EulerTour();

	//g.ArticulationPoint();

	/*g.Bridges();*/
	g.GetBioConnectedComponent();

	return 0;
}

