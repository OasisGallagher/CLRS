#include <iostream>
#include <vector>
#include <map>

class table {
	typedef unsigned value_type;
	typedef std::pair<unsigned, unsigned> key_type;
	typedef std::map<key_type, value_type> container_type;
	container_type cont;
public:
	value_type& operator()(key_type::first_type key1, key_type::second_type key2) {
		return cont[std::make_pair(key1, key2)];
	}
};

// c(i, x)为选择前i个物品, 背包总载重量为x时的可以取得的最大价值.
// w[i]为物品i的重量. v[i]为物品i的价值.
//           { c(i-1, x)								if w[i] > x
// c(i, x) = | max(c(i-1, x), c(i-1, x - w[i]) + v[i])	if w[i] <= x
//           { 0										if i == 0 or x == 0

struct Goods {
	Goods(unsigned _W, unsigned _V) : w(_W), v(_V) { }
	unsigned w;
	unsigned v;
};

typedef std::vector<Goods> GoodsContainer;

void solution(table& s, GoodsContainer& goods, unsigned n, unsigned w) {
	if (n == 0 || w == 0)
		return;

	unsigned sel = s(n, w);
	if (sel != 0) {
		solution(s, goods, n - 1, w - goods[n].w);
		std::cout << "Goods " << n << " selected\n";
	}
	else {
		solution(s, goods, n - 1, w);
	}
}

unsigned _01knapsack(GoodsContainer& goods, unsigned total_weight) {
	GoodsContainer::size_type n = goods.size() - 1;
	table c;
	table s;

	for (size_t i = 0; i <= n; ++i)
		c(i, 0) = 0;
	for (size_t i = 0; i <= total_weight; ++i)
		c(0, i) = 0;

	for (unsigned i = 1; i <= n; ++i) {
		for (unsigned x = 1; x <= total_weight; ++x) {
			unsigned max_v = c(i - 1, x);
			unsigned sel = 0;
			if (goods[i].w <= x) {
				unsigned t = c(i - 1, x - goods[i].w) + goods[i].v;
				if (t > max_v) {
					max_v = t;
					sel = 1;
				}
			}

			c(i, x) = max_v;
			s(i, x) = sel;
		}
	}

	solution(s, goods, n, total_weight);

	return c(n, total_weight);
}

int main() {
	GoodsContainer cont;
	cont.push_back(Goods(0, 0));
	cont.push_back(Goods(10, 60));
	cont.push_back(Goods(20, 100));
	cont.push_back(Goods(30, 120));
	std::cout << "max price: " << _01knapsack(cont, 50) << std::endl;
}

