#pragma once
// point of max overlap.
#include <stdexcept>
#include <vector>
#include <algorithm>
#include <functional>
#include <iostream>

static const bool red = true, black = false;
// error messages.
static const char* err_access_nil = "err: unable to access nil";
static const char* err_invalid_rb_tree = "err: invalid rb tree";
static const char* err_bad_alloc = "err: bad alloc";
static const char* err_key_doesnot_exist = "err: key doesn't exist";

// #define _DEBUG_NODE_MEM

class pom {
public:
	typedef std::pair<int, int> interval;
	typedef std::vector<interval> interval_container;
	pom()
		: root_(null()), size_(0) {
			Nil.augmenting.max_index = null();
			Nil.augmenting.maximum = 0;
			Nil.augmenting.number = 0;
			Nil.augmenting.sum = 0;
	}
	~pom() {
		// TODO:
		// destroy all nodes with inorder tree walk described in binarysearchtree.hpp.
	}

	void calc_pom(const interval_container& intervals);

	int getpom() const;

	int pom_count() const;

	void insert(int val, bool lp = true);
	bool contains(int val) const;
	bool erase(int val);

#ifdef _DEBUG_NODE_MEM
	static int rb_tree_node_count() { return rb_tree_node::c_rb_tree_node; }
#endif // _DEBUG_NODE_MEM
private:
	struct rb_tree_node;
	struct pom_augmenting {
		void initialize();
		int number;
		int sum;
		int maximum;
		rb_tree_node* max_index;
	};

	struct rb_tree_node {
		rb_tree_node* parent;
		rb_tree_node* l_child;
		rb_tree_node* r_child;
		int val;
		bool color;

		pom_augmenting augmenting;
		// 
#ifdef _DEBUG_NODE_MEM
		void* operator new(size_t size) {
			++c_rb_tree_node;
			return ::operator new(size);
		}
		void operator delete(void* ptr) {
			--c_rb_tree_node;
			::operator delete(ptr);
		}
		static int c_rb_tree_node;
#endif // _DEBUG_NODE_MEM
	};

	rb_tree_node* __find_node(int val) const;

	void __left_rotate(rb_tree_node* pivot);
	void __right_rotate(rb_tree_node* pivot);
	bool __isnil(const rb_tree_node* node) const;
	void __raise_rb_tree_err(const char* msg) const;

	void __bst_insert(rb_tree_node* node);
	void __on_bst_inserted(rb_tree_node* node);

	bool __update_max_sum(rb_tree_node* node);

	void __rb_insert_fixup(rb_tree_node* node);

	typedef std::pair<rb_tree_node*, rb_tree_node*> rb_erase_result;
	rb_erase_result __bst_erase(rb_tree_node* node);
	void __rb_erase_fixup(rb_tree_node* node);

	bool __is_left_child(const rb_tree_node* node) const;
	bool __is_right_child(const rb_tree_node* node) const;

	rb_tree_node* __successor(rb_tree_node* node);

	rb_tree_node* __min_node(rb_tree_node* from);

	const rb_tree_node* __grandparent(rb_tree_node* node) const;
	rb_tree_node* __grandparent(rb_tree_node* node);

	rb_tree_node* __buy_node(int val);

	static rb_tree_node* null() { return &Nil; }

	struct point {
		int cord;
		bool lp;
	};

	typedef std::vector<point> point_container;
	void __split_endpoint(interval in, point_container& cont);
	static bool __sort_endpoint(const point& left, const point& right);

	size_t size_;
	rb_tree_node* root_;
	static rb_tree_node Nil;
};

void pom::calc_pom(const interval_container& intervals) {
	point_container endpoints;
	for(interval_container::const_iterator ite = intervals.begin(); ite != intervals.end(); ++ite)
		__split_endpoint(*ite, endpoints);

	std::sort(endpoints.begin(), endpoints.end(), __sort_endpoint);

	for(point_container::iterator ite = endpoints.begin(); ite != endpoints.end(); ++ite)
		insert(ite->cord, ite->lp);
}

int pom::getpom() const {
	if(__isnil(root_) || __isnil(root_->augmenting.max_index))
		__raise_rb_tree_err(err_access_nil);

	return root_->augmenting.max_index->val; 
}

