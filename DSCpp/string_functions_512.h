#pragma once
size_t StringLength512(const char* s, size_t maxlength = ~0);
size_t StringLength512(const wchar_t* s, size_t maxlength = ~0);

int  StringCompare512(const char* s1, const char* s2, size_t maxlength = ~0);
int  StringCompare512(const wchar_t* s1, const wchar_t* s2, size_t maxlength = ~0);
bool StringEqual512(const char* s1, const char* s2, size_t maxlength = ~0);
bool StringEqual512(const wchar_t* s1, const wchar_t* s2, size_t maxlength = ~0);
int  StringIndexOf512(const char* s, const char c, size_t maxlength = ~0);
int  StringIndexOf512(const wchar_t* s, const wchar_t c, size_t maxlength = ~0);
int  StringIndexOf512(const char* s, const char* cs, size_t maxlength = ~0);
int  StringIndexOf512(const wchar_t* s, const wchar_t* cs, size_t maxlength = ~0);
