﻿#include <iostream>
#include <intrin.h>
#include <algorithm>

void do_print(__m512i data, bool nl = false) {
	bool first = true;
	printf("[");
	for (int i = 0; i < sizeof(data.m512i_u32) / sizeof(int); i++) {
		if (!first) {
			printf(", ");
		}
		printf("%08X", data.m512i_i32[i]);
		first = false;
	}
	printf("]");
	if (nl)printf("\n");
}
void do_print(__m256i data, bool nl = false) {
	bool first = true;
	printf("[");
	for (int i = 0; i < sizeof(data.m256i_u32) / sizeof(int); i++) {
		if (!first) {
			printf(", ");
		}
		printf("%08X", data.m256i_u32[i]);
		first = false;
	}
	printf("]");
	if (nl)printf("\n");
}
void do_print(__m128i data, bool nl = false) {
	bool first = true;
	printf("[");
	for (int i = 0; i < sizeof(data.m128i_u32) / sizeof(int); i++) {
		if (!first) {
			printf(", ");
		}
		printf("%08X", data.m128i_u32[i]);
		first = false;
	}
	printf("]");
	if (nl)printf("\n");
}
void do_print(int data[], int N, bool nl = false) {
	bool first = true;
	printf("[");
	for (int i = 0; i < N; i++) {
		if (!first) {
			printf(", ");
		}
		printf("%08X", data[i]);
		first = false;
	}
	printf("]");
	if (nl)printf("\n");
}
void QuickSortImpl(int* data, int low, int high)
{
	if (low < high)
	{
		int i = low, j = high;
		int k = data[i];
		while (i < j)
		{
			while (i < j && data[j] > k)
				--j;
			if (i < j)
				data[i++] = data[j];
			while (i < j && data[i] < k)
				++i;
			if (i < j)
				data[j--] = data[i];
		}
		data[i] = k;
		QuickSortImpl(data, low, i - 1);
		QuickSortImpl(data, i + 1, high);
	}
}
void QuickSort(int data[], int n)
{
	QuickSortImpl(data, 0, n - 1);
}
//Fast Quick Sort:
//  using AVX512 long stride to achive a faster speed than common quick sort
//
bool FastQuickSortImpl256(int data[], int low, int high) {
	const int stride = sizeof(__m256i) / sizeof(int);
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
				__mmask16 rt = _mm256_cmple_epi32_mask(ds, t);

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
				__mmask16 rt = _mm256_cmpge_epi32_mask(ds, t);
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
	const int stride = sizeof(__m256i) / sizeof(int);
	if (n % stride > 0) return false;

	return FastQuickSortImpl256(data, 0, n - 1);
}
bool FastQuickSortImpl512(int data[], int low, int high) {
	const int stride = sizeof(__m512i) / sizeof(int);
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
	const int stride = sizeof(__m512i) / sizeof(int);
	if (n % stride > 0) return false;

	return FastQuickSortImpl512(data, 0, n - 1);
}
void Swap(int* a, int i, int j)
{
	int t = a[i];
	a[i] = a[j];
	a[j] = t;
}
void OddEvenSort(int* a, int n)
{
	for (int c = 0; c < n; c++)
	{
		int p = c % 2;
		for (int d = 0; d < n - p - p; d += 2)
		{
			//this is for odd length
			if (d + p + 1 >= n) break;
			//this step should be in parallel
			if (a[d + p + 0] > a[d + p + 1])
			{
				Swap(a, d + p + 0, d + p + 1);
			}
		}
	}
}
int* FastOddEvenSort256(int* t, int n)
{
	const int size = sizeof(__m256i) / sizeof(int);
	const int last = size - 1;
	const int half = size >> 1;
	const int skip = sizeof(int);
	const int dbls = (size << 1);
	const int quad = dbls << 1;

	if (n < dbls || n % dbls>0) {
		std::sort(t, t + n);
		return t;
	}
	int* a = new int[n + 1];
	memcpy_s(a, (n) * sizeof(int), t, (n) * sizeof(int));
	a[n] = 0;

	__m256i zeros = _mm256_setzero_si256();
	__m256i ones = _mm256_cmpeq_epi32(zeros, zeros);
	__m256i ipo = _mm256_setr_epi32(1, 3, 5, 7, 9, 11, 13, 15);
	__m256i ipe = _mm256_setr_epi32(0, 2, 4, 6, 8, 10, 12, 14);
	__m256i ipt = _mm256_setr_epi32(2, 4, 6, 8, 10, 12, 14, 16);

	for (int repeat = 0; repeat < n / 2; repeat++)
	{
		for (int part = 0; part < n; part += dbls)
		{
			int* ptr = a + part;

			__m256i veven = _mm256_i32gather_epi32(ptr, ipe, skip);
			__m256i vodd_ = _mm256_i32gather_epi32(ptr, ipo, skip);

			__m256i min = _mm256_min_epi32(vodd_, veven);
			__m256i max = _mm256_max_epi32(vodd_, veven);

			__m256i minlows256 = _mm256_cvtepi32_epi64(_mm256_extracti32x4_epi32(min, 0));
			__m256i minhighs256 = _mm256_cvtepi32_epi64(_mm256_extracti32x4_epi32(min, 1));
			__m256i maxlows256 = _mm256_cvtepi32_epi64(_mm256_extracti32x4_epi32(max, 0));
			__m256i maxhighs256 = _mm256_cvtepi32_epi64(_mm256_extracti32x4_epi32(max, 1));

			__m256i lows = _mm256_mask_alignr_epi32(
				minlows256, 0b10101010, maxlows256, maxhighs256, 7);

			__m256i highs = _mm256_mask_alignr_epi32(
				minhighs256, 0b10101010, maxhighs256, maxlows256, 7);

			_mm256_storeu_epi32(ptr, lows);
			_mm256_storeu_epi32(ptr+ size, highs);

			//_mm256_i32scatter_epi32(ptr, ipe, min, skip);
			//_mm256_i32scatter_epi32(ptr, ipo, max, skip);
		}
		for (int part = 0; part < n; part += dbls)
		{
			if (dbls >= n - part - 1) a[n] = a[n - 1];

			int* ptr = a + part;

			__m256i veven = _mm256_i32gather_epi32(ptr, ipt, skip);
			__m256i vodd_ = _mm256_i32gather_epi32(ptr, ipo, skip);

			__m256i min = _mm256_min_epi32(vodd_, veven);
			__m256i max = _mm256_max_epi32(vodd_, veven);

			__m256i minlows256 = _mm256_cvtepi32_epi64(_mm256_extracti32x4_epi32(min, 0));
			__m256i minhighs256 = _mm256_cvtepi32_epi64(_mm256_extracti32x4_epi32(min, 1));
			__m256i maxlows256 = _mm256_cvtepi32_epi64(_mm256_extracti32x4_epi32(max, 0));
			__m256i maxhighs256 = _mm256_cvtepi32_epi64(_mm256_extracti32x4_epi32(max, 1));

			__m256i lows = _mm256_mask_alignr_epi32(
				minlows256, 0b10101010, maxlows256, maxhighs256, 7);

			__m256i highs = _mm256_mask_alignr_epi32(
				minhighs256, 0b10101010, maxhighs256, maxlows256, 7);

			_mm256_storeu_epi32(ptr + 1, lows);
			_mm256_storeu_epi32(ptr + 1 + size, highs);

			//_mm256_i32scatter_epi32(ptr, ipo, min, skip);
			//_mm256_i32scatter_epi32(ptr, ipt, max, skip);
		}
	}

	memcpy_s(t, (n) * sizeof(int), a, (n) * sizeof(int));

	delete[] a;

	return t;
}
int* FastOddEvenSort512(int* t, int n)
{
	const int size = sizeof(__m512i) / sizeof(int);
	const int last = size - 1;
	const int half = size >> 1;
	const int skip = sizeof(int);
	const int dbls = (size << 1);
	const int quad = dbls << 1;

	if (n < dbls || n % dbls>0) {
		std::sort(t, t + n);
		return t;
	}
	int* a = new int[n + 1];
	memcpy_s(a, (n) * sizeof(int), t, (n) * sizeof(int));
	a[n] = 0;

	__m256i zeros = _mm256_setzero_si256();
	__m256i ones = _mm256_cmpeq_epi32(zeros, zeros);
	__m512i ipo = _mm512_setr_epi32(1, 3, 5, 7, 9, 11, 13, 15, 17, 19, 21, 23, 25, 27, 29, 31);
	__m512i ipe = _mm512_setr_epi32(0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30);
	__m512i ipt = _mm512_setr_epi32(2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32);

	for (int repeat = 0; repeat < n / 2; repeat++)
	{
		for (int part = 0; part < n; part += dbls)
		{
			int* ptr = a + part;

			__m512i veven = _mm512_i32gather_epi32(ipe, ptr, skip);
			__m512i vodd_ = _mm512_i32gather_epi32(ipo, ptr, skip);

			__m512i min = _mm512_min_epi32(vodd_, veven);
			__m512i max = _mm512_max_epi32(vodd_, veven);

			__m512i minlows512 = _mm512_cvtepi32_epi64(_mm512_extracti32x8_epi32(min, 0));
			__m512i minhighs512 = _mm512_cvtepi32_epi64(_mm512_extracti32x8_epi32(min, 1));
			__m512i maxlows512 = _mm512_cvtepi32_epi64(_mm512_extracti32x8_epi32(max, 0));
			__m512i maxhighs512 = _mm512_cvtepi32_epi64(_mm512_extracti32x8_epi32(max, 1));

			__m512i lows = _mm512_mask_alignr_epi32(
				minlows512, 0b1010101010101010, maxlows512, maxhighs512, 15);

			__m512i highs = _mm512_mask_alignr_epi32(
				minhighs512, 0b1010101010101010, maxhighs512, maxlows512, 15);

			_mm512_storeu_epi32(ptr, lows);
			_mm512_storeu_epi32(ptr + size, highs);

			//_mm512_i32scatter_epi32(ptr, ipe, min, skip);
			//_mm512_i32scatter_epi32(ptr, ipo, max, skip);
		}
		for (int part = 0; part < n; part += dbls)
		{
			if (dbls >= n - part - 1) a[n] = a[n - 1];

			int* ptr = a + part;

			__m512i veven = _mm512_i32gather_epi32(ipt, ptr, skip);
			__m512i vodd_ = _mm512_i32gather_epi32(ipo, ptr, skip);

			__m512i min = _mm512_min_epi32(vodd_, veven);
			__m512i max = _mm512_max_epi32(vodd_, veven);

			__m512i minlows512 = _mm512_cvtepi32_epi64(_mm512_extracti32x8_epi32(min, 0));
			__m512i minhighs512 = _mm512_cvtepi32_epi64(_mm512_extracti32x8_epi32(min, 1));
			__m512i maxlows512 = _mm512_cvtepi32_epi64(_mm512_extracti32x8_epi32(max, 0));
			__m512i maxhighs512 = _mm512_cvtepi32_epi64(_mm512_extracti32x8_epi32(max, 1));

			__m512i lows = _mm512_mask_alignr_epi32(
				minlows512, 0b1010101010101010, maxlows512, maxhighs512, 15);

			__m512i highs = _mm512_mask_alignr_epi32(
				minhighs512, 0b1010101010101010, maxhighs512, maxlows512, 15);

			_mm512_storeu_epi32(ptr + 1, lows);
			_mm512_storeu_epi32(ptr + 1 + size, highs);

			//_mm512_i32scatter_epi32(ptr, ipo, min, skip);
			//_mm512_i32scatter_epi32(ptr, ipt, max, skip);
		}
	}

	memcpy_s(t, (n) * sizeof(int), a, (n) * sizeof(int));

	delete[] a;

	return t;

}

