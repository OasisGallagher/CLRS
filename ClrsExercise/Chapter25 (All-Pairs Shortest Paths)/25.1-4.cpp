//
// 对于3个矩阵, A, B, C, 如果:
// ABC = A(BC)
// 那么, EXTEND-SHORTEST-PATHS定义的矩阵乘法满足结合律.
// 考虑两个矩阵, X, Y的积, Z:
// Z(i, j) = min(X(i, k) + Y(k, j)), 对k = [1, n].
// 那么, AB的结果:
// P(i, j) = min(X(i, k) + Y(k, j)), 对k = [1, n].
// PC的结果为:
// Q(i, j) = min(P(i, k) + Y(k, j)), 对k = [1, n].
// 将P(i, k)替换得到:
// Q(i, j) = min(X(i, k) + Y(k, j) + Y(k, j)), 对k = [1, n].
// 同理, 计算A(BC)会得到相同的结果.
// 因此, 由EXTEND-SHORTEST-PATHS定义的矩阵乘法满足结合律.
//
