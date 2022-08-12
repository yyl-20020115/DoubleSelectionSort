#pragma once

int KMPSearch(char* str, char* ptr);
int KMPSearch(char* str, size_t slen, char* ptr, size_t plen);

int KMPSearch(wchar_t* str, wchar_t* ptr);
int KMPSearch(wchar_t* str, size_t slen, wchar_t* ptr, size_t plen);

int SimpleSearch(const char* s, const char* cs);
int SimpleSearch(const char* s, const char* cs, size_t haystack_length, size_t needle_length);
int SimpleSearch(const wchar_t* s, const wchar_t* cs);
int SimpleSearch(const wchar_t* s, const wchar_t* cs, size_t haystack_length, size_t needle_length);
