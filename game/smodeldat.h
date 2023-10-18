/*
 *	modeldat.h	-- Structures used for model and game version of car data
 *
 *	======================================================================
 *	$Author: gray $						$Date: 1997/05/11 07:55:11 $
 *	$Revision: 3.31 $						$Locker:  $
 *	======================================================================
 *	Change Log:
 *
 *	$Log: modeldat.h,v $
 * Revision 3.31  1997/05/11  07:55:11  gray
 * Removed uvs from game car data (now everything uses dr_uvs).
 *
 * Revision 3.30  1997/05/11  04:37:27  gray
 * Cleaning model interface.
 *
 * Revision 3.29  1997/05/11  03:37:57  gray
 * Cleaning model interface.
 *
 * Revision 3.28  1997/02/05  19:32:31  geraci
 * drone difficulty
 *
 * Revision 3.27  1996/12/13  12:06:20  geraci
 * more new lap stuff
 *
 * Revision 3.26  1996/12/13  08:15:46  geraci
 * new lap stuff
 *
 * Revision 3.25  1996/12/05  00:02:58  geraci
 * end of game score removed from entity state
 *
 * Revision 3.24  1996/12/04  01:57:03  geraci
 * checkpoints and end of game scores are handled by entity state updates
 *
 * Revision 3.23  1996/11/27  16:35:10  geraci
 * each car has its own tunnel flag
 *
 * Revision 3.22  1996/11/27  14:30:02  gray
 * Dead reckon shadow.
 *
 * Revision 3.21  1996/11/27  12:56:10  gray
 * Changed NetTime from F32 to 10microsec S32.
 *
 * Revision 3.20  1996/11/22  05:00:07  geraci
 * hide resurrecting car defined in look field, quats passed instead of uvs
 *
 * Revision 3.19  1996/11/20  21:14:27  geraci
 * new view 2 collision stuff
 *
 * Revision 3.18  1996/11/19  18:48:25  gray
 * Different size collision boxes.
 *
 * Revision 3.17  1996/11/16  16:07:26  geraci
 * stop car was not needed
 *
 * Revision 3.16  1996/11/16  12:59:13  geraci
 * added variable to stop car when necessary
 *
 * Revision 3.15  1996/11/07  05:42:02  geraci
 * added weight index for drone path weight selection
 *
 * Revision 3.14  1996/11/04  05:08:52  geraci
 * added default path to maxpath struct
 *
 * Revision 3.13  1996/11/04  05:07:02  gray
 * Removed unused R[3] from MODELDAT.
 *
 * Revision 3.12  1996/10/31  06:55:32  geraci
 * car data is in upper case now
 *
 * Revision 3.11  1996/10/24  07:35:52  geraci
 * added index for multiple maxpath tables
 *
 * Revision 3.10  1996/10/23  02:50:53  gray
 * Added susvel[4] (suspension velocity).
 *
 * Revision 3.9  1996/10/02  07:57:21  gray
 * Made gravity vectors floating point.
 *
 * Revision 3.8  1996/09/23  10:02:17  gray
 * Fixing dead reckon control.
 *
 * Revision 3.7  1996/09/20  02:08:14  geraci
 * new shortcut code histerisis time
 *
 * Revision 3.6  1996/09/19  11:15:47  geraci
 * new field for maxpath entry point of checkpoint
 *
 * Revision 3.5  1996/09/18  10:12:12  geraci
 * added game version of sound and short cut codes and short cut data
 *
 * Revision 3.4  1996/09/18  01:46:33  gray
 * Made model.idt F32 (was S32).
 *
 * Revision 3.3  1996/09/17  00:58:40  gray
 * New roaddat structure. Passing roadcode,sviscode,roadboost,sound_flags, and shortcut_flags.
 *
 * Revision 3.2  1996/09/13  23:03:15  geraci
 * bump state taken out of model structure
 *
 * Revision 3.1  1996/09/09  02:23:24  gray
 * JAMMA version
 *
 * Revision 2.32  1996/09/06  21:44:46  geraci
 * we didn't need a game version of can we cheat
 *
 * Revision 2.31  1996/09/06  20:55:58  geraci
 * oops, that's better
 *
 * Revision 2.30  1996/09/06  20:54:03  geraci
 * can we cheat for drone cheating is new
 *
 * Revision 2.29  1996/09/05  01:45:35  gray
 * Passing airdist (for shadow) across link.
 *
 * Revision 2.28  1996/09/05  00:09:33  gray
 * Backed out xyzv mod, evil audio bug.
 *
 * Revision 2.27  1996/09/04  23:32:27  gray
 * Removed xv, yv, zv from mainout.
 *
 * Revision 2.26  1996/09/04  20:32:33  gray
 * Adding dead reackoned data to car_data. Killing mainout.
 *
 * Revision 2.25  1996/09/01  23:56:16  gray
 * Added real world acceleration for reckon. Added real reckon struct.
 *
 * Revision 2.24  1996/09/01  11:02:28  gray
 * Trimming dead tissue.
 *
 * Revision 2.23  1996/09/01  09:18:02  gray
 * Model types cleanup. First pass - removed fptyp.
 *
 * Revision 2.22  1996/08/30  16:25:36  geraci
 * lap holdoff not needed any more
 *
 * Revision 2.21  1996/08/27  05:32:23  geraci
 * cleaned up some unused variables
 *
 * Revision 2.20  1996/08/20  19:45:23  gray
 * Preliminary dead reckoning.
 *
 * Revision 2.19  1996/08/09  04:49:03  geraci
 * time base and time base valid are used for superior dead reckoning
 *
 * Revision 2.18  1996/08/07  20:36:32  geraci
 * game version of road normals.  time fudge factor added for dead reckoning
 *
 * Revision 2.17  1996/08/02  21:34:24  geraci
 * temp fix for lap unsynced problem
 *
 * Revision 2.16  1996/08/02  05:34:11  geraci
 * new flag to help model task to sync to number of laps
 *
 * Revision 2.15  1996/07/31  15:57:13  geraci
 * new time boost for faster cars
 *
 * Revision 2.14  1996/07/31  07:58:33  gray
 * Floating point model time.
 *
 * Revision 2.13  1996/07/29  09:47:29  geraci
 * added new maxpath index to resync maxpath from game task
 *
 * Revision 2.12  1996/07/25  09:17:51  geraci
 * moved forward progress and last distance to game data structure
 *
 * Revision 2.11  1996/07/23  20:48:21  gray
 * Added colrad (collision radius) to modeldat struct.
 *
 * Revision 2.10  1996/07/20  04:41:19  geraci
 * moved the mpctl structure to here
 *
 * Revision 2.9  1996/07/13  06:59:58  geraci
 * added variables for wrong-way indication
 *
 * Revision 2.8  1996/07/09  04:16:02  geraci
 * rpm fudge for cars in lead
 *
 * Revision 2.7  1996/06/28  05:13:44  geraci
 * new model flag to indicate when drone wants to be aborted
 *
 * Revision 2.6  1996/06/12  06:51:22  geraci
 * added mph to game data
 *
 * Revision 2.5  1996/05/29  02:20:54  geraci
 * new car bump sounds
 *
 * Revision 2.4  1996/05/24  22:14:11  geraci
 * added save area for coords of car when it died
 *
 * Revision 2.3  1996/05/21  19:51:36  geraci
 * quaternions added to resurrection structure
 *
 * Revision 2.2  1996/05/08  21:36:12  geraci
 * removed old screech code
 *
 * Revision 2.1  1996/05/04  02:56:18  petrick
 * Focus 2 release
 *
 * Revision 1.53  1996/05/01  19:11:35  gray
 * Added airdist to MODELDAT for shadow placement.
 *
 * Revision 1.52  1996/04/30  03:38:18  petrick
 * Added network wheel spinning code.
 *
 * Revision 1.51  1996/04/27  05:03:07  petrick
 * Added world V to car_data
 *
 * Revision 1.50  1996/04/23  22:39:31  gray
 * Add CENTERMOMENT to MODELDAT for anti-spin stuff.
 *
 * Revision 1.49  1996/04/19  22:55:30  geraci
 * added game version of tire position also some stuff for new resurrection
 *
 * Revision 1.48  1996/04/16  23:44:03  petrick
 * Got rid of ckpt and score and returned to 2 queues.
 *
 * Revision 1.47  1996/04/12  00:20:00  geraci
 * remove load and dummy from model and use engtorque for load
 *
 * Revision 1.46  1996/04/11  01:55:38  geraci
 * changed comment of drone type field
 *
 * Revision 1.45  1996/04/04  00:42:11  geraci
 * added thump side
 *
 * Revision 1.44  1996/04/03  10:29:59  gray
 * Added CENTERFORCE to MODELDAT for forces through center of gravity.
 *
 * Revision 1.43  1996/04/02  16:20:32  geraci
 * added drone scale for drone pacing
 *
 * Revision 1.42  1996/04/02  10:26:43  petrick
 * Added checkpoint and score fields.
 *
 * Revision 1.41  1996/04/02  10:25:43  geraci
 * added stuff for drone pacing
 *
 * Revision 1.40  1996/04/01  13:53:46  geraci
 * added more collision stuff
 *
 * Revision 1.39  1996/03/30  08:16:11  petrick
 * Top scrape added.
 *
 * Revision 1.38  1996/03/30  02:55:50  geraci
 * added collidable field
 *
 * Revision 1.37  1996/03/28  12:38:59  gray
 * Made 1.0 = full catchup.
 *
 * Revision 1.36  1996/03/27  03:13:35  geraci
 * added stuck time for drones to abort
 *
 * Revision 1.35  1996/03/26  23:14:57  geraci
 * added drone type to model
 *
 * Revision 1.34  1996/03/26  04:08:41  petrick
 * Got rid of some unused variables.
 *
 * Revision 1.33  1996/03/23  02:14:01  geraci
 * added tick time (maybe should be dt
 *
 * Revision 1.32  1996/03/21  05:27:31  petrick
 * Added blastIndex.
 *
 * Revision 1.31  1996/03/20  03:51:15  geraci
 * added we died and just crashed
 *
 * Revision 1.30  1996/03/16  01:51:13  geraci
 * added score
 *
 * Revision 1.29  1996/03/12  20:38:08  geraci
 * added place
 *
 * Revision 1.28  1996/03/12  10:02:04  geraci
 * added time stamp to game_car
 *
 * Revision 1.27  1996/03/12  07:41:13  geraci
 * relative distances between players
 *
 * Revision 1.26  1996/03/12  05:20:01  gray
 * Added BODYR and BODYRWR to modeldat.
 *
 * Revision 1.25  1996/03/09  07:33:37  petrick
 * Added objnum to car_data.
 *
 * Revision 1.24  1996/03/09  07:28:42  geraci
 * added timestamp and net_node
 *
 * Revision 1.23  1996/03/08  13:08:36  geraci
 * more finish line stuff
 *
 * Revision 1.22  1996/03/08  10:26:09  geraci
 * added checkpoint stuff
 *
 * Revision 1.21  1996/03/08  06:16:26  geraci
 * added bogged down engine sound
 *
 * Revision 1.20  1996/03/07  23:48:41  geraci
 * moved rpm, load, appearance and engine_type to same place in model struct
 *
 *
 *	======================================================================
 */

