#include <cstdlib>
#include <ctime>
#include <iostream>
#include <utility>

static void RandomShuffle(int* first, int* last) {
	srand((unsigned)time(NULL));
	int len = last - first;
	if(len > 1) {
		for(int i = 0; i < len; ++i) {
			int offset = i + rand() % (len - i);
			std::swap(first[i], first[offset]);
		}
	}
}

int main() {
	int range[] = { 1, 2, 3, 4, 5 };
	RandomShuffle(range, range + _countof(range));
}