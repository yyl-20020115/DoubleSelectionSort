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
