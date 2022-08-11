#include "horizontal_max_min_256.h"
#include "utils.h"
int HorizontalMin16(__m128i data, unsigned short* p) {
	__m128i result = _mm_minpos_epu16(data);
	if (p != 0) {
		*p = result.m128i_i16[0];
	}
	return result.m128i_i16[1] & 0x7;
}
int HorizontalMax16(__m128i data, unsigned short* p) {
	__m128i zero = _mm_setzero_si128();
	__m128i ones = _mm_cmpeq_epi32(zero, zero);
	__m128i subs = _mm_subs_epu16(ones, data);
	__m128i result = _mm_minpos_epu16(subs);
	int index = result.m128i_i16[1] & 0x7;
	if (p != 0) {
		*p = data.m128i_i16[index];
	}
	return index;
}
int HorizontalMin16(__m256i data, unsigned short* p) {
	const int stride = sizeof(data) / sizeof(*p);
	const int half = stride >>1;
	__m128i zero = _mm_setzero_si128();
	__m128i low = _mm256_extracti128_si256(data, 0);
	__m128i high = _mm256_extracti128_si256(data, 1);
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
int HorizontalMax16(__m256i data, unsigned short* p) {
	const int stride = sizeof(data) / sizeof(*p);
	const int half = stride >>1;
	__m128i zero = _mm_setzero_si128();
	__m128i low = _mm256_extracti128_si256(data, 0);
	__m128i high = _mm256_extracti128_si256(data, 1);
	unsigned short lv = 0;
	unsigned short hv = 0;
	int li = HorizontalMax16(low, &lv);
	int hi = HorizontalMax16(high, &hv);
	if (lv <= hv) {
		if (p != 0) *p = lv;
		return li;
	}
	else {
		if (p != 0) *p = hv;
		return half + hi;
	}
}
int HorizontalMin8(__m128i data, unsigned char* p) {
	const int stride = sizeof(data) / sizeof(*p);
	const int half = stride >>1;
	__m128i zero = _mm_setzero_si128();
	__m128i low = _mm_unpacklo_epi8(data, zero);
	__m128i high = _mm_unpackhi_epi8(data, zero);
	unsigned short lv = 0;
	unsigned short hv = 0;
	int li = HorizontalMin16(low, &lv);
	int hi = HorizontalMin16(high, &hv);
	if (lv <= hv) {
		if (p != 0) *p = (unsigned char)(lv & 0xff);
		return li;
	}
	else {
		if (p != 0) *p = (unsigned char)(hv & 0xff);
		return half + hi;
	}
}
int HorizontalMax8(__m128i data, unsigned char* p) {
	const int stride = sizeof(data) / sizeof(*p);
	const int half = stride >>1;
	__m128i zero = _mm_setzero_si128();
	__m128i low = _mm_unpacklo_epi8(data, zero);
	__m128i high = _mm_unpackhi_epi8(data, zero);
	unsigned short lv = 0;
	unsigned short hv = 0;
	int li = HorizontalMax16(low, &lv);
	int hi = HorizontalMax16(high, &hv);
	if (lv >= hv) {
		if (p != 0) *p = (unsigned char)(lv & 0xff);
		return li;
	}
	else {
		if (p != 0) *p = (unsigned char)(hv & 0xff);
		return half + hi;
	}
}
int HorizontalMin8(__m256i data, unsigned char* p) {
	const int stride = sizeof(data) / sizeof(*p);
	const int half = stride >>1;
	__m128i zero = _mm_setzero_si128();
	__m128i low = _mm256_extracti128_si256(data, 0);
	__m128i high = _mm256_extracti128_si256(data, 1);
	unsigned short lv = 0;
	unsigned short hv = 0;
	int li = HorizontalMin16(low, &lv);
	int hi = HorizontalMin16(high, &hv);
	if (lv <= hv) {
		if (p != 0) *p = (unsigned char)(lv & 0xff);
		return li;
	}
	else {
		if (p != 0) *p = (unsigned char)(hv & 0xff);
		return half + hi;
	}
}
int HorizontalMax8(__m256i data, unsigned char* p) {
	const int stride = sizeof(data) / sizeof(*p);
	const int half = stride >>1;
	__m128i zero = _mm_setzero_si128();
	__m128i low = _mm256_extractf128_si256(data, 0);
	__m128i high = _mm256_extractf128_si256(data, 1);
	unsigned short lv = 0;
	unsigned short hv = 0;
	int li = HorizontalMax16(low, &lv);
	int hi = HorizontalMax16(high, &hv);
	if (lv >= hv) {
		if (p != 0) *p = (unsigned char)(lv & 0xff);
		return li;
	}
	else {
		if (p != 0) *p = (unsigned char)(hv & 0xff);
		return half + hi;
	}
}
int HorizontalMin32P(__m128i data, unsigned int* p) {
	__m128i counts = _mm_setr_epi16(
		__popcnt(_mm_cmpge_epi32_mask(_mm_set1_epi32(_mm_extract_epi32(data, 0)), data)),
		__popcnt(_mm_cmpge_epi32_mask(_mm_set1_epi32(_mm_extract_epi32(data, 1)), data)),
		__popcnt(_mm_cmpge_epi32_mask(_mm_set1_epi32(_mm_extract_epi32(data, 2)), data)),
		__popcnt(_mm_cmpge_epi32_mask(_mm_set1_epi32(_mm_extract_epi32(data, 3)), data)),
		8, 8, 8, 8);
	__m128i result = _mm_minpos_epu16(counts);
	int index = result.m128i_i16[1] & 0x3;
	if (p != 0) {
		*p = data.m128i_i32[index];
	}
	return index;
}
int HorizontalMin32F(__m256i data, unsigned int* p)
{
	const int stride = sizeof(data) / sizeof(*p);
	unsigned long index = stride; //8 is out of range
	__m128i zero = _mm_setzero_si128();
	__m128i ones = _mm_cmpeq_epi32(zero, zero);
	__m256i indices = _mm256_setr_epi16(0, 2, 4, 6, 8, 10, 12, 14, 1, 3, 5, 7, 9, 11, 13, 15);
	__m256i permution = _mm256_permutexvar_epi16(indices, data);
	__m128i _low = _mm256_extracti128_si256(permution, 0);
	__m128i high = _mm256_extracti128_si256(permution, 1);

	__m128i high_result = _mm_minpos_epu16(high);

	short high_value = high_result.m128i_i16[0];
	short high_index = high_result.m128i_i16[1] & 0b00000111;
	__m128i all_high = _mm_set1_epi16(high_value);
	__mmask8 mask = _mm_cmpeq_epi16_mask(high, all_high);
	unsigned int c = __popcnt(mask);
	if (c == stride) { //all highs are same, only dif in lows
		__m128i low_result = _mm_minpos_epu16(_low);
		short low_value = low_result.m128i_i16[0];
		index = low_result.m128i_i16[1];
		if (p != 0) {
			*p = data.m256i_i32[index];
		}
		return index;
	}
	else if (c == 1) { //this high is special, no need to check lows
		if (p != 0) {
			*p = data.m256i_i32[high_index];
		}
		return high_index;
	}
	else { //multiple lows for same highs
		//try replace unrelated shorts with 0xffff
		__m128i tries = ones;
		__m128i maxlevel = _mm_mask_blend_epi16(mask, tries, _low);
		__m128i low_result = _mm_minpos_epu16(maxlevel);
		if (low_result.m128i_i16[0] == (short)0xffff) {
			//if found 0xffff, change replacement with 0
			tries = zero;
			maxlevel = _mm_mask_blend_epi16(mask, tries, _low);
			mask = _mm_cmpeq_epi16_mask(maxlevel, ones);
			//the first found non-zero bit index is the index
			//for original 0xffff value
			low_result = _mm_minpos_epu16(maxlevel);
			if (_BitScanForward(&index, mask)) {
				if (p != 0) {
					*p = data.m256i_i32[index];
				}
				return index;
			}
		}
		else {
			index = low_result.m128i_i16[1];
			if (p != 0) {
				*p = data.m256i_i32[index];
			}
			return index;
		}
	}
	return index;
}

int HorizontalMin32(__m128i data, unsigned int* p) {
	const int stride = sizeof(data) / sizeof(*p);

	int a0 = _mm_extract_epi32(data, 0);
	int a1 = _mm_extract_epi32(data, 1);
	int a2 = _mm_extract_epi32(data, 2);
	int a3 = _mm_extract_epi32(data, 3);

	int m0 = a0 <= a1 ? a0 : a1;
	int p0 = (a0 > a1);

	int m1 = a2 <= a3 ? a2 : a3;
	int p1 = (a2 > a3);

	if (p != 0)*p = (m0 <= m1) ? m0 : m1;

	return (m0 <= m1) ? p0 : (p1 + 2);
}
int HorizontalMax32(__m128i data, unsigned int* p) {
	__m128i zero = _mm_setzero_si128();
	__m128i ones = _mm_cmpeq_epi32(zero, zero);
	__m128i result = _mm_andnot_si128(data, ones);
	int index = HorizontalMin32(result, p);
	if (p != 0) {
		*p = data.m128i_u32[index];
	}
	return index;
}
//algorithm: how to find min int from __m256i with _mm_minpos_epu16
//  0. for each k in [0,7]
//  1. broadcast data.m256i_i32[k] to make 
//     (data.m256i_i32[k],data.m256i_i32[k],data.m256i_i32[k],data.m256i_i32[k],
//		data.m256i_i32[k],data.m256i_i32[k],data.m256i_i32[k],data.m256i_i32[k])
//  2. use the former template to compare with data to genrate a mask
//     with bit set to 1 indicatiing the value in data is greater than 
//     data.m256i_i32[k]
//  3. count the set bid with popcnt and make __m128i with 8 * i16 of popcnt result
//     (popcnt of (mask8) is no more than 8, therefore i16 is enough for storing)
//  4. use _mm_minpos_epu16 with counts to find the position and the value of
//     min count i32 within data which is greater or equal to data.m256i_i32[k]
//  5. the data.m256i_i32[k] with min number of greaters is the min data.m256i_i32[k]
//
int HorizontalMin32(__m256i data, unsigned int* p) {
	__m128i counts = _mm_setr_epi16(
		__popcnt(_mm256_cmpge_epi32_mask(_mm256_set1_epi32(_mm256_extract_epi32(data, 0)), data)),
		__popcnt(_mm256_cmpge_epi32_mask(_mm256_set1_epi32(_mm256_extract_epi32(data, 1)), data)),
		__popcnt(_mm256_cmpge_epi32_mask(_mm256_set1_epi32(_mm256_extract_epi32(data, 2)), data)),
		__popcnt(_mm256_cmpge_epi32_mask(_mm256_set1_epi32(_mm256_extract_epi32(data, 3)), data)),
		__popcnt(_mm256_cmpge_epi32_mask(_mm256_set1_epi32(_mm256_extract_epi32(data, 4)), data)),
		__popcnt(_mm256_cmpge_epi32_mask(_mm256_set1_epi32(_mm256_extract_epi32(data, 5)), data)),
		__popcnt(_mm256_cmpge_epi32_mask(_mm256_set1_epi32(_mm256_extract_epi32(data, 6)), data)),
		__popcnt(_mm256_cmpge_epi32_mask(_mm256_set1_epi32(_mm256_extract_epi32(data, 7)), data))
	);
	__m128i result = _mm_minpos_epu16(counts);
	int index = result.m128i_i16[1] & 0x7;
	if (p != 0) {
		*p = data.m256i_i32[index];
	}
	return index;
}
int HorizontalMax32(__m256i data, unsigned int* p) {
	__m128i zero = _mm_setzero_si128();
	__m128i ones = _mm_cmpeq_epi32(zero, zero);
	__m256i zero_ = _mm256_setzero_si256();
	__m256i ones_ = _mm256_cmpeq_epi32(zero_, zero_);
	__m256i result = _mm256_andnot_si256(data, ones_);
	int index = HorizontalMin32(result, p);
	if (p != 0) {
		*p = data.m256i_u32[index];
	}
	return index;
}
int HorizontalMin64(__m128i data, unsigned long long* p)
{
	const int stride = sizeof(data) / sizeof(*p);
	const int half = stride >>1;
	unsigned long long lv = _mm_extract_epi64(data, 0);
	unsigned long long hv = _mm_extract_epi64(data, 1);
	if (lv <= hv) {
		if (p != 0) *p = lv;
		return 0;
	}
	else {
		if (p != 0) *p = hv;
		return 1;
	}
}
int HorizontalMax64(__m128i data, unsigned long long* p)
{
	const int stride = sizeof(data) / sizeof(*p);
	const int half = stride >>1;

	unsigned long long lv = _mm_extract_epi64(data, 0);
	unsigned long long hv = _mm_extract_epi64(data, 1);
	if (lv >= hv) {
		if (p != 0) *p = lv;
		return 0;
	}
	else {
		if (p != 0) *p = hv;
		return 1;
	}
}
int HorizontalMin64(__m256i data, unsigned long long* p)
{
	const int stride = sizeof(data) / sizeof(*p);
	const int half = stride >>1;
	__m128i low = _mm256_extracti64x2_epi64(data, 0);
	__m128i high = _mm256_extracti64x2_epi64(data, 0);

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
int HorizontalMax64(__m256i data, unsigned long long* p)
{
	const int stride = sizeof(data) / sizeof(*p);
	const int half = stride >>1;
	__m128i low = _mm256_extracti64x2_epi64(data, 0);
	__m128i high = _mm256_extracti64x2_epi64(data, 0);

	unsigned long long lv = 0;
	unsigned long long hv = 0;

	int li = HorizontalMax64(low, &lv);
	int hi = HorizontalMax64(high, &hv);
	if (lv >= hv) {
		if (p != 0) *p = lv;
		return li;
	}
	else {
		if (p != 0)*p = hv;
		return half + hi;
	}

}
