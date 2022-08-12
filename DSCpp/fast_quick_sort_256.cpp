#include "fast_sort_256.h"
#include "utils.h"

//Fast Quick Sort:
//  using AVX512 long stride to achive a faster speed than common quick sort
//
bool FastQuickSortH256(int data[], int low, int high) 
{
	const int stride = sizeof(__m256i) / sizeof(data[0]);
	if (low < high)
	{
		unsigned long index = 0;
		unsigned long i = low, j = high;
		int delta = 0;
		int k = data[i];
		__m256i t = _mm256_set1_epi32(data[i]);
		while (i < j)
		{
			while (i < j)
			{
				__m256i ds = _mm256_loadu_epi32(data + j - stride + 1);
				__mmask8 rt = _mm256_cmple_epi32_mask(ds, t);
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
				__m256i ds = _mm256_loadu_epi32(data + i);
				__mmask8 rt = _mm256_cmpge_epi32_mask(ds, t);
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
		FastQuickSortH256(data, low, i - 1);
		FastQuickSortH256(data, i + 1, high);
	}
	return true;
}
bool FastQuickSortH256(int data[], int n)
{
	const int stride = sizeof(__m256i) / sizeof(data[0]);
	if (n < stride || n % stride > 0) return false;
	return FastQuickSortH256(data, 0, n - 1);
}
bool FastQuickSortV256(int data[], __m256i low, __m256i high)
{
	const int stride = sizeof(__m256i) / sizeof(data[0]);
	//if (low < high)
	if (_mm256_cmplt_epi32_mask(low, high) == 0) 
		return true;
	else
	{
		__m256i _stride = _mm256_set1_epi32(stride);
		__m256i i = low, j = high;
		//k = data[i]
		__m256i k = _mm256_i32gather_epi32(data, i, sizeof(int));
		//while (i < j)
		__mmask8 i_j_lt0 = 0;
		__mmask8 i_j_lt1 = 0;
		__mmask8 i_j_lt2 = 0;
		__mmask8 i_j_lt3 = 0;
		__mmask8 i_j_lt4 = 0;
		__mmask8 data_j_k_gt = 0;
		__mmask8 data_i_k_lt = 0;
		__m256i data_j = { 0 };
		__m256i data_i = { 0 };
		__m256i oldvalue = { 0 };
		while(0!=(i_j_lt0 = _mm256_cmplt_epi32_mask(i, j)))
		{
			//while (i < j && data[j] > k)
			while(true)
			{
				i_j_lt1 = _mm256_cmplt_epi32_mask(i, j);
				data_j = _mm256_i32gather_epi32(data, j, sizeof(int));
				data_j_k_gt = _mm256_cmpgt_epi32_mask(data_j, k);
				if (i_j_lt1 == 0) break;
				if (data_j_k_gt == 0) break;

				oldvalue = j;
				//j--;
				if (0 == _mm256_cmpneq_epi32_mask(oldvalue,
					j = _mm256_mask_sub_epi32(j, i_j_lt0 & i_j_lt1 & data_j_k_gt, j, _stride)))
				{
					j = oldvalue;
					break;
				}
			}
			
			//if (i < j)
			{
				i_j_lt2 = _mm256_cmplt_epi32_mask(i, j);
				//data[i] = data[j];
				//data_j = _mm256_i32gather_epi32(data, j, sizeof(int));
				//data_i = _mm256_i32gather_epi32(data, i, sizeof(int));
				data_i = _mm256_mask_blend_epi32(i_j_lt0 & i_j_lt2, data_i, data_j);
				_mm256_mask_i32scatter_epi32(data, i_j_lt0 & i_j_lt2, i, data_i, sizeof(int));
				//i++;
				i = _mm256_mask_add_epi32(i, i_j_lt0 & i_j_lt2, i, _stride);
			}

			//while (i < j && data[i] < k)
			while(true)
			{
				i_j_lt3 = _mm256_cmplt_epi32_mask(i, j);
				data_i = _mm256_i32gather_epi32(data, i, sizeof(int));
				data_i_k_lt = _mm256_cmplt_epi32_mask(data_i, k);
				
				if (i_j_lt3 == 0) break;
				if (data_i_k_lt == 0) break;

				oldvalue = i;
				//i++;
				if (0 == _mm256_cmpneq_epi32_mask(oldvalue,
					i = _mm256_mask_add_epi32(i, i_j_lt0 & i_j_lt3 & data_i_k_lt, i, _stride)))
				{
					i = oldvalue;
					break;
				}
			}
			
			//if (i < j)
			//if(0!=(i_j_lt4 = _mm256_cmplt_epi32_mask(i, j)))
			{
				i_j_lt4 = _mm256_cmplt_epi32_mask(i, j);
				//data[j] = data[i];
				//data_i = _mm256_i32gather_epi32(data, i, sizeof(int));
				//data_j = _mm256_i32gather_epi32(data, j, sizeof(int));
				data_j = _mm256_mask_blend_epi32(i_j_lt0 & i_j_lt4, data_j, data_i);
				_mm256_mask_i32scatter_epi32(data, i_j_lt0 & i_j_lt4, j, data_j, sizeof(int));
				//j--;
				j = _mm256_mask_sub_epi32(j, i_j_lt0 & i_j_lt4, j, _stride);
			}
		}
		//data[i] = k;
		_mm256_mask_i32scatter_epi32(data, ~0, i, k, sizeof(int));
		//FastQuickSortP256(data, low, i - 1);
		//FastQuickSortP256(data, i + 1, high);		
		FastQuickSortV256(data, low, _mm256_sub_epi32(i, _stride));
		FastQuickSortV256(data, _mm256_add_epi32(i,_stride), high);
	}
	return true;

}
bool FastQuickSortV256(int data[], int n)
{
	const int stride = sizeof(__m256i) / sizeof(data[0]);
	if (n < stride || n % stride > 0) return false;
	__m256i low = _mm256_setr_epi32(
		0, 1, 2, 3, 4, 5, 6, 7);
	__m256i high = _mm256_setr_epi32(
		n - 8, n - 7, n - 6, n - 5,
		n - 4, n - 3, n - 2, n - 1);

	if (FastQuickSortV256(data,low , high))
	{
		Merge(data, n, stride);
		return true;
	}
	return false;
}
