/*
 *	cars.c	-- Parameters for all the cars to be modeled.
 *
 * 	the notation -- dlm 4-91 -- indicates comments made by Doug
 *	Milliken, Milliken Research Associates, 716-632-6710, FAX633-9283.
 *	MRA maintaines a large file of different vehicle data and reference
 *	material relative to vehicle dynamics.  Comments have been added to
 *	most data items in the first car structure (the SHORTTRKCAR) and
 *	other comments added throughout the file.
 *
 *	Copyright 1996 Time Warner Interactive.
 *	Unauthorized reproduction, adaptation, distribution, performance or 
 *	display of this computer program or the associated audiovisual work
 *	is strictly prohibited.
 *
 *	======================================================================
 *	$Author: gray $						$Date: 1997/09/18 00:50:37 $
 *	$Revision: 3.11 $						$Locker:  $
 *	======================================================================
 *	Change Log:
 *
 *	$Log: cars.c,v $
 * Revision 3.11  1997/09/18  00:50:37  gray
 * Reorder cars and add collision boxes for 4 new cars.
 *
 * Revision 3.10  1997/09/13  01:46:37  grjost
 * Fixed for 12 cars.
 *
 * Revision 3.9  1996/11/27  06:49:42  gray
 * Cams new collision sizes.
 *
 * Revision 3.8  1996/11/19  18:48:25  gray
 * Different size collision boxes.
 *
 * Revision 3.7  1996/11/07  06:35:17  gray
 * very minor difficulty tweak - slightly bigger gap between extream and others.
 *
 * Revision 3.6  1996/10/30  00:12:21  gray
 * New suspension for new roads.
 *
 * Revision 3.5  1996/10/07  09:57:24  gray
 * Tweak suspension and roll moment.
 *
 * Revision 3.4  1996/10/03  21:20:56  gray
 * More realistic moment of inertia.
 *
 * Revision 3.3  1996/09/21  04:37:59  gray
 * Seperated car model and car difficulty.
 *
 * Revision 3.2  1996/09/21  02:26:30  gray
 * Added slot for 4th car. Starting to genericise cars data.
 *
 * Revision 3.1  1996/09/09  02:24:52  gray
 * JAMMA version
 *
 * Revision 2.12  1996/08/21  18:16:08  gray
 * Bump up beginner car torque.
 *
 * Revision 2.11  1996/08/17  18:12:39  gray
 * Slight torque tweak so advanced cars are always faster.
 *
 * Revision 2.10  1996/08/02  08:00:45  gray
 * Little stickier tires.
 *
 * Revision 2.9  1996/08/01  19:13:22  gray
 * Another pass at medium speed cars.
 *
 * Revision 2.8  1996/07/31  12:13:23  gray
 * Little slower fast cars.
 *
 * Revision 2.7  1996/07/31  07:38:05  gray
 * Expert car tweaks.
 *
 * Revision 2.6  1996/07/30  00:12:24  gray
 * Modify engine torque,gear ratio, suspension,weight,tires,etc for faster cars.
 *
 * Revision 2.5  1996/07/23  20:42:25  gray
 * Get real collision bounding box into cars.
 *
 * Revision 2.4  1996/05/10  23:16:32  gray
 * Beginner, Intermed, and Expert cars all have Mustang wheelbase now.
 *
 * Revision 2.3  1996/05/09  18:02:17  gray
 * SNX wheelbase same as Mustang.
 *
 * Revision 2.2  1996/05/08  01:21:55  gray
 * Changed wheelbase for Mustang.
 *
 * Revision 2.1  1996/05/04  02:56:18  petrick
 * Focus 2 release
 *
 * Revision 1.9  1996/04/25  09:19:51  gray
 * Defined intermed_car. Gave more difficult cars slight torque
 * advantage. Made brg,int,and exp cars tires the same.
 *
 * Revision 1.8  1996/04/23  21:44:22  petrick
 * Added header.
 *
 *
 *	======================================================================
 */

#define mbx extern
#include "drivsym.h"
#include "dis.h"

/* These are the standard torque curves for all the Race Drivin' game
cars.
	Each of the 10 rows contains torque values for a particular
throttle opening, from 0% to 100% (~11% increments).  Each column
contains torque values for a particular RPM, starting at 0 RPM, the
spacing is typically 1000 RPM (0-11000 RPM) and is called out near
the end of each car description.  
	A two way interpolation determines engine torque for any
given combination of throttle opening and RPM.  The interpolation
routine can not extrapolate outside the RPM range specified, thus
several negative (motoring torque) values should be provided at the
high-RPM end for each throttle opening, even full throttle.
	The value of "torquescale" in each car description scales the
torque curve to different power levels.
	Actual horsepower (BHP) can be calculated by:
		[torque*RPM*6.28*torquescale/33000]
	This data is not easy to get for real engines, the Subaru
data is the only complete set that I have seen.  Some one suggested
that the place to get this data is from automatic transmission
engineering -- they need it to determine auto shift points, etc. -- dlm 4-91

*/

const short stdtorquecurve[10][12]={
    {	75, -10, -30, -50, -70, -80, -80, -80, -80, -80, -80, -80 },
    {	75,  100,   1, -12, -29, -41, -49, -60, -71, -76, -78, -80 },
    {	75,  150,  32,  26,  12,  -2, -18, -40, -62, -71, -76, -80 },
    {	75,  150,  63,  63,  53,  37,  13, -20, -53, -67, -73, -80 },
    {	75,  150,  94, 101,  94,  76,  44,   0, -44, -62, -71, -80 },
    {	75, 175, 126, 139, 136, 114,  76,  20, -36, -58, -69, -80 },
    {	75, 175, 157, 177, 177, 153, 107,  40, -27, -53, -67, -80 },
    {	75, 175, 188, 214, 218, 192, 138,  60, -18, -49, -64, -80 },
    {	75, 177, 219, 252, 259, 231, 169,  80,  -9, -44, -62, -80 },
    {	75, 200, 250, 290, 300, 270, 200, 100,   0, -40, -60, -80 }
};

const short stddirttorquecurve[10][12]={
    {	75, -10, -30, -30, -40, -40, -40, -40, -40, -40, -40, -40 },
    {	75,  100,   1, -12, -29, -41, -40, -40, -40, -40, -40, -40 },
    {	75,  150,  60,  26,  12,  -2, -18, -40, -40, -40, -40, -40 },
    {	75,  150,  94,  63,  53,  37,  13, -10, -40, -40, -40, -40 },
    {	75,  150,  94, 101,  94,  76,  44,   0, -40, -40, -40, -40 },
    {	75,  150,  94, 101,  94,  76,  44,   0, -40, -40, -40, -40 },
    {	75,  150,  94, 101,  150,  76,  44,   0, -40, -40, -40, -40 },
    {	75,  150,  150, 150,  150,  76,  44,   0, -40, -40, -40, -40 },
    {	75,  150,  150, 150,  150,  76,  44,   0, -40, -40, -40, -40 },
    {	75,  150,  150, 150,  150,  76,  44,   0, -40, -40, -40, -40 }
};

const short subtorquecurve[10][12]={
    {	68.,-13.6,-22.5,-28.0,-33.4,-36.8,-38.9,-40.9,-40.9,-40.9,-40.9,-40.9 },
    {	68.2,91.4,53.2,24.6,6.8,-7.5,-20.5,-34.1,-40.9,-40.9,-40.9,-40.9 },
    {	68.2,102.3,107.8,90.7,68.2,47.7,25.2,3.4,-20.5,-40.9,-40.9,-40.9 },
    {	68.2,106.4,143.2,143.2,118.0,95.5,70.9,43.6,13.6,-13.6,-40.9,-40.9 },
    {	68.2,109.1,151.4,154.8,142.5,126.2,100.9,69.6,37.5,6.8,-23.9,-40.9 },
    {	68.2,113.9,156.9,160.3,152.1,138.4,114.6,83.9,51.2,19.1,-13.6,-40.9 },
    {	68.2,114.4,158.7,162.8,155.3,141.7,118.3,87.5,54.4,22.0,-11.1,-39.2 },
    {	68.2,114.9,160.6,165.4,158.6,144.9,122.1,91.0,57.6,24.9,-8.5,-37.5 },
    {	68.2,115.4,162.5,167.9,161.8,148.2,125.8,94.6,60.9,27.8,-6.0,-35.8 },
    {	68.2,115.9,164.4,170.5,165.0,151.4,129.6,98.2,64.1,30.7,-3.4, -34.1 }
};

const short racetorquecurve[10][12]={
    {	100, -20, -33, -41, -49, -54, -57, -60, -60, -60, -60, -60 }, 
    {	100, 134,  78,  36,  10, -11, -30, -50, -60, -60, -60, -60 },
    {	100, 150, 158, 133, 100,  70,  37,   5, -30, -60, -60, -60 }, 
    {	100, 156, 210, 210, 173, 140, 104,  64,  20, -20, -60, -60 },
    {	100, 160, 222, 227, 209, 185, 148, 102,  55,  10, -35, -60 },
    {	100, 167, 230, 235, 223, 203, 168, 123,  75,  28, -20, -60 },
    {	100, 168, 233, 239, 240, 240, 280, 320, 340,  32, -16, -58 },
    {	100, 169, 236, 243, 260, 270, 310, 350, 360,  37, -13, -55 },
    {	100, 169, 238, 246, 280, 290, 330, 360, 380,  41,  -9, -53 },
    {	100, 170, 241, 250, 300, 310, 350, 380, 400,  45,  -5, -50 }
};

const short rushtorquecurve[10][12]={
    {	100, -12, -33, -51, -61, -54, -71, -75, -75, -75, -75, -75 }, 
    {	100, 112,  62,   3, -30, -51, -65, -69, -71, -73, -75, -75 },
    {	100, 114, 113, 112, 104,  70,  87,  32, -32, -62, -70, -75 }, 
    {	100, 116, 126, 131, 136, 136, 131,  90,  -1, -36, -69, -75 },
    {	100, 119, 137, 151, 161, 170, 175, 160,  90,   0, -50, -75 },
    {	100, 127, 156, 178, 196, 215, 223, 225, 205,  75, -32, -75 },
    {	100, 133, 163, 190, 212, 230, 245, 256, 243, 108, -23, -75 },
    {	100, 137, 175, 206, 230, 250, 265, 280, 282, 175, -10, -75 },
    {	100, 146, 186, 222, 248, 275, 293, 305, 310, 251,   4, -69 },
    {	100, 155, 202, 240, 275, 300, 320, 327, 328, 312,  49, -65 }
};


