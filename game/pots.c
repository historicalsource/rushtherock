/*
 *	pots.c -- these functions read 8-bit and 12-bit pots for an MCUBE host
 *
 *	Copyright 1996 Time Warner Interactive
 *	Unauthorized reproduction, adaptation, distribution, performance or 
 *	display of this computer program or the associated audiovisual work
 *	is strictly prohibited.
 *
 *	======================================================================
 *	$Author: gray $						$Date: 1997/09/05 22:39:49 $
 *	$Revision: 3.16 $						$Locker:  $
 *	======================================================================
 *	Change Log:
 *
 *	$Log: pots.c,v $
 * Revision 3.16  1997/09/05  22:39:49  gray
 * Removed bit juggling from debug switch reading.
 *
 * Revision 3.15  1997/06/04  02:44:44  gray
 * Zapped ithrottle.
 *
 * Revision 3.14  1997/06/04  02:15:46  gray
 * Restructuring controls (gas,brake,clutch,etc) handling.
 *
 * Revision 3.13  1997/05/30  19:02:58  gray
 * Got rid of FLAGSTAFF conditionals.
 *
 * Revision 3.12  1996/12/02  04:54:01  gray
 * Debug switched always zero for production version.
 *
 * Revision 3.11  1996/11/21  13:51:44  petrick
 * has_shifter now a game option.
 *
 * Revision 3.10  1996/11/15  12:57:47  geraci
 * new switch handling allows sw 12 13 and 14
 *
 * Revision 3.9  1996/11/14  10:00:51  geraci
 * fixed pots startup bug
 *
 * Revision 3.8  1996/11/11  22:09:02  geraci
 * removed steering wheel end point search
 *
 * Revision 3.7  1996/11/11  09:19:45  geraci
 * new way to deal with pots
 *
 * Revision 3.6  1996/11/09  12:28:30  geraci
 * fixed auto-calibration/GUTS problem
 *
 * Revision 3.5  1996/11/08  07:03:41  geraci
 * pot aging and recalibration
 *
 * Revision 3.4  1996/11/03  06:08:41  geraci
 * split up debug switches and regular game switches
 *
 * Revision 3.3  1996/10/10  05:12:22  petrick
 * Made irqtime a true global in globals_h.
 *
 * Revision 3.2  1996/10/08  05:20:58  gray
 * Flagstaff port.
 *
 * Revision 3.1  1996/09/09  02:24:52  gray
 * JAMMA version
 *
 *	======================================================================
 */

#include "config.h"
#include "controls.h"
#include "eer_defs.h"
#include "os_proto.h"
#include "modeldat.h"
#include "game.h"
#include "globals.h"
#include "model.h"
#include "debug.h"
#include "dis.h"

#include "Pro/pots.pro"
#include "Pro/fswheel.pro"


/*===============================  DEFINES  =================================*/

#define PIN(lo,m,hi)	(((m) < (lo)) ? (lo) : ((m) > (hi)) ? (hi) : (m))

#define SHIFT_VAL     3

typedef struct POT_DEFS
{
	S16  guts_pot_name;
	S16  our_pot_name;
	S16  *min;
	S16  *max;
	S16  *range;
	S16  eer_pot_lo;
	S16  eer_pot_hi;

} POT_DEFS;

typedef struct POT_TAB
{
	S16  pot_lo;
	S16  pot_hi;
	S32  avg_lo;
	S32  avg_hi;
} POT_TAB;

/*===============================  GLOBALS  =================================*/

static S16 	ACCEL_MIN,  ACCEL_MAX,  ACCEL_RNG;
static S16 	BRAKE_MIN,  BRAKE_MAX,  BRAKE_RNG;
static S16 	CLUTCH_MIN, CLUTCH_MAX, CLUTCH_RNG;
S16			WHEEL_MIN,	WHEEL_MAX,	WHEEL_RNG;

U8			man_gear;
U32     	last_levels, last_dlevels;
U8			ad8val[8];
S32			check_limit_time, init_pots_flag;

POT_TAB		pot_tab[NUM_POTS];

const POT_DEFS pot_def[NUM_POTS] = {
	{ POT_BRAKE,  ADC_BRAKE,  &BRAKE_MIN,  &BRAKE_MAX,  &BRAKE_RNG, EER_POT0L, EER_POT0H },
	{ POT_GAS,    ADC_ACCEL,  &ACCEL_MIN,  &ACCEL_MAX,  &ACCEL_RNG, EER_POT1L, EER_POT1H },
	{ POT_CLUTCH, ADC_CLUTCH, &CLUTCH_MIN, &CLUTCH_MAX, &CLUTCH_RNG,EER_POT2L, EER_POT2H },
	{ POT_WHEEL,  ADC_WHEEL,  &WHEEL_MIN,  &WHEEL_MAX,  &WHEEL_RNG, EER_POT3L, EER_POT3H } };


/* ----------------------------------------------------------------------------- */
void PotsInit(void)
{

	InitSW();
	init_pots_flag = 0xfeedface;
}	/* end PotsInit */


