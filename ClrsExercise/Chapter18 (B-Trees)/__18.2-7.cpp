/*
 B_TREE_SEARCH存取磁盘页面数为
 O(h * x) = O(log_t(n) * x)
 其中, x为读取磁盘页的时间, x = at + b.
 总的查找时间为:
  O(h * (at + b) * t)
= O(log_t(n) * (at^2 + bt))

 let a = 5, b = 10
 O(log_t(n) * (5t^2 + 10t))
 */
