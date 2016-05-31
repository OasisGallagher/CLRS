#include <algorithm>
#include <iterator>
#include <iostream>
#include <map>
#include <vector>

class _2dmap {
	typedef std::pair<unsigned, unsigned> key_type;
	typedef std::map<key_type, unsigned> container;
public:
	unsigned& operator()(key_type::first_type key1, key_type::second_type key2) {
		return impl_[std::make_pair(key1, key2)];
	}
private:
	container impl_;
};

struct matrix {
	unsigned c_row;
	unsigned c_column;
};

typedef std::vector<unsigned> matrix_dimension;
typedef std::vector<matrix> matrix_container;
static void init_matrix(matrix_container& result, const matrix_dimension& dimension) {
	for(matrix_dimension::const_iterator ite = ++dimension.begin(); ite != dimension.end(); ++ite) {
		matrix tmp;
		tmp.c_row = *(ite - 1);
		tmp.c_column = *ite;
		result.push_back(tmp);
	}
}

static void chain_order(_2dmap& result, const matrix_container& matrixes) {
	_2dmap cost;
	typedef matrix_container::size_type size_type;
	size_type count = matrixes.size();
	for(size_type i = 0; i <= count; ++i)
		cost(i, i) = 0;

	for(size_t i = 2; i <= count; ++i) {
		for(size_t s = 0; s <= count - i; ++s) {
			size_t t = s + i - 1;
			// split at k.
			cost(s, t) = std::numeric_limits<unsigned>::max();
			for(size_t k = s; k < t; ++k) {
				unsigned split = cost(s, k) + cost(k + 1, t) 
					+ matrixes[s].c_row * matrixes[k].c_column * matrixes[t].c_column;
				if(split < cost(s, t)) {
					cost(s, t) = split;
					result(s, t) = k;
				}
			}
		}
	}
}
static void solution(_2dmap& order, size_t first, size_t last) {
	if(first == last)
		std::cout << "A" << first + 1;
	else {
		std::cout << "(";
		unsigned k = order(first, last);
		solution(order, first, k);
		solution(order, k + 1, last); 
		std::cout << ")";
	}
}

int main() {
	const unsigned arr[] = { 5, 10, 3, 12, 5, 50, 6 }; 
	matrix_dimension d;
	std::copy(arr, arr + _countof(arr), std::back_inserter(d));
	matrix_container matrixes;
	init_matrix(matrixes, d);

	_2dmap order;
	chain_order(order, matrixes);

	solution(order, 0, matrixes.size() - 1);

	std::cout << std::endl;
}
// output:
// ((A1A2)((A3A4)(A5A6)))