/* ----------------------------------------------------------------------------- */

void PotsRead(void) /* called from Level 2 Interrupt handler (every 1ms or 4ms) */
{
	S16  i, min, max;
	S32	t;

	/* get raw pot data */
	for (i=0; i<NUM_POTS; i++)
		ad8val[pot_def[i].our_pot_name] = pot_raw (pot_def[i].guts_pot_name);

	if (init_pots_flag == 0xfeedface)
		{
		check_limit_time = IRQTIME + ONE_SEC;
		init_pots_flag = 0;
		}

	if (init_pots_flag == 0)
		{
		if (IRQTIME - check_limit_time < 0)
			initPots();
		else
			init_pots_flag = 1;
		}

	/* every 200 millisecs check raw against limits and save if different */
	if (IRQTIME - check_limit_time > ONE_SEC / 8)
		{
		/* do first three pots */
		for (i=0; i<NUM_POTS-1; i++)
			{
			/* munge/update lower range */
			if (ad8val[pot_def[i].our_pot_name] > pot_tab[i].pot_lo + 0x20)
				min = pot_tab[i].pot_lo;
			else
				min = ad8val[pot_def[i].our_pot_name];

			pot_tab[i].avg_lo -= pot_tab[i].avg_lo >> SHIFT_VAL;
			pot_tab[i].avg_lo += min;

			*pot_def[i].min = pot_tab[i].avg_lo>>SHIFT_VAL;

			/* now munge/update upper range */
			if (ad8val[pot_def[i].our_pot_name] < pot_tab[i].pot_hi - 0x20)
				max = pot_tab[i].pot_hi;
			else
				max = ad8val[pot_def[i].our_pot_name];

			pot_tab[i].avg_hi -= pot_tab[i].avg_hi >> SHIFT_VAL;
			pot_tab[i].avg_hi += max;

			*pot_def[i].max = pot_tab[i].avg_hi>>SHIFT_VAL;

			}

		sanity_check_pots();
		check_limit_time = IRQTIME;
		}

	/* now check raw pot reading against end points */
	for (i=0; i<NUM_POTS; i++)
		{
		if (ad8val[pot_def[i].our_pot_name] <= *pot_def[i].min+2)
			ad8val[pot_def[i].our_pot_name] = *pot_def[i].min;

		else if (ad8val[pot_def[i].our_pot_name] >= *pot_def[i].max-2)
			ad8val[pot_def[i].our_pot_name] = *pot_def[i].max;
		}

#if 1


	gGasRaw = ad8val[ADC_ACCEL];
	t = PIN(ACCEL_MIN,gGasRaw,ACCEL_MAX) - ACCEL_MIN;
	gGasInt = (t << 12) / ACCEL_RNG;
	gGasVal = (F32)gGasInt * (1.0/4096.0);

	gBrakeRaw = ad8val[ADC_BRAKE];
	t = PIN(BRAKE_MIN,gBrakeRaw,BRAKE_MAX) - BRAKE_MIN;
	gBrakeInt = (t << 12) / BRAKE_RNG;
	gBrakeVal = (F32)gBrakeInt * (1.0/4096.0);

	gClutchRaw = ad8val[ADC_CLUTCH];
	t = PIN(CLUTCH_MIN,gClutchRaw,CLUTCH_MAX) - CLUTCH_MIN;
	gClutchInt = (t << 12) / CLUTCH_RNG;
	gClutchVal = (F32)gClutchInt * (1.0/4096.0);

	gWheelRaw = ad8val[ADC_WHEEL];
#if 1
	/* keep half scale for now. (should be -0x1000 to 0x1000) */
	t = PIN(WHEEL_MIN,gWheelRaw,WHEEL_MAX) - WHEEL_MIN;
	gWheelInt = ((t << 12) / WHEEL_RNG) -0x800;
	gWheelVal = (F32)gWheelInt * (1.0/2048.0);
#else
	t = PIN(WHEEL_MIN,gWheelRaw,WHEEL_MAX) - WHEEL_MIN;
	gWheelInt = ((t << 12) / WHEEL_RNG) -0x1000;
	gWheelVal = (F32)gWheelInt * (1.0/4096.0);
#endif

#endif

}	/* end PotsRead */

/* ----------------------------------------------------------------------------- *
 *	Initializes the A/D converter ranges.
 * ----------------------------------------------------------------------------- */
