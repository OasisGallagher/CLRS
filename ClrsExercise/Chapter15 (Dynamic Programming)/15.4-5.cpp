// n为字符串长度.
// OptimalSubstructure:
// m[i]为从i到n的最长递增序列的长度.
// j为i之后的, a[j]大于a[i]的字符的索引(i < j <= n).
//        { max(i < j <= n)(m[j] + 1).	if i < n.
// m[i] =
//        { 0							if i > n.
// 初始化 m[1, n] = 1
//
// CLRS solution:
// 取另一个字符串, 为s2, 排序s2, 对s和s2取LCS, 即为所求.
// 代价为O(log(n)) + O(n^2) = O(n^2).
//
