// 对i为2的整数幂, 收取2i-1的费用.
// 其中, i费用用来支付第i部操作的实际消耗.
// 剩余的用来支付2^(n+1) - 2^n - 1 = 2^n - 1 = i-1个其他操作.
// 因此, 其他操作的平摊代价为0.
// 所以, 总的代价为:
// sum(2^i, i = 0, 1, ... ceil(log(n))) = O(n).
//
