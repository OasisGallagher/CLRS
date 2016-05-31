// http://stackoverflow.com/questions/20265527/intersection-of-n-circles
// 用(p, q, r)表示圆, 其中(p, q)为圆心坐标, r为半径.
// 每个圆转换为线段(p - r, q), (p + r, q).
// 排序这些点, 运行监测线段相交的程序.
// 当将一个圆加入树中时, 按照圆心的y坐标排序.
// 
