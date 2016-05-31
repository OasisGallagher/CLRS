#include <cstdio>
#include <utility>
#include <stdexcept>
#include <ctime>
#pragma warning(disable:4996)
#pragma warning(disable:4101)

typedef unsigned char uint16_t;

bool less(uint16_t x, uint16_t y) { return x < y; }
bool less_eq(uint16_t x, uint16_t y) { return x <= y; }

// O(n), inplace.
template<size_t length>
void partition(uint16_t (&arr)[length]) {
	if(length <= 1) return;
	uint16_t& key = arr[length - 1];
	bool (&cmp)(uint16_t, uint16_t) = key == 0 ? less_eq : less;
	int i = -1;
	for(unsigned j = 0; j < length - 1; ++j) {
		if((*cmp)(arr[j], key))
			std::swap(arr[++i], arr[j]);
	}

	std::swap(arr[++i], key);
}

// O(n), stable.
template<size_t length>
void countsort(uint16_t (&arr)[length], uint16_t (&out)[length]) {
	if(arr == out) throw std::invalid_argument("input array should be equal to the output array");

	unsigned c[2] = { 0 };
	for(unsigned i = 0; i < length; ++i) ++c[arr[i]];

	for(unsigned i = 1; i < 2; ++i) c[i] += c[i - 1];

	for(int i = int(length - 1); i >= 0; --i)
		out[--c[arr[i]]] = arr[i];
}

// stable, inplace.
template<size_t length>
void insertionsort(uint16_t (&arr)[length]) {
	for(unsigned i = 1; i < length; ++i) {
		uint16_t key = arr[i];
		int hole = i - 1;
		for(; hole >= 0 && arr[hole] > key; --hole)
			arr[hole + 1] = arr[hole];

		arr[hole + 1] = key;
	}
}

// d) 只有稳定的算法可以作为基数排序的子程序, 所以countsort和insertionsort可以.

// e).
// each element in array is less than K.
template<size_t length>
void CountSortInPlace_notStable(uint16_t (&arr)[length], unsigned K) {
	unsigned* c = new unsigned[K];
	std::fill(c, c + K, 0);

	for(unsigned i = 0; i < length; ++i) ++c[arr[i]];
	
	unsigned* c2 = new unsigned[K];
	std::copy(c, c + K, c2);

	for(unsigned i = 1; i < K; ++i) c2[i] += c2[i - 1];
	
	// O(n).相当于对数组中的数遍历一次.
	for(unsigned i = 0; i < K; ++i) {
		for(; c[i] != 0; --c[i])
			arr[--c2[i]] = i;
	}

	// 不稳定.
	delete[] c;
	delete[] c2;
}

int main() {
	uint16_t arr[] = { 0, 1, 0, 0, 1, 1, 1, 0, 1, 1, 0, 0 };
	uint16_t out[sizeof(arr) / sizeof(arr[0])];
	//partition(arr);
	//countsort(arr, out);
	insertionsort(arr);

	srand((unsigned)time(NULL));
	for(unsigned i = 0; i < sizeof(arr) / sizeof(arr[0]); ++i)
		arr[i] = rand() % 10;
	CountSortInPlace_notStable(arr, 10);
}
