/*************************************************************************************************

Copyright 1994,1995 Time Warner Interactive.  Unauthorized reproduction, adaptation,
distribution, performance or display of this computer program or the associated
audiovisual work is strictly prohibited.
*************************************************************************************************/
#include "mb.h"
#include "model.h"
#include "trigtab.h"
#include "fmath.h"
#include "Pro/math.pro"
#include "Pro/mathsubs.pro"
/***************************************************************************
 This file was originally taken from Hard Drivin'
This file contains trigonometric functions which operate on fixed point
using only the hardware multiply and divide instructions for
efficiency. The values are scaled as follows:

Angles are represented by 16 bit signed integers.  The range
from -32768 to 32767 corresponds to angles from -pi to pi radians.
Note that since sine and cosine are naturally periodic, the sine
or cosine of a 32 bit angle is just determined by the sine or
cosine of the lower 16 bits of the value, if this scaling is used.

The output values from the sin and cosine functions range
from -0x4000 to 0x4000, representing values from -1.0 to 1.0.
Note that the largest absolute value is 4000 hex. This prevents overflow
if a 16 by 16 bit signed multiply instruction giving a 32 bit product
is used to multiply a 16 bit signed value by the sine of some angle.
The posatan function, on the other hand, takes an unsinged integer
from 0 to 32768, representing a value between 0 and 1.0

The square root routine takes an unsigned long integer (32 bits) as it's
argument and returns the largest 16 bit integer which, when squared,
is less than the argument.

These trig functions use 16 bit sine and arctangent tables produced
by a vax C program "mksintab.c" and residing in the file "trigtab.h".
The number of values in each table can be changed by running
mksintab and entering different values for each table.  It seems that
128 values of sine and 64 values of arctangent are sufficient to give
16 bits of resolution in sine and cosine. For better than one percent
error, you only need 8 values in the sine table! No modifications
to this file are needed to change the number of values in the tables.

Programmers:	Max Behensky
				Terry Farnham
Written:		6/85
Updates:		4/94
****************************************************************************/

/* This is the value equal to 1.0 */

#define MAXSINE 0x4000

/* These are the scale factors for each table (the number
* of numbers between each table value)*/
#define STABSCALE	(0x4000/NSINEVALUES)
#define ATABSCALE	(0x8000/NATANVALUES)
#define ASTABSCALE	(0x8000/NASINEVALUES)

#define STAB32SCL	0x400000L
#define ASTAB32SCL	0x400000L


/** This routine returns the sine of an angle scaled as described above **/

S16 ssin(U16 angle) {
	switch(angle & 0xc000) {
		case 0:								/*quadrant 1, 0 <= x < pi/2*/
			return(possin(angle));

		case 0x4000:						/*quadrant 2, pi/2 <= x < pi */
			return(possin(0x8000 - angle));

		case 0x8000:						/*quadrant 3, -pi <= x < -pi/2 */
			return(-possin(0x8000 + angle));

		case 0xc000:						/*quadrant 4,	-pi/2 <= x < 0 */
			return(-possin(-angle));
	}
	return(0); /*Default */
}



/** This routine returns the cosine of an angle scaled as described above. **/
S16 scos(U16 angle) {
	return(ssin(0x4000 + angle));	/* cos(x) = sin(90+x) */
}


/** This routine calculates the sine and cosine of an angle scaled as described above. **/
void sincos(U16 angle, S16 *sina, S16 *cosa) {
	*sina = ssin(angle);
	*cosa = ssin(0x4000 + angle);	/* cos(x) = sin(90+x) */
}


/***** This routine calculates the arctangent of y/x for any y and x *****/
S16 satan(S16 x,S16 y) {
	if(x < 0){
		if(y < 0)
			return(0x8000 + posdir(-x,-y));
		return(0x8000 - posdir(-x,y));
	}

	if(y < 0)
		return(-posdir(x,-y));

	return(posdir(x,y));
}

S32 ssin32(U32 angle) {
	switch(angle & 0xc0000000) {
		case 0:						/*quadrant 1, 0 <= x < pi/2*/
			return(possin32(angle));

		case 0x40000000:				/*quadrant 2, pi/2 <= x < pi */
			return(possin32(0x80000000 - angle));

		case 0x80000000:				/*quadrant 3, -pi <= x < -pi/2 */
			return(-possin32(0x80000000 + angle));

		case 0xc0000000:				/*quadrant 4,	-pi/2 <= x < 0 */
			return(-possin32(-angle));
    	}
	return(0);
}

S32 scos32(U32 angle) {
	return(ssin32(0x40000000 + angle));	/* cos(x) = sin(90+x) */
}

/* This is the routine which actually calculates the sine of an
 * angle using linear interpolation from the sine table. It
 * takes a positive angle between 0 and pi/2 and returns its
 * sine.														*/

	
S16 possin(U16 angle) {
	U16 a,b;
	S16 sina,delta;

	a=angle/STABSCALE;
	b=a+1;
	sina=sintab[a];
	delta = ((S16)(sintab[b] - sina) * (S16)(angle & (STABSCALE-1)))/(S16)STABSCALE;
	
	return(sina+delta);
}

S32 possin32(U32 angle) {
	U32 a,b;
	S32 sina,delta;

	a=((S32)angle)/((S32)STAB32SCL);
	b=a+1;
	sina=sintab[a]<<16;
	delta = INTRP32((S32)((sintab[b]<<16) - sina),(S32)(angle & (STAB32SCL-1)),(S32)STAB32SCL);
	return(sina+delta);
}



