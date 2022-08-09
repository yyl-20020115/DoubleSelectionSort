#include <iostream>
#include <intrin.h>
#include <algorithm>

void do_print_hex(__m512i data, bool nl = false) {
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
void do_print_hex(__m256i data, bool nl = false) {
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
void do_print_hex(__m128i data, bool nl = false) {
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
void do_print_hex(int data[], int N, bool nl = false) {
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
void do_print_dec(__m512i data, bool nl = false) {
	bool first = true;
	printf("[");
	for (int i = 0; i < sizeof(data.m512i_u32) / sizeof(int); i++) {
		if (!first) {
			printf(", ");
		}
		printf("%d", data.m512i_i32[i]);
		first = false;
	}
	printf("]");
	if (nl)printf("\n");
}
void do_print_dec(__m256i data, bool nl = false) {
	bool first = true;
	printf("[");
	for (int i = 0; i < sizeof(data.m256i_u32) / sizeof(int); i++) {
		if (!first) {
			printf(", ");
		}
		printf("%d", data.m256i_u32[i]);
		first = false;
	}
	printf("]");
	if (nl)printf("\n");
}
void do_print_dec(__m128i data, bool nl = false) {
	bool first = true;
	printf("[");
	for (int i = 0; i < sizeof(data.m128i_u32) / sizeof(int); i++) {
		if (!first) {
			printf(", ");
		}
		printf("%d", data.m128i_u32[i]);
		first = false;
	}
	printf("]");
	if (nl)printf("\n");
}
void do_print_dec(int data[], int N, bool nl = false) {
	bool first = true;
	printf("[");
	for (int i = 0; i < N; i++) {
		if (!first) {
			printf(", ");
		}
		printf("%d", data[i]);
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
#if 1
			__m256i minlows256 = _mm256_cvtepi32_epi64(_mm256_extracti32x4_epi32(min, 0));
			__m256i minhighs256 = _mm256_cvtepi32_epi64(_mm256_extracti32x4_epi32(min, 1));
			__m256i maxlows256 = _mm256_cvtepi32_epi64(_mm256_extracti32x4_epi32(max, 0));
			__m256i maxhighs256 = _mm256_cvtepi32_epi64(_mm256_extracti32x4_epi32(max, 1));

			__m256i lows = _mm256_mask_alignr_epi32(
				minlows256, 0b10101010, maxlows256, maxhighs256, 7);

			__m256i highs = _mm256_mask_alignr_epi32(
				minhighs256, 0b10101010, maxhighs256, maxlows256, 7);

			_mm256_storeu_epi32(ptr, lows);
			_mm256_storeu_epi32(ptr + size, highs);
#else
			_mm256_i32scatter_epi32(ptr, ipe, min, skip);
			_mm256_i32scatter_epi32(ptr, ipo, max, skip);
#endif
		}
		for (int part = 0; part < n; part += dbls)
		{
			if (dbls >= n - part - 1) a[n] = a[n - 1];

			int* ptr = a + part;

			__m256i veven = _mm256_i32gather_epi32(ptr, ipt, skip);
			__m256i vodd_ = _mm256_i32gather_epi32(ptr, ipo, skip);

			__m256i min = _mm256_min_epi32(vodd_, veven);
			__m256i max = _mm256_max_epi32(vodd_, veven);
#if 1
			__m256i minlows256 = _mm256_cvtepi32_epi64(_mm256_extracti32x4_epi32(min, 0));
			__m256i minhighs256 = _mm256_cvtepi32_epi64(_mm256_extracti32x4_epi32(min, 1));
			__m256i maxlows256 = _mm256_cvtepi32_epi64(_mm256_extracti32x4_epi32(max, 0));
			__m256i maxhighs256 = _mm256_cvtepi32_epi64(_mm256_extracti32x4_epi32(max, 1));
			//AVX512 used
			__m256i lows = _mm256_mask_alignr_epi32(
				minlows256, 0b10101010, maxlows256, maxhighs256, 7);
			//AVX512 used
			__m256i highs = _mm256_mask_alignr_epi32(
				minhighs256, 0b10101010, maxhighs256, maxlows256, 7);
			_mm256_storeu_epi32(ptr + 1, lows);
			_mm256_storeu_epi32(ptr + 1 + size, highs);
#else
			_mm256_i32scatter_epi32(ptr, ipo, min, skip);
			_mm256_i32scatter_epi32(ptr, ipt, max, skip);
#endif
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
#if 1
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
#else
			_mm512_i32scatter_epi32(ptr, ipe, min, skip);
			_mm512_i32scatter_epi32(ptr, ipo, max, skip);
#endif
		}
		for (int part = 0; part < n; part += dbls)
		{
			if (dbls >= n - part - 1) a[n] = a[n - 1];

			int* ptr = a + part;

			__m512i veven = _mm512_i32gather_epi32(ipt, ptr, skip);
			__m512i vodd_ = _mm512_i32gather_epi32(ipo, ptr, skip);

			__m512i min = _mm512_min_epi32(vodd_, veven);
			__m512i max = _mm512_max_epi32(vodd_, veven);
#if 1
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
#else
			_mm512_i32scatter_epi32(ptr, ipo, min, skip);
			_mm512_i32scatter_epi32(ptr, ipt, max, skip);
#endif
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
int HorizentalMin16(__m256i data, unsigned short* p = 0) {
	const int stride = sizeof(data) / sizeof(*p);
	const int half = stride / 2;
	__m128i zero = _mm_setzero_si128();
	__m128i low = _mm256_extracti128_si256(data, 0);
	__m128i high = _mm256_extracti128_si256(data, 1);
	unsigned short lv = 0;
	unsigned short hv = 0;
	int li = HorizentalMin16(low, &lv);
	int hi = HorizentalMin16(high, &hv);
	if (lv <= hv) {
		if (p != 0) *p = lv;
		return li;
	}
	else {
		if (p != 0) *p = hv;
		return half + hi;
	}
}
int HorizentalMax16(__m256i data, unsigned short* p = 0) {
	const int stride = sizeof(data) / sizeof(*p);
	const int half = stride / 2;
	__m128i zero = _mm_setzero_si128();
	__m128i low = _mm256_extracti128_si256(data, 0);
	__m128i high = _mm256_extracti128_si256(data, 1);
	unsigned short lv = 0;
	unsigned short hv = 0;
	int li = HorizentalMax16(low, &lv);
	int hi = HorizentalMax16(high, &hv);
	if (lv <= hv) {
		if (p != 0) *p = lv;
		return li;
	}
	else {
		if (p != 0) *p = hv;
		return half + hi;
	}
}
int HorizentalMin16(__m512i data, unsigned short* p = 0) {
	const int stride = sizeof(data) / sizeof(*p);
	const int half = stride / 2;
	__m256i low = _mm512_extracti64x4_epi64(data, 0);
	__m256i high = _mm512_extracti64x4_epi64(data, 1);
	unsigned short lv = 0;
	unsigned short hv = 0;
	int li = HorizentalMin16(low, &lv);
	int hi = HorizentalMin16(high, &hv);
	if (lv <= hv) {
		if (p != 0) *p = lv;
		return li;
	}
	else {
		if (p != 0) *p = hv;
		return half + hi;
	}
}
int HorizentalMax16(__m512i data, unsigned short* p = 0) {
	const int stride = sizeof(data) / sizeof(*p);
	const int half = stride / 2;
	__m256i low = _mm512_extracti64x4_epi64(data, 0);
	__m256i high = _mm512_extracti64x4_epi64(data, 1);
	unsigned short lv = 0;
	unsigned short hv = 0;
	int li = HorizentalMax16(low, &lv);
	int hi = HorizentalMax16(high, &hv);
	if (lv >= hv) {
		if (p != 0) *p = lv;
		return li;
	}
	else {
		if (p != 0) *p = hv;
		return half + hi;
	}
}
int HorizentalMin8(__m128i data, unsigned char* p = 0) {
	const int stride = sizeof(data) / sizeof(*p);
	const int half = stride / 2;
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
		return half + hi;
	}
}
int HorizentalMax8(__m128i data, unsigned char* p = 0) {
	const int stride = sizeof(data) / sizeof(*p);
	const int half = stride / 2;
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
		return half + hi;
	}
}
int HorizentalMin8(__m256i data, unsigned char* p = 0) {
	const int stride = sizeof(data) / sizeof(*p);
	const int half = stride / 2;
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
		return half + hi;
	}
}
int HorizentalMax8(__m256i data, unsigned char* p = 0) {
	const int stride = sizeof(data) / sizeof(*p);
	const int half = stride / 2;
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
		return half + hi;
	}
}
int HorizentalMin8(__m512i data, unsigned char* p = 0) {
	const int stride = sizeof(data) / sizeof(*p);
	const int half = stride / 2;
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
		return half + hi;
	}
}
int HorizentalMax8(__m512i data, unsigned char* p = 0) {
	const int stride = sizeof(data) / sizeof(*p);
	const int half = stride / 2;
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
		return half + hi;
	}
}
int HorizentalMin32(__m128i data, unsigned int* p = 0) {
#if 0
	__m128i counts = _mm_setr_epi16(
		__popcnt(_mm_cmpge_epi32_mask(_mm_set1_epi32(_mm_extract_epi32(data, 0)), data)),
		__popcnt(_mm_cmpge_epi32_mask(_mm_set1_epi32(_mm_extract_epi32(data, 1)), data)),
		__popcnt(_mm_cmpge_epi32_mask(_mm_set1_epi32(_mm_extract_epi32(data, 2)), data)),
		__popcnt(_mm_cmpge_epi32_mask(_mm_set1_epi32(_mm_extract_epi32(data, 3)), data)),
		8, 8, 8, 8);
	__m128i result = _mm_minpos_epu16(counts);
	int index = result.m128i_i16[1] & 0x3;
	if (p != 0) {
		*p = data.m128i_i32[index];
	}
	return index;
#else
	const int stride = sizeof(data) / sizeof(*p);

	int a0 = data.m128i_i32[0];
	int a1 = data.m128i_i32[1];
	int a2 = data.m128i_i32[2];
	int a3 = data.m128i_i32[3];

	int m0 = a0 <= a1 ? a0 : a1;
	int p0 = (a0 > a1);

	int m1 = a2 <= a3 ? a2 : a3;
	int p1 = (a2 > a3);

	if (p != 0)*p = (m0 <= m1) ? m0 : m1;

	return (m0 <= m1) ? p0 : (p1 + 2);
#endif
}
int HorizentalMax32(__m128i data, unsigned int* p = 0) {
	__m128i zero = _mm_setzero_si128();
	__m128i ones = _mm_cmpeq_epi32(zero, zero);
	__m128i result = _mm_andnot_si128(data, ones);
	int index = HorizentalMin32(result, p);
	if (p != 0) {
		*p = data.m128i_u32[index];
	}
	return index;
}
//algorithm: how to find min int from __m256i with _mm_minpos_epu16
//  0. for each k in [0,7]
//  1. broadcast data.m256i_i32[k] to make 
//     (data.m256i_i32[k],data.m256i_i32[k],data.m256i_i32[k],data.m256i_i32[k],
//		data.m256i_i32[k],data.m256i_i32[k],data.m256i_i32[k],data.m256i_i32[k])
//  2. use the former template to compare with data to genrate a mask
//     with bit set to 1 indicatiing the value in data is greater than 
//     data.m256i_i32[k]
//  3. count the set bid with popcnt and make __m128i with 8 * i16 of popcnt result
//     (popcnt of (mask8) is no more than 8, therefore i16 is enough for storing)
//  4. use _mm_minpos_epu16 with counts to find the position and the value of
//     min count i32 within data which is greater or equal to data.m256i_i32[k]
//  5. the data.m256i_i32[k] with min number of greaters is the min data.m256i_i32[k]
//
int HorizentalMin32(__m256i data, unsigned int* p = 0) {
	__m128i counts = _mm_setr_epi16(
		__popcnt(_mm256_cmpge_epi32_mask(_mm256_set1_epi32(_mm256_extract_epi32(data, 0)), data)),
		__popcnt(_mm256_cmpge_epi32_mask(_mm256_set1_epi32(_mm256_extract_epi32(data, 1)), data)),
		__popcnt(_mm256_cmpge_epi32_mask(_mm256_set1_epi32(_mm256_extract_epi32(data, 2)), data)),
		__popcnt(_mm256_cmpge_epi32_mask(_mm256_set1_epi32(_mm256_extract_epi32(data, 3)), data)),
		__popcnt(_mm256_cmpge_epi32_mask(_mm256_set1_epi32(_mm256_extract_epi32(data, 4)), data)),
		__popcnt(_mm256_cmpge_epi32_mask(_mm256_set1_epi32(_mm256_extract_epi32(data, 5)), data)),
		__popcnt(_mm256_cmpge_epi32_mask(_mm256_set1_epi32(_mm256_extract_epi32(data, 6)), data)),
		__popcnt(_mm256_cmpge_epi32_mask(_mm256_set1_epi32(_mm256_extract_epi32(data, 7)), data))
	);
	__m128i result = _mm_minpos_epu16(counts);
	int index = result.m128i_i16[1] & 0x7;
	if (p != 0) {
		*p = data.m256i_i32[index];
	}
	return index;
#if 0 //unused
	const int stride = sizeof(data) / sizeof(*p);
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
	unsigned int c = __popcnt(mask);
	if (c == stride) { //all highs are same, only dif in lows
		__m128i low_result = _mm_minpos_epu16(_low);
		short low_value = low_result.m128i_i16[0];
		index = low_result.m128i_i16[1];
		if (p != 0) {
			*p = data.m256i_i32[index];
		}
		return index;
	}
	else if (c == 1) { //this high is special, no need to check lows
		if (p != 0) {
			*p = data.m256i_i32[high_index];
		}
		return high_index;
	}
	else { //multiple lows for same highs
		//try replace unrelated shorts with 0xffff
		__m128i tries = ones;
		__m128i maxlevel = _mm_mask_blend_epi16(mask, tries, _low);
		__m128i low_result = _mm_minpos_epu16(maxlevel);
		if (low_result.m128i_i16[0] == (short)0xffff) {
			//if found 0xffff, change replacement with 0
			tries = zero;
			maxlevel = _mm_mask_blend_epi16(mask, tries, _low);
			mask = _mm_cmpeq_epi16_mask(maxlevel, ones);
			//the first found non-zero bit index is the index
			//for original 0xffff value
			low_result = _mm_minpos_epu16(maxlevel);
			if (_BitScanForward(&index, mask)) {
				if (p != 0) {
					*p = data.m256i_i32[index];
				}
				return index;
			}
		}
		else {
			index = low_result.m128i_i16[1];
			if (p != 0) {
				*p = data.m256i_i32[index];
			}
			return index;
		}
	}
	return index;
#endif
}
int HorizentalMax32(__m256i data, unsigned int* p = 0) {
	__m128i zero = _mm_setzero_si128();
	__m128i ones = _mm_cmpeq_epi32(zero, zero);
	__m256i zero_ = _mm256_setzero_si256();
	__m256i ones_ = _mm256_cmpeq_epi32(zero_, zero_);

	__m256i result = _mm256_andnot_si256(data, ones_);
	int index = HorizentalMin32(result, p);
	if (p != 0) {
		*p = data.m256i_u32[index];
	}
	return index;
}
int HorizentalMin32(__m512i data, unsigned int* p = 0)
{
	const int stride = sizeof(data) / sizeof(*p);
	const int half = stride / 2;
	__m256i low = _mm512_extracti32x8_epi32(data, 0);
	__m256i high = _mm512_extracti32x8_epi32(data, 1);

	unsigned int lv = 0;
	unsigned int hv = 0;

	int li = HorizentalMin32(low, &lv);
	int hi = HorizentalMin32(high, &hv);
	if (lv <= hv) {
		if (p != 0)*p = lv;
		return li;
	}
	else //hv<lv
	{
		if (p != 0)*p = hv;
		return half + hi;
	}
}
int HorizentalMax32(__m512i data, unsigned int* p = 0)
{
	const int stride = sizeof(data) / sizeof(*p);
	const int half = stride / 2;
	__m256i low = _mm512_extracti32x8_epi32(data, 0);
	__m256i high = _mm512_extracti32x8_epi32(data, 1);

	unsigned int lv = 0;
	unsigned int hv = 0;

	int li = HorizentalMax32(low, &lv);
	int hi = HorizentalMax32(high, &hv);
	if (lv >= hv) {
		if (p != 0)*p = lv;
		return li;
	}
	else
	{
		if (p != 0)*p = hv;
		return half + hi;
	}
}
int HorizentalMin64(__m128i data, unsigned long long* p = 0)
{
	const int stride = sizeof(data) / sizeof(*p);
	const int half = stride / 2;
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
	const int stride = sizeof(data) / sizeof(*p);
	const int half = stride / 2;

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
	const int stride = sizeof(data) / sizeof(*p);
	const int half = stride / 2;
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
		return half + hi;
	}
}
int HorizentalMax64(__m256i data, unsigned long long* p = 0)
{
	const int stride = sizeof(data) / sizeof(*p);
	const int half = stride / 2;
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
		return half + hi;
	}

}
int HorizentalMin64(__m512i data, unsigned long long* p = 0)
{
	const int stride = sizeof(data) / sizeof(*p);
	const int half = stride / 2;
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
		return half + hi;
	}
}
int HorizentalMax64(__m512i data, unsigned long long* p = 0)
{
	const int stride = sizeof(data) / sizeof(*p);
	const int half = stride / 2;
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
		return half + hi;
	}
}
__m128i HorizentalSort8(__m128i data, unsigned char* pmin = 0, unsigned char* pmax = 0) {
	const int stride = sizeof(data) / sizeof(*pmin);
	__m128i result = { 0 };
	unsigned char min = 0;
	unsigned char max = 0;

	int imin = HorizentalMin8(data, &min);
	int imax = HorizentalMax8(data, &max);
	if (pmin != 0)*pmin = min;
	if (pmax != 0)*pmax = max;
	result.m128i_u8[0] = min;
	result.m128i_u8[stride - 1] = max;
	data.m128i_u8[imin] = max;
	for (int i = 1; i < stride - 1; i++)
	{
		imin = HorizentalMin8(data, &min);
		result.m128i_u8[i] = min;
		data.m128i_u8[imin] = max;
	}

	return result;
}
__m256i HorizentalSort8(__m256i data, unsigned char* pmin = 0, unsigned char* pmax = 0) {
	const int stride = sizeof(data) / sizeof(*pmin);
	__m256i result = { 0 };
	unsigned char min = 0;
	unsigned char max = 0;

	int imin = HorizentalMin8(data, &min);
	int imax = HorizentalMax8(data, &max);
	if (pmin != 0)*pmin = min;
	if (pmax != 0)*pmax = max;
	result.m256i_u8[0] = min;
	result.m256i_u8[stride - 1] = max;
	data.m256i_u8[imin] = max;
	for (int i = 1; i < stride - 1; i++)
	{
		imin = HorizentalMin8(data, &min);
		result.m256i_u8[i] = min;
		data.m256i_u8[imin] = max;
	}

	return result;
}
__m512i HorizentalSort8(__m512i data, unsigned char* pmin = 0, unsigned char* pmax = 0) {
	const int stride = sizeof(data) / sizeof(*pmin);
	__m512i result = { 0 };
	int imin = HorizentalMin8(data, pmin);
	int imax = HorizentalMax8(data, pmax);
	unsigned char min = data.m512i_u8[imin];
	unsigned char max = data.m512i_u8[imax];
	if (pmin != 0)*pmin = min;
	if (pmax != 0)*pmax = max;
	result.m512i_u8[0] = min;
	result.m512i_u8[stride - 1] = max;
	data.m512i_u8[imin] = max;
	for (int i = 1; i < stride - 1; i++)
	{
		imin = HorizentalMin8(data, &min);
		result.m512i_u8[i] = min;
		data.m512i_u8[imin] = max;
	}

	return result;
}
__m128i HorizentalSort16(__m128i data, unsigned short* pmin = 0, unsigned short* pmax = 0) {
	const int stride = sizeof(data) / sizeof(*pmin);
	__m128i result = { 0 };
	unsigned short min = 0;
	unsigned short max = 0;

	int imin = HorizentalMin16(data, &min);
	int imax = HorizentalMax16(data, &max);
	if (pmin != 0)*pmin = min;
	if (pmax != 0)*pmax = max;
	result.m128i_u16[0] = min;
	result.m128i_u16[stride - 1] = max;
	data.m128i_u16[imin] = max;
	for (int i = 1; i < stride - 1; i++)
	{
		imin = HorizentalMin16(data, &min);
		result.m128i_u16[i] = min;
		data.m128i_u16[imin] = max;
	}

	return result;
}
__m256i HorizentalSort16(__m256i data, unsigned short* pmin = 0, unsigned short* pmax = 0) {
	const int stride = sizeof(data) / sizeof(*pmin);
	__m256i result = { 0 };
	unsigned short min = 0;
	unsigned short max = 0;

	int imin = HorizentalMin16(data, &min);
	int imax = HorizentalMax16(data, &max);
	if (pmin != 0)*pmin = min;
	if (pmax != 0)*pmax = max;
	result.m256i_u16[0] = min;
	result.m256i_u16[stride - 1] = max;
	data.m256i_u16[imin] = max;
	for (int i = 1; i < stride - 1; i++)
	{
		imin = HorizentalMin16(data, &min);
		result.m256i_u16[i] = min;
		data.m256i_u16[imin] = max;
	}

	return result;
}
__m512i HorizentalSort16(__m512i data, unsigned short* pmin = 0, unsigned short* pmax = 0) {
	const int stride = sizeof(data) / sizeof(*pmin);
	__m512i result = { 0 };
	int imin = HorizentalMin16(data, pmin);
	int imax = HorizentalMax16(data, pmax);
	unsigned short min = data.m512i_u16[imin];
	unsigned short max = data.m512i_u16[imax];
	if (pmin != 0)*pmin = min;
	if (pmax != 0)*pmax = max;
	result.m512i_u16[0] = min;
	result.m512i_u16[stride - 1] = max;
	data.m512i_u16[imin] = max;
	for (int i = 1; i < stride - 1; i++)
	{
		imin = HorizentalMin16(data, &min);
		result.m512i_u16[i] = min;
		data.m512i_u16[imin] = max;
	}

	return result;
}
__m128i HorizentalSort32(__m128i data, unsigned int* pmin = 0, unsigned int* pmax = 0) {
	const int stride = sizeof(data) / sizeof(*pmin);
	__m128i result = { 0 };
	unsigned int min = 0;
	unsigned int max = 0;

	int imin = HorizentalMin32(data, &min);
	int imax = HorizentalMax32(data, &max);
	if (pmin != 0)*pmin = min;
	if (pmax != 0)*pmax = max;
	result.m128i_u32[0] = min;
	result.m128i_u32[stride - 1] = max;
	data.m128i_u32[imin] = max;
	for (int i = 1; i < stride - 1; i++)
	{
		imin = HorizentalMin32(data, &min);
		result.m128i_u32[i] = min;
		data.m128i_u32[imin] = max;
	}

	return result;
}
__m256i HorizentalSort32(__m256i data, unsigned int* pmin = 0, unsigned int* pmax = 0) {
	const int stride = sizeof(data) / sizeof(*pmin);
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
	for (int i = 1; i < stride - 1; i++)
	{
		imin = HorizentalMin32(data, &min);
		result.m256i_u32[i] = min;
		data.m256i_u32[imin] = max;
	}

	return result;
}
__m512i HorizentalSort32(__m512i data, unsigned int* pmin = 0, unsigned int* pmax = 0) {
	const int stride = sizeof(data) / sizeof(*pmin);
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
	for (int i = 1; i < stride - 1; i++)
	{
		imin = HorizentalMin32(data, &min);
		result.m512i_u32[i] = min;
		data.m512i_u32[imin] = max;
	}

	return result;
}
__m128i HorizentalSort64(__m128i data, unsigned long long* pmin = 0, unsigned long long* pmax = 0) {
	const int stride = sizeof(data) / sizeof(*pmin);
	__m128i result = { 0 };

	int li = HorizentalMin64(data, pmin);
	int hi = HorizentalMax64(data, pmax);
	result.m128i_i64[li] = data.m128i_i64[li];
	result.m128i_i64[hi] = data.m128i_i64[hi];

	return result;
}
__m256i HorizentalSort64(__m256i data, unsigned long long* pmin = 0, unsigned long long* pmax = 0) {
	const int stride = sizeof(data) / sizeof(*pmin);
	__m256i result = { 0 };
	unsigned long long min = 0;
	unsigned long long max = 0;

	int imin = HorizentalMin64(data, &min);
	int imax = HorizentalMax64(data, &max);
	if (pmin != 0)*pmin = min;
	if (pmax != 0)*pmax = max;
	result.m256i_u64[0] = min;
	result.m256i_u64[stride - 1] = max;
	data.m256i_u64[imin] = max;
	for (int i = 1; i < stride - 1; i++)
	{
		imin = HorizentalMin64(data, &min);
		result.m256i_u64[i] = min;
		data.m256i_u64[imin] = max;
	}

	return result;
}
__m512i HorizentalSort64(__m512i data, unsigned long long* pmin = 0, unsigned long long* pmax = 0) {
	const int stride = sizeof(data) / sizeof(*pmin);
	__m512i result = { 0 };
	int imin = HorizentalMin64(data, pmin);
	int imax = HorizentalMax64(data, pmax);
	unsigned long long min = data.m512i_u64[imin];
	unsigned long long max = data.m512i_u64[imax];
	if (pmin != 0)*pmin = min;
	if (pmax != 0)*pmax = max;
	result.m512i_u64[0] = min;
	result.m512i_u64[stride - 1] = max;
	data.m512i_u64[imin] = max;
	for (int i = 1; i < stride - 1; i++)
	{
		imin = HorizentalMin64(data, &min);
		result.m512i_u64[i] = min;
		data.m512i_u64[imin] = max;
	}

	return result;
}

