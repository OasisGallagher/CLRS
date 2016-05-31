SOLVE(M)
	for i = 0; i < n; ++i
		for j = 0; j < n; ++j
			if i == j or m(i, j) == INF
				¦Ð(i, j) = NIL
			else
				for k = 1; k < n; ++k
					if (m(i, k) + w(k, j) == m(i, j) and k != j)
						¦Ð(i, j) = k
