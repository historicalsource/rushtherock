#include "vecmath.h"
#include "fmath.h"

#include "Pro/vecmath.pro"
#include "Pro/unitvecs.pro"
#include "Pro/d3math.pro"

/* This file contains functions to implement Jed and Greg's
unit vector system as modified by me */

#define radtosdeg (0x8000/3.141592654)
#define sdegtorad (3.141592654/0x8000)

		
#define ROTSPERFIX 250		/* number of rotations per fix of matrix */

/* This function converts a vector from body to real world coordinates */
void bodtorw(F32 *v, F32 *vprime, struct uvect *uvst)
{
	F32 *uvp,r;
	register int i;
	
	uvp = uvst->fpuvs[0];

	i=3;
	do{
		r = *v++ * *uvp++;
		r += *v++ * *uvp++;
		*vprime++ = r + *v++ * *uvp++;
		v -= 3;
	}while(--i);
}

/* This function converts a vector from real world to body coordinates */
void rwtobod(F32 *vprime, F32 *v, struct uvect *uvst)
{
	F32 *uvp,r;
	register int i;
	
	uvp = uvst->fpuvs[0];

	i=3;
	do{
		r = (float)*vprime++ * (float)*uvp;
		uvp += 3;
		r += (float)*vprime++ * (float)*uvp;
		uvp += 3;
		*v++ = r + (float)*vprime++ * (float)*uvp;
		uvp -= 5;
		vprime -= 3;
	}while(--i);
}

/* This function converts an integer vector from body to real world coordinates */
void ibodtorw(register S32 *v, register S32 *vprime, struct uvect *uvst)
{
	register S16 *uvp;
	register int i;
	F32 r;
	
	uvp = uvst->uvs[0];
	i=3;
	do{
		r = (float)*v++ * (float)*uvp++;
		r += (float)*v++ * (float)*uvp++;
		*vprime++ = (r + (float)*v++ * (float)*uvp++) * (1.0/0x4000);
		v -= 3;
	}while(--i);
}
		

/* This function converts an integer vector from real world to body coordinates */

void irwtobod(S32 *vprime, S32 *v, struct uvect *uvst)
{
	register S16 *uvp;
	register int i;
	F32 r;
	
	uvp = uvst->uvs[0];
	i=3;
	do{
		r = (float)*vprime++ * (float)*uvp;
		uvp += 3;
		
		r += (float)*vprime++ * (float)*uvp;
		uvp += 3;

		*v++ = (r + (float)*vprime++ * (float)*uvp) * (1.0/0x4000);
		vprime -= 3;
		uvp -= 5;
	}while(--i);
}

/* This function converts a S16 vector from body to real world coords */

void sbodtorw(register S16 *v, register S16 *vprime, struct uvect *uvst)
{
	register S16 *uvp;
	register int i;
	F32 r;
	
	uvp = uvst->uvs[0];
	i=3;
	do{
		r = (float)*v++ * (float)*uvp++;
		r += (float)*v++ * (float)*uvp++;
		*vprime++ = (r + (float)*v++ * (float)*uvp++) * (1.0/0x4000);
		v -= 3;
	}while(--i);
}
		
/* This function converts a S16 vector from real world to body coords */

void srwtobod(register S16 *vprime, register S16 *v, struct uvect *uvst)
{
	register S16 *uvp;
	register int i;
	F32 r;
	
	uvp = uvst->uvs[0];
	i=3;
	do{
		r = (float)*vprime++ * (float)*uvp;
		uvp += 3;
		
		r += (float)*vprime++ * (float)*uvp;
		uvp += 3;

		*v++ = (r + (float)*vprime++ * (float)*uvp) * (1.0/0x4000);
		vprime -= 3;
		uvp -= 5;
	}while(--i);
}

/* This function rotates a F32 3x3 unit vector by a rotation vector in the body axis */
void rotateuv(F32 rv[3], F32 uvs[3][3])
{
	if(rv[ZCOMP]) {
		fyaw(fsin(rv[ZCOMP]),fcos(rv[ZCOMP]),uvs);
        }
	if(rv[YCOMP])
		fpitch(fsin(rv[YCOMP]),fcos(rv[YCOMP]),uvs);

	if(rv[XCOMP])
		froll(fsin(rv[XCOMP]),fcos(rv[XCOMP]),uvs);
}