void initPots (void)
{
	S16  i;

	for (i=0; i<NUM_POTS; i++)
		{
		*pot_def[i].max = eer_gets (pot_def[i].eer_pot_hi);
		*pot_def[i].min = eer_gets (pot_def[i].eer_pot_lo);
		pot_tab[i].pot_hi = *pot_def[i].max;
		pot_tab[i].pot_lo = *pot_def[i].min;
		pot_tab[i].avg_hi = pot_tab[i].pot_hi<<SHIFT_VAL;
		pot_tab[i].avg_lo = pot_tab[i].pot_lo<<SHIFT_VAL;
		}

	sanity_check_pots();
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Make sure pots are righty-tighty not lefty-loosey                    */

void sanity_check_pots (void)
{
	S16  i;

	for (i=0; i<NUM_POTS; i++)
		{
		if (*pot_def[i].min >= *pot_def[i].max)
			*pot_def[i].min = *pot_def[i].max - 1;

		*pot_def[i].range = *pot_def[i].max - *pot_def[i].min;
		}

}	/* end initPots */

/* ----------------------------------------------------------------------------- *
 *	Reads the current gear taking into account manual or auto transmission.
 * ----------------------------------------------------------------------------- */
void ReadClutch(void)
{
	S32	t = PIN(CLUTCH_MIN,ad8val[ADC_CLUTCH],CLUTCH_MAX) - CLUTCH_MIN;

	clutch = (t << 12) / CLUTCH_RNG; 
}	/* end ReadClutch */




/* ----------------------------------------------------------------------------- */
void ReadShifter(void)
{
    if (levels & SW_FIRST)
        man_gear = 1;
    else if (levels & SW_SECOND)
        man_gear = 2;
    else if (levels & SW_THIRD)
        man_gear = 3;
    else if (levels & SW_FOURTH)
        man_gear = 4;
    else 
        man_gear = 0; /* Must be in neutral */

}	/* end ReadShifter */


/* ----------------------------------------------------------------------------- *
 *	Reads the current gear taking into account manual or auto transmission.
 * ----------------------------------------------------------------------------- */
short ReadGear(short *gear)
{
	if (HAS_SHIFTER)
		{
		model[gThisNode].mainin.autotrans = autotrans;
		if (!autotrans) 
			{
			*gear = man_gear;
			ReadClutch();
			} 
		else 
			{
			*gear 	= 4; /* max gear for automatic (= Drive) */
			clutch 	= 0;
			}
		}
	else
		{
		*gear		= 4; /* max gear for automatic (= Drive) */
		clutch 		= 0;
		}

	if (levels & SW_REVERSE)
		*gear = -1;

	return clutch;
}	/* end ReadGear */


/* ----------------------------------------------------------------------------- *
 *	Reads the current Pot values for the gas and brake, then scales them to the
 *	range (0-0x1000), where 0 is none and 0x1000 is full on.  Note 0x1000 = 1.0
 * ----------------------------------------------------------------------------- */
void ReadGasAndBrake(void)
{
	short 		bshort;

	bshort 		= PIN(BRAKE_MIN,ad8val[ADC_BRAKE],BRAKE_MAX) - BRAKE_MIN;
	brake 		= ((long)bshort << 12) / BRAKE_RNG;	/* scale 0x1000 = 1.0 */
}	/* end ReadGasAndBrake */

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Read switches and save values to appropriate areas
*         Upon entry: mode -> Initialize = initialize last_levels
*                             Do_it = read switches
*         Upon exit: levels -> has bit mask of current switch state
*                    edges -> has bit mask of momentary rising edge
*                    trailing_edges -> has bit mask of momen. falling edge
*                    last_levels -> used to determine both edges             */

void get_switches (S16 mode)
{
	if (mode == Initialize)
		{
		last_levels = 0;
		last_dlevels = 0;
		return;
		}

	if (mode == Do_it)
		{
		/* Read player controls     */
		levels			= get_levels();
		edges			= get_edges();
		trailing_edges	= last_levels & ~levels;
		last_levels		= levels;

#if PRODUCTION_RELEASE
		/* Disable debug switchs for production. */
		dlevels = dedges = trailing_dedges = 0;
#else
		dlevels			= get_dlevels();
		dedges			= get_dedges();
		trailing_dedges	= last_dlevels & ~dlevels;
		last_dlevels	= dlevels;
#endif
		}
}	/* end get_switches */

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Get current state of switches
*         Upon exit: returned -> bit mask of current switch state            */

U32 get_levels()
{
	U32 tlevels;

	/* Read player controls     */
	tlevels	= ctl_read_sw(SW_LEVELS) & SW_EDGES;
	return (tlevels);
}	/* end get_levels */

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Get current state of debug switches
*         Upon exit: returned -> bit mask of current switch state            */

U32 get_dlevels()
{
	U32 tlevels;

	tlevels = ctl_read_debug(SW_LEVELS);
	return (tlevels);
}	/* end get_levels */

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Get current edge state of switches
*         Upon exit: returned -> bit mask of current edge switch state            */

U32 get_edges()
{
	U32 tedges;

	tedges = ctl_read_sw(SW_EDGES);
	return (tedges);
}	/* end get_edges */

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Get current edge state of debug switch edges
*         Upon exit: returned -> bit mask of current edge switch state            */

U32 get_dedges()
{
	U32 tedges, debug_switches, temp;

	tedges = ctl_read_debug(SW_EDGES);
	return (tedges);
}	/* end get_edges */