int pom::pom_count() const {
	if(__isnil(root_) || __isnil(root_->augmenting.max_index))
		__raise_rb_tree_err(err_access_nil);

	return root_->augmenting.maximum;
}

void pom::insert(int val, bool lp) {
	rb_tree_node* node = __buy_node(val);
	node->augmenting.maximum = node->augmenting.number = lp ? 1 : -1;
	__bst_insert(node);
	node->color = red;
	__rb_insert_fixup(node);
	++size_;
}

void pom::__left_rotate(rb_tree_node* pivot) {
	if(__isnil(pivot) || __isnil(pivot->r_child))
		__raise_rb_tree_err(err_access_nil);

	rb_tree_node* right_child = pivot->r_child;
	pivot->r_child = right_child->l_child;

	if(!__is_nil(right_child->l_child))
		right_child->l_child->parent = pivot;

	right_child->parent = pivot->parent;

	if(__isnil(pivot->parent))
		root_ = right_child;
	else if(__is_left_child(pivot))
		pivot->parent->l_child = right_child;
	else
		pivot->parent->r_child = right_child;

	right_child->l_child = pivot;
	pivot->parent = right_child;

	right_child->augmenting.sum = pivot->augmenting.sum;
	right_child->augmenting.maximum = pivot->augmenting.maximum;
	right_child->augmenting.max_index = pivot->augmenting.max_index;
	pivot->augmenting.sum = pivot->l_child->augmenting.sum + pivot->r_child->augmenting.sum + pivot->augmenting.number;
	__update_max_sum(pivot);
}

void pom::__right_rotate(rb_tree_node* pivot) { 
	if(__isnil(pivot) || __isnil(pivot->l_child))
		__raise_rb_tree_err(err_access_nil);

	rb_tree_node* left_child = pivot->l_child;
	pivot->l_child = left_child->r_child;

	if(!__is_nil(left_child->r_child))
		left_child->r_child->parent = pivot;

	left_child->parent = pivot->parent;

	if(__isnil(pivot->parent))
		root_ = left_child;
	else if(__is_left_child(pivot))
		pivot->parent->l_child = left_child;
	else
		pivot->parent->r_child = left_child;

	left_child->r_child = pivot;
	pivot->parent = left_child;

	// DO NOT COPY number field.
	left_child->augmenting.sum = pivot->augmenting.sum;
	left_child->augmenting.maximum = pivot->augmenting.maximum;
	left_child->augmenting.max_index = pivot->augmenting.max_index;

	pivot->augmenting.sum = pivot->l_child->augmenting.sum + pivot->r_child->augmenting.sum + pivot->augmenting.number;
	__update_max_sum(pivot);
}

void pom::__on_bst_inserted(rb_tree_node* node) {
	node->augmenting.max_index = node;

	// O(log(n)).
	for(rb_tree_node* n2 = node; !__isnil(n2); n2 = n2->parent)
		n2->augmenting.sum += node->augmenting.number;

	// O(log(n)).
	for(node = node->parent; !__isnil(node) && __update_max_sum(node); node = node->parent)
		;
}

bool pom::__update_max_sum(rb_tree_node* p) {
	int ml = p->l_child->augmenting.sum;
	int m = ml + p->augmenting.number;
	int mr = m + p->r_child->augmenting.sum;

	rb_tree_node* pmax = p->augmenting.max_index;
	int maximum = m;
	// try left half.
	if(ml > maximum && !__isnil(p->l_child)) {
		maximum = ml;
		pmax = p->l_child->augmenting.max_index;
	}
	// try right half.
	if(mr > maximum && !__isnil(p->r_child)) {
		maximum = mr;
		pmax = p->r_child->augmenting.max_index;
	}

	// nothing changed.
	if(pmax == p->augmenting.max_index)
		return false;

	p->augmenting.maximum = maximum;
	p->augmenting.max_index = pmax;

	return true;
}

void pom::__bst_insert(rb_tree_node* node) {
	rb_tree_node* last_valid = null();
	for(rb_tree_node* iter = root_; !__isnil(iter); ) {
		last_valid = iter;
		iter = (node->val < iter->val) ? iter->l_child : iter->r_child;
	}

	node->parent = last_valid;

	if(__isnil(last_valid)) // empty tree.
		root_ = node;
	else if(node->val < last_valid->val)
		last_valid->l_child = node;
	else
		last_valid->r_child = node;

	__on_bst_inserted(node);
}