void rotuv(F32 rv[3], struct uvect *uvstruct)
{
	S16 fixed[3][3];

	if(rv[ZCOMP]) {
		fyaw(fsin(rv[ZCOMP]),fcos(rv[ZCOMP]),uvstruct->fpuvs);
        }
	if(rv[YCOMP])
		fpitch(fsin(rv[YCOMP]),fcos(rv[YCOMP]),uvstruct->fpuvs);

	if(rv[XCOMP])
		froll(fsin(rv[XCOMP]),fcos(rv[XCOMP]),uvstruct->fpuvs);

	makesuvs(uvstruct);
	
	if(uvstruct->fixcnt <= 0){
		uvstruct->fixcnt = ROTSPERFIX;
		matfix(uvstruct->uvs,fixed);
		matcopy((S16 *)(fixed),(S16 *)(uvstruct->uvs));

		makefpuvs(uvstruct);
	}
	else
		uvstruct->fixcnt--;

}

/* This function rotates a unit vector structure by a rotation vector 
in the universe axis */

void urotuv(F32 rv[3], struct uvect *uvstruct)
{
	S16 fixed[3][3];

	if(rv[ZCOMP])
		fuyaw(fsin(rv[ZCOMP]),fcos(rv[ZCOMP]),uvstruct->fpuvs);

	if(rv[YCOMP])
		fupitch(fsin(rv[YCOMP]),fcos(rv[YCOMP]),uvstruct->fpuvs);

	if(rv[XCOMP])
		furoll(fsin(rv[XCOMP]),fcos(rv[XCOMP]),uvstruct->fpuvs);

	makesuvs(uvstruct);
	
	if(uvstruct->fixcnt <= 0){
		uvstruct->fixcnt = ROTSPERFIX;
		matfix(uvstruct->uvs,fixed);
		matcopy((S16 *)(fixed),(S16 *)(uvstruct->uvs));

		makefpuvs(uvstruct);
	}
	else
		uvstruct->fixcnt--;

}


/* This function creates a unit vector structure rolled, pitched, and yawed
with respect to the real world coordinates */

#if 1
const struct uvect munituvs={
	{
		{0x4000,0,0},
		{0,0x4000,0},
		{0,0,0x4000}
	},
	{
		{1.0,0.0,0.0},
		{0.0,1.0,0.0},
		{0.0,0.0,1.0}
	},
	ROTSPERFIX
};
	
void mkuv(S16 phi, S16 theta, S16 psi, struct uvect *uvstruct)
{
	F32 rotv[3];
	*uvstruct = munituvs;

	rotv[0] = phi * sdegtorad;
	rotv[1] = theta * sdegtorad;
	rotv[2] = psi * sdegtorad;
	
	rotuv(rotv,uvstruct);
}
	
	
#else
void mkuv(S16 phi, S16 theta, S16 psi, struct uvect *uvstruct)
{
	S16 sphi,cphi,stheta,ctheta,spsi,cpsi;
	register S16 *suvp;
	F32 *fpuvp;
	register int i;

	sincosptp(phi,theta,psi,&sphi,&cphi,&stheta,&ctheta,&spsi,&cpsi);

	uvstruct->uvs[0][0]= (float)((cpsi*(float)ctheta)*(1.0/0x4000));
	uvstruct->uvs[0][1]= (float)((spsi*(float)ctheta)*(1.0/0x4000));
	uvstruct->uvs[0][2]= -stheta;
	uvstruct->uvs[1][0]= (float)(((float)((cpsi*(float)stheta)*(1.0/0x4000))*(float)sphi-spsi*(float)cphi)*(1.0/0x4000));
	uvstruct->uvs[1][1]= (float)((cpsi*(float)cphi+(float)((stheta*(float)spsi)*(1.0/0x4000))*(float)sphi)*(1.0/0x4000));
	uvstruct->uvs[1][2]= (float)((ctheta*(float)sphi)*(1.0/0x4000));
	uvstruct->uvs[2][0]= (float)((spsi*(float)sphi+(float)((cpsi*(float)stheta)*(1.0/0x4000))*(float)cphi)*(1.0/0x4000));
	uvstruct->uvs[2][1]= (float)(((float)((stheta*(float)spsi)*(1.0/0x4000))*(float)cphi-cpsi*(float)sphi)*(1.0/0x4000));
	uvstruct->uvs[2][2]= (float)((ctheta*(float)cphi)*(1.0/0x4000));

	makefpuvs(uvstruct);

	uvstruct->fixcnt=ROTSPERFIX;
}
#endif

