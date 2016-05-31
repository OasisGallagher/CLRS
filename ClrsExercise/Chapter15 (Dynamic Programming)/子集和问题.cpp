#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include "Utilities/table.hpp"
typedef std::vector<int> array;
// ������bool�������table. ����Ϊ������.
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

// input���Ƿ�����Ӽ�, ʹ�ø��Ӽ��� = sum.
// 0-1��������, ���Ӷ�Ϊ�Ƕ���ʽʱ��.
bool subset(const array& input, int sum, mytable& result) {
	// ��С��.
	int min = std::accumulate(input.begin(), input.end(), 0, accumulate_if(false));
	// ����.
	int max = std::accumulate(input.begin(), input.end(), 0, accumulate_if(true));

	mytable s;

	// s(i, j)��ʾ, input[0, i]��Ʒ��ѡ, �ɷ�ﵽjֵ.
	// result(i, j)��ʾ, input[0, i]��ѡ, ����jֵʱ, i��Ʒ�Ƿ���Ҫ.
	
	// û����Ʒ��ѡ.
	for (int i = min; i <= max; ++i)
		s.set(-1, i, false);

	// �ﵽĿ��ֵ0.
	for (int i = -1; i < (int)input.size(); ++i)
		s.set(i, 0, true);

	// O(n(max - min)), NP.
	for (array::size_type i = 0; i < input.size(); ++i) {
		for (int dest = min; dest <= max; ++dest) {
			bool needed = false;
			bool can_reach = false;
			// ʹ��[0, i-1]��Ʒ, ���ɴﵽj.
			if (s.get(i - 1, dest))
				can_reach = true;
			// [0, i-1]��Ʒ�޷��ﵽj, ��ô, ����ʰȡ��Ʒi.
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
