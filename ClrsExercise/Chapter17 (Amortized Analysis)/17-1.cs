using System;
using System.Diagnostics;
using System.Collections.Generic;

class Application
{
	static void Main()
	{
		Application p = new Application();
		const int k = 4;
		for (int i = 0, r = 0; i < (1 << 4); ++i)
		{
			Console.WriteLine(p.BinaryExpr(r, k));
			r = p.RevK(p.RevK(r, k) + 1, k);
		}
	}

	// (b)
	class BitReversedCounter
	{
		// 题意中的序列, 0, 8, 4, 12 ...
		// 恰好是RevK(0, 4), RevK(0, 1), RevK(0, 2) ...
		// 按照"Incrementing a binary counter"实现即可.
		// 同时可以证明, 每次操作的平摊代价为O(1), n次的代价为O(n).
	}

	// (c)
	// b中的方法, 如果按照"Incrementing a binary counter"中的做法按照二进制数组
	// 保存的话, 与位运算无关, 位运算的代价不影响该方法的代价.
	// 

	int RevK(int x, int k)	// O(k).
	{
		int result = 0;
		for (int i = 0; i < k; ++i)
			if (((1 << i) & x) != 0)
				result |= (1 << k - i - 1);

		return result;
	}

	void BitReversalPermutation(int k)	// O(kn).
	{
		for (int i = 0; i < (1 << k); ++i)		// O(n).
			Console.WriteLine(BinaryExpr(i, k));	// O(k).
	}

	string BinaryExpr(int x, int k)
	{
		string expr = BinaryExpr(x);
		return new string('0', k - expr.Length) + expr;
	}

	string BinaryExpr(int x)
	{
		return (x > 0)
			? BinaryExpr(x >> 1) + (x & 1) : string.Empty;
	}
}
