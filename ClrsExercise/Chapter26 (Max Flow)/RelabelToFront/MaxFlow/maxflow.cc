// CLRS P.422
// 最大流的重标记与前移算法.
#include <vector>
#include <deque>
#include <map>
#include <cstdarg>
#include <algorithm>
#include <iostream>

typedef int VIndex;
struct Vertex;
typedef std::vector<Vertex*> VArray;
typedef std::deque<Vertex*> Deque;

Vertex* next(Deque* deque, Vertex* cur)
{
	Deque::iterator ite = std::find(deque->begin(), deque->end(), cur);
	return (++ite != deque->end()) ? *ite : nullptr;
}

struct AdjTable
{
	typedef Deque::iterator Iterator;
	Iterator begin() { return mAdjTable.begin(); }
	Iterator end() { return mAdjTable.end(); }
	Vertex* front() { return mAdjTable.front(); }
	void push_back(Vertex* v) { mAdjTable.push_back(v); }	
	Vertex* next() { return ::next(&mAdjTable, mCurrent); }
	Vertex* operator[](unsigned index) { return mAdjTable[index]; }

	std::deque<Vertex*> mAdjTable;
	Vertex* mCurrent;
};

typedef std::map<int, int> Hash;
#define V_COUNT	5

Hash gHeight;
VArray gVertexArray;
Hash gCapacity;
Hash gCurFlow;
Hash gExceedFlow;

struct Vertex
{
	Vertex(int i) : mIndex(i){}
	VIndex mIndex;
	AdjTable mAdjTable;
};

inline int make_key(VIndex x, VIndex y)
{
	return ((x << 16) | (y & 0xFFFF));
}

inline short part(int val, bool high)
{
	return high ?  short((val >> 16) & 0xFFFF) :  short(val & 0xFFFF);
}

inline int residual_capcacity(VIndex u, VIndex v)
{
	return gCapacity[make_key(u, v)] - gCurFlow[make_key(u, v)];
}

inline int exceed_flow(VIndex u)
{
	return gExceedFlow[u];	
}

inline AdjTable& get_adjtable(int i)
{
	return gVertexArray[i]->mAdjTable;
}

inline int height(VIndex u)
{
	return gHeight[u];
}

inline bool overflow(int u)
{
	return exceed_flow(u) > 0;
}

inline bool check_push(VIndex u, VIndex v)
{
	return overflow(u) && (residual_capcacity(u, v) > 0) && (height(u) == height(v) + 1);
}

inline void do_push(VIndex u, VIndex v)
{
	int minValue = std::min(exceed_flow(u), residual_capcacity(u, v));
	gCurFlow[make_key(u, v)] += minValue;
	gCurFlow[make_key(v, u)] = -gCurFlow[make_key(u, v)];
	gExceedFlow[u] -= minValue;
	gExceedFlow[v] += minValue;
}

void init_preflow(int* edge, int count)
{
	for(int i = 0; i < V_COUNT; ++i)
	{
		gHeight[i] = 0;
		gExceedFlow[i] = 0;
	}

	for(int i = 0; i < count; ++i)
	{
		gCurFlow[edge[i]] = 0;
		gCurFlow[short(edge[i] << 16) | short(edge[i] >> 16)] = 0;
	}

	gHeight[0] = V_COUNT;

	AdjTable& adj = get_adjtable(0);
	for(AdjTable::Iterator ite = adj.begin(); ite != adj.end(); ++ite)
	{
		int index = (*ite)->mIndex;
		gCurFlow[make_key(0, index)] = gCapacity[make_key(0, index)];
		gCurFlow[make_key(index, 0)] = -gCapacity[make_key(0, index)];
		gExceedFlow[index] = gCapacity[make_key(0, index)];
		gExceedFlow[0] -= gCapacity[make_key(0, index)];
	}
}

inline void push(VIndex u, VIndex v)
{
	if(check_push(u, v))
		do_push(u, v);
}

bool check_relable(int u)
{
	if(gExceedFlow[u] <= 0) return false;
	AdjTable& adj = get_adjtable(u);
	for(AdjTable::Iterator ite = adj.begin(); ite != adj.end(); ++ite)
	{
		int index = (*ite)->mIndex;
		if(gHeight[u] <= gHeight[index])
			return true;
	}
	return false;
}

int do_relable(int u, int curHeight = 0)
{	// return current height.
	AdjTable& adj = get_adjtable(u);
	int minHeight = INT_MAX;
	// 返回的高度一定要比当前u节点的高度高, 否则, 就会出现高度不增加, 
	// 无法"流动",而其他的低的定点，又没有容量的死循环.
	for(AdjTable::Iterator ite = adj.begin(); ite != adj.end(); ++ite)
		minHeight = std::min(minHeight, gHeight[(*ite)->mIndex]);
	return gHeight[u] = std::max(curHeight + 1, minHeight + 1);
}

