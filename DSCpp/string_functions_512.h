#pragma once
size_t StringLength512(const char* s);
size_t StringLength512(const wchar_t* s);

int  StringCompare512(const char* s1, const char* s2);
int  StringCompare512(const wchar_t* s1, const wchar_t* s2);
bool StringEqual512(const char* s1, const char* s2);
bool StringEqual512(const wchar_t* s1, const wchar_t* s2);
int  StringIndexOf512(const char* s, const char c);
int  StringIndexOf512(const wchar_t* s, const wchar_t c);
int  StringIndexOf512(const char* s, const char* cs);
int  StringIndexOf512(const wchar_t* s, const wchar_t* cs);
