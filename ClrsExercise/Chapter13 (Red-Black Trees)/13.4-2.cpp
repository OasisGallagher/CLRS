// 如果x和p[y]都是红色, 那么根据删除时的原理, p[y]是红色, x是红黑的(
// 即x有一重黑色).
// 在fixup时, 会跳过循环, 直接把x置为黑色, 从而是性质4得到满足.
// 而且, 这样在x的分支上, 增加了一重黑色, 从而使该分支上的黑节点数与
// 之sibling分支上的黑节点个数相同.
//
