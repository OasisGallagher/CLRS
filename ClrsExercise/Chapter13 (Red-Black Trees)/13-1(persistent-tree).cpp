#pragma once
// clrs 13-1.
// (a) 从根节点到需要插到的位置的父节点(包括二者)都需要复制一份.
// (b)
#include <vector>	// version controller.
#include <stdexcept>

class persistent_tree
{
public:
	persistent_tree() : versions_(1, NULL) { }
	// insert val into current version.
	void insert(int val);
	// start a new version and insert val into it.
	void modify_insert(int val);
private:
	struct tree_node {
		int val;
		// without parent ptr.
		tree_node* l_child;
		tree_node* r_child;
	};
	// map version to tree root;
	typedef std::vector<tree_node*> version_container;
	tree_node* __start_version(int val);
	tree_node* __start_version(tree_node* now, int val);
	tree_node* __get_hole(int val);
	tree_node* __allocate(int val);
	tree_node* __duplicate_node(const tree_node* other);
	tree_node* __first_version();
	version_container versions_;
};

void persistent_tree::insert(int val) {
	tree_node* last = __get_hole(val);
	tree_node* node = __allocate(val);

	if(last == NULL)
		versions_[0] = node;
	else if(val < last->val)
		last->l_child = node;
	else
		last->r_child = node;
}

persistent_tree::tree_node* persistent_tree::__get_hole(int val) {
	tree_node* last = NULL;
	for(tree_node* iter = __first_version(); iter != NULL; ) {
		last = iter;
		iter = (val < iter->val) ? iter->l_child : iter->r_child;
	}
	return last;
}

void persistent_tree::modify_insert(int val) {
	if(versions_[0] == NULL) {
		throw std::logic_error("first version doesn't exist");
	}
	//versions_.push_back(__start_version(val));
	versions_.push_back(__start_version(__first_version(), val));
}

persistent_tree::tree_node* persistent_tree::__first_version() {
	return versions_.empty() ? NULL : versions_[0];
}

persistent_tree::tree_node* persistent_tree::__start_version(tree_node* now, int val) {
	if(now == NULL) {
		now = __allocate(val);
	}
	else {
		if(val < now->val) {
			now = __duplicate_node(now);
			now->l_child = __start_version(now->l_child, val);
		}
		else {
			now = __duplicate_node(now);
			now->r_child = __start_version(now->r_child, val);
		}
	}

	return now;
}

persistent_tree::tree_node* persistent_tree::__start_version(int val) {
	tree_node* first = __first_version();
	tree_node* new_root = __duplicate_node(first);

	tree_node* parent = NULL;
	for(tree_node* now = new_root; now != NULL; ) {
		parent = now;
		if(val < parent->val) {
			now = __duplicate_node(parent->l_child);
			parent->l_child = now;
		}
		else {
			now = __duplicate_node(parent->r_child);
			parent->r_child = now;
		}
	}

	tree_node* node = __allocate(val);
	// first version is empty.
	if(parent == NULL)
		new_root = node;
	else if(val < parent->val)
		parent->l_child = node;
	else
		parent->r_child = node;

	return new_root;
}

persistent_tree::tree_node* persistent_tree::__duplicate_node(const tree_node* other) {
	tree_node* result = NULL;
	if(other != NULL) {
		result = __allocate(0);
		result->val = other->val;
		result->l_child = other->l_child;
		result->r_child = other->r_child;
	}
	return result;
}

persistent_tree::tree_node* persistent_tree::__allocate(int val) {
	try {
		tree_node* ptr = new tree_node;
		ptr->l_child = ptr->r_child = NULL;
		ptr->val = val;
		return ptr;
	}
	catch(...) {
		// bad alloc.
		return NULL;
	}
}

int main() {
	persistent_tree tree;
	// version 1.
	tree.insert(2);
	tree.insert(1);
	tree.insert(3);

	// version 2.
	tree.modify_insert(4);

	// version 3.
	tree.modify_insert(0);
}

// (c) 根据(a), 可知插入一个节点需要复制h个节点. 所以时间和空间都为O(h).
// (d) persistent_tree没有提供parent节点, 增加这个节点会增加额外的负担.
//     因为此时再次增加一个新节点后, 所有的节点都要被复制, 这样才能使得
//	   子节点正确的指向其根节点.
// (e) see Instructor's Manual. 
