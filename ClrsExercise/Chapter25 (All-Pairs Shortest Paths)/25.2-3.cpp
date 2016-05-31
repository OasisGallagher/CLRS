/*
COMPUTE-SHORTEST-PATHS(D)
	for i = 0; i < n; ++i
		for j = 0; j < n; ++j
			if w(i, j) != INF
				π(i, j) = i
			else
				π(0)(i, j) = NIL

	for k = 1; k < n; ++k
		for i = 0; i < n; ++i
			for j = 0; j < n; ++j
				# 如果包含不k时, 更优.
				if π(k-1)(i, j) <= π(k-1)(i, k) + π(k-1)(k, j)
					π(k)(i, j) = π(k-1)(i, j)
				else
					π(k)(i, j) = π(k-1)(k, j)
*/

