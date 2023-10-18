/* atan.c hacked from gcc mathlib */
/* from /home/wiz/usr4/gnu/cygnus4/newlib/libm/math/ */
#include "stdtypes.h"
#include "fmath.h"
#include "mathlib.h"
#include "error.h"

#define pio2	 1.570796326794896619
#define sq2p1	 2.414213562373095048802e0
#define sq2m1	  .414213562373095048802e0
#define pio4	  .785398163397448309615e0
#define p4	  .161536412982230228262e2
#define p3	  .26842548195503973794141e3
#define p2	  .11530293515404850115428136e4
#define p1	  .178040631643319697105464587e4
#define p0	  .89678597403663861959987488e3
#define q4	  .5895697050844462222791e2
#define q3	  .536265374031215315104235e3
#define q2	  .16667838148816337184521798e4
#define q1	  .207933497444540981287275926e4
#define q0	  .89678597403663861962481162e3

/*
	xatan evaluates a series valid in the
	range [-0.414...,+0.414...].
*/

static F32 fxatan(F32 arg)
{
	F32 argsq;
	F32 value;

	argsq = arg*arg;
	value = ((((p4*argsq + p3)*argsq + p2)*argsq + p1)*argsq + p0);
	value = value/(((((argsq + q4)*argsq + q3)*argsq + q2)*argsq + q1)*argsq + q0);
	return(value*arg);
}


F32 fsatan(F32 arg)
{
	if(arg < sq2m1)
		return(fxatan(arg));
	else if(arg > sq2p1)
		return(pio2 - fxatan(1.0/arg));
	return(pio4 + fxatan((arg-1.0)/(arg+1.0)));
}


F32 fatan2(F32 arg_1,F32 arg_2)
{
	if( arg_1+arg_2 == arg_1) 
	{
		if(arg_1 >= 0.0) return PI_2;
		if(arg_1 == 0.0) return 0;
		
		return -PI_2;
	}
	else if(arg_2 <0.)
		if(arg_1 >= 0.)
			return PI - fsatan(-arg_1/arg_2);
		else
			return -PI + fsatan(arg_1/arg_2);
	else if(arg_1>0)
		return fatan(arg_1/arg_2);
	return -fatan(-arg_1/arg_2);
}


F32 fatan(F32 arg) 
{
	if(arg>0)
		return(fsatan(arg));
	else
		return(-fsatan(-arg));
}