/* This routine gives the arc tangent of a value between 0-32768
 * representing a number between zero and 1.0 as an angle
 * between 0 and 8192 representing 0 to 45 degrees.
 * Note that for this function, 1.0=32768, whereas for sine and
 * cosine 1.0=32767												*/

static U16 posatan(U16 value) {
	U16 a,b,atana,delta;

	if(value >= 0x8000)
		return(0x2000);

	a=value/ATABSCALE;
	b=a+1;
	atana=atantab[a];
	delta = ((S16)(atantab[b] - atana) * (S16)(value & (ATABSCALE - 1)))/(S16)ATABSCALE;

	return(atana + delta);
}

/*static */
S16 sasin(U16 value) {
	U16 a,b,asina,delta;

	value += 0x4000;

	a=value/ASTABSCALE;
	b=a+1;
	asina=asintab[a];
	delta = ((S16)(asintab[b] - asina) * (S16)(value & (ASTABSCALE - 1)))/(S16)ASTABSCALE;

	return(-(asina + delta));
}

static S32 sasin32(U32 value) {
	U32 a,b,asina,delta;

	value += 0x40000000;

	a=((S32)value)/((S32)ASTAB32SCL);
	b=a+1;
	asina=asintab[a]<<16;
	delta = INTRP32((S32)((asintab[b]<<16) - asina),(S32)(value & (ASTAB32SCL-1)),(S32)ASTAB32SCL);

	return(-(asina + delta));
}

/* This routine calculates the arctangent of y/x where x and y
* are both positive*/

U16 posdir(U16 x,U16 y) {
	U16 fract;

	if(!x && !y) return(0);	/*special case 0,0*/
	if(x > y){
		fract = (U16)((unsigned)(0x8000 * y) / (U16)x);
		return(posatan(fract));
	}
	fract = (U16)((unsigned)(0x8000 * x) / (U16)y);
	return(0x4000 - posatan(fract));
} 



/* rotate routines */

const S16 unituvs16[3][3]={{0x4000,0,0},{0,0x4000,0},{0,0,0x4000}};

/***** yaw in universe axis *****/
void uyaw(S16 sint,S16 cost,S16 uv[][3]) {
	S16 ut,i; 

	for(i=0;i<3;i++) { 
		ut	 = MUL(uv[0][i],cost) - MUL(uv[2][i],sint);
		uv[2][i] = MUL(uv[2][i],cost) + MUL(uv[0][i],sint);
		uv[0][i] = ut; 
	} 
} 

/***** pitch unit vector using sint & cost *****/
void upitch(S16 sint,S16 cost,S16 uv[][3]) {
	S16 ut,i; 

	for(i=0;i<3;i++) { 
		ut	 = MUL(uv[1][i],cost) - MUL(uv[2][i],sint);
		uv[2][i] = MUL(uv[2][i],cost) + MUL(uv[1][i],sint);
		uv[1][i] = ut; 
	} 
} 


/***** roll unit vector using sint & cost *****/
void uroll(S16 sint,S16 cost,S16 uv[][3]) {
	S16 ut,i;

	for(i=0;i<3;i++) { 
		ut	 = MUL(uv[0][i],cost) - MUL(uv[1][i],sint);
		uv[1][i] = MUL(uv[1][i],cost) + MUL(uv[0][i],sint);
		uv[0][i] = ut; 
	} 
} 


/***** yaw unit vector using sint & cost *****/
void yaw(S16 sint,S16 cost,S16 uv[][3]) {
	S16 ut,i; 

	for(i=0;i<3;i++) {
		ut	 = MUL(uv[i][0],cost) - MUL(uv[i][2],sint);
		uv[i][2] = MUL(uv[i][2],cost) + MUL(uv[i][0],sint);
		uv[i][0] = ut; 
	} 
}

/***** yaw unit vector using sint & cost *****/
void yaw32(S32 sint,S32 cost,S32 uv[][3]) {
	S32 ut,i; 

	for(i=0;i<3;i++) {
		ut	 = MULUV32(uv[i][0],cost) - MULUV32(uv[i][2],sint);
		uv[i][2] = MULUV32(uv[i][2],cost) + MULUV32(uv[i][0],sint);
		uv[i][0] = ut; 
	} 
}


/***** pitch unit vector using sint & cost *****/
void pitch(S16 sint,S16 cost,S16 uv[][3]) {
	S16 ut,i; 

	for(i=0;i<3;i++) { 
		ut	 = MUL(uv[i][1],cost) - MUL(uv[i][2],sint);
		uv[i][2] = MUL(uv[i][2],cost) + MUL(uv[i][1],sint);
		uv[i][1] = ut; 
	} 
} 

/***** pitch unit vector using sint & cost *****/
void pitch32(S32 sint,S32 cost,S32 uv[][3]) {
	S32 ut,i; 

	for(i=0;i<3;i++) { 
		ut	 = MULUV32(uv[i][1],cost) - MULUV32(uv[i][2],sint);
		uv[i][2] = MULUV32(uv[i][2],cost) + MULUV32(uv[i][1],sint);
		uv[i][1] = ut; 
	} 
} 


/***** roll unit vector using sint & cost *****/
void roll(S16 sint,S16 cost,S16 uv[][3]) {
	S16 ut,i;

	for(i=0;i<3;i++) { 
		ut	 = MUL(uv[i][0],cost) - MUL(uv[i][1],sint);
		uv[i][1] = MUL(uv[i][1],cost) + MUL(uv[i][0],sint);
		uv[i][0] = ut; 
	} 
} 

