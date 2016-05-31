#include <iostream>
#include <map>
#include <string>

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

typedef std::string sequence;

enum { DEC_BOTH, DEC_I, DEC_J };

static void lcs(_2dmap& result, const sequence& s1, const sequence& s2) {
	_2dmap c;
	typedef sequence::size_type size_type;
	size_type len1 = s1.length(), len2 = s2.length();

	for(size_type i = 1; i <= len1; ++i)
		c(i, 0) = 0;

	for(size_type j = 0; j <= len2; ++j)
		c(0, j) = 0;

	for(size_type i = 1; i <= len1; ++i) {
		for(size_type j = 1; j <= len2; ++j) {
			int w = DEC_BOTH;
			if(s1[i - 1] == s2[j - 1])
				c(i, j) = c(i - 1, j - 1) + 1;
			else {
				w = DEC_I;
				unsigned q = c(i - 1, j);
				if(c(i, j - 1) > q) {
					w = DEC_J;
					q = c(i, j - 1);
				}
				c(i, j) = q;
			}

			result(i, j) = w;
		}
	}
}

static void solution(const sequence& seq, _2dmap& sol, unsigned i, unsigned j) {
	if(i != 0 && j != 0) {
		if(sol(i, j) == DEC_BOTH) {
			solution(seq, sol, i - 1, j - 1);
			std::cout << seq[i - 1];
		}
		else if(sol(i, j) == DEC_I)
			solution(seq, sol, i - 1, j);
		else
			solution(seq, sol, i, j - 1);
	}
}

int main() {
	sequence s1 = "10010101", s2 = "010110110";
	_2dmap sol;
	lcs(sol, s1, s2);
	solution(s1, sol, s1.length(), s2.length());
	std::cout << std::endl;
}

// output: 100110
