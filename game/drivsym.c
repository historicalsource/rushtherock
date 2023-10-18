/*
 *	drivsym.c
 *
 * This is a translation into C of the math model of car
 * performance purchased from Milliken Engineering in Buffalo.
 *
 * Original model Copyright 1985 Milliken Engineering.
 * Translation started 10/20/85, Copyright 1985 Atari Inc.
 *
 * Translated by Max Behensky
 *
 *	Copyright 1996 Time Warner Interactive.
 *	Unauthorized reproduction, adaptation, distribution, performance or 
 *	display of this computer program or the associated audiovisual work
 *	is strictly prohibited.
 *
 *	======================================================================
 *	$Author: gray $						$Date: 1997/10/02 03:20:38 $
 *	$Revision: 3.16 $						$Locker:  $
 *	======================================================================
 *	Change Log:
 *
 *	$Log: drivsym.c,v $
 * Revision 3.16  1997/10/02  03:20:38  gray
 * fix upside-down death bug.
 *
 * Revision 3.15  1997/10/02  02:02:00  gray
 * More robust upside-down death test.
 * No more car damage from major suspension compression.
 *
 * Revision 3.14  1997/05/30  04:05:35  gray
 * mainout is history!
 *
 * Revision 3.13  1996/11/27  14:30:02  gray
 * Dead reckon shadow.
 *
 * Revision 3.12  1996/11/20  21:14:43  geraci
 * new view 2 collision stuff
 *
 * Revision 3.11  1996/11/11  13:12:33  gray
 * Psuedo power slide for beginner cars. Old anti spin for drones.
 *
 * Revision 3.10  1996/10/30  00:24:21  gray
 * New suspension tires and roads.
 *
 * Revision 3.9  1996/10/18  00:34:30  gray
 * Reduce drag force when turning.
 *
 * Revision 3.8  1996/10/10  05:19:24  petrick
 * Cleaned out some unused globals.
 *
 * Revision 3.7  1996/10/03  21:27:51  gray
 * Fixed acceleration calculation. Removed bogus coriolis term.
 *
 * Revision 3.6  1996/10/02  07:57:21  gray
 * Made gravity vectors floating point.
 *
 * Revision 3.5  1996/09/23  20:51:15  gray
 * tweak car difficulty table.
 *
 * Revision 3.4  1996/09/21  10:15:29  gray
 * New anti spin forces.
 *
 * Revision 3.3  1996/09/21  02:25:39  gray
 * Made anti spin values for 4 cars.
 *
 * Revision 3.2  1996/09/13  23:01:11  geraci
 * bump effects removed from here
 *
 * Revision 3.1  1996/09/09  02:24:52  gray
 * JAMMA version
 *
 * Revision 2.13  1996/09/05  21:47:50  gray
 * Beginner anti spin is now Advanced level, Advanced in the middle.
 *
 * Revision 2.12  1996/09/01  23:54:44  gray
 * Calculate real world acceleration (RWA) for dead reackoning.
 *
 * Revision 2.11  1996/09/01  09:16:10  gray
 * Model types cleanup. First pass - removed fptyp.
 *
 * Revision 2.10  1996/08/21  06:21:57  gray
 * Clean up anti-spin.
 *
 * Revision 2.9  1996/08/21  05:28:18  gray
 * Made wall push slightly stronger.
 *
 * Revision 2.8  1996/08/17  18:13:42  gray
 * Changed upside-down test to opposing corners.
 *
 * Revision 2.7  1996/07/30  00:02:23  gray
 * More anti-spin force for faster, stickier cars. Made blowup speed higher.
 *
 * Revision 2.6  1996/07/23  20:46:03  gray
 * Got rid of model data dump stuff.
 *
 * Revision 2.5  1996/07/07  01:28:55  petrick
 * New car structure and knock down targets.
 *
 * Revision 2.4  1996/05/29  02:20:30  geraci
 * new car bump sounds
 *
 * Revision 2.3  1996/05/09  14:12:22  gray
 * More beginner car experimenting.
 *
 * Revision 2.2  1996/05/08  01:22:46  gray
 * Trying new beginner car models.
 *
 * Revision 2.1  1996/05/04  02:56:18  petrick
 * Focus 2 release
 *
 * Revision 1.39  1996/04/30  22:05:26  gray
 * Added torque bounce for wall. Torque in direction of wheel. (test in antispin).
 *
 * Revision 1.38  1996/04/25  09:25:56  gray
 * Orbit mode _seems_ to be fixed. Removed force limiting stuff.
 * Added intermediate car anti-spin code. Expert has no anti-spin.
 *
 * Revision 1.37  1996/04/23  22:38:36  gray
 * Fix carnum setting for beginner and expert cars. First pass anit-spin.
 *
 * Revision 1.36  1996/04/12  00:21:47  geraci
 * got rid of dummy for engine torque
 *
 * Revision 1.35  1996/04/04  00:45:13  geraci
 * added direction to curb whump
 *
 * Revision 1.34  1996/04/03  10:28:54  gray
 * Added CENTERFORCE to force summation.
 *
 * Revision 1.33  1996/04/01  06:17:28  petrick
 * Moved visuals checking to mdrive.
 *
 * Revision 1.32  1996/03/30  08:16:11  petrick
 * Top scrape added.
 *
 * Revision 1.31  1996/03/30  02:04:49  geraci
 * helped 'evil' collision somewhat
 *
 *
 *	======================================================================
 */

