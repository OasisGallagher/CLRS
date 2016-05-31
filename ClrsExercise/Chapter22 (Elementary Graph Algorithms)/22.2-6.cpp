/**
for each vertex u in V[G]
	if color[u] == WHITE
		BFS(u)

for each vertex u in V[G]
	if d[u] % 2 == 0
		print u + "is good guy"
	else
		print u + "is bad guy"

对于第一个for循环, 执行到BFS(u)的次数 = G中子图的个数.
每次BFS(u)的效率为O(E_i+V_i), 所有BFS的效率为O(E+V).
再加上两次for循环, 效率为O(V), 总的时间复杂度为O(E+V).
*/