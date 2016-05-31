/*
 对于无向图:
 - (white, white)
   两个端点都没有被访问, 因此作为什么边都有可能.
 - (white, gray)
   由白色端点(w)指向灰色端点(g). 因为g为灰色, 所以g为当前递归栈中的节点,
   如果此时有白色节点指向, 那么该边:
   不可能是树边, 因为不会有访问到(w, g)的路径, g已经不是white.
   可能是一条反向边
   不可能是正向边因为g不会是w的后裔.
   有可能是交叉边, 比如对于a,b,c三个节点的图:
   a: b, c
   b:
   c: b
   dfs(a)时, 先访问b, 此时b为gray, c为white.
   然后访问c, c的parent是a, b的parent也是a, a和b没有父子关系.
 - (white, black)
   由白色端点(w)指向黑色端点(b):
   不可能是树边
   可能是反向边
   不可能是正向边
   可能是交叉边
   与(white, gray)类似
 - (gray, white)
   由灰色端点(g)指向白色顶点(w):
   可能是树边
   不可能是反向边, w必然是g的后裔, 发现w时, g必然是gray.
   可能是正向边.
   不可能是交叉边.
 - (gray, gray)
   两个在调用栈里的端点, g1和g2.
   可能是树边
   可能是反向边
   不可能是正向边
   不可能是交叉边
 - (gray, black)
   由灰色端点(g)指向黑色端点(b), 黑色端点已经被访问完毕, 因此:
   可能是树边, 发生在访问过(g, b)边, 再访问完b之后, g为gray, b为black.
   不可能是反向边, 因为祖先节点不会在子节点之前变为black.
   可能是正向边
   可能是交叉边
 - (black, white)
   由黑色端点(b)指向白色端点(w):
   不可能是树边
   不可能是反向边
   不可能是正向边, 如果二者连通, w不会在b变为black之后依然是white.
   可能是交叉边
 - (black, gray)
   不可能是树边
   可能是反向边
   不可能是正向边
   可能是交叉边
 - (black, black)
   比如树被完全遍历完的时刻, 所有节点的颜色都为black, 所以, 各种类型的边
   都有可能.

 对于无向图, (color1, color2)和(color2, color1)是一致的, 因此, 只需考虑:
 (white, white), (white, gray), (white, black)
 (gray, gray), (gray, black)
 (black, black)
 而且, 无向图的(color1, color2)可以由有向图的
 (color1, color2)∪(color2, color1)
 获得.
 (white, white): 都有可能.
 (white, gray): 都有可能.
 (white, black): 树边和正向边不可能.
 (gray, gray): 正向边和交叉边不可能.
 (gray, black): 都有可能.
 (black, black): 都有可能.
*/

