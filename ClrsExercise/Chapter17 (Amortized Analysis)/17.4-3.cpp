// 定义num[i]为第i次操作之后, 结构中的元素个数.
// 定义size[i]为第i次操作之后, 结构的大小.
// 定义f[i]为第i次操作之后, 结构的装载因子, f[i] = num[i] / size[i],
// 2/3 ≤ f[i] ≤ 1.
// 定义Φi为第i次操作之后, 数据结构的势.
// 根据题意, 考虑第i步为DELETE时的势能变化.
// 注意Φ(i) = |2num[i] - size[i]|:
// 只有当f[i] < 1/2时, 2num[i] - size[i] < 0, 
//         { 2num[i] - size[i], for f[i] ≥ 1/2.
// Φ(i) =  |
//         { size[i] - 2num[i], for 1/2 > f[i] ≥ 1/3.
// 考虑:
// (1) f[i-1] = 1/3
//     c^i = ci + size[i] - 2num[i] - size[i-1] + 2num[i-1]
//         = ci + 2size[i-1]/3 - 2(num[i-1]-1) - size[i-1] + 2num[i-1]
//         = ci - size[i-1]/3 + 2 < ci + 2 = 3.
// (2) 1/2 > f[i-1] > 1/3
//     c^i = ci + size[i] - 2num[i] - size[i-1] + 2num[i-1]
//         = ci + size[i] - 2num[i] - size[i] + 2(num[i] + 1)
//         = ci + 2 = 3.
// (3) f[i-1] ≥ 1/2
//     c^i = ci + 2num[i] - size[i] - 2num[i-1] + size[i-1]
//         = ci + 2num[i] - size[i] - 2(num[i] + 1) + size[i]
//         = ci - 2 = -1.
// 综上, 每一个DELETE操作的代价不会超过3.
//
