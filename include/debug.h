/*************************************************************************
    > File Name: debug.h
    > Author: Function_Dou
    > Mail: NOT
    > Created Time: 2018年10月17日 星期三 10时28分29秒
 ************************************************************************/

#ifndef INCLUDE_DEBUG_H_
#define INCLUDE_DEBUG_H_

#include "types.h"
#include "console.h"
#include "string.h"
#include "vargs.h"
int printk(char *fmt, ...);

int vprintk(char *buf, char *fmt, va_list varg);

#endif
