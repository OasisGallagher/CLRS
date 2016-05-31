// clrs 8.3-4.
#pragma warning(disable:4996)
#include <iostream>
#include <functional>
#include <algorithm>

template <class ValueAt>
static void CountSort(unsigned limit, unsigned* buffer, unsigned len, const ValueAt& func) {
	unsigned* count = new unsigned[limit];

	std::fill(count, count + limit, 0);

	for(unsigned i = 0; i < len; ++i) {
		if(func(buffer[i]) >= limit) throw "number in buffer out of range";
		++count[func(buffer[i])];
	}

	for(unsigned i = 1; i < limit; ++i)
		count[i] += count[i - 1];

	unsigned* result = new unsigned[len];

	unsigned j = len - 1;
	do {
		result[--count[func(buffer[j])]] = buffer[j];
		if(j > 0) --j;
		else break;
	}while(true);

	delete[] count;

	std::copy(result, result + len, buffer);
	delete[] result;
}
// O(n). stable. NOT in place.
static void SortRangeNSquare(unsigned n, unsigned* buffer, unsigned len) {
	typedef std::binary_function<unsigned, unsigned, unsigned> BinaryFun;
	struct Sort2 : public BinaryFun { 
		unsigned operator() (unsigned number, unsigned n) const { return number % n; }
	};
	struct Sort1 : public BinaryFun { 
		unsigned operator() (unsigned number, unsigned n) const { return number / n; } 
	};
	// divide number = (number / n), (number % n).
	CountSort(n, buffer, len, std::bind2nd(Sort2(), n));
	CountSort(n, buffer, len, std::bind2nd(Sort1(), n));
}
// type [& varname] [N]: 引用数组, 引用一个N个元素的一维数组.
// int array[10];
// int (&ref_array)[10] = array;	// 数组的引用.
/*
void Test(int arr[10]) {
	std::cout << sizeof(arr) << std::endl;	// 4, arr退化为指针.
}
void Test2(int (&arr)[10]) {
	std::cout << sizeof(arr) << std::endl;	// 40, 不会退化, 依然是sizeof(int[40]).
}
*/
// 因此, 这里只有传递一个引用数组, N才会被推导出来.
// 否则, 如果是unsigned buffer[N], buffer退化为指针, 无法推导出N !.

template <unsigned N>
void trace(unsigned (&buffer)[N]) {
	for(unsigned i = 0; i < N; ++i) {
		std::cout << buffer[i] << " ";
		if((i + 1) % 5 == 0)
			std::cout << "\n";
	}
	std::cout << "\n";
}

int main() {
	const unsigned n = 10;
	unsigned buffer[50];

	for(unsigned i = 0; i < _countof(buffer); ++i) {
		buffer[i] = i + 1;
	}

	std::random_shuffle(buffer, buffer + _countof(buffer));

	trace(buffer);

	SortRangeNSquare(n, buffer, _countof(buffer));
	trace(buffer);

	return 0;
}