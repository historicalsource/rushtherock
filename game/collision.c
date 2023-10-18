/*
 *	collision.c	-- Functions for handling collisions.
 *
 *	Copyright 1996 Time Warner Interactive.
 *	Unauthorized reproduction, adaptation, distribution, performance or 
 *	display of this computer program or the associated audiovisual work
 *	is strictly prohibited.
 *
 *	======================================================================
 *	$Author: gray $						$Date: 1997/06/28 00:43:23 $
 *	$Revision: 3.8 $						$Locker:  $
 *	======================================================================
 *	Change Log:
 *
 *	$Log: collision.c,v $
 * Revision 3.8  1997/06/28  00:43:23  gray
 * Made everything look at dead reckoned uvs.
 *
 * Revision 3.7  1996/11/21  15:57:59  gray
 * No-surf collisions.
 *
 * Revision 3.6  1996/11/20  21:15:15  geraci
 * new view 2 collision stuff
 *
 * Revision 3.5  1996/11/19  18:48:25  gray
 * Different size collision boxes.
 *
 * Revision 3.4  1996/11/03  05:56:30  geraci
 * split up debug switches and regular game switches
 *
 * Revision 3.3  1996/10/18  22:42:16  petrick
 * Made Car a typedef.
 *
 * Revision 3.2  1996/09/17  11:12:19  gray
 * Made collision box shorter.
 *
 * Revision 3.1  1996/09/09  02:24:52  gray
 * JAMMA version
 *
 * Revision 2.11  1996/09/01  23:57:28  gray
 * Real Dead Reckon structure.
 *
 * Revision 2.10  1996/08/31  01:41:20  gray
 * Kludgy init of colrad for remote cars.
 *
 * Revision 2.9  1996/08/22  23:05:24  gray
 * Setting damage level from car/car collisions.
 *
 * Revision 2.8  1996/08/21  01:30:22  gray
 * Slightly better, still some surf action.
 *
 * Revision 2.7  1996/08/20  21:43:04  gray
 * No overlap collisions.
 *
 * Revision 2.6  1996/08/20  19:48:53  gray
 * Collisions use dead reckon data now.
 *
 * Revision 2.5  1996/08/19  04:43:52  gray
 * Better collisions but still overlap.
 *
 * Revision 2.4  1996/08/01  19:13:55  gray
 * No more overlap collision. Still too bouncy for front back.
 *
 * Revision 2.3  1996/07/29  23:54:38  gray
 * Check collisions for velocities up to 300 fps (was 220).
 *
 * Revision 2.2  1996/07/23  20:43:40  gray
 * Cleaned the mess. Made bodyforce and centerforce use common routine.
 *
 * Revision 2.1  1996/05/04  02:56:18  petrick
 * Focus 2 release
 *
 * Revision 1.19  1996/04/25  09:22:32  gray
 * Made car/car collisions take delta velocity into account.
 *
 * Revision 1.18  1996/04/03  23:27:47  gray
 * Check for which cars are actually in race now.
 *
 * Revision 1.17  1996/04/03  10:26:50  gray
 * Added wacky collisions (easy_collision) = No moment collision forces.
 *
 * Revision 1.16  1996/04/03  02:19:56  gray
 * Made speeding death cars non-collidable.
 *
 * Revision 1.15  1996/04/01  13:57:02  geraci
 * not collidable code
 *
 * Revision 1.14  1996/03/30  02:05:35  geraci
 * helped 'evil' collision somewhat
 *
 * Revision 1.13  1996/03/28  12:26:00  gray
 * Make collisions disabled on SW_DEBUG10.
 *
 * Revision 1.12  1996/03/27  22:39:43  gray
 * Car/car collisions working OK.
 *
 * Revision 1.11  1996/03/16  02:04:04  petrick
 * Added header.
 *
 * 10/31/95 asg	Created
 *
 *	======================================================================
 */

#include "drivsym.h"
#include "globals.h"
#include "model.h"
#include "modelcom.h"
#include "road.h"
#include "dis.h"
#include "debug.h"

#include "Pro/mdrive.pro"
#include "Pro/d3math.pro"
#include "Pro/stree.pro"
#include "Pro/vecmath.pro"
#include "Pro/unitvecs.pro"
#include "Pro/collision.pro"

/*===============================  DEFINES  =================================*/

#define COLL_ALG_SOFTWALL 0
#define COLL_ALG_HARDWALL 1
#define COLL_ALG_CARBODY 2

