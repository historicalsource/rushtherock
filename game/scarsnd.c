/*
 *  carsnd.c - this file includes code to send CAGE model parameters for the engine sound
 *
 *	Copyright 1996 Time Warner Interactive
 *	Unauthorized reproduction, adaptation, distribution, performance or 
 *	display of this computer program or the associated audiovisual work
 *	is strictly prohibited.
 *
 *	======================================================================
 *	$Author: gray $						$Date: 1997/09/05 01:03:34 $
 *	$Revision: 3.41 $						$Locker:  $
 *	======================================================================
 *	Change Log:
 *
 *	$Log: carsnd.c,v $
 * Revision 3.41  1997/09/05  01:03:34  gray
 * default torque scale for engine audio = 0.5.
 *
 * Revision 3.40  1997/09/04  18:24:47  gray
 * Tweak car bump volumes (per Gunner).
 *
 * Revision 3.39  1997/09/03  01:27:30  gray
 * Try higher rpm limit (0x1FFF).
 *
 * Revision 3.38  1997/08/22  01:32:05  gray
 * Playing with RPM and torque scale for engine sound.
 *
 * Revision 3.37  1997/05/30  04:05:35  gray
 * mainout is history!
 *
 * Revision 3.36  1997/05/30  01:25:14  gray
 * Clean up crashflag stuff.
 *
 * Revision 3.35  1997/05/29  21:08:03  gray
 * Clean up mph stuff.
 *
 * Revision 3.34  1997/05/13  20:47:29  gray
 * Coordinate utils name change.
 *
 * Revision 3.33  1997/05/13  18:50:50  gray
 * Use dead reckoned uvs.
 *
 * Revision 3.32  1997/05/11  03:37:10  gray
 * Cleaning model interface.
 *
 * Revision 3.31  1996/12/14  06:19:11  geraci
 * added sound to street light targets
 *
 * Revision 3.30  1996/12/12  09:42:24  geraci
 * targets hit in game task
 *
 * Revision 3.29  1996/12/06  17:26:58  geraci
 * made target collide stuff to use model coords
 *
 * Revision 3.28  1996/11/27  17:04:50  geraci
 * each car has its own in tunnel flag
 *
 * Revision 3.27  1996/11/26  23:41:59  geraci
 * put back old skid thresholds
 *
 * Revision 3.26  1996/11/25  11:36:52  geraci
 * 2nd tunnel texture supported
 *
 * Revision 3.25  1996/11/23  02:52:46  geraci
 * reverb happens on opposite side when turned around
 *
 * Revision 3.24  1996/11/19  23:16:26  geraci
 * own engine doesn't run in attract
 *
 * Revision 3.23  1996/11/14  12:51:32  geraci
 * change skid and smoke values
 *
 * Revision 3.22  1996/11/09  12:36:02  geraci
 * new reverb parm table
 *
 * Revision 3.21  1996/11/07  05:05:57  geraci
 * made bush sound work correctly
 *
 * Revision 3.20  1996/11/05  01:35:04  petrick
 * Changed cone to cone1 for consistancy, fixed vanished cone bug.
 *
 * Revision 3.19  1996/10/25  09:59:42  geraci
 * fixed water road sound, added init reverb routine
 *
 * Revision 3.18  1996/10/24  09:26:00  petrick
 * Added multiple trees and poles to sound list.
 *
 * Revision 3.17  1996/10/24  08:41:00  geraci
 * sound added to go with fence collision
 *
 * Revision 3.16  1996/10/22  08:51:56  geraci
 * new base sound for collisions
 *
 * Revision 3.15  1996/10/17  08:52:05  geraci
 * killed engine sound when out of time or end game
 *
 * Revision 3.14  1996/10/16  07:44:28  geraci
 * tire squeals during attract work correctly now
 *
 * Revision 3.13  1996/10/09  02:15:06  geraci
 * water sound when driving in water
 *
 * Revision 3.12  1996/10/07  16:05:06  geraci
 * used new distance mask for sound flags
 *
 * Revision 3.11  1996/10/05  09:08:06  geraci
 * move reverb volume adjust to here
 *
 * Revision 3.10  1996/10/04  07:45:10  petrick
 * Moved 'abs()' macro to dis.h.
 *
 * Revision 3.9  1996/10/04  07:30:57  geraci
 * attract sound is now split into attract effects and attract music
 *
 * Revision 3.8  1996/09/30  08:18:50  gray
 * Getting rid of old model globals.
 *
 * Revision 3.7  1996/09/19  05:59:36  geraci
 * parking meters are boring now
 *
 * Revision 3.6  1996/09/18  10:09:07  geraci
 * lowered collision sounds
 *
 * Revision 3.5  1996/09/17  11:21:02  petrick
 * Uses road visual info instead of road code.
 *
 * Revision 3.4  1996/09/17  08:39:06  geraci
 * skids and smoke now have variable tranlucent levels
 *
 * Revision 3.3  1996/09/16  02:14:43  geraci
 * fixed drone engine cut-out
 *
 * Revision 3.2  1996/09/13  22:59:20  geraci
 * bumps tied to collision forces, drone can skid
 *
 * Revision 3.1  1996/09/09  02:24:52  gray
 * JAMMA version
 *
 *	======================================================================
 */

