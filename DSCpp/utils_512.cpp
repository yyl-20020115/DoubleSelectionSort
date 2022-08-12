#include <intrin.h>
#include <memory>
#include "utils_512.h"
#include "horizontal_max_min_512.h"

void Merge512(int data[], int n)
{
#if 1
#else
	const int stride = sizeof(__m512i) / sizeof(int);
	__m512i _stride = _mm512_set1_epi32(stride);
	//merege:
	int merge_indices_buffer[stride]{};
	for (int i = 0; i < stride; i++)
		merge_indices_buffer[i] = i;

	__m512i merge_indices = _mm512_loadu_epi32(merge_indices_buffer);

	int* buffer = new int[(n + stride)];
	memset(buffer, ~0, (n + stride) * sizeof(int));

	//memset(buffer, 0, n * sizeof(int));
	for (int i = 0; i < n; i++)
	{
		__m512i current = _mm512_i32gather_epi32(merge_indices, data, sizeof(int));
		unsigned int min_value = 0;
		int min_index = HorizontalMin32(current, &min_value);
		__mmask8 mask = (1 << min_index);
		merge_indices = _mm512_mask_add_epi32(merge_indices, mask, merge_indices, _stride);
		buffer[i] = min_value;
	}
	memcpy_s(data, n * sizeof(int), buffer, n * sizeof(int));
	delete[] buffer;
#endif
}
