/*
 *	fswheel.c -- force steering wheel
 *
 *	======================================================================
 *	$Author: gray $						$Date: 1997/09/19 04:47:44 $
 *	$Revision: 3.16 $						$Locker:  $
 *	======================================================================
 *	Change Log:
 *
 *	$Log: fswheel.c,v $
 * Revision 3.16  1997/09/19  04:47:44  gray
 * Make sure wheel selection is valid.
 *
 * Revision 3.15  1997/06/04  02:15:46  gray
 * Restructuring controls (gas,brake,clutch,etc) handling.
 *
 * Revision 3.14  1997/05/30  07:53:15  gray
 * fastout is history.
 *
 * Revision 3.13  1997/05/11  03:32:58  gray
 * Cleaning wheel pos stuff.
 *
 * Revision 3.12  1997/05/08  16:24:49  gray
 * Cleaned out old stuff.
 *
 * Revision 3.11  1996/11/14  12:49:43  geraci
 * move raw wheel range define to controls
 *
 * Revision 3.10  1996/11/11  09:18:44  geraci
 * new way to deal with pots
 *
 * Revision 3.9  1996/10/10  05:19:24  petrick
 * Cleaned out some unused globals.
 *
 * Revision 3.8  1996/10/08  05:20:58  gray
 * Flagstaff port.
 *
 * Revision 3.7  1996/10/05  09:09:28  geraci
 * new routine that returns current steering wheel notch number
 *
 * Revision 3.6  1996/10/04  06:54:20  petrick
 * Moved macro def 'rng' to dis.h.
 *
 * Revision 3.5  1996/09/23  20:38:59  petrick
 * Brought back offcenter_wheel.
 *
 * Revision 3.4  1996/09/23  20:34:06  gray
 * Changed the force_frac to gFeedback x gOffset table.
 * /
 *
 * Revision 3.3  1996/09/23  07:43:35  petrick
 * Minor cleanup.
 *
 * Revision 3.2  1996/09/21  04:12:53  petrick
 * Got rid of offcenter_wheel, obsolete.
 *
 * Revision 3.1  1996/09/09  02:24:52  gray
 * JAMMA version
 *
 * Revision 2.22  1996/09/06  02:36:54  petrick
 * Light steering option.
 *
 * Revision 2.21  1996/08/21  03:18:30  petrick
 * Got rid of stack specific stuff.
 *
 * Revision 2.20  1996/08/20  09:00:51  geraci
 * better steering wheel select/attract forces
 *
 * Revision 2.19  1996/08/19  08:17:03  petrick
 * MAde steering wheel seek center while in attract.
 *
 * Revision 2.18  1996/08/18  20:53:09  gray
 * Made Nucleus default, removed flag.
 *
 * Revision 2.17  1996/08/17  07:28:09  geraci
 * new high score character selector
 *
 * Revision 2.16  1996/08/03  04:27:25  petrick
 * Made wheel range an actual value.
 *
 * Revision 2.15  1996/08/02  22:32:18  petrick
 * Extended steering range slightly.
 *
 * Revision 2.14  1996/08/02  08:01:18  gray
 * Fix select forces. Better driving force.
 *
 * Revision 2.13  1996/08/01  05:06:03  gray
 * Debug steering wheel.
 *
 * Revision 2.12  1996/07/31  07:33:04  gray
 * Changes for new ALANSTACK.
 *
 * Revision 2.11  1996/07/30  00:15:11  gray
 * Got rid of non-linear steering pos. Dont need with faster cars.
 *
 * Revision 2.10  1996/07/22  00:07:25  gray
 * Fixed wheel damping after update frequency changed.
 *
 * Revision 2.9  1996/07/20  00:18:51  gray
 * Slimmed timer interrrupt routine. wheelpos() and forcewheel() moved.
 *
 * Revision 2.8  1996/07/18  06:25:52  geraci
 * new stand alone version support
 *
 * Revision 2.7  1996/07/12  12:26:30  petrick
 * New car select changes, workaround hack to get all stacks working.
 *
 * Revision 2.6  1996/06/28  04:59:47  geraci
 * cathy has a new steering wheel
 *
 * Revision 2.5  1996/06/13  23:55:17  geraci
 * new calibration code for pot-based steering wheels
 *
 * Revision 2.4  1996/06/08  01:36:12  geraci
 * new cabinet for john stack
 *
 * Revision 2.3  1996/05/17  21:22:27  petrick
 * Changed order of SMSG params.
 *
 * Revision 2.2  1996/05/11  09:02:37  petrick
 * Better notches on FOCL & FOCR stacks.
 *
 * Revision 2.1  1996/05/04  02:56:18  petrick
 * Focus 2 release
 *
 * Revision 1.45  1996/05/02  00:19:26  petrick
 * Changed select wheel forces.
 *
 * Revision 1.44  1996/05/01  03:20:46  geraci
 * both focus stacks have new steering wheel
 *
 * Revision 1.43  1996/04/29  22:16:17  geraci
 * recenters wheel during resurrect
 *
 * Revision 1.42  1996/04/24  23:39:45  petrick
 * Put select defaults back to normal.
 *
 * Revision 1.41  1996/04/23  22:58:19  gray
 * Put in controls for Race Drivin wheel.
 *
 * Revision 1.40  1996/04/15  04:21:00  gray
 * First pass at 270 degree wheel whith pot (no opto).
 *
 * Revision 1.39  1996/04/09  23:52:59  petrick
 * Added extra wheel positioning routine.
 *
 * Revision 1.38  1996/04/03  00:32:00  gray
 * More new motor amp tweaks.
 *
 * Revision 1.37  1996/04/02  10:12:23  gray
 * Playing with ModelSteerForce with new tire traction.
 *
 * Revision 1.36  1996/04/01  09:13:19  petrick
 * Better select force override and got rid of more unused vars.
 *
 * Revision 1.34  1996/04/01  06:09:07  petrick
 * Added some dampening to the select steering force.
 *
 * Revision 1.33  1996/04/01  01:37:37  petrick
 * Got rid of feedback level adjust.
 *
 * Revision 1.32  1996/03/31  11:11:07  gray
 * Complete overhaul of wheelpos() and doforcewheel(). Clean setswforce().
 * Tuning for double force motor amp and Happ motor.
 *
 * Revision 1.31  1996/03/27  22:38:19  petrick
 * Put back a simple notch.
 *
 * Revision 1.30  1996/03/27  05:58:23  petrick
 * Changed scale factor for force, got rid of notch at end.
 *
 * Revision 1.29  1996/03/26  04:08:41  petrick
 * New explosion method.
 *
 * Revision 1.28  1996/03/23  02:20:31  geraci
 * wheel test for pete
 *
 * Revision 1.27  1996/03/21  11:02:27  petrick
 * Added better notches to selects, made select screen elements vanish when not active.
 *
 * Revision 1.26  1996/03/21  06:59:03  petrick
 * Added notches to selects and high score entry.
 *
 * Revision 1.25  1996/03/20  22:56:04  geraci
 * both focus stack have new steering wheel
 *
 * Revision 1.24  1996/03/12  20:38:50  geraci
 * new steering wheel for focus stack
 *
 * Revision 1.23  1996/03/12  10:02:40  geraci
 * new steering wheel for focus
 *
 * Revision 1.22  1996/02/28  21:28:56  gray
 * Fixed so wheel force doesnt push wheel past 1 1/2 turns.
 *
 * Revision 1.21  1996/02/28  04:12:25  petrick
 * Got rid of 12-bit pot reference.
 *
 * Revision 1.20  1996/02/27  13:55:07  gray
 * Include cleanup.
 *
 * Revision 1.19  1996/02/22  00:58:33  gray
 * Maintains wheel center when turned more than 1 1/2 turns. Notches at limits.
 *
 * Revision 1.18  1996/02/07  02:44:53  geraci
 * put defines from gstate.h into game.h; made PLAYGAME state
 *
 * Revision 1.17  1996/01/20  04:44:02  petrick
 * Changed 'trackno' to S16.
 *
 * Revision 1.16  1996/01/20  03:41:06  geraci
 * Different stacks are controlled with debug switches
 *
 * Revision 1.15  1996/01/19  02:50:14  geraci
 * changed wheel delay for most stacks
 *
 * Revision 1.14  1996/01/11  22:03:38  geraci
 * asta la vista BETSY
 *
 *
 *
 *	======================================================================
 */