int HorizentalMin16(__m128i data, unsigned short* p = 0) {
	__m128i result = _mm_minpos_epu16(data);
	if (p != 0) {
		*p = result.m128i_i16[0];
	}
	return result.m128i_i16[1] & 0x7;
}
int HorizentalMax16(__m128i data, unsigned short* p = 0) {
	__m128i zero = _mm_setzero_si128();
	__m128i ones = _mm_cmpeq_epi32(zero, zero);
	__m128i subs = _mm_subs_epu16(ones, data);
	__m128i result = _mm_minpos_epu16(subs);
	int index = result.m128i_i16[1] & 0x7;
	if (p != 0) {
		*p = data.m128i_i16[index];
	}
	return index;
}
int HorizentalMin8(__m128i data, unsigned char* p = 0) {
	__m128i zero = _mm_setzero_si128();
	__m128i low = _mm_unpacklo_epi8(data, zero);
	__m128i high = _mm_unpackhi_epi8(data, zero);
	unsigned short lv = 0;
	unsigned short hv = 0;
	int li = HorizentalMin16(low, &lv);
	int hi = HorizentalMin16(high, &hv);
	if (lv <= hv) {
		if (p != 0) *p = (unsigned char)(lv & 0xff);
		return li;
	}
	else {
		if (p != 0) *p = (unsigned char)(hv & 0xff);
		return 8 + hi;
	}
}
int HorizentalMax8(__m128i data, unsigned char* p = 0) {
	__m128i zero = _mm_setzero_si128();
	__m128i low = _mm_unpacklo_epi8(data, zero);
	__m128i high = _mm_unpackhi_epi8(data, zero);
	unsigned short lv = 0;
	unsigned short hv = 0;
	int li = HorizentalMax16(low, &lv);
	int hi = HorizentalMax16(high, &hv);
	if (lv >= hv) {
		if (p != 0) *p = (unsigned char)(lv & 0xff);
		return li;
	}
	else {
		if (p != 0) *p = (unsigned char)(hv & 0xff);
		return 8 + hi;
	}
}
int HorizentalMin8(__m256i data, unsigned char* p = 0) {
	__m128i zero = _mm_setzero_si128();
	__m128i low = _mm256_extracti128_si256(data, 0);
	__m128i high = _mm256_extracti128_si256(data, 1);
	unsigned short lv = 0;
	unsigned short hv = 0;
	int li = HorizentalMin16(low, &lv);
	int hi = HorizentalMin16(high, &hv);
	if (lv <= hv) {
		if (p != 0) *p = (unsigned char)(lv & 0xff);
		return li;
	}
	else {
		if (p != 0) *p = (unsigned char)(hv & 0xff);
		return 16 + hi;
	}
}
int HorizentalMax8(__m256i data, unsigned char* p = 0) {
	__m128i zero = _mm_setzero_si128();
	__m128i low = _mm256_extractf128_si256(data, 0);
	__m128i high = _mm256_extractf128_si256(data, 1);
	unsigned short lv = 0;
	unsigned short hv = 0;
	int li = HorizentalMax16(low, &lv);
	int hi = HorizentalMax16(high, &hv);
	if (lv >= hv) {
		if (p != 0) *p = (unsigned char)(lv & 0xff);
		return li;
	}
	else {
		if (p != 0) *p = (unsigned char)(hv & 0xff);
		return 16 + hi;
	}
}
int HorizentalMin8(__m512i data, unsigned char* p = 0) {
	__m256i low = _mm512_extracti64x4_epi64(data, 0);
	__m256i high = _mm512_extracti64x4_epi64(data, 1);
	unsigned char lv = 0;
	unsigned char hv = 0;
	int li = HorizentalMin8(low, &lv);
	int hi = HorizentalMin8(high, &hv);
	if (lv <= hv) {
		if (p != 0) *p = lv;
		return li;
	}
	else {
		if (p != 0) *p = hv;
		return 32 + hi;
	}
}
int HorizentalMax8(__m512i data, unsigned char* p = 0) {
	__m256i low = _mm512_extracti64x4_epi64(data, 0);
	__m256i high = _mm512_extracti64x4_epi64(data, 1);
	unsigned char lv = 0;
	unsigned char hv = 0;
	int li = HorizentalMax8(low, &lv);
	int hi = HorizentalMax8(high, &hv);
	if (lv >= hv) {
		if (p != 0) *p = lv;
		return li;
	}
	else {
		if (p != 0) *p = hv;
		return 32 + hi;
	}
}
int HorizentalMin32(__m256i data, unsigned int* p = 0) {
	const int stride = sizeof(__m256i) / sizeof(int);
	unsigned long index = stride; //8 is out of range
	__m128i zero = _mm_setzero_si128();
	__m128i ones = _mm_cmpeq_epi32(zero, zero);	
	__m256i indices = _mm256_setr_epi16(0, 2, 4, 6, 8, 10, 12, 14, 1, 3, 5, 7, 9, 11, 13, 15);
	__m256i permution = _mm256_permutexvar_epi16(indices, data);	
	__m128i _low = _mm256_extracti128_si256(permution, 0);
	__m128i high = _mm256_extracti128_si256(permution, 1);
	
	__m128i high_result = _mm_minpos_epu16(high);

	short high_value = high_result.m128i_i16[0];
	short high_index = high_result.m128i_i16[1] & 0b00000111;
	__m128i all_high = _mm_set1_epi16(high_value);
	__mmask8 mask = _mm_cmpeq_epi16_mask(high, all_high);

	__m128i low_compressed = _mm_maskz_compress_epi16(mask, _low);
	__m128i low_result = _mm_minpos_epu16(low_compressed);
	short low_value = low_result.m128i_i16[0];
	if (p != 0) {
		*p = low_value;
	}
	__m128i all_lows = _mm_set1_epi16(low_value);
	mask = _mm_cmpeq_epi16_mask(_low, all_lows);
	if (_BitScanForward(&index, mask)) {
		return index;
	}

	return index;
}
int HorizentalMax32(__m256i data, unsigned int* p = 0) {
	__m128i zero = _mm_setzero_si128();
	__m128i ones = _mm_cmpeq_epi32(zero, zero);
	__m256i zero_ = _mm256_setzero_si256();
	__m256i ones_ = _mm256_cmpeq_epi32(zero_, zero_);

	__m256i result = _mm256_andnot_si256(data, ones_);
	int index = HorizentalMin32(result,p);
	if (p != 0) {
		*p = data.m256i_u32[index];
	}
	return index;
}
int HorizentalMin32(__m512i data, unsigned int* p = 0)
{
	__m256i low = _mm512_extracti32x8_epi32(data, 0);
	__m256i high = _mm512_extracti32x8_epi32(data, 1);

	unsigned int lv = 0;
	unsigned int hv = 0;

	int li = HorizentalMin32(low, &lv);
	int hi = HorizentalMin32(low, &hv);
	if (lv <= hv) {
		if (p != 0)*p = lv;
		return li;
	}
	else //hv<lv
	{
		if (p != 0)*p = hv;
		return 8 + hi;
	}
}
int HorizentalMax32(__m512i data, unsigned int* p = 0)
{
	__m256i low = _mm512_extracti32x8_epi32(data, 0);
	__m256i high = _mm512_extracti32x8_epi32(data, 1);

	unsigned int lv = 0;
	unsigned int hv = 0;

	int li = HorizentalMax32(low, &lv);
	int hi = HorizentalMax32(low, &hv);
	if (lv >= hv) {
		if (p != 0)*p = lv;
		return li;
	}
	else
	{
		if (p != 0)*p = hv;
		return 8 + hi;
	}
}
int HorizentalMin64(__m128i data, unsigned long long* p = 0)
{
	unsigned long long lv = _mm_extract_epi64(data, 0);
	unsigned long long hv = _mm_extract_epi64(data, 1);
	if (lv <= hv) {
		if (p != 0) *p = lv;
		return 0;
	}
	else {
		if (p != 0) *p = hv;
		return 1;
	}
}
int HorizentalMax64(__m128i data, unsigned long long* p = 0)
{
	unsigned long long lv = _mm_extract_epi64(data, 0);
	unsigned long long hv = _mm_extract_epi64(data, 1);
	if (lv >= hv) {
		if (p != 0) *p = lv;
		return 0;
	}
	else {
		if (p != 0) *p = hv;
		return 1;
	}
}
int HorizentalMin64(__m256i data, unsigned long long* p = 0)
{
	__m128i low = _mm256_extracti64x2_epi64(data, 0);
	__m128i high = _mm256_extracti64x2_epi64(data, 0);

	unsigned long long lv = 0;
	unsigned long long hv = 0;

	int li = HorizentalMin64(low, &lv);
	int hi = HorizentalMin64(high, &hv);
	if (lv <= hv) {
		if (p != 0) *p = lv;
		return li;
	}
	else {
		if (p != 0)*p = hv;
		return hi + 2;
	}
}
int HorizentalMax64(__m256i data, unsigned long long* p = 0)
{
	__m128i low = _mm256_extracti64x2_epi64(data, 0);
	__m128i high = _mm256_extracti64x2_epi64(data, 0);

	unsigned long long lv = 0;
	unsigned long long hv = 0;

	int li = HorizentalMax64(low, &lv);
	int hi = HorizentalMax64(high, &hv);
	if (lv >= hv) {
		if (p != 0) *p = lv;
		return li;
	}
	else {
		if (p != 0)*p = hv;
		return hi + 2;
	}

}
int HorizentalMin64(__m512i data, unsigned long long* p = 0)
{
	__m256i low = _mm512_extracti64x4_epi64(data, 0);
	__m256i high = _mm512_extracti64x4_epi64(data, 1);

	unsigned long long lv = 0;
	unsigned long long hv = 0;

	int li = HorizentalMin64(low, &lv);
	int hi = HorizentalMin64(high, &hv);
	if (lv <= hv) {
		if (p != 0) *p = lv;
		return li;
	}
	else {
		if (p != 0)*p = hv;
		return hi + 4;
	}
}
int HorizentalMax64(__m512i data, unsigned long long* p = 0)
{
	__m256i low = _mm512_extracti64x4_epi64(data, 0);
	__m256i high = _mm512_extracti64x4_epi64(data, 1);

	unsigned long long lv = 0;
	unsigned long long hv = 0;

	int li = HorizentalMin64(low, &lv);
	int hi = HorizentalMin64(high, &hv);
	if (lv >= hv) {
		if (p != 0) *p = lv;
		return li;
	}
	else {
		if (p != 0)*p = hv;
		return hi + 4;
	}
}
__m256i HorizentalSort32(__m256i data, unsigned int* pmin = 0, unsigned int* pmax = 0) {
	const int stride = sizeof(__m256i) / sizeof(int);
	__m256i result = { 0 };
	unsigned int min = 0;
	unsigned int max = 0;

	int imin = HorizentalMin32(data, &min);
	int imax = HorizentalMax32(data, &max);
	if (pmin != 0)*pmin = min;
	if (pmax != 0)*pmax = max;
	result.m256i_u32[0] = min;
	result.m256i_u32[stride - 1] = max;
	data.m256i_u32[imin] = max;
	//min is set to max, we can find next min
	for (int i = 1; i < stride - 1; i++)
	{
		imin = HorizentalMin32(data, &min);
		result.m256i_u32[i] = min;
		data.m256i_u32[imin] = max;
	}

	return result;
}
__m512i HorizentalSort32(__m512i data, unsigned int* pmin = 0, unsigned int* pmax = 0) {
	const int stride = sizeof(__m512i) / sizeof(int);
	__m512i result = { 0 };
	int imin = HorizentalMin32(data, pmin);
	int imax = HorizentalMax32(data, pmax);
	unsigned int min = data.m512i_u32[imin];
	unsigned int max = data.m512i_u32[imax];
	if (pmin != 0)*pmin = min;
	if (pmax != 0)*pmax = max;
	result.m512i_u32[0] = min;
	result.m512i_u32[stride - 1] = max;
	data.m512i_u32[imin] = max;
	//min is set to max, we can find next min
	for (int i = 1; i <  stride - 1; i++)
	{
		imin = HorizentalMin32(data, &min);
		result.m512i_u32[i] = min;
		data.m512i_u32[imin] = max;
	}

	return result;
}

