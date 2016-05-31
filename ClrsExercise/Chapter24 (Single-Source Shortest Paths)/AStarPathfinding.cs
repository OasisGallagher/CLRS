using System;
using System.Collections;
using System.Collections.Generic;
using System.Diagnostics;

class Node
{
	public float G;
	/// <summary>
	/// 该点到终点的最短距离的估计值(该值不可以比实际的最小值大, 否则会导致"过高估计",
	/// 而无法找到正确的最小值.
	/// 如果该值为0, A*就成为了Dijkstra算法.
	/// </summary>
	/// <seealso cref="http://en.wikipedia.org/wiki/A*_search_algorithm"/>
	public float H;
	public float F { get { return G + H; } }

	public enum CellType : int
	{
		Walkable,
		Unwalkable,
	}

	public Node(int row, int col, CellType type)
	{
		this.row = row;
		this.col = col;
		this.type = type;
	}
	
	public readonly int row, col;

	public readonly CellType type;

	public Node parent = null;

	public int index = -1;
}

class Heap
{
	public void Push(Node node)
	{
		node.index = container.Count;
		container.Add(node);

		AdjustAt(node);

		Debug.Assert(IsHeap());
	}

	public int Count { get { return container.Count;} }

	bool IsHeap()
	{
#if DEBUG
		for (int i = 1; i < container.Count; ++i)
			if (container[Parent(i)].F > container[i].F)
				return false;
#endif
		return true;
	}

	public Node Pop()
	{
		Swap(0, container.Count - 1);
		Node result = container[container.Count - 1];
		container.RemoveAt(container.Count - 1);

		int current = 0;
		for (; ; )
		{
			int min = current;
			int lchild = LeftChild(min), rchild = RightChild(min);
			if (lchild < container.Count && container[lchild].F < container[min].F)
				min = lchild;

			if (rchild < container.Count && container[rchild].F < container[min].F)
				min = rchild;

			if (min == current)
				break;

			Swap(min, current);

			current = min;
		}

		result.index = -1;

		Debug.Assert(IsHeap());
		return result;
	}

	public void DecreaseG(Node node, float value)
	{
		if (node.G <= value)
			throw new ArgumentException();

		node.G = value;

		AdjustAt(node);

		Debug.Assert(IsHeap());
	}

	void AdjustAt(Node node)
	{
		for (int parent = Parent(node.index); parent >= 0 && node.F < container[parent].F; parent = Parent(parent))
		{
			Swap(node.index, parent);
		}
	}

	public bool Contains(Node node)
	{
		return node.index != -1;
	}

	void Swap(int i, int j)
	{
		Node tmp = container[i];
		container[i] = container[j];
		container[j] = tmp;

		container[i].index = i;
		container[j].index = j;
	}

	int Parent(int i)
	{
		return (i - 1) / 2;
	}

	int LeftChild(int i)
	{
		return 2 * i + 1;
	}

	int RightChild(int i)
	{
		return 2 * i + 2;
	}
	
	List<Node> container = new List<Node>();
}

class Map
{
	// http://www.policyalmanac.org/games/aStarTutorial.htm
	public Map()
	{
		for (int i = 0; i < 8; ++i)
		{
			container.Add(new List<Node>());
			for (int j = 0; j < 8; ++j)
			{
				Node.CellType type = Node.CellType.Walkable;
				if (i >= 1 && i <= 3 && j == 4)
					type = Node.CellType.Unwalkable;

				Node node = new Node(i, j, type);
				container[i].Add(node);
			}
		}
	}

	public List<Node> FindPath(Tuple<int, int> startPosition, Tuple<int, int> destPosition)
	{
		return FindPath(container[startPosition.Item1][startPosition.Item2],
			container[destPosition.Item1][destPosition.Item2]);
	}

	List<Node> FindPath(Node start, Node dest)
	{
		Heap open = new Heap();
		HashSet<Node> close = new HashSet<Node>();

		for (int i = 0; i < RowCount; ++i)
		{
			for (int j = 0; j < ColCount; ++j)
			{
				container[i][j].H = (Math.Abs(dest.row - i) + Math.Abs(dest.col - j)) * 10;
			}
		}

		start.G = 0;

		open.Push(start);

		for (; open.Count != 0; )
		{
			start = open.Pop();

			if (start == dest)
				return BuildPath(dest);

			foreach (Node current in AdjacencyList(start))
			{
				if (current.type == Node.CellType.Unwalkable || close.Contains(current))
					continue;

				if (!open.Contains(current))
				{
					current.G = float.PositiveInfinity;
					open.Push(current);
				}

				float tmp = start.G + W(start, current);
				if (tmp < current.G)
				{
					open.DecreaseG(current, tmp);
					current.parent = start;
				}
			}

			close.Add(start);
		}

		// no path is found.
		return null;
	}

	List<Node> BuildPath(Node dest)
	{
		List<Node> result = new List<Node>();
		if (dest.parent != null)
		{
			result.AddRange(BuildPath(dest.parent));
		}

		result.Add(dest);

		return result;
	}

	/// <summary>
	/// 获取node的邻接表.
	/// </summary>
	List<Node> AdjacencyList(Node node)
	{
		List<Node> result = new List<Node>();
		if (node.row != 0 && node.col != 0)
			result.Add(container[node.row - 1][node.col - 1]);

		if (node.row != 0)
			result.Add(container[node.row - 1][node.col]);

		if (node.row != 0 && node.col + 1 < ColCount)
			result.Add(container[node.row - 1][node.col + 1]);

		if (node.col != 0)
			result.Add(container[node.row][node.col - 1]);

		if (node.col + 1 < ColCount)
			result.Add(container[node.row][node.col + 1]);

		if (node.row + 1 < RowCount && node.col != 0)
			result.Add(container[node.row + 1][node.col - 1]);

		if (node.row + 1 < RowCount)
			result.Add(container[node.row + 1][node.col]);

		if (node.row + 1 < RowCount && node.col + 1 < ColCount)
			result.Add(container[node.row + 1][node.col + 1]);

		return result;
	}

	/// <summary>
	/// 获取连接u和v的边的权重.
	/// </summary>
	float W(Node u, Node v)
	{
		if (u.row == v.row && u.col == v.col)
			return 0;

		if ((u.row == v.row || u.col == v.col) && (Math.Abs(u.row - v.row) <= 1 || Math.Abs(u.col - v.col) <= 1))
			return 10;

		if (Math.Abs(u.row - v.row) == 1 && Math.Abs(u.col - v.col) == 1)
			return 14;

		return float.PositiveInfinity;
	}

	int RowCount { get { return container.Count; } }
	int ColCount { get { return container.Count == 0 ? 0 : container[0].Count; } }

	List<List<Node>> container = new List<List<Node>>();
}

class Program
{
	static void Main(string[] args)
	{
		Map map = new Map();
		List<Node> path = map.FindPath(new Tuple<int, int>(2, 2), new Tuple<int, int>(2, 6));

		foreach (Node node in path)
		{
			Console.WriteLine("({0}, {1})", node.row, node.col);
		}
	}
}

