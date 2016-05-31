// clrs 11.2-4
#include <iostream>
#include <functional>
static const size_t HashTableDefaultCap = 701;

// <TODO>
// 重复的key, 依然可以插入到hashtable. 如果在insert时检测, 那么插入的效率
// 无法达到O(1).
//
// free_list不能动态增长.
// </TODO>

template <class Ty>
class free_list {
public:
	typedef Ty value_type;
	typedef size_t size_type;

	class free_list_node {
		friend class free_list;
		free_list_node* prev;
		free_list_node* next;
		bool free;
		value_type val;
	public:
		free_list_node() : prev(NULL), next(NULL), free(true) {}
	};

	// hash函数采用的mod, 因此, cap需要是一个不接近2的幂的质数.
	free_list(size_t cap = HashTableDefaultCap) {
		if(cap == 0)
			throw std::invalid_argument("invalid argument");
		__init(cap);
	}
	// 设置pos处的值.
	// 如果当前pos节点为free, 那么会置为busy.
	void assign(size_t pos, const value_type& val) {
		__assign(__checkpos(pos), val);
	}

	// 在cur的后面构造节点, 插入val值.
	void back_insert(free_list_node* cur, const value_type& val) {
		free_list_node* hole = allocate();
		hole->val = val;
		__back_insert(cur, hole);
	}

	// 在first开头的链表中搜索, 满足pred的value. 返回其指针或者NULL.
	template <class Pred>
	const value_type* search_list(const free_list_node* first, const Pred& pred) const {
		return (first != NULL && !first->free) ? __search_list(first, pred) : NULL;
	}
	// 在first开头的链表中移除满足pred的元素.
	template <class Pred>
	void erase(free_list_node* first, const Pred& pred) {
		if(first != NULL && !first->free)
			__erase(first, pred);
	}

	// 释放node的当前位置, 并在freelist中再次分配一个位置, 将该值插入.
	// 完成后, node是一个空闲位置.
	void relocate(free_list_node* node) {
		if(!node->free) {
			free_list_node* newnode = allocate();
			newnode->val = node->val;
			__back_insert(node->prev, newnode);

			__set_free(node);
			__dealloc(node);
		}
	}

	size_type size() const {
		return max_size() - free_count;
	}

	~free_list() {
		delete[] memory;
	}

	free_list_node* allocate() {
		if(free_head == NULL)
			throw std::bad_alloc("no enough memory");

		return __alloc();
	}
	// node节点是否空闲.
	bool isfree(size_type pos) {
		__checkpos(pos);
		return __node_at(pos)->free;
	}

	value_type& at(size_type pos) {
		__checkpos(pos);
		return __node_at(pos)->val;
	}

	free_list_node* node_at(size_type pos) {
		__checkpos(pos);
		return __node_at(pos);
	}

	const free_list_node* node_at(size_type pos) const {
		__checkpos(pos);
		return __node_at(pos);
	}

	void deallocate(free_list_node* x) {
		if(x == NULL)
			throw std::invalid_argument("invalid argument");
		__dealloc(x);
	}

	size_type max_size() const {
		return mymaxsize;
	}
private:
	size_type __checkpos(size_type pos) const {
		if(pos >= max_size())
			throw std::out_of_range("index out of range");
		return pos;
	}
	// node的free位置的值统一在这里设置(初始化时除外), 以便统计
	// 空闲数目.
	void __set_node_free(free_list_node* node, bool free) {
		if(node != NULL && node->free != free) {
			node->free = free;
			free_count += free ? 1 : -1;
		}
	} 
	template <class Pred>
	const value_type* __search_list(const free_list_node* first, Pred pred) const {
		for(; first != NULL && !pred(first->val); first = first->next)
			;
		return first != NULL ? &first->val : NULL;
	}
	template <class Pred>
	void __erase(free_list_node* first, Pred pred) {
		for(; first != NULL && !pred(first->val); first = first->next)
			;
		if(first != NULL)
			__dealloc(first);
	}

	// 取pos位置的元素.
	free_list_node* __node_at(size_type pos) {
		return memory + pos;
	}
	const free_list_node* __node_at(size_type pos) const {
		return memory + pos;
	}
	// 将node插入到cur的后面.
	void __back_insert(free_list_node* cur, free_list_node* node) {
		if(node != NULL) {
			node->prev = cur;
			node->next = (cur != NULL) ? cur->next : NULL;
		}

		if(cur != NULL) {
			if(cur->next != NULL)
				cur->next->prev = node;
			cur->next = node;
		}
	}
	void __assign(size_type pos, const value_type& val) {
		free_list_node* node = __node_at(pos);
		// 如果不是空闲位置, 那么直接修改这个位置. 
		if(node->free)
			__set_busy(node);
		node->val = val;
	}

	// 将node移出freelist.
	void __set_busy(free_list_node* node) {
		__set_state(node, false);
	}

	// 将node置为free, 处理指针.
	void __set_free(free_list_node* node) {
		__set_state(node, true);
	}

	// 将node置为free/busy状态.
	void __set_state(free_list_node* node, bool free) {
		if(node != NULL && node->free != free) {
			// freelist的开头.
			if(node->prev == NULL) {
				if(node == free_head)
					__set_free_head(free_head->next);
				else {
					node = node->next;
					node->prev = NULL;
				}
			}
			// freelist的结尾.
			else if(node->next == NULL)
				node->prev->next = NULL;
			// 在freelist中间, 那么移除node的链接.
			else {
				node->prev->next = node->next;
				node->next->prev = node->prev;
			}
			__clear_link(node);
			__set_node_free(node, free);
		}
	}

