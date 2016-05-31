//
// 对于一个图G的邻接矩阵表示, M, 通过类似矩阵乘法的方法, 得到所有节点的最
// 短路径.
// 为了计算点i开始的单源最短路径, 维护一个向量V, 令V[i] = 0, 其它值为INF,
// 维度=n, 因此, V和M是相容的.
// 计算VM^(n-1).
// 从第i次乘法之前, V表示从i到各个节点的最短路径, 长度至多为i-1.
// 当进行过n-1次之后, 计算出所有的最短路径(最短路径长度至多为n-1, 当图
// 没有负环时).
// 得到的就是单源最短路径.
// 因此, 在进行向量和矩阵乘法时, 也是将所有的边都进行relax.
// (因为一次乘法,遍历所有的从(i, j)的组合, 并遍历该路径).
// 乘法执行n-1次, 就得到了类似BELLMAN-FORD的最短路径.
// 