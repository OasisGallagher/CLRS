/*
 * key: 单独维护这四个域.
 * 对每个节点维护successor和presuccessor指针;
 * 对树, 维护maximum和minimum指针.
 *
 * <successor_and_presuccessor>
 * 插入x后, 设置x的successor和presuccessor域, 通过调用树的__successor和
 * __presuccessor.
 * 过程类似于将x插入到一个链表中, 置x的前驱和后继, 然后再取得x的前驱, 设置
 * 其后继为x, 最后取x的后继, 置其前驱为x.
 * 删除x时, 取x的前驱, 设置其后继指向x的后继; 取x的后继, 设置其前驱指向x
 * 的前驱.
 * 旋转不会影响前驱和后继.
 * </successor_and_presuccessor>
 *
 * <maximum_and_minimum>
 * 在整棵树内维护这两个指针, 每次插入时, 通过比较来决定是否改变.
 * 删除时, 如果删除的是最小值, 那么取其后继, 为新的最小值.
 * 如果删除的是最大值, 取其前驱为新的最大值(如果取得到了NIL, 表示树已为空).
 * </maximum_and_minimum>
 *
 * sol2:
 * 注意到max, min, successor和presuccessor的求法, 都是沿着树的脊柱
 * (see treap)下降或上升. 那么可不可以通过记录脊柱的起点和终点, 使上述操作
 * 的效率提高到O(1)?
 */