/***** roll unit vector using sint & cost *****/
void roll32(S32 sint,S32 cost,S32 uv[][3]) {
	S32 ut,i;

	for(i=0;i<3;i++) { 
		ut	 = MULUV32(uv[i][0],cost) - MULUV32(uv[i][1],sint);
		uv[i][1] = MULUV32(uv[i][1],cost) + MULUV32(uv[i][0],sint);
		uv[i][0] = ut; 
	} 
} 


/***** Performs a long Square Root *****/
U32 lsqrt(U32 value) {
#if 1
        float fvalue,fresult;
	S32 result;
	
	fvalue = (float)(value);
	fresult = fsqrt(value);
	result = (S32)(fresult);
#else        
	U32 squarebit,remainder,restimes;
	U16 resbit,result;

	restimes=0xfffffff0;
	squarebit=0x4;
	resbit=0x2;
	remainder=value;
	if(remainder < 4)
		return(remainder);

	while(remainder & restimes) {
		restimes <<= 2;
		squarebit <<= 2;
		resbit <<= 1;
	}

	result = resbit;
	restimes = squarebit;
	remainder -= squarebit;
	resbit >>= 1;	
	squarebit >>= 2;

	while(resbit) {
		if(remainder >= restimes + squarebit) {
			result |= resbit;
			remainder -= restimes + squarebit;
			restimes += squarebit<<1;
		}
		resbit >>= 1;
		squarebit >>= 2;
		restimes >>= 1;
	}
#endif
	return(result);
}



	
/*
*   Derive the roll, pitch, and yaw from a unit vector.
*
*   Positive angle is clockwise, looking down the angle of rotation
*/
void GetEuler( S16 uv[][3], S16 rpy[] )
{
	register S16 x,y,z,a;
	S16 temp[3][3];

	COPYUV( uv, temp );

	/* find yaw and undo */
	x = temp[0][2];		/* opposite side */
	z = temp[2][2];		/* adjacent side */
	a = satan(z,x);
	uyaw( ssin(a), scos(a), temp );
	rpy[2] = a;

	/* find pitch and undo */
	z = temp[2][2];		/* opposite side */
 	y = temp[1][2];		/* adjacent side */
	a = satan(z,y);
	upitch( ssin(a), scos(-a), temp );
	rpy[1] = -a;

	/*  find roll */
	y = temp[1][0];		/* opposite side */
	x = temp[0][0];		/* adjacent side */
	a = satan(x,y);
	uroll( ssin(-a), scos(a), temp );
	rpy[0] = a;


}
    /* For now, to be able to use for 32-bit values (even though 
   accuracy is still 16-bit) */
   
void GetEuler32( S32 uv[][3], S32 rpy[] )
{
    S16 uv16[3][3],rpy16[3];
    U32 i,j;
    
    for (i = 0;i < 3; i++) {
	for (j = 0; j < 3; j++) {
	    uv16[i][j] = (S16)(uv[i][j]>>16);
	}
    }    
    GetEuler(uv16,rpy16);
    for (i = 0;i < 3; i++) {
	rpy[i] = (S32)(rpy16[i]<<16);
    }
}

/* This function is needed since Zoid uvs are transformed in
   the order yaw, -pitch, -roll (as defined by our x,y,z 
   right-handed coordinate axes 
*/
   

void GetZoidEuler( S32 uv[][3], S32 rpy[] )
{
    S16 uv16[3][3],rpy16[3];
    U32 i,j;
    register S16 x,y,z,a;
    S16 temp[3][3];
    
    for (i = 0;i < 3; i++) {
	for (j = 0; j < 3; j++) {
	    uv16[i][j] = (S16)(uv[i][j]>>16);
	}
    }    

    COPYUV( uv16, temp );

    /* find -roll and undo */
    x = temp[0][1];		/* opposite side */
    z = temp[0][0];		/* adjacent side */
    if (abs(temp[1][0]) < 0x10) 
        a = 0;
    else
        a = satan(z,x);    /* get atan of (x/z) */
    uroll( ssin(a), scos(a), temp );
    rpy16[0] = a;

    /* find -pitch and undo */
    y = -temp[1][2];		/* adjacent side */
    z = temp[1][1];		/* opposite side */
    if (abs(temp[2][1]) < 0x10) 
        a = 0;
    else
        a = satan(z,y);            /* Get atan of (y/z) */
    upitch( ssin(a), scos(a), temp );
    rpy16[1] = -a;

    /*  find yaw */
    y = -temp[2][0];		/* opposite side */
    x = temp[0][0];		/* adjacent side */
    a = satan(x,y);
    rpy16[2] = a;

    for (i = 0;i < 3; i++) {
	rpy[i] = (S32)(rpy16[i]<<16);
    }
}

/****************************************************************
 **** The following code operates on 3 different data types. ****
 ****   1.) Signed 16 bit data                         "16"  ****
 ****   2.) Signed 32 bit data                         "32"  ****
 ****   3.) Signed 16 bit data in format 1.0 = 0x4000  "14"  ****
 ****************************************************************/


S16 DotProduct14(S16 v1[3], S16 v2[3]) {
	S16 res;

	res = MUL(v1[0],v2[0]) + MUL(v1[1],v2[1]) + MUL(v1[2],v2[2]);

	return(res);
}


S32 DotProduct16(S16 v1[3], S16 v2[3]) {
	S32 res;

	res = v1[0]*v2[0] + v1[1]*v2[1] + v1[2]*v2[2];

	return(res);
}