inline void relable(int u)
{
	if(check_relable(u))
		do_relable(u);
}

void discharge(int u)
{
	AdjTable& adj = get_adjtable(u);
	adj.mCurrent = adj.front();
	int curHeight = 0;
	while(gExceedFlow[u] > 0)
	{
		if(adj.mCurrent == NULL)
		{
			curHeight = do_relable(u, curHeight);
			adj.mCurrent = adj.front();
		}
		else if(check_push(u, adj.mCurrent->mIndex))
			do_push(u, adj.mCurrent->mIndex);
		else
			adj.mCurrent = adj.next();
	}
}

void init_adj(AdjTable* adj, int node)
{
	std::vector<Vertex*> tmp;
	switch(node)
	{
	case 0:
		tmp.push_back(gVertexArray[1]);
		tmp.push_back(gVertexArray[2]);
		break;
	case 1:
		tmp.push_back(gVertexArray[0]);
		tmp.push_back(gVertexArray[2]);
		tmp.push_back(gVertexArray[3]);
		tmp.push_back(gVertexArray[4]);
		break;
	case 2:
		tmp.push_back(gVertexArray[0]);
		tmp.push_back(gVertexArray[1]);
		tmp.push_back(gVertexArray[3]);
		break;
	case 3:
		tmp.push_back(gVertexArray[1]);
		tmp.push_back(gVertexArray[2]);
		tmp.push_back(gVertexArray[4]);
		break;
	case 4:
		tmp.push_back(gVertexArray[1]);
		tmp.push_back(gVertexArray[3]);
		break;
	default:
		break;
	}

	for(std::vector<Vertex*>::iterator ite = tmp.begin(); ite != tmp.end(); ++ite)
		adj->push_back(*ite);
}

void init_capacity_helper(int u, int v, int goAhead = -1, int goBack = -1)
{
	gCapacity[make_key(u, v)] = goAhead == -1 ? 0 : goAhead;
	gCapacity[make_key(v, u)] = goBack == -1 ? 0 : goBack;
}

void init_capacity()
{
	for(int i = 0; i < V_COUNT; ++i)
	{
		for(int j = 0; j < V_COUNT; ++j)
		{
			if(i != j)
				init_capacity_helper(i, j);
	
		}
	}
	init_capacity_helper(0, 1, 12);
	init_capacity_helper(0, 2, 14);
	init_capacity_helper(1, 2, 5);
	init_capacity_helper(1, 4, 16);
	init_capacity_helper(2, 3, 8);
	init_capacity_helper(3, 1, 7);
	init_capacity_helper(3, 4, 10);
}

void GetMaxFlow()
{
	for(int i = 0; i < V_COUNT; ++i)
		gVertexArray.push_back(new Vertex(i));

	int edges[] = {
		make_key(0, 1), make_key(0, 2), make_key(1, 2),
		make_key(1, 4), make_key(2, 3), make_key(3, 1), make_key(3, 4) 
	};
	init_capacity();

	std::deque<Vertex*> deque;
	for(int i = 0; i < V_COUNT; ++i)
	{
		Vertex* tmp = gVertexArray[i];
		tmp->mIndex = i;
		init_adj(&tmp->mAdjTable, i);
		tmp->mAdjTable.mCurrent = tmp->mAdjTable.front();
		if(i != 0 && i != V_COUNT - 1)
			deque.push_back(tmp);
	}

	init_preflow(edges, _countof(edges));

	for(Vertex* u = deque.front(); u != nullptr; )
	{
		int oldHeight = gHeight[u->mIndex];

		discharge(u->mIndex);
		if(gHeight[u->mIndex] > oldHeight)
		{
			deque.erase(std::remove(deque.begin(), deque.end(), u));
			deque.push_front(u);
		}
		u = next(&deque, u);
	}
}

void trace()
{
	for(Hash::iterator ite = gCurFlow.begin(); ite != gCurFlow.end(); ++ite)
	{
		int val = ite->second;
		if(val != INT_MAX && val > 0 && part(ite->first, true) != part(ite->first, false))
			std::cout << part(ite->first, true) << ", " << part(ite->first, false) << ": " << val << std::endl;
	}
}

int main()
{
	GetMaxFlow();
	trace();
	for(VArray::iterator ite = gVertexArray.begin(); ite != gVertexArray.end(); ++ite)
		delete *ite;
}
