#include "horizontal_sort_256.h"
#include "horizontal_max_min_256.h"
#include "utils_256.h"
__m128i HorizontalSort8(__m128i data, unsigned char* pmin, unsigned char* pmax) {
	const int stride = sizeof(data) / sizeof(*pmin);
	__m128i result = { 0 };
	unsigned char min = 0;
	unsigned char max = 0;

	int imin = HorizontalMin8(data, &min);
	int imax = HorizontalMax8(data, &max);
	if (pmin != 0)*pmin = min;
	if (pmax != 0)*pmax = max;
	result.m128i_u8[0] = min;
	result.m128i_u8[stride - 1] = max;
	data.m128i_u8[imin] = max;
	for (int i = 1; i < stride - 1; i++)
	{
		imin = HorizontalMin8(data, &min);
		result.m128i_u8[i] = min;
		data.m128i_u8[imin] = max;
	}

	return result;
}
__m256i HorizontalSort8(__m256i data, unsigned char* pmin, unsigned char* pmax) {
	const int stride = sizeof(data) / sizeof(*pmin);
	__m256i result = { 0 };
	unsigned char min = 0;
	unsigned char max = 0;

	int imin = HorizontalMin8(data, &min);
	int imax = HorizontalMax8(data, &max);
	if (pmin != 0)*pmin = min;
	if (pmax != 0)*pmax = max;
	result.m256i_u8[0] = min;
	result.m256i_u8[stride - 1] = max;
	data.m256i_u8[imin] = max;
	for (int i = 1; i < stride - 1; i++)
	{
		imin = HorizontalMin8(data, &min);
		result.m256i_u8[i] = min;
		data.m256i_u8[imin] = max;
	}

	return result;
}
__m128i HorizontalSort16(__m128i data, unsigned short* pmin, unsigned short* pmax) {
	const int stride = sizeof(data) / sizeof(*pmin);
	__m128i result = { 0 };
	unsigned short min = 0;
	unsigned short max = 0;

	int imin = HorizontalMin16(data, &min);
	int imax = HorizontalMax16(data, &max);
	if (pmin != 0)*pmin = min;
	if (pmax != 0)*pmax = max;
	result.m128i_u16[0] = min;
	result.m128i_u16[stride - 1] = max;
	data.m128i_u16[imin] = max;
	for (int i = 1; i < stride - 1; i++)
	{
		imin = HorizontalMin16(data, &min);
		result.m128i_u16[i] = min;
		data.m128i_u16[imin] = max;
	}

	return result;
}
__m256i HorizontalSort16(__m256i data, unsigned short* pmin, unsigned short* pmax) {
	const int stride = sizeof(data) / sizeof(*pmin);
	__m256i result = { 0 };
	unsigned short min = 0;
	unsigned short max = 0;

	int imin = HorizontalMin16(data, &min);
	int imax = HorizontalMax16(data, &max);
	if (pmin != 0)*pmin = min;
	if (pmax != 0)*pmax = max;
	result.m256i_u16[0] = min;
	result.m256i_u16[stride - 1] = max;
	data.m256i_u16[imin] = max;
	for (int i = 1; i < stride - 1; i++)
	{
		imin = HorizontalMin16(data, &min);
		result.m256i_u16[i] = min;
		data.m256i_u16[imin] = max;
	}

	return result;
}
__m128i HorizontalSort32(__m128i data, unsigned int* pmin, unsigned int* pmax) {
	const int stride = sizeof(data) / sizeof(*pmin);
	__m128i result = { 0 };
	unsigned int min = 0;
	unsigned int max = 0;

	int imin = HorizontalMin32(data, &min);
	int imax = HorizontalMax32(data, &max);
	if (pmin != 0)*pmin = min;
	if (pmax != 0)*pmax = max;
	result.m128i_u32[0] = min;
	result.m128i_u32[stride - 1] = max;
	data.m128i_u32[imin] = max;
	for (int i = 1; i < stride - 1; i++)
	{
		imin = HorizontalMin32(data, &min);
		result.m128i_u32[i] = min;
		data.m128i_u32[imin] = max;
	}

	return result;
}
__m256i HorizontalSort32(__m256i data, unsigned int* pmin, unsigned int* pmax) {
	const int stride = sizeof(data) / sizeof(*pmin);
	__m256i result = { 0 };
	unsigned int min = 0;
	unsigned int max = 0;

	int imin = HorizontalMin32(data, &min);
	int imax = HorizontalMax32(data, &max);
	if (pmin != 0)*pmin = min;
	if (pmax != 0)*pmax = max;
	result.m256i_u32[0] = min;
	result.m256i_u32[stride - 1] = max;
	data.m256i_u32[imin] = max;
	for (int i = 1; i < stride - 1; i++)
	{
		imin = HorizontalMin32(data, &min);
		result.m256i_u32[i] = min;
		data.m256i_u32[imin] = max;
	}

	return result;
}
__m128i HorizontalSort64(__m128i data, unsigned long long* pmin, unsigned long long* pmax) {
	const int stride = sizeof(data) / sizeof(*pmin);
	__m128i result = { 0 };

	int li = HorizontalMin64(data, pmin);
	int hi = HorizontalMax64(data, pmax);
	result.m128i_i64[li] = data.m128i_i64[li];
	result.m128i_i64[hi] = data.m128i_i64[hi];

	return result;
}
__m256i HorizontalSort64(__m256i data, unsigned long long* pmin, unsigned long long* pmax) {
	const int stride = sizeof(data) / sizeof(*pmin);
	__m256i result = { 0 };
	unsigned long long min = 0;
	unsigned long long max = 0;

	int imin = HorizontalMin64(data, &min);
	int imax = HorizontalMax64(data, &max);
	if (pmin != 0)*pmin = min;
	if (pmax != 0)*pmax = max;
	result.m256i_u64[0] = min;
	result.m256i_u64[stride - 1] = max;
	data.m256i_u64[imin] = max;
	for (int i = 1; i < stride - 1; i++)
	{
		imin = HorizontalMin64(data, &min);
		result.m256i_u64[i] = min;
		data.m256i_u64[imin] = max;
	}

	return result;
}
