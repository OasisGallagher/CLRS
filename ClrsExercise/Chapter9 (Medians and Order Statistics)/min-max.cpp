#include <algorithm>
// 
void cmp2(int _1, int _2, std::pair<int, int>& result) {
	if(_1 < _2) {
		result.first = std::min(_1, result.first);
		result.second = std::max(_2, result.second);
	}
	else {
		result.first = std::min(_2, result.first);
		result.second = std::max(_1, result.second);
	}
}
std::pair<int, int> MinMax(int* arr, int from, int to) {
	int length = to - from;
	std::pair<int, int> result = std::make_pair(INT_MAX, INT_MIN);
	
	++from;
	if(length % 2 == 0) {
		++from;
		result.first = arr[0], result.second = arr[1];
		if(result.first > result.second)
			std::swap(result.first, result.second);
	}
	else {
		result.first = result.second = arr[0];
	}

	// to - from must be even.
	for(; from < to; from += 2) {
		// 每两个元素, 进行三次比较. 一共进行3 * floor(n / 2)次比较.
		// 如果遍历两次, 需要2n比较.
		cmp2(arr[from], arr[from + 1], result);
	}

	return result;
}

int main()
{
	int arr[] = { 5, 3, 7, 9, 2, 1 };
	std::pair<int, int> result = MinMax(arr, 0, sizeof(arr) / sizeof(arr[0]));
}