/*************************************************************************/

#if 0
const struct car shorttrkcar={
	"SHORTTRKCAR",		/* Name of car */
	3600/32.2,			/* mass of car in slugs
					-- 3600 is weight in pounds,
					-- 32.2 ft/sec^2 is acceleration of
					   gravity -- dlm 4-91  */

	{
		1500.0,	/* roll moment of inertia slug/ft^2
			Inertia data can be approximated using data in
			"Typical Vehicle Parameters for Dynamic Studies",
			Rassmussen, et al; GM Proving Ground, 4/1970.  There
			have been follow-on papers published by SAE, available
			at Milliken Research -- dlm 4/91
			*/
		2500.0,
		2500.0	/* yaw moment of inertia slug/ft^2 */
	},

	/* spring rate of each wheel of car, lb/ft

	Typical spring rates are given AT THE SPRING, in pounds/inch,
these must be converted to rates AT THE WHEEL by using the linkage
ratio (also called "velocity ratio") of the particular suspension
design, and converted to pounds/foot.  This model is for an
INDEPENDENT SUSPENSION, not a SOLID AXLE as is found on the rear of
large US cars and on both ends of 4WD vehicles.
	The following set of rates are pretty stiff, sort of like a
semi-race car.  One way to think about ride spring rates is in terms
of un-damped natural frequency.  Soft passenger cars are around 1Hz,
sporty cars are 1.5-2.0 Hz, race cars can be over 5 Hz.  -- dlm 4-91
*/
	{-4000,-4000,-2500,-2500},

	-1000,	/* front anti roll bar springrates, lb/ft */
	0,	/* rear anti roll bar springrates, lb/ft

	The anti roll bar rate applies a differential force to each
wheel on the axle proportional to the differential wheel vertical
(ride) position.  The effect is to reduce steady-state vehicle roll
(lean) angles.  This is also called a "stabilizer bar".  It has no
effect on parallel bump where both wheels on an axle travel equal
amounts in ride.
	For a value of -1000, with the left wheel 0.5 foot
higher than the right wheel (relative to the car chassis, as found in
a right hand turn) then the left wheel spring force is increased by
500 pounds and the right wheel spring force is decreased by 500
pounds.  
	Note that the roll moment produced on the car chassis
includes the effect of the track width of the car.  Also note that
the (ride) spring rates also provide a roll moment with differential
wheel ride position.  -- dlm 4-91
*/

	/* suspension damping of each wheel of car lb-sec/ft */
	{-500,-500,-300,-300},	/* Compression damping */
	{-500,-500,-300,-300},  /* Rebound damping

	Damping values can be thought of in terms of %-Critical
Damping.  Typical passenger cars have less than critical damping and
have lower values for compression (bump) travel than for rebound
travel.  The numbers above are intended to represent a stiff race
car.  See "Typical Parameters...", GM PRoving Ground, for a more
complete discussion.  See "Marks Handbook..." or other Mech.E.
handbook for equations.  -- dlm 4-91
*/
		
	14.0,	/* steering ratio deg/deg 

	This is "degrees of steering wheel rotation/degrees of
corresponding front wheel steer angle".
	Passenger cars range from 24:1 (manual steering, heavy car or
utility vehicle) to 14:1 for sports cars (typically with power
steering).  Sedans are usually in the 16:1 to 18:1 range.  Race cars
can be 10:1 or even "faster".  -- dlm 4-91
*/
	4,	/* steering wheel torque, ft-lbs/lateral g

	This represents the self-centering torque reflected from the
front wheel up to the steering wheel.  While this representation is
linear with lateral acceleration, true cars are far from linear.
Power steering systems typically have two slopes, to give a steep
torque gradient near center for straight running and a flat gradient
to reduce parking effort for large wheel motions.  Tires also
generate "self-aligning torques that are not modeled, these also
effect the centering torque.
	Keep in mind the torque capabilities of the steering torque
motor when choosing this value.  -- dlm 4-91 
*/
	4,	/* Max steering damping, 0-16 */
	0x10,	/* Min steering friction 0-0x100 */
	0x100,	/* Max steering friction 0-0x100 

	Used to stabilize the force feedback steering wheel -- dlm 4-91
*/


/* aerodynamic drag and rolling resistance */

	.0087,	/* srefcybo2

	This number includes lumped constants for drag coefficient,
frontal area, and density of air.  When multiplied by velocity^2 (in
ft/sec) it gives the total drag force due to the air drag.  This
combined with rolling resistance, engine power (and gear ratios), and
road grade determines the top speed of a car.  -- dlm 4-91
*/
	36,	/* rollresist, 1% of weight
		
	Varies over a wide range depending on tire pressure, tire
design and road surface (higher on dirt...).  1% of weight is good
for pavement, bicycles can be .25% -- dlm 4-91
*/

/* These are the tire description blocks for each wheel */
	{	
	    {	/* tires[0] (right front) */
		1.0,		/* tradius
	distance from axle to ground in feet  -- dlm 4-91*/
		340*12,		/* springK 
	lateral spring constant of tire in pounds/ft, used to stabilize
	lateral motion of vehicle at very low vehicle speeds -- dlm 4-91*/
		200,		/* rubdamp
	used to stabilize vehicle lateral motion at low speeds -- dlm 4-91*/
		16000,		/* PaveCstiff used to be 16000
			
	Cornering stiffness is the initial slope of the tire lateral
force vs. slip angle curve for the tire, measured near 0 slip angle,
in pounds/radian.  Higher values can be thought of as representing
stiffer tires with shorter sidewalls.  
	The effect of lateral weight transfer (higher loading on the
outside tire than the inside in a corner) is to reduce the Cstiff.
This effect (called "tire load sensitivity") is not included in this
tire model, thus this number can be tweaked to represent the effect
of anti roll bars on the tire performance -- dlm 4-91
*/
		1.15,		/* PaveCfmax
	Max lateral cornering force (Cf) determines the maximum
lateral (side) force that a tire can generate, in non-dimensional
("g') terms = lateral force/load.  The tire model uses a polynomial
function to generate a smooth curve for tire lateral force generation
that starts at an initial slope of Cstiff and ends in a plateau of
Cfmax.
	Real cars do not corner as well as the individual tire data
would indicate, a number of operating conditions degrade tire
performance.
	Numbers for the value for the whole car can be found in
magazine road test data panels as "maximum lateral acceleration", see
"Road & Track" for example.  Typical passenger cars are around .7g,
sports cars are .8 to .9 g, the Hard Drivin cars are more like
moderate race cars.  The best race cars can be 1.5g or higher.
	To determine the actual lateral acceleration that a
particular car generates, drive on the smaller skid-pad circle (100
foot radius) and note the maximum speed (V=velocity) (or time a lap
and calculate the speed) in ft/second.  Then, 
  lateral acceleration (Ay) = V^2/(Radius*acceleration of gravity)
				  = V^2/(100*32.2)  -- dlm 4-91
*/
		16000,		/* Cstiff used to be 16000*/
		1.15,		/* Cfmax, make same as above - dlm*/

/*the following are internal variables used by the tire model -- dlm 4-91*/
		.1,		/* invmi */
		0,		/* Zforce */	
		0,		/* Afmax */
		0,		/* k1 */
		0,		/* k2 */
		0,		/* k3 */
		0,		/* l2 */
		0,		/* l3 */
		0,0,0,0,	/* m1,m2,m3,m4 */
		0,		/* patchy */
		0,		/* angvel */
		0,		/* sliptorque */
		0,		/* screech */
		0		/* slipflag */
	    },
	    {	/* tires[1] (left front) 
	Generally the two front tires and the two rear tires will be
the same.  A possible exception might be modeling a flat tire where
the flat would have much lower tradius, Cstiff and Cf. -- dlm 4-91 
*/
		1.0,		/* tradius */
		340*12,		/* springK */
		200,		/* rubdamp */
		16000,		/* PaveCstiff used to be 16000*/
		1.15,		/* PaveCfmax */
		16000,		/* Cstiff used to be 16000*/
		1.15,		/* Cfmax */
		.1,		/* invmi */
		0,		/* Zforce */	
		0,		/* Afmax */
		0,		/* k1 */
		0,		/* k2 */
		0,		/* k3 */
		0,		/* l2 */
		0,		/* l3 */
		0,0,0,0,	/* m1,m2,m3,m4 */
		0,		/* patchy */
		0,		/* angvel */
		0,		/* sliptorque */
		0,		/* screech */
		0		/* slipflag */
	    },
	    {	/* tires[2] (right rear) */
		1.0,		/* rtdes.tradius */
		340*12,		/* rtdes.springK */
		200,		/* rtdes.rubdamp */
		50000,		/* rtdes.PaveCstiff used to be 64000*/
		1.3,		/* rtdes.PaveCfmax */
		50000,		/* rtdes.Cstiff used to be 64000*/
		1.3,		/* rtdes.Cfmax */
		.1,		/* rtdes.invmi */
		0,		/* rtdes.Zforce */	
		0,		/* rtdes.Afmax */
		0,		/* rtdes.k1 */
		0,		/* rtdes.k2 */
		0,		/* rtdes.k3 */
		0,		/* rtdes.l2 */
		0,		/* rtdes.l3 */
		0,0,0,0,	/* rtdes.m1,m2,m3,m4 */
		0,		/* rtdes.patchy */
		0,		/* rtdes.angvel */
		0,		/* rtdes.sliptorque */
		0,		/* rtdes.screech */
		0		/* rtdes.slipflag */
	    },
	    {	/* tires[3] (left rear) */
		1.0,		/* rtdes.tradius */
		340*12,		/* rtdes.springK */
		200,		/* rtdes.rubdamp */
		50000,		/* rtdes.PaveCstiff used to be 64000*/
		1.3,		/* rtdes.PaveCfmax */
		50000,		/* rtdes.Cstiff used to be 64000*/
		1.3,		/* rtdes.Cfmax */
		.1,		/* rtdes.invmi */
		0,		/* rtdes.Zforce */	
		0,		/* rtdes.Afmax */
		0,		/* rtdes.k1 */
		0,		/* rtdes.k2 */
		0,		/* rtdes.k3 */
		0,		/* rtdes.l2 */
		0,		/* rtdes.l3 */
		0,0,0,0,	/* rtdes.m1,m2,m3,m4 */
		0,		/* rtdes.patchy */
		0,		/* rtdes.angvel */
		0,		/* rtdes.sliptorque */
		0,		/* rtdes.screech */
		0		/* rtdes.slipflag */
	    }
	},
		    
/* following are the distances from the CG to each wheel
	x, y, z, in feet, to the tire contact patch with no load on
the tire.  x is determined by the fore-aft weight distribution, CG
forward of center means that the front wheels will be closer (smaller
number) to the CG than the rear wheels.  y is 1/2 of the track (tread)
width of the axle, for symmetrical cars.  This data can be found in
"Road & Track". 
	z is broken into 2 parts, the first part is the desired CG
height and the second is the additional height before the spring is
deflected by the weight (this static deflection must be calculated
and entered, to get the correct standing height).  The static
deflection depends on the spring rate and the car weight.  CG height
is of the whole vehicle, the model does not know about unsprung
weight (the weight of the tire/wheel/brake and ~1/2 the suspension
components).  CG height data is not easy to find in the leterature,
sedans are usually around 1.7 feet, race cars can be as low as 1.0
foot. -- dlm 4-91 
*/
	{	/* TIRER */
		{4.5,2.75,1.5+.2},		/* right front */
		{4.5,-2.75,1.5+.2},		/* left front */
		{-5.5,2.75,1.5+.3},		/* right rear */
		{-5.5,-2.75,1.5+.3}		/* left rear */
	},

	.65,			/* brakebal - brake balance at front wheel 
	Passenger car brake balance is set to lock the front wheels
before the rear wheels.  This can be determined by making hard stops
from high speed varying the brake pedal force in the range of wheel
lockup.  If the car tends to spin, the rear wheels have locked up
first.  In the original Hard Drivin car, the balance was all to the
front wheel, with the Race Drivin model, brake balance can be set
more realistically.
	Because of forward weight transfer on braking, the brake
balance is always more to the front than the static weight
distribution. -- dlm 4-91 
*/
	.3,			/* moment of inertia of flywheel
	This is actually the moment of inertia of all the engine
parts before the clutch.  The units are slug-ft^2.  Bigger numbers
cause the engine to rev-up slower in neutral.  Bigger numbers also
increase the transient torque output available (in excess of the
engine torque listed in the torque table) when the clutch is dropped
(engaged), at low road speed-high RPM (typically on standing start).
-- dlm 4-91 */

 	3.3,			/* Differential gear ratio 
	This gear ratio multiplies engine torque while reducing wheel
RPM, also called axle ratio. -- dlm 4-91*/

	800.0,			/* maximum torque with clutch fully engaged
	Torque higher than this (typically from energy stored in the
flywheel) results in clutch slippage and delivery of just this amount of
pounds-ft of torque.  -- dlm  4-91
*/

	-2.0,			/* height from CG to eye level 
	This z dimension determines the viewpoint height of the
display.  -- dlm 4-91
*/

	3.0,			/* Collision width */
	6.0,			/* Collision length */
	20.0,			/* Collision height */
	0,			/* Nothrusttorque */
	0,			/* Magicdif
	Setting this to 1 produces a limited slip differential that
also has the "magic" feature that it does not put yaw moments on the
car when one wheel is delivering higher torque than the other.
Real-world mechanical differentials can not produce this
function...  -- dlm 4-91 
*/
	1.0,			/* fgtorquescale */
	1.0,			/* sgtorquescale */
	1.0,			/* torquescale
	These three can all be the same, they scale the torque curve to give different power levels at the same throttle opening and RPM -- an easy way to make a different engine. -- dlm 4-91
*/
	1.0,			/* dirttorquescale
	Different torque scale for use when the wheels are
off of the road -- dlm 4-91.
*/
	{
		-2.4,				/*reverse*/
		0.0,				/*neutral*/
		1.8,				/*first gear*/
		1.45,				/*second gear*/
		1.2,				/*third gear*/
		1.0,				/*fourth gear
	Transmission gear ratios, i.e., 1.8:1.  This set is a fairly
extreme set of "close ratio gears" as might be found in a racing car.
The other Hard Drivin cars have more typical passenger car ratios.
The high ratio in first gear makes getting started and climbing hills
very difficult.  "Road & Track" lists this data. -- dlm 4-91
*/
	},
	&racetorquecurve[0][0],	/* Pointer to torque curve */
	&racetorquecurve[0][0],	/* Pointer to torque curve on dirt */
	1000,			/* RPM per horizontal entry in torque curve */
	4,			/* Top gear for automatic transmission */
	5750*rpmtordps,		/* Ang velocity for auto trans upshift */
	3500*rpmtordps		/* Ang velocity for auto trans downshift 
	Units are radians/sec.  This provides hysteresis so the auto
transmission doesn't hunt (or limit cycle) if you are driving at or
near a shift rpm.  -- dlm 4-91
*/
};

