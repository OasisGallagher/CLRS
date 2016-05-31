//
// 算法EXTEND-SHORTEST-PATHS所述, 
// l'(i, j) = min(l'(i, j), l(i, k) + w(k, j)).
// 当W中, w(i, i) = 0, 其它为INF时, 表示l'(i, j) = w(i, j).
// 即, 只有当k = i时, w(k, j)才不为INF, 得到的l(i, k) + w(k, k)才不为INF.
// 因此, 题意中的矩阵, 相当于矩阵乘法中的单位矩阵.
//
