#include <iostream>
#include <string>
#include <algorithm>
#include <functional>
#include <stack>

class radix_tree {
public:
	radix_tree();
	~radix_tree();
	typedef std::string x01string;
	void insert(const x01string& x01);
	void output_sorted() const;
private:
	struct tree_node {
		tree_node() : parent(NULL), l_child(NULL), r_child(NULL) { }
		tree_node* parent;
		tree_node* l_child;
		tree_node* r_child;
		x01string str;
	};
	// insert ch into tree. returns the node.
	tree_node* __insert_element(x01string::value_type ch, tree_node* from);
	void __preorder_tree_walk(const tree_node* node) const;
	void __chk_radix(const x01string& str) const;
	static bool equal_to_01(x01string::value_type ch) { return ch == '0' || ch == '1'; }

	tree_node* root_;
};

radix_tree::radix_tree() : root_(new tree_node) {}
radix_tree::~radix_tree() {}

void radix_tree::insert(const x01string& x01) {
	__chk_radix(x01);
	tree_node* pos = root_;
	for(x01string::const_iterator ite = x01.begin(); ite != x01.end(); ++ite)
		pos = __insert_element(*ite, pos);
	if(pos != root_ && pos != NULL)
		pos->str = x01;
}

radix_tree::tree_node* radix_tree::__insert_element(x01string::value_type ch, tree_node* from) {
	tree_node* dest = NULL;
	if(ch == '0')
		dest = from->l_child;
	else
		dest = from->r_child;

	if(dest == NULL) {
		dest = new tree_node;
		dest->parent = from;

		if(ch == '0')
			from->l_child = dest;
		else
			from->r_child = dest;
	}

	return dest;
}

void radix_tree::output_sorted() const {
	__preorder_tree_walk(root_);
}

void radix_tree::__preorder_tree_walk(const tree_node* node) const {
	/*if(node != NULL) {
		if(!node->str.empty())
			std::cout << node->str << std::endl;
		__output_sorted(node->l_child);
		__output_sorted(node->r_child);
	}*/
	std::stack<const tree_node*> helper;
	while(node != NULL || !helper.empty()) {
		for(; node != NULL; node = node->l_child) {
			if(!node->str.empty())
				std::cout << node->str << std::endl;
			helper.push(node);
		}

		if(!helper.empty()) {
			const tree_node* cur = helper.top();
			helper.pop();
			node = cur->r_child;
		}
	}
}

void radix_tree::__chk_radix(const x01string& str) const {
	if(std::find_if(str.begin(), str.end(), std::not1(std::ptr_fun(equal_to_01))) != str.end())
		throw "x01string must be made up with 0 or 1";
}

void read_tree(radix_tree* tree);

int main() {
	radix_tree tree;
	read_tree(&tree);
	tree.output_sorted();
}

void read_tree(radix_tree* tree) {
	const char* str_array[] = {
		"1011",
		"10",
		"011",
		"100",
		"0"
	};
	for(unsigned i = 0; i < _countof(str_array); ++i)
		tree->insert(str_array[i]);
}
