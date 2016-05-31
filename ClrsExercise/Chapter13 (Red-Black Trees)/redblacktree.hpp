#pragma once
// red-black tree.
#include <stdexcept>
#include <functional>

static const bool red = true, black = false;
static const char* err_access_nil = "err: unable to access nil";
static const char* err_invalid_rb_tree = "err: invalid rb tree";
static const char* err_bad_alloc = "err: bad alloc";
static const char* err_invalid_predicate = "err: invalid predicate";

#undef _DEBUG_NODE_MEM

template <class K, class T, class P = std::less<K> >
class rb_tree {
public:
	rb_tree() : root_(null()) {}
	~rb_tree() {
		// destroy all nodes with inorder tree walk described in binarysearchtree.hpp.
	}
	typedef K key_type;
	typedef T mapped_type;
	typedef P pred_type;

	void insert(const key_type& key, const mapped_type& value) {
		rb_tree_node* node = __buy_node(key, value);
		__bst_insert(node);
		node->color = red;
		__rb_insert_fixup(node);
	}

	bool contains(const key_type& key) const {
		return !__isnil(__find_node(key));
	}

	bool erase(const key_type& key) {
		rb_tree_node* pos = __find_node(key);
		if (__isnil(pos))
			return false;
		rb_erase_result result = __bst_erase(pos);
		// tree above garbage is legal.
		if (result.first->color == black)
			__rb_erase_fixup(result.second);

		delete result.first;
		return true;
	}

	bool successor(const key_type& key, key_type* p) {
		rb_tree_node* pos = __find_node(key);
		if (!__isnil(pos) && !__isnil(pos = __successor(pos))) {
			if (p != nullptr)
				*p = pos->value.first;
			return true;
		}

		return false;
	}

	bool predecessor(const key_type& key, key_type* p) {
		rb_tree_node* pos = __find_node(key);
		if (!__isnil(pos) && !__isnil(pos = __predecessor(pos))) {
			if (p != nullptr)
				*p = pos->value.first;
			return true;
		}

		return false;
	}
#ifdef _DEBUG_NODE_MEM
	static int rb_tree_node_count() { return rb_tree_node::c_rb_tree_node; }
#endif // _DEBUG_NODE_MEM
private:
	typedef std::pair<key_type, mapped_type> value_type;
	struct rb_tree_node {
		rb_tree_node* parent;
		rb_tree_node* l_child;
		rb_tree_node* r_child;
		value_type value;
		bool color;
#ifdef _DEBUG_NODE_MEM
		void* operator new(size_t size) {
			++c_rb_tree_node;
			return ::operator new(size);
		}
		void operator delete(void* ptr) {
			--c_rb_tree_node;
			::operator delete(ptr);
		}
		static int c_rb_tree_node;
#endif // _DEBUG_NODE_MEM
	};

	rb_tree_node* __find_node(const key_type& key) const {
		rb_tree_node* iter = root_;
		for (; !__isnil(iter) && !equivalent(iter->value.first, key);)
			iter = (less(key, iter->value.first)) ? iter->l_child : iter->r_child;

		return iter;
	}
	
	void __left_rotate(rb_tree_node* pivot) {
		if (__isnil(pivot) || __isnil(pivot->r_child))
			__raise_rb_tree_err(err_access_nil);

		rb_tree_node* right_child = pivot->r_child;
		pivot->r_child = right_child->l_child;

		// 这里对nil的判断尤其重要.
		// 在erase fixup时, 传入的参数可能是nil, 之中记录着正确的parent,
		// 如果单纯的在这里, 不检查nil, 对nil也设置parent, 就会使nil的parent
		// 失效(nil是tree的static变量).
		if (!__isnil(right_child->l_child))
			right_child->l_child->parent = pivot;

		right_child->parent = pivot->parent;

		if (__isnil(pivot->parent))
			root_ = right_child;
		else if (__is_left_child(pivot))
			pivot->parent->l_child = right_child;
		else
			pivot->parent->r_child = right_child;

		right_child->l_child = pivot;
		pivot->parent = right_child;
	}

