#include <intrin.h>
#include "utils.h"
#include <memory>
#include "common_string_functions.h"
#include "horizontal_max_min_256.h"
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
				p += index;
				break;
			}
			else {
				len += stride;
				p += stride;
			}
		}
		if (*p != 0) {
			for (p++; len++ < (~0LL); p++) {
				if (*p == 0)
					break;
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
				p += index;
				break;
			}
			else {
				len += stride;
				p += stride;
			}
		}
		if (*p != 0) {
			for (p++; len++ < (~0LL); p++) {
				if (*p == 0)
					break;
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
		unsigned long i = 0;
		for (; i < ln; i += stride)
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
		for (; i < ln; i++) {
			if (s1[i] > s2[i])
				return +1;
			else if (s1[i] < s2[i])
				return -1;
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
		unsigned long i = 0;
		for (; i < ln; i += stride)
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
		for (; i < ln; i++) {
			if (s1[i] > s2[i])
				return +1;
			else if (s1[i] < s2[i])
				return -1;
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
		unsigned long i = 0;
		for (; i < ln; i += stride)
		{
			unsigned long iet = 0;
			unsigned long most = (unsigned long)(i + stride < ln ? stride : ln - i);
			__m256i part1 = _mm256_loadu_epi8(s1 + i);
			__m256i part2 = _mm256_loadu_epi8(s2 + i);
			__mmask32 neqt = _mm256_cmpneq_epi8_mask(part1, part2);
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
		unsigned long i = 0;
		for (; i < ln; i += stride)
		{
			unsigned long iet = 0;
			unsigned long most = (unsigned long)(i + stride < ln ? stride : ln - i);
			__m256i part1 = _mm256_loadu_epi16(s1 + i);
			__m256i part2 = _mm256_loadu_epi16(s2 + i);
			__mmask16 neqt = _mm256_cmpneq_epi16_mask(part1, part2);
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
int  StringIndexOf256(const char* s, const char c)
{
	if (s != 0) {
		const int stride = sizeof(__m256i) / sizeof(*s);
		unsigned long index = 0;
		__m256i _chs = _mm256_set1_epi8(c);
		__m256i part = { 0 };
		size_t length = StringLength256(s);

		int i = 0;
		for (; i < length; i += stride)
		{
			part = _mm256_loadu_epi8(s + i);
			__mmask32 result = _mm256_cmpeq_epi8_mask(part, _chs);
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
int  StringIndexOf256(const wchar_t* s, const wchar_t c)
{
	if (s != 0) {
		const int stride = sizeof(__m256i) / sizeof(*s);
		unsigned long index = 0;
		__m256i _chs = _mm256_set1_epi16(c);
		__m256i part = { 0 };
		size_t length = StringLength256(s);
		int i = 0;
		for (; i < length; i += stride)
		{
			part = _mm256_loadu_epi16(s + i);
			__mmask16 result = _mm256_cmpeq_epi16_mask(part, _chs);
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
int  StringIndexOf256(const char* s, const char* cs)
{
	if (s == 0) return -1;
	if (cs == 0) return -1;
	size_t haystack_length = StringLength256(s);
	size_t needle_length = StringLength256(cs);
	if (haystack_length == 0 && needle_length == 0) return 0;
	if (haystack_length == 0 && needle_length > 0) return -1;
	if (haystack_length < needle_length) return -1;
	if (needle_length == 1) return StringIndexOf256(s, cs[0]);
	//at least 2 chars
	//l1>=l2
	const int stride = sizeof(__m256i) / sizeof(*s);
	//l1 and l2 should be all greater than stride
	if (needle_length < stride || haystack_length < stride) {
		return SimpleSearch(s, cs, haystack_length, needle_length);
	}
	int haystack_tail_length = haystack_length % stride;
	int needle_tail_length = needle_length % stride;


	size_t i = 0;
	size_t j = 0;

	unsigned char needle_head_min = 0;
	unsigned char haystack_part_min = 0;

	__m128i needle_head = _mm_loadu_epi8(cs + j);

	while (j <needle_length - stride && i < haystack_length - stride) {

		__m128i haystack_part = _mm_loadu_epi8(s + i);

		int haystack_part_min_index = HorizontalMin8(needle_head, &haystack_part_min);
		int needle_head_min_index = HorizontalMin8(needle_head, &needle_head_min);

		//min has higher priority
		if (needle_head_min != haystack_part_min)
		{
			//then first char
			__m128i mins = _mm_set1_epi8(cs[0]);
			__mmask8 found = _mm_cmpeq_epi8_mask(needle_head, haystack_part);
			unsigned long index = stride;
			if (_BitScanForward(&index, found)) {
				//found needle_min
				if (i + index > haystack_length)break;
				i += index;
			}
			else { 
				i += stride;
			}
			continue;
		}
	
		//min aligned 
		{
			int delta = haystack_part_min_index - needle_head_min_index;
			if (i + delta < 0) {
				i = haystack_part_min_index + 1;
				if (i> haystack_length)break;
				//do nothing
			}
			else //i+delta>=0
			{
				i += delta;
				haystack_part = _mm_loadu_epi8(s + i);
				__mmask8 m = _mm_cmpneq_epi8_mask(haystack_part, needle_head);
				unsigned long index = 0;
				if (_BitScanForward(&index, m)) 
				{
					//got first not equal
					j = 0;
					if (i + index > haystack_length)break;
					i += index;
				}
				else //all equals
				{
					if (i + stride > haystack_length)break;
					if (j + stride > needle_length)break;
					i += stride;
					j += stride;
					haystack_part = _mm_loadu_epi8(s + i);
					needle_head = _mm_loadu_epi8(cs + j);
				}
			}	
		}
		//TODO: final stage

	}

	return -1;
}
int  StringIndexOf256(const wchar_t* s, const wchar_t* cs)
{
	if (s == 0) return -1;
	if (cs == 0) return -1;
	size_t haystack_length = StringLength256(s);
	size_t needle_length = StringLength256(cs);
	if (haystack_length == 0 && needle_length == 0) return 0;
	if (haystack_length == 0 && needle_length > 0) return -1;
	if (haystack_length < needle_length) return -1;
	if (needle_length == 1) return StringIndexOf256(s, cs[0]);
	//at least 2 chars
	//l1>=l2
	const int stride = sizeof(__m256i) / sizeof(*s);
	//l1 and l2 should be all greater than stride
	if (needle_length < stride || haystack_length < stride) {
		return SimpleSearch(s, cs, haystack_length, needle_length);
	}
	int haystack_tail_length = haystack_length % stride;
	int needle_tail_length = needle_length % stride;

	size_t i = 0;
	size_t j = 0;

	unsigned short needle_head_min = 0;
	unsigned short haystack_part_min = 0;

	__m256i needle_head = _mm256_loadu_epi16(cs + j);

	while (j < needle_length - stride && i < haystack_length - stride) {

		__m256i haystack_part = _mm256_loadu_epi16(s + i);

		int haystack_part_min_index = HorizontalMin16(needle_head, &haystack_part_min);
		int needle_head_min_index = HorizontalMin16(needle_head, &needle_head_min);

		//min has higher priority
		if (needle_head_min != haystack_part_min)
		{
			//then first char
			__m256i mins = _mm256_set1_epi16(cs[0]);
			__mmask16 found = _mm256_cmpeq_epi16_mask(needle_head, haystack_part);
			unsigned long index = stride;
			if (_BitScanForward(&index, found)) {
				//found needle_min
				if (i + index > haystack_length)break;
				i += index;
			}
			else {
				i += stride;
			}
			continue;
		}

		//min aligned 
		{
			int delta = haystack_part_min_index - needle_head_min_index;
			if (i + delta < 0) {
				i = haystack_part_min_index + 1;
				if (i > haystack_length)break;
				//do nothing
			}
			else //i+delta>=0
			{
				i += delta;
				haystack_part = _mm256_loadu_epi16(s + i);
				__mmask16 m = _mm256_cmpneq_epi16_mask(haystack_part, needle_head);
				unsigned long index = 0;
				if (_BitScanForward(&index, m))
				{
					//got first not equal
					j = 0;
					if (i + index > haystack_length)break;
					i += index;
				}
				else //all equals
				{
					if (i + stride > haystack_length)break;
					if (j + stride > needle_length)break;
					i += stride;
					j += stride;
					haystack_part = _mm256_loadu_epi16(s + i);
					needle_head = _mm256_loadu_epi16(cs + j);
				}
			}
		}
		//TODO: final stage
	}

	return -1;
}
