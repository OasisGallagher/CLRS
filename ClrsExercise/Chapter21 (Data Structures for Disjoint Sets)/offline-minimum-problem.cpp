// Off-line minimum problem.
#include <vector>
#include <cstdarg>
#include <iterator>
#include <algorithm>
#include <string>
#include <iostream>

class OfflineMinimum {
	struct Element {
		int dat;
		int index;
	};
	typedef std::vector<unsigned> Array;
	struct Set {
		typedef std::vector<unsigned> Elements;
		Elements elem;
		int index;
		int rank;
	};
	typedef std::vector<Element> ElementContainer;
	ElementContainer elements;
	typedef std::vector<Set*> SetContainer;
	SetContainer sets;
public:
	~OfflineMinimum() {
		for (SetContainer::iterator ite = sets.begin(); ite != sets.end();
				++ite)
			delete *ite;
	}
	void read() {
		std::cout << "enter n: ";
		int n = 0;
		std::cin >> n;

		elements.resize(n);

		std::cout << "enter the sequence: ";
		std::string sequence;
		std::cin >> sequence;

		std::vector<Array> subsequences;
		size_t off = 0, pos;
		do {
			pos = sequence.find('E', off);
			if (pos == std::string::npos) {
				pos = sequence.size();
			}
			Array arr;
			std::transform(sequence.begin() + off, sequence.begin() + pos,
					std::back_inserter(arr), ctoi);
			subsequences.push_back(arr);
			off = pos + 1;
		} while (pos < sequence.size());


		sets.resize(subsequences.size());

		for (size_t i = 0; i < subsequences.size(); ++i)
			sets[i] = make_set(subsequences[i], i);
	}
	// 元素个数为n, 那么, for循环的次数为O(n).
	// find_set和make_set都被执行O(n)次, union_set最多执行n-1次.
	// 因此, 集合操作为O(3n) = O(n)次.
	// 总的效率为O(n*a(n)).
	// 其中a(n)为书中所述的一个增长极慢的函数.
	void compute() {
		Array extracted(sets.size() - 1);

		for (size_t i = 1; i <= elements.size(); ++i) {
			unsigned j = find_set(elements[i - 1]);
			if (j + 1 >= sets.size()) continue;

			extracted[j] = i;
			SetContainer::size_type l = j + 1;
			for (; l < sets.size() && sets[l] == NULL; ++l)
				;

			if (l < sets.size() && l != j)
				union_set(sets[j], sets[l]);
		}

		std::copy(extracted.begin(), extracted.end(), 
				std::ostream_iterator<int>(std::cout, " "));
		std::cout << std::endl;
	}

private:
	static int ctoi(char c) { return c - '0'; }
	Set* make_set(const Array& arr, int index) {
		Set* result = new Set;
		for (Array::const_iterator ite = arr.begin(); ite != arr.end();
				++ite)
		{
			Element& e = elements[*ite - 1];
			e.dat = *ite;
			e.index = index;
			result->elem.push_back(*ite - 1);
		}
		result->index = index;
		result->rank = 0;
		return result;
	}

	int find_set(Element x) {
		return x.index;
	}

	void link(Set* x, Set* y, int index) {
		std::copy(x->elem.begin(), x->elem.end(), 
				std::back_inserter(y->elem));

		for (Set::Elements::iterator ite = y->elem.begin(); 
				ite != y->elem.end(); ++ite)
			elements[*ite].index = index;
		y->index = index;
	}

	void union_set(Set*& x, Set*& y) {
		if (x->rank < y->rank) {
			link(x, y, y->index);
			delete x;
			x = NULL;
		}
		else {
			link(y, x, y->index);
			if (y->rank == x->rank)
				++x->rank;
			delete y;
			y = x;
			x = NULL;
		}
	}
};

int main() {
	OfflineMinimum obj;
	obj.read();
	obj.compute();
}

