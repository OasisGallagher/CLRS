#include <iostream>
#include <algorithm>

int KPart(int k, int* arr, int from, int to) {
	int i = from - k;
	int& key = arr[to - k];

	for(int j = from; j < to - k; j += k) {
		if(arr[j] <= key)
			std::swap(arr[i += k], arr[j]);
	}
	std::swap(arr[i += k], key);
	return i;
}

void KSortPart(int k, int* arr, int from, int to) {
	if(from < to) {
		int p = KPart(k, arr, from, to);
		KSortPart(k, arr, from, p);
		KSortPart(k, arr, p + k, to);
	}
}
// O(k * n / k * log(n / k) = O(n * log(n / k)).
void KSort(int k, int* arr, int from, int to) {
	int boundry = ((to - from) / k) * k;
	for(int i = 0; i < k; ++i) {	// O(k).
		int upper = (boundry + i);
		if(upper >= to) upper -= k;
		// Quick sort. O(log(n / k) * n / k).
		KSortPart(k, arr, from + i, upper + k);
	}
}

// ��k��������������ȫ����, ���Խ�������鿴��k������������, ���������Ѿ��ź���.
// ��: arr[0], arr[k], arr[2k] .. | arr[1], arr[k + 1], arr[2k + 1].
// Ȼ��, ����Щ����ĵ�һ��Ԫ�ز��뵽һ��С������, ������Ҫ��¼�ĸ�Ԫ�������Ǹ�����.
// ����, ��С������, ȡ��һ����С��Ԫ��, ����Ľ��������, ��ȡ�����Ԫ�������ĸ�
// ������k������, ȡ�Ǹ��������һ��Ԫ��, ���뵽С������.
// ֱ��k�����鶼��Ϊ��Ϊֹ.
// ÿ����С�����в���ʹ�֮�ж�ȡ, ����Ҫlog(k)��ʱ��(С���������ֻ����k��Ԫ��).
// ��Ҫ������n������, ����, Ч��Ϊnlog(k).

int main() {
	int arr[10] = { 1, 4, 5, 2, 9, 6, 8, 3, 10, 7 };
	//for(int i = 0; i < _countof(arr); ++i)
	//	arr[i] = i + 1;
	//std::random_shuffle(arr, arr + _countof(arr));
	KSort(3, arr, 0, _countof(arr));
}
