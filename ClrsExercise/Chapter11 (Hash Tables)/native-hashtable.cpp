class Hashtable {
	static const unsigned HashTableSize = 103;
	typedef struct Node {
		Node(int k, int v) : key(k), value(v), next(NULL) {
		}
		int key;
		int value;
		Node* next;
	} *NodePtr;

	NodePtr nodes[HashTableSize];

	// 返回值一定是unsigned, 因为它会用作数组的索引!
	unsigned hashCode(int x) const {
		return x % HashTableSize;
	}

public:
	Hashtable() {
		std::fill(nodes, nodes + HashTableSize, (NodePtr)NULL);
	}

	void put(int key, int val) {
		unsigned h = hashCode(key);
		NodePtr solt = new Node(key, val);
		solt->next = nodes[h];
		nodes[h] = solt;
	}

	bool get(int key, int* value = NULL) const {
		unsigned h = hashCode(key);
		for (NodePtr p = nodes[h]; p != NULL; p = p->next) {
			if (p->key == key) {
				if (value != NULL)
					*value = p->value;
				return true;
			}
		}

		return false;
	}
};
