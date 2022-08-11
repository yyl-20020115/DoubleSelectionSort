#include "fast_sort_256.h"
#include "utils.h"

//Fast Quick Sort:
//  using AVX512 long stride to achive a faster speed than common quick sort
//
bool FastQuickSortImpl256(int data[], int low, int high) {
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
		FastQuickSortImpl256(data, low, i - 1);
		FastQuickSortImpl256(data, i + 1, high);
	}
	return true;
}
bool FastQuickSort256(int data[], int n)
{
	const int stride = sizeof(__m256i) / sizeof(data[0]);
	if (n % stride > 0) return false;

	return FastQuickSortImpl256(data, 0, n - 1);
}
bool FastQuickSortImpl512(int data[], int low, int high) {
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
		FastQuickSortImpl512(data, low, i - 1);
		FastQuickSortImpl512(data, i + 1, high);
	}
	return true;
}
bool FastQuickSort512(int data[], int n)
{
	const int stride = sizeof(__m512i) / sizeof(data[0]);
	if (n % stride > 0) return false;

	return FastQuickSortImpl512(data, 0, n - 1);
}
