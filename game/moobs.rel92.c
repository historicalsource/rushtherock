/*
 *  moobs.c: this file contains functions that format data for CAGE audio motion objects 
 *
 *	Copyright 1996 Time Warner Interactive
 *	Unauthorized reproduction, adaptation, distribution, performance or 
 *	display of this computer program or the associated audiovisual work
 *	is strictly prohibited.
 *
 *	======================================================================
 *	$Author: gray $						$Date: 1997/06/13 18:44:10 $
 *	$Revision: 3.28 $						$Locker:  $
 *	======================================================================
 *	Change Log:
 *
 *	$Log: moobs.c,v $
 * Revision 3.28  1997/06/13  18:44:10  gray
 * Audio mods to be compatible with old and new sounds.
 *
 * Revision 3.27  1997/05/30  04:05:35  gray
 * mainout is history!
 *
 * Revision 3.26  1997/05/29  21:08:03  gray
 * Clean up mph stuff.
 *
 * Revision 3.25  1997/05/21  06:04:56  gray
 * Killed xv,yv,zv in mainout (again).
 *
 * Revision 3.24  1997/05/21  00:10:16  gray
 * caruvs finally exterminated.
 *
 * Revision 3.23  1997/04/30  00:52:47  gray
 * Made drone version of velocity and vel_angle.
 *
 * Revision 3.22  1997/01/22  02:11:27  geraci
 * fixed buffer too small bug
 *
 * Revision 3.21  1996/12/05  01:50:24  geraci
 * limits miscellaneous moobs to 2
 *
 * Revision 3.20  1996/12/01  23:55:30  geraci
 * patch fix for locked sounds at end of race due to cage board overload
 *
 * Revision 3.19  1996/11/25  11:37:40  geraci
 * old engine is gone
 *
 * Revision 3.18  1996/11/23  02:59:14  geraci
 * allows skid in attract without sound
 *
 * Revision 3.17  1996/11/19  23:12:46  geraci
 * camera gets moobed during demo, moobs shut off at end of race
 *
 * Revision 3.16  1996/11/14  12:44:13  geraci
 * starts sound regulation at a slow pace.
 *
 * Revision 3.15  1996/11/11  22:20:34  geraci
 * down to 2 other engine moobs, waits 7 secs before any misc moobs
 *
 * Revision 3.14  1996/11/03  06:08:57  geraci
 * split off debug switches, displayed scut codes, bumped up sound updates
 *
 * Revision 3.13  1996/10/25  11:08:39  geraci
 * call init reverb to initialize reverb
 *
 * Revision 3.12  1996/10/23  00:14:33  petrick
 * Made track_no a global.
 *
 * Revision 3.11  1996/10/17  08:52:52  geraci
 * got rid of some static sound effects
 *
 * Revision 3.10  1996/10/16  07:55:56  geraci
 * handles miscellaneous moob objects that are defined as targets
 *
 * Revision 3.9  1996/10/05  09:18:45  geraci
 * reverb volume adjust moved to carsnd
 *
 * Revision 3.8  1996/10/04  07:36:53  geraci
 * left and right reverb can have volume due to distance
 *
 * Revision 3.7  1996/10/03  05:16:57  geraci
 * added some reverb effects
 *
 * Revision 3.6  1996/09/27  00:03:12  gray
 * Killed Spencer (hahahaaaaaaaa).
 *
 * Revision 3.5  1996/09/19  05:57:22  geraci
 * static chatter happens at certain spots in each track
 *
 * Revision 3.4  1996/09/17  08:44:42  geraci
 * don't hear other cars during countdown
 *
 * Revision 3.3  1996/09/16  02:13:55  geraci
 * fixed drone engine cut-out
 *
 * Revision 3.2  1996/09/13  23:04:22  geraci
 * new bump update state for sounds
 *
 * Revision 3.1  1996/09/09  02:24:52  gray
 * JAMMA version
 *
 * Revision 2.19  1996/09/05  21:07:31  geraci
 * no more choppy drone sounds
 *
 * Revision 2.18  1996/09/05  00:09:33  gray
 * Backed out xyzv mod, evil audio bug.
 *
 * Revision 2.17  1996/09/04  23:32:27  gray
 * Removed xv, yv, zv from mainout.
 *
 * Revision 2.16  1996/09/04  20:37:22  gray
 * Adding dead reckon data to game_car. Killing mainout.
 *
 * Revision 2.15  1996/08/18  03:30:29  gray
 * Removing ZOID stuff.
 *
 * Revision 2.14  1996/08/07  20:29:47  geraci
 * kills drone moobs first then updates new ones. Also no drones during resurrect.
 *
 * Revision 2.13  1996/08/02  19:01:14  geraci
 * don't hear drones that are dying and don't listen to them if you died
 *
 * Revision 2.12  1996/07/31  12:35:41  geraci
 * localized more sounds, killed moob engines no longer in range
 *
 * Revision 2.11  1996/07/27  08:18:44  geraci
 * 7 more bits of precision sent for listener and drone positions
 *
 * Revision 2.10  1996/07/25  09:19:12  geraci
 * turns off drones that are no longer in range
 *
 * Revision 2.9  1996/07/24  02:25:33  geraci
 * debug stuff
 *
 * Revision 2.7  1996/07/19  09:59:19  geraci
 * changed drone and listener update priority and order
 *
 * Revision 2.6  1996/05/29  23:06:11  geraci
 * cleaned some stuff up, added three drones to reverb switch
 *
 * Revision 2.5  1996/05/16  20:46:07  geraci
 * added an if 0
 *
 * Revision 2.4  1996/05/14  12:09:44  geraci
 * down to one drone for sound
 *
 * Revision 2.3  1996/05/11  01:18:36  geraci
 * removed time stamp display stuff from here
 *
 * Revision 2.2  1996/05/08  19:50:22  geraci
 * added way to re-initialize time-stamp data
 *
 * Revision 2.1  1996/05/04  02:56:18  petrick
 * Focus 2 release
 *
 * Revision 1.38  1996/04/30  22:55:50  geraci
 * moved tunnel test to better spot
 *
 * Revision 1.37  1996/04/30  01:26:16  geraci
 * added tunnel test code
 *
 * Revision 1.36  1996/04/27  02:18:44  geraci
 * don't update car relative sounds during abort/resurrect
 *
 * Revision 1.35  1996/04/20  07:14:27  geraci
 * more accurate steering wheel position display
 *
 * Revision 1.34  1996/04/17  20:23:56  geraci
 * new steering wheel debug display
 *
 * Revision 1.33  1996/04/12  00:22:34  geraci
 * use engtorque for load not dummy
 *
 * Revision 1.32  1996/03/30  08:14:53  petrick
 * Prelocated dynamic objects added.
 *
 * Revision 1.31  1996/03/30  02:02:57  geraci
 * changed range of trolley cars
 *
 * Revision 1.30  1996/03/26  07:04:21  geraci
 * general purpose sound effects based on track and position
 *
 * Revision 1.29  1996/03/23  02:22:04  geraci
 * wheel test for pete
 *
 * Revision 1.28  1996/03/21  03:45:53  geraci
 * added miscellaneous sound update
 *
 * Revision 1.27  1996/03/20  03:52:26  geraci
 * sw_debug3 can be used to enable debug data to be displayed
 *
 * Revision 1.26  1996/03/19  01:10:52  geraci
 * allows only two closest drones to be heard
 *
 * Revision 1.25  1996/03/12  11:04:40  geraci
 * bug fix
 *
 * Revision 1.24  1996/03/12  10:33:20  geraci
 * fixed divide by zero
 *
 * Revision 1.23  1996/03/12  10:01:24  geraci
 * disabled sw_debug3 stuff
 *
 * Revision 1.22  1996/03/09  08:11:04  geraci
 * put debug stuff on a switch
 *
 * Revision 1.21  1996/03/08  07:48:39  geraci
 * new checkpoint moob
 *
 * Revision 1.20  1996/03/07  23:50:17  geraci
 * other drone engines are heard using model data and not carz
 *
 * Revision 1.19  1996/03/07  10:42:37  geraci
 * drone jitter
 *
 * Revision 1.18  1996/03/01  10:51:36  geraci
 * sends out correct moobs engine sound for each player
 *
 * Revision 1.17  1996/03/01  02:40:03  geraci
 * player_model and drone_model are now model
 *
 * Revision 1.16  1996/02/28  21:35:31  geraci
 * moved code to change engine model to updatelistener (model task)
 *
 * Revision 1.15  1996/02/22  03:43:22  geraci
 * moved stuff to moobsdefs.h
 *
 * Revision 1.14  1996/02/14  02:30:26  geraci
 * sounds are now being dispatched using a feedback mechanism that
 * regulates the rate in which sounds are sent to the CAGE board.
 *
 * Revision 1.13  1996/02/02  05:29:56  geraci
 * added some time slicing
 *
 * Revision 1.12  1996/01/25  02:18:20  geraci
 * band-aid fix for spurious garbage sounds
 *
 * Revision 1.11  1996/01/23  01:19:04  geraci
 * added some temp. debug stuff
 *
 * Revision 1.10  1996/01/20  05:23:41  petrick
 * Got rid of some includes that were no longer needed.
 *
 * Revision 1.9  1996/01/19  20:47:43  gray
 * remove souncall.h (for jrg).
 *
 * Revision 1.8  1996/01/11  23:43:57  geraci
 * replaced ints with S32s...
 *
 * Revision 1.7  1996/01/11  21:04:24  geraci
 * sound effects are called from a common routine
 * unique sound handles are assigned to all moobs needing them
 * other player's engines can now be heard
 *
 *
 *	======================================================================
 */