/*************************************************************************/

const struct car crownvic={
	"CROWNVIC",		/* Name of car
	First cut at a "full-size" sedan, as used for police cars.  -- dlm 4-91
*/
	4200/32.2,			/* mass of car in slugs */

	{
		1500.0,	/* roll moment of inertia slug/ft^2 */
		2500.0,
		2500.0	/* yaw moment of inertia slug/ft^2 */
	},

	/* spring rate of each wheel of car, lb/ft */
	{-1900,-1900,-1900,-1900},

	-1000,	/* front anti roll bar springrates, lb/ft */
	0,	/* rear anti roll bar springrates, lb/ft */

	/* suspension damping of each wheel of car lb-sec/ft */
#if 0
	{-310,-310,-280,-280},	/* Compression damping */
	{-465,-465,-420,-420},  /* Rebound damping */
#else
	{-250,-250,-200,-200},	/* Compression damping */
	{-325,-325,-300,-300},  /* Rebound damping */
#endif
		
	18.0,	/* steering ratio deg/deg */
	2,	/* steering wheel torque, ft-lbs/lateral g */
	4,	/* Max steering damping, 0-16 */
	0x10,	/* Min steering friction 0-0x100 */
	0x80,	/* Max steering friction 0-0x100 */

/* aerodynamic drag and rolling resistance */

	.0095,	/* srefcybo2 */
	42,	/* rollresist, 1% of weight */

/* These are the tire description blocks for each wheel */
	{	
	    {	/* tires[0] (right front) */
		1.0,		/* tradius */
		340*12,		/* springK */
		200,		/* rubdamp */
		16000,		/* PaveCstiff used to be 16000*/
		1.0,		/* PaveCfmax */
		16000,		/* Cstiff used to be 16000*/
		1.0,		/* Cfmax */
		.1,		/* invmi */
		0,		/* Zforce */	
		0,		/* Afmax */
		0,		/* k1 */
		0,		/* k2 */
		0,		/* k3 */
		0,		/* l2 */
		0,		/* l3 */
		0,0,0,0,	/* m1,m2,m3,m4 */
		0,		/* patchy */
		0,		/* angvel */
		0,		/* sliptorque */
		0,		/* screech */
		0		/* slipflag */
	    },
	    {	/* tires[1] (left front) */
		1.0,		/* tradius */
		340*12,		/* springK */
		200,		/* rubdamp */
		16000,		/* PaveCstiff used to be 16000*/
		1.0,		/* PaveCfmax */
		16000,		/* Cstiff used to be 16000*/
		1.0,		/* Cfmax */
		.1,		/* invmi */
		0,		/* Zforce */	
		0,		/* Afmax */
		0,		/* k1 */
		0,		/* k2 */
		0,		/* k3 */
		0,		/* l2 */
		0,		/* l3 */
		0,0,0,0,	/* m1,m2,m3,m4 */
		0,		/* patchy */
		0,		/* angvel */
		0,		/* sliptorque */
		0,		/* screech */
		0		/* slipflag */
	    },
	    {	/* tires[2] (right rear) */
		1.0,		/* rtdes.tradius */
		340*12,		/* rtdes.springK */
		200,		/* rtdes.rubdamp */
		32000,		/* rtdes.PaveCstiff used to be 64000*/
		1.1,		/* rtdes.PaveCfmax */
		32000,		/* rtdes.Cstiff used to be 64000*/
		1.1,		/* rtdes.Cfmax */
		.1,		/* rtdes.invmi */
		0,		/* rtdes.Zforce */	
		0,		/* rtdes.Afmax */
		0,		/* rtdes.k1 */
		0,		/* rtdes.k2 */
		0,		/* rtdes.k3 */
		0,		/* rtdes.l2 */
		0,		/* rtdes.l3 */
		0,0,0,0,	/* rtdes.m1,m2,m3,m4 */
		0,		/* rtdes.patchy */
		0,		/* rtdes.angvel */
		0,		/* rtdes.sliptorque */
		0,		/* rtdes.screech */
		0		/* rtdes.slipflag */
	    },
	    {	/* tires[3] (left rear) */
		1.0,		/* rtdes.tradius */
		340*12,		/* rtdes.springK */
		200,		/* rtdes.rubdamp */
		32000,		/* rtdes.PaveCstiff used to be 64000*/
		1.1,		/* rtdes.PaveCfmax */
		32000,		/* rtdes.Cstiff used to be 64000*/
		1.1,		/* rtdes.Cfmax */
		.1,		/* rtdes.invmi */
		0,		/* rtdes.Zforce */	
		0,		/* rtdes.Afmax */
		0,		/* rtdes.k1 */
		0,		/* rtdes.k2 */
		0,		/* rtdes.k3 */
		0,		/* rtdes.l2 */
		0,		/* rtdes.l3 */
		0,0,0,0,	/* rtdes.m1,m2,m3,m4 */
		0,		/* rtdes.patchy */
		0,		/* rtdes.angvel */
		0,		/* rtdes.sliptorque */
		0,		/* rtdes.screech */
		0		/* rtdes.slipflag */
	    }
	},
		    
/* these are the distances from the CG to each wheel */
	{	/* TIRER */
		{4.5,2.75,1.8+.69},		/* right front */
		{4.5,-2.75,1.8+.69},		/* left front */
		{-5.5,2.75,1.8+.56},		/* right rear */
		{-5.5,-2.75,1.8+.56}		/* left rear */
	},

	.65,			/* brakebal - brake balance at front wheel */
	.4,			/* moment of inertia of flywheel */

 	3.0,			/* Differential gear ratio */
	600.0,			/* maximum torque with clutch fully engaged */

	-2.5,			/* height from CG to eye level */

	3.0,			/* Collision width */
	6.0,			/* Collision length */
	20.0,			/* Collision height */
	0,			/* Nothrusttorque */
	0,			/* Magicdif */
	1.58,			/* fgtorquescale */
	1.58,			/* sgtorquescale */
	1.58,			/* torquescale */
	1.58,			/* dirttorquescale */
	{
		-2.4,				/*reverse*/
		0.0,				/*neutral*/
		3.0,				/*first gear*/
		2.0,				/*second gear*/
		1.4,				/*third gear*/
		0.95,				/*fourth gear*/
	},
	&subtorquecurve[0][0],	/* Pointer to torque curve */
	&subtorquecurve[0][0],	/* Pointer to torque curve on dirt */
	1000,			/* RPM per horizontal entry in torque curve */
	4,			/* Top gear for automatic transmission */
	5750*rpmtordps,		/* Ang velocity for auto trans upshift */
	3500*rpmtordps		/* Ang velocity for auto trans downshift */
};

