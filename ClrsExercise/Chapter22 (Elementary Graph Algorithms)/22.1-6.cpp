#include "graph.hpp"
#include <iostream>

class universal_sink {
	adj_matrix& matrix;

	// 图matrix中是否有出度为0, 入度为size - 1的顶点.
	bool has_universal_sink() const {
		size_t row = 0, col = 0, sz = matrix.size();
		for(; row < sz && col < sz; ) {
			// 如果第row行, 含有1, 那么, row#顶点一定不是.
			// 因为它的出度不为0.
			if(matrix(row, col))
				++row;
			else
				++col;
		}

		// [0, row)行都是不可能的(出度不为0).
		if(row >= sz)
			return false;

		// row < sz && col >= sz.
		// [row, sz)中, 每一列都有0.
		// 讨论k <- (row, sz), 每个matrix(row, k)都为0, 且一定不在matrix的
		// 对角线上. 仅有matrix(row, row)在对角线上.
		// 综上, 仅有row#有可能, 单独检查.
		return has_universal_sink(row);
	}

	// 点vertex是否是出度为0, 入度为size - 1的顶点.
	bool has_universal_sink(int vertex) const {
		size_t sz = matrix.size();

		// 如果出度不为0, 返回false.
		for(size_t i = 0; i < sz; ++i)
			if(matrix(vertex, i))
				return false;

		// 如果非对角线的位置, 为0, 那么, 入度一定小于size-1.
		// 因为对角线的位置一定为0(不存在自环).
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