/* Externals defined here */
#define mbx
#include "drivsym.h"
#include "model.h"
#include "modelcom.h"
#include "road.h"
#include "stree.h"
#include "fmath.h"
#include "game.h"
#include "dis.h"
#include "globals.h"

#include "Pro/drivsym.pro"
#include "Pro/stree.pro"
#include "Pro/unitvecs.pro"
#include "Pro/initiali.pro"
#include "Pro/communic.pro"
#include "Pro/tires.pro"
#include "Pro/drivetra.pro"
#include "Pro/controls.pro"
#include "Pro/d3math.pro"
#include "Pro/vecmath.pro"
#include "Pro/road.pro"
#include "Pro/timint.pro"

extern void rwtobod(register F32 *vprime, register F32 *v, struct uvect *uvst);

const F32 maxrwr = 10.0;	/* Maximum value for extended precision on RWR */

extern const S16  thump_tab[16];

/*---------------------------------------------------------------------------*/

void syminit(MODELDAT *m)
{
	initialize(m);
	
	road(m);
	positions(m);
	mcommunication(m);
}

void sym(MODELDAT *m)
{
	F32 pneumtrail,realtrail;
	int absvel, i, j;

	controls(m);		/* do controls */

	if (m->crashflag)
		{
		m->steerangle = 0;
		m->clutch = 0;
		m->brake = 0;
		m->throttle = 0;
		}

	checkok(m);		/* are values within limits? */
	drivetrain(m);		/* figure out engine and drivetrain */
	regular(m);

	if((m->TIREFORCE[0][ZCOMP] + m->TIREFORCE[1][ZCOMP]) != 0){
		pneumtrail = 1.0;
		/* Calculate steering wheel torque based on front wheel side
			 force, as well as traction force and steer angle */
		m->swtorque = (m->TIREFORCE[0][YCOMP]+m->TIREFORCE[1][YCOMP])
			*m->swtorquegain*pneumtrail -
			(m->TIREFORCE[0][XCOMP]+m->TIREFORCE[1][XCOMP])
			*m->swtorquegain*m->steerangle;

		/* Reduce force after a crash */
		if(m->crashflag || (m->thetime < .5)){
			if(m->swtorque > 0x1000)
				m->swtorque = 0x1000;
			if(m->swtorque < -0x1000)
				m->swtorque = - 0x1000;
			m->swtorque = m->swtorque * .3;
		}
		
		absvel = m->V[XCOMP];
		absvel = mabs(absvel);
		if(absvel < 88)
			m->swdamp = (absvel * m->maxswdamp)/88;
		else
			m->swdamp = m->maxswdamp;

		absvel = (5*.5)*(m->tires[0].angvel+m->tires[1].angvel);
		absvel = mabs(absvel);
		
		if(absvel < 100){
/*			swfrict = 0x100 - (0xe0 * absvel)/100;*/
			m->swfrict = m->maxswfrict - 
				(((m->maxswfrict-m->minswfrict)*absvel)/100);
		}
		else{
			m->swfrict = m->minswfrict;
		}
	}
	else{
		pneumtrail = 0;
		m->swtorque = 0;
		m->swdamp = 0;
		m->swfrict = 0x20;
	}
	mcommunication(m);	/* communicate results */

	/* 0=no bog, 1=start bogging, 2=bogging, 3=stop bogging */
	if ((m->autotrans==0) && (m->throttle > .5) && (abs(m->rpm)<600))
		{
		if (m->bog_state == 0)
			m->bog_state = 1;
		}
	else
		{
		if (m->bog_state)
			m->bog_state = 3;
		}

	/* do this only for human */
	if (m->net_node == this_node)
		{
		for (i=0; i<3; i++)
			{
			if (m->CENTERFORCE[i] > m->peak_center_force[0][i])
				m->peak_center_force[0][i] = m->CENTERFORCE[i];

			if (m->CENTERFORCE[i] < m->peak_center_force[1][i])
				m->peak_center_force[1][i] = m->CENTERFORCE[i];

			for (j=0; j<4; j++)
				{
				if (m->BODYFORCE[j][i] > m->peak_body_force[0][i])
					m->peak_body_force[0][i] = m->BODYFORCE[j][i];

				if (m->BODYFORCE[j][i] < m->peak_body_force[1][i])
					m->peak_body_force[1][i] = m->BODYFORCE[j][i];
				}
			}
		}
}