#ifndef __MODELDAT_H
#define __MODELDAT_H

#include "vecmath.h"
#include "eptyp.h"
#include "tiredes.h"
#include "road.h"
#include "stree.h"
#include "visuals.h"

/* NOTE: All data in model coordinates. */
typedef struct reckontyp
{
	/* Base Data used to compute Dead Reckoned Data */
	S32	base_time;	/* Dead Reckon Base: ElapsedTime (10 microseconds = lsb) */
	F32	base_fudge;	/* Dead Reckon Base: Time Fudge */
	F32	base_RWA[3];	/* Dead Reckon Base: Real World Acceleration (ft/(sec*sec)) */
	F32	base_RWV[3];	/* Dead Reckon Base: Real World Velocity     (ft/sec) */
	F32	base_RWR[3];	/* Dead Reckon Base: Real World Position     (ft) */
	F32	base_W[3];	/* Dead Reckon Base: Angular Velocity        (radians/sec) */
	F32	base_UV[3][3];	/* Dead Reckon Base: Rotation Matrix         (unit length matrix) */
	F32	base_airdist[4];/* Dead Reckon Base: Shadow Distance		(ft) */
	F32	base_airvel[4];	/* Dead Reckon Base: Shadow Velocity		(Ft/sec) */
	F32 base_quat[4];   /* Dead Reckon Base: quaternion base */
	F32 delta_quat[4];  /* delta to change quat by */
	S16 num_quat_steps; /* number of steps left to get to desired quat */
	/* Data Updated on change */
	S16	torque;		/* Integer Engine Torque	(ft*lbs)*/
	U16	rpm;		/* Engine RPM */
	F32	steerangle;	/* Front Tire Steering angle 		(rad) */
	F32	tireW[4];	/* Rolling Velocity of each tire	(rad/sec)*/
	F32	suscomp[4];	/* Suspension Compression of each tire	(ft) */
	F32	airdist[4];	/* Vertical Dist. to Road for each tire (ft) */
	U32	look;		/* Engine type (spare) */
	U32	appearance;	/* Visual appearance flags. */
	/* Dead Reckoned Data */
	F32	RWV[3];		/* Dead Reckoned: Real World Velocity	(ft/sec) */
	F32	RWR[3];		/* Dead Reckoned: Real World Position	(ft) */
	F32	UV[3][3];	/* Dead Reckoned: Real Rotation Matrix	(unit length matrix) */
} RECKON;

