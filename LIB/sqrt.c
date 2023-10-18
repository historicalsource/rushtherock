/* asincos.c hacked from gcc mathlib sqrt.c */

#include "stdtypes.h"
#include "fmath.h"
#include "mathlib.h"
#include "error.h"
#if 0
F32 fsqrt(F32 x)
{
	return sqrtf(x);
}

#else
#include "sqrttab.h"

F32 fsqrt(F32 x)
{
	U32 ieee;
	S32 e;
	U32 f;
	F32 sr;
	F32 r;
	
	if (x < 0.0) 
    {
		Errorf(MATHERROR,"sqrt bad input: %f",(F32)x);
		return 0;
    }
	if (x == 0.0)
		return 0.0;

	r = x;
	ieee = *(U32 *)&r;
	e = ((ieee & 0x7F800000)>>23) - 127;
	f = ieee & 0x7FFFFF;

	if (e == -127)
		return 0.0;	/* This uses a slightly different format and is so small... */
	
	sr = fsqrt_table[f>>(23-FSQRT_BITS)];
	if (e & 1)
		sr *= 1.414213562;	/*sqrt(2)*/
	e>>= 1;

	f = (U32)((sr - 1.0)*((F32)(1<<23)));
	ieee = (((e+127)<<23) & 0x7F800000)  | (f & 0x7FFFFF);
	r = *(F32 *)&ieee;

	return r;
}
#endif
