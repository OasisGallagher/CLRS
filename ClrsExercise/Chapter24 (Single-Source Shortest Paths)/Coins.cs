using System;
using System.Collections;
using System.Collections.Generic;
using Dict = System.Collections.Generic.Dictionary<System.Tuple<int, int>, float>;

class Application
{
	enum CellType : int
	{
		Invalid = -1,
		Block,
		Walkable,
		Start,
		Dest,
		Coin,	// x >= Coin的类型表示该格子是金币, 且金币的索引为(x - Coin).
	}

	class Vertex
	{
		// 节点的位置(行, 列, 从0开始).
		public int row, col;
		// 当前收集到的金币, 用二进制表示(如: 101表示第0#和2#金币已经收集到).
		public int collection = 0;
		// 节点的类型.
		public CellType type = CellType.Invalid;
		// 节点的最短路径估计值.
		public float d = float.PositiveInfinity;
		// 最短路径时, 该点的父节点.
		public int parent = -1;

		public string Position { get { return "(" + row + ", " + col + ", " + collection + ")"; } }
	}

	/// <summary>
	/// 读取path指定的文件, 格式化, 为Vertex.
	/// </summary>
	/// <param name="coinCount">金币的个数</param>
	static List<Vertex> ReadFile(string path, out int coinCount)
	{
		string[] lines = System.IO.File.ReadAllLines(path);
		List<Vertex> vertices = new List<Vertex>();

		coinCount = 0;

		for (int row = 0; row < lines.Length; ++row)
		{
			string item = lines[row];
			for (int col = 0; col < item.Length; ++col)
			{
				CellType type = CellType.Invalid;
				switch (item[col])
				{
					case '@':
						type = (CellType)((int)CellType.Coin + (coinCount++));
						break;
					case '.':
						type = CellType.Walkable;
						break;
					case 'S':
						type = CellType.Start;
						break;
					case 'G':
						type = CellType.Dest;
						break;
					case '#':
						type = CellType.Block;
						break;
					default:
						throw new System.ArgumentException("invalid char");
				}

				Vertex u = new Vertex();
				u.row = row;
				u.col = col;
				u.type = type;

				vertices.Add(u);
			}
		}

		return vertices;
	}

	/// <summary>
	/// 对从文件格式化之后的vertex进行处理, 将每个节点分裂为2^coinCount个节点.
	/// </summary>
	static List<Vertex> PrepareVertices(List<Vertex> vertices, int coinCount)
	{
		List<Vertex> result = new List<Vertex>();

		for (int i = 0; i < vertices.Count; ++i)
		{
			Vertex u = vertices[i];
			for (int j = 0; j < (1 << coinCount); ++j)
			{
				Vertex v = new Vertex();
				v.row = u.row;
				v.col = u.col;
				v.collection = j;
				v.type = u.type;
				v.d = float.PositiveInfinity;

				if (j == 0 && v.type == CellType.Start)
					v.d = 0;

				result.Add(v);
			}
		}

		return result;
	}

	/// <summary>
	/// 为分裂后的新图添加边, 返回边的权重.
	/// </summary>
	static Dict PrepareEdges(List<Vertex> vertices)
	{
		Dict w = new Dict();
		for (int i = 0; i < vertices.Count; ++i)
		{
			Vertex u = vertices[i];
			for (int j = 0; j < vertices.Count; ++j)
			{
				Vertex v = vertices[j];

				float distance = float.PositiveInfinity;

				if (i == j)
					distance = 0;
				else if (u.type != CellType.Block && v.type != CellType.Block
					&& ((u.row == v.row && Math.Abs(u.col - v.col) <= 1) || (u.col == v.col && Math.Abs(u.row - v.row) <= 1)))
				{
					if (v.type >= CellType.Coin && (u.collection | (1 << (v.type - CellType.Coin))) == v.collection)
						distance = 1;
					else if (u.collection == v.collection)
						distance = 1;
				}

				if (!float.IsPositiveInfinity(distance))
					w.Add(new Tuple<int, int>(i, j), distance);
			}
		}

		return w;
	}

	static void PrintPath(List<Vertex> vertices, int start, int dest)
	{
		Vertex u = null;
		if (start == dest)
			u = vertices[dest];
		else if (vertices[dest].parent >= 0)
		{
			u = vertices[dest];
			PrintPath(vertices, start, vertices[dest].parent);
		}

		if(u != null)
			Console.WriteLine(u.Position);
	}

	static bool BellmanFord(List<Vertex> vertices, Dict w)
	{
		for (int i = vertices.Count; i > 1; --i)
		{
			foreach (Tuple<int, int> key in w.Keys)
			{
				Vertex u = vertices[key.Item1], v = vertices[key.Item2];
				float tmp = u.d + w[key];
				if (tmp < v.d)
				{
					v.d = tmp;
					v.parent = key.Item1;
				}
			}
		}

		// 不可能存在负权环, 永远返回true.
		return true;
	}

	static void Main()
	{
		int coinCount = 0;
		List<Vertex> vertices = ReadFile("coins.txt", out coinCount);
		vertices = PrepareVertices(vertices, coinCount);

		Dict w = PrepareEdges(vertices);

		BellmanFord(vertices, w);
		
		int start = vertices.FindIndex(x => x.type == CellType.Start && x.collection == 0);
		int dest = vertices.FindIndex(x => x.type == CellType.Dest && x.collection == ((1 << coinCount) - 1));

		PrintPath(vertices, start, dest);

		Console.WriteLine("total length = " + vertices[dest].d);
	}
}

