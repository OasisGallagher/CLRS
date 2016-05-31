// ��ۼ������ͼ��˷������ᵽ�Ķ����Ƽ������ķ���һ��.
// O(1), ÿ��������ƽ̯����.
INCREMENT(A)
{
    i = 0
    while i < A.length and A[i] == 1
    {
        A[i] = 0
        i = i + 1
    }

    if i < A.length
    { 
        A[i] = 1
        // Additions to book��s INCREMENT start here.
        if i > A.pointer
            A.pointer = i
    }
    else    // 
        A.pointer = -1
}

RESET(A)
{
    for i = 0 to A.pointer
        A[i] = 0
    A.pointer = -1
}
// RESET�ĸ��Ӷ�Ϊfloor(log(m)) + 1, mΪ��ǰ����������ʾ��ֵ.
// floor(log(m)) + 1 <= m.
// ���, Ϊÿһ����λ��0��1�Ĳ�������Ϊ4.
// ����, 1������λ, 1������pointer, 1�洢��������֮��INCREMENTʱ����λ��0,
// 1�洢����RESET.
