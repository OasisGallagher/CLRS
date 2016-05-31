void Graph::Fixed(GraphContainer& g, WeightContainer& weight, unsigned max_w, VertexPtr src) const {
	// 在某时刻, heap中的最小值为x.
	// 在这一时刻之前, 上一次heap中pop出的最小值为y, 那么, 在对y的邻接点进行relax时, 
	// 邻接点的最大值为y+max_w.
	// 因此, 当x为最小值时, y+max_w-x <= max_w, 因为y<=x.
	// 所以, 当x为最小值时, 最大值至多为x+max_w, 加上INF, 任意时刻heap中的不同值至多有max_w+2个.
	// 因此可以维护一个堆, 每个节点为一个链表, 之中存储所有key值相同的节点.
	// 见 instructor's manual.
}

