permute_without_identity(A)
	n = length(A)
	for i = 1 to n - 1
		do swap(A[i], A[random(i + 1, n)])

目的是产生与A不一致的排列.
这个做法显然是不对的.
当A的长度为3时, 理应产生3!-1 = 5种排列.
然而, 在for中, 第一次循环可以交换两个值, 第二次为1个值, 只可以产生2*1=2种
排列.
错误的原因是, 如果第一次第1个位置的值变化了, 第二次就可以不替换任何值, 也
可以产生不同的排列. 而程序没有考虑这个情况.

