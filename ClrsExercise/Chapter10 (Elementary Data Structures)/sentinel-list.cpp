#include <iostream>
// list with sentinel.
// ͨ���ڱ�(sentinel)ʹ˫�������Ϊһ����.
// ����, ���Բ����Ǳ߽������ض�������в���.
// ����, �ڱ���nextָ������ĵ�һ���ڵ�, prevָ�����һ���ڵ�, Ҳʡȥ��
// head��tailָ��ı���.
//
class List {
	struct Node {
		int number;
		Node* prev;
		Node* next;
	};

public:
	List();
	~List();

	void push_front(int val) {
		Node* x = _Buynode(val);

		// setinal->nextָ������ͷ.
		// setianl->prevָ������β.
		x->next = sentinel->next;
		sentinel->next->prev = x;

		sentinel->next = x;
		x->prev = sentinel;
	}

	void push_back(int val) {
		Node* x = _Buynode(val);

		sentinel->prev->next = x;
		x->prev = sentinel->prev;

		sentinel->prev = x;
		x->next = sentinel;
	}

	void reverse() {
		// 
		// n1 -> n2 -> n3.
		Node* n1 = sentinel;
		Node* n2 = n1->next;
		while(n2 != sentinel) {
			Node* n3 = n2->next;
			n2->next = n1;
			n1->prev = n2;

			n1 = n2;
			n2 = n3;
		}

		n2->next = n1;
		n1->prev = n2;
	}

	void dump() const {
		for(Node* n = sentinel->next; n != sentinel; n = n->next) {
			std::cout << n->number << " ";
		}
		std::cout << std::endl;
	}

	void reverse_dump() const {
		for(Node* n = sentinel->prev; n != sentinel; n = n->prev) {
			std::cout << n->number << " ";
		}
		std::cout << std::endl;
	}

	void erase(int val) {
		Node* ite = sentinel->next;
		for(; ite != sentinel; ite = ite->next) {
			if(ite->number == val) {
				ite->prev->next = ite->next;
				ite->next->prev = ite->prev;
				delete ite;
				break;
			}
		}
	}
private:
	Node* _Buynode(int val) {
		Node* result = new Node;
		result->number = val;
		return result;
	}
	Node* sentinel;
};

List::List() {
	sentinel = _Buynode(0x7FFFFFFF);
	sentinel->next = sentinel;
	sentinel->prev = sentinel;
}

List::~List() {
	Node* ite = sentinel->next;
	for(; ite != sentinel; ) {
		Node* x = ite->next;
		delete ite;
		ite = x;
	}
	delete ite;
}

int main()
{
	List list;
	list.push_front(3);
	list.push_front(2);
	list.push_front(1);
	list.dump();

	list.reverse();

	list.dump();

	list.reverse();

	list.reverse_dump();
}