void regular(MODELDAT *m)
{
	road(m);		/* figure out what is going on with road */

	forces(m);		/* calc F(t) acting on vehicle */
	antispin(m);		/* calc M to reduce spin-out */
	torques(m);		/* calc M(t) based on F(t) */

	accelerations(m);	/* calc A(t) based on F(t) and M(t) */
	velocities(m);		/* calc velocities based on A(t) */
	positions(m);		/* calc positions based on V(t) */
}

#define NUM_CAR_TYPES 4
#define NUM_DIFF_OPT 3

const F32 as_road[NUM_CAR_TYPES][NUM_DIFF_OPT] = {
	{	600.0,	300.0,	150.0	},
	{	400.0,	200.0,	100.0	},
	{	200.0,	100.0,	 50.0	},
	{	 10.0,	  5.0,	  0.0	}	};

const F32 as_air[NUM_CAR_TYPES][NUM_DIFF_OPT] = {
	{	600.0,	300.0,	150.0	},
	{	400.0,	200.0,	100.0	},
	{	200.0,	100.0,	 50.0	},
	{	 10.0,	  5.0,	  0.0	}	};


/* put in cartype and modeldat later */
const F32 as_scale[NUM_CAR_TYPES] = {	1200.0,	600.0,	100.0,	0.0	};

void antispin(MODELDAT *m)
{
	F32 as_val;
	BOOL air,drone;
	S32 car;

	F32 M,f,f1,f2,f4,lf;

	/*check if stuck on a wall*/
	/*if( absspeed>5fps && velocity=backwards && car=right-side-up && front_contact) */
	if( (m->magvel < 5.0) && (m->V[XCOMP] < 0.0) && (m->UV.fpuvs[ZCOMP][ZCOMP] > 0.0) &&
		((m->BODYFORCE[FRTIRE][XCOMP] < 0.0) || (m->BODYFORCE[FLTIRE][XCOMP] < 0.0)) )
		{
		m->CENTERMOMENT[XCOMP] = 0.0;
		m->CENTERMOMENT[YCOMP] = 0.0;
		m->CENTERMOMENT[ZCOMP] = (m->steerangle > 0.0) ? 90000.0 : -90000.0;
		return;
		}

	/*anti spin moment*/
	air = ((m->roadcode[0] == AIR) &&
		(m->roadcode[1] == AIR) &&
		(m->roadcode[1] == AIR) &&
		(m->roadcode[1] == AIR));

	drone = (m->we_control);

	if(drone)	/* dont change drone model*/
		{
		as_val = 100.0;	/* dont change drone model*/

		m->CENTERMOMENT[XCOMP] = 0.0;
		m->CENTERMOMENT[YCOMP] = 0.0;
		/*only anti-spin if rotating away from direction of travel*/
		if((m->W[ZCOMP] * m->V[YCOMP]) < 0.0)
			m->CENTERMOMENT[ZCOMP] = m->V[YCOMP] * as_val;
		else
			m->CENTERMOMENT[ZCOMP] = 0.0;
		}
	else
		{
		as_val = as_scale[m->carnum & 0x3];

		m->CENTERMOMENT[XCOMP] = 0.0;
		m->CENTERMOMENT[YCOMP] = 0.0;

		f1 = (m->magvel < 0.1) ? 0.0 : (1.0 - (m->V[XCOMP]/m->magvel));
		f2 = f1 * f1;
		f4 = f2 * f2;

		f = f4 + (f1 - f4)*m->throttle;	

/*		M = (m->magvel - m->V[XCOMP]) * as_val;*/
		M = f * m->magvel * as_val;
		/*only anti-spin if rotating away from direction of travel*/
		if((m->W[ZCOMP] * m->V[YCOMP]) < 0.0)
			m->CENTERMOMENT[ZCOMP] = (m->V[YCOMP] > 0.0) ? M : -M;
		else
			m->CENTERMOMENT[ZCOMP] = 0.0;
		}



}
	
