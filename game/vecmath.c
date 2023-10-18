#include "typedefs.h"

#include "Pro/vecmath.pro"
#include "Pro/d3math.pro"

/* This file contains vector mathematics functions */

/*F32 fsqrt(),invsqr();*/
/*F32 xxsqrt(),invsqr();*/

F32 magnitude(F32 vec[3])
{
	return((F32)(xxsqrt(vec[0]*vec[0] + vec[1] * vec[1] +
		vec[2]*vec[2])));
}

F32 invmagnitude(F32 vec[3])
{
	return(invsqr(vec[0]*vec[0] + vec[1] * vec[1] +
		vec[2]*vec[2]));
}

void direction(F32 vec[3], F32 dir[3])
{
	F32 invmag;

	invmag = invmagnitude(vec);
	dir[0] = vec[0]*invmag;
	dir[1] = vec[1]*invmag;
	dir[2] = vec[2]*invmag;
}

#if !VECMACROS

void scalmul(F32 *a, F32 b, F32 *r)
{
	register int i;
	register F32 *ap,*bp,*rp;
	F32 bval;

	ap=a;
	bval = b;

	bp= &bval;
	rp=r;
	for(i=0;i<3;++i)
		*rp++ = *ap++ * *bp;
}

void scaldiv(F32 *a, F32 b, F32 *r)
{
	register int i;
	register F32 *ap,*bp,*rp;
	F32 binv;

	
	ap=a;
	
	binv = 1.0/b;
	bp= &binv;
	
	rp=r;
	for(i=0;i<3;++i)
		*rp++ = *ap++ * *bp;
}

void vecadd(register F32 *ap, register F32 *bp, register F32 *rp)
{
	*rp++ = *ap++ + *bp++;
	*rp++ = *ap++ + *bp++;
	*rp++ = *ap++ + *bp++;
}

void vecsub(register F32 *ap, register F32 *bp, register F32 *rp)
{
	*rp++ = *ap++ - *bp++;
	*rp++ = *ap++ - *bp++;
	*rp++ = *ap++ - *bp++;
}

void ivecadd(register int *ap, register int *bp, register int *rp)
{

	*rp++ = *bp++ + *ap++;
	*rp++ = *bp++ + *ap++;
	*rp++ = *bp++ + *ap++;
}

void ivecsub(register S32 *ap, register S32 *bp, register S32 *rp)
{
	*rp++ = *ap++ - *bp++;
	*rp++ = *ap++ - *bp++;
	*rp++ = *ap++ - *bp++;
}

void crossprod(F32 a[3], F32 b[3], F32 r[3])
{
	F32 temp[3];
	r[0]    = a[1] * b[2];
	temp[0] = a[2] * b[1]; 
	r[1]    = a[2] * b[0];
	temp[1] = a[0] * b[2]; 
	r[2]    = a[0] * b[1];
	temp[2] = a[1] * b[0];
	vecsub(r,temp,r);
}

F32 dotprod(F32 a[3], F32 b[3])
{
	return(a[0]*b[0] + a[1]*b[1] + a[2]*b[2]);

#if 0
/*optimized for old processor*/
	int i;
	F32 temp[3],foo;
	register F32 *tmp,*ap,*bp;
	
	tmp=temp;
	ap=a;
	bp=b;

	for(i=0;i<3;++i)
		*tmp++ = *ap++ * *bp++;

	foo = temp[0];
	tmp= &temp[1];

	for(i=1;i<3;++i)
		foo += *tmp++;

	return(foo);
#endif
}

void veccopy(F32 *a, F32 *r)
{
	register int i;
	register F32 *ap,*bp,*rp;

	ap=a;
	rp=r;
	for(i=0;i<3;++i)
		*rp++ = *ap++;
}
#endif

float smagnitude(short vec[3])
{
	return(xxsqrt((float)vec[0]*(float)vec[0] + 
		(float)vec[1] * (float)vec[1] +
		(float)vec[2]*(float)vec[2]));
}

F32 invsmagnitude(short vec[3])
{
    if (vec == 0) {
	while (1);
	}
	return(invsqr((float)vec[0]*(float)vec[0] + 
		(float)vec[1] * (float)vec[1] +
		(float)vec[2]*(float)vec[2]));
}

float invmag;

void sdirection(short vec[3], short dir[3])
{

	invmag = invsmagnitude(&(vec[0]))*0x4000;
	dir[0] = vec[0]*invmag;
	dir[1] = vec[1]*invmag;
	dir[2] = vec[2]*invmag;
}

void scrossprod(short a[3], short b[3], short r[3])
{
	r[0] = ((float)a[1] * (float)b[2] - (float)a[2] * (float)b[1])*
		(1.0/0x4000);
	r[1] = ((float)a[2] * (float)b[0] - (float)a[0] * (float)b[2])*
		(1.0/0x4000);
	r[2] = ((float)a[0] * (float)b[1] - (float)a[1] * (float)b[0])*
		(1.0/0x4000);
}

