/**
假设函数COST_SUM(t)以某一个树节点为参数, 返回集合:
{ x, y }, x表示r节点为根的子树的代价; y表示x为根的子树的叶子的频度之和.
COST_SUM(r)
	X = Y = 0
	if left[r] is leaf
		X += f[left[r]]
		Y += f[left[r]]
	else
		set = COST_SUM(left[r])
		X += set.x + set.y
		Y += set.y
	
	if right[r] is leaf
		X += f[right[r]]
		Y += f[right[r]]
	else
		set = COST_SUM(right[r])
		X += set.x + set.y
		Y += set.y
	
	return { X, Y }
 */
