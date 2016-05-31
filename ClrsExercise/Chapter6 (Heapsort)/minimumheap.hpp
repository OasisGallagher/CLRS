#pragma once
#include <queue>

template <class Ty, class Pr = std::less<Ty> >
class minimum_heap : public std::priority_queue<Ty, std::vector<Ty>, Pr> {
	typedef std::vector<Ty> Cont;
	typedef std::priority_queue<Ty, Cont, Pr> MyBase;
public:
	template <class InputIterator>
	minimum_heap(InputIterator first, InputIterator last) : MyBase(first, last) {
	}

	bool contains(Ty value) const {
		const Cont& container = GetContainer();
		return std::find(container.begin(), container.end(), value) != container.end();
	}

	// rearrange heap from value's position after value's key is modified.
	void rearrange(Ty value) {
		Cont& cont = GetContainer();
		size_t pos = std::distance(cont.begin(), std::find(cont.begin(), cont.end(), value));
		Pr pred;

		for (; pos != 0;) {
			size_t parent = Parent(pos);
			if (pred(cont[parent], cont[pos])) {
				std::swap(cont[parent], cont[pos]);
			}
			pos = parent;
		}

		assert(std::is_heap(cont.begin(), cont.end(), pred));
	}

private:
#if defined(_WIN32)
	const Cont& GetContainer() const {
		return c;
	}
	Cont& GetContainer() {
		return c;
	}
#else
#error invalid platform
#endif
	size_t Parent(size_t from) const {
		return (from - 1) / 2;
	}
};
