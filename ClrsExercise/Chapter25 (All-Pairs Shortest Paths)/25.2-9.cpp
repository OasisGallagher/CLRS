// 计算G' = SCC[G].
// G' = {V', E'}, V'中的一个元素, 表示顶点集, 对于该顶点集中的每一对顶点, 
// 都是连通的, 加入到传递闭包中.
// 在G'上(G'一定是DAG), 运行题意中的方法, 得到另外的一些边, 这些边是连接
// 各个V'中的两个元素的, 而非G中的边.
// 因此, 需要转换为G中的边.
// 如果V'中的边, (V'1, V'2)∈E', 有V'1中的任一点u到V'2中的任一点v, 都是传
// 递闭包中的路径.
// 即:
// foreach (u, v) ∈ E'
//     v1 = V'[u.SubGraphIndex]	// SCC会计算每个节点在SCC的V'中的索引,
//     可以根据这个索引, 得到V'中的一个集合.
//     v2 = V'[v.SubGraphIndex]
//     foreach p in v1
//         foreach q in v2
//             container.Add(p, q).
// cost = cost(SCC) + O(E* + V) + f(V+E) = f(V+E) + O(E* + E + V).
// ∵ E* >= E
// ∴ cost = f(V+E) + O(E* + V).
//
