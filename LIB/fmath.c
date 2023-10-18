/*****************************************************************************************
3D Hockey :

Copyright 1994,1995 Time Warner Interactive.  Unauthorized reproduction, adaptation,
distribution, performance or display of this computer program or the associated
audiovisual work is strictly prohibited.
*****************************************************************************************/

#include <stdlib.h>
#include "stdtypes.h"
#include "os_proto.h"
#include "fmath.h"
#include "error.h"
#include "config.h"

#if 0
extern F32 fatan2(F32 y,F32 x);
#endif

S32 TrigCalls;
S32 SqrtCalls;
/******* MATRIX ROUTINES *******/

/* general matix routines */
void CopyUV(const F32 a[][3], F32 b[][3]) {
	int i,j;

	for(i=0;i<3;i++) {
		for(j=0;j<3;j++)
			b[i][j] = a[i][j];
	}
}


/* transpose a 3x3 matrix */
void TransposeUV(F32 uv[][3]) {
	F32 tmp;
	
	tmp = uv[0][1];
	uv[0][1] = uv[1][0];
	uv[1][0] = tmp;

	tmp = uv[0][2];
	uv[0][2] = uv[2][0];
	uv[2][0] = tmp;

	tmp = uv[1][2];
	uv[1][2] = uv[2][1];
	uv[2][1] = tmp;
}

/* transpose a 3x3 matrix */
void TransposeCopyUV(F32 uv[][3],F32 uv2[][3]) {
	uv2[0][0] = uv[0][0];
	uv2[0][1] = uv[1][0];
	uv2[0][2] = uv[2][0];

	uv2[1][0] = uv[0][1];
	uv2[1][1] = uv[1][1];
	uv2[1][2] = uv[2][1];

	uv2[2][0] = uv[0][2];
	uv2[2][1] = uv[1][2];
	uv2[2][2] = uv[2][2];
}


/* scale a UV structure */
void ScaleUV(const F32 a[][3], F32 b[][3], F32 sfactor) {
	int i,j;

	for(i=0;i<3;i++) {
		for(j=0;j<3;j++)
			b[i][j] = a[i][j]*sfactor;
	}
}


/* Fix the rows of a UV structure - row wise */
void FixRowUV(F32 a[][3]) {
	SlowNormalVector(a[0]);
	SlowNormalVector(a[2]);
	CrossVector(a[2],a[0],a[1]);
}


/* Fix the columns of a UV structure - column wise */
void FixColumnUV(F32 a[][3]) {
	TransposeUV(a);
	FixRowUV(a);
	TransposeUV(a);
}


/* Rotate a unit vector structure by a vector */
void RotateUV(F32 rpyv[3], F32 uvs[][3]) {
	if(rpyv[1])
		YawUV(rpyv[1],uvs);

	if(rpyv[0])
		PitchUV(rpyv[0],uvs);

	if(rpyv[2])
		RollUV(rpyv[2],uvs);
}


/***** yaw in universe axis *****/
void WYawUV(F32 angle, F32 uv[][3]) {
	F32 ut,sint,cost;
	int i;

	if((angle < -0.001) || (angle > 0.001)) {
		sint = fsin(angle);
		cost = fcos(angle);
		for(i=0;i<3;i++) {
			ut		 = uv[i][0]*cost - uv[i][2]*sint;
			uv[i][2] = uv[i][2]*cost + uv[i][0]*sint;
			uv[i][0] = ut; 
		}
	}
}


/***** pitch unit vector using sint & cost *****/
void WPitchUV(F32 angle, F32 uv[][3]) {
	F32 ut,sint,cost;
	int i; 

	if((angle < -0.001) || (angle > 0.001)) {
		sint = fsin(angle);
		cost = fcos(angle);
		for(i=0;i<3;i++) { 
			ut		 = uv[i][1]*cost - uv[i][2]*sint;
			uv[i][2] = uv[i][2]*cost + uv[i][1]*sint;
			uv[i][1] = ut; 
		}
	}
} 