const F32 g_Kcs = (-10000.0);
const F32 g_Krs = (-11000.0);
const F32 g_Kcd = (-50.0);
const F32 g_Krd = (0.0);

#define MAXFORCE  4e3
#define MAX_VEL_SQ (300*300)

/*===============================  GLOBALS  =================================*/


/* ----------------------------------------------------------------------------- */
void init_collision(MODELDAT *m)
{
	MODELDAT 		*m2;
	Car 			*carp;
	extern STREETYP	*stree;
	extern Car 		*carlist[];
	S32 			i,j;

	for(i=0;i<3;i++)
		{
		m->CENTERFORCE[i] = 0.0;
		m->peak_center_force[0][i] = 0.0;
		m->peak_center_force[1][i] = 0.0;
		m->peak_body_force[0][i] = 0.0;
		m->peak_body_force[1][i] = 0.0;
		}

	for(i=0;i<4;i++)
		{
		m->lasttp[i] = &stree[0];
		for(j=0;j<3;j++)
			m->BODYFORCE[i][j] = 0.0;
		}
}

void collision(MODELDAT *m)
{
	MODELDAT *m2;
	F32 vec[3],pos[3],posr[3],dsq;
	S32 mnum,i;

/*DEBUG*/
	if(dlevels & SW_DEBUG10)
		return;	/*SW_DEBUG10 = disable collisions */
/*DEBUG*/

	/* check if car is collidable at this time */
	if (m->collidable == false)
		return;

	/* Temp - If absolute velocity > 300 fps, somethings wrong. */
	if((m->V[0]*m->V[0] + m->V[1]*m->V[1] + m->V[2]*m->V[2]) > MAX_VEL_SQ)
		return;

	for(mnum=0,m2=&model[0];mnum<MAX_LINKS;mnum++,m2++)
		{
		 /*dont test collision with yourself*/
		if(m2 == m)
			continue;

		/* make sure car is real */
		if(!m2->in_game)
			continue;

		/* check if car is collidable */
		if (m2->collidable == false)
			continue;

		/* Temp - If absolute velocity > 300 fps, somethings wrong. */
		if((m2->V[0]*m2->V[0] + m2->V[1]*m2->V[1] + m2->V[2]*m2->V[2]) > MAX_VEL_SQ)
			continue;

		/* see if bounding spheres intersect (vec = vector from other CG to our CG)*/
		vecsub(m->reckon.RWR,m2->reckon.RWR,vec);
		for(i=0,dsq=0.0;i<3;i++)
			dsq += (vec[i]*vec[i]);
		if(dsq > ((m->colrad + m2->colrad) * (m->colrad + m2->colrad)))
			continue;	/* no overlap - test next model */

		/* test if our corners in  other*/
		for(i=0;i<4;i++)
			{
			fbodtorw(m->BODYR[i],pos,m->reckon.UV[0]);
			vecadd(pos,m->reckon.RWR,pos);
			vecsub(pos,m2->reckon.RWR,pos);
			frwtobod(pos,posr,m2->reckon.UV[0]);
			if(PointInBody(m2,posr))
				{
/*				setCollisionForce(m,m,m2,vec,posr);*/
				setFBCollisionForce(m,m,m2,vec,posr);
				return;
				}
			}

		/* test if other car corners in  */
		for(i=0;i<4;i++)
			{
			fbodtorw(m2->BODYR[i],pos,m2->reckon.UV[0]);
			vecadd(pos,m2->reckon.RWR,pos);
			vecsub(pos,m->reckon.RWR,pos);
			frwtobod(pos,posr,m->reckon.UV[0]);
			if(PointInBody(m,posr))
				{
/*				setCollisionForce(m,m2,m,vec,posr);*/
				setFBCollisionForce(m,m2,m,vec,posr);
				return;
				}
			}
		}	/* end for(mnum) */
}


#define COLL_TOP_DIST (-5.0)
#define COLL_BOT_DIST (1.0)

BOOL PointInBody(MODELDAT *m,F32 pt[3])
{
	if((pt[0] > m->BODYR[0][0]) || (pt[0] < m->BODYR[3][0])) return(0);
	if((pt[1] > m->BODYR[0][1]) || (pt[1] < m->BODYR[3][1])) return(0);
	if((pt[2] > COLL_BOT_DIST) || (pt[2] < COLL_TOP_DIST)) return(0);

	return(1);
}

