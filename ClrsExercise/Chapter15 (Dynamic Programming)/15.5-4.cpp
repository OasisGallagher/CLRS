/*
 * 可以将第9行的for循环, 替换为
 * for r = root[i, j-1] to root[i+1, j]
 * 之前需要迭代j-i+1次, 这样只需要迭代root[i+1, j] - root[i, j-1] + 1次.
 * 将j表示为i+L-1
 * 上式为:
 * root[i+1, i+L-1] - root[i, i+L-2] + 1.
 * i=1: root[2, L] - root[1, L-1] + 1
 * i=2: root[3, L+1] - root[2, L] + 1
 * i=3: root[4, L+3] - root[3, L+1] + 1
 * ...
 * i=n-L:root[n-L+1, n-1] - root[i, n-2] + 1
 * i=n-L+1:root[n-L+2, n] - root[n-L+1, n-1] + 1
 * 累加(telescope):
 * sum = root[n-L+2, n] - root[1, L-1] + n
 * 因为1<=root[i, j]<=n, 所以sum<=2n.
 * 因此, 修改后的过程的代价为: O(n) * O(2n) = O(n^2).
 */
