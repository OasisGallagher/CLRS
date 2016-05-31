5.1-1: 本身就是排序过程.

5.1-2:
int random(a, b) {
	return (b - a) * rand01();
}

5.1-3:
biased_random输出1的概率是p, 输出0的概率是1 - p.
利用该方法, 写一个random, 以1 / 2的概率返回1, 1 / 2的概率返回0.
int random() {
	while(true) {
		int ret = biased_random();
		if(biased_random() != ret)
			return ret;
	}
	// 
	return 0;
}
当两次调用biased_random的返回值不同时, 概率恒为p(1 - p).
因此, 返回0和1的概率都是1 / 2.
该分布符合几何分布, 因此第一次成功尝试的期望次数为1 / P.
这里P为成功的概率, 即p(1 - p).
综上, while内的程序期望执行次数为1 / (p(1 - p)).

