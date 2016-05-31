m[i]为末端为i的, 总和最大的连续序列的最大和.
	   { a[1]								i = 1
m[i] = { arr[i] + m[i - 1]					m[i - 1] >= 0
	   { arr[i]								m[i - 1] < 0

max(m[i], for i = 1 to n)即为所求.

