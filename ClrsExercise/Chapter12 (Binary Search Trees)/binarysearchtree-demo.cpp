#include "sample-hdr.hpp"
#include <vector>
#include <numeric>
#include <algorithm>

int binary_search_tree::tree_node::instance_count = 0;

const int tree_test_value[] = { 925, 202, 911, 240, 912, 245, 363 };

void read_tree(binary_search_tree* tree);
void trace_tree(binary_search_tree* tree);

int main() {
	try {
		binary_search_tree tree;
		//tree.create_by_input();
		read_tree(&tree);
		trace_tree(&tree);

		std::vector<int> arr(_countof(tree_test_value), 1);
		std::partial_sum(arr.begin(), arr.end(), arr.begin());
		std::random_shuffle(arr.begin(), arr.end());

		for(size_t i = 0; i < arr.size() / 2; ++i)
			tree.tree_erase(tree_test_value[arr[i]]);

		trace_tree(&tree);
	}
	catch(const std::exception& exp) {
		std::cout << "cought an exception: " << exp.what() << std::endl;
	}
	return 0;
}

void read_tree(binary_search_tree* tree) {
	/*for(; ;) {
		std::cout << "enter a number, 0 to terminate: ";
		int number = 0;
		std::cin >> number;
		if(number == 0)
			break;
		tree->tree_insert(number);
	}
	*/
	
	for(int i = 0; i < _countof(tree_test_value); ++i)
		tree->tree_insert(tree_test_value[i]);
}

void trace_tree(binary_search_tree* tree) {
	std::cout << "inorder tree walk:\n";
	tree->inorder_tree_walk();

	std::cout << "min element is " << tree->min_element() << std::endl;
	std::cout << "max element is " << tree->max_element() << std::endl;
}