bool FastMergeSort256(int data[], int n) {
	const int stride = sizeof(__m256i) / sizeof(int);
	const int dual = stride << 1;
	if (data == 0 || n<dual || n % dual > 0)
		return false;
	int* buffer = new int[n];

	int gap = stride;
	while (gap < n) {
		int gap2 = gap << 1;
		for (int i = 0; i < n; i += gap2) {
			if (gap == stride) {
				__m256i values = _mm256_loadu_epi32(data + i);
				__m256i sorted = HorizentalSort32(values);
					
				_mm256_storeu_epi32(data + i, sorted);

				values = _mm256_loadu_epi32(data + i + stride);
				sorted = HorizentalSort32(values);

				_mm256_storeu_epi32(data + i + stride, sorted);

			}
			else {
				int i_left = i;
				int i_middle = i + gap;
				int i_right = i_middle + gap;
				for (int j = i_left; j < i_middle; j += stride) {
					__m256i left = _mm256_loadu_epi32(data + j);
					__m256i right = _mm256_loadu_epi32(data + j + i_middle);
					__m256i min = _mm256_min_epi32(left, right);
					__m256i max = _mm256_min_epi32(left, right);
					_mm256_storeu_epi32(buffer + j, min);
					_mm256_storeu_epi32(buffer + j + i_middle, min);
				}
				memcpy_s(data + i, sizeof(int) * gap2, buffer + i, sizeof(int) * gap2);
			}
		}
		gap = gap2;
	}
	delete[] buffer;
	return true;
}