void pom::__rb_insert_fixup(rb_tree_node* node) {
	// red parent.
	for(; node->parent->color == red; ) {
		// node's parent is left child.
		if(__is_left_child(node->parent)) {
			// node->parent->color == red ==> node->parent != root_ ==> 
			// node->parent->parent != nil.
			// red uncle.
			rb_tree_node* uncle = __grandparent(node)->r_child;
			if(uncle->color == red) {
				// BH of grandparent +1.
				uncle->color = node->parent->color = black;
				__grandparent(node)->color = red;
				node = __grandparent(node);
			}
			else {
				// make sure node to be left child.
				if(__is_right_child(node)) {
					node = node->parent;
					__left_rotate(node);
				}
				// fall through.
				// black height of left subtree rooted by node->parent->parent.
				node->parent->color = black;
				__grandparent(node)->color = red;
				// right rotate to adjust black height.
				__right_rotate(__grandparent(node));

				// exit loop here. cos' node's parent is black(
				//     node->parent->color = black;
				// ).
			}
		}
		// node's parent is right child.
		else {
			rb_tree_node* uncle = __grandparent(node)->l_child;
			if(uncle->color == red) {
				uncle->color = node->parent->color = black;
				__grandparent(node)->color = red;
				node = __grandparent(node);
			}
			else {
				// make sure node to be right child.
				if(__is_left_child(node)) {
					node = node->parent;
					__right_rotate(node);
				}
				node->parent->color = black;
				__grandparent(node)->color = red;
				__left_rotate(__grandparent(node));
			}
		}
	}

	root_->color = black;
}

bool pom::contains(int val) const {
	return !__isnil(__find_node(val));
}

bool pom::erase(int val) {
	rb_tree_node* pos = __find_node(val);
	if(__isnil(pos))
		return false;
	rb_erase_result result = __bst_erase(pos);
	// tree above garbage is legal.
	if(result.first->color == black)
		__rb_erase_fixup(result.second);

	delete result.first;
	--size_;

	return true;
}

pom::rb_erase_result pom::__bst_erase(rb_tree_node* node) {
	rb_tree_node* garbage = node;
	// two children.
	if(!__isnil(node->l_child) && !__isnil(node->r_child))
		garbage = __successor(node);
	// garbage has only one child at most.
	rb_tree_node* child = null();
	if(!__isnil(garbage->l_child))
		child = garbage->l_child;
	else
		child = garbage->r_child;

	// set parent even if child is nil to ensure __rb_erase_fixup
	// can reach child's parent(garbage's parent previously).
	child->parent = garbage->parent;

	if(__isnil(garbage->parent))
		root_ = child;
	else { 
		if(__is_left_child(garbage))
			garbage->parent->l_child = child;
		else
			garbage->parent->r_child = child;		
	}
	if(garbage != node)
		node->val = garbage->val;

	return std::make_pair(garbage, child);
}

void pom::__rb_erase_fixup(rb_tree_node* node) {
	for(; node != root_ && node->color == black; ) {
		// node is left child.
		if(__is_left_child(node)) {
			// sibling != null, due to the black-height rule.
			rb_tree_node* sibling = node->parent->r_child;
			if(sibling->color == red) {
				// node->parent->color = black.
				// exchange sibling's color with node's parent's color.
				sibling->color = black;
				node->parent->color = red;
				__left_rotate(node->parent);
				sibling = node->parent->r_child;
			}
			// fall through.
			// black sibling with two black children.
			if(sibling->l_child->color == black && sibling->r_child->color == black) {
				// decrease black height.
				sibling->color = red;
				// subtree rooted by node is legal.
				node = node->parent;
			}
			// black sibling with one black child at most.
			else {
				// left child is red.
				if(sibling->r_child->color == black) {
					sibling->color = red;
					sibling->l_child->color = black;
					__right_rotate(sibling);
					sibling = node->parent->r_child;
				}
				// fall through.
				// right child is red.
				sibling->color = sibling->parent->color;
				sibling->parent->color = black;
				sibling->r_child->color = black;
				__left_rotate(node->parent);
				// exit from for loop.
				node = root_;
			}
		}
		// node is right child.
		else {
			rb_tree_node* sibling = node->parent->l_child;
			if(sibling->color == red) {
				sibling->color = black;
				node->parent->color = red;
				__right_rotate(node->parent);
				sibling = node->parent->l_child;
			}

			if(sibling->l_child->color == black && sibling->r_child->color == black) {
				sibling->color = red;
				node = node->parent;
			}
			else {
				if(sibling->l_child->color == black) {
					sibling->color = red;
					sibling->r_child->color = black;
					__left_rotate(sibling);
					sibling = node->parent->l_child;
				}
				sibling->color = sibling->parent->color;
				sibling->parent->color = black;
				sibling->l_child->color = black;
				__right_rotate(node->parent);
				node = root_;
			}
		}
	}
	node->color = black;
}