void makefpuvs(struct uvect *uvstruct)
{
	register S16 *suvp;
	F32 *fpuvp;
	register int i;

	for(i=9+1,suvp = uvstruct->uvs[0],fpuvp = uvstruct->fpuvs[0];
	    --i;){
		*fpuvp++ = *suvp++ * (1.0/0x4000);
	}
}
	
void makesuvs(struct uvect *uvstruct)
{
	register S16 *suvp;
	F32 *fpuvp;
	register int i;

	for(i=9+1,suvp = uvstruct->uvs[0],fpuvp = uvstruct->fpuvs[0];
	    --i;){
		*suvp++ = *fpuvp++ * 16384.0;
	}
}
	
void matcopy(register S16 *ap, register S16 *bp)
{
	*bp++ = *ap++;
	*bp++ = *ap++;
	*bp++ = *ap++;

	*bp++ = *ap++;
	*bp++ = *ap++;
	*bp++ = *ap++;

	*bp++ = *ap++;
	*bp++ = *ap++;
	*bp++ = *ap++;
}

void fmatcopy(F32 *ap, F32 *bp)
{
	*bp++ = *ap++;
	*bp++ = *ap++;
	*bp++ = *ap++;

	*bp++ = *ap++;
	*bp++ = *ap++;
	*bp++ = *ap++;

	*bp++ = *ap++;
	*bp++ = *ap++;
	*bp++ = *ap++;
}

void matmul(S16 a[3][3], S16 b[3][3], S16 result[3][3])
{
	register S16 *resp,*ap,*bp;
	register int i;
	int j;

	for(j=3,resp=result[0],ap=a[0];j--;ap+=3)
		for(i=3,bp=b[0];i--;bp++,resp++)
			*resp = 
				(ap[0] * bp[0]+
				ap[1] * bp[3]+
				ap[2] * bp[6])>>14;
}

void matfix(S16 uv[3][3], S16 result[3][3])
{

	scrossprod(uv[0],uv[1],result[2]);	/* Make all vectors perp */
	scrossprod(result[2],uv[0],result[1]);
	scrossprod(result[1],result[2],result[0]);

	sdirection(result[0],result[0]);	/* Make vectors of length 1 */
	sdirection(result[1],result[1]);
	sdirection(result[2],result[2]);
}

/* This subroutine does the math necessary to rotate a S16 unit vector
matrix in the objects frame of reference */
void rot(F32 sint, F32 cost, register S16 *p1, register S16 *p2)
{ 
	F32 f1,f2;
  
	f1 = *p1;
	f2 = *p2;
	*p1 = f1 * cost + f2 * sint;
	p1 += 3;
	*p2 = f2 * cost - f1 * sint;
	p2 += 3;

	f1 = *p1;
	f2 = *p2;
	*p1 = f1 * cost + f2 * sint;
	p1 += 3;
	*p2 = f2 * cost - f1 * sint;
	p2 += 3;

	f1 = *p1;
	f2 = *p2;
	*p1 = f1 * cost + f2 * sint;
	*p2 = f2 * cost - f1 * sint;

} 

/* This subroutine does the math necessary to rotate a S16 unit vector
matrix in the universe frame of reference */

void urot(F32 sint, F32 cost, register S16 *p1, register S16 *p2)
{ 
	F32 f1,f2;
  
	f1 = *p1;
	f2 = *p2;
	*p1++ = f1 * cost + f2 * sint;
	*p2++ = f2 * cost - f1 * sint;

	f1 = *p1;
	f2 = *p2;
	*p1++ = f1 * cost + f2 * sint;
	*p2++ = f2 * cost - f1 * sint;

	f1 = *p1;
	f2 = *p2;
	*p1 = f1 * cost + f2 * sint;
	*p2 = f2 * cost - f1 * sint;


} 

/* rotate unit vectors around x body axis */
void mroll(S16 sint, S16 cost, S16 uv[3][3])
{
	rot(sint*(1.0/0x4000),cost*(1.0/0x4000),&uv[0][1],&uv[0][2]);
}

/* rotate unit vectors around x univ axis */
void muroll(S16 sint, S16 cost, S16 uv[3][3])
{
	urot(sint*(1.0/0x4000),cost*(1.0/0x4000),&uv[1][0],&uv[2][0]);
}