int AVX2_SUM(int data[], size_t size)
{
	const int stride = sizeof(__m256i) / sizeof(int);
	int sum[stride] = { 0 };
	__m256i sum256 = _mm256_setzero_si256();
	__m256i load256 = _mm256_setzero_si256();
	for (size_t i = 0; i < size; i += stride)
	{
		load256 = _mm256_loadu_si256((__m256i*) & data[i]);
		sum256 = _mm256_add_epi32(sum256, load256);
	}
	sum256 = _mm256_hadd_epi32(sum256, sum256);
	sum256 = _mm256_hadd_epi32(sum256, sum256);
	_mm256_storeu_si256((__m256i*)sum, sum256);
	return sum[0] + sum[4];
}
int AVX512_SUM(int data[], size_t size)
{
	const int stride = sizeof(__m512i) / sizeof(int);
	int sum[stride] = { 0 };
	__m512i sum512 = _mm512_setzero_si512();
	__m512i load512 = _mm512_setzero_si512();
	for (size_t i = 0; i < size; i += stride)
	{
		load512 = _mm512_loadu_si512((__m512i*)&data[i]);
		sum512 = _mm512_add_epi32(sum512, load512);
	}
	__m256i sum256_low = _mm512_extracti32x8_epi32(sum512, 0);
	__m256i sum256_high = _mm512_extracti32x8_epi32(sum512, 1);

	sum256_low = _mm256_hadd_epi32(sum256_low, sum256_low);
	sum256_low = _mm256_hadd_epi32(sum256_low, sum256_low);

	sum256_high = _mm256_hadd_epi32(sum256_high, sum256_high);
	sum256_high = _mm256_hadd_epi32(sum256_high, sum256_high);

	__m256i sum256_full = _mm256_add_epi32(sum256_low, sum256_high);
	_mm256_storeu_si256((__m256i*)sum, sum256_full);

	return sum[0] + sum[4];
}
long long AVX2_SUM(long long data[], size_t size)
{
	const int stride = sizeof(__m256i) / sizeof(int);
	long long sum[stride] = { 0 };
	__m256i sum256 = _mm256_setzero_si256();
	__m256i load256 = _mm256_setzero_si256();
	for (size_t i = 0; i < size; i += stride)
	{
		load256 = _mm256_loadu_si256((__m256i*) & data[i]);
		sum256 = _mm256_add_epi64(sum256, load256);
	}
	_mm256_storeu_si256((__m256i*)sum, sum256);
	return sum[0] + sum[1] + sum[2] + sum[3];
}
long long AVX512_SUM(long long data[], size_t size)
{
	const int stride = sizeof(__m512i) / sizeof(int);
	long long sum[stride >> 1] = { 0 };
	__m512i sum512 = _mm512_setzero_si512();
	__m512i load512 = _mm512_setzero_si512();
	for (size_t i = 0; i < size; i += stride)
	{
		load512 = _mm512_loadu_si512((__m512i*)&data[i]);
		sum512 = _mm512_add_epi64(sum512, load512);
	}
	__m256i sum256_low = _mm512_extracti64x4_epi64(sum512, 0);
	__m256i sum256_high = _mm512_extracti64x4_epi64(sum512, 1);
	__m256i sum256_full = _mm256_add_epi64(sum256_low, sum256_high);
	_mm256_storeu_si256((__m256i*)sum, sum256_full);

	return sum[0] + sum[1] + sum[2] + sum[3];
}
size_t AVX2_StringLength(char* s)
{
	size_t len = 0;
	if (s != 0) {
		const int stride = sizeof(__m256i) / sizeof(int);
		unsigned long index = 0;
		__m256i zero = _mm256_setzero_si256();
		__m256i part = { 0 };
		char* p = s;
		while (len <= (~0LL) - stride) {
			part = _mm256_loadu_epi8(p);
			__mmask32 result = _mm256_cmpeq_epi8_mask(part, zero);
			if (_BitScanForward(&index, result))
			{
				len += index;
				break;
			}
			else {
				len += stride;
				p += stride;
			}
		}
	}

	return len;
}
size_t AVX2_StringLength(wchar_t* s)
{
	size_t len = 0;
	if (s != 0) {
		const int stride = sizeof(__m256i) / sizeof(int);
		unsigned long index = 0;
		__m256i zero = _mm256_setzero_si256();
		__m256i part = { 0 };
		wchar_t* p = s;
		while (len <= (~0LL) - stride) {
			part = _mm256_loadu_epi16(p);
			__mmask16 result = _mm256_cmpeq_epi16_mask(part, zero);
			if (_BitScanForward(&index, result))
			{
				len += index;
				break;
			}
			else {
				len += stride;
				p += stride;
			}
		}
	}

	return len;
}
size_t AVX512_StringLength(char* s)
{
	size_t len = 0;
	if (s != 0) {
		const int stride = sizeof(__m512i) / sizeof(int);
		unsigned long index = 0;
		__m512i zero = _mm512_setzero_si512();
		__m512i part = { 0 };
		char* p = s;
		while (len <= (~0LL) - stride) {
			part = _mm512_loadu_epi8(p);
			__mmask64 result = _mm512_cmpeq_epi8_mask(part, zero);
			if (_BitScanForward64(&index, result))
			{
				len += index;
				break;
			}
			else {
				len += stride;
				p += stride;
			}
		}
	}

	return len;
}
size_t AVX512_StringLength(wchar_t* s)
{
	size_t len = 0;
	if (s != 0) {
		const int stride = sizeof(__m512i) / sizeof(int);
		unsigned long index = 0;
		__m512i zero = _mm512_setzero_si512();
		__m512i part = { 0 };
		wchar_t* p = s;
		while (len <= (~0LL) - stride) {
			part = _mm512_loadu_epi16(p);
			__mmask32 result = _mm512_cmpeq_epi16_mask(part, zero);
			if (_BitScanForward(&index, result))
			{
				len += index;
				break;
			}
			else {
				len += stride;
				p += stride;
			}
		}
	}

	return len;
}

