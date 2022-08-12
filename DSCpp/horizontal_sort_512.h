#pragma once
#include <intrin.h>

__m512i HorizontalSort8(__m512i data, unsigned char* pmin = 0, unsigned char* pmax = 0);
__m512i HorizontalSort16(__m512i data, unsigned short* pmin = 0, unsigned short* pmax = 0);
__m512i HorizontalSort32(__m512i data, unsigned int* pmin = 0, unsigned int* pmax = 0);
__m512i HorizontalSort64(__m512i data, unsigned long long* pmin = 0, unsigned long long* pmax = 0);

__m512i HorizontalSort8(__m512i data, char* pmin = 0, char* pmax = 0);
__m512i HorizontalSort16(__m512i data, short* pmin = 0, short* pmax = 0);
__m512i HorizontalSort32(__m512i data, int* pmin = 0, int* pmax = 0);
__m512i HorizontalSort64(__m512i data, long long* pmin = 0, long long* pmax = 0);
