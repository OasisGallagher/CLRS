/**
for each vertex u in V[G]
	if color[u] == WHITE
		BFS(u)

for each vertex u in V[G]
	if d[u] % 2 == 0
		print u + "is good guy"
	else
		print u + "is bad guy"

���ڵ�һ��forѭ��, ִ�е�BFS(u)�Ĵ��� = G����ͼ�ĸ���.
ÿ��BFS(u)��Ч��ΪO(E_i+V_i), ����BFS��Ч��ΪO(E+V).
�ټ�������forѭ��, Ч��ΪO(V), �ܵ�ʱ�临�Ӷ�ΪO(E+V).
*/