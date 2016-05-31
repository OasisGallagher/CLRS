#include <iostream>
#include <functional>
#include <algorithm>
#include <numeric>

// clrs 9-2.
// 
// 1) ���N������Ȩ�ض���1 / N, ������λ����Kλ��, ��ô, K = ceil(N / 2),
//    ��ôK��ߵ�����Ȩ�غͱ�Ȼ < 1 / 2. ��Щ����Ȩ�غ� + ��λ����Ȩ��
//    ��Ȼ >= 1 / 2.
//    ����, ���������ֵ�Ȩ�غͱ�Ȼ <= 1 / 2.
//    ����, ��Kλ�õ�Ԫ��С��Ԫ�ص�Ȩ�غ� < 1 / 2; ��Kλ�õ�Ԫ�ش��Ԫ��
//    ��Ȩ�غ� <= 1 / 2.
//    ����, Kλ�õ�Ԫ���Ǹ�����Ĵ�Ȩ��λ��.
// 
// 2) �����Ƚ���N��������X����. Ȼ��, ��������, �ۼ�Ȩ��. ��Ȩ�� >= 1 / 2
//    ʱ��ֹ, ��ʱ����Ϊi.
//    ��ô, ��i���Ǵ�Ȩ��λ��(��ʱ, array[0, i)��Ȩ�غ� < 1 / 2;
//    array[0, i] <= 1 / 2. array[0, length)
//    ��Ȩ�غ� <= 1 / 2.
// 3) ����select, һ��partition, pivotΪiλ��, ���[0, i)��Ȩ�� < 1 / 2, ��
//    [0, i]��Ȩ�� >= 1 / 2, ��ֹ.
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

// 4) һά�ʾ�λ������:
//    ֱ������N����, ��Ϊp1, p2 ... pn, �ֱ���Ȩ��w1, w2 ... wn,
//    ��ֱ����Ѱ��һ����, p, ʹ��sum(i = 1 ... n)((abs(p - pi)) * wi)��С.
//    ��ȡ����N����Ĵ�Ȩ��λ����, pi, �ɴ�Ȩ��λ���Ķ���, ����֪��:
//    ����С��pi��Ԫ�ص�Ȩ�غ� < 1 / 2, ���д���pi��Ԫ�ص�Ȩ�غ�<= 1 / 2.
//    ������������Ŀ���.
//    ��ô, ��Ŀ��������ƶ���λ1, ��ô, ��ߵ�Ԫ�ص�Ȩ�غ;ͻ����, 
//    �������ֵ < 1 / 2.��Ϊ��ߵ�Ԫ�غ�Ȩ�غ� < 1 / 2.
//    ���ұߵ�Ԫ�ص�Ȩ�غ;ͻ�����, �����ƶ�֮��, �㲻��pi��, ������pi
//    ���, ��ô�ұߵ�Ȩ�غ;ͻ����� > 1 / 2, ����, �ܵ�Ŀ��ֵ�ͻ���.
//    ͬ��, ��Ŀ��������ƶ�һ����λ, ��ʱ�ĵ��Ϊpj, ��ô, p[1, i]��Ȩ��
//    �;ͻ�����sum(k = 1 ... i)(wk), pj�ұߵĵ��Ȩ�غͼ���sum(
//    k = i + 1, N)(wk).
//    �����ʱsum(k = 1 ... i)(wk)Ϊ1 / 2, ��ô����ƶ�����ı�����ֵ, ֱ��
//    �������ƶ�����p(i - 1), ����Ȩ��λ���ĵ�ǰ����Ǹ���.
//    ��������, ��Ȩ��λ���ĵ��Ȼ�����ŵ�. ����, �����Ȩ��λ���ĵ����
//    ��ߵĵ��Ȩ��֮�� = 1 / 2, ��ô[p(i - 1), p(i)], ��Ȩ��λ���ĵ����
//    ֮ǰ���Ǹ���������֮��(������������)�������ŵ�.
// 5) ��ά�ʾ�λ������:
//    ��Ϊ�������Ķ�����abs(x - x0) + abs(y - y0), ����, �������ҳ�x�����
//    ����ֵ, ����y���������ֵ. ���ߵĽ���, �Ϳ���ʹ����������.
