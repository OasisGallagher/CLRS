INTERVAL_FIND_EXACTLY(i)
	x = root
	while x != NIL and x does not overlap i exactly
		if low[i] < low[x]
			if left[x] != NIL and max[left[x]] >= low[i]
				x = left[x]
			else
				return NIL
		else if right[x] != NIL 
			and max[right[x]] >= high[i]
			x = right[x]
		else
			x = NIL

