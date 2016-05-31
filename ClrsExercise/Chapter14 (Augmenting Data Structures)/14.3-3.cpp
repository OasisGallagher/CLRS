/*
 * 14.3-3
 * 找出与i重叠的, 具有最小低端点的区间.
 * call the follwing procedure with FIND_MIN_INTERVAL(root, i).
 * FIND_MIN_INTERVAL(n, i)
 *     if LEFT(n) != NIL and MAX[LEFT[x]] >= LOW[i]
 *	       # find smaller.
 *         y = FIND_MIN_INTERVAL(n, LEFT(n))
 *         # the smaller exists.
 *         if y != NIL
 *             return y
 *         elseif n overlaps i
 *             return n
 *         # 满足第一个if之后, 如果在左子树中不存在这个点, 那么在右子树
 *         # 这个点一定也不存在(CLRS P.189, 循环不变式的对称性证明).
 *         else
 *             return NIL
 *     # left subtree doesn't contains a overlapped interval.
 *     elseif n overlaps i
 *         return n
 *     # otherwise, try right subtree.
 *     return FIND_MIN_INTERVAL(RIGHT(n), i)
 */
