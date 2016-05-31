#include <stack>

class Queue {	
	typedef std::stack<int> S;
	typedef S::value_type V;
	S in, out;
	int pop(S& s) {
		V tmp = s.top();
		s.pop();
		return tmp;
	}
public:
	void push(V dat) {
		in.push(dat);
	}

	V pop() {
		if(out.empty()) {
			while(!in.empty()) {
				out.push(pop(in));
			}
		}

		return pop(out);
	}

	// 定义势函数: Pi = 4*size[in] + size[out].
	// - 如果第i个操作是push,
	//   Pi(i) - Pi(i-1) = 4.
	//   因此, 第i个操作的平摊代价为: 4 + 1 = 5.
	// - 如果第i个操作是pop, 假设此时size[in] = c[i]
	//   操作的实际代价为3*c[i] + 1.(while, pop(in), push()各占c[i]).
	//   势能变化为: -4*c[i] + c[i] - 1 = -3*c[i] - 1
	//   因此, 第i个操作的平摊代价为: 0.
	//   Pi(n) - Pi(0) >= 0, 且Pi(i) >= 0, 所以, push和pop
	//   操作的平摊代价为O(1).
};

