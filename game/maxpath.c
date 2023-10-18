/*
 *	maxpath.c	-- Functions for handling drone path following.
 *
 *	Copyright 1996 Atari Corporation
 *	Unauthorized reproduction, adaptation, distribution, performance or 
 *	display of this computer program or the associated audiovisual work
 *	is strictly prohibited.
 *
 *	======================================================================
 *	$Author: gray $						$Date: 1997/09/25 03:05:32 $
 *	$Revision: 3.48 $						$Locker:  $
 *	======================================================================
 *	Change Log:
 *
 *	$Log: maxpath.c,v $
 * Revision 3.48  1997/09/25  03:05:32  gray
 * Negate drone path x pos if mirror mode.
 *
 * Revision 3.47  1997/09/24  22:03:23  gray
 * Prevent divide by zero cp dist.
 *
 * Revision 3.46  1997/09/19  01:57:29  gray
 * Bandaid fix for backward T4 bug.
 *
 * Revision 3.45  1997/09/15  22:30:23  gray
 * Removed trackset hack - trackno now 0 to NTRACKS.
 *
 * Revision 3.44  1997/07/29  00:52:57  gray
 * Fixed maxpath_dist sentinel to fit into a short. Warning with gnu2.7.
 *
 * Revision 3.43  1997/07/23  01:35:38  gray
 * Added sanity check before linking maxpaths.
 *
 * Revision 3.42  1997/06/28  01:41:43  gray
 * Extended to 4 tracks.
 *
 * Revision 3.41  1997/06/04  02:44:44  gray
 * Zapped ithrottle.
 *
 * Revision 3.40  1997/06/04  02:15:46  gray
 * Restructuring controls (gas,brake,clutch,etc) handling.
 *
 * Revision 3.39  1997/05/30  01:25:14  gray
 * Clean up crashflag stuff.
 *
 * Revision 3.38  1997/05/21  00:10:16  gray
 * caruvs finally exterminated.
 *
 * Revision 3.37  1997/05/11  03:37:10  gray
 * Cleaning model interface.
 *
 * Revision 3.36  1997/04/22  21:37:08  gray
 * Fixed some drone path recording stuff.
 *
 * Revision 3.35  1997/02/26  03:49:36  geraci
 * don't throw race if free game is enabled
 *
 * Revision 3.34  1997/02/15  01:49:34  geraci
 * drones stay with humans, lead drone has own difficulty, t1 drone doesn't ram into you at start
 *
 * Revision 3.33  1997/01/30  23:19:23  geraci
 * uses drone difficulty correctly
 *
 * Revision 3.32  1997/01/30  20:32:26  geraci
 * new drone difficulty setting
 *
 * Revision 3.31  1997/01/15  20:02:13  geraci
 * defined number of max paths
 *
 * Revision 3.30  1997/01/10  03:41:04  geraci
 * fixed initialization bug for maxpath data
 *
 * Revision 3.29  1996/12/14  22:44:26  geraci
 * fixed backwards through shortcut problem
 *
 * Revision 3.28  1996/12/14  08:18:38  geraci
 * fixed wrong-way problem on track 1
 *
 * Revision 3.27  1996/12/13  16:04:59  geraci
 * fixed track 1 cheat due to weird track layout
 *
 * Revision 3.26  1996/12/13  12:06:04  geraci
 * more new lap stuff
 *
 * Revision 3.25  1996/12/13  08:16:13  geraci
 * new lap stuff
 *
 * Revision 3.24  1996/12/05  13:49:45  geraci
 * fixed bug
 *
 * Revision 3.23  1996/12/04  01:58:46  geraci
 * fixed entering from wrong end of short cut cheat
 *
 * Revision 3.22  1996/11/27  08:12:12  geraci
 * I can write to the hard drive again
 *
 * Revision 3.21  1996/11/25  11:39:23  geraci
 * here is gone
 *
 * Revision 3.20  1996/11/25  03:49:31  geraci
 * drones loop better, when resurrecting won't start beyond lap end
 *
 * Revision 3.19  1996/11/23  04:02:51  geraci
 * default path assign is back
 *
 * Revision 3.18  1996/11/19  21:08:32  geraci
 * save game version of initial start position
 *
 * Revision 3.17  1996/11/16  10:10:18  gray
 * Free up some old maxpath arrays.
 *
 * Revision 3.16  1996/11/15  09:53:39  petrick
 * Changes for the new driver.
 *
 * Revision 3.15  1996/11/15  05:12:22  geraci
 * auto changing of default path
 *
 * Revision 3.14  1996/11/11  22:08:10  geraci
 * linked priority and solo priority replaced by priority
 *
 * Revision 3.13  1996/11/07  14:37:24  geraci
 * ease on maxpath hint, lot's of new tools
 *
 * Revision 3.12  1996/10/31  07:03:57  geraci
 * multi max paths supported
 *
 * Revision 3.11  1996/10/25  11:07:21  geraci
 * start of multi maxpath support
 *
 * Revision 3.10  1996/10/23  00:14:33  petrick
 * Made track_no a global.
 *
 * Revision 3.9  1996/10/22  09:14:14  geraci
 * lap can be cancelled during record
 *
 * Revision 3.8  1996/10/09  02:21:07  geraci
 * removed unused routine
 *
 * Revision 3.7  1996/10/07  16:15:28  geraci
 * fixed up maxpath record and display
 *
 * Revision 3.6  1996/10/03  06:35:47  geraci
 * made drone min speed around 60 mph
 *
 * Revision 3.5  1996/09/25  00:45:34  gray
 * Removed old RamRom recording stuff.
 *
 * Revision 3.4  1996/09/25  00:22:18  gray
 * Removed old ZOID download stuff.
 *
 * Revision 3.3  1996/09/24  23:35:27  gray
 * Removed MAXPATHDBG stuff (part of ZGLU purge).
 *
 * Revision 3.2  1996/09/20  02:14:57  geraci
 * keeps track of relative distances through short cuts
 *
 * Revision 3.1  1996/09/09  02:24:52  gray
 * JAMMA version
 *
 * Revision 2.40  1996/09/06  22:05:33  geraci
 * no cheating drones
 *
 * Revision 2.39  1996/09/06  11:02:26  geraci
 * first pass at track hints
 *
 * Revision 2.38  1996/09/05  08:12:47  geraci
 * new maxpath data structure (widths and hints)
 *
 * Revision 2.37  1996/09/04  05:14:31  geraci
 * drones are wacky, but boy can they pass
 *
 * Revision 2.36  1996/09/01  09:18:02  gray
 * Model types cleanup. First pass - removed fptyp.
 *
 * Revision 2.35  1996/08/30  17:07:05  geraci
 * more docile drones
 *
 * Revision 2.34  1996/08/23  01:59:42  geraci
 * drone can pass much better
 *
 * Revision 2.33  1996/08/21  16:39:16  geraci
 * best lap recording mode, also drones are slightly vicious
 *
 * Revision 2.32  1996/08/21  11:10:54  gray
 * Recording maxpath on phoenix.
 *
 * Revision 2.31  1996/08/21  10:35:32  geraci
 * temp speed boost
 *
 * Revision 2.30  1996/08/21  07:08:05  geraci
 * fixed drone straying bug
 *
 * Revision 2.29  1996/08/19  03:48:29  geraci
 * docile drones
 *
 * Revision 2.28  1996/08/18  21:25:20  gray
 * Removing zoid stuff. No more zoid.h.
 *
 * Revision 2.27  1996/08/18  03:30:29  gray
 * Removing ZOID stuff.
 *
 * Revision 2.26  1996/08/09  04:51:36  geraci
 * initializes some maxpath variables when path point is reset
 *
 * Revision 2.25  1996/08/02  21:35:41  geraci
 * temp fix for lap unsynced problem
 *
 * Revision 2.24  1996/08/02  19:02:50  geraci
 * common routine to find next maxpath
 *
 * Revision 2.23  1996/07/31  15:58:05  geraci
 * new tools for maxpath
 *
 * Revision 2.22  1996/07/29  09:51:30  geraci
 * nnew routine to sync to last checkpoint
 *
 * Revision 2.21  1996/07/27  08:21:05  geraci
 * running distance calculated with maxpath data only
 *
 * Revision 2.20  1996/07/25  09:19:36  geraci
 * fixed out of range distance in find maxpath intervals
 *
 * Revision 2.19  1996/07/20  04:39:32  geraci
 * does a much better distance calculation and subsequently gives a better place.
 *
 * Revision 2.18  1996/07/20  00:19:44  gray
 * Slimmed timer interrrupt routine. wheelpos() and forcewheel() moved.
 *
 * Revision 2.17  1996/07/19  23:38:33  geraci
 * fixed drone abort due to fast model time
 *
 * Revision 2.16  1996/07/19  10:03:32  geraci
 * disabled drone training
 *
 * Revision 2.15  1996/07/17  13:39:11  gray
 * Turned off some debug text.
 *
 * Revision 2.14  1996/07/16  21:49:37  gray
 * Load drone path data into RAM at track load time.
 *
 * Revision 2.13  1996/07/13  06:56:08  geraci
 * made maxpath into one loop plus 2 checkpoints
 *
 * Revision 2.12  1996/07/06  01:05:33  geraci
 * more betterer drones
 *
 * Revision 2.11  1996/06/28  05:16:03  geraci
 * max path edit mode, also drones are a bit smarter
 *
 * Revision 2.10  1996/06/22  01:44:29  geraci
 * new endianess for phoenix
 *
 * Revision 2.9  1996/06/21  17:28:34  geraci
 * drones drive faster and less toward maxpath when slower than maxpath speed
 *
 * Revision 2.8  1996/06/18  04:58:05  geraci
 * fixed file sizes for track hint table
 *
 * Revision 2.7  1996/06/17  23:11:35  geraci
 * read/write track hints file
 *
 * Revision 2.6  1996/06/15  03:01:48  geraci
 * new drone training code
 *
 * Revision 2.5  1996/06/12  21:41:22  geraci
 * keeps track of drone times around track
 *
 * Revision 2.4  1996/06/08  01:47:03  geraci
 * new drone smarts
 *
 * Revision 2.3  1996/06/05  19:40:53  geraci
 * drones are a little better, still more to do
 *
 * Revision 2.2  1996/06/01  02:37:35  geraci
 * initial new drone smarts
 *
 * Revision 2.1  1996/05/04  02:56:18  petrick
 * Focus 2 release
 *
 * Revision 1.23  1996/05/02  01:10:23  geraci
 * fixed bug when view1 pressed for maxpath record
 *
 * Revision 1.22  1996/05/01  19:49:26  geraci
 * added some routines for path to maxpath linking
 *
 * Revision 1.21  1996/04/30  02:21:17  geraci
 * finished link between path and maxpath
 *
 * Revision 1.20  1996/04/27  05:00:04  petrick
 * Added header.
 *
 *
 * 10/04/95 asg	Created
 *
 *	======================================================================
 */

#include "stdtypes.h"
#include "typedefs.h"
#include "os_proto.h"
#include "mb.h"
#include "model.h"
#include "modeldat.h"
#include "maxpath.h"
#include "menus.h"
#include "stree.h"
#include "checkpoint.h"
#include "debug.h"
#include "dis.h"
#include "game.h"
#include "globals.h"
#include "hdrive.h"

#include "Pro/d3math.pro"
#include "Pro/dnl.pro"
#include "Pro/unitvecs.pro"
#include "Pro/mathsubs.pro"
#include "Pro/math.pro"
#include "Pro/mdrive.pro"
#include "Pro/pots.pro"
#include "Pro/select.pro"
#include "Pro/stree.pro"
#include "Pro/vecmath.pro"

/*---------------------------------------------------------------------------*/

#define range_it(v,l,h)		( ((v) < (l)) ? (l) : (((v) > (h)) ? (h) : (v)) )

/* hint types */
typedef enum HINT_TYPES
{ 
	STAY_WITHIN_WIDTH,
	GET_ON_MPATH,
	STAY_ON_MPATH,
	STAY_PARALLEL,
	RESET_PARALLEL,
	EASE_ON_MPATH,
	NUM_HINT_TYPES
} HINT_TYPES;

#define SWITCH_OKAY			0x20;

#define NUM_POSTS			20
#define FWD_BACK			0
#define LEFT_RIGHT			1

#define REC_START_IDX		0

/*---------------------------------------------------------------------------*/

MPHEADER *gMaxPath, *gmpath_save, *gmpath_save2;
MPATH	 *mpath, *mpath_save, *mpath_save2;
MPHEADER *mpheader_tab[MAX_MPATHS];
MPATH	 *mpath_tab[MAX_MPATHS];

S32 	*mpathnum, *mpath_num[MAX_MPATHS], *mpath_active[MAX_MPATHS];
S32		*new_pathnum;
S32 	mprecording;
S16		maxpath_index;
char 	mpstrbuf[128];

S32 	lastwheel[MAX_LINKS];
S32 	lastbrake[MAX_LINKS];
S32 	lastthrottle[MAX_LINKS];
F32		last_rwr[MAX_LINKS][MAX_LINKS][3], last_dir[MAX_LINKS][3];
F32		last_save_dir[MAX_LINKS][3];
F32		last_relpos[MAX_LINKS][MAX_LINKS][3];

MPCTL 	 mpctl[MAX_LINKS];
MPWEIGHT weight_table[NTRACKS][MAX_LINKS];

