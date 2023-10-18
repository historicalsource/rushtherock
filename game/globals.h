/*
 *	globals.h	-- Declares global variables and symbols.
 *
 *	Copyright 1996 Time Warner Interactive.
 *	Unauthorized reproduction, adaptation, distribution, performance or 
 *	display of this computer program or the associated audiovisual work
 *	is strictly prohibited.
 *
 *	======================================================================
 *	$Author: gray $						$Date: 1997/10/02 00:54:17 $
 *	$Revision: 3.32 $						$Locker:  $
 *	======================================================================
 *	Change Log:
 *
 *	$Log: globals.h,v $
 * Revision 3.32  1997/10/02  00:54:17  gray
 * Added seconds-since-poweron variable (gUpSeconds).
 *
 * Revision 3.31  1997/10/01  17:46:27  grjost
 * Added new "Number of Laps" menu in GUTS
 *
 * Revision 3.30  1997/09/25  00:28:56  gray
 * NTRACKS now 7. Added mirror mode global.
 *
 * Revision 3.29  1997/09/15  22:30:23  gray
 * Removed trackset hack - trackno now 0 to NTRACKS.
 *
 * Revision 3.28  1997/09/13  01:47:43  grjost
 * Fixed for 12 cars.
 *
 * Revision 3.27  1997/07/01  18:58:32  gray
 * Made alternate tracks (additional 4 tracks) view 1 selectable.
 *
 * Revision 3.26  1997/06/28  01:41:43  gray
 * Extended to 4 tracks.
 *
 * Revision 3.25  1997/06/04  02:15:46  gray
 * Restructuring controls (gas,brake,clutch,etc) handling.
 *
 * Revision 3.24  1997/03/31  00:04:09  gray
 * update_game_data blocks model ast with gModeldatLock.
 *
 * Revision 3.23  1997/01/29  02:14:18  geraci
 * track options and drone difficulty added
 *
 * Revision 3.22  1997/01/23  07:46:47  geraci
 * each track can have a different difficulty setting
 *
 * Revision 3.21  1997/01/15  00:24:00  geraci
 * added a save version of track len
 *
 * Revision 3.20  1997/01/15  00:07:11  geraci
 * added global array for track lengths defined by operator
 *
 * Revision 3.19  1996/12/13  12:10:27  geraci
 * saved version of marker flag
 *
 * Revision 3.18  1996/12/02  04:51:38  gray
 * Removed obsolete USE_NET_TIME.
 *
 * Revision 3.17  1996/11/27  12:56:10  gray
 * Changed NetTime from F32 to 10microsec S32.
 *
 * Revision 3.16  1996/11/25  04:10:37  petrick
 * CHanged tourney mode to tourney setup.
 *
 * Revision 3.15  1996/11/06  13:48:20  geraci
 * model version of edges supported
 *
 * Revision 3.14  1996/11/03  05:56:56  geraci
 * split up debug switches and regular game switches
 *
 * Revision 3.13  1996/10/31  06:55:38  geraci
 * car data is in upper case now
 *
 * Revision 3.12  1996/10/24  09:25:07  petrick
 * Added human/drone counts.
 *
 * Revision 3.11  1996/10/24  07:13:29  petrick
 * Added win_opts and continue_opts.
 *
 * Revision 3.10  1996/10/23  00:14:33  petrick
 * Made track_no a global.
 *
 * Revision 3.9  1996/10/17  08:17:33  petrick
 * Made gstate a GState so gdb would know the state names.
 *
 * Revision 3.8  1996/10/11  10:08:56  petrick
 * Added marker and damage flags.
 *
 * Revision 3.7  1996/10/10  07:25:20  petrick
 * Made tourney mode global.
 *
 * Revision 3.6  1996/10/10  05:20:15  petrick
 * Made RUSHGLOBAL so globals only need be defined in globals_h and not globals.c too.
 *
 * Revision 3.5  1996/10/07  16:15:12  geraci
 * moved some game options to globals
 *
 * Revision 3.4  1996/09/28  01:36:36  gray
 * Made global camera pos and uvs (gCamPos gCamUvs).
 *
 * Revision 3.3  1996/09/27  03:26:51  gray
 * Made camera control local. Eliminated CamFunc.
 *
 * Revision 3.2  1996/09/26  00:23:30  gray
 * Killed ztypes.h
 *
 * Revision 3.1  1996/09/09  02:23:24  gray
 * JAMMA version
 *
 *	======================================================================
 */

