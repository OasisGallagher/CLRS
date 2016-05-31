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

	// ������ʾ��SP����ΪM, �����е�Ԫ��(i, j)��ʾ��i��j��SPȨֵ��.
	// ��M[lhs] = x.
	// �÷�����lhs�Ļ�����, ͨ��rhs, ��չlhs��Mֵ.
	// ���յĽ��M[result] = x + M[rhs].
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

	// ����Ŀ����Ϊ�˵õ�matrix^(n-1), ��, ����matrix^k, k > (n-1),
	// ��Ȼ��matrix^(n-1)���.
	Matrix MatrixMultiplicationAllPairsShortestPaths2() {	// O(n^3log(n)).
		Matrix result = matrix;
		for (size_t i = 1; i < (matrix.Dimension() - 1); i *= 2) {
			result *= result;
		}

		// ��Ϊ��ÿ�ν���for�ıȽ�ʱ, �õ���resultΪmatrix^i.
		// ����i > matrix.Dimension -1.
		// forѭ��������ceil(log(matrix.Dimension - 1))��.

		return result;
	}
	
	//            { W(i, j)					if k = 0
	// Dk(i, j) = |
	//            { min(D(k-1)(i, j), D(k-1)(i, k) + d(k-1)(k, j).
	// Dk(i, j)��ʾʹ��[1, k]�����ڵĶ���ʱ, i��j�����·������.
	Matrix FloydWarshall() {	// O(n^3).
		Matrix result = matrix;
		// ��ʼ����.
		// ���i == j or w(i, j) = INF, ��(i, j) = INF.
		// ����, ��(i, j) = w(i, j).
		Matrix ��(matrix.Dimension());

		for (size_t i = 0; i < ��.Dimension(); ++i) {
			for (size_t j = 0; j < ��.Dimension(); ++j) {
				if (i == j || matrix(i, j) == Matrix::MaxWeight)
					��(i, j) = Matrix::InvalidVertex;
				else
					��(i, j) = i;
			}
		}

		for (size_t k = 1; k < matrix.Dimension(); ++k) {
			//Matrix prev = result;
			// ���Զ����ϱ�, ��ʹ��resultk, ����result_(k-1).see CLRS 25.2-4.
			Matrix& prev = result;
			Matrix prev_�� = ��;
			for (size_t i = 0; i < matrix.Dimension(); ++i) {
				for (size_t j = 0; j < matrix.Dimension(); ++j) {
					Matrix::Weight w = prev(i, k) + prev(k, j);
					if (w < prev(i, j)) {	// k��·����.
						result(i, j) = w;
						// k��·����, ��ô:
						// i��j��, �м�ڵ��Ͻ�Ϊk��ǰ�� = �м�ڵ��Ͻ�Ϊk-1, k��j��ǰ��.
						��(i, j) = prev_��(k, j);

						// ����ֱ�Ӽ�¼���:
						// http://en.wikipedia.org/wiki/Floyd%E2%80%93Warshall_algorithm
						// next(i, j) = next(i, k);
					}
					else {
						result(i, j) = prev(i, j);
						// k����·����, ��ô:
						// i��j��, �м�ڵ��Ͻ�Ϊk��ǰ�� = �м�ڵ��Ͻ�Ϊk-1, i��k��ǰ��.
						��(i, j) = prev_��(i, j);
					}
				}
			}
		}
	}

	bool Johnson() {
		// let w'(u, v) = w(u, v) + ��(u) - ��(v).
		// Prove:
		// for ��(u) + w(u, v) >= ��(v)
		// ��(u) - ��(v) >= -w(u, v)
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

