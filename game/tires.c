
/* This file is the heart of the simulation. It calculates the
forces between the tires and the road.  Based on the original
milliken model, many discussions with Doug and Bill milliken, and
two research papers:

	Lateral Stiffness, Cornering Stiffness,
	and Relaxation length of the Pneumatic Tire

		Jeffery S. Loeb, M.S.
			OSU

		John R. Ellis, Ph. D.
			OSU

		Dennis A. Guenther, Ph. D., P.E.
			OSU

	This paper lays out the basics of an extended model
	of tire characteristics for a linear modeled tire.


	The Effect of Steering Amplitude
		on Relaxation Length

		John R. Ellis Ph. D.
			OSU

	This paper extends the above model into the areas where
	tire characteristics become nonlinear.

The tire model in this file is an extension of the above model
for all directions, slip angles, and adding friction circle
capabilities.

		Max Behensky 11/25/86
		for Atari Games
*/

#include "drivsym.h"
#include "modeldat.h"
#include "road.h"

#include "Pro/tires.pro"
#include "Pro/unitvecs.pro"
#include "Pro/d3math.pro"
#include "Pro/initiali.pro"
#include "Pro/vecmath.pro"

#define DIDDLE 0

void calctireuv(F32 v[3], F32 w[3], F32 r[3], F32 steer, 
				struct uvect *caruvs, struct uvect *roaduvs, 
				struct uvect *tireuvs, F32 tirev[3])
{
#if DIDDLE
	F32 cartirev[3],rwtirev[3];
#else
	F32 cartirev[3];
#endif
	short ivec[3],roadz[3],sang,ssteer,csteer,cptch,sptch;
	
	crossprod(w,r,cartirev);	/* calculate rotational components */
	vecadd(cartirev,v,cartirev);	/* add translation components */
#if DIDDLE
	bodtorw(cartirev,rwtirev,caruvs);
#endif
	
	if(steer != 0){	/* make copy of short uvs */
		/* integer scaled steer angle */
		sang = steer * radtosdeg;

		/* integer sin and cos of steer ang */
		ssteer = mssin(sang);
		csteer = mscos(sang);
		
		tireuvs->uvs[0][0] = csteer;
		tireuvs->uvs[1][0] = ssteer;
		tireuvs->uvs[2][0] = 0;
		tireuvs->uvs[0][1] = -ssteer;
		tireuvs->uvs[1][1] = csteer;
		tireuvs->uvs[2][1] = 0;
		tireuvs->uvs[0][2] = 0;
		tireuvs->uvs[1][2] = 0;
		tireuvs->uvs[2][2] = 0x4000;
	}
	else{
		tireuvs->uvs[0][0] = 0x4000;
		tireuvs->uvs[1][0] = 0;
		tireuvs->uvs[2][0] = 0;
		tireuvs->uvs[0][1] = 0;
		tireuvs->uvs[1][1] = 0x4000;
		tireuvs->uvs[2][1] = 0;
		tireuvs->uvs[0][2] = 0;
		tireuvs->uvs[1][2] = 0;
		tireuvs->uvs[2][2] = 0x4000;
	}
		

	ivec[XCOMP] = roaduvs->uvs[0][2]; /* Get road z vector, univ coords */
	ivec[YCOMP] = roaduvs->uvs[1][2];
	ivec[ZCOMP] = roaduvs->uvs[2][2];

	srwtobod(ivec,roadz,caruvs);
	
	shypotsincos(roadz[XCOMP],roadz[ZCOMP],&sptch,&cptch);

	/* pitch unit vectors to road pitch */
	mpitch(sptch,cptch,tireuvs->uvs);

	makefpuvs(tireuvs);		/* make full floating point uvs */
	
#if DIDDLE
	rwtobod(rwtirev,tirev,roaduvs);	/* Convert to road coords */
#else
	rwtobod(cartirev,tirev,tireuvs);	/* Convert to wheel coords */
#endif
}