/* maxpath model structure */
typedef struct mpctltyp
{
	S32 mpi;			/* index of current maxpath point */
	F32 xrel;			/* left/right offset to next point (in car reference) */
	F32 yrel;			/* forward/backward offset */
	F32 cyrel;			/* rotation to next point */
	F32 len;			/* distance to next point */
	F32 tgtspd;			/* interpolated speed required for current spot */
	F32 tgtpos[3];		/* direction to steer to get to target point */
	S32 interval_time;	/* time that car has been at current maxpath point */
	S16 new_mpi;		/* maxpath point to update to on next model slice */
	S16 mpath_index;	/* current maxpath table being used */
	S16 default_path;	/* default path that drone tries to stay on */
} MPCTL;

/* game task's version of all drone and model data */
typedef struct CAR_DATA
{
	/* dead reckoned, display coordinates*/
	F32 dr_pos[3];		/*dead reckoned position*/
	F32 dr_vel[3];		/*dead reckoned velocity*/
	F32 dr_uvs[3][3];	/*dead reckoned orientation*/
	F32 dr_tirepos[4][3];	/*dead reckoned tire position*/

	F32 dr_acc[3];		/*reckon base acceleration*/
#if 0
	F32		pos[3];			/* Position */
	F32		vel[3];			/* Velocity */
	F32		acc[3];			/* Acceleration */
	F32		angv[3];		/* Angular velocity */
	F32		uvs[3][3];		/* UV orientation array */
	uvect 	UV;
#endif
	F32 	RWV[3];
	F32     RWR[3];

#if 1
	F32		V[3];			/* Velocity in world coordinates. */
#endif
	F32	TIRERWR[4][3];	/* tire positions, real world coordinates */
	F32	TIRER[4][3];	/* tire positions, body coordinates */
	S16	 	rpm;
	S16		rwmph;			/* mph */
	S16		engine_type;	/* Engine type for sound model */
	S16		body_type;		/* Car body style. */
	U32		appearance;		/* Appearance of car (smoke, damage, etc) */ 
	BOOL	data_valid;		/* Set when data has been updated at least once */
	S8		place;			/* place/position in race (first, second etc.) */
	S8		place_locked;	/* has end-of-race place been locked? */
	U32		score;			/* player score when passes finish line */
	S32		objnum;			/* ZOID object number for this object. */
	F32		distance;		/* distance from start of race */
	S8		we_died;		/* if set, then this car should be resurrected */
	S8		just_crashed;	/* indicates that car is in its death throes */
	U32		crashtime;		/* time stamp for crash timer */
	Visual	visuals[NUM_VISUALS];	/* Visual effects to overlay on car. */
	S16		collidable;		/* true when drone/car is collidable */
	U32		collide_time;	/* time-stamp used when car in not collidable */
	S8		collide_count;	/* count down to next state */
	S8		collide_state;	/* state of trasparency */
	MPCTL	mpath;			/* game version of maxpath data */
	S32		fwd_progress_t; /* time stamp to determine when car needs help arrows */
	F32		last_distance;	/* previous forward progress marker */
	U16		sound_flags, shortcut_flags;
	S32		sound_flag_time;
	S32		scut_flag_time;
	S16		old_shortcut;	/* value of last shortcut section (-1 = none) */
	F32		shortcut_pos[3];/* entry point into shortcut section */
	S16		last_mpath;		/* last maxpath index before short cut */
	S16		weight_index;	/* slot to use for path weighting */
	S8		difficulty;		/* dynamicly assigned drone difficulty setting */
	F32		road_norm[4][3];/* road normals for shadow */
	S8		in_tunnel;		/* car is in a tunnel */
	S8		laps;			/* game version of current lap */
	S8		mpath_laps;		/* game maxpath version of lap */
	S8		syncmpath_laps;	/* maxpath laps should match this value eventually */
	S8		checkpoint;		/* game version of current checkpoint */
	S32		lap_sync_time;	/* duration of lap and maxpath lap num not matching */
} CAR_DATA;

