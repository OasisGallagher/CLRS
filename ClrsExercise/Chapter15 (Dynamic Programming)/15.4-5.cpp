// nΪ�ַ�������.
// OptimalSubstructure:
// m[i]Ϊ��i��n����������еĳ���.
// jΪi֮���, a[j]����a[i]���ַ�������(i < j <= n).
//        { max(i < j <= n)(m[j] + 1).	if i < n.
// m[i] =
//        { 0							if i > n.
// ��ʼ�� m[1, n] = 1
//
// CLRS solution:
// ȡ��һ���ַ���, Ϊs2, ����s2, ��s��s2ȡLCS, ��Ϊ����.
// ����ΪO(log(n)) + O(n^2) = O(n^2).
//