/***** roll unit vector using sint & cost *****/
void WRollUV(F32 angle, F32 uv[][3]) {
	F32 ut,sint,cost;
	int i;

	if((angle < -0.001) || (angle > 0.001)) {
		sint = fsin(angle);
		cost = fcos(angle);
		for(i=0;i<3;i++) { 
			ut		 = uv[i][0]*cost - uv[i][1]*sint;
			uv[i][1] = uv[i][1]*cost + uv[i][0]*sint;
			uv[i][0] = ut; 
		}
	}
} 


/***** yaw unit vector using sint & cost *****/
void YawUV(F32 angle, F32 uv[][3]) {
	F32 ut,sint,cost;
	int i; 

	if((angle < -0.001) || (angle > 0.001)) {
		sint = fsin(angle);
		cost = fcos(angle);
		for(i=0;i<3;i++) { 
			ut		 = uv[0][i]*cost - uv[2][i]*sint;
			uv[2][i] = uv[2][i]*cost + uv[0][i]*sint;
			uv[0][i] = ut; 
		}
	}
}


/***** pitch unit vector using sint & cost *****/
void PitchUV(F32 angle, F32 uv[][3]) {
	F32 ut,sint,cost;
	int i; 

	if((angle < -0.001) || (angle > 0.001)) {
		sint = fsin(angle);
		cost = fcos(angle);
		for(i=0;i<3;i++) { 
			ut		 = uv[1][i]*cost - uv[2][i]*sint;
			uv[2][i] = uv[2][i]*cost + uv[1][i]*sint;
			uv[1][i] = ut; 
		}
	}
} 


/***** roll unit vector using sint & cost *****/
void RollUV(F32 angle, F32 uv[][3]) {
	F32 ut,sint,cost;
	int i;

	if((angle < -0.001) || (angle > 0.001)) {
		sint = fsin(angle);
		cost = fcos(angle);
		for(i=0;i<3;i++) { 
			ut		 = uv[0][i]*cost - uv[1][i]*sint;
			uv[1][i] = uv[1][i]*cost + uv[0][i]*sint;
			uv[0][i] = ut; 
		}
	}
} 

typedef F32 fvec3[3];

/* Multiply a 4*3 matrix times a 4*3 matrix. */
/* ACTUALLY MULTIPLIES MATRICIES IN REVERSE ORDER BECAUSE THEY'RE RIGHT-HANDED */
/* Note, this multiplies 4x3 matricies, NOT 3x4 mats */
void MulMat(MATRIX *BB, MATRIX *AA, MATRIX *CC)
{
	int	i;
	fvec3 *A,*B,*C;
	F32 *Apos,*Bpos,*Cpos;
	
	A = AA->mat3.uvs;
	B = BB->mat3.uvs;
	C = CC->mat3.uvs;
	Apos = AA->mat3.pos;
	Bpos = BB->mat3.pos;
	Cpos = CC->mat3.pos;

	for(i=0;i<3;i++)
	{
		C[i][0] = A[i][0]*B[0][0] + A[i][1]*B[1][0] + A[i][2]*B[2][0];
		C[i][1] = A[i][0]*B[0][1] + A[i][1]*B[1][1] + A[i][2]*B[2][1];
		C[i][2] = A[i][0]*B[0][2] + A[i][1]*B[1][2] + A[i][2]*B[2][2];
		Cpos[i] = B[0][i]*Apos[0] + B[1][i]*Apos[1] + B[2][i]*Apos[2] + Bpos[i];
	}
}

