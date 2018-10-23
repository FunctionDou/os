/*************************************************************************
    > File Name: include/string.h
    > Author: Function_Dou
    > Mail: NOT
    > Created Time: 2018年10月16日 星期二 09时18分05秒
 ************************************************************************/

#ifndef INCLUDE_STRING_H_
#define INCLUDE_STRING_H_

#include "types.h"
#include "debug.h"

void memcpy(void *dest, const void *src, size_t n);	/* 将src中的n个字节复制到dest字符串中 */ 

void memset(void *s, const uint8_t ch, size_t n);	/* s : 起始地址, 从起始开始有n个字符要置成 ch */

void bzero(void *s, size_t n);  /* s : 置零的起始地址, n : n个字节 */

int strcmp(const char *str1, const char *str2);

int strncmp(const char *str1, const char *str2, size_t n);

char *strcpy(char *dest, const char *str);

char *strncpy(char *dest, const char *str, size_t n);

char *strcat(char *dest, const char *str);

size_t strlen(const char *str);

int isdigit(const char c);

int atoi(const char *str);


#endif
