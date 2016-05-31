#include <iostream>
#include <vector>

#pragma once
// rank tree.
#include <stdexcept>

static const bool red = true, black = false;
static const char* err_access_nil = "err: unable to access nil";
static const char* err_invalid_rb_tree = "err: invalid rb tree";
static const char* err_bad_alloc = "err: bad alloc";
static const char* err_key_doesnot_exist = "err: key doesn't exist";

// #define _DEBUG_NODE_MEM

class rank_tree {
public:
	rank_tree() : root_(null()), size_(0) {}
	~rank_tree() {
		// TODO:
		// destroy all nodes with inorder tree walk described in binarysearchtree.hpp.
	}
	typedef unsigned rank_t;

	void insert(int val);
	bool contains(int val) const;
	bool erase(int val);
	// nth(starts from 0) element in inorder-tree-walk.
	bool select(rank_t rank, int* result) const;

	rank_t key_rank(int key) const;

	void josephus(size_t m);

#ifdef _DEBUG_NODE_MEM
	static int rb_tree_node_count() { return rb_tree_node::c_rb_tree_node; }
#endif // _DEBUG_NODE_MEM
private:
	struct rb_tree_node {
		rb_tree_node* parent;
		rb_tree_node* l_child;
		rb_tree_node* r_child;
		// rank info.
		size_t size;	// size[l_child] + size[r_child] + 1[itself]
		int val;
		bool color;
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

	// nth(starts from 1) element in inorder-tree-walk.
	// rank to node.
	const rb_tree_node* __select(rank_t rank, const rb_tree_node* node) const;
	rb_tree_node* __select(rank_t rank, rb_tree_node* node);
	// 14.1-3.
	const rb_tree_node* __select2(rank_t rank) const;
	rb_tree_node* __select2(rank_t rank);
	// 14.1-4.
	rank_t __key_rank(const rb_tree_node* node, int key) const;
	// 14.1-5 (n starts from 1).
	const rb_tree_node* __nth_successor(const rb_tree_node* node, size_t n) const;

	// result starts from 1.
	// the same with __select. node to rank.
	size_t __rank(const rb_tree_node* node) const;

	rb_tree_node* __find_node(int val) const;

	void __left_rotate(rb_tree_node* pivot);
	void __right_rotate(rb_tree_node* pivot);
	bool __isnil(const rb_tree_node* node) const;
	void __raise_rb_tree_err(const char* msg) const;

	void __bst_insert(rb_tree_node* node);
	void __rb_insert_fixup(rb_tree_node* node);
	typedef std::pair<rb_tree_node*, rb_tree_node*> rb_erase_result;
	rb_erase_result __bst_erase(rb_tree_node* node);
	void __erase(rb_tree_node* pos);
	void __rb_erase_fixup(rb_tree_node* node);

	bool __is_left_child(const rb_tree_node* node) const;
	bool __is_right_child(const rb_tree_node* node) const;

	rb_tree_node* __successor(rb_tree_node* node);

	rb_tree_node* __min_node(rb_tree_node* from);

	const rb_tree_node* __grandparent(rb_tree_node* node) const;
	rb_tree_node* __grandparent(rb_tree_node* node);

	rb_tree_node* __buy_node(int val);

	static rb_tree_node* null() { return &Nil; }

