#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

// http://en.wikipedia.org/wiki/Longest_increasing_subsequence

typedef std::string array;
typedef std::vector<unsigned> table;

unsigned b_search(const table& t, unsigned last, const array& arr, array::value_type v) {
 	unsigned r = 0, first = 1;
 	unsigned count = last;
 	for(; count != 0; ) {
 		unsigned count2 = count / 2;
 		unsigned mid = first + count2;
 		if(arr[t[mid] - 1] < v) {
 			first = mid + 1;
 			count -= count2 + 1;
 		}
 		else
 			count = count2;
 	}
 
	return first - 1;
}

/*
template<class _RanIt> inline
void _Reverse(_RanIt _First, _RanIt _Last, random_access_iterator_tag)
{	// reverse elements in [_First, _Last), random-access iterators
	_DEBUG_RANGE(_First, _Last);
	for (; _First < _Last; ++_First)
		std::iter_swap(_First, --_Last);
}
*/

// longest increasing sequence.
array lis(const array& arr) {
	// m[j]: stores the position k of the smallest value x[k] such that there is an increasing subsequence
	//       of length j ending at x[k] on the range k <= i (note we have j <= k <= i here, because j represents
	//       the length of the increasing subsequence, and k represents the position of its termination. 
	//       Obviously, we can never have an increasing subsequence of length 13 ending at position 11. k <= i by definition).
	// p[k]: stores the position of the predecessor of x[k] in the longest increasing subsequence ending at x[k].
	// the sequence: x[m[1]], x[m[2]], ..., x[m[r]] is nondecreasing.
	table m(arr.size() + 1, 0), p(arr.size() + 1, 1);
	unsigned r = 0;
	for(size_t i = 1; i <= arr.size(); ++i) {
		// j is an index of m such that arr[m[j]] < arr[i](or set j = 0 if no such value exists).
		unsigned j = b_search(m, r, arr, arr[i - 1]);
		// save prev index.
		p[i] = m[j];
		// update maximum length and the tail character.
		if(j == r || arr[m[j + 1]] > arr[i - 1]) {
			m[j + 1] = i;
			r = std::max(r, j + 1);
		}
	}

	array result;
	result.push_back(arr[m[r] - 1]);
	for(unsigned t = m[r--]; r != 0; --r) {
		t = p[t];
		result.push_back(arr[t - 1]);
	}
	
	std::reverse(result.begin(), result.end());

	return result;
}

int main() {
	table t;
	for(size_t i = 0; i < 5; ++i)
		t.push_back(i);

	array str("abfecd");
	std::cout << lis(str) << std::endl;
}

