#ifndef _FMATH_H
#define _FMATH_H

#include "stdtypes.h"
#include <math.h>
#include <ieeefp.h>

#if CUSTOM_MATH
#include "mathlib.h"
#endif

#define FINITE(x) (((__ieee_float_shape_type *)&(x))->number.exponent != __IEEE_FLT_NAN_EXP)

/*************************** CONVERT LIBRARY ROUTINES TO OURS ************************/

#if 1 || !HOCKEY
#define fsin(a) 	sinf(a)
#define fcos(a) 	cosf(a)
#define ftan(a) 	tanf(a)
#define fasin(a) 	asinf(a)
#define facos(a) 	acosf(a)
#define fatan(a)	atanf(a)
#define fatan2(y,x)	atan2f(y,x)
#else
extern F32 fsin(F32 a);
extern F32 fcos(F32 a);
extern F32 ftan(F32 a);
extern F32 fasin(F32 a);
extern F32 facos(F32 a);
extern F32 fatan(F32 a);
extern F32 fatan2(F32 y,F32 x);
#endif

#if 1
#define fsqrt(a)	sqrtf(a)
#else
extern F32 fsqrt(F32 a);
#endif

/* MATRICIES are stored as RIGHT-handed 3x3 matrices plus a pos vector.
 * If expanded to a 4x4 matrix, the representation would look like this:
 *	  |	U0	U1	U2	X  |
 *	  | V0	V1	V2	Y  |		U = XUV, V = YUV, N = ZUV
 *	  | N0	N1	N2	Z  |
 *	  | 0	0	0	1  |
 * Note that this is NOT how the data is stored in menory, ie the 3x3
 *	portion is correct, but X,Y,Z are stored as a vector after the
 *	3x3 portion for ease of access.
 */

typedef struct {		/* 3x3 with position matrix */
	F32 uvs[3][3];			/* orientation matrix - right handed */
	F32 pos[3];				/* position data */
}MAT3;

typedef struct {		/* 3x3 with position matrix */
	F32 uvs[4][3];			/* orientation matrix - right handed */
}MAT4;

typedef struct {		/* vector matrix */
	F32 xuv[3];				/* direction - in world frame - of x vector */
	F32 yuv[3];				/* direction - in world frame - of y vector */
	F32 zuv[3];				/* direction - in world frame - of z vector */
 	F32 pos[3];				/* position data */
}MATV;


typedef union {			/* union containing the matrix data */
	MAT3 mat3;
	MAT4 mat4;
	MATV matv;
	F32 uvs[4][3];
}MATRIX;

/********************************* SPECIAL MACROS ************************************/
#define MPos	Mat.mat3.pos
#define MUVS	Mat.mat3.uvs
#define DMPos	DOB->Mat.mat3.pos
#define DMUVS	DOB->Mat.mat3.uvs
#define POS	mat.mat3.pos
#define UVS mat.mat3.uvs
#define POSP mat->mat3.pos
#define UVSP mat->mat3.uvs
#define INITPOS	initmat.mat3.pos
#define INITUVS initmat.mat3.uvs
#define XUV mat.matv.xuv
#define YUV mat.matv.yuv
#define ZUV mat.matv.zuv
#define XUVP mat->matv.xuv
#define YUVP mat->matv.yuv
#define ZUVP mat->matv.zuv

/*************************************************************************************/
extern const F32 unituvs[3][3];		/* unit vector matrix */
extern const F32 zerovector[3];		/* for zeroing out vectors */
extern const F32 uvector[3];		/* up vector */
extern const F32 dvector[3];		/* down vector */
extern const F32 rvector[3];		/* right vector */
extern const F32 lvector[3];		/* left vector */
extern const F32 fvector[3];		/* forward vector */
extern const F32 bvector[3];		/* backward vector */
extern const F32 hugevector[3];		/* huge vector */
extern const MAT3 unitmat;			/* Unit MATRIX */
extern const MAT3 lrmat;			/* L-hand to R-hand transform */
extern const F32 unitquat[4];		/* Unit quaternian */
extern const F32 quatyaw180[4];		/* Quaternian yaw of 180 deg */

/******************************* GENERAL MACROS **************************************/
#define PI			3.141592654
#define PI_2		1.570796327
#define BIGFLOAT	1.0e20

