#include "common_string_functions.h"
#include <cstring>
#include <memory>

int SimpleSearch(const char* s, const char* cs)
{
	size_t haystack_length = strlen(s);
	size_t needle_length = strlen(cs);
	return SimpleSearch(s, cs, haystack_length, needle_length);
}
int SimpleSearch(const char* s, const char* cs, size_t haystack_length,size_t needle_length) {
	size_t i = 0;
	size_t j = 0;
	while (i < haystack_length && j < needle_length)
	{
		if (s[i] == cs[j])
		{
			i++;
			j++;
		}
		else
		{
			i = i - j + 1;
			j = 0;
		}
	}

	if (j >= needle_length)
	{
		return (int)(i - j);
	}
	else
	{
		return -1;
	}
	return 0;

}
int SimpleSearch(const wchar_t* s, const wchar_t* cs)
{
	size_t haystack_length = wcslen(s);
	size_t needle_length = wcslen(cs);
	return SimpleSearch(s, cs, haystack_length, needle_length);
}
int SimpleSearch(const wchar_t* s, const wchar_t* cs, size_t haystack_length, size_t needle_length) {
	size_t i = 0;
	size_t j = 0;
	while (i < haystack_length && j < needle_length)
	{
		if (s[i] == cs[j])
		{
			i++;
			j++;
		}
		else
		{
			i = i - j + 1;
			j = 0;
		}
	}

	if (j >= needle_length)
	{
		return (int)(i - j);
	}
	else
	{
		return -1;
	}
	return 0;

}

void KMPBuildNext(char* str, int* next, size_t len)
{
	next[0] = -1;//next[0]初始化为-1，-1表示不存在相同的最大前缀和最大后缀
	int k = -1;//k初始化为-1
	for (size_t q = 1; q <= len - 1; q++)
	{
		while (k > -1 && str[k + 1] != str[q])//如果下一个不同，那么k就变成next[k]，注意next[k]是小于k的，无论k取任何值。
		{
			k = next[k];//往前回溯
		}
		if (str[k + 1] == str[q])//如果相同，k++
		{
			k = k + 1;
		}
		next[q] = k;//这个是把算的k的值（就是相同的最大前缀和最大后缀长）赋给next[q]
	}
}
int KMPSearch(char* str, size_t slen, char* ptr, size_t plen)
{
	int* next = new int[plen];
	KMPBuildNext(ptr, next, plen);//计算next数组
	int k = -1;
	for (size_t i = 0; i < slen; i++)
	{
		while (k > -1 && ptr[k + 1] != str[i])//ptr和str不匹配，且k>-1（表示ptr和str有部分匹配）
			k = next[k];//往前回溯
		if (ptr[k + 1] == str[i])
			k = k + 1;
		if (k == plen - 1)//说明k移动到ptr的最末端
		{
			//cout << "在位置" << i-plen+1<< endl;
			//k = -1;//重新初始化，寻找下一个
			//i = i - plen + 1;//i定位到该位置，外层for循环i++可以继续找下一个（这里默认存在两个匹配字符串可以部分重叠），感谢评论中同学指出错误。
			return (int)(i - plen + 1);//返回相应的位置
		}
	}
	return -1;
}
int KMPSearch(char* str, char* ptr) {
	return KMPSearch(str, strlen(str), ptr, strlen(ptr));
}

void KMPBuildNext(wchar_t* str, int* next, size_t len)
{
	next[0] = -1;//next[0]初始化为-1，-1表示不存在相同的最大前缀和最大后缀
	int k = -1;//k初始化为-1
	for (size_t q = 1; q <= len - 1; q++)
	{
		while (k > -1 && str[k + 1] != str[q])//如果下一个不同，那么k就变成next[k]，注意next[k]是小于k的，无论k取任何值。
		{
			k = next[k];//往前回溯
		}
		if (str[k + 1] == str[q])//如果相同，k++
		{
			k = k + 1;
		}
		next[q] = k;//这个是把算的k的值（就是相同的最大前缀和最大后缀长）赋给next[q]
	}
}
int KMPSearch(wchar_t* str, size_t slen, wchar_t* ptr, size_t plen)
{
	int* next = new int[plen];
	KMPBuildNext(ptr, next, plen);//计算next数组
	int k = -1;
	for (size_t i = 0; i < slen; i++)
	{
		while (k > -1 && ptr[k + 1] != str[i])//ptr和str不匹配，且k>-1（表示ptr和str有部分匹配）
			k = next[k];//往前回溯
		if (ptr[k + 1] == str[i])
			k = k + 1;
		if (k == plen - 1)//说明k移动到ptr的最末端
		{
			//cout << "在位置" << i-plen+1<< endl;
			//k = -1;//重新初始化，寻找下一个
			//i = i - plen + 1;//i定位到该位置，外层for循环i++可以继续找下一个（这里默认存在两个匹配字符串可以部分重叠），感谢评论中同学指出错误。
			return (int)(i - plen + 1);//返回相应的位置
		}
	}
	return -1;
}
int KMPSearch(wchar_t* str, wchar_t* ptr) {
	return KMPSearch(str, wcslen(str), ptr, wcslen(ptr));
}
