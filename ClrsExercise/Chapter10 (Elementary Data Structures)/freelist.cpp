// CLRS p125
#include <iostream>
// ��һ������(mMemList), ֮��ά������ڴ��.
// ����Щ�ڴ����, ��ģ������(ʵ��������nextֻ��ָ������ڴ����������ڴ��ĵ�Ԫ�ĵ�ַ, 
// ������ͨ����˵������)��mFreelist, ��mNextFreeElement��¼�˿��е�λ��.
// ����һ���ڴ浥Ԫ, ��ȡmFreelist�Ŀ�ͷ�ڵ�; �ͷ�һ����Ԫ, �������Ԫ���뵽
// mFreelist�Ŀ�ͷ, ���´�ʹ��.
// ʡȥ�˱����ڴ浥Ԫ, Ѱ�ҿ��н��Ĳ���.

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

	// ��FreeList�Ŀ�ͷ, ȡ���ڴ浥Ԫ. ���û�п����ڴ����, ���Զ�����.
	T* allocate() {
		if(mFreelist == NULL) { 
			_Grow(mPoolGrowSize *= 2);
			mPoolCurSize += mPoolGrowSize;
		}

		T* result = (T*)_Advance(mFreelist, HeadSize);
		mFreelist = mFreelist->mHeadInfo.mNextFreeElement;
		return result;
	}

	// �ͷ�ptr, ��ptr��Element�ŵ�FreeList�Ŀ�ͷ, �������Element��Ϊ����.
	void deallocate(T* ptr) {
		Element* ele = (Element*)_Advance(ptr, -HeadSize);
		ele->mHeadInfo.mNextFreeElement = mFreelist;
		mFreelist = ele;
	}
private:
	// ����size������λ��, ���ô˺���ʱ, �ɵ��ڴ�ռ���붼�Ѿ�������.
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

	// ��ptrָ���ƶ�off����.
	template <class T>
	void* _Advance(T* ptr, int off) {
		return (char*)ptr + off;
	}
	// ÿ��_Grow����mPoolGrowSize��С.
	size_t mPoolGrowSize;
	// ��ǰPool���ܴ�С.
	size_t mPoolCurSize;
	
	// ���Ӹ����ڴ��.
	MemListNode* mMemList;
	// ���һ���ڴ��, ��������һ���µ��ڴ������(Ҳ���Ա���mMemList�Ӷ�ʡ�Ըñ���).
	MemListNode* mLastMem;

	// ���е�λ����ɵ�����.
	// ÿ�����µĿ����ڴ����, ����ŵ��������ǰ��, �Ӷ�����������.
	// ÿ��ȡ��ʱ, �������ǰ�˿�ʼȡ, ������mFreeListָ����һ����Ԫ, �Ӷ���������.
	// ����������ǽ�������������ڴ���е�, ��MemListͳһ�������,
	// ��˲��ؿ��Ƕ��������, ֻ��Ҫά��һ�����е�Ԫ�����Ϳ���.
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