#include "config.h"
#include "model.h"
#include "controls.h"
#include "game.h"
#include "globals.h"
#include "debug.h"
#include "dis.h"

#include "Pro/fswheel.pro"
#include "Pro/select.pro"


#define SWFMIN 		(-127)
#define SWFMAX 		(127)

/* force_frac[gFeedback][gOffset] */
/* Percent of total force at various feedback levels. */
static const S16	force_frac[3][3] = {
				{ 55,	60,	90	},
				{ 60,	80,	95	},
				{ 65,	95,	100	}	};


const S16 play = 20;
const S16 swtorquefres = 3;
S16 playpos;
S16 rawswforce;
S16 velfres;

S16 	rawswvel;

S32 oldrawpos;

static S16 	swfrtor;
static S16	oldpos,swfrk,swfrblk;
static S16 	filtswtorque;
static S32	avg_sw_vel, prev_notch;
static S16	doing_notch;
static S32	last_notch;

S16 		swvel;
S16 		gFscale;
S16			last_swforce,tottorque,scaletorque;
U32			avg_sw_force;
S16			recenter_flag;

extern S16	WHEEL_MIN, WHEEL_MAX, WHEEL_RNG;
extern S16	gForceOffset;

extern void AttractForce(void);

/*---------------------------------------------------------------------------------*/
void InitSW(void)
{
	gSWforce		= 0;
	playpos			= 0;
	rawswforce		= 0;
	oldpos			= 0;
	swvel			= 0;
	rawswvel		= 0;
	avg_sw_vel		= 0;

	gSWtorque 		= 0;
	filtswtorque 	= 0;
}	/* end InitSW */

