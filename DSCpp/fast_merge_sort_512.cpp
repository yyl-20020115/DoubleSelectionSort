#include <memory>
#include <intrin.h>
#include "horizontal_sort.h"

bool FastMergeSort512(int data[], int n) {
	const int stride = sizeof(__m512i) / sizeof(data[0]);
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
				__m512i values = _mm512_loadu_epi32(data + i);
				__m512i sorted = HorizontalSort32(values);
				_mm512_storeu_epi32(data + i, sorted);

				//stride - 2*stride
				values = _mm512_loadu_epi32(data + i + stride);
				sorted = HorizontalSort32(values);
				_mm512_storeu_epi32(data + i + stride, sorted);
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

#if 0 //takes more time than expected
			int p1 = 0;
			int p2 = 0;
			__m512i left = { 0 };
			__m512i right = { 0 };
			while (begin1 < end1 && begin2 < end2) {
				if (p1 == 0) {
					left = _mm512_loadu_epi32(data + begin1);
				}
				if (p2 == 0) {
					right = _mm512_loadu_epi32(data + begin2);
				}
				int v1 = left.m512i_i32[p1];
				int v2 = right.m512i_i32[p2];
				if (v1 < v2) {
					buffer[left_index++] = v1;
					begin1++;
				}
				else {
					buffer[left_index++] = v2;
					begin2++;
				}

				p1 = (begin1 - i_left) % stride;
				p2 = (begin2 - i_middle) % stride;
			}
#else

			while (begin1 < end1 && begin2 < end2) {
				if (data[begin1] < data[begin2]) {
					buffer[left_index++] = data[begin1++];
				}
				else {
					buffer[left_index++] = data[begin2++];
				}
			}
#endif
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