void forces(MODELDAT *m)
{
	forces1(m);
	forces2(m);
}

const F32 dirtdamp = -(1000.0/100.0);


void forces1(MODELDAT *m)
{
	int poortract,i;
	F32 temp[3],gameoverdrag,airfact;
	

	if(m->thetime <= m->dt){
		m->D[XCOMP]=0;
		m->D[YCOMP]=0;
		m->D[ZCOMP]=0;

		for(i=0;i<4;++i){
			m->TIREFORCE[i][XCOMP] = 0;
			m->TIREFORCE[i][YCOMP] = 0;
			m->TIREFORCE[i][ZCOMP] = 0;
			m->suscomp[i] = 0;
		}
	}

	if((m->roadcode[0] == AIR) || 
	    (m->roadcode[1] == AIR) || 
	    (m->roadcode[2] == AIR) ||
	    (m->roadcode[3] == AIR)) {
		m->airtime = m->thetime;	/* Last time in air */
	}
	
#if 1
	airfact = 1.0;
#else
	airfact = (m->thetime - m->airtime)*.3;
#endif
	
/*AG - same traction in DIRT*/
#if 1
	poortract = 0;
#else
	poortract = ((m->roadcode[0] == DIRT) && 
		(m->roadcode[2] == DIRT)) && 
		((m->roadcode[1] == DIRT) && 
		(m->roadcode[3] == DIRT));
#endif
	
	/* Calculate tirev and tireuvs for front tires */
	calctireuv(m->V,m->W,m->TIRER[0],m->steerangle,&m->UV,&m->roaduvs[0],
		&m->tireuvs[0],m->TIREV[0]);
	calctireuv(m->V,m->W,m->TIRER[1],m->steerangle,&m->UV,&m->roaduvs[1],
		&m->tireuvs[1],m->TIREV[1]);

	/* Calculate tirev and tireuvs for rear tires */
	calctireuv(m->V,m->W,m->TIRER[2],0.0,&m->UV,&m->roaduvs[2],
		&m->tireuvs[2],m->TIREV[2]);
	calctireuv(m->V,m->W,m->TIRER[3],0.0,&m->UV,&m->roaduvs[3],
		&m->tireuvs[3],m->TIREV[3]);
	
	/* Front tires */
	dotireforce(m,m->TIREV[0],m->TIREV[1],&m->tireuvs[0],&m->tires[0],m->torque[0],
		m->TIREFORCE[0],m->suscomp[0],m->suscomp[1],m->springrate[0],
		m->farspringrate,m->cdamping[0],m->rdamping[0],poortract,airfact);

	dotireforce(m,m->TIREV[1],m->TIREV[0],&m->tireuvs[1],&m->tires[1],m->torque[1],
		m->TIREFORCE[1],m->suscomp[1],m->suscomp[0],m->springrate[1],
		m->farspringrate,m->cdamping[1],m->rdamping[1],poortract,airfact);


{
	/* reduce drag force when turning*/
	F32 tsc;
	tsc = 1.0 - m->throttle;
	if(m->TIREFORCE[0][XCOMP] < 0.0)
		m->TIREFORCE[0][XCOMP] *= tsc;
	if(m->TIREFORCE[1][XCOMP] < 0.0)
		m->TIREFORCE[1][XCOMP] *= tsc;
}

	/* Rear tires */
	dotireforce(m,m->TIREV[2],m->TIREV[3],&m->tireuvs[2],&m->tires[2],m->torque[2],
		m->TIREFORCE[2],m->suscomp[2],m->suscomp[3],m->springrate[2],
		m->farspringrate,m->cdamping[2],m->rdamping[2],poortract,airfact);
	
	dotireforce(m,m->TIREV[3],m->TIREV[2],&m->tireuvs[3],&m->tires[3],m->torque[3],
		m->TIREFORCE[3],m->suscomp[3],m->suscomp[2],m->springrate[3],
		m->farspringrate,m->cdamping[3],m->rdamping[3],poortract,airfact);
	
/* Calculate rolling resistance and aerodynamic drag forces on car */

	m->D[YCOMP]=m->D[ZCOMP]=0;

	if(m->V[XCOMP] > 0)
		m->D[XCOMP] = -(30+ m->rollresist + m->srefpcybo2 * m->V[XCOMP] * m->V[XCOMP]);
	else
		m->D[XCOMP] = -30 + m->rollresist + m->srefpcybo2 * m->V[XCOMP] * m->V[XCOMP];
	if((m->roadcode[2] == DIRT) && (m->roadcode[3] == DIRT))
		m->D[XCOMP] += dirtdamp*m->V[XCOMP];

	if(m->gameover){
		gameoverdrag = magnitude(m->V);
		if(gameoverdrag > 1){
			gameoverdrag = -(1500)/gameoverdrag;
			scalmul(m->V,gameoverdrag,m->D);
		}
	}
}

