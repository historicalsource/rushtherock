/**************************************************************************

  Copyright 1994,1995 Time Warner Interactive.  Unauthorized
  reproduction, adaptation, distribution, performance or display
  of this computer program or the associated audiovisual work is
  strictly prohibited.
**************************************************************************/

/*	mathsubs.c replaces the 68K asm routines in asmsubs.asm with ANSI C routines */

#include "config.h"

S16 MUL(S16 a, S16 b) /* 0x4000 * 0x4000 = 0x4000 */
{
	S16 i;
	i = (S16) (((S32)(a * b))>>14);
	return(i);
}

S16 MULSUM(S16 a, S16 b, S16 c, S16 d) /* returns ((a*b)+(c*d)) >> 14 */
{
	S16 i;
	i = (S16) (((S32)(a * b) + (S32)(c * d)) >> 14);
	return(i);
}

S32 MUL32(S16 a, S32 b) /* returns 32-bit result of (a * b) */	
{
	float i;
	i = (float)a/16384;		/* -1 <= i <= 1 */
	return((S32)(i * b));
}

S32 MULUV32(S32 uva, S32 uvb)
{
    float result;
    
    /* normalize the result to 0x40000000 (shift right 30 places) */
    result = ((float)uva * (float)uvb) / 1.074E+9;
    return((S32)(result));
}

long DIV32(S32 a, S32 b)
{
	return((long)(a/b));
}

S32 INTRP32(S32 a, S32 b, S32 c)
{
    float result;

    if (c == 0) return(0);    
    result = ((float)a * (float)b) / (float)c;
    return((S32)result);
}

unsigned int L_DIV_S(unsigned int A, unsigned short B)
{
	return((unsigned int)(A / B));
}

void COPYUV(const S16 suv[3][3], S16 duv[3][3])
{
	int i, j;
	for (i=0;i<3;i++)
		for (j=0;j<3;j++)
			duv[i][j] = suv[i][j];
}

void COPYUV32(S32 suv[3][3], S32 duv[3][3])
{
	int i, j;
	for (i=0;i<3;i++)
		for (j=0;j<3;j++)
			duv[i][j] = suv[i][j];
}

void intswap(int *ip)
{
    U32 hi, low;
    
    hi = (*ip & 0xFFFF0000)>>16;	
    low = (*ip& 0x0000FFFF)<<16;
    *ip = hi | low;
}

/* Other obsolete ASM routines */

/* copies nbytes from *source to *dest */
void MOVMEM (char *source, char *dest, int nbytes)
{
	int i;
	for (i=0;i<nbytes;i++)
		dest[i] = source[i];
}

