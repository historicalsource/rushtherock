#if !defined(_NSPRINTF_H_)
#define _NSPRINTF_H_

#include <stdarg.h>
extern int nsprintf(char *, int, const char *, ...);
extern int nisprintf(char *, int, const char *, ...);
int nvfprintf(char *fp, int maxlen, const char *fmt0, va_list ap);
int nivfprintf(char *fp, int maxlen, const char *fmt0, va_list ap);

#ifndef TWI_VFPRINTF_TYPE
# define TWI_VFPRINTF_TYPE char
#endif

int twi_vfprintf(int (*ortn)(void *, const char *, int), TWI_VFPRINTF_TYPE *fp, int maxlen, const char *fmt0, va_list ap);
int twi_vifprintf(int (*ortn)(void *, const char *, int), TWI_VFPRINTF_TYPE *fp, int maxlen, const char *fmt0, va_list ap);

#endif
