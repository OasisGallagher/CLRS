#include "graph.hpp"

// O(|V|*|E|).
// 可以在O(|V|+|E|)时间内去掉重复的边. see ./22.1-4.cpp.
void square(adj_matrix& matrix, const adj_matrix& input) {
	size_t sz = input.size();
	for(size_t row = 0; row < sz; ++row) {	// O(|V|).
		for(size_t col = 0; col < sz; ++col) {	// O(|E|). 这行的for和下面的for, 最多遍历O(|E|)次.
			if(!input(row, col)) continue;

			for(size_t col2 = 0; col2 < sz; ++col2) {
				if(input(col, col2))
					matrix(row, col2) = input(col, col2);
			}
		}
	}
}

void square(adj_table& table, const adj_table& input) {
	size_t sz = input.size();
	for(size_t i = 0; i < sz; ++i) {
		adj_table::ConstIterators iters = input(i);
		for(adj_table::NodeConstIterator it = iters.first; it != iters.second; ++it) {
			adj_table::ConstIterators iters2 = input(it->connected);
			for(adj_table::NodeConstIterator it2 = iters2.first; it2 != iters2.second; ++it2) {
				table.append(i, it2->connected);
			}
		}
	}
}
// - 比如, A是2*2的矩阵, 为a(0, 0), a(0, 1), a(1, 0), a(1, 1).
//   那么, A*A为一个2*2的矩阵, 为
//   a(0, 0)*a(0, 0) + a(0, 1)*a(1, 0), a(0, 0)*a(0,1) + a(0,1)*a(1,1)
//   a(1, 0)*a(0, 0) + a(1, 1)*a(1, 0), a(1, 0)*a(0,1) + a(1,1)*a(1,1)
//   如果a(i,j)不为0, 表示i和j顶点之间可达.
//   如果a(0, 0)*a(0, 0) + a(0, 1)*a(1, 0)不为0, 表示, 0和0之间, 存在一个
//   顶点, 使得
//   0 -> v -> 0路径存在, (0,0)在G^2中.
//   其中, a(0,1)*a(1,0)如果不为0, 那么, 二因子都不为0, 表示, 0->1->0是可
//   达的.
//   因此, (0, 0)就会在G^2中.
//   通过矩阵相乘, 新矩阵的每个位置的值就是路径的条数.
void square(adj_matrix& matrix) {
	adj_matrix other = matrix;
	matrix = matrix * other;
}

// 0: 0, 1
// 1: 1
// _______
// 1 1
// 0 1
#include <iostream>

int main() {
	/*
	adj_table table(2);
	table.append(0, 0);
	table.append(0, 1);
	table.append(1, 1);
	adj_table table2(2);
	square(table2, table);
	*/
	adj_matrix matrix(2);
	matrix(0, 0) = 1; matrix(0, 1) = 1;
	matrix(1, 0) = 0; matrix(1, 1) = 1;
	//adj_matrix matrix2(2);
	//square(matrix2, matrix);
	//matrix2.print(std::cout);
	square(matrix);
	matrix.print(std::cout);
}