	// 将node的prev和next域置为NULL.
	void __clear_link(free_list_node* node) {
		if(node != NULL)
			node->prev = node->next = NULL;
	}
	// 初始化freelist, 分配cap个元素.
	void __init(size_type cap) {
		memory = new free_list_node[cap];
		for(size_type i = 1; i < cap; ++i) {
			memory[i].prev = memory + i - 1;
			memory[i - 1].next = memory + i;
		}

		mymaxsize = free_count = cap;
		free_head = memory;
	}

	void __set_free_head(free_list_node* head) {
		if((free_head = head) != NULL)
			free_head->prev = NULL;
	}

	free_list_node* __alloc() {
		free_list_node* result = free_head;
		__set_free_head(free_head->next);

		__clear_link(result);
		__set_node_free(result, false);
		return result;
	}
	void __dealloc(free_list_node* x) {
		x->next = free_head;
		// 可能此时没有空闲位置, x为这次加入的空闲位置.
		if(free_head != NULL)
			free_head->prev = x;
		__set_node_free(x, true);

		__set_free_head(x);
	}

	free_list_node* memory;
	free_list_node* free_head;
	size_type free_count;
	size_type mymaxsize;
};

// hash函数.
template <class Key>
struct modhash : public std::binary_function<Key, size_t, size_t> {
	int operator () (const Key& key, size_t container_max_size) const {
		return key % container_max_size;
	}
};

template <class Key, class Value, class Hash = modhash<Key> >
class hash_table {
	typedef Key key_type;
	typedef Value value_type;
	typedef size_t size_type;

	// 存在hash表中的键值对.
	struct hash_element {
		hash_element(const key_type& k, const value_type& v) : key(k), val(v) { }
		hash_element() { }
		key_type key;
		value_type val;
	};
	// hash表使用的容器.
	typedef free_list<hash_element> container_type;
	// 容器中存放的元素.
	typedef typename container_type::free_list_node storage_type;
	// 容器.
	container_type cont;

	Hash hash;
	// 在freelist的pos处, 插入key和value.
	void __insert_free(size_type pos, const key_type& key, const value_type& val) {
		cont.assign(pos, hash_element(key, val));
	}
	// 在cur的后面插入一个元素.
	void __back_insert(storage_type* cur, const key_type& key, const value_type& val) {
		cont.back_insert(cur, hash_element(key, val));
	}
	// 将node释放, 并重新分配到另外一个不同的位置上.
	void __relocate(storage_type* node) {
		cont.relocate(node);
	}

	size_type max_size() const {
		return cont.max_size();
	}

	void __insert_occupied(size_type hash_value, const key_type& key, const value_type& val) {
		storage_type* node = cont.node_at(hash_value);
		// 落到一个正在使用的槽内, 而且, 这个是与需要插入的元素hash值一样的链表内的一个元素,
		// 那么, 需要插入的元素也属于这个链表. 插入这个链表即可.
		if(hash_value == hash(cont.at(hash_value).key, max_size())) {
			__back_insert(node, key, val);
		}
		// 落到一个正在使用的槽, 而且该槽的hash值, 与需要插入的key的hash值不一样.
		// 首先, 可以断定, 这个槽一定不是一个链的起点.
		// 因为链的起点的hash值, 与槽位是对应的(而非链起点的hash值可能不对应,
		// 只是在链接时恰巧落在某个位置), 会进入上述的if.
		// 下面将这个链断开, 之后的元素重新在freelist分配, 并再次连接.
		// 而需要插入的值, 插入到这个位置.
		else {
			__relocate(node);
			__insert_free(hash_value, key, val);
		}
	}
public:
	hash_table() { }
	~hash_table() { }
	void insert(const key_type& key, const value_type& val) {
		size_type hash_value = hash(key, max_size());
		// 位置是空闲的, 那么直接插入到这个位置.
		if(cont.isfree(hash_value))
			__insert_free(hash_value, key, val);
		else
			__insert_occupied(hash_value, key, val);
	}

	size_type size() const {
		return cont.size();
	}

	struct equalkey : public std::binary_function<hash_element, key_type, bool> {
		bool operator() (const hash_element& element, const key_type& key) const {
			return element.key == key;
		}
	};
	bool find(const key_type& key, value_type* val =  NULL) const {
		size_type hash_value = hash(key, max_size());
		const hash_element* element = cont.search_list(cont.node_at(hash_value), std::bind2nd(equalkey(), key));
		if(element == NULL)
			return false;
		if(val != NULL)
			*val = element->val;
		return true;
	}

	void erase(const key_type& key) {
		size_type hash_value = hash(key, max_size());
		cont.erase(cont.node_at(hash_value), std::bind2nd(equalkey(), key));
	}
};

int main() {
	try {
		hash_table<int, int> table;
		for(int i = 0; i < HashTableDefaultCap; ++i) {
			table.insert(i + 100, (i + 1) * 2);
		}
		
		// bad_alloc.
		/*table.insert(0, 0);*/

		std::cout << table.size() << std::endl;

		for(int i = 0; i < HashTableDefaultCap; ++i)
			if(!table.find(i + 100))
				throw std::runtime_error("not found");

		if(table.find(80))
			throw std::runtime_error("should not exist");

		for(int i = 0; i < HashTableDefaultCap; ++i)
			table.erase(i + 100);

		std::cout << table.size() << std::endl;

		for(int i = 0; i < HashTableDefaultCap; ++i)
			if(table.find(i + 100))
				throw std::runtime_error("not exist");

		std::cout << "done.\n";
	}
	catch(const std::exception& exp) {
		std::cout << "exception: " << exp.what() << std::endl;
	}
}