S16		maxpath_to_path[MAXMPATH];
U16		maxpath_dist[MAXMPATH];
S16		drone_done_count;
S16		num_mpaths, mpaths[MAX_MPATHS];
S32		mpath_save_num;
S16		marker_a, marker_b;
S16		post_obj_index[NUM_POSTS], mpath_change_mode, cur_mpath_point_s16;
F32		mpFlyTime, mpFlySpeed, mpFlyDist, mpFlyYaw, change_vector[2];
F32		mpFlyUvs[3][3], mpFlyPos[3], cur_mpath_point_f32, speed_delta, dummy;
S32		loop_stamp, best_loop_time;
S16		cur_point_set, marker_point_set, current_drone;
S16		cur_weight_drone, weight_index;
S32		cur_hint_type, last_mpath_time;

const F32	unit_uvs[3][3] = {
                               {1.0, 0.0, 0.0},
                               {0.0, 1.0, 0.0},
                               {0.0, 0.0, 1.0}
                             };

const char  hint_types[NUM_HINT_TYPES+1][20]  = { "Within width   ",
												  "Get on maxpath ",
												  "Stay on maxpath",
												  "Stay parallel  ",
												  "Reset parallel ",
												  "Ease on maxpath",
												  "Invalid hint   " };

/*---------------------------------------------------------------------------*/

extern S16 		num_path_points;
extern S16 		path_dist_index[];
extern U16 		path_dist[];
extern S16 		path_index[];
extern S16 		path_to_maxpath[];
extern Track	CP_Track[];
extern PATHTYP	*path;
extern F32		ZCLOCK;
extern S16		brake;
extern U32		number_checkpoints;
extern S16		lap_loop_index;
extern F32		lap_loop_distance;
extern U32		play_time;
extern U8		man_gear;
extern void WrFile(const char * path_str, const char * filename, U32 len, U32 * src);


/*****************************************************************************/

