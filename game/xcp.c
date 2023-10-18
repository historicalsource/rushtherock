/*
 *	checkpoint.c -- register checkpoints as you go around the track
 *
 *	Copyright 1996 Time Warner Interactive
 *	Unauthorized reproduction, adaptation, distribution, performance or 
 *	display of this computer program or the associated audiovisual work
 *	is strictly prohibited.
 *
 *	======================================================================
 *	$Author: gray $						$Date: 1997/10/15 09:34:36 $
 *	$Revision: 3.69 $						$Locker:  $
 *	======================================================================
 *	Change Log:
 *
 *	$Log: checkpoint.c,v $
 * Revision 3.69  1997/10/15  09:34:36  gray
 * Removed debug from finish line crossing interpolation.
 *
 * Revision 3.68  1997/10/15  08:53:24  gray
 * Determine finish line crossing time to nearest msec.
 *
 * Revision 3.67  1997/10/12  07:55:24  gray
 * Brute force method to make sure gameover times are send and received.
 * Should be rolled into gamenet later.
 *
 * Revision 3.66  1997/10/09  18:42:46  grjost
 * Fixed "tie" crash
 *
 * Revision 3.65  1997/10/08  23:35:04  grjost
 * Made so no two cars can tie.
 *
 * Revision 3.64  1997/09/30  02:19:33  grjost
 * Added new lap font
 *
 * Revision 3.63  1997/09/25  03:04:47  gray
 * negate checkpoint x pos if mirror mode.
 *
 * Revision 3.62  1997/09/25  02:07:31  grjost
 * Fixed tourney mode for 12 tracks, 8 cars, and menus in GUTS.
 *
 * Revision 3.61  1997/09/15  22:27:35  gray
 * Removed trackset hack - trackno now 0 to NTRACKS.
 *
 * Revision 3.60  1997/09/13  01:47:02  grjost
 * Made sure music tracks were shut off at finish
 *
 * Revision 3.59  1997/09/02  00:02:02  gray
 * Lap hack for Alcatraz. Don't visually count down track for 1st finish cross.
 *
 * Revision 3.58  1997/08/28  20:46:01  gray
 * Audio for 10 thru 19 laps-to-go removed from rel_96.
 *
 * Revision 3.57  1997/07/27  22:29:14  gray
 * Checkpoint set select for consumer track dev.
 *
 * Revision 3.56  1997/07/01  18:58:32  gray
 * Made alternate tracks (additional 4 tracks) view 1 selectable.
 *
 * Revision 3.55  1997/06/28  01:40:05  gray
 * Extended to 4 tracks.
 *
 * Revision 3.54  1997/05/11  03:37:10  gray
 * Cleaning model interface.
 *
 * Revision 3.53  1997/04/30  00:51:28  gray
 * Now handles skill path points.
 *
 * Revision 3.52  1997/02/26  03:32:18  geraci
 * fixed hang up when resurrecting through finish line
 *
 * Revision 3.51  1997/02/21  00:08:46  geraci
 * time box targets can no longer give out more time than regular game
 *
 * Revision 3.50  1997/02/15  01:45:19  geraci
 * join quick mode implemented
 *
 * Revision 3.49  1997/01/31  23:33:59  geraci
 * swaps finish line on checkpoint before finish
 *
 * Revision 3.48  1997/01/30  20:33:08  geraci
 * four more difficulty settings
 *
 * Revision 3.47  1997/01/25  04:32:34  geraci
 * cleared some debug stuff
 *
 * Revision 3.46  1997/01/25  01:24:10  geraci
 * new checkpoint scheme
 *
 * Revision 3.45  1997/01/15  20:01:29  geraci
 * number of laps for track 2 and 3 can be changed by operator
 *
 * Revision 3.44  1997/01/10  03:40:31  geraci
 * fixed no more time bug when car passes finish line
 *
 * Revision 3.43  1996/12/13  14:54:50  geraci
 * back to using laps in modeldat for real lap counter
 *
 * Revision 3.42  1996/12/13  12:05:56  geraci
 * more new lap stuff
 *
 * Revision 3.41  1996/12/13  08:16:07  geraci
 * new lap stuff
 *
 * Revision 3.40  1996/12/05  10:19:55  geraci
 * no bonus time through finish line
 *
 * Revision 3.39  1996/12/05  01:52:47  geraci
 * fixed extra time through checkpoints bug
 *
 * Revision 3.38  1996/12/05  00:05:17  geraci
 * end of game score removed from entity state
 *
 * Revision 3.37  1996/12/04  01:56:42  geraci
 *  checkpoints and end of game scores are handled by entity state updates
 *
 * Revision 3.36  1996/11/27  17:06:13  geraci
 * laps to go is displayed
 *
 * Revision 3.35  1996/11/27  01:58:27  geraci
 * time at checkpoint is accurate now
 *
 * Revision 3.34  1996/11/25  16:02:16  geraci
 * errors messages are now on sw 11 debug
 *
 * Revision 3.33  1996/11/18  10:22:19  geraci
 * removed unneeded debug flag
 *
 * Revision 3.32  1996/11/16  18:58:31  geraci
 * stops tourney game after all cars cross finish line
 *
 * Revision 3.31  1996/11/14  12:52:25  geraci
 * race leader sound changed, debug code for checkpoints
 *
 * Revision 3.30  1996/11/11  19:34:47  petrick
 * Finish line checkpoint texture swap.
 *
 * Revision 3.29  1996/11/07  05:07:01  geraci
 * rush whisper instead of cheering when in first
 *
 * Revision 3.28  1996/11/03  05:58:12  geraci
 * split up debug switches and regular game switches
 *
 * Revision 3.27  1996/11/02  09:11:53  geraci
 * drone can be skipped ahead
 *
 * Revision 3.26  1996/11/01  12:25:49  geraci
 * drone and human types are now defines
 *
 * Revision 3.25  1996/11/01  11:20:57  geraci
 * players that are resurrecting do not get a place, end of tourney works right
 *
 * Revision 3.24  1996/10/28  10:07:20  petrick
 * Supressed checkpoint msg if demo game.
 *
 * Revision 3.23  1996/10/24  11:41:34  geraci
 * supports multi-lap tournament mode
 *
 * Revision 3.22  1996/10/24  08:42:56  geraci
 * checkpoint moob removed
 *
 * Revision 3.21  1996/10/23  00:14:33  petrick
 * Made track_no a global.
 *
 * Revision 3.20  1996/10/22  08:53:23  geraci
 * current lap for recording can be cancelled
 *
 * Revision 3.19  1996/10/18  08:37:01  geraci
 * debug switch 14 controls whether or not checkpoints objects are used
 *
 * Revision 3.18  1996/10/16  07:53:13  geraci
 * removed unnecessary equate
 *
 * Revision 3.17  1996/10/16  07:46:20  geraci
 * removed static announcer code as well as moob on each checkpoint
 *
 * Revision 3.16  1996/10/10  05:19:24  petrick
 * Cleaned out some unused globals.
 *
 * Revision 3.15  1996/10/09  02:15:38  geraci
 * announcer is a moob
 *
 * Revision 3.14  1996/10/08  07:45:36  petrick
 * Got rid of temporary patch that didn't work anyway.
 *
 * Revision 3.13  1996/10/07  16:11:27  geraci
 * game difficulty adds progressively more bonus time
 *
 * Revision 3.12  1996/10/05  09:08:38  geraci
 * added many more static announcer sound bites
 *
 * Revision 3.11  1996/10/04  07:31:45  geraci
 * attract sound is now split into attract effects and attract music
 *
 * Revision 3.10  1996/10/03  09:08:25  petrick
 * MAde sure some args were right size.
 *
 * Revision 3.9  1996/10/02  21:15:02  petrick
 * Changed params in PutEventReport PDU call.
 *
 * Revision 3.8  1996/10/02  02:54:49  geraci
 * fixed checkpoint center display to show actual saved values
 *
 * Revision 3.7  1996/09/25  00:22:18  gray
 * Removed old ZOID download stuff.
 *
 * Revision 3.6  1996/09/19  15:05:37  petrick
 * Added params to Checkpoint and Game over msgs.
 *
 * Revision 3.5  1996/09/19  05:56:53  geraci
 * don't do static chatter after each checkpoint
 *
 * Revision 3.4  1996/09/18  10:09:39  geraci
 * background announcer sound effect after checkpoint
 *
 * Revision 3.3  1996/09/17  08:42:42  geraci
 * crowd cheers when you get in first place
 *
 * Revision 3.2  1996/09/13  23:00:13  geraci
 * laps to go sound byte
 *
 * Revision 3.1  1996/09/09  02:24:52  gray
 * JAMMA version
 *
 *
 *	======================================================================
 */

