#include <intrin.h>
#include "utils.h"
size_t StringLength256(const char* s)		
{
	size_t len = 0;
	if (s != 0) {
		const int stride = sizeof(__m256i) / sizeof(*s);
		unsigned long index = 0;
		__m256i zero = _mm256_setzero_si256();
		__m256i part = { 0 };
		char* p = (char*)s;
		while (len <= (~0LL) - stride) {
			part = _mm256_loadu_epi8(p);
			__mmask32 result = _mm256_cmpeq_epi8_mask(part, zero);
			if (_BitScanForward(&index, result))
			{
				len += index;
				break;
			}
			else {
				len += stride;
				p += stride;
			}
		}
	}

	return len;
}
size_t StringLength256(const wchar_t* s)					
{
	size_t len = 0;
	if (s != 0) {
		const int stride = sizeof(__m256i) / sizeof(*s);
		unsigned long index = 0;
		__m256i zero = _mm256_setzero_si256();
		__m256i part = { 0 };
		wchar_t* p = (wchar_t*)s;
		while (len <= (~0LL) - stride) {
			part = _mm256_loadu_epi16(p);
			__mmask16 result = _mm256_cmpeq_epi16_mask(part, zero);
			if (_BitScanForward(&index, result))
			{
				len += index;
				break;
			}
			else {
				len += stride;
				p += stride;
			}
		}
	}

	return len;
}

int  StringCompare256(const char* s1, const char* s2)
{
	const int stride = sizeof(__m256i) / sizeof(*s1);

	if (s1 == 0 && s2 == 0) {
		return 0;
	}
	else if (s2 == 0) {
		return +1;
	}
	else if (s1 == 0) {
		return -1;
	}
	else {
		size_t l1 = StringLength256(s1);
		size_t l2 = StringLength256(s2);
		if (l1 == l2 && l2 == 0) return 0;
		size_t ln = l1 > l2 ? l1 : l2;

		for (unsigned long i = 0; i < ln; i += stride)
		{
			unsigned long igt = 0;
			unsigned long ilt = 0;
			unsigned long most = (unsigned long)(i + stride < ln ? stride : ln - i);
			__m256i part1 = _mm256_loadu_epi8(s1 + i);
			__m256i part2 = _mm256_loadu_epi8(s2 + i);
			__mmask32 rgt = _mm256_cmpgt_epi8_mask(part1, part2);
			__mmask32 rlt = _mm256_cmplt_epi8_mask(part1, part2);
			unsigned char bgt = _BitScanForward(&igt, rgt);
			unsigned char blt = _BitScanForward(&ilt, rlt);
			if (!bgt && !blt) continue;

			ilt = ilt > most ? most : ilt;
			igt = igt > most ? most : igt;
			if (bgt && !blt) {
				return i + igt;
			}
			else if (!bgt && blt) {
				return i + ilt;
			}
			else {
				return igt < ilt ? igt : ilt;
			}
		}
		return 0;//all the same
	}
}
int  StringCompare256(const wchar_t* s1, const wchar_t* s2)
{
	const int stride = sizeof(__m256i) / sizeof(*s1);

	if (s1 == 0 && s2 == 0) {
		return 0;
	}
	else if (s2 == 0) {
		return +1;
	}
	else if (s1 == 0) {
		return -1;
	}
	else {
		size_t l1 = StringLength256(s1);
		size_t l2 = StringLength256(s2);
		if (l1 == l2 && l2 == 0) return 0;
		size_t ln = l1 > l2 ? l1 : l2;

		for (unsigned long i = 0; i < ln; i += stride)
		{
			unsigned long igt = 0;
			unsigned long ilt = 0;
			unsigned long most = (unsigned long)(i + stride < ln ? stride : ln - i);
			__m256i part1 = _mm256_loadu_epi16(s1 + i);
			__m256i part2 = _mm256_loadu_epi16(s2 + i);
			__mmask16 rgt = _mm256_cmpgt_epi16_mask(part1, part2);
			__mmask16 rlt = _mm256_cmplt_epi16_mask(part1, part2);
			unsigned char bgt = _BitScanForward(&igt, rgt);
			unsigned char blt = _BitScanForward(&ilt, rlt);
			if (!bgt && !blt) continue;

			ilt = ilt > most ? most : ilt;
			igt = igt > most ? most : igt;
			if (bgt && !blt) {
				return i + igt;
			}
			else if (!bgt && blt) {
				return i + ilt;
			}
			else {
				return igt < ilt ? igt : ilt;
			}
		}
		return 0;//all the same
	}
}

