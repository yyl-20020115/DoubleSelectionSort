#include "utils.h"
void InsertionSort(int data[], int n)
{
	for (int i = 1; i < n; i++)
	{
		for (int j = i; j > 0 && data[j] < data[j - 1]; j--)
		{
			Swap(data, j, j - 1);
		}
	}
}
void InsertionSort(unsigned int data[], int n)
{
	for (int i = 1; i < n; i++)
	{
		for (int j = i; j > 0 && data[j] < data[j - 1]; j--)
		{
			Swap(data, j, j - 1);
		}
	}
}