void MergeData(int data[], int left, int mid, int right, int* buffer) {
	int begin1 = left;
	int begin2 = mid;
	int end1 = mid;
	int end2 = right;
	int index = left;
	while (begin1 < end1 && begin2 < end2) {
		if (data[begin1] < data[begin2]) {
			buffer[index++] = data[begin1++];
		}
		else {
			buffer[index++] = data[begin2++];
		}
	}
	while (begin1 < end1) {
		buffer[index++] = data[begin1++];
	}
	while (begin2 < end2) {
		buffer[index++] = data[begin2++];
	}
}
void MergeSortRecursive(int data[], int left, int right, int* buffer) {
	if (right - left <= 1) return;
	int mid = left + ((right - left) >> 1);
	MergeSortRecursive(data, left, mid, buffer);
	MergeSortRecursive(data, mid, right, buffer);
	MergeData(data, left, mid, right, buffer);
	memcpy(data + left, buffer + left, (right - left) * sizeof(data[0]));
}
void MergeSortRecursive(int a[], int n) {
	int* buffer = new int[n];
	MergeSortRecursive(a, 0, n, buffer);
	delete[] buffer;
}
void MergeSortNonRecursive(int data[], int n) {
	int* buffer = new int[n];
	int gap = 1;
	while (gap < n) {
		for (int i = 0; i < n; i += 2 * gap) {
			int left = i;
			int mid = left + gap;
			int right = mid + gap;
			if (mid >= n)
				mid = n;
			if (right >= n)
				right = n;
			MergeData(data, left, mid, right, buffer);
		}
		memcpy_s(data, n * sizeof(data[0]), buffer, n * sizeof(data[0]));
		gap *= 2;
	}
	delete[] buffer;
}
bool FastMergeSort256(int data[], int n) {
	const int stride = sizeof(__m256i) / sizeof(data[0]);
	const int dual = stride << 1;
	if (data == 0 || n < dual || n % dual > 0)
		return false;
	int* buffer = new int[n];

	int gap = stride;
	while (gap < n) {
		int gap2 = gap << 1;
		for (int i = 0; i < n; i += gap2) {
			if (gap == stride) {
				//0 - stride
				__m256i values = _mm256_loadu_epi32(data + i);
				__m256i sorted = HorizentalSort32(values);

				_mm256_storeu_epi32(data + i, sorted);

				//stride - 2*stride
				values = _mm256_loadu_epi32(data + i + stride);
				sorted = HorizentalSort32(values);
				_mm256_storeu_epi32(data + i + stride, sorted);
			}

			int i_left = i;
			int i_middle = i + gap;
			int i_right = i_middle + gap;

			int begin1 = i_left;
			int begin2 = i_middle;
			int end1 = i_middle;
			int end2 = i_right;

			int left_index = i_left;
			int right_index = i_right;
#if 0 //takes more time than expected
			int p1 = 0;
			int p2 = 0;
			__m256i left = { 0 };
			__m256i right = { 0 };
			while (begin1 < end1 && begin2 < end2) {
				if (p1 == 0) {
					left = _mm256_loadu_epi32(data + begin1);
				}
				if (p2 == 0) {
					right = _mm256_loadu_epi32(data + begin2);
				}

				int v1 = left.m256i_i32[p1];
				int v2 = right.m256i_i32[p2];
				if (v1 < v2) {
					buffer[left_index++] = v1;
					begin1++;
				}
				else {
					buffer[left_index++] = v2;
					begin2++;
				}

				p1 = (begin1 - i_left) % stride;
				p2 = (begin2 - i_middle) % stride;
			}
#else
			while (begin1 < end1 && begin2 < end2) {
				if (data[begin1] < data[begin2]) {
					buffer[left_index++] = data[begin1++];
				}
				else {
					buffer[left_index++] = data[begin2++];
				}
			}

#endif
			int delta = end1 - begin1;
			if (delta > 0) {
				memcpy_s(buffer + left_index, sizeof(int) * delta, data + begin1, sizeof(int) * delta);
				left_index += delta;
			}
			delta = end2 - begin2;
			if (delta > 0) {
				memcpy_s(buffer + left_index, sizeof(int) * delta, data + begin2, sizeof(int) * delta);
			}
			memcpy_s(data + i, sizeof(int) * gap2, buffer + i, sizeof(int) * gap2);
		}
		gap = gap2;
	}
	delete[] buffer;
	return true;
}
bool FastMergeSort512(int data[], int n) {
	const int stride = sizeof(__m512i) / sizeof(data[0]);
	const int dual = stride << 1;
	if (data == 0 || n < dual || n % dual > 0)
		return false;
	int* buffer = new int[n];

	int gap = stride;
	while (gap < n) {
		int gap2 = gap << 1;
		for (int i = 0; i < n; i += gap2) {
			if (gap == stride) {
				//0 - stride
				__m512i values = _mm512_loadu_epi32(data + i);
				__m512i sorted = HorizentalSort32(values);
				_mm512_storeu_epi32(data + i, sorted);

				//stride - 2*stride
				values = _mm512_loadu_epi32(data + i + stride);
				sorted = HorizentalSort32(values);
				_mm512_storeu_epi32(data + i + stride, sorted);
			}

			int i_left = i;
			int i_middle = i + gap;
			int i_right = i_middle + gap;

			int begin1 = i_left;
			int begin2 = i_middle;
			int end1 = i_middle;
			int end2 = i_right;

			int left_index = i_left;
			int right_index = i_right;

#if 0 //takes more time than expected
			int p1 = 0;
			int p2 = 0;
			__m512i left = { 0 };
			__m512i right = { 0 };
			while (begin1 < end1 && begin2 < end2) {
				if (p1 == 0) {
					left = _mm512_loadu_epi32(data + begin1);
				}
				if (p2 == 0) {
					right = _mm512_loadu_epi32(data + begin2);
				}
				int v1 = left.m512i_i32[p1];
				int v2 = right.m512i_i32[p2];
				if (v1 < v2) {
					buffer[left_index++] = v1;
					begin1++;
				}
				else {
					buffer[left_index++] = v2;
					begin2++;
				}

				p1 = (begin1 - i_left) % stride;
				p2 = (begin2 - i_middle) % stride;
			}
#else

			while (begin1 < end1 && begin2 < end2) {
				if (data[begin1] < data[begin2]) {
					buffer[left_index++] = data[begin1++];
				}
				else {
					buffer[left_index++] = data[begin2++];
				}
			}
#endif
			int delta = end1 - begin1;
			if (delta > 0) {
				memcpy_s(buffer + left_index, sizeof(int) * delta, data + begin1, sizeof(int) * delta);
				left_index += delta;
			}
			delta = end2 - begin2;
			if (delta > 0) {
				memcpy_s(buffer + left_index, sizeof(int) * delta, data + begin2, sizeof(int) * delta);
			}
			memcpy_s(data + i, sizeof(int) * gap2, buffer + i, sizeof(int) * gap2);

		}
		gap = gap2;
	}
	delete[] buffer;
	return true;
}

