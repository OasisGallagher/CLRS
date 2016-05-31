#include <vector>
#include <algorithm>
#include <iostream>

template <class Value>
class min_heap
{
	typedef Value value_type;
	typedef std::vector<value_type> container_t;
	typedef std::greater<value_type> pred_t;
public:
	void push(const value_type& val)
	{
		cont.push_back(val);
		std::push_heap(cont.begin(), cont.end(), pred_t());
	}
	void pop()
	{
		std::pop_heap(cont.begin(), cont.end(), pred_t());
		cont.pop_back();
	}
	const value_type& min() const
	{
		return cont.at(0);
	}
	bool empty() const
	{
		return cont.empty();
	}
private:
	container_t cont;
};

struct ListNode
{
	ListNode() : mVal(0), mNext(nullptr), mPrev(nullptr) {}
	int mVal;
	ListNode* mNext;
	ListNode* mPrev;
};

ListNode* CreateList(int* arr, int count)
{
	ListNode* head = nullptr, *cur = nullptr;
	for(int i = 0; i < count; ++i)
	{
		ListNode* now = new ListNode;
		
		if(head == nullptr)
			head = cur = now;
		else
		{
			cur->mNext = now;
			now->mPrev = cur;
		}

		now->mVal = arr[i];
		cur = now;
	}
	cur->mNext = nullptr;
	return head;
}

void TraceList(const ListNode* head)
{
	for(; head != nullptr; head = head->mNext)
		std::cout << head->mVal << " ";
	std::cout << std::endl;
}

void DestroyList(ListNode* list)
{
	for(; list != nullptr; )
	{
		ListNode* next = list->mNext;
		delete list;
		list = next;
	}
}

#ifndef _countof
#define _countof(_Arr)	(sizeof(_Arr) / sizeof(_Arr[0]))
#endif
static bool __less_than(int x, int y)
{
	return x < y;
}

typedef bool (*_ListComp)(int, int); 
void SortList(ListNode* list, _ListComp comp = __less_than)
{
	if(list == nullptr)
		return;

	for(ListNode* cur = list->mNext; cur != nullptr; cur = cur->mNext)
	{
		int key = cur->mVal;
		ListNode* prev = cur->mPrev;
		for(; prev != nullptr && prev->mVal > key; prev = prev->mPrev)
		{
			prev->mNext->mVal = prev->mVal;
		}
		if(prev == nullptr)
			list->mVal = key;
		else
			prev->mNext->mVal = key;
	}
}

int CountListElement(const ListNode* head)
{
	int result = 0;
	for(; head != nullptr; head = head->mNext)
		++result;
	return result;
}

struct Pair
{
	Pair(int _Value, int _Index) : mValue(_Value), mIndexOfList(_Index){}
	int mValue;
	int mIndexOfList;
	bool operator > (const Pair& other) const
	{
		return mValue > other.mValue;
	}
};

ListNode* MergeSortedList(ListNode** listarr, int list_count)
{
	int ele_count = 0;
	// O(ele_count).
	for(int i = 0; i < list_count; ele_count += CountListElement(listarr[i++]))
		;

	int* result = new int[ele_count];

	int index = 0;
	min_heap<Pair> heap;

	// O(list_count).
	for(int i = 0; i < list_count; ++i)
	{
		if(listarr[i] != nullptr)
		{
			heap.push(Pair(listarr[i]->mVal, i));
			listarr[i] = listarr[i]->mNext;
		}
	}

	for(; !heap.empty(); )	// ele_count times.
	{
		// O(1).
		Pair min_ele = heap.min();
		
		result[index++] = min_ele.mValue;
		// O(lg(list_count)).
		heap.pop();

		if(listarr[min_ele.mIndexOfList] != nullptr)
		{
			// O(lg(list_count)).
			heap.push(Pair(listarr[min_ele.mIndexOfList]->mVal, min_ele.mIndexOfList));
			listarr[min_ele.mIndexOfList] = listarr[min_ele.mIndexOfList]->mNext;
		}
	}
	// O(ele_count).
	ListNode* newlist = CreateList(result, ele_count);
	delete[] result;
	return newlist;
}

int main()
{
	int arr1[] = { 4, 6, 1, 7, 2, 9, 8 };
	int arr2[] = { 11, 5, 3, 10, 0  };
	int arr3[] = { 14, 12, 16, 15, 13 };
	ListNode* listarray[3];
	listarray[0] = CreateList(arr1, _countof(arr1));
	listarray[1] = CreateList(arr2, _countof(arr2));
	listarray[2] = CreateList(arr3, _countof(arr3));
	for(int i = 0; i < 3; ++i)
	{
		SortList(listarray[i]);
		TraceList(listarray[i]);
	}

	ListNode* merge = MergeSortedList(listarray, 3);
	TraceList(merge);

	for(int i = 0; i < 3; ++i)
		DestroyList(listarray[i]);

	DestroyList(merge);
}