/* Multiply a 4*3 matrix times a 4*3 matrix. */
/* ACTUALLY MULTIPLIES MATRICIES IN REVERSE ORDER BECAUSE THEY'RE RIGHT-HANDED */
/* Note, this multiplies 4x3 matricies, NOT 3x4 mats */
void MulUVS(F32 B[][3], F32 A[][3], F32 C[][3])
{
	int	i;
	for(i=0;i<3;i++)
	{
		C[i][0] = A[i][0]*B[0][0] + A[i][1]*B[1][0] + A[i][2]*B[2][0];
		C[i][1] = A[i][0]*B[0][1] + A[i][1]*B[1][1] + A[i][2]*B[2][1];
		C[i][2] = A[i][0]*B[0][2] + A[i][1]*B[1][2] + A[i][2]*B[2][2];
	}
}

/******* VECTOR ROUTINES *******/

const F32 unituvs[3][3]={{1.0,0,0},{0,1.0,0},{0,0,1.0}};	/* unit vector matrix */
const F32 zerovector[3]={0,0,0};							/* for zeroing out vectors */
const F32 uvector[3]={0,1.0,0};								/* up vector */
const F32 dvector[3]={0,-1.0,0};							/* down vector */
const F32 rvector[3]={1.0,0,0};								/* right vector */
const F32 lvector[3]={-1.0,0,0};							/* left vector */
const F32 fvector[3]={0,0,1.0};								/* forward vector */
const F32 bvector[3]={0,0,-1.0};							/* backward vector */
const F32 hugevector[3]={1000000.0,1000000.0,1000000.0};	/* huge vector */
const MAT3 unitmat = {{{1.0,0,0},{0,1.0,0},{0,0,1.0}},{0.0,0.0,0.0}};
const MAT3 lrmat =  {{{-1.0,0,0},{0,1.0,0},{0,0,-1.0}},{0.0,0.0,0.0}};
const F32 unitquat[4] = {0.0,0.0,0.0,1.0};
const F32 quatyaw180[4] = {0.0,1.0,0.0,0.0};

void NumberIntegrity(F32 *n) {
#if 1
	if(!FINITE(*n))
		*n = 0.0;
	else if((*n < -1.0e6) || (*n > 1.0e6))
		*n = 0.0;
	else if((*n > -1.0e-4) && (*n < 1.0e-4))
		*n = 0.0;
#endif
}

void VectorIntegrity(F32 v[3]) {
#if 1
	int i;

	for(i=0;i<3;i++)
		NumberIntegrity(&v[i]);
#endif
}


/* Transform a point from world coordinates into the uvs body coordinates */
void BodyVector(const F32 uvec[3],F32 bodvec[3],const F32 uvs[][3]) {
	bodvec[0] = uvec[0]*uvs[0][0] + uvec[1]*uvs[0][1] + uvec[2]*uvs[0][2];
	bodvec[1] = uvec[0]*uvs[1][0] + uvec[1]*uvs[1][1] + uvec[2]*uvs[1][2];
	bodvec[2] = uvec[0]*uvs[2][0] + uvec[1]*uvs[2][1] + uvec[2]*uvs[2][2];
}

/* Transform a point from world coordinates into the uvs body - Get just the X */
F32 BodyX(const F32 uvec[3],const F32 uvs[][3]) {
	return( (uvec[0]*uvs[0][0] + uvec[1]*uvs[0][1] + uvec[2]*uvs[0][2]) );
}

/* Transform a point from world coordinates into the uvs body - Get just the Y */
F32 BodyY(const F32 uvec[3],const F32 uvs[][3]) {
	return( (uvec[0]*uvs[1][0] + uvec[1]*uvs[1][1] + uvec[2]*uvs[1][2]) );
}

/* Transform a point from world coordinates into the uvs body - Get just the Z */
F32 BodyZ(const F32 uvec[3],const F32 uvs[][3]) {
	return( (uvec[0]*uvs[2][0] + uvec[1]*uvs[2][1] + uvec[2]*uvs[2][2]) );
}


/* Transform a point from body/uvs coordinates into the world coordinates */
void WorldVector(const F32 bodvec[3],F32 uvec[3],const F32 uvs[][3]) {
	uvec[0] = bodvec[0]*uvs[0][0] + bodvec[1]*uvs[1][0] + bodvec[2]*uvs[2][0];
	uvec[1] = bodvec[0]*uvs[0][1] + bodvec[1]*uvs[1][1] + bodvec[2]*uvs[2][1];
	uvec[2] = bodvec[0]*uvs[0][2] + bodvec[1]*uvs[1][2] + bodvec[2]*uvs[2][2];
}

