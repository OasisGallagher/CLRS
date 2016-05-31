#include <algorithm>
#include <cassert>
#include <functional>
#include <iostream>
#include <numeric>
#include <vector>

class Matrix {
public:
	typedef int Weight;
	typedef std::vector<Weight> Line;
	typedef std::vector<Line> MatrixContainer;
	static const Weight MaxWeight;
	static const size_t InvalidVertex;
	// Create n*n matrix.
	Matrix(size_t n) : container(n) {
		std::for_each(container.begin(), container.end(), std::bind2nd(std::mem_fun_ref(&Line::resize), n));
	}

#pragma region Indexing
	// Get line.
	Line& operator[](size_t i) {
		return at(i);
	}

	// Get element.
	Weight& operator()(size_t line, size_t column) {
		return at(line, column);
	}

	// Get line.
	const Line& operator[](size_t i) const {
		return at(i);
	}

	// Get element.
	const Weight& operator()(size_t line, size_t column) const {
		return at(line, column);
	}

	// Get line.
	Line& at(size_t i) {
		return container[i];
	}

	// Get element.
	Weight& at(size_t line, size_t column) {
		return container[line][column];
	}

	// Get line.
	const Line& at(size_t i) const {
		return container[i];
	}

	// Get element.
	const Weight& at(size_t line, size_t column) const {
		return container[line][column];
	}

#pragma endregion

	size_t Dimension() const {
		return container.size();
	}

	// 设矩阵表示的SP长度为M, 矩阵中的元素(i, j)表示点i到j的SP权值和.
	// 设M[lhs] = x.
	// 该方法在lhs的基础上, 通过rhs, 扩展lhs的M值.
	// 最终的结果M[result] = x + M[rhs].
	Matrix operator * (const Matrix& other) const {	// O(n^3).
		size_t n = Dimension();
		Matrix result(n);

		for (size_t i = 0; i < n; ++i) {
			for (size_t j = 0; j < n; ++j) {
				result(i, j) = Matrix::MaxWeight;
				for (size_t k = 0; k < n; ++k) {
					result(i, j) = std::min(result(i, j), at(i, k) + other(k, j));
				}
			}
		}

		return result;
	}

	Matrix& operator *= (const Matrix& other) { // O(n^3).
		*this = operator * (other);
		return *this;
	}
private:
	MatrixContainer container;
};

const Matrix::Weight Matrix::MaxWeight = std::numeric_limits<Weight>::max();
const size_t Matrix::InvalidVertex = std::numeric_limits<size_t>::max();

class Graph {
	Matrix matrix;
public:
	Graph(size_t n): matrix(n) {

	}

	Matrix MatrixMultiplicationAllPairsShortestPaths() { // O(n^4).
		Matrix result = matrix;
		for (size_t i = 2; i < matrix.Dimension(); ++i) {	// n-2 passes.
			result *= matrix;
		}

		// result = matrix^(n-1)

		return result;
	}

	// 最终目的是为了得到matrix^(n-1), 且, 对于matrix^k, k > (n-1),
	// 依然与matrix^(n-1)相等.
	Matrix MatrixMultiplicationAllPairsShortestPaths2() {	// O(n^3log(n)).
		Matrix result = matrix;
		for (size_t i = 1; i < (matrix.Dimension() - 1); i *= 2) {
			result *= result;
		}

		// 因为在每次进入for的比较时, 得到的result为matrix^i.
		// 最终i > matrix.Dimension -1.
		// for循环共迭代ceil(log(matrix.Dimension - 1))次.

		return result;
	}
	
	//            { W(i, j)					if k = 0
	// Dk(i, j) = |
	//            { min(D(k-1)(i, j), D(k-1)(i, k) + d(k-1)(k, j).
	// Dk(i, j)表示使用[1, k]区间内的顶点时, i到j的最短路径长度.
	Matrix FloydWarshall() {	// O(n^3).
		Matrix result = matrix;
		// 初始化π.
		// 如果i == j or w(i, j) = INF, π(i, j) = INF.
		// 否则, π(i, j) = w(i, j).
		Matrix π(matrix.Dimension());

		for (size_t i = 0; i < π.Dimension(); ++i) {
			for (size_t j = 0; j < π.Dimension(); ++j) {
				if (i == j || matrix(i, j) == Matrix::MaxWeight)
					π(i, j) = Matrix::InvalidVertex;
				else
					π(i, j) = i;
			}
		}

		for (size_t k = 1; k < matrix.Dimension(); ++k) {
			//Matrix prev = result;
			// 可以丢弃上标, 即使用resultk, 而非result_(k-1).see CLRS 25.2-4.
			Matrix& prev = result;
			Matrix prev_π = π;
			for (size_t i = 0; i < matrix.Dimension(); ++i) {
				for (size_t j = 0; j < matrix.Dimension(); ++j) {
					Matrix::Weight w = prev(i, k) + prev(k, j);
					if (w < prev(i, j)) {	// k在路径上.
						result(i, j) = w;
						// k在路径上, 那么:
						// i到j的, 中间节点上界为k的前驱 = 中间节点上界为k-1, k到j的前驱.
						π(i, j) = prev_π(k, j);

						// 或者直接记录后继:
						// http://en.wikipedia.org/wiki/Floyd%E2%80%93Warshall_algorithm
						// next(i, j) = next(i, k);
					}
					else {
						result(i, j) = prev(i, j);
						// k不在路径上, 那么:
						// i到j的, 中间节点上界为k的前驱 = 中间节点上界为k-1, i到k的前驱.
						π(i, j) = prev_π(i, j);
					}
				}
			}
		}
	}

	bool Johnson() {
		// let w'(u, v) = w(u, v) + δ(u) - δ(v).
		// Prove:
		// for δ(u) + w(u, v) >= δ(v)
		// δ(u) - δ(v) >= -w(u, v)
		// H(u, v) >= 0.
		// End
		// Run Dijkstra.
		return false;
	}
private:
	class GraphContainer;
	class WeightContainer;
	typedef class Vertex;
	typedef Vertex* VertexPtr;
	bool BellmanFord(GraphContainer& g, WeightContainer& w, VertexPtr src) const;
	void Dijkstra(GraphContainer& g, WeightContainer& w, VertexPtr src) const;
};

int main() {
}

