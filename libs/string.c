/*************************************************************************
    > File Name: libs/string.c
    > Author: Function_Dou
    > Mail: NOT
    > Created Time: 2018年10月16日 星期二 09时24分02秒
 ************************************************************************/

#include "string.h"

inline void memcpy(void *dest, const void *src, size_t n)
{
    char *tmp = (char *)dest;
    while((char *)src && n)
    {
	*tmp++ = (char *)src++;
	n--;
    }
}

inline void memset(void *s, int ch, size_t n)
{
    while((char *)s && n)
    {
	*(char *)s++ = ch + '0';
	n--;
    }
}

inline void bzero(void *s, size_t n)
{
    while((char *)s && n)
    {
	*(char *)s++ = 0;
	n--;
    }
}

inline int strcmp(const char *str1, const char *str2)
{
    for(; *str1 == *str2; )
    {
	if(*str1 == '\0')
	    return 0;
	str1++;
	str2++;
    }
    return (*str1 - *str2 ? 1 : -1);
}

inline int strncmp(const char *str1, const char *str2, size_t n)
{
    while(*str1 && *str2 && n)
    {
	if(*str1 > *str2)
	    return 1;
	else if(*str1 < *str2)
	    return -1;
	str1++, str2++, n--;
    }
    return 0;
}

inline char *strcpy(char *dest, const char *str)
{
    char *tmp = dest;
    while(*str)
	*tmp++ = *str++;
    return dest;
}


inline char *strncpy(char *dest, const char *str, size_t n)
{
    char *tmp = dest;
    while(*str && n)
    {
	*tmp++ = *str++;
	n--;
    }

    if(*tmp != '\0')
	*tmp = '\0';

    return dest;
}

inline char *strcat(char *dest, const char *str)
{
    char *tmp = dest;
    while(*tmp)
	tmp++;

    while(*str)
    {
	*tmp++ = *str++;
    }

    return dest;
}

inline size_t strlen(const char *str)
{
    size_t n = 0;
    while(*str++ != '\0')
	n++;

    return n;
}


inline int isdigit(const char c)
{
    if(c >= '0' && c <= '9')
	return 1;
    return 0;
}

inline int atoi(const char *str)
{
    int num = 0;
    while(isdigit(*str))
    {
	num = (*str++ - '0') + num * 10;
    }

    return num;
}

