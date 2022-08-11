#include <memory>
void MergeData(int data[], int left, int mid, int right, int* buffer) {
	int begin1 = left;
	int begin2 = mid;
	int end1 = mid;
	int end2 = right;
	int index = left;
	while (begin1 < end1 && begin2 < end2) {
		if (data[begin1] < data[begin2]) {
			buffer[index++] = data[begin1++];
		}
		else {
			buffer[index++] = data[begin2++];
		}
	}
	while (begin1 < end1) {
		buffer[index++] = data[begin1++];
	}
	while (begin2 < end2) {
		buffer[index++] = data[begin2++];
	}
}
void RecursiveMergeSort(int data[], int left, int right, int* buffer) {
	if (right - left <= 1) return;
	int mid = left + ((right - left) >> 1);
	RecursiveMergeSort(data, left, mid, buffer);
	RecursiveMergeSort(data, mid, right, buffer);
	MergeData(data, left, mid, right, buffer);
	memcpy(data + left, buffer + left, (right - left) * sizeof(data[0]));
}
void RecursiveMergeSort(int a[], int n) 
{
	int* buffer = new int[n];
	RecursiveMergeSort(a, 0, n, buffer);
	delete[] buffer;
}
void NonRecursiveMergeSort(int data[], int n) 
{
	int* buffer = new int[n];
	int gap = 1;
	while (gap < n) {
		for (int i = 0; i < n; i += 2 * gap) {
			int left = i;
			int mid = left + gap;
			int right = mid + gap;
			if (mid >= n)
				mid = n;
			if (right >= n)
				right = n;
			MergeData(data, left, mid, right, buffer);
		}
		memcpy_s(data, n * sizeof(data[0]), buffer, n * sizeof(data[0]));
		gap *= 2;
	}
	delete[] buffer;
}
