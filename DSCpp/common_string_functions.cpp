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
	next[0] = -1;//next[0]��ʼ��Ϊ-1��-1��ʾ��������ͬ�����ǰ׺������׺
	int k = -1;//k��ʼ��Ϊ-1
	for (size_t q = 1; q <= len - 1; q++)
	{
		while (k > -1 && str[k + 1] != str[q])//�����һ����ͬ����ôk�ͱ��next[k]��ע��next[k]��С��k�ģ�����kȡ�κ�ֵ��
		{
			k = next[k];//��ǰ����
		}
		if (str[k + 1] == str[q])//�����ͬ��k++
		{
			k = k + 1;
		}
		next[q] = k;//����ǰ����k��ֵ��������ͬ�����ǰ׺������׺��������next[q]
	}
}
int KMPSearch(char* str, size_t slen, char* ptr, size_t plen)
{
	int* next = new int[plen];
	KMPBuildNext(ptr, next, plen);//����next����
	int k = -1;
	for (size_t i = 0; i < slen; i++)
	{
		while (k > -1 && ptr[k + 1] != str[i])//ptr��str��ƥ�䣬��k>-1����ʾptr��str�в���ƥ�䣩
			k = next[k];//��ǰ����
		if (ptr[k + 1] == str[i])
			k = k + 1;
		if (k == plen - 1)//˵��k�ƶ���ptr����ĩ��
		{
			//cout << "��λ��" << i-plen+1<< endl;
			//k = -1;//���³�ʼ����Ѱ����һ��
			//i = i - plen + 1;//i��λ����λ�ã����forѭ��i++���Լ�������һ��������Ĭ�ϴ�������ƥ���ַ������Բ����ص�������л������ͬѧָ������
			return (int)(i - plen + 1);//������Ӧ��λ��
		}
	}
	return -1;
}
int KMPSearch(char* str, char* ptr) {
	return KMPSearch(str, strlen(str), ptr, strlen(ptr));
}

void KMPBuildNext(wchar_t* str, int* next, size_t len)
{
	next[0] = -1;//next[0]��ʼ��Ϊ-1��-1��ʾ��������ͬ�����ǰ׺������׺
	int k = -1;//k��ʼ��Ϊ-1
	for (size_t q = 1; q <= len - 1; q++)
	{
		while (k > -1 && str[k + 1] != str[q])//�����һ����ͬ����ôk�ͱ��next[k]��ע��next[k]��С��k�ģ�����kȡ�κ�ֵ��
		{
			k = next[k];//��ǰ����
		}
		if (str[k + 1] == str[q])//�����ͬ��k++
		{
			k = k + 1;
		}
		next[q] = k;//����ǰ����k��ֵ��������ͬ�����ǰ׺������׺��������next[q]
	}
}
int KMPSearch(wchar_t* str, size_t slen, wchar_t* ptr, size_t plen)
{
	int* next = new int[plen];
	KMPBuildNext(ptr, next, plen);//����next����
	int k = -1;
	for (size_t i = 0; i < slen; i++)
	{
		while (k > -1 && ptr[k + 1] != str[i])//ptr��str��ƥ�䣬��k>-1����ʾptr��str�в���ƥ�䣩
			k = next[k];//��ǰ����
		if (ptr[k + 1] == str[i])
			k = k + 1;
		if (k == plen - 1)//˵��k�ƶ���ptr����ĩ��
		{
			//cout << "��λ��" << i-plen+1<< endl;
			//k = -1;//���³�ʼ����Ѱ����һ��
			//i = i - plen + 1;//i��λ����λ�ã����forѭ��i++���Լ�������һ��������Ĭ�ϴ�������ƥ���ַ������Բ����ص�������л������ͬѧָ������
			return (int)(i - plen + 1);//������Ӧ��λ��
		}
	}
	return -1;
}
int KMPSearch(wchar_t* str, wchar_t* ptr) {
	return KMPSearch(str, wcslen(str), ptr, wcslen(ptr));
}