/*---------------------------------------------------------------------------------*/
/* Return wheel position. Scale so that:		*/
/* full turn CCW = -0x800, full turn CW = 0x800	*/
/* This scaling expected by model.			*/
S16 wheelpos(void)
{
	return(PotWheelPos());
} /* end wheelpos */


S32 xxxpos;

S16 PotWheelPos(void)
{
	S16 diff;
	S32 pos;
	extern U8 ad8val[8];

	pos = (((U32)(ad8val[ADC_WHEEL] - WHEEL_MIN) << 12)/WHEEL_RNG) - 0x800;

	/* velocity in tics/sec. Move 1/2 of the delta each model iteration.*/
	/* vel = oldvel + ((newvel - oldvel)/2) */
	rawswvel = (rawswvel + ((pos - oldrawpos) * 200)) >> 1;	/*200 model iterations/sec */
	avg_sw_vel = avg_sw_vel - (avg_sw_vel>>3) + rawswvel; /* this is retarded! */
	oldrawpos = pos;

	diff = pos - playpos;
	if(diff > play)
		playpos = pos - play;
	else if(diff < -play)
		playpos = pos + play;
	else if(pos > playpos)
		playpos++;
	else if(pos < playpos)
		playpos--;

	swvel = (swvel + ((playpos - xxxpos) * 2)) >> 1;
	swvel = ((playpos - xxxpos) * 6)>>2; /* x 1.5 */
	xxxpos = playpos;
	oldpos=playpos;

	return(playpos);
}	/* end wheelpos */

/*---------------------------------------------------------------------------------*/
void freewheel(void)
{
	gSWfrict = 0;
	gSWdamp  = 4;
	gSWtorque = 0;
	velfres  = 8;
}	/* end freewheel */


/*---------------------------------------------------------------------------------*/
void frictwheel(void)
{
	S16 i;

	swfrblk		= gSWpos;                /* Move friction block to wheel pos*/
	gSWfrict		= 32;
	swfrk 		= -16;
	gSWdamp		= 4;
	velfres 	= 8;
	gSWtorque 	= 0;

}	/* end frictwheel */
	
/*---------------------------------------------------------------------------------*/
void setswforce(S16 force)
{
	extern void SetForce(S16);
	U8	opval;
	U16 t;
	
	force	 = rng(force, SWFMIN, SWFMAX); 	/* Make sure the force is limited */

	avg_sw_force -= (avg_sw_force>>14);
	if (force < 0)
		avg_sw_force -= force;
	else
		avg_sw_force += force;

	last_swforce = force;

	SetForce(force);

}	/* end setswforce */

