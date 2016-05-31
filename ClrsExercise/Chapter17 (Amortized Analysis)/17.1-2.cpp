/*
DECREMENT(A)
	i = 0
	while i < A.length and A[i] == 0
		A[i] = 1
		i = i + 1;

	if i < A.length
		A[i] = 0
*/
// http://classes.soe.ucsc.edu/cmps102/Fall05/hw/hw8sol.pdf
// `... if A DECREMENT operation ...`, 指的是若干个DECREMENT?
// 最坏情况发生在INCREMENT反转了k bits, 继而DECREMENT, 再接着INCREMENT...
// 使复杂度降低为O(kn).
//
