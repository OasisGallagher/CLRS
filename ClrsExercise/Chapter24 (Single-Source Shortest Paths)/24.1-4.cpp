/*
在检测环时:
foreach (u, v) in E(g)
	if(u.d + w(u, v) < v.d)
		container.add(v)

foreach v in container
	v.d = -inf

由于会遍历图中的所有边, 且把d值置为-inf, 因此, 方法结束后, 所有在负环上的点
的d值都被置为-inf.
*/
