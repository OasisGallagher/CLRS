permute_by_cyclic(A)
	n = length(A)
	offset = random(1, n)
	for i = 1 to n
		do dest = i + offset
			if dest > n then
				dest = dest - n
			B[dest] = A[i]
	return B

 ��ΪoffsetΪ[1, n]���κ�һ�����ĸ��ʶ���1/n, ���, ��i=1ʱ, Ԫ��A[1]
 �䵽B������λ�õĸ��ʶ���1/n. ��i > 1ʱ, A[i]����B������λ�õĸ�����A[1]
 ����B������λ�õĸ�����ͬ(��Ϊ��һ��offset������A[1]��λ��, ͬʱҲ������
 ֮�������.
 ��Ȼ, �������ֻ���Բ���n������. ��Ϊoffset��������������, ��offsetֻ����
 ȡ��n��ֵ.

