using System;

public class ArrayLinkedList<T>
{
	public ArrayLinkedList(int size)
	{
		container = new ListNode[size];
		
		for (int i = 0; i < size; ++i)
		{
			container[i] = new ListNode();
			container[i].index = i;

			if (i < size - 1)
			{
				container[i].nextFree = i + 1;
			}
		}

		freeListHead = 0;
	}

	public int Add(T value)
	{
		int pos = PopFreeList();
		if (linkedListTail == -1)
		{
			linkedListHead = pos;
			container[pos].prev = -1;
		}
		else
		{
			container[linkedListTail].next = pos;
			container[pos].prev = linkedListTail;
		}

		container[pos].value = value;
		linkedListTail = pos;

		return linkedListTail;
	}

	public int Remove(int index)
	{
		int next = container[index].next;

		ListNode node = container[index];
		if (node.prev != -1) { container[node.prev].next = node.next; }
		if (node.next != -1) { container[node.next].prev = node.prev; }
		if (node.index == linkedListHead) { linkedListHead = node.next; }
		if (node.index == linkedListTail) { linkedListTail = node.prev; }

		PushFreeList(node);

		return next;
	}

	public int NextIndex(int current)
	{
		return container[current].next;
	}

	public T this[int index]
	{
		get { return container[index].value; }
	}

	public int First
	{
		get { return linkedListHead; }
	}

	public override string ToString()
	{
		string text = string.Empty;
		for (int index = First; index >= 0; index = NextIndex(index))
		{
			if (!string.IsNullOrEmpty(text)) { text += " "; }
			text += this[index];
		}

		return text;
	}

	int PopFreeList()
	{
		if (freeListHead == -1) { throw new OutOfMemoryException(); }

		int answer = freeListHead;
		freeListHead = container[freeListHead].nextFree;
		container[answer].nextFree = -1;

		return answer;
	}

	void PushFreeList(ListNode node)
	{
		node.value = default(T);
		node.prev = node.next = -1;
		node.nextFree = freeListHead;
		freeListHead = node.index;
	}

	class ListNode
	{
		public T value;
		public int index = -1;
		public int prev = -1;
		public int next = -1;
		public int nextFree = -1;
	}

	ListNode[] container = null;
	int freeListHead = -1;
	int linkedListHead = -1, linkedListTail = -1;
}

public static class Program
{
	static void Main()
	{
		ArrayLinkedList<int> dl = new ArrayLinkedList<int>(5);
		dl.Add(1);
		bool enter = false;
		for (int index = dl.First; index >= 0; )
		{
			Console.WriteLine(dl[index]);
			if (!enter)
			{
				enter = true;
				dl.Remove(index);
				index = dl.Add(3);
				dl.Add(4);
				dl.Add(5);
				dl.Add(6);
				dl.Add(7);
			}
			else
			{
				index = dl.NextIndex(index);
			}
		}
	}
}
