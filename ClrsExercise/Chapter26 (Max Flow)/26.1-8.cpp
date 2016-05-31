// 定义n = |V|, 为G中顶点的个数, 每个顶点为vi, 1 <= i <= n.
// 定义变量Xij, 表示f(vi, vj), 满足Xij <= c(i, j), Xii = 0, Xij = -Xji.
// 为使流入等于流出, 需要约束条件:
// sum(Xij, for i = 1 to n) = sum(j, k, for k = 1 to n).
// 以上使得Xij满足流的条件.
// 目标函数为max(X1i, for i = 2 to n), 即源点的输出流最大.
// (或者max(Xin, for i = 1 to n-1)).
