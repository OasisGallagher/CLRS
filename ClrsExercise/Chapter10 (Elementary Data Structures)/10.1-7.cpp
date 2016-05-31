#include <queue>

class Stack {
	typedef std::queue<int> Q;
	typedef Q::value_type V;
	Q first, second;
	Q *in, *out;

	V pop(Q& q) {
		V tmp = q.front();
		q.pop();
		return tmp;
	}
public:
	Stack() : in(&first), out(&second) { }
	void push(V dat) {
		in->push(dat);
	}

	V pop() {
		while(in->size() > 1) {
			out->push(pop(*in));
		}
		std::swap(in, out);
		return pop(*out);
	}

	// 由于每次pop, 都需要拷贝in中O(size[in])个值到out中,
	// 所以n次对该容器的操作的代价为O(n^2).
};

