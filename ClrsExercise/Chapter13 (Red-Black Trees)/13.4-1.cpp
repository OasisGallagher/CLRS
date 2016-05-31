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
			// exist from for loop.
			node = root_;
		}
	}
// 考虑上述fixup的一个分支.
// 21行之后的else分支可以不必考虑, 因为它最终会把node置为root_, 并且
// 设置其颜色为black.
// 只有第8行和第16行会把一个节点置为红色.
// 第16行是将node的sibling置为红色, 显然这个sibling不会是root, root
// 不会作为任何节点的sibling.
// 只有第8行将node的parent置为红色, 如果这个点为root, 那么经过左旋
// 之后, sibling会作为新的root, 而这个root, 颜色是黑色的.
//