int AVX2_StringCompare(char* s1, char* s2)
{
	const int stride = sizeof(__m256i) / sizeof(int);

	if (s1 == 0 && s2 == 0) {
		return 0;
	}
	else if (s2 == 0) {
		return +1;
	}
	else if (s1 == 0) {
		return -1;
	}
	else {
		size_t l1 = AVX2_StringLength(s1);
		size_t l2 = AVX2_StringLength(s2);
		if (l1 == l2 && l2 == 0) return 0;
		size_t ln = l1 > l2 ? l1 : l2;

		for (unsigned long i = 0; i < ln; i += stride)
		{
			unsigned long igt = 0;
			unsigned long ilt = 0;
			unsigned long most = i + stride < ln ? stride : ln - i;
			__m256i part1 = _mm256_loadu_epi8(s1 + i);
			__m256i part2 = _mm256_loadu_epi8(s2 + i);
			__mmask32 rgt = _mm256_cmpgt_epi8_mask(part1, part2);
			__mmask32 rlt = _mm256_cmplt_epi8_mask(part1, part2);
			unsigned char bgt = _BitScanForward(&igt, rgt);
			unsigned char blt = _BitScanForward(&ilt, rlt);
			if (!bgt && !blt) continue;
			
			ilt = ilt > most ? most : ilt;
			igt = igt > most ? most : igt;
			if (bgt && !blt) {
				return i + igt;
			}
			else if (!bgt && blt) {
				return i + ilt;
			}
			else {
				return igt < ilt ? igt : ilt;
			}
		}
		return 0;//all the same
	}
}
int AVX2_StringCompare(wchar_t* s1, wchar_t* s2)
{
	const int stride = sizeof(__m256i) / sizeof(int);

	if (s1 == 0 && s2 == 0) {
		return 0;
	}
	else if (s2 == 0) {
		return +1;
	}
	else if (s1 == 0) {
		return -1;
	}
	else {
		size_t l1 = AVX2_StringLength(s1);
		size_t l2 = AVX2_StringLength(s2);
		if (l1 == l2 && l2 == 0) return 0;
		size_t ln = l1 > l2 ? l1 : l2;

		for (unsigned long i = 0; i < ln; i += stride)
		{
			unsigned long igt = 0;
			unsigned long ilt = 0;
			unsigned long most = i + stride < ln ? stride : ln - i;
			__m256i part1 = _mm256_loadu_epi16(s1 + i);
			__m256i part2 = _mm256_loadu_epi16(s2 + i);
			__mmask16 rgt = _mm256_cmpgt_epi16_mask(part1, part2);
			__mmask16 rlt = _mm256_cmplt_epi16_mask(part1, part2);
			unsigned char bgt = _BitScanForward(&igt, rgt);
			unsigned char blt = _BitScanForward(&ilt, rlt);
			if (!bgt && !blt) continue;

			ilt = ilt > most ? most : ilt;
			igt = igt > most ? most : igt;
			if (bgt && !blt) {
				return i + igt;
			}
			else if (!bgt && blt) {
				return i + ilt;
			}
			else {
				return igt < ilt ? igt : ilt;
			}
		}
		return 0;//all the same
	}
}
int AVX512_StringCompare(char* s1, char* s2)
{
	const int stride = sizeof(__m512i) / sizeof(int);
	
	if (s1 == 0 && s2 == 0) {
		return 0;
	}
	else if (s2 == 0) {
		return +1;
	}
	else if (s1 == 0) {
		return -1;
	}
	else {
		size_t l1 = AVX512_StringLength(s1);
		size_t l2 = AVX512_StringLength(s2);
		if (l1 == l2 && l2 == 0) return 0;
		size_t ln = l1 > l2 ? l1 : l2;

		for (unsigned long i = 0; i < ln; i += stride)
		{
			unsigned long igt = 0;
			unsigned long ilt = 0;
			unsigned long most = i + stride < ln ? stride : ln - i;
			__m512i part1 = _mm512_loadu_epi8(s1 + i);
			__m512i part2 = _mm512_loadu_epi8(s2 + i);
			__mmask64 rgt = _mm512_cmpgt_epi8_mask(part1, part2);
			__mmask64 rlt = _mm512_cmplt_epi8_mask(part1, part2);
			unsigned char bgt = _BitScanForward64(&igt, rgt);
			unsigned char blt = _BitScanForward64(&ilt, rlt);
			if (!bgt && !blt) continue;

			ilt = ilt > most ? most : ilt;
			igt = igt > most ? most : igt;
			if (bgt && !blt) {
				return i + igt;
			}
			else if (!bgt && blt) {
				return i + ilt;
			}
			else {
				return igt < ilt ? igt : ilt;
			}
		}
		return 0;//all the same
	}
}
int AVX512_StringCompare(wchar_t* s1, wchar_t* s2)
{
	const int stride = sizeof(__m512i) / sizeof(int);

	if (s1 == 0 && s2 == 0) {
		return 0;
	}
	else if (s2 == 0) {
		return +1;
	}
	else if (s1 == 0) {
		return -1;
	}
	else {
		size_t l1 = AVX512_StringLength(s1);
		size_t l2 = AVX512_StringLength(s2);
		if (l1 == l2 && l2 == 0) return 0;
		size_t ln = l1 > l2 ? l1 : l2;

		for (unsigned long i = 0; i < ln; i += stride)
		{
			unsigned long igt = 0;
			unsigned long ilt = 0;
			unsigned long most = i + stride < ln ? stride : ln - i;
			__m512i part1 = _mm512_loadu_epi16(s1 + i);
			__m512i part2 = _mm512_loadu_epi16(s2 + i);
			__mmask32 rgt = _mm512_cmpgt_epi16_mask(part1, part2);
			__mmask32 rlt = _mm512_cmplt_epi16_mask(part1, part2);
			unsigned char bgt = _BitScanForward(&igt, rgt);
			unsigned char blt = _BitScanForward(&ilt, rlt);
			if (!bgt && !blt) continue;

			ilt = ilt > most ? most : ilt;
			igt = igt > most ? most : igt;
			if (bgt && !blt) {
				return i + igt;
			}
			else if (!bgt && blt) {
				return i + ilt;
			}
			else {
				return igt < ilt ? igt : ilt;
			}
		}
		return 0;//all the same
	}
}

