#include <intrin.h>
#include <memory>
#include "utils_512.h"
#include "horizontal_max_min_512.h"

void Merge512(int data[], int n)
{
	const int stride = sizeof(__m512i) / sizeof(data[0]);
	__m512i _stride = _mm512_set1_epi32(stride);
	__m512i merge_indices = _mm512_setr_epi32(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);

	int* buffer = new int[n];
	memset(buffer, 0, sizeof(data[0]) * n);

	__m512i ns = _mm512_set1_epi32(n);
	for (int i = 0; i < n; i++)
	{
		__m512i current = _mm512_i32gather_epi32(merge_indices, data, sizeof(data[0]));
		int min_value = 0;
		int min_index = -1;
		__mmask16 lts = _mm512_cmple_epi32_mask(merge_indices, ns);
		if (lts == 0xffff) {
			min_index = HorizontalMin32(current, &min_value);
		}
		else
		{
			bool first = true;
			for (int j = 0; j < stride; j++) {
				int idx = merge_indices.m512i_i32[j];
				if (idx < n) {
					if (first) {
						min_value = data[idx];
						min_index = j;
						first = false;
					}
					else if (data[idx] < min_value) {
						min_value = data[idx];
						min_index = j;
					}
				}
			}
		}
		if (min_index >= 0) {
			__mmask16 mask = (1 << min_index);
			__mmask16 lt = _mm512_cmple_epi32_mask(merge_indices, ns);
			merge_indices = _mm512_mask_add_epi32(
				merge_indices, mask & lt, merge_indices, _stride);
			buffer[i] = min_value;
		}
	}
	memcpy_s(data, n * sizeof(data[0]), buffer, n * sizeof(data[0]));
	delete[] buffer;
}
void Merge512(unsigned int data[], int n)
{
	const int stride = sizeof(__m512i) / sizeof(data[0]);
	__m512i _stride = _mm512_set1_epi32(stride);
	__m512i merge_indices = _mm512_setr_epi32(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);

	unsigned int* buffer = new unsigned int[n];
	memset(buffer, 0, sizeof(data[0]) * n);

	__m512i ns = _mm512_set1_epi32(n);
	for (int i = 0; i < n; i++)
	{
		__m512i current = _mm512_i32gather_epi32(merge_indices, data, sizeof(data[0]));
		unsigned int min_value = 0;
		int min_index = -1;
		__mmask16 lts = _mm512_cmple_epi32_mask(merge_indices, ns);
		if (lts == 0xff) {
			min_index = HorizontalMin32(current, &min_value);
		}
		else
		{
			bool first = true;
			for (int j = 0; j < stride; j++) {
				int idx = merge_indices.m512i_i32[j];
				if (idx < n) {
					if (first) {
						min_value = data[idx];
						min_index = j;
						first = false;
					}
					else if (data[idx] < min_value) {
						min_value = data[idx];
						min_index = j;
					}
				}
			}
		}
		if (min_index >= 0) {
			__mmask16 mask = (1 << min_index);
			__mmask16 lt = _mm512_cmple_epi32_mask(merge_indices, ns);
			merge_indices = _mm512_mask_add_epi32(
				merge_indices, mask & lt, merge_indices, _stride);
			buffer[i] = min_value;
		}
	}
	memcpy_s(data, n * sizeof(data[0]), buffer, n * sizeof(data[0]));
	delete[] buffer;
}