#if 0
/* m  = model to apply collision force. */
/* m1 = model with corner in m2 bounding box. */
/* dir = vector from other CG to out CG. */
/* pos = reative postion of corner in m2 bounding box. */
void setCollisionForce(MODELDAT *m,MODELDAT *m1,MODELDAT *m2,F32 dir[3],F32 pos[3])
{
	F32 invdist,vin,force,temp[3],rvel[3];
	S32 i;

	/*normalize direction to apply force (from other CG to our CG)*/
	invdist = dir[0]*dir[0] + dir[1]*dir[1] + dir[2]*dir[2];
	if(invdist < 0.0001)	/*coincident centers, bounce up*/
		{
		dir[0] = dir[1] = 0.0;
		dir[2] = -1.0;
		invdist = 1.0;
		}
	else
		{
		invdist = 1.0/sqrt(invdist);
		for(i=0;i<3;i++)
			dir[i] = dir[i] * invdist;
		}

	/*get rel velocity from m1 to m2 into m2 frame*/
	vecsub(m1->reckon.base_RWV,m2->reckon.base_RWV,temp);
	rwtobod(temp,rvel,&m2->UV);
	rvel[1] *= 0.5;
	/* find maximum velocity into bounding box. */
	for(i=0;i<3;i++)
		temp[i] = (pos[i] > 0.0) ? -rvel[i] : rvel[i];
	vin = (temp[0] > temp[1]) ? temp[0] : temp[1];
	if(temp[2] > vin) vin = temp[2];

	/*set force based on max velocity into*/
	force = (vin + 5.0) * 2000.0;
	if(force < 0.0)
		force = 0.0;

	for(i=0;i<3;i++)
		temp[i] = force * dir[i];
	rwtobod(temp,m->CENTERFORCE,&m->UV);
	if(m->CENTERFORCE[0] > 0.0)
		m->CENTERFORCE[0] *= 0.5;
	else
		m->CENTERFORCE[0] *= 1.5;

	setCollisionDamage(m);
}

#endif


void ForceApart(MODELDAT *m,MODELDAT *m1,MODELDAT *m2,F32 dir[3],F32 pos[3])
{
	MODELDAT *mother;
	F32 invdist,force,temp[3];
	S32 i;


	/*normalize direction to apply force (from other CG to our CG)*/
	invdist = dir[0]*dir[0] + dir[1]*dir[1] + dir[2]*dir[2];
	if(invdist < 0.0001)
		{
		/* if coincident centers, force in opposite directions */
		mother = (m == m1) ? m2 : m1;
		dir[0] = dir[2] = 0.0;
		dir[1] = (m->slot > mother->slot) ? 1.0 : -1.0;
		invdist = 1.0;
		}
	else
		{
		invdist = 1.0/sqrt(invdist);
		for(i=0;i<3;i++)
			dir[i] = dir[i] * invdist;
		}

	force = 100000.0;

	for(i=0;i<3;i++)
		temp[i] = force * dir[i];
	rwtobod(temp,m->CENTERFORCE,&m->UV);
}

