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
		// ���妵Ϊk*length, ��k���ڵ�ǰ������Ԫ�ظ���.
		// ��(0) = 0, ��ʼ����Ϊ��.
		// ��(Di) >= 0, ������Ԫ�ظ����Ǹ�.
		// �ڽ��е�i������֮������ݽṹ��Di, ��ô, 
		// �����i��������Add, ��ô:
		// c^i = ci + ��(Di) - ��(D(i-1)) = ci + k = k+1.
		// �����i��������DeleteLargerHalf, ����L[i]Ϊ��i��֮��, ������Ԫ�ظ���, ��ô:
		// c^i = ci + ��(Di) - ��(D(i-1)) = ci + k*(L[i] - L[i-1])
		// ���ڵ�i-1������i��, ɾ����ceil(L[i-1]/2)��Ԫ��, ����,
		// c^i = ci - k*ceil(L[i-1]/2) <= ci - k*L[i-1]/2.
		// ͨ��ȡ��λ���ķ���, ��ȡ��������λ��i��Ԫ�ص�Ч��ΪO(n), ����ci = O(n), 
		// ��ci <= d*L[i-1], dΪ����, ��ô:
		// c^i = d*L[i-1] - kL[i-1]/2 = (d-k/2)*L[i-1].
		// ��k = 2d, ��c^i = 0.
		// ���, Add��DeleteLargerHalf�ĸ��Ӷȶ�ΪO(1), ����, 
		// m��һϵ�е�Add��DeleteLargerHalf�����ĸ��Ӷ�ΪO(m).
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
