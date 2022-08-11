#include "horizontal_max_min_512.h"
#include "horizontal_max_min_256.h"
#include "utils.h"
int HorizontalMin16(__m512i data, unsigned short* p) {
	const int stride = sizeof(data) / sizeof(*p);
	const int half = stride >> 1;
	__m256i low = _mm512_extracti64x4_epi64(data, 0);
	__m256i high = _mm512_extracti64x4_epi64(data, 1);
	unsigned short lv = 0;
	unsigned short hv = 0;
	int li = HorizontalMin16(low, &lv);
	int hi = HorizontalMin16(high, &hv);
	if (lv <= hv) {
		if (p != 0) *p = lv;
		return li;
	}
	else {
		if (p != 0) *p = hv;
		return half + hi;
	}
}
int HorizontalMax16(__m512i data, unsigned short* p) {
	const int stride = sizeof(data) / sizeof(*p);
	const int half = stride >> 1;
	__m256i low = _mm512_extracti64x4_epi64(data, 0);
	__m256i high = _mm512_extracti64x4_epi64(data, 1);
	unsigned short lv = 0;
	unsigned short hv = 0;
	int li = HorizontalMax16(low, &lv);
	int hi = HorizontalMax16(high, &hv);
	if (lv >= hv) {
		if (p != 0) *p = lv;
		return li;
	}
	else {
		if (p != 0) *p = hv;
		return half + hi;
	}
}
int HorizontalMin8(__m512i data, unsigned char* p) {
	const int stride = sizeof(data) / sizeof(*p);
	const int half = stride >> 1;
	__m256i low = _mm512_extracti64x4_epi64(data, 0);
	__m256i high = _mm512_extracti64x4_epi64(data, 1);
	unsigned char lv = 0;
	unsigned char hv = 0;
	int li = HorizontalMin8(low, &lv);
	int hi = HorizontalMin8(high, &hv);
	if (lv <= hv) {
		if (p != 0) *p = lv;
		return li;
	}
	else {
		if (p != 0) *p = hv;
		return half + hi;
	}
}
int HorizontalMax8(__m512i data, unsigned char* p) {
	const int stride = sizeof(data) / sizeof(*p);
	const int half = stride >> 1;
	__m256i low = _mm512_extracti64x4_epi64(data, 0);
	__m256i high = _mm512_extracti64x4_epi64(data, 1);
	unsigned char lv = 0;
	unsigned char hv = 0;
	int li = HorizontalMax8(low, &lv);
	int hi = HorizontalMax8(high, &hv);
	if (lv >= hv) {
		if (p != 0) *p = lv;
		return li;
	}
	else {
		if (p != 0) *p = hv;
		return half + hi;
	}
}
int HorizontalMin32(__m512i data, unsigned int* p)
{
	const int stride = sizeof(data) / sizeof(*p);
	const int half = stride >> 1;
	__m256i low = _mm512_extracti32x8_epi32(data, 0);
	__m256i high = _mm512_extracti32x8_epi32(data, 1);

	unsigned int lv = 0;
	unsigned int hv = 0;

	int li = HorizontalMin32(low, &lv);
	int hi = HorizontalMin32(high, &hv);
	if (lv <= hv) {
		if (p != 0)*p = lv;
		return li;
	}
	else //hv<lv
	{
		if (p != 0)*p = hv;
		return half + hi;
	}
}
int HorizontalMax32(__m512i data, unsigned int* p)
{
	const int stride = sizeof(data) / sizeof(*p);
	const int half = stride >> 1;
	__m256i _low = _mm512_extracti32x8_epi32(data, 0);
	__m256i high = _mm512_extracti32x8_epi32(data, 1);

	unsigned int lv = 0;
	unsigned int hv = 0;

	int li = HorizontalMax32(_low, &lv);
	int hi = HorizontalMax32(high, &hv);
	if (lv >= hv) {
		if (p != 0)*p = lv;
		return li;
	}
	else
	{
		if (p != 0)*p = hv;
		return half + hi;
	}
}
int HorizontalMin64(__m512i data, unsigned long long* p)
{
	const int stride = sizeof(data) / sizeof(*p);
	const int half = stride >> 1;
	__m256i low = _mm512_extracti64x4_epi64(data, 0);
	__m256i high = _mm512_extracti64x4_epi64(data, 1);

	unsigned long long lv = 0;
	unsigned long long hv = 0;

	int li = HorizontalMin64(low, &lv);
	int hi = HorizontalMin64(high, &hv);
	if (lv <= hv) {
		if (p != 0) *p = lv;
		return li;
	}
	else {
		if (p != 0)*p = hv;
		return half + hi;
	}
}
int HorizontalMax64(__m512i data, unsigned long long* p)
{
	const int stride = sizeof(data) / sizeof(*p);
	const int half = stride >> 1;
	__m256i low = _mm512_extracti64x4_epi64(data, 0);
	__m256i high = _mm512_extracti64x4_epi64(data, 1);

	unsigned long long lv = 0;
	unsigned long long hv = 0;

	int li = HorizontalMin64(low, &lv);
	int hi = HorizontalMin64(high, &hv);
	if (lv >= hv) {
		if (p != 0) *p = lv;
		return li;
	}
	else {
		if (p != 0)*p = hv;
		return half + hi;
	}
}