void forces2(MODELDAT *m)
{
	F32 temp;
	
	if(m->nothrusttorque){
		/* Remove any yawing moment caused by different thrust
		from right and left driving wheels */
		temp = (m->TIREFORCE[2][XCOMP]+m->TIREFORCE[3][XCOMP])* .5;
		m->TIREFORCE[2][XCOMP] = temp;
		m->TIREFORCE[3][XCOMP] = temp;
	}

	/* Sum tire forces */
	vecadd(m->TIREFORCE[0],m->TIREFORCE[1],m->F);
	vecadd(m->TIREFORCE[2],m->F,m->F);
	vecadd(m->TIREFORCE[3],m->F,m->F);

	/* Add gravitational forces */
	rwtobod(m->GRW,m->G,&m->UV);
	vecadd(m->G,m->F,m->F);

	/* Add drag forces*/
	vecadd(m->D,m->F,m->F);

#if 1
	/* Add forces from car body */
	vecadd(m->BODYFORCE[0],m->F,m->F);
	vecadd(m->BODYFORCE[1],m->F,m->F);
	vecadd(m->BODYFORCE[2],m->F,m->F);
	vecadd(m->BODYFORCE[3],m->F,m->F);

	vecadd(m->CENTERFORCE,m->F,m->F);
#endif

}

void torques(MODELDAT *m)
{
	F32 temp[3],temp1[3],*rp,*fp;
	int i;

	
	/* Zero moment */
	m->M[XCOMP]=0;m->M[YCOMP]=0;m->M[ZCOMP]=0;
	
	/* Add moment from each tire */
	for(i=0,rp=m->TIRER[0],fp=m->TIREFORCE[0];i<4;++i,rp+=3,fp+=3){
		veccopy(rp,temp1);
		temp1[ZCOMP] -= m->suscomp[i];
		crossprod(temp1,fp,temp);
		vecadd(m->M,temp,m->M);		/* add to moment */
	}

#if 1
	/* Add moment from each corner of body */
	for(i=0,rp=m->TIRER[0],fp=m->BODYFORCE[0];i<4;++i,rp+=3,fp+=3){
		veccopy(rp,temp1);
		temp1[ZCOMP] = 0.0;
		crossprod(temp1,fp,temp);
		vecadd(m->M,temp,m->M);		/* add to moment */
	}
	vecadd(m->CENTERMOMENT,m->M,m->M);
#endif

}

void accelerations(MODELDAT *m)
{
	F32 corriolis[3];

	/* F = m(A + WxV), F/m = A + WxV, F/m - WxV = A */
	scalmul(m->F,m->massinv,m->A);	/* calculate F/m */
#if 0
	crossprod(m->W,m->V,corriolis);	/* calc corriolis forces */
	vecsub(m->A,corriolis,m->A);	/* calc total force */
#endif
	calcaa(m,m->M,m->AA);		/* calculate angular acceleration */
}

