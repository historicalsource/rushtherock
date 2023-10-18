/*
 *	stats.h  non-guts statistics header file
 *
 *	Copyright 1996 Time Warner Interactive
 *	Unauthorized reproduction, adaptation, distribution, performance or 
 *	display of this computer program or the associated audiovisual work
 *	is strictly prohibited.
 *
 *	======================================================================
 *	$Author: gray $						$Date: 1997/09/16 03:44:03 $
 *	$Revision: 3.2 $						$Locker:  $
 *	======================================================================
 *	Change Log:
 *
 *	$Log: stats.h,v $
 * Revision 3.2  1997/09/16  03:44:03  gray
 * Update for 8 tracks and limit to 4 car difficulties.
 *
 * Revision 3.1  1996/09/09  02:23:24  gray
 * JAMMA version
 *
 * Revision 1.5  1996/08/31  05:58:18  gray
 * Added hi score count.
 *
 * Revision 1.4  1996/08/30  19:49:16  gray
 * Added some fields.
 *
 * Revision 1.3  1996/08/30  01:02:57  gray
 * Added time-of-day histogram.
 *
 * Revision 1.2  1996/08/28  06:29:11  gray
 * Added stuff to running stat struct.
 *
 * Revision 1.1  1996/08/27  08:23:14  gray
 * Initial revision
 *
 *
 *	======================================================================
 */

#ifndef _STATS_H
#define _STATS_H

#include "typedefs.h"

#define STAT_NUM_HOURS 24
#define STAT_NUM_DAYS   7
#define STAT_NUM_TRACKS 8
#define STAT_NUM_CARS   4
#define STAT_NUM_TIMES 32
#define STAT_BIN_SECS  15

#define STAT_NUM_DRONE_OPTIONS   3
#define STAT_DRONE_REG    0
#define STAT_DRONE_NONE   1
#define STAT_DRONE_RACERX 2

#define STAT_NUM_FORCE_OPTIONS   3
#define STAT_FORCE_REG  0
#define STAT_FORCE_MORE 1
#define STAT_FORCE_FULL 2

#define STAT_NUM_KETCHUP_OPTIONS 2
#define STAT_KETCHUP_ON  0
#define STAT_KETCHUP_OFF 1

#define STAT_NUM_TUNES 7
#define STAT_NUM_VIEWS 3

typedef struct trkstat {
	U32 games;
	U32 trksel_secs;
	U32 carsel_secs;
	U32 transsel_secs;
	U32 race_secs;
	U32 hiscore_secs;
	U32 finish;
	U32 hiscore;
	U32 linked;
	U32 autotrans;
	U32 reverse;
	U32 abort;
	U32 crash;
	U32 car[STAT_NUM_CARS];
	U32 music_secs[STAT_NUM_TUNES];
	U32 view_secs[STAT_NUM_VIEWS];
	U32 drone_opt[STAT_NUM_DRONE_OPTIONS];
	U32 force_opt[STAT_NUM_FORCE_OPTIONS];
	U32 ketchup_opt[STAT_NUM_KETCHUP_OPTIONS];
	U32 tc_hist[STAT_NUM_TIMES][STAT_NUM_CARS];	/*time x car histogram*/
} TRKSTAT;

typedef struct stattyp {
	U32 attract_secs;
	U32 tod_hist[STAT_NUM_HOURS][STAT_NUM_DAYS];	/*time-of-day histogram*/
	TRKSTAT	trk[STAT_NUM_TRACKS];
} STAT;

typedef struct gamestat {
	BOOL game_in_progress;
	S16 next_gstate;
	U32 attract_mstime;
	U32 trksel_mstime;
	U32 carsel_mstime;
	U32 transel_mstime;
	U32 race_mstime;
	U32 hiscore_mstime;
	U32 trk_num;
	U32 car_num;
	U32 autotrans;
	U32 linked;
	U32 reverse_cnt;
	U32 about_cnt;
	U32 death_cnt;
	U32 finish;
	U32 hiscore;
	U32 music_mstime[STAT_NUM_TUNES];
	U32 view_mstime[STAT_NUM_VIEWS];
} GAMESTAT;


#include "Pro/stats.pro"

#endif	/*#ifndef _STATS_H*/


