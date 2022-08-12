#pragma once
#include <intrin.h>
__m128i HorizontalSort8(__m128i data, unsigned char* pmin = 0, unsigned char* pmax = 0);
__m256i HorizontalSort8(__m256i data, unsigned char* pmin = 0, unsigned char* pmax = 0);
__m128i HorizontalSort16(__m128i data, unsigned short* pmin = 0, unsigned short* pmax = 0);
__m256i HorizontalSort16(__m256i data, unsigned short* pmin = 0, unsigned short* pmax = 0);
__m128i HorizontalSort32(__m128i data, unsigned int* pmin = 0, unsigned int* pmax = 0);
__m256i HorizontalSort32(__m256i data, unsigned int* pmin = 0, unsigned int* pmax = 0);
__m128i HorizontalSort64(__m128i data, unsigned long long* pmin = 0, unsigned long long* pmax = 0);
__m256i HorizontalSort64(__m256i data, unsigned long long* pmin = 0, unsigned long long* pmax = 0);

__m128i HorizontalSort8(__m128i data, char* pmin = 0, char* pmax = 0);
__m256i HorizontalSort8(__m256i data, char* pmin = 0, char* pmax = 0);
__m128i HorizontalSort16(__m128i data, short* pmin = 0, short* pmax = 0);
__m256i HorizontalSort16(__m256i data, short* pmin = 0, short* pmax = 0);
__m128i HorizontalSort32(__m128i data, int* pmin = 0, int* pmax = 0);
__m256i HorizontalSort32(__m256i data, int* pmin = 0, int* pmax = 0);
__m128i HorizontalSort64(__m128i data, long long* pmin = 0, long long* pmax = 0);
__m256i HorizontalSort64(__m256i data, long long* pmin = 0, long long* pmax = 0);