bool AVX2_StringEqual(char* s1, char* s2)
{
	const int stride = sizeof(__m256i) / sizeof(int);

	if (s1 == 0 && s2 == 0) {
		return true;
	}
	else if (s2 == 0) {
		return false;
	}
	else if (s1 == 0) {
		return false;
	}
	else {
		size_t l1 = AVX2_StringLength(s1);
		size_t l2 = AVX2_StringLength(s2);
		if (l1 == l2 && l2 == 0) return true;
		if (l1 != l2) return false;
		size_t ln = l1;

		for (unsigned long i = 0; i < ln; i += stride)
		{
			unsigned long iet = 0;
			unsigned long most = i + stride < ln ? stride : ln - i;
			__m256i part1 = _mm256_loadu_epi8(s1 + i);
			__m256i part2 = _mm256_loadu_epi8(s2 + i);
			__mmask32 neqt = _mm256_cmpneq_epi8_mask(part1, part2);
			unsigned char bet = _BitScanForward(&iet, neqt);
			if (bet && iet<most) return false;
		}
		return true;//all the same
	}
}
bool AVX2_StringEqual(wchar_t* s1, wchar_t* s2)
{
	const int stride = sizeof(__m256i) / sizeof(int);

	if (s1 == 0 && s2 == 0) {
		return true;
	}
	else if (s2 == 0) {
		return false;
	}
	else if (s1 == 0) {
		return false;
	}
	else {
		size_t l1 = AVX2_StringLength(s1);
		size_t l2 = AVX2_StringLength(s2);
		if (l1 == l2 && l2 == 0) return true;
		if (l1 != l2) return false;
		size_t ln = l1;

		for (unsigned long i = 0; i < ln; i += stride)
		{
			unsigned long iet = 0;
			unsigned long most = i + stride < ln ? stride : ln - i;
			__m256i part1 = _mm256_loadu_epi16(s1 + i);
			__m256i part2 = _mm256_loadu_epi16(s2 + i);
			__mmask16 neqt = _mm256_cmpneq_epi16_mask(part1, part2);
			unsigned char bet = _BitScanForward(&iet, neqt);
			if (bet && iet < most) return false;
		}
		return true;//all the same
	}
}
bool AVX512_StringEqual(char* s1, char* s2)
{
	const int stride = sizeof(__m512i) / sizeof(int);

	if (s1 == 0 && s2 == 0) {
		return true;
	}
	else if (s2 == 0) {
		return false;
	}
	else if (s1 == 0) {
		return false;
	}
	else {
		size_t l1 = AVX512_StringLength(s1);
		size_t l2 = AVX512_StringLength(s2);
		if (l1 == l2 && l2 == 0) return true;
		if (l1 != l2) return false;
		size_t ln = l1;

		for (unsigned long i = 0; i < ln; i += stride)
		{
			unsigned long iet = 0;
			unsigned long most = i + stride < ln ? stride : ln - i;
			__m512i part1 = _mm512_loadu_epi8(s1 + i);
			__m512i part2 = _mm512_loadu_epi8(s2 + i);
			__mmask64 neqt = _mm512_cmpneq_epi8_mask(part1, part2);
			unsigned char bet = _BitScanForward64(&iet, neqt);
			if (bet && iet < most) return false;
		}
		return true;//all the same
	}
}
bool AVX512_StringEqual(wchar_t* s1, wchar_t* s2)
{
	const int stride = sizeof(__m512i) / sizeof(int);

	if (s1 == 0 && s2 == 0) {
		return true;
	}
	else if (s2 == 0) {
		return false;
	}
	else if (s1 == 0) {
		return false;
	}
	else {
		size_t l1 = AVX512_StringLength(s1);
		size_t l2 = AVX512_StringLength(s2);
		if (l1 == l2 && l2 == 0) return true;
		if (l1 != l2) return false;
		size_t ln = l1;

		for (unsigned long i = 0; i < ln; i += stride)
		{
			unsigned long iet = 0;
			unsigned long most = i + stride < ln ? stride : ln - i;
			__m512i part1 = _mm512_loadu_epi16(s1 + i);
			__m512i part2 = _mm512_loadu_epi16(s2 + i);
			__mmask32 neqt = _mm512_cmpneq_epi16_mask(part1, part2);
			unsigned char bet = _BitScanForward(&iet, neqt);
			if (bet && iet < most) return false;
		}
		return true;//all the same
	}
}