/*************************************************************************/

const struct car corvette={
	"CORVETTE",		/* Name of car */
	3525/32.2,			/* mass of car in slugs */

	{
#if 0
		321.0,		/* roll moment of inertia slug/ft^2 */
		1527.0,		/* pitch moment of inertia slug/ft^2 */
		1636.0	/* yaw moment of inertia slug/ft^2 */
#else
		1000.0,	/* roll moment of inertia slug/ft^2 */
		2000.0,
		2000.0	/* yaw moment of inertia slug/ft^2 */
#endif
	},

	/* spring rate of each wheel of car */
	{-1500,-1500,-1750,-1750},

	-1500,	/* front anti roll bar springrates */
	-1500,	/* rear anti roll bar springrates */

	/* suspension damping of each wheel of car */
	{-400,-400,-500,-500},	/* Compression damping */
	{-200,-200,-250,-250},  /* Rebound damping */
		
	16.0,	/* steering ratio */
	4,	/* steering wheel torque per g */
	16,	/* Max steering damping, 0-16 */
	0x20,	/* Min steering friction 0-0x100 */
	0x100,	/* Max steering friction 0-0x100 */

/* aerodynamic drag and rolling resistance */

/*	.0087,*/	/* srefcybo2, 11 hp at 60 mph */
/*	35,*/	/* rollresist 6 hp at 60 mph   */
	.0087,	/* srefcybo2, 11 hp at 60 mph */
	75,	/* rollresist 6 hp at 60 mph   */

/* These are the tire description blocks for each wheel */
	{	
	    {	/* tires[0] (right front) */
		1.0,		/* tradius */
		340*12,		/* springK */
		200,		/* rubdamp */
		16000,		/* PaveCstiff used to be 16000*/
		1.25,		/* PaveCfmax */
		16000,		/* Cstiff used to be 16000*/
		1.25,		/* Cfmax */
		.1,		/* invmi */
		0,		/* Zforce */	
		0,		/* Afmax */
		0,		/* k1 */
		0,		/* k2 */
		0,		/* k3 */
		0,		/* l2 */
		0,		/* l3 */
		0,0,0,0,	/* m1,m2,m3,m4 */
		0,		/* patchy */
		0,		/* angvel */
		0,		/* sliptorque */
		0,		/* screech */
		0		/* slipflag */
	    },
	    {	/* tires[1] (left front) */
		1.0,		/* tradius */
		340*12,		/* springK */
		200,		/* rubdamp */
		16000,		/* PaveCstiff used to be 16000*/
		1.25,		/* PaveCfmax */
		16000,		/* Cstiff used to be 16000*/
		1.25,		/* Cfmax */
		.1,		/* invmi */
		0,		/* Zforce */	
		0,		/* Afmax */
		0,		/* k1 */
		0,		/* k2 */
		0,		/* k3 */
		0,		/* l2 */
		0,		/* l3 */
		0,0,0,0,	/* m1,m2,m3,m4 */
		0,		/* patchy */
		0,		/* angvel */
		0,		/* sliptorque */
		0,		/* screech */
		0		/* slipflag */
	    },
	    {	/* tires[2] (right rear) */
		1.0,		/* rtdes.tradius */
		340*12,		/* rtdes.springK */
		200,		/* rtdes.rubdamp */
		32000,		/* rtdes.PaveCstiff used to be 64000*/
		1.3,		/* rtdes.PaveCfmax */
		32000,		/* rtdes.Cstiff used to be 64000*/
		1.3,		/* rtdes.Cfmax */
		.1,		/* rtdes.invmi */
		0,		/* rtdes.Zforce */	
		0,		/* rtdes.Afmax */
		0,		/* rtdes.k1 */
		0,		/* rtdes.k2 */
		0,		/* rtdes.k3 */
		0,		/* rtdes.l2 */
		0,		/* rtdes.l3 */
		0,0,0,0,	/* rtdes.m1,m2,m3,m4 */
		0,		/* rtdes.patchy */
		0,		/* rtdes.angvel */
		0,		/* rtdes.sliptorque */
		0,		/* rtdes.screech */
		0		/* rtdes.slipflag */
	    },
	    {	/* tires[3] (left rear) */
		1.0,		/* rtdes.tradius */
		340*12,		/* rtdes.springK */
		200,		/* rtdes.rubdamp */
		32000,		/* rtdes.PaveCstiff used to be 64000*/
		1.3,		/* rtdes.PaveCfmax */
		32000,		/* rtdes.Cstiff used to be 64000*/
		1.3,		/* rtdes.Cfmax */
		.1,		/* rtdes.invmi */
		0,		/* rtdes.Zforce */	
		0,		/* rtdes.Afmax */
		0,		/* rtdes.k1 */
		0,		/* rtdes.k2 */
		0,		/* rtdes.k3 */
		0,		/* rtdes.l2 */
		0,		/* rtdes.l3 */
		0,0,0,0,	/* rtdes.m1,m2,m3,m4 */
		0,		/* rtdes.patchy */
		0,		/* rtdes.angvel */
		0,		/* rtdes.sliptorque */
		0,		/* rtdes.screech */
		0		/* rtdes.slipflag */
	    }
	},
		    
/* these are the distances from the CG to each wheel */
	{	/* TIRER */
		{5.0,2.75,1.567+.60},		/* right front */
		{5.0,-2.75,1.567+.60},		/* left front */
		{-5.0,2.75,1.567+.50},		/* right rear */
		{-5.0,-2.75,1.567+.50}		/* left rear */
	},

	1.0,			/* brakebal - brake balance at front wheel */
	.4,			/* moment of inertia of flywheel */

 	3.5,			/* Differential gear ratio */
	600.0,			/* maximum torque with clutch fully engaged */

	-1.95,			/* height from CG to eye level */

	3.0,			/* Collision width */
	6.0,			/* Collision length */
	20.0,			/* Collision height */
	1,			/* Nothrusttorque */
	1,			/* Magicdif */
	1.2,			/* fgtorquescale */
	1.25,			/* sgtorquescale */
	1.3,			/* torquescale */
	1.1,			/* dirttorquescale */
	{
		-2.4,				/*reverse*/
		0.0,				/*neutral*/
		2.4,				/*first gear*/
		1.7,				/*second gear*/
		1.25,				/*third gear*/
		0.95,				/*fourth gear*/
	},
	&stdtorquecurve[0][0],		/* Pointer to torque curve */
	&stddirttorquecurve[0][0],	/* Pointer to torque curve on dirt */
	1000,			/* RPM per horizontal entry in torque curve */
	4,			/* Top gear for automatic transmission */
	5750*rpmtordps,		/* Ang velocity for auto trans upshift */
	3500*rpmtordps		/* Ang velocity for auto trans downshift */
};


