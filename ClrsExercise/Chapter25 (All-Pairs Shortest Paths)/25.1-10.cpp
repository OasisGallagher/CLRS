//
// 在第m次进行EXTEND之后, 检查结果矩阵的对角线.
// 如果对角线中存在负值, 那么表示存在负权, 且长度为m.
// 当开始进行第m次EXTEND时, 表示m-1条边组成的路径不存在负环.
// 所以, 当第m次EXTEND之后, 发现负环, 那么, 环的长度为m.
// time: O(n^4).
// see Instructor's manual for a O(log(n)n^3) algorithm.
//
