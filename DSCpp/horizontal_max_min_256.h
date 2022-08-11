#pragma once
#include <intrin.h>

int HorizontalMin16(__m128i data, unsigned short* p = 0);
int HorizontalMax16(__m128i data, unsigned short* p = 0);
int HorizontalMin16(__m256i data, unsigned short* p = 0);
int HorizontalMax16(__m256i data, unsigned short* p = 0);
int HorizontalMin8(__m128i data, unsigned char* p = 0);
int HorizontalMax8(__m128i data, unsigned char* p = 0);
int HorizontalMin8(__m256i data, unsigned char* p = 0);
int HorizontalMax8(__m256i data, unsigned char* p = 0);
int HorizontalMin32(__m128i data, unsigned int* p = 0);
int HorizontalMax32(__m128i data, unsigned int* p = 0);
int HorizontalMin32(__m256i data, unsigned int* p = 0);
int HorizontalMax32(__m256i data, unsigned int* p = 0);
int HorizontalMin64(__m128i data, unsigned long long* p = 0);
int HorizontalMax64(__m128i data, unsigned long long* p = 0);
int HorizontalMin64(__m256i data, unsigned long long* p = 0);
int HorizontalMax64(__m256i data, unsigned long long* p = 0);

