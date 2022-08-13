#include <intrin.h>
#include "utils.h"
#include "utils_512.h"

bool FastSingleSelectionSort512(int data[], int n) {
	const int stride = sizeof(__m512i) / sizeof(data[0]);

	if (data == 0 || n < stride || n % stride>0) return false;

	for (int i = 0; i <= n - stride; i += stride) {

		__m512i minValues = _mm512_loadu_epi32(data + i);
		__m512i i_Indices = _mm512_setr_epi32(
			i + 0, i + 1, i + 2, i + 3,
			i + 4, i + 5, i + 6, i + 7,
			i + 8, i + 9, i + 10, i + 11,
			i + 12, i + 13, i + 14, i + 15
		);
		__m512i minIndices = i_Indices;

		for (int j = i + stride; j < n; j += stride) {

			__m512i j_Index = _mm512_setr_epi32(
				j + 0, j + 1, j + 2, j + 3,
				j + 4, j + 5, j + 6, j + 7,
				j + 8, j + 9, j + 10, j + 11,
				j + 12, j + 13, j + 14, j + 15
			);
			__m512i values = _mm512_loadu_epi32(data + j);

			__mmask16 mask = _mm512_cmplt_epi32_mask(values, minValues);
			minIndices = _mm512_mask_blend_epi32(mask, minIndices, j_Index);

			minValues = _mm512_i32gather_epi32(minIndices, data, sizeof(data[0]));
		}
		long mask = _mm512_cmpneq_epi32_mask(
			minIndices, i_Indices);

		unsigned long idx = 0;
		while (_BitScanForward(&idx, mask))
		{
			_bittestandreset(&mask, idx);
			int tp = minIndices.m512i_i32[idx];
			Swap(data, i + idx, tp);
		}
	}
	Merge(data, n, stride);

	return true;
}
bool FastSingleSelectionSort512(unsigned int data[], int n) {
	const int stride = sizeof(__m512i) / sizeof(data[0]);

	if (data == 0 || n < stride || n % stride>0) return false;

	for (int i = 0; i <= n - stride; i += stride) {

		__m512i minValues = _mm512_loadu_epi32(data + i);
		__m512i i_Indices = _mm512_setr_epi32(
			i + 0, i + 1, i + 2, i + 3,
			i + 4, i + 5, i + 6, i + 7,
			i + 8, i + 9, i + 10, i + 11,
			i + 12, i + 13, i + 14, i + 15
		);
		__m512i minIndices = i_Indices;

		for (int j = i + stride; j < n; j += stride) {

			__m512i j_Index = _mm512_setr_epi32(
				j + 0, j + 1, j + 2, j + 3,
				j + 4, j + 5, j + 6, j + 7,
				j + 8, j + 9, j + 10, j + 11,
				j + 12, j + 13, j + 14, j + 15
			);
			__m512i values = _mm512_loadu_epi32(data + j);

			__mmask16 mask = _mm512_cmplt_epi32_mask(values, minValues);
			minIndices = _mm512_mask_blend_epi32(mask, minIndices, j_Index);

			minValues = _mm512_i32gather_epi32(minIndices, data, sizeof(data[0]));
		}
		long mask = _mm512_cmpneq_epi32_mask(
			minIndices, i_Indices);

		unsigned long idx = 0;
		while (_BitScanForward(&idx, mask))
		{
			_bittestandreset(&mask, idx);
			int tp = minIndices.m512i_i32[idx];
			Swap(data, i + idx, tp);
		}
	}
	Merge(data, n, stride);

	return true;
}