#include "model.h"
#include "modeldat.h"
#include "carsnd.h"
#include "checkpoint.h"
#include "dis.h"
#include "debug.h"
#include "globals.h"
#include "mb.h"
#include "moobdefs.h"
#include "os_proto.h"
#include "select.h"
#include "sounds.h"

#include "Pro/math.pro"
#include "Pro/sounds.pro"

#define ROAD_SOUNDS     1
#define BUMP_SOUNDS		1
#define ENGINE_SOUND    1
#define MISC_SOUNDS     1
#define DRONE_SOUNDS    1
#define LISTENER        1

/*---------------------------------------------------------------------------*/

extern	int aud_qlen(void);
extern	S16 which_engine;
extern	S16	display_pos_data, clear_pos_data, disp_sw_bar;
extern  F32 mcam[3];

/*---------------------------------------------------------------------------*/

U32     player_update_timer;    /* Don't update listener too often! Every 100 ms seems to work */ 

U8      global_handle;          /* Give out handles to MOOBS by incrementing this, */
                                /* if MOOBS turn on and off alot, may need to do   */
                                /* something more complex (keep a tally of which   */
                                /* handles are being used) */

S8      drone_handle[MAX_LINKS];
S8		drone_active[MAX_LINKS]; /* 0=inactive, 1=currently active, 2=previously active */