bool SingleSelectionSort(int data[], int n) {
	for (int i = 0; i < n - 1; i++)
	{
		int minIndex = i;
		for (int j = i + 1; j < n; j++)
		{
			if (data[j] < data[minIndex])
			{
				minIndex = j;
			}
		}
		if (i != minIndex) Swap(data, i, minIndex);
	}
	return true;
}
void DoMerge256(int data[], int n) {
	const int stride = sizeof(__m256i) / sizeof(data[0]);
	//merege:
	int merge_indices[stride] = { 0,1,2,3,4,5,6,7 };
	int* buffer = new int[n];
	//memset(buffer, 0, n * sizeof(int));
	for (int i = 0; i < n; i++)
	{
		int min_value = 0;
		int min_index = -1;
		bool first = true;
		for (int j = 0; j < stride; j++) {
			int index = merge_indices[j];
			if (index < n) {
				if (first) {
					min_value = data[index];
					min_index = j;
					first = false;
				}
				else
				{
					if (data[index] < min_value) {
						min_value = data[index];
						min_index = j;
					}
				}
			}
		}
		if (min_index >= 0) {
			merge_indices[min_index] += stride;
		}
		if (first) break;
		buffer[i] = min_value;
	}
	memcpy_s(data, n * sizeof(int), buffer, n * sizeof(int));
	delete[]buffer;

}
bool FastSingleSelectionSort256(int data[], int n) {
	const int stride = sizeof(__m256i) / sizeof(data[0]);

	if (data == 0 || n < stride || n % stride>0) return false;

	for (int i = 0; i <= n - stride; i += stride) {

		__m256i minValues = _mm256_loadu_epi32(data + i);
		__m256i i_Indices = _mm256_setr_epi32(i + 0, i + 1, i + 2, i + 3, i + 4, i + 5, i + 6, i + 7);
		__m256i minIndices = i_Indices;

		for (int j = i + stride; j < n; j += stride) {

			__m256i values = _mm256_loadu_epi32(data + j);

			__mmask8 mask = _mm256_cmplt_epi32_mask(values, minValues);
			minIndices = _mm256_mask_blend_epi32(mask,
				minIndices,
				_mm256_setr_epi32(j + 0, j + 1, j + 2, j + 3, j + 4, j + 5, j + 6, j + 7));

			minValues = _mm256_i32gather_epi32(data, minIndices, sizeof(int));
		}
		long mask = _mm256_cmpneq_epi32_mask(
			minIndices, i_Indices);

		unsigned long idx = 0;
		while (_BitScanForward(&idx, mask))
		{
			_bittestandreset(&mask, idx);
			int tp = minIndices.m256i_i32[idx];
			Swap(data, i + idx, tp);
		}
	}

	DoMerge256(data, n);

	return true;
}
void DoMerge512(int data[], int n) {
	const int stride = sizeof(__m512i) / sizeof(data[0]);

	//merege:
	int merge_indices[stride] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };
	int* buffer = new int[n];
	//memset(buffer, 0, n * sizeof(int));
	for (int i = 0; i < n; i++)
	{
		int min_value = 0;
		int min_index = -1;
		bool first = true;
		for (int j = 0; j < stride; j++) {
			int index = merge_indices[j];
			if (index < n) {
				if (first) {
					min_value = data[index];
					min_index = j;
					first = false;
				}
				else
				{
					if (data[index] < min_value) {
						min_value = data[index];
						min_index = j;
					}
				}
			}
		}
		if (min_index >= 0) {
			merge_indices[min_index] += stride;
		}
		if (first) break;
		buffer[i] = min_value;
	}
	memcpy_s(data, n * sizeof(int), buffer, n * sizeof(int));
	delete[]buffer;

}
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

			__m512i values = _mm512_loadu_epi32(data + j);

			__mmask16 mask = _mm512_cmplt_epi32_mask(values, minValues);
			minIndices = _mm512_mask_blend_epi32(mask,
				minIndices,
				_mm512_setr_epi32(
					j + 0, j + 1, j + 2, j + 3,
					j + 4, j + 5, j + 6, j + 7,
					j + 8, j + 9, j + 10, j + 11,
					j + 12, j + 13, j + 14, j + 15
					));

			minValues = _mm512_i32gather_epi32(minIndices, data, sizeof(int));
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
	DoMerge512(data, n);

	return true;
}
bool DoubleSelectionSort(int data[], int n)
{
	if (data == 0 || n<1)
	{
		return false;
	}
	int staIndex = 0;
	int endIndex = n - 1;
	while (staIndex < endIndex)
	{
		int minIndex = staIndex;
		int maxIndex = endIndex;

		int staValue = data[staIndex];
		int endValue = data[endIndex];
		int minValue = data[minIndex];
		int maxValue = data[maxIndex];

		for (int j = staIndex; j <= endIndex; j++)
		{
			if (data[j] < minValue)
			{
				minValue = data[j];
				minIndex = j;
			}
			if (data[j] > maxValue)
			{
				maxValue = data[j];
				maxIndex = j;
			}
		}
		if (minValue == maxValue)
		{
			break;
		}
		else
		{
			data[staIndex] = minValue;
			data[endIndex] = maxValue;

			bool both = maxIndex == staIndex && minIndex == endIndex;
			if (!both && maxIndex != staIndex && minIndex != endIndex)
			{
				data[minIndex] = staValue;
				data[maxIndex] = endValue;
			}
			if (!both && maxIndex == staIndex)
			{
				data[minIndex] = endValue;
			}
			if (!both && minIndex == endIndex)
			{
				data[maxIndex] = staValue;
			}
		}

		endIndex--;
		staIndex++;
	}
	return true;
}
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

			//only add for these having 1s
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
	DoMerge256(data, n);
	return true;
}
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

		__m512i staValue = _mm512_i32gather_epi32(staIndex, data, sizeof(int));
		__m512i endValue = _mm512_i32gather_epi32(endIndex, data, sizeof(int));
		__m512i minValue = staValue;
		__m512i maxValue = endValue;

		__m512i j = staIndex;
		while (true) {
			__mmask16 le = _mm512_cmple_epi32_mask(j, endIndex);
			if (le == 0) break;//any 1, still le

			__m512i data_j = _mm512_i32gather_epi32(j, data, sizeof(int));

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
		_mm512_mask_i32scatter_epi32(data, ~0, staIndex, minValue, sizeof(int));
		_mm512_mask_i32scatter_epi32(data, ~0, endIndex, maxValue, sizeof(int));

		__mmask16 max_sta = _mm512_cmpeq_epi32_mask(maxIndex, staIndex);
		__mmask16 min_end = _mm512_cmpeq_epi32_mask(minIndex, endIndex);
		__mmask16 both = max_sta & min_end;
		__mmask16 not_max_sta = ~max_sta;
		__mmask16 not_min_end = ~min_end;
		__mmask16 both_not = not_max_sta & not_min_end;

		//if (!both && both_not)
		//	data[minIndex] = staValue;
		//	data[maxIndex] = endValue;
		_mm512_mask_i32scatter_epi32(data, (~both) & both_not, minIndex, staValue, sizeof(int));
		_mm512_mask_i32scatter_epi32(data, (~both) & both_not, maxIndex, endValue, sizeof(int));

		//if (!both && maxIndex == staIndex)
		//data[minIndex] = endValue;
		_mm512_mask_i32scatter_epi32(data, (~both) & max_sta, minIndex, endValue, sizeof(int));
		//if (!both && minIndex == endIndex)
		//data[maxIndex] = staValue;
		_mm512_mask_i32scatter_epi32(data, (~both) & min_end, maxIndex, staValue, sizeof(int));

		//next
		staIndex = _mm512_mask_add_epi32(staIndex, lt, staIndex, _mm512_set1_epi32(stride));
		endIndex = _mm512_mask_sub_epi32(endIndex, lt, endIndex, _mm512_set1_epi32(stride));
	}
	DoMerge512(data, n);
	return true;
}

