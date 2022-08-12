#include "utils.h"
void HeapAdjust(int data[], int position, int n)
{
	int temporary = data[position];
	int childpos = 2 * position + 1;
	while (childpos < n) {
		if (childpos + 1 < n && data[childpos] < data[childpos + 1]) {
			++childpos;
		}
		if (data[position] < data[childpos]) {
			data[position] = data[childpos];
			position = childpos;
			childpos = 2 * position + 1;
		}
		else 
		{
			break;
		}
		data[position] = temporary;
	}
}
void HeapSort(int data[], int n)
{
	for (int i = (n - 1) >> 1; i >= 0; --i)
		HeapAdjust(data, i, n);
	for (int i = n - 1; i > 0; --i)
	{
		Swap(data, i, 0);
		HeapAdjust(data, 0, i);
	}
}

void HeapAdjust(unsigned int data[], int position, int n)
{
	unsigned int temporary = data[position];
	int childpos = 2 * position + 1;
	while (childpos < n) {
		if (childpos + 1 < n && data[childpos] < data[childpos + 1]) {
			++childpos;
		}
		if (data[position] < data[childpos]) {
			data[position] = data[childpos];
			position = childpos;
			childpos = 2 * position + 1;
		}
		else
		{
			break;
		}
		data[position] = temporary;
	}
}
void HeapSort(unsigned int data[], int n)
{
	for (int i = (n - 1) >> 1; i >= 0; --i)
		HeapAdjust(data, i, n);
	for (int i = n - 1; i > 0; --i)
	{
		Swap(data, i, 0);
		HeapAdjust(data, 0, i);
	}
}