S32 DotProduct32(S32 v1[3], S32 v2[3]) {
	S32 res, mult[3];

        res = MULUV32(v1[0],v2[0]) + MULUV32(v1[1],v2[1]) + 
	      MULUV32(v1[2],v2[2]);

	return(res);
}


void CrossProduct14(S16 v1[3], S16 v2[3], S16 res[3]) {
	res[0] = MUL(v1[1],v2[2]) - MUL(v1[2],v2[1]);
	res[1] = MUL(v1[2],v2[0]) - MUL(v1[0],v2[2]);
	res[2] = MUL(v1[0],v2[1]) - MUL(v1[1],v2[0]);
}


void CrossProduct16(S16 v1[3], S16 v2[3], S16 res[3]) {
	res[0] = (v1[1] * v2[2]) - (v1[2] * v2[1]); 
	res[1] = (v1[2] * v2[0]) - (v1[0] * v2[2]); 
	res[2] = (v1[0] * v2[1]) - (v1[1] * v2[0]);
}


void CrossProduct32(S32 v1[3], S32 v2[3], S32 res[3]) {
	res[0] = (v1[1] * v2[2]) - (v1[2] * v2[1]); 
	res[1] = (v1[2] * v2[0]) - (v1[0] * v2[2]); 
	res[2] = (v1[0] * v2[1]) - (v1[1] * v2[0]);
}


void SubVector16(S16 v1[3], S16 v2[3], S16 res[3]) {
	res[0] = v1[0] - v2[0];
	res[1] = v1[1] - v2[1];
	res[2] = v1[2] - v2[2];
}


void SubVector32(S32 v1[3], S32 v2[3], S32 res[3]) {
	res[0] = v1[0] - v2[0];
	res[1] = v1[1] - v2[1];
	res[2] = v1[2] - v2[2];
}


void AddVector16(S16 v1[3], S16 v2[3], S16 res[3]) {
	res[0] = v1[0] + v2[0];
	res[1] = v1[1] + v2[1];
	res[2] = v1[2] + v2[2];
}


void AddVector32(S32 v1[3], S32 v2[3], S32 res[3]) {
	res[0] = v1[0] + v2[0];
	res[1] = v1[1] + v2[1];
	res[2] = v1[2] + v2[2];
}


void ScaleVector16(S16 v[3], S16 s) {
	v[0] *= s;
	v[1] *= s;
	v[2] *= s;
}


void ScaleVector32(S32 v[3], S32 s) {
	v[0] *= s;
	v[1] *= s;
	v[2] *= s;
}


/***
**
**	Functions to support 32bit unit vectors
**
**	Brian Dodd - 6/22/94
**
***/
void Concat32( S32 *a, S32 *b, S32 *c )
{
    *c++ = MULUV32(a[0],b[0])+MULUV32(a[1],b[3])+MULUV32(a[2],b[6]);
    *c++ = MULUV32(a[0],b[1])+MULUV32(a[1],b[4])+MULUV32(a[2],b[7]);
    *c++ = MULUV32(a[0],b[2])+MULUV32(a[1],b[5])+MULUV32(a[2],b[8]);
    *c++ = MULUV32(a[3],b[0])+MULUV32(a[4],b[3])+MULUV32(a[5],b[6]);
    *c++ = MULUV32(a[3],b[1])+MULUV32(a[4],b[4])+MULUV32(a[5],b[7]);
    *c++ = MULUV32(a[3],b[2])+MULUV32(a[4],b[5])+MULUV32(a[5],b[8]);
    *c++ = MULUV32(a[6],b[0])+MULUV32(a[7],b[3])+MULUV32(a[8],b[6]);
    *c++ = MULUV32(a[6],b[1])+MULUV32(a[7],b[4])+MULUV32(a[8],b[7]);
    *c++ = MULUV32(a[6],b[2])+MULUV32(a[7],b[5])+MULUV32(a[8],b[8]);
}

S32 DotUV32( S32 v1[3], S32 v2[3] )
{
    return(MULUV32(v1[0],v2[0])+MULUV32(v1[1],v2[1])+MULUV32(v1[2],v2[2]));
}

S16 GetQuadrant(S16 x, S16 y, S16 z) {
	if(y >= 0) {
		if(x >= 0) {
			if(z >= 0)
				return(0);
			else
				return(1);
		} else {
			if(z >= 0)
				return(3);
			else
				return(2);
		}
	} else {
		if(x >= 0) {
			if(z >= 0)
				return(4);
			else
				return(5);
		} else {
			if(z >= 0)
				return(6);
			else
				return(7);
		}
	}
}

const S32 unituvs32[3][3]={{0x40000000,0,0},{0,0x40000000,0},{0,0,0x40000000}};
 
/***** yaw in universe axis *****/
void uyaw32(S32 sint,S32 cost,S32 uv[][3]) {
	S32 ut,i; 

	for(i=0;i<3;i++) { 
		ut	 = MULUV32(uv[0][i],cost) - MULUV32(uv[2][i],sint);
		uv[2][i] = MULUV32(uv[2][i],cost) + MULUV32(uv[0][i],sint);
		uv[0][i] = ut; 
	} 
} 


/***** pitch unit vector using sint & cost *****/
void upitch32(S32 sint,S32 cost,S32 uv[][3]) {
	S32 ut,i; 

	for(i=0;i<3;i++) { 
		ut	 = MULUV32(uv[1][i],cost) - MULUV32(uv[2][i],sint);
		uv[2][i] = MULUV32(uv[2][i],cost) + MULUV32(uv[1][i],sint);
		uv[1][i] = ut; 
	} 
} 


