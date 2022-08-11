#include "common_sort.h"
#include "utils.h"

void OddEvenSort(int data[], int n)
{
	for (int c = 0; c < n; c++)
	{
		int p = c % 2;
		for (int d = 0; d < n - p - p; d += 2)
		{
			//this is for odd length
			if (d + p + 1 >= n) break;
			//this step should be in parallel
			if (data[d + p + 0] > data[d + p + 1])
			{
				Swap(data, d + p + 0, d + p + 1);
			}
		}
	}
}
