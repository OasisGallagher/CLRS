#include <iostream>
// Use bubbleSortWithFlag, you may gain something.
//
// use a flag to indicate the swap op.
// if no swap occurs, this algorithm costs O(n) with flag == 0(the sequence is
// pure ascending).
// otherwise, it costs O(n^2).
void bubbleSortWithFlag(int arr[], int size)
	{
	for(int flag = size - 1; flag != 0; )
		{
		int bound = flag;
		flag = 0;	// no swap op occurs.
		for(int i = 0; i < bound; ++i)
			{
			if(arr[i] > arr[i + 1])
				{
				int temp = arr[i];
				arr[i] = arr[i + 1];
				arr[i + 1] = temp;
				flag = i + 1;		// the last swap op occurs here.
				//arr[i + 1] must be the largest element.
				}
			}
		}
	}
// normal bubble sort.
void bubbleSort(int arr[], int size)
	{
	for(int i = 0; i < size; ++i)
		for(int j = i + 1; j < size; ++j)
			{
			if(arr[i] > arr[j])
				{
				int temp = arr[i];
				arr[i] = arr[j];
				arr[j] = temp;
				}
			}
	}

void trace(int *arr, int count)
	{
	for(int i = 0; i < count; ++i)
		std::cout << arr[i] << " ";
	std::cout << std::endl;
	}

int main()
	{
	/*int arr[] = {3, 2, 1, 4, 5, 6};*/	// the two algorithms above are the same.
	int arr[] = {1, 2, 3, 4, 5, 6};		// better use bubbleSortWithFlag.
	trace(arr, _countof(arr));

	/*bubbleSort(arr, _countof(arr));*/
	bubbleSortWithFlag(arr, _countof(arr));
	trace(arr, _countof(arr));
	}

