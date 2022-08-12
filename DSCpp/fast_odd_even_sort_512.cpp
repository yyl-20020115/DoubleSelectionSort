#include "utils_512.h"
#include <intrin.h>
#include <memory>

bool FastOddEvenSort512(int data[], int n)
{
	const int size = sizeof(__m512i) / sizeof(int);
	const int last = size - 1;
	const int half = size >> 1;
	const int skip = sizeof(int);
	const int dbls = (size << 1);
	const int quad = dbls << 1;

	if (n < dbls || n % dbls>0) {
		return false;
	}
	int* buffer = new int[n + 1];
	memcpy_s(buffer, (n) * sizeof(int), data, (n) * sizeof(int));
	buffer[n] = 0;

	__m256i zeros = _mm256_setzero_si256();
	__m256i ones = _mm256_cmpeq_epi32(zeros, zeros);
	__m512i ipo = _mm512_setr_epi32(1, 3, 5, 7, 9, 11, 13, 15, 17, 19, 21, 23, 25, 27, 29, 31);
	__m512i ipe = _mm512_setr_epi32(0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30);
	__m512i ipt = _mm512_setr_epi32(2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32);

	for (int repeat = 0; repeat < n / 2; repeat++)
	{
		for (int part = 0; part < n; part += dbls)
		{
			int* ptr = buffer + part;

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
			if (dbls >= n - part - 1) buffer[n] = buffer[n - 1];

			int* ptr = buffer + part;

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

	memcpy_s(data, (n) * sizeof(int), buffer, (n) * sizeof(int));

	delete[] buffer;

	return true;
}
bool FastOddEvenSort512(unsigned int data[], int n)
{
	const int size = sizeof(__m512i) / sizeof(int);
	const int last = size - 1;
	const int half = size >> 1;
	const int skip = sizeof(int);
	const int dbls = (size << 1);
	const int quad = dbls << 1;

	if (n < dbls || n % dbls>0) {
		return false;
	}
	unsigned int* buffer = new unsigned int[n + 1];
	memcpy_s(buffer, (n) * sizeof(int), data, (n) * sizeof(int));
	buffer[n] = 0;

	__m256i zeros = _mm256_setzero_si256();
	__m256i ones = _mm256_cmpeq_epi32(zeros, zeros);
	__m512i ipo = _mm512_setr_epi32(1, 3, 5, 7, 9, 11, 13, 15, 17, 19, 21, 23, 25, 27, 29, 31);
	__m512i ipe = _mm512_setr_epi32(0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30);
	__m512i ipt = _mm512_setr_epi32(2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32);

	for (int repeat = 0; repeat < n / 2; repeat++)
	{
		for (int part = 0; part < n; part += dbls)
		{
			unsigned int* ptr = buffer + part;

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
			if (dbls >= n - part - 1) buffer[n] = buffer[n - 1];

			unsigned int* ptr = buffer + part;

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

	memcpy_s(data, (n) * sizeof(int), buffer, (n) * sizeof(int));

	delete[] buffer;

	return true;
}
