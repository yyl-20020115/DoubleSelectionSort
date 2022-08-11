#include <intrin.h>
int FastSum256(int data[], size_t size)
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
long long FastSum256(long long data[], size_t size)
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
