/*
 类似9-2的带权中位数, 可以在O(n)时间内确定, 部分背包问题也可以在O(n)时间
 内确定.
 通常的部分背包问题需要先按照物品的价值由高到低排序, 再通过O(n)的循环来确
 定, 需要耗费O(log(n))的时间.
 可以通过partition的方法, 来简化这个过程.
 物品编号为1到n, 背包的容积为W.
 w[i]为物品i的体积, w[m, n]为物品m到物品n的体积和.
 v[i]表示物品i的单价.
 令i=1, j=n.
 (1) 调用k = partition(i, j), compare函数比较两个物品的单价.
     那么v[k-1] <= v[k] >= v[k+1].
 (2) w = sum(w[k, j]).
     if w > W: 表示k到j之间的物品过重, i=k, goto (1).
	 if s == W: print "max value is ", total_v(k, j), return
	 else, w < W:
	     if w[k] + w >= W:
		     lf = w[k] + w - W
		     print "max value is ", total_v(k, j) + lf * v[k]
         else:
		     W -= w[k] + w
			 j = k - 1
			 goto (1)
 假设partition在理想情况下运行, 即总能将数组二分, 那么:
 T(n) = T(n/2) + O(n) # 主方法(3).
 T(n) = O(n).
 */