/*---------------------------------------------------------------------------------*/
void ModelSteerForce(void)
{
	S16 torque,adjswpos;
	S16 fmax;

	if (recenter_flag == true)
		{
		force_wheel_to_center (0, 3, 10, 80);
		return;
		}

	adjswpos = gSWpos + (S16)(rawswvel/(S16)16);

	filtswtorque += (gSWtorque - filtswtorque)>>swtorquefres;

	torque = rng(filtswtorque >> 3,-512,512);

	/* Clip torque from model */
	torque = rng(torque,-360,360);

	/* Add friction */
	swfrtor = (S16)(((S16)(adjswpos - swfrblk) * swfrk)/0x10);

	if(swfrtor > gSWfrict)
		{
		swfrtor = gSWfrict;
		swfrblk = adjswpos - (S16)((swfrtor<<4)/(S16)swfrk);
		}

	if(swfrtor < -gSWfrict)
		{
		swfrtor = -gSWfrict;
		swfrblk = adjswpos - (S16)((swfrtor<<4)/(S16)swfrk);
		}

	/* Set steering wheel force based on model, friction,servo torque, and damping */
	tottorque = torque + swfrtor - (swvel * gSWdamp);

	/* should scale by 1/4 for RD 9-bit to SFR 7-bit */
	/* but want FULL force to max out early, so x 1/2 */
	scaletorque = (tottorque >> 2); /* x 1/4 */

	/* apply force scale */
	gFscale = force_frac[gFeedbackLevel][gForceOffset];
	scaletorque = (scaletorque * gFscale)/100;

	/* limit max torque for any scale less than FULL */
	fmax = (SWFMAX * gFscale)/100;
	scaletorque = rng(scaletorque,-fmax,fmax);

	gSWforce = scaletorque;

}	/* end ModelSteerForce */


/* ----------------------------------------------------------------------------- *
 *	Handles steering force when in select and high score entry.  
 *	Used with WheelSection.
 *  mode (force wheel to default): 0=init, 1=allow default, 2=disable default
 * ----------------------------------------------------------------------------- */
void OverrideForce(S16 sections, S16 max, S16 mult, S16 mode, S16 range_scale)
{
	S16         force;
	S32         pos, goto_pos, range;
	static S16  override_mode;
	static S32  override_time;

	switch (mode)
		{
		case 0:
			override_time = IRQTIME;
			override_mode = true;
			break;

		case 1:
			if (IRQTIME - override_time > 2 * ONE_SEC)
				override_mode = false;
			break;

		case 2:
			override_mode = false;
			break;
		}

	force = 0;
	range = RAW_WHEEL_RANGE/range_scale;

	pos = WheelSection (sections, range_scale);

	if (override_mode)
		{
		if (pos == 0)
			override_mode = false;
		else
			pos = 0;
		}

	if (pos != prev_notch)
		{
		doing_notch = 1;
		prev_notch = pos;
		}

	if (doing_notch > 3)
		doing_notch = 0;
	else if (doing_notch > 1)
		doing_notch++;
	else if (doing_notch > 0)
		{
		doing_notch++;
		force = -avg_sw_vel>>(1+mult);
		}

	if (doing_notch == 0)
		{
		goto_pos = pos*range*2/(sections-1) - range;
		force_wheel_to_center (goto_pos, mult, 11, max);
		}
	else
		{
		gSWforce = rng (force,-40,40);
		}

}	/* end OverrideForce */


/* ----------------------------------------------------------------------------- *
 *	Returns a value based on wheel position.  Use in conjuction with OverrideForce
 * ----------------------------------------------------------------------------- */
S32	WheelSection( S32 nsections, S32 mult )
{
	S32 cur_notch, real_notch, range;

	range = RAW_WHEEL_RANGE / mult;

	cur_notch = rng((gSWpos+range) /((2*range)/(nsections<<1)),0,(nsections<<1)-1);
	if ((last_notch >= nsections) || (abs(last_notch-cur_notch) >= 2))
		{
		last_notch = cur_notch;
		real_notch = cur_notch>>1;
		}
	else
		real_notch = last_notch>>1;

	return real_notch;

} /* end WheelSection */

/* ------------------------------------------------------------------------- *
 *	Returns a value based on real wheel position. (No hysteresis)
 * ------------------------------------------------------------------------- */
S32	RawWheelSection( S32 nsections, S32 mult )
{
	S32 cur_notch, real_notch, range;

	range = RAW_WHEEL_RANGE / mult;

	return (rng((gSWpos+range) /((2*range)/(nsections<<1)),0,(nsections<<1)-1)>>1);

} /* end RawWheelSection */

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Force steering wheel back to center position during resurrect
*         Upon entry: position -> steering wheel position to center to
*                     force_shift -> amount to modify speed in which to get to position
*                     vel_shift -> amount to modify velocity motion by
*                     max -> maximum force to apply                          */

void force_wheel_to_center (S32 position, S16 force_shift, S16 vel_shift, S16 max)
{
	S16 dampforce,centerforce;

	centerforce = -(gSWpos-position) >> force_shift;
	dampforce = -avg_sw_vel >> vel_shift;
	centerforce += dampforce;
	centerforce = rng (centerforce, -max, max);
	gSWforce = (S16)centerforce;
}
