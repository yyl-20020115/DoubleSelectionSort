#include "utils.h"
#include "utils_256.h"
bool FastDoubleSelectionSort256(int data[], int n)
{
	const int stride = sizeof(__m256i) / sizeof(data[0]);
	if (data == 0 || n < stride || n % stride>0)
	{
		return false;
	}

	__m256i staIndex = _mm256_setr_epi32(0, 1, 2, 3, 4, 5, 6, 7);
	__m256i endIndex = _mm256_setr_epi32(n - 8, n - 7, n - 6, n - 5, n - 4, n - 3, n - 2, n - 1);

	while (true) {
		__mmask8 lt = _mm256_cmplt_epi32_mask(staIndex, endIndex);
		if (lt == 0) break;
		__m256i minIndex = staIndex;
		__m256i maxIndex = endIndex;

		__m256i staValue = _mm256_i32gather_epi32(data, staIndex, sizeof(int));
		__m256i endValue = _mm256_i32gather_epi32(data, endIndex, sizeof(int));
		__m256i minValue = staValue;
		__m256i maxValue = endValue;

		__m256i j = staIndex;
		while (true) {
			__mmask8 le = _mm256_cmple_epi32_mask(j, endIndex);
			if (le == 0) break;//any 1, still le

			__m256i data_j = _mm256_i32gather_epi32(data, j, sizeof(int));

			__mmask8 ltj = _mm256_cmplt_epi32_mask(data_j, minValue);

			minValue = _mm256_mask_blend_epi32(ltj & lt, minValue, data_j);
			minIndex = _mm256_mask_blend_epi32(ltj & lt, minIndex, j);

			__mmask8 gtj = _mm256_cmpgt_epi32_mask(data_j, maxValue);
			maxValue = _mm256_mask_blend_epi32(gtj & lt, maxValue, data_j);
			maxIndex = _mm256_mask_blend_epi32(gtj & lt, maxIndex, j);

			j = _mm256_mask_add_epi32(j, lt, j, _mm256_set1_epi32(stride));
		}

		__mmask8 neq = _mm256_cmpneq_epi32_mask(minValue, maxValue);
		if (neq == 0) break;

		//data[staIndex] = minValue;
		//data[endIndex] = maxValue;

		_mm256_mask_i32scatter_epi32(data, ~0, staIndex, minValue, sizeof(int));
		_mm256_mask_i32scatter_epi32(data, ~0, endIndex, maxValue, sizeof(int));

		__mmask8 max_sta = _mm256_cmpeq_epi32_mask(maxIndex, staIndex);
		__mmask8 min_end = _mm256_cmpeq_epi32_mask(minIndex, endIndex);
		__mmask8 both = max_sta & min_end;
		__mmask8 not_max_sta = ~max_sta;
		__mmask8 not_min_end = ~min_end;
		__mmask8 both_not = not_max_sta & not_min_end;

		//if (!both && both_not)
		//	data[minIndex] = staValue;
		//	data[maxIndex] = endValue;
		_mm256_mask_i32scatter_epi32(data, (~both) & both_not, minIndex, staValue, sizeof(int));
		_mm256_mask_i32scatter_epi32(data, (~both) & both_not, maxIndex, endValue, sizeof(int));

		//if (!both && maxIndex == staIndex)
		//data[minIndex] = endValue;
		_mm256_mask_i32scatter_epi32(data, (~both) & max_sta, minIndex, endValue, sizeof(int));
		//if (!both && minIndex == endIndex)
		//data[maxIndex] = staValue;
		_mm256_mask_i32scatter_epi32(data, (~both) & min_end, maxIndex, staValue, sizeof(int));

		//next
		staIndex = _mm256_mask_add_epi32(staIndex, lt, staIndex, _mm256_set1_epi32(stride));
		endIndex = _mm256_mask_sub_epi32(endIndex, lt, endIndex, _mm256_set1_epi32(stride));
	}
	Merge(data, n, stride);
	return true;
}
