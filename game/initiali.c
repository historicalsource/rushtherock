/*
 *	initiali.c	-- Functions for model initialization.
 *
 *	Copyright 1996 Time Warner Interactive.
 *	Unauthorized reproduction, adaptation, distribution, performance or 
 *	display of this computer program or the associated audiovisual work
 *	is strictly prohibited.
 *
 *	======================================================================
 *	$Author: gray $						$Date: 1997/05/30 04:05:35 $
 *	$Revision: 3.11 $						$Locker:  $
 *	======================================================================
 *	Change Log:
 *
 *	$Log: initiali.c,v $
 * Revision 3.11  1997/05/30  04:05:35  gray
 * mainout is history!
 *
 * Revision 3.10  1996/12/04  01:57:40  geraci
 * checkpoints and end of game scores are handled by entity state updates
 *
 * Revision 3.9  1996/11/27  14:30:02  gray
 * Dead reckon shadow.
 *
 * Revision 3.8  1996/11/19  18:48:25  gray
 * Different size collision boxes.
 *
 * Revision 3.7  1996/11/10  13:36:47  petrick
 * Made alternate cars use correct models.
 *
 * Revision 3.6  1996/11/07  06:39:10  gray
 * Start of robust model init.
 *
 * Revision 3.5  1996/10/18  22:42:16  petrick
 * Made Car a typedef.
 *
 * Revision 3.4  1996/10/10  05:19:24  petrick
 * Cleaned out some unused globals.
 *
 * Revision 3.3  1996/10/02  07:57:21  gray
 * Made gravity vectors floating point.
 *
 * Revision 3.2  1996/09/13  23:02:27  geraci
 * bumps tied to collision forces, drone can skid
 *
 * Revision 3.1  1996/09/09  02:24:52  gray
 * JAMMA version
 *
 * Revision 2.8  1996/09/04  20:34:54  gray
 * Adding dead reckon data to game_car. Killing mainout.
 *
 * Revision 2.7  1996/09/01  09:18:02  gray
 * Model types cleanup. First pass - removed fptyp.
 *
 * Revision 2.6  1996/07/31  07:57:27  gray
 * Floating point model time.
 *
 * Revision 2.5  1996/07/23  20:46:57  gray
 * Initialize collision bounding box and collision radius for car struct.
 *
 * Revision 2.4  1996/07/16  03:19:58  geraci
 * clears all model data now
 *
 * Revision 2.3  1996/06/20  22:57:10  gray
 * minit_stree back to init_stree.
 *
 * Revision 2.2  1996/05/29  02:21:14  geraci
 * new car bump sounds
 *
 * Revision 2.1  1996/05/04  02:56:18  petrick
 * Focus 2 release
 *
 * Revision 1.15  1996/04/27  02:19:31  geraci
 * fixed bogging and skidding locked sound when aborting/resurrecting
 *
 * Revision 1.14  1996/04/23  21:44:33  petrick
 * Added header.
 *
 *
 *	======================================================================
 */

#include "carsnd.h"
#include "dis.h"
#include "drivsym.h"
#include "modelcom.h"
#include "road.h"
#include "globals.h"
#include "Pro/initiali.pro"
#include "Pro/unitvecs.pro"
#include "Pro/controls.pro"
#include "Pro/vecmath.pro"
#include "Pro/road.pro"
#include "Pro/collision.pro"
#include "Pro/stree.pro"

/*===============================  EXTERNS  =================================*/

extern STREETYP *stree;

/* ----------------------------------------------------------------------------- */
/* Powerup initialization for model code */
void modelinit(void)
{
	init_stree();
}

/* ----------------------------------------------------------------------------- */
void initialize(MODELDAT *m)
{
	extern Car *carlist[];
	extern short num_cars;	/* Number of cars in data base */
	S32 i,j;
	
	m->pitchangle = 0;

	m->torque[0]=m->torque[1]=0;
	m->torque[2]=50; m->torque[3]=50;
	
	m->dt = .0025;

	m->brakeroll = 3;

	m->thetime=0;
	m->airtime=0;
	m->reflecttime = -1;
	
	m->shifttime=0;
	m->dtcycles=0;
	m->iter=0;
	
	m->swampgain = 76.8 * 8;	/* (swtorque output to model)/
					(steering torque ft-lbs) */

/* gauge variables */

	m->amperes=0;
	m->enginetemp=70;
	m->oilpressure=0;
	m->fuel = 8;
/* Collision stuff */
	m->thumpthreshold = 7500;/*20000;*/
	m->lastthump = m->thumpflag = 0;	/* No initial thump */

/* Initial conditions and model vectors */

	m->F[XCOMP]=0;m->F[YCOMP]=0;m->F[ZCOMP]=0;
	m->M[XCOMP]=0;m->M[YCOMP]=0;m->M[ZCOMP]=0;

	m->A[XCOMP]=0;m->A[YCOMP]=0;m->A[ZCOMP]=0;
	m->AA[XCOMP]=0;m->AA[YCOMP]=0;m->AA[ZCOMP]=0;

	m->W[XCOMP]=0;m->W[YCOMP]=0;m->W[ZCOMP]=0;
	m->V[XCOMP]=0;m->V[YCOMP]=0;m->V[ZCOMP]=0;

	m->engangvel = 0;
	
	mkuv(0,0,0,&m->UV);

	if (m->carnum < 0)
		m->carnum = 0;	/* default to beginner_car if invalid */
	m->carnum %= num_cars;
	
	carsetup(m,carlist[m->carnum]);

	initcontrols(m);

	init_road(m);
	init_collision(m);

	m->crashflag = 0;
	m->forceavg = 0;
	m->bog_state = 3;

	InitCarSnds(m->net_node);

	if (m->net_node == gThisNode)
		{
		init_bump_sounds();
		}
}


