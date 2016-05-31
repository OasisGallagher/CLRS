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
		else {	// tag[1].
			// make sure node to be left child.
			if(__is_right_child(node)) {
				....
			}
			node->parent->color = black;
			__grandparent(node)->color = red;
			// right rotate to adjust black height.
			__right_rotate(__grandparent(node));
		}
	}
}

// ֻҪ����tag[1]��ʾ��else, �ͻ���18�н�node��parent��Ϊ��ɫ, �Ӷ����´�
// forѭ�����Ƴ�. �����else��, ֻ������node��parent,����grantparent...��
// ��node��parent��صĽڵ�.
// ͬ��, 8�е�if��֧Ҳֻ�ڷ�����node��ص�parent.
// ���, ���Ա���node��parent, grantparent�Ƚڵ�, �ڵ���fixupʱ����.
// ���Բ���stack. ��ôջ��ָ����parent�ĸ��ڵ�.
//
