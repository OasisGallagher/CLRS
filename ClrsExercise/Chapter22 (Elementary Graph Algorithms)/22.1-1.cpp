/*
- 一个顶点的出度即这个顶点的邻接表的长度, 因此, 计算每个节点的邻接表长度需要  O(|E| + |V|).

- 计算入度也可以在O(|E| + |V|)时间内完成.
  只需要遍历每个邻接表一次. 节点x在邻接表中出现一次, 它对应的值+1即可.

- 对于邻接矩阵表示的图, 出度和入度都需要在O(|V|^2)时间内完成.
*/
