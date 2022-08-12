#include "fast_sort_512.h"
#include "utils.h"
bool FastQuickSortH512(int data[], int low, int high) {
	const int stride = sizeof(__m512i) / sizeof(data[0]);
	if (low < high)
	{
		unsigned long index = 0;
		unsigned long i = low, j = high;
		int delta = 0;
		int k = data[i];
		__m512i t = _mm512_set1_epi32(data[i]);
		while (i < j)
		{
			while (i < j)
			{
				__m512i ds = _mm512_loadu_epi32(data + j - stride + 1);
				__mmask16 rt = _mm512_cmple_epi32_mask(ds, t);

				if (rt == 0) {
					j = j - stride < i ? i : j - stride;
				}
				else {
					if (_BitScanReverse(&index, rt)) {
						delta = stride - index - 1;
						j = j - delta < i ? i : j - delta;
					}
					break;
				}
			}

			if (i < j)
				data[i++] = data[j];

			while (i < j)
			{
				__m512i ds = _mm512_loadu_epi32(data + i);
				__mmask16 rt = _mm512_cmpge_epi32_mask(ds, t);
				if (rt == 0) {
					i = i + stride > j ? j : i + stride;
				}
				else {
					if (_BitScanForward(&index, rt)) {
						i = i + index > j ? j : i + index;
					}
					break;
				}
			}
			if (i < j)
				data[j--] = data[i];
		}
		data[i] = k;
		FastQuickSortH512(data, low, i - 1);
		FastQuickSortH512(data, i + 1, high);
	}
	return true;
}
bool FastQuickSortH512(int data[], int n)
{
	const int stride = sizeof(__m512i) / sizeof(data[0]);
	if (n < stride || n % stride > 0) return false;

	return FastQuickSortH512(data, 0, n - 1);
}

bool FastQuickSortV512(int data[], __m512i low, __m512i high)
{
	const int stride = sizeof(__m512i) / sizeof(data[0]);
	//if (low < high)
	if (_mm512_cmplt_epi32_mask(low, high) == 0)
		return true;
	else
	{
		__m512i one = _mm512_set1_epi32(1);
		__m512i i = low, j = high;
		//k = data[i]
		__m512i k = _mm512_i32gather_epi32(i, data, sizeof(int));
		//while (i < j)
		while (true)
		{
			__mmask16 i_j_lt0 = _mm512_cmplt_epi32_mask(i, j);
			if (i_j_lt0 == 0) break;
			__mmask16 i_j_lt1 = 0;
			__mmask16 i_j_lt2 = 0;
			__mmask16 i_j_lt3 = 0;
			__mmask16 i_j_lt4 = 0;
			__mmask16 data_j_k_gt = 0;
			__mmask16 data_i_k_lt = 0;
			__m512i data_j = { 0 };
			__m512i data_i = { 0 };

			//while (i < j && data[j] > k)
			while (true)
			{
				i_j_lt1 = _mm512_cmplt_epi32_mask(i, j);
				if (i_j_lt1) break;
				data_j = _mm512_i32gather_epi32(j, data, sizeof(int));
				data_j_k_gt = _mm512_cmpgt_epi32_mask(data_j, k);
				if (data_j_k_gt == 0) break;
				//j--;
				j = _mm512_mask_sub_epi32(j, i_j_lt0 & i_j_lt1 & data_j_k_gt, j, one);
			}

			//if (i < j)
			{
				i_j_lt2 = _mm512_cmplt_epi32_mask(i, j);
				//data[i] = data[j];
				data_j = _mm512_i32gather_epi32(j, data, sizeof(int));
				data_i = _mm512_mask_blend_epi32(i_j_lt0 & i_j_lt2, data_i, data_j);
				_mm512_i32scatter_epi32(data, i, data_i, sizeof(int));
				//i++;
				i = _mm512_mask_add_epi32(i, i_j_lt0 & i_j_lt2, i, one);
			}

			//while (i < j && data[i] < k)
			while (true)
			{
				i_j_lt3 = _mm512_cmplt_epi32_mask(i, j);
				if (i_j_lt3 == 0) break;
				data_i = _mm512_i32gather_epi32(i, data, sizeof(int));
				data_i_k_lt = _mm512_cmplt_epi32_mask(data_i, k);
				if (data_i_k_lt == 0)break;
				//i++;
				i = _mm512_mask_add_epi32(i, i_j_lt0 & i_j_lt3 & data_i_k_lt, i, one);
			}

			//if (i < j)
			{
				i_j_lt4 = _mm512_cmplt_epi32_mask(i, j);
				//data[j] = data[i];
				data_i = _mm512_i32gather_epi32(i, data, sizeof(int));
				data_j = _mm512_mask_blend_epi32(i_j_lt0 & i_j_lt4, data_i, data_j);
				_mm512_i32scatter_epi32(data, j, data_j, sizeof(int));
				//j--;
				j = _mm512_mask_sub_epi32(j, i_j_lt0 & i_j_lt4, j, one);
			}
		}

		//data[i] = k;
		_mm512_i32scatter_epi32(data, i, k, sizeof(int));
		//FastQuickSortP512(data, low, i - 1);
		//FastQuickSortP512(data, i + 1, high);		
		FastQuickSortV512(data, low, _mm512_sub_epi32(i, one));
		FastQuickSortV512(data, _mm512_add_epi32(i, one), high);
	}
	return true;

}

bool FastQuickSortV512(int data[], int n)
{
	const int stride = sizeof(__m512i) / sizeof(data[0]);
	if (n < stride || n % stride > 0) return false;

	if (FastQuickSortV512(data, _mm512_set1_epi32(0), _mm512_set1_epi32(n - 1)))
	{
		Merge(data, n, stride);
		return true;
	}
	return false;
}
