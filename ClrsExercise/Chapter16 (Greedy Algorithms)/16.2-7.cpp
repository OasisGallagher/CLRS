/*
 遍历所有的情况, 代价为O((n!)^2).
 该问题具有最优子结构, 假设对一个a和b中的一个元素索引k, 对剩余的元素取最优
 连乘值, 再乘以pow(a[k], b[k])一定也是最优的.
 下面证明其贪心性质:
 将数组a和b按照递减的顺序排序.
 考虑两对索引, i>j, m>n, 分别是a和b数组的索引.
 那么pow(a[i], b[m]) * pow(a[j], b[n])以及pow(a[i], b[n]) * pow(a[j], b[m])
 设k = m-n > 0, 那么:
 pow(a[i], b[m]) * pow(a[j], b[n]) = 
     pow(a[i], b[n]) * pow(a[j], b[n]) * pow(a[i], k)
 pow(a[i], b[n]) * pow(a[j], b[m]) = 
     pow(a[i], b[n]) * pow(a[j], b[n]) * pow(a[j], k)
 
 可知:
 pow(a[i], b[m]) * pow(a[j], b[n]) > pow(a[i], b[n]) * pow(a[j], b[m])
 所以, 按照递减排序的顺序, 排列a和b数组, 得到的乘积最大.
 */
