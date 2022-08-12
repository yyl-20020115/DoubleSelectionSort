#include "utils.h"
#include "utils_512.h"
#include <intrin.h>

bool FastInsertionSort512(int data[], int n) {
	const int stride = sizeof(__m512i) / sizeof(data[0]);
	if (data == 0 || n < stride || n % stride > 0)
		return false;

	for (int i = stride; i < n; i += stride)
	{
		int j = i;
		do {
			__m512i last = _mm512_loadu_epi32(data + j - stride);
			__m512i current = _mm512_loadu_epi32(data + j);
			__mmask16 lt = _mm512_cmplt_epi32_mask(current, last);
			if (lt == 0) break;
			__m512i min = _mm512_min_epi32(current, last);
			__m512i max = _mm512_max_epi32(current, last);
			_mm512_storeu_epi32(data + j - stride, min);
			_mm512_storeu_epi32(data + j, max);
			j -= stride;
		} while (j > 0);
	}
	Merge(data, n, stride);
	return true;
}
