struct Node {
	Node() : parent(nullptr), lchild(nullptr), rchild(nullptr) { }
	int dat;
	Node* parent;
	Node* lchild;
	Node* rchild;
};

Node* least_common_ancestor_in_binary_search_tree(Node* from, Node* lhs, Node* rhs) {
	if(lhs->dat <= from->dat && rhs->dat > from->dat)
		return from;

	if(lhs->dat > from->dat)
		return least_common_ancestor_in_binary_search_tree(from->rchild, lhs, rhs);
	
	return least_common_ancestor_in_binary_search_tree(from->lchild, lhs, rhs);
}

Node* __least_common_ancestor(Node* from, Node* lhs, Node* rhs) {
	if(from == nullptr)
		return from;

	if(from->lchild == lhs || from->lchild == rhs || from->rchild == lhs || from->rchild == rhs)
		return from;

	Node* lp = __least_common_ancestor(from->lchild, lhs, rhs);
	Node* rp = __least_common_ancestor(from->rchild, lhs, rhs);

	if(lp == nullptr && rp == nullptr)
		return nullptr;

	if(lp != nullptr && rp != nullptr)
		return from;

	if(lp != nullptr)
		return __least_common_ancestor(from->lchild, lhs, rhs);

	return __least_common_ancestor(from->rchild, lhs, rhs);
}

Node* least_common_ancestor(Node* root, Node* lhs, Node* rhs) {
	// if the tree rooted by root is a binary search tree.
	// return least_common_ancestor_in_binary_search_tree(root, lhs, rhs);
	// otherwise
	return __least_common_ancestor(root, lhs, rhs);
}

int main() {
	Node* nodes[5];
	for(int i = 0; i < 5; ++i) {
		nodes[i] = new Node;
		nodes[i]->dat = i;
	}
	/*
	      0
	  1       3
    2   4
	*/
	nodes[0]->lchild = nodes[1];
	nodes[0]->rchild = nodes[3];
	nodes[1]->lchild = nodes[2];
	nodes[1]->rchild = nodes[4];

	Node* ancestor = least_common_ancestor(nodes[0], nodes[2], nodes[4]);
	(void)ancestor;
}

