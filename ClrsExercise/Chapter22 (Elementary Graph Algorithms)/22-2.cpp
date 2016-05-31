/*
 A biconnected undirected graph is a connected graph that is not broken
 into disconnected pieces by deleting any single vertex (and its incident 
 edges).
 设G'是G的dfs树.
 a)
 设r时G'的根.
 =>
 如果r只有一个孩子, 那么移除r, 不会影响连通性.
 <=
 如果r有有两个孩子(u, v), 那么移除r之后, u和v之间无法联通.
 因为u和v之间肯定是不联通的, 否则在访问u时, 会访问到v, u和v不可能同时为
 r的孩子.
 b)
 设v是G'中的非根节点.
 =>
 如果v是挂接点, 那么, 必然存在题目中的子节点s.
 如果v不存在子节点, 去掉v不会影响连通性.
 如果存在s, 且s存在backedge, 那么去掉v之后, 可以通过s的backedge访问v的父节
 点.
 <=
 如果存在子节点s, 且没有从s到达v的父节点v0的边, 那么v是挂接点.
 如果v不是挂接点, 即, 去掉v之后, s到v0依然可达, 这与s的后代到v的祖先不可达
 不符.
 c)
 // 
 // see
 // file: graph.hpp
 // procedure: dfs_22_2_c
 //
 // DFS => O(V+E).
 // 由于G时连通的, 所以, E >= V-1.
 // 即, V <= E + 1.
 // 所以, O(V + E) <= O(2E + 1) = O(E).
 d)
 计算low值, 并生成DFS树. 检查DFS树的每个节点v, 如果v时根, 检查它是否有两个
 以上的孩子(a); 否则, 检测v是否存在一个孩子b, 满足low[b] >= d[v](c), 如果
 存在, 证明v是挂接点.
 e)
 如果边(u,v)属于G的简单回路, 那么, 移除(u,v)之后, u和v, 以及其它两两点之间
 依然是可达的, 并不能满足桥的定义.
 f)
 使用c中的low, 遍历dfs树中的节点, 对每个节点v, 遍历其子节点d1, d2 ... dn,  
 如果对于di: low[di]>d[v], 表明v是挂接点(d).所以, (v, di)是桥, 如果去掉该
 边, v和di无法连接.
 (只访问dfs树即可, 因为在无向图中, 只存在tree edge和back edge, tree edge存
 在于dfs树中, back edge会构成环, 不可能是桥(e)).
 g)
 双连通分支包含着G中的非桥边, 因为双连通分支中的任意两条边在一个简单回路之
 内, 如果双连通分支包含桥边, 那么, 就表示任意一条双连通分支内的非桥边和该
 桥边在一个回路之内, 由e可知, 这是不可能的.
 h)

 */
