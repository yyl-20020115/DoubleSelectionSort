#include "utils.h"
#include "utils_256.h"
bool FastSingleSelectionSort256(int data[], int n) {
	const int stride = sizeof(__m256i) / sizeof(data[0]);

	if (data == 0 || n < stride || n % stride>0) return false;

	for (int i = 0; i <= n - stride; i += stride) {

		__m256i minValues = _mm256_loadu_epi32(data + i);
		__m256i i_Indices = _mm256_setr_epi32(i + 0, i + 1, i + 2, i + 3, i + 4, i + 5, i + 6, i + 7);
		__m256i minIndices = i_Indices;

		for (int j = i + stride; j < n; j += stride) {

			__m256i j_Indices = _mm256_setr_epi32(j + 0, j + 1, j + 2, j + 3, j + 4, j + 5, j + 6, j + 7);
			__m256i values = _mm256_loadu_epi32(data + j);

			__mmask8 mask = _mm256_cmplt_epi32_mask(values, minValues);

			/* convert 8 bit mask to __m128i control word mask */
			//__m256i _mask = bitmap2vecmask((__mmask8)mask);			
			//minIndices = _mm256_castps_si256(
			//	_mm256_blendv_ps(_mm256_castsi256_ps(minIndices),
			//		_mm256_castsi256_ps(j_Indices), _mm256_castsi256_ps(_mask)));

			minIndices = _mm256_mask_blend_epi32(mask, minIndices, j_Indices);

			minValues = _mm256_i32gather_epi32(data, minIndices, sizeof(data[0]));
		}
		long mask = _mm256_cmpneq_epi32_mask(minIndices, i_Indices);

		unsigned long idx = 0;
		while (_BitScanForward(&idx, mask))
		{
			_bittestandreset(&mask, idx);
			int tp = minIndices.m256i_i32[idx];
			Swap(data, i + idx, tp);
		}
	}

	Merge(data, n, stride);

	return true;
}
bool FastSingleSelectionSort256(unsigned int data[], int n) {
	const int stride = sizeof(__m256i) / sizeof(data[0]);

	if (data == 0 || n < stride || n % stride>0) return false;

	for (int i = 0; i <= n - stride; i += stride) {

		__m256i minValues = _mm256_loadu_epi32(data + i);
		__m256i i_Indices = _mm256_setr_epi32(i + 0, i + 1, i + 2, i + 3, i + 4, i + 5, i + 6, i + 7);
		__m256i minIndices = i_Indices;

		for (int j = i + stride; j < n; j += stride) {

			__m256i j_Indices = _mm256_setr_epi32(j + 0, j + 1, j + 2, j + 3, j + 4, j + 5, j + 6, j + 7);
			__m256i values = _mm256_loadu_epi32(data + j);

			__mmask8 mask = _mm256_cmplt_epi32_mask(values, minValues);

			/* convert 8 bit mask to __m128i control word mask */
			//__m256i _mask = bitmap2vecmask((__mmask8)mask);			
			//minIndices = _mm256_castps_si256(
			//	_mm256_blendv_ps(_mm256_castsi256_ps(minIndices),
			//		_mm256_castsi256_ps(j_Indices), _mm256_castsi256_ps(_mask)));

			minIndices = _mm256_mask_blend_epi32(mask, minIndices, j_Indices);

			minValues = _mm256_i32gather_epi32((int*)data, minIndices, sizeof(data[0]));
		}
		long mask = _mm256_cmpneq_epi32_mask(minIndices, i_Indices);

		unsigned long idx = 0;
		while (_BitScanForward(&idx, mask))
		{
			_bittestandreset(&mask, idx);
			int tp = minIndices.m256i_i32[idx];
			Swap(data, i + idx, tp);
		}
	}

	Merge(data, n, stride);

	return true;
}