int AVX2_StringIndexOf(char* s, char c)
{
	if (s != 0) {
		const int stride = sizeof(__m256i) / sizeof(int);
		unsigned long index = 0;
		__m256i _chs = _mm256_set1_epi8(c);
		__m256i part = { 0 };
		size_t length = AVX2_StringLength(s);

		for (int i = 0; i < length; i += stride)
		{
			part = _mm256_loadu_epi8(s + i);
			__mmask32 result = _mm256_cmpeq_epi8_mask(part, _chs);
			if (_BitScanForward(&index, result))
			{
				return i + index;
			}
		}
	}

	return -1;
}
int AVX2_StringIndexOf(wchar_t* s, wchar_t c)
{
	if (s != 0) {
		const int stride = sizeof(__m256i) / sizeof(int);
		unsigned long index = 0;
		__m256i _chs = _mm256_set1_epi16(c);
		__m256i part = { 0 };
		size_t length = AVX2_StringLength(s);

		for (int i = 0; i < length; i += stride)
		{
			part = _mm256_loadu_epi16(s + i);
			__mmask16 result = _mm256_cmpeq_epi16_mask(part, _chs);
			if (_BitScanForward(&index, result))
			{
				return i + index;
			}
		}
	}

	return -1;
}
int AVX512_StringIndexOf(char* s, char c)
{
	if (s != 0) {
		const int stride = sizeof(__m512i) / sizeof(int);
		unsigned long index = 0;
		__m512i _chs = _mm512_set1_epi8(c);
		__m512i part = { 0 };
		size_t length = AVX512_StringLength(s);

		for (int i = 0; i < length; i += stride)
		{
			part = _mm512_loadu_epi8(s + i);
			__mmask64 result = _mm512_cmpeq_epi8_mask(part, _chs);
			if (_BitScanForward64(&index, result))
			{
				return i + index;
			}
		}
	}

	return -1;
}
int AVX512_StringIndexOf(wchar_t* s, wchar_t c)
{
	if (s != 0) {
		const int stride = sizeof(__m512i) / sizeof(int);
		unsigned long index = 0;
		__m512i _chs = _mm512_set1_epi16(c);
		__m512i part = { 0 };
		size_t length = AVX512_StringLength(s);

		for (int i = 0; i < length; i += stride)
		{
			part = _mm512_loadu_epi16(s + i);
			__mmask32 result = _mm512_cmpeq_epi16_mask(part, _chs);
			if (_BitScanForward(&index, result))
			{
				return i + index;
			}
		}
	}

	return -1;
}

int AVX2_StringIndexOf(char* s, char* cs)
{
	if (s == 0) return -1;
	if (cs == 0) return -1;
	size_t l1 = AVX2_StringLength(s);
	size_t l2 = AVX2_StringLength(cs);
	if (l1 == 0 && l2 == 0) return 0;
	if (l1 == 0 && l2 > 0) return -1;
	if (l1 < l2) return -1;

	//TODO:find needle in hystack

	return -1;
}
int AVX2_StringIndexOf(wchar_t* s, wchar_t* cs)
{
	if (s == 0) return -1;
	if (cs == 0) return -1;
	size_t l1 = AVX2_StringLength(s);
	size_t l2 = AVX2_StringLength(cs);
	if (l1 == 0 && l2 == 0) return 0;
	if (l1 == 0 && l2 > 0) return -1;
	if (l1 < l2) return -1;

	//TODO:find needle in hystack

	return -1;
}
int AVX512_StringIndexOf(char* s, char* cs)
{
	if (s == 0) return -1;
	if (cs == 0) return -1;
	size_t l1 = AVX512_StringLength(s);
	size_t l2 = AVX2_StringLength(cs);
	if (l1 == 0 && l2 == 0) return 0;
	if (l1 == 0 && l2 > 0) return -1;
	if (l1 < l2) return -1;

	//TODO:find needle in hystack

	return -1;
}
int AVX512_StringIndexOf(wchar_t* s, wchar_t *cs)
{
	if (s == 0) return -1;
	if (cs == 0) return -1;
	size_t l1 = AVX512_StringLength(s);
	size_t l2 = AVX512_StringLength(cs);
	if (l1 == 0 && l2 == 0) return 0;
	if (l1 == 0 && l2 > 0) return -1;
	if (l1 < l2) return -1;

	//TODO:find needle in hystack

	return -1;
}

const int DATA_SIZE = 16;

//int data0[DATA_SIZE] = { 0 };
//int data0[DATA_SIZE] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };
//int data0[DATA_SIZE] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31 };
//int data0[DATA_SIZE] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63 };
//int data0[DATA_SIZE] = { 12,2,23,25,17,1,16,19,31,19,1,23,31,19,13,25,26,14,10,28,31,25,7,14,17,3,25,24,21,22,8,14 };
int data0[DATA_SIZE] = { 15,2,1,4,3,9,8,6,7,10,12,11,0,5,14,13 };
int data1[DATA_SIZE] = { 0 };
int data2[DATA_SIZE] = { 0 };
int data3[DATA_SIZE] = { 0 };
int data4[DATA_SIZE] = { 0 };
int data5[DATA_SIZE] = { 0 };
int data6[DATA_SIZE] = { 0 };
int data7[DATA_SIZE] = { 0 };

