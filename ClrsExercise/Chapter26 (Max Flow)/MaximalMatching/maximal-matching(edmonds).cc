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
		read("����ͼ���Ľڵ���Ϊ", &mLeft);
		read("����ͼ�Ҳ�Ľڵ���Ϊ", &mRight);
		read("���������ıߵ���ĿΪ", &mCount);
		trace("������Щ��");
		read_adjmatrix();
		}

	int GetMatchingCount()
		{
		int result = 0;
		std::fill(mResult, mResult + _countof(mResult), -1);
		for(int i = 0; i < mLeft; ++i)
			{
			// ���״̬.
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
			// ����ɴ�,����i��û�б����ҹ�.
			if(mMatrix[leftPos][i] && !mTmpState[i])
				{
				mTmpState[i] = true;
				// i��δ���뵽ƥ���л�������ƥ�����ߵĵ㻹���ҵ�ƥ��.
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
	bool mMatrix[64][64];	// �ڽӾ���.
	bool mTmpState[64];		// ����ݹ������,һ���㱻��η���.
	int mResult[64];		// x = mResult[i], �ұߵĵ�i��������ߵĵ�x����ƥ��.
	};

int main()
	{
	MaximalMatching t;
	t.Initialize();
	std::cout << t.GetMatchingCount() << std::endl;
	}