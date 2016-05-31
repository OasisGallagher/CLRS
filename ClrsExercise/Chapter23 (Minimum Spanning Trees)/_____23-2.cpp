MST_REDUCE(G, orig, c, T) {
	// T is empty.
	// for each edge (u, v) in E, orig[u, v] = (u, v), c(u, v) = w(u, v).
	for each v in V(G) {	// O(V).
		mark[v] = false;
		ds.MakeSet(v);
	}
	
	// MakeSet: V.
	// UnionSet: E.
	// FindSet: 2E+V.

	// ������.
	for each u in V(G) {	// O(V).
		if (!mark[u]) {
			// O(E).
			choose v in Adj[u] such that c[u, v] is minimised;
			ds.UnionSet(u, v);
			T += { orig[u, v] };
			mark[u] = mark[v] = true;
		}
	}
	
	// ������.
	// orig2[u, v]��¼���������������ϵ�Ȩ����С��ԭͼ�еı�.
	V[G2] = { ds.FindSet(v) : v in V[G] };
	E[G2] = Empty;
	for each(x, y) in E[G] {	// O(E).
		u = ds.FindSet(x);
		v = ds.FindSet(y);
		if (u, v) not in E[G2] {
			E[G2] += { (u, v) };
			orig2[u, v] = orig[x, y];
			c2[u, v] = c[x, y];
		}
		else if (c[x, y] < c2[u, v]) {
			orig2[u, v] = orig[x, y];
			c2[u, v] = c[x, y];
		}
	}

	construct adjacency lists Adj for G2;
	return G2, orig2, c2 and T;
}

/*
(a)	TΪ�ڵ��ù�REDUCE�ϵķ���ֵ֮һ, ��ôT�а��������������������, (u,v), ����
	��ԭͼ��, v��ʹ��w(u, v)��С���ڽӵ�. T��Ȼ��ԭͼ��MST���Ӽ�.
	��orig'��, ��¼��Ŀ(u, v)��ʾ, ��uΪ����ļ��ϵ���vΪ����ļ��ϵ���СȨֵ��.
	ȡorig'[x, y], ��(x,y)����ԭͼ��MST, ������T�е�������������, Ҳ��������
	T���������㼯�����, չ���������㼯(����Ԫ��֮����ͨ), ����������, ���µ�MST
	�ǰ�ȫ��.
	��A�е����������ڵ㶼ִ����������, �����յ�ͼ����ͨ���޻���.
	����, T and {orig'[x, y] : (x, y) belongs to A(MST of T)}�������µ�ԭͼ��MST.
(b) G'�б���������ɸ����ϵĴ���, ���ں���V���ڵ��ԭͼ, ��V�е����гɶԵĽڵ㶼��
	��G'��ʱ, V[G'] = V[G]/2, ����, ��Ȼ��V[G'] < V[G]/2.
	���, V[G'] <= V[G]/2.
(c) see below: MST_REDUCE_O_E.
(d) ִ��k��MST_REDUCE_O_E�Ĵ���Ϊ, �ȱ��������, a1 = E, q=1/2, 2E*(1-(1/2^k))
*/

// ÿ�ε�UnionSet���ǽ�һ���ڵ�, ���뵽��һ��������(������Ԫ�� >= 1), ���, Ϊÿ���ڵ�
// ά��һ����, representative, ֱ��ָ���������ϵĴ���.
MST_REDUCE_O_E(G, orig, c, T) {	// O(V+E), ϡ�����ͨͼ��, ��ΪV=E, ���Դ���ΪO(E).
	for each v in V(G) {	// O(V).
		mark[v] = false;
		v.representative = v;
	}

	// ͨ��BFS, ���ÿ��u���ڽӵ�v, ʹ��c[u, v]��u�������ڽӵ�����С.
	// ����m��.
	// O(V+E).

	// ������.
	for each u in V(G) {	// O(V).
		if (!mark[u]) {
			//choose v in Adj[u] such that c[u, v] is minimised;
			v = m[u];
			v.representative = u.representative;
			T += { orig[u, v] };
			mark[u] = mark[v] = true;
		}
	}

	// ������.
	// orig2[u, v]��¼���������������ϵ�Ȩ����С��ԭͼ�еı�.
	V[G2] = { v.representative : v in V[G] };
	E[G2] = Empty;
	for each(x, y) in E[G] {	// O(E).
		u = x.representative;
		v = y.representative;
		if (u, v) not in E[G2] {
			E[G2] += { (u, v) };
			orig2[u, v] = orig[x, y];
			c2[u, v] = c[x, y];
		}
		else if (c[x, y] < c2[u, v]) {
			orig2[u, v] = orig[x, y];
			c2[u, v] = c[x, y];
		}
	}

	construct adjacency lists Adj for G2;
	return G2, orig2, c2 and T;
}
