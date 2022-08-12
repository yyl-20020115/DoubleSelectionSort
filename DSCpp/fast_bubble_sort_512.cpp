#include "utils.h"
#include "utils_512.h"
#include <intrin.h>

bool FastBubbleSort512(int data[], int n) {
	const int stride = sizeof(__m512i) / sizeof(data[0]);
	if (data == 0 || n < stride || n % stride > 0)
		return false;

	bool swapped = false;
	for (int i = 0; i < n; i += stride) {
		swapped = false;
		__m512i data_i = _mm512_loadu_epi32(data + i);
		for (int j = i + stride; j < n; j += stride) {
			__m512i data_j = _mm512_loadu_epi32(data + j);
			__mmask16 gt = _mm512_cmpgt_epi32_mask(data_i, data_j);
			if (gt != 0) {
				__m512i min = _mm512_min_epi32(data_i, data_j);
				__m512i max = _mm512_max_epi32(data_i, data_j);
				data_i = min;
				data_j = max;
				_mm512_storeu_epi32(data + i, data_i);
				_mm512_storeu_epi32(data + j, data_j);

				swapped = true;
			}
		}
		if (!swapped)break;
	}
	Merge(data, n, stride);
	return true;
}
