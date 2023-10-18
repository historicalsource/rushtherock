#ifndef _STRINGS_H
#define _STRINGS_H

#include <stdtypes.h>
#include <stdarg.h>
#include <nsprintf.h>

#include "Pro/strings.pro"

extern int nvfprintf(char *fp, int maxlen, const char *fmt0, va_list ap);
extern S32 ClearScreen;

#if 0
#define DBG vxyprintf(0,-1,0,"%s:%d",__FILE__,__LINE__);
#else
#define DBG
#endif

#endif
