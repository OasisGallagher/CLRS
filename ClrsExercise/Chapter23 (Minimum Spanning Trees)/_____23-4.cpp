/*****
 (a) 该方法是正确的.
     假设T是该算法计算出来的MST, 它必然是无环的, 如果有环的话, 移除之中的边
	 不会影响连通性.
	 运用反证法, 设存在另一棵的MST, T', 使得w[T'] < w[T].
	 根据瓶颈生成树(23-2)的相关推论, T'的最大权值的边e', 一定不大于T的最大
	 权值的边e.
	 所以, 当处理T中, 权值大于w[e']的边时, 该边是可以被移除的, 因为存在T'
	 不包含该边, 依然可以生成连通树. 因此, 边e在T中是多余的, 致使T中存在环.
	 与T的定义不符, 因此, 不可能存在T', 因此, T是MST.
	 sort all edges in O(V*log(V)) time.
	 foreach e in edges in sorted(nonincreasing) order
	     run dfs on G-e to test if it's connected in O(V+E) time
		 if yes
		     remove e from G
	totally, O(V*log(V)) + O(E*(V+E)) = O(E^2) cos' E >= V-1 for a connected
	graph.
 (b) 加入的边不一定总权值最小.
     如
	     a
	    / \
	   b - c
	 设:
	 w(a, b) < w(a, c) < w(b, c), 如果(b)取得边的顺序与该顺序相反,得到的
	 不是MST.
	 使用disjoint sets, 来检测环. 即, 对边(u,v), 如果:
	 FIND_SET(u) == FIND_SET(v), 证明, 加入(u,v), 会产生环.
 (c) 该方法是正确的. 
     C方法生成的MST, 记做T; 如果存在另一个MST, T', 使得w[T'] < w[T]. 那么,
	 移除T'中的, 不属于T的边e', 将e'加入到T中, 使得T中产生环, 且该环中, e'
	 的权值一定大于其他边(根据C方法). 也就是说, T'中, 去掉e'后的两个分量可以通
	 过更小的边连接, 这与T'是MST矛盾. 因此, 不存在T', 使得w[T'] < w[T], 所以T
	 是MST.
	 void Test() {
	     G2 = Empty;
	     foreach e in E[G] {	// O(E)
	         G2 += e;
			 # O(V), there are at most V vertices and V-1 edges in G2;
	    	 run dfs to detect whether G2 is acyclic;
	    	 if yes, say it's (u, u) {
		         m = find the edge with max weight(use 'parent' field);
		    	 remove m from G2;
		     }
	     }
	     return G2;
	 } // O(VE).
 */
