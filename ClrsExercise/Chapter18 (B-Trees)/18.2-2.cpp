/*
 B树每个操作开始前, 只有root存在于主存中.
 由于每次写入的都是修改过的节点, 因此DISK_WRITE操作没有冗余.

 在调用B_TREE_INSERT时, 如果根为满的, 那么ALLOCATE_NODE产生新根R', 并将旧的
 根R作为第1个孩子, 插入到R'的子节点表中.
 SPLIT之后, 调用B_TREE_INSERT_NONFULL, 此时的R和R'都是存在于主存中的.
 在B_TREE_INSERT_NONFULL中, 如果x不是叶子, 且在第9, 10行过后, i=1, 即需要
 插入到新根R'的第1个孩子内, 这个孩子就是R, 是已存的, 再进行DISK_READ就是
 冗余的.
 */
