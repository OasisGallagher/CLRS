// 由于是离散的概率分布, 下面的inf并不是无穷, 只是表示最值!
// 分成n个桶, 并且使得 (-inf, n1), [n1, n2) .. 各个区间段的与概率分布曲线围成的面积相等.
// 再通过判断X的值, 如果属于(-inf, n1), 那么放在1号桶....