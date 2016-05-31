/**
interval_tree可以确定一维的重叠问题, 对于二维的重叠问题, 就需要另外一个
数据结构维持一个维度上的重叠, 用interval_tree维护另外一个维度的重叠.
两种数据结构都应该满足复杂度为O(nlog(n))(中文版翻译有误, 不是O(log(n)).

用O(nlog(n))的排序维护矩形的x坐标. 因为一个矩形有xl(左x), 和xr(右x), 因此,
对应一个矩形会在队列中出现两次, 一次取xl为排序的键, 一次取xr为排序的键.
在比较时, 如果两个矩形, 一个的xl和另一个的xr相等, 那么将第一个矩形排到前面,
这对处理边界条件来说很重要.
然后遍历这2n个x坐标, 如果遇到xl, 那么将这个矩形插入到interval_tree(通过
ylow和yhigh组成区间, 插入), 如果遇到xr, 那么将矩形移出interval_tree.
对于边界情况, 比如矩形1的xl与矩形2的xr相等, 先插入矩形1, 就可以与矩形2检测
重叠, 再访问矩形2的xr, 将矩形2移出.
在任意时刻, 存在于interval_tree中的矩形在x轴上必然是有重叠的, 再在每次
将矩形插入到interval_tree之前, 检测该矩形的y区间是否与树中的某一个区间重叠,
如果有, 即找到了重叠的矩形.

CHECK_OVERLAP()
	points contains xl and xr of all rectangles.
	sort points. make sure that xl compare less than xr if x1 == xr(
	two distinct rectangles)

	for i = 0; i < count[points]; ++i
		if points[i] is xl
			if this rectangle overlaps with the others in interval_tree
				return true
			insert this rectangle into interval_tree sorted by y interval.
		else
			remove this rectangle from interval_tree

	return false
*/
