// 性质1, 不一定满足:
// (f1+f2)(u, v) = f1(u, v) + f2(u, v) <= c(u, v) + c(u, v) = 2c(u, v).
// 
// 满足性质2:
//(f1 + f2)(u, v) = f1(u, v) + f2(u, v) = -f1(v, u) - f2(v, u)
//                = -(f1(v, u) + f2(v, u)) = -(f1+f2)(v, u).
// 满足性质3:
// 设u∈V-s-t.
// sum(f1+f2)(u, v)(v∈V) = f1(u, v1) + f2(u, v1) + f1(u, v2) + f2(u, v2) ...
// = sum(f1(u, v)) + sum(f2(u, v)) = 0.