/***** roll unit vector using sint & cost *****/
void uroll32(S32 sint,S32 cost,S32 uv[][3]) {
	S32 ut,i;

	for(i=0;i<3;i++) { 
		ut	 = MULUV32(uv[0][i],cost) - MULUV32(uv[1][i],sint);
		uv[1][i] = MULUV32(uv[1][i],cost) + MULUV32(uv[0][i],sint);
		uv[0][i] = ut; 
	} 
} 

/* This routine gives the arc tangent of a value between 0-32768
 * representing a number between zero and 1.0 as an angle
 * between 0 and 8192 representing 0 to 45 degrees.
 * Note that for this function, 1.0=32768, whereas for sine and
 * cosine 1.0=32767												*/

static U32 posatan32(U32 ivalue) {
	U16 value,a,b,atana,delta;

    	value = (U16)(ivalue>>16);
	if(value >= 0x8000)
		return(0x20000000);

	a=value/ATABSCALE;
	b=a+1;
	atana=atantab[a];
	delta = ((S16)(atantab[b] - atana) * (S16)(value & (ATABSCALE - 1)))/(S16)ATABSCALE;

	return((U32)((atana + delta)<<16));
}

/* This routine calculates the arctangent of y/x where x and y
* are both positive*/

U32 posdir32(U32 x,U32 y) {
	U32 fract;

	if(!x && !y) return(0);	/*special case 0,0*/
	if(x > y){
		fract = (U32)((unsigned)(0x80000000 * y) / (U32)x);
		return(posatan32(fract));
	}
	fract = (U32)((unsigned)(0x80000000 * x) / (U32)y);
	return(0x40000000 - posatan32(fract));
} 

/***** This routine calculates the arctangent of y/x for any y and x *****/
S32 satan32(S32 x,S32 y) {
        S16 tx, ty, result;
	S32 res2;
	
	tx = (x>>16);
	ty = (y>>16);
        result = satan(tx,ty);
	res2 = result<<16;
	return(res2); 
#if 0   
	if(x < 0){
		if(y < 0)
			return(0x80000000 + posdir32(-x,-y));
		return(0x80000000 - posdir32(-x,y));
	}

	if(y < 0)
		return(-posdir32(x,-y));

	return(posdir32(x,y));
#endif
}


void Get32Euler( S32 uv[3][3], S32 rpy[] )
{
	register S32 x,y,z,a;
	S32 temp[3][3];

 	COPYUV32( uv, temp );

	/* find yaw and undo */
	x = temp[0][2];		/* opposite side */
	z = temp[2][2];		/* adjacent side */
	a = satan32(z,x);
	uyaw32( ssin32(a), scos32(a), temp );
	rpy[2] = a;

	/* find pitch and undo */
	z = temp[2][2];		/* opposite side */
	y = temp[1][2];		/* adjacent side */
	a = satan32(z,y);
	upitch32( ssin32(a), scos32(a), temp );
	rpy[1] = -a;

	/* finally, find roll */
	y = temp[1][0];		/* opposite side */
	x = temp[0][0];		/* adjacent side */
	rpy[0] = satan32(x,y);
}

void Get32UVS( S32 rpy[], S32 uv[3][3] )
{
    MOVMEM((char *)(unituvs32),(char *)(uv),36);
    yaw32(ssin32(rpy[2]),scos32(rpy[2]),uv);
    pitch32(ssin32(rpy[1]),scos32(rpy[1]),uv);
    roll32(ssin32(rpy[0]),scos32(rpy[0]),uv);
}

void GetZoid32UVS( S32 rpy[], S32 uv[3][3] )
{
    MOVMEM((char *)(unituvs32),(char *)(uv),36);
    yaw32(ssin32(rpy[2]),scos32(rpy[2]),uv);
    pitch32(ssin32(-rpy[1]),scos32(-rpy[1]),uv);
    roll32(ssin32(-rpy[0]),scos32(-rpy[0]),uv);
}

void GetUVS( S16 rpy[3], S16 uv[3][3] )
{
    MOVMEM((char *)(unituvs16),(char *)(uv),18);
    yaw(ssin(rpy[2]),scos(rpy[2]),uv);
    pitch(ssin(rpy[1]),scos(rpy[1]),uv);
    roll(ssin(rpy[0]),scos(rpy[0]),uv);
}

void GetUnivZoidUVS(S32 zrpy[3], S16 uv[3][3]) {
    S16 rpy[3];
    int i;
    
    for (i = 0; i < 3; i++)
        rpy[i] = CvtAng16(zrpy[i]);
    MOVMEM((char *)(unituvs16),(char *)(uv),18);
    yaw(ssin(rpy[2]),scos(rpy[2]),uv);
    pitch(ssin(rpy[1]),scos(rpy[1]),uv);
    roll(ssin(rpy[0]),scos(rpy[0]),uv);
}

void GetZoidUVS(S32 zrpy[3], S16 uv[3][3]) {
    S16 rpy[3];
    int i;
    
    for (i = 0; i < 3; i++)
        rpy[i] = CvtAng16(zrpy[i]);
    MOVMEM((char *)(unituvs16),(char *)(uv),18);
    yaw(ssin(rpy[2]),scos(rpy[2]),uv);
    pitch(ssin(-rpy[1]),scos(-rpy[1]),uv);
    roll(ssin(-rpy[0]),scos(-rpy[0]),uv);
}

void GetZoidUVS16(S16 rpy[3], S16 uv[3][3]) 
{
    S16 tuvs[3][3];
    
    MOVMEM((char *)(unituvs16),(char *)(tuvs),18);
    yaw(ssin(rpy[2]),scos(rpy[2]),tuvs);
    pitch(ssin(-rpy[1]),scos(-rpy[1]),tuvs);
    roll(ssin(-rpy[0]),scos(-rpy[0]),tuvs);
    MOVMEM((char *)(tuvs),(char *)(uv),18);
}

