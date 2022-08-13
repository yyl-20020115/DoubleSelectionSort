#include <intrin.h>
#include "fast_sort_512.h"
#include "utils.h"
#include "utils_512.h"
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
		__m512i _stride = _mm512_set1_epi32(stride);
		__m512i i = low, j = high;
		//k = data[i]
		__m512i k = _mm512_i32gather_epi32(i, data, sizeof(data[0]));
		//while (i < j)
		__mmask16 i_j_lt0 = 0;
		__mmask16 i_j_lt1 = 0;
		__mmask16 i_j_lt2 = 0;
		__mmask16 i_j_lt3 = 0;
		__mmask16 i_j_lt4 = 0;
		__mmask16 data_j_k_gt = 0;
		__mmask16 data_i_k_lt = 0;
		__m512i data_j = { 0 };
		__m512i data_i = { 0 };
		__m512i oldvalue = { 0 };
		while (0 != (i_j_lt0 = _mm512_cmplt_epi32_mask(i, j)))
		{
			//while (i < j && data[j] > k)
			while (true)
			{
				i_j_lt1 = _mm512_cmplt_epi32_mask(i, j);
				data_j = _mm512_i32gather_epi32(j, data, sizeof(data[0]));
				data_j_k_gt = _mm512_cmpgt_epi32_mask(data_j, k);
				if (i_j_lt1 == 0) break;
				if (data_j_k_gt == 0) break;

				oldvalue = j;
				//j--;
				if (0 == _mm512_cmpneq_epi32_mask(oldvalue,
					j = _mm512_mask_sub_epi32(j, i_j_lt0 & i_j_lt1 & data_j_k_gt, j, _stride)))
				{
					j = oldvalue;
					break;
				}
			}

			//if (i < j)
			{
				i_j_lt2 = _mm512_cmplt_epi32_mask(i, j);
				//data[i] = data[j];
				data_j = _mm512_i32gather_epi32(j, data, sizeof(data[0]));
				data_i = _mm512_i32gather_epi32(i, data, sizeof(data[0]));
				data_i = _mm512_mask_blend_epi32(i_j_lt0 & i_j_lt2, data_i, data_j);
				_mm512_mask_i32scatter_epi32(data, i_j_lt0 & i_j_lt2, i, data_i, sizeof(data[0]));
				//i++;
				i = _mm512_mask_add_epi32(i, i_j_lt0 & i_j_lt2, i, _stride);
			}

			//while (i < j && data[i] < k)
			while (true)
			{
				i_j_lt3 = _mm512_cmplt_epi32_mask(i, j);
				data_i = _mm512_i32gather_epi32(i, data, sizeof(data[0]));
				data_i_k_lt = _mm512_cmplt_epi32_mask(data_i, k);

				if (i_j_lt3 == 0) break;
				if (data_i_k_lt == 0) break;

				oldvalue = i;
				//i++;
				if (0 == _mm512_cmpneq_epi32_mask(oldvalue,
					i = _mm512_mask_add_epi32(i, i_j_lt0 & i_j_lt3 & data_i_k_lt, i, _stride)))
				{
					i = oldvalue;
					break;
				}
			}

			//if (i < j)
			//if(0!=(i_j_lt4 = _mm512_cmplt_epi32_mask(i, j)))
			{
				i_j_lt4 = _mm512_cmplt_epi32_mask(i, j);
				//data[j] = data[i];
				data_i = _mm512_i32gather_epi32(i, data, sizeof(data[0]));
				data_j = _mm512_i32gather_epi32(j, data, sizeof(data[0]));
				data_j = _mm512_mask_blend_epi32(i_j_lt0 & i_j_lt4, data_j, data_i);
				_mm512_mask_i32scatter_epi32(data, i_j_lt0 & i_j_lt4, j, data_j, sizeof(data[0]));
				//j--;
				j = _mm512_mask_sub_epi32(j, i_j_lt0 & i_j_lt4, j, _stride);
			}
		}
		//data[i] = k;
		_mm512_mask_i32scatter_epi32(data, ~0, i, k, sizeof(data[0]));
		//FastQuickSortP512(data, low, i - 1);
		//FastQuickSortP512(data, i + 1, high);		
		FastQuickSortV512(data, low, _mm512_sub_epi32(i, _stride));
		FastQuickSortV512(data, _mm512_add_epi32(i, _stride), high);
	}
	return true;

}
bool FastQuickSortV512(int data[], int n)
{
	const int stride = sizeof(__m512i) / sizeof(data[0]);
	if (n < stride || n % stride > 0) return false;
	__m512i low = _mm512_setr_epi32(
		0, 1, 2, 3, 4, 5, 6, 7,
		8, 9, 10, 11, 12, 13, 14, 15);
	__m512i high = _mm512_setr_epi32(
		n - 16, n - 15, n - 14, n - 13,
		n - 12, n - 11, n - 10, n - 9,
		n - 8, n - 7, n - 6, n - 5,
		n - 4, n - 3, n - 2, n - 1);

	if (FastQuickSortV512(data, low, high))
	{
		Merge(data, n, stride);
		return true;
	}
	return false;
}

