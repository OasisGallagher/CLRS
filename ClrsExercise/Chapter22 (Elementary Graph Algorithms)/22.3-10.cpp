/*
 u可以存在自环.
 或者:
 w <- u <- v
 那么:
 dfs_visit(w), 将w置为black, u和v为white.
 接着访问u, v依然是white.
 最终, 三者都在仅含有一个节点的dfs树中.
 */

