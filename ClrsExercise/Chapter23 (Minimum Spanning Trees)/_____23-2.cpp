MST_REDUCE(G, orig, c, T) {
	// T is empty.
	// for each edge (u, v) in E, orig[u, v] = (u, v), c(u, v) = w(u, v).
	for each v in V(G) {	// O(V).
		mark[v] = false;
		ds.MakeSet(v);
	}
	
	// MakeSet: V.
	// UnionSet: E.
	// FindSet: 2E+V.

	// 缩减点.
	for each u in V(G) {	// O(V).
		if (!mark[u]) {
			// O(E).
			choose v in Adj[u] such that c[u, v] is minimised;
			ds.UnionSet(u, v);
			T += { orig[u, v] };
			mark[u] = mark[v] = true;
		}
	}
	
	// 缩减边.
	// orig2[u, v]记录的是连接两个集合的权重最小的原图中的边.
	V[G2] = { ds.FindSet(v) : v in V[G] };
	E[G2] = Empty;
	for each(x, y) in E[G] {	// O(E).
		u = ds.FindSet(x);
		v = ds.FindSet(y);
		if (u, v) not in E[G2] {
			E[G2] += { (u, v) };
			orig2[u, v] = orig[x, y];
			c2[u, v] = c[x, y];
		}
		else if (c[x, y] < c2[u, v]) {
			orig2[u, v] = orig[x, y];
			c2[u, v] = c[x, y];
		}
	}

	construct adjacency lists Adj for G2;
	return G2, orig2, c2 and T;
}

/*
(a)	T为在调用过REDUCE上的返回值之一, 那么T中包含的是这样的若干组边, (u,v), 满足
	在原图中, v是使得w(u, v)最小的邻接点. T必然是原图的MST的子集.
	而orig'中, 记录条目(u, v)表示, 从u为代表的集合到以v为代表的集合的最小权值边.
	取orig'[x, y], 对(x,y)属于原图的MST, 是连接T中的两个顶点的轻边, 也就是连接
	T中两个顶点集的轻边, 展开两个顶点集(集合元素之间连通), 并加入该轻边, 对新的MST
	是安全的.
	对A中的任意两个节点都执行上述操作, 是最终的图是连通且无环的.
	所以, T and {orig'[x, y] : (x, y) belongs to A(MST of T)}生成了新的原图的MST.
(b) G'中保存的是若干个集合的代表, 对于含有V个节点的原图, 当V中的所有成对的节点都存
	入G'中时, V[G'] = V[G]/2, 否则, 必然有V[G'] < V[G]/2.
	因此, V[G'] <= V[G]/2.
(c) see below: MST_REDUCE_O_E.
(d) 执行k次MST_REDUCE_O_E的代价为, 等比数列求和, a1 = E, q=1/2, 2E*(1-(1/2^k))
*/

// 每次的UnionSet都是将一个节点, 加入到另一个集合中(集合内元素 >= 1), 因此, 为每个节点
// 维护一个域, representative, 直接指向所属集合的代表.
MST_REDUCE_O_E(G, orig, c, T) {	// O(V+E), 稀疏的连通图中, 认为V=E, 所以代价为O(E).
	for each v in V(G) {	// O(V).
		mark[v] = false;
		v.representative = v;
	}

	// 通过BFS, 获得每个u的邻接点v, 使得c[u, v]在u的所有邻接点中最小.
	// 存在m中.
	// O(V+E).

	// 缩减点.
	for each u in V(G) {	// O(V).
		if (!mark[u]) {
			//choose v in Adj[u] such that c[u, v] is minimised;
			v = m[u];
			v.representative = u.representative;
			T += { orig[u, v] };
			mark[u] = mark[v] = true;
		}
	}

	// 缩减边.
	// orig2[u, v]记录的是连接两个集合的权重最小的原图中的边.
	V[G2] = { v.representative : v in V[G] };
	E[G2] = Empty;
	for each(x, y) in E[G] {	// O(E).
		u = x.representative;
		v = y.representative;
		if (u, v) not in E[G2] {
			E[G2] += { (u, v) };
			orig2[u, v] = orig[x, y];
			c2[u, v] = c[x, y];
		}
		else if (c[x, y] < c2[u, v]) {
			orig2[u, v] = orig[x, y];
			c2[u, v] = c[x, y];
		}
	}

	construct adjacency lists Adj for G2;
	return G2, orig2, c2 and T;
}