	void __right_rotate(rb_tree_node* pivot) {
		if (__isnil(pivot) || __isnil(pivot->l_child))
			__raise_rb_tree_err(err_access_nil);

		rb_tree_node* left_child = pivot->l_child;
		pivot->l_child = left_child->r_child;

		// 这里对nil的判断尤其重要.
		// 在erase fixup时, 传入的参数可能是nil, 之中记录着正确的parent,
		// 如果单纯的在这里, 不检查nil, 对nil也设置parent, 就会使nil的parent
		// 失效(nil是tree的static变量).
		if (!__isnil(left_child->r_child))
			left_child->r_child->parent = pivot;

		left_child->parent = pivot->parent;

		if (__isnil(pivot->parent))
			root_ = left_child;
		else if (__is_left_child(pivot))
			pivot->parent->l_child = left_child;
		else
			pivot->parent->r_child = left_child;

		left_child->r_child = pivot;
		pivot->parent = left_child;
	}

	bool __isnil(rb_tree_node* node) const {
		return (node == null());
	}

	void __raise_rb_tree_err(const char* msg) const {
		throw std::logic_error(msg);
	}

	void __bst_insert(rb_tree_node* node) {
		rb_tree_node* last_valid = null();
		for (rb_tree_node* iter = root_; !__isnil(iter);) {
			last_valid = iter;
			iter = (less(node->value.first, iter->value.first)) ? iter->l_child : iter->r_child;
		}

		node->parent = last_valid;

		if (__isnil(last_valid)) // empty tree.
			root_ = node;
		else if (less(node->value.first, last_valid->value.first))
			last_valid->l_child = node;
		else
			last_valid->r_child = node;
	}

	void __rb_insert_fixup(rb_tree_node* node) {
		// red parent.
		for (; node->parent->color == red;) {
			// node's parent is left child.
			if (__is_left_child(node->parent)) {
				// node->parent->color == red ==> node->parent != root_ ==> 
				// node->parent->parent != nil.
				// red uncle.
				rb_tree_node* uncle = __grandparent(node)->r_child;
				if (uncle->color == red) {
					uncle->color = node->parent->color = black;
					__grandparent(node)->color = red;
					node = __grandparent(node);
				}
				else {
					// make sure node to be left child.
					if (__is_right_child(node)) {
						node = node->parent;
						__left_rotate(node);
					}
					// fall through.
					// black height of left subtree rooted by node->parent->parent.
					node->parent->color = black;
					__grandparent(node)->color = red;
					// right rotate to adjust black height.
					__right_rotate(__grandparent(node));

					// exit loop here. cos' node's parent is black(Line: 167).
				}
			}
			// node's parent is right child.
			else {
				rb_tree_node* uncle = __grandparent(node)->l_child;
				if (uncle->color == red) {
					uncle->color = node->parent->color = black;
					__grandparent(node)->color = red;
					node = __grandparent(node);
				}
				else {
					// make sure node to be right child.
					if (__is_left_child(node)) {
						node = node->parent;
						__right_rotate(node);
					}
					node->parent->color = black;
					__grandparent(node)->color = red;
					__left_rotate(__grandparent(node));
				}
			}
		}

		root_->color = black;
	}

	typedef std::pair<rb_tree_node*, rb_tree_node*> rb_erase_result;
	rb_erase_result __bst_erase(rb_tree_node* node) {
		rb_tree_node* garbage = node;
		// two children.
		if (!__isnil(node->l_child) && !__isnil(node->r_child))
			garbage = __successor(node);
		// garbage has only one child at most.
		rb_tree_node* child = null();
		if (!__isnil(garbage->l_child))
			child = garbage->l_child;
		else
			child = garbage->r_child;

		// set parent even if child is nil to ensure __rb_erase_fixup
		// can reach child's parent(garbage's parent previously).
		child->parent = garbage->parent;

		if (__isnil(garbage->parent))
			root_ = child;
		else if (__is_left_child(garbage))
			garbage->parent->l_child = child;
		else
			garbage->parent->r_child = child;

		if (garbage != node)
			node->value = garbage->value;

		return std::make_pair(garbage, child);
	}

