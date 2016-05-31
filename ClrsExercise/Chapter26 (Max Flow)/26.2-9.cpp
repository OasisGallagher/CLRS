// 需要计算边连通度的无向图图记为G, 另外构造与之对应的有向图G', 包含G中的所有点,
// 边(u, v)∈G'且(v, u)∈G'当且仅当(u, v)∈G(G为无向图), 如果边存在, 则边的权值为1.
// 对一组顶点u和v∈G', 分别记为s和t, 并在在G'上求最大流, 根据最大流最小割定理, 
// 最大流即为最小割的容量, 即穿过S, T这一最小割的由S指向T的边的数目.
// 需要注意的是, 由于G'中, 如果边(u, v)存在, 那么边(v, u)一定存在, 所以
// 为了取得顶点u和v作为s和t, 可以任取u, 再取V - {u}作为v. 从而在|V - 1| < |V|
// 个流网络中调用最大流算法.
/*
function EDGE-CONECTIVITY(G)
	select any u from G;
	min = int.MinValue;
	construct G';
	foreach v in V - {u}	
		min = Min(MAX-FLOW(G', u, v), min)
	return min
end
*/
// 设实际的边连通度为k, x = EDGE-CONECTIVITY(G), 那么, k <= x, 因为如果
// 将最小割上的x条正向边移除, 那么, G必然不连通.
// 对任意的p和q∈V - {u, v}, 不存在p为s, q为t, 且MAX-FLOW小于x.
// 如果存在这样的p和q, 那么必然存在一个割, 一个割包含u, 也就是说, 这种情况
// 一定在遍历u为s, v∈V-{u}时出现过, 因此, 不存在p, q, 使MAX-FLOW小于x.
// 因为EDGE-CONNECTIVITY计算得出最小割的容量x, 因此, 为了使图不连通, 必然要
// 移除不少于x条边, 否则, 就会出现另一个割, 容量为y, y < x, 显然与x为最小割的
// 容量矛盾. 因此, k >= x.
// 因此, k <= x && k >= x得到k == x.
