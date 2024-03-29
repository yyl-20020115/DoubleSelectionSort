#include "utils.h"
#include <memory>
bool CheckSequence(int a[], int b[], int n)
{
	for (int i = 0; i < n; i++) {
		if (a[i] != b[i])
			return false;
	}
	return true;
}
bool CheckSequence(unsigned int a[], int b[], int n)
{
	for (int i = 0; i < n; i++) {
		if (a[i] != b[i])
			return false;
	}
	return true;
}

void Merge(int data[], int n, int stride)
{
	//merege:
	int* merge_indices = new int[stride];
	//= { 0,1,2,3,4,5,6,7 };
	for (int i = 0; i < stride; i++)
		merge_indices[i] = i;
	int* buffer = new int[n];
	//memset(buffer, 0, n * sizeof(data[0]));
	for (int i = 0; i < n; i++)
	{
		int min_value = 0;
		int min_index = -1;
		bool first = true;
		for (int j = 0; j < stride; j++) {
			int index = merge_indices[j];
			if (index < n) {
				if (first) {
					min_value = data[index];
					min_index = j;
					first = false;
				}
				else
				{
					if (data[index] < min_value) {
						min_value = data[index];
						min_index = j;
					}
				}
			}
		}
		if (min_index >= 0) {
			merge_indices[min_index] += stride;
		}
		if (first) break;
		buffer[i] = min_value;
	}
	memcpy_s(data, n * sizeof(data[0]), buffer, n * sizeof(data[0]));
	delete[] buffer;
	delete[] merge_indices;
}
void Merge(unsigned int data[], int n, int stride)
{
	//merege:
	int* merge_indices = new int[stride];
	//= { 0,1,2,3,4,5,6,7 };
	for (int i = 0; i < stride; i++)
		merge_indices[i] = i;
	int* buffer = new int[n];
	//memset(buffer, 0, n * sizeof(data[0]));
	for (int i = 0; i < n; i++)
	{
		unsigned int min_value = 0;
		int min_index = -1;
		bool first = true;
		for (int j = 0; j < stride; j++) {
			int index = merge_indices[j];
			if (index < n) {
				if (first) {
					min_value = data[index];
					min_index = j;
					first = false;
				}
				else
				{
					if (data[index] < min_value) {
						min_value = data[index];
						min_index = j;
					}
				}
			}
		}
		if (min_index >= 0) {
			merge_indices[min_index] += stride;
		}
		if (first) break;
		buffer[i] = min_value;
	}
	memcpy_s(data, n * sizeof(data[0]), buffer, n * sizeof(data[0]));
	delete[] buffer;
	delete[] merge_indices;
}
