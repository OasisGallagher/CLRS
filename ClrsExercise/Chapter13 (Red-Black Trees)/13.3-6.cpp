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

// 只要进入tag[1]所示的else, 就会在18行将node的parent置为黑色, 从而再下次
// for循环处推出. 在这个else内, 只访问了node的parent,或者grantparent...等
// 与node的parent相关的节点.
// 同理, 8行的if分支也只在访问与node相关的parent.
// 因此, 可以保存node的parent, grantparent等节点, 在调用fixup时传入.
// 可以采用stack. 那么栈顶指针是parent的父节点.
//