#ifndef FOOT
#define FOOT 1.0F
#endif
#define ABS(a)		fabsf(a)
#define SQR(a)		((a)*(a))
#define MIN(a,b)	(((a)<(b)) ? (a) : (b))
#define MAX(a,b)	(((a)>(b)) ? (a) : (b))
#define SIGN(a)		(((a)>=0)  ?  1  : -1)
#define MINMAX(a,min,max) (a<min ? min : a>max ? max : a)
#ifndef RAD
#define RAD(a) 	(((a)*PI)*(1.0/180.0))		/* convert degrees to radians */
#endif
#ifndef DEG
#define DEG(a)	(((a)*180.0)*(1.0/PI))		/* convert radians to degrees */
#endif
/********************* MACROS THAT PERFORM MATH FUNCTIONS ****************************/
#define SIMPLEANGLE(ang)		(ang >   RAD(180) ? ang-RAD(360) : \
								 ang <= -RAD(180) ? ang+RAD(360) : ang)

#define CopyVector(v1,v2)		(v2[0] = v1[0], v2[1] = v1[1], v2[2] = v1[2])
#define ZeroVector(v1)			(v1[0] = 0.0, v1[1] = 0, v1[2] = 0.0)

#define SubVector(v1,v2,r)		(r[0] = v1[0]-v2[0], r[1] = v1[1]-v2[1], r[2] = v1[2]-v2[2])
#define SubVector2D(v1,v2,r)	(r[0] = v1[0]-v2[0], r[1] = 0, r[2] = v1[2]-v2[2])
#define AddVector(v1,v2,r)		(r[0] = v1[0]+v2[0], r[1] = v1[1]+v2[1], r[2] = v1[2]+v2[2])
#define AddVector2D(v1,v2,r)	(r[0] = v1[0]+v2[0], r[1] = 0, r[2] = v1[2]+v2[2])
#define ScaleVector(v1,s,r)		(r[0] = v1[0]*(s), r[1] = v1[1]*(s), r[2] = v1[2]*(s))
#define ScaleVector2D(v1,s,r)	(r[0] = v1[0]*(s), r[1] = 0, r[2] = v1[2]*(s))
#define ScaleAddVector(a,s,b,r)	(r[0] = (a[0]*(s)+b[0]), r[1] = (a[1]*(s)+b[1]), r[2] = (a[2]*(s)+b[2]))
#define ScaleAddVector2D(a,s,b,r)	(r[0] = (a[0]*(s)+b[0]), r[1] = 0, r[2] = (a[2]*(s)+b[2]))
#define ScaleSubVector(a,s,b,r)	(r[0] = (a[0]*(s)-b[0]), r[1] = (a[1]*(s)-b[1]), r[2] = (a[2]*(s)-b[2]))
#define ScaleSubVector2D(a,s,b,r)	(r[0] = (a[0]*(s)-b[0]), r[1] = 0, r[2] = (a[2]*(s)-b[2]))
#define CrossVector(v1,v2,r)	(r[0] = v1[1]*v2[2] - v1[2]*v2[1], \
								 r[1] = v1[2]*v2[0] - v1[0]*v2[2], \
    							 r[2] = v1[0]*v2[1] - v1[1]*v2[0])
#define DotVector(v1,v2)		(v1[0]*v2[0] + v1[1]*v2[1] + v1[2]*v2[2])
#define DotVector2D(v1,v2)		(v1[0]*v2[0] + v1[2]*v2[2])
#define SameVector(v1,v2)		((v1[0]!=v2[0]) ? 0 : (v1[1]!=v2[1]) ? 0 : \
								 (v1[2]!=v2[2]) ? 0 : 1)
#define DotQuat(q1,q2)			(q1[0]*q2[0] + q1[1]*q2[1] + q1[2]*q2[2] + q1[3]*q2[3])
#define VectorLength(v1)		(fsqrt(SQR(v1[0]) + SQR(v1[1]) + SQR(v1[2])))
#define VectorLength2D(v1)		(fqdist(v1[0],v1[2]))
#define VectorLength2(v1)		(SQR(v1[0]) + SQR(v1[1]) + SQR(v1[2]))

#define MoveVector(p,v,amt)		(p[0] += v[0]*(amt), p[1] += v[1]*(amt), p[2] += v[2]*(amt))
#define MoveinX(mat,amt)		(MoveVector(mat.matv.pos,mat.matv.xuv,amt))
#define MoveinY(mat,amt)		(MoveVector(mat.matv.pos,mat.matv.yuv,amt))
#define MoveinZ(mat,amt)		(MoveVector(mat.matv.pos,mat.matv.zuv,amt))

#define GetYaw(vec)				(fatan2(vec[0],vec[2]))

#define CopyQuat(q1,q2)			(q2[0] = q1[0], q2[1] = q1[1], q2[2] = q1[2], q2[3] = q1[3])

#include "Pro/fmath.pro"

	
#endif
