// clrs 9.3-6
#include <iostream>
#include <algorithm>

void random_shuffle(int* from, int* to) {
	int length = to - from;
	for(int i = 0; i < length; ++i) {
		std::iter_swap(from + i, from + rand() % (length - i)); 
	}
}

int partition(int* arr, int from, int to) {
	int i = from - 1;
	int& key = arr[to - 1];
	for(int j = from; j < to - 1; ++j) {
		if(arr[j] <= key)
			std::swap(arr[++i], arr[j]);
	}

	std::swap(arr[++i], key);
	return i;
}

int select(int* arr, int from, int to, int k) {
	while(true) {
		if(from >= to)
			return arr[from];

		int p = partition(arr, from, to);
		if(p == k)
			return arr[p];

		if(p < k) {
			from = p + 1;
			//k -= p + 1;
		}
		else	// p > k.
			to = p;
	}

	return -1;
}

// 将数组分成k组, 然后, 将这k个组平分, 找出中位数, 这个中位数把数组平分
// 为两份, 在这两份上持续执行此过程, 直到子数组不足两组为止.
//
void kth_quantile(int* arr, int from, int to, int k) {
	int gs = (to - from) / k;
	if(gs > 1) {
		int mid = k * gs / 2 + from;
		select(arr, from, to, mid);
		kth_quantile(arr, from, mid, k);
		kth_quantile(arr, mid, to, k);
	}
}

int main() {
	int arr[20];
	for(int i = 0; i < _countof(arr); ++i)
		arr[i] =  i + 1;
	random_shuffle(arr, arr + _countof(arr));
	kth_quantile(arr, 0, _countof(arr), 5);
}
