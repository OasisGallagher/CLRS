/*
 才用线性查找的B树的效率为:
 O(th) = O(t*log_t(n))	h为树的高度, n为B树的关键字个数.
 如果将查找换做二分查找, 那么, 效率为:
 O(log(t) * h) = O(log(t) * log_t(n))
 将log_t(n)换做2为底:
 log(n) / log(t)
 所以, 
 O(log(t) * h) = O(log(t) * log(n) / log(t)) = O(log(n)).
 可见, 采用二分查找后, 时间复杂度仅与节点个数有关!
 */
