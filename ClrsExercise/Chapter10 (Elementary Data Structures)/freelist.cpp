// CLRS p125
#include <iostream>
// 用一个链表(mMemList), 之中维护多个内存块.
// 在这些内存块中, 用模拟链表(实际上它的next只是指向这个内存块或者其他内存块的单元的地址, 
// 并不是通常所说的链表)的mFreelist, 用mNextFreeElement记录了空闲的位置.
// 申请一个内存单元, 则取mFreelist的开头节点; 释放一个单元, 则将这个单元插入到
// mFreelist的开头, 供下次使用.
// 省去了遍历内存单元, 寻找空闲结点的操作.

template <class T>
class MemoryPool {
	struct Head {
		struct Element* mNextFreeElement;
	};
	struct Element {
		Head mHeadInfo;
		T mValue;
	};
	struct MemListNode {
		Element* mMemBlock;
		MemListNode* mNextNode;
	};
	static const int HeadSize = sizeof(Head);
public:
	MemoryPool() : mPoolGrowSize(1), mPoolCurSize(mPoolGrowSize), mMemList(NULL), mFreelist(NULL) {
		_Grow(mPoolCurSize);
	}

	~MemoryPool() { 
		while(mMemList != NULL) {
			MemListNode* next = mMemList->mNextNode;
			delete mMemList->mMemBlock;
			delete mMemList;
			mMemList = next;
		}
	}

	// 从FreeList的开头, 取出内存单元. 如果没有空闲内存可用, 会自动增长.
	T* allocate() {
		if(mFreelist == NULL) { 
			_Grow(mPoolGrowSize *= 2);
			mPoolCurSize += mPoolGrowSize;
		}

		T* result = (T*)_Advance(mFreelist, HeadSize);
		mFreelist = mFreelist->mHeadInfo.mNextFreeElement;
		return result;
	}

	// 释放ptr, 将ptr的Element放到FreeList的开头, 即把这个Element置为可用.
	void deallocate(T* ptr) {
		Element* ele = (Element*)_Advance(ptr, -HeadSize);
		ele->mHeadInfo.mNextFreeElement = mFreelist;
		mFreelist = ele;
	}
private:
	// 分配size个空闲位置, 调用此函数时, 旧的内存空间必须都已经被用完.
	void _Grow(size_t size) {
		if(mFreelist != NULL) throw "_Grow is not necessary";

		Element* memory = new Element[mPoolGrowSize];
		for(size_t i = 0; i < mPoolGrowSize; ++i) {
			if(i >= 1)
				memory[i - 1].mHeadInfo.mNextFreeElement = &memory[i];
		}
		memory[mPoolGrowSize - 1].mHeadInfo.mNextFreeElement = NULL;

		MemListNode* mem = new(std::nothrow) MemListNode;
		if(mem == NULL) {
			delete[] memory;
			throw std::bad_alloc();
		}

		mem->mMemBlock = memory;
		mem->mNextNode = NULL;

		if(mMemList == NULL) {
			mMemList = mLastMem = mem;
		}
		else {
			mLastMem->mNextNode = mem;
			mLastMem = mem;
		}

		mFreelist = memory;
	}

	// 将ptr指针移动off距离.
	template <class T>
	void* _Advance(T* ptr, int off) {
		return (char*)ptr + off;
	}
	// 每次_Grow增长mPoolGrowSize大小.
	size_t mPoolGrowSize;
	// 当前Pool的总大小.
	size_t mPoolCurSize;
	
	// 连接各块内存块.
	MemListNode* mMemList;
	// 最后一个内存块, 用来跟下一个新的内存块连接(也可以遍历mMemList从而省略该变量).
	MemListNode* mLastMem;

	// 空闲的位置组成的链表.
	// 每次有新的空闲内存加入, 都会放到该链表的前端, 从而增长该链表.
	// 每次取用时, 从链表的前端开始取, 并更新mFreeList指向下一个单元, 从而缩减链表.
	// 这里的链表是建立在数组或者内存块中的, 由MemList统一负责管理,
	// 因此不必考虑断链等情况, 只需要维护一个空闲单元的链就可以.
	Element* mFreelist;
};

int main()
{
	MemoryPool<int> pool;
	int* p[10240] = { 0 };
	for(int i = 0; i < 10240; ++i) {
		p[i] = pool.allocate();
	}

	for(int i = 0; i < 10240; ++i) {
		if(i % 2 == 0) {
			pool.deallocate(p[i]);
		}
	}

	for(int i = 0; i < 10240 / 2; ++i) {
		if(i % 2 == 0)
			p[i] = pool.allocate();
	}
}