#include "typedefs.h"
#include "stdtypes.h"
#include "attract.h"
#include "blit.h"
#include "carsnd.h"
#include "checkpoint.h"
#include "config.h"      /* color & switches etc. */
#include "controls.h"
#include "debug.h"
#include "dis.h"
#include "hud.h"
#include "model.h"
#include "fmath.h"
#include "game.h"
#include "globals.h"
#include "mb.h"
#include "menus.h"
#include "maxpath.h"
#include "mdrive.h"
#include "moobdefs.h"
#include "options.h"
#include "optmenu.h"
#include "os_proto.h"
#include "resurrect.h"
#include "sounds.h"
#include "stree.h"

#include "Pro/carsnd.pro"
#include "Pro/drivsym.pro"
#include "Pro/mathsubs.pro"
#include "Pro/math.pro"
#include "Pro/select.pro"
#include "Pro/sounds.pro"
#include "Pro/stree.pro"
#include "Pro/vecmath.pro"

#define LAP_HACK 1
#define GAMEOVER_RESEND 1

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/

extern  PATHTYP     *path;
extern  U16			path_start, path_end, path_loop;
extern  const U32	track_cp_times_best[];
extern  const U32	track_cp_times_par[];
extern  U32			play_time;
extern	U32			last_levels;
extern	U16			maxpath_dist[];
extern	U32 		gCheckPtTimeout2;
extern  Tourney     gTourney;
extern	S16			max_radio;

extern  void 		ChangeFinishLine(void);

/*********************************************************************************/

U32     number_checkpoints, last_checkpoint_time;
U32     old_last_cp_index, index1, index2;
S16     closest_cp_index[MAX_CPS+2];
U32     post_object[8];
S32		first_place_time;
BOOL	in_first_place, cancel_mpath_lap;

/* checkpoint data in its final form that is used during play */
CheckPoint	TrackCPs[MAX_TRACKS][MAX_CPS];
Track		CP_Track[MAX_TRACKS];
Track_Data  track_data[MAX_TRACKS];
char		buf[66];
S16			close_chkpnt, path_point_index, num_path_points;
S16			path_dist_index[MAX_CPS];
U16			path_dist[1500];
S16			path_index[1500];
S16			path_to_maxpath[1500];
S16			global_laps, global_last_cp, global_next_cp;
S16			lap_loop_index;
F32			lap_loop_distance;
U32			time_to_be_given, total_time_given, total_time_box_time;
const S16	laps_to_go[10] = { S_FINALL, S_2L, S_3L, S_4L, S_5L, 
							   S_6L, S_7L, S_8L, S_9L };

const F32	tmpuvs[3][3] = {
                             {1.0, 0.0, 0.0},
                             {0.0, 1.0, 0.0},
                             {0.0, 0.0, 1.0}
                           };

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*		Initialize all checkpoint structures                                 */