/* I/O structures for model (used to be host/model DSP I/O) */

typedef struct fastouttyp {
	S16 	swtorque;		/* Steering wheel torque */
	S16 	swdamp;			/* Steering wheel damping */
	S16 	swfrict;		/* Steering wheel friction */
} fastouttyp;


typedef struct mainouttyp {
#if 1
	S16	 	caruvs[3][3];	/* Unit vectors for car */
#endif
	S16	 	xv;				/* X, Y, and Z velocities */
	S16	 	yv;				/* (S4SCALE*VELSCALE)/TICKTIME */
	S16	 	zv;	
	S16	 	rollv;			/* roll, pitch, yaw velocities */
	S16	 	pitchv;			/* sdeg/TICKTIME */
	S16	 	yawv;
	S16	 	rwmph;			/* Rear wheel speed, 1 mph=MPHSCALE */
	S16	 	crashflag;		/* flag we've crashed */
	S16	 	ftroadcode;		/* front tire road code */
	S16	 	rtroadcode;		/* rear tire road code */
	S16	 	thumpflag;		/* Flag that we should thump */
} mainouttyp;


typedef struct fastintyp {
	S32		lasttime;		/* time of last model iteration (microsec / 10) */
	F32		modeltime;		/* Model Iteration time (seconds) */
	S16 	wheel;			/* Wheel position -1.0 to 1.0 */
	S16 	swvel;			/* Steering wheel velocity */
	S16 	modelrun;		/* Stall model if zero */
} fastintyp;


