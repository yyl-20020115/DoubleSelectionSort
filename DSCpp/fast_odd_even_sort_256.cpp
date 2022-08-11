#include <intrin.h>
#include <memory>

bool FastOddEvenSort256(int data[], int n)
{
	const int size = sizeof(__m256i) / sizeof(int);
	const int last = size - 1;
	const int half = size >> 1;
	const int skip = sizeof(int);
	const int dbls = (size << 1);
	const int quad = dbls << 1;

	if (n < dbls || n % dbls>0) {
		return false;
	}
	int* buffer = new int[(size_t)((long long)n + 1)];
	memcpy_s(buffer, (n) * sizeof(int), data, (n) * sizeof(int));
	buffer[n] = 0;

	__m256i zeros = _mm256_setzero_si256();
	__m256i ones = _mm256_cmpeq_epi32(zeros, zeros);
	__m256i ipo = _mm256_setr_epi32(1, 3, 5, 7, 9, 11, 13, 15);
	__m256i ipe = _mm256_setr_epi32(0, 2, 4, 6, 8, 10, 12, 14);
	__m256i ipt = _mm256_setr_epi32(2, 4, 6, 8, 10, 12, 14, 16);

	for (int repeat = 0; repeat < n / 2; repeat++)
	{
		for (int part = 0; part < n; part += dbls)
		{
			int* ptr = buffer + part;

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
			if (dbls >= n - part - 1) buffer[n] = buffer[n - 1];

			int* ptr = buffer + part;

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

	memcpy_s(data, (n) * sizeof(int), buffer, (n) * sizeof(int));

	delete[] buffer;

	return true;
}
