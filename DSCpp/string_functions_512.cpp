#include <intrin.h>
#include "string_functions_512.h"
#include "string_functions_256.h"
size_t StringLength512(const char* s, size_t maxlength)
{
	size_t len = 0;
	if (s != 0) {
		const int stride = sizeof(__m512i) / sizeof(*s);
		unsigned long index = 0;
		__m512i zero = _mm512_setzero_si512();
		char* p = (char*)s;
		while (len <= maxlength - stride) {
			__m512i part = _mm512_loadu_epi8(p);
			__mmask64 result = _mm512_cmpeq_epi8_mask(part, zero);
			if (_BitScanForward64(&index, result))
			{
				len += index;
				p += index;
				break;
			}
			else {
				len += stride;
				p += stride;
			}
		}
		if (*p != 0) {
			for (p++; len++ < maxlength; p++) {
				if (*p == 0)
					break;
			}
		}
	}

	return len;
}
size_t StringLength512(const wchar_t* s, size_t maxlength)
{
	size_t len = 0;
	if (s != 0) {
		const int stride = sizeof(__m512i) / sizeof(*s);
		unsigned long index = 0;
		__m512i zero = _mm512_setzero_si512();
		wchar_t* p = (wchar_t*)s;
		while (len <= maxlength - stride) {
			__m512i part = _mm512_loadu_epi16(p);
			__mmask32 result = _mm512_cmpeq_epi16_mask(part, zero);
			if (_BitScanForward(&index, result))
			{
				len += index;
				p += index;
				break;
			}
			else {
				len += stride;
				p += stride;
			}
		}
		if (*p != 0) {
			for (p++; len++ < maxlength; p++) {
				if (*p == 0)
					break;
			}
		}
	}

	return len;
}
int StringCompare512(const char* s1, const char* s2, size_t maxlength)
{
	const int stride = sizeof(__m512i) / sizeof(*s1);

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
		size_t l1 = StringLength512(s1, maxlength);
		size_t l2 = StringLength512(s2, maxlength);
		if (l1 == l2 && l2 == 0) return 0;
		size_t ln = l1 <= l2 ? l1 : l2;
		unsigned long i = 0;
		for (; i < ln; i += stride)
		{
			unsigned long igt = 0;
			unsigned long ilt = 0;
			unsigned long most = (unsigned long)(i + stride < ln ? stride : ln - i);
			__m512i part1 = _mm512_loadu_epi8(s1 + i);
			__m512i part2 = _mm512_loadu_epi8(s2 + i);
			__mmask64 rgt = _mm512_cmpgt_epi8_mask(part1, part2);
			__mmask64 rlt = _mm512_cmplt_epi8_mask(part1, part2);
			unsigned char bgt = _BitScanForward64(&igt, rgt);
			unsigned char blt = _BitScanForward64(&ilt, rlt);
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
		for (; i < ln; i++) {
			if (s1[i] > s2[i])
				return +1;
			else if (s1[i] < s2[i])
				return -1;
		}
		return 0;//all the same
	}
}
int StringCompare512(const wchar_t* s1, const wchar_t* s2, size_t maxlength)
{
	const int stride = sizeof(__m512i) / sizeof(*s1);

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
		size_t l1 = StringLength512(s1, maxlength);
		size_t l2 = StringLength512(s2, maxlength);
		if (l1 == l2 && l2 == 0) return 0;
		size_t ln = l1 <= l2 ? l1 : l2;
		unsigned long i = 0;
		for (; i < ln; i += stride)
		{
			unsigned long igt = 0;
			unsigned long ilt = 0;
			unsigned long most = (unsigned long)(i + stride < ln ? stride : ln - i);
			__m512i part1 = _mm512_loadu_epi16(s1 + i);
			__m512i part2 = _mm512_loadu_epi16(s2 + i);
			__mmask32 rgt = _mm512_cmpgt_epi16_mask(part1, part2);
			__mmask32 rlt = _mm512_cmplt_epi16_mask(part1, part2);
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
		for (; i < ln; i++) {
			if (s1[i] > s2[i])
				return +1;
			else if (s1[i] < s2[i])
				return -1;
		}

		return 0;//all the same
	}
}
bool StringEqual512(const char* s1, const char* s2, size_t maxlength)
{
	const int stride = sizeof(__m512i) / sizeof(*s1);

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
		size_t l1 = StringLength512(s1, maxlength);
		size_t l2 = StringLength512(s2, maxlength);
		if (l1 == l2 && l2 == 0) return true;
		if (l1 != l2) return false;
		size_t ln = l1;
		unsigned long i = 0;
		for (; i < ln; i += stride)
		{
			unsigned long iet = 0;
			unsigned long most = (unsigned long)(i + stride < ln ? stride : ln - i);
			__m512i part1 = _mm512_loadu_epi8(s1 + i);
			__m512i part2 = _mm512_loadu_epi8(s2 + i);
			__mmask64 neqt = _mm512_cmpneq_epi8_mask(part1, part2);
			unsigned char bet = _BitScanForward64(&iet, neqt);
			if (bet && iet < most) return false;
		}
		for (; i < ln; i++) {
			if (s1[i] != s2[i])
				return false;
		}

		return true;//all the same
	}
}
bool StringEqual512(const wchar_t* s1, const wchar_t* s2, size_t maxlength)
{
	const int stride = sizeof(__m512i) / sizeof(*s1);

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
		size_t l1 = StringLength512(s1, maxlength);
		size_t l2 = StringLength512(s2, maxlength);
		if (l1 == l2 && l2 == 0) return true;
		if (l1 != l2) return false;
		size_t ln = l1;
		unsigned long i = 0;
		for (; i < ln; i += stride)
		{
			unsigned long iet = 0;
			unsigned long most = (unsigned long)(i + stride < ln ? stride : ln - i);
			__m512i part1 = _mm512_loadu_epi16(s1 + i);
			__m512i part2 = _mm512_loadu_epi16(s2 + i);
			__mmask32 neqt = _mm512_cmpneq_epi16_mask(part1, part2);
			unsigned char bet = _BitScanForward(&iet, neqt);
			if (bet && iet < most) return false;
		}
		for (; i < ln; i++) {
			if (s1[i] != s2[i])
				return false;
		}
		return true;//all the same
	}
}
int StringIndexOf512(const char* s, const char c, size_t maxlength)
{
	if (s != 0) {
		const int stride = sizeof(__m512i) / sizeof(*s);
		unsigned long index = 0;
		__m512i _chs = _mm512_set1_epi8(c);
		__m512i part = { 0 };
		size_t length = StringLength512(s, maxlength);
		int i = 0;
		for (; i < length; i += stride)
		{
			part = _mm512_loadu_epi8(s + i);
			__mmask64 result = _mm512_cmpeq_epi8_mask(part, _chs);
			if (_BitScanForward64(&index, result))
			{
				return i + index;
			}
		}
		for (; i < length; i++) {
			if (s[i] == c)
				return i;
		}
	}

	return -1;
}
int StringIndexOf512(const wchar_t* s, const wchar_t c, size_t maxlength)
{
	if (s != 0) {
		const int stride = sizeof(__m512i) / sizeof(*s);
		unsigned long index = 0;
		__m512i _chs = _mm512_set1_epi16(c);
		__m512i part = { 0 };
		size_t length = StringLength512(s, maxlength);
		int i = 0;
		for (; i < length; i += stride)
		{
			part = _mm512_loadu_epi16(s + i);
			__mmask32 result = _mm512_cmpeq_epi16_mask(part, _chs);
			if (_BitScanForward(&index, result))
			{
				return i + index;
			}
		}
		for (; i < length; i++) {
			if (s[i] == c)
				return i;
		}
	}

	return -1;
}
int StringIndexOf512(const char* s, const char* cs, size_t maxlength)
{
	return StringIndexOf256(s,cs, maxlength);
}
int StringIndexOf512(const wchar_t* s, const wchar_t* cs, size_t maxlength)
{
	return StringIndexOf256(s, cs, maxlength);
}

