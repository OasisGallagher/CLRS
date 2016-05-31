void Graph::Signal(GraphContainer& g) const {
	// 对每条边的权值, w1, w2, w3 ... wk.
	// 使w = w1 * w2 * ... * wk最大.
	// 即: log(w) = log(w1) + log(w2) + ... + log(wk).
	// 由于0 <= wi <= 1, 所以log(wi) <= 0, 因此, 
	// 在权值分别为log(w1), log(w2) ... log(wk)的图上计算
	// 最短路径, 得到的是abs(log(w))最大的值.
	// abs(log(w))最大, 即w的值最大, 因为log(w)是单调递增的.
}

