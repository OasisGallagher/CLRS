#include <vector>
#include <list>
#include <cassert>
#include <cstdlib>
#include <numeric>
#include <algorithm>

typedef std::vector<int> Vector;
typedef std::list<int> List;

// 从长度为m的vector中, 选择n个元素(n <= m), 使得每个元素被选中的概率相同.
// Fisher-Yates shuffle: http://stackoverflow.com/questions/9345087/choose-m-elements-randomly-from-a-vector-containing-n-elements
// 当n << m时, 比直接random_shuffle效率高很多.
// 证明:
// 对于任意元素, 最终被放到前n个位置的概率为(1-不被选择的概率):
// 1 - ((m-1)/m * (m-2)/(m-1) * (m-n)/(m-n-1)) = 1 - (m-n)/m = n/m.
// 注意, input会被改变.
Vector RandomSampling(Vector& input, size_t n) {
	const size_t m = input.size();
	assert(m >= n);
	const size_t desiredCount = n;

	// 从m个元素中, 任取一个元素, 放到input的0#位置.
	// 从倒数前m-1个元素, 任取一个元素放到1#位置.
	// ...
	// 从倒数前m-(n-1)个元素, 任取一个元素放到n-1#位置.
	for(size_t i = 0; n != 0; --n, ++i) {
		size_t pos = i + rand() % (m - i);
		if(pos != i)
			std::swap(input[pos], input[i]);
	}

	// 取前n个元素, 即为所求.
	return Vector(input.begin(), input.begin() + desiredCount);
}

class Container {
	List container;
	mutable List::const_iterator current;
public:
	Container() {
		Vector numbers(100, 1);
		std::partial_sum(numbers.begin(), numbers.end(), numbers.begin());
		container.assign(numbers.begin(), numbers.end());
		current = container.begin();
	}

	bool eof() const {
		return current == container.end();
	}

	int read() const {
		return *current++;
	}
};

// 对任意容器, container, 只要它能提供forward iterator, 而且无需提前知道
// container的size, 可以在线的在container中随机选取n个元素.
// Reservior Sampling(蓄水池抽样): http://stackoverflow.com/questions/54059/efficiently-selecting-a-set-of-random-elements-from-a-linked-list
// 证明:
// 设S = container.size().
// 如果n >= S, 那么每个元素最终被选取的概率都为1, 无需证明.
// 如果n < S, 那么, 对于container中的任意位置为i的元素x,
// 被选入最终结果的概率为:
// 如果i>=n, 也就是在第二个for循环之内才会被read,它被选取的概率为n/(i+1), 放在result的第m个位置.
// 且, 对任意j > i, 需要:
// (第j个元素被选中 && 没有替换result的m位置)或者(第j个元素没有被选中),
// 二者的概率和为: n/(j+1) * (n-1)/n + (j-n+1)/(j+1) = j/(j+1). 对于之后的S-i个元素, 总的概率为:
// (n/(i+1)) * ((i+1)/(i+2)) * ((i+2)/(i+3)) * ... * ((S-1) / S) = n/S.
// 如果i<n, 在第一个for循环被read, 只需要保证, 读取到container中的第j(j>=n)个以及以后的元素时, 该元素
// 不会被替换掉, 概率为(j/(j+1)) * ((j+1)/(j+2)) * ... * ((S-1)/S) = j/S = n/S.
// 因此, container中的每个元素被最终选取的概率都为n/S.
Vector RandomSampling(const Container& container, size_t n) {
	Vector result;

	// 将container中的前min(n,container.size())个元素加入到result中.
	for (size_t i = 0; i < n && !container.eof(); ++i)
		result.push_back(container.read());

	// 从container中的第i(i >= n)个元素开始, 取随机数r属于[0, i+1],
	// 如果r < n(概率为n/(i+1)), 那么, 将result中的第r个元素替换为container中的第i个元素.
	for (size_t i = n; !container.eof(); ++i) {
		size_t r = rand() % (i + 1 + 1);	// [0, i + 1].
		int number = container.read();
		if (r < n)
			result[r] = number;
	}

	return result;
}

int main() {
	Container cont;
	Vector result = RandomSampling(cont, 10);
}
