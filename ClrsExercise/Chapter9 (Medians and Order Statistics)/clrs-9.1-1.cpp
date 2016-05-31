#include <iostream>
#include <cmath>
#pragma warning(disable:4996)

// 将N个数, 分成floor(N / 2)组, 每组两个元素, 进行比较, 小的值"晋级".
// 然后对floor(N / 2)( + 1)个元素重复此过程, 直到只剩下一个元素, 此时
// 得到的是最小元素, 一共比较了N - 1次.
// 如果希望找到第二小的元素, 可以知道, 它一定和最小的元素比较过, 因为
// 只有最小的元素可以小过第二小的元素.
// 因此, 比较这些和最小元素比较过的元素, 最小值就是第二小的元素.
// 这些元素, 共有lg(N)个, 因此需要比较lg(N) - 1次.
// 总共比较N + lg(N) - 2.
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

	// 寻找最小值, 并构造一个二叉树, 记录的是每一场"比赛".
	// 如2, 1, 3.
	/*
		   1
		 1   3		// 数字3轮空, 直接晋级.
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
	// 寻找第二最小值, 比较ceil(lgN) - 1次.
	for(int i = 1; i < Layer; ++i) {
		int* cur = container[i];
		// 判断最小节点是从哪个下一层节点中晋级的.
		if(minvalue == cur[index * 2]) {
			// 从左节点晋级, 那么比较其右节点, 寻找第二小的节点值.
			result = std::min(result, cur[2 * index + 1]); 
			index = 2 * index;
		}
		else {
			// 从右节点晋级, 那么比较其左节点, 寻找第二小的节点值.
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
