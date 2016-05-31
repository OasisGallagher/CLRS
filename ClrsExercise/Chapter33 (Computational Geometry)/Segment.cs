struct Segment
{
	public Vector2 left;
	public Vector2 right;
	/// <summary>
	/// 点point在以该线段为对角线, 且边分别平行于坐标轴的矩形中.
	/// </summary>
	bool InRectangle(Vector2 point)
	{
		float minX = Mathf.Min(left.x, right.x), maxX = Mathf.Max(left.x, right.x);
		float minY = Math.Min(left.y, right.y), maxY = Math.Max(left.y, right.y);

		return point.x >= minX && point.x <= maxX && point.y >= minY && point.y <= maxY;
	}

	float Direction(Vector2 point)
	{
		Vector2 d = right - left;
		point -= left;
		return Cross(d, point);
	}

	float Cross(Vector2 lhs, Vector2 rhs)
	{
		return lhs.x * rhs.y - lhs.y * rhs.x;
	}

	public bool IntersectWith(Segment other)
	{
		float f1 = Direction(other.left);
		float f2 = Direction(other.right);
		float f3 = Direction(other.left);
		float f4 = Direction(other.right);

		// 线段x, y的两端点都分居对方两侧.
		if (f1 * f2 < 0f && f3 * f4 < 0f)
			return true;

		if (Mathf.Approximately(f1, 0f) && InRectangle(other.left))
			return true;

		if (Mathf.Approximately(f2, 0f) && InRectangle(other.right))
			return true;

		if (Mathf.Approximately(f3, 0f) && InRectangle(left))
			return true;

		return (Mathf.Approximately(f4, 0f) && InRectangle(right));
	}

	public enum IntersectResult
	{
		Collinear,
		Parallel,
		Intersected,
		NotIntersected,
	}

	/// <summary>
	/// 返回两线段是否相交(如果是, 返回交点).
	/// </summary>
	/// <see cref="http://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect"/>
	IntersectResult IntersectAt(Segment other, out Vector2 ip)
	{
		ip = Vector2.zero;

		Vector2 p = left, r = right - left;
		Vector2 q = other.left, s = other.right - other.left;
		// p, q为二者的左端点, r, s为二者的方向向量.
		// 假设二者有交点, 那么, 対常数x和y, 有:
		// p + rx = q + sy.
		// 为求x, 两边叉乘s, 得:
		// ps + rxs = qs.
		// x = (q-p)s / rs.
		// 同理, pr = qr + ysr
		// y = (p-q)r / sr = (q-p)r / rs, (ab = -ba for cross product)
		// 当rs = 0时, 二者共线或者平行.
		// 注意到v = (p-q)是从线段1左端点到2的左端点的向量, 如果vs或者vr = 0,
		// 那么, 两线段必然共线, 否则二者平行.
		// 如果二者共线, 可以通过比较长度来判断二者是否相交(使用点乘).
		// 如果rs ≠ 0, 表示二者有交点, 交点 = p + rx = q + sy.
		float rs = Cross(r, s);
		float qpCrossr = Cross(q - p, r);
		if (Mathf.Approximately(rs, 0f))
		{
			if (Mathf.Approximately(qpCrossr, 0f))
				return IntersectResult.Collinear;

			return IntersectResult.Parallel;
		}

		float y = qpCrossr / rs;

		if (y >= 0f && y <= 1f)
		{
			ip = q + y * s;
			return IntersectResult.Intersected;
		}

		return IntersectResult.NotIntersected;
	}
}