typedef struct mainintyp {
	S16		reinit;			/* Re-initialize model if non-zero */
	S16 	clutch;			/* Clutch position 0 to 1.0 */
	S16 	brake;			/* Brake position 0 to 1.0 */
	S16 	throttle;		/* Throttle position 0 to 1.0 */
	S16 	gear;			/* Current gear 0=neutral */
	S16 	autotrans;		/* 1 if automatic trans, 0 manual */
	S16 	ignition;		/* 1 if ignition on, 0 ignition off, 2 end-game */

	S16 	startermotor;	/* Non zero if starter engaged */
	S16 	crashthreshold;	/* Crash threshold, tens of pounds */
} mainintyp;


typedef struct initintyp {
	S16 	polepos;		/* Start line position (0-7) */
	F32 	pos[3];			/* Start line universe XYZ position */
	F32 	uvs[3][3];		/* Start line unitvecs */
	F32 	offset[3];		/* Offset from start pos */
} initintyp;


typedef struct resurrecttyp
{
	F32  	pos[3];			/* resurrection point/reference point */
	F32		save_pos[3];	/* point at which crash occurred */
	F32  	uvs[3][3];		/* resurrection uvs */
	F32		quat_start[4];	/* quat from which to generate car uvs at beginning of resurrection */
	F32		quat_end[4];	/* ending quat */
	S32  	velocity;		/* reference velocity */
	U32  	crash_time;		/* time stamp of crash time (if 0, just use pos */
	S16		moving_state;	/* set when car is being moved to new location */
	U32		resurrect_time; /* time stamp of when resurrection started */
} resurrecttyp;


