/*************************************************************************
    > File Name: include/vargs.h
    > Author: Function_Dou
    > Mail: NOT
    > Created Time: 2018年10月16日 星期二 17时14分36秒
 ************************************************************************/

#ifndef INCLUDE_VARGS_H_
#define INCLUDE_VARGS_H_ 

#include "types.h"

/* 
typedef __builtin_va_list va_list;

#define va_start(ap, last)	(__builtin_va_list(ap, last))
#define va_arg(ap, type) (__builtin_va_list(ap, type))
#define va_end(ap)
*/ 


#define _INSIZEOF(v) ((sizeof(v) + sizeof(int) - 1) & ~(sizeof(int) - 1))

typedef char * va_list;
#define va_start(ap, v) (ap = (va_list)&v + _INSIZEOF(v))
#define va_arg(ap, type) (*(type *)((ap += _INSIZEOF(type)) - _INSIZEOF(type)))
#define va_end(ap) (ap = (va_list)0)

#endif
