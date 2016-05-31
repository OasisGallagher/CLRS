#pragma once
#include <iostream>
#include <stack>		// tree walk helper.

static const char* left_child_dump = "enter left child(0 to terminate): ";
static const char* right_child_dump = "enter right child(0 to terminate): ";
static const char* root_dump = "enter root(0 to terminate): ";

class binary_search_tree {
protected:
	struct tree_node {
		tree_node* parent;
		tree_node* l_child;
		tree_node* r_child;
		int val;
		tree_node(tree_node* init = NULL) : parent(init), l_child(init), r_child(init) {}
		void* operator new(size_t size) {
			++instance_count;
			__debug_instance_count();
			return ::operator new(size);
		}
		void operator delete(void* ptr) {
			--instance_count;
			__debug_instance_count();
			::operator delete(ptr);
		}
		static void __debug_instance_count() {
			//std::cout << "debug: instance_count = " << instance_count << std::endl;
		}
		static int instance_count;
	};
public:
	binary_search_tree() : root_(NULL) { }
	~binary_search_tree() { __clear(); }
	void create_by_input();
	void tree_insert(int element);
	void inorder_tree_walk();
	bool tree_contains(int test) const;
	void tree_erase(int erase);
	int min_element() const;
	int max_element() const;
	void clear();
protected:
	void __clear();
	tree_node* __alloc_node(int element);
	void __tree_insert(tree_node* const node);
	tree_node* __successor(tree_node* node);
	tree_node* __predecessor(tree_node* node);
	tree_node* __min_node(tree_node* from) const;
	tree_node* __max_node(tree_node* from) const;
	tree_node* __tree_find(int test) const;
	void __tree_erase(tree_node* node);
	void __create_child_by_input(tree_node* node);
	tree_node* __read_node();

	typedef void (binary_search_tree::*tree_walk_callback)(tree_node* node);
	void __loop_inorder_tree_walk(tree_walk_callback callback);
	
	void __raise_empty_tree() const { throw std::invalid_argument("empty tree"); }
	void __callback_trace_node(tree_node* node) { std::cout << node->val << std::endl; }
	void __callback_node_killer(tree_node* node) { delete node; }
	void __trace(const char* msg) { std::cout << msg << std::endl; }
	void __recursively_inorder_tree_walk(tree_node* node);
	void __recursively_insert(tree_node* const node, tree_node* cur);

	tree_node* root_;
};

void binary_search_tree::create_by_input() {
	__trace(root_dump);
	int number = 0;
	std::cin >> number;
	if(number != 0) {
		root_ = new tree_node;
		root_->val = number;
		__create_child_by_input(root_);
	}
}

void binary_search_tree::inorder_tree_walk() {
	//__recursively_inorder_tree_walk(root);
	__loop_inorder_tree_walk(&binary_search_tree::__callback_trace_node);
}

bool binary_search_tree::tree_contains(int test) const {
	return __tree_find(test) != NULL;
}

void binary_search_tree::tree_erase(int erase) {
	tree_node* pos = __tree_find(erase);
	if(pos != NULL)
		__tree_erase(pos);
}

void binary_search_tree::__tree_erase(tree_node* node) {
	tree_node* real = NULL;
	// has at most one child.
	if(node->l_child == NULL || node->r_child == NULL)
		real = node;
	else	// has two children. delete the successor of node.
		real = (rand() % 2 == 0) ? __predecessor(node) : __successor(node); // real != NULL.
	
	// child of the node.
	tree_node* real_child = NULL;
	// if real is the successor of node, real->l_child must be null.
	// then child points to real's right child.
	if(real->l_child != NULL)
		real_child = real->l_child;
	else
		real_child = real->r_child;

	// handle pointer from child to parent.
	// link real's parent and it's child.
	// skip real.
	if(real_child != NULL)
		real_child->parent = real->parent;

	// handle pointer from parent to child.
	// root will be deleted.
	if(real->parent == NULL)
		root_ = real_child;
	// real is the left child of it's parent.
	else if(real == real->parent->l_child)
		real->parent->l_child = real_child;
	else
		real->parent->r_child = real_child;

	// copy data of *real to node.
	if(real != node)
		node->val = real->val;

	delete real;
}

int binary_search_tree::max_element() const {
	tree_node* max_node = __max_node(root_);
	if(max_node == NULL)
		__raise_empty_tree();
	return max_node->val;
}

int binary_search_tree::min_element() const {
	tree_node* min_node = __min_node(root_);
	if(min_node == NULL)
		__raise_empty_tree();
	return min_node->val;
}

