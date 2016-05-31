/**
 * (a) F(h) <= n, 求解h约为1.44*log(n+2)-0.328
 *     (http://en.wikipedia.org/wiki/AVL_Trees)
 * (b) BALANCE函数实现, 如果插入插入节点之后, AVL树变得不平衡, 那么:
 *     abs(blance(l_child), blance(r_child))=2
 *     不平衡时差值的绝对值一定为2, 否则之前的AVL树就不合法.
 *     比如左孩子的高度更高, 那么, 通过沿着root右旋, 即可以减少左孩子的高度
 *     而且增加右孩子的高度.
 *     然而, 右旋过后, root的左孩子的右孩子会成为原root节点的左孩子, 从而
 *     出现在新树的右孩子的左孩子(具体见右旋的定义), 从而使原root的左分支
 *     的高度减少过多, 因此可以先将这种情况的右孩子先左旋, 这样, 再绕着这个
 *     这个右孩子的高度就会减一, 左孩子的高度就会加一, 二者依然相差1, 但是
 *     此时是左孩子更高, 再进行右旋就可以了.
 *     root右旋.
 *     HEIGHT(x)
 *	       if x == Nil:
 *	           return -1
 *         return max(HEIGHT(L(x)), HEIGHT(R(x))) + 1
 *     #end of HEIGHT.
 *
 *     CHILD_HEIGHT_DIFF(x)
 *         return HEIGHT(L(x)) - HEIGHT(R(x))
 *     #end of CHILD_HEIGHT_DIFF
 *
 *	   BLANCE(x)
 *	       diff = CHILD_HEIGHT_DIFF(x)
 *	       if abs(diff) > 2:
 *	           error("invalid AVL tree")
 *         
 *         if diff == 2:	#left child is higher.
 *             d2 = CHILD_HEIGHT_DIFF(L(x))
 *             if d2 < 0:   #right child is higher.
 *                 LEFT_ROTATE(P(x))
 *             RIGHT_ROTATE(root)
 *         else	#right child is higher.
 *             d2 = CHILD_HEIGHT_DIFF(R(x))
 *             if(d2 > 0): #left child is higher.
 *		           RIGHT_ROTATE(P(x))
 *		       LEFT_ROTATE(root)
 *	  #end of BLANCE.
 * (c) 按照二叉树插入节点, 然后从这个节点至根, 调用BLANCE.
 *     AVL_INSERT(x, z)
 *	       if data[x] < data[z]:
 *	           if L[x] != NIL:
 *	               AVL_INSERT(L[x], z)
 *	               c = L[x]
 *	           else:
 *			       P[z] = x
 *			       L[x] = z
 *			       H[z] = 0
 *			       c = z
 *	       else:
 *	           if R[x] != NIL:
 *	               AVL_INSERT(R[x], z)
 *	               c = R[x]
 *	           else:
 *	               P[z] = x
 *	               R[x] = z
 *	               H[z] = 0
 *	               c = z
 *		   H[x] = H[c] + 1
 *         BALANCE(x)
 *     #end AVL_INSERT
 *     #O(h)的效率, 而且调用O(h)次BLANCE, 旋转至少h次, 即至少log(n)次.
 */

