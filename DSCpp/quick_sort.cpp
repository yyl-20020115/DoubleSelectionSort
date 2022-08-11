#include "common_sort.h"
void QuickSortImpl(int* data, int low, int high)
{
	if (low < high)
	{
		int i = low, j = high;
		int k = data[i];
		while (i < j)
		{
			while (i < j && data[j] > k)
				--j;
			if (i < j)
				data[i++] = data[j];
			while (i < j && data[i] < k)
				++i;
			if (i < j)
				data[j--] = data[i];
		}
		data[i] = k;
		QuickSortImpl(data, low, i - 1);
		QuickSortImpl(data, i + 1, high);
	}
}
void QuickSort(int data[], int n)
{
	QuickSortImpl(data, 0, n - 1);
}
