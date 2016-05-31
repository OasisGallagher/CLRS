foreach Vertex v in V(G) {
	key[v] = INF;
	parent[v] = NULL;
}

PriorityQueue q = V(G) sorted by increasing key;
for arbitrary vertex u, let k = 0;

while(!q.empty()) {	// O(V).
	u = q.pop();	// O(log(V)).
	array line = matrix[u];
	for(int v = 0; v < line.size(); ++v) {	// O(V).
		if(line[v] != INF && q.contains(v) && i.k > w(u, v)) {
			parent[v] = u;
			key[i] = w(u, v);
		}

		if(parent[u] != NULL)
			result.push(Edge(parent[u], u));
	}
}

// O(V^2).