binary_search_tree::tree_node* binary_search_tree::__alloc_node(int element) {
	tree_node* node = new tree_node;
	node->val = element;
	return node;
}

void binary_search_tree::clear() {
	__clear();
}

void binary_search_tree::tree_insert(int element) {
	//__tree_insert(__alloc_node(element));
	__recursively_insert(__alloc_node(element), root_);
}

void binary_search_tree::__clear() {
	__loop_inorder_tree_walk(&binary_search_tree::__callback_node_killer);
}

void binary_search_tree::__tree_insert(tree_node* const node) {
	// last tree node != NULL.
	tree_node* last_valid = NULL;
	for(tree_node* iter = root_; iter != NULL; ) {
		last_valid = iter;
		iter = (node->val < iter->val) ? iter->l_child : iter->r_child;
	}

	node->parent = last_valid;
	if(last_valid == NULL)	// empty tree.
		root_ = node;
	else if(node->val < last_valid->val)
		last_valid->l_child = node;
	else
		last_valid->r_child = node;
}

binary_search_tree::tree_node* binary_search_tree::__min_node(tree_node* from) const {
	tree_node* node = from;
	for(; node != NULL && node->l_child != NULL;
		node = node->l_child)
		;

	return node;
	//
	// return from != NULL && from->l_child != NULL ? __min_node(from->l_child) : from;
	//
}

binary_search_tree::tree_node* binary_search_tree::__max_node(tree_node* from) const {
	tree_node* node = from;
	for(; node != NULL && node->r_child != NULL;
		node = node->r_child)
		;

	return node;
}

binary_search_tree::tree_node* binary_search_tree::__successor(tree_node* node) {
	if(node != NULL && node->r_child != NULL)
		return __min_node(node->r_child);

	tree_node* p = (node != NULL) ? node->parent : NULL;
	for(; p != NULL && p->r_child == node; ) {
		node = p;
		p = p->parent;
	}

	return p;
}

binary_search_tree::tree_node* binary_search_tree::__predecessor(tree_node* node) {
	if(node != NULL && node->l_child != NULL)
		return __max_node(node->l_child);

	tree_node* p = (node != NULL) ? node->parent : NULL;
	for(; p != NULL && p->l_child == node; ) {
		node = p;
		p = p->parent;
	}

	return p;
}

binary_search_tree::tree_node* binary_search_tree::__tree_find(int test) const {
	tree_node* node = root_;
	for(; node != NULL && node->val != test; ) {
		node = (test < node->val) ? node->l_child : node->r_child;
	}

	return node;
}

void binary_search_tree::__create_child_by_input(tree_node* node) {
	// left child.
	__trace(left_child_dump);
	if((node->l_child = __read_node()) != NULL) {
		node->l_child->parent = node;
		__create_child_by_input(node->l_child);
	}

	// right child.
	__trace(right_child_dump);
	if((node->r_child = __read_node()) != NULL) {
		node->r_child->parent = node;
		__create_child_by_input(node->r_child);
	}
}

void binary_search_tree::__loop_inorder_tree_walk(tree_walk_callback callback) {
	std::stack<tree_node*> helper;
	tree_node* cur = root_;
	while(!helper.empty() || cur != NULL) {
		for(; cur != NULL; cur = cur->l_child)
			helper.push(cur);

		if(!helper.empty()) {
			tree_node* proc = helper.top();
			helper.pop();
			cur = proc->r_child;

			(this->*callback)(proc);
		}
	}
}

binary_search_tree::tree_node* binary_search_tree::__read_node() {
	int number = 0;
	std::cin >> number;
	if(number != 0) {
		tree_node* child = new tree_node;
		child->val = number;
		return child;
	}

	return NULL;
}

void binary_search_tree::__recursively_inorder_tree_walk( tree_node* node ) {
	if(node != NULL) {
		__recursively_inorder_tree_walk(node->l_child);
		std::cout << node->val << std::endl;
		__recursively_inorder_tree_walk(node->r_child);
	}
}

void binary_search_tree::__recursively_insert(tree_node* const node, tree_node* cur) {
	if(cur == NULL) {
		root_ = node;
		return;
	}

	if(node->val < cur->val) {
		if(cur->l_child != NULL)
			__recursively_insert(node, cur->l_child);
		else {
			cur->l_child = node;
			node->parent = cur;
		}
	}
	else {
		if(cur->r_child != NULL)
			__recursively_insert(node, cur->r_child);
		else {
			cur->r_child = node;
			node->parent = cur;
		}
	}
}

