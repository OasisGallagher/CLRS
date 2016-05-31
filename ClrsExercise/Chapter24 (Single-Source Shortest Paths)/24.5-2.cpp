/*
A <--> B
 \   /
   C
存在边, (C, A), (C, B), (A, B), (B, A).
且权值为w(C, A) = w(C, B) = 2, w(A, B) = 0.
那么, 生成的最短路径树可以为: (C, A), (C, B).
或者: (C, B), (B, A).
同样以C为起点, 一个包含(C, B), 一个不包含.
*/
