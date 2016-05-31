#include <iostream>
#include <cmath>
#pragma warning(disable:4996)

// ��N����, �ֳ�floor(N / 2)��, ÿ������Ԫ��, ���бȽ�, С��ֵ"����".
// Ȼ���floor(N / 2)( + 1)��Ԫ���ظ��˹���, ֱ��ֻʣ��һ��Ԫ��, ��ʱ
// �õ�������СԪ��, һ���Ƚ���N - 1��.
// ���ϣ���ҵ��ڶ�С��Ԫ��, ����֪��, ��һ������С��Ԫ�رȽϹ�, ��Ϊ
// ֻ����С��Ԫ�ؿ���С���ڶ�С��Ԫ��.
// ���, �Ƚ���Щ����СԪ�رȽϹ���Ԫ��, ��Сֵ���ǵڶ�С��Ԫ��.
// ��ЩԪ��, ����lg(N)��, �����Ҫ�Ƚ�lg(N) - 1��.
// �ܹ��Ƚ�N + lg(N) - 2.
//
inline int Lg2(int n) {
	static float log10_2 = log10(2.f);
	return (int)ceil((log10((float)n) / log10_2));
}

inline int Roundup2(int n) {
	return (n + (2 - 1)) & (~(2 - 1));
}

int second_min(int* first, int* last);

inline int SecondMin(int* first, int* last) {
	int length = last - first;
	if(length >= 2)
		return second_min(first, last);

	if(length <= 0) throw "invalid range";
	return first[0];
}

int second_min(int* first, int* last) {
	int count = last - first;
	int cLayer = 1 + Lg2(count);

	const int Layer = cLayer;

	int** container = new int*[cLayer];
	
	for(int i = 0, c = 1; i < cLayer; ++i, c *= 2)
		container[i] = new int[c];

	// Ѱ����Сֵ, ������һ��������, ��¼����ÿһ��"����".
	// ��2, 1, 3.
	/*
		   1
		 1   3		// ����3�ֿ�, ֱ�ӽ���.
		2 1 3 NaN
	*/
	std::copy(first, last, container[--cLayer]);
	for(; (--cLayer) >= 0; ) {
		int upper = count & ~(1);
		for(int i = 0; i < upper; i += 2) {
			container[cLayer][i / 2] = std::min(container[cLayer + 1][i], container[cLayer + 1][i + 1]);
		}
		if(upper < count) {
			container[cLayer][upper / 2] = container[cLayer + 1][count - 1];
		}
		count = Roundup2(count) / 2;
	}

	const int& minvalue = container[0][0];
	int result = INT_MAX;
	int index = 0;
	// Ѱ�ҵڶ���Сֵ, �Ƚ�ceil(lgN) - 1��.
	for(int i = 1; i < Layer; ++i) {
		int* cur = container[i];
		// �ж���С�ڵ��Ǵ��ĸ���һ��ڵ��н�����.
		if(minvalue == cur[index * 2]) {
			// ����ڵ����, ��ô�Ƚ����ҽڵ�, Ѱ�ҵڶ�С�Ľڵ�ֵ.
			result = std::min(result, cur[2 * index + 1]); 
			index = 2 * index;
		}
		else {
			// ���ҽڵ����, ��ô�Ƚ�����ڵ�, Ѱ�ҵڶ�С�Ľڵ�ֵ.
			result = std::min(result, cur[2 * index]);
			index = 2 * index + 1;
		}
	}

	for(int i = 0; i < Layer; ++i)
		delete[] container[i];

	delete[] container;

	return result;
}
 
int main() {
	int arr[] = { 2, 1, 3, 5, 4 };
	std::cout << "the second smallest value is " << SecondMin(arr, arr + _countof(arr)) << "\n";
}
