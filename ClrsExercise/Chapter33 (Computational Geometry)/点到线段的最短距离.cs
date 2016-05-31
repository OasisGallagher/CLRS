float MinDistanceFromPointToSegment(Vector2 point, Vector2 leftEndPoint, Vector2 rightEndPoint)
{
	// 从线段的左端点到右端点的向量.
	Vector2 ray = rightEndPoint - leftEndPoint;
	// V = 左端点到point的向量, 点乘ray, 得到|V| * |ray| * cos(α), α是二者的夹角.
	// 再除以|ray|^2, 得到|V|*cos(α) / |ray|, 即, V投影到ray上的比例.
	float ratio = Vector2.Dot(point - leftEndPoint, ray) / ray.sqrMagnitude;

	// 比例小于0, 表示点到ray的垂足在ray的反向延长线上.
	if (ratio < 0f)
		ratio = 0f;
	// 如果大于1, 表示点到ray的垂足在ray的延长线上.
	else if (ratio > 1f)
		ratio = 1f;
	// 否则, 在ray上.

	// leftEndPoint + ratio * ray:
	// 如果ratio = 0, 垂足在ray的反向延长线上, 到ray的最短距离为到左端点的距离.
	// 如果ratio = 1, 垂足在ray的延长线上, 到ray的最短距离为到右端点的距离.
	// 否则, 得到垂足的位置.
	// 再计算两点的距离即可.
	return (leftEndPoint + ratio * ray - point).magnitude;
}