void dotireforce(MODELDAT *m, F32 tirev[3], F32 ottirev[3], struct uvect *tireuvs, 
				struct tiredes *tire, F32 torque, F32 forcevec[3], F32 suscomp, 
				F32 otsuscomp, F32 springrate, F32 arspringrate, F32 cdamping, 
				F32 rdamping, int poortract, F32 airfact)
{
	register int i;
	F32 wheelalpha,spd,temp,normal,maxnorm,damping;
	F32 sideforce,traction,arforce;
	F32 rsforce[3],cartirev[3],tireforcevec[3];
	
	
#if 1
			if(tire->Cstiff != tire->PaveCstiff)
				{
				tire->Cstiff = tire->PaveCstiff;
				tire->Cfmax = tire->PaveCfmax;
				tire_constants(tire);
				}
#else
	if(poortract){
		if(tire->Cstiff != (temp = tire->PaveCstiff * (F32).8)){
			tire->Cstiff = temp;
			tire->Cfmax = tire->PaveCfmax * (F32).8;
			tire_constants(tire);
		}
	}
	else{
		if(airfact < (F32).5){
			tire->Cstiff = tire->PaveCstiff * ((F32).5 + airfact);
			tire->Cfmax = tire->PaveCfmax;
			tire_constants(tire);
		}
		else{
			if(tire->Cstiff != tire->PaveCstiff){
				tire->Cstiff = tire->PaveCstiff;
				tire->Cfmax = tire->PaveCfmax;
				tire_constants(tire);
			}
		}
	}
#endif

#if 1
	if((suscomp > 0.0) && (otsuscomp > 0.0))
		arforce = (suscomp - otsuscomp)*arspringrate;
	else
		arforce = 0.0;
#else
	/* calculate suspension damping forces */
	if(suscomp > 0.0) 
		suscomp += tirev[ZCOMP]*m->dt*.5;	/* little runge-kutta */
	
	if(otsuscomp > 0)
		{
		otsuscomp += ottirev[ZCOMP]*m->dt*.5;	/* little runge-kutta */

		/* Calculate differences in suspension compression for anti roll bar */
		arforce = (suscomp - otsuscomp)*arspringrate;
		}
	else
		arforce = 0;
#endif
	
	if(tirev[ZCOMP] > 0)	/* Compression damping */
		damping = cdamping;
	else
		damping = rdamping;
	
	if(suscomp > (F32)10.0)
#if 1
/*		tireforcevec[ZCOMP] = arforce + suscomp * suscomp * springrate + damping * tirev[ZCOMP];*/
/*		tireforcevec[ZCOMP] = arforce + (suscomp*suscomp)*(springrate + damping * tirev[ZCOMP]);*/
/*		tireforcevec[ZCOMP] = arforce + (suscomp * springrate + damping * tirev[ZCOMP])*2.0;*/
		if(tirev[ZCOMP] > -1.0)
			tireforcevec[ZCOMP] = (tirev[ZCOMP] + 1.0) * m->mass * -0.25 * m->idt;
		else
			tireforcevec[ZCOMP] = arforce + suscomp * springrate + damping * tirev[ZCOMP];
#else
		tireforcevec[ZCOMP] = arforce +
			((suscomp + tirev[ZCOMP]*m->dt*(F32).5)*springrate + 
			damping* tirev[ZCOMP])*(F32)2;
#endif
	else if(suscomp > 0)
		tireforcevec[ZCOMP] = arforce + suscomp * springrate + damping * tirev[ZCOMP];
/*		tireforcevec[ZCOMP] = suscomp * (arforce + suscomp * springrate + damping * tirev[ZCOMP]);*/
	else
		tireforcevec[ZCOMP] = 0;

	normal = -tireforcevec[ZCOMP];
#if 1
	if(normal < 0.0)
		{
		normal = 0.0;
		tireforcevec[ZCOMP]=0;
		}
	else if(normal > m->weight)
		normal = m->weight;
	frictioncircle(m,tirev,normal,torque,tire,&sideforce,&traction);
		
#else
	maxnorm = m->weight * 4.0;	/* Maximum normal force, prevent tire
				simulation instability */
	if(normal > (maxnorm)){
		frictioncircle(m,tirev,maxnorm,
			torque,tire,&sideforce,&traction);
	}
	else{
		if(normal < 0){
			normal = 0;
			tireforcevec[ZCOMP]=0;
		}
		frictioncircle(m,tirev,normal,
			torque,tire,&sideforce,&traction);
	}
#endif

	tireforcevec[XCOMP]= traction;
	tireforcevec[YCOMP]= sideforce;

	bodtorw(tireforcevec,forcevec,tireuvs);	/* convert to car */

	tire->sideforce = sideforce;
	tire->traction = traction;
}

#define DAMPSPEED 400


void frictioncircle(MODELDAT *m, F32 tirev[3], F32 normalforce, F32 torque, 
					struct tiredes *tire, F32 *sfp, F32 *trp)
{
	F32 maxtraction;
	F32 maxtorque,maxf,Cfmax,Afmax,temp,roadangvel;
	F32 ydot,p,k2,k3,l2,l3,patchvel,patchspeed,realtorque;
    	F32 alpha;
	F32 calcalpha();

	realtorque = torque;
	temp = m->mass*tire->tradius;

	tire->slipflag=0;

