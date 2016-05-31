#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include "Utilities/table.hpp"
typedef std::vector<int> array;
// 可以用bool数组代替table. 这里为简便起见.
typedef table<int, int, bool> mytable;

class accumulate_if : std::binary_function<int, int, int> {
	bool positive;
public:
	accumulate_if(bool positive) {
		this->positive = positive;
	}

	int operator()(int lhs, int rhs) const {
		if (positive == (rhs > 0)) lhs += rhs;
		return lhs;
	}
};

// input中是否存在子集, 使得该子集和 = sum.
// 0-1背包问题, 复杂度为非多项式时间.
bool subset(const array& input, int sum, mytable& result) {
	// 最小和.
	int min = std::accumulate(input.begin(), input.end(), 0, accumulate_if(false));
	// 最大和.
	int max = std::accumulate(input.begin(), input.end(), 0, accumulate_if(true));

	mytable s;

	// s(i, j)表示, input[0, i]物品可选, 可否达到j值.
	// result(i, j)表示, input[0, i]可选, 到达j值时, i物品是否需要.
	
	// 没有物品可选.
	for (int i = min; i <= max; ++i)
		s.set(-1, i, false);

	// 达到目标值0.
	for (int i = -1; i < (int)input.size(); ++i)
		s.set(i, 0, true);

	// O(n(max - min)), NP.
	for (array::size_type i = 0; i < input.size(); ++i) {
		for (int dest = min; dest <= max; ++dest) {
			bool needed = false;
			bool can_reach = false;
			// 使用[0, i-1]物品, 即可达到j.
			if (s.get(i - 1, dest))
				can_reach = true;
			// [0, i-1]物品无法达到j, 那么, 尝试拾取物品i.
			else if (dest >= input[i] && (dest - input[i] <= max) && s.get(i - 1, dest - input[i])) {
				needed = true;
				can_reach = true;
			}

			s.set(i, dest, can_reach);
			result.set(i, dest, needed);
		}
	}

	return s.get(input.size() - 1, sum);
}

void solution(mytable& result, const array& input, int i, int sum) {
	if (sum != 0) {
		if (result.get(i, sum)) {
			solution(result, input, i - 1, sum - input[i]);
			std::cout << input[i] << " ";
		}
		else {
			solution(result, input, i - 1, sum);
		}
	}
}

int main() {
	int buffer[] = { 2, 2, -2, -1, 5, 4, 7 };
	mytable result;

	if (subset(array(buffer, buffer + _countof(buffer)), 10, result))
		solution(result, array(buffer, buffer + _countof(buffer)), _countof(buffer) - 1, 10);

	std::cout << std::endl;
}
