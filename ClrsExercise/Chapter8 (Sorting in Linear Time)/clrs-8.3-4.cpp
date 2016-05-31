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
// type [& varname] [N]: ��������, ����һ��N��Ԫ�ص�һά����.
// int array[10];
// int (&ref_array)[10] = array;	// ���������.
/*
void Test(int arr[10]) {
	std::cout << sizeof(arr) << std::endl;	// 4, arr�˻�Ϊָ��.
}
void Test2(int (&arr)[10]) {
	std::cout << sizeof(arr) << std::endl;	// 40, �����˻�, ��Ȼ��sizeof(int[40]).
}
*/
// ���, ����ֻ�д���һ����������, N�Żᱻ�Ƶ�����.
// ����, �����unsigned buffer[N], buffer�˻�Ϊָ��, �޷��Ƶ���N !.

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