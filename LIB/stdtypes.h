#ifndef _STDTYPES_H
#define _STDTYPES_H

#if HOCKEY /* does this need to be here?  - BER */
#include <config.h>
#endif

#ifndef mbx
#define mbx extern
#endif

/*
**	Data types
*/
#ifndef __VS08_TYPE_DEFINED
#define __VS08_TYPE_DEFINED
typedef volatile signed char VS08;
#endif /* __VS08_TYPE_DEFINED */
#ifndef __VU08_TYPE_DEFINED
#define __VU08_TYPE_DEFINED
typedef volatile unsigned char VU08;
#endif /* __VU08_TYPE_DEFINED */
#ifndef __U08_TYPE_DEFINED
#define __U08_TYPE_DEFINED
typedef unsigned char U08;
#endif /* __U08_TYPE_DEFINED */
#ifndef __S08_TYPE_DEFINED
#define __S08_TYPE_DEFINED
typedef signed char S08;
#endif /* __S08_TYPE_DEFINED */
#ifndef __VS32_TYPE_DEFINED
#define __VS32_TYPE_DEFINED
typedef volatile long VS32;
#endif /* __VS32_TYPE_DEFINED */
#ifndef __VS16_TYPE_DEFINED
#define __VS16_TYPE_DEFINED
typedef volatile short VS16;
#endif /* __VS16_TYPE_DEFINED */
#ifndef __VS8_TYPE_DEFINED
#define __VS8_TYPE_DEFINED
typedef volatile signed char VS8;
#endif /* __VS8_TYPE_DEFINED */
#ifndef __VU32_TYPE_DEFINED
#define __VU32_TYPE_DEFINED
typedef volatile unsigned long VU32;
#endif /* __VU32_TYPE_DEFINED */
#ifndef __VU16_TYPE_DEFINED
#define __VU16_TYPE_DEFINED
typedef volatile unsigned short VU16;
#endif /* __VU16_TYPE_DEFINED */
#ifndef __VU8_TYPE_DEFINED
#define __VU8_TYPE_DEFINED
typedef volatile unsigned char VU8;
#endif /* __VU8_TYPE_DEFINED */
#ifndef __VF32_TYPE_DEFINED
#define __VF32_TYPE_DEFINED
typedef volatile float VF32;
#endif /* __VF32_TYPE_DEFINED */
#ifndef __VF64_TYPE_DEFINED
#define __VF64_TYPE_DEFINED
typedef volatile double VF64;
#endif /* __VF64_TYPE_DEFINED */
#ifndef __m_int_TYPE_DEFINED
#define __m_int_TYPE_DEFINED
typedef int m_int;
#endif /* __m_int_TYPE_DEFINED */
#ifndef __m_uint_TYPE_DEFINED
#define __m_uint_TYPE_DEFINED
typedef unsigned int m_uint;
#endif /* __m_uint_TYPE_DEFINED */
#ifndef __U8_TYPE_DEFINED
#define __U8_TYPE_DEFINED
typedef unsigned char U8;
#endif /* __U8_TYPE_DEFINED */
#ifndef __S8_TYPE_DEFINED
#define __S8_TYPE_DEFINED
typedef signed char S8;
#endif /* __S8_TYPE_DEFINED */
#ifndef __U16_TYPE_DEFINED
#define __U16_TYPE_DEFINED
typedef unsigned short U16;
#endif /* __U16_TYPE_DEFINED */
#ifndef __S16_TYPE_DEFINED
#define __S16_TYPE_DEFINED
typedef short S16;
#endif /* __S16_TYPE_DEFINED */
#ifndef __U32_TYPE_DEFINED
#define __U32_TYPE_DEFINED
typedef unsigned long U32;
#endif /* __U32_TYPE_DEFINED */
#ifndef __S32_TYPE_DEFINED
#define __S32_TYPE_DEFINED
typedef long S32;
#endif /* __S32_TYPE_DEFINED */
#ifndef __F32_TYPE_DEFINED
#define __F32_TYPE_DEFINED
typedef float F32;
#endif /* __F32_TYPE_DEFINED */
#ifndef __F64_TYPE_DEFINED
#define __F64_TYPE_DEFINED
typedef double F64;
#endif /* __F64_TYPE_DEFINED */

/* RUSH - added for antique programmers (petrick) */
#ifndef __BOOL_TYPE_DEFINED
#define __BOOL_TYPE_DEFINED
typedef unsigned char		BOOL;
#endif /* __BOOL_TYPE_DEFINED */

typedef long MBFLT;
typedef long ANG32;
typedef struct {
	S32 x,y,z;
} S32XYZ;

#ifndef NULL
#define NULL ((void *)0)
#endif

#define xcase break;case
#define ARRAY_SIZE(n) (sizeof(n) / sizeof((n)[0]))

/* Colors */
#ifndef MAG_PAL
#define MAG_PAL VIO_PAL
#define MAG_PALB VIO_PALB
#endif

#if HOCKEY
#undef AN_BIG_SET
#define AN_BIG_SET 0 /* does this need to be here?  - BER */
#endif

#ifndef RUN_ON_UNIX
#include "strings.h" /* does this need to be here?  - BER */
#endif

#endif

