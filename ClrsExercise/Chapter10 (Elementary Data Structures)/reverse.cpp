REVERSE(L)
	for(q = head[T], p = NIL; q != NIL; )
		r = next[q]
		next[q] = p
		p = q
		q = r

