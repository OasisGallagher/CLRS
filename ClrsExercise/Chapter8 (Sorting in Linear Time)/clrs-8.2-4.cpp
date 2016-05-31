// clrs p100, 8.2-4.
#include <iostream>
#include <algorithm>
#include <stdexcept>

// n numbers, between[0, k). count how many numbers in range[a, b] in O(1).
class Counter {
public:
	Counter(unsigned MaxNumber, unsigned* Buffer, unsigned Count) {
		unsigned* pos = std::max_element(Buffer, Buffer + Count);
		if(pos == Buffer + Count || *pos > MaxNumber) throw std::invalid_argument("value in Buffer is greater than MaxNumber");

		mNumbers = new unsigned[Count];
		memcpy(mNumbers, Buffer, sizeof(unsigned) * Count);

		mUpper = MaxNumber + 1;

		mCount = new unsigned[mUpper]();
		memset(mCount, 0, sizeof(unsigned) * mUpper);

		for(unsigned i = 0; i < mUpper; ++i)
			++mCount[Buffer[i]];

		for(unsigned i = 1; i < mUpper; ++i)
			mCount[i] += mCount[i - 1];
	}
	~Counter() { delete[] mNumbers; delete[] mCount; }
	int GetCount(unsigned low, unsigned high) const {
		if(low > high) return 0;
		if(low >= mUpper) return 0;
		// mUpper must be greater than 0.
		low = std::min(mUpper - 1, low);
		high = std::min(mUpper - 1, high);
		return mCount[high] - ((low == 0) ? 0 : mCount[low - 1]);
	}
private:
	unsigned *mNumbers;
	// mCount[index] means the count of numbers less than of equal to index.
	unsigned *mCount;
	unsigned mUpper;
};

int main() {
	unsigned buffer[] = { 4, 5, 2, 3, 5, 7, 9, 7, 1, 8, 2, 5, 0, 7 };
	Counter ct(*std::max_element(buffer, buffer + _countof(buffer)), buffer, _countof(buffer));

	std::cout << ct.GetCount(0, 100) << std::endl;
	std::cout << ct.GetCount(7, 6) << std::endl;
	std::cout << ct.GetCount(6, 6) << std::endl;
	std::cout << ct.GetCount(7, 7) << std::endl;
	std::cout << ct.GetCount(6, 9) << std::endl;

	return 0;
}