	torque = temp*torque/(temp + (F32)1.0/tire->invmi);

	maxtraction = tire->Cfmax*normalforce;
	maxtorque = maxtraction*tire->tradius;
	roadangvel = tirev[XCOMP]/tire->tradius;
	if(tire->angvel > roadangvel){
		tire->sliptorque = maxtorque;
		tire->angvel += (realtorque - tire->sliptorque)*tire->invmi*m->dt;
		if(tire->angvel <= roadangvel){
			tire->slipflag = 10;
			tire->angvel = roadangvel;
		}
		else{
			patchvel = tirev[XCOMP] - tire->angvel*tire->tradius;
			patchspeed = xxsqrt(patchvel*patchvel +
				 tirev[YCOMP]*tirev[YCOMP]);
			if(patchspeed == 0){
				*trp = maxtraction;
				*sfp = 0;
			}
			else{
				*trp = -maxtraction * patchvel / patchspeed;
				*sfp = -maxtraction * tirev[YCOMP] / patchspeed;
			}
			tire->slipflag=20;
			return;
		}
	}
	else if(tire->angvel < roadangvel){
		tire->sliptorque = -(F32)maxtorque;
		tire->angvel += (realtorque-tire->sliptorque)*tire->invmi*m->dt;

		if(tire->angvel >= roadangvel){
			tire->angvel = roadangvel;
			tire->slipflag = 30;
		}
		else{
			patchvel = tirev[XCOMP] - tire->angvel*tire->tradius;
			patchspeed = xxsqrt(patchvel*patchvel +
				 tirev[YCOMP]*tirev[YCOMP]);

			if(patchspeed == 0){
				*trp = -maxtraction;
				*sfp = 0;
			}
			else{
				*trp = -maxtraction * patchvel / patchspeed;
				*sfp = -maxtraction * tirev[YCOMP] / patchspeed;
			}

			tire->slipflag=40;
			return;
		}
	}

	tire->sliptorque=0;

	if(normalforce <= 0){
		*trp = *sfp = 0;
		tire->slipflag += 3;
		tire->angvel += realtorque * tire->invmi * m->dt;
		return;
	}

	if(torque != 0){
	
		if(torque >= maxtorque){
			*trp = maxtraction;
			*sfp = 0;
			tire->slipflag += 4;
			tire->sliptorque = maxtorque;
			tire->angvel += (realtorque - tire->sliptorque)*
				tire->invmi*m->dt;
			
			return;
		}

		if(torque <= -maxtorque){
			*trp = -(F32)maxtraction;
			*sfp = 0;
			tire->slipflag += 5;
			tire->sliptorque = -(F32)maxtorque;
			tire->angvel += (realtorque - tire->sliptorque)*
				tire->invmi*m->dt;
			return;
		}
		
		*trp = torque/tire->tradius;
		temp = *trp/normalforce;

		Cfmax = xxsqrt(tire->Cfmax * tire->Cfmax - temp * temp);

		k2 = tire->k1*tire->k1/(3*Cfmax);
		k3 = tire->k1*tire->k1*tire->k1/(27*Cfmax*Cfmax);
		Afmax = 3*Cfmax/tire->k1;
	}
	else{
		*trp = 0;
		k2 = tire->k2;
		k3 = tire->k3;
		Cfmax = tire->Cfmax;
		Afmax = tire->Afmax;
	}

	l2 = k2;
	l3 = k3;

	maxf = Cfmax * normalforce;

	alpha = calcalpha(tirev);

	tire->sliptorque=0;

