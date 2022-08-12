#include "utils.h"
#include "utils_256.h"
bool FastInsertionSort256(int data[], int n) {
	const int stride = sizeof(__m256i) / sizeof(data[0]);
	if (data == 0 || n < stride || n % stride > 0)
		return false;

	for (int i = stride; i < n; i += stride)
	{
		int j = i;
		do {
			__m256i last = _mm256_loadu_epi32(data + j - stride);
			__m256i current = _mm256_loadu_epi32(data + j);
			__mmask8 lt = _mm256_cmplt_epi32_mask(current, last);
			if (lt == 0) break;
			__m256i min = _mm256_min_epi32(current, last);
			__m256i max = _mm256_max_epi32(current, last);
			_mm256_storeu_epi32(data + j - stride, min);
			_mm256_storeu_epi32(data + j, max);
			j -= stride;
		} while (j > 0);
	}
	Merge(data, n, stride);
	return true;
}