typedef struct mdattyp {
/* Body Axis Variables */
	F32			F[3],M[3],A[3],AA[3],W[3],V[3],D[3];
	F32 		TIREFORCE[4][3],TIRER[4][3],TIREV[4][3];
	F32 		BODYFORCE[4][3],BODYR[4][3];
	F32			peak_body_force[2][3];
	F32			CENTERFORCE[3],CENTERMOMENT[3];
	F32			peak_center_force[2][3];
	uvect 		tireuvs[4];
	F32 		G[3];
/* Real World Axis Variables */
	F32 		RWA[3],RWV[3],RWR[3];
	F32			TIRERWR[4][3],BODYRWR[4][3];
	EPTYPE 		EPRWR[3];
	F32 		GRW[3];
	uvect 		UV;
/* Scalar Vechicle Variables */
	F32 		steerangle,steerratio,torque[4];
	F32 		steergain,swtpg,trail,swtorquegain,clutch,throttle;
	F32 		brake,brakeroll,brakegain[4],pitchangle,magvel;
	S16 		gear,commandgear,autotrans,ignition,startermotor;
	S32 		swtorque,swdamp,swfrict,maxswdamp,maxswfrict,minswfrict;
	S16 		carnum;	/* car model type (beginner,advanced,expert,extreme) */
/* These are scalars relating to the engine and drivetrain */
	F32			catchup;			/* CATCHUP. 1.0=full catchup,0.5= 1/2 torque,etc. */
	F32 		engtorque;			/* torque produced by engine in ft/lbs */
	F32 		enginvmi;			/* 1/moment of inertia of flywheel */
	F32 		engangvel;			/* rotational speed of engine, rad/sec */
/* These are the variables relating to the differential and driving wheels */
	F32 		dwratio;			/* Differential gear ratio */
	F32 		dwtorque;			/* Torque at driving wheels */
	F32 		dwcf;				/* Coefficient of friction of driving wheels */
	F32 		dwzforce;
	F32 		dwinvmi;			/* 1/moment of inertia of driving wheels */
	F32 		efdwinvmi;			/* 1/effective moment of inertia, dwheels */
	F32 		dwangvel;			/* Rotational velocity of driving wheels */
	F32 		dwinvradius;		/* 1/radius of driving wheels */
	F32 		dwsliptorque;		/* torque from slipping rear wheels */
	F32 		fgtorquescale;		/* torque scale in first gear */
	F32 		sgtorquescale;		/* torque scale in second gear */
	F32 		torquescale;		/* torque scale if gear > 2 */
	F32 		dirttorquescale;	/* torque scale off road */
	S16 		dwslipflag;			/* flag that tire is slipping */
	F32 		roadtorque;			/* This is the torque that the driving wheels apply to the road. */
	S08 		nothrusttorque;		/* Flag for zero yaw torque from thrust */
	S08 		magicdif;			/* Flag for magic load sensitive torque split */
/* Clutch variables */
	F32 		clutchmaxt;			/* maximum torque with clutch fully engaged */
	F32 		clutchtorque,clutchangvel;
/* Transmission variables */
	F32 		transratio;			/* Transmission gear ratio */
	F32 		totalratio;			/* Total gear ratio including differential */
	S16 		*torquecurve;		/* Torque curve on pavement */
	S16 		*dirttorquecurve;	/* Torque curve on dirt */
	F32 		*transarray;		/* Transmission ratio */
	S16 		rpmperent;			/* Rpm per entry in torque curve */
	S16 		topgear;			/* Top gear for automatic transmission */
	F32 		upshiftangvel;		/* Ang velocity for auto trans upshift */
	F32 		downshiftangvel;	/* Ang velocity for auto trans downshift */
/* These are the tire descriptions for each tire */
    tiredes 	tires[4];
/* These are the variables displayed on the gauges on the dashboard */
	F32 		amperes,enginetemp,oilpressure,fuel;
/* Car and world constants */
	F32 		mass,massinv,weight,wheelbase;
	F32 		springrate[4],cdamping[4],rdamping[4],rideheight[4],suscomp[4];
	F32 		tpcomp[4],airdist[4],airvel[4];
	F32 		farspringrate,rarspringrate;
	uvect 		roaduvs[4];			/* unit vectors for road sections */
	S32 		roadcode[4];	/* determines torque table */
	U16			sviscode[4];	/* surface visuals (DIRT,WATER,PAVE,etc.)*/
	U16			roadboost[4];	/* 4-bit boost value (0=none, 15=max) */
	U16			sound_flags[4],shortcut_flags[4];
	S16 		reinit,gameover;
	STREETYP 	*lasttp[4];
	F32 		swampgain;
	F32 		yawdampfix,reflecttime;
	F32 		thetime,airtime,dt,idt,minspeed;
	S32 		dtcycles;
	F32 		I[3],INVI[3];
	F32 		srefpcybo2,rollresist;	
	S32 		iter;
	S32 		crashflag;
	S32 		crashthreshold;		/* Minimum force for crash */
	BOOL		topscrape;			/* Set when top scraping across ground. */
	S32 		thumpflag;			/* Flag that we should thump */
	F32 		thumpthreshold;		/* Minimum force for thump */
	S16			thump_side;			/* -1 = left, 0 = middle, 1 = right */
	F32 		forceavg;			/* Force average for crash detect */
	F32 		swhpitch,viewheight;
	F32 		colrad;	/* collision radius */
/* Scattered Vars */
	F32 		shifttime;			/* In automatic transmission mode, this is
									   the time after which the next shift is allowed */
	S16	 		bog_state;			/* 0=no bogging, 1=start bog, 2=bogging, 3=stop bogging */
	resurrecttyp resurrect;  		/* resurrection data */
/* Model/Host Communication */
	initintyp	initin;				/* Data that is passed to the model for initialization */ 
	fastintyp	fastin;				/* Data that is passed to the model every model iteration by the game */ 
	fastouttyp 	fastout;			/* Data that is output to the game every model iteration */
	mainintyp	mainin;				/* This stuff received less often  */
	mainouttyp	mainout;			/* This stuff gets sent less often */
	RECKON		reckon;				/* Dead reckoning data structure */
/* Linking data */
	S16			slot;				/* Index into gLink */	
	S16			net_node;			/* index reference: model[i].net_node = i */
	S16			in_game;			/* drone/player is in current linked game */
	S16			we_control;			/* Indicates if stack controls this drone/player */
	S16			drone_type;			/* 0=no car, 1=drone, 2=human */
	S16			engine_type;		/* Engine type for sound model */
	S16			body_type;			/* Body type */
	S16			rpm;
	U32			appearance;			/* Car visual appearance data. */
	U32			stuck_time;			/* for drones; used to determine abort condition */
	S8			mcheckpoint;		/* model version of current checkpoint */
	S8			mlap;				/* model version of current lap */
	S8			mmpath_lap;			/* model version of maxpath lap */
	S8			laps;				/* real laps (before link stuff) */
	S16			last_cp_index;		/* last checkpoint index */
	S16			next_cp_index;  	/* next checkpoint index */
	S16			collidable;			/* true when drone/car is collidable */
	S16			drone_target;		/* drone targets this player */
	F32			drone_scale;		/* scale drone velocity by this factor */
	BOOL		data_valid;			/* Set when data has been updated at least once */
	BOOL		can_we_cheat;		/* is it okay for drone to cheat? */
	BOOL		hide_car;			/* hide car from human when resurrecting */
	S16			please_abort_me;	/* drone is requesting abort */
	F32			time_boost;			/* make car move faster (1.0=normal, 2.0=twice as fast */
	F32			time_fudge;			/* all time fudges (catchup and boost) */
} MODELDAT;

#endif

