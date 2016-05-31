#include <iostream>
#include <vector>
#include <algorithm>

// clrs 9.3-8.
// 两个已经排序的, 长度分别为N的数组, X和Y, 在O(lg(N))时间内找出它们的中位数.
// 假设这个中位数在X中, 这个数的值为m, 位置在x[k]处, 那么, 显然, 在X中, 有k + 1个数
// 不大于m, 有N - k - 1个数不小于m.
// 而且, 在两个数组合并后的数组Z中, 必然有N个数不大于m, N个数不小于m(中位数的性质).
// 因此, 在Y中, 就有N - k - 1个数不大于m, 有N - (N - k - 1) = k + 1个数不小于m.
// 可以知道, Y[N - k - 1]为最后一个不大于m的数. Y[N - k]为第一个不小于m的数.
// 只要让Y[N - k - 1] <= X[k] <= Y[N - k], X[k]就是这两个数组合并后的数组Z的中位数.
// 如果中位数在X中, 但是不在X[k], 而是在i(i < k)的位置, Y[N - i - 1] <= X[i] < X[K].
// 因为i < k, 所以i + 1 <= k, 所以-i - 1 >= -k, 所以Y[N - i - 1] >= Y[N - k].
// 所以X[k] > Y[N - k].
// 同理, 如果中位数在X中, 但是不在X[k], 而是在j(j > k)的位置, 
// X[k] < Y[N - k - 1].
// 同理, 如果中位数在Y中, 方法一样.

typedef std::vector<int>	Vector;
static const int kNopos	= -1;

// binary search(O(lg(n))).
int __find_median(const Vector& X, const Vector& Y) {
	for(int first = 0, last = X.size(), len = X.size(); first < last; ) {
		int mid = (first + last) / 2;
		if(mid == 0) {
			if(X[mid] >= Y[len - mid - 1])
				return mid;
		}
		else if(X[mid] > Y[len - mid]) {
			last = mid;
		}
		else if(X[mid] < Y[len - mid - 1]) {
			first = mid + 1;
		}
		else {
			return mid;
		}
	}

	return kNopos;
}

int median(const Vector& X, const Vector& Y) {
	int pos = __find_median(X, Y);
	if(pos != kNopos) {
		return X[pos];
	}

	pos = __find_median(Y, X);
	if(pos == kNopos) _asm int 3;
	
	return Y[pos];
}

int main() {
	int arr[] = { 1, 2, 3, 5, 8, 10, 12, 13, 16, 20, 4, 6, 7, 9, 11, 14, 15, 17, 18, 19 };
	Vector X, Y, Z;
	for(int i = 0; i < _countof(arr); ++i) {
		Z.push_back(arr[i]);
	}

	std::copy(Z.begin(), Z.begin() + _countof(arr) / 2, std::back_inserter(X));
	std::copy(Z.begin() + _countof(arr) / 2, Z.end(), std::back_inserter(Y));
	std::sort(X.begin(), X.end());
	std::sort(Y.begin(), Y.end());

	int mid = median(X, Y);
	std::cout << mid <<std::endl;
}
