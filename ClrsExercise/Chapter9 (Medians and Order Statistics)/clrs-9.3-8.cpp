#include <iostream>
#include <vector>
#include <algorithm>

// clrs 9.3-8.
// �����Ѿ������, ���ȷֱ�ΪN������, X��Y, ��O(lg(N))ʱ�����ҳ����ǵ���λ��.
// ���������λ����X��, �������ֵΪm, λ����x[k]��, ��ô, ��Ȼ, ��X��, ��k + 1����
// ������m, ��N - k - 1������С��m.
// ����, ����������ϲ��������Z��, ��Ȼ��N����������m, N������С��m(��λ��������).
// ���, ��Y��, ����N - k - 1����������m, ��N - (N - k - 1) = k + 1������С��m.
// ����֪��, Y[N - k - 1]Ϊ���һ��������m����. Y[N - k]Ϊ��һ����С��m����.
// ֻҪ��Y[N - k - 1] <= X[k] <= Y[N - k], X[k]��������������ϲ��������Z����λ��.
// �����λ����X��, ���ǲ���X[k], ������i(i < k)��λ��, Y[N - i - 1] <= X[i] < X[K].
// ��Ϊi < k, ����i + 1 <= k, ����-i - 1 >= -k, ����Y[N - i - 1] >= Y[N - k].
// ����X[k] > Y[N - k].
// ͬ��, �����λ����X��, ���ǲ���X[k], ������j(j > k)��λ��, 
// X[k] < Y[N - k - 1].
// ͬ��, �����λ����Y��, ����һ��.

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
