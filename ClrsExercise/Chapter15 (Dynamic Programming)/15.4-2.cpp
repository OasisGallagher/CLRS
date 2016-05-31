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

static void lcs(_2dmap* oc, const sequence& s1, const sequence& s2) {
	_2dmap c;
	typedef sequence::size_type size_type;
	size_type len1 = s1.length(), len2 = s2.length();

	for(size_type i = 1; i <= len1; ++i)
		c(i, 0) = 0;

	for(size_type j = 0; j <= len2; ++j)
		c(0, j) = 0;

	for(size_type i = 1; i <= len1; ++i) {
		for(size_type j = 1; j <= len2; ++j) {
			//int w = DEC_BOTH;
			if(s1[i - 1] == s2[j - 1])
				c(i, j) = c(i - 1, j - 1) + 1;
			else {
				// two chioces.
				//w = DEC_I;
				unsigned q = c(i - 1, j);
				if(c(i, j - 1) > q) {
					//w = DEC_J;
					q = c(i, j - 1);
				}
				c(i, j) = q;
			}
		}
	}

	*oc = c;
}

static int chk_from(_2dmap& c, const sequence& seq1, const sequence& seq2, unsigned i, unsigned j) {
	if(seq1[i - 1] == seq2[j - 1])
		return DEC_BOTH;
	if(c(i, j - 1) > c(i - 1, j))
		return DEC_J;

	return DEC_I;
}

static void solution(const sequence& seq1, const sequence& seq2, _2dmap& c, unsigned i, unsigned j) {
	if(i != 0 && j != 0) {
		switch(chk_from(c, seq1, seq2, i, j)) {
		case DEC_BOTH:
			solution(seq1, seq2, c, i - 1, j - 1);
			std::cout << seq1[i - 1];
			break;
		case DEC_I:
			solution(seq1, seq2, c, i - 1, j);
			break;
		default:
			solution(seq1, seq2, c, i, j - 1);
			break;
		}
	}
}

int main() {
	sequence s1 = "10010101", s2 = "010110110";
	_2dmap c;
	lcs(&c, s1, s2);
	solution(s1, s2, c, s1.length(), s2.length());
	std::cout << std::endl;
}

// output: 100110
