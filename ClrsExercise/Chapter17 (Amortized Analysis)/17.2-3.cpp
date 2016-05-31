// 与聚集分析和记账方法中提到的二进制计数器的方法一致.
// O(1), 每个操作的平摊代价.
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
        // Additions to book’s INCREMENT start here.
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
// RESET的复杂度为floor(log(m)) + 1, m为当前计数器所表示的值.
// floor(log(m)) + 1 <= m.
// 因此, 为每一个将位从0置1的操作代价为4.
// 其中, 1用来置位, 1用来置pointer, 1存储下来用作之后INCREMENT时将该位置0,
// 1存储用作RESET.
