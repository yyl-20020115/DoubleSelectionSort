#include "utils.h"

bool SingleSelectionSort(int data[], int n) {
	for (int i = 0; i < n - 1; i++)
	{
		int minIndex = i;
		for (int j = i + 1; j < n; j++)
		{
			if (data[j] < data[minIndex])
			{
				minIndex = j;
			}
		}
		if (i != minIndex) Swap(data, i, minIndex);
	}
	return true;
}
bool SingleSelectionSort(unsigned int data[], int n) {
	for (int i = 0; i < n - 1; i++)
	{
		int minIndex = i;
		for (int j = i + 1; j < n; j++)
		{
			if (data[j] < data[minIndex])
			{
				minIndex = j;
			}
		}
		if (i != minIndex) Swap(data, i, minIndex);
	}
	return true;
}
