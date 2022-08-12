#include "horizontal_max_min_512.h"
#include "utils_512.h"
__m512i HorizontalSort8(__m512i data, unsigned char* pmin, unsigned char* pmax) {
	const int stride = sizeof(data) / sizeof(*pmin);
	__m512i result = { 0 };
	int imin = HorizontalMin8(data, pmin);
	int imax = HorizontalMax8(data, pmax);
	unsigned char min = data.m512i_u8[imin];
	unsigned char max = data.m512i_u8[imax];
	if (pmin != 0)*pmin = min;
	if (pmax != 0)*pmax = max;
	result.m512i_u8[0] = min;
	result.m512i_u8[stride - 1] = max;
	data.m512i_u8[imin] = max;
	for (int i = 1; i < stride - 1; i++)
	{
		imin = HorizontalMin8(data, &min);
		result.m512i_u8[i] = min;
		data.m512i_u8[imin] = max;
	}

	return result;
}
__m512i HorizontalSort16(__m512i data, unsigned short* pmin, unsigned short* pmax) {
	const int stride = sizeof(data) / sizeof(*pmin);
	__m512i result = { 0 };
	int imin = HorizontalMin16(data, pmin);
	int imax = HorizontalMax16(data, pmax);
	unsigned short min = data.m512i_u16[imin];
	unsigned short max = data.m512i_u16[imax];
	if (pmin != 0)*pmin = min;
	if (pmax != 0)*pmax = max;
	result.m512i_u16[0] = min;
	result.m512i_u16[stride - 1] = max;
	data.m512i_u16[imin] = max;
	for (int i = 1; i < stride - 1; i++)
	{
		imin = HorizontalMin16(data, &min);
		result.m512i_u16[i] = min;
		data.m512i_u16[imin] = max;
	}

	return result;
}
__m512i HorizontalSort32(__m512i data, unsigned int* pmin, unsigned int* pmax) {
	const int stride = sizeof(data) / sizeof(*pmin);
	__m512i result = { 0 };
	int imin = HorizontalMin32(data, pmin);
	int imax = HorizontalMax32(data, pmax);
	unsigned int min = data.m512i_u32[imin];
	unsigned int max = data.m512i_u32[imax];
	if (pmin != 0)*pmin = min;
	if (pmax != 0)*pmax = max;
	result.m512i_u32[0] = min;
	result.m512i_u32[stride - 1] = max;
	data.m512i_u32[imin] = max;
	for (int i = 1; i < stride - 1; i++)
	{
		imin = HorizontalMin32(data, &min);
		result.m512i_u32[i] = min;
		data.m512i_u32[imin] = max;
	}

	return result;
}
__m512i HorizontalSort64(__m512i data, unsigned long long* pmin, unsigned long long* pmax) {
	const int stride = sizeof(data) / sizeof(*pmin);
	__m512i result = { 0 };
	int imin = HorizontalMin64(data, pmin);
	int imax = HorizontalMax64(data, pmax);
	unsigned long long min = data.m512i_u64[imin];
	unsigned long long max = data.m512i_u64[imax];
	if (pmin != 0)*pmin = min;
	if (pmax != 0)*pmax = max;
	result.m512i_u64[0] = min;
	result.m512i_u64[stride - 1] = max;
	data.m512i_u64[imin] = max;
	for (int i = 1; i < stride - 1; i++)
	{
		imin = HorizontalMin64(data, &min);
		result.m512i_u64[i] = min;
		data.m512i_u64[imin] = max;
	}

	return result;
}
