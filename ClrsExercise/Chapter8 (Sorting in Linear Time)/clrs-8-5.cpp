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

// 对k排序的数组进行完全排序, 可以将这个数组看作k个独立的数组, 并且他们已经排好序.
// 即: arr[0], arr[k], arr[2k] .. | arr[1], arr[k + 1], arr[2k + 1].
// 然后, 将这些数组的第一个元素插入到一个小根堆中, 这里需要记录哪个元素来自那个数组.
// 接着, 从小根堆中, 取出一个最小的元素, 加入的结果数组中, 并取得这个元素来自哪个
// 上述的k个数组, 取那个数组的下一个元素, 插入到小根堆中.
// 直到k个数组都都为空为止.
// 每次向小根堆中插入和从之中读取, 都需要log(k)的时间(小根堆中最多只会有k个元素).
// 需要遍历这n个数字, 所以, 效率为nlog(k).

int main() {
	int arr[10] = { 1, 4, 5, 2, 9, 6, 8, 3, 10, 7 };
	//for(int i = 0; i < _countof(arr); ++i)
	//	arr[i] = i + 1;
	//std::random_shuffle(arr, arr + _countof(arr));
	KSort(3, arr, 0, _countof(arr));
}
