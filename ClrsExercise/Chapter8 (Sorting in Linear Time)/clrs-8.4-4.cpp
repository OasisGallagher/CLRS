// �������еĵ㶼��Բ֮�ھ��ȷֲ�, ���, ��Ͱ�Ĵ�С����Ϊr1, r2, r3 ... rn,
// ʹ��, ͬ��Բc1, c2, c3 .. cn��ɵ�һ����Բ��������.
// Ϊ�˽���;��ȵķֲ�������Ͱ, �����ж�, ����õ㵽Բ�ĵľ�����[0, r1),
// ��ô, ����1��Ͱ; ��[r1, r2)����2��Ͱ, �Դ�����.
// ��Ϊ����Բ����������, �������շ��䵽����Ͱ��Ԫ�ظ���Ҳ�����.
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

	// ����ֵ�ŵ�ָ����bucket��.
	for(unsigned i = 0; i < count; ++i) {
		if(range[i] < 0.f || range[i] >= 1.f) throw "argument error";
		vec[unsigned(range[i] * BucketCount)].push_back(range[i]);
	}
	
	// ��ÿ��bucket, ���в�������.
	for(BucketVector::iterator ite = vec.begin(); ite != vec.end(); ++ite) {
		std::_Insertion_sort(ite->begin(), ite->end());
	}

	// �ϲ����.
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

