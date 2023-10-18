/* asincos.c hacked from gcc mathlib acos.h */

#include "stdtypes.h"
#include "mathlib.h"
#include "fmath.h"
#include "error.h"

#define P1 -0.27368494524164255994e2
#define P2  0.57208227877891731407e2
#define P3 -0.39688862997504877339e2
#define P4  0.10152522233806463645e2
#define P5 -0.69674573447350646411e0
#define Q0 -0.16421096714498560795e3
#define Q1  0.41714430248260412556e3
#define Q2 -0.38186303361750149284e3
#define Q3  0.15095270841030604719e3
#define Q4 -0.23823859153670238830e2
#define Q5  1.00000000000000000000e0 /* Inherent */

#define P(g) ((((P5*(g)+P4)*(g)+P3)*(g)+P2)*(g)+P1)
#define Q(g) ((((((g)+Q4)*(g)+Q3)*(g)+Q2)*(g)+Q1)*(g)+Q0)


/*
 * Arcsin/arccos routines from Cody and Waite
 */


static F32 farcsine(F32 x, int flag)
{
	F32 y, g, r;
	int i;
	static const F32 a[2] = { 0.0, 0.78539816339744830962 };
	static const F32 b[2] = { 1.57079632679489661923, 0.78539816339744830962 };

	if (x < -1.0 || x > 1.0) 
	{
		Errorf(MATHERROR,"arcsine(%d) bad input: %f",flag,(F32)x);
		return 0;
	}

	y = ABS(x);
	i = flag;
	if (y < 2.3e-10)
		r = y;
	else
	{
		if (y > 0.5)
		{
			i = 1 - i;
			if (y > 1.0)
			{
				Errorf(MATHERROR,"arcsine(%d) bad input: %f",flag,(double)x);
				return 0;
			}
			
			g = (0.5 - y) + 0.5;
			g = ldexpf(g,-1);
			y = fsqrt(g);
			y = -(y + y);
		}
		else
			g = y * y;
		r = y + y * ((P(g)*g)/Q(g));
    }
	if (flag) 
	{
		if (x < 0.0)
			r = (b[i] + r) + b[i];
		else
			r = (a[i] - r) + a[i];
	} 
	else 
	{
		r = (a[i] + r) + a[i];
		if (x < 0.0)
			r = -r;
	}
	return r;
}

F32 fasin(F32 x)
{
  return farcsine(x,0);
}

F32 facos(F32 x)
{
  return farcsine(x,1);
}

#if 0
F32 fatan(F32 x)
{
	return atanf(x);
}

F32 fatan2(F32 x,F32 y)
{
	return atan2f(x,y);
}
#endif