const struct car roadster={
	"ROADSTER",		/* Name of car */
	2500/32.2,			/* mass of car in slugs */

	{
#if 0
		321.0,		/* roll moment of inertia slug/ft^2 */
		1527.0,		/* pitch moment of inertia slug/ft^2 */
		1636.0	/* yaw moment of inertia slug/ft^2 */
#else
		700.0,	/* roll moment of inertia slug/ft^2 */
		1500.0,
		1500.0	/* yaw moment of inertia slug/ft^2 */
#endif
	},

	/* spring rate of each wheel of car */
	{-1500,-1500,-1750,-1750},

	-1500,	/* front anti roll bar springrates */
	-0,	/* rear anti roll bar springrates */

	/* suspension damping of each wheel of car */
	{-400,-400,-500,-500},	/* Compression damping */
	{-200,-200,-250,-250},  /* Rebound damping */
		
	16.0,	/* steering ratio */
	4,	/* steering wheel torque per g */
	16,	/* Max steering damping, 0-16 */
	0x20,	/* Min steering friction 0-0x100 */
	0x100,	/* Max steering friction 0-0x100 */

/* aerodynamic drag and rolling resistance */

/*	.0087,*/	/* srefcybo2, 11 hp at 60 mph */
/*	35,*/	/* rollresist 6 hp at 60 mph   */
	.0087,	/* srefcybo2, 11 hp at 60 mph */
	75,	/* rollresist 6 hp at 60 mph   */

/* These are the tire description blocks for each wheel */
	{	
	    {	/* tires[0] (right front) */
		1.0,		/* tradius */
		340*12,		/* springK */
		200,		/* rubdamp */
		16000,		/* PaveCstiff used to be 16000*/
		1.15,		/* PaveCfmax */
		16000,		/* Cstiff used to be 16000*/
		1.15,		/* Cfmax */
		.1,		/* invmi */
		0,		/* Zforce */	
		0,		/* Afmax */
		0,		/* k1 */
		0,		/* k2 */
		0,		/* k3 */
		0,		/* l2 */
		0,		/* l3 */
		0,0,0,0,	/* m1,m2,m3,m4 */
		0,		/* patchy */
		0,		/* angvel */
		0,		/* sliptorque */
		0,		/* screech */
		0		/* slipflag */
	    },
	    {	/* tires[1] (left front) */
		1.0,		/* tradius */
		340*12,		/* springK */
		200,		/* rubdamp */
		16000,		/* PaveCstiff used to be 16000*/
		1.15,		/* PaveCfmax */
		16000,		/* Cstiff used to be 16000*/
		1.15,		/* Cfmax */
		.1,		/* invmi */
		0,		/* Zforce */	
		0,		/* Afmax */
		0,		/* k1 */
		0,		/* k2 */
		0,		/* k3 */
		0,		/* l2 */
		0,		/* l3 */
		0,0,0,0,	/* m1,m2,m3,m4 */
		0,		/* patchy */
		0,		/* angvel */
		0,		/* sliptorque */
		0,		/* screech */
		0		/* slipflag */
	    },
	    {	/* tires[2] (right rear) */
		1.0,		/* rtdes.tradius */
		340*12,		/* rtdes.springK */
		200,		/* rtdes.rubdamp */
		32000,		/* rtdes.PaveCstiff used to be 64000*/
		1.3,		/* rtdes.PaveCfmax */
		32000,		/* rtdes.Cstiff used to be 64000*/
		1.3,		/* rtdes.Cfmax */
		.1,		/* rtdes.invmi */
		0,		/* rtdes.Zforce */	
		0,		/* rtdes.Afmax */
		0,		/* rtdes.k1 */
		0,		/* rtdes.k2 */
		0,		/* rtdes.k3 */
		0,		/* rtdes.l2 */
		0,		/* rtdes.l3 */
		0,0,0,0,	/* rtdes.m1,m2,m3,m4 */
		0,		/* rtdes.patchy */
		0,		/* rtdes.angvel */
		0,		/* rtdes.sliptorque */
		0,		/* rtdes.screech */
		0		/* rtdes.slipflag */
	    },
	    {	/* tires[3] (left rear) */
		1.0,		/* rtdes.tradius */
		340*12,		/* rtdes.springK */
		200,		/* rtdes.rubdamp */
		32000,		/* rtdes.PaveCstiff used to be 64000*/
		1.3,		/* rtdes.PaveCfmax */
		32000,		/* rtdes.Cstiff used to be 64000*/
		1.3,		/* rtdes.Cfmax */
		.1,		/* rtdes.invmi */
		0,		/* rtdes.Zforce */	
		0,		/* rtdes.Afmax */
		0,		/* rtdes.k1 */
		0,		/* rtdes.k2 */
		0,		/* rtdes.k3 */
		0,		/* rtdes.l2 */
		0,		/* rtdes.l3 */
		0,0,0,0,	/* rtdes.m1,m2,m3,m4 */
		0,		/* rtdes.patchy */
		0,		/* rtdes.angvel */
		0,		/* rtdes.sliptorque */
		0,		/* rtdes.screech */
		0		/* rtdes.slipflag */
	    }
	},
		    
/* these are the distances from the CG to each wheel */
	{	/* TIRER */
		{3.75,2.75,1.0+.60},		/* right front */
		{3.75,-2.75,1.0+.60},		/* left front */
		{-3.75,2.75,1.0+.50},		/* right rear */
		{-3.75,-2.75,1.0+.50}		/* left rear */
	},

	1.0,			/* brakebal - brake balance at front wheel */
	.4,			/* moment of inertia of flywheel */

 	3.5,			/* Differential gear ratio */
	600.0,			/* maximum torque with clutch fully engaged */

	-2.0,			/* height from CG to eye level */

	3.0,			/* Collision width */
	6.0,			/* Collision length */
	20.0,			/* Collision height */
	0,			/* Nothrusttorque */
	0,			/* Magicdif */
	.9,			/* fgtorquescale */
	.95,			/* sgtorquescale */
	.95,			/* torquescale */
	.8,			/* dirttorquescale */
	{
		-2.4,				/*reverse*/
		0.0,				/*neutral*/
		2.4,				/*first gear*/
		1.7,				/*second gear*/
		1.25,				/*third gear*/
		0.95,				/*fourth gear*/
	},
	&stdtorquecurve[0][0],		/* Pointer to torque curve */
	&stddirttorquecurve[0][0],	/* Pointer to torque curve on dirt */
	1000,			/* RPM per horizontal entry in torque curve */
	4,			/* Top gear for automatic transmission */
	5750*rpmtordps,		/* Ang velocity for auto trans upshift */
	3500*rpmtordps		/* Ang velocity for auto trans downshift */
};

const struct car speedster={
	"SPEEDSTER",		/* Name of car */
	3525/32.2,			/* mass of car in slugs */

	{
#if 0
		321.0,		/* roll moment of inertia slug/ft^2 */
		1527.0,		/* pitch moment of inertia slug/ft^2 */
		1636.0	/* yaw moment of inertia slug/ft^2 */
#else
		1000.0,	/* roll moment of inertia slug/ft^2 */
		2000.0,
		2000.0	/* yaw moment of inertia slug/ft^2 */
#endif
	},

	/* spring rate of each wheel of car */
	{-1500,-1500,-1750,-1750},

	-1500,	/* front anti roll bar springrates */
	-0,	/* rear anti roll bar springrates */

	/* suspension damping of each wheel of car */
	{-400,-400,-500,-500},	/* Compression damping */
	{-200,-200,-250,-250},  /* Rebound damping */
		
	16.0,	/* steering ratio */
	4,	/* steering wheel torque per g */
	16,	/* Max steering damping, 0-16 */
	0x20,	/* Min steering friction 0-0x100 */
	0x100,	/* Max steering friction 0-0x100 */

/* aerodynamic drag and rolling resistance */

/*	.0087,*/	/* srefcybo2, 11 hp at 60 mph */
/*	35,*/	/* rollresist 6 hp at 60 mph   */
	.0067,	/* srefcybo2, less than corvette */
	75,	/* rollresist 6 hp at 60 mph   */

/* These are the tire description blocks for each wheel */
	{	
	    {	/* tires[0] (right front) */
		1.0,		/* tradius */
		340*12,		/* springK */
		200,		/* rubdamp */
		16000,		/* PaveCstiff used to be 16000*/
		1.20,		/* PaveCfmax */
		16000,		/* Cstiff used to be 16000*/
		1.20,		/* Cfmax */
		.1,		/* invmi */
		0,		/* Zforce */	
		0,		/* Afmax */
		0,		/* k1 */
		0,		/* k2 */
		0,		/* k3 */
		0,		/* l2 */
		0,		/* l3 */
		0,0,0,0,	/* m1,m2,m3,m4 */
		0,		/* patchy */
		0,		/* angvel */
		0,		/* sliptorque */
		0,		/* screech */
		0		/* slipflag */
	    },
	    {	/* tires[1] (left front) */
		1.0,		/* tradius */
		340*12,		/* springK */
		200,		/* rubdamp */
		16000,		/* PaveCstiff used to be 16000*/
		1.20,		/* PaveCfmax */
		16000,		/* Cstiff used to be 16000*/
		1.20,		/* Cfmax */
		.1,		/* invmi */
		0,		/* Zforce */	
		0,		/* Afmax */
		0,		/* k1 */
		0,		/* k2 */
		0,		/* k3 */
		0,		/* l2 */
		0,		/* l3 */
		0,0,0,0,	/* m1,m2,m3,m4 */
		0,		/* patchy */
		0,		/* angvel */
		0,		/* sliptorque */
		0,		/* screech */
		0		/* slipflag */
	    },
	    {	/* tires[2] (right rear) */
		1.0,		/* rtdes.tradius */
		340*12,		/* rtdes.springK */
		200,		/* rtdes.rubdamp */
		32000,		/* rtdes.PaveCstiff used to be 64000*/
		1.3,		/* rtdes.PaveCfmax */
		32000,		/* rtdes.Cstiff used to be 64000*/
		1.3,		/* rtdes.Cfmax */
		.1,		/* rtdes.invmi */
		0,		/* rtdes.Zforce */	
		0,		/* rtdes.Afmax */
		0,		/* rtdes.k1 */
		0,		/* rtdes.k2 */
		0,		/* rtdes.k3 */
		0,		/* rtdes.l2 */
		0,		/* rtdes.l3 */
		0,0,0,0,	/* rtdes.m1,m2,m3,m4 */
		0,		/* rtdes.patchy */
		0,		/* rtdes.angvel */
		0,		/* rtdes.sliptorque */
		0,		/* rtdes.screech */
		0		/* rtdes.slipflag */
	    },
	    {	/* tires[3] (left rear) */
		1.0,		/* rtdes.tradius */
		340*12,		/* rtdes.springK */
		200,		/* rtdes.rubdamp */
		32000,		/* rtdes.PaveCstiff used to be 64000*/
		1.3,		/* rtdes.PaveCfmax */
		32000,		/* rtdes.Cstiff used to be 64000*/
		1.3,		/* rtdes.Cfmax */
		.1,		/* rtdes.invmi */
		0,		/* rtdes.Zforce */	
		0,		/* rtdes.Afmax */
		0,		/* rtdes.k1 */
		0,		/* rtdes.k2 */
		0,		/* rtdes.k3 */
		0,		/* rtdes.l2 */
		0,		/* rtdes.l3 */
		0,0,0,0,	/* rtdes.m1,m2,m3,m4 */
		0,		/* rtdes.patchy */
		0,		/* rtdes.angvel */
		0,		/* rtdes.sliptorque */
		0,		/* rtdes.screech */
		0		/* rtdes.slipflag */
	    }
	},
		    
/* these are the distances from the CG to each wheel */
	{	/* TIRER */
		{4.033,2.75,1.0+.60},		/* right front */
		{4.033,-2.75,1.0+.60},		/* left front */
		{-3.967,2.75,1.0+.50},		/* right rear */
		{-3.967,-2.75,1.0+.50}		/* left rear */
	},

	1.0,			/* brakebal - brake balance at front wheel */
	.4,			/* moment of inertia of flywheel */

 	3.5,			/* Differential gear ratio */
	600.0,			/* maximum torque with clutch fully engaged */

	-2.0,			/* height from CG to eye level */

	3.0,			/* Collision width */
	6.0,			/* Collision length */
	20.0,			/* Collision height */
	0,			/* Nothrusttorque */
	0,			/* Magicdif */
	1.1,			/* fgtorquescale */
	1.1,			/* sgtorquescale */
	1.2,			/* torquescale */
	1.0,			/* dirttorquescale */
	{
		-2.4,				/*reverse*/
		0.0,				/*neutral*/
		2.4,				/*first gear*/
		1.7,				/*second gear*/
		1.25,				/*third gear*/
		0.95,				/*fourth gear*/
	},
	&stdtorquecurve[0][0],		/* Pointer to torque curve */
	&stddirttorquecurve[0][0],	/* Pointer to torque curve on dirt */
	1000,			/* RPM per horizontal entry in torque curve */
	4,			/* Top gear for automatic transmission */
	5750*rpmtordps,		/* Ang velocity for auto trans upshift */
	3500*rpmtordps		/* Ang velocity for auto trans downshift */
};
#endif

