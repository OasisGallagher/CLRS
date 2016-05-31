// clrs 11.2-4
#include <iostream>
#include <functional>
static const size_t HashTableDefaultCap = 701;

// <TODO>
// �ظ���key, ��Ȼ���Բ��뵽hashtable. �����insertʱ���, ��ô�����Ч��
// �޷��ﵽO(1).
//
// free_list���ܶ�̬����.
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

	// hash�������õ�mod, ���, cap��Ҫ��һ�����ӽ�2���ݵ�����.
	free_list(size_t cap = HashTableDefaultCap) {
		if(cap == 0)
			throw std::invalid_argument("invalid argument");
		__init(cap);
	}
	// ����pos����ֵ.
	// �����ǰpos�ڵ�Ϊfree, ��ô����Ϊbusy.
	void assign(size_t pos, const value_type& val) {
		__assign(__checkpos(pos), val);
	}

	// ��cur�ĺ��湹��ڵ�, ����valֵ.
	void back_insert(free_list_node* cur, const value_type& val) {
		free_list_node* hole = allocate();
		hole->val = val;
		__back_insert(cur, hole);
	}

	// ��first��ͷ������������, ����pred��value. ������ָ�����NULL.
	template <class Pred>
	const value_type* search_list(const free_list_node* first, const Pred& pred) const {
		return (first != NULL && !first->free) ? __search_list(first, pred) : NULL;
	}
	// ��first��ͷ���������Ƴ�����pred��Ԫ��.
	template <class Pred>
	void erase(free_list_node* first, const Pred& pred) {
		if(first != NULL && !first->free)
			__erase(first, pred);
	}

	// �ͷ�node�ĵ�ǰλ��, ����freelist���ٴη���һ��λ��, ����ֵ����.
	// ��ɺ�, node��һ������λ��.
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
	// node�ڵ��Ƿ����.
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
	// node��freeλ�õ�ֵͳһ����������(��ʼ��ʱ����), �Ա�ͳ��
	// ������Ŀ.
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

	// ȡposλ�õ�Ԫ��.
	free_list_node* __node_at(size_type pos) {
		return memory + pos;
	}
	const free_list_node* __node_at(size_type pos) const {
		return memory + pos;
	}
	// ��node���뵽cur�ĺ���.
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
		// ������ǿ���λ��, ��ôֱ���޸����λ��. 
		if(node->free)
			__set_busy(node);
		node->val = val;
	}

	// ��node�Ƴ�freelist.
	void __set_busy(free_list_node* node) {
		__set_state(node, false);
	}

	// ��node��Ϊfree, ����ָ��.
	void __set_free(free_list_node* node) {
		__set_state(node, true);
	}

	// ��node��Ϊfree/busy״̬.
	void __set_state(free_list_node* node, bool free) {
		if(node != NULL && node->free != free) {
			// freelist�Ŀ�ͷ.
			if(node->prev == NULL) {
				if(node == free_head)
					__set_free_head(free_head->next);
				else {
					node = node->next;
					node->prev = NULL;
				}
			}
			// freelist�Ľ�β.
			else if(node->next == NULL)
				node->prev->next = NULL;
			// ��freelist�м�, ��ô�Ƴ�node������.
			else {
				node->prev->next = node->next;
				node->next->prev = node->prev;
			}
			__clear_link(node);
			__set_node_free(node, free);
		}
	}

	// ��node��prev��next����ΪNULL.
	void __clear_link(free_list_node* node) {
		if(node != NULL)
			node->prev = node->next = NULL;
	}
	// ��ʼ��freelist, ����cap��Ԫ��.
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
		// ���ܴ�ʱû�п���λ��, xΪ��μ���Ŀ���λ��.
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

// hash����.
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

	// ����hash���еļ�ֵ��.
	struct hash_element {
		hash_element(const key_type& k, const value_type& v) : key(k), val(v) { }
		hash_element() { }
		key_type key;
		value_type val;
	};
	// hash��ʹ�õ�����.
	typedef free_list<hash_element> container_type;
	// �����д�ŵ�Ԫ��.
	typedef typename container_type::free_list_node storage_type;
	// ����.
	container_type cont;

	Hash hash;
	// ��freelist��pos��, ����key��value.
	void __insert_free(size_type pos, const key_type& key, const value_type& val) {
		cont.assign(pos, hash_element(key, val));
	}
	// ��cur�ĺ������һ��Ԫ��.
	void __back_insert(storage_type* cur, const key_type& key, const value_type& val) {
		cont.back_insert(cur, hash_element(key, val));
	}
	// ��node�ͷ�, �����·��䵽����һ����ͬ��λ����.
	void __relocate(storage_type* node) {
		cont.relocate(node);
	}

	size_type max_size() const {
		return cont.max_size();
	}

	void __insert_occupied(size_type hash_value, const key_type& key, const value_type& val) {
		storage_type* node = cont.node_at(hash_value);
		// �䵽һ������ʹ�õĲ���, ����, ���������Ҫ�����Ԫ��hashֵһ���������ڵ�һ��Ԫ��,
		// ��ô, ��Ҫ�����Ԫ��Ҳ�����������. �������������.
		if(hash_value == hash(cont.at(hash_value).key, max_size())) {
			__back_insert(node, key, val);
		}
		// �䵽һ������ʹ�õĲ�, ���Ҹò۵�hashֵ, ����Ҫ�����key��hashֵ��һ��.
		// ����, ���Զ϶�, �����һ������һ���������.
		// ��Ϊ��������hashֵ, ���λ�Ƕ�Ӧ��(����������hashֵ���ܲ���Ӧ,
		// ֻ��������ʱǡ������ĳ��λ��), �����������if.
		// ���潫������Ͽ�, ֮���Ԫ��������freelist����, ���ٴ�����.
		// ����Ҫ�����ֵ, ���뵽���λ��.
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
		// λ���ǿ��е�, ��ôֱ�Ӳ��뵽���λ��.
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
