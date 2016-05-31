MATRIX_CHAIN_MULTIPLY(A, s, i, j)
	if(j != i)	// more than one matrix.
		k = s(i, j)
		r = MATRIX_CHAIN_MULTIPLY(A, s, i, k)
		MATRIX_MUL(r, MATRIX_CHAIN_MULTIPLY(A, s, k + 1, j))
	return r