/* rotate unit vectors around y body axis */
void mpitch(S16 sint, S16 cost, S16 uv[3][3])
{
	/* Pitch has signs reversed for some reason */
	rot(-sint*(1.0/0x4000),cost*(1.0/0x4000),&uv[0][0],&uv[0][2]);
}

/* rotate unit vectors around y univ axis */
void mupitch(S16 sint, S16 cost, S16 uv[3][3])
{
	/* Pitch has signs reversed for some reason */
	urot(-sint*(1.0/0x4000),cost*(1.0/0x4000),&uv[0][0],&uv[2][0]);
}

/* rotate unit vectors around z body axis */
void myaw(S16 sint, S16 cost, S16 uv[3][3])
{
	rot(sint*(1.0/0x4000),cost*(1.0/0x4000),&uv[0][1],&uv[0][0]);
}

/* rotate unit vectors around z body axis */
void muyaw(S16 sint, S16 cost, S16 uv[3][3])
{
	urot(sint*(1.0/0x4000),cost*(1.0/0x4000),&uv[1][0],&uv[0][0]);
}

/* This subroutine does the math necessary to rotate a unit vector
matrix in the objects frame of reference */
void frot(F32 sint, F32 cost, F32 *p1, F32 *p2)
{ 
	F32 ut;
	F32 *utp;
  
	utp = &ut;
	
	*utp = *p1 * cost + *p2 * sint;
	*p2 = *p2 * cost - *p1 * sint;
	p2 += 3;
	*p1 = *utp;
	p1 += 3;

	*utp = *p1 * cost + *p2 * sint;
	*p2 = *p2 * cost - *p1 * sint;
	p2 += 3;
	*p1 = *utp;
	p1 += 3;

	*utp = *p1 * cost + *p2 * sint;
	*p2 = *p2 * cost - *p1 * sint;
	*p1 = *utp;
} 

/* This subroutine does the math necessary to rotate a unit vector
matrix in the universe frame of reference */
void furot(F32 sint, F32 cost, F32 *p1, F32 *p2)
{ 
	F32 ut;
	F32 *utp;
  
	utp = &ut;
	
	*utp = *p1 * cost + *p2 * sint;
	*p2++ = *p2 * cost - *p1 * sint;
	*p1++ = *utp;

	*utp = *p1 * cost + *p2 * sint;
	*p2++ = *p2 * cost - *p1 * sint;
	*p1++ = *utp;

	*utp = *p1 * cost + *p2 * sint;
	*p2 = *p2 * cost - *p1 * sint;
	*p1 = *utp;
} 

/* rotate unit vectors around x body axis */
void froll(F32 sint, F32 cost, F32 uv[3][3])
{
	frot(sint,cost,&uv[0][1],&uv[0][2]);
}

/* rotate unit vectors around x univ axis */
void furoll(F32 sint, F32 cost, F32 uv[3][3])
{
	furot(sint,cost,&uv[1][0],&uv[2][0]);
}

/* rotate unit vectors around y body axis */
void fpitch(F32 sint, F32 cost, F32 uv[3][3])
{
	/* Pitch doesn't work unless signs reversed */
	frot(-sint,cost,&uv[0][0],&uv[0][2]);
}

/* rotate unit vectors around y univ axis */
void fupitch(F32 sint, F32 cost, F32 uv[3][3])
{
	/* Pitch doesn't work unless signs reversed */
	furot(-sint,cost,&uv[0][0],&uv[2][0]);
}

/* rotate unit vectors around z body axis */
void fyaw(F32 sint, F32 cost, F32 uv[3][3])
{
	frot(sint,cost,&uv[0][0],&uv[0][1]);
}

/* rotate unit vectors around z body axis */
void fuyaw(F32 sint, F32 cost, F32 uv[3][3])
{
	furot(sint,cost,&uv[0][0],&uv[1][0]);
}

void transpose(S16 uv[3][3], S16 result[3][3])
{
	register int i;
	register S16 *uvp,*resp;
	
    	resp = (S16 *)(&(uv[0][0]));
    	uvp = (S16 *)(&(result[0][0]));
	for(i=3;--i>= 0;){
		*resp++ = *uvp;
		uvp += 3;
		*resp++ = *uvp;
		uvp += 3;
		*resp++ = *uvp;
		uvp -= 5;
	}
}