pom::rb_tree_node* pom::__find_node(int val) const {
	rb_tree_node* iter = root_;
	for(; !__isnil(iter) && iter->val != val; ) 
		iter = (val < iter->val) ? iter->l_child : iter->r_child;

	return iter;
}

pom::rb_tree_node* pom::__buy_node(int val) {
	try {
		rb_tree_node* node = new rb_tree_node;
		node->color = black;
		node->parent = node->r_child = node->l_child = null();
		node->val = val;

		node->augmenting.initialize();

		return node;
	}
	catch(...) {
		__raise_rb_tree_err(err_bad_alloc);
		return null();
	}
}

const pom::rb_tree_node* pom::__grandparent(rb_tree_node* node) const {
	if(__isnil(node->parent))
		__raise_rb_tree_err(err_access_nil);
	return node->parent->parent;
}

pom::rb_tree_node* pom::__grandparent(rb_tree_node* node) {
	if(__isnil(node->parent))
		__raise_rb_tree_err(err_access_nil);
	return node->parent->parent;
}

bool pom::__is_left_child(const rb_tree_node* node) const {
	return node->parent->l_child == node;
}

bool pom::__is_right_child(const rb_tree_node* node) const {
	return node->parent->r_child == node;
}

bool pom::__isnil(const rb_tree_node* node) const {
	return (node == null());
}

void pom::__raise_rb_tree_err(const char* msg) const {
	throw std::runtime_error(msg);
}

pom::rb_tree_node* pom::__min_node(rb_tree_node* from) {
	for(; !__isnil(from) && !__isnil(from->l_child); from = from->l_child)
		;
	return from;
}

pom::rb_tree_node* pom::__successor(rb_tree_node* node) {
	if(!__isnil(node->r_child))
		return __min_node(node->r_child);

	rb_tree_node* p = node->parent;
	for(; !__isnil(p) && __is_right_child(node); node = p, p = node->parent)
		;
	return p;
}

void pom::pom_augmenting::initialize() {
	maximum = 0;
	number = 0;
	sum = 0;
	max_index = null();
}

bool pom::__sort_endpoint(const point& left, const point& right) {
	// when multiple endpoints have the same value, insert all the left
	// endpoints with that value before inserting any of the right endpoints 
	// with that value to deal with the boundary conditions.
	return left.cord == right.cord ? left.lp : left.cord < right.cord;
}

void pom::__split_endpoint(interval in, point_container& cont) {
	point p;
	p.cord = in.first;
	p.lp = true;
	cont.push_back(p);
	p.cord = in.second;
	p.lp = false;
	cont.push_back(p);
}

pom::rb_tree_node pom::Nil;

#ifdef _DEBUG_NODE_MEM
int pom::rb_tree_node::c_rb_tree_node;
#endif

int main() {
	try {
		int arr[] = {
			0, 3,
			5, 8,
			6, 10,
			8, 9, 
			15, 23,
			16, 21,
			17, 19,
			19, 20,
			25, 30,
			26, 26
		};
		pom::interval_container cont;
		for(size_t i = 0; i < _countof(arr); i += 2)
			cont.push_back(std::make_pair(arr[i], arr[i + 1]));

		pom tree;
		tree.calc_pom(cont);

		std::cout << tree.getpom() << ", pom count: " << tree.pom_count() << std::endl;
	}
	catch(const std::exception& exp) {
		std::cout << "caught an exception: " << exp.what() << std::endl;
	}

	return 0;
}
