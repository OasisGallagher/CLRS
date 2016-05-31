/*
 E_pi包含E中的一些边, 并且, E_pi中包含s->v的最短路径的边.
 但是, E_pi却不能通过BFS得到.

   / b - c
 a     \
   \ d - e
 如图所示, 如果E_pi中包含: (a, b), (b, c), (a, d), (d, e), 且起点为a,
 那么, E_pi中包含s->v的最短路径的边(v belongs to {a, b, c, d, e}),
 但是, (b,c)和(d,e)边不可能在一次BFS中同时被遍历到.
 */
