// (1)
/*
DYNAMIC-UPDATE(u, v)
	for i = 0; i < n; ++i
		for j = 0; j < n; ++j
			if w(i, u) && w(v, j)
				w(i, j) = true
*/
// 
// (2)
// 对链式的环v1, v2 ... vk, v1 == vk, 当更新到v1 ... v(k-1)之后, 加入vk, 
// 使得任意的vi, 都可以访问到vj, 其中j < i, 也就是说, 环的出现使每个顶点都
// 可以访问到其他的索引比自己小的顶点, 更新这些所耗时为O(V^2).
//
// (3)
/*
DYNAMIC-UPDATE(u, v)
	for i = 0; i < n; ++i
		if m(i, u) and !m(i, v)	# 如果i到v已经可达, 那么没有继续的必要.
			for j = 0; j < n; ++j
				if w(v, j)
					m(i, j) = true

G中有V^2个顶点, 因此外层的for执行O(V^3)次.
内层循环只有m(i, v) == false时才执行, 而该值只会被设置为true, 之后便不再执行.
因此, 内层的循环一共执行O(V^2)次.
因此, cost = O(V^3).
*/ 