/* ----------------------------------------------------------------------------- */
/* This routine sets up all the car dependent parameters based on
a pointer to a car structure */

void carsetup(MODELDAT *m, Car *carp)
{
	extern COLLSIZE collsize[];
	COLLSIZE *sizep;
	int i, j;
	F32 maxlen;

	m->mass=carp->mass;
	m->massinv = 1.0/carp->mass;
	m->weight = m->mass * GRAVCON;

	veccopy(carp->I,m->I);

	m->INVI[XCOMP] = 1/m->I[XCOMP];	/* 1/roll moment of inertia  */
	m->INVI[YCOMP] = 1/m->I[YCOMP];	/* 1/pitch moment of inertia  */
	m->INVI[ZCOMP] = 1/m->I[ZCOMP];	/* 1/yaw moment of inertia */

/* aerodynamic drag and rolling resistance */

	m->srefpcybo2 = carp->srefpcybo2;
	m->rollresist = carp->rollresist;	/* 6 hp at 60 mph   */

	m->wheelbase = carp->TIRER[0][XCOMP] - carp->TIRER[2][XCOMP];	

	sizep = &collsize[m->body_type];
	m->BODYR[0][0] = m->BODYR[1][0] = sizep->colfront;	/* Front X */
	m->BODYR[2][0] = m->BODYR[3][0] = -sizep->colrear;	/* Back X */
	m->BODYR[0][1] = m->BODYR[2][1] = sizep->colside;	/* Right Y */
	m->BODYR[1][1] = m->BODYR[3][1] = -sizep->colside;	/* Left Y */
	m->BODYR[0][2] = m->BODYR[1][2] = m->BODYR[2][2] = m->BODYR[3][2] = -sizep->colheight;	/* Top Z */
	maxlen = (sizep->colfront > sizep->colrear) ? sizep->colfront : sizep->colrear;
	m->colrad = sqrt( (maxlen * maxlen) +
			(sizep->colside * sizep->colside) +
			(sizep->colheight * sizep->colheight) );

	for(i=0;i<4;++i){
		/* these are the distances from the CG to each wheel */

		veccopy(carp->TIRER[i],m->TIRER[i]);

		if(i < 2)
			copy_tire_info(m,&carp->tires[i],&m->tires[i],
				-carp->TIRER[i+2][XCOMP],m->wheelbase);
		else
			copy_tire_info(m,&carp->tires[i],&m->tires[i],
				carp->TIRER[i-2][XCOMP],m->wheelbase);
			
		tire_constants(&m->tires[i]);	/* Calculate dependent tire cnstnts */
		m->tires[i].angvel = m->V[XCOMP]/m->tires[i].tradius;
		
		m->springrate[i] = carp->springrate[i];
		m->cdamping[i] = carp->cdamping[i];
		m->rdamping[i] = carp->rdamping[i];

		/* calculate amount springs compressed on level ground */
		m->rideheight[i] = -m->tires[i].Zforce/m->springrate[i];
		m->suscomp[i]=0;	/* suspension compression */
		m->TIREV[i][XCOMP] = 0;
		m->TIREV[i][YCOMP] = 0;
		m->TIREV[i][ZCOMP] = 0;
	}
	
/* brake balance */
/* This algorithm always gives full braking force to the front
and a percentage of that to the rear. Weird cars beware!! */
	
	m->brakegain[0]= m->mass*GRAVCON*m->tires[0].Cfmax*1.1*.5;
	m->brakegain[1]= m->mass*GRAVCON*m->tires[1].Cfmax*1.1*.5;
	m->brakegain[2]= m->mass*GRAVCON*m->tires[2].Cfmax*1.1*.5*
		(1.0-carp->brakebal)/carp->brakebal;
	m->brakegain[3]= m->mass*GRAVCON*m->tires[3].Cfmax*1.1*.5*
		(1.0-carp->brakebal)/carp->brakebal;

	
/* engine variables */
	m->enginvmi=1.0/carp->engmi;
	
/* These are the variables relating to the differential and driving wheels*/

/* 1/radius of driving wheels */
	m->dwinvradius = 2.0/(m->tires[2].tradius+m->tires[3].tradius);	
 	m->dwratio=carp->dwratio;		/* Differential gear ratio */
	m->dwinvmi= m->tires[2].invmi/2;/* 1/moment of inertia of driving wheels */

	m->nothrusttorque = carp->nothrusttorque;
	m->magicdif = carp->magicdif;
	
	m->fgtorquescale = carp->fgtorquescale;
	m->sgtorquescale = carp->sgtorquescale;
	m->torquescale = carp->torquescale;
	m->dirttorquescale = carp->dirttorquescale;
	
/* Torque curves and transmission ratio */
	
	m->transarray = carp->transarray;
	m->torquecurve = (short *)carp->torquecp;
	m->dirttorquecurve = (short *)carp->dirttorquecp;
	m->rpmperent = carp->rpmperent;
	m->topgear = carp->topgear;
	m->upshiftangvel = carp->upshiftangvel;
	m->downshiftangvel = carp->downshiftangvel;
	
/* Anti roll bar spring rates */
	m->farspringrate = carp->farspringrate;
	m->rarspringrate = carp->rarspringrate;
	
/* Steering wheel calculations */
	m->steerratio = carp->steerratio;
	m->swtpg = carp->swtpg;
	
	m->maxswfrict = carp->maxswfrict;
	m->minswfrict = carp->minswfrict;
	
	m->maxswdamp = carp->maxswdamp;
	
	steer_constants(m);

/* Clutch variables */

	m->clutchmaxt=carp->clutchmaxt;	/* maximum torque with clutch fully engaged */

	m->viewheight = carp->viewheight;	/* height from CG to eye level */
	
	m->GRW[XCOMP]=m->GRW[YCOMP]=0.0;
	m->GRW[ZCOMP]= m->mass * GRAVCON;
	
/* This is for the automatic transmission */
	m->commandgear = m->gear = m->mainin.gear = 0;
	m->autotrans = 1;	/* Default to automatic */

	m->idt = 0;
	m->fastin.modeltime = 0.0;

	for (i=0; i<3; i++)
		{
		m->D[i] = 0;
		m->G[i] = 0.0;
		m->CENTERMOMENT[i] = 0;

		for (j=0; j<4; j++)
			m->TIREFORCE[j][i] = 0;
		}

	m->brake = 0;
	m->engtorque = 0;

	m->clutchtorque = 0;
  	m->clutchangvel = 0;
  	m->transratio = 0;
  	m->totalratio = 0;

  	m->dwtorque = 0;
  	m->efdwinvmi = 0;
  	m->dwangvel = 0;

  	m->ignition = 0;
  	m->startermotor = 0;
  	m->swtorque = 0;
  	m->swdamp = 0;
  	m->swfrict = 0;
}

