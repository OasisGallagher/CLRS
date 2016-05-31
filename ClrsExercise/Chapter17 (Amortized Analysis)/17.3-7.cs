using System;
using System.Diagnostics;
using System.Collections.Generic;
using Vector = System.Collections.Generic.List<int>;

class Application
{
	static void Main()
	{
		Application p = new Application();

		p.Add(3, 5, 1, 7);
		p.DeleteLargerHalf();

		p.Add(2);
		p.DeleteLargerHalf();

		p.DeleteLargerHalf();

		// Complexity:
		// 定义Φ为k*length, 即k倍于当前容器的元素个数.
		// Φ(0) = 0, 初始容器为空.
		// Φ(Di) >= 0, 容器的元素个数非负.
		// 在进行第i个操作之后的数据结构是Di, 那么, 
		// 如果第i个操作是Add, 那么:
		// c^i = ci + Φ(Di) - Φ(D(i-1)) = ci + k = k+1.
		// 如果第i个操作是DeleteLargerHalf, 定义L[i]为第i步之后, 容器的元素个数, 那么:
		// c^i = ci + Φ(Di) - Φ(D(i-1)) = ci + k*(L[i] - L[i-1])
		// 由于第i-1步到第i步, 删除了ceil(L[i-1]/2)个元素, 所以,
		// c^i = ci - k*ceil(L[i-1]/2) <= ci - k*L[i-1]/2.
		// 通过取中位数的方法, 获取数组中排位第i的元素的效率为O(n), 所以ci = O(n), 
		// 令ci <= d*L[i-1], d为常数, 那么:
		// c^i = d*L[i-1] - kL[i-1]/2 = (d-k/2)*L[i-1].
		// 令k = 2d, 得c^i = 0.
		// 因此, Add和DeleteLargerHalf的复杂度都为O(1), 进而, 
		// m个一系列的Add和DeleteLargerHalf操作的复杂度为O(m).
	}

	Vector container = new Vector();

	public void Add(params int[] array)
	{
		container.AddRange(array);
	}

	public void DeleteLargerHalf()
	{
		int pos = PartitionLargerHalf(container);
		container.RemoveRange(pos, container.Count - pos);
	}

	int PartitionLargerHalf(Vector container)
	{
		int half = (int)Math.Ceiling(container.Count / 2f);
		half = container.Count - half;
		NthElement(container, half);
		return half;
	}

	void NthElement(Vector container, int n)
	{
		int first = 0, last = container.Count;
		for (int pos = 0; (pos = Partition(container, first, last)) != n ; )
		{
			if (pos > n)
				last = pos;
			else
			{
				first = pos + 1;
				n = n - pos - 1;
			}
		}
	}

	int Partition(Vector container, int first, int last)
	{
		int tail = container[last - 1];
		int i = -1;
		for (int j = 0; j < last -1; ++j)
		{
			if (container[j] < tail && ++i != j)
			{
				Swap(container, i, j);
			}
		}

		Swap(container, ++i, last - 1);
		return i - first;
	}

	void Swap(Vector container, int i, int j)
	{
		int tmp = container[i];
		container[i] = container[j];
		container[j] = tmp;
	}
}
