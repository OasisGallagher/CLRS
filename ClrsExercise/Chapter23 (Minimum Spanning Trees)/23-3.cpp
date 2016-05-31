/*****
 (a) BST的value, 即kruskal算法最后加入的边的权重(因为kruskal按照边的权重
     递增的顺序加入边). 在加入最后一条边e之前, e连接的两个分量是不连接的,
	 而且, 这两个分量的轻边必然是e, 不可能存在比e的权值还小的边, 否则,
	 在e之前, 这两个分量就已经连接.
	 因此, 其它的ST为了连接这两个分量, 也不可能选择到比e的权值更小的边,
	 所以, MST的value一定是所有生成树中最小的.
 (b) 移除所有权值大于b的边, 如果G'依然连通, 那么表示它的BST的value必
     然 <= b.
	 TEST(G, b)
	     edges = E(G)
		 edges.RemoveAll(e => w[e] > b)	# O(E)
		 construct G' from edges
		 run DFS on G' to test whether it's connnected # O(E+V)
 (c)
 from 22-2.cpp import MST_REDUCE(G, orig, c, T)
 while G has more than one vertex
	 let e = E[G]
	 find the edge with median weight[m] from e
	 remove all edges weight greater than m(they are 'f'), we get G'
	 test if G' is connected
	 if yes
		 G = G'
	 else
		 reduce G' with MST_REDUCE
		 construct new graph G with reduced G' and left edged named f
 0 如果图G中只有一个点, 那么展开收缩的边和点, 得到BST.
 1 找到具有中位数权值的边m.
 2 移除G中权值大于w[m]的边.
 3 如果图G依然连通, 那么, 重复上述步骤.
   否则, 缩减G中的边, 将连通分支收缩为一个点. 再添加上2中被移除的边,
   重复上述步骤.
 */
