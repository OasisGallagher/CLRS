#include <vector>
#include <cassert>
#include <algorithm>

// 装载因子.
static const size_t payload = 2;	// payload > 1. see exercise 18.1-1.
static const int positive_inf = std::numeric_limits<int>::max();
static const int negative_inf = std::numeric_limits<int>::min();

// 对于满的B树节点, 有(2 * payload - 1)个关键字, (2 * payload)个子节点.
struct btree_node {
	// key的数量.
	size_t n;
	// 该节点是否是叶子.
	bool leaf;

	typedef std::vector<int> key_container;
	typedef btree_node* child_ptr;
	typedef std::vector<child_ptr> child_container;
	// 关键字容器. 其个数由n标识, 而不是该容器的size.
	key_container keys;
	// 子节点的容器.
	child_container children;

	// 
	void* operator new(size_t size){
		++node_count;
		return ::operator new(size);
	}

	void operator delete(void* ptr) {
		--node_count;
		::operator delete(ptr);
	}

	static int node_count;
};

int btree_node::node_count = 0;

// 关键字唯一!
class btree {
	typedef btree_node* node_ptr;
	typedef btree_node::child_container child_container;
	typedef btree_node::key_container key_container;
public:
	btree() : root(NULL) { }
	~btree() { if (root != NULL) destroy(root); }
	//
	void insert(int dat);
	//
	bool contains(int dat) const;
	//
	void erase(int dat);
	// 
	int min_key() const;
	//
	int max_key() const;

	int predecessor(int key) const;
private:
	// 分配并初始化一个B树节点.
	node_ptr buynode() const;

	// 从from开始, 移除dat.
	// dat必须在from为根的树内.
	void erase_from(node_ptr from, int dat);

	int predecessor_from(node_ptr from, int key, int prev) const;

	// 查找from为根的B树的最小key值.
	int find_minkey(node_ptr from) const;

	int find_maxkey(node_ptr from) const;

	// 将dat插入到非满节点node中.
	void insert_nonfull(node_ptr node, int dat);
	// node节点是否满.
	bool isfullnode(node_ptr node) const;
	// 创建B树, 并将dat插入其中.
	void create(int dat);
	
	// 销毁以from为根的子树. 如果from == root, 那么销毁整棵B树.
	void destroy(node_ptr from);

	// 以mid_key为中间关键字, 合并y到x中. 二者必须有相同的父节点.
	// 该方法不处理内存释放.
	void merge(node_ptr x, int mid_key, node_ptr y);

	// 从from为根的子树开始, 查找含有dat的节点.
	bool find_from(node_ptr from, int dat) const;

	// 将parent的第childpos个节点N分裂.
	// 要求N节点必须是满的.
	void split_node(node_ptr parent, unsigned childpos);

	// 在node的所有key中, 查找第一个 >= dat的key的位置.
	size_t first_greater_equal(node_ptr node, int dat) const;

	child_container::iterator first_child(node_ptr ptr) {
		return ptr->children.begin();
	}

	child_container::iterator last_child(node_ptr ptr) {
		return ptr->children.begin() + ptr->n + 1;
	}

	key_container::iterator first_key(node_ptr ptr) {
		return ptr->keys.begin();
	}

	key_container::iterator last_key(node_ptr ptr) {
		return ptr->keys.begin() + ptr->n;
	}

	child_container::iterator first_child(node_ptr ptr) const {
		return ptr->children.begin();
	}

	child_container::iterator last_child(node_ptr ptr) const {
		return ptr->children.begin() + ptr->n + 1;
	}

	key_container::iterator first_key(node_ptr ptr) const {
		return ptr->keys.begin();
	}

	key_container::iterator last_key(node_ptr ptr) const {
		return ptr->keys.begin() + ptr->n;
	}
	
	node_ptr root;
};

