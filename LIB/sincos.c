/* sincos.c hacked from gcc mathlib */

#include "stdtypes.h"
#include "fmath.h"
#include "mathlib.h"
#include "error.h"

#define SINCOSACC 3

#define P7	 0.27204790957888846175e-14F
#define P6	-0.76429178068910467734e-12F
#define P5	 0.16058936490371589114e-9F
#define P4	-0.25052106798274584544e-7F
#define P3	 0.27557319210152756119e-5F
#define P2	-0.19841269841201840457e-3F
#define P1	 0.83333333333331650314e-2F
#define P0	-0.16666666666666665052e0F

static F32 SIN_POLY(F32 a)
{
	F32 z = a*a;
	F32 r;

#if (SINCOSACC >= 7)
	r = z*P7;
#else
	r = 0;
#endif
#if (SINCOSACC >= 6)
	r = z*(P6+r);
#endif
#if (SINCOSACC >= 5)
	r = z*(P5+r);
#endif
#if (SINCOSACC >= 4)
	r = z*(P4+r);
#endif
#if (SINCOSACC >= 3)
	r = z*(P3+r);
#endif
#if (SINCOSACC >= 2)
	r = z*(P2+r);
#endif
#if (SINCOSACC >= 1)
	r = z*(P1+r);
#endif
	r = z*(P0+r);
	
	return a + a*r;
}


extern S32 TrigCalls;

F32 fsin(F32 x)
{
	TrigCalls++;

	if (!FINITE(x))
	{
		Errorf(MATHERROR,"sin bad input: %f",(F32)x);
		return 0;
	}

	while (x < 0.0)
		x += 2.0*PI;
	while (x >= 2.0*PI)
		x -= 2.0*PI;

	if (x < PI_2)
		return SIN_POLY (x);
	else if (x < PI) 
	{
		x = PI - x;
		
		return SIN_POLY (x);
	}
	else if (x < 3.0 * PI_2) 
	{
		x = x - PI;
		return (-SIN_POLY (x));
	}
	else 
	{
		x = 2.0*PI - x;
		return (-SIN_POLY (x));
	}
}

F32 fcos(F32 x)
{
	if (!FINITE(x))
	{
		Errorf(MATHERROR,"cos bad input: %f",(F32)x);
		return 0;
	}
	return fsin(x+PI_2);
}
