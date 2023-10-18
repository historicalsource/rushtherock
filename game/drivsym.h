  
/* This file defines all the vehicle simulator variables */

#include "typedefs.h"

#ifndef mbx
#define mbx extern
#endif

#include "vecmath.h"
#include "eptyp.h"

#define radtodeg (180.0/3.141592654)
#define degtorad (3.141592654/180.0)
#define radtoideg ((0x40000000*2.0)/3.141592654)
#define idegtorad (3.141592654/(0x40000000*2.0))
#define radtosdeg (0x8000/3.141592654)
#define sdegtorad (3.141592654/0x8000)
#define idegtodeg (F32)(90.0/0x40000000)
#define degtoideg (0x40000000/90.0)
#define msininv (F32)(1.0/0x7fff)
#define rdpstorpm (60.0/(2.0*3.14159))
#define rpmtordps (2.0*3.14159/60.0)
#define pival 3.141592654

#define MINGEAR 1		/* Minimum gear for auto trans */
#define MAXGEAR 4		/* Number of forward speeds */

#include "tiredes.h"

/*try to get rid of drivsym.h*/
#if 0

/* The following vectors are the body and real world forces, moments,
accelerations, velocities, and positions for the simulated vehicle 

Body Axis Variables:
	F = 	total force vector acting on the simulated vehicle 
	M = 	total moment vector
	A = 	acceleration vector of vehicle
	AA =	angular acceleration vector of vehicle
	V  =	velocity vector of vehicle
	W  =    angular velocity of vehicle
	R  =	radius vector in body axis

	TIREFORCE	force vectors from tires

	TIRER	radius vector of front tire from CG
		
	TIREV	velocity of tire, suspension coordinate system

	TIRERWR	ground position at tires
		
	tireuvs	ground tire unit vectors for each tire

	G	gravitational vector in body axis, integer vec for speed
	D	rolling resistance and aerodynamic drag vector
 */

/*Real World Axis Variables:
	RWV 	velocity vector of vehicle in real world coordinates
	RWR    	position vector of vehicle of vehicle in real world coords
	EPRWR	extended precision position vector of vehicle

	GRW	gravitational vector in real world, integer vec for speed
	UV  	unit vectors of vehicle
*/

/* The following are scalar vechicle variables 
	torque		torque on tires
	steerangle	steer angle of tires
	steergain	relationship between wheel pos and steer angle
	msteerratio	ratio between degrees at steering wheel and
			degrees at tires

	swtpg		Steering wheel torque per G, ft-lbs
	trail		Actual front wheel trail in feet
	swtorquegain	The relationship between FTFORCE[YCOMP] and
			the swtorque variable sent to the 68000
	swtorque	actual steering wheel torque
	gSWpos		integer steering wheel pos (-0x200 to 0x200)
	wheel		steering wheel pos (varies from -1.0 to 1.0)
	mclutch		clutch position (0 to 1.0)
	mbrake		brake position (0 to 1.0)
	throttle	throttle pos (0 to 1.0)
	mgear		current gear (-1 to maximum number of gears)
	commandgear	gear input from 68000
	gearslip	array of gear slip rpms;
	mautotrans	1 if automatic transmission, 0 otherwise
	mcarnum		number of car
	mignition	non-zero if ignition on, 0 otherwise
	mstartermotor	non-zero if starter engaged, 0 otherwise

	brakeroll 	speed to start rolling off brake response
	pitchangle	sprung mass angle wrt road
	magvel		magnitude of velocity
		
	swdamp		damping factor for steering wheel
	mmaxswdamp	maximum steering damping (high speed)
		
	swfrict		friction in steering wheel
	mmaxswfrict	maximum steering friction (zero speed,tires scrubbing)
	mminswfrict	minimum steering friction (high speed)
		
*/

/*********************************************************************/

/* Car and world constants 
	gravcon		gravitational constant
	mass		mass of car in slugs;
	massi		1/mass
	weight		weight of car in pounds
	wheelbase	distance between contact patches
	springrate	spring rate of suspension for each tire, lbs/foot
	farspringrate	spring rate of front anti-roll bar, lbs/foot
	rarspringrate	spring rate of rear anti-roll bar, lbs/foot
	damping		damping of suspension for each tire, lbs*sec/foot
	rideheight	amount springs compressed on level ground	
	thetime		time in seconds
	airtime		time in seconds when last flying through air
	dt		time between iterations of the model
	iter		counts iterations of model;
	alpfmax		alpha for maximum force at front wheels
	cffmax		coeff of friction at max force
	alprmax		alpha for maximum force at rear wheels
	cfrmax		coeff of friction at max force
	I		moment of inertia vector
	INVI		vector of inverses of moment of inertias
	msrefpcybo2	SREF*p*CYB/2 for aerodynamic force
	mrollresist	force due to rolling resistance
	minspeed	speed where model switches to slow mode
	mreinit		reinitialize model
	gameover	game is over flag, do quick coast down
	swampgain	(swtorque output to model)/
				(Steering wheel torque (ft-lbs))
	reflecttime	time when next reflection allowed
*/

/*try to get rid of drivsym.h*/
#endif

/* This is the structure for car data */
typedef struct car
{
	char *name;	/* pointer to name of car */
	F32 mass;	/* mass of car in slugs */
	F32 I[3];	/* moment of inertia vector */

	F32 springrate[4];	/* spring constant of suspension */
	F32 farspringrate;	/* spring rate of front anti roll bar */
	F32 rarspringrate;	/* spring rate of rear anti roll bar */
	F32 cdamping[4];	/* compression damping of suspension */
	F32 rdamping[4];	/* rebound damping of suspension */

	F32 steerratio;	/* steering ratio, wheel/kingpin */
	F32 swtpg;		/* steering wheel torque per G */
	short maxswdamp;	/* Maximum steering wheel damping */
	short minswfrict;	/* Minimum steering wheel friction */
	short maxswfrict;	/* Maximum steering wheel friction */

	F32 srefpcybo2;		/*aerodynamic drag factor */
	F32 rollresist;		/*rolling resistance, lbs */

	struct tiredes tires[4];	/*tire descriptions: 
						0 = right front,
						1 = left front,
						2 = right rear,
						3 = left rear */

	F32 TIRER[4][3];		/*vectors from CG to each tire*/
	
	F32 brakebal;			/*front brake balance*/

	F32 engmi;		/* engine flywheel moment of inertia */

	F32 dwratio;		/* differential gear ratio */

	F32 clutchmaxt;	/* maximum torque through clutch */
	
	F32 viewheight;	/* height of observer above CG */
	char nothrusttorque;	/* Flag for zero yaw torque from thrust */
	char magicdif;		/* Flag for magic load sensitive torque split
				differential */
	F32 fgtorquescale;	/* Torquescale in first gear */
	F32 sgtorquescale;	/* Torquescale in second gear */
	F32 torquescale;	/* Torquescale if gear > 2 */
	F32 dirttorquescale;	/* Torquescale off road */
	F32 transarray[MAXGEAR+2];	/* Transmission gear ratio */
	const short *torquecp;	/* Pointer to torque curve */
	const short *dirttorquecp;	/* Pointer to torque curve on dirt */
	short rpmperent;	/* RPM per entry in torque curve */
	short topgear;		/* Top gear for automatic transmission */
	F32 upshiftangvel;	/* Ang velocity for auto trans upshift */
	F32 downshiftangvel;	/* Ang velocity for auto trans downshift */
} Car;

typedef struct collsizetyp {
	F32	colfront;
	F32	colrear;
	F32	colside;
	F32	colheight;
}COLLSIZE;


