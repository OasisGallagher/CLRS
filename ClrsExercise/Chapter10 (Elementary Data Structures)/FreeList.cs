using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

class Entry {
	public Entry(uint i) {
		index = i;
	}
	public bool IsSetinal { get { return index == uint.MaxValue; } }

	public bool free = true;

	public readonly uint index;
	public Entry prev = null;
	public Entry next = null;
	public object data = null;
}

class MyEnumerator : IEnumerator
{
	Entry mFirst = null;
	public MyEnumerator(Entry first)
	{
		mFirst = first;
	}

	public bool MoveNext()
	{
		mFirst = mFirst.next;
		return !mFirst.IsSetinal;
	}

	public void Reset()
	{
		mFirst = null;
	}

	public object Current
	{
		get
		{
			return mFirst;
		}
	}
}

class SentineledLinkedList : IEnumerable
{
	Entry mSetinal = new Entry(uint.MaxValue);

	public SentineledLinkedList()
	{
		mSetinal.next = mSetinal.prev = mSetinal;
	}

	public IEnumerator GetEnumerator()
	{
		return new MyEnumerator(mSetinal);
	}

	public Entry Front { get { return mSetinal.next; } }

	public bool IsEmpty { get { return mSetinal.next == mSetinal; } }

	public void PushBack(Entry element)
	{
		mSetinal.prev.next = element;
		element.prev = mSetinal.prev;

		mSetinal.prev = element;
		element.next = mSetinal;
	}

	public void PushFront(Entry element)
	{
		element.next = mSetinal.next;
		mSetinal.next.prev = element;

		mSetinal.next = element;
		element.prev = mSetinal;
	}

	public void Delete(Entry element)
	{
		element.prev.next = element.next;
		element.next.prev = element.prev;
	}
}

class MyList : IEnumerable
{
	uint mSize = 0;

	readonly uint mcExponent = 0;
	List<Entry[]> mContainer = new List<Entry[]>();

	SentineledLinkedList free = new SentineledLinkedList();
	SentineledLinkedList busy = new SentineledLinkedList();

	public MyList(uint exponent) {
		mcExponent = exponent;
	}

	public IEnumerator GetEnumerator()
	{
		return busy.GetEnumerator();
	}

	public uint Put(object data)
	{
		if (free.IsEmpty)
			Grow();

		Entry element = free.Front;
		uint result = element.index;

		element.data = data;
		element.free = false;

		free.Delete(element);
		busy.PushBack(element);

		++mSize;
		return result;
	}

	public void Delete(uint index)
	{
		uint high = High(index);
		Entry[] bucket = mContainer[(int)high];

		Entry entry = bucket[(int)Low(index)];

		if (!entry.free)
		{
			entry.free = true;

			busy.Delete(entry);
			free.PushFront(entry);
			
			--mSize;
		}
#if DEBUG
		else
		{
			throw new Exception("delete " + index + " twice.");
		}
#endif
	}

	public uint Capacity { get { return BucketSize * (uint)mContainer.Count; } }

	public uint Size { get { return mSize; } }

	void Grow()
	{
		uint size = BucketSize;
		uint capacity = size * (uint)mContainer.Count;
		Entry[] buffer = new Entry[size];

		for (uint i = 0; i < size; ++i)
		{
			buffer[i] = new Entry(i + capacity);
			free.PushBack(buffer[i]);
		}

		mContainer.Add(buffer);

		Console.WriteLine("capacity = " + Capacity);
	}

	uint High(uint i) {
		return i >> (int)mcExponent;
	}

	uint Low(uint i) {
		return i & (BucketSize - 1);
	}

	uint BucketSize { get { return 1u << (int)mcExponent; } }
}

class Prog
{
	static void Main()
	{
		MyList mylist = new MyList(2);

		uint index = mylist.Put(1);
		uint index2 = mylist.Put(2);
		//uint index3 = mylist.Put(3);
		uint index4 = mylist.Put(4);
		//uint index5 = mylist.Put(5);

		mylist.Delete(index);
		mylist.Delete(index2);

		mylist.Delete(index4);

		foreach (Entry obj in mylist)
		{
			Console.WriteLine(obj.data);
		}
	}
}
