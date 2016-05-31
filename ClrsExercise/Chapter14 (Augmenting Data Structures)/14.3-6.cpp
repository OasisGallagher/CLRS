/*
在节点内存储min(子树和自身中, 最小的节点值), max(子树和自身中, 最大的
节点值), mingap, 子树和自身中所有节点中, 两个节点的最小差值.
@正确性:
最简单的情况:
    b	
 a     c
这样的BST中, a <= b <= c, 那么GAP(a, c) >= GAP(a, b)且 >= GAP(b, c).
再比较ab和bc中的最值和b的GAP, 得到的一定是这棵树的mingap.
@维护:
mingap[x] = min(
		mingap[left[x]],		// 如果left[x]为NIL, 那么该项为INF.
		mingap[right[x]],		// 如果right[x]为NIL, 那么该项为INF.
		key[x] - max[left[x]],	// ...
		max[right[x]] - key[x]  // ...
	);
在BST的插入过程中, 新节点会被作为叶子, 所以该节点的mingap为INF, min和max为
自身的key. 插入的节点会影响沿路上的节点的三个域. min和max直接比较更新即可;
BST完成之后, 在沿着新节点的父节点, 一路更新mingap(如果发现某个节点为右孩子,
而且mingap没变, 即可停止更新),效率为O(log(n)).
在BST的删除过程中, 已知被删除的节点一定只有一个孩子, 该孩子的三个域不变,
再对该孩子的parent进行类似BST插入时的更新.
就左旋而言:
	x的所有域属性可以拷贝给y.
	x的属性通过上述公式进行更新.

@扩展:
mingap可以返回最小的差值, 也可以找到这两个值.
FIND_MIN_GAP_PAIR()
	let m = mingap
	while x != NIL
		if left[x] != NIL and mingap[left[x]] == m
			x = left[x]
		else if right[x] != NIL and mingap[right[x]] == m
			x = right[x]
		// x的mingap是通过与孩子的最值做差所得.
		// 此时的x必然至少有一个孩子, 否则m不会被计算出来.
		else if left[x] != NIL and GAP(MAX_NODE(left[x], x) == m
			return PAIR(key[MAX_NODE(left[x])], key[x])
		else
			return PAIR(key[x], key[MIN_NODE(x)])
	error("logic error");
*/
