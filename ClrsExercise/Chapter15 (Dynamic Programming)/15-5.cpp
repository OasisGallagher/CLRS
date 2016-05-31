/**
(a):
b[1], b[2] ... b[k]表示字符序列.
题目中并没有说明它们是互异的.
define queue Q.
define ways count: wc = 0
FIND_PATH(x, i)
	if(i == k + 1)
		++wc
		print Q
	for each y in x's adj-table:
		if d(x, y) == b[i]
			push y
			FIND_PATH(y, i + 1)
			pop y

call FIND_PATH(v[0], 1)
if wc == 0
	print "no such solution"
##########################################
在一条从v[0]到v[i]的长度为k的路径上(i不一定等于k, 可能访问G中的某个点多次)
的一点j, 从v[0]到v[j]的边的概率乘积一定也是最大的.
形式化地:
令c(0, i)为边数目为k的, 概率乘积最大的路径的最大概率值.
对于之中的一个顶点k, c(0, k)一定是v[0]到v[k]的最大概率值.
有递归式:
c(0, k) = 对所有k的前驱, m to n, max(p = m to n)(c(0, p))

*/

