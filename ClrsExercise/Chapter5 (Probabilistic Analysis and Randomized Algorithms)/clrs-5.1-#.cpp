5.1-1: ��������������.

5.1-2:
int random(a, b) {
	return (b - a) * rand01();
}

5.1-3:
biased_random���1�ĸ�����p, ���0�ĸ�����1 - p.
���ø÷���, дһ��random, ��1 / 2�ĸ��ʷ���1, 1 / 2�ĸ��ʷ���0.
int random() {
	while(true) {
		int ret = biased_random();
		if(biased_random() != ret)
			return ret;
	}
	// 
	return 0;
}
�����ε���biased_random�ķ���ֵ��ͬʱ, ���ʺ�Ϊp(1 - p).
���, ����0��1�ĸ��ʶ���1 / 2.
�÷ֲ����ϼ��ηֲ�, ��˵�һ�γɹ����Ե���������Ϊ1 / P.
����PΪ�ɹ��ĸ���, ��p(1 - p).
����, while�ڵĳ�������ִ�д���Ϊ1 / (p(1 - p)).

