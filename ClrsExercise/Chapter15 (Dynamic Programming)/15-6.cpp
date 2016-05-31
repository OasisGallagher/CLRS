/**
令c[i, j]为从第i行第j列的格子, 到顶部格子中任意一个格子的最高分数.
那么:
          { c[i-1, j] + p((i-1, j), (i, j))		if i > 1(always)
c[i, j] = | c[i-1, j-1] + p((i-1, j-1), (i, j))	if i > 1 and j > 1
          { c[i-1, j+1]	+ p((i-1, j+1), (i, j))	if i > 1 and j < n

# main loop looks like:
for row = n to 1
	for col = 1 to n
		...
O(n^2).
*/