/* ----------------------------------------------------------------------------- */
void steer_constants(MODELDAT *m)
{
	m->steergain = ((360*1.5)/m->steerratio)*degtorad;

	m->trail = (m->swtpg * m->steerratio) / (m->weight * (m->TIRER[2][XCOMP]/m->wheelbase));
	
	m->swtorquegain = (m->trail * m->swampgain)/m->steerratio;
}


/* ----------------------------------------------------------------------------- */
/* otw	- width to other tire
 * wb	- wheelbase */
void copy_tire_info(MODELDAT *m, struct tiredes *tp1, struct tiredes *tp2, 
					F32 otw, F32 wb)	
{
	tp2->tradius = tp1->tradius;
	tp2->springK = tp1->springK;
	tp2->rubdamp = tp1->rubdamp;
	tp2->Cstiff = tp2->PaveCstiff = tp1->PaveCstiff ;
	tp2->Cfmax = tp2->PaveCfmax = tp1->PaveCfmax;
	tp2->invmi = tp1->invmi;
	tp2->Zforce = (m->mass*GRAVCON*otw/wb)*.5;
}

/* ----------------------------------------------------------------------------- */
void tire_constants(struct tiredes *tdes)
{
	tdes->Afmax = 3*tdes->Cfmax*tdes->Zforce/tdes->Cstiff;
	tdes->k1 = tdes->Cstiff/tdes->Zforce;
	tdes->k2 = tdes->k1*tdes->k1/(3*tdes->Cfmax);
	tdes->k3 = tdes->k1*tdes->k1*tdes->k1/(27*tdes->Cfmax*tdes->Cfmax);
	tdes->l2 = tdes->k2*2;
	tdes->l3 = tdes->k3*3;
	tdes->m1 = 40*.052/(tdes->Cfmax/2);
	tdes->m2 = tdes->m1*tdes->m1/3.4;
	tdes->m3 = tdes->m1*tdes->m1*tdes->m1/46.3;
	tdes->m4 = 1.0/.000055;
 	tdes->patchy = 0;
}


