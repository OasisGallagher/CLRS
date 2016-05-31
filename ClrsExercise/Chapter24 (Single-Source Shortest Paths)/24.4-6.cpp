/*
 分别加入边
 w(u, v) = n
 w(v, u) = -n,.
 即为约束方程加入:
 x_v - x_u <= n
 x_u - x_v <= -n 即 x_v - x_u >= n
 二者联立, 表示x_v - x_u = n.
 */