void UndoZoidUVS(S16 rpy[3], S16 uv[3][3]) {
    register S16 x,y,z,a;
    S16 temp[3][3];

    MOVMEM((char *)(uv),(char *)(temp),18);

    y = -temp[1][0];		/* opposite side */
    x = -temp[1][1];		/* adjacent side */
    a = satan(x,y);
    /* Undo roll by rolling back -(-roll), which is just roll! */ 
    roll( ssin(a), scos(a), temp );
    rpy[0] = a;

    /* find pitch and undo */
    z = temp[1][1];		/* opposite side */
    y = -temp[1][2];		/* adjacent side */
    a = (-satan(z,y));
    pitch( ssin(a), scos(a), temp );
    rpy[1] = a;

    /* find yaw and undo */
    x = temp[0][2];		/* opposite side */
    z = temp[2][2];		/* adjacent side */
    a = satan(z,x);
    rpy[2] = 0x8000 - a;
}

void GetZoidLongUVS(S32 zrpy[3], S32 uv[3][3]) {
    S32 rpy[3];
    int i;

    MOVMEM((char *)(unituvs32),(char *)(uv),36);    
    yaw32(ssin32(rpy[2]),scos32(rpy[2]),uv);
    pitch32(ssin32(-rpy[1]),scos32(-rpy[1]),uv);
    roll32(ssin32(-rpy[0]),scos32(-rpy[0]),uv);
}

void ConvLongUVS(S32 *uv32, S16 *uv16)
{ 
    int i,j;
    S16 *sptr;
    S32 *lptr;
    
    lptr = (S32 *)(uv32);
    sptr = (S16 *)(uv16);
    for ( i = 0;i < 9; i++) {
	*lptr++ = ((*sptr++)<<16);
    }
}
	    
void Conv16UVS(S32 uv32[3][3], S16 uv16[3][3])
{ 
    int i,j;
    
    for ( i = 0;i < 3; i++) {
        for ( j = 0; j < 3; j++)
            uv16[i][j] = (S16)(uv32[i][j]>>16);
    }
}

void ConvU16UVS(U32 uv32[3][3], S16 uv16[3][3])
{ 
    int i,j;
    
    for ( i = 0;i < 3; i++) {
        for ( j = 0; j < 3; j++)
            uv16[i][j] = (S16)(uv32[i][j]>>16);
    }
}

/* Get a delta universe-coordinates vector for an offset from 
   object's body coordinates x-axis (left-right) */
   	    
void adjust_x(S32 uv[3][3], S32 xmove, S32 movevec[3])
{
    movevec[0]= MULUV32(xmove,uv[0][0]);
    movevec[1]= MULUV32(xmove,uv[1][0]);
    movevec[2]= MULUV32(xmove,uv[2][0]);
}
			
 
/* Get a delta universe-coordinates vector for an offset from 
   object's body coordinates y-axis (up-down) */
   	    
void adjust_y(S32 uv[3][3], S32 ymove, S32 movevec[3])
{
    movevec[0]= MULUV32(ymove,uv[0][1]);
    movevec[1]= MULUV32(ymove,uv[1][1]);
    movevec[2]= MULUV32(ymove,uv[2][1]);
}
			
/* Get a delta universe-coordinates vector for an offset from 
   object's body coordinates z-axis (front-back) */

void adjust_z(S32 uv[3][3], S32 zmove, S32 movevec[3])
{
    movevec[0]= MULUV32(zmove,uv[0][2]);
    movevec[1]= MULUV32(zmove,uv[1][2]);
    movevec[2]= MULUV32(zmove,uv[2][2]);
}

/* A routine to add angles for zoid scaling (0x02000000 = pi) */ 
U32 AddAngles(S32 angle, S32 delta)
{
    S32 result;
    S16 a,d,r;

    a = (S16)(angle >> 10);
    d = (S16)(delta >> 10);
    r = a + d;
    result = (U32)((r <<10) & 0x03ffffff);
    return(result);    
}    

/* Convert from signed 16-bit angle format to 32-bit format
   where 0x0200 0000 = 180 degrees */
S32 CvtAng32(S16 angle)
{
    return((S32)((angle<<10)&0x03ffffff));
}    

/* Convert from ZOID format to signed 16-bit angle */
S16 CvtAng16(S32 angle)
{
    return((S16)((angle&0x03ffffff)>>10));
}    


void LookInDirection(short LookDir[3], short uvs[3][3]) {
   unsigned short L;
   short ZVec[3],XVec[3];
   
   L = (unsigned short)lsqrt((unsigned long)((LookDir[0]*LookDir[0])+
					     (LookDir[1]*LookDir[1])+
					     (LookDir[2]*LookDir[2])));
   if(L <= 0)
	   L = 1;
   /* Forward vector */
   ZVec[0] = uvs[0][2] = (short)((LookDir[0]<<14)/L);
   ZVec[1] = uvs[1][2] = (short)((LookDir[1]<<14)/L);
   ZVec[2] = uvs[2][2] = (short)((LookDir[2]<<14)/L);
   
   L= (unsigned short)lsqrt((unsigned long)((LookDir[0]*LookDir[0]+
				   LookDir[2]*LookDir[2])));
   /* */
   if(L != 0) {
      XVec[0] = uvs[0][0] = (short)((LookDir[2]<<14)/L);
      XVec[1] = uvs[1][0] = 0;
      XVec[2] = uvs[2][0] = (short)(-(LookDir[0]<<14)/L);
   } else {
      XVec[0] = uvs[0][0] = 0x4000;
      XVec[1] = uvs[1][0] = 0;
      XVec[2] = uvs[2][0] = 0;
   }
   /* Take cross product to get Y-vect */ 
   uvs[0][1] = MUL(ZVec[1],XVec[2]) - MUL(ZVec[2],XVec[1]);
   uvs[1][1] = MUL(ZVec[2],XVec[0]) - MUL(ZVec[0],XVec[2]);
   uvs[2][1] = MUL(ZVec[0],XVec[1]) - MUL(ZVec[1],XVec[0]);
}

