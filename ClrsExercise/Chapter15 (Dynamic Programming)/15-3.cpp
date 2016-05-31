#include <map>
#include <vector>
#include <iostream>

// (1)
// X(i)为X的i前缀(如X = "abcdef", X(2)="ab", X(0)为空.
// Y(j)为Y的j前缀.
// 记c(i, j)为从X(i)->Y(j)的最小代价.
// 分析过程的最后一个操作:
// copy: 必有X[i] == Y[j], 递归式: c(i, j) = c(i-1, j-1) + cost(copy).
// replace: 必有X[i] != Y[j], 递归式: c(i, j) = c(i-1, j-1) + cost(replace).
// delete: 递归式: c(i, j) = c(i-1, j) + cost(delete).
// insert: 递归式: c(i, j) = c(i, j-1) + cost(insert).
// twiddle: 必有: i >= 2 and j >= 2 and X[i-1] == Y[j] and X[i] == Y[j-1]
//			递归式: c(i, j) = c(i-2, j-2) + cost(twiddle).
// kill: 根据题意, 如果执行kill, 那么一定是最后一个操作, 而且, kill前后,
//       此时必有: i=m, j=n.
//       j的值在最后一次操作后是不变的, 所以:
//       c(i, j) = c(m, n) = min(0 <= p < m)c(p,n) + cost(kill).
//
// c(i, 0)为i次kill的代价.
// c(0, j)为j次insert的代价.
//

class table { 
	typedef std::pair<unsigned, unsigned> key_type;
	typedef unsigned value_type;
	typedef std::map<key_type, value_type> container_type;
	container_type cont_;
public:
	value_type& operator() (key_type::first_type key1, key_type::second_type key2) {
		return cont_[std::make_pair(key1, key2)];
	}
};

enum option{ op_copy, op_replace, op_delete, op_insert, op_twiddle, op_kill };

const char* op_str(option op) {
	const char* str[] = {
		"copy",
		"replace",
		"delete",
		"insert",
		"twiddle",
		"kill"
	};
	return str[op];
}

typedef std::string string;
typedef std::vector<unsigned> cost_table;

unsigned costof(const cost_table& tbl, option op) {
	return op;
}

void solution(table& opt, unsigned m, unsigned n) {
	if (m == 0 && n == 0)
		return;
	option p = option(opt(m, n));
	unsigned m2 = m, n2 = n;
	switch (p) {
	case op_copy:
	case op_replace:
		m2 = m - 1;
		n2 = n - 1;
		break;
	case op_delete:
		m2 = m - 1;
		break;
	case op_insert:
		n2 = n - 1;
		break;
	case op_twiddle:
		m2 = m - 2;
		n2 = n - 2;
		break;
	default:
		m2 = opt(m, n);
		break;
	}
	solution(opt, m2, n2);
	std::cout << op_str(p) << std::endl;
}

unsigned edit_distance(const string& X, const string& Y, const cost_table& costtbl) {
	table c, opt;
	unsigned m = X.size() - 1, n = Y.size() - 1;
	for (unsigned i = 0; i <= m; ++i) {
		c(i, 0) = i * costof(costtbl, op_delete);
		opt(i, 0) = op_delete;
	}

	for (unsigned j = 0; j <= n; ++j) {
		c(0, j) = j * costof(costtbl, op_insert);
		opt(0, j) = op_insert;
	}

	for(unsigned i = 1; i <= m; ++i) {
		for(unsigned j = 1; j <= n; ++j) {
			unsigned min_cost = std::numeric_limits<unsigned>::max();
			option op = op_copy;
			unsigned t = 0;
			if(X[i] == Y[j]) {
				// copy.
				t = c(i - 1, j - 1) + costof(costtbl, op_copy);
				if(t < min_cost) {
					min_cost = t;
					op = op_copy;
				}
			}
			else {
				// replace.
				t = c(i - 1, j - 1) + costof(costtbl, op_replace);
				if(t < min_cost) {
					min_cost = t;
					op = op_replace;
				}
			}

			// delete.
			t = c(i - 1, j) + costof(costtbl, op_delete);
			if(t < min_cost) {
				min_cost = t;
				op = op_delete;
			}

			// insert.
			t = c(i, j - 1) + costof(costtbl, op_insert);
			if(t < min_cost) {
				min_cost = t;
				op = op_insert;
			}

			// twiddle.
			if(i >= 2 && j >= 2 && X[i - 1] == Y[j] && X[i] == Y[j - 1]) {
				t = c(i - 2, j - 2) + costof(costtbl, op_twiddle);
				if(t < min_cost) {
					min_cost = t;
					op = op_twiddle;
				}
			}

			// kill.
			if(i == m && j == n) {
				for(unsigned p = 0; p < m; ++p) {
					t = c(p, Y.size()) + costof(costtbl, op_kill);
					if(t < min_cost) {
						min_cost = t;
						op = option(p);// op_kill;
					}
				}
			}

			c(i, j) = min_cost;
			opt(i, j) = op;
		}
	}

	solution(opt, m, n);
	return c(m, n);
}

int main() {
	cost_table tbl;
	string X("al"), Y("ad");
	X.insert(X.begin(), ' ');
	Y.insert(Y.begin(), ' ');
	std::cout << edit_distance(X, Y, tbl) << std::endl;

	option p = option(100);
}

// (2)
// X[j] == Y[j] != [space], 相当于copy.
// X[j] != Y[j] != [space], 相当于replace.
// X[j] == [space] or Y[j] == [space], 如果X[j] == [space], 表示是insert; 
// 否则是delete.
// 置kill和twiddle和kill的代价为INF.
// 为了最小化编辑距离, 置copy的代价为-1, replace为1, insert/delete的代价为2.

