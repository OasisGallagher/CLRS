// (a)
/// <summary>
/// 返回一个数组A, A[i]表示pattern的[0, i]子串的循环节的最大长度.
/// </summary>
/// <cost>O(n), n为pattern的长度</cost>
public int[] Repetition()
{
	int[] rep = new int[pattern.Length];
	for (int i = 1; i <= pattern.Length; ++i) 
	{
		// 假设pi从1开始索引.
		// i为当前检查的字符串的长度, 下图字符串的总长.
		// j = pi[i]为长度为i的子串的最长后缀的长度, 下图重叠部分的长度.
		// k = i - j为剩余的非后缀的长度, 如下图所示.
		// ************<- k->
		// <- k->************
		// 因为i % k = 0, 因此可以将P分割, 每段为k个字符.
		// Pk表示P的k前缀.
		// 令下面的字符串为Q, 且Q = P, 那么, Pk = Qk.
		// 又由于Qk是P(2k)的后缀, 因此:
		// Qk = Pk = P(k+1, 2k] = Q(k+1, 2k] = P(2k + 1, 3k] ...
		// 因此, P可以分割为若干个长度为k的子串, 且两两相等.
		// 而且, 该方法分割出的k一定是最小的, 因为Pj是Pi的最长后缀
		// (使得k = i - j得到最小值).
		if (i % (i - pi[i - 1]) == 0)
			rep[i - 1] = i / (i - pi[i - 1]);
	}

	return rep;
}

