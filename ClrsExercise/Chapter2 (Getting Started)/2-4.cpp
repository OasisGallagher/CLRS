#pragma warning(disable:4996)
#include <iostream>
#include <algorithm>

unsigned merge(int* io_buffer, int start, int mid, int last) {
	// both buffer[start, mid) and buffer[mid, last) must be sorted.
	int* tmp = new int[last - start];
	std::copy(io_buffer + start, io_buffer + last, tmp);

	int write_ptr = start;

	mid -= start, last -= start, start = 0;
	int start2 = mid;

	unsigned inversion_count = 0;

	for(; start != mid && start2 != last; ) {
		if(tmp[start] < tmp[start2]) {
			io_buffer[write_ptr++] = tmp[start++];

			// tmp[start] must be greater than tmp[start - 1].
			if(start != mid)
				inversion_count *= 2;
		}
		else {	// inversion.
			io_buffer[write_ptr++] = tmp[start2++];
			++inversion_count;
		}
	}
	
	// tails in left range.
	if(mid - start - 1 > 0)
		inversion_count *= (mid - start);

	std::copy(tmp + start, tmp + mid, io_buffer + write_ptr);

	write_ptr += mid - start;
	std::copy(tmp + start2, tmp + last, io_buffer + write_ptr);

	delete tmp;

	return inversion_count;
}

unsigned merge_sort(int* arr, int first, int last) {
	unsigned inversion_count = 0;
	// more the 1 element. worth dividing.
	if(first + 1 < last) {
		int mid = (first + last) / 2;
		inversion_count += merge_sort(arr, first, mid);
		inversion_count += merge_sort(arr, mid, last);
		inversion_count += merge(arr, first, mid, last);
	}
	return inversion_count;
}

int main() {
	int arr[] = { 2, 3, 8, 6, 1};
	std::cout << "inversion count = " 
		<< merge_sort(arr, 0, _countof(arr)) << std::endl;

	return 0;
}
