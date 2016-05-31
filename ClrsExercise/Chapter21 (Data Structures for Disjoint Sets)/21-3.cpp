/*
 (a)
 执行到第10行时, u必然为BLACK, 表示u以及u的所有子节点都已经被访问过.
 如果此时v依然为黑色, 表示v以及它的子节点也已经被访问过, 那么输出它们的LCA
 之后, 不会再访问v.
 如果此时v不为黑色, 那么, 会在访问v时, 继续访问{v, u}组合, 与第一种情况相
 同, u以及它的子节点已经被访问过, 输出它们的LCA, 不会再访问u.
 因此, 对一组{u, v}, 第10行只会被执行一次.
 (b)
 初始状态, root的depth为0, set数为0.
 第2行, MAKE-SET, set数为1.
 第3行, 访问root的子节点, 它们的深度比root多1, 所以, 在下一次递归进入时,
 set数为1.
 当下一次递归结束后, 会将子节点新产生的集合和父节点的集合进行UNION, 从而
 又减少一个集合, 使集合数依然为1, 再递归到root的其它子节点.
 所以, 每次递归调用LCA前的集合数, 为v的深度. LCA递归调用结束后, 集合数多1,
 但是, 通过UNION, 使集合数又减1, 保证了下一次递归调用u的其它孩子时, 集合
 数依然等于它的深度.
 (c)
 每个节点, 在访问过它的所有子节点之后, 都会形成一个set, 包含该节点和
 其所有的子节点, 代表为该节点, 代表的祖先也是该节点.
 case 1:
 当u和v有一个是root时, 肯定是正确的: 只有当u和v, 颜色都为BLACK时, 才会输出,
 而要等到root的颜色为BLACK, 就需要所有的节点都遍历完成, 此时仅剩下一个集合,
 即所有节点的集合, 代表的祖先就是root.
 case 2:
 不失一般性, 考虑一棵二叉树, 如果u和v分居该树的某一个高度高于u和v的节点x的
 两侧.
 那么, 在第3行遍历过u所在分支之后, v依然是WHITE, 所以, 不会输出.
 u所在分支和x的集合UNION, 形成新的集合, 代表的祖先是root.
 在遍历过v及其所有子节点之后, 此时u为BLACK, 就会在第9行输出u所在集合代表的
 祖先, 即x.
 case 3:
 如果u和v在x的某一个分支上, 那么, 仅需考虑该分支即可, 它们的LCA一定是这个
 分支上的某点.
 如果二者是兄弟关系或者叔侄关系, 等同于case 2, 返回它们分支处的节点.
 如果二者是父子或者爷孙等关系, 比如u是v的父节点, 需要等同于case 1, u就是
 "root"节点.
 综上, 该算法是正确的.
 (d)
 最坏情况下, 需要遍历树中的所有节点, 为O(n).
 LCA一开始, 为每一个节点执行一次MAKE-SET和一次FIND-SET. 并为所有的子节点各
 执行一次UNION和一次FIND-SET, 所以, 共n次MAKE-SET和2n-1次FIND-SET和n-1次
 UNION. 共4n-2次操作.
 所以, 总的代价即这4n-2次集合操作的代价:
 O((4n-2)a(n-1)) = O(n*a(n)).
 */