int AVX2_SUM(int data[], size_t size)
{
	const int stride = sizeof(__m256i) / sizeof(data[0]);
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
	const int stride = sizeof(__m512i) / sizeof(data[0]);
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
	const int stride = sizeof(__m256i) / sizeof(data[0]);
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
	const int stride = sizeof(__m512i) / sizeof(data[0]);
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
		const int stride = sizeof(__m256i) / sizeof(*s);
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
		const int stride = sizeof(__m256i) / sizeof(*s);
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
		const int stride = sizeof(__m512i) / sizeof(*s);
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
		const int stride = sizeof(__m512i) / sizeof(*s);
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
	const int stride = sizeof(__m256i) / sizeof(*s1);

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
	const int stride = sizeof(__m256i) / sizeof(*s1);

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
	const int stride = sizeof(__m512i) / sizeof(*s1);

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
	const int stride = sizeof(__m512i) / sizeof(*s1);

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
	const int stride = sizeof(__m256i) / sizeof(*s1);

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
			if (bet && iet < most) return false;
		}
		return true;//all the same
	}
}
bool AVX2_StringEqual(wchar_t* s1, wchar_t* s2)
{
	const int stride = sizeof(__m256i) / sizeof(*s1);

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
	const int stride = sizeof(__m512i) / sizeof(*s1);

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
	const int stride = sizeof(__m512i) / sizeof(*s1);

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
		const int stride = sizeof(__m256i) / sizeof(*s);
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
		const int stride = sizeof(__m256i) / sizeof(*s);
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
		const int stride = sizeof(__m512i) / sizeof(*s);
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
		const int stride = sizeof(__m512i) / sizeof(*s);
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
int AVX512_StringIndexOf(wchar_t* s, wchar_t* cs)
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
bool CheckSequence(int a[], int b[], int n) {
	for (int i = 0; i < n; i++) {
		if (a[i] != b[i])
			return false;
	}
	return true;
}

const int DATA_SIZE = 16384;// *16 * 16;
const bool use_random = true;
const bool show = false;

//int data0[DATA_SIZE] = { 0 };
//int data0[DATA_SIZE] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };
//int data0[DATA_SIZE] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31 };
//int data0[DATA_SIZE] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63 };
int data0[DATA_SIZE] = { 12,2,23,25,17,1,16,19,31,19,1,23,31,19,13,25,26,14,10,28,31,25,7,14,17,3,25,24,21,22,8,14 };
//int data0[DATA_SIZE] = { 15,2,1,4,3,9,8,6,7,10,12,11,0,5,14,13 };
int data1[DATA_SIZE] = { 0 };
int data2[DATA_SIZE] = { 0 };
int data3[DATA_SIZE] = { 0 };
int data4[DATA_SIZE] = { 0 };
int data5[DATA_SIZE] = { 0 };
int data6[DATA_SIZE] = { 0 };
int data7[DATA_SIZE] = { 0 };
int data8[DATA_SIZE] = { 0 };
int data9[DATA_SIZE] = { 0 };
int data10[DATA_SIZE] = { 0 };
int data11[DATA_SIZE] = { 0 };
int data12[DATA_SIZE] = { 0 };
int data13[DATA_SIZE] = { 0 };
int data14[DATA_SIZE] = { 0 };
int data15[DATA_SIZE] = { 0 };
int data16[DATA_SIZE] = { 0 };

int main()
{
#if 0
	__m128i __data8 = _mm_setr_epi8(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
	__m128i _data16 = _mm_setr_epi16(5, 3, 1, 4, 2, 9, 7, 6); //0 is last
	__m256i _data32 = _mm256_setr_epi32(
		0x00050055,
		0x00240044,
		0x0002ffff,
		0x0002ffff,
		0x00050033,
		0x0002ffff,
		0x00170077,
		0x00030033
	);

	unsigned char r8 = 0;
	unsigned short r16 = 0;
	unsigned int r32 = 0;
	unsigned long long r64 = 0;
	int i = 0;

	i = HorizentalMin8(__data8, &r8);
	i = HorizentalMax8(__data8, &r8);
	i = HorizentalMin16(_data16, &r16);
	i = HorizentalMax16(_data16, &r16);
	i = HorizentalMin32(_data32, &r32);
	i = HorizentalMax32(_data32, &r32);
	i = HorizentalMin64(_data32, &r64);
	i = HorizentalMax64(_data32, &r64);
#endif
	long long t0;
	//init
	if (true)
	{
		srand((unsigned)time(0));
		printf("original data(count = %d):\n", DATA_SIZE);
		for (int i = 0; i < DATA_SIZE; i++) {
			if (use_random) {
				data0[i] = (int)((rand() / (double)RAND_MAX) * DATA_SIZE);
			}
			data16[i]
				= data15[i]
				= data14[i]
				= data13[i]
				= data12[i]
				= data11[i]
				= data10[i]
				= data9[i]
				= data8[i]
				= data7[i]
				= data6[i]
				= data5[i]
				= data4[i]
				= data3[i]
				= data2[i]
				= data1[i]
				= data0[i];
			if (show) {
				printf("%d ", data0[i]);
			}
		}
		printf("\n\n");
	}
	//system quick sort
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
	//quick sort
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
	//fast quick sort 256
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
	//fast quick sort 512
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
	//odd even sort
	if (false)
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
	//odd even sort 256
	if (false)
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
	//odd even sort 512
	if (false)
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
	//merge sort recursive
	if (true)
	{
		printf("for merge sort recursive:\n");
		t0 = _Query_perf_counter();
		{
			MergeSortRecursive(data9, DATA_SIZE);
		}
		printf("time:%lf(ms)\n",
			((_Query_perf_counter() - t0) / (double)_Query_perf_frequency() * 1000.0));
		bool b = CheckSequence(data0, data9, DATA_SIZE);
		printf("correct:%s\n", b ? "true" : "false");

		if (!b)
		{
			for (int i = 0; i < DATA_SIZE; i++) {
				printf("%d ", data9[i]);
			}
		}
		printf("\n\n");
	}
	//merge sort non recursive
	if (true)
	{
		printf("for merge sort non-recursive:\n");
		t0 = _Query_perf_counter();
		{
			MergeSortRecursive(data10, DATA_SIZE);
		}
		printf("time:%lf(ms)\n",
			((_Query_perf_counter() - t0) / (double)_Query_perf_frequency() * 1000.0));
		bool b = CheckSequence(data0, data10, DATA_SIZE);
		printf("correct:%s\n", b ? "true" : "false");

		if (!b)
		{
			for (int i = 0; i < DATA_SIZE; i++) {
				printf("%d ", data10[i]);
			}
		}
		printf("\n\n");
	}
	//fast merge sort 256
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
	//fast merge sort 512
	if (true)
	{
		printf("for fast merge sort 512:\n");
		t0 = _Query_perf_counter();
		{
			FastMergeSort512(data8, DATA_SIZE);
		}
		printf("time:%lf(ms)\n",
			((_Query_perf_counter() - t0) / (double)_Query_perf_frequency() * 1000.0));
		bool b = CheckSequence(data0, data8, DATA_SIZE);
		printf("correct:%s\n", b ? "true" : "false");

		if (!b)
		{
			for (int i = 0; i < DATA_SIZE; i++) {
				printf("%d ", data8[i]);
			}
		}
		printf("\n\n");
	}
	//single selection sort
	if (true)
	{
		printf("for single selection sort:\n");
		t0 = _Query_perf_counter();
		{
			SingleSelectionSort(data11, DATA_SIZE);
		}
		printf("time:%lf(ms)\n",
			((_Query_perf_counter() - t0) / (double)_Query_perf_frequency() * 1000.0));
		bool b = CheckSequence(data0, data11, DATA_SIZE);
		printf("correct:%s\n", b ? "true" : "false");

		if (!b)
		{
			for (int i = 0; i < DATA_SIZE; i++) {
				printf("%d ", data11[i]);
			}
		}
		printf("\n\n");
	}
	//single selection sort 256
	if (true)
	{
		printf("for fast single selection sort 256:\n");
		t0 = _Query_perf_counter();
		{
			FastSingleSelectionSort256(data12, DATA_SIZE);
		}
		printf("time:%lf(ms)\n",
			((_Query_perf_counter() - t0) / (double)_Query_perf_frequency() * 1000.0));
		bool b = CheckSequence(data0, data12, DATA_SIZE);
		printf("correct:%s\n", b ? "true" : "false");

		if (!b)
		{
			for (int i = 0; i < DATA_SIZE; i++) {
				printf("%d", data12[i]);
				if (data12[i] != data0[i]) {
					printf("[%d], ", data0[i]);
				}
				else
					printf(", ");
			}
		}
		printf("\n\n");
	}
	//single selection sort 512
	if (true)
	{
		printf("for fast single selection sort 512:\n");
		t0 = _Query_perf_counter();
		{
			FastSingleSelectionSort512(data13, DATA_SIZE);
		}
		printf("time:%lf(ms)\n",
			((_Query_perf_counter() - t0) / (double)_Query_perf_frequency() * 1000.0));
		bool b = CheckSequence(data0, data13, DATA_SIZE);
		printf("correct:%s\n", b ? "true" : "false");

		if (!b)
		{
			for (int i = 0; i < DATA_SIZE; i++) {
				printf("%d", data13[i]);
				if (data13[i] != data0[i]) {
					printf("[%d], ", data0[i]);
				}
				else
					printf(", ");
			}
		}
		printf("\n\n");
	}
	//double selection sort
	if (true)
	{
		printf("for double selection sort:\n");
		t0 = _Query_perf_counter();
		{
			DoubleSelectionSort(data14, DATA_SIZE);
		}
		printf("time:%lf(ms)\n",
			((_Query_perf_counter() - t0) / (double)_Query_perf_frequency() * 1000.0));
		bool b = CheckSequence(data0, data14, DATA_SIZE);
		printf("correct:%s\n", b ? "true" : "false");

		if (!b)
		{
			for (int i = 0; i < DATA_SIZE; i++) {
				printf("%d ", data14[i]);
			}
		}
		printf("\n\n");
	}
	//double selection sort 256
	if (true)
	{
		printf("for fast double selection sort 256:\n");
		t0 = _Query_perf_counter();
		{
			FastDoubleSelectionSort256(data15, DATA_SIZE);
		}
		printf("time:%lf(ms)\n",
			((_Query_perf_counter() - t0) / (double)_Query_perf_frequency() * 1000.0));
		bool b = CheckSequence(data0, data15, DATA_SIZE);
		printf("correct:%s\n", b ? "true" : "false");

		if (!b)
		{
			for (int i = 0; i < DATA_SIZE; i++) {
				printf("%d ", data15[i]);
			}
		}
		printf("\n\n");
	}
	//double selection sort 512
	if (true)
	{
		printf("for fast double selection sort 512:\n");
		t0 = _Query_perf_counter();
		{
			FastDoubleSelectionSort512(data16, DATA_SIZE);
		}
		printf("time:%lf(ms)\n",
			((_Query_perf_counter() - t0) / (double)_Query_perf_frequency() * 1000.0));
		bool b = CheckSequence(data0, data16, DATA_SIZE);
		printf("correct:%s\n", b ? "true" : "false");

		if (!b)
		{
			for (int i = 0; i < DATA_SIZE; i++) {
				printf("%d ", data16[i]);
			}
		}
		printf("\n\n");
	}
	return 0;
}
