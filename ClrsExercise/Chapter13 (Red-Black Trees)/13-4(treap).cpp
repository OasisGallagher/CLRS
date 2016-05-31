// (a) 证明: 给定x[1], x[2] ... x[n], 以及他们的优先级priority[1],
//     priority[2] .. priority[n], 存在唯一的treap与这些节点关联.
// (b) 证明treap的期望高度是O(log(n)).
// (c) 解释TREAP_INSERT如何工作并给出伪代码.
//     TREAP_INSERT(x)
//	       insert x into treap use bst_insert(x)
//	       if p[x] == NIL:
//		       root = x
//		       return;
//	       for(; p[x] != NIL && priority[p[x]] > priority[x]; x = p[x]):
//	           if(L[p[x]] == x):
//	               RIGHT_ROTATE(p[x])
//	           else:
//			       LEFT_ROTATE(p[x])
//	           x = p[x]
//	   先按照bst的性质, 将节点x插入到树中.
//     再比较x与其父节点的优先级, 调整最小堆性质.
//     如果父亲的优先级高, 那么需要以父亲为轴, 旋转, 调整优先级.
//     如果x是左孩子, y为x的父亲, 那么进行右旋后, x的兄弟z成为x的右孩子(y)
//     的右孩子, 因为priority[z] > priority[y] > priority[x], 所以旋转后, 
//     满足最小堆. 同理, 原x的右孩子u成为y的左孩子, 因为u原就是y的后代, 
//     经过旋转后成为的x的孩子, 所以priority[u] > priority[y].
//     同理可得当x是右孩子的情况.
// (d) 旋转次数, 最多为treap的高度, 即(b)中证明的O(log(n)).
// (e) 证明在插入x的过程中的旋转次数, 等于:
//     插入x后, x的左孩子的右脊柱长度 + x的右孩子的左脊柱长度.
//
