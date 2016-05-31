template <class Ty>
class fixed_minimum_heap {
	typedef std::list<Ty> bucket_type;
	typedef std::vector<bucket_type> container_type;
	const size_t Infinite;
	container_type cont;
	size_t min;
	size_t count;
public:
	template <class InputIterator>
	fixed_minimum_heap(InputIterator first, InputIterator last, size_t w)
		:Infinite(w + 1), min(0) {
		cont.resize(Infinite + 1);
		cont[Infinite].insert(cont[Infinite].begin(), first, last);
		count = std::distance(first, last);
	}

	Ty pop_top() {
		for (; cont[min].empty(); ++min)
			;

		VertexPtr result = cont[min].front();
		cont[min].pop_front();
		--count;
		return result;
	}

	void decrease(Ty v, unsigned value) {
		// 保证是在递减值, 且递减不低于当前的min.
		assert(v->d > value && value >= min);
		unsigned bucket_index = std::min(Infinite, get(v));
		cont[bucket_index].remove(v);
		set(v, value);
		cont[value].push_front(v);
	}

	bool empty() const {
		return count == 0;
	}
};

void Graph::Fixed(GraphContainer& g, WeightContainer& weight, unsigned max_w, VertexPtr src) const {
	VertexContainer vertices = V(g);
	// 任意情况下, 节点的d值(非INF)最大为:
	// W = max_w(V-1), 当所有边权重都为max_w, 且为链式时.
	fixed_minimum_heap<VertexPtr> heap(vertices.begin(), vertices.end(), max_w);
	// src->d = 0;
	for (; !heap.empty();) {
		// u为heap中key最小的, 且在对u的邻接节点relax时, 被relax的节点
		// 一定在heap内, 即它们在relax前的key值一定不小于u的key值.
		// relax时, 将被relax的节点的key值改为key[u]+tmp, tmp>=0,
		// 因此, 一个u的邻接节点在relax之后, key值依然不会小于u的key.
		// 所以, 每次从heap中取得的最小值, 是不递减的.
		// V+W, 因为heap内的min索引是不递减的, 共O(W)次.
		VertexPtr u = heap.pop_top();
		VertexContainer adj = g[u];
		for (VertexContainer::iterator ite = adj.begin(); ite != adj.end(); ++ite) {
			VertexPtr v = *ite;
			unsigned tmp = weight[std::make_pair(u, v)];
			// O(1), 如果在VertexPtr中, 维护节点信息.
			if (u->d + tmp < v->d)
				heap.decrease(v, u->d + tmp);	
		}
	}

	// O(V+W+E) = O(max_w * V + E).
}