void DistPtOnLine(S32 pt[3], S32 from[3], S32 to[3], S32 *zdist, S32 *xdist) {
    S32 diff[3], diffpt[3], xd, yd, zd, dst1, dflen;
    U16 err_cnt;
    U32 len;
    int i;
    
    for ( i= 0; i < 3; i++) 
        diff[i] = (to[i]-from[i])>>10;  /* convert to feet so overflow doesn't occur */
    len = lsqrt((unsigned long)((diff[0]*diff[0]) + (diff[1]*diff[1]) + (diff[2]*diff[2])));
    for ( i= 0; i < 3; i++)
        diff[i] = (diff[i] << 16) / len;  /* Normalize differences (0x10000 = 1) */
    for ( i= 0; i < 3; i++) {
        diffpt[i] = pt[i] - from[i];
        if (abs(diffpt[i]) > 0x07fff) err_cnt++;
    }
    xd = INTRP32(diff[0], diffpt[0], 0x10000);
    yd = INTRP32(diff[1], diffpt[1], 0x10000);
    zd = INTRP32(diff[2], diffpt[2], 0x10000);
    if (abs(xd) > 0x07fff) err_cnt++;
    if (abs(yd) > 0x07fff) err_cnt++;
    if (abs(zd) > 0x07fff) err_cnt++;
    *zdist = dst1 = xd + yd + zd;
    dflen = diffpt[0]*diffpt[0] + diffpt[1]*diffpt[1] + diffpt[2]*diffpt[2];
    if (abs(dflen) > 0x07fff) err_cnt++;
    *xdist = lsqrt(dflen -(dst1*dst1));
}

/* Scale up a direction vector */
void ScaleUp(S16 original[3], S16 result[3]) {
    int done, loop_cnt, neg[3],i;
    
    done = loop_cnt = 0;
    memcpy(result, original, 6);
    for (i = 0; i < 3; i++) {
        if (result[i] < 0) 
            neg[i] = 1;
        else
            neg[i] = 0;
    }
    while ((!done) && (loop_cnt < 14)) {
        if ( ((abs(result[0]) & 0xC000) == 0) && ((abs(result[1]) & 0xC000) == 0) &&
	    ((abs(result[2]) & 0xC000) == 0) ) {
            result[0] = (abs(result[0])) << 1;  
            result[1] = (abs(result[1])) << 1;
            result[2] = (abs(result[2])) << 1;
	    loop_cnt++;
        } else 
            done = 1;
    }
    for (i = 0; i < 3; i++) {
	if (neg[i] != 0) 
	    result[i] = -result[i];    
    }	  
}

void ScaleDown(S32 original[3], S16 result[3]) {
    int done, loop_cnt, i;
    S32 orig_copy[3];
    
    done = loop_cnt = 0;
    memcpy(orig_copy, original, 12);
    
    while ((!done) && (loop_cnt < 16)) {
        if ( ((abs(orig_copy[0]) & 0xFFFF8000) != 0) || ((abs(orig_copy[1]) & 0xFFFF8000) != 0) 
            || ((abs(orig_copy[2]) & 0xFFFF8000) != 0) ) {
            orig_copy[0] >>= 1;  
            orig_copy[1] >>= 1;
            orig_copy[2] >>= 1;
	    loop_cnt++;
        } else 
            done = 1;
    }
    for (i = 0; i < 3; i++) {
        result[i] = (S16)(orig_copy[i]);
    }	  
}

void GetEulerAngles( S32 LookDir[3], S16 rpy[] )
{
	S16 x,y,z,a;

	x = (S16)(LookDir[0]>>10);
    	y = (S16)(LookDir[1]>>10);
	z = (S16)(LookDir[2]>>10);
	
        /* Get roll */
	rpy[0] = satan(x,y);

	/* Get pitch */
	a = (satan(y,z));
	rpy[1] = -a + 0x4000; /* Offset by 90deg */

	/* Get yaw */
	a = satan(z,x);
	rpy[2] = a;
}

void MakeUVS(short LookDir[3], short uvs[3][3]) {
   short Temp1[3],Temp2[3];
   unsigned short L;
   short N[3];
   
   LookDir[1] = 0;
   L = (unsigned short)lsqrt((unsigned long)((LookDir[0]*LookDir[0])+
					     (LookDir[1]*LookDir[1])+
					     (LookDir[2]*LookDir[2])));
   if(L <= 0)
	   L = 1;
   /* Forward vector */
   uvs[2][0] = N[0] = (short)((LookDir[0]<<14)/L);
   uvs[2][1] = N[1] = (short)((LookDir[1]<<14)/L);
   uvs[2][2] = N[2] = (short)((LookDir[2]<<14)/L);
   /* The Y-up vector */
   uvs[1][0] = Temp1[0] = 0;
   uvs[1][1] = Temp1[1] = 0x4000;
   uvs[1][2] = Temp1[2] = 0;
   /* Right vector */
   CrossProduct14(Temp1,N,Temp2);
   uvs[0][0] = Temp2[0];
   uvs[0][1] = Temp2[1];
   uvs[0][2] = Temp2[2];
}
 