void velocities(MODELDAT *m)
{
	F32 temp[3],velfact;
	
	scalmul(m->A,m->dt,temp);		/* V = V + A*dt */
	vecadd(m->V,temp,m->V);
	
	/* To make sure instant replay works, the velocity can't get too
	big.  If it does, scale it back down to 1000 fps (around the speed
	of sound) */
	m->magvel = xxsqrt((m->V[XCOMP]*m->V[XCOMP])+(m->V[YCOMP]*m->V[YCOMP])+(m->V[ZCOMP]*m->V[ZCOMP]));
	if(m->magvel > 1000.0){
		velfact = 1000.0/m->magvel;
		m->magvel = 1000.0;
		scalmul(m->V,velfact,m->V);
	}

	scalmul(m->AA,m->dt,temp);		/* W = W + AA*dt */
	vecadd(m->W,temp,m->W);
}

void positions(MODELDAT *m)
{
	F32 temp[3],temp1[3],temp2[3],*bp,*rwp;
	F32 movef,mover,moveup,ptch;
	F32 yvect[3],ftroll[3],rtroll[3];
	F32 rollang;
	int i;

#if 0
	temp[XCOMP]=m->W[XCOMP]*m->dt;
	temp[YCOMP]=m->W[YCOMP]*m->dt;
	temp[ZCOMP]=m->W[ZCOMP]*m->dt;

	rotuv(temp,&m->UV);	/* rotate unit vectors by angular vel */
#endif

	bodtorw(m->A,m->RWA,&m->UV);	/* calculate real world acceleration */
	bodtorw(m->V,m->RWV,&m->UV);	/* calculate real world velocity */

	scalmul(m->RWV,m->dt,temp);	
	
	epvecacc(temp,m->EPRWR,m->RWR,maxrwr);	/* RWR = RWR + V*dt */
#if 1
	temp[XCOMP]=m->W[XCOMP]*m->dt;
	temp[YCOMP]=m->W[YCOMP]*m->dt;
	temp[ZCOMP]=m->W[ZCOMP]*m->dt;

	rotuv(temp,&m->UV);	/* rotate unit vectors by angular vel */

	rwtobod(m->RWV,m->V,&m->UV);	/*update V to new uvs*/
#endif

	for(i=0,bp= &m->TIRER[0][0],rwp= &m->TIRERWR[0][0];i<4;i++,rwp+=3)
		{
		temp[0] = *bp++;
		temp[1] = *bp++;
		temp[2] = *bp++ - m->tpcomp[i];
		bodtorw(temp,rwp,&m->UV);
		vecadd(rwp,m->RWR,rwp);
		}
	
	for(i=0,bp= &m->BODYR[0][0],rwp= &m->BODYRWR[0][0];i<4;i++,bp+=3,rwp+=3)
		{
		bodtorw(bp,rwp,&m->UV);
		vecadd(rwp,m->RWR,rwp);
		}

	m->thetime+=m->dt;
}

void calcaa(MODELDAT *m, F32 moment[3], F32 angaccel[3])
{
	angaccel[XCOMP] = moment[XCOMP]*m->INVI[XCOMP];
	angaccel[YCOMP] = moment[YCOMP]*m->INVI[YCOMP];
	angaccel[ZCOMP] = moment[ZCOMP]*m->INVI[ZCOMP];
}

