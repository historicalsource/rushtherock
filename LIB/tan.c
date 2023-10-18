/* tan.c hacked from gcc mathlib */

#include "stdtypes.h"
#include "fmath.h"
#include "mathlib.h"
#include "error.h"

#define P1 -0.13338350006421960681e+0
#define P2  0.34248878235890589960e-2
#define P3 -0.17861707342254426711e-4
#define Q0  1.0
#define Q1 -0.46671683339755294240e+0
#define Q2  0.25663832289440112864e-1
#define Q3 -0.31181531907010027307e-3
#define Q4  0.49819433993786512270e-6

#define P(f,g) (((P3*(g)+P2)*(g)+P1)*(g)*(f)+(f))
#define Q(g) ((((Q4*(g)+Q3)*(g)+Q2)*(g)+Q1)*(g)+Q0)

#define YMAX 6.74652e09

/* ~540 cycles */
F32 ftan(F32 x)
{
	F32 f, g, xn;
	F32 xnum, xden;
	F32 y = ABS(x);
	
	if (y > YMAX) 
	{
		Errorf(MATHERROR,"tan overflow: %f",(F32)x);
		return 0;
	}
	if (ABS(modff(x*0.63661977236758134308, &xn)) >= 0.5)
		xn += (x < 0.0) ? -1.0 : 1.0;
	f = modff(x, &g);
	f = ((g - xn*(3217.0/2048)) + f) - xn*-4.454455103380768678308e-6;
	if (ABS(f) < 2.33e-10) 
	{
		xnum = f;
		xden = 1.0;
	} else 
	{
		g = f*f;
		xnum = P(f,g);
		xden = Q(g);
	}
	if ((int)xn & 1)
		return xden/-xnum;
	else
		return xnum/xden;
}