bool FastQuickSortH512(unsigned int data[], int low, int high) {
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
bool FastQuickSortH512(unsigned int data[], int n)
{
	const int stride = sizeof(__m512i) / sizeof(data[0]);
	if (n < stride || n % stride > 0) return false;

	return FastQuickSortH512(data, 0, n - 1);
}
bool FastQuickSortV512(unsigned int data[], __m512i low, __m512i high)
{
	const int stride = sizeof(__m512i) / sizeof(data[0]);
	//if (low < high)
	if (_mm512_cmplt_epi32_mask(low, high) == 0)
		return true;
	else
	{
		__m512i _stride = _mm512_set1_epi32(stride);
		__m512i i = low, j = high;
		//k = data[i]
		__m512i k = _mm512_i32gather_epi32(i, data, sizeof(data[0]));
		//while (i < j)
		__mmask16 i_j_lt0 = 0;
		__mmask16 i_j_lt1 = 0;
		__mmask16 i_j_lt2 = 0;
		__mmask16 i_j_lt3 = 0;
		__mmask16 i_j_lt4 = 0;
		__mmask16 data_j_k_gt = 0;
		__mmask16 data_i_k_lt = 0;
		__m512i data_j = { 0 };
		__m512i data_i = { 0 };
		__m512i oldvalue = { 0 };
		while (0 != (i_j_lt0 = _mm512_cmplt_epi32_mask(i, j)))
		{
			//while (i < j && data[j] > k)
			while (true)
			{
				i_j_lt1 = _mm512_cmplt_epi32_mask(i, j);
				data_j = _mm512_i32gather_epi32(j, data, sizeof(data[0]));
				data_j_k_gt = _mm512_cmpgt_epi32_mask(data_j, k);
				if (i_j_lt1 == 0) break;
				if (data_j_k_gt == 0) break;

				oldvalue = j;
				//j--;
				if (0 == _mm512_cmpneq_epi32_mask(oldvalue,
					j = _mm512_mask_sub_epi32(j, i_j_lt0 & i_j_lt1 & data_j_k_gt, j, _stride)))
				{
					j = oldvalue;
					break;
				}
			}

			//if (i < j)
			{
				i_j_lt2 = _mm512_cmplt_epi32_mask(i, j);
				//data[i] = data[j];
				data_j = _mm512_i32gather_epi32(j, data, sizeof(data[0]));
				data_i = _mm512_i32gather_epi32(i, data, sizeof(data[0]));
				data_i = _mm512_mask_blend_epi32(i_j_lt0 & i_j_lt2, data_i, data_j);
				_mm512_mask_i32scatter_epi32(data, i_j_lt0 & i_j_lt2, i, data_i, sizeof(data[0]));
				//i++;
				i = _mm512_mask_add_epi32(i, i_j_lt0 & i_j_lt2, i, _stride);
			}

			//while (i < j && data[i] < k)
			while (true)
			{
				i_j_lt3 = _mm512_cmplt_epi32_mask(i, j);
				data_i = _mm512_i32gather_epi32(i, data, sizeof(data[0]));
				data_i_k_lt = _mm512_cmplt_epi32_mask(data_i, k);

				if (i_j_lt3 == 0) break;
				if (data_i_k_lt == 0) break;

				oldvalue = i;
				//i++;
				if (0 == _mm512_cmpneq_epi32_mask(oldvalue,
					i = _mm512_mask_add_epi32(i, i_j_lt0 & i_j_lt3 & data_i_k_lt, i, _stride)))
				{
					i = oldvalue;
					break;
				}
			}

			//if (i < j)
			//if(0!=(i_j_lt4 = _mm512_cmplt_epi32_mask(i, j)))
			{
				i_j_lt4 = _mm512_cmplt_epi32_mask(i, j);
				//data[j] = data[i];
				data_i = _mm512_i32gather_epi32(i, data, sizeof(data[0]));
				data_j = _mm512_i32gather_epi32(j, data, sizeof(data[0]));
				data_j = _mm512_mask_blend_epi32(i_j_lt0 & i_j_lt4, data_j, data_i);
				_mm512_mask_i32scatter_epi32(data, i_j_lt0 & i_j_lt4, j, data_j, sizeof(data[0]));
				//j--;
				j = _mm512_mask_sub_epi32(j, i_j_lt0 & i_j_lt4, j, _stride);
			}
		}
		//data[i] = k;
		_mm512_mask_i32scatter_epi32(data, ~0, i, k, sizeof(data[0]));
		//FastQuickSortP512(data, low, i - 1);
		//FastQuickSortP512(data, i + 1, high);		
		FastQuickSortV512(data, low, _mm512_sub_epi32(i, _stride));
		FastQuickSortV512(data, _mm512_add_epi32(i, _stride), high);
	}
	return true;

}
bool FastQuickSortV512(unsigned int data[], int n)
{
	const int stride = sizeof(__m512i) / sizeof(data[0]);
	if (n < stride || n % stride > 0) return false;
	__m512i low = _mm512_setr_epi32(
		0, 1, 2, 3, 4, 5, 6, 7,
		8, 9, 10, 11, 12, 13, 14, 15);
	__m512i high = _mm512_setr_epi32(
		n - 16, n - 15, n - 14, n - 13,
		n - 12, n - 11, n - 10, n - 9,
		n - 8, n - 7, n - 6, n - 5,
		n - 4, n - 3, n - 2, n - 1);

	if (FastQuickSortV512(data, low, high))
	{
		Merge(data, n, stride);
		return true;
	}
	return false;
}
