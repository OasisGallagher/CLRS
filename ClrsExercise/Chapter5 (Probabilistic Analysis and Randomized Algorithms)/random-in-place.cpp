// random number belongs [min, max).
size_t bounded_random(size_t min, size_t max) {
	return min + rand() % (max - min);
}

void random_shuffle(int* first, int* last) {
	size_t length = last - first;
	for(size_t i = 0; i < length; ++i) {
		int* tmp = first + bounded_random(i, length);
		std::iter_swap(first + i, tmp);
	}
}