void checkok(MODELDAT *m)
{
	F32 temp,thingvel[3],dtfact;
	static F32 thumpavg[8];
	F32 forcechange, delta_vel[3];
	F32 ucar[3],rcar[3];
	S16   i, thump_it, thump_side;

	temp = (m->V[XCOMP]+m->V[YCOMP]+m->V[ZCOMP]);
	if((temp > 400.0) || (temp < -400.0))
		m->crashflag = 1;

	dtfact = m->dt * (1.0 / .02);
	if(dtfact > .5) dtfact = .5;

	m->forceavg = (m->forceavg*(1.0 - dtfact)) + 
		((m->F[XCOMP]+m->F[YCOMP]+m->F[ZCOMP])*dtfact);

	if((m->forceavg > m->crashthreshold) || (m->forceavg < -m->crashthreshold))
		{
		m->crashflag = 1;
		m->forceavg = 0;
		}

	forcechange = fabs (m->forceavg - thumpavg[m->net_node]);

	if ((m->thumpflag == m->lastthump) && (m->net_node==gThisNode))
		{
		thump_it = 0;

		thump_side = 0;

		for (i=0; i<4; i++)
			{
			if ((m->tpcomp[i] > 1.8) && (thump_it < 3))
				{
				thump_it = 3;
				thump_side |= 1<<i;
				}
			else if ((m->tpcomp[i] > 1.3) && (thump_it < 2))
				{
				thump_it = 2;
				thump_side |= 1<<i;
				}
			else if ((m->tpcomp[i] > .9) && (thump_it < 1))
				{
				thump_it = 1;
				thump_side |= 1<<i;
				}
			}

		m->thump_side = thump_tab[thump_side];

#if 0
		if ((forcechange > m->thumpthreshold) && (thump_it == 0))
			thump_it = 2;
#endif

		m->thumpflag = thump_it;
		}

	thumpavg[m->net_node] = thumpavg[m->net_node] * (1.0 - (dtfact*.25)) + m->forceavg*(dtfact*.25);

	if((m->W[ZCOMP] > 10.0) || (m->W[ZCOMP] < -10.0))
		m->crashflag = 1;

	/* To see if we are upside down on the road, transform the car's
	Z vector into the frame of reference of one of the tires */
	m->topscrape = 0;
	ucar[XCOMP] = m->UV.fpuvs[XCOMP][ZCOMP];
	ucar[YCOMP] = m->UV.fpuvs[YCOMP][ZCOMP];
	ucar[ZCOMP] = m->UV.fpuvs[ZCOMP][ZCOMP];
	rwtobod(ucar,rcar,&m->roaduvs[0]);
	if(rcar[ZCOMP] < -.7071)
		{	/* Car is upside-down */
		if((m->roadcode[0] != AIR) && 
		   (m->roadcode[1] != AIR) && 
		   (m->roadcode[2] != AIR) && 
		   (m->roadcode[3] != AIR))
			{
			m->crashflag = 1;	/* Car is on road surface */
			m->topscrape = 1;
			m->appearance = (m->appearance & ~App_M_TOP_DAMAGE) | (MAX_DAMAGE << gDamageShift[4]);
			}
		}

	/* new test for upside down on road */
/*	if((m->BODYFORCE[0][2] != 0.0) && (m->BODYFORCE[1][2] != 0.0) &&
	   (m->BODYFORCE[2][2] != 0.0) && (m->BODYFORCE[3][2] != 0.0))*/
	/* make sure not mashing suspension enough to hit top corners. */
	if( ((m->suscomp[0]+m->suscomp[1]+m->suscomp[2]+m->suscomp[3]) < 0) &&
	    (((m->BODYFORCE[0][2] != 0.0) && (m->BODYFORCE[3][2] != 0.0)) ||
	     ((m->BODYFORCE[1][2] != 0.0) && (m->BODYFORCE[2][2] != 0.0))) )	/*chk opp corners */
		{
		S16	damage = 1 + ((m->appearance & App_M_TOP_DAMAGE) >> gDamageShift[4]);
		if (m->magvel < 100.0)
			m->crashflag = 1;
		m->topscrape = 1;

		/*	Damage the top of the car. */
		if (damage < MAX_DAMAGE)
			m->appearance = (m->appearance & ~App_M_TOP_DAMAGE) | (damage << gDamageShift[4]);
		}
}


/* This subroutine does a double precision accumulate.  It adds
a to the extended precision number pointed to by dpr.  It then returns
the result in standard floating point format. This is designed to prevent
loss of precision when accumulating small numbers into a large result */

F32 epacc(F32 a,EPTYPE *dpr,F32 max)
{
	dpr->lopart += a;
	
	if((dpr->lopart > max) || (dpr->lopart < -max)){
		dpr->hipart += dpr->lopart;
		dpr->lopart = 0;
	}
	return(dpr->lopart + dpr->hipart);
}

void epvecacc(F32 v[3],EPTYPE dprv[3],F32 rv[3],F32 max)
{
	rv[0] = epacc(v[0],dprv,max);
	rv[1] = epacc(v[1],dprv+1,max);
	rv[2] = epacc(v[2],dprv+2,max);
}

void epveccopy(F32 v[3],EPTYPE dprv[3])
{
	dprv[0].hipart = v[0];
	dprv[0].lopart = 0;

	dprv[1].hipart = v[1];
	dprv[1].lopart = 0;

	dprv[2].hipart = v[2];
	dprv[2].lopart = 0;
}
