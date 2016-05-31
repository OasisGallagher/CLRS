// In graph theory of directed graphs, a graph is said to be 
// strongly connected if every vertex is reachable from every other vertex. 
//
// In graph theory, a connected component (or just component) of an 
// undirected graph is a subgraph in which any two vertices are connected 
// to each other by paths, and which is connected to no additional vertices
// in the supergraph.
//
// 简言之, 连通分量是对无向图而言, 强连通分量对有向图而言.
// 二者皆表示, 在分量内部, 任意两个点之间是可达的(存在路径).
//
// 初始化c = 0
// 每次从dfs调用dfs_visit时, ++c
// 在dfs_visit中, 将访问到的点的c域置为c(在dfs_visit开始, 置color为Gray处).
// 在经过dfs调用dfs_visit时, dfs_visit中访问的节点都是相互可达的(仅适用于
// 无向图), 而另外一次通过dfs调用的dfs_visit访问到的节点与这一次访问到的节
// 点之间一定是不可达的.
// 因此, dfs结束之后, c域相同的节点在同一个连通分量中.
// 最终的c值, 就是G的连通分量的个数.
//