/*************************************************************************/

/* std = 1.25,1.30,16000,32000 */
/* flip edge 1.5,1.6,64000,128000*/
/* beg 1.4,1.5,64000,128000*/



#if 0
/* 3/4 weight */
#define WEIGHT (2350.0)
#define FSPRING (-1000)
#define RSPRING (-1167)
#define FCDAMP (-300)
#define RCDAMP (-375)
#define FRDAMP (-150)
#define RRDAMP (-188)
#endif

#if 0
/*stiff suspension*/
#define FSPRING (-2250)
#define RSPRING (-2625)
#define FCDAMP (-494)
#define RCDAMP (-612)
#define FRDAMP (-247)
#define RRDAMP (-306)
#endif

#if 0
/*alt stiff*/
#define FSPRING (-2250)
#define RSPRING (-2625)
#define FCDAMP (-494)
#define RCDAMP (-612)
#define FRDAMP (-370)
#define RRDAMP (-460)
#endif

#if 0
/*standard suspension*/
#define FSPRING (-1500)
#define RSPRING (-1750)
#define FCDAMP (-400)
#define RCDAMP (-500)
#define FRDAMP (-200)
#define RRDAMP (-250)
#endif

/* xxx */
/*** GENERIC VALUES ***/
/* suspension */
#define WEIGHT (3525.0)
#define CG_HEIGHT (0.50)
#define F_SUSCOMP (0.3)
#define R_SUSCOMP (0.25)
#define ROLL_MOI  (1000.0)
#define PITCH_MOI (1527.0)
#define YAW_MOI   (1636.0)
#define FSPRING (-3600)
#define RSPRING (-4200)
#define FAROLL (-1000)
#define RAROLL (-1000)
#define FCDAMP (-880)
#define RCDAMP (-1100)
#define FRDAMP (-480)
#define RRDAMP (-550)
/* engine */
#define TORQUE_SCALE (1.8)
#define RPM_SCALE (1150)
#define GEAR_RATIO_REV (-3.1)
#define GEAR_RATIO_NEU (0.0)
/* transmission */
#define UPSHIFT_RPM (6325)
#define DNSHIFT_RPM (4100)
/* tires */
#define TIRE_SPRINGK (340*12)
#define TIRE_RUBDAMP (200)
#define TIRE_FCFMAX  (2.6)
#define TIRE_RCFMAX  (2.8)
#define TIRE_FCSTIFF (16000)
#define TIRE_RCSTIFF (32000)


/*** CAR LEVEL VALUES ***/
/* first gear torque scale */
#define BEG_FGTOR (1.800)
#define ADV_FGTOR (1.803)
#define EXP_FGTOR (1.805)
#define EXT_FGTOR (1.810)
/* second gear torque scale */
#define BEG_SGTOR (1.800)
#define ADV_SGTOR (1.806)
#define EXP_SGTOR (1.812)
#define EXT_SGTOR (1.850)
/* third and forth gear torque scale */
#define BEG_TORSC (1.800)
#define ADV_TORSC (1.825)
#define EXP_TORSC (1.850)
#define EXT_TORSC (1.900)
/* first gear ratio */
#define BEG_GEAR_RATIO_1ST (3.10)
#define ADV_GEAR_RATIO_1ST (3.10)
#define EXP_GEAR_RATIO_1ST (3.10)
#define EXT_GEAR_RATIO_1ST (3.10)
/* second gear ratio */
#define BEG_GEAR_RATIO_2ND (1.78)
#define ADV_GEAR_RATIO_2ND (1.78)
#define EXP_GEAR_RATIO_2ND (1.77)
#define EXT_GEAR_RATIO_2ND (1.75)
/* third gear ratio */
#define BEG_GEAR_RATIO_3RD (1.29)
#define ADV_GEAR_RATIO_3RD (1.28)
#define EXP_GEAR_RATIO_3RD (1.27)
#define EXT_GEAR_RATIO_3RD (1.25)
/* forth gear ratio */
#define BEG_GEAR_RATIO_4TH (0.99)
#define ADV_GEAR_RATIO_4TH (0.98)
#define EXP_GEAR_RATIO_4TH (0.97)
#define EXT_GEAR_RATIO_4TH (0.95)

const struct car beginner={
	"BEGINNER",		/* Name of car */
	WEIGHT/32.2,		/* mass of car in slugs */

	/* roll, pitch, and yaw moment of inertia (slug/ft^2) */
	{ROLL_MOI,PITCH_MOI,YAW_MOI},

	/* spring rate of each wheel of car */
	{FSPRING,FSPRING,RSPRING,RSPRING},
	
	FAROLL,	/* front anti roll bar springrates */
	RAROLL,	/* rear anti roll bar springrates */

	/* suspension damping of each wheel of car */
	{FCDAMP,FCDAMP,RCDAMP,RCDAMP},	/* Compression damping */
	{FRDAMP,FRDAMP,RRDAMP,RRDAMP},  /* Rebound damping */
		
	16.0,	/* steering ratio */
	4,	/* steering wheel torque per g */
	16,	/* Max steering damping, 0-16 */
	0x20,	/* Min steering friction 0-0x100 */
	0x100,	/* Max steering friction 0-0x100 */

	/* aerodynamic drag and rolling resistance */
	.0087,	/* srefcybo2, 11 hp at 60 mph */
	75,	/* rollresist 6 hp at 60 mph   */

	/* These are the tire description blocks for each wheel */
	{	
	    {	/* tires[0] (right front) */
		1.0,		/* tradius */
		TIRE_SPRINGK,	/* springK */
		TIRE_RUBDAMP,	/* rubdamp */
		TIRE_FCSTIFF,	/* PaveCstiff */
		TIRE_FCFMAX,	/* PaveCfmax */
		TIRE_FCSTIFF,	/* Cstiff */
		TIRE_FCFMAX,	/* Cfmax */
		.1,		/* invmi */
		0,0,0,0,0,0,0,0,0,0,0,	/* Zforce,Afmax,k1,k2,k3,l2,l3,m1,m2,m3,m4 */
		0,0,0,0,0		/* patchy,angvel,sliptorque,screech,slipflag */
	    },
	    {	/* tires[1] (left front) */
		1.0,		/* tradius */
		TIRE_SPRINGK,	/* springK */
		TIRE_RUBDAMP,	/* rubdamp */
		TIRE_FCSTIFF,	/* PaveCstiff */
		TIRE_FCFMAX,	/* PaveCfmax */
		TIRE_FCSTIFF,	/* Cstiff */
		TIRE_FCFMAX,	/* Cfmax */
		.1,		/* invmi */
		0,0,0,0,0,0,0,0,0,0,0,	/* Zforce,Afmax,k1,k2,k3,l2,l3,m1,m2,m3,m4 */
		0,0,0,0,0		/* patchy,angvel,sliptorque,screech,slipflag */
	    },
	    {	/* tires[2] (right rear) */
		1.0,		/* rtdes.tradius */
		TIRE_SPRINGK,	/* rtdes.springK */
		TIRE_RUBDAMP,	/* rtdes.rubdamp */
		TIRE_RCSTIFF,	/* rtdes.PaveCstiff */
		TIRE_RCFMAX,	/* rtdes.PaveCfmax */
		TIRE_RCSTIFF,	/* rtdes.Cstiff */
		TIRE_RCFMAX,	/* rtdes.Cfmax */
		.1,		/* rtdes.invmi */
		0,0,0,0,0,0,0,0,0,0,0,	/* Zforce,Afmax,k1,k2,k3,l2,l3,m1,m2,m3,m4 */
		0,0,0,0,0		/* patchy,angvel,sliptorque,screech,slipflag */
	    },
	    {	/* tires[3] (left rear) */
		1.0,		/* rtdes.tradius */
		TIRE_SPRINGK,	/* rtdes.springK */
		TIRE_RUBDAMP,	/* rtdes.rubdamp */
		TIRE_RCSTIFF,	/* rtdes.PaveCstiff */
		TIRE_RCFMAX,	/* rtdes.PaveCfmax */
		TIRE_RCSTIFF,	/* rtdes.Cstiff */
		TIRE_RCFMAX,	/* rtdes.Cfmax */
		.1,		/* rtdes.invmi */
		0,0,0,0,0,0,0,0,0,0,0,	/* Zforce,Afmax,k1,k2,k3,l2,l3,m1,m2,m3,m4 */
		0,0,0,0,0		/* patchy,angvel,sliptorque,screech,slipflag */
	    }
	},

	/* these are the distances from the CG to each wheel */
	{	/* TIRER */
		{ 3.925, 2.75,CG_HEIGHT+F_SUSCOMP},	/* right front */
		{ 3.925,-2.75,CG_HEIGHT+F_SUSCOMP},	/* left front */
		{-3.925, 2.75,CG_HEIGHT+R_SUSCOMP},	/* right rear */
		{-3.925,-2.75,CG_HEIGHT+R_SUSCOMP}	/* left rear */
	},

	1.0,			/* brakebal - brake balance at front wheel */
	.4,			/* moment of inertia of flywheel */

 	3.3,			/* Differential gear ratio */
	600.0,			/* maximum torque with clutch fully engaged */

	-1.95,			/* height from CG to eye level */

	1,			/* Nothrusttorque */
	1,			/* Magicdif */
	BEG_FGTOR,		/* fgtorquescale */
	BEG_SGTOR,		/* sgtorquescale */
	BEG_TORSC,		/* torquescale */
	1.3,			/* dirttorquescale */
	{
		GEAR_RATIO_REV,	/*reverse*/
		GEAR_RATIO_NEU,	/*neutral*/
		BEG_GEAR_RATIO_1ST,	/*first gear*/
		BEG_GEAR_RATIO_2ND,	/*second gear*/
		BEG_GEAR_RATIO_3RD,	/*third gear*/
		BEG_GEAR_RATIO_4TH,	/*fourth gear*/
	},
	&stdtorquecurve[0][0],		/* Pointer to torque curve */
	&stddirttorquecurve[0][0],	/* Pointer to torque curve on dirt */
	RPM_SCALE,			/* RPM per horizontal entry in torque curve */
	4,			/* Top gear for automatic transmission */
	UPSHIFT_RPM*rpmtordps,		/* Ang velocity for auto trans upshift */
	DNSHIFT_RPM*rpmtordps		/* Ang velocity for auto trans downshift */
};

