// CLRS 7-6.
#include <stdio.h>
#include <assert.h>
#include <time.h>
#include <stdlib.h>

struct range {
	typedef int value_t;
	value_t low;
	value_t high;
	// swap this range with another.
	void swapwith(range& other) {
		if(this != &other) {
			__swap_helper(low, other.low);
			__swap_helper(high, other.high);
		}
	}
	// inner function.
	void __swap_helper(value_t& x, value_t& y) {
		value_t tmp = x; x = y; y = tmp;
	}
};

struct pair {
	pair(const range::value_t& _1st, const range::value_t& _2nd)
		: first(_1st), second(_2nd) { }
	range::value_t first;
	range::value_t second;
};

static pair fuzzypartition(range* buffer, int from, int to) {
	assert(from >= 0 && to > 0 && to >= from);
	range::value_t key = buffer[to - 1].low;
	// i points to the position of the key eventually.
	int i = from - 1;
	// make [from, i + 1]::low be less than or equal to key.
	for(int j = from; j < to - 1; ++j) {
		if(buffer[j].low <= key && ++i != j)
			buffer[j].swapwith(buffer[i]);
	}
	
	// [from, i]::low is less than or equal to key by swapping key with range[i].
	buffer[++i].swapwith(buffer[to - 1]);

	// k points to the position where [from, k]::high is less than key.
	int k = from - 1;
	for(int m = from; m < i; ++m) {
		if(buffer[m].high < key && ++k != m)
			buffer[m].swapwith(buffer[k]);
	}

	// [from, k), both low and high is less than or equal to key.
	// range(s) in [k, i] all contain key, cos' lower bound of them is less than or equal to key,
	// upper bound of them is greater than or equal to key.
	return pair(k + 1, i + 1);
}

static void fuzzysort(range* buffer, int from, int to) {
	if(from < to) {
		pair pivot = fuzzypartition(buffer, from, to);
		fuzzysort(buffer, from, pivot.first);
		fuzzysort(buffer, pivot.second, to);
		// [pivot.first, pivot.second) is already sorted.
	}
}

#define NumberOfTestCases	(5)

static void trace(range* testcase, size_t count) {
	for(size_t i = 0; i < count; ++i) {
		printf("(%d, %d) ", testcase[i].low, testcase[i].high);
		if((i + 1) % 5 == 0) printf("\n");
	}
}

int main() {
	// build ranges.
	srand(time(NULL));
	range testcase[NumberOfTestCases];
	for(int i = 0; i < NumberOfTestCases; ++i) {
		int low = rand() % 20;
		int high = rand() % 20;
		if(low > high) {
			int tmp = low;
			low = high;
			high = tmp;
		}
		testcase[i].low = low;
		testcase[i].high = high;
	}
	
	trace(testcase, NumberOfTestCases);
	// sort them.
	fuzzysort(testcase, 0, NumberOfTestCases);

	trace(testcase, NumberOfTestCases);
	
	return 0;
}
// eof