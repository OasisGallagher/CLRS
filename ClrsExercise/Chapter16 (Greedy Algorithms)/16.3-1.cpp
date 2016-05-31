/*
 * 对于一颗不满的二叉树(满二叉树指树中的任意内节点, 都有两个孩子), 取之中
 * 只有一个孩子的内节点(没有孩子的节点是叶子), 完全可以将它的单一孩子代替
 * 自己的位置, 并删除这个节点, 那么, 树的代价会降低.
 * 因此, 一个最优的前缀编码树, 不可能是一颗不满的二叉树.
 */
