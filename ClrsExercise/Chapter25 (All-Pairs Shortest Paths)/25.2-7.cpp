//              { i, if w(i, j) != INF
// φ(0)(i, j) = |
//              { INF, otherwise.
//				
//				{ k, if d(k-1)(i, k) + d(k-1)(k, j) <= d(k-1)(i, j)<contains k>
// φ(k)(i, j) = |
//              { φ(k-1)(i, j), otherwise.
//
/*
PRINT-SHORTEST-PATHS(φ, i, j)
	if i == j
		print i
	else if φ(i, j) == NIL
		print "no path from i to j"
	else
		k = φ(i, j)
		# [unsolved] k will be printed twice.
		PRINT-SHORTEST-PATHS(φ, i, k)
		PRINT-SHORTEST-PATHS(φ, k, j)
*/
