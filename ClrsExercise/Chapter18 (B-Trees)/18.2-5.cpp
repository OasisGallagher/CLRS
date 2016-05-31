/**
整体而言, 需要注意:
(1) 判断叶子节点是否满的方法改为n ?= 4t-1.
(2) 在叶子节点中查找第一个大于等于一个k的位置的方法时需要带入子节点指针
    存储的关键字值.
其它的部分无需改变.

例如:
对于查找操作, 在第2行, 需要判断是不是叶子. 如果是叶子就要在它所有的关键字
和子节点中进行查找.
B_TREE_SEARCH(x, k)
	# c | k | c | k | c | k | c
	if x is leaf:
		search all c and k.
		return true(finded)/false(otherwise).

	# x is not leaf.
	i = 1
	while i <= n[x] and k > key_i[x]:
		do i = i + 1
	if i <= n[x] and k = key_i[x]:
		return true
	
	DISK_READ(c_i[x])
	return B_TREE_SEARCH(c_i[x], k)
*/