bool CheckSequence(int a[], int b[], int n) {
	for (int i = 0; i < n; i++) {
		if (a[i] != b[i])
			return false;
	}
	return true;
}
int main()
{
	__m128i __data8 = _mm_setr_epi8(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
	__m128i _data16 = _mm_setr_epi16(5, 3, 1, 4, 2, 9, 7, 6); //0 is last
	__m256i _data32 = _mm256_setr_epi32(
		0x00050055,
		0x00240044,
		0x00060066,
		0x00090099,
		0x00010033,
		0x00020011,
		0x00170077,
		0x00030033
	);
	//unsigned char r8 = 0;
	//unsigned short r16 = 0;
	//unsigned int r32 = 0;
	//unsigned long long r64 = 0;
	//int i = 0;

	//i = HorizentalMin8(__data8, &r8);
	//i = HorizentalMax8(__data8, &r8);
	//i = HorizentalMin16(_data16, &r16);
	//i = HorizentalMax16(_data16, &r16);
	//i = HorizentalMin32(_data32, &r32);
	//i = HorizentalMax32(_data32, &r32);
	//i = HorizentalMin64(_data32, &r64);
	//i = HorizentalMax64(_data32, &r64);

	bool show = true;
	long long t0;
	srand((unsigned)time(0));
	if (true)
	{
		printf("original data(count = %d):\n", DATA_SIZE);
		for (int i = 0; i < DATA_SIZE; i++) {
			data7[i]
				= data6[i]
				= data5[i]
				= data4[i]
				= data3[i]
				= data2[i]
				= data1[i]
				= data0[i]
				;// = (int)((rand() / (double)RAND_MAX) * DATA_SIZE);
			if (show) {
				printf("%d ", data0[i]);
			}
		}
		printf("\n\n");
	}

	if (true)
	{
		printf("for system quick sort:\n");
		t0 = _Query_perf_counter();
		{
			std::sort(data0, data0 + DATA_SIZE);
		}
		printf("time:%lf(ms)\n",
			((_Query_perf_counter() - t0) / (double)_Query_perf_frequency() * 1000.0));
		if (show) {
			for (int i = 0; i < DATA_SIZE; i++) {
				printf("%d ", data0[i]);
			}
		}
		printf("\n\n");
	}

	if (true)
	{
		printf("for quick sort:\n");
		t0 = _Query_perf_counter();
		{
			QuickSort(data1, DATA_SIZE);
		}
		printf("time:%lf(ms)\n",
			((_Query_perf_counter() - t0) / (double)_Query_perf_frequency() * 1000.0));

		bool c = CheckSequence(data0, data1, DATA_SIZE);
		printf("correct:%s\n", c ? "true" : "false");
		if (!c) {
			for (int i = 0; i < DATA_SIZE; i++) {
				printf("%d ", data1[i]);
			}
		}
		printf("\n\n");
	}

	if (true)
	{
		printf("for fast quick sort 256:\n");
		t0 = _Query_perf_counter();
		{
			FastQuickSort256(data5, DATA_SIZE);
		}
		printf("time:%lf(ms)\n",
			((_Query_perf_counter() - t0) / (double)_Query_perf_frequency() * 1000.0));

		bool c = CheckSequence(data0, data5, DATA_SIZE);
		printf("correct:%s\n", c ? "true" : "false");
		if (!c) {
			for (int i = 0; i < DATA_SIZE; i++) {
				printf("%d ", data5[i]);
			}
		}
		printf("\n\n");
	}
	if (true)
	{
		printf("for fast quick sort 512:\n");
		t0 = _Query_perf_counter();
		{
			FastQuickSort512(data6, DATA_SIZE);
		}
		printf("time:%lf(ms)\n",
			((_Query_perf_counter() - t0) / (double)_Query_perf_frequency() * 1000.0));

		bool c = CheckSequence(data0, data6, DATA_SIZE);
		printf("correct:%s\n", c ? "true" : "false");
		if (!c) {
			for (int i = 0; i < DATA_SIZE; i++) {
				printf("%d ", data6[i]);
			}
		}
		printf("\n\n");
	}

	if (true)
	{
		t0 = _Query_perf_counter();
		printf("for odd even sort:\n");
		{
			OddEvenSort(data2, DATA_SIZE);
		}
		printf("time:%lf(ms)\n",
			((_Query_perf_counter() - t0) / (double)_Query_perf_frequency() * 1000.0));

		bool c = CheckSequence(data0, data2, DATA_SIZE);
		printf("correct:%s\n", c ? "true" : "false");
		if (!c) {
			for (int i = 0; i < DATA_SIZE; i++) {
				printf("%d ", data2[i]);
			}
		}
		printf("\n\n");
	}

	if (true)
	{
		printf("for fast odd even sort 256:\n");
		t0 = _Query_perf_counter();
		{
			FastOddEvenSort256(data3, DATA_SIZE);
		}
		printf("time:%lf(ms)\n",
			((_Query_perf_counter() - t0) / (double)_Query_perf_frequency() * 1000.0));
		bool b = CheckSequence(data0, data3, DATA_SIZE);

		printf("correct:%s\n", b ? "true" : "false");
		if (!b)
		{
			for (int i = 0; i < DATA_SIZE; i++) {
				printf("%d ", data3[i]);
			}
		}
		printf("\n\n");
	}

	if (true)
	{
		printf("for fast odd even sort 512:\n");
		t0 = _Query_perf_counter();
		{
			FastOddEvenSort512(data4, DATA_SIZE);
		}
		printf("time:%lf(ms)\n",
			((_Query_perf_counter() - t0) / (double)_Query_perf_frequency() * 1000.0));
		bool b = CheckSequence(data0, data4, DATA_SIZE);
		printf("correct:%s\n", b ? "true" : "false");
		if (!b)
		{
			for (int i = 0; i < DATA_SIZE; i++) {
				printf("%d ", data4[i]);
			}
		}
		printf("\n\n");
	}

	if (true)
	{
		printf("for fast odd even sort 512:\n");
		t0 = _Query_perf_counter();
		{
			FastOddEvenSort512(data4, DATA_SIZE);
		}
		printf("time:%lf(ms)\n",
			((_Query_perf_counter() - t0) / (double)_Query_perf_frequency() * 1000.0));
		bool b = CheckSequence(data0, data4, DATA_SIZE);
		printf("correct:%s\n", b ? "true" : "false");
		if (!b)
		{
			for (int i = 0; i < DATA_SIZE; i++) {
				printf("%d ", data4[i]);
			}
		}
		printf("\n\n");
	}

	if (true)
	{
		printf("for fast merge sort 256:\n");
		t0 = _Query_perf_counter();
		{
			FastMergeSort256(data7, DATA_SIZE);
		}
		printf("time:%lf(ms)\n",
			((_Query_perf_counter() - t0) / (double)_Query_perf_frequency() * 1000.0));
		bool b = CheckSequence(data0, data7, DATA_SIZE);
		printf("correct:%s\n", b ? "true" : "false");
		if (!b)
		{
			for (int i = 0; i < DATA_SIZE; i++) {
				printf("%d ", data7[i]);
			}
		}
		printf("\n\n");
	}

	return 0;
}
