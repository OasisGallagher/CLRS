// 
// 在EXTEND-SHORTEST-PATHS中, 
// C(i, j) = min(C(i, j), A(i, k) + B(k, j))
// 改为:
/*
// π(i, j)表示, 从i到j的最短路径上, j的前驱.
if A(i, k) + B(k, j) < C(i, j)
    C(i, j) = A(i, k) + B(k, j)
	π(i, j) = k

for each pair (i, j)
	π(i, j) = NIL
*/
//