/* Transform a point from body/uvs coordinates into the world - Get Just the X */
F32 WorldX(const F32 bodvec[3], const F32 uvs[][3]) {
	return( (bodvec[0]*uvs[0][0] + bodvec[1]*uvs[1][0] + bodvec[2]*uvs[2][0]) );
}

/* Transform a point from body/uvs coordinates into the world - Get Just the Y */
F32 WorldY(const F32 bodvec[3], const F32 uvs[][3]) {
	return( (bodvec[0]*uvs[0][1] + bodvec[1]*uvs[1][1] + bodvec[2]*uvs[2][1]) );
}

/* Transform a point from body/uvs coordinates into the world - Get Just the Z */
F32 WorldZ(const F32 bodvec[3], const F32 uvs[][3]) {
	return( (bodvec[0]*uvs[0][2] + bodvec[1]*uvs[1][2] + bodvec[2]*uvs[2][2]) );
}

/* Multiply a vector times a 4*3 matrix into another vector (L-side) */
void MulVecMat(const F32 vin[3],F32 vout[3],const F32 M[4][3])
{
	vout[0] = vin[0]*M[0][0]+vin[1]*M[1][0]+vin[2]*M[2][0] + M[3][0];
	vout[1] = vin[0]*M[0][1]+vin[1]*M[1][1]+vin[2]*M[2][1] + M[3][1];
	vout[2] = vin[0]*M[0][2]+vin[1]*M[1][2]+vin[2]*M[2][2] + M[3][2];
}

/* Normalize a vector and return it's original length */
F32 NormalVector(F32 v[3]) {
	register F32 Sqrs,mag,v0,invmag;
	
	v0 = fqdist(v[0],v[2]);
	mag = fqdist(v0,v[1]);
	if(mag <= 0.0)
		invmag = 1.0;
	else
		invmag = (1.0/mag);

	ScaleVector(v,invmag,v);

	return(mag);
}

#define SLOWNORMALDBG	0
/* Normalize a vector and return it's original length */
static F32 oldtime;
static int line;

F32 SlowNormalVector(F32 v[3]) {
	F32 Sqrs,mag;
	F32 invmag;
#if SLOWNORMALDBG
	extern VF32 TIME;

	if (oldtime != TIME)
	{
		oldtime = TIME;
		line = 4;
	}
	xyechof(1,line++,"V:(%8.6f %8.6f %8.6f)    ",v[0],v[1],v[2]);
#endif
	Sqrs = 0.0;
	if(FINITE(v[0]) && ABS(v[0]) > 0.001)
		Sqrs += SQR(v[0]);
	if(FINITE(v[1]) && ABS(v[1]) > 0.001)
		Sqrs += SQR(v[1]);
	if(FINITE(v[2]) && ABS(v[2]) > 0.001)
		Sqrs += SQR(v[2]);
#if SLOWNORMALDBG
	xyechof(4,line++,"SQRS:%8.6f    ",Sqrs);
#endif
	if (!FINITE(Sqrs))
		return 0.0;
	if(Sqrs <= 1e-10)
		return(0.0);
	
	mag = fsqrt(Sqrs);
	if(!(mag > 0.000001))
		invmag = 100000.0;
	else	
		invmag = (1.0/mag);

	ScaleVector(v,invmag,v);
#if SLOWNORMALDBG
	xyechof(4,line++,"MAG:%8.6f INVMAG:%8.6f    ",mag,invmag);
#endif
	if (!(v[0] > .0000001 || v[0] < -.0000001)) v[0] = 0.0;
	if (!(v[1] > .0000001 || v[1] < -.0000001)) v[1] = 0.0;
	if (!(v[2] > .0000001 || v[2] < -.0000001)) v[2] = 0.0;
	if (!(mag > .0000001 || mag < -.0000001)) mag = 0.0;
	return(mag);
}

