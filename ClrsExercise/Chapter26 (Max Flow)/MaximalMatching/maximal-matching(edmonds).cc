#include <iostream>
// Edmonds algorithm.

class Utility
	{
protected:
	void trace(const char* msg)
		{
		std::cout << msg << ": ";
		}
	void read(const char* msg, int* input)
		{
		trace(msg);
		std::cin >> *input;
		}
	};

class MaximalMatching : private Utility
	{
public:
	void Initialize()
		{
		read("二分图左侧的节点数为", &mLeft);
		read("二分图右侧的节点数为", &mRight);
		read("二者相连的边的数目为", &mCount);
		trace("输入这些边");
		read_adjmatrix();
		}

	int GetMatchingCount()
		{
		int result = 0;
		std::fill(mResult, mResult + _countof(mResult), -1);
		for(int i = 0; i < mLeft; ++i)
			{
			// 清除状态.
			std::fill(mTmpState, mTmpState + _countof(mTmpState), false);
			result += dfs(i);
			}
		return result;
		}
private:
	bool dfs(int leftPos)
		{
		for(int i = 0; i < mRight; ++i)
			{
			// 两点可达,并且i点没有被查找过.
			if(mMatrix[leftPos][i] && !mTmpState[i])
				{
				mTmpState[i] = true;
				// i点未加入到匹配中或者它所匹配的左边的点还能找到匹配.
				if(mResult[i] == -1 || dfs(mResult[i]))
					{
					mResult[i] = leftPos;
					return true;
					}
				}
			}
		return false;
		}
	void read_adjmatrix()
		{
		for(int i = 0; i < mCount; ++i)
			{
			int tmp1, tmp2;
			std::cin >> tmp1 >> tmp2;
			mMatrix[tmp1][tmp2] = true;
			}
		}
	int mLeft, mRight, mCount;
	bool mMatrix[64][64];	// 邻接矩阵.
	bool mTmpState[64];		// 避免递归过程中,一个点被多次访问.
	int mResult[64];		// x = mResult[i], 右边的第i个点与左边的第x个点匹配.
	};

int main()
	{
	MaximalMatching t;
	t.Initialize();
	std::cout << t.GetMatchingCount() << std::endl;
	}