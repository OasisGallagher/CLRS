/*
 假设从v开始bfs.
 当从queue中取出w时, 
 如果w的邻接表是: t, x, 那么, u的parent就是t.
 如果w的邻接表是: x, t, 那么, u的parent就是x.
 
 但是, 无论顺序如何, u的depth是不变的, 
 depth[u] = depth[t] + 1 = depth[x] + 1.
 必然是以邻接表中的一个点作为其父节点, 这些邻接表中的点的depth是相等的.
 */