F32 NormalVector2D(F32 v[3]) {
	register F32 mag,invmag;

	mag = fqdist(v[0],v[2]);

	if(mag <= 0.0)
		invmag = 1.0;
	else	
		invmag = (1.0/mag);
	
	v[1] = 0.0;
	v[0] *= invmag;
	v[2] *= invmag;
	
	return(mag);
}

/****************************************************************
 * Perspective divide:
 * x2d = (x3d * 320) / z3d
 * y2d = (y3d * 320) / z3d
 * Uses reciprocal table for fast divides
 *****************************************************************/
void ProjectVector(const F32 v[3],S16 pt[2])
{
	pt[0] = 256 + v[0] * 2.41/2 * 512 / v[2];
	pt[1] = 192 - v[1] * 3.30/2 * 384 / v[2];
}

/****************************************************************
 * Returns the determinant of three Vec3's
 * v0,0 v0,1 v0,2
 * v1,0 v1,1 v1,2
 * v2,0 v2,1 v2,2
 *****************************************************************/
F32	Det3Vectors(const F32 v0[3],const F32 v1[3],const F32 v2[3])
{
	F32 a,b,c,d,e,f;

	a = v0[0] * v1[1] * v2[2];
	b = v0[1] * v1[2] * v2[0];
	c = v0[2] * v1[0] * v2[1];
	d = v2[0] * v1[1] * v0[2];
	e = v2[1] * v1[2] * v0[0];
	f = v2[2] * v1[0] * v0[1];
	return(a + b + c - d - e - f);
}


/*********************** MISCELLAEOUS MATH ROUTINES ***********************/
/******
  From a direction vector, assuming no roll, extract the yaw and pitch
  of the vector - extracting yaw first then pitch.
  ******/
void GetYawPitch(const F32 dvec[3], F32 *yawp, F32 *pitp) {
	F32 dist;
	
	*yawp = fatan2(dvec[0],dvec[2]);
	dist = fqdist(dvec[0],dvec[2]);
	*pitp = fatan2(dvec[1],dist);
}

void GetPitchYaw(const F32 dvec[3], F32 *pitp, F32 *yawp) {
	F32 dist;
	
	*pitp = fatan2(dvec[1],dvec[2]);
	dist = fqdist(dvec[1],dvec[2]);
	*yawp = fatan2(dvec[0],dist);
}

/*** compute the difference between two angles ***/
F32 SubAngle(F32 a, F32 b) {
	F32 r;

	r = a - b;
	if (r > 8*RAD(360.0) || r < -8*RAD(360.0))
		return 0;
	while(r > RAD(180.0))
		r -= RAD(360.0);	
	while(r <= RAD(-180.0))
		r += RAD(360.0);
	return(r);
}

F32 AddAngle(F32 a, F32 b) {
	F32 r;
	
	r = a + b;
	while(r > RAD(180.0))
		r -= RAD(360.0);	
	while(r <= RAD(-180.0))
		r += RAD(360.0);
	return(r);
}

F32 FixAngle(F32 a) {
	while(a > RAD(180.0))
		a -= RAD(360.0);	
	while(a <= RAD(-180.0))
		a += RAD(360.0);
	return(a);
}

F32 SubAngleP(F32 a, F32 b) {
	F32 r;

	r = a - b;
	while(r > RAD(360.0))
		r -= RAD(360.0);	
	while(r < RAD(0.0))
		r += RAD(360.0);
	return(r);
}

F32 AddAngleP(F32 a, F32 b) {
	F32 r;
	
	r = a + b;
	while(r >= RAD(360.0))
		r -= RAD(360.0);	
	while(r < RAD(0.0))
		r += RAD(360.0);
	return(r);
}

