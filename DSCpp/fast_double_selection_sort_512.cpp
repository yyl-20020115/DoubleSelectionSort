#include <intrin.h>
#include "utils.h"
#include "utils_512.h"
bool FastDoubleSelectionSort512(int data[], int n)
{
	const int stride = sizeof(__m512i) / sizeof(data[0]);
	if (data == 0 || n < stride || n % stride>0)
	{
		return false;
	}

	__m512i staIndex = _mm512_setr_epi32(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
	__m512i endIndex = _mm512_setr_epi32(
		n - 16, n - 15, n - 14, n - 13, n - 12, n - 11, n - 10, n - 9,
		n - 8, n - 7, n - 6, n - 5, n - 4, n - 3, n - 2, n - 1);

	while (true) {
		__mmask16 lt = _mm512_cmplt_epi32_mask(staIndex, endIndex);
		if (lt == 0) break;
		__m512i minIndex = staIndex;
		__m512i maxIndex = endIndex;

		__m512i staValue = _mm512_i32gather_epi32(staIndex, data, sizeof(data[0]));
		__m512i endValue = _mm512_i32gather_epi32(endIndex, data, sizeof(data[0]));
		__m512i minValue = staValue;
		__m512i maxValue = endValue;

		__m512i j = staIndex;
		while (true) {
			__mmask16 le = _mm512_cmple_epi32_mask(j, endIndex);
			if (le == 0) break;//any 1, still le

			__m512i data_j = _mm512_i32gather_epi32(j, data, sizeof(data[0]));

			__mmask16 ltj = _mm512_cmplt_epi32_mask(data_j, minValue);

			minValue = _mm512_mask_blend_epi32(ltj & lt, minValue, data_j);
			minIndex = _mm512_mask_blend_epi32(ltj & lt, minIndex, j);

			__mmask16 gtj = _mm512_cmpgt_epi32_mask(data_j, maxValue);
			maxValue = _mm512_mask_blend_epi32(gtj & lt, maxValue, data_j);
			maxIndex = _mm512_mask_blend_epi32(gtj & lt, maxIndex, j);

			//only add for these having 1s
			j = _mm512_mask_add_epi32(j, lt, j, _mm512_set1_epi32(stride));
		}

		__mmask16 neq = _mm512_cmpneq_epi32_mask(minValue, maxValue);
		if (neq == 0) break;

		//data[staIndex] = minValue;
		//data[endIndex] = maxValue;
		_mm512_mask_i32scatter_epi32(data, ~0, staIndex, minValue, sizeof(data[0]));
		_mm512_mask_i32scatter_epi32(data, ~0, endIndex, maxValue, sizeof(data[0]));

		__mmask16 max_sta = _mm512_cmpeq_epi32_mask(maxIndex, staIndex);
		__mmask16 min_end = _mm512_cmpeq_epi32_mask(minIndex, endIndex);
		__mmask16 both = max_sta & min_end;
		__mmask16 not_max_sta = ~max_sta;
		__mmask16 not_min_end = ~min_end;
		__mmask16 both_not = not_max_sta & not_min_end;

		//if (!both && both_not)
		//	data[minIndex] = staValue;
		//	data[maxIndex] = endValue;
		_mm512_mask_i32scatter_epi32(data, (~both) & both_not, minIndex, staValue, sizeof(data[0]));
		_mm512_mask_i32scatter_epi32(data, (~both) & both_not, maxIndex, endValue, sizeof(data[0]));

		//if (!both && maxIndex == staIndex)
		//data[minIndex] = endValue;
		_mm512_mask_i32scatter_epi32(data, (~both) & max_sta, minIndex, endValue, sizeof(data[0]));
		//if (!both && minIndex == endIndex)
		//data[maxIndex] = staValue;
		_mm512_mask_i32scatter_epi32(data, (~both) & min_end, maxIndex, staValue, sizeof(data[0]));

		//next
		staIndex = _mm512_mask_add_epi32(staIndex, lt, staIndex, _mm512_set1_epi32(stride));
		endIndex = _mm512_mask_sub_epi32(endIndex, lt, endIndex, _mm512_set1_epi32(stride));
	}
	Merge(data, n, stride);
	return true;
}
bool FastDoubleSelectionSort512(unsigned int data[], int n)
{
	const int stride = sizeof(__m512i) / sizeof(data[0]);
	if (data == 0 || n < stride || n % stride>0)
	{
		return false;
	}

	__m512i staIndex = _mm512_setr_epi32(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
	__m512i endIndex = _mm512_setr_epi32(
		n - 16, n - 15, n - 14, n - 13, n - 12, n - 11, n - 10, n - 9,
		n - 8, n - 7, n - 6, n - 5, n - 4, n - 3, n - 2, n - 1);

	while (true) {
		__mmask16 lt = _mm512_cmplt_epi32_mask(staIndex, endIndex);
		if (lt == 0) break;
		__m512i minIndex = staIndex;
		__m512i maxIndex = endIndex;

		__m512i staValue = _mm512_i32gather_epi32(staIndex, data, sizeof(data[0]));
		__m512i endValue = _mm512_i32gather_epi32(endIndex, data, sizeof(data[0]));
		__m512i minValue = staValue;
		__m512i maxValue = endValue;

		__m512i j = staIndex;
		while (true) {
			__mmask16 le = _mm512_cmple_epi32_mask(j, endIndex);
			if (le == 0) break;//any 1, still le

			__m512i data_j = _mm512_i32gather_epi32(j, data, sizeof(data[0]));

			__mmask16 ltj = _mm512_cmplt_epi32_mask(data_j, minValue);

			minValue = _mm512_mask_blend_epi32(ltj & lt, minValue, data_j);
			minIndex = _mm512_mask_blend_epi32(ltj & lt, minIndex, j);

			__mmask16 gtj = _mm512_cmpgt_epi32_mask(data_j, maxValue);
			maxValue = _mm512_mask_blend_epi32(gtj & lt, maxValue, data_j);
			maxIndex = _mm512_mask_blend_epi32(gtj & lt, maxIndex, j);

			//only add for these having 1s
			j = _mm512_mask_add_epi32(j, lt, j, _mm512_set1_epi32(stride));
		}

		__mmask16 neq = _mm512_cmpneq_epi32_mask(minValue, maxValue);
		if (neq == 0) break;

		//data[staIndex] = minValue;
		//data[endIndex] = maxValue;
		_mm512_mask_i32scatter_epi32(data, ~0, staIndex, minValue, sizeof(data[0]));
		_mm512_mask_i32scatter_epi32(data, ~0, endIndex, maxValue, sizeof(data[0]));

		__mmask16 max_sta = _mm512_cmpeq_epi32_mask(maxIndex, staIndex);
		__mmask16 min_end = _mm512_cmpeq_epi32_mask(minIndex, endIndex);
		__mmask16 both = max_sta & min_end;
		__mmask16 not_max_sta = ~max_sta;
		__mmask16 not_min_end = ~min_end;
		__mmask16 both_not = not_max_sta & not_min_end;

		//if (!both && both_not)
		//	data[minIndex] = staValue;
		//	data[maxIndex] = endValue;
		_mm512_mask_i32scatter_epi32(data, (~both) & both_not, minIndex, staValue, sizeof(data[0]));
		_mm512_mask_i32scatter_epi32(data, (~both) & both_not, maxIndex, endValue, sizeof(data[0]));

		//if (!both && maxIndex == staIndex)
		//data[minIndex] = endValue;
		_mm512_mask_i32scatter_epi32(data, (~both) & max_sta, minIndex, endValue, sizeof(data[0]));
		//if (!both && minIndex == endIndex)
		//data[maxIndex] = staValue;
		_mm512_mask_i32scatter_epi32(data, (~both) & min_end, maxIndex, staValue, sizeof(data[0]));

		//next
		staIndex = _mm512_mask_add_epi32(staIndex, lt, staIndex, _mm512_set1_epi32(stride));
		endIndex = _mm512_mask_sub_epi32(endIndex, lt, endIndex, _mm512_set1_epi32(stride));
	}
	Merge(data, n, stride);
	return true;
}
