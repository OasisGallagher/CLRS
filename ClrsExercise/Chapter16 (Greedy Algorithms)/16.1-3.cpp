/*
 1: 低效的做法:
    循环调用GreedyActivitySelector, 直到集合为空, 效率为O(n^2).
 2: 更高效的做法:
    共有n个活动, 那么至多需要n个教室, 即可完成分配.
    定义一个freelist, 之中的元素代表一个教室.
	将n个活动的起止时间按照递增的顺序排序, 得到一个2n长度的数组.
	采用14.3-7中类似的扫描的方法, 遍历这n个元素:
	如果遇到活动x的起始时间, 那么, 在freelist中分配一个教室(freelist得到的
	一定是可用的, 最后一次被归还的教室, 可以参见第10章的freelist), 并将活
	动x置于其中举行, 记录该教室在另一个列表b中;
	如果遇到的是活动x的结束时间, 那么在b中找到并删除x所在的教室, 归还到
	freelist中.
	freelist的最大使用量, 就是需要的教室数目.
	使用freelist使得每次分配教室时, 分配到一个使用过的教室, 而不是分配一个
	新的教室.
	freelist可以保证:
	被访问过的元素数目 = freelist最大使用量 = 最多需要的教室数.
	证明:
		如果freelist最大使用量 > 被访问过的元素数目, 比如最多使用了5个元素,
		而被访问的元素数目只有3个, 显然是不可能的.
		如果freelist最大使用量 < 被访问过的元素数目, 比如最多使用了x个元素,
		而被访问的元素数目有y个, x<y, 那么当freelist到达最大使用量x时, 还
		有y-x个元素没有被访问到. 那么当着y-x个元素被访问到时, freelist的用
		量一定已经不小于x了, 否则不会再去访问新的元素. 就可以推出最大使用量
		大于x, 这与x是最大使用量时矛盾的.
		因此: 被访问过的元素数目 = freelist最大使用量.
	代价为O(nlog(n)) + O(n).
当然, 使用freelist只是为了更清晰的说明问题, 实际上, 不使用freelist, 而是
直接进行计数也是可行的.
CLASSROOM_NEED(ac)
	n = count of activities.
	freelist f
	normal list b
	for i = 1 to n
		insert s[i] and f[i] into list v
	sort v
	# let fs denotes the elements used in freelist.
	needed = 0
	for i = 1 to 2n
		if v[i] is start time of activity x
			alloc classroom c from freelist
			# needed = max(needed, ++fs)
			needed = max(needed, freelist.size)
			mark activity[c] = x
			push c into b
		else # v[i] is finish time of activity x
			find class room c which activity[c] = x
			remove c from b
			dealloc c and put c back to freelist
			# --fs
	
	return needed
 */
