/*
see:
http://cs.stackexchange.com/questions/6919/getting-negative-cycle-using-bellman-ford
通过bellmanford算法, 找到在环上的顶点u.
再根据u的Pi值回溯, 直到再遇到u为止.
由于题意中的图只有一个负权值环, 因此, 一定可以返回u.
因为bellmanford算法返回false时找到的环, 一定是负权值的环.
 */
