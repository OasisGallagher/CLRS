5.2-1:
在hire_assistant中, 正好调用N次, 即应聘者按照递增的顺序出现.
这种情况的概率为1 / n!.
5.2-2:
如果正好雇用两次, 第一次雇佣的一定是第一次出现的应聘者. 
假设第二次雇佣的是第k个应聘者(显然这里k > 1, 否则只需要一次应聘. 而且第k
个应聘者是最优的应聘者, 否则之后还会发生招聘), 那么, 在[2, k - 1]轮面试中,
必然没有发生应聘, 即这k - 2个应聘者都弱于第一个应聘者.
第k个应聘者满足上述条件的概率是(n-1)/n.
在[1, k - 1]轮面试中, 必须保证第一个面试者是这些面试者中最优的. 概率是
1/(k - 1).
因此, 面试两次的概率为: 
(n-1)/n * (1/1 + 1/2 + ... 1/(n-1)).
5.2-3:
骰子每次投掷结果的样本空间{1, 2, 3, 4, 5, 6}, 而且, 随机变量X为这次投掷
的值, 服从平均分布, 因此E(X) = (1 + 2 + ... + 6) / 2 = 11.5
因此, n次的期望和为23/(2n).
5.2-4:
定义指示器随机变量Xi = I{第i位顾客拿到自己的帽子}.
因此, n位顾客拿到自己的帽子的期望值就是X1 + X2 + ... + Xn
Xi的期望为, 第一位顾客拿到自己的帽子的概率为1/n, 第二位拿到自己的帽子的
期望为: 第一位顾客没拿到第二位顾客的帽子的概率 * 第二位顾客拿到自己的帽子
的概率, 即(n-1)/n * 1/(n-1) = 1/n, 以此类推, Pr{Xi = 1} = 1/n.
因此, 拿到自己帽子的顾客的期望数为: n/n = 1.
5.2-5:
定义指示器变量Xij={A[i]和A[j]是逆序对}, 因此逆序对个数的期望值就为:
C(n, 2) * E(Xij = 1).
E(Xij = 1)为A[i]和A[j]是逆序对的指示器变量的期望, 即A[i]和A[j]是逆序对
的概率, 即1/2.
因此,期望的逆序对的个数为n(n-1)/4.


