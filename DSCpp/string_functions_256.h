#pragma once
size_t StringLength256(const char* s);
size_t StringLength256(const wchar_t* s);

int  StringCompare256(const char* s1, const char* s2);
int  StringCompare256(const wchar_t* s1, const wchar_t* s2);
bool StringEqual256(const char* s1, const char* s2);
bool StringEqual256(const wchar_t* s1, const wchar_t* s2);
int  StringIndexOf256(const char* s, const char c);
int  StringIndexOf256(const wchar_t* s, const wchar_t c);
int  StringIndexOf256(const char* s, const char* cs);
int  StringIndexOf256(const wchar_t* s, const wchar_t* cs);