void btree::insert(int dat) {
	// 单向, 自上而下的为dat寻找插入节点.
	if (root == NULL) {
		create(dat);
		return;
	}
	// 如果根为满节点.
	if (isfullnode(root)) {
		node_ptr oldroot = root;
		// 构造新的根, 并将旧根作为其第0个孩子.
		root = buynode();
		root->children.front() = oldroot;
		// 分裂新根的第0个孩子, 即之前的旧根.
		split_node(root, 0);
	}

	// 此时, root必然是非满的.
	insert_nonfull(root, dat);
}

bool btree::contains(int dat) const {
	return root != NULL && find_from(root, dat);
}

int btree::min_key() const {
	return (root == NULL) ? positive_inf : find_minkey(root);
}

int btree::max_key() const {
	return (root == NULL) ? negative_inf : find_maxkey(root);
}

int btree::predecessor(int key) const {
	return (root == NULL) ? positive_inf : predecessor_from(root, key, negative_inf);
}

void btree::erase(int dat) {
	if(root != NULL && find_from(root, dat))
		erase_from(root, dat);
}

// private.

void btree::erase_from(node_ptr from, int dat) {
	size_t pos;
	for (; ; ) {
		pos = first_greater_equal(from, dat);
		
		if(pos < from->n && from->keys[pos] == dat)
			break;

		node_ptr child = from->children[pos];

		if (child->n >= payload) {
			from = child;
			continue;
		}

		node_ptr next_sibling = from->children[pos + 1];
		node_ptr prev_sibling = (pos > 0) ? from->children[pos - 1] : NULL;

		node_ptr swappable_sibling = NULL;
		
		if (prev_sibling != NULL && prev_sibling->n >= payload)
			swappable_sibling = prev_sibling;
		else if (next_sibling->n >= payload)
			swappable_sibling = next_sibling;

		if (swappable_sibling != NULL) {
			if (swappable_sibling == prev_sibling) {
				std::copy_backward(first_key(child), last_key(child), last_key(child) + 1);
				std::copy_backward(first_child(child), last_child(child), last_child(child) + 1);

				child->keys[0] = from->keys[pos];
				from->keys[pos] = swappable_sibling->keys[swappable_sibling->n - 1];
				child->children[0] = swappable_sibling->children[swappable_sibling->n];
				swappable_sibling->children[swappable_sibling->n] = NULL;
			}
			else {
				child->keys[child->n] = from->keys[pos];
				from->keys[pos] = swappable_sibling->keys[0];
				child->children[child->n + 1] = swappable_sibling->children[0];
				std::copy(first_key(swappable_sibling) + 1, last_key(swappable_sibling), first_key(swappable_sibling));
				//swappable_sibling->keys[swappable_sibling->n - 1] = -1;

				std::copy(first_child(swappable_sibling) + 1, last_child(swappable_sibling), first_child(swappable_sibling));
				swappable_sibling->children[swappable_sibling->n] = NULL;
			}

			--swappable_sibling->n;
			++child->n;
		}
		else {
			merge(child, from->keys[pos], next_sibling);

			std::copy(first_key(from) + pos + 1, last_key(from), first_key(from) + pos);
			//from->keys[from->n - 1] = -1;
			std::copy(first_child(from) + pos + 2, last_child(from), first_child(from) + pos + 1);
			from->children[from->n] = NULL;

			if (--from->n == 0 && from == root) {
				delete root;
				root = child;
			}

			delete next_sibling;
		}

		from = child;
	}

	assert(from == root || from->n >= payload);

	if (from->leaf) {
		key_container::iterator first = from->keys.begin() + pos + 1;
		std::copy(first_key(from) + pos + 1, last_key(from), first_key(from) + pos);
		--from->n;
	}
	else {
		assert(from->children[pos] != NULL && from->children[pos + 1]);
		node_ptr p;
		if ((p = from->children[pos])->n >= payload) {
			dat = from->keys[pos] = p->keys[p->n - 1];
			from = p;
		}
		else if ((p = from->children[pos + 1])->n >= payload) {
			dat = from->keys[pos] = p->keys[0];
			from = p;
		}
		else {
			// merge from->children[pos] and from->children[pos + 1].
			node_ptr child = from->children[pos], next_sibling = from->children[pos + 1];
			merge(child, dat, next_sibling);

			std::copy(first_key(from) + pos + 1, last_key(from), first_key(from) + pos);
			//from->keys[from->n - 1] = -1;

			std::copy(first_child(from) + pos + 2, last_child(from), first_child(from) + pos + 1);
			from->children[from->n] = NULL;

			if (--from->n == 0 && from == root) {
				delete root;
				root = child;
			}

			from = child;
			delete next_sibling;
		}

		erase_from(from, dat);
	}
}

