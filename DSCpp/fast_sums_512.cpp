#include <intrin.h>
int FastSum512(int data[], size_t size)
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
long long FastSum512(long long data[], size_t size)
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
