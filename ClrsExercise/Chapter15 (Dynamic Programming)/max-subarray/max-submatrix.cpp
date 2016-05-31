#include <iostream>
#include <algorithm>
#include <vector>
#include <functional>
#include <cstdarg>

typedef std::vector<int> Array;
typedef std::vector<Array> Matrix;

// Dynamic programming.
// 需要注意的是, 如果tmp <= 0,那么,此时m到i的字串,此时m>0,不可能比现在这个小于0的值大.
// 因为,假设存在这样的字串,那么,当[0,m]的字串>0时,剩下的字串只会更小.
// 当[0,m]的字串<0时,迭代到位置m时就会已经发现
int DPMaxSumOfSubArray(const Array& arr)
	{
	int result = INT_MIN;	// max[i, j]
	int prev = 0;			// max[i, j - 1]
	size_t size = arr.size();
	for(size_t i = 0; i < size; ++i)
		{
		if(prev > 0)
			prev += arr[i];
		else
			prev = arr[i];
		result = std::max(prev, result);
		}
	return result;
	}

int DPMaxSumOfSubMatrix(const Matrix& matrix)
	{	// O(n^3)
	int result = INT_MIN;
	size_t row = matrix.size(), col = matrix[0].size();

	for(size_t from = 0; from < row; ++from)
		{
		Array tmp(col);
		for(size_t to = from; to < row; ++to)
			{	// 矩阵从行from到to,逐行加和,更新最大值.
			std::transform(tmp.begin(), tmp.end(), matrix[to].begin(), tmp.begin(), std::plus<int>());
			result = std::max(result, DPMaxSumOfSubArray(tmp));
			}
		}
	return result;
	}

void PushArray(Array* dest, int count, ...)
	{
	va_list vl;
	va_start(vl, count);
	for(int i = 0; i < count; ++i)
		dest->push_back(va_arg(vl, int));
	va_end(vl);
	}

int main()
	{
	Matrix matrix(4);
	PushArray(&matrix[0], 4, 0, -2, -7, 0);
	PushArray(&matrix[1], 4, 9, 2, -6, 2);
	PushArray(&matrix[2], 4, -4, 1, -4, 1);
	PushArray(&matrix[3], 4, -1, 8, 0, -2);

	std::cout << DPMaxSumOfSubMatrix(matrix) << std::endl;
	}