#ifndef _GLOBALS_H
#define _GLOBALS_H

#include "game.h"
#include "modeldat.h"
#include "Pro/globals.pro"


/*===============================  DEFINES  =================================*/
#define MAX_LINKS		8				/*	Max. cars active at any one time. */
#define NCARS			12				/*	Max. cars selectable  */

#define MAX_TRACKS 8
#define NTRACKS	7
/* flag for temporary track hacks */
#define TRKFIX 1

/*	Define this only in globals.c so globals get declared correctly. */
#ifndef RUSHGLOBAL			
#define RUSHGLOBAL 		extern
#endif


/*===============================  GLOBALS  =================================*/

RUSHGLOBAL S32          gModeldatLock;  /* Inticates game task is copying modeldat. */
RUSHGLOBAL F32		gCamPos[3],gCamUvs[3][3];	/* Camera position and orientation. */
RUSHGLOBAL GState 	gstate;
RUSHGLOBAL U32 		edges; 				/*	Bits set for 1 cycle when a switch changes to on. */
RUSHGLOBAL U32 		m_edges; 			/*	model's version of edges */
RUSHGLOBAL U32 		trailing_edges;		/*	Bits set for 1 cycle when a switch changes to off. */
RUSHGLOBAL U32 		levels;				/*	Current values of the switches. */
RUSHGLOBAL U32		m_levels;			/*	model's version of switches */

RUSHGLOBAL U32 		dedges; 			/*	edges for debug switches */
RUSHGLOBAL U32 		m_dedges; 			/*	model's version of edges for debug switches */
RUSHGLOBAL U32 		trailing_dedges;	/*	Bits set for 1 cycle when a switch changes to off. */
RUSHGLOBAL U32 		dlevels;			/*	Current values of debug switches. */
RUSHGLOBAL U32		m_dlevels;			/*	model's version of switches */

RUSHGLOBAL S16 		swforce;			/* st. wheel force. Sent to motor amp every timer interrupt */
RUSHGLOBAL BOOL		HAS_SHIFTER;