void init_cp_data()
{
	S16   i, j, k, m, t_cent, t_fwd, t_back, p_index;
	U32   objindex;
	S32   temp[3];
	F32   delta1[3], delta2[3], pos[3];
	F32   dist[20], x, y, z, dx, dy, dz, cur_dist, px, py, pz;
	CheckPoint *chkpnt;
	InitCheckPoint *initchkpnt;
	Track_Data *trck;
	S32 itrk;

	m = 0;

	for(i=0;i<MAX_TRACKS;i++)
		CP_Track[i].chk_point = &TrackCPs[i][0];

	/* print index for error messages */
	p_index = 0;

	trck = &track_data[trackno];

	/* initialize data for target track */
	itrk = trackno;
	for (i=0; i<8; i++)
		{
		trck->start_time[i] = init_track_data[itrk].start_time[i] * ONE_SEC;
		trck->end_time[i] = init_track_data[itrk].end_time[i] * ONE_SEC;
		}

	trck->loop_chkpnt = init_track_data[itrk].loop_chkpnt;
	trck->finish_line = init_track_data[itrk].finish_line;
	trck->before_finish = init_track_data[itrk].before_finish;

	/* initialize checkpoint structure for track */
	initchkpnt = InitCP_Track[itrk];
	chkpnt = CP_Track[trackno].chk_point;

	for (j=0; j<MAX_CPS; j++)
		{
		chkpnt[j].logical_cp = initchkpnt[j].logical_cp;
		for (k=0; k<2; k++)
			chkpnt[j].bonus_t[k] = initchkpnt[j].best_t[k] * ONE_SEC;
		chkpnt[j].radius = initchkpnt[j].radius;

		if (chkpnt[j].logical_cp == -1)
			break;
		}

	CP_Track[trackno].num_checkpoints = j;

	/* init important track data (path_end etc.) */
	init_stree();

	initchkpnt = InitCP_Track[itrk];
	chkpnt = CP_Track[trackno].chk_point;

	for (i=0; i<CP_Track[trackno].num_checkpoints; i++)
		{
		dist[i] = 9999999;
		}

	px = py = pz = 99999*40;

	/* determine closest track center (linear search method) */
	for (i=(S16)path_start; i<(S16)path_end; i++)
		{
		x =  path[i].pos[1]/40;
		y = -path[i].pos[2]/40;
		z =  path[i].pos[0]/40;

		if (px==x && py==y && pz==z && (dlevels & SW_DEBUG11))
			{
			sprintf (buf, "Repeated path on track:%d (%g %g %g)", trackno+1, (double)x, (double)y, (double)z);
			txt_str (5, 10+p_index++, buf, RED_PALB);
			}

		if (i != (S16)path_start)
			{
			pos[0] = px - x;   pos[1] = py - y;   pos[2] = pz - z;
			if (magnitude (pos) > 700 && (dlevels & SW_DEBUG11))
				{
				sprintf (buf, "Shorten path on track:%d to < 700 (%g %g %g)", trackno+1, (double)x, (double)y, (double)z);
				txt_str (5, 10+p_index++, buf, RED_PALB);
				}
			}

		px = x;  py = y;  pz = z;

		for (j=0; j<CP_Track[trackno].num_checkpoints; j++)
			{
			/* swap checkpoint x for mirrored tracks. */
			dx = (gMirrorMode) ? -initchkpnt[j].pos[0] - x : initchkpnt[j].pos[0] - x;
			dy = initchkpnt[j].pos[1] - y;
			dz = initchkpnt[j].pos[2] - z;

			cur_dist = dx*dx + dy*dy + dz*dz;

			if (cur_dist < dist[j])
				{
				dist[j] = cur_dist;
				chkpnt[j].track_cent = i;
				}
			}
		}

	/* save direction deltas here */
	k = 0;

	/* get general direction of track centers and save closest path point */
	for (j=0; j<CP_Track[trackno].num_checkpoints; j++)
		{
		i = chkpnt[j].track_cent;

		if (j != 0)
			k += (i < chkpnt[j-1].track_cent);

		chkpnt[j].pos[0] = path[i].pos[1]/40;
		chkpnt[j].pos[1] = -path[i].pos[2]/40;
		chkpnt[j].pos[2] = path[i].pos[0]/40;
		}

	if (k > 1)
		CP_Track[trackno].direction = -1;
	else
		CP_Track[trackno].direction = 1;

	/* calculate uv's for all checkpoints */
	for (k=0; k<CP_Track[trackno].num_checkpoints; k++)
		{
		t_cent = chkpnt[k].track_cent;
		t_fwd = get_next_center (t_cent, CP_Track[trackno].direction);
		t_back = get_prev_center (t_cent, CP_Track[trackno].direction);

		/* cent - back */
		ivecsub (path[t_cent].pos, path[t_back].pos, temp);
		delta1[0] = ((F32)temp[1])/40;
		delta1[1] = 0;
		delta1[2] = ((F32)temp[0])/40;

		/* forward - cent */
		ivecsub (path[t_fwd].pos, path[t_cent].pos, temp);

		delta2[0] = ((F32)temp[1])/40;
		delta2[1] = 0;
		delta2[2] = ((F32)temp[0])/40;

		/* unit vectorize */
		if (magnitude (delta1) < .1)
			{
			if (dlevels & SW_DEBUG11)
				{
				sprintf (buf, "error 1a near (%g %g %g) ",
						 (double)path[t_back].pos[0]/40,
						 (double)path[t_back].pos[1]/40,
						 (double)path[t_back].pos[2]/40
						 );
				txt_str (30, 13+m++, buf, RED_PAL);
				}
			}
		else
			direction (delta1, delta1);

		if (magnitude (delta2) < .1)
			{
			if (dlevels & SW_DEBUG11)
				{
				sprintf (buf, "error 1b near (%g %g %g) ",
						 (double)path[t_fwd].pos[0]/40,
						 (double)path[t_fwd].pos[1]/40,
						 (double)path[t_fwd].pos[2]/40
						 );
				txt_str (30, 13+m++, buf, RED_PAL);
				}
			}
		else
			direction (delta2, delta2);

		/* average two deltas */
		pos[0] = (delta1[0] + delta2[0]) / 2;
		pos[1] = 0;
		pos[2] = (delta1[2] + delta2[2]) / 2;

		if (magnitude (pos) < .1)
			{
			if (dlevels & SW_DEBUG11)
				{
				sprintf (buf, "error 1c near (%g %g %g) ",
						 (double)path[t_cent].pos[0]/40,
						 (double)path[t_cent].pos[1]/40,
						 (double)path[t_cent].pos[2]/40
						 );
				txt_str (30, 13+m++, buf, RED_PAL);
				}
			pos[0] = 0;
			pos[2] = 1;
			}
		else
			direction (pos, pos);

		chkpnt[k].uvs[0][0] = pos[2];
		chkpnt[k].uvs[1][0] = 0;
		chkpnt[k].uvs[2][0] = pos[0];

		chkpnt[k].uvs[0][1] = 0;
		chkpnt[k].uvs[1][1] = 1;
		chkpnt[k].uvs[2][1] = 0;

		chkpnt[k].uvs[0][2] = -pos[0];
		chkpnt[k].uvs[1][2] = 0;
		chkpnt[k].uvs[2][2] = pos[2];
		}
}


/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*		Get next track center
*         Upon entry: t_index -> current track center
*                     t_direction -> -1 = track center indices are decreasing
*                                     1 = track center indices are increasing
*         Upon exit: returned -> next track center                           */

