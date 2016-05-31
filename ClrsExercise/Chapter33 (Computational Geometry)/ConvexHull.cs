using System;
using System.Collections;
using System.Collections.Generic;

public static class ConvexHullComputer
{
	class VertexStack
	{
		List<Vertex> container = new List<Vertex>();
		public Vertex Pop()
		{
			Vertex result = container[container.Count - 1];
			container.RemoveAt(container.Count - 1);
			return result;
		}

		public void Push(Vertex u)
		{
			container.Add(u);
		}

		public Vertex Peek(int index = 0)
		{
			return container[container.Count - 1 - index];
		}

		public List<Vertex> Container { get { return new List<Vertex>(container); } }
	}

	static Vertex PopLowestVertex(List<Vertex> vertices)
	{
		float minZ = vertices[0].transform.position.z;
		int index = 0;
		for (int i = 1; i < vertices.Count; ++i)
		{
			if (vertices[i].transform.position.z < minZ)
			{
				index = i;
				minZ = vertices[i].transform.position.z;
			}
		}

		Vertex result = vertices[index];
		vertices[index] = vertices[vertices.Count - 1];
		vertices.RemoveAt(vertices.Count - 1);
		return result;
	}

	class VertexComparer : IComparer<Vertex>
	{
		Vertex start = null;
		public VertexComparer(Vertex o)
		{
			start = o;
		}

		int IComparer<Vertex>.Compare(Vertex lhs, Vertex rhs)
		{
			bool b1 = Utility.Cross2D(lhs.transform.position - start.transform.position, new Vector3(1, 0, 0)) > 0;
			bool b2 = Utility.Cross2D(rhs.transform.position - start.transform.position, new Vector3(1, 0, 0)) > 0;

			if (b1 != b2) { return b2 ? -1 : 1; }

			float c = Utility.Cross2D(lhs.transform.position - start.transform.position, rhs.transform.position - start.transform.position);
			if (!Mathf.Approximately(c, 0)) { return c > 0 ? -1 : 1; }

			// 对于极角相等的两个点, 按照到start的距离递减的顺序排列.
			// 从而在Unique中删除.
			Vector3 drhs = rhs.transform.position - start.transform.position;
			Vector3 dlhs = lhs.transform.position - start.transform.position;
			drhs.y = dlhs.y = 0f;

			return Math.Sign(drhs.sqrMagnitude - dlhs.sqrMagnitude);
		}
	}

	public static List<Vertex> Compute(List<Vertex> vertices)
	{
		if (vertices.Count <= 3) { return vertices; }

		Vertex p0 = PopLowestVertex(vertices);
		vertices.Sort(new VertexComparer(p0));

		VertexStack stack = new VertexStack();
		stack.Push(p0);
		stack.Push(vertices[0]);
		stack.Push(vertices[1]);

		for (int i = 2; i < vertices.Count; ++i)
		{
			Vertex pi = vertices[i];
			for (; ; )
			{
				System.Diagnostics.Debug.Assert(stack.Container.Count > 0);

				Vertex top = stack.Peek(), next2top = stack.Peek(1);
				// next2top -> top -> pi is a non-left turn.
				float cr = Utility.Cross2D(
					next2top.transform.position - top.transform.position,
					pi.transform.position - top.transform.position
				);

				if (cr <= 0) { break; }

				stack.Pop();
			}

			stack.Push(pi);
		}

		return stack.Container;
	}
}