/* m  = model to apply collision force. */
/* m1 = model with corner in m2 bounding box. */
/* dir = vector from other CG to out CG. */
/* pos = reative postion of corner in m2 bounding box. */
void setFBCollisionForce(MODELDAT *m,MODELDAT *m1,MODELDAT *m2,F32 dir[3],F32 pos[3])
{
	F32 dist[6],mindist,force[3],temp[3],rvel[3],cent[3];
	S32 i,side;
	BOOL beside,behind;

	/* 0=front,1=back,2=right,3=left,4=bottom,5=top */
	dist[0] = m2->BODYR[0][0] - pos[0];
	dist[1] = pos[0] - m2->BODYR[3][0];
	dist[2] = m2->BODYR[0][1] - pos[1];
	dist[3] = pos[1] - m2->BODYR[3][1];
	dist[4] = COLL_BOT_DIST - pos[2];
	dist[5] = pos[2] - COLL_TOP_DIST;


	/* get rel center of m1 */
	vecsub(m1->reckon.RWR,m2->reckon.RWR,temp);
	frwtobod(temp,cent,m2->reckon.UV[0]);
	beside = ((cent[0] < m2->BODYR[0][0]) && (cent[0] > m2->BODYR[3][0]));
	behind = ((cent[1] < m2->BODYR[0][1]) && (cent[1] > m2->BODYR[3][1]));

	if(beside && behind)	/* major overlap - blast apart */
		{
		ForceApart(m,m1,m2,dir,pos);
		setCollisionDamage(m);
		return;
		}

	/*get rel velocity from m1 to m2 into m2 frame*/
	vecsub(m1->reckon.base_RWV,m2->reckon.base_RWV,temp);
	frwtobod(temp,rvel,m2->reckon.UV[0]);

	/* get x (front/back) force for m2 */
	if(beside)
		force[0] = 0.0;	/* no front/back forces if directly beside. */
	else
		{
		force[0] = rvel[0] * 4000.0;
		if((pos[0] > 0.0) && (force[0] > -8000.0))
			force[0] = -8000.0;
		else if((pos[0] < 0.0) && (force[0] < 8000.0))
			force[0] = 8000.0;
		}

	/* get y (side) force for m2 */
	if(behind)
		force[1] = 0.0;	/* no side forces if directly in front or behind. */
	else
		{
		force[1] = rvel[1] * 4000.0;
		if((pos[1] > 0.0) && (force[1] > -8000.0))
			force[1] = -8000.0;
		else if((pos[1] < 0.0) && (force[1] < 8000.0))
			force[1] = 8000.0;
		}

	/* get z (up/down) force for m2 */
	force[2] = rvel[2] * 4000.0;

	/* get forces into correct frmae of ref */
	if(m == m2)	/* forces generated for m2, just copy. */
		{
		for(i=0;i<3;i++)
			m->CENTERFORCE[i] = force[i];
		}
	else		/* apply opposite forces to m1 (m)*/
		{
		for(i=0;i<3;i++)
			force[i] = -force[i];
		fbodtorw(force,temp,m2->reckon.UV[0]);
		rwtobod(temp,m->CENTERFORCE,&m->UV);
		}

	setCollisionDamage(m);
}

#define DAMAGE1_FORCE (40000.0)
#define DAMAGE2_FORCE (155000.0)

void setCollisionDamage(MODELDAT *m)
{
	S16 i,quad,damage,olddamage;
	F32 fsq,f[3],af[3];

	/*figure out quad and damge level*/
	for(fsq=0.0,i=0;i<3;i++)
		{
		f[i] = m->CENTERFORCE[i];
		af[i] = (f[i] < 0.0) ? -f[i] : f[i];
		fsq += (f[i]*f[i]);
		}

	/*determine damage level*/
	if(fsq >= (DAMAGE2_FORCE * DAMAGE2_FORCE))
		damage = 2;
	else if(fsq >= (DAMAGE1_FORCE * DAMAGE1_FORCE))
		damage = 1;
	else
		return;	/*no damage*/

	/*determine which corner*/
	if((f[2] > af[0]) && (f[2] > af[1])) /*top*/
		quad = 4;
	else if(f[0] > 0.0)	/*back*/
		quad = (f[1] > 0.0) ? 3 : 2;
	else			/*front*/
		quad = (f[1] > 0.0) ? 1 : 0;

	/*if more damage, update appearance*/
	olddamage  = (m->appearance & gDamageMask[quad]) >> gDamageShift[quad];
	if(damage > olddamage)
		{
		/*clear quad damage bits*/
		m->appearance &= ~gDamageMask[quad];
		/*set new quad damage bits*/
		m->appearance |= (damage << gDamageShift[quad]);
		}
}

/*dir = vector from our CG to other CG*/
void simpleCollForce(MODELDAT *m, MODELDAT *m2, F32 dir[3])
{
	F32 invdist,vin,force,rvec[3];
	S32 i;


	/*normalize direction to apply force (from other CG to our CG)*/
	invdist = dir[0]*dir[0] + dir[1]*dir[1] + dir[2]*dir[2];
	if(invdist < 0.0001)	/*coincident centers, bounce up*/
		{
		dir[0] = dir[1] = 0.0;
		dir[2] = -1.0;
		invdist = 1.0;
		}
	else
		{
		invdist = 1.0/sqrt(invdist);
		for(i=0;i<3;i++)
			dir[i] = dir[i] * invdist;
		}

	/*get rel velocity between car centers*/
	/*rvec = rel velocity from other CG to our CG*/
	/*vin  = collision velocity along dir*/
	vecsub(m2->reckon.base_RWV,m->reckon.base_RWV,rvec);
	vin = dotprod(rvec,dir);

/*	force = (vin > 0.0) ? (vin + 5.0) * 20000.0 : 0.0;*/
	force = (vin + 5.0) * 80000.0;


	if(force < 0.0)
		force = 0.0;
	else if(force > 100000.0)
		force = 100000.0;


	for(i=0;i<3;i++)
		rvec[i] = force * dir[i];
	rwtobod(rvec,m->CENTERFORCE,&m->UV);
/*	m->CENTERFORCE[1] *= 2.0;*/

}

