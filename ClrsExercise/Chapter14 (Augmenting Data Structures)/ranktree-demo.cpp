#include "sample-hdr.hpp"
#include <iostream>

int main() {
	try {
		rank_tree tree;
		tree.insert(2);
		tree.insert(1);
		tree.insert(3);
		tree.insert(4);
		tree.insert(7);

		//tree.erase(1);

		int result = 0;
		//tree.select(1, &result);

		std::cout << tree.key_rank(4) << std::endl;

		//tree.nth_successor(0, &result);
		//tree.nth_successor(3, &result);
	}
	catch(const std::exception& exp) {
		std::cout << "caught an exception: " << exp.what() << std::endl;
	}

	return 0;
}