F32 FixAngleP(F32 a) {
	while(a >= RAD(360.0))
		a -= RAD(360.0);	
	while(a < RAD(0.0))
		a += RAD(360.0);
	return(a);
}

/*** Create a PYR matrix from scratch from triple pyr[3] (pitch, yaw, roll)  */
/*** NOTE: Slams numbers close to 0.0 to 0.0 to deal with lack of fp exception handler */
void CreatePYRMatrix(F32 mat[][3],F32 *pyr)
{
#if 1
	F32 cosP,sinP,cosY,sinY,cosR,sinR,temp;

	if (!(pyr[0] > .0000001 || pyr[0] < -.0000001)) pyr[0] = 0.0;
	if (!(pyr[1] > .0000001 || pyr[1] < -.0000001)) pyr[1] = 0.0;
	if (!(pyr[2] > .0000001 || pyr[2] < -.0000001)) pyr[2] = 0.0;
	/* Matricies in the game are transposed, so the pitch, yaw, and roll matrices
	   are also transposed. An easy way to do this is just to change the sign of sin */
	cosP = fcos(pyr[0]);
	sinP = -fsin(pyr[0]);
	cosY = fcos(pyr[1]);
	sinY = -fsin(pyr[1]);
	cosR = fcos(pyr[2]);
	sinR = -fsin(pyr[2]);

	mat[0][0] =	 cosY * cosR;
	mat[1][0] =	-cosY * sinR;
	mat[2][0] =	-sinY;
	
	temp = 	 	 sinP * sinY;
	mat[0][1] = -temp * cosR + cosP * sinR;
	mat[1][1] =  temp * sinR + cosP * cosR;
	mat[2][1] = -sinP * cosY;

	temp = 	 	 cosP * sinY;
	mat[0][2] =  temp * cosR + sinP * sinR;
	mat[1][2] =  temp *-sinR + sinP * cosR;
	mat[2][2] =  cosP * cosY;
#else
	CopyUV(unituvs,mat);
	PitchUV(pyr[0],mat);
	YawUV(pyr[1],mat);
	RollUV(pyr[2],mat);
#endif
}

/*** Create a RYP matrix from scratch from triple pyr[3] (pitch, yaw, roll)  */
/*** NOTE! pyr = Pitch Yaw Roll, NOT Yaw Pitch Roll (even though matrix is made YPR) */
/*** NOTE: Slams numbers close to 0.0 to 0.0 to deal with lack of fp exception handler */
void CreateRYPMatrix(F32 mat[][3], F32 *pyr)
{
#if 1
	F32 cosP,sinP,cosY,sinY,cosR,sinR,temp;

	if (!(pyr[0] > .0000001 || pyr[0] < -.0000001)) pyr[0] = 0.0;
	if (!(pyr[1] > .0000001 || pyr[1] < -.0000001)) pyr[1] = 0.0;
	if (!(pyr[2] > .0000001 || pyr[2] < -.0000001)) pyr[2] = 0.0;
	/* Matricies in the game are transposed, so the pitch, yaw, and roll matrices
	   are also transposed. An easy way to do this is just to change the sign of sin */
	cosP = fcos(pyr[0]);
	sinP = -fsin(pyr[0]);
	cosY = fcos(pyr[1]);
	sinY = -fsin(pyr[1]);
	cosR = fcos(pyr[2]);
	sinR = -fsin(pyr[2]);

	temp = 	 	-cosR * sinY;
	mat[0][0] =	 cosR * cosY;
	mat[1][0] =	-sinR * cosP + temp * sinP;
	mat[2][0] =	 sinR * sinP + temp * cosP;
	
	temp = 	 	-sinR * sinY;
	mat[0][1] =  sinR * cosY;
	mat[1][1] =  cosR * cosP + temp * sinP;
	mat[2][1] = -cosR * sinP + temp * cosP;

	mat[0][2] = sinY;
	mat[1][2] = cosY * sinP;
	mat[2][2] = cosY * cosP;
#else
	CopyUV(unituvs,mat);
	RollUV(pyr[2],mat);
	YawUV(pyr[1],mat);
	PitchUV(pyr[0],mat);
#endif	
}

