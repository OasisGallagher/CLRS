foreach u in Adj[v0]
	u.d = 0

// 以v0为起点的边, 只需要松弛一次. 因为没有边进入v0, v0开始的边一定是最短
// 路径的第一条边.
remove all edges from G start from v0

for i = 1; i < |V[G]|; ++i
	foreach (u, v) in E[G]
		relax(u, v, w)

// 也可以在开始前直接去掉v0以及v0连接的边(v0, v), 并将v的初始d值置为0.
