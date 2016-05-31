/*
(1)
// O(mn) = O(n) when m is a constant.
void josephus(size_t n, size_t m) {
	if(m == 0 || n == 0)
		throw "invalid param";

	struct node {
		size_t order;
		size_t next;
	};

	node* circle = new node[n]();
	for(size_t i = 0; i < n; ++i) {
		circle[i].order = i + 1;
		circle[i].next = (i + 1) % n;
	}

	circle[n - 1].next = 0;

	
	for(size_t i = n - 1, prev = n - 1; ; ) {
		for(size_t j = 0; j < m; ++j) {
			prev = i;
			i = circle[i].next;
		}

		if(circle[i].next == i) {
			// last one.
			std::cout << circle[i].order << std::endl;
			break;
		}

		std::cout << circle[i].order << std::endl;
		circle[prev].next = circle[i].next;
	}

	delete[] circle;
}
(2)
当m不是常数, 上述的方法就会是O(mn), 但是通过使用顺序统计树, 可以让其效率
成为O(nlog(n)).
先将n个值插入到顺序统计树中.
然后, 取秩为x = (m % n)的节点, 就是第一个值.
接着移除这个值.
再取秩为x = (m + m - 1) % n的节点, 为第二个值.
再取秩为x = (m + (m - 1) * 2) % n的节点, 为第三个值.
以此类推, 第y个节点, 为秩是x = (m + (m - 1) * 2) % n的节点(n为树的当前节
点数, 不是起始的节点数), 直到树为空.
see ./josephus.cpp
 */
