#include "utils.h"
void BubbleSort(int data[], int n) 
{
	bool swapped = false;
	for (int i = 0; i < n; i++) {
		swapped = false;
		for (int j = i + 1; j < n; j++) {
			if (data[i] >= data[j]) {
				Swap(data, i, j);
				swapped = true;
			}
		}
		if (!swapped)break;
	}
}
