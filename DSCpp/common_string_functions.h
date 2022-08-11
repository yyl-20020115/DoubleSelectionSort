#pragma once

int KMP(char* str, int slen, char* ptr, int plen);
int SimpleSearch(const char* s, const char* cs);
int SimpleSearch(const char* s, const char* cs, size_t haystack_length, size_t needle_length);
int SimpleSearch(const wchar_t* s, const wchar_t* cs);
int SimpleSearch(const wchar_t* s, const wchar_t* cs, size_t haystack_length, size_t needle_length);