RUSHGLOBAL MODELDAT model[NCARS];		/* player, drone and net player/drone saved here */
RUSHGLOBAL CAR_DATA game_car[NCARS];/* array with car data that game task can use */
RUSHGLOBAL S8		num_active_cars;	/* number of active cars/slots */
RUSHGLOBAL U8		gThisNode;			/* Slot number (model index) of the local node. */
RUSHGLOBAL S16 		trackno;			/* Current track number (0-3) */
RUSHGLOBAL S8		coast_flag;			/* set when time run's out and cars are coasting */
RUSHGLOBAL S8		end_game_flag;		/* set when someone crosses finish line */
RUSHGLOBAL S8		in_tunnel;			/* 1=enter tunnel, -1=exit tunnel, 0=hold */
RUSHGLOBAL S16		last_track_loaded;	/* last track loaded, -1=no track */
RUSHGLOBAL S16		this_node;			/* drone that camera is following */
RUSHGLOBAL S16		target_drone;		/* when follow mode enable, use this drone */
RUSHGLOBAL S32		gFlyMode;			/* 1=put model into fly-around mode, 0=no */
RUSHGLOBAL S16		mpath_edit;			/* 1=max path edit mode on, 0=off */
RUSHGLOBAL S16		gSWforce;			/* steering wheel force. Sent to motor amp every timer interrupt.*/
RUSHGLOBAL S16		gSWpos;				/* steering wheel pos. scaled to -0x1000 to 0x1000, update at model time.*/
RUSHGLOBAL S16		demo_game;			/* 0=disabled, 1=enable demo game, 2=stop demo */
RUSHGLOBAL U32		guts_opt;			/* Options set by guts. (GUTS_OPT_DBG_SW) */
RUSHGLOBAL U32		game_opt;			/* Options set via Game Options menu. */
RUSHGLOBAL U32		track_opt;			/* Options set via Game Difficulty menu. */
RUSHGLOBAL U32		lap_opt;			/* Options set via Number of Laps menu. */
RUSHGLOBAL S16		lap_flag;			/* Set to true if one checkpoint to go */
RUSHGLOBAL S8		language;			/* Japanese=?, English=? */
RUSHGLOBAL S8		win_opts;			/* Options for 1st place finishers. */
RUSHGLOBAL S8		continue_opts;		/* Options for 1st place finishers. */
RUSHGLOBAL S8		difficulty[MAX_TRACKS];		/* 0=easiest, 7=hardest */
RUSHGLOBAL S8		difficulty_save[MAX_TRACKS];	/* default saved value of difficulty */
RUSHGLOBAL S8		drone_diff;		/* 0=easy, 1=medium, 2=hard, 3=real hard */
RUSHGLOBAL S8		drone_diff_save;	/* default saved value of drone difficulty */
RUSHGLOBAL S8		metric;				/* metric=?, standard=? */
RUSHGLOBAL S8		cheats;				/* cheat keys on/off */
RUSHGLOBAL S8		tourney_setup;		/* Tournament mode state. */
RUSHGLOBAL S8		damage_flag;		/* 0=disable car damage. */
RUSHGLOBAL S8		marker_flag;		/* 1=show marker over player's cars. */
RUSHGLOBAL S8		marker_flag_save;	/* default saved value of marker flag. */
RUSHGLOBAL BOOL		attract_effects;	/* true = car sound heard during demo */
RUSHGLOBAL BOOL		attract_effects_save;	/* default saved value of attract effects */
RUSHGLOBAL BOOL		attract_music;		/* true = music heard during attract */
RUSHGLOBAL BOOL		attract_music_save;	/* default saved value of attract music. */
RUSHGLOBAL S8		track_len[MAX_TRACKS];		/* Number of laps (0=use cpinit default) */
RUSHGLOBAL S8		track_len_save[MAX_TRACKS];	/* default saved value of number of laps */
RUSHGLOBAL S32		gMirrorMode;		/* 0=normal tracks, 1=mirror tracks. */

RUSHGLOBAL S16  	num_humans;			/*	Number of humans in the race. */
RUSHGLOBAL S16		num_drones;			/*	Number of drones on the race. */
RUSHGLOBAL S16  	humans[MAX_LINKS];	/*	List of slots that humans are using. */
RUSHGLOBAL S16  	drones[MAX_LINKS];	/*	List of slots that drones are using. */

RUSHGLOBAL F32		gSecPerCount;		/* CPU counts per second */
RUSHGLOBAL F32		gMsecPerCount;		/* CPU counts per msec */
RUSHGLOBAL F32		ginvMsecPerCount;	/* msec per CPU count */
RUSHGLOBAL F32		gNetTimeToSec;		/* convert net timer (10 microsec) to seconds */

RUSHGLOBAL VS32		IRQTIME;			/* Game millisecond counter. Reset in attract. */
RUSHGLOBAL VS32		gUpSeconds;		/* Elapsed seconds. Reset only at powerup. */

RUSHGLOBAL S16	gGasRaw, gBrakeRaw, gClutchRaw, gWheelRaw;	/* Raw pot value. */
RUSHGLOBAL S16	gGasInt, gBrakeInt, gClutchInt, gWheelInt;	/* Integer scaled 0-0x1000 (wheel -0x1000-0x1000) */
RUSHGLOBAL F32	gGasVal, gBrakeVal, gClutchVal, gWheelVal;	/* Float Scaled 0.0-1.0, (wheel -1.0 - 1.0) */

#endif	/* !_GLOBALS_H */
