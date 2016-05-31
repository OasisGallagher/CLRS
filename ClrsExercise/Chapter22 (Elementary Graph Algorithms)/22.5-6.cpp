//
// 计算图G的SCC.
// 再简化G的SCC, 从而得到所求的G'.
// 对于由n个点组成的强连通分量, 所含的边的数目至少为n, 即:
// (v1, v2), (v2, v3) ... (v_(n-1), v_n), (v_n, v1).
// 去掉SCC中的所有边, 加入上述边即完成了对该SCC的简化.
//
// 对于SCC之间的边, 如果C1和C2有边联通, 那么, 去掉二者之间的所有边, 再加入
// 一条C1到C2的边, 即完成了SCC之间的简化.
//