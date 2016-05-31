/*
 (a)
 m=3n-1, 即n个MAKE, n-1个GRAFT, n个FIND.
 MAKE和GRAFT各为O(1), 总和为O(2n-1) = O(n).
 如果形成的是一个链表, 那么, 该树的高度为n-1=O(n), 执行n次FIND,
 效率为O(n^2).
 所以总的效率为O(n^2) = O(m^2).
 (b, c, d) see ./depth-determination-problem.cpp
 (e)
 对pseudodistance的维护可以在O(1)时间完成, 所以depth-determination-problem
 可以在O(m*a(n))时间内完成.
 */