void btree::merge(node_ptr x, int mid_key, node_ptr y) {
	x->keys[x->n] = mid_key;

	std::copy(first_key(y), last_key(y), last_key(x) + 1);
	std::copy(first_child(y), last_child(y), last_child(x));
	x->n += (1 + y->n);
}

int btree::predecessor_from(node_ptr from, int key, int prev) const {
	size_t i = first_greater_equal(from, key);
	if (i < from->n && from->keys[i] == key) {
		if (from->children[i] != NULL)
			return find_maxkey(from->children[i]);
		return i == 0 ? prev : from->keys[i - 1];
	}

	if (from->leaf)
		return negative_inf;

	return predecessor_from(from->children[i], key, i == 0 ? prev : from->keys[i - 1]);
}

int btree::find_minkey(node_ptr from) const {
	//if(from->n == 0)
	//	return positive_inf;

	int cur = from->keys[0];

	return (from->children[0] == NULL)
		? cur : std::min(cur, find_minkey(from->children[0]));
}

int btree::find_maxkey(node_ptr from) const {
	//if(from->n == 0)
	//	return negative_inf;

	int cur = from->keys[from->n - 1];

	return (from->children[from->n] == NULL)
		? cur : std::max(cur, find_maxkey(from->children[from->n]));
}

void btree::insert_nonfull(node_ptr node, int dat) {
	assert(!isfullnode(node));
	// 找到第一个 > dat的key的索引hole.
	size_t hole = first_greater_equal(node, dat);
	// 插入过程, 只将值插入到叶子节点.
	if (node->leaf) {
		// 将hole之后的key后移一位, 为dat留出位置.
		std::copy_backward(first_key(node) + hole, last_key(node), last_key(node) + 1);
		// 不用拷贝子节点, 因为叶子节点的子节点必然为NULL.
		node->keys[hole] = dat;
		++node->n;
	}
	else {
		// 因为hole处的关键字 >= dat, 所以, 第hole个孩子中的
		// 所有关键字 < node->keys[hole].
		node_ptr child = node->children[hole];
		// 如果这个孩子是满的.
		if (isfullnode(child)) {
			// 分裂该孩子.
			split_node(node, hole);
			// 之前的child被分裂为两棵B树, node->keys[hole]为从之前的child
			// 提升上去的关键字的值. 根据此关键字的值, 选择将dat插入到哪棵新分裂的
			// B树中.
			if (dat > node->keys[hole])
				++hole;
		}
		// 插入dat到非满节点node->children[hole]中.
		insert_nonfull(node->children[hole], dat);
	}
}

bool btree::find_from(node_ptr from, int dat) const {
	// 在该节点中查找.
	// from的第i个关键字为第一个 > dat的关键字.
	// 因此, from的第i个child内的所有关键字都小于第i个关键字.
	size_t i = first_greater_equal(from, dat);

	// 存在于该节点中.
	if (i < from->n && dat == from->keys[i])
		return true;

	//
	if (from->leaf)
		return false;

	// 在子节点中查找.
	return find_from(from->children[i], dat);
}