#define NORMALQUAT 0
/* Does not require that q be normalized! */
/*   mat = 3x3 (or 4x3) */
void CreateQuatMat(F32 *mat, F32 q[], int normal)
{
	F32 s;
	F32 xs,	ys,	zs;
#if !NORMALQUAT
	if (!normal) {
		F32 Nq;
		Nq = q[0]*q[0] + q[1]*q[1] + q[2]*q[2] + q[3]*q[3];
		s = (Nq > 0.0) ? (2.0 / Nq) : 0.0;
	} else
		s = 2.0;
#else
	s = 2.0;
#endif
	xs = q[0]*s;
	ys = q[1]*s;
	zs = q[2]*s;
	
	{
		F32 xx = q[0]*xs;
		F32 yy = q[1]*ys;
		F32 zz = q[2]*zs;
		mat[0]= 1.0 - (yy + zz);
		mat[4]= 1.0 - (xx + zz);
		mat[8]= 1.0 - (xx + yy);
	}
	{
		F32 xy = q[0]*ys;
		F32 wz = q[3]*zs;
		mat[1]= -(xy + wz);
		mat[3]= -(xy - wz);
	}
	{
		F32 yz = q[1]*zs;
		F32 wx = q[3]*xs;
		mat[5]= -(yz + wx);
		mat[7]= -(yz - wx);
	}
	{
		F32 xz = q[0]*zs;
		F32 wy = q[3]*ys;
		mat[2]= xz - wy;
		mat[6]= xz + wy;
	}
}

void NormalizeQuat(F32 *q)
{
	F32 Nq,iNq;

	Nq = q[0]*q[0] + q[1]*q[1] + q[2]*q[2] + q[3]*q[3];
	iNq = 1.0 / fsqrt(Nq);

	q[0] *= iNq;
	q[1] *= iNq;
	q[2] *= iNq;
	q[3] *= iNq;
}

void InterpQuats(F32 frac, const F32 *q1, const F32 *q2, F32 *qr)
{
	F32 dp,theta;
	F32 sint,isint;
	F32 X1,X2;
	int i;

	if (frac < .01)
	{
		CopyQuat(q1,qr);
		return;
	}
	else if (frac > .99)
	{
		CopyQuat(q2,qr);
		return;
	}
	/* The delta between quats should be small enough that
	   spherical interpolation should not be required. */
#if 0
	dp = DotQuat(q1,q2) * .999;
	if (dp > 1.0)
		dp = 1.0;
	if (dp < -1.0)
		dp = -1.0;
	theta = facos(dp);
	sint = fsin(theta);
	if (sint < .01 && sint > -.01)
	{
		for (i=0; i<4; i++)
		{
			X1 = q2[i] - q1[i];
			if (X1 > 1.0)
				X1 -= 2.0;
			if (X1 < -1.0)
				X1 += 2.0;
			qr[i] = q1[i] + frac * (X1);
		}
	}
	else
	{
		isint = 1.0 / sint;
		X1 = fsin((1-frac)*theta) * isint;
		X2 = fsin(frac*theta) * isint;
		for (i=0; i<4; i++)
		{
			qr[i] = q1[i]*X1 + q2[i]*X2;
		}
	}
#else
	for (i=0; i<4; i++)
	{
		X1 = q2[i] - q1[i];
		qr[i] = q1[i] + frac * (X1);
		if (qr[i] > 1.0)
			qr[i] -= 2.0;
		else if (qr[i] < -1.0)
			qr[i] += 2.0;
	}
#if NORMALQUAT
	NormalizeQuat(qr);
#endif
#endif
}