bool StringEqual256(const char* s1, const char* s2)
{
	const int stride = sizeof(__m256i) / sizeof(*s1);

	if (s1 == 0 && s2 == 0) {
		return true;
	}
	else if (s2 == 0) {
		return false;
	}
	else if (s1 == 0) {
		return false;
	}
	else {
		size_t l1 = StringLength256(s1);
		size_t l2 = StringLength256(s2);
		if (l1 == l2 && l2 == 0) return true;
		if (l1 != l2) return false;
		size_t ln = l1;

		for (unsigned long i = 0; i < ln; i += stride)
		{
			unsigned long iet = 0;
			unsigned long most = (unsigned long)(i + stride < ln ? stride : ln - i);
			__m256i part1 = _mm256_loadu_epi8(s1 + i);
			__m256i part2 = _mm256_loadu_epi8(s2 + i);
			__mmask32 neqt = _mm256_cmpneq_epi8_mask(part1, part2);
			unsigned char bet = _BitScanForward(&iet, neqt);
			if (bet && iet < most) return false;
		}
		return true;//all the same
	}
}
bool StringEqual256(const wchar_t* s1, const wchar_t* s2)
{
	const int stride = sizeof(__m256i) / sizeof(*s1);

	if (s1 == 0 && s2 == 0) {
		return true;
	}
	else if (s2 == 0) {
		return false;
	}
	else if (s1 == 0) {
		return false;
	}
	else {
		size_t l1 = StringLength256(s1);
		size_t l2 = StringLength256(s2);
		if (l1 == l2 && l2 == 0) return true;
		if (l1 != l2) return false;
		size_t ln = l1;

		for (unsigned long i = 0; i < ln; i += stride)
		{
			unsigned long iet = 0;
			unsigned long most = (unsigned long)(i + stride < ln ? stride : ln - i);
			__m256i part1 = _mm256_loadu_epi16(s1 + i);
			__m256i part2 = _mm256_loadu_epi16(s2 + i);
			__mmask16 neqt = _mm256_cmpneq_epi16_mask(part1, part2);
			unsigned char bet = _BitScanForward(&iet, neqt);
			if (bet && iet < most) return false;
		}
		return true;//all the same
	}
}
int  StringIndexOf256(const char* s, const char c)
{
	if (s != 0) {
		const int stride = sizeof(__m256i) / sizeof(*s);
		unsigned long index = 0;
		__m256i _chs = _mm256_set1_epi8(c);
		__m256i part = { 0 };
		size_t length = StringLength256(s);

		for (int i = 0; i < length; i += stride)
		{
			part = _mm256_loadu_epi8(s + i);
			__mmask32 result = _mm256_cmpeq_epi8_mask(part, _chs);
			if (_BitScanForward(&index, result))
			{
				return i + index;
			}
		}
	}

	return -1;
}
int  StringIndexOf256(const wchar_t* s, const wchar_t c)
{
	if (s != 0) {
		const int stride = sizeof(__m256i) / sizeof(*s);
		unsigned long index = 0;
		__m256i _chs = _mm256_set1_epi16(c);
		__m256i part = { 0 };
		size_t length = StringLength256(s);

		for (int i = 0; i < length; i += stride)
		{
			part = _mm256_loadu_epi16(s + i);
			__mmask16 result = _mm256_cmpeq_epi16_mask(part, _chs);
			if (_BitScanForward(&index, result))
			{
				return i + index;
			}
		}
	}

	return -1;
}
int  StringIndexOf256(const char* s, const char* cs)
{
	if (s == 0) return -1;
	if (cs == 0) return -1;
	size_t l1 = StringLength256(s);
	size_t l2 = StringLength256(cs);
	if (l1 == 0 && l2 == 0) return 0;
	if (l1 == 0 && l2 > 0) return -1;
	if (l1 < l2) return -1;

	//TODO:find needle in hystack

	return -1;
}
int  StringIndexOf256(const wchar_t* s, const wchar_t* cs)
{
	if (s == 0) return -1;
	if (cs == 0) return -1;
	size_t l1 = StringLength256(s);
	size_t l2 = StringLength256(cs);
	if (l1 == 0 && l2 == 0) return 0;
	if (l1 == 0 && l2 > 0) return -1;
	if (l1 < l2) return -1;

	//TODO:find needle in hystack

	return -1;
}
