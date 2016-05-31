#include <iostream>
#include <map>
#include <string>
#include <cassert>

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

static unsigned __memo_lcs(_2dmap& result, _2dmap& memo, const sequence& s1, const sequence& s2, size_t i, size_t j) {
	if(memo(i, j) != std::numeric_limits<unsigned>::max())
		return memo(i, j);

	int w = DEC_BOTH;
	if(s1[i - 1] == s2[j - 1])
		memo(i, j) = __memo_lcs(result, memo, s1, s2, i - 1, j - 1) + 1;
	else {
		w = DEC_I;
		unsigned q = __memo_lcs(result, memo, s1, s2, i - 1, j);
		unsigned r = __memo_lcs(result, memo, s1, s2, i, j - 1);
		if(r > q) {
			w = DEC_J;
			q = r;
		}
		memo(i, j) = q;
	}

	result(i, j) = w;

	return memo(i, j);
}

static void memo_lcs(_2dmap& result, const sequence& s1, const sequence& s2) {
	_2dmap memo;
	for(size_t i = 1; i <= s1.length(); ++i)
		memo(i, 0) = 0;
	for(size_t j = 0; j <= s2.length(); ++j)
		memo(0, j) = 0;

	for(size_t i = 1; i <= s1.length(); ++i)
		for(size_t j = 1; j <= s2.length(); ++j)
			memo(i, j) = std::numeric_limits<unsigned>::max();

	__memo_lcs(result, memo, s1, s2, s1.length(), s2.length());
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
	memo_lcs(sol, s1, s2);
	solution(s1, sol, s1.length(), s2.length());
	std::cout << std::endl;
}

// output: 100110