#include "mb.h"
#include "model.h"
#include "modeldat.h"
#include "carsnd.h"
#include "debug.h"
#include "dis.h"
#include "fmath.h"
#include "game.h"
#include "globals.h"
#include "sounds.h"
#include "os_proto.h"
#include "visuals.h"

#include "Pro/sounds.pro"
#include "Pro/d3math.pro"
#include "Pro/unitvecs.pro"
#include "Pro/vecmath.pro"
#include "Pro/math.pro"

#define get_max(a,b) ((a>b)?a:b)
#define range(a,b,c) ((a<b)?b:((a>c)?c:a))
#define sign(a)      ((a<0)?-1:1)
#define SCRAPE_VOL   165

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/

extern S16    which_engine;
extern U8     radio_station;
extern S8     wrong_way;
extern void frwtobod(F32 *vprime, F32 *v, F32 *uvp);

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/
/* State structures for various car sounds, includes volume, pitch, on/off
   flag, and a state variable (in case different sound samples used for 
   same type of car sound */
snd_state  rdnoise_state, wind_state, scrch_state[MAX_LINKS][4];
bump_state car_bump[5];
S16        scrape_state;
S16        skid_intensity[MAX_LINKS][4][2], smoke_intensity[MAX_LINKS][4][2];
S32        scrape_time, bump_time;

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/

const U32 tire_smoke[4] = { App_M_LR_SMOKE,
							App_M_RR_SMOKE,
							App_M_LF_SMOKE | App_M_RF_SMOKE,
							App_M_LR_SMOKE | App_M_RR_SMOKE};

const U32 tire_skid[4] =  { App_M_LF_SKID | App_M_LR_SKID,
							App_M_RF_SKID | App_M_RR_SKID,
							App_M_LF_SKID | App_M_RF_SKID,
							App_M_LR_SKID | App_M_RR_SKID};

const U32 tire_tab[4][2] = { {1, 3}, {2, 0}, {0, 1}, {3, 2} };

typedef enum              {CONE, GLASS, PMETER, BUSH,
						   LIGHTPOLE, TREE, FENCE, NUM_TARGETS} target_types;

const S16 target_snd[] = {S_CONES, S_GLASS, S_PMETER, S_BUSH,
						  S_LIGHTPOLE, S_TREE, S_FENCE};

const S16 target_volume[] = {255, 255, 255,
							 255, 255, 230};

const skid_data skid_tab[4] =
{
	/* left side */
	{ {1500, 1700, 1900, 2200, 2600},
	  1240, 800,  /* pitch */
	  255, 1,     /* volume */
	  160, 250    /* skid, smoke */
	},
	/* right side */
	{ {1500, 1700, 1900, 2200, 2600},
	  1240, 800,  /* pitch */
	  255, 1,     /* volume */
	  160, 250    /* skid, smoke */
	},
	/* front tires */
	{ {800, 975, 1250, 1325, 1500},
	  1340, 800,  /* pitch */
	  255, 1,     /* volume */
	  128, 250    /* skid, smoke */
	},
	/* rear tires */
	{ {2400, 2225, 2050, 1875, 1700},
	  1140, 800,  /* pitch */
	  255, 1,     /* volume */
	  120, 190    /* skid, smoke */
	}
};

                          /* where 64=FL, 448=FR, 192=BL, 320=BR */
const S16 thump_tab[16] = {0,64,448,0,192,128,0,64,320,0,384,448,256,192,320,0};

                       /* where 0=forward, 128=right, 256=backward, 384=left */
const S16 bump_tab[16] = {0,0,256,0,128,64,192,128,384,448,320,384,0,0,256,0};

/* ------------------------------------------------------------------------- */

void InitSndState (snd_state *sndst)
{
    sndst->volume		= 0;
    sndst->pitch		= 0;
    sndst->on			= 0;
    sndst->state_var	= 0;
}

