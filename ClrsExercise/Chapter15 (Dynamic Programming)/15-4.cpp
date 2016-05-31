/** mine
员工个数为n.
将员工按照登记从低到高的排序(O(nlog(n))).
员工i喜爱聚会的评分为A(i).
定义c(i, j)为从索引在[i, j]之间的参加聚会员工的喜欢聚会程度的最大值.
那么, 最优化问题为找到c(1, n).
假设选中了员工k, 为L[k] = i, 那么, 所有i+1和i-1的员工不能参加.
如果L[k+1] - L[k] == 0 or >= 2, 那么k+1可以参加聚会.
同理, 对于L[k-1] - L[k] == 0 or >= 2, k-1也可以参加聚会.
因此, 如果k+1和k-1都可以参加聚会, 那么:
c(i, j) = c(i, k-1) + A(i) + c(k+1, j).
如果L[k+1] == L[k], 那么, 需要找到 > k+1的第一个满足L差不小于2的员工,
设为m, 将上式中的k替换为m即可.
综上所述:
		  { 0 otherwise
c(i, j) = |
          { max(k=i to j)(c(i, p) + A(k) + c(q, j)) i <= j

其中, p和q分别为小于k的和大于k的第一个与k的等级差不小于2的员工.
*/

/* see ./hw15and16.pdf for more details.
这个问题可以在线性时间内解决.
定义员工数为n, 员工按照题意中树的层数的顺序从左向右排列.
定义c[i]为从员工i到员工n的员工的最大聚会喜爱度的值, 那么本题需要得到
c[1]的值.
假设从节点x开始, x的索引为k, 那么:
       { A[k] + sum of c[i to j], i to j为k的孙子节点. 如果k被选择.
c[k] = |
       { sum of c[m to n], m to n为k的子节点.
定义两个备忘录:
memo1[x]为邀请x时, 以x为根的树的最大喜爱度的值.
memo2[x]为不邀请x时, 以x为根的树的最大喜爱度的值.
因此可得递归方法:

MAX_CONVIVIALITY(x)
	memo1[x] = A[x]
	memo2[x] = 0
	lc = LEFT_CHILD(x)
	while lc != NIL
		MAX_CONVIVIALITY(lc)
		# x is chosen.
		memo1[x] += memo2[lc]
		# x is discarded.
		memo2[x] += max(memo1[lc], memo2[lc])
		lc = NEXT_SIBLING(lc)
	return max(memo1[x], memo2[x])

// 每个节点都只被访问一次, 代价为O(n).

INVITE(x)
	z = LEFT_CHILD[x]
	if memo1[x] > memo2[x]
		print x
		z = LEFT_CHILD[z]
	
	while z != NIL
		INVITE(z)
		z = NEXT_SIBLING(z)
*/