S16 get_next_center (S16 t_index, S16 t_direction)
{
	if (t_index < (S16)path_start)
		t_index = path_start;
	else
		if (t_index > (S16)path_end)
			t_index = path_end;

	switch (t_direction)
		{
		case -1:
			if (t_index <= (S16)path_start)
				if (path_loop == path_start)
					t_index = path_end;
				else
					t_index = path_loop;
			else
				t_index--;
			break;

		case 1:
			if (t_index >= (S16)path_end)
				t_index = path_loop;
			else
				t_index++;
			break;
		}

	return (t_index);
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*		Get previous track center
*         Upon entry: t_index -> current track center
*                     t_direction -> -1 = track center indices are decreasing
*                                     1 = track center indices are increasing
*         Upon exit: returned -> previous track center                       */

S16 get_prev_center (S16 t_index, S16 t_direction)
{
	if (t_index < (S16)path_start)
		t_index = path_start;
	else
		if (t_index > (S16)path_end)
			t_index = path_end;

	switch (t_direction)
		{
		case -1:
			if ((t_index == (S16)path_loop) && (path_loop != path_start))
				t_index = path_start;
			else
				if (t_index < (S16)path_end)
					t_index++;
				else
					t_index = path_end;
			break;

		case 1:
			if (t_index == (S16)path_loop)
				t_index = path_end;
			else
				if (t_index > (S16)path_start)
					t_index--;
				else
					t_index = path_start;
			break;
		}

	return (t_index);
}

S32 dbgGOtime_cnt;

/*****************************************************************************/

void InitCPS (void) 
{
	S16			i, j, k, cur_dist, c_index, n_index, num_laps;
	S16			c_cent, l_cent, loop_cnt, p_index;
	BOOL		check_it[MAX_CPS];
	S32			dx, dy, dz, l_type, lap;
	F32			total_time, other_time, last_cp_time;
	F32			start_time, end_time, fudge_factor;
	CheckPoint  *chkpnt = CP_Track[trackno].chk_point;
	Track_Data	*trck = &track_data[trackno];
	InitCheckPoint *initchkpnt = InitCP_Track[trackno];


dbgGOtime_cnt = 0;

	global_laps = 0;
	global_last_cp = 0;
	global_next_cp = 1;
	number_checkpoints = 0;
	end_game_flag = 0;
	total_time_given = 0;
	total_time_box_time = 0;

	while ((chkpnt[number_checkpoints].logical_cp != -1) && (number_checkpoints < MAX_CPS))
		number_checkpoints++;

	lap_loop_index = trck->loop_chkpnt;

	/* get number of laps of race */
	if (TourneyOn() && gTourney.laps>0)
		num_laps = gTourney.laps;
	else
		num_laps = track_len[trackno];

#if LAP_HACK
/* should put flag in cpinit struct. */
	if(trackno == 3)
		num_laps++;
#endif

	/* save number of laps in race */
	trck->number_of_laps = num_laps;

	/* calculate all bonus times */
	n_index = 0;
	total_time = 0;
	other_time = 0;
	last_cp_time = 0;

	/* get optimal time through first lap (last cp is in last_cp_time) */
	while (1)
		{
		n_index = get_next_checkpoint (n_index);

		last_cp_time = chkpnt[n_index].bonus_t[0];

		if (n_index == trck->finish_line)
			break;

		total_time += last_cp_time;
		}

	if (num_laps > 1)
		{
		/* make first lap complete */
		total_time += last_cp_time;

		/* set other_time to normal lap less last cp (saved in last_cp_time) */
		while (1)
			{
			n_index = get_next_checkpoint (n_index);

			last_cp_time = chkpnt[n_index].bonus_t[1];

			if (n_index == trck->finish_line)
				break;

			other_time += last_cp_time;
			}

		total_time += (other_time + last_cp_time) * (num_laps-2) + other_time;
		}

	start_time = trck->start_time[difficulty[trackno]];
	end_time = trck->end_time[difficulty[trackno]];

	/* total_time is time it takes to drive track less finish line checkpoint */
	/* now get total time that will be awarded */
	time_to_be_given = (total_time+last_cp_time) - start_time + end_time;

	/* factor to adjust ideal time to get actual checkpoint times */
	fudge_factor = (F32)time_to_be_given / total_time;

#if 0
		{
		char buf[50];

		nsprintf (buf, 50, "%8.1f %8.1f %8.1f", total_time, last_cp_time, other_time);
		txt_str (10, 38, buf, WHT_PALB);
		nsprintf (buf, 50, "%8.1f %8.1f %8.3f ", start_time, end_time, fudge_factor);
		txt_str (10, 39, buf, WHT_PALB);
		}
#endif

	for (j=0; j<number_checkpoints; j++)
		for (k=0; k<2; k++)
			{
			char buf[50];

			chkpnt[j].bonus_t[k] *= fudge_factor;

#if 0
			nsprintf (buf, 50, "%8.3f ", chkpnt[j].bonus_t[k]);
			txt_str (40+k*10, 33+j, buf, WHT_PALB);
#endif
			}

	/* point to finish line checkpoint for lap counter */
	update_lap_counter (Initialize, trck->finish_line);

	/* clear table that indicates checkpoint has been processed */
	loop_cnt = 0;
	for (i=0; i<number_checkpoints; i++)
		check_it[i] = false;

	/* start with first checkpoint */
	c_index = 0;
	c_cent = chkpnt[c_index].track_cent;
	c_cent = get_prev_center (c_cent, CP_Track[trackno].direction);
	c_cent = get_prev_center (c_cent, CP_Track[trackno].direction);

	/* next checkpoint in line */
    n_index = get_next_checkpoint (c_index);
	l_cent = chkpnt[n_index].track_cent;

	cur_dist = 0;

	/* index into path dist/index tables */
	i = 0;
	path_index[i] = c_cent;
	path_dist[i++] = 0;

	/* k is index into checkpoint index table */
	k = 0;
	/* first checkpoint's path point will be at 2 */
	path_dist_index[k++] = i+1;

	while (1)
		{
		/* get next path point */
		j = get_next_center (c_cent, CP_Track[trackno].direction);

		dx =  (path[c_cent].pos[1] - path[j].pos[1]);
		dy = -(path[c_cent].pos[2] - path[j].pos[2]);
		dz =  (path[c_cent].pos[0] - path[j].pos[0]);

		cur_dist += lsqrt (dx*dx + dy*dy + dz*dz)/40;
		path_index[i] = j;
		path_dist[i++] = cur_dist;

		/* are we at the next checkpoint? */
		if (j == l_cent)
			/* yes */
			{
			/* set index into path_dist table */
			path_dist_index[k++] = i - 1;

			/* have two checkpoints of wrap data happened yet? */
			if (check_it[c_index] == true)
				if (++loop_cnt == 2)
					break;

			/* mark it as used */
			check_it[c_index] = true;

			c_index = n_index;

			/* get next checkpoint */
			n_index = get_next_checkpoint (c_index);
			l_cent = chkpnt[n_index].track_cent;
			}

		c_cent = j;
		}

	num_path_points = i;
	cancel_mpath_lap = false;
}


/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*		Initialize first checkpoint time                                     */

void init_cp_time (void)
{
	S16   i, j, num, temp;

	last_checkpoint_time = IRQTIME;
	first_place_time = 0;
	in_first_place = false;
}


/*****************************************************************************/

void JumpToNextCP (void)
{
	MODELDAT	*m = &model[this_node];

	PassedCP (m, true);
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Routine to set game time to one second                               */

void set_game_time_to_one_second()
{
	S16 i;

	play_time = GetElapsedTime() + ONE_SEC;
	gIgnoreScore = true;

	for (i=0; i<num_active_cars; i++)
		game_car[model[i].slot].place_locked = -1;
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Get next checkpoint
*         Upon entry: cur_checkpoint -> current checkpoint index
*         Upon exit: returned -> next checkpoint index                       */

U32 get_next_checkpoint (U32 cur_checkpoint)
{
    cur_checkpoint++;

    if (cur_checkpoint >=  number_checkpoints)
		cur_checkpoint = lap_loop_index;

	return (cur_checkpoint);
}

/*****************************************************************************/
/* For inside-outside check:

        Given a plane defined by a point A and normal N, a point Q will be inside 
	halfspace if T = (Q-A) dot N < 0 (the normal N is assumed to point "out"
	
	Use uvs Z-vec for normal N
*/	

void CheckCPs (void)
{
	F32  cent_dist, obs_posn[3], diff[3], zvec[3], dist_tab[MAX_LINKS];
	S16  i, j, index, temp, high_dist, high_index, num_left, lap;
	S16  place[MAX_LINKS];
	CheckPoint *chkpnt;
	CAR_DATA   *gc;
	MODELDAT   *m;
	U32 time;

	chkpnt = CP_Track[trackno].chk_point;

	for (index=0; index<MAX_LINKS; index++)
		{
		m = &model[index];

		if ((  (m->we_control) || (index==gThisNode))
			&& (game_car[index].just_crashed == false)
			&& (m->resurrect.moving_state == -1))
			{
			/* get car position */
			for (i=0; i<3; i++)
				obs_posn[i] = game_car[index].dr_pos[i];

			for (i = 0; i < 3; i++)
				{
				diff[i] = obs_posn[i] - chkpnt[m->next_cp_index].pos[i];
				zvec[i] = chkpnt[m->next_cp_index].uvs[2][i];
				}

			cent_dist = diff[0]*diff[0] + diff[2]*diff[2];

			if (cent_dist < chkpnt[m->next_cp_index].radius)
				{
				if (DotVector(diff, zvec) > 0.0)
					{ /* Increment checkpoint */
						PassedCP (m, false);
					}
				}
			}
		}

#if GAMEOVER_RESEND
	/* (re)send any gameover times not recieved. */
	for(i=0;i<MAX_LINKS;i++)
		{
		/* copy into local var since asynronously modified. */
		time = game_car[i].gameover_time;
		if(time > 0)
			PutEventReportPDU( Event_GameOver, 4,
							   Report_Node, gThisNode,
							   Report_Slot, (U32)i,
							   Report_Score, time,
							   Report_Flags, 0 );
		}
#endif

	/* get all maxpath intervals and set current distances */
	if (!mpath_edit)
		find_maxpath_intervals();

	/* if place is not locked, then add to list */
	num_left = 0;
	for (i=0; i<num_active_cars; i++)
		{
		if (game_car[model[i].slot].place_locked != 1)
			place[num_left++] = model[i].slot;
		}

	if (num_left != 0)
		{
		for (i=0; i<num_left; i++)
			dist_tab[place[i]] = game_car[place[i]].distance;

		/* sort in distance order */
		for (i=0; i<num_left; i++)
			{
			high_dist = dist_tab[place[i]];
			high_index = i;
			for (j=i+1; j<num_left; j++)
				{
				index = place[j];
				if (dist_tab[index] > dist_tab[place[high_index]])
					{
					high_dist = dist_tab[index];
					high_index = j;
					}
				}

			game_car[place[high_index]].place = i + num_active_cars - num_left;
			temp = place[high_index];
			place[high_index] = place[i];
			place[i] = temp;
			}
		}

	/* check if first place cheer should expire */
	if (first_place_time && IRQTIME-first_place_time > 3*ONE_SEC)
		{
		first_place_time = 0;
		SOUND (S_KLEADERLIGHT);
		}

	if (game_car[this_node].place==0 &&
		(!demo_game || (demo_game && attract_effects)))
		{
		if (in_first_place==false && first_place_time==0)
			/* we were not in first place previously */
			{
			first_place_time = IRQTIME;
			in_first_place = true;
			SOUND (S_LEADERLIGHT);
			}
		}
	else
		in_first_place = false;

	check_everyones_checkpoints();
}

/*****************************************************************************/
/*   flag -> true = put 600 seconds on clock, false = normal checkpoint      */

void PassedCP (MODELDAT *m, S8 flag)
{
	S16			i, node, sound_code, lap;
	CheckPoint	*chkpnt = CP_Track[trackno].chk_point;
	Track_Data	*trck = &track_data[trackno];
	CAR_DATA	*gc;
	F32 rpos[3],dist,vel;
	S32 dtime;
	
	node = m->net_node;
	gc = &game_car[node];

	if (flag)
		cancel_mpath_lap = true;

	/* bump number of laps */
	if (m->next_cp_index == trck->finish_line)
		{
		m->laps++;

		if (node == this_node)
			{
			check_mpath_save (cancel_mpath_lap);
			cancel_mpath_lap = false;
			}

		if (!mpath_edit)
			{
			/* uses skip ahead, just set index to appropriate value */
			if (flag)
				set_maxpath_index (m, path_to_maxpath[path_dist_index[lap_loop_index]]);
			}
		}

	/* update last and next checkpoint indices */
 	m->last_cp_index = m->next_cp_index;
	m->next_cp_index = get_next_checkpoint (m->last_cp_index);

	/* report checkpoint to all */
	if (!demo_game && flag)
		PutEventReportPDU( Event_CheckPoint, 4,
						   Report_Node, gThisNode,
						   Report_Channel, node,
						   Report_CheckPoint, (m->laps<<16)+(m->last_cp_index<<8)+(m->next_cp_index),
						   Report_Flags, flag );

	if (node == gThisNode)
		{
		/* default code */
		sound_code = S_CHKPNTSTATIC;

		if (m->last_cp_index == trck->finish_line)
			{
			i = trck->number_of_laps - m->laps;
			update_lap_counter (Do_it, i);

			if (i > 10)
				i = 10;

			if (i > 1)
				sound_code = laps_to_go[i-1];
			else if (i == 1)
				{
				ShowFinishFlag (chkpnt[m->last_cp_index].pos, false);
				sound_code = S_FINALL;
				}
			else if (i==0 && game_car[node].place_locked!=-1)
				{
				int j;
				sound_code = S_WINNER;
				ShowGameOver(true);
				for (j = 0; j < max_radio; j++)
					StopRadio(j);
				}
			}
		else if (m->last_cp_index == trck->before_finish)
			{
			if (trck->number_of_laps - m->laps == 1)
				ChangeFinishLine();
			}

		/* send checkpoint sound */
		SOUND (sound_code);

		/* look ahead one checkpoint and determine if it is finish line */
		lap_flag = ((  m->last_cp_index == trck->before_finish)
					&& m->laps == trck->number_of_laps-1);
		}

	if (node == this_node)
		{
		/* get and display current checkpoint time and then init for next */
		S32  current_time = IRQTIME;
		S32  delta_time = current_time - last_checkpoint_time;
		ShowCheckpointTime (delta_time);
		last_checkpoint_time = current_time;

#if 0
		if (node == gThisNode)
			{
			char buf[50];

			nsprintf (buf, 50, "%5.2f ", (F32)delta_time / ONE_SEC);
			txt_str (2+m->last_cp_index*7, 42+m->laps, buf, GRN_PAL);
			}
#endif
		}

	/* report finish-line crossing event to all */
	if (m->last_cp_index == trck->finish_line)
		{
		if (trck->number_of_laps==m->laps && game_car[node].place_locked==0 && game_car[node].gameover_time==0)
			{
#if GAMEOVER_RESEND
			if(node == gThisNode)
				{
				SOUND( S_DANGEROUS );
				dbgGOtime_cnt++;
				}
			/* flag to resend gameover time until message recieved. */
			game_car[node].gameover_time = GetElapsedTime();
			SubVector(gc->dr_pos, chkpnt[m->last_cp_index].pos, rpos);
			dist = BodyZ(rpos, chkpnt[m->last_cp_index].uvs);
			vel = BodyZ(gc->dr_vel, chkpnt[m->last_cp_index].uvs);
			/* get msec since crossed finish line. */
			dtime = (vel > 1.0) ? (dist/vel)*1000.0 : 0;
			game_car[node].gameover_time -= dtime;
if(node == gThisNode)
 DBG_printf(24,41,YEL_PALB,"IRQTIME  :%ld    ",game_car[node].gameover_time);
#else
			/* report game over to all */
			PutEventReportPDU( Event_GameOver, 4,
							   Report_Node, gThisNode,
							   Report_Slot, node,
							   Report_Score, GetElapsedTime(),
							   Report_Flags, 0 );
#endif
			}
		}

	/* drone learning */
	if (dlevels & SW_DEBUG1)
		{
		if (m->last_cp_index == trck->finish_line)
			{
			if (trck->number_of_laps == m->laps)
				update_drone_time (node, GetElapsedTime());
			}
		}
}

/*****************************************************************************/
/* create and place all checkpoint objects */

void FPutCheckpoints(S16 track)
{
	S16 i;
	CheckPoint *chkpnt;
	
	chkpnt = CP_Track[track].chk_point;

	if (dlevels & SW_DEBUG14)
		{
		for(i=0; i<number_checkpoints; i++)
			FPlaceOneCP (&chkpnt[i], OBJ_UVARROWS );
		}
}

/* FPlaceOneCP	-	place one particular checkpoint
 ------------------------------------------------------  
 * *chkpnt		-	pointer to checkpoint structure
 * chptno		-	the checkpoint number
 * *cp_objnum	-	the index of the object in gObjList.
 ------------------------------------------------------
 */
U32 FPlaceOneCP(CheckPoint *chkpnt, S32 cp_objnum)
{
	F32  pos[3];

	pos[0] = chkpnt->pos[0];
	pos[1] = chkpnt->pos[1] + 20;
	pos[2] = chkpnt->pos[2];

	return MBOX_NewObject( gObjList[cp_objnum], pos, chkpnt->uvs,-1L,(U32)ZATR_SORTALPHA);
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Check global checkpoints for transition                              */

void check_everyones_checkpoints (void)
{
	CheckPoint	*chkpnt;
	Track_Data	*trck = &track_data[trackno];
	S16  		i, slot, cur_laps, cur_lcp, cur_ncp, next_lap, next_ncp, next_lcp;
	S16			max_laps, make_beep, num_laps;
	U32			bonus_t;
	BOOL		finish_flag;

	chkpnt = CP_Track[trackno].chk_point;

	/* determine expected next checkpoint (for time) */
	next_lap = global_laps;

	if (global_next_cp == trck->finish_line)
		next_lap++;
	next_lcp = global_next_cp;
	next_ncp = get_next_checkpoint (next_lcp);

	make_beep = false;

	for (i=0; i<num_active_cars; i++)
		{
		slot = model[i].slot;

		cur_laps = game_car[slot].laps;
		cur_lcp = game_car[slot].checkpoint;
		cur_ncp = get_next_checkpoint (cur_lcp);

		/* if we don't control this car, update where that car is */
		if (model[slot].we_control == false && slot != gThisNode)
			{
			model[slot].last_cp_index = cur_lcp;
			model[slot].next_cp_index = cur_ncp;
			}

		finish_flag = false;

		/* don't give time at finish line */
		if (next_lcp == trck->finish_line   &&
			trck->number_of_laps == next_lap)
			finish_flag = true;

		/* check if the lead human made it past the next checkpoint and award time */
		if ((model[slot].drone_type == HUMAN)
			&& ((   cur_laps>next_lap)
				|| ((cur_laps==next_lap) && (chkpnt[cur_lcp].logical_cp>=chkpnt[next_lcp].logical_cp)))
			&& (finish_flag == false))
			{
			/* there are two groups of cp times, one for first laps and one for all others */
			if (global_laps > 0)
				bonus_t = chkpnt[next_lcp].bonus_t[1];
			else
				bonus_t = chkpnt[next_lcp].bonus_t[global_laps];

#if 0
				{
				char buf[50];
				nsprintf (buf, 50, "%7ld %7ld %7ld ", time_to_be_given, total_time_given, total_time_box_time);
				txt_str (10, 40, buf, WHT_PALB);
				}
#endif

			/* if we already gave that time, just deduct it from tbx time */
			if (total_time_box_time > bonus_t)
				{
				total_time_box_time -= bonus_t;
				make_beep = false;
				}
			else
				{
				play_time += bonus_t - total_time_box_time;
				total_time_given += bonus_t - total_time_box_time;
				total_time_box_time = 0;
				make_beep = true;
				}

#if 0
				{
				char buf[50];
				nsprintf (buf, 50, "%7ld %7ld %7ld ", time_to_be_given, total_time_given, total_time_box_time);
				txt_str (10, 41, buf, WHT_PALB);
				}
#endif

			global_laps = next_lap;
			global_last_cp = next_lcp;
			global_next_cp = next_ncp;

			next_lap = global_laps;
			if (global_next_cp == trck->finish_line)
				next_lap++;
			next_lcp = global_next_cp;
			next_ncp = get_next_checkpoint (next_lcp);
			}
		}

	if (make_beep)
		{
		SOUND (S_BEEP1);
		gCheckPtTimeout2 = IRQTIME + CHECKPT_TIME;
		}

}	/* end: check_everyones_checkpoints */ 

/* ----------------------------------------------------------------------------- *
 * Called when a message is received that the car in the given slot has passed the
 * given checkpoint.
 * ----------------------------------------------------------------------------- */
void CarPassedCheckpoint( S32 slot, S32 checkpoint, S32 flag )
{
	CheckPoint	*chkpnt;
	Track_Data	*trck = &track_data[trackno];
	S16  		i, cur_laps, cur_lcp, cur_ncp, next_lap, next_ncp, next_lcp;

	chkpnt = CP_Track[trackno].chk_point;

	/* determine expected next checkpoint (for time) */
	next_lap = global_laps;

	if (global_next_cp == trck->finish_line)
		next_lap++;

	next_lcp = global_next_cp;
	next_ncp = get_next_checkpoint (next_lcp);

	if (flag)
		{
		play_time = GetElapsedTime() + 600 * ONE_SEC;
		gIgnoreScore = true;

		for (i=0; i<num_active_cars; i++)
			game_car[model[i].slot].place_locked = -1;

		SOUND (S_KWINNER);
		end_game_flag = 0;
		}

}	/* end CarPassedCheckpoint */ 

/* ----------------------------------------------------------------------------- *
 * Called when a message is received that the car in the given slot has achieved
 * a game over state (out of time && (coasted to a stop || past final timeout)).
 * ----------------------------------------------------------------------------- */
void CarReportsGameOver( S32 slot, U32 score, U32 flag )
{
	S16  i, j, index, temp, num_locked, place[MAX_LINKS], num_humans_locked;
	S16  num_not_locked;

#if GAMEOVER_RESEND
	/* flag gameover message received. */
	game_car[slot].gameover_time == -1;
#endif

	/* human? */
	if (model[slot].drone_type == HUMAN && end_game_flag == 0)
		{
		if (!TourneyOn() || gTourney.ext_start == JOIN_QUICK)
			{
			if (GetCountdownTime() > 5 * ONE_SEC)
				play_time = 5 * ONE_SEC + GetElapsedTime();
			}
		}

	if (flag)
		{
		gstate = ENDGAME;
		return;
		}

/*---------------------------------------------------------------------------*/

	game_car[slot].score = score;
	game_car[slot].place_locked = 1;
	num_humans_locked = 0;

	/* re-sort all locked players */
	for (i=0,num_locked=-1; i<num_active_cars; i++)
		{
		index = model[i].slot;

		if (game_car[index].place_locked == 1)
			{
			place[++num_locked] = index;
			if (model[index].drone_type == HUMAN)
				num_humans_locked++;
			}
		}

	for (i=0; i<num_locked; i++)
		for (j=0; j<num_locked-i; j++)
			{
			if (game_car[place[j]].score > game_car[place[j+1]].score)
				{
				temp = place[j];
				place[j] = place[j+1];
				place[j+1] = temp;
				}
			}

	/* Check for ties;  If there is one, have lowest node "win" */
	for (i = 1; i <= num_locked; i++)
	    if ((game_car[place[i-1]].score == game_car[place[i]].score) &&
	        (place[i-1] > place[i]))
	    {
			temp = place[i];
			place[i] = place[i-1];
			place[i-1] = temp;
			i = 0;                /* restart loop */
	    }

	for (i=0; i<num_locked+1; i++)
		game_car[place[i]].place = i;

	/* now re-assign all non-locked cars */
	for (i=0,num_not_locked=-1; i<num_active_cars; i++)
		{
		index = model[i].slot;

		if (game_car[index].place_locked == 0)
			place[++num_not_locked] = index;
		}

	for (i=0; i<num_not_locked; i++)
		for (j=0; j<num_not_locked-i; j++)
			{
			if (game_car[place[j]].score > game_car[place[j+1]].score)
				{
				temp = place[j];
				place[j] = place[j+1];
				place[j+1] = temp;
				}
			}

	/* Check for ties;  If there is one, have lowest node "win" */
	for (i = 1; i <= num_not_locked; i++)
	    if ((game_car[place[i-1]].score == game_car[place[i]].score) &&
	        (place[i-1] > place[i]))
	    {
			temp = place[i];
			place[i] = place[i-1];
			place[i-1] = temp;
			i = 0;                /* restart loop */
	    }

	for (i=0; i<num_not_locked+1; i++)
		game_car[place[i]].place = i + num_locked + 1;

	if (TourneyOn() && num_humans_locked==num_humans)
		{
		end_game_flag = 1;
		play_time = ONE_SEC;
		}

}	/* end CarReportsGameOver */ 

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Display current checkpoint table
*         Upon entry: mode -> Initialize = display data for first time
*                             Do_it = update menu data                       */

void display_chkpnts (S16 mode)
{
	S16  i, j;
	F32  pos[3];
	CheckPoint		*chkpnt;
	InitCheckPoint	*initchkpnt;

	switch (mode)
		{
		case Initialize:
			close_chkpnt = 0;

			initchkpnt = InitCP_Track[trackno];
			chkpnt = CP_Track[trackno].chk_point;

			sprintf (buf, "Path start=%d  end=%d  loop=%d", path_start, path_end, path_loop);
			txt_str (2, 15, buf, GRN_PAL);

			txt_str (2, 17, "index       position          old saved pos", GRN_PAL);

			for (i=0; i<CP_Track[trackno].num_checkpoints; i++)
				{
				j = chkpnt[i].track_cent;

				pos[0] = path[j].pos[1]/40;
				pos[1] = -path[j].pos[2]/40;
				pos[2] = path[j].pos[0]/40;

				sprintf (buf, "  %3d (%5g %5g %5g)    (%g %g %g)",
						 j,
						 (double)pos[0], (double)pos[1], (double)pos[2],
						 (double)initchkpnt[i].pos[0],
						 (double)initchkpnt[i].pos[1],
						 (double)initchkpnt[i].pos[2]
						 );

				txt_str (2, 18+i, buf, GRN_PAL);
				}
			break;

		case Do_it:
			chkpnt = CP_Track[trackno].chk_point;
			initchkpnt = InitCP_Track[trackno];

			i = close_chkpnt;
			j = chkpnt[i].track_cent;

			pos[0] = path[j].pos[1]/40;
			pos[1] = -path[j].pos[2]/40;
			pos[2] = path[j].pos[0]/40;

			sprintf (buf, "  %3d (%5g %5g %5g)    (%g %g %g)",
					 j,
					 (double)pos[0], (double)pos[1], (double)pos[2],
					 (double)initchkpnt[i].pos[0],
					 (double)initchkpnt[i].pos[1],
					 (double)initchkpnt[i].pos[2]
					 );

			txt_str (2, 18+i, buf, (IRQTIME & 0x100) ? GRN_PAL : RED_PAL);
			break;
		}
}

void add_chkpnt (S16 mode)
{
	txt_str (2, 34, "add", WHT_PAL);
}

void del_chkpnt (S16 mode)
{
	txt_str (2, 34, "del", WHT_PAL);
}

void fwd_chkpnt (S16 mode)
{
	S16  i, j;

	for (i=path_start-5,j=0; i<path_start+5; i++,j++)
		{
		txt_decnum (j*4+2,41,
					get_next_center(i,CP_Track[trackno].direction),
					3,2,RED_PAL
					);
		}

	for (i=path_loop-5,j=0; i<path_loop+5; i++,j++)
		{
		txt_decnum (j*4+2,42,
					get_next_center(i,CP_Track[trackno].direction),
					3,2,RED_PAL
					);
		}

	for (i=path_end-5,j=0; i<path_end+5; i++,j++)
		{
		txt_decnum (j*4+2,43,
					get_next_center(i,CP_Track[trackno].direction),
					3,2,RED_PAL
					);
		}
}

void back_chkpnt (S16 mode)
{
	S16  i, j;

	for (i=path_start-5,j=0; i<path_start+5; i++,j++)
		{
		txt_decnum (j*4+2,41,
					get_prev_center(i,CP_Track[trackno].direction),
					3,2,RED_PAL
					);
		}

	for (i=path_loop-5,j=0; i<path_loop+5; i++,j++)
		{
		txt_decnum (j*4+2,42,
					get_prev_center(i,CP_Track[trackno].direction),
					3,2,RED_PAL
					);
		}

	for (i=path_end-5,j=0; i<path_end+5; i++,j++)
		{
		txt_decnum (j*4+2,43,
					get_prev_center(i,CP_Track[trackno].direction),
					3,2,RED_PAL
					);
		}
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Display path points from path point list
*         Upon entry: mode -> Initialize = display data for first time
*                             Do_it = update path data                       */

void display_path_points (S16 mode)
{
	S16 i, j;
	F32 pos[3];

	switch (mode)
		{
		case Initialize:
			path_point_index = 0;

		case Do_it:
			sprintf (buf, "Path start=%d  end=%d  loop=%d", path_start, path_end, path_loop);
			txt_str (2, 15, buf, GRN_PAL);

			for (i=path_point_index,j=16; j<36 && i<=(S16)path_end; j++,i++)
				{
				pos[0] = path[i].pos[1]/40;
				pos[1] = -path[i].pos[2]/40;
				pos[2] = path[i].pos[0]/40;

				sprintf (buf, "  %3d (%5g %5g %5g) %5d %5d %3d %3d %4d %4d ",
						 i, (double)pos[0], (double)pos[1], (double)pos[2],
                         path_dist[i], path_dist[i+path_end+1],
						 path_index[i], path_index[i+path_end+1],
						 path_to_maxpath[i], path_to_maxpath[i+path_end+1]
						 );

				txt_str (2, j, buf, GRN_PAL);
				}
			break;
		}
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Advance to next set of path points                                   */

void fwd_path_points (S16 mode)
{
	S16 i;

	clear_display();

	if (path_point_index+20 <= path_end)
		path_point_index += 20;
	else
		path_point_index = 0;
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Move back to previous set of path points                             */

void back_path_points (S16 mode)
{
	S16 i;

	clear_display();

	if (path_point_index-20 >= 0)
		path_point_index -= 20;
	else
		path_point_index = (path_end/20)*20;
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Updates lap counter to screen
*         Upon entry: mode -> Initialize = set up for first time
*                             Do_it = update to new value
*                             Cleanup = remove from screen
*                     laps_left -> current laps to go                        */
extern S16    gLapsLeft;
void update_lap_counter (S16 mode, S16 laps_left)
{
	char        buf[20];
	static BOOL first_time_flag;
	static S16  lap_x, lap_y, chkpnt_save;
	CheckPoint  *chkpnt = CP_Track[trackno].chk_point;
	Track_Data	*trck = &track_data[trackno];
	extern S16  gFinishX, gFinishY;

	switch (mode)
		{
		case Initialize:
			first_time_flag = true;
			chkpnt_save = laps_left;
			break;

		case Do_it:
			if (first_time_flag)
				{
				laps_left = trck->number_of_laps;
#if LAP_HACK
/* should put flag in cpinit struct. */
				if(trackno == 3)
					laps_left--;
#endif
				ShowFinishFlag (chkpnt[chkpnt_save].pos, laps_left > 1 || demo_game != 0);
				lap_x = (gFinishX >> 3) + 1;
				lap_y = (gFinishY >> 3) + 1;
				}

			first_time_flag = false;
			gLapsLeft = laps_left;

#if 0
			if (laps_left > 1 && demo_game == 0)
				{
				sprintf (buf, "%2d", laps_left);
				txt_str (lap_x, lap_y, buf, WHT_PAL);
				}
			else
				txt_str (lap_x, lap_y, "  ", WHT_PAL);
#endif
			break;

		case Cleanup:
			txt_str (lap_x, lap_y, "  ", WHT_PAL);
			break;
		}
}

