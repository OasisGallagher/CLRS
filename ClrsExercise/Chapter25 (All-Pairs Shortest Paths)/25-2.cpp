// (1)
// INSERT: log(d)(n) = log(n^α)(n) = 1/α.
// EXTRACT-MIN: d/α, 因为有从d个孩子中, 查找最小值的操作.
// DECREASE-KEY: 1/α.
// (2)
// n*cost(EXTRACT-MIN) + E*cost(DECREASE-KEY).
// 通过(1)的提示, 令d = n^ε.
// 因此, cost(EXTRACT-MIN) = d/ε, cost(DECREASE-KEY) = 1/ε.
// cost = n^(ε+1) / ε + E/ε = O(n^(ε+1) + E) = O(E). (|E| = O(n^(ε+1)).
// (3)
// 对V个顶点调用(2)中的方法, cost = O(VE).
// (4)
// 使用Jhonson算法.
//
