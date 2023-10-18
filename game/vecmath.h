
#ifndef _VECMATH_H
#define _VECMATH_H

#include "typedefs.h"

#define VECMACROS 0			/* Use macros for vector functions */

#define mabs(v) 		(((v) < 0) ? -(v) : (v))

typedef struct uvect {
	short uvs[3][3];
	F32 fpuvs[3][3];
	short fixcnt;
} uvect;

#define mscalmul(a,b,r)	{r[0]=a[0]*b; r[1]=a[1]*b; r[2]=a[2]*b;}
#define mscaldiv(a,b,r)	{r[0]=a[0]/b; r[1]=a[1]/b; r[2]=a[2]/b;}
#define mvecadd(a,b,r)	{r[0]=a[0]+b[0]; r[1]=a[1]+b[1]; r[2]=a[2]+b[2];}
#define mvecsub(a,b,r)	{r[0]=a[0]-b[0]; r[1]=a[1]-b[1]; r[2]=a[2]-b[2];}
#define mveccopy(a,r)	{r[0]=a[0]; r[1]=a[1]; r[2]=a[2];}

#if VECMACROS
#define scalmul(a,b,r)	mscalmul(a,b,r)
#define scaldiv(a,b,r)	mscaldiv(a,b,r)
#define ivecadd(a,b,r)	mvecadd(a,b,r)
#define ivecsub(a,b,r)	mvecsub(a,b,r)
#define vecadd(a,b,r)	{r[0]=a[0]+b[0]; r[1]=a[1]+b[1]; r[2]=a[2]+b[2];}
#define vecsub(a,b,r)	{r[0]=a[0]-b[0]; r[1]=a[1]-b[1]; r[2]=a[2]-b[2];}
#define crossprod(a,b,r) {r[0] = a[1] * b[2] - a[2] * b[1]; r[1] = a[2] * b[0] - a[0] * b[2]; r[2] = a[0] * b[1] - a[1] * b[0];}
#define dotprod(a,b)	{a[0]*b[0]+a[1]*b[1]+a[2]*b[2];}
#define veccopy(a,r)	{r[0]=a[0]; r[1]=a[1]; r[2]=a[2];}
#endif

#define XCOMP 0
#define YCOMP 1
#define ZCOMP 2


#endif
/* end #ifndef _VECMATH_H */









