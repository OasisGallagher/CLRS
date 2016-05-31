/*
 (a)
 硬币分别为25, 10, 5, 1.
 对于总需找零的面值, t, 如果t使用了索引不小于i的硬币, 但是, 存在
 面值为v[i-1]的硬币, 且v[i-1]<=t没有使用. 那么通过证明该i-1的硬币
 加入到找零之中, 得到的总的找零硬币数最少, 来证明, 该零钱面值的找
 零方案是贪心的, 也就是, 取最大的不大于需要找的零钱的面值即可.
 证明的关键在于, 已经选择的不包括v[i-1]的集合S中, 一定可以找出多个
 硬币, 它们的面值和恰好为v[i-1], 那么, 将这些硬币替换为i-1#硬币, 总
 的找零的硬币数就会减少.
 分情况讨论.
 (b)
 与上述证明过程类似, 该问题设零钱的面值分别为c0, c1, c2 ... cn,
 当n >= c(i+1), 如果S使用了c0 ~ ci的面值, 个数为a0 ~ ai, 那么, 有:
 n = a0c0 + a1c1 + ... + aici	(1)
 对于每个ai, 必然 < c, 即 <= c-1, 否则, 可以使用1个a(i+1)代替这c个ai, 
 可以减少c-1个硬币数量.
 所以, (1) <= (c-1)(c0 + c1 + ... ci) = (c-1)((1-c^(i+1)) / (1-c)) = c^(i+1) -1
 < c^(i+1)
 所以, n < c^(i+1).
 与假设矛盾.
 所以, 如果不使用c^(i+1), 不能得到最优解.
 因此, 该问题是贪心的.
 (c)
 零钱的面值为6, 5, 1.
 找零10, 那么, 贪心解为: 6, 1, 1, 1, 1.
 实际最优解显然是5, 5.
 (d)
 找换硬币, 是一个动态规划的问题.
 假设硬币的面值个数为k, 第i个硬币的面值为v[i], 总的找换面值为n.
 那么, c[i](i<=n)表示:
 找换面值为i, 需要的最少找换硬币数.
 那么:
		{ 0 : if i = 0
 c[i] =	|
        { min(j=1 to k and v[j]<=i)(c[i - v[j]]) + 1

 因为对任意的最优方案, 拿掉一个硬币, 在剩下的总面值中找零, 得到的结果一定
 也是最优的.
 CALC_CHANGE()
	 c[i] = 0
	 let s[x] denotes the index of coin need to give for total x.
	 for i = 1 to n
		c[i] = INF
		for j = 1 to k
			# or sort the coins in increasing order.
			if v[j] > i
				continue
			t = 1 + c[i-v[j]]
			if t < c[i]
				c[i] = t
				# need to give one coin[j]
				s[i] = j
	 return c[n]

 GIVE_CHANGE(t, s)
	if t > 0
	    print "Give one coin indexed ", s[t]
		GIVE_CHANGE(t - v[s[t]], s) 
 */
