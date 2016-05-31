#include <map>
#include <vector>
#include <iostream>
#include <algorithm>

class table {
private:
	typedef std::pair<unsigned, unsigned> key_type;
	typedef unsigned value_type;
	typedef std::map<key_type, value_type> container_type;
	container_type cont_;
public:
	value_type& operator() (key_type::first_type key1, key_type::second_type key2) {
		return cont_[std::make_pair(key1, key2)];
	}
};

struct activity {
	activity(unsigned _s, unsigned _f) : s(_s), f(_f) {}
	activity() { }
	bool operator < (const activity& other) const {
		return f < other.f;
	}
	unsigned s;
	unsigned f;
};

typedef std::vector<activity> activity_container;

void prepare(activity_container& cont) {
	activity dummy;
	dummy.s = dummy.f = 0;
	cont.insert(cont.begin(), dummy);
	dummy.s = dummy.f = std::numeric_limits<unsigned>::max();
	cont.push_back(dummy);
}

void solution(table& cont, unsigned i, unsigned j) {
	unsigned k = cont(i, j);
	if(k != 0) {
		std::cout << "activity " << k << std::endl;
		solution(cont, i, k);
		solution(cont, k, j);
	}
}

unsigned activity_select(activity_container& cont) {
	prepare(cont);
	table c;
	table ac;
	typedef activity_container::size_type size_type;
	size_type n = cont.size();
	for(size_type len = 2; len <= n; ++len) {
		for(size_type i = 0; i <= n - len; ++i) {
			size_type j = i + len - 1;
			unsigned m = std::numeric_limits<unsigned>::min();
			for(size_type k = i; k <= j; ++k) {
				if(!(cont[k].s > cont[i].f && cont[k].f < cont[j].s))
					continue;

				unsigned t = c(i, k) + 1 + c(k, j);
				if(t > m) {
					m = t;
					ac(i, j) = k;
				}
			}

			c(i, j) = m;
		}
	}

	solution(ac, 0, cont.size() - 1);

	return c(0, cont.size() - 1);
}

unsigned gready_activity_select(const activity_container& cont) {
	std::vector<unsigned> result(1, 0);
	for(size_t i = 1, j = 0; i < cont.size(); ++i) {
		if(cont[i].s >= cont[j].f) {
			result.push_back(i);
			j = i;
		}
	}

	for(std::vector<unsigned>::iterator ite = result.begin(); ite != result.end(); ++ite)
		std::cout << "activity " << *ite << std::endl;

	return result.size();
}

int main() {
	activity_container cont;
	cont.push_back(activity(1, 4));
	cont.push_back(activity(3, 5));
	cont.push_back(activity(0, 6));
	cont.push_back(activity(5, 7));
	cont.push_back(activity(3, 8));
	cont.push_back(activity(5, 9));
	cont.push_back(activity(6, 10));
	cont.push_back(activity(8, 11));
	cont.push_back(activity(8, 12));
	cont.push_back(activity(2, 13));
	cont.push_back(activity(12, 14));

	std::sort(cont.begin(), cont.end());

	std::cout << gready_activity_select(cont) << std::endl;
	std::cout << activity_select(cont) << std::endl;
}

