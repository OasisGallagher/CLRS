#include <iostream>
#include <functional>
#include <algorithm>
#include <numeric>

// clrs 9-2.
// 
// 1) 如果N个数的权重都是1 / N, 假设中位数在K位置, 那么, K = ceil(N / 2),
//    那么K左边的数的权重和必然 < 1 / 2. 这些数的权重和 + 中位数的权重
//    必然 >= 1 / 2.
//    所以, 其他的数字的权重和必然 <= 1 / 2.
//    这样, 比K位置的元素小的元素的权重和 < 1 / 2; 比K位置的元素大的元素
//    的权重和 <= 1 / 2.
//    所以, K位置的元素是该数组的带权中位数.
// 
// 2) 可以先将这N个数按照X排序. 然后, 迭代数组, 累加权重. 到权重 >= 1 / 2
//    时终止, 此时索引为i.
//    那么, 该i就是带权中位数(此时, array[0, i)的权重和 < 1 / 2;
//    array[0, i] <= 1 / 2. array[0, length)
//    的权重和 <= 1 / 2.
// 3) 进行select, 一次partition, pivot为i位置, 如果[0, i)的权重 < 1 / 2, 且
//    [0, i]的权重 >= 1 / 2, 终止.
//
struct element{
	int number;
	float weight;
};

class selproc : public std::unary_function<const element&, bool> {
public:
	selproc(int pivot) : pivot_(pivot) {}
	bool operator () (const element& ele) const {
		return ele.number < pivot_;
	}
private:
	int pivot_;
};

class accumproc : public std::binary_function<float, const element&, float> {
public:
	float operator() (float f, const element& ele) const {
		return f + ele.weight;
	}
};

static const float kMiddle = 0.5f;
int weight_select(element* ele, int from, int to, float weight) {
	for(; ;) {
		if(from >= to)
			return from;

		int pos = std::partition(ele + from, ele + to, 
			selproc(ele[to - 1].number)) - ele;

		if(pos != to - 1)
			std::iter_swap(ele + pos, ele + to - 1);
		
		float w = weight + std::accumulate(ele + from, ele + pos, 0.f, 
			accumproc());
		if(w < kMiddle && w + ele[pos].weight >= kMiddle)
			return pos;

		if(w < kMiddle) {	// w + ele[pos].weight < kMiddle. too low.
			from = pos + 1;
			weight = w + ele[pos].weight;
		}
		else	// w > kMiddle. too high.
			to = pos;
	}
	
	return -1;
}

void test_weight_select() {
	element ele[5] = { 
		4, 0.2f,
		3, 0.1f,
		1, 0.4f,
		5, 0.2f,
		7, 0.1f
	};

	int pos = weight_select(ele, 0, _countof(ele), 0.f);
}

int main()
{
	test_weight_select();
}

// 4) 一维邮局位置问题:
//    直线上有N个点, 记为p1, p2 ... pn, 分别有权重w1, w2 ... wn,
//    在直线上寻找一个点, p, 使得sum(i = 1 ... n)((abs(p - pi)) * wi)最小.
//    先取得这N个点的带权中位数点, pi, 由带权中位数的定义, 可以知道:
//    所有小于pi的元素的权重和 < 1 / 2, 所有大于pi的元素的权重和<= 1 / 2.
//    假设这个点就是目标点.
//    那么, 将目标点向左移动单位1, 那么, 左边的元素的权重和就会减少, 
//    而且这个值 < 1 / 2.因为左边的元素和权重和 < 1 / 2.
//    而右边的元素的权重和就会增加, 由于移动之后, 点不在pi点, 而是在pi
//    左边, 那么右边的权重和就会增加 > 1 / 2, 这样, 总的目标值就会变大.
//    同理, 将目标点向右移动一个单位, 此时的点记为pj, 那么, p[1, i]的权重
//    和就会增加sum(k = 1 ... i)(wk), pj右边的点的权重和减少sum(
//    k = i + 1, N)(wk).
//    如果此时sum(k = 1 ... i)(wk)为1 / 2, 那么这个移动不会改变最优值, 直到
//    点向右移动超过p(i - 1), 即带权中位数的点前面的那个点.
//    综上所述, 带权中位数的点必然是最优点. 而且, 如果带权中位数的点和它
//    左边的点的权重之和 = 1 / 2, 那么[p(i - 1), p(i)], 带权中位数的点和它
//    之前的那个点这两点之间(包括这两个点)都是最优点.
// 5) 二维邮局位置问题:
//    因为这里距离的定义是abs(x - x0) + abs(y - y0), 所以, 可以先找出x方向的
//    最优值, 再找y方向的最优值. 二者的交点, 就可以使该问题最优.