	void __rb_erase_fixup(rb_tree_node* node) {
		for (; node != root_ && node->color == black;) {
			// node is left child.
			if (__is_left_child(node)) {
				// sibling != null, due to the black-height rule.
				rb_tree_node* sibling = node->parent->r_child;
				if (sibling->color == red) {
					// node->parent->color = black.
					// exchange sibling's color with node's parent's color.
					sibling->color = black;
					node->parent->color = red;
					__left_rotate(node->parent);
					sibling = node->parent->r_child;
				}
				// fall through.
				// black sibling with two black children.
				if (sibling->l_child->color == black && sibling->r_child->color == black) {
					// decrease black height.
					sibling->color = red;
					// subtree rooted by node is legal.
					node = node->parent;
				}
				// black sibling with one black child at most.
				else {
					// left child is red.
					if (sibling->r_child->color == black) {
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
					// exit from for loop.
					node = root_;
				}
			}
			// node is right child.
			else {
				rb_tree_node* sibling = node->parent->l_child;
				if (sibling->color == red) {
					sibling->color = black;
					node->parent->color = red;
					__right_rotate(node->parent);
					sibling = node->parent->l_child;
				}

				if (sibling->l_child->color == black && sibling->r_child->color == black) {
					sibling->color = red;
					node = node->parent;
				}
				else {
					if (sibling->l_child->color == black) {
						sibling->color = red;
						sibling->r_child->color = black;
						__left_rotate(sibling);
						sibling = node->parent->l_child;
					}
					sibling->color = sibling->parent->color;
					sibling->parent->color = black;
					sibling->l_child->color = black;
					__right_rotate(node->parent);
					node = root_;
				}
			}
		}
		node->color = black;
	}

	bool __is_left_child(rb_tree_node* node) {
		return node->parent->l_child == node;
	}

	bool __is_right_child(rb_tree_node* node) {
		return node->parent->r_child == node;
	}

	rb_tree_node* __successor(rb_tree_node* node) {
		if (!__isnil(node->r_child))
			return __min_node(node->r_child);

		rb_tree_node* p = node->parent;
		for (; !__isnil(p) && __is_right_child(node); node = p, p = node->parent)
			;
		return p;
	}

	rb_tree_node* __predecessor(rb_tree_node* node) {
		if (node != NULL && node->l_child != NULL)
			return __max_node(node->l_child);

		rb_tree_node* p = (node != NULL) ? node->parent : NULL;
		for (; p != NULL && p->l_child == node;) {
			node = p;
			p = p->parent;
		}

		return p;
	}

	rb_tree_node* __min_node(rb_tree_node* from) {
		for (; !__isnil(from) && !__isnil(from->l_child); from = from->l_child)
			;
		return from;
	}

	rb_tree_node* __max_node(rb_tree_node* from) {
		for (; !__isnil(from) && !__isnil(from->r_child); from = from->r_child)
			;
		return from;
	}

	const rb_tree_node* __grandparent(rb_tree_node* node) const {
		if (__isnil(node->parent))
			__raise_rb_tree_err(err_access_nil);
		return node->parent->parent;
	}

	rb_tree_node* __grandparent(rb_tree_node* node) {
		if (__isnil(node->parent))
			__raise_rb_tree_err(err_access_nil);
		return node->parent->parent;
	}

	rb_tree_node* __buy_node(const key_type& key, const mapped_type& value) {
		try {
			rb_tree_node* node = new rb_tree_node;
			node->color = black;
			node->parent = node->r_child = node->l_child = null();
			node->value = std::make_pair(key, value);
			return node;
		}
		catch (...) {
			__raise_rb_tree_err(err_bad_alloc);
			return null();
		}
	}

	rb_tree_node* null() { return &Nil; }
	const rb_tree_node* null() const {
		return &Nil;
	}

	bool less(const key_type& lhs, const key_type& rhs) const {
		if (!pred(lhs, rhs))
			return false;
#ifdef DEBUG_PREDICATE
		else if (pred(rhs, lhs))
			__raise_rb_tree_err(err_invalid_predicate);
#endif // DEBUG_PREDICATE
		return true;
	}

	bool equivalent(const key_type& lhs, const key_type& rhs) const {
		return !pred(lhs, rhs) && !pred(rhs, lhs);
	}

	pred_type pred;
	rb_tree_node* root_;
	rb_tree_node Nil;
};