/*m  = car to set force on.*/
/*m1 = car with corner in.*/
/*m2 = car for bounding box test.*/
void setCenterForce(MODELDAT *m,MODELDAT *m1,MODELDAT *m2, F32 dir[3], F32 pos[3])
{
	F32 del[6],rvec[3],dist,invdist,mul,v1,force;
	S32 i,side,comp;

	/*find closest side (inside = negative)*/
	del[0] = pos[0] - m2->BODYR[0][0];
	del[1] = pos[1] - m2->BODYR[0][1];
	del[2] = -10.0;
	del[3] = m2->BODYR[3][0] - pos[0];
	del[4] = m2->BODYR[3][1] - pos[1];
	del[5] = -10.0;

	/*find closest side*/
	for(dist= -10.0,side=0,i=0;i<6;i++)
		if((del[i] < 0.0) && (del[i] > dist))
			{
			dist = del[i];
			side = i;
			}

	mul = (side < 3) ? 1.0 : -1.0;
	comp = (side < 3) ? side : (side - 3);

	/*normalize direction vector (world coord)*/
	invdist = dir[0]*dir[0] + dir[1]*dir[1] + dir[2]*dir[2];
	if(invdist < 0.0001)	/*coincident centers, bounce up*/
		{
		dir[0] = dir[1] = 0.0;
		dir[2] = -1.0;
		invdist = 1.0;
		}
	else
		{
		invdist = 1.0/sqrt(invdist);
		for(i=0;i<3;i++)
			dir[i] = dir[i] * invdist;
		}

	/*get rel velocity between car centers*/
#if 1
	vecsub(m2->RWV,m->RWV,rvec);
	v1 = dotprod(rvec,dir);
#else
	if(m == m1)
		vecsub(m->RWV,m2->RWV,rvec);
	else
		vecsub(m->RWV,m1->RWV,rvec);
	v1 = -dotprod(rvec,dir);
#endif


	/* get force in driver car frame */
	force = collForceNormal(m,COLL_ALG_CARBODY,dist,v1);
/*	force = collForceNormal(m,COLL_ALG_SOFTWALL,dist,v1);*/
/*	force = collForceNormal(m,COLL_ALG_HARDWALL,dist,v1);*/

	v1 += 5.0;
	force = (v1 > 0.0) ? v1 * m->mass * 50.0 : 0.0;



	for(i=0;i<3;i++)
		pos[i] = force * dir[i];
	rwtobod(pos,m->CENTERFORCE,&m->UV);
}

F32 collForceNormal(MODELDAT *m,U32 coll_alg,F32 din,F32 vin)
{
	F32 force,vout, maxforce;

#if 0
	if(m != &model[gThisNode])
		return(0.0);
#endif

	switch(coll_alg)
		{
		case COLL_ALG_SOFTWALL:
			if(vin < 0.0)	/*compression*/
				force = (din * g_Kcs) - (vin * vin * g_Kcd);
			else		/*rebound*/
				force = (din * g_Krs) + (vin * vin * g_Krd);
			force *= 2.0;
			if(force < 0.0) force = 0.0;	/*only force AWAY*/
			break;
		case COLL_ALG_HARDWALL:
			vout = (din * 4.0) + 1.0; /* 5fps @ 1 foot*/
			force = (vin + vout) * m->mass * 0.5 * m->idt;
			if(force < 0.0) force = 0.0;	/*only force AWAY*/
			break;
		case COLL_ALG_CARBODY:
/*			force = (vin * vin) * m->mass;*/
			if(vin > 0.0)
				force = (vin * m->mass * 0.5 * m->idt);
#if 0
			else
				force = ((din * -5.0) + vin) * m->mass * 0.5 * m->idt;
#endif
/*			force = (vin * m->mass * 0.5 * m->idt) + (din * din * 10000);*/
/*			force = (vin > 0.0) ? 1000.0 * vin * vin : 0.0;*/
			force = 10000.0 * vin;
			if(force < 10000.0) force = 10000.0;
			force = (vin > 0.0) ? 100000 : 0.0;
			force = 50000;
			if(force < 0.0) force = 0.0;	/*only force AWAY*/
			break;
		default:
			force = 0.0;
		}

#if 0
	maxforce = MAXFORCE * m->idt;

	if (force > maxforce)
		force = maxforce;
	else if (force < -maxforce)
		force = -maxforce;
#endif

	return(force);
}

