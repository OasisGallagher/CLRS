// 如果p1被pop, 那么, 必然存在一个点q, 使得p0, p1, q存在右转, 即q相对于p0的
// 极角大于p1相对于p0的, 与p1是相对于p0的极角最小的点矛盾.
// 而pm, 在GRAHAM-SCAN的第9行, 会始终将pm加入stack.
// 