/*
(1)
{ 4, 3, 2, 6, 8, 1 }
(2)
证明, 每次执行extracted[j]=i时, 加入到extracted[j]中的i都是正确的.
当i=1时, 该值一定是在之后最近的一次EXTRACT被移出, 如果没有这样的一次
EXTRACT, 那么表示1不会被加入到extracted中.
当i>1时, 其上一次操作, i-1时, 将其所在集合j'中, 所有的元素与之后的第一个有
效集合合并. 也就是说, 剩余的值可能会在之后的EXTRACT时被取到.
每次为extracted[j]赋值之后, Kj都会被destroy, 也就是说, 不会对extracted[j]
赋值多次, 也就是说, 每次设置extracted[j]都是正确的值, 不会二次更新.
程序结束时, extracted中的每个值都是正确的.
(3)
OFF-LINE-MINIMUM(m, n)
1	for i=1 to n do
2		determine j such that i belongs to Kj
3		if j != m+1 then 
4			extracted[j]=i
5			let l be the smallest value greater than j for which set Kl
			exists
6			Kl = union(Kj, Kl), destroying Kj	
7	return extracted
第i个集合内的元素, 调用find_set返回i, i作为它们所处集合的标识.
那么, 第2行可以用:
	j = find_set(i)

*/