const struct car advanced={
	"ADVANCED",		/* Name of car */
	WEIGHT/32.2,		/* mass of car in slugs */

	/* roll, pitch, and yaw moment of inertia (slug/ft^2) */
	{ROLL_MOI,PITCH_MOI,YAW_MOI},

	/* spring rate of each wheel of car */
	{FSPRING,FSPRING,RSPRING,RSPRING},

	FAROLL,	/* front anti roll bar springrates */
	RAROLL,	/* rear anti roll bar springrates */

	/* suspension damping of each wheel of car */
	{FCDAMP,FCDAMP,RCDAMP,RCDAMP},	/* Compression damping */
	{FRDAMP,FRDAMP,RRDAMP,RRDAMP},  /* Rebound damping */
		
	16.0,	/* steering ratio */
	4,	/* steering wheel torque per g */
	16,	/* Max steering damping, 0-16 */
	0x20,	/* Min steering friction 0-0x100 */
	0x100,	/* Max steering friction 0-0x100 */

	/* aerodynamic drag and rolling resistance */
	.0087,	/* srefcybo2, 11 hp at 60 mph */
	75,	/* rollresist 6 hp at 60 mph   */

	/* These are the tire description blocks for each wheel */
	{	
	    {	/* tires[0] (right front) */
		1.0,		/* tradius */
		TIRE_SPRINGK,	/* springK */
		TIRE_RUBDAMP,	/* rubdamp */
		TIRE_FCSTIFF,	/* PaveCstiff */
		TIRE_FCFMAX,	/* PaveCfmax */
		TIRE_FCSTIFF,	/* Cstiff */
		TIRE_FCFMAX,	/* Cfmax */
		.1,		/* invmi */
		0,0,0,0,0,0,0,0,0,0,0,	/* Zforce,Afmax,k1,k2,k3,l2,l3,m1,m2,m3,m4 */
		0,0,0,0,0		/* patchy,angvel,sliptorque,screech,slipflag */
	    },
	    {	/* tires[1] (left front) */
		1.0,		/* tradius */
		TIRE_SPRINGK,	/* springK */
		TIRE_RUBDAMP,	/* rubdamp */
		TIRE_FCSTIFF,	/* PaveCstiff */
		TIRE_FCFMAX,	/* PaveCfmax */
		TIRE_FCSTIFF,	/* Cstiff */
		TIRE_FCFMAX,	/* Cfmax */
		.1,		/* invmi */
		0,0,0,0,0,0,0,0,0,0,0,	/* Zforce,Afmax,k1,k2,k3,l2,l3,m1,m2,m3,m4 */
		0,0,0,0,0		/* patchy,angvel,sliptorque,screech,slipflag */
	    },
	    {	/* tires[2] (right rear) */
		1.0,		/* rtdes.tradius */
		TIRE_SPRINGK,	/* rtdes.springK */
		TIRE_RUBDAMP,	/* rtdes.rubdamp */
		TIRE_RCSTIFF,	/* rtdes.PaveCstiff */
		TIRE_RCFMAX,	/* rtdes.PaveCfmax */
		TIRE_RCSTIFF,	/* rtdes.Cstiff */
		TIRE_RCFMAX,	/* rtdes.Cfmax */
		.1,		/* rtdes.invmi */
		0,0,0,0,0,0,0,0,0,0,0,	/* Zforce,Afmax,k1,k2,k3,l2,l3,m1,m2,m3,m4 */
		0,0,0,0,0		/* patchy,angvel,sliptorque,screech,slipflag */
	    },
	    {	/* tires[3] (left rear) */
		1.0,		/* rtdes.tradius */
		TIRE_SPRINGK,	/* rtdes.springK */
		TIRE_RUBDAMP,	/* rtdes.rubdamp */
		TIRE_RCSTIFF,	/* rtdes.PaveCstiff */
		TIRE_RCFMAX,	/* rtdes.PaveCfmax */
		TIRE_RCSTIFF,	/* rtdes.Cstiff */
		TIRE_RCFMAX,	/* rtdes.Cfmax */
		.1,		/* rtdes.invmi */
		0,0,0,0,0,0,0,0,0,0,0,	/* Zforce,Afmax,k1,k2,k3,l2,l3,m1,m2,m3,m4 */
		0,0,0,0,0		/* patchy,angvel,sliptorque,screech,slipflag */
	    }
	},

	/* these are the distances from the CG to each wheel */
	{	/* TIRER */
		{ 3.925, 2.75,CG_HEIGHT+F_SUSCOMP},	/* right front */
		{ 3.925,-2.75,CG_HEIGHT+F_SUSCOMP},	/* left front */
		{-3.925, 2.75,CG_HEIGHT+R_SUSCOMP},	/* right rear */
		{-3.925,-2.75,CG_HEIGHT+R_SUSCOMP}	/* left rear */
	},

	1.0,			/* brakebal - brake balance at front wheel */
	.4,			/* moment of inertia of flywheel */

 	3.3,			/* Differential gear ratio */
	600.0,			/* maximum torque with clutch fully engaged */

	-1.95,			/* height from CG to eye level */

	1,			/* Nothrusttorque */
	1,			/* Magicdif */
	ADV_FGTOR,		/* fgtorquescale */
	ADV_SGTOR,		/* sgtorquescale */
	ADV_TORSC,		/* torquescale */
	1.3,			/* dirttorquescale */
	{
		GEAR_RATIO_REV,	/*reverse*/
		GEAR_RATIO_NEU,	/*neutral*/
		ADV_GEAR_RATIO_1ST,	/*first gear*/
		ADV_GEAR_RATIO_2ND,	/*second gear*/
		ADV_GEAR_RATIO_3RD,	/*third gear*/
		ADV_GEAR_RATIO_4TH,	/*fourth gear*/
	},
	&stdtorquecurve[0][0],		/* Pointer to torque curve */
	&stddirttorquecurve[0][0],	/* Pointer to torque curve on dirt */
	RPM_SCALE,			/* RPM per horizontal entry in torque curve */
	4,			/* Top gear for automatic transmission */
	UPSHIFT_RPM*rpmtordps,		/* Ang velocity for auto trans upshift */
	DNSHIFT_RPM*rpmtordps		/* Ang velocity for auto trans downshift */
};

const struct car expert={
	"EXPERT",		/* Name of car */
	WEIGHT/32.2,		/* mass of car in slugs */

	/* roll, pitch, and yaw moment of inertia (slug/ft^2) */
	{ROLL_MOI,PITCH_MOI,YAW_MOI},

	/* spring rate of each wheel of car */
	{FSPRING,FSPRING,RSPRING,RSPRING},

	FAROLL,	/* front anti roll bar springrates */
	RAROLL,	/* rear anti roll bar springrates */

	/* suspension damping of each wheel of car */
	{FCDAMP,FCDAMP,RCDAMP,RCDAMP},	/* Compression damping */
	{FRDAMP,FRDAMP,RRDAMP,RRDAMP},  /* Rebound damping */
		
	16.0,	/* steering ratio */
	4,	/* steering wheel torque per g */
	16,	/* Max steering damping, 0-16 */
	0x20,	/* Min steering friction 0-0x100 */
	0x100,	/* Max steering friction 0-0x100 */

	/* aerodynamic drag and rolling resistance */
	.0087,	/* srefcybo2, 11 hp at 60 mph */
	75,	/* rollresist 6 hp at 60 mph   */

	/* These are the tire description blocks for each wheel */
	{	
	    {	/* tires[0] (right front) */
		1.0,		/* tradius */
		TIRE_SPRINGK,	/* springK */
		TIRE_RUBDAMP,	/* rubdamp */
		TIRE_FCSTIFF,	/* PaveCstiff */
		TIRE_FCFMAX,	/* PaveCfmax */
		TIRE_FCSTIFF,	/* Cstiff */
		TIRE_FCFMAX,	/* Cfmax */
		.1,		/* invmi */
		0,0,0,0,0,0,0,0,0,0,0,	/* Zforce,Afmax,k1,k2,k3,l2,l3,m1,m2,m3,m4 */
		0,0,0,0,0		/* patchy,angvel,sliptorque,screech,slipflag */
	    },
	    {	/* tires[1] (left front) */
		1.0,		/* tradius */
		TIRE_SPRINGK,	/* springK */
		TIRE_RUBDAMP,	/* rubdamp */
		TIRE_FCSTIFF,	/* PaveCstiff */
		TIRE_FCFMAX,	/* PaveCfmax */
		TIRE_FCSTIFF,	/* Cstiff */
		TIRE_FCFMAX,	/* Cfmax */
		.1,		/* invmi */
		0,0,0,0,0,0,0,0,0,0,0,	/* Zforce,Afmax,k1,k2,k3,l2,l3,m1,m2,m3,m4 */
		0,0,0,0,0		/* patchy,angvel,sliptorque,screech,slipflag */
	    },
	    {	/* tires[2] (right rear) */
		1.0,		/* rtdes.tradius */
		TIRE_SPRINGK,	/* rtdes.springK */
		TIRE_RUBDAMP,	/* rtdes.rubdamp */
		TIRE_RCSTIFF,	/* rtdes.PaveCstiff */
		TIRE_RCFMAX,	/* rtdes.PaveCfmax */
		TIRE_RCSTIFF,	/* rtdes.Cstiff */
		TIRE_RCFMAX,	/* rtdes.Cfmax */
		.1,		/* rtdes.invmi */
		0,0,0,0,0,0,0,0,0,0,0,	/* Zforce,Afmax,k1,k2,k3,l2,l3,m1,m2,m3,m4 */
		0,0,0,0,0		/* patchy,angvel,sliptorque,screech,slipflag */
	    },
	    {	/* tires[3] (left rear) */
		1.0,		/* rtdes.tradius */
		TIRE_SPRINGK,	/* rtdes.springK */
		TIRE_RUBDAMP,	/* rtdes.rubdamp */
		TIRE_RCSTIFF,	/* rtdes.PaveCstiff */
		TIRE_RCFMAX,	/* rtdes.PaveCfmax */
		TIRE_RCSTIFF,	/* rtdes.Cstiff */
		TIRE_RCFMAX,	/* rtdes.Cfmax */
		.1,		/* rtdes.invmi */
		0,0,0,0,0,0,0,0,0,0,0,	/* Zforce,Afmax,k1,k2,k3,l2,l3,m1,m2,m3,m4 */
		0,0,0,0,0		/* patchy,angvel,sliptorque,screech,slipflag */
	    }
	},

	/* these are the distances from the CG to each wheel */
	{	/* TIRER */
		{ 3.925, 2.75,CG_HEIGHT+F_SUSCOMP},	/* right front */
		{ 3.925,-2.75,CG_HEIGHT+F_SUSCOMP},	/* left front */
		{-3.925, 2.75,CG_HEIGHT+R_SUSCOMP},	/* right rear */
		{-3.925,-2.75,CG_HEIGHT+R_SUSCOMP}	/* left rear */
	},

	1.0,			/* brakebal - brake balance at front wheel */
	.4,			/* moment of inertia of flywheel */

 	3.3,			/* Differential gear ratio */
	600.0,			/* maximum torque with clutch fully engaged */

	-1.95,			/* height from CG to eye level */

	1,			/* Nothrusttorque */
	1,			/* Magicdif */
	EXP_FGTOR,		/* fgtorquescale */
	EXP_SGTOR,		/* sgtorquescale */
	EXP_TORSC,		/* torquescale */
	1.3,			/* dirttorquescale */
	{
		GEAR_RATIO_REV,	/*reverse*/
		GEAR_RATIO_NEU,	/*neutral*/
		EXP_GEAR_RATIO_1ST,	/*first gear*/
		EXP_GEAR_RATIO_2ND,	/*second gear*/
		EXP_GEAR_RATIO_3RD,	/*third gear*/
		EXP_GEAR_RATIO_4TH,	/*fourth gear*/
	},
	&stdtorquecurve[0][0],		/* Pointer to torque curve */
	&stddirttorquecurve[0][0],	/* Pointer to torque curve on dirt */
	RPM_SCALE,			/* RPM per horizontal entry in torque curve */
	4,			/* Top gear for automatic transmission */
	UPSHIFT_RPM*rpmtordps,		/* Ang velocity for auto trans upshift */
	DNSHIFT_RPM*rpmtordps		/* Ang velocity for auto trans downshift */
};

