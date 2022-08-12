#include <memory>
#include "horizontal_sort_256.h"
#include "utils_256.h"

bool FastMergeSort256(int data[], int n) {
	const int stride = sizeof(__m256i) / sizeof(data[0]);
	const int dual = stride << 1;
	if (data == 0 || n < dual || n % dual > 0)
		return false;
	int* buffer = new int[n];

	int gap = stride;
	while (gap < n) {
		int gap2 = gap << 1;
		for (int i = 0; i < n; i += gap2) {
			if (gap == stride) {
				//0 - stride
				__m256i values = _mm256_loadu_epi32(data + i);
				__m256i sorted = HorizontalSort32(values,(int*)0,(int*)0);

				_mm256_storeu_epi32(data + i, sorted);

				//stride - 2*stride
				values = _mm256_loadu_epi32(data + i + stride);
				sorted = HorizontalSort32(values, (int*)0, (int*)0);
				_mm256_storeu_epi32(data + i + stride, sorted);
			}

			int i_left = i;
			int i_middle = i + gap;
			int i_right = i_middle + gap;

			int begin1 = i_left;
			int begin2 = i_middle;
			int end1 = i_middle;
			int end2 = i_right;

			int left_index = i_left;
			int right_index = i_right;
			while (begin1 < end1 && begin2 < end2) {
				if (data[begin1] < data[begin2]) {
					buffer[left_index++] = data[begin1++];
				}
				else {
					buffer[left_index++] = data[begin2++];
				}
			}
			int delta = end1 - begin1;
			if (delta > 0) {
				memcpy_s(buffer + left_index, sizeof(int) * delta, data + begin1, sizeof(int) * delta);
				left_index += delta;
			}
			delta = end2 - begin2;
			if (delta > 0) {
				memcpy_s(buffer + left_index, sizeof(int) * delta, data + begin2, sizeof(int) * delta);
			}
			memcpy_s(data + i, sizeof(int) * gap2, buffer + i, sizeof(int) * gap2);
		}
		gap = gap2;
	}
	delete[] buffer;
	return true;
}
bool FastMergeSort256(unsigned int data[], int n) {
	const int stride = sizeof(__m256i) / sizeof(data[0]);
	const int dual = stride << 1;
	if (data == 0 || n < dual || n % dual > 0)
		return false;
	unsigned int* buffer = new unsigned int[n];

	int gap = stride;
	while (gap < n) {
		int gap2 = gap << 1;
		for (int i = 0; i < n; i += gap2) {
			if (gap == stride) {
				//0 - stride
				__m256i values = _mm256_loadu_epi32(data + i);
				__m256i sorted = HorizontalSort32(values, (int*)0, (int*)0);

				_mm256_storeu_epi32(data + i, sorted);

				//stride - 2*stride
				values = _mm256_loadu_epi32(data + i + stride);
				sorted = HorizontalSort32(values, (unsigned int*)0, (unsigned int*)0);
				_mm256_storeu_epi32(data + i + stride, sorted);
			}

			int i_left = i;
			int i_middle = i + gap;
			int i_right = i_middle + gap;

			int begin1 = i_left;
			int begin2 = i_middle;
			int end1 = i_middle;
			int end2 = i_right;

			int left_index = i_left;
			int right_index = i_right;
			while (begin1 < end1 && begin2 < end2) {
				if (data[begin1] < data[begin2]) {
					buffer[left_index++] = data[begin1++];
				}
				else {
					buffer[left_index++] = data[begin2++];
				}
			}
			int delta = end1 - begin1;
			if (delta > 0) {
				memcpy_s(buffer + left_index, sizeof(int) * delta, data + begin1, sizeof(int) * delta);
				left_index += delta;
			}
			delta = end2 - begin2;
			if (delta > 0) {
				memcpy_s(buffer + left_index, sizeof(int) * delta, data + begin2, sizeof(int) * delta);
			}
			memcpy_s(data + i, sizeof(int) * gap2, buffer + i, sizeof(int) * gap2);
		}
		gap = gap2;
	}
	delete[] buffer;
	return true;
}