S8		misc_handles[20];		/* miscellaneous moob handles */

U16		sound_time;				/* counter used for time slicing sounds */
U16		sound_adjust;			/* sound slice time adjustment factor */
U32		buffer_average;			/* rolling average of buffer state */
U16		num_samples;
S16		num_drone_moobs, cur_misc_moobs;

/* sound types: */
typedef enum
{ 
	ENGINESOUND,
	ROADSOUNDS,
	BUMPSOUNDS,
	LISTENUPDATE,
	OTHERCARS,
	MISCSOUNDS,
	NUM_SOUND_TYPES
} Sound_types;

SOUND_T sound_table[NUM_SOUND_TYPES];

/* for each sound type defined there must be a table entry here */
const SOUND_T init_sound_table[NUM_SOUND_TYPES] =
{   /* sound num, 0, update percentage */
	{ ENGINESOUND,  0, 67 },
	{ ROADSOUNDS,   0, 23 },
	{ BUMPSOUNDS,	0, 66 },
	{ LISTENUPDATE, 0, 55 },
	{ OTHERCARS,    0, 55 },
	{ MISCSOUNDS,	0,  2 }
};

const MISC_SOUND misc_sound_tab[] =
{
	{S_SEAGULLS,   S_KSEAGULLS, 0, {-1623, 152,  4080}, 1000*1000},
	{S_PARKBIRD,   S_KPARKBIRD, 0, {-2003,   0, -1796}, 550*550},
	{S_SEAGULLS,   S_KSEAGULLS, 1, {-3493, 145,   321}, 493*493},
	{S_CROWDSCARE, S_KPARKBIRD, 1, {-4130,  65,   895}, 140*140},
	{S_FOGHORN,    S_KFOGHORN,  1, {-4960,   4,  2700}, 920*920},
};

