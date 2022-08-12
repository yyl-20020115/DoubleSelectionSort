#include "common_sort.h"
void QuickSort(int data[], int low, int high)
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
		QuickSort(data, low, i - 1);
		QuickSort(data, i + 1, high);
	}
}
void QuickSort(int data[], int n)
{
	QuickSort(data, 0, n - 1);
}
void QuickSort(unsigned int data[], int low, int high)
{
	if (low < high)
	{
		int i = low, j = high;
		unsigned int k = data[i];
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
		QuickSort(data, low, i - 1);
		QuickSort(data, i + 1, high);
	}
}
void QuickSort(unsigned int data[], int n)
{
	QuickSort(data, 0, n - 1);
}