const struct car extreme={
	"EXTREME",		/* Name of car */
	WEIGHT/32.2,		/* mass of car in slugs */

	/* roll, pitch, and yaw moment of inertia (slug/ft^2) */
	{ROLL_MOI,PITCH_MOI,YAW_MOI},

	/* spring rate of each wheel of car */
	{FSPRING,FSPRING,RSPRING,RSPRING},

	FAROLL,	/* front anti roll bar springrates */
	RAROLL,	/* rear anti roll bar springrates */

	/* suspension damping of each wheel of car */
	{FCDAMP,FCDAMP,RCDAMP,RCDAMP},	/* Compression damping */
	{FRDAMP,FRDAMP,RRDAMP,RRDAMP},  /* Rebound damping */
		
	16.0,	/* steering ratio */
	4,	/* steering wheel torque per g */
	16,	/* Max steering damping, 0-16 */
	0x20,	/* Min steering friction 0-0x100 */
	0x100,	/* Max steering friction 0-0x100 */

	/* aerodynamic drag and rolling resistance */
	.0087,	/* srefcybo2, 11 hp at 60 mph */
	75,	/* rollresist 6 hp at 60 mph   */

	/* These are the tire description blocks for each wheel */
	{	
	    {	/* tires[0] (right front) */
		1.0,		/* tradius */
		TIRE_SPRINGK,	/* springK */
		TIRE_RUBDAMP,	/* rubdamp */
		TIRE_FCSTIFF,	/* PaveCstiff */
		TIRE_FCFMAX,	/* PaveCfmax */
		TIRE_FCSTIFF,	/* Cstiff */
		TIRE_FCFMAX,	/* Cfmax */
		.1,		/* invmi */
		0,0,0,0,0,0,0,0,0,0,0,	/* Zforce,Afmax,k1,k2,k3,l2,l3,m1,m2,m3,m4 */
		0,0,0,0,0		/* patchy,angvel,sliptorque,screech,slipflag */
	    },
	    {	/* tires[1] (left front) */
		1.0,		/* tradius */
		TIRE_SPRINGK,	/* springK */
		TIRE_RUBDAMP,	/* rubdamp */
		TIRE_FCSTIFF,	/* PaveCstiff */
		TIRE_FCFMAX,	/* PaveCfmax */
		TIRE_FCSTIFF,	/* Cstiff */
		TIRE_FCFMAX,	/* Cfmax */
		.1,		/* invmi */
		0,0,0,0,0,0,0,0,0,0,0,	/* Zforce,Afmax,k1,k2,k3,l2,l3,m1,m2,m3,m4 */
		0,0,0,0,0		/* patchy,angvel,sliptorque,screech,slipflag */
	    },
	    {	/* tires[2] (right rear) */
		1.0,		/* rtdes.tradius */
		TIRE_SPRINGK,	/* rtdes.springK */
		TIRE_RUBDAMP,	/* rtdes.rubdamp */
		TIRE_RCSTIFF,	/* rtdes.PaveCstiff */
		TIRE_RCFMAX,	/* rtdes.PaveCfmax */
		TIRE_RCSTIFF,	/* rtdes.Cstiff */
		TIRE_RCFMAX,	/* rtdes.Cfmax */
		.1,		/* rtdes.invmi */
		0,0,0,0,0,0,0,0,0,0,0,	/* Zforce,Afmax,k1,k2,k3,l2,l3,m1,m2,m3,m4 */
		0,0,0,0,0		/* patchy,angvel,sliptorque,screech,slipflag */
	    },
	    {	/* tires[3] (left rear) */
		1.0,		/* rtdes.tradius */
		TIRE_SPRINGK,	/* rtdes.springK */
		TIRE_RUBDAMP,	/* rtdes.rubdamp */
		TIRE_RCSTIFF,	/* rtdes.PaveCstiff */
		TIRE_RCFMAX,	/* rtdes.PaveCfmax */
		TIRE_RCSTIFF,	/* rtdes.Cstiff */
		TIRE_RCFMAX,	/* rtdes.Cfmax */
		.1,		/* rtdes.invmi */
		0,0,0,0,0,0,0,0,0,0,0,	/* Zforce,Afmax,k1,k2,k3,l2,l3,m1,m2,m3,m4 */
		0,0,0,0,0		/* patchy,angvel,sliptorque,screech,slipflag */
	    }
	},

	/* these are the distances from the CG to each wheel */
	{	/* TIRER */
		{ 3.925, 2.75,CG_HEIGHT+F_SUSCOMP},	/* right front */
		{ 3.925,-2.75,CG_HEIGHT+F_SUSCOMP},	/* left front */
		{-3.925, 2.75,CG_HEIGHT+R_SUSCOMP},	/* right rear */
		{-3.925,-2.75,CG_HEIGHT+R_SUSCOMP}	/* left rear */
	},

	1.0,			/* brakebal - brake balance at front wheel */
	.4,			/* moment of inertia of flywheel */

 	3.3,			/* Differential gear ratio */
	600.0,			/* maximum torque with clutch fully engaged */

	-1.95,			/* height from CG to eye level */

	1,			/* Nothrusttorque */
	1,			/* Magicdif */
	EXT_FGTOR,		/* fgtorquescale */
	EXT_SGTOR,		/* sgtorquescale */
	EXT_TORSC,		/* torquescale */
	1.3,			/* dirttorquescale */
	{
		GEAR_RATIO_REV,	/*reverse*/
		GEAR_RATIO_NEU,	/*neutral*/
		EXT_GEAR_RATIO_1ST,	/*first gear*/
		EXT_GEAR_RATIO_2ND,	/*second gear*/
		EXT_GEAR_RATIO_3RD,	/*third gear*/
		EXT_GEAR_RATIO_4TH,	/*fourth gear*/
	},
	&stdtorquecurve[0][0],		/* Pointer to torque curve */
	&stddirttorquecurve[0][0],	/* Pointer to torque curve on dirt */
	RPM_SCALE,			/* RPM per horizontal entry in torque curve */
	4,			/* Top gear for automatic transmission */
	UPSHIFT_RPM*rpmtordps,		/* Ang velocity for auto trans upshift */
	DNSHIFT_RPM*rpmtordps		/* Ang velocity for auto trans downshift */
};

#if 1
const COLLSIZE collsize[NCARS] = {
/*           		front	rear	side	height	*/
/* BMW     */	{	5.75,	6.50,	3.0,	2.0	},
/* INTEGRA */	{	6.29,	6.58,	2.9,	2.0	},
/* VETTE   */	{	6.05,	6.65,	2.9,	2.0	},
/* BUGATTI */	{	6.25,	5.75,	3.0,	2.0	},
/* TAXI    */	{	6.44,	6.64,	3.0,	2.0	},
/* VIPER   */	{	6.25,	6.75,	3.0,	2.0	},
/* BUG     */	{	5.75,	6.00,	3.0,	2.0	},
/* TRUCK   */	{	6.70,	6.97,	2.8,	2.0	},
/* BUS     */	{	6.25,	6.00,	3.0,	2.0	},
/* CAMERO  */	{	6.75,	7.00,	3.0,	2.0	},
/* SUPRA   */	{	6.50,	6.50,	3.0,	2.0	},
/* CONCEPT */	{	6.25,	5.50,	3.0,	2.0	},
};
#endif

#if 1
const struct car * const carlist[]={
	&beginner,	/* BEGINNER model. */
	&advanced,	/* ADVANCED model. */
	&expert,	/* EXPERT   model. */
	&extreme,	/* EXTREME  model. */
};
#else
const struct car * const carlist[]={
	&corvette,
	&roadster,
	&speedster,
	&crownvic,	/* Crown victoria */
	&shorttrkcar,	/* New car 2 for test purposes */
	&rushcar,	/* S.F.Rush car (for dynamic tweaking) */
};
#endif
const short num_cars=sizeof(carlist)/sizeof(*carlist);	/* This is the number
							of cars in the array*/
