permute_by_cyclic(A)
	n = length(A)
	offset = random(1, n)
	for i = 1 to n
		do dest = i + offset
			if dest > n then
				dest = dest - n
			B[dest] = A[i]
	return B

 因为offset为[1, n]中任何一个数的概率都是1/n, 因此, 当i=1时, 元素A[1]
 落到B中任意位置的概率都是1/n. 当i > 1时, A[i]落在B中任意位置的概率与A[1]
 落在B中任意位置的概率相同(因为第一次offset决定了A[1]的位置, 同时也决定了
 之后的序列.
 显然, 这个方法只可以产生n种排列. 因为offset决定了整个序列, 而offset只可以
 取得n个值.

