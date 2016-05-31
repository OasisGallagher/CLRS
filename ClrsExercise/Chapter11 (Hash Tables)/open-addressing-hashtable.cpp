#include <iostream>
#include <vector>
#include <algorithm>

// open addressing hash table.
// 
#define OPEN_ADDRESSING_HASH_POW2(X)		(1 << (X))

template <class Key, class Value>
class open_addressing_hash {
	typedef std::pair<Key, Value> node;
	typedef std::vector<node*> container;
public:
	open_addressing_hash() 
		: capacity(OPEN_ADDRESSING_HASH_POW2(10)), cont(capacity) {}
	~open_addressing_hash() {
		std::for_each(cont.begin(), cont.end(), killer);
	}
	void insert(const Key& key, const Value& val) {
		for(size_t i = 0, first = 0; i < capacity; ++i) {
			first = __nextpos(i) % capacity;
			if(cont[first] == NULL) { // empty.
				node* n = new node;
				n->first = key;
				n->second = val;
				cont[first] = n;
				return;
			}
		}

		throw std::overflow_error("hash map overflow");
	}
	bool find(const Key& key, Value* val = NULL) {
		for(size_t i = 0, first = 0; i < capacity; ++i) {
			first = __nextpos(i) % capacity;
			if(cont[first] != NULL && cont[first]->first == key) {
				if(val != NULL)
					*val = cont[first]->second;
				return true;
			}
		}

		return false;
	}
private:
	size_t __nextpos(size_t step) const {
		return (step * (step + 1)) / 2;
	}

	static void killer(node* ptr) {
		delete ptr;
	}

	size_t capacity;
	container cont;
};

int main() {
	open_addressing_hash<int, int> hash;
	for(int i = 0; i < OPEN_ADDRESSING_HASH_POW2(10); ++i) {
		hash.insert(i, i * i);
	}

	std::cout << std::boolalpha << hash.find(10) << std::endl;
	std::cout << std::boolalpha << hash.find(10012) << std::endl;
}
