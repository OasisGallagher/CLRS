#include "graph.hpp"
#include <iostream>

class universal_sink {
	adj_matrix& matrix;

	// ͼmatrix���Ƿ��г���Ϊ0, ���Ϊsize - 1�Ķ���.
	bool has_universal_sink() const {
		size_t row = 0, col = 0, sz = matrix.size();
		for(; row < sz && col < sz; ) {
			// �����row��, ����1, ��ô, row#����һ������.
			// ��Ϊ���ĳ��Ȳ�Ϊ0.
			if(matrix(row, col))
				++row;
			else
				++col;
		}

		// [0, row)�ж��ǲ����ܵ�(���Ȳ�Ϊ0).
		if(row >= sz)
			return false;

		// row < sz && col >= sz.
		// [row, sz)��, ÿһ�ж���0.
		// ����k <- (row, sz), ÿ��matrix(row, k)��Ϊ0, ��һ������matrix��
		// �Խ�����. ����matrix(row, row)�ڶԽ�����.
		// ����, ����row#�п���, �������.
		return has_universal_sink(row);
	}

	// ��vertex�Ƿ��ǳ���Ϊ0, ���Ϊsize - 1�Ķ���.
	bool has_universal_sink(int vertex) const {
		size_t sz = matrix.size();

		// ������Ȳ�Ϊ0, ����false.
		for(size_t i = 0; i < sz; ++i)
			if(matrix(vertex, i))
				return false;

		// ����ǶԽ��ߵ�λ��, Ϊ0, ��ô, ���һ��С��size-1.
		// ��Ϊ�Խ��ߵ�λ��һ��Ϊ0(�������Ի�).
		for(size_t i = 0; i < sz; ++i)
			if(!matrix(i, vertex) && i != sz - 1)
				return false;

		std::cout << "found vertex " << vertex << std::endl;
		return true;
	}
public:
	universal_sink(adj_matrix& m) : matrix(m) { }
	operator bool() const {
		return has_universal_sink();
	}
};

// 1 0 1
// 0 1 1
// 0 0 0
adj_matrix create_matrix() {
	adj_matrix matrix(3);
	matrix(0, 0) = 1;
	matrix(0, 1) = 0;
	matrix(0, 2) = 1;
	matrix(1, 0) = 0;
	matrix(1, 1) = 1;
	matrix(1, 2) = 1;
	matrix(2, 0) = 0;
	matrix(2, 1) = 0;
	matrix(2, 2) = 0;
	return matrix;
}

int main() {
	adj_matrix matrix = create_matrix();
	matrix.print(std::cout);

	std::cout << std::boolalpha << universal_sink(matrix) << std::endl;
}