MISC_SND  misc_sound[20]; /* allows up to 10 miscellaneous sounds */
S16       num_misc_sounds;

#define    MAX_MISC_MOOBS   20
MISC_MOOBS misc_moobs[MAX_MISC_MOOBS];
S16        num_misc_moobs;

/******************************************************************************
*
*       Initialize stuff for sounds (once during init)                       */

void InitMoobs(void)
{
	global_handle = 0;
}

#define REL92_TEST 1
#if REL92_TEST
U32 blurb_time;
#endif

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Initialize stuff for sounds (during pre-play)                        */

void init_moobs (void)
{
	S16  i;

	player_update_timer = IRQTIME;

	for (i=0; i<MAX_LINKS; i++)
		drone_active[i] = 0;

	num_misc_moobs = 0;

	if (demo_game)
		num_drone_moobs = 4;
	else
		num_drone_moobs = 2;

	cur_misc_moobs = 0;

#if REL92_TEST
	blurb_time = IRQTIME + 3000;
#endif
}

/******************************************************************************
*
*       Update sound once per model loop
*         Upon entry: update_car_sounds -> true = update road and tire squeals
*                                          false = don't do car sounds
*                     skids_only -> true = update tire skids and smoke only
*                                   false = update everything                */

void UpdateListener (S8 update_car_sounds, S8 skids_only)
{
	S16			yaw, velocity, vel_angle, dvelocity, dvel_angle, num_dists;
	S16			i, j, k, drone_index, xvel, zvel, dist_index[MAX_LINKS];
	S32			xpos, ypos, zpos, dxpos, dypos, dzpos;
	F32			dist[MAX_LINKS], distance;
	S16			xx,yy;
	U16			snd_flags, cmd_tab[2], temp_vol;
	MODELDAT	*m = &model[this_node];
	CAR_DATA	*gc = &game_car[this_node];

	check_forces_on_car (m);

	if ((IRQTIME - player_update_timer) < 10)
		return;

	player_update_timer = IRQTIME;

	if (demo_game)
		{
		xpos = mcam[0] * 128;
		ypos = mcam[1] * 128;
		zpos = mcam[2] * 128;
		}
	else
		{
		/* x,z position is in meters */
		xpos =  m->reckon.RWR[YCOMP] * 128;  /* scale to meters */
		ypos = -m->reckon.RWR[ZCOMP] * 128;  /* scale to meters */
		zpos =  m->reckon.RWR[XCOMP] * 128;  /* scale to meters */    
		}

	/* yaw, vel_angle in 512 units/360 degrees, going clockwise */
	xx = m->reckon.UV[0][0] * 16384.0;
	yy = m->reckon.UV[1][0] * 16384.0;
	yaw = ((satan(xx, yy)) >> 7) & MAX_MOOB_ANGLE;

	xx = m->reckon.RWV[XCOMP] * 2.56;
	yy = m->reckon.RWV[YCOMP] * 2.56;
	vel_angle = ((satan(xx, yy)) >> 7) & MAX_MOOB_ANGLE;


	/* velocity in mm/frame      */
	/* This SHOULD come out to mm/16.67 ms */

	/* Conversion from mph to mm/s is times 4.470400e+02 (as given by "units" utility) */
	velocity = (S16)m->mph;

	if (velocity < 0)
		velocity = -velocity;

	check_sound_time (Do_it);

#if BUMP_SOUNDS
	/* engine sounds */
	if (sound_table[BUMPSOUNDS].sound_index && skids_only==false)
		do_bump_sounds (update_car_sounds);
#endif

#if ROAD_SOUNDS
	/* road, wind etc. */
	if (sound_table[ROADSOUNDS].sound_index)
		DoCarSounds (update_car_sounds, skids_only);
#endif

#if ENGINE_SOUND
	/* engine sounds */
	if (sound_table[ENGINESOUND].sound_index && skids_only==false)
		{
		DoIntCarSounds();
		if (m->resurrect.moving_state == -1)
			handle_reverb();
		else if (in_tunnel)
			init_reverb();
		}
#endif

#if MISC_SOUNDS
	if (sound_table[MISCSOUNDS].sound_index && skids_only==false && GetElapsedTime() > 7 * ONE_SEC)
		do_misc_sounds (xpos, ypos, zpos);
#endif

	if (display_pos_data)
		{
		S16          i;
		static F32   percent, max_p;
		char         buf[140];
		extern  U32  avg_sw_force;
		extern  S16  last_swforce;

		sprintf (buf, "ppos (%ld %ld %ld)   ", xpos>>7, ypos>>7, zpos>>7);
		txt_str (28, 9, buf, WHT_PALB);

		percent = ((F32)aud_qlen()) / (F32)0xffff;
		if (percent > max_p) max_p = percent;
		sprintf (buf, "%1.4f %1.4f %ld %d ", (double)percent, (double)max_p, buffer_average, sound_adjust);
		txt_str (28, 10, buf, WHT_PALB);

		sprintf (buf, "sw %lx %4.4x %4.4x ", avg_sw_force>>14, gc->sound_flags, gc->shortcut_flags);
		txt_str (28, 11, buf, WHT_PALB);

		if (disp_sw_bar)
			{
			txt_str (2, 42, "70  60  50  40  30  20  10   0 -10 -20 -30 -40 -50 -60 -70", WHT_PALB);
			sprintf (buf, "%64.64sT%64.64s", "", "");
			i = ((last_swforce+128)>>2) + 64;
			buf[i] = 0;
			txt_str (0, 43, &(buf[i-63]), WHT_PALB);
			}

#if 0
		for (i=0; i<MAX_LINKS; i++)
			{
			if (model[i].in_game)
				{
#if 0
				sprintf (buf, "ppos %d (%g %g %g)   ", i, (double)game_car[i].pos[0], (double)game_car[i].pos[1], (double)game_car[i].pos[2]);
#endif
				sprintf (buf, "%g %d  ", (double)model[i].dt, model[i].idt);
				txt_str (28, 13+i, buf, WHT_PALB);
				}
			}
#endif
		}

	if (clear_pos_data)
		{
		char  buf[80];

		clear_pos_data = false;

		sprintf (buf, "%30.30s", "");
		for (i=9; i<13; i++)
			txt_str (28, i, buf, WHT_PAL);

		sprintf (buf, "%64.64s", "");
		txt_str (1, 42, buf, WHT_PAL);
		txt_str (1, 43, buf, WHT_PAL);
		}

#if 0
	if (sound_table[OTHERCARS].sound_index)
		{
		if (SW_DEBUG12 & m_dlevels)
			sndUpdateMoobEngine( 2,
								 drone_handle[0],
								 DRONE_PRIORITY,
								 xpos, zpos+(10<<7),
								 0, 0,
								 4000,
								 100
								 );

		if (SW_DEBUG13 & m_dlevels)
			sndUpdateMoobEngine( 2,
								 drone_handle[1],
								 DRONE_PRIORITY,
								 xpos-(10<<7), zpos,
								 0, 0,
								 7000,
								 100
								 );

		if (SW_DEBUG14 & m_dlevels)
			sndUpdateMoobEngine( 2,
								 drone_handle[2],
								 DRONE_PRIORITY,
								 xpos-(20<<7), zpos+(10<<7),
								 0, 0,
								 5000,
								 100
								 );

		sndListenerUpdate (xpos, zpos, velocity, vel_angle, yaw);
		}
#endif

#if DRONE_SOUNDS
	/* update other cars on track */
	if (sound_table[OTHERCARS].sound_index && gstate!=COUNTDOWN && skids_only==false)
		{
		if (update_car_sounds == false)
			{
			/* just kill them all */
			for (j=0; j<MAX_LINKS; j++)
				{
				if (drone_active[j] == 2)
					{
					sndKillMoob (drone_handle[j]);
					drone_active[j] = 0;
					}
				}
			}
		else
			{
			num_dists = 0;

			for (i=0; i<MAX_LINKS; i++)
				{
				if ((  model[i].in_game)
					&& (i != this_node || demo_game == true)
					&& (game_car[i].we_died == false)
					&& (model[i].crashflag == false)
					&& (model[i].resurrect.moving_state == -1))
					{
					dxpos = abs ((model[i].reckon.RWR[YCOMP]*128) - xpos);
					dzpos = abs ((model[i].reckon.RWR[XCOMP]*128) - zpos);

					if ((dxpos < (1000<<7)) && (dzpos < (1000<<7)))
						{
						distance = (F32)dxpos*dxpos + (F32)dzpos*dzpos;

						for (j=0; j<num_dists; j++)
							{
							if (distance < dist[j])
								break;
							}

						if (j < num_drone_moobs)
							{
							for (k=num_dists; k>j; k--)
								{
								dist[k] = dist[k-1];
								dist_index[k] = dist_index[k-1];
								}

							dist[j] = distance;
							dist_index[j] = i;
							if (num_dists < num_drone_moobs)
								num_dists++;
							}
						}
					}
				}

			for (j=0; j<num_dists; j++)
				drone_active[dist_index[j]] = 1;

			for (i=0; i<num_active_cars; i++)
				{
				j = model[i].net_node;

				if (drone_active[j] == 2)
					{
					sndKillMoob (drone_handle[j]);
					drone_active[j] = 0;
					}
				}

			for (j=0; j<num_dists; j++)
				{
				i = dist_index[j];

				dxpos = model[i].reckon.RWR[YCOMP] * 128;
				dzpos = model[i].reckon.RWR[XCOMP] * 128;

				xvel = (S16)(model[i].reckon.RWV[YCOMP] * 0.64);
				zvel = (S16)(model[i].reckon.RWV[XCOMP] * 0.64);

				dvel_angle = ( satan (model[i].reckon.RWV[YCOMP]*2.56, model[i].reckon.RWV[XCOMP]*2.56) >> 7) & MAX_MOOB_ANGLE;

				dvelocity = lsqrt (xvel*xvel + zvel*zvel);

				sndUpdateMoobEngine( model[i].engine_type,
									 drone_handle[i],
									 DRONE_PRIORITY,
									 dxpos, dzpos,
									 dvelocity, dvel_angle,
									 model[i].rpm,
									 model[i].engtorque
									 );

				drone_active[i] = 2;
				}
			}
		}
#endif

#if LISTENER
	/* update listener position */
	if (sound_table[LISTENUPDATE].sound_index && skids_only==false)
		{
		sndListenerUpdate (xpos, zpos, velocity, vel_angle, yaw);
		}
#endif

	check_sound_time (Allocate);
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       See if it's okay to check skid for other drones                      */

BOOL okay_to_check_skid (void)
{
#if ROAD_SOUNDS
	return (sound_table[ROADSOUNDS].sound_index);
#else
	return (false);
#endif
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Add moob object to moob list                                         */

void add_moob_to_list (S16 moob_sound, F32 *pos, S32 radius)
{
	if (num_misc_moobs+1 >= MAX_MISC_MOOBS)
		{
		char buf[50];

		sprintf (buf, "Maximum number of %d moobs reached", MAX_MISC_MOOBS);
		txt_str (20, 20, buf, RED_PALB);
		}
	else
		{
		misc_moobs[num_misc_moobs].moob_sound = moob_sound;
		misc_moobs[num_misc_moobs].pos[0] = pos[0];
		misc_moobs[num_misc_moobs].pos[1] = pos[1];
		misc_moobs[num_misc_moobs].pos[2] = pos[2];
		misc_moobs[num_misc_moobs++].radius_sq = radius*radius;
		misc_moobs[num_misc_moobs].moob_state = false;

#if 0
			{
			char buf[50];

			sprintf (buf, "%4.4x [%ld %ld %ld] %ld ",
					 misc_moobs[num_misc_moobs-1].moob_sound,
					 misc_moobs[num_misc_moobs-1].pos[0],
					 misc_moobs[num_misc_moobs-1].pos[1],
					 misc_moobs[num_misc_moobs-1].pos[2],
					 misc_moobs[num_misc_moobs-1].radius_sq);
			txt_str (10, 20+num_misc_moobs, buf, WHT_PALB);
			}
#endif
		}
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Activate/deactivate miscellaneous sounds                             */

void do_misc_sounds (S32 xpos, S32 ypos, S32 zpos)
{
	S16		i, j;
	S32		dxpos, dypos, dzpos;
	F32		distance;

#if REL92_TEST
	static S32 blurbnum;

	if(IRQTIME > blurb_time)
		{
		blurbnum++;
		if(blurbnum > 8)
			blurbnum = 0;
		switch(blurbnum)
			{
			case 0:	SOUND (S_YELL);	break;
			case 1:	SOUND (S_ITSFAT);	break;
			case 2:	SOUND (S_CHIPS);	break;
			case 3:	SOUND (S_YOUGOTTA);	break;
			case 4:	SOUND (S_ITSINSANE);	break;
			case 5:	SOUND (S_LOOKOUT);	break;
			case 6:	SOUND (S_BADLICIOUS);	break;
			case 7:	SOUND (S_WELCOMENET);	break;
			case 8:	SOUND (S_LOGIN);	break;
			}
		blurb_time = IRQTIME + 3000;
		}
#else
	/* go through all static miscellaneous sounds */
	for (i=0; i<num_misc_sounds; i++)
		{
		j = misc_sound[i].index;

		dxpos = (xpos>>7) - misc_sound_tab[j].pos[0];
		dypos = (ypos>>7) - misc_sound_tab[j].pos[1];
		dzpos = (zpos>>7) - misc_sound_tab[j].pos[2];
		distance = dxpos*dxpos + dypos*dypos + dzpos*dzpos;

		if (distance < misc_sound_tab[j].radius_sq && lap_flag == false)
			{
			if (misc_sound[i].flag == false && cur_misc_moobs < MAX_ACTIVE_MOOBS)
				{
				misc_sound[i].flag = true;
				SOUND (misc_sound_tab[j].sound_id_on);
				cur_misc_moobs++;
				}
			}
		else if (misc_sound[i].flag == true)
			{
			misc_sound[i].flag = false;
			SOUND (misc_sound_tab[j].sound_id_off);
			cur_misc_moobs--;
			}
		}
#endif

	/* now do all moob miscellaneous sounds */
	for (i=0; i<num_misc_moobs; i++)
		{
		dxpos = (xpos>>7) - misc_moobs[i].pos[0];
		dypos = (ypos>>7) - misc_moobs[i].pos[1];
		dzpos = (zpos>>7) - misc_moobs[i].pos[2];
		distance = dxpos*dxpos + dypos*dypos + dzpos*dzpos;

		if (distance < misc_moobs[i].radius_sq && !(coast_flag || end_game_flag || game_car[this_node].place_locked==1 || lap_flag))
			{
			if (misc_moobs[i].moob_state == false && cur_misc_moobs < MAX_ACTIVE_MOOBS)
				{
				/* it's within radius and is currently off, so turn it on */
				misc_moobs[i].moob_state = true;

				sndStartStaticUnpitched (misc_moobs[i].moob_sound,
										 misc_handles[i],
										 HIGHEST_PRIORITY,
										 misc_moobs[i].pos[0],
										 misc_moobs[i].pos[2]);
				cur_misc_moobs++;
				}
			}
		else if (misc_moobs[i].moob_state == true)
			{
			/* it's out of radius and is currently on, so turn it off */
			misc_moobs[i].moob_state = false;
			sndKillMoob (misc_handles[i]);
			cur_misc_moobs--;
			}
		}
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*		Update/Check time slice state
*		  Upon entry: mode       -> Initialize = initialize time slice tables
*									Allocate   = update sound_adjust factor
*                                   Doit       = update 'play sound' flag    */

void check_sound_time (S16 mode)
{
	S16	i, j, cmd_tab[2];
	U32	buffer_state;
	U32 div_factor;

	switch (mode)
		{
		case Initialize:
			buffer_average = 0;
			num_samples = 0;

			sound_time = 0;

			num_misc_sounds = 0;
			for (i=0; i<sizeof(misc_sound_tab)/sizeof(MISC_SOUND); i++)
				{
				if (misc_sound_tab[i].track_num == trackno)
					{
					misc_sound[num_misc_sounds].index = i;
					misc_sound[num_misc_sounds++].flag = false;
					}

				}

			for (i=0; i<NUM_SOUND_TYPES; i++)
				{
				/* get sound index (init_s... uses i) (sound_t... use j) */
				j = init_sound_table[i].sound_index;

				/* used as a flag to indicate whether or not sound should update */
				sound_table[j].sound_index = 0;

				/* initially use percentages at 100% */
				sound_adjust = 0x40;

				/* current slice number; 15% = 7 sound updates per cycle */
				div_factor = (init_sound_table[i].cur_t_inc * sound_adjust) / 100;
				if (div_factor == 0)
					div_factor = 1;

				sound_table[j].cur_t_inc = 0x10000 / div_factor;
				sound_table[j].next_update = sound_table[j].cur_t_inc;
				}

			break;


		case Allocate:
			/* adjust running average */
			if (num_samples >= 16)
				buffer_average -= (buffer_average>>4);
			else
				num_samples++;

			/* get sound buffer's fullness state (0x8000 = 1/2 full, 0x4000 = 1/4 full */
			buffer_average += (U32)aud_qlen();

			if (num_samples < 16)
				break;

			if (buffer_average > 0x30000)
				/* time to decrease frequency */
				{
				/* lose 12.5% */
				sound_adjust -= (sound_adjust >> 4);

				/* don't go less than 8: (8>>3=1, 7>>3=0) */
				if (sound_adjust < 8)
					sound_adjust = 8;

				for (i=0; i<NUM_SOUND_TYPES; i++)
					{
					/* get sound index (init_s... uses i) (sound_t... use j) */
					j = init_sound_table[i].sound_index;

					/* current slice number; 15% = 7 sound updates per cycle */
					div_factor = (init_sound_table[i].cur_t_inc * sound_adjust) / 100;
					if (div_factor == 0)
						div_factor = 1;

					sound_table[j].cur_t_inc = 0x10000 / div_factor;
					}
				}

			else if (buffer_average < 0x10000)
				/* time to increase frequency */
				{
				/* increase by 12.5% */
				sound_adjust++;

				/* cap at 800 percent of default */
				if (sound_adjust > 0x800)
					sound_adjust = 0x800;

				for (i=0; i<NUM_SOUND_TYPES; i++)
					{
					/* get sound index (init_s... uses i) (sound_t... use j) */
					j = init_sound_table[i].sound_index;

					/* current slice number; 15% = 7 sound updates per cycle */
					div_factor = (init_sound_table[i].cur_t_inc * sound_adjust) / 100;
					if (div_factor > 0x100)
						div_factor = 0x100;
					if (div_factor < 2)
						div_factor = 2;

					sound_table[j].cur_t_inc = 0x10000 / div_factor;
					}
				}
			break;


		case Do_it:
			/* bump time counter */
			sound_time += 0x100;

			if (sound_time == 0)
				/* reset all next update times */
				for (i=0; i<NUM_SOUND_TYPES; i++)
					sound_table[i].next_update = sound_table[i].cur_t_inc;

			/* go through all times and determine which sound should update */
			for (i=0; i<NUM_SOUND_TYPES; i++)
				{
				if (sound_table[i].next_update < (U32)sound_time)
					/* time to update */
					{
					sound_table[i].sound_index = 1;
					sound_table[i].next_update += sound_table[i].cur_t_inc;
					}
				else
					sound_table[i].sound_index = 0;
				}

			break;
		}
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*        Get next global handle for sound routines
*        Upon exit: returned -> next global handle                           */

U8
get_next_handle()
{
  return (global_handle++);
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Initialize handles for drones and miscellaneous moob objects         */

void
init_drone_handles()
{
  S16   i;

  for (i=0; i<MAX_LINKS; i++)
	  drone_handle[i] = get_next_handle();

  for (i=0; i<20; i++)
	  misc_handles[i] = get_next_handle();
}