	if(alpha >= 0){
		if(alpha >= Afmax){
			ydot = -(F32)tirev[YCOMP];
			tire->patchy += ydot*m->dt;

			*sfp = tire->springK*tire->patchy +
				tire->rubdamp * ydot;

			if(*sfp < -maxf){
				*sfp = -(F32)maxf;
				tire->patchy = *sfp/tire->springK;
			}
			tire->slipflag += 6;
			tire->sliptorque = *trp * tire->tradius;
			tire->angvel += (realtorque - tire->sliptorque)*
				tire->invmi*m->dt;
			return;
		}
		tire->slipflag += 7;
		p = tirev[XCOMP]*tire->springK/(normalforce * 
			(tire->k1 - l2*alpha + l3*alpha*alpha));
		if(p <  0)p = -(F32)p;

		if(p * m->dt < (F32).5){
			ydot = -p * tire->patchy - tirev[YCOMP];
			tire->patchy += ydot*m->dt;

			*sfp = tire->springK*tire->patchy +
				tire->rubdamp * ydot;
			tire->slipflag += 100;
		}
		else{
			*sfp = -(tire->k1*alpha - k2*alpha*alpha +
				k3*alpha*alpha*alpha)*normalforce;

			tire->patchy = *sfp/tire->springK;
			tire->slipflag += 200;
		}

	}
	else{
		if(alpha <= -Afmax){
			ydot = -(F32)tirev[YCOMP];
			tire->patchy += ydot*m->dt;

			*sfp = tire->springK*tire->patchy +
				tire->rubdamp * ydot;
			if(*sfp > maxf){
				*sfp = maxf;
				tire->patchy = *sfp/tire->springK;
			}
			tire->slipflag += 8;
			tire->sliptorque = *trp * tire->tradius;
			tire->angvel += (realtorque - tire->sliptorque)*
				tire->invmi*m->dt;
			return;
		}

		tire->slipflag += 9;
		p = tirev[XCOMP]*tire->springK/(normalforce * 
			(tire->k1 + l2*alpha + l3*alpha*alpha));

		if(p <  0)p = -(F32)p;

		if(p * m->dt < (F32).5){
			ydot = -p * tire->patchy - tirev[YCOMP];
			tire->patchy += ydot*m->dt;

			*sfp = tire->springK*tire->patchy +
				tire->rubdamp * ydot;
			tire->slipflag += 100;
		}
		else{
			*sfp = -(tire->k1*alpha + k2*alpha*alpha +
				k3*alpha*alpha*alpha)*normalforce;

			tire->patchy = *sfp / tire->springK;
			tire->slipflag += 200;
		}

	}
}

F32 calcalpha(F32 tirev[3])
{
	F32 alpha;

	alpha = tirev[YCOMP];
	if(tirev[XCOMP] > 0){
		if(tirev[XCOMP] < DAMPSPEED)
			alpha = tirev[YCOMP]/tirev[XCOMP];	/* find tire alpha */
		else
			alpha = tirev[YCOMP]*(F32)(1.0/DAMPSPEED);	/* find tire alpha */
	}

	if(tirev[XCOMP] < 0){
		if(tirev[XCOMP] > -DAMPSPEED)
			alpha = -tirev[YCOMP]/tirev[XCOMP];	/* find tire alpha */
		else
			alpha = tirev[YCOMP]*(F32)(1.0/DAMPSPEED);	/* find tire alpha */
	}
	return(alpha);
}


void make_tire_road_uvs(struct uvect *caruvs, F32 steer, struct uvect *roaduvs, struct uvect *truvs)
{
	struct uvect tmpuvs;
	S16 ssteer,csteer,uvarray[3][3];
	S32 sang,tempvec[3],rdvec[3];
	
	matcopy((short *)(caruvs->uvs),(short *)(tmpuvs.uvs));

	if(steer != 0){	/* make copy of short uvs */
		/* integer scaled steer angle */
		sang = steer * radtosdeg;
		/* integer sin and cos of steer ang */
		ssteer = mssin(sang);
		csteer = mscos(sang);
		/* create front wheel unit vectors */
		myaw(ssteer,csteer,tmpuvs.uvs);
	}
	
	tempvec[XCOMP]=tmpuvs.uvs[0][1];    /* make integer tire y vector */
	tempvec[YCOMP]=tmpuvs.uvs[1][1];   /* universe frame */
	tempvec[ZCOMP]=tmpuvs.uvs[2][1];

	irwtobod(tempvec,rdvec,roaduvs);    /* make tire y vec, road frame */
	rdvec[ZCOMP]=0;			/* Make vector in XY plane */
	ibodtorw(rdvec,tempvec,roaduvs);    /* convert to universe frame */
	
	veccopy((F32 *)(&tempvec[0]),(F32 *)(&uvarray[1][0]));	/* y vec into transposed uvarrray */
	sdirection(uvarray[1],uvarray[1]);	/* normalize y vec */

	uvarray[2][0]=roaduvs->uvs[0][2];    /* road z vec into uvarray */
	uvarray[2][1]=roaduvs->uvs[1][2];
	uvarray[2][2]=roaduvs->uvs[2][2];

#if 0
	uvarray[2][0]=tmpuvs.uvs[0][2];    /* road z vec into uvarray */
	uvarray[2][1]=tmpuvs.uvs[1][2];
	uvarray[2][2]=tmpuvs.uvs[2][2];
#endif

	scrossprod(uvarray[1],uvarray[2],uvarray[0]);
				/* make x vec perp to y and z vecs */

	transpose(uvarray,truvs->uvs);
			/* un-transpose and put in tire road unit vecs */
	makefpuvs(truvs);	/* make floating point uvs */
	truvs->fixcnt = 0;
}
	
