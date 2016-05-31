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
// ��������fixup��һ����֧.
// 21��֮���else��֧���Բ��ؿ���, ��Ϊ�����ջ��node��Ϊroot_, ����
// ��������ɫΪblack.
// ֻ�е�8�к͵�16�л��һ���ڵ���Ϊ��ɫ.
// ��16���ǽ�node��sibling��Ϊ��ɫ, ��Ȼ���sibling������root, root
// ������Ϊ�κνڵ��sibling.
// ֻ�е�8�н�node��parent��Ϊ��ɫ, ��������Ϊroot, ��ô��������
// ֮��, sibling����Ϊ�µ�root, �����root, ��ɫ�Ǻ�ɫ��.
//
