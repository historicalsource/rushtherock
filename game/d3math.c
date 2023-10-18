#include "fmath.h"

#define radtodeg (180.0/3.141592654)
#define degtorad (3.141592654/180.0)
#define radtoideg ((0x40000000*2.0)/3.141592654)
#define idegtorad (3.141592654/(0x40000000*2.0))
#define radtosdeg (0x8000/3.141592654)
#define sdegtorad (3.141592654/0x8000)
#define idegtodeg (F32)(90.0/0x40000000)
#define degtoideg (0x40000000/90.0)
#define msininv (F32)(1.0/0x7fff)
#define rdpstorpm (60.0/(2.0*3.14159))
#define rpmtordps (2.0*3.14159/60.0)
#define pival 3.141592654

float debug_sqrt;

float xxsqrt(float x)
{
    double temp;
    
    temp = (double)(x);
    /* !!!! figure out sqrt later !!!!! */
    debug_sqrt = (float)(fsqrt(x));
    return(debug_sqrt);
}

#if 1
F32 invsqr(F32 val)
{
	if (val<1e-4 && val>-1e-4)
		val = (val<0) ? -1e-4 : 1e-4;

	return(1.0/xxsqrt(val));
}

#if 0
F32 fatan2(F32 x, F32 y)
{
	return(fatan(x,y));
}
#endif

#ifndef fcos
F32 fcos(F32 angle)
{
	int cycle;
	
	if(angle < 0){
		angle = -angle;
	}

	if(angle > (pival*2)){
		cycle = angle/(pival*2);
		angle -= cycle*(pival*2);
	}
	
	if(angle > pival){
		/* qsin needs a value from pi/2 to -pi/2 */
		return(qsin(angle-(pival*3.0/2.0)));
	}
	else{
		/* qsin needs a value from pi/2 to -pi/2 */
		return(qsin((pival/2)-angle));
	}
}
#endif

#ifndef fsin
F32 fsin(F32 angle)
{
	return(fcos((pival/2)-angle));
}
#endif

/* Given x and y, this routine returns the sine and cosine of the
angle adjacent to x */

void hypotsincos(float x, float y, float *sinp, float *cosp)
{
/*	float invhypot;
	
	invhypot = invsqr(x*x+y*y);
	*sinp = x*invhypot;
	*cosp = y*invhypot;*/
	float angle;

	angle = fatan2((float)x,(float)y);
	*sinp = fsin(angle);
	*cosp = fcos(angle);
	
}
	
/* Given x and y, this routine returns the sine and cosine of the
angle adjacent to x, scaled 1.0 = 0x4000 */

void shypotsincos(short x, short y, short *sinp, short *cosp)
{
	register float fx,fy;
	float invhypot;
	
	fx=(float)x;
	fy=(float)y;
	invhypot = 0x4000*invsqr(fx*fx+fy*fy);
	*sinp = fx*invhypot;
	*cosp = fy*invhypot;
}

short mssin(short val)
{
	return(fsin(sdegtorad*val)*(float)0x4000);
}

short mscos(short val)
{
	return(fcos(sdegtorad*val)*(float)0x4000);
}

void sincosptp(short phi, short theta, short psi, short *sphip, short *cphip, 
				short *sthetap, short *cthetap, short *spsip, short *cpsip)
{
	*sthetap=mssin(theta);
	*cthetap=mscos(theta);
	*sphip=mssin(phi);
	*cphip=mscos(phi);
	*spsip=mssin(psi);
	*cpsip=mscos(psi);
}
#endif




