#include <vector>
#include <list>
#include <cassert>
#include <cstdlib>
#include <numeric>
#include <algorithm>

typedef std::vector<int> Vector;
typedef std::list<int> List;

// �ӳ���Ϊm��vector��, ѡ��n��Ԫ��(n <= m), ʹ��ÿ��Ԫ�ر�ѡ�еĸ�����ͬ.
// Fisher-Yates shuffle: http://stackoverflow.com/questions/9345087/choose-m-elements-randomly-from-a-vector-containing-n-elements
// ��n << mʱ, ��ֱ��random_shuffleЧ�ʸߺܶ�.
// ֤��:
// ��������Ԫ��, ���ձ��ŵ�ǰn��λ�õĸ���Ϊ(1-����ѡ��ĸ���):
// 1 - ((m-1)/m * (m-2)/(m-1) * (m-n)/(m-n-1)) = 1 - (m-n)/m = n/m.
// ע��, input�ᱻ�ı�.
Vector RandomSampling(Vector& input, size_t n) {
	const size_t m = input.size();
	assert(m >= n);
	const size_t desiredCount = n;

	// ��m��Ԫ����, ��ȡһ��Ԫ��, �ŵ�input��0#λ��.
	// �ӵ���ǰm-1��Ԫ��, ��ȡһ��Ԫ�طŵ�1#λ��.
	// ...
	// �ӵ���ǰm-(n-1)��Ԫ��, ��ȡһ��Ԫ�طŵ�n-1#λ��.
	for(size_t i = 0; n != 0; --n, ++i) {
		size_t pos = i + rand() % (m - i);
		if(pos != i)
			std::swap(input[pos], input[i]);
	}

	// ȡǰn��Ԫ��, ��Ϊ����.
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

// ����������, container, ֻҪ�����ṩforward iterator, ����������ǰ֪��
// container��size, �������ߵ���container�����ѡȡn��Ԫ��.
// Reservior Sampling(��ˮ�س���): http://stackoverflow.com/questions/54059/efficiently-selecting-a-set-of-random-elements-from-a-linked-list
// ֤��:
// ��S = container.size().
// ���n >= S, ��ôÿ��Ԫ�����ձ�ѡȡ�ĸ��ʶ�Ϊ1, ����֤��.
// ���n < S, ��ô, ����container�е�����λ��Ϊi��Ԫ��x,
// ��ѡ�����ս���ĸ���Ϊ:
// ���i>=n, Ҳ�����ڵڶ���forѭ��֮�ڲŻᱻread,����ѡȡ�ĸ���Ϊn/(i+1), ����result�ĵ�m��λ��.
// ��, ������j > i, ��Ҫ:
// (��j��Ԫ�ر�ѡ�� && û���滻result��mλ��)����(��j��Ԫ��û�б�ѡ��),
// ���ߵĸ��ʺ�Ϊ: n/(j+1) * (n-1)/n + (j-n+1)/(j+1) = j/(j+1). ����֮���S-i��Ԫ��, �ܵĸ���Ϊ:
// (n/(i+1)) * ((i+1)/(i+2)) * ((i+2)/(i+3)) * ... * ((S-1) / S) = n/S.
// ���i<n, �ڵ�һ��forѭ����read, ֻ��Ҫ��֤, ��ȡ��container�еĵ�j(j>=n)���Լ��Ժ��Ԫ��ʱ, ��Ԫ��
// ���ᱻ�滻��, ����Ϊ(j/(j+1)) * ((j+1)/(j+2)) * ... * ((S-1)/S) = j/S = n/S.
// ���, container�е�ÿ��Ԫ�ر�����ѡȡ�ĸ��ʶ�Ϊn/S.
Vector RandomSampling(const Container& container, size_t n) {
	Vector result;

	// ��container�е�ǰmin(n,container.size())��Ԫ�ؼ��뵽result��.
	for (size_t i = 0; i < n && !container.eof(); ++i)
		result.push_back(container.read());

	// ��container�еĵ�i(i >= n)��Ԫ�ؿ�ʼ, ȡ�����r����[0, i+1],
	// ���r < n(����Ϊn/(i+1)), ��ô, ��result�еĵ�r��Ԫ���滻Ϊcontainer�еĵ�i��Ԫ��.
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
