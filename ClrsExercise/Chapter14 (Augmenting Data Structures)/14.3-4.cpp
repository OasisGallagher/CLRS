/**
INTERVAL_SEARCH_ALL(i, x)
	if x == NIL
		return empty_container

	if x overlaps i
		insert x into container
	
	if left[x] != NIL and max[left[x]] >= low[x]
		container = INTERVAL_SEARCH_ALL(left[x])
		// CLRS P.189, 循环不变式的对称性证明
		if container is empty
			return container
	
	if right[x] != NIL and max[right[x]] >= low[x]
		container = INTERVAL_SEARCH_ALL(right[x])

	return container
*/