void btree::create(int dat) {
	root = buynode();
	root->leaf = true;
	root->keys.front() = dat;
	++root->n;
}

void btree::destroy(node_ptr from) {
	for (btree_node::child_container::iterator ite = from->children.begin();
		ite != from->children.end(); ++ite) {
		if (*ite != NULL)
			destroy(*ite);
	}
	delete from;
}

void btree::split_node(node_ptr parent, unsigned childpos) {
	// 需要被分裂的子节点.
	node_ptr child = parent->children[childpos];
	assert(isfullnode(child));
	assert(!isfullnode(parent));
	// 分裂出的新节点.
	node_ptr sibling = buynode();

	sibling->leaf = child->leaf;

	// c1 | k1 | c2 | k2 | c3
	// c1 | __ | __ | k1 | c2 | k2 | c3
	// 从第childpos个key, 和第childpos + 1个child开始, 后移一位.
	std::copy_backward(first_key(parent) + childpos, last_key(parent), last_key(parent) + 1);
	std::copy_backward(first_child(parent) + childpos + 1, last_child(parent), last_child(parent) + 1);

	// 连接新节点与父节点.
	parent->children[childpos + 1] = sibling;
	// 拷贝分裂点的key.
	parent->keys[childpos] = child->keys[child->n / 2];
	++parent->n;

	// c1 | k1 | c2 | k2 | c3 | k3 | c4
	// c1 | k1 | c2    &   c3 | k3 | c4
	// 将分裂点之后的key和child都拷贝到新节点中.
	// 新节点恰好有payload-1个key和payload个child.
	std::copy(first_child(child) + child->n / 2 + 1, last_child(child), first_child(sibling));

	// 清空孩子节点(否则在B树析构时会有问题).
	std::fill(first_child(child) + child->n / 2 + 1, last_child(child), (node_ptr)NULL);

	std::copy(first_key(child) + child->n / 2 + 1, last_key(child), first_key(sibling));

	// 清空之前的数据.
	//std::fill(child->keys.begin() + child->n / 2, child->keys.begin() + child->n, -1);

	sibling->n = (child->n /= 2);
}

size_t btree::first_greater_equal(node_ptr node, int dat) const {
	// 关键字按照递增的顺序排列(关键字唯一).
	return std::distance(node->keys.begin(), std::lower_bound(
		first_key(node), last_key(node), dat));
}

btree::node_ptr btree::buynode() const {
	node_ptr ptr = new btree_node;
	ptr->n = 0, ptr->leaf = false;
	ptr->keys.resize(2 * payload - 1, int());
	ptr->children.resize(2 * payload);
	return ptr;
}

bool btree::isfullnode(node_ptr node) const {
	return node != NULL && node->n == 2 * payload - 1;
}

#include <iostream>
#include <ctime>

void chk_node() {
	assert(btree_node::node_count == 0);
	std::cout << "**************** All done ****************\n";
}

class performance {
	clock_t start;
	clock_t init;
	void seperator() {
		printf("__________________________________________\n");
	}
public:
	performance() : start(clock()), init(start) { }
	~performance() {
		printf("totally: %.2f second(s).\n", (clock() - init) / float(CLOCKS_PER_SEC));
	}
	void tell(const char* title) {
		clock_t now = clock();
		printf("%s: %.2f second(s).\n", title, (now - start) / float(CLOCKS_PER_SEC));
		start = now;
		seperator();
	}
};

int main() {
	atexit(chk_node);
	performance c;
	int test_count = 100000;
	btree tree;
	c.tell("initialize");

	for (int i = 0; i < test_count; ++i)
		tree.insert(i);

	c.tell("insert 100000 items");

	for (int i = 0; i < test_count; ++i)
		assert(tree.contains(i));

	c.tell("query 100000 items");
	
	for (int i = 0; i < test_count; ++i)
		tree.erase(i);

	c.tell("erase 100000 items");
	return 0;
}

