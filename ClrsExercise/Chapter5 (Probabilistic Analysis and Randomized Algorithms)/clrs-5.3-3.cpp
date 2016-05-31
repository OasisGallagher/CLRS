permute_with_all(A)
	n = length(A)
	for i = 1 to n
		do swap(A[i], A[random(1, n)])

对序列{a, b, c}, 为了生成{a, b, c}序列, 每次random(1, n)的结果需要是:
1 2 3
1 3 2
2 1 3
3 2 1
概率为4/27.
而实际上, 每一个序列生成的概率应该都是1/A(3,3) = 1/6.