/* unused */
void MulQuat(const F32 *q1, const F32 *q2, F32 *qr)
{
	F32 cp[3];
	F32 w1,w2,dp;
	int i;

	w1 = q1[3]; w2 = q2[3];
	dp = DotVector(q1,q2);
	CrossVector(q1,q2,cp);
	for (i=0; i<3; i++)
		qr[i] = w1*q2[i] + w2*q1[i] + cp[i];
	qr[3] = w1*w2 - dp;
}


/* Extracts RYP from a matrix where -180 < Y <= 180, -90 < P <= 90, and -180 <= R <= 180 */
void ExtractRYP(F32 mat[][3],F32 *pyr)
{
	F32 R,P,Y;
	F32 cosY,cosP;

	if (fabs(1.0 - fabs(mat[2][0])) <= .000001)
		/* Special case: Y = +- 90, R = 0 */
	{
		P = fatan2(-mat[1][2],mat[1][1]);
		Y = mat[2][0] > 0 ? RAD(90.0) : RAD(-90.0);
		R = 0;
	}
	else
	{
		P = fatan2(mat[2][1],mat[2][2]);
		cosP = fcos(P);
		if (cosP == 0.0) {
			Y = fatan2(mat[2][0], mat[2][1]);
			R = fatan2(mat[0][2],-mat[1][2]);			
		} else {
			cosY = mat[2][2] / cosP;
			Y = fatan2(mat[2][0],cosY);
			R = fatan2(mat[1][0] / cosY, mat[0][0] / cosY);
		}
	}
	pyr[0] = P;
	pyr[1] = Y;
	pyr[2] = R;
}



/**** Initialize the random number generator with a seed ****/
void Randomize(U32 seed) {
	srand((unsigned int)seed);
}

/**** Obtain a random number >= 0.0 and <= max ****/
F32 Random(F32 max) {
	F32 rannum;

	rannum = ( ((F32)(rand() & 0x07FFF)*max) / 32767.0);

	return(rannum);
}

int RandInt(int max)
{
#define A			(48271)
#define RAND_10K	(399268537)		/* value of seed at ten-thousandth iteration */
#define M			(2147483647)	/* 2**31-1 */
static U32	seed_less_one;
U32			hprod,lprod,result,seed = seed_less_one + 1;

	if (max <= 0) return 0;
    /* first do two 16x16 multiplies */
    hprod = ((seed>>16)&0xFFFF) * A;
    lprod = (seed&0xFFFF)*A;

    /* combine the products (suitably shifted) to form 48-bit product,
    *  with bottom 32 bits in lprod and top 16 bits in hprod.
    */
    hprod += ((lprod>>16) & 0xFFFF);
    lprod = (lprod&0xFFFF)|((hprod&0xFFFF)<<16);
    hprod >>= 16;

    /* now subtract the top 17 bits from the bottom 31 bits to implement
    *  a deferred "end-around carry".
    */
    hprod = hprod + hprod + ((lprod>>31)&1);
    lprod += hprod;

    /* final "and" gives modulo(2^31-1) */
    seed = lprod & 0x7FFFFFFF;
    result = (seed_less_one = seed - 1);

	return result % max;
}

F32 fqdist(F32 x, F32 y)
{
	F32 a,b,dist;
	if (x < 0.0)
		x = -x;
	if (y < 0.0)
		y = -y;

	/* Avoid underflow errors */
	if (x < .000001)
		return y;
	if (y < .000001)
		return x;
	
	if (x >= y)
		a = x, b = y;
	else
		a = y, b = x;
	
	if (b <= .5*a)
	{
		if (b <= .25*a)
		{
			if (b <= .125*a)
				dist = a + .064*b;
			else
				dist = a + .124*b;
		}
		else
		{
			if (b <= .375*a)
				dist = a + .181*b;
			else
				dist = a + .236*b;
		}
	}
	else
	{
		if (b <= .75*a)
		{
			if (b <= .625*a)
				dist = a + .287*b;
			else
				dist = a + .333*b;
		}
		else
		{
			if (b <= .875*a)
				dist = a + .376*b;
			else
				dist = a + .414*b;
		}
	}
	return dist;
}
