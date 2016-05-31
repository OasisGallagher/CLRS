#include <iostream>
#include <numeric>
#include <vector>
#include <algorithm>

template <class value_type>
class maxheap {
	typedef std::vector<value_type> container;
	container cont_;
	typedef typename container::size_type index_type;
	index_type parent(index_type i) {
		return i / 2;
	}
	index_type lchild(index_type i) {
		return i * 2;
	}
	index_type rchild(index_type i) {
		return i * 2 + 1;
	}
	void heapify(index_type p) {
		index_type max_pos = p;
		while(true) {
			index_type child = lchild(p);
			if(child < cont_.size() && cont_[child] > cont_[max_pos])
				max_pos = child;
			
			if((child = rchild(p)) < cont_.size() && cont_[child] > cont_[max_pos])
				max_pos = child;

			if(max_pos == p)
				break;
			std::swap(cont_[max_pos], cont_[p]);

			p = max_pos;
		}
	}
public:
	maxheap() {
		// place holder.
		cont_.push_back(value_type());
	}
	void push(const value_type& val) {
		cont_.push_back(val);
		index_type i = cont_.size() - 1;
		for(; i > 1 && cont_[parent(i)] < cont_[i]; ) {
			std::swap(cont_[parent(i)], cont_[i]);
			i = parent(i);
		}
	}
	value_type pop() {
		if(empty())
			throw std::runtime_error("heap underflow");
		value_type result = cont_[1];
		cont_[1] = cont_.back();
		cont_.pop_back();
		heapify(1);

		return result;
	}

	bool empty() const {
		return cont_.size() == 1;
	}
};

void test_heap()
{
	std::vector<int> cont(10, 1);
	std::partial_sum(cont.begin(), cont.end(), cont.begin());
	std::random_shuffle(cont.begin(), cont.end());

	// O(N)时间建堆.
	maxheap<int> heap;
	for(std::vector<int>::iterator ite = cont.begin(); ite != cont.end(); ++ite)
		heap.push(*ite);

	// 获取前5个最大值.
	// 每次都以O(lg(N))时间重建堆.
	for(int i = 0; i < 5; ++i)
		std::cout << heap.pop() << std::endl;
	// 但是, 相比于select的期望时间O(N), heap可以将取得的M个元素, 按照顺序取出.
}

int main() {
	test_heap();
	return 0;
}