void Transvec(S32 vec[3], S32 res[3], S16 uv[3][3]) {
    S32 uv32[3][3];

    ConvLongUVS((S32 *)(uv32), (S16 *)(uv));
    res[0] = MULUV32(vec[0],uv32[0][0]) + MULUV32(vec[1],uv32[0][1]) + MULUV32(vec[2],uv32[0][2]);  
    res[1] = MULUV32(vec[0],uv32[1][0]) + MULUV32(vec[1],uv32[1][1]) + MULUV32(vec[2],uv32[1][2]);  
    res[2] = MULUV32(vec[0],uv32[2][0]) + MULUV32(vec[1],uv32[2][1]) + MULUV32(vec[2],uv32[2][2]);  
}

#if 0
void UTransvec(S32 vec[3], S32 res[3], S16 uv[3][3]) {
#else
void UTransvec(S32 *vec, S32 *res, S16 uv[3][3]) {
#endif
    S32 uv32[3][3];

    ConvLongUVS((S32 *)(uv32), (S16 *)(uv));
    res[0] = MULUV32(vec[0],uv32[0][0]) + MULUV32(vec[1],uv32[1][0]) + MULUV32(vec[2],uv32[2][0]);  
    res[1] = MULUV32(vec[0],uv32[0][1]) + MULUV32(vec[1],uv32[1][1]) + MULUV32(vec[2],uv32[2][1]);  
    res[2] = MULUV32(vec[0],uv32[0][2]) + MULUV32(vec[1],uv32[1][2]) + MULUV32(vec[2],uv32[2][2]);  
}

void swaparray(U32 *ptr, U32 size)
{
    int i;
    
    for (i = 0; i < size; i++,ptr++) {
	intswap((int *)(ptr));
    }
}

void ftransvec(float vec[3], float res[3], float uvs[3][3]) 
{
    res[0] = vec[0]*uvs[0][0] + vec[1]*uvs[0][1] + vec[2]*uvs[0][2];
    res[1] = vec[0]*uvs[1][0] + vec[1]*uvs[1][1] + vec[2]*uvs[1][2];
    res[2] = vec[0]*uvs[2][0] + vec[1]*uvs[2][1] + vec[2]*uvs[2][2];
}     

void uftransvec(float vec[3], float res[3], float uvs[3][3]) 
{
    res[0] = vec[0]*uvs[0][0] + vec[1]*uvs[1][0] + vec[2]*uvs[2][0];
    res[1] = vec[0]*uvs[0][1] + vec[1]*uvs[1][1] + vec[2]*uvs[2][1];
    res[2] = vec[0]*uvs[0][2] + vec[1]*uvs[1][2] + vec[2]*uvs[2][2];
}     

/* The following is a QUICK (and 2-D) distance approximation */
 
U32 FastDst(S32 xdist,S32 zdist)
{
    U32 xdiff,zdiff;
                 
    xdiff = abs(xdist);
    zdiff = abs(zdist);
    return(xdiff+zdiff - (((xdiff > zdiff) ? zdiff : xdiff) >> 1));
}

/* model/display coordinate system conversion utilities. */
/*				*/
/*	axis	model	display	*/
/*	----	-----	-------	*/
/*	right	  Y	   X	*/
/*	forward	  X	   Z	*/
/*	up	 -Z	   Y	*/
/*				*/

/* convert a vector in model coordinates to display coordinates. */
void vecM2D(F32 v[3])
{
	F32 t[3];
	t[0] = v[0]; t[1] = v[1]; t[2] = v[2];
	v[0] = t[1]; v[1] = -t[2]; v[2] = t[0];
}

/* copy a vector and convert from model coordinates to display coordinates. */
void veccopyM2D(F32 m[3], F32 d[3])
{
	d[0] = m[1]; d[1] = -m[2]; d[2] = m[0];
}

/* convert a vector in display coordinates to model coordinates. */
void vecD2M(F32 v[3])
{
	F32 t[3];
	t[0] = v[0]; t[1] = v[1]; t[2] = v[2];
	v[0] = t[2]; v[1] = t[0]; v[2] = -t[1];
}

/* copy a vector and convert from display coordinates to model coordinates. */
void veccopyD2M(F32 d[3], F32 m[3])
{
	m[0] = d[2]; m[1] = d[0]; m[2] = -d[1];
}

/* copy a matrix and convert from model coordinates to display coordinates. */
void matcopyM2D(F32 m[3][3], F32 d[3][3])
{
	d[0][0] =  m[1][1];
	d[0][1] = -m[1][2];
	d[0][2] =  m[1][0];
	d[1][0] = -m[2][1];
	d[1][1] =  m[2][2];
	d[1][2] = -m[2][0];
	d[2][0] =  m[0][1];
	d[2][1] = -m[0][2];
	d[2][2] =  m[0][0];
}

void matcopyD2M(F32 d[3][3],F32 m[3][3])
{
	m[0][0] =  d[2][2];
	m[0][1] =  d[2][0];
	m[0][2] = -d[2][1];
	m[1][0] =  d[0][2];
	m[1][1] =  d[0][0];
	m[1][2] = -d[0][1];
	m[2][0] = -d[1][2];
	m[2][1] = -d[1][0];
	m[2][2] =  d[1][1];
}

