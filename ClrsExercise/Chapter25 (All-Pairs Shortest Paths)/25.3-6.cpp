// G中存在, (u, v), (p, v), 以u作为起点, δ(v) != INF, δ(p) = INF.
// 那么, w'(p, v) = w(p, v) = δ(p) - δ(v) = INF.
// 最终, δ(p, v) = INF - (δ(p) - δ(v)) = 0.
// 显然是错误的.
// 如果图G是强联通的, 即从任一点对图中的其他点可达, 效果与加入一个
// super vertex s的效果一致, 因此正确.
//
