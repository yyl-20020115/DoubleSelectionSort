#include "utils.h"
bool FastBubbleSort256(int data[], int n) {
	const int stride = sizeof(__m256i) / sizeof(data[0]);
	if (data == 0 || n < stride || n % stride > 0)
		return false;

	bool swapped = false;
	for (int i = 0; i < n; i += stride) {
		swapped = false;
		__m256i data_i = _mm256_loadu_epi32(data + i);
		for (int j = i + stride; j < n; j += stride) {
			__m256i data_j = _mm256_loadu_epi32(data + j);
			__mmask8 ge = _mm256_cmpge_epi32_mask(data_i, data_j);
			if (ge != 0) {
				__m256i min = _mm256_min_epi32(data_i, data_j);
				__m256i max = _mm256_max_epi32(data_i, data_j);
				data_i = min;
				data_j = max;
				_mm256_storeu_epi32(data + i, data_i);
				_mm256_storeu_epi32(data + j, data_j);

				swapped = true;
			}
		}
		if (!swapped)break;
	}
	Merge(data, n, stride);
	return true;
}
