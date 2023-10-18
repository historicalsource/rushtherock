
#ifndef __MODEL_H
#define __MODEL_H

#include "stdtypes.h"

#ifdef DEFINE_GLOBALS
#define OPEX
#else
#define OPEX extern
#endif

#define CTLSCALE (1.0/0x1000)	/* 1.0 for control inputs */
#define INVCTLSCALE 0x1000	/* 1.0 for control inputs */
#define CTLSHIFT 12		/* Amount to shift to mult by invctlscale */

/* These are the array elements for X, Y, and Z components */
#define XCOMP 0
#define YCOMP 1
#define ZCOMP 2

/* These are the model tires index */
/* FR=front_right, FL=front_left, RR=rear_right, RL=rear_left */
#define FRTIRE 0
#define FLTIRE 1
#define RRTIRE 2
#define RLTIRE 3


#define VECSUB(a,b,r) {r[0]=a[0]-b[0]; r[1]=a[1]-b[1]; r[2]=a[2]-b[2];}
#define VECCOPY(a,r) {r[0]=a[0]; r[1]=a[1]; r[2]=a[2];}


#define DEFCRASHTHRESH 3900 /*5200*/	/* Default crash threshold, tens of pounds */
#define TICSPERSEC 1000	/* This is the number of interrupts per second */




OPEX short amp,tmp,gas,oil;
OPEX short rpm;			/* Engine rpm from model */ 



/************************************************/
/* GLOBAL DATA INITIALLY DECLARED IN FORCEWHE.C */
/************************************************/
OPEX S16 rawswvel;
OPEX unsigned short modelrun;
/*OPEX short crashflag;*/
OPEX short crashthreshold;		/* Crash threshold, tens of pounds */

/************************************************/
/* GLOBAL DATA INITIALLY DECLARED IN DSKCOMM.C (model.h) */
/************************************************/

/* This value is a function of the velocity squared */
OPEX unsigned int velsq;

/* These are variables for each of the control inputs.
They are scaled with 1.0 = INVCTLSCALE */

OPEX S16	wheel,brake,clutch,clutchdepressed,gear/*,ithrottle*/;
OPEX S16	ignition,startermotor;
OPEX U16	carnum;		/* Car number */
OPEX S16	autotrans;		/* Automatic transmission flag */

/* These are the variables for the force feedback steering wheel */
OPEX S16	gSWtorque,gSWfrict,gSWdamp;

#include "Pro/model.pro"

#endif