void InitMaxPath(S32 record)
{
	S16      i, j, k;
	S16      humans[MAX_LINKS], drones[MAX_LINKS], cars_in_order[MAX_LINKS];
	S16      num_humans, num_drones;

	MPHEADER *header;
	MPATH *mp;

	num_mpaths = 0;

	for (i=0; i<MAX_MPATHS; i++)
		{
		mpath_tab[i] = (MPATH *)(gMaxPath + i*MAXMPATH + 1);
		header = (MPHEADER *)(gMaxPath + i*MAXMPATH);
		mpheader_tab[i] = header;
		mpath_num[i] = &(header->num_points);
		mpath_active[i] = &(header->mpath_active);

		if (*mpath_active[i])
			{
			mpaths[num_mpaths++] = i;
			if(gMirrorMode)
				{
				/* negate model y (world x) coordinate. */
				for(mp=mpath_tab[i],j=0;j<header->num_points;j++,mp++)
					mp->pos[1] = -mp->pos[1];
				}
			}
		}

	/* point to base path and get number of points */
	mpath = mpath_tab[0];
	mpathnum = &(gMaxPath->num_points);

	mpath_save = (MPATH *)(gmpath_save);
	mpath_save2 = (MPATH *)(gmpath_save2);

	read_weight_table();

	if (record != -1)
		{
		char buf[50];
		sprintf (buf, "Point set %ld", record);
		txt_str (34, 40, buf, RED_PAL);
		txt_str (34, 41, "MAX PATH RECORD:", RED_PAL);
		mprecording = 1;
		cur_point_set = record;
		mpath_save_num = REC_START_IDX;
		}
    else
		{
		txt_str(34, 41, "                ", RED_PAL);
		mprecording = 0;

		place_cars_in_order (cars_in_order, humans, drones, &num_humans, &num_drones);
		assign_default_paths (num_drones, drones);

		for (i=0; i<MAX_LINKS; i++)
			{
			mpctl[i].mpi = 0;
			mpctl[i].xrel = 0;
			mpctl[i].yrel = 0;
			mpctl[i].interval_time = IRQTIME;
			mpctl[i].new_mpi = -1;
			lastwheel[i] = 0;
			lastbrake[i] = 0;
			lastthrottle[i] = 0;

			if (model[i].drone_type==DRONE)
				mpctl[i].mpath_index  = mpctl[i].default_path;
			else
				/* init to base path */
				mpctl[i].mpath_index = 0;
			}

		cur_point_set = 0;
		marker_point_set = 0;
		}

	cur_hint_type = STAY_ON_MPATH;
	last_mpath_time = IRQTIME;
	current_drone = -1;

	loop_stamp = 0;
	best_loop_time = 9999999;
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Assign default paths to drones                                       */

void assign_default_paths (S16 num_drones, S16 *drones)
{
	S16  i, j, k;

	for (i=0,j=0; i<num_drones; i++)
		{
		/* in drone view mode, use next path */
		for (k=0; k<num_mpaths; k++)
			{
			if (mpheader_tab[j]->priority > 5 ||
				dlevels & SW_DEBUG10)
				break;

			if (++j >= num_mpaths)
				j = 0;
			}
				
		/* if we couldn't find a high priority path, just use base path */
		if (k == MAX_MPATHS)
			j = 0;

#if 0
		if (mpctl[drones[i]].default_path == mpaths[j])
#endif
			mpctl[drones[i]].default_path = mpaths[j];

		if (++j >= num_mpaths)
			j = 0;
		}
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*      Make list of ordered cars by place
*        Upon entry: cars_in_order -> array to be set to all cars in order
*                    humans -> array to be set to all humans in order
*                    drones -> array to be set to all drones in order
*                    num_humans -> int pointer to be set to number of humans
*                    num_drones -> int pointer to be set to number of drones */

void place_cars_in_order (S16 *cars_in_order, S16 *humans, S16 *drones, S16 *num_humans, S16 *num_drones)
{
	S16        i, j, index;

	*num_humans = *num_drones = 0;

	for (i=0; i<num_active_cars; i++)
		{
		index = 0;
		for (j=0; j<num_active_cars; j++)
			{
			index = model[j].slot;

			if (game_car[index].place == i)
				break;
			}

		cars_in_order[i] = index;

		/* place car into proper array */
		if (model[index].drone_type==HUMAN)
			humans[(*num_humans)++] = index;
		else
			drones[(*num_drones)++] = index;
		}
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Synchronize maxpath index to last checkpoint crossed
*         Upon entry: node -> index of car to initialize                     */

void sync_maxpath_to_last_checkpoint (S16 node)
{
	S16  index;

	/* get maxpath close to dist1 */
	index = path_to_maxpath[path_dist_index[model[node].last_cp_index]];

	mpctl[node].new_mpi = index;

	if (index+100 < game_car[node].mpath.mpi)
		game_car[node].mpath_laps += 2;

	game_car[node].mpath.mpi = index;
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Check if current lap is faster than previous one and save if so      */

void check_mpath_save (BOOL cancel_last_lap)
{
	S16  i, max_save;
	S32  state, loop_time;
	char buf[60];

	if (mprecording)
		{
		if ((cancel_last_lap == false) || (levels & SW_MUSIC))
			{
			if (loop_stamp == 0)
				loop_time = GetElapsedTime();
			else
				loop_time = IRQTIME - loop_stamp;

			if ((loop_time < best_loop_time) || (levels & SW_MUSIC))
				{
				state = prc_set_ipl (INTS_OFF);

				for (i=0; i<mpath_save_num; i++)
					mpath_tab[cur_point_set][i] = mpath_save2[i];

				*mpath_num[cur_point_set] = mpath_save_num;
				*mpath_active[cur_point_set] = 1;

				prc_set_ipl (state);

/*				write_drone_path_to_hd (0);*/

				best_loop_time = loop_time;
				}

			sprintf (buf, "Cur Lap Time:  %8.3f ", (double)loop_time/(double)ONE_SEC);
			txt_str (2, 36, buf, WHT_PALB);
			sprintf (buf, "Best Lap Time: %8.3f ", (double)best_loop_time/(double)ONE_SEC);
			txt_str (2, 37, buf, WHT_PALB);
			}
		else
			{
			txt_str (2, 36, "Cancelled Lap              ", WHT_PALB);
			}

		mpath_save_num = REC_START_IDX;

		loop_stamp = IRQTIME;

		play_time += 200 * ONE_SEC;

		}
}

/*****************************************************************************/

void MaxPath(MODELDAT *m)
{
    if(mprecording)
    	RecordMaxPath(m);

#if 0
	sprintf(mpstrbuf,"NUM:%ld MPI:%ld GAS:%ld BRK:%ld    ",
			*mpathnum,
			mpctl[m->net_node].mpi,
			lastthrottle[m->net_node],
			lastbrake[m->net_node]);
	txt_str(6,20,mpstrbuf,RED_PALB);
#endif
    }

/*****************************************************************************/

void RecordMaxPath (MODELDAT *m)
{
	MPATH *mpp,*pmp;
	F32 x,y,z,dist;
        
	mpp = (MPATH *)&mpath_save2[mpath_save_num];

	mpp->hints = mpath_tab[cur_point_set][mpath_save_num].hints;

	mpp->base_to_path = mpath_save_num;
	mpp->path_to_base = MAXMPATH-mpath_save_num-2;

	if (mpath_save_num == REC_START_IDX)
		{
		SaveMaxPathData (m, mpp);
		PrintMaxPath (mpath_save_num);
		mpath_save_num++;
		}
	else if (mpath_save_num < MAXMPATH)
		{
		pmp = mpp - 1;
		x = m->RWR[0] - pmp->pos[0];
		y = m->RWR[1] - pmp->pos[1];
		z = m->RWR[2] - pmp->pos[2];
		dist = xxsqrt(x*x + y*y + z*z);
		if(dist > 20.0)
			{
			SaveMaxPathData (m, mpp);
			PrintMaxPath (mpath_save_num);
			mpath_save_num++;
			}
		}
}

/*****************************************************************************/

void SaveMaxPathData(MODELDAT *m,MPATH *mp)
{
	S32 i,j;
	F32 x,y,z;

	mp->pos[0] = m->RWR[0];
	mp->pos[1] = m->RWR[1];
	mp->pos[2] = m->RWR[2];
	x = m->RWV[0];
	y = m->RWV[1];
	z = m->RWV[2];
	mp->speed = sqrt(x*x + y*y + z*z);
    }

/*****************************************************************************/

void PrintMaxPath(S32 mpi)
{
    MPATH *mp;
    
    mp = (MPATH *)&mpath_save2[mpi];
    sprintf(mpstrbuf,"path index = %ld",mpi);
    txt_str(34,42,mpstrbuf,RED_PAL);
    sprintf(mpstrbuf,"xpos = %f",(double)mp->pos[0]);
    txt_str(34,43,mpstrbuf,RED_PAL);
    sprintf(mpstrbuf,"ypos = %f",(double)mp->pos[1]);
    txt_str(34,44,mpstrbuf,RED_PAL);
    sprintf(mpstrbuf,"zpos = %f",(double)mp->pos[2]);
    txt_str(34,45,mpstrbuf,RED_PAL);
    sprintf(mpstrbuf,"speed = %f",(double)mp->speed);
    txt_str(34,46,mpstrbuf,RED_PAL);
    }

/*************** DRONE CONTROL ******************/

void MaxPathControls(MODELDAT *m)
{
    F32 x,y,len,tpos[3],tspd;
	S16   index;
    MPCTL *cp;

    if (mprecording)
		{
		MaxPathZeroControls(m);
		return;
		}

    /* If any AIR, adjust wheel pos and rotation speed */
    /* else if slipping, get control (may be same as air) */
    /* else get to ideal path and speed */
    
    /* 1st pass, ignore AIR and slip - just ideal */

    cp = &mpctl[m->net_node];
    MP_FindInterval (m, cp);
    MP_TargetSpeed (m, cp);
    MP_TargetSteerPos (m, cp);
    
	avoid_areas (m->net_node);

	tspd = cp->tgtspd * m->drone_scale;

	if (tspd < 100)
		{
		if (cp->tgtspd < 100)
			tspd = cp->tgtspd;
		else
			tspd = 100;
		}

	/* feel good fix */
	if (lap_flag && trackno==0 && win_opts==0)
		tspd *= .9;

    tpos[0] = cp->tgtpos[0];
    tpos[1] = cp->tgtpos[1];
    tpos[2] = cp->tgtpos[2];

    AdjustSpeed(m,tspd);    
    AdjustSteer(m,tpos);

	m->please_abort_me = (IRQTIME-cp->interval_time > ONE_SEC) && (cp->mpi > 10);

	if (fabs(cp->xrel)>100 || fabs(cp->yrel)>100)
		m->please_abort_me = true;
    }

/*****************************************************************************/

void MaxPathZeroControls(MODELDAT *m)
{
#if KILLMAININ
    m->throttle = 0.0;
#else
    m->mainin.throttle = 0;
#endif
    m->mainin.brake = 0;
    m->fastin.wheel = 0;
    m->fastin.swvel = 0;
    }

/*****************************************************************************/

S32 PrevMaxPath (S32 mpi, S16 path_index)
{
    if (mpi <= 0)
    	return (mpheader_tab[path_index]->lap_end);
	else
		return (mpi - 1);
}

/*****************************************************************************/

S32 NextMaxPath (S32 mpi, S16 path_index)
{
    if (mpi < (mpheader_tab[path_index]->lap_end))
    	return (mpi + 1);
	else
		return (mpheader_tab[path_index]->lap_start);
}

/*****************************************************************************/

S32 prev_maxpath (S32 mpi, S16 path_index, S16 *loop_flag)
{
    if (mpi <= 0)
		{
		if ((trackno == 0) || (trackno == 3))
			{
			*loop_flag = 0;
			return (0);
			}
		else
			{
			*loop_flag = 1;
			return (mpheader_tab[path_index]->lap_end);
			}
		}
	else
		{
		*loop_flag = 0;
		return (mpi - 1);
		}
}

/*****************************************************************************/

S32 next_maxpath (S32 mpi, S16 path_index, S16 *loop_flag)
{
    if (mpi < (mpheader_tab[path_index]->lap_end))
		{
		*loop_flag = 0;
    	return (mpi + 1);
		}
	else
		{
		*loop_flag = 1;
		return (mpheader_tab[path_index]->lap_start);
		}
}

/*****************************************************************************/

void MP_FindInterval(MODELDAT *m,MPCTL *cp)
{
	S16 flag;

    MP_IntervalPos (m, cp, true);
    if(cp->xrel >= cp->len)
		{
		cp->mpi = NextMaxPath (cp->mpi, cp->mpath_index);
		cp->interval_time = IRQTIME;
		MP_IntervalPos (m, cp, false);
		}
    else if(cp->xrel < 0.0)
		{
		cp->mpi = PrevMaxPath (cp->mpi,cp->mpath_index);
		MP_IntervalPos (m, cp, false);
		}
}

/*****************************************************************************/

void MP_IntervalPos (MODELDAT *m, MPCTL *cp, BOOL change_flag)
{
    S16      i, j, nmpi, nnmpi, index, base_mpi, high_index, high_nextmp;
    F32      x, y, z, nx, ny, invdist, score, high_score, delta_spd, mspd;
	F32      xpos, ypos, x1, y1, x2, y2, dx, dy, a1, b1, c1, dist;
	F32      speed, xy_speed, vec0, vec1, dp, dist_save;
	S32      cur_time;
	BOOL     flag;
    MPATH    *mp, *nmp, *nnmp;
	MPWEIGHT *wt;
	S16      node = m->net_node;

	/* do we need to resync to maxpath? */
	if (cp->new_mpi >= 0)
		{
		nmpi = mpath_tab[cp->mpath_index][cp->new_mpi].base_to_path;

		if (nmpi == -1)
			{
			cp->mpi = cp->new_mpi;
			cp->mpath_index = 0;
			}
		else
			cp->mpi = nmpi;

		cp->interval_time = IRQTIME;
		cp->new_mpi = -1;
		}

	/* check for manual switching of maxpath */
	if ((m_dlevels & SW_DEBUG1) && (m_levels & SW_REVERSE) && (this_node == node))
		{
		base_mpi = mpath_tab[cp->mpath_index][cp->mpi].path_to_base;

		if (base_mpi != -1)
			{
			index  = (SW_DEBUG12 & m_dlevels) ? 4 : 0;
			index += (SW_DEBUG13 & m_dlevels) ? 2 : 0;
			index += (SW_DEBUG14 & m_dlevels) ? 1 : 0;

			nmpi = mpath_tab[index][base_mpi].base_to_path;

			if (nmpi != -1)
				{
				cp->mpi = nmpi;
				cp->xrel = 0;
				cp->yrel = 0;
				cp->interval_time = IRQTIME;
				cp->new_mpi = -1;
				cp->mpath_index = index;
				}
			}
		}

	cur_time = IRQTIME;
	if (change_flag && (cur_time - last_mpath_time > ONE_SEC/2) &&
		(current_drone == -1 || drones[current_drone] == node))
		{
		/* pick next one */
		if (++current_drone >= num_drones)
			current_drone = 0;

		/* find good one */
		for (j=0; j<num_drones; j++)
			{
			if (model[drones[current_drone]].we_control)
				break;
			if (++current_drone >= num_drones)
				current_drone = 0;
			}

		/* update time */
		last_mpath_time = cur_time;

		/* switch to best path if collisions are enabled */
		if (!(m_dlevels & SW_DEBUG10))
			{
			base_mpi = mpath_tab[cp->mpath_index][cp->mpi].path_to_base;

			if (base_mpi != -1)
				{
				/* get position of car */
				xpos =  m->reckon.RWR[XCOMP];
				ypos =  m->reckon.RWR[YCOMP];

				high_score = 0;
				high_index = -1;
				high_nextmp = 0;

				/* calculate speed of car */
				x = m->RWV[0];
				y = m->RWV[1];
				z = m->RWV[2];
				xy_speed = x*x + y*y;
				speed = xy_speed + z*z;

				/* calculate speed in x-y direction */
				vec0 = vec1 = 0;

				wt = &weight_table[trackno][game_car[node].weight_index];
				/* zero is below threshold */
				if (xy_speed < 50*50)
					xy_speed = 0;
				else
					{
					/* turn into unit vectors */
					xy_speed = 1.0 / xxsqrt (xy_speed);
					vec0 = x * xy_speed;
					vec1 = y * xy_speed;
					}

				/* go through all paths and derive a score */
				for (i=0; i<num_mpaths; i++)
					{
					score = 0;
					index = mpaths[i];

					/* see if a link to this path exists */
					nmpi = mpath_tab[index][base_mpi].base_to_path;
					nnmpi = 0;

					if (nmpi != -1)
						{
						x1 =  mpath_tab[index][nmpi].pos[0];
						y1 =  mpath_tab[index][nmpi].pos[1];

						/* get second point on line */
						nnmpi = NextMaxPath (nmpi, index);

						x2 =  mpath_tab[index][nnmpi].pos[0];
						y2 =  mpath_tab[index][nnmpi].pos[1];

						dx = x1 - xpos;
						dy = y1 - ypos;

						dist_save = -1;

						/* is point within 10 feet? */
						if (dx*dx + dy*dy < 400)
							{
							dy = y1 - y2;
							dx = x1 - x2;

							a1 =  dy;
							b1 = -dx;
							c1 =  y1 * dx - x1 * dy;

							/* get distance of car to max path */
							dist = xxsqrt (a1*a1 + b1*b1);

							if (dist > .01)
								{
								dist = abs (a1*xpos + b1*ypos + c1) / dist;

								if (dist < 10)
									score = (1 - dist * .1) * wt->distance_weight;
								}
							}

						/* now check speed deltas */
						mspd = mpath_tab[index][nmpi].speed;
						delta_spd = abs (speed - mspd*mspd);

						/* count toward score if speed is within 50 mph */
						if (delta_spd < 50*1.4667)
							score += (1 - delta_spd / (50*1.4667)) * wt->speed_weight;

						/* bump score if this is supposed to be our path */
						if (index == cp->default_path)
							score += wt->own_path_weight;

						/* if we're scoring our current path, score the current hint */
						if (index == cp->mpath_index)
							{
							j = mpath_tab[cp->mpath_index][cp->mpi].hints;
							score += (j==STAY_ON_MPATH || j==STAY_PARALLEL || j==RESET_PARALLEL) * wt->hint_weight;
							}

						/* compare vector of car velocity against path vector */
						if (xy_speed == 0)
							score += wt->direction_weight;
						else
							{
							dx = x2 - x1;
							dy = y2 - y1;

							dist = 1.0 / xxsqrt (dx*dx + dy*dy);
							dx *= dist;
							dy *= dist;

							/* get dot product to determine percent of vector difference */
							dp = (vec0 * dx + vec1 * dy + 1) * .5;

							score += dp * wt->direction_weight;
							}

						/* give a score based on path priority */
						score += (mpheader_tab[index]->priority / 10.0) * wt->priority_weight;

						/* now see if this score rules */
						if (score > high_score)
							{
							high_score = score;
							high_index = index;
							high_nextmp = nmpi;
							}

#if 0
						if (dlevels & SW_DEBUG1 && node == this_node)
							{
							char buf[50];
							sprintf (buf, "%6.3f ", (double)score);
							txt_str (10, 35+index, buf, WHT_PALB);
							sprintf (buf, "%d  ", game_car[node].weight_index);
							txt_str (10, 34, buf, WHT_PALB);
							}
#endif

						}
					else if (dlevels & SW_DEBUG1 && node == this_node)
						txt_str (10, 35+index, "       ", WHT_PAL);

					}

				if (high_index != -1 && high_index != cp->mpath_index)
					{
					cp->mpi = high_nextmp;
					cp->interval_time = IRQTIME;
					cp->new_mpi = -1;
					cp->mpath_index = high_index;
					}
				}
			}
		}

	mp_interval_pos (m->RWR, cp);
}

/*****************************************************************************/

void mp_interval_pos (F32 *RWR, MPCTL *cp)
{
    MPATH *mp,*nmp,*nnmp;
	S16   flag;
    S32   nmpi;
    F32   x,y,nx,ny,invdist;
    
    /* setup pointers for current and next */
    mp = &mpath_tab[cp->mpath_index][cp->mpi];
    nmpi = NextMaxPath (cp->mpi, cp->mpath_index);
    nmp = &mpath_tab[cp->mpath_index][nmpi];

    /* direction vector for interval (xy plane) */
    nx = nmp->pos[0] - mp->pos[0];
    ny = nmp->pos[1] - mp->pos[1];
    cp->len = xxsqrt(nx*nx + ny*ny);
    /* prevent divide by zero. */
    invdist = (cp->len > 0.01) ? 1.0/(cp->len) : 0.0;
    nx *= invdist;
    ny *= invdist;

    /* real world distance to car CG */
    x = RWR[0] - mp->pos[0];
    y = RWR[1] - mp->pos[1];

    /* CG relative to interval (simplified matrix multiply in xy plane) */
    cp->xrel = x*nx + y*ny;
    cp->yrel = y*nx - x*ny;

    /* Get Path Curvature*/
    nnmp = &mpath_tab[cp->mpath_index][NextMaxPath(nmpi,cp->mpath_index)];
    cp->cyrel = (nnmp->pos[1] - mp->pos[1])*nx - (nnmp->pos[0] - mp->pos[0])*ny;
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Find closest maxpath interval (for all other cars (do in game task)) */

void find_maxpath_intervals()
{
	S16			i, node, slot, sc_section, okay_to_use_avg;
	F32			new_dist, scale, mag1, mag2, dist1, dist2, delta_dist;
	F32			pos[3], delta1[3], delta2[3];
    MPCTL		*cp;
	MODELDAT	*m;
	CAR_DATA	*gc;
	CheckPoint	*chkpnt;

	for (slot=0; slot<num_active_cars; slot++)
		{
		node = model[slot].slot;

		m = &model[node];
		gc = &game_car[node];
		cp = &gc->mpath;

		if ((   gc->we_died == true)
			|| (gc->crashflag == true)
			|| (m->resurrect.moving_state >= 0))
			continue;

		/* try to re-sync both lap counters */
		if (gc->syncmpath_laps != gc->mpath_laps)
			{
			if (gc->lap_sync_time == 0)
				gc->lap_sync_time = IRQTIME;
			else if (abs (gc->lap_sync_time - IRQTIME) > 5 * ONE_SEC)
				gc->mpath_laps = gc->syncmpath_laps;
			}
		else
			gc->lap_sync_time = 0;

		/* first get distance due to lap number */
		gc->distance = gc->mpath_laps * lap_loop_distance;

#if 0
		if (node == this_node)
			{
			char buf[50];

			sprintf (buf, "%4.4x %d ", gc->shortcut_flags, gc->last_mpath);

			txt_str (12, 30, buf, WHT_PALB);
			}
#endif

		okay_to_use_avg = true;

		/* if we're in a short cut, update short cut progress data */
		if (gc->shortcut_flags & S_CUT_MASK)
			{
			sc_section = gc->shortcut_flags & SC_DIST_MASK;

			if (gc->old_shortcut != -1)
				{
				if (gc->old_shortcut != sc_section)
					{
					veccopy (gc->dr_pos, gc->shortcut_pos);
					gc->old_shortcut = sc_section;
					}
				}
			else
				/* first time into short cut */
				{
				if (sc_section > 4)
					{
					okay_to_use_avg = false;
					}
				else
					{
					gc->last_mpath = cp->mpi;
					veccopy (gc->dr_pos, gc->shortcut_pos);
					gc->old_shortcut = sc_section;
					}
				}

			if (gc->old_shortcut != -1)
				{
				vecsub (gc->shortcut_pos, gc->dr_pos, delta1);
				mag1 = magnitude (delta1);

				gc->distance += maxpath_dist[gc->last_mpath] + sc_section*100 + mag1;
				}
			}

		/* are we in an invalid shortcut, coming out of one or never in one? */
		if (gc->old_shortcut == -1 || !(gc->shortcut_flags & S_CUT_MASK))
			{
			/* were we in one? */
			if (gc->old_shortcut > 0)
				{
				vecsub (gc->shortcut_pos, gc->dr_pos, delta1);
				mag1 = magnitude (delta1);

				dist1 = maxpath_dist[gc->last_mpath] + gc->old_shortcut*100 + mag1;

				/* find max path point closest to re-entry spot */
				i = find_maxpath_dist (gc->last_mpath, dist1);

				mpctl[node].new_mpi = i;
				cp->mpi = i;
				cp->xrel = 0;
				cp->yrel = 0;
				}

			gc->old_shortcut = -1;

			/* check if out of maxpath sync */
			if ((fabs(cp->xrel)>300 || fabs(cp->yrel)>300) && okay_to_use_avg)
				/* yes, use average method */
				{
				chkpnt = CP_Track[trackno].chk_point;

				/* we are somewhere between these two distances */
				dist1 = (F32)path_dist[path_dist_index[m->last_cp_index]];
				dist2 = (F32)path_dist[path_dist_index[m->last_cp_index+1]];

				for (i=0; i<3; i++)
					pos[i] = gc->dr_pos[i];

				vecsub ((F32 *)pos, chkpnt[m->last_cp_index].pos, delta1);
				vecsub (chkpnt[m->next_cp_index].pos, (F32 *)pos, delta2);

				mag1 = magnitude (delta1);
				mag2 = magnitude (delta2);

				vecsub (chkpnt[m->last_cp_index].pos, chkpnt[m->next_cp_index].pos, delta1);
				delta_dist = magnitude (delta1);

				if ((mag1 > delta_dist) || (mag2 > delta_dist))
					scale = 0;
				else
					{
					scale = mag1 + mag2;

					if (scale != 0.0)
						scale = mag1 / scale;
					}

				/* get maxpath close to dist1 */
				i = path_to_maxpath[path_dist_index[m->last_cp_index]];

				/* get delta from checkpoint and add maxpath dist at checkpoint */
				new_dist = (dist2 - dist1) * scale + maxpath_dist[i];

				/* find max path point closest to resurrect spot */
				i = find_maxpath_dist (i, new_dist);

				cp->mpi = i;

				/* get new displacement position */
				gc->distance += new_dist;
				}
			else
				gc->distance += maxpath_dist[cp->mpi] + cp->xrel;
			}

#if 1
		if (dlevels & SW_DEBUG12)
			{
			char buf[50];

			sprintf (buf, "%4.4ld %2d %2d %7.3f ", cp->mpi, gc->mpath_laps, gc->laps, (double)gc->distance);

			if (m->collidable==true && gc->crashflag==false)
				txt_str (2, 37+node, buf, WHT_PALB);
			else
				txt_str (2, 37+node, buf, RED_PALB);
			}
#endif

#if 0
			{
			char buf[50];

			sprintf (buf, "%ld %8.3f %6.3f %6.3f 6.3f  ",
					 mpctl[node].mpi,
					 (double)gc->distance,
					 (double)mpctl[node].xrel,
					 (double)mpctl[node].yrel,
					 (double)mpctl[node].cyrel);
			txt_str (5, 35+node, buf, WHT_PALB);
			}
#endif
		}
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Update maxpath index for a particular car
*         Upon entry: node -> slot number of car to update                   */

void update_mpath_index (S16 node)
{
	S16      flag;
	CAR_DATA *gc = &game_car[node];
	MPCTL    *cp = &gc->mpath;

	mp_interval_pos (gc->RWR, cp);
	if (cp->xrel >= cp->len)
		{
		cp->mpi = next_maxpath (cp->mpi, cp->mpath_index, &flag);
		gc->mpath_laps += flag;
		mp_interval_pos (gc->RWR, cp);
		}
    else if(cp->xrel < 0.0)
		{
		cp->mpi = prev_maxpath (cp->mpi, cp->mpath_index, &flag);
		gc->mpath_laps -= flag;
		mp_interval_pos (gc->RWR, cp);
		}
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Fix maxpath index due to new lap                                     */

void adjust_maxpath_due_to_lap (S16 node, S16 mode)
{
    MPCTL	*cp;
	S32		new_index;

	if (mode == 0)
		cp = &mpctl[node];
	else
		cp = &game_car[node].mpath;

	new_index = cp->mpi - mpheader_tab[cp->mpath_index]->num_in_lap;

	if (new_index < 0)
		new_index = 0;

	cp->mpi = new_index;
}

/*****************************************************************************/

const F32 g_MPspdscale = 1.0;

/*****************************************************************************/

void MP_TargetSpeed (MODELDAT *m, MPCTL *cp)
{
    MPATH *mp,*nmp;
    S32 nmpi;
    F32 interp,yrel,offsc;
	S16 node = m->net_node;

    mp = &mpath_tab[cp->mpath_index][cp->mpi];
    nmpi = NextMaxPath(cp->mpi,cp->mpath_index);
    nmp = &mpath_tab[cp->mpath_index][nmpi];

    /* interpolate speed in current interval */
    interp = cp->xrel/cp->len;
    if(interp < 0.0)
    	interp = 0.0;
    else if(interp > 1.0)
    	interp = 1.0;
    cp->tgtspd = mp->speed + (interp * (nmp->speed - mp->speed));
    cp->tgtspd *= g_MPspdscale;

    /* slow down if off target line*/
    yrel = (cp->yrel > 0.0) ? cp->yrel : -cp->yrel;
    if(yrel < 0.1)
    	offsc = 1.0;
    else if(yrel > 80.0)
    	offsc = 0.8;
    else
    	offsc = 1.0 - (yrel * yrel * 0.00003125); /*offsc=(1-((yrel/30)**2))*/

    cp->tgtspd *= offsc;

    if(cp->tgtspd < 28.0)
    	cp->tgtspd = 28.0;
    }

const F32 g_lookahd = 80.0;

/*****************************************************************************/

void MP_TargetSteerPos (MODELDAT *m, MPCTL *cp)
{
    MPATH *mp,*nmp;
    S32   nmpi;
    F32   x,y,z,dist,tgtdist,interp;
	S16   node = m->net_node;

    mp = &mpath_tab[cp->mpath_index][cp->mpi];
    nmpi = NextMaxPath(cp->mpi,cp->mpath_index);
    nmp = &mpath_tab[cp->mpath_index][nmpi];

    /* look ahead g_lookahd feet and interpolate steer position */
    tgtdist = g_lookahd + cp->xrel;
    dist = cp->len;
    tgtdist -= dist;
    while(tgtdist > 0.0)
		{
		mp = nmp;
		nmpi = NextMaxPath(nmpi,cp->mpath_index);
		nmp = &mpath_tab[cp->mpath_index][nmpi];

		x = nmp->pos[0] - mp->pos[0];
		y = nmp->pos[1] - mp->pos[1];
		z = nmp->pos[2] - mp->pos[2];
		dist = xxsqrt(x*x + y*y + z*z);
		tgtdist -= dist;
		}

	if (dist < 1e-5)
		{
		txt_str (10, 20, "Corrupt Maxpath Data", RED_PAL);
		interp = 1;
		}
	else
		interp = (dist + tgtdist)/dist;

    if(interp < 0.0)
    	interp = 0.0;
    else if(interp > 1.0)
    	interp = 1.0;
    
    cp->tgtpos[0] = mp->pos[0] + (interp * (nmp->pos[0] - mp->pos[0]));
    cp->tgtpos[1] = mp->pos[1] + (interp * (nmp->pos[1] - mp->pos[1]));
    cp->tgtpos[2] = mp->pos[2] + (interp * (nmp->pos[2] - mp->pos[2]));
    }

/*****************************************************************************/

void AdjustSpeed(MODELDAT *m, F32 tspd)
{
    S32   i, slip, spddir, node, cur_time;
    F32 x, y, z, dspd, delta_speed;

	node = m->net_node;

	tspd *= 1.05;

    if((m->tires[0].slipflag == 40) || (m->tires[1].slipflag == 40))
		{
		lastbrake[node] = 0;
		lastthrottle[node] = 0;
		}
    else
		{
    	x = m->RWV[0];
    	y = m->RWV[1];
    	z = m->RWV[2];
    	dspd = xxsqrt(x*x + y*y + z*z);

		delta_speed = tspd - dspd;

		if (delta_speed < 0)
			delta_speed *= .5;

    	lastthrottle[node] += (delta_speed * 750.0);

		if (lastthrottle[node] < -3000)
			lastbrake[node] -= (lastthrottle[node]+3000)/4;
		else
			lastbrake[node] = 0;
		}

    if(lastthrottle[node] < 0)
        lastthrottle[node] = 0;
    else if(lastthrottle[node] > 0x1000)
        lastthrottle[node] = 0x1000;

    if(lastbrake[node] < 0)
        lastbrake[node] = 0;
    else if(lastbrake[node] > 0x1000)
        lastbrake[node] = 0x1000;

#if KILLMAININ
/* make fp later*/
    m->throttle = lastthrottle[node]*CTLSCALE;
#else
    m->mainin.throttle = lastthrottle[node];
#endif
    m->mainin.brake = lastbrake[node];
    }

/*****************************************************************************/

const F32 g_sfscale = 0.00012;

/*****************************************************************************/

void AdjustSteer(MODELDAT *m, F32 *pos)
{
    S32 i, node;
    F32 dpos[3],rpos[3];
    F32 x,y,z,ivmag,dstr;
    F32 angle,ay0,ay1,sideforce,fbal;
    S32 newwheel;
    
	node = m->net_node;

	pos[LEFT_RIGHT] *= 1.8;

	angle = fatan2 (pos[0],pos[1]);

	/* range between 0 and pi */
	angle = range_it (angle, 0, 3.1415926536);

	/* ((pi - angle) / pi * 2 - 1) * 0x1000 */
	lastwheel[node] = 4096.0 - angle * 2607.6;

#if 0
		{
		char buf[60];

		sprintf (buf, "%6.3f %6.3f %6.3f %6.3f  ",
				 (double)pos[1], (double)pos[0],
				 (double)angle, (double)lastwheel[node]);
		txt_str (10, 40+node, buf, WHT_PALB);
		}
#endif

    m->fastin.wheel = lastwheel[node];
    m->fastin.swvel = 0;
	/*    m->fastin.wheel = -0xFFF;*/
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Decide best direction to move toward using maxpath as a guide
*         Upon exit: mpctl[].tgtpos[] -> converted to relative direction     */

void avoid_areas (S16 drone_index)
{
	S16			i, index, fwd, fwd_left, fwd_right, can_we_cheat, hint;
	F32			dist, scale, scale1, scale2, tmp2;
	F32 		rpos[3], lrpos[3], nrpos[3], cur_delta[3];
	F32			pos[3], tpos[3], own_rwr[3], dir[3], temp[3];
	F32			dir_list[MAX_LINKS][3], dir_weight[MAX_LINKS];
	F32			cur_rate[3], save_dir[3];
	char		buf[60];
    MPCTL		*cp;
	MODELDAT	*m;

	m = &model[drone_index];
    cp = &mpctl[drone_index];

	/* get real world position of maxpath guide point */
    tpos[0] = cp->tgtpos[0];
    tpos[1] = cp->tgtpos[1];
    tpos[2] = cp->tgtpos[2];

	/* get desired direction [0]=+fwd/-back, [1]=-left/+right, [2]=-up/+down */
	vecsub (tpos, m->RWR, pos);
	rwtobod (pos, rpos, &m->UV);

	/* note: before exiting, cp->tgtpos must be updated to relative coords */

	/* if just started, update last positions */
	if ((m->idt == 0) || (m->magvel < .1))
		{
		for (i=0; i<num_active_cars; i++)
			{
			index = model[i].slot;

			veccopy (model[index].RWR, last_rwr[drone_index][index]);
			last_relpos[drone_index][index][FWD_BACK] = 300;
			last_relpos[drone_index][index][LEFT_RIGHT] = 0;
			}

		model[drone_index].can_we_cheat = false;

		/* init at high speed, straight ahead */
		rpos[LEFT_RIGHT] = 0;
		cp->tgtspd = 100;

		veccopy (rpos, cp->tgtpos);

		for (i=0; i<3; i++)
			last_dir[drone_index][i] = 0;

		return;
		}

/*---------------------------------------------------------------------------*/

	/* if going too slow then maintain course and move slowly toward maxpath */
	if (m->magvel < mpath_tab[cp->mpath_index][cp->mpi].speed*.5)
		{
		for (i=0; i<num_active_cars; i++)
			{
			index = model[i].slot;

			veccopy (model[index].RWR, last_rwr[drone_index][index]);
			}

		rpos[LEFT_RIGHT] *= .2;
		cp->tgtspd = mpath_tab[cp->mpath_index][cp->mpi].speed*1.5;
		veccopy (rpos, cp->tgtpos);

		for (i=0; i<3; i++)
			last_dir[drone_index][i] = 0;

		model[drone_index].can_we_cheat = false;

		return;
		}

/*---------------------------------------------------------------------------*/

	/* very far off maxpath (backwards or to side), just try to get back on */
	if (rpos[FWD_BACK] < fabs(rpos[LEFT_RIGHT]*2))
		{
		/* just use direction vector during drone training */
		veccopy (rpos, cp->tgtpos);

		for (i=0; i<3; i++)
			last_dir[drone_index][i] = 0;

		model[drone_index].can_we_cheat = false;

		return;
		}

/*---------------------------------------------------------------------------*/

	/* get distance of raw desired direction */
	dist = magnitude (rpos);

	/* get scale factor to convert to actual attainable direction after model slice */
	scale = m->magvel * m->dt / dist;

	/* make vector in terms of how far we are actually going to move */
	scalmul (rpos, scale, dir);

	/* save raw maxpath desired direction vector */
	veccopy (dir, save_dir);

#if 0
		{
		char buf[50];
		sprintf (buf, "%8.3f %8.3f  ",
				 (double)dir[LEFT_RIGHT], (double)dir[FWD_BACK]);
		txt_str (10, 10+drone_index, buf, WHT_PALB);
		}
#endif

	/* get coords of drone in world */
	veccopy (model[drone_index].RWR, own_rwr);

	can_we_cheat = true;
	scale1 = 1;

	/* determine possible routes */
	for (i=0; i<num_active_cars; i++)
		{
		index = model[i].slot;
		if (index == drone_index)
			continue;

		m = &model[index];

		/* get rel position */
		vecsub (m->RWR, own_rwr, pos);
		rwtobod (pos, rpos, &model[drone_index].UV);

		/* get last rel position */
		veccopy (last_relpos[drone_index][index], lrpos);
		veccopy (rpos, last_relpos[drone_index][index]);

		dist = rpos[LEFT_RIGHT]*rpos[LEFT_RIGHT] + rpos[FWD_BACK]*rpos[FWD_BACK];

		/* disregard drone if out of range (300 ft) */
		if (dist > 300.0*300.0)
			continue;

		/* check if it's okay to cheat */
		if (m->drone_type == HUMAN)
			{
			if (fabs(rpos[LEFT_RIGHT])<60 && fabs(rpos[FWD_BACK])<200)
				can_we_cheat = false;
			}

		/* disregard car if it's behind you (for now; later try to block him!) */
		if (rpos[FWD_BACK] < 0)
			continue;

#if 0
		/* get delta distance relative to us */
		vecsub (lrpos, rpos, cur_rate);
#endif

#if 0
		if (drone_index == this_node)
			{
			char buf[60];
			sprintf (buf, "%8.3f:%8.3f %8.3f:%8.3f %8.3f:%8.3f  ", lrpos[FWD_BACK], lrpos[LEFT_RIGHT], rpos[FWD_BACK], rpos[LEFT_RIGHT], cur_rate[0], cur_rate[1]);
			txt_str (5, 20+index, buf, WHT_PALB);
			}
#endif

#if 0
		/* we're moving apart, we won't crash */
		if (cur_rate[0] < 0)
			continue;
#endif

		/* not in front, go for it */
		if (fabs(rpos[LEFT_RIGHT]) > 8)
			continue;

		scale2 = 1;

		if (rpos[FWD_BACK] < 20)
			scale2 = .9;
		else if (rpos[FWD_BACK] < 100)
			scale2 = 1.0 - (100-rpos[FWD_BACK])/(80/(1-.9));

		if (scale2 < scale1)
			scale1 = scale2;

#if 0
		if (drone_index == this_node)
			{
			char buf[60];
			sprintf (buf, "%7.3f %7.3f   ", scale1, scale2);
			txt_str (10, 30+index, buf, WHT_PALB);
			}
#endif

#if 0
		if (index == gThisNode)
			{
			sprintf (buf, "%8.3f %8.3f   %8.3f %8.3f  ",
					 (double)lrpos[LEFT_RIGHT], (double)lrpos[FWD_BACK],
					 (double)scale1, (double)scale2);
			txt_str (5, 20+drone_index, buf, WHT_PALB);
			}
#endif

		} /* end: determine possible routes */



#if 0
	for (i=0; i<num_active_cars; i++)
		{
		human_conflicts[i] = 0;
		drone_conflicts[i] = 0;
		}


	for (i=0; i<num_active_cars; i++)
		{
		index = model[i].slot;
		if (index == drone_index)
			continue;

		for (j=0; j<num_active_cars; i++)
			{
			index2 = model[j].slot;
			if (index2 == drone_index || index2 == index)
				continue;
			}

		}
#endif

#if 0
	if (scale1 != 1.0)
		{
		char buf[50];
		sprintf (buf, "%7.3f   ", scale1);
		txt_str (10, 30+drone_index, buf, WHT_PALB);
		}
#endif

	/* if off of maxpath by more than 14 degrees, just follow maxpath direction */
	if (fabs(dir[LEFT_RIGHT])*4 < fabs(dir[FWD_BACK]))
		{
		if (dir[LEFT_RIGHT] < 0)
			dir[LEFT_RIGHT] += (1-scale1)*8;
		else
			dir[LEFT_RIGHT] -= (1-scale1)*8;
		}

/*---------------------------------------------------------------------------*/
/*  okay: decision time.  dir is direction due to traffic. save_dir is maxpath */

	hint = mpath_tab[cp->mpath_index][cp->mpi].hints;

	/* if easy drones, modify hint */
#if 0
	if (game_car[drone_index].difficulty > 5)
		{
		if (hint != STAY_PARALLEL && hint != RESET_PARALLEL)
			hint = EASE_ON_MPATH;
		}
	else if (game_car[drone_index].difficulty > 3)
		{
		if (hint == STAY_ON_MPATH)
			hint = EASE_ON_MPATH;
		}
#endif

	/* if we have no previous data, just go straight this time */
	if (last_dir[drone_index][LEFT_RIGHT]==0 && last_dir[drone_index][FWD_BACK]==0)
		{
		dir[LEFT_RIGHT] = 0;

		/* back off of leader */
		cp->tgtspd *= 1-(1-scale1)*.3;
		}

	/* we should stay on maxpath */
	else if ((   hint == STAY_ON_MPATH)
			 || (last_dir[drone_index][LEFT_RIGHT]==0 && last_dir[drone_index][FWD_BACK]==0))
		{
		veccopy (save_dir, dir);

		/* back off of leader */
		cp->tgtspd *= 1-(1-scale1)*.3;
		}

	/* we should be getting over to maxpath */
	else if (mpath_tab[cp->mpath_index][cp->mpi].hints == GET_ON_MPATH)
		{
		/* previous directional data? */
		if (last_dir[drone_index][LEFT_RIGHT]==0 && last_dir[drone_index][FWD_BACK]==0)
			/* no, just do it */
			veccopy (save_dir, dir);

		else
			dir[LEFT_RIGHT] = save_dir[LEFT_RIGHT]*.15 + last_dir[drone_index][LEFT_RIGHT]*.85;

		/* back off of leader a little */
		cp->tgtspd *= 1-(1-scale1)*.1;
		}

	/* we should be getting over to maxpath and have respect for traffic */
	else if (mpath_tab[cp->mpath_index][cp->mpi].hints == EASE_ON_MPATH)
		{
		dir[LEFT_RIGHT] *= .4;
		}

	/* we should be parallel to maxpath */
	else if (mpath_tab[cp->mpath_index][cp->mpi].hints == STAY_PARALLEL)
		{
		dir[LEFT_RIGHT] = (save_dir[LEFT_RIGHT] - last_save_dir[drone_index][LEFT_RIGHT]);

		veccopy (last_save_dir[drone_index], save_dir);
		}

	/* we should be parallel to maxpath but allow parallel path to reset */
	else if (mpath_tab[cp->mpath_index][cp->mpi].hints == RESET_PARALLEL)
		{
		dir[LEFT_RIGHT] = (save_dir[LEFT_RIGHT] - last_save_dir[drone_index][LEFT_RIGHT]);
		}

	/* do whatever */
	else
		dir[LEFT_RIGHT] = dir[LEFT_RIGHT]*.5 + last_dir[drone_index][LEFT_RIGHT]*.5;

	model[drone_index].can_we_cheat = can_we_cheat;

	/* save new direction */
	veccopy (dir, cp->tgtpos);

	/* save for next time through */
	veccopy (dir, last_dir[drone_index]);

	/* save last raw desired position */
	veccopy (save_dir, last_save_dir[drone_index]);

#if 0
		{
		char buf[50];
		sprintf (buf, "%6.3f %6.3f ", (double)dir[LEFT_RIGHT], (double)dir[FWD_BACK]);
		txt_str (20, 30+drone_index, buf, WHT_PALB);
		}
#endif

}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Display path points from maxpath point list
*         Upon entry: mode -> Initialize = display data for first time
*                             Do_it = update path data                       */

void display_maxpath_points (S16 mode)
{
	S16  i, j, k;
	F32  pos[3];
	char buf[80];

	switch (mode)
		{
		case Initialize:
			maxpath_index = 0;

			/* clear posts from screen in case exiting from edit mode */
			edit_maxpath_points (Cleanup);
			/* fall through and update screen */

		case Do_it:
			sprintf (buf, "Current set=%d   Num points=%ld", cur_point_set, *mpath_num[cur_point_set]);
			txt_str (2, 15, buf, GRN_PAL);

			for (i=maxpath_index,j=16; j<36 && i<(S16)*mpath_num[cur_point_set]; j++,i++)
				{
				pos[0] =  mpath_tab[cur_point_set][i].pos[1];
				pos[1] = -mpath_tab[cur_point_set][i].pos[2];
				pos[2] =  mpath_tab[cur_point_set][i].pos[0];

				sprintf (buf, "%4d (%7.1f %7.1f %7.1f) %5.1f %5u %4d %1.1lx %4d %4d ",
						 i, (double)pos[0], (double)pos[1], (double)pos[2],
						 (double)mpath_tab[cur_point_set][i].speed, maxpath_dist[i],
						 maxpath_to_path[i], mpath_tab[cur_point_set][i].hints,
						 mpath_tab[cur_point_set][i].path_to_base,
						 mpath_tab[cur_point_set][i].base_to_path
						 );

				txt_str (2, j, buf, GRN_PAL);
				}
			break;
		}
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Advance to next set of path points                                   */

void fwd_maxpath_points (S16 mode)
{
	S16 i;

	clear_display();

	if (maxpath_index+20 <= *mpath_num[cur_point_set])
		maxpath_index += 20;
	else
		maxpath_index = 0;
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Move back to previous set of path points                             */

void back_maxpath_points (S16 mode)
{
	S16 i;

	clear_display();

	if (maxpath_index-20 >= 0)
		maxpath_index -= 20;
	else
		maxpath_index = (*mpath_num[cur_point_set]/20)*20;
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Link maxpath to path points and vice versa                           */

void link_maxpath_and_pathpoints (void)
{
	S16 i, j, cur_path, t_prev, t_fwd, t_back, init_flag;
	F32 val, path_dir, val_back, dx, dz, old_x, old_z;

	if (*mpathnum == 0)
		return;

	/* link maxpath to path points */
	init_flag = true;

	/* get first path point (near first checkpoint) */
	cur_path = path_index[0];

	t_back = cur_path;
	t_fwd = get_next_center (t_back, CP_Track[trackno].direction);

	/* get direction of path points */
	val = check_inside_out (path[t_fwd].pos[1]/40, path[t_fwd].pos[0]/40,
							path[t_back].pos[1]/40, path[t_back].pos[0]/40,
							path[t_fwd].pos[1]/40, path[t_fwd].pos[0]/40, 1);

	path_dir = val<0 ? -1 : 1;

	old_x = mpath[0].pos[1];
	old_z = mpath[0].pos[0];

	maxpath_dist[0] = 0;
	maxpath_to_path[0] = cur_path;

	/* go through all max path points */
	for (i=1; i<*mpathnum; i++)
		{
		dx = old_x - mpath[i].pos[1];
		dz = old_z - mpath[i].pos[0];
		old_x = mpath[i].pos[1];
		old_z = mpath[i].pos[0];

		maxpath_dist[i] = maxpath_dist[i-1] + fsqrt (dx*dx + dz*dz);

		t_back = cur_path;
		t_fwd = get_next_center (t_back, CP_Track[trackno].direction);

		val_back = check_inside_out (mpath[i].pos[1], mpath[i].pos[0],
									 path[t_back].pos[1]/40, path[t_back].pos[0]/40,
									 path[t_fwd].pos[1]/40, path[t_fwd].pos[0]/40, path_dir);

		/* first find closet path point */
		while (1)
			{
			t_prev = t_back;
			t_back = t_fwd;
			t_fwd = get_next_center (t_back, CP_Track[trackno].direction);

			val = check_inside_out (mpath[i].pos[1], mpath[i].pos[0],
									path[t_back].pos[1]/40, path[t_back].pos[0]/40,
									path[t_fwd].pos[1]/40, path[t_fwd].pos[0]/40, path_dir);

			if ((val_back >= 0) && (val < 0))
				{
				init_flag = false;
				break;
				}
			else if ((val_back < 0) && init_flag)
				break;

			val_back = val;
			}

		/* now we have a point between t_prev and t_back */
		maxpath_to_path[i] = t_prev;
		cur_path = t_prev;
		}

	/* sentinel */
/*	maxpath_dist[i] = 1e10;*/
	maxpath_dist[i] = 0x7fff;

	/*-----------------------------------------------------------------------*/
	/* link path to maxpath points */
	init_flag = true;

	/* get first max path point (usually around first checkpoint) */
	cur_path = 0;

	t_back = cur_path;
	t_fwd = NextMaxPath (t_back,0);

	/* get direction of maxpath points */
	val = check_inside_out (mpath[t_fwd].pos[1], mpath[t_fwd].pos[0],
							mpath[t_back].pos[1], mpath[t_back].pos[0],
							mpath[t_fwd].pos[1], mpath[t_fwd].pos[0], 1);

	path_dir = val<0 ? -1 : 1;

	/* go through all path points */
	/* path index [i] corresponds to path point number */
	/* and path to maxpath [i] corresponds to maxpath point number */
	for (i=0; i<num_path_points; i++)
		{
		j = path_index[i];

		t_back = cur_path;
		t_fwd = NextMaxPath (t_back,0);

		val_back = check_inside_out (path[j].pos[1]/40, path[j].pos[0]/40,
									 mpath[t_back].pos[1], mpath[t_back].pos[0],
									 mpath[t_fwd].pos[1], mpath[t_fwd].pos[0], path_dir);

		/* first find closet path point */
		while (1)
			{
			t_prev = t_back;
			t_back = t_fwd;
			t_fwd = NextMaxPath (t_back,0);

			val = check_inside_out (path[j].pos[1]/40, path[j].pos[0]/40,
									mpath[t_back].pos[1], mpath[t_back].pos[0],
									mpath[t_fwd].pos[1], mpath[t_fwd].pos[0], path_dir);

			if ((val_back >= 0) && (val < 0))
				{
				init_flag = false;
				break;
				}
			else if ((val_back < 0) && init_flag)
				break;

			val_back = val;
			}

		/* now we have a point between t_prev and t_back */
		path_to_maxpath[i] = t_prev;
		cur_path = t_prev;
		}

	lap_loop_distance =  maxpath_dist[path_to_maxpath[path_dist_index[number_checkpoints]]]
                       - maxpath_dist[path_to_maxpath[path_dist_index[lap_loop_index]]];
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Check if a point is inside or outside of line passing through coord1
*         and normal to coord2
*       Upon entry: pos -> point to check whether inside or outside
*                   coord1 -> point on line to test against
*                   coord2 -> normal to line
*                   direction -> mult. by -1 or 1 for direction correction   */

F32 check_inside_out (F32 pos_x, F32 pos_y,
					  F32 coord1_x, F32 coord1_y,
					  F32 coord2_x, F32 coord2_y,
					  F32 direction)
{
	F32 dx, dy, x1, y1, x2, y2, c;

	/* find two points on line */
	dx = coord1_x - coord2_x;
	dy = coord1_y - coord2_y;

	x1 = coord1_x - dy;
	y1 = coord1_y + dx;
	x2 = coord1_x + dy;
	y2 = coord1_y - dx;

	/* generate value for 0 = (x1-x2)y + (y1-y2)x - ((x1-x2)y1 + (y1-y2)x1) */
	dx = x1 - x2;
	dy = y1 - y2;
	c = y1 * dx - x1 * dy;

	/* now return inside/out value */
	return ((pos_x * dy - pos_y * dx + c) * direction);
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Find maxpath distance in list of distances
*         Upon entry: index -> start looking at this index
*                     dist -> distance to look for
*         Upon exit: returned -> index of maxpath distance                   */

S16 find_maxpath_dist (S16 index, F32 dist)
{
	if (index > *mpathnum)
		return (*mpathnum - 1);

	if (dist > maxpath_dist[*mpathnum-1])
		return (*mpathnum - 1);

	while (dist > maxpath_dist[index])
		index++;

	return (index);
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Set maxpath index
*         Upon entry: m -> point to model version of data
*                     index -> maxpath index to use                          */

void set_maxpath_index (MODELDAT *m, S16 index)
{
	S16   nmpi;
	S16   node = m->net_node;
    MPCTL *cp = &mpctl[node];

	/* check bounds of index in base path */
	if (index > mpheader_tab[0]->lap_end)
		index -= mpheader_tab[0]->num_in_lap;

	/* get path point of target path */
	nmpi = mpath_tab[cp->mpath_index][index].base_to_path;

	if (nmpi == -1)
		{
		cp->mpi = index;
		cp->mpath_index = 0;
		}
	else
		cp->mpi = nmpi;

    cp->xrel = 20;
    cp->yrel = 0;
	cp->interval_time = IRQTIME;
	cp->new_mpi = -1;

	cp = &game_car[m->net_node].mpath;
	cp->mpi = index;
    cp->xrel = 20;
    cp->yrel = 0;
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Initialize maxpath speed data                                        */

void init_hint_table()
{
	S16  i, j;

	drone_done_count = 0;
	target_drone = 0;
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Save/make changes to maxpath data due to race results                */

void update_maxpath_data()
{
	drone_done_count = 0;
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Display/update drone track times after race
*         Upon entry: node -> index of drone that completed race
*                     millisecs -> time it took drone to finish race         */

void update_drone_time (S16 node, S32 millisecs)
{
	if (++drone_done_count == 3)
		set_game_time_to_one_second();
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Write the current maxpath data back to the hard drive                */

void write_cur_path_to_hd (S16 mode)
{
	U32  len;

	len = (U32)(MAXMPATH*MAX_MPATHS) * sizeof(MPATH);
	WrFile (get_path(trackno), "dpath.rom", len, (U32 *)gMaxPath);
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Write the current maxpath data to save area                          */

void write_drone_path_to_hd (S16 mode)
{
	U32  len;

	len = (U32)(MAXMPATH*MAX_MPATHS) * sizeof(MPATH);

	write_file_to_hd ("dpath1.rom", len, (U32 *)gMaxPath);
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Edit path points from maxpath point list
*         Upon entry: mode -> Initialize = set posts and init view
*                             Do_it = update posts/view                      */

void edit_maxpath_points (S16 mode)
{
	S16  i, j, k;
	F32  pos[3];
	char buf[80], *string1, *string2;

	switch (mode)
		{
		case Initialize:
			update_mpath_edit (Initialize);

			/* fall through */

		case Do_it:
			string1 = string2 = "";

			sprintf (buf, "Editing set: %d     Priority: %d  ",
					 cur_point_set,
					 mpheader_tab[cur_point_set]->priority);
			txt_str (2, 27, buf, CYN_PAL);

			sprintf (buf, "Lap_start=%3d  end=%3d  len=%3d  ",
					 mpheader_tab[cur_point_set]->lap_start,
					 mpheader_tab[cur_point_set]->lap_end,
					 mpheader_tab[cur_point_set]->num_in_lap);
			txt_str (2, 28, buf, CYN_PAL);

			switch (cur_hint_type)
				{
				case STAY_WITHIN_WIDTH:
					string2 = "Stay within width       ";
					break;
				case GET_ON_MPATH:
					string2 = "Get on maxpath          ";
					break;
				case EASE_ON_MPATH:
					string2 = "Ease on maxpath         ";
					break;
				case STAY_ON_MPATH:
					string2 = "Stay on maxpath         ";
					break;
				case STAY_PARALLEL:
					string2 = "Stay parallel to maxpath";
					break;
				case RESET_PARALLEL:
					string2 = "Reset parallel path     ";
					break;
				}

			sprintf (buf, "Current hint: %s", string2);
			txt_str (2, 29, buf, CYN_PAL);

			i = cur_mpath_point_s16;
			if (i-1 >= 0)
				{
				sprintf (buf, "Previous point/speed/hint = %4.4d  %6.2f  %s", i-1, (double)mpath_tab[cur_point_set][i-1].speed, hint_types[range_it(mpath_tab[cur_point_set][i-1].hints,0,NUM_HINT_TYPES-1)]);
				txt_str (2, 33, buf, GRN_PAL);
				}
			else
				txt_str (2, 33, "                                                                   ", GRN_PAL);

			sprintf (buf, "Current point/speed/hint  = %4.4d  %6.2f  %s", i, (double)mpath_tab[cur_point_set][i].speed, hint_types[range_it(mpath_tab[cur_point_set][i].hints,0,NUM_HINT_TYPES-1)]);
			txt_str (2, 32, buf, GRN_PALB);

			if (i+1 < *mpath_num[cur_point_set])
				{
				sprintf (buf, "Next point/speed/hint     = %4.4d  %6.2f  %s", i+1, (double)mpath_tab[cur_point_set][i+1].speed, hint_types[range_it(mpath_tab[cur_point_set][i+1].hints,0,NUM_HINT_TYPES-1)]);
				txt_str (2, 31, buf, GRN_PAL);
				}
			else
				txt_str (2, 31, "                                                                   ", GRN_PAL);

			break;

		case Cleanup:

			update_mpath_edit (Cleanup);
			break;
		}
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Advance to next path point                                           */

void next_maxpath_point (S16 mode)
{
	S16 i, j;
	F32 pos[3];

	if (cur_mpath_point_s16 < *mpath_num[cur_point_set]-1)
		cur_mpath_point_s16++;

	cur_mpath_point_f32 = cur_mpath_point_s16;

	update_posts();
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Move to previous path point                                          */

void prev_maxpath_point (S16 mode)
{
	S16 i, j;
	F32 pos[3];

	if (cur_mpath_point_s16 > 0)
		cur_mpath_point_s16--;

	cur_mpath_point_f32 = cur_mpath_point_s16;

	update_posts();
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Increment path priority                                              */

void inc_path_priority (S16 mode)
{
	if (++mpheader_tab[cur_point_set]->priority > 10)
		mpheader_tab[cur_point_set]->priority = 0;
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Decrement path priority                                              */

void dec_path_priority (S16 mode)
{
	if (--mpheader_tab[cur_point_set]->priority < 0)
		mpheader_tab[cur_point_set]->priority = 10;
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Move all posts to new locations                                      */

void update_posts()
{
	S16 i, j;
	F32 pos[3];

	for (i=0; i<NUM_POSTS; i++)
		{
		j = cur_mpath_point_s16 + i - (NUM_POSTS>>2) + 2;

		if (i == NUM_POSTS-2)
			{
			set_post (pos, marker_a, 5);
			}
		else if (i == NUM_POSTS-1)
			{
			set_post (pos, marker_b, 5);
			}
		else if (j<0 || j >= *mpath_num[cur_point_set])
			{
			pos[0] = pos[1] = pos[2] = 0;
			}
		else
			{
			set_post (pos, j, 3);
			}

		MBOX_UpdateObject (post_obj_index[i], pos, unit_uvs);
		}
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Save post position according to current point set
*         Upon entry: pos -> save area for post position
*                     index -> maxpath index to grab
*                     height -> height offset to place post                  */

void set_post (F32 *pos, S16 index, S16 height)
{
	pos[0] =  mpath_tab[marker_point_set][index].pos[1];
	pos[1] = -mpath_tab[marker_point_set][index].pos[2] + height;
	pos[2] =  mpath_tab[marker_point_set][index].pos[0];
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Set/clear curve mode                                                 */

void curve_maxpath_point (S16 mode)
{
	S16 i;

	if (mpath_change_mode)
		{
		mpath_change_mode = 0;

		for (i=0; i<*mpath_num[cur_point_set]; i++)
			{
			mpath_save[i].pos[0] = mpath_tab[cur_point_set][i].pos[0];
			mpath_save[i].pos[1] = mpath_tab[cur_point_set][i].pos[1];
			mpath_save[i].pos[2] = mpath_tab[cur_point_set][i].pos[2];
			mpath_save[i].speed = mpath_tab[cur_point_set][i].speed;
			}
		}
	else
		{
		mpFlyDist = 0;
		speed_delta = 0;
		mpath_change_mode = 1;
		}

	set_direction_vector();
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Set/clear move mode                                                  */

void move_maxpath_point (S16 mode)
{
	S16 i;

	if (mpath_change_mode)
		{
		mpath_change_mode = 0;

		for (i=0; i<*mpath_num[cur_point_set]; i++)
			{
			mpath_save[i].pos[0] = mpath_tab[cur_point_set][i].pos[0];
			mpath_save[i].pos[1] = mpath_tab[cur_point_set][i].pos[1];
			mpath_save[i].pos[2] = mpath_tab[cur_point_set][i].pos[2];
			mpath_save[i].speed = mpath_tab[cur_point_set][i].speed;
			}
		}
	else
		{
		mpFlyDist = 0;
		speed_delta = 0;
		mpath_change_mode = 2;
		}

	set_direction_vector();
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Cancel current move/curve operation                                  */

void cancel_move_curve (S16 mode)
{
	S16    i;

	mpath_change_mode = 0;

	for (i=0; i<*mpath_num[cur_point_set]; i++)
		mpath_tab[cur_point_set][i] = mpath_save[i];

	set_direction_vector();
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Copy maxpath section from Marker A-B to current point                */

void copy_mpath_section (S16 mode)
{
	S16    i, start, range;
	S32    state;
	MPATH  *t_mpath;

#if 0
	state = prc_set_ipl (INTS_OFF);
#endif

	range = abs (marker_a - marker_b) + 1;

	if (range+cur_mpath_point_s16 >= MAXMPATH)
		range = MAXMPATH - cur_mpath_point_s16 - 1;

	if (marker_a < marker_b)
		start = marker_a;
	else
		start = marker_b;

	if (marker_point_set != cur_point_set)
		t_mpath = mpath_tab[marker_point_set];
	else
		t_mpath = mpath_save;

	for (i=0; i<range; i++)
		{
		mpath_save[cur_mpath_point_s16+i] = t_mpath[start+i];
		mpath_tab[cur_point_set][cur_mpath_point_s16+i] = t_mpath[start+i];
		}

#if 0
	prc_set_ipl (state);
#endif
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Insert maxpath section from Marker A-B to current point              */

void insert_mpath_section (S16 mode)
{
	S16    i, start, range;
	S32    state;
	MPATH  *t_mpath;

	state = prc_set_ipl (INTS_OFF);

	range = abs (marker_a - marker_b) + 1;

	if (marker_a < marker_b)
		start = marker_a;
	else
		start = marker_b;

	if (*mpath_num[cur_point_set] + range >= MAXMPATH)
		range = MAXMPATH - *mpath_num[cur_point_set] - 1;

	if (range <= 0)
		return;

	/* select source mpath set */
	if (marker_point_set != cur_point_set)
		t_mpath = mpath_tab[marker_point_set];
	else
		t_mpath = mpath_save;

	for (i=*mpath_num[cur_point_set]-1; i>=cur_mpath_point_s16; i--)
		{
		mpath_tab[cur_point_set][i+range] = mpath_tab[cur_point_set][i];
		}

	*mpath_num[cur_point_set] += range;

	for (i=0; i<range; i++)
		{
		mpath_tab[cur_point_set][cur_mpath_point_s16+i] = t_mpath[start+i];
		}

	for (i=0; i<*mpath_num[cur_point_set]; i++)
		mpath_save[i] = mpath_tab[cur_point_set][i];

	prc_set_ipl (state);
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Straighten maxpath section from Marker A to Marker B                 */

void straighten_maxpath (S16 mode)
{
	S16    i, start, end, range, scale;
	F32    delta_x, delta_y;

	/* exit if nothing to do */
	if (abs (marker_a-marker_b) < 2)
		return;

	if (marker_a < marker_b)
		{
		start = marker_a;
		end = marker_b;
		}
	else
		{
		start = marker_b;
		end = marker_a;
		}

	range = end - start;

	delta_x = (mpath_tab[cur_point_set][end].pos[0] - mpath_tab[cur_point_set][start].pos[0]) / range;
	delta_y = (mpath_tab[cur_point_set][end].pos[1] - mpath_tab[cur_point_set][start].pos[1]) / range;

	for (i=start+1,scale=1; i<end; i++,scale++)
		{
		mpath_save[i].pos[0] = mpath_save[start].pos[0] + delta_x*scale;
		mpath_save[i].pos[1] = mpath_save[start].pos[1] + delta_y*scale;

		mpath_tab[cur_point_set][i].pos[0] = mpath_save[i].pos[0];
		mpath_tab[cur_point_set][i].pos[1] = mpath_save[i].pos[1];
		}
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Delete maxpath section from Marker A to Marker B                     */

void delete_mpath_section (S16 mode)
{
	S16    i, start, end, range;
	S32    state;

	state = prc_set_ipl (INTS_OFF);

	if (marker_a < marker_b)
		{
		start = marker_a;
		end = marker_b+1;
		}
	else
		{
		start = marker_b;
		end = marker_a+1;
		}

	range = abs (*mpath_num[cur_point_set] - end);

	for (i=0; i<range; i++)
		{
		mpath_save[start+i] = mpath_save[end+i];
		mpath_tab[cur_point_set][start+i] = mpath_save[end+i];
		}

	*mpath_num[cur_point_set] -= end - start;

	prc_set_ipl (state);
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Set direction vector                                                 */

void set_direction_vector()
{
	S16 point_a, point_b;
	F32 delta1[3];

	if (cur_mpath_point_s16 == 0)
		point_a = 0;
	else
		point_a = cur_mpath_point_s16 - 1;

	if (cur_mpath_point_s16 >= *mpath_num[cur_point_set]-1)
		point_b = *mpath_num[cur_point_set] - 1;
	else
		point_b = cur_mpath_point_s16 + 1;

	delta1[0] = mpath_tab[cur_point_set][point_a].pos[0] - mpath_tab[cur_point_set][point_b].pos[0];
	delta1[1] = mpath_tab[cur_point_set][point_a].pos[1] - mpath_tab[cur_point_set][point_b].pos[1];
	delta1[2] = 0;

	direction (delta1, delta1);

	change_vector[0] = -delta1[1];
	change_vector[1] =  delta1[0];

	switch (mpath_change_mode)
		{
		case 0:
			txt_str (2, 34, "       ", RED_PAL);
			break;

		case 1:
			txt_str (2, 34, "Curving", RED_PAL);
			break;

		case 2:
			txt_str (2, 34, "Moving ", RED_PAL);
			break;
		}
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Set marker A to current maxpath point                                */

void set_marker_a (S16 mode)
{
	marker_a = cur_mpath_point_s16;
	marker_point_set = cur_point_set;

	update_posts();
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Set marker B to current maxpath point                                */

void set_marker_b (S16 mode)
{
	marker_b = cur_mpath_point_s16;
	marker_point_set = cur_point_set;

	update_posts();
}

#define CHECK_LINK 1

#if CHECK_LINK
/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Check link data                                          */

BOOL check_link_ok (void)
{
	S32 row = 20;
	S32 col = 37;
	S16 i,j;
	MPATH  *t_mpath;
	S32 len;
	F32 dx,dy,dz,dsq;

	/*clear link status line.*/
	DBG_printf(row,col,RED_PAL,"                                     ");


	/* check base set loop. */
	if(mpheader_tab[0]->num_points >= MAXMPATH)
		{
		mpheader_tab[0]->num_points = MAXMPATH;
		DBG_printf(row,col,RED_PAL,
			"BaseSet Error: Too many points (clipped).");
		return 0;
		}
	if( (mpheader_tab[0]->lap_start >= mpheader_tab[0]->num_points) ||
	    (mpheader_tab[0]->lap_end >= mpheader_tab[0]->num_points) ||
	    (mpheader_tab[0]->lap_start >= mpheader_tab[0]->lap_end) )
		{
		DBG_printf(row,col,RED_PAL,"BaseSet Error: Bad Loop points.");
		return 0;
		}

	for (i=0; i<num_mpaths; i++)
		{
		t_mpath = mpath_tab[mpaths[i]];
		len = *mpath_num[mpaths[i]];

		if(len >= MAXMPATH)
			{
			*mpath_num[mpaths[i]] = MAXMPATH;
			DBG_printf(row,col,RED_PAL,"Set%d Error: Too many points (clipped).",mpaths[i]);
			return 0;
			}

		/* check all inervals. */
		for (j=1; j<len; j++)
			{
			dx = t_mpath[j].pos[0] - t_mpath[j-1].pos[0];
			dy = t_mpath[j].pos[1] - t_mpath[j-1].pos[1];
			dz = t_mpath[j].pos[2] - t_mpath[j-1].pos[2];
			dsq = dx*dx + dy*dy+ dz*dz;

			if((dsq < 1.0) || (dsq > (50.0*50.0)))
				{
				DBG_printf(row,col,RED_PAL,
					"Set%d Error: Funky points at %d. %f",mpaths[i],j,dsq);
				return 0;
				}
			}


		}
	return 1;
}
#endif

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Create maxpath link tables                                           */

void create_maxpath_links (S16 mode)
{
	S16    i, j, k, len, closest, last_index;
	F32    dist, max_dist, pos_x, pos_y, pos_z, delta_x, delta_y, delta_z;
	MPATH  *t_mpath;

	num_mpaths = 0;

	for (i=0; i<MAX_MPATHS; i++)
		{
		if (*mpath_active[i])
			mpaths[num_mpaths++] = i;
		}


#if CHECK_LINK
	if( !check_link_ok() )
		return;
#endif


	/* match points from each path back to base path */
	for (j=0; j<num_mpaths; j++)
		{
		t_mpath = mpath_tab[mpaths[j]];
		len = *mpath_num[mpaths[j]];

		last_index = 0;

		for (k=0; k<len; k++)
			{
WDOG = 0;
			max_dist = 99999999;
			closest = -1;

			pos_x = t_mpath[k].pos[0];
			pos_y = t_mpath[k].pos[1];
			pos_z = t_mpath[k].pos[2];

			/* go through all points */
			for (i=0; i<mpheader_tab[0]->lap_end; i++)
				{
				delta_x = mpath[i].pos[0] - pos_x;
				delta_y = mpath[i].pos[1] - pos_y;
				delta_z = mpath[i].pos[2] - pos_z;

				dist = (delta_x * delta_x) + (delta_y * delta_y) + (delta_z * delta_z);

				if (dist < max_dist && dist < 50*50 && (last_index==-1 || abs(i-last_index) < 5))
					{
					max_dist = dist;
					closest = i;
					}
				}

			t_mpath[k].path_to_base = closest;
			last_index = closest;
			}


		last_index = 0;

		/* go through all points */
		for (i=0; i<*mpathnum; i++)
			{
			max_dist = 99999999;
			closest = -1;

			pos_x = mpath[i].pos[0];
			pos_y = mpath[i].pos[1];
			pos_z = mpath[i].pos[2];

			for (k=0; k<len; k++)
				{
				delta_x = t_mpath[k].pos[0] - pos_x;
				delta_y = t_mpath[k].pos[1] - pos_y;
				delta_z = t_mpath[k].pos[2] - pos_z;

				dist = (delta_x * delta_x) + (delta_y * delta_y) + (delta_z * delta_z);

				if (dist < max_dist && dist < 50*50 && (last_index==-1 || abs(k-last_index) < 5))
					{
					max_dist = dist;
					closest = k;
					}
				}

			t_mpath[i].base_to_path = closest;
			last_index = closest;
			}
		}
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Set loop start, loop end and calculate loop size for current set     */

void set_loop_data (S16 mode)
{
	S16  start, end;

	if (marker_a < marker_b)
		{
		start = marker_a;
		end = marker_b;
		}
	else
		{
		start = marker_b;
		end = marker_a;
		}

	mpheader_tab[cur_point_set]->lap_start = start;
	mpheader_tab[cur_point_set]->lap_end = end;
	mpheader_tab[cur_point_set]->num_in_lap = end - start + 1;
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Edit maxpath hint data (just redirect to edit maxpath points)        */

void edit_hint_data (S16 mode)
{
	edit_maxpath_points (Do_it);
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Deactivate current maxpath set                                       */

void deactivate_mpath_set (S16 mode)
{
	if (cur_point_set == 0)
		txt_str (2, 26, "Can't De-activate set 0", RED_PAL);
	else
		{
		char buf[50];

		*mpath_active[cur_point_set] = 0;
		sprintf (buf, "Set %d De-activated     ", cur_point_set);
		txt_str (2, 26, buf, CYN_PAL);
		cur_point_set = 0;
		update_mpath_edit (Initialize);
		}
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Activate maxpath set 1                                               */

void activate_mpath_set1 (S16 mode)
{
	activate_mpath_set (1);
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Activate maxpath set 2                                               */

void activate_mpath_set2 (S16 mode)
{
	activate_mpath_set (2);
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Activate maxpath set 3                                               */

void activate_mpath_set3 (S16 mode)
{
	activate_mpath_set (3);
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Activate maxpath set 4                                               */

void activate_mpath_set4 (S16 mode)
{
	activate_mpath_set (4);
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Activate maxpath set                                                */

void activate_mpath_set5 (S16 mode)
{
	activate_mpath_set (5);
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Activate maxpath set 6                                               */

void activate_mpath_set6 (S16 mode)
{
	activate_mpath_set (6);
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Activate maxpath set 7                                               */

void activate_mpath_set7 (S16 mode)
{
	activate_mpath_set (7);
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Activate maxpath set                                                 */

void activate_mpath_set (S16 set)
{
	char buf[50];

	*mpath_active[set] = 1;
	sprintf (buf, "Set %d activated       ", set);
	txt_str (2, 26, buf, CYN_PAL);
	cur_point_set = set;
	update_mpath_edit (Initialize);
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Select maxpath base set                                              */

void select_mpath_base (S16 mode)
{
	cur_point_set = 0;
	txt_str (2, 26, "               ", CYN_PAL);
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Select maxpath points to edit                                        */

void select_mpath_set1 (S16 mode) { select_mpath_set (1); }
void select_mpath_set2 (S16 mode) {	select_mpath_set (2); }
void select_mpath_set3 (S16 mode) { select_mpath_set (3); }
void select_mpath_set4 (S16 mode) { select_mpath_set (4); }
void select_mpath_set5 (S16 mode) { select_mpath_set (5); }
void select_mpath_set6 (S16 mode) { select_mpath_set (6); }
void select_mpath_set7 (S16 mode) { select_mpath_set (7); }

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Select maxpath points to edit                                        */

void select_mpath_set (S16 set)
{
	char buf[50];

	if (*mpath_active[set] == 0)
		{
		sprintf (buf, "Set %d not valid", set);
		txt_str (2, 26, buf, RED_PAL);
		}
	else
		{
		txt_str (2, 26, "               ", CYN_PAL);
		cur_point_set = set;
		update_mpath_edit (Initialize);
		}
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Copy from path to current path                                       */

void copy_from_base (S16 mode) { copy_from_path (0); }
void copy_from_set1 (S16 mode) { copy_from_path (1); }
void copy_from_set2 (S16 mode) { copy_from_path (2); }
void copy_from_set3 (S16 mode) { copy_from_path (3); }
void copy_from_set4 (S16 mode) { copy_from_path (4); }
void copy_from_set5 (S16 mode) { copy_from_path (5); }
void copy_from_set6 (S16 mode) { copy_from_path (6); }
void copy_from_set7 (S16 mode) { copy_from_path (7); }

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Copy from path to current path
*         Upon entry: path_num -> path to copy from                          */

void copy_from_path (S16 path_num)
{
	S16 i;

	for (i=0; i<MAXMPATH; i++)
		mpath_tab[cur_point_set][i] = mpath_tab[path_num][i];

	*mpheader_tab[cur_point_set] = *mpheader_tab[path_num];
}

void stay_within_width1 (S16 mode) { mpath_tab[cur_point_set][cur_mpath_point_s16].hints = cur_hint_type = STAY_WITHIN_WIDTH; }
void get_on_maxpath1 (S16 mode) { mpath_tab[cur_point_set][cur_mpath_point_s16].hints = cur_hint_type = GET_ON_MPATH; }
void ease_on_maxpath1 (S16 mode) { mpath_tab[cur_point_set][cur_mpath_point_s16].hints = cur_hint_type = EASE_ON_MPATH; }
void stay_on_maxpath1 (S16 mode) { mpath_tab[cur_point_set][cur_mpath_point_s16].hints = cur_hint_type = STAY_ON_MPATH; }
void stay_parallel_to_maxpath1 (S16 mode) { mpath_tab[cur_point_set][cur_mpath_point_s16].hints = cur_hint_type = STAY_PARALLEL; }
void reset_parallel1 (S16 mode) { mpath_tab[cur_point_set][cur_mpath_point_s16].hints = cur_hint_type = RESET_PARALLEL; }

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Stay within maxpath width                                            */

void stay_within_width2 (S16 mode)
{
	cur_hint_type = STAY_WITHIN_WIDTH;
	save_new_hint();
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Try to get back to maxpath                                           */

void get_on_maxpath2 (S16 mode)
{
	cur_hint_type = GET_ON_MPATH;
	save_new_hint();
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Try to ease back to maxpath                                          */

void ease_on_maxpath2 (S16 mode)
{
	cur_hint_type = EASE_ON_MPATH;
	save_new_hint();
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Stay on maxpath                                                      */

void stay_on_maxpath2 (S16 mode)
{
	cur_hint_type = STAY_ON_MPATH;
	save_new_hint();
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Stay parallel to maxpath                                             */

void stay_parallel_to_maxpath2 (S16 mode)
{
	cur_hint_type = STAY_PARALLEL;
	save_new_hint();
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Reset data for stay on parallel mode                                 */

void reset_parallel2 (S16 mode)
{
	cur_hint_type = RESET_PARALLEL;
	save_new_hint();
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Stay within maxpath width                                            */

void save_new_hint (void)
{
	S16    i, start, end;

	if (marker_a < marker_b)
		{
		start = marker_a;
		end = marker_b;
		}
	else
		{
		start = marker_b;
		end = marker_a;
		}

	for (i=start; i<=end; i++)
		mpath_tab[cur_point_set][i].hints = cur_hint_type;
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Deal with posts and call routine to get new data from controls
*         Upon entry: mode -> Allocate   = create posts objects and hide
*                             Initialize = show posts and put time on clock
*                             Do_it      = pass through to controls routine
*                             Cleanup    = hide posts                        */

void update_mpath_edit (S16 mode)
{
	S16 i;
	F32 pos[3];

	switch (mode)
		{
		case Allocate:
			mpath_edit = 0;

			marker_a, marker_b = 0;

			cur_mpath_point_s16 = 0;
			cur_mpath_point_f32 = 0;

			pos[0] = pos[1] = pos[2] = 0;

			/* create five posts and make them invisible */
			for (i=0; i<NUM_POSTS; i++)
				{
				post_obj_index[i] = MBOX_NewObject( gObjList[i==3 ? OBJ_GREENPOST : (i>=NUM_POSTS-2 ? OBJ_BLUEPOST : OBJ_REDPOST)], pos, unit_uvs, -1, 0);

				MBOX_HideObject (post_obj_index[i], HIDE_ALL);
				}
			break;


		case Initialize:
			SetCountdownTimer (800 * ONE_SEC);
			cur_weight_drone = 0;
			weight_index = 0;

			for (i=0; i<*mpath_num[cur_point_set]; i++)
				mpath_save[i] = mpath_tab[cur_point_set][i];

			update_posts();

			for (i=0; i<NUM_POSTS; i++)
				MBOX_ShowObject (post_obj_index[i], SHOW_ALL);

			break;


		case Do_it:
			/* use current mpath point for car position */
			break;


		case Cleanup:
			for (i=0; i<NUM_POSTS; i++)
				MBOX_HideObject (post_obj_index[i], HIDE_ALL);

			break;
		}

	mp_fly_around (mode);
}


/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Rotate around max path post (courtesy of A. Gray)
*         Upon entry: mode -> Initialize = first time setup
*                             Do_it = update uvs due to steering
*                             Cleanup = exit edit mode and drop car          */

void mp_fly_around (S16 mode)
{
	MODELDAT  *m = &model[gThisNode];
	F32       curflytime, delflytime, delta, speed_delta1, speed_delta2;
	S32       i, j, index, index_a, index_b, range, center;
	S16	      point_a, point_b;
	F32       delta1[3], scale;

	switch (mode)
		{
		case Initialize:

			/* tell model task */
			mpath_edit = true;

			mpFlyTime = ZCLOCK;
			mpFlySpeed = 0.0;
			mpFlyDist = 0.0;
			speed_delta = 0;
			for(i=0;i<3;i++)
				for(j=0;j<3;j++)
					mpFlyUvs[i][j] = game_car[gThisNode].dr_uvs[i][j];
			model[gThisNode].resurrect.moving_state = 0;

			break;


		case Cleanup:

			MOVMEM((char *)(m->UV.fpuvs), (char *)(m->resurrect.uvs), sizeof(F32)*3*3);

			for (i=0; i<3; i++)
				m->resurrect.pos[i] = m->RWR[i];

			model[gThisNode].resurrect.moving_state = -2;

			mpath_change_mode = false;

			/* tell model task */
			mpath_edit = false;
			break;


		case Do_it:

			curflytime = ZCLOCK;
			delflytime = curflytime - mpFlyTime;	/*seconds*/
			mpFlyTime = curflytime;

			/*Get iteration time to nearest msec*/
			i = (S32)((delflytime*1000.0) + 0.5);
			delflytime = (F32)i * 0.001;

			/*update uvs*/
			if((gSWpos < -0x10) || (gSWpos > 0x10))
				mpFlyYaw = 6.2830 * delflytime * (F32)gSWpos * 0.000244;
			else
				mpFlyYaw = 0.0;
			WYawUV(mpFlyYaw, mpFlyUvs);

			/*update pos*/
			if(gGasVal > 0.01)
				mpFlySpeed = 400.0 * delflytime * gGasVal;
			else if(brake > 0x8)
				mpFlySpeed = -400.0 * delflytime * (F32)brake * 0.000244;
			else mpFlySpeed = 0.0;

			if (gGasVal>0.5 || brake>0x800)
				mpFlySpeed *= 3;

			ReadShifter();
			mpFlySpeed *= man_gear;

			/*update speed*/
			if (m_levels & SW_COINL)
				speed_delta1 = -0.1;
			else if (m_levels & SW_COINR)
				speed_delta1 = 0.1;
			else
				speed_delta1 = 0;

			switch (mpath_change_mode)
				{
				case 0:
					/* update index mode */
					cur_mpath_point_f32 += mpFlySpeed*.05;

					if (cur_mpath_point_f32 > *mpath_num[cur_point_set]-1)
						cur_mpath_point_f32 = *mpath_num[cur_point_set] - 1;

					if (cur_mpath_point_f32 < 0)
						cur_mpath_point_f32 = 0;

					index = cur_mpath_point_s16 = cur_mpath_point_f32;

					mpath_tab[cur_point_set][index].speed = mpath_tab[cur_point_set][index].speed+speed_delta1;
					break;

					
				case 1:
					/* curve path */
					if (marker_a < marker_b)
						{
						index_a = marker_a;
						index_b = marker_b;
						}
					else
						{
						index_a = marker_b;
						index_b = marker_a;
						}

					speed_delta += speed_delta1;
					mpFlyDist += mpFlySpeed * .04;
					range = ((index_b - index_a + 1) >> 1) + 1;
					center = (index_a + index_b) >> 1;

					for (index=index_a; index<=index_b; index++)
						{
						if (index<0 || index>*mpath_num[cur_point_set]-1)
							continue;

						j = index - center;

						scale = fsin(((F32)range-abs(j))/range*3.1415926/2);
						delta = mpFlyDist * scale;
						speed_delta2 = speed_delta * scale;

						if (index == 0)
							point_a = 0;
						else
							point_a = index - 1;

						if (index >= *mpath_num[cur_point_set]-1)
							point_b = *mpath_num[cur_point_set] - 1;
						else
							point_b = index + 1;

						delta1[0] = mpath_save[point_a].pos[0] - mpath_save[point_b].pos[0];
						delta1[1] = mpath_save[point_a].pos[1] - mpath_save[point_b].pos[1];
						delta1[2] = 0;

						direction (delta1, delta1);

						change_vector[0] = -delta1[1] * delta;
						change_vector[1] =  delta1[0] * delta;

						mpath_tab[cur_point_set][index].pos[0] = mpath_save[index].pos[0]+change_vector[0];
						mpath_tab[cur_point_set][index].pos[1] = mpath_save[index].pos[1]+change_vector[1];

						mpath_tab[cur_point_set][index].speed = mpath_save[index].speed+speed_delta2;
						}
					break;

				case 2:
					/* move path */
					if (marker_a < marker_b)
						{
						index_a = marker_a;
						index_b = marker_b;
						}
					else
						{
						index_a = marker_b;
						index_b = marker_a;
						}

					center = (index_a + index_b) >> 1;
					speed_delta += speed_delta1;
					mpFlyDist += mpFlySpeed * .04;

					for (index=index_a; index<=index_b; index++)
						{
						if (index<0 || index>*mpath_num[cur_point_set]-1)
							continue;

						j = index - center;

						mpath_tab[cur_point_set][index].pos[0] = mpath_save[index].pos[0]+change_vector[0]*mpFlyDist;
						mpath_tab[cur_point_set][index].pos[1] = mpath_save[index].pos[1]+change_vector[1]*mpFlyDist;

						mpath_tab[cur_point_set][index].speed = mpath_save[index].speed+speed_delta;
						}
				}

			update_posts();

			m->RWR[XCOMP] = mpath_tab[cur_point_set][cur_mpath_point_s16].pos[0];
			m->RWR[YCOMP] = mpath_tab[cur_point_set][cur_mpath_point_s16].pos[1];
			m->RWR[ZCOMP] = mpath_tab[cur_point_set][cur_mpath_point_s16].pos[2];

			/* update uvs */
			matcopyD2M(mpFlyUvs, m->UV.fpuvs);
			makesuvs(&m->UV);

			break;
		}
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Get current hint that car is using on maxpath                        */

char *get_cur_hint (S16 index)
{
	S16         i;
    MPCTL *cp = &mpctl[index];

	i = mpath_tab[cp->mpath_index][cp->mpi].hints;

	if (i > NUM_HINT_TYPES)
		i = NUM_HINT_TYPES;

	return ((char *)hint_types[i]);
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Edit weight table
*         Upon entry: mode -> Initialize = init view
*                             Do_it = update view                            */

void edit_weight_table (S16 mode)
{
	F32       *weight_ptr, dir;
	MPWEIGHT  *wt;
	char      buf[80];

	switch (mode)
		{
		case Initialize:
			/* fall through */

		case Do_it:
			wt = &weight_table[trackno][cur_weight_drone];

			sprintf (buf, "Editing drone:         %d ", cur_weight_drone);
			txt_str (2, 27, buf, CYN_PAL);

			sprintf (buf, "Distance weight:     %6.2f ", (double)wt->distance_weight);
			txt_str (2, 29, buf, weight_index==1?RED_PAL:CYN_PAL);

			sprintf (buf, "Direction weight:    %6.2f ", (double)wt->direction_weight);
			txt_str (2, 30, buf, weight_index==2?RED_PAL:CYN_PAL);

			sprintf (buf, "Speed weight:        %6.2f ", (double)wt->speed_weight);
			txt_str (2, 31, buf, weight_index==3?RED_PAL:CYN_PAL);

			sprintf (buf, "Own path weight:     %6.2f ", (double)wt->own_path_weight);
			txt_str (2, 32, buf, weight_index==4?RED_PAL:CYN_PAL);

			sprintf (buf, "Priority weight:     %6.2f ", (double)wt->priority_weight);
			txt_str (2, 33, buf, weight_index==5?RED_PAL:CYN_PAL);

			sprintf (buf, "On good hint weight: %6.2f ", (double)wt->hint_weight);
			txt_str (2, 34, buf, weight_index==6?RED_PAL:CYN_PAL);

			/* point to weight value to update */
			switch (weight_index)
				{
				case 0:
					weight_ptr = &dummy;
					break;

				case 1:
					weight_ptr = &wt->distance_weight;
					break;

				case 2:
					weight_ptr = &wt->direction_weight;
					break;

				case 3:
					weight_ptr = &wt->speed_weight;
					break;

				case 4:
					weight_ptr = &wt->own_path_weight;
					break;

				case 5:
					weight_ptr = &wt->priority_weight;
					break;

				case 6:
				default:
					weight_ptr = &wt->hint_weight;
					break;
				}

			/*update speed*/
			if (edges & SW_COINL)
				dir = -1;
			else if (edges & SW_COINR)
				dir = 1;
			else
				dir = 0;

			ReadShifter();

			switch (man_gear)
				{
				case 1:
					*weight_ptr += dir;
					break;
				case 2:
					*weight_ptr += .1 * dir;
					break;
				case 3:
					*weight_ptr = 0;
					break;
				case 4:
					*weight_ptr = 1;
					break;
				}

			break;

		case Cleanup:
			break;
		}
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Write the current weight file to the hard drive                      */

void save_weight_table (S16 mode)
{
	write_file_to_hd ("temp0.rom", sizeof(MPWEIGHT)*MAX_MPATHS*3, (U32 *)weight_table);
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Read the current weight file from the hard drive                     */

void read_weight_table()
{
	read_file_from_hd ("temp0.rom", sizeof(MPWEIGHT)*MAX_MPATHS*3, (U32 *)weight_table);

#if TRKFIX
{
S16 i,j;
/* temp - copy t4-8 weights form t2. */
	for(i=3;i<NTRACKS;i++)
		for(j=0;j<MAX_LINKS;j++)
			{
		weight_table[i][j] = weight_table[1][j];
		}
}
#endif
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Set drone to use for weight table editing                            */

void select_drone0 (S16 mode) { cur_weight_drone = 0; }
void select_drone1 (S16 mode) { cur_weight_drone = 1; }
void select_drone2 (S16 mode) { cur_weight_drone = 2; }
void select_drone3 (S16 mode) { cur_weight_drone = 3; }
void select_drone4 (S16 mode) { cur_weight_drone = 4; }
void select_drone5 (S16 mode) { cur_weight_drone = 5; }
void select_drone6 (S16 mode) { cur_weight_drone = 6; }
void select_drone7 (S16 mode) { cur_weight_drone = 7; }

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Set index of weight to adjust                                        */

void set_dist_weight (S16 mode)		{ weight_index = 1; }
void set_dir_weight (S16 mode)		{ weight_index = 2; }
void set_speed_weight (S16 mode)	{ weight_index = 3; }
void set_own_weight (S16 mode)		{ weight_index = 4; }
void set_priority_weight (S16 mode)	{ weight_index = 5; }
void set_hint_weight (S16 mode)		{ weight_index = 6; }