void CollForceMineIn(MODELDAT *m,MODELDAT *m2,F32 vec[3],S32 corner)
{
	F32 del[6],rvec[3],dist,mul,v1,force;
	S32 i,side,dir;

	/*find closest side (inside = negative)*/
	del[0] = vec[0] - m2->BODYR[0][0];
	del[1] = vec[1] - m2->BODYR[0][1];
	del[2] = -10.0;
	del[3] = m2->BODYR[3][0] - vec[0];
	del[4] = m2->BODYR[3][1] - vec[1];
	del[5] = -10.0;

	/*find closest side*/
	for(dist= -10.0,side=0,i=0;i<6;i++)
		if((del[i] < 0.0) && (del[i] > dist))
			{
			dist = del[i];
			side = i;
			}

	mul = (side < 3) ? 1.0 : -1.0;
	dir = (side < 3) ? side : (side - 3);

	/*get rel velocity*/
	vecsub(m->RWV,m2->RWV,vec);
	rwtobod(vec,rvec,&m2->UV);
	v1 = rvec[dir] * mul;	/*velocity IN is negative*/

	/* get force in driver car frame */
	force = collForceNormal(m,COLL_ALG_SOFTWALL,dist,v1);
	for(i=0;i<3;i++)
	    vec[i] = (i == dir) ? (force * mul) : 0.0;
	bodtorw(vec,rvec,&m2->UV);
	rwtobod(rvec,m->BODYFORCE[corner],&m->UV);



}

void CollForceOtherIn(MODELDAT *m,MODELDAT *m2,F32 vec[3],S32 corner)
{
	F32 del[6],rvec[3],vvec[3],dist,mul,v1,force;
	S32 i,side,dir;

	/*find closest side (inside = negative)*/
	del[0] = vec[0] - m->BODYR[0][0];
	del[1] = vec[1] - m->BODYR[0][1];
	del[2] = -10.0;
	del[3] = m->BODYR[3][0] - vec[0];
	del[4] = m->BODYR[3][1] - vec[1];
	del[5] = -10.0;

	/*find closest side*/
	for(dist= -10.0,side=0,i=0;i<6;i++)
		if((del[i] < 0.0) && (del[i] > dist))
			{
			dist = del[i];
			side = i;
			}

	mul = (side < 3) ? 1.0 : -1.0;
	dir = (side < 3) ? side : (side - 3);

	/*get rel velocity !!!MOD FROM MINE IN!!!*/
	vecsub(m2->RWV,m->RWV,vvec);
	rwtobod(vvec,rvec,&m->UV);
	v1 = rvec[dir] * mul;	/*velocity IN is negative*/

	/* get force in driver car frame */
	force = collForceNormal(m,COLL_ALG_SOFTWALL,dist,v1);


if(side == 0)
 distributeForce(-force,vec[1],m->BODYR[0][1],&m->BODYFORCE[0][0],&m->BODYFORCE[1][0]);
else if (side == 1)
 distributeForce(-force,vec[0],m->BODYR[0][0],&m->BODYFORCE[0][1],&m->BODYFORCE[2][1]);
else if (side == 3)
 distributeForce(force,vec[1],m->BODYR[0][1],&m->BODYFORCE[2][0],&m->BODYFORCE[3][0]);
else if (side == 4)
 distributeForce(force,vec[0],m->BODYR[0][0],&m->BODYFORCE[1][1],&m->BODYFORCE[3][1]);
}

void distributeForce(F32 force,F32 offset,F32 width,F32 *fpos,F32 *fneg)
{
	F32 interp;

	interp = (width + offset)/(width * 2.0);
	if(interp > 1.0) interp = 1.0;
	else if(interp < 0.0) interp = 0.0;
	*fpos = interp * force;
	interp = 1.0 - interp;
	*fneg = interp * force;
}


