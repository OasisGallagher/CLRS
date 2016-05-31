#include <vector>
#include <string>
#include <numeric>
#include <iostream>
/*
 m[j]为1到j的单词写入到文件中, 行末空白字符的立方和的最小值.
 这j个单词组成的最后一行, 为i, 最后一行的代价c(i, j), 有:
           { 0						如果这行是全文的最后一行.
 c(i, j) = { INF					如果(字符和 + j - i) > M
		   { M - (字符和 + j - i)

        { 0									j = 0
 m[j] = |
        { min(i = 1, j)(m[i - 1] + c(i, j))  otherwise.

 上式将m[j]转换为m[i - 1]的子问题, 子问题必然是最优的.
*/
static const unsigned M = 9;

typedef std::string word_type;
typedef std::vector<word_type> sequence_type;
typedef std::vector<unsigned> table;

unsigned calc_length(unsigned v, const word_type& w) {
	return v + w.length();
}

unsigned lc(const sequence_type& seq, unsigned i, unsigned j) {
	unsigned char_count = std::accumulate(seq.begin() + i, seq.begin() + j + 1, 0,
		calc_length);

	return char_count + j - i;
}

void trace(const sequence_type& seq, unsigned i, unsigned j) {
	for (; i <= j; ++i) {
		std::cout << seq[i] << " ";
	}
	std::cout << std::endl;
}

// k = q[i]表示, seq[1, i]在k位置分割出最后一行.
void solution(const sequence_type seq, const table& p, unsigned i) {
	if (i > 0) {
		unsigned k = p[i];
		solution(seq, p, k - 1);
		trace(seq, k, i);
	}
}

unsigned dp_work(const sequence_type& seq) {
	table m(seq.size(), 0);
	table p(seq.size(), 0);

	for(unsigned j = 1; j <= seq.size() - 1; ++j) {
		unsigned min = std::numeric_limits<unsigned>::max();
		for (unsigned i = j; i >= 1; --i) {
			unsigned lastline_cost = lc(seq, i, j);
			if (lastline_cost > M)
				break;
			lastline_cost = M - lastline_cost;
			// the last line.
			if (j == seq.size() - 1)
				lastline_cost = 0;

			unsigned t = lastline_cost;
			if (t != std::numeric_limits<unsigned>::max())
				t += m[i - 1];

			if(t < min) {
				min = t;
				p[j] = i;
			}
		}
		m[j] = min;
	}

	solution(seq, p, seq.size() - 1);

	return m[seq.size() - 1];
}

int main() {
	const char* seq[] = { "__placeholder", "nobody", "can", "give", "you", "freedom"};
	sequence_type cont(seq, seq + _countof(seq));
	std::cout << dp_work(cont) << std::endl;
}
