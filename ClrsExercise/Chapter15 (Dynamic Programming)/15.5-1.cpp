#include <map>
#include <vector>
#include <limits>
#include <iostream>

template <class value_type>
class table {
	typedef std::pair<unsigned, unsigned> key_type;
	typedef std::map<key_type, value_type> container_type;
	container_type cont;
public:
	value_type& operator()(key_type::first_type key1, key_type::second_type key2) {
		return cont[std::make_pair(key1, key2)];
	}
};

typedef float probability;
typedef std::vector<probability> node_container;

const char* root_fmt = "K%d is the root.\n";
const char* k_lchild_fmt = "K%d is the left child of K%d.\n";
const char* k_rchild_fmt = "K%d is the right child of K%d.\n";
const char* d_lchild_fmt = "D%d is the left child of K%d.\n";
const char* d_rchild_fmt = "D%d is the right child of K%d.\n";

// returns the node index mapped by t[first, last].
unsigned solution(table<unsigned>& t, size_t first, size_t last, bool rooted = true) {
	if(first <= last) {
		unsigned mid = t(first, last);
		if(!rooted) {
			printf(root_fmt, mid);
		}
		// left part.
		unsigned r = solution(t, first, mid - 1);
		if(r != 0)
			printf(k_lchild_fmt, r, mid);
		else
			printf(d_lchild_fmt, mid - 1, mid);

		// right part.
		r = solution(t, mid + 1, last);
		if(r != 0)
			printf(k_rchild_fmt, r, mid);
		else
			printf(d_rchild_fmt, mid + 1, mid);
		return mid;
	}
	return 0;
}

float optimal_bst(const node_container& p, const node_container& q) {
	if(p.size() + 1 != q.size())
		throw std::logic_error("incompitable probabilities");

	table<float> w, e;
	table<unsigned> root;
	for(size_t i = 1; i <= p.size() + 1; ++i) {
		e(i, i - 1) = q[i - 1];	
		w(i, i - 1) = q[i - 1];
	}

	for(size_t len = 1; len <= p.size(); ++len) {
		for(size_t i = 1; i <= p.size() - len + 1; ++i) {
			size_t j = i + len - 1;
			e(i, j) = std::numeric_limits<float>::max();
			w(i, j) = w(i, j - 1) + p[j - 1] + q[j];
			for(size_t r = i; r <= j; ++r) {
				float t = e(i, r - 1) + e(r + 1, j) + w(i, j);
				if(t < e(i, j)) {
					e(i, j) = t;
					root(i, j) = r;
				}
			}
		}
	}

	solution(root, 1, p.size(), false);

	return e(1, p.size());
}

int main() {
	const float p[] = { 0.15f, 0.10f, 0.05f, 0.10f, 0.20f };
	const float q[] = { 0.05f, 0.10f, 0.05f, 0.05f, 0.05f, 0.10f };
	std::cout << optimal_bst(node_container(p, p + _countof(p)), node_container(q, q + _countof(q))) << std::endl;
}