	size_t size_;
	rb_tree_node* root_;
	static rb_tree_node Nil;
};

void rank_tree::insert(int val) {
	rb_tree_node* node = __buy_node(val);
	__bst_insert(node);
	node->color = red;
	__rb_insert_fixup(node);
	++size_;
}

void rank_tree::__left_rotate(rb_tree_node* pivot) {
	if(__isnil(pivot) || __isnil(pivot->r_child))
		__raise_rb_tree_err(err_access_nil);

	rb_tree_node* right_child = pivot->r_child;
	pivot->r_child = right_child->l_child;

	if(!__isnil(right_child->l_child))
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

	// <size_info>
	right_child->size = pivot->size;
	pivot->size = pivot->l_child->size + pivot->r_child->size + 1;
	// </size_info>
}

void rank_tree::__right_rotate(rb_tree_node* pivot) { 
	if(__isnil(pivot) || __isnil(pivot->l_child))
		__raise_rb_tree_err(err_access_nil);

	rb_tree_node* left_child = pivot->l_child;
	pivot->l_child = left_child->r_child;

	if(!__isnil(left_child->r_child))
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

	// <size_info>
	left_child->size = pivot->size;
	pivot->size = pivot->l_child->size + pivot->r_child->size + 1;
	// </size_info>
}

void rank_tree::__bst_insert(rb_tree_node* node) {
	rb_tree_node* last_valid = null();
	for(rb_tree_node* iter = root_; !__isnil(iter); ) {
		last_valid = iter;

		// size_info.
		++last_valid->size;

		iter = (node->val < iter->val) ? iter->l_child : iter->r_child;
	}

	node->parent = last_valid;

	if(__isnil(last_valid)) // empty tree.
		root_ = node;
	else if(node->val < last_valid->val)
		last_valid->l_child = node;
	else
		last_valid->r_child = node;
}

void rank_tree::__rb_insert_fixup(rb_tree_node* node) {
	// red parent.
	for(; node->parent->color == red; ) {
		// node's parent is left child.
		if(__is_left_child(node->parent)) {
			// node->parent->color == red ==> node->parent != root_ ==> 
			// node->parent->parent != nil.
			// red uncle.
			rb_tree_node* uncle = __grandparent(node)->r_child;
			if(uncle->color == red) {
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

				// exit loop here. cos' node's parent is black(Line: 167).
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

bool rank_tree::contains(int val) const {
	return !__isnil(__find_node(val));
}

bool rank_tree::select(rank_t rank, int* result) const {
	if(rank >= size_)
		return false;

	//const rb_tree_node* p = __select(++rank, root_);
	const rb_tree_node* p = __select2(++rank);
	if(!__isnil(p) && result != NULL)
		*result = p->val;
	return true;
}

rank_tree::rank_t rank_tree::key_rank(int key) const {
	return __key_rank(root_, key);
}

void rank_tree::josephus(size_t m) {
	for(rank_t r = 1; size_ > 0; ) {
		// <simplify the following code>
		// r = (r + m - 1) % size_;
		// if(r == 0) r = size_;
		// <to>
		r = ((r + m - 1) - 1) % size_ + 1;

		rb_tree_node* p = __select2(r);
		std::cout << p->val << std::endl;
		__erase(p);
	}
}

bool rank_tree::erase(int val) {
	rb_tree_node* pos = __find_node(val);
	if(__isnil(pos))
		return false;

	__erase(pos);
	return true;
}

void rank_tree::__erase(rb_tree_node* pos) {
	rb_erase_result result = __bst_erase(pos);
	// tree above garbage is legal.
	if(result.first->color == black)
		__rb_erase_fixup(result.second);

	delete result.first;
	--size_;
}

rank_tree::rb_erase_result rank_tree::__bst_erase(rb_tree_node* node) {
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

		// size_info
		for(rb_tree_node* p = garbage->parent; !__isnil(p); p = p->parent)
			--p->size;
	}
	if(garbage != node)
		node->val = garbage->val;

	return std::make_pair(garbage, child);
}

void rank_tree::__rb_erase_fixup(rb_tree_node* node) {
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

const rank_tree::rb_tree_node* rank_tree::__select(rank_t rank, const rb_tree_node* node) const {
	rank_t sz = node->l_child->size + 1;
	if(sz == rank)
		return node;

	return (sz < rank) ? __select(rank - sz, node->r_child) : __select(rank, node->l_child);
}

rank_tree::rb_tree_node* rank_tree::__select(rank_t rank, rb_tree_node* node) {
	rank_t sz = node->l_child->size + 1;
	if(sz == rank)
		return node;

	return (sz < rank) ? __select(rank - sz, node->r_child) : __select(rank, node->l_child);
}

const rank_tree::rb_tree_node* rank_tree::__select2(rank_t rank) const {
	rb_tree_node* cur = root_;
	for(size_t lsize = 0; (lsize = cur->l_child->size + 1) != rank; ) {
		if(lsize < rank) {
			cur = cur->r_child;
			rank -= lsize;
		}
		else
			cur = cur->l_child;
	}

	return cur;
}

rank_tree::rb_tree_node* rank_tree::__select2(rank_t rank) {
	rb_tree_node* cur = root_;
	for(size_t lsize = 0; (lsize = cur->l_child->size + 1) != rank; ) {
		if(lsize > rank)
			cur = cur->l_child;
		else{
			cur = cur->r_child;
			rank -= lsize;
		}
	}

	return cur;
}

rank_tree::rank_t rank_tree::__key_rank(const rb_tree_node* node, int key) const {
	if(key == node->val)
		return 1;

	if(key < node->val && !__isnil(node->l_child))
		return __key_rank(node->l_child, key);

	if(!__isnil(node->r_child))
		return node->l_child->size + 1 + __key_rank(node->r_child, key);

	__raise_rb_tree_err(err_key_doesnot_exist);
	return 0;
}

const rank_tree::rb_tree_node* rank_tree::__nth_successor(const rb_tree_node* node, size_t n) const {
	return __select(__rank(node) + n, root_);
}

size_t rank_tree::__rank(const rb_tree_node* node) const {
	size_t rank = node->l_child->size + 1;
	for(; node != root_; node = node->parent) {
		if(__is_right_child(node))
			rank += node->parent->l_child->size + 1;
	}

	return rank;
}

rank_tree::rb_tree_node* rank_tree::__find_node(int val) const {
	rb_tree_node* iter = root_;
	for(; !__isnil(iter) && iter->val != val; ) 
		iter = (val < iter->val) ? iter->l_child : iter->r_child;

	return iter;
}

rank_tree::rb_tree_node* rank_tree::__buy_node(int val) {
	try {
		rb_tree_node* node = new rb_tree_node;
		node->color = black;
		node->parent = node->r_child = node->l_child = null();
		node->val = val;
		node->size = 1;
		return node;
	}
	catch(...) {
		__raise_rb_tree_err(err_bad_alloc);
		return null();
	}
}

const rank_tree::rb_tree_node* rank_tree::__grandparent(rb_tree_node* node) const {
	if(__isnil(node->parent))
		__raise_rb_tree_err(err_access_nil);
	return node->parent->parent;
}

rank_tree::rb_tree_node* rank_tree::__grandparent(rb_tree_node* node) {
	if(__isnil(node->parent))
		__raise_rb_tree_err(err_access_nil);
	return node->parent->parent;
}

bool rank_tree::__is_left_child(const rb_tree_node* node) const {
	return node->parent->l_child == node;
}

bool rank_tree::__is_right_child(const rb_tree_node* node) const {
	return node->parent->r_child == node;
}

bool rank_tree::__isnil(const rb_tree_node* node) const {
	return (node == null());
}

void rank_tree::__raise_rb_tree_err(const char* msg) const {
	throw std::logic_error(msg);
}

rank_tree::rb_tree_node* rank_tree::__min_node(rb_tree_node* from) {
	for(; !__isnil(from) && !__isnil(from->l_child); from = from->l_child)
		;
	return from;
}

rank_tree::rb_tree_node* rank_tree::__successor(rb_tree_node* node) {
	if(!__isnil(node->r_child))
		return __min_node(node->r_child);

	rb_tree_node* p = node->parent;
	for(; !__isnil(p) && __is_right_child(node); node = p, p = node->parent)
		;
	return p;
}

rank_tree::rb_tree_node rank_tree::Nil;

#ifdef _DEBUG_NODE_MEM
int rank_tree::rb_tree_node::c_rb_tree_node;
#endif


int main() {
	rank_tree tree;
	for(size_t i = 1; i <= 7; ++i)
		tree.insert(i);

	tree.josephus(3);

	return 0;
}
