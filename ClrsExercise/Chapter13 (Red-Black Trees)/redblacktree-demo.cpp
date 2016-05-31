#include "sample-hdr.hpp"
#include <iostream>

rb_tree::rb_tree_node rb_tree::Nil;

#ifdef _DEBUG_NODE_MEM
int rb_tree::rb_tree_node::c_rb_tree_node;
#endif // _DEBUG_NODE_MEM

int main() {
	rb_tree tree;
	int arr[] = { 41, 38, 31, 12, 19, 8 };
	for(size_t i = 0; i < _countof(arr); ++i)
		tree.insert(arr[i]);

	for(size_t i = 0; i < _countof(arr); ++i)
		if(!tree.erase(arr[i]))
			std::cout << "invalid erase on index " << i << std::endl;
#ifdef _DEBUG_NODE_MEM
	std::cout << rb_tree::rb_tree_node_count() << " rb_tree_node remains.\n";
#endif // _DEBUG_NODE_MEM
}
