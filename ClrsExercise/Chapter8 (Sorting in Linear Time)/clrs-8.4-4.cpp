// 由于所有的点都在圆之内均匀分布, 因此, 将桶的大小设置为r1, r2, r3 ... rn,
// 使得, 同心圆c1, c2, c3 .. cn组成的一个个圆环面积相等.
// 为了将点就均匀的分布到各个桶, 可以判断, 如果该点到圆心的距离在[0, r1),
// 那么, 放在1号桶; 在[r1, r2)放在2号桶, 以此类推.
// 因为各个圆环的面积相等, 所以最终分配到各个桶的元素个数也会相等.
//
// Bucket Sort.
#include <vector>
#include <algorithm>
#include <iostream>
#include <ctime>

typedef float Number;
void BucketSort(Number* range, unsigned count) {
	const int BucketCount = 10;
	typedef std::vector<Number> Bucket;
	typedef std::vector<Bucket> BucketVector;
	BucketVector vec;
	vec.resize(BucketCount);

	// 将数值放到指定的bucket中.
	for(unsigned i = 0; i < count; ++i) {
		if(range[i] < 0.f || range[i] >= 1.f) throw "argument error";
		vec[unsigned(range[i] * BucketCount)].push_back(range[i]);
	}
	
	// 对每个bucket, 进行插入排序.
	for(BucketVector::iterator ite = vec.begin(); ite != vec.end(); ++ite) {
		std::_Insertion_sort(ite->begin(), ite->end());
	}

	// 合并结果.
	unsigned j = 0;
	for(BucketVector::iterator ite = vec.begin(); ite != vec.end(); ++ite) {
		std::copy(ite->begin(), ite->end(), range + j);
		j += ite->size();
	}
}

int main() {
	float range[100];
	srand(time(NULL));

	for(int i = 0; i < _countof(range); ++i) {
		int r = rand() % 100;
		range[i] = r / 100.f;
	}
	BucketSort(range, _countof(range));
}