/* ------------------------------------------------------------------------- */
void InitCarSnds (S16 drone_index)
{
	S16 i;

    InitSndState (&rdnoise_state);
	if (drone_index == this_node)
		{
		sndGravelNoise (0, 0); 
		sndRoadNoise (0, 0); 
		sndSplashNoise (0, 0);
		}

    InitSndState (&wind_state);

	if (drone_index == this_node)
		sndWindNoise (0, 0);

	init_skids (drone_index);
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Initalize car bump sounds                                            */

void init_bump_sounds (void)
{
	S16  i;

	for (i=0; i<5; i++)
		init_bump (i);

	kill_scrape_sound();

	bump_time = 0;
}

/* ----------------------------------------------------------------------------- */
/* High priority car sounds that are updated every time the model runs */

void DoIntCarSounds(void)
{
    DoEngineSound();
}

/* ----------------------------------------------------------------------------- */
/* Car sounds updated once every model iteration */
void DoCarSounds (S16 update_car_sounds, S8 skids_only)
{
    S16      i, pitch, volume, vol, angle;
	MODELDAT *m = &model[this_node];

    if ((m->crashflag==1) || (update_car_sounds==0))
		{
		InitCarSnds (this_node);
		}
	else
		{
		if (skids_only == false)
			{
			/* engine bogging */
			if (m->bog_state == 1)
				{
				SOUND (S_BOG);
				m->bog_state = 2;
				}
			else if (m->bog_state == 3)
				{
				SOUND (S_KBOG);
				m->bog_state = 0;
				}

			/* Do wind, and road or gravel */
			/* If we're off-road, do gravel sound */
			/* If player's in the air, turn off road noise, turn up wind volume */
			if ((m->sviscode[0] == AIR) && (m->sviscode[2] == AIR))
				{
				if (rdnoise_state.state_var == DIRT)
					sndGravelNoise(0,0); 
				else if (rdnoise_state.state_var == PAVEMENT)
					sndRoadNoise(0,0); 
				else if (rdnoise_state.state_var == WATER)
					sndSplashNoise (0, 0);

				wind_state.volume = ((vol=velsq>>10) > WIND_MAX_VOL) ? WIND_MAX_VOL : vol;
				wind_state.pitch = WIND_PITCH;
				sndWindNoise(wind_state.pitch, wind_state.volume);
				rdnoise_state.state_var = AIR;
				init_skids (this_node);
				}
			else
				{
				/* Do wind (not as loud when on ground) */
				if (m->mph > 0.0)
					wind_state.volume = ((vol=velsq>>10) > WIND_MAX_VOL) ? WIND_MAX_VOL : vol;
				else 
					wind_state.volume = 0;
				wind_state.pitch = WIND_PITCH;
				sndWindNoise(wind_state.pitch, wind_state.volume);
				/* If on dirt, do gravel sound */
				if ((m->sviscode[0] == DIRT) || (m->sviscode[2] == DIRT))
					{
					if (rdnoise_state.state_var == PAVEMENT) 
						sndRoadNoise(0,0);
					else if (rdnoise_state.state_var == WATER)
						sndSplashNoise (0, 0);

					/* Volume (and pitch) proportional to velocity */
					rdnoise_state.pitch = range (abs((short)(m->mph*2.0)), ROAD_MIN_PITCH, ROAD_MAX_PITCH);
					rdnoise_state.volume = abs((short)(m->mph));
					rdnoise_state.volume += rdnoise_state.volume>>1;
					rdnoise_state.volume = range (rdnoise_state.volume, 0, ROAD_MAX_VOL);
					sndGravelNoise (rdnoise_state.pitch, rdnoise_state.volume);
					rdnoise_state.state_var = DIRT;
					}
				else if ((m->sviscode[0] == WATER) || (m->sviscode[2] == WATER))
					{
					if (rdnoise_state.state_var == DIRT) 
						sndGravelNoise(0,0);
					else if (rdnoise_state.state_var == PAVEMENT) 
						sndRoadNoise(0,0);

					if (rdnoise_state.state_var != WATER)
						SOUND (S_SPLASH);

					/* Volume (and pitch) proportional to velocity */ 
					rdnoise_state.pitch = range (abs(m->mph), ROAD_MIN_PITCH, ROAD_MAX_PITCH);
					rdnoise_state.volume = ((vol=velsq>>8) > ROAD_MAX_VOL) ? ROAD_MAX_VOL : vol;
					sndSplashNoise (rdnoise_state.pitch, rdnoise_state.volume);
					rdnoise_state.state_var = WATER;
					}
				else
					{ /* Do pavement sound (road noise) */
					if (rdnoise_state.state_var == DIRT) 
						sndGravelNoise(0,0);
					else if (rdnoise_state.state_var == WATER)
						sndSplashNoise (0, 0);

					rdnoise_state.pitch = range (abs(m->mph), ROAD_MIN_PITCH, ROAD_MAX_PITCH);
					rdnoise_state.volume = ((vol=velsq>>8) > ROAD_MAX_VOL) ? ROAD_MAX_VOL : vol;
					sndRoadNoise (rdnoise_state.pitch, rdnoise_state.volume);
					rdnoise_state.state_var = PAVEMENT;
					}

#if 0
					{
					S8    buf[50];

					sprintf (buf, "wv %d | wp %d    ", wind_state.volume, wind_state.pitch);
					txt_str (25, 14, buf, GRN_PAL);


					sprintf (buf, "mph %d | velsq %d    ", (S16)(m->mph*0.25), velsq);
					txt_str (25, 15, buf, GRN_PAL);
        

					sprintf (buf, "rdv %d | rdp %d   ", rdnoise_state.volume, rdnoise_state.pitch);
					txt_str (25, 16, buf, GRN_PAL);
					}        
#endif
				}

			/* Do all tire sounds */
			DoTireSqueals (this_node);
			}
		} /* end: if crashflag */
}

S8   count_num;

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Do car collision sounds                                              */
/*	Updated every model iteration	*/
void do_bump_sounds (S16 update_car_sounds)
{
    S16      i, volume, bump_index, scrape_side, scrape_snd;
	F32      high_force;
	MODELDAT *m = &model[this_node];

    if ((m->crashflag==1) || (update_car_sounds==0))
		{
		init_bump_sounds();
		}
	else
		{
		/* make sounds due to large side forces */
		bump_index = 0;
		volume = 0;
		high_force = 0;

		if (bump_time != 0)
			{
			if (IRQTIME - bump_time > ONE_SEC)
				bump_time = 0;
			}

		for (i=0; i<4; i++)
			{
			if (car_bump[i].bump_it && (car_bump[i].peak > high_force))
				high_force = car_bump[i].peak;
			}

		volume = range (high_force*.01, 150, 235);

		for (i=0; i<4; i++)
			{
			if (car_bump[i].bump_it)
				{
				/* if it's big enough, count it */
				if (car_bump[i].peak > high_force*.5)
					bump_index |= 1<<i;

				car_bump[i].peak = 0;
				car_bump[i].bump_it = false;
				}
			}

		if (bump_index)
			{
			if (bump_time == 0)
				{
				sndPositionSound (S_CARBUMP, thump_tab[bump_index], (S8)((F32)volume*0.8));
				sndPositionSound (S_BOOM, thump_tab[bump_index], volume);
				bump_time = IRQTIME;
				}
			else
				{
				sndPositionSound (S_CARSMASH, thump_tab[bump_index], volume);
				sndPositionSound (S_BOOM, thump_tab[bump_index], volume);
				bump_time = 0;
				}
			}
		else
			/* now check car to car collision */
			{
			bump_index = 0;

			if (car_bump[4].bump_it)
				{
				high_force = 0;

				for (i=0; i<3; i++)
					{
					if (abs(car_bump[4].peak_vec[i]) > high_force)
						high_force = abs (car_bump[4].peak_vec[i]);
					}

				volume = range (high_force*.001, 190, 255);

				if (abs(car_bump[4].peak_vec[0]) > high_force*.5)
					{
					if (car_bump[4].peak_vec[0] < 0)
						bump_index |= 0x1;
					else
						bump_index |= 0x2;
					}

				if (abs(car_bump[4].peak_vec[1]) > high_force*.5)
					{
					if (car_bump[4].peak_vec[1] < 0)
						bump_index |= 0x4;
					else
						bump_index |= 0x8;
					}

				if (abs(car_bump[4].peak_vec[2]) > high_force*.5)
					bump_index |= 0x1;

				car_bump[4].peak = 0;
				car_bump[4].bump_it = false;
				}

			if (bump_index)
				{
				sndPositionSound (S_CARSMASH, bump_tab[bump_index], volume);
				sndPositionSound (S_BOOM, bump_tab[bump_index], volume);
				}
			} /* end: now check car to car collision */

		if (bump_index==0 && m->magvel > 20)
			/* if we're fast enough and scraping, make scraping sound */
			{
			scrape_side = 0;

			if (m->BODYFORCE[0][1] && m->BODYFORCE[2][1])
				scrape_side |= 1;

			if (m->BODYFORCE[1][1] && m->BODYFORCE[3][1])
				scrape_side |= 2;

			scrape_snd = 0;

			switch (scrape_state)
				{
				case 0:
					scrape_time = IRQTIME;
					scrape_state = scrape_side;
					scrape_snd = scrape_side;
					break;

				case 1:
				case 2:
				case 3:
					if (scrape_side == 0)
						kill_scrape_sound();

					else if (IRQTIME - scrape_time > ONE_SEC/10)
						{
						scrape_state += 3;
						scrape_snd = scrape_state;
						}
					break;

				case 4:
				case 5:
				case 6:
					if (scrape_side == 0)
						kill_scrape_sound();
					break;
				}

			switch (scrape_snd)
				{
				case 1:
					sndPositionSound (S_CARSCRAPE, 128, SCRAPE_VOL);
					sndPositionSound (S_BOOM, 128, SCRAPE_VOL);
					break;

				case 2:
					sndPositionSound (S_CARSCRAPE, 384, SCRAPE_VOL);
					sndPositionSound (S_BOOM, 384, SCRAPE_VOL);
					break;

				case 3:
					sndPositionSound (S_CARSCRAPE, 128, SCRAPE_VOL);
					sndPositionSound (S_BOOM, 128, SCRAPE_VOL);
					sndPositionSound (S_CARSCRAPE, 384, SCRAPE_VOL);
					sndPositionSound (S_BOOM, 384, SCRAPE_VOL);
					break;

				case 4:
					sndPositionSound (S_SCRAPELOOP, 128, SCRAPE_VOL);
					break;

				case 5:
					sndPositionSound (S_SCRAPELOOP, 384, SCRAPE_VOL);
					break;

				case 6:
					sndPositionSound (S_SCRAPELOOP, 128, SCRAPE_VOL);
					sndPositionSound (S_SCRAPELOOP, 384, SCRAPE_VOL);
					break;
				}
			}
		else
			kill_scrape_sound();

		if (m->lastthump > m->thumpflag)
			m->lastthump = m->thumpflag;

		if (m->lastthump != m->thumpflag)
			{
			m->lastthump = m->thumpflag;

			switch (m->lastthump)
				{
				case 1:
					sndPositionSound (S_CURBWHUMP, m->thump_side, 255);
					break;

				case 2:
					SOUND (S_CAR_LANDS);
					sndPositionSound (S_BOOM, m->thump_side, 140);
					break;

				case 3:
					sndPositionSound (S_BOTTOMOUT, m->thump_side, 140);
					sndPositionSound (S_BOOM, m->thump_side, 140);
					break;
				}
			}
		}
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Check forces due to car and world collisions
*         Upon entry: m -> points to model structure of target car           */

void check_forces_on_car (MODELDAT *m)
{
	S16  i;
	F32  vec[3];

	/* get force on all body corners */
	for (i=0; i<4; i++)
		{
		veccopy (m->BODYFORCE[i], vec);

		get_force_and_peak (i, vec, 5000);
		}

	/* get force due to other cars */
	veccopy (m->CENTERFORCE, vec);
	get_force_and_peak (4, vec, 5000);
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Get force due to collision vector and determine peak state
*         Upon entry: index -> car bump slot to update
*                     vec -> force vector to use
*                     threshold -> force threshold to trigger bump           */

void get_force_and_peak (S16 index, F32 vec[3], F32 threshold)
{
	F32  force;

	if (car_bump[index].bump_it == false)
		{
		/* if time was set and peak is clear, then wait for force to clear */
		if (car_bump[index].time && car_bump[index].peak==0)
			{
			if (vec[0]==0 && vec[1]==0 && vec[2]==0)
				{
				if (IRQTIME-car_bump[index].time > ONE_SEC/2)
					car_bump[index].time = 0;
				}
			else
				car_bump[index].time = IRQTIME;
			}
		else
			{
			if (vec[0]!=0 || vec[1]!=0 || vec[2]!=0)
				{
				force = fsqrt(vec[0]*vec[0] + vec[1]*vec[1] + vec[2]*vec[2]);

				/* check if we are over threshold and lower than last force */
				if (threshold < force)
					{
					if (force > car_bump[index].peak)
						{
						car_bump[index].peak = force;
						veccopy (vec, car_bump[index].peak_vec);
						car_bump[index].time = IRQTIME;
						}
					}
				}

			if (car_bump[index].time && (IRQTIME-car_bump[index].time > ONE_SEC/10))
				{
				car_bump[index].bump_it = true;
				car_bump[index].time = IRQTIME;
				}
			}
		}
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Initialize bump state structure (car/world collisions)
*         Upon entry: index -> car bump slot to initialize                   */

void init_bump (S16 index)
{
	car_bump[index].peak	= 0;
	car_bump[index].time	= 0;
	car_bump[index].bump_it	= false;
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Kill scrape and other locked sounds (for abort and death states)     */

void kill_scrape_sound()
{
	if (scrape_state!=0 || scrape_time!=0)
		sndPositionSound (S_KSCRAPELOOP, 0, 0);

	scrape_state = 0;
	scrape_time = 0;
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Routine called from model task when object has been hit
*         Upon entry: m -> pointer to model slot to check against
*                     t -> pointer to object structure
*         Upon exit: returned -> true = object inside car
*                                false = object outside car                  */

BOOL target_inside_box (S16 slot, Target *t)
{

	F32      pos[3], pos2[3], delta[3];
	CAR_DATA *gc = &game_car[slot];

	/* save offset in relation to car in pos[] */
	vecsub (t->pos, gc->RWR, delta);

	vecM2D(delta);
	frwtobod (delta, pos2, gc->dr_uvs[0]);
	vecD2M(pos2);

	return ((fabs(pos2[0]) < 7) && (fabs(pos2[1]) < 3));
}

S16 temp_sound;

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Routine called from game task when object has been hit
*         Upon entry: t -> pointer to object structure
*                     slot -> car that hit object                            */

void target_sound (Target *t, S16 slot)
{
	S16      i, s_index, bump_index;
	F32      pos[3], pos2[3], delta[3], sq_mag, vec1, vec2, vec3, vec4;
	CAR_DATA *gc = &game_car[this_node];


	/* bail for now if it is not the point of view car */
	if (slot != this_node)
		return;

	if (demo_game && !attract_effects)
		return;

	/*-----------------------------------------*/

	switch (t->type)
		{
		case OBJ_CONE1:
			s_index = CONE;
			break;

		case OBJ_METER1:
			s_index = PMETER;
			break;

		case OBJ_TREEA1:
		case OBJ_TREEC1:
		case OBJ_TREED1:
		case OBJ_TREEE1:
			s_index = TREE;
			break;

		case OBJ_TREEB1:
			s_index = BUSH;
			break;

		case OBJ_WINDOWA1:
		case OBJ_WINDOWB1:
			s_index = GLASS;
			break;

		case OBJ_FENCE1:
			s_index = FENCE;
			break;

		case OBJ_POLE1:
		case OBJ_TLIGHT1:
		case OBJ_SLIGHT1:
			s_index = LIGHTPOLE;
			break;
#if 0
		case OBJ_PEOPLE1:
#endif
		default:
			s_index = -1;
			break;
		}

	if (s_index == -1)
		return;

	pos[1] = gc->dr_pos[0];   pos[2] = -gc->dr_pos[1];   pos[0] = gc->dr_pos[2];

	/* save offset in relation to car in pos[] */
	vecsub (t->pos, pos, delta);

	vecM2D(delta);
	frwtobod (delta, pos2, gc->dr_uvs[0]);
	vecD2M(pos2);

	vec1 = pos2[0] * pos2[0];
	vec2 = pos2[1] * pos2[1];
	sq_mag = vec1 + vec2;

	bump_index = 0;

	if (sq_mag > 1)
		{
		vec1 *= sign(pos2[0]);
		vec2 *= sign(pos2[1]);
		vec3 = .924 * .924 * sq_mag;  /* cos 22.5 deg. */

		/* on right side */
		if (vec2 > vec3)
			bump_index |= 4;

		/* left side */
		else if (vec2 < -vec3)
			bump_index |= 8;

		/* forward */
		if (vec1 > vec3)
			bump_index |= 1;

		else if (vec1 < -vec3)
			bump_index |= 2;
		}

	sndPositionSound (target_snd[s_index], bump_tab[bump_index], target_volume[s_index]);
}

F32 rpm_fix, torque_fix;

#define ENGINE_TWEAK 1
#if ENGINE_TWEAK
F32 dbgRPMbias;
F32 dbgRPMscale;
S32 dbgRPMval;
F32 dbgTORbias;
F32 dbgTORscale;
S32 dbgTORval;
#endif

/* ------------------------------------------------------------------------- */
void StartEngineSound(void)
{
	if (demo_game == 0)
		sndStartManualEngine( which_engine, 250, 100);

	rpm_fix = .8;
	torque_fix = .3;

#if ENGINE_TWEAK
	dbgRPMbias = 0.0;
	dbgRPMscale = 0.8;
	dbgTORbias = 0.0;
	dbgTORscale = 1.0;
#endif
}

#if ENGINE_TWEAK
/* ------------------------------------------------------------------------- */
void DoEngineSound(void)
{
	F32 ftorque;
	U16 clip_rpm;
	S16 clip_torque;

	dbgRPMval = ((F32)(abs(rpm)) * dbgRPMscale) + dbgRPMbias;
#if 1
	if (dbgRPMval > 0x1700)
		clip_rpm = 0x1700;
	else
		clip_rpm = dbgRPMval;
#else
	if (dbgRPMval > MAX_ENG_RPM_SCL)
		clip_rpm = MAX_ENG_RPM_SCL;
	else
		clip_rpm = dbgRPMval;
#endif

	ftorque = model[this_node].engtorque;
	if(ftorque > 0.0)
		dbgTORval = (ftorque * dbgTORscale) + dbgTORbias;
	else
		dbgTORval = (ftorque * dbgTORscale) - dbgTORbias;
	if(dbgTORval > 0x7FFF)
		clip_torque = 0x7FFF;
	else if(dbgTORval < -0x7FFF)
		clip_torque = -0x7FFF;
	else
		clip_torque = dbgTORval;

	if (demo_game == 0)
		{
		if (coast_flag)
			sndUpdateManualEngine (0, 0);
		else
			sndUpdateManualEngine (clip_rpm, clip_torque);
		}
}
#else
/* ------------------------------------------------------------------------- */
void DoEngineSound(void)
{
    U16 my_rpm;

	my_rpm = (F32)(abs(rpm)) * rpm_fix;
	if (my_rpm > MAX_ENG_RPM_SCL)
		my_rpm = MAX_ENG_RPM_SCL;

	if (demo_game == 0)
		{
		if (coast_flag)
			sndUpdateManualEngine (0, 0);
		else
			sndUpdateManualEngine (my_rpm, model[this_node].engtorque*torque_fix);
		}
}
#endif

/* ------------------------------------------------------------------------- */
void StopEngineSound(void)
{
    sndStopEngine();
}    

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*        Generate tire squeals due to brakes, flailing and peelout           */

void DoTireSqueals (S16 drone_index)
{
    S16        i, index, tnum, gr_smoke_thresh, gr_skid_thresh;
    U16        force;
	S32        force_h, force_l, v_thresh, intensity;
	S32        pitch_h, pitch_l, vol_h, vol_l, delta;
	BOOL       dirt_flag, air_flag, water_flag, make_noise;
	char       buf[60];
	tiredes    *tire;
	skid_data  *sk_tab;
	snd_state  *screech_state = &scrch_state[drone_index][0];
	MODELDAT   *m = &model[drone_index];

	dirt_flag = (m->sviscode[0] == DIRT) || (m->sviscode[2] == DIRT);
	air_flag = (m->sviscode[0] == AIR) || (m->sviscode[2] == AIR);
	water_flag = (m->sviscode[0] == WATER) || (m->sviscode[2] == WATER);

	make_noise = (drone_index == this_node) && (demo_game==0 || (demo_game!=0 && attract_effects==true));

    if ((dirt_flag | water_flag) && (m->mph > 10.0))
		m->appearance |= App_M_TIRE_SMOKE;
	else
		m->appearance &= ~(App_M_TIRE_SMOKE | App_M_SKID_MARK);

    /* If tires are not on pavement, turn off all squeals, screeches */
    if (dirt_flag || air_flag || water_flag || coast_flag)
		{
		init_skids (drone_index);

		if (dirt_flag | water_flag)
			{
			for (i=0; i<4; i++)
				{
				smoke_intensity[drone_index][tire_tab[i][0]][0] = 0xff;
				smoke_intensity[drone_index][tire_tab[i][1]][1] = 0xff;
				}
			}

        return;
		}

	tire = &m->tires[0];

	for (tnum=0; tnum<4; tnum++)
		{
#if 0
		sprintf (buf, "%8.2f %8.2f   ", (double)tire[tnum].sideforce, (double)tire[tnum].traction);
		txt_str (5, 35+tnum, buf, WHT_PALB);
#endif

		sk_tab = (skid_data *)&skid_tab[tnum];

		switch (tnum)
			{
			case 0:
				/* left side */
				force = get_max (fabs(tire[1].sideforce), fabs(tire[3].sideforce));
				break;

			case 1:
				/* right side */
				force = get_max (fabs(tire[0].sideforce), fabs(tire[2].sideforce));
				break;

			case 2:
				/* front tires */
				force = get_max (fabs(tire[0].traction), fabs(tire[1].traction));
				break;

			case 3:
				/* rear tires */
				force = fabs(tire[2].traction) + fabs(tire[3].traction);
				break;

			default:
				/* this is for compiler warning only */
				force = 1;
				break;
			}

		if (force >= sk_tab->force_tab[0])
			{
			screech_state[tnum].on = TRUE; /* The sound is ON */

			/* do high bound check */
			if (force > sk_tab->force_tab[4])
				force = sk_tab->force_tab[4];

			/* find spot inside interpolation table */
			for (index=0; index<4; index++)
				{
				if (force <= sk_tab->force_tab[index+1])
					break;
				}

			force_h = sk_tab->force_tab[index+1];
			force_l = sk_tab->force_tab[index];

			delta = (sk_tab->pitch_h - sk_tab->pitch_l)>>2;
			pitch_l = sk_tab->pitch_l + delta * index;
			pitch_h = pitch_l + delta;

			delta = (sk_tab->vol_h - sk_tab->vol_l)>>2;
			vol_l = sk_tab->vol_l + delta * index;
			vol_h = vol_l + delta;

			/* linear scale pitch and volume */
			screech_state[tnum].pitch = (pitch_h-pitch_l)*force / (force_h-force_l) - (pitch_h-pitch_l)*force_l/(force_h-force_l) + pitch_l;
			screech_state[tnum].volume = (vol_h-vol_l)*force / (force_h-force_l) - (vol_h-vol_l)*force_l/(force_h-force_l) + vol_l;

			/* send sound */
			if (make_noise)
				sndDoSkid (tnum, screech_state[tnum].pitch, screech_state[tnum].volume);

			v_thresh = sk_tab->gr_skid_thresh;

			/* set graphic skid mark */
			if (screech_state[tnum].volume > v_thresh)
				{
				model[drone_index].appearance |= tire_skid[tnum];
				intensity = (screech_state[tnum].volume - v_thresh) * 256 / (256 - v_thresh);

				/* set intensity of skid mark */
				skid_intensity[drone_index][tire_tab[tnum][0]][0] = intensity;
				skid_intensity[drone_index][tire_tab[tnum][1]][1] = intensity;
				}
			else
				{
				skid_intensity[drone_index][tire_tab[tnum][0]][0] = 0;
				skid_intensity[drone_index][tire_tab[tnum][1]][1] = 0;
				}

			v_thresh = sk_tab->gr_smoke_thresh;

			/* set graphic smoke */
			if (screech_state[tnum].volume > v_thresh)
				{
				model[drone_index].appearance |= tire_smoke[tnum];
				intensity = (screech_state[tnum].volume - v_thresh) * 256 / (256 - v_thresh);

				/* set intensity of skid mark */
				smoke_intensity[drone_index][tire_tab[tnum][0]][0] = intensity;
				smoke_intensity[drone_index][tire_tab[tnum][1]][1] = intensity;
				}
			else
				{
				smoke_intensity[drone_index][tire_tab[tnum][0]][0] = 0;
				smoke_intensity[drone_index][tire_tab[tnum][1]][1] = 0;
				}
			}
		else if (screech_state[tnum].on)
			/* previously on, decay sound/skid/smoke */
			{
			v_thresh = sk_tab->gr_skid_thresh;

			/* set graphic skid mark */
			if (screech_state[tnum].volume > v_thresh)
				{
				model[drone_index].appearance |= tire_skid[tnum];
				intensity = (screech_state[tnum].volume - v_thresh) * 256 / (256 - v_thresh);

				/* set intensity of skid mark */
				skid_intensity[drone_index][tire_tab[tnum][0]][0] = intensity;
				skid_intensity[drone_index][tire_tab[tnum][1]][1] = intensity;
				}
			else
				{
				skid_intensity[drone_index][tire_tab[tnum][0]][0] = 0;
				skid_intensity[drone_index][tire_tab[tnum][1]][1] = 0;
				}

			v_thresh = sk_tab->gr_smoke_thresh;

			/* set graphic smoke */
			if (screech_state[tnum].volume > v_thresh)
				{
				model[drone_index].appearance |= tire_smoke[tnum];
				intensity = (screech_state[tnum].volume - v_thresh) * 256 / (256 - v_thresh);

				/* set intensity of skid mark */
				smoke_intensity[drone_index][tire_tab[tnum][0]][0] = intensity;
				smoke_intensity[drone_index][tire_tab[tnum][1]][1] = intensity;
				}
			else
				{
				smoke_intensity[drone_index][tire_tab[tnum][0]][0] = 0;
				smoke_intensity[drone_index][tire_tab[tnum][1]][1] = 0;
				}

			screech_state[tnum].volume -= VOL_FADE_VAL;

			/* if now off, clear values */
			if (screech_state[tnum].volume <= 0)
				{
				screech_state[tnum].volume = 0;
				screech_state[tnum].on = FALSE;
				model[drone_index].appearance &= ~(tire_skid[tnum] | tire_smoke[tnum]);
				}

			/* send sound state */
			if (make_noise)
				sndDoSkid (tnum, screech_state[tnum].pitch, screech_state[tnum].volume);
			}

#if 0
		sprintf (buf, "scr %4d | vol %3d | ptch %4d | stat %d   ", 
				 force,
				 screech_state[tnum].volume,
				 screech_state[tnum].pitch,
				 screech_state[tnum].on
				 );

		txt_str (5, 40+tnum, buf, GRN_PALB);
#endif

		}
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Stop all skid sounds                                                 */

void init_skids (S16 drone_index)
{
	S16  i;

	for (i=0; i<4; i++)
		{
		if ( scrch_state[drone_index][i].volume!=0 ||
			 scrch_state[drone_index][i].pitch!=0 ||
			 scrch_state[drone_index][i].on!=0 ||
			 scrch_state[drone_index][i].state_var!=0
			)
			InitSndState (&scrch_state[drone_index][i]);

		skid_intensity[drone_index][tire_tab[i][0]][0] = 0;
		skid_intensity[drone_index][tire_tab[i][1]][1] = 0;
		smoke_intensity[drone_index][tire_tab[i][0]][0] = 0;
		smoke_intensity[drone_index][tire_tab[i][1]][1] = 0;

		if (drone_index == this_node)
			sndDoSkid (i, 0, 0);
		}
}

/* ------------------------------------------------------------------------- */
void StopRadio (U8 radio_station)
{
	music (Undo_it, radio_station);
}

/* ------------------------------------------------------------------------- */
void StartRadio (U8 radio_station)
{
	music (Do_it, radio_station);
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Initialize reverb                                                    */

void init_reverb (void)
{
	U16  i, cmd_tab[2];

	i = 6;
	sound_wparms (S_REVERB_PARMS, 1, &i);

	cmd_tab[0] = 0;
	cmd_tab[1] = 0;

	sound_wparms (S_REVERB_RETURN_VOL, 2, &cmd_tab[0]);

	in_tunnel = false;
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Handle all reverb volumes and set tunnel flag                        */

void handle_reverb (void)
{
	S16			i, j, k;
	U16			snd_flags, cmd_tab[2], temp_vol;
	static U16	old_snd_flags;
	CAR_DATA	*gc = &game_car[this_node];

	snd_flags = gc->sound_flags & (ALL_REVERB_MASK | ALL_DIST_MASK);
	in_tunnel = gc->in_tunnel;

	if (snd_flags != old_snd_flags)
		{
		if (snd_flags == 0)
			{
			cmd_tab[0] = 0;
			cmd_tab[1] = 0;

			sound_wparms (S_REVERB_RETURN_VOL, 2, &cmd_tab[0]);
			}
		else
			{
			if (snd_flags & LF_REVERB_MASK)
				{
				temp_vol = 3 - ((gc->sound_flags & LF_DIST_MASK) >> LF_DIST_SHIFT);
				temp_vol = temp_vol * 0xAAA;

				cmd_tab[wrong_way] = temp_vol;
				}
			else
				cmd_tab[wrong_way] = 0;

			if (snd_flags & RT_REVERB_MASK)
				{
				temp_vol = 3 - ((gc->sound_flags & RT_DIST_MASK) >> RT_DIST_SHIFT);
				temp_vol = temp_vol * 0xAAA;

				cmd_tab[1-wrong_way] = temp_vol;
				}
			else
				cmd_tab[1-wrong_way] = 0;

			sound_wparms (S_REVERB_RETURN_VOL, 2, &cmd_tab[0]);
			}

#if 0
			{
			char buf[50];
			sprintf (buf, "%4.4x %4.4x  ", cmd_tab[0], cmd_tab[1]);
			txt_str (20, 20, buf, WHT_PALB);
			}
#endif

		old_snd_flags = snd_flags;
		}
}
