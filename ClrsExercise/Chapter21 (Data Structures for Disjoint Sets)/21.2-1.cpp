#include <iostream>
#include <cassert>
#include <set>
#include <algorithm>

class DisjointSet {
	struct List;
	class ListNode {
		friend class DisjointSet;
		List* rep;
		ListNode* next;
		int dat;
	};

	struct List {
		ListNode *head, *tail;
		size_t size;
	};

	void link(List* x, List* y) {
		if(x->size >= y->size)
			std::swap(x, y);

		for(ListNode* p = x->head; p != NULL; p = p->next)
			p->rep = y;

		y->tail->next = x->head;
		y->tail = x->tail;
		y->size += x->size;

		sets.erase(x);
		delete x;
	}

	template <class Ty>
	static void killer(Ty ptr) { delete ptr; }

	typedef std::set<List*> SetContainer;
	SetContainer sets;
public:
	~DisjointSet() {
		for(SetContainer::iterator ite = sets.begin(); ite != sets.end(); ++ite) {
			for(ListNode* ptr = (*ite)->head; ptr != NULL; ) {
				ListNode* tmp = ptr->next;
				delete ptr;
				ptr = tmp;
			}
		}
		std::for_each(sets.begin(), sets.end(), killer<List*>);
	}

	typedef ListNode* NodePtr;
	NodePtr makeSet(int dat) {
		List* list = new List;
		sets.insert(list);

		list->head = list->tail = new ListNode;
		list->size = 1;
		list->head->next = NULL;
		list->head->rep = list;
		list->head->dat = dat;
		return list->head;
	}

	typedef unsigned SetIdentifier;
	SetIdentifier findSet(NodePtr x) {
		return (SetIdentifier)x->rep;
	}

	void linkSets(NodePtr x, NodePtr y) {
		link(x->rep, y->rep);
	}

	void linkSets(SetIdentifier x, SetIdentifier y) {
		link((List*)x, (List*)y);
	}
};

int main() {
	DisjointSet djset;
	DisjointSet::NodePtr id[3];
	id[0] = djset.makeSet(0);
	id[1] = djset.makeSet(1);
	id[2] = djset.makeSet(2);

	djset.linkSets(id[0], id[1]);
	djset.linkSets(id[1], id[2]);

	DisjointSet::SetIdentifier t1 = djset.findSet(id[0]);
	DisjointSet::SetIdentifier t2 = djset.findSet(id[2]);
}

