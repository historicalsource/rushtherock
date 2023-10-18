/*
 *	resurrect.c -- handles all things having to do with resurrections
 *
 *	Copyright 1996 Time Warner Interactive
 *	Unauthorized reproduction, adaptation, distribution, performance or 
 *	display of this computer program or the associated audiovisual work
 *	is strictly prohibited.
 *
 *	======================================================================
 *	$Author: gray $						$Date: 1997/10/01 22:12:23 $
 *	$Revision: 3.42 $						$Locker:  $
 *	======================================================================
 *	Change Log:
 *
 *	$Log: resurrect.c,v $
 * Revision 3.42  1997/10/01  22:12:23  gray
 * Heal any damage on abort.
 *
 * Revision 3.41  1997/09/26  09:37:29  gray
 * modify pole pos for mirror mode.
 *
 * Revision 3.40  1997/06/04  02:15:46  gray
 * Restructuring controls (gas,brake,clutch,etc) handling.
 *
 * Revision 3.39  1997/05/30  04:05:35  gray
 * mainout is history!
 *
 * Revision 3.38  1997/05/30  01:25:14  gray
 * Clean up crashflag stuff.
 *
 * Revision 3.37  1997/05/21  00:10:16  gray
 * caruvs finally exterminated.
 *
 * Revision 3.36  1997/05/13  20:48:49  gray
 * Removed UV from game car data (now everything uses dr_uvs).
 *
 * Revision 3.35  1997/05/11  07:55:11  gray
 * Removed uvs from game car data (now everything uses dr_uvs).
 *
 * Revision 3.34  1997/05/11  03:37:57  gray
 * Cleaning model interface.
 *
 * Revision 3.33  1997/02/15  01:48:46  geraci
 * fixed track 2 resurrect bug
 *
 * Revision 3.32  1997/01/24  02:17:55  geraci
 * new checkpoint scheme
 *
 * Revision 3.31  1996/12/13  12:06:29  geraci
 * more new lap stuff
 *
 * Revision 3.30  1996/12/13  08:16:20  geraci
 * new lap stuff
 *
 * Revision 3.29  1996/12/08  05:49:21  geraci
 * applied track width to only horizontal offset
 *
 * Revision 3.28  1996/12/04  01:58:30  geraci
 * axed some junk
 *
 * Revision 3.27  1996/12/02  03:33:42  geraci
 * fixed yank time when pressing reverse
 *
 * Revision 3.26  1996/11/27  17:07:18  geraci
 * with reverse pressed you get 3 more secs before being yanked
 *
 * Revision 3.25  1996/11/27  01:58:58  geraci
 * can't cheat after being resurrected at end of short cut
 *
 * Revision 3.24  1996/11/27  00:52:03  petrick
 * Changed objnum handling to account for objnum == 0.
 *
 * Revision 3.23  1996/11/25  03:47:03  geraci
 * drones maxpath index is correct after resurrect
 *
 * Revision 3.22  1996/11/22  05:02:15  geraci
 * reversed parameters in make uvs from quat, added hide car to are we solid
 *
 * Revision 3.21  1996/11/19  13:02:23  geraci
 * drones go around track in attract without snuffling
 *
 * Revision 3.20  1996/11/19  09:54:54  geraci
 * drones start at preset spots on each track during attract
 *
 * Revision 3.19  1996/11/17  11:46:43  geraci
 * fixed bad resurrections using average method bug
 *
 * Revision 3.18  1996/11/11  22:48:10  geraci
 * fixed auto-abort bug
 *
 * Revision 3.17  1996/11/03  06:02:03  geraci
 * cars in shortcut will be resurrected properly now.
 *
 * Revision 3.16  1996/11/02  00:15:19  geraci
 * added routines to tell if you're dead or dying and when you're solid
 *
 * Revision 3.15  1996/10/25  11:07:51  geraci
 * car data in caps, re-init reverb during resurrect
 *
 * Revision 3.14  1996/10/23  00:14:33  petrick
 * Made track_no a global.
 *
 * Revision 3.13  1996/10/22  09:38:53  geraci
 * crash times decrease by 1/2 sec till at one sec long
 *
 * Revision 3.12  1996/10/10  05:19:24  petrick
 * Cleaned out some unused globals.
 *
 * Revision 3.11  1996/10/09  02:21:35  geraci
 * during demo game drones are dropped from high up
 *
 * Revision 3.10  1996/10/05  09:19:45  geraci
 * abort blinks during car explosion
 *
 * Revision 3.9  1996/10/04  07:32:06  geraci
 * attract sound is now split into attract effects and attract music
 *
 * Revision 3.8  1996/09/28  01:36:36  gray
 * Made global camera pos and uvs (gCamPos gCamUvs).
 *
 * Revision 3.7  1996/09/27  03:26:51  gray
 * Made camera control local. Eliminated CamFunc.
 *
 * Revision 3.6  1996/09/26  00:23:30  gray
 * Killed ztypes.h
 *
 * Revision 3.5  1996/09/25  02:08:10  petrick
 * Got rid of include file.
 *
 * Revision 3.4  1996/09/20  02:16:23  geraci
 * gets better distance of death in short cuts
 *
 * Revision 3.3  1996/09/18  10:13:10  geraci
 * used new road codes for wrong way indicator and auto abort
 *
 * Revision 3.2  1996/09/17  09:00:56  geraci
 * crowd screams when you explode
 *
 * Revision 3.1  1996/09/09  02:24:52  gray
 * JAMMA version
 *
 * Revision 2.33  1996/09/06  00:26:54  geraci
 * new starting positions
 *
 * Revision 2.32  1996/09/05  08:13:04  geraci
 * new maxpath data structure (widths and hints)
 *
 * Revision 2.31  1996/09/04  20:37:22  gray
 * Adding dead reckon data to game_car. Killing mainout.
 *
 * Revision 2.30  1996/08/30  17:08:39  geraci
 * uses rwr instead of mainout for position
 *
 * Revision 2.29  1996/08/27  05:36:20  geraci
 * cleaned up some unused variables
 *
 * Revision 2.28  1996/08/20  10:33:16  geraci
 * target sounds during demo games are operator selectable
 *
 * Revision 2.27  1996/08/18  21:25:20  gray
 * Removing zoid stuff. No more zoid.h.
 *
 * Revision 2.26  1996/08/09  04:57:07  geraci
 * got rid of velocity averaging on resurrect (might go back in)
 * fixed it so that no one can be resurrected closer that half the distance
 * to the finish line
 *
 * Revision 2.25  1996/08/01  05:16:06  petrick
 * Fix for car staying hulk if abort is hit during explosion.
 *
 * Revision 2.24  1996/07/29  09:49:41  geraci
 * new routine to sync to last checkpoint
 *
 * Revision 2.23  1996/07/27  08:22:37  geraci
 * track width now looked at for resurrections
 *
 * Revision 2.22  1996/07/25  09:20:14  geraci
 * wrong way/direction arrows now work properly
 *
 * Revision 2.21  1996/07/20  04:42:31  geraci
 * drones don't abort if keeping pace with a slow human
 *
 * Revision 2.20  1996/07/20  00:19:44  gray
 * Slimmed timer interrrupt routine. wheelpos() and forcewheel() moved.
 *
 * Revision 2.19  1996/07/14  05:40:57  petrick
 * Added param to SetupCar.
 *
 * Revision 2.18  1996/07/13  07:00:54  geraci
 * made maxpath into one loop plus 2 checkpoints
 *
 * Revision 2.17  1996/07/08  08:24:47  petrick
 * Changed appearance and car body visual handling.
 *
 * Revision 2.16  1996/07/07  01:28:55  petrick
 * New car structure and knock down targets.
 *
 * Revision 2.15  1996/06/28  05:14:06  geraci
 * drone can request to be aborted
 *
 * Revision 2.14  1996/06/21  17:28:12  geraci
 * made resurrected car more fairly placed, discouraged aborting around track
 *
 * Revision 2.13  1996/06/17  23:13:21  geraci
 * drones can now be auto aborted if collisions are off
 *
 * Revision 2.12  1996/06/17  02:12:47  geraci
 * fixed resurrection bug where second lap would use first laps maxpath data
 *
 * Revision 2.11  1996/06/12  21:39:50  geraci
 * fixed camera swap for death view
 *
 * Revision 2.10  1996/05/30  06:01:19  petrick
 * Made hulkiness and translucency part of appearance.
 *
 * Revision 2.9  1996/05/29  02:21:21  geraci
 * new car bump sounds
 *
 * Revision 2.8  1996/05/28  21:06:42  geraci
 * will not feeb-abort car if in out-of-gas mode
 *
 * Revision 2.7  1996/05/24  22:20:32  geraci
 * fixed smooth quat resurrections by simplifying method
 * also fixed a resurrection bug
 *
 * Revision 2.6  1996/05/23  06:06:20  geraci
 * more better quaternions
 *
 * Revision 2.5  1996/05/21  21:15:56  petrick
 * Improved car resurrection visuals.
 *
 * Revision 2.4  1996/05/21  19:51:17  geraci
 * new quaternionized resurrection
 *
 * Revision 2.3  1996/05/17  19:25:23  petrick
 * New visuals handling.
 *
 * Revision 2.2  1996/05/14  09:31:34  geraci
 * no longer removing cars or creating mulitple ones
 *
 * Revision 2.1  1996/05/04  02:56:18  petrick
 * Focus 2 release
 *
 * Revision 1.9  1996/05/02  02:08:41  geraci
 * fix for init other stack cars
 *
 * Revision 1.8  1996/05/02  01:07:35  geraci
 * better non-collidable stuff
 *
 * Revision 1.7  1996/05/01  19:49:49  geraci
 * helped make resurrection placement more fair
 *
 * Revision 1.6  1996/04/30  03:39:36  petrick
 * Added translucent resurrect.
 *
 * Revision 1.5  1996/04/30  02:21:53  geraci
 * recenters steering wheel during resurrection
 *
 * Revision 1.4  1996/04/27  02:21:27  geraci
 * uses road uvs to resurrect car
 *
 * Revision 1.3  1996/04/25  01:16:16  petrick
 * Added hood to view 2
 *
 * Revision 1.2  1996/04/24  23:46:13  geraci
 * moved drone/car abort to here
 *
 * Revision 1.1  1996/04/24  02:54:35  geraci
 * Initial revision
 *
 *
 *	======================================================================
 */

#include "mb.h"
#include "typedefs.h"
#include "stdtypes.h"
#include "carsnd.h"
#include "checkpoint.h"
#include "config.h"      /* color & switches etc. */
#include "debug.h"
#include "dis.h"
#include "globals.h"
#include "maxpath.h"
#include "mdrive.h"
#include "model.h"
#include "os_proto.h"
#include "resurrect.h"
#include "sounds.h"

#include "Pro/drivsym.pro"
#include "Pro/fswheel.pro"
#include "Pro/math.pro"
#include "Pro/mathsubs.pro"
#include "Pro/select.pro"
#include "Pro/sounds.pro"
#include "Pro/unitvecs.pro"
#include "Pro/vecmath.pro"

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/

extern  PATHTYP     *path;
extern  U16			path_start, path_end, path_loop;

extern	MATRIX	 	fcam;
extern	S8 			view,saveview;

extern	S16			crash_delay;
extern	U32			number_checkpoints;
extern	Track		CP_Track[];
extern	S16			path_dist_index[];
extern	U16			path_dist[];
extern	S16			path_index[];
extern	S16			recenter_flag;
extern	MPATH		*mpath;
extern	S16			path_to_maxpath[];
extern	U16			maxpath_dist[];
extern	F32			gStrayAngle;
extern	F32			lap_loop_distance;
extern	S16			demo_checkpoint;
extern	Track_Data  track_data[];

extern  U32         ctl_mod_latch(int x);

extern void frwtobod(F32 *vprime, F32 *v, F32 *uvp);
extern void fbodtorw(F32 *v, F32 *vprime, F32 *uvp);


/*********************************************************************************/

char	buf[66];
F32		save_pos[MAX_LINKS][3];

const S16 major_axis[3][2] = { {1, 2}, {2, 0}, {0, 1} };

#if 1
/* first eight are for starting line, last eight are for resurrection */
/* real start offsets */
const F32 pole_pos_offset[MAX_LINKS*2][3] = {
    {POLEVDIST *  0.0,	POLEHDIST *  0.0,	0.0},
    {POLEVDIST *  0.0,	POLEHDIST *  1.0,	0.0},
    {POLEVDIST * -1.0,	POLEHDIST * -0.5,	0.0},
    {POLEVDIST * -1.0,	POLEHDIST *  0.5,	0.0},
    {POLEVDIST * -2.0,	POLEHDIST *  0.0,	0.0},
    {POLEVDIST * -2.0,	POLEHDIST *  1.0,	0.0},
    {POLEVDIST * -3.0,	POLEHDIST * -0.5,	0.0},
    {POLEVDIST * -3.0,	POLEHDIST *  0.5,	0.0},
    {POLEVDIST *  0.0,	POLEHDIST * -1.4,	0.0},
    {POLEVDIST *  0.0,	POLEHDIST * -1.0,	0.0},
    {POLEVDIST *  0.0,	POLEHDIST * -0.6,	0.0},
    {POLEVDIST *  0.0,	POLEHDIST * -0.2,	0.0},
    {POLEVDIST *  0.0,	POLEHDIST *  0.2,	0.0},
    {POLEVDIST *  0.0,	POLEHDIST *  0.6,	0.0},
    {POLEVDIST *  0.0,	POLEHDIST *  1.0,	0.0},
    {POLEVDIST *  0.0,	POLEHDIST *  1.4,	0.0}
};
#else
/* test start offsets */
const F32 pole_pos_offset[MAX_LINKS*2][3] = {
    {POLEVDIST *  0.0,	POLEHDIST *  0.0,	0.0},
    {POLEVDIST * -1.0,	POLEHDIST *  0.5,	0.0},
    {POLEVDIST * -1.0,	POLEHDIST * -0.5,	0.0},
    {POLEVDIST * -2.0,	POLEHDIST *  0.5,	0.0},
    {POLEVDIST * -2.0,	POLEHDIST * -0.5,	0.0},
    {POLEVDIST * -3.0,	POLEHDIST *  0.5,	0.0},
    {POLEVDIST * -3.0,	POLEHDIST * -0.5,	0.0},
    {POLEVDIST * -4.0,	POLEHDIST *  0.0,	0.0},
    {POLEVDIST * 0.0,	POLEHDIST * -1.4,	0.0},
    {POLEVDIST * 0.0,	POLEHDIST * -1.0,	0.0},
    {POLEVDIST * 0.0,	POLEHDIST * -0.6,	0.0},
    {POLEVDIST * 0.0,	POLEHDIST * -0.2,	0.0},
    {POLEVDIST * 0.0,	POLEHDIST *  0.2,	0.0},
    {POLEVDIST * 0.0,	POLEHDIST *  0.6,	0.0},
    {POLEVDIST * 0.0,	POLEHDIST *  1.0,	0.0},
    {POLEVDIST * 0.0,	POLEHDIST *  1.4,	0.0}
};
#endif

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Use resurrection data to place car
*         Upon entry: m -> Points to modeldat structure to be updated
*     note: model should not be running for this node during call            */

void resurrect_car (MODELDAT *m)
{
	F32			*dp, new_dist, scale, mag1, mag2, dist1, dist2, new_dist2;
	F32			pos[3], delta1[3], delta2[3], track_width, crash_time;
	F32			dx, dy, dz, x, y, z, angle, vel, half_dist, time_scale;
	F32			c_dist, dist, dist_e, dist_d, dist_bc, dist_b1, dist_b2;
	S16			i, j, k, close_index, t_fwd, t_back, laps, old_mpi;
	S16			node, uv1, uv2, index, p_index, old_collidable_state;
	S32			ipos[3], temp[3];
	F32			quat[4], uvs[3][3];
	const F32	*sp;
	CheckPoint	*chkpnt;
	Track_Data	*trck;
	CAR_DATA	*gc;

	node = m->net_node;
	gc = &game_car[node];

	/* if jumping to next checkpoint or initializing, just copy data */
	if (m->resurrect.crash_time == -1)
		{
		/* copy position and uvs */
		MOVMEM((char *)((F32 *)m->resurrect.save_pos), (char *)((F32 *)m->initin.pos), 12);
		MOVMEM((char *)((F32 *)m->resurrect.uvs), (char *)((F32 *)m->initin.uvs), 12*3);

		/* copy pole position offset */
		sp = &pole_pos_offset[m->initin.polepos][0];
		dp = &m->initin.offset[0];
#if 1
		dp[0] = sp[0];
		dp[1] = (gMirrorMode) ? -sp[1] : sp[1];
		dp[2] = sp[2];
#else
		for(i=0;i<3;i++)
			*dp++ = *sp++;
#endif


		/* disable move */
		m->resurrect.moving_state = -1;
		recenter_flag = false;

		syminit (m);

		if (demo_game)
			{
			/* get maxpath dist of last checkpoint */
			i = path_to_maxpath[path_dist_index[m->last_cp_index]];
			m->resurrect.velocity = mpath[i].speed;

			m->V[XCOMP] = m->resurrect.velocity * .4;

			for (j=0; j<4; j++)
				{
				m->TIREV[j][XCOMP] = m->resurrect.velocity * .4;
				m->tires[j].angvel = m->TIREV[j][XCOMP]/m->tires[j].tradius;
				}
			}

		multigo (node);
		}
	else
		/* calculate resurrection point and uvs */
		{
		old_collidable_state = gc->collidable;
		m->collidable = false;
		gc->collidable = false;
		gc->collide_time = IRQTIME;
		gc->collide_state = -1;
		gc->collide_count = -1;
		m->hide_car = true;

		/* if aren't resurrecting, set full projection else pull back some */
		if (old_collidable_state == true)
			time_scale = 1.1;
		else
			time_scale = .8;

		chkpnt = CP_Track[trackno].chk_point;
		trck = &track_data[trackno];

		/* we are somewhere between these two distances */
		dist1 = (F32)path_dist[path_dist_index[m->last_cp_index]];
		dist2 = (F32)path_dist[path_dist_index[m->last_cp_index+1]];

		/* set first index and save coords in (z*40,x*40,-y*40) format */
		j = path_dist_index[m->last_cp_index+1];
		c_dist = 1e20;
		x =  m->resurrect.save_pos[0] * 40;
		y = -m->resurrect.save_pos[1] * 40;
		z =  m->resurrect.save_pos[2] * 40;
		close_index = 0;

		for (i=path_dist_index[m->last_cp_index]; i<=j; i++)
			{
			k = path_index[i];
			dx = path[k].pos[1] - x;
			dy = path[k].pos[2] - y;
			dz = path[k].pos[0] - z;

			dist = dx*dx + dy*dy + dz*dz;

			if (dist < c_dist)
				{
				c_dist = dist;
				close_index = i;
				}
			}

		/* if we're in a short cut, use that data */
		if ((gc->shortcut_flags & S_CUT_MASK) && (gc->old_shortcut != -1))
			{
			index = gc->shortcut_flags & SC_DIST_MASK;

			if (gc->old_shortcut != index)
				veccopy (gc->dr_pos, gc->shortcut_pos);

			vecsub (gc->shortcut_pos, gc->dr_pos, delta1);
			dist = magnitude (delta1);

			/* get maxpath dist of last checkpoint */
			i = path_to_maxpath[path_dist_index[m->last_cp_index]];

			/* get distance using position in short cut */
			new_dist = dist1 + maxpath_dist[gc->last_mpath] - maxpath_dist[i] + index*100 + dist;

#if 0
			sprintf (buf, "scut  %d %g %d %d %d %g   ",
					 index, (double)dist, i,
					 gc->last_mpath, maxpath_dist[i], (double)new_dist
					 );
			txt_str (15, 13, buf, WHT_PALB);
#endif
			}

		/* are we within 200 feet of path point? */
		else if (c_dist < (F32)400*40*400*40)
			/* yes, use it */
			{
			k = path_index[close_index];
			t_back = get_prev_center (k, CP_Track[trackno].direction);
			t_fwd = get_next_center (k, CP_Track[trackno].direction);

			dx = path[k].pos[1] - x;
			dy = path[k].pos[2] - y;
			dz = path[k].pos[0] - z;

			dist_d = dx*dx + dy*dy + dz*dz;

			dx = path[t_back].pos[1] - x;
			dy = path[t_back].pos[2] - y;
			dz = path[t_back].pos[0] - z;

			dist_e = dx*dx + dy*dy + dz*dz;

			dx = path[k].pos[1] - path[t_back].pos[1];
			dy = path[k].pos[2] - path[t_back].pos[2];
			dz = path[k].pos[0] - path[t_back].pos[0];

			dist_bc = lsqrt (dx*dx + dy*dy + dz*dz);

			if (dist_bc < .0001)
				dist_b1 = -1;
			else
				{
				dist_b1 = (dist_bc + (dist_d-dist_e)/dist_bc) / 2;

				if ((dist_b1 > dist_bc) || (dist_b1 < 0))
					dist_b1 = -1;
				}

			dx = path[t_fwd].pos[1] - x;
			dy = path[t_fwd].pos[2] - y;
			dz = path[t_fwd].pos[0] - z;

			dist_e = dx*dx + dy*dy + dz*dz;

			dx = path[k].pos[1] - x;
			dy = path[k].pos[2] - y;
			dz = path[k].pos[0] - z;

			dist_d = dx*dx + dy*dy + dz*dz;

			dx = path[t_fwd].pos[1] - path[k].pos[1];
			dy = path[t_fwd].pos[2] - path[k].pos[2];
			dz = path[t_fwd].pos[0] - path[k].pos[0];

			dist_bc = lsqrt (dx*dx + dy*dy + dz*dz);

			if (dist_bc < .0001)
				dist_b2 = -1;
			else
				{
				dist_b2 = (dist_bc + (dist_d-dist_e)/dist_bc) / 2;

				if ((dist_b2 > dist_bc) || (dist_b2 < 0))
					dist_b2 = -1;
				}

			if ((   (dist_b2 != -1) && (dist_b1 != -1))
				|| ((dist_b2 == -1) && (dist_b1 == -1)))
				new_dist = path_dist[close_index];

			else if (dist_b1 != -1)
				new_dist = path_dist[close_index] - dist_b1/40;

			else
				new_dist = path_dist[close_index] + dist_b2/40;

#if 0
			sprintf (buf, "cls  %g %g %d %g %g     ",
					 (double)new_dist, (double)c_dist, path_index[close_index],
					 (double)dist1, (double)dist2
					 );
			txt_str (15, 13, buf, WHT_PALB);

			sprintf (buf, "%g %g     ",
					 (double)dist_b1/40, (double)dist_b2/40
					 );
			txt_str (15, 14, buf, WHT_PALB);
#endif
			} /* end: close to path point method */

		else
			/* no, use ratio method to find closest path point */
			{
			vecsub (m->resurrect.save_pos, chkpnt[m->last_cp_index].pos, delta1);
			vecsub (chkpnt[m->next_cp_index].pos, m->resurrect.save_pos, delta2);

			mag1 = magnitude (delta1);
			mag2 = magnitude (delta2);

			scale = mag1 + mag2;

			if (scale != 0.0)
				scale = mag1 / scale;

			/* get initial displacement due to crash position */
			new_dist = (dist2 - dist1) * scale + dist1;

#if 0
			sprintf (buf, "avg  %g %g %g %g %g %g   ",
					 (double)mag1, (double)mag2, (double)scale,
					 (double)dist1, (double)dist2, (double)new_dist
					 );
			txt_str (15, 13, buf, WHT_PALB);

			sprintf (buf, "%g %d %d     ",
					 (double)c_dist, path_index[close_index], close_index
					 );
			txt_str (15, 14, buf, WHT_PALB);
#endif
			} /* end: ratio method */

		m->resurrect.resurrect_time = IRQTIME;

		/* get maxpath close to dist1 */
		i = path_to_maxpath[path_dist_index[m->last_cp_index]];

		/* get delta from checkpoint and add maxpath dist at checkpoint */
		dist = new_dist - dist1 + maxpath_dist[i];

		/* find max path point closest to resurrect spot */
		j = find_maxpath_dist (i, dist);

		vel = mpath[j].speed;

		crash_time = m->resurrect.resurrect_time - m->resurrect.crash_time + RESURRECT_TIME;

		/* get (velocity+fudge) * time /ms = displacement from checkpoint */
		new_dist2 = new_dist + vel * time_scale * crash_time * .001;

		/* look ahead one checkpoint and determine if it is finish line */
		if (   m->last_cp_index == trck->before_finish
			&& trck->number_of_laps-1 == gc->laps)
			{
			/* get half the distance to the finish line */
			half_dist = (dist2 - new_dist) * .5 + new_dist;

			if (new_dist2 > half_dist)
				new_dist2 = half_dist;
			}

		new_dist = new_dist2;

#if 0
		sprintf (buf, "vel  %g %g %g   ",
				 (double)vel, (double)new_dist, (double)new_dist2
				 );
		txt_str (15, 15, buf, WHT_PALB);
#endif

		/* get delta from checkpoint and add maxpath dist at checkpoint */
		dist = new_dist2 - dist1 + maxpath_dist[i];

		/* find max path point closest to resurrect spot */
		k = find_maxpath_dist (i, dist);

		old_mpi = gc->mpath.mpi;

		/* save new maxpath index */
		set_maxpath_index (m, k);

		if (gc->mpath.mpi+100 < old_mpi)
			gc->mpath_laps++;

		m->resurrect.velocity = mpath[k].speed;

		if (new_dist > dist2)
			/* begin search at next checkpoint */
			index = path_dist_index[m->last_cp_index+1];
		else
			/* begin search at previous checkpoint */
			index = path_dist_index[m->last_cp_index];

		while (path_dist[index] < new_dist)
			index++;

		/* never resurrect behind last checkpoint */
		if (index == path_dist_index[m->last_cp_index])
			new_dist = path_dist[index-1];

		t_back = path_index[index-1];
		t_fwd = path_index[index];

#if 0
		sprintf (buf, "%d %d %g (%g %g %g)  ",
                 index-1, t_back, (double)new_dist,
				 (double)path[t_back].pos[1]/40,
				 (double)-path[t_back].pos[2]/40,
				 (double)path[t_back].pos[0]/40
				 );
		txt_str (15, 15, buf, WHT_PALB);

		sprintf (buf, "%d %d %g (%g %g %g)  ",
                 index, t_fwd, (double)new_dist,
				 (double)path[t_fwd].pos[1]/40,
				 (double)-path[t_fwd].pos[2]/40,
				 (double)path[t_fwd].pos[0]/40
				 );
		txt_str (15, 16, buf, WHT_PALB);
#endif

		/* now find center point between path points */
		delta1[0] = path[t_back].pos[0];
		delta1[1] = path[t_back].pos[1];
		delta1[2] = path[t_back].pos[2];
		delta2[0] = path[t_fwd].pos[0];
		delta2[1] = path[t_fwd].pos[1];
		delta2[2] = path[t_fwd].pos[2];

		interpolate (delta1, delta2, 3,
					 (F32)path_dist[index-1], (F32)path_dist[index], new_dist,
		             pos);

		m->resurrect.pos[0] = pos[0]/40;
		m->resurrect.pos[1] = pos[1]/40;
		m->resurrect.pos[2] = pos[2]/40 - 2;

		/* get track width */
		if (CP_Track[trackno].direction == -1)
			track_width = (F32)path[t_fwd].width * .013333;
		else
			track_width = (F32)path[t_back].width * .013333;

		ipos[0] = pole_pos_offset[m->initin.polepos][0];
		ipos[1] = pole_pos_offset[m->initin.polepos][1]*track_width;
		ipos[2] = pole_pos_offset[m->initin.polepos][2];

		if(gMirrorMode)
			ipos[1] = -ipos[1];

		if (CP_Track[trackno].direction == -1)
			Transvec (ipos, temp, &path[t_fwd].uvs[0]);
		else
			Transvec (ipos, temp, &path[t_back].uvs[0]);

		for (i=0; i<3; i++)
			m->resurrect.pos[i] += (F32)temp[i] * .025;

#if 0
		sprintf (buf, "%d %d %g (%g %g %g)  ",
                 path_dist[index-1], path_dist[index], (double)new_dist,
				 (double)m->initin.pos[1],
				 (double)-m->initin.pos[2],
				 (double)m->initin.pos[0]
				 );
		txt_str (15, 17, buf, WHT_PALB);


		sprintf (buf, "%ld %ld v=%ld (%g %g %g)   ",
				 m->resurrect.crash_time,
				 IRQTIME,
				 m->resurrect.velocity,
				 (double)m->resurrect.pos[0],
				 (double)m->resurrect.pos[1],
				 (double)m->resurrect.pos[2]
				 );
		txt_str (15, 18, buf, WHT_PALB);
#endif
		p_index = 0;

		/* get uv's from path data */
		if (CP_Track[trackno].direction == -1)
			for (i=0; i<3; i++)
				for (j=0; j<3; j++)
					{
					if (j == 2)
						m->resurrect.uvs[i][j] = (F32)path[t_fwd].fuvs[i][j];
					else
						m->resurrect.uvs[i][j] = -(F32)path[t_fwd].fuvs[i][j];
					}
		else
			for (i=0; i<3; i++)
				for (j=0; j<3; j++)
					m->resurrect.uvs[i][j] = (F32)path[t_back].fuvs[i][j];

#if 1
		matcopyD2M(gc->dr_uvs, uvs);
		make_quat_from_uvs (uvs, m->resurrect.quat_start);
#else
		make_quat_from_uvs (gc->UV.fpuvs, m->resurrect.quat_start);
#endif
		make_quat_from_uvs (m->resurrect.uvs, m->resurrect.quat_end);

		/* prepare quats for optimized interpolation */
		find_best_quat (m->resurrect.quat_start, m->resurrect.quat_end);

		if (new_dist > dist2)
			/* start at next checkpoint */
			PassedCP (m, false);

		/* initialize to first state */
		m->resurrect.moving_state = 0;
		gc->old_shortcut = -1;

#if 1
		MOVMEM((char *)(m->UV.fpuvs), (char *)(m->initin.uvs), sizeof(F32)*3*3);
#else
		MOVMEM((char *)(m->resurrect.uvs), (char *)(m->initin.uvs), sizeof(F32)*3*3);
#endif

		veccopy(m->RWR, m->initin.pos);

		/* copy pole position offset */
		dp = &m->initin.offset[0];
		for(i=0;i<3;i++)
			*dp++ = 0;

		if (node == gThisNode)
			recenter_flag = true;

		/* preserve velocity vector for VIEW 3 continuity */
		for (i=0; i<3; i++)
			pos[i] = m->RWV[i];

		syminit(m);

		/* fix tires */
		for (i=0; i<4; i++)
			{
			m->suscomp[i] = 0;
			m->BODYFORCE[i][1] = 0;
			m->sound_flags[i] = 0;
			m->shortcut_flags[i] = 0;
			}

		gc->sound_flags = 0;
		gc->sound_flag_time = 0;
		gc->shortcut_flags = 0;
		gc->scut_flag_time = 0;

		m->topscrape = 0;
		m->lastthump = m->thumpflag = 0;

		for (i=0; i<3; i++)
			m->RWV[i] = pos[i];
		}

	/*	Clean up its appearance. */
/*	m->appearance = 0; */
}	/* end resurrect_car */

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Create uv matrix from quaternion
*         Upon entry: q -> 4 element quaternion to use
*                     uvs -> pointer to 3x3 uv matrix to create              */

void make_uvs_from_quat (F32 q[], F32 uvs[][3])
{
	F32 Nq, q1, q2, q3, q4, q11, q22, q33, q44;
	F32 q12, q13, q14, q23, q24, q34;
	F32 s;

	q1 = q[0];      q2 = q[1];      q3 = q[2];      q4 = q[3];

	q11 = q1*q1;    q22 = q2*q2;    q33 = q3*q3;    q44 = q4*q4;
	q14 = q1*q4;    q13 = q1*q3;    q12 = q1*q2;
	q24 = q2*q4;    q23 = q2*q3;
    q34 = q3*q4;

	Nq = q11 + q22 + q33+ q44;
	s = (Nq > 0.0) ? (1.0 / Nq) : 0.0;

	uvs[0][0] = s * (q11 + q22 - q33 - q44);
	uvs[0][1] = s * (2 * (q23 - q14));
	uvs[0][2] = s * (2 * (q24 + q13));
	uvs[1][0] = s * (2 * (q23 + q14));
	uvs[1][1] = s * (q11 - q22 + q33 - q44);
	uvs[1][2] = s * (2 * (q34 - q12));
	uvs[2][0] = s * (2 * (q24 - q13));
	uvs[2][1] = s * (2 * (q34 + q12));
	uvs[2][2] = s * (q11 - q22 - q33 + q44);
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Create quaternion from uv matrix
*         Upon entry: uvs -> 3x3 source uv matrix to use
*                     q -> pointer to 4 element quaternion to be created     */

void make_quat_from_uvs (F32 uvs[][3], F32 q[])
{
	if (uvs[0][0] >= 0)
		{
		if (uvs[1][1] + uvs[2][2] >= 0)
			{
			q[0] = 1 + uvs[0][0] + uvs[1][1] + uvs[2][2];
			q[1] = uvs[2][1] - uvs[1][2];
			q[2] = uvs[0][2] - uvs[2][0];
			q[3] = uvs[1][0] - uvs[0][1];
			}
		else
			{
			q[0] = uvs[2][1] - uvs[1][2];
			q[1] = 1 + uvs[0][0] - uvs[1][1] - uvs[2][2];
			q[2] = uvs[1][0] + uvs[0][1];
			q[3] = uvs[0][2] + uvs[2][0];
			}
		}
	else
		{
		if (uvs[1][1] - uvs[2][2] >= 0)
			{
			q[0] = uvs[0][2] - uvs[2][0];
			q[1] = uvs[1][0] + uvs[0][1];
			q[2] = 1 - uvs[0][0] + uvs[1][1] - uvs[2][2];
			q[3] = uvs[2][1] + uvs[1][2];
			}
		else
			{
			q[0] = uvs[1][0] - uvs[0][1];
			q[1] = uvs[0][2] + uvs[2][0];
			q[2] = uvs[2][1] + uvs[1][2];
			q[3] = 1 - uvs[0][0] - uvs[1][1] + uvs[2][2];
			}
		}
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Make two quats close to each other
*         Upon entry: q1 -> pointer to first quat
*                     q2 -> pointer to second quat                           */

void find_best_quat (F32 *q1, F32 *q2)
{
	S16  i;
	F32  dq1, dq2;

	/* test for special reflective case using q2[0-3] and -q2[0-3] */
	/* note: q[0-3] and -q[0-3] are equivalent quats */
	dq1 = dq2 = 0;
	for (i=0; i<4; i++)
		{
		dq1 += abs (q1[i] - q2[i]);
		dq2 += abs (q1[i] + q2[i]);
		}

	/* the -q2 quat is closer to q1 than +q2*/
	if (dq2 < dq1)
		{
		for (i=0; i<4; i++)
			q2[i] = -q2[i];
		}
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Find normal vector to two vectors (cross product) and return angle
*         between two original vectors
*         Upon entry: vec1 -> pointer to 3 entry vector
*                     vec2 -> pointer to second vector
*                     nvec -> normal vector returned here
*         Upon exit: returned -> angle between original two vectors          */

F32 get_normal_vector (F32 *vec1, F32 *vec2, F32 *nvec)
{
	F32  dx, angle;
	S16  index, uv1, uv2;

	dx = dot_prod (vec1, vec2) / (magnitude (vec1) * magnitude (vec2));

	if (dx > 1)
		dx = 1;
	else if (dx < -1)
		dx = -1;

	if (dx < -.99)
		/* vectors are opposite of each other generate cooked crossprods */
		{
		/* get major axis in index and 2 other minor axes in uv1 and uv2 */
		index = get_major_axis (vec1);
		uv1 = major_axis[index][0];
		uv2 = major_axis[index][1];

		nvec[index] = vec1[uv1];
		nvec[uv1] = vec1[uv2];
		nvec[uv2] = -(vec1[uv1]+vec1[uv2]) / vec1[index];
		}
	else
		/* get cross product (will get vector normal to original vecs */
		crossprod (vec1, vec2, nvec);

	angle = facos (dx);

	if (dx < 0)
		angle = PI - angle;

	return (angle);
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Find major axis of passed vector
*         Upon entry: vec -> pointer to 3 entry vector
*         Upon exit: returned -> index of major axis                         */

S16 get_major_axis (F32 *vec)
{
	if ((fabs(vec[0]) > fabs(vec[1])) && (fabs(vec[0]) > fabs(vec[2])))
		return (0);
	else if (fabs(vec[1]) > fabs(vec[2]))
		return (1);
	else
		return (2);
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Get dot product of two vectors
*         Upon entry: vec1 -> points to first vector
                      vec2 -> points to second vector
*         Upon exit: returned -> dot product of two vectors                  */

F32 dot_prod (F32 *vec1, F32 *vec2)
{
	return (vec1[0]*vec2[0] + vec1[1]*vec2[1] + vec1[2]*vec2[2]);
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Interpolate between two points
*         Upon entry: pointa -> pointer to first point
*                     pointb -> pointer to second point
*                     p_size -> array size of pointa/pointb
*                     vala   -> value associated with pointa
*                     valb   -> value associated with pointb
*                     inab   -> value between vala and valb which will be used
*                               to determine interpolated point
*         Upon exit: pointab -> pointer to resulting point of interpolation  */

void interpolate (F32 *pointa, F32 *pointb, S16 p_size, F32 vala, F32 valb, F32 inab, F32 *pointab)
{
	F32  scale;
	S16  i;

	/* get scaling factor */
	scale = valb - vala;

	/* get % of dist: vala=0.0 and valb=1.0 */
	if (scale != 0.0)
		scale = (inab - vala) / scale;

	for (i=0; i<p_size; i++)
		pointab[i] = (pointb[i] - pointa[i]) * scale + pointa[i];
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Setup data for a non-linear scale of the form y = ax^2 + bx + c
*         Upon entry: a -> amount to skew data (0 = linear)
*                     b -> points to area in which save 1st degree term
*                     c -> points to area in which save constant term
*                     x1, y1 -> end point 1
*                     x2, y2 -> end point 2                                  */

void non_linear_scale_setup (F32 a, F32 *b, F32 *c, F32 x1, F32 y1, F32 x2, F32 y2)
{
	F32  dx;

	dx = x2 - x1;

	if (dx < .00001 && dx > -.00001)
		{
		*b = 0;
		*c = 0;
		return;
		}

	*b = (y2 - y1) / dx - a * (x1 + x2);

	*c = y2 - a * x2 * x2 - *b * x2;
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Initialize/set resurrection data
*         Upon entry: mode -> Initialize = set resurrection data to checkpoint
*                             Do_it = use current model data for res. point
*                     m -> Points to model_dat structure to be set        */

void set_resurrection_data (S16 mode, MODELDAT *m)
{
	S16			i, j, node, t_fwd, t_back, index;
	F32			*dp;
	S32			xv, yv, zv, last_cp_index;
	CheckPoint	*chkpnt = CP_Track[trackno].chk_point;

	last_cp_index = m->last_cp_index;

	if (m->net_node == this_node)
		init_reverb();

	switch (mode)
		{
		case Initialize:
			if (demo_game)
				{
				index = get_demo_path_point();
				t_fwd = path_index[index];
				t_back = path_index[index-1];
				veccopy (path[t_fwd].fpos, m->resurrect.save_pos);

				/* get uv's from path data */
				if (CP_Track[trackno].direction == -1)
					for (i=0; i<3; i++)
						for (j=0; j<3; j++)
							{
							if (j == 2)
								m->resurrect.uvs[i][j] = path[t_fwd].fuvs[i][j];
							else
								m->resurrect.uvs[i][j] = -path[t_fwd].fuvs[i][j];
							}
				else
					for (i=0; i<3; i++)
						for (j=0; j<3; j++)
							m->resurrect.uvs[i][j] = path[t_back].fuvs[i][j];

				m->last_cp_index = demo_checkpoint;
				m->next_cp_index = get_next_checkpoint (m->last_cp_index);

				index = path_to_maxpath[index];
				set_maxpath_index (m, index);
				}
			else
				{
				/* copy checkpoint pos in (z,x,-y) format */
				dp    =  (F32 *)m->resurrect.save_pos;
				*dp++ =  chkpnt[last_cp_index].pos[2];
				*dp++ =  chkpnt[last_cp_index].pos[0];
				*dp++ = -chkpnt[last_cp_index].pos[1] - 1;

				/* copy checkpoint uvs */
				dp = (F32 *)m->resurrect.uvs;
				*dp++ =  chkpnt[last_cp_index].uvs[2][2];
				*dp++ =  chkpnt[last_cp_index].uvs[0][2];
				*dp++ = -chkpnt[last_cp_index].uvs[1][2];
				*dp++ =  chkpnt[last_cp_index].uvs[2][0];
				*dp++ =  chkpnt[last_cp_index].uvs[0][0];
				*dp++ = -chkpnt[last_cp_index].uvs[1][0];
				*dp++ = -chkpnt[last_cp_index].uvs[2][1];
				*dp++ = -chkpnt[last_cp_index].uvs[0][1];
				*dp++ =  chkpnt[last_cp_index].uvs[1][1];

				sync_maxpath_to_last_checkpoint (m->net_node);
				}

			m->resurrect.crash_time = -1;

			break;

		case Do_it:
			/* save current position (in (x,y,z) format) and velocity */
			m->resurrect.save_pos[0] = m->RWR[YCOMP];
			m->resurrect.save_pos[1] = -m->RWR[ZCOMP];
			m->resurrect.save_pos[2] = m->RWR[XCOMP];

			/* save current time */
			m->resurrect.crash_time = IRQTIME;

			break;
		}
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Check to see if a car has just crashed and do explosion and resurrect
*         Upon entry: drone_index -> index of car to check                   */

void CheckCrash (S16 drone_index)
{
    S32 index, bIndex;

    if (game_car[drone_index].crashflag)
		{
		if (game_car[drone_index].crashtime == 0) 
			/* just crashed */
			{
			/* save current position and velocity */
			set_resurrection_data (Do_it, &model[drone_index]);

			if (drone_index == this_node)
				{
				saveview	= view;

				/* set to deathcam */
				view 		= 4;

				/* set camera position for death cam */
				fcam.mat3.pos[0] = gCamPos[0];
				fcam.mat3.pos[1] = gCamPos[1];
				fcam.mat3.pos[2] = gCamPos[2];

				if (!demo_game || (demo_game && attract_effects))
					{
					SOUND (S_EXPLO);
					sndPositionSound (S_BOOM, 0, 255);
					if (crash_delay > ONE_SEC)
						crash_delay -= ONE_SEC / 2;
					}
				}

			/*	lose the car */
			game_car[drone_index].just_crashed 	= 1;
			game_car[drone_index].crashtime 	= IRQTIME;
			StartBlast( drone_index );
			}

		if (drone_index == this_node)
			BlinkUnlessPressed (LA_ABORT, SW_ABORT);

		if (((IRQTIME - game_car[drone_index].crashtime) > crash_delay) /*|| (model[drone_index].magvel < 0.01)*/)
			{
			game_car[drone_index].we_died = true;

			if (drone_index == this_node)
				SOUND (S_KEXPLO);
			}
		}
    else
    	game_car[drone_index].crashtime = 0;

}	/* end CheckCrash */	


/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Handle cars that have arrived at their resurrection location         */

void check_if_finished_resurrecting (void)
{
	S16			i, j, node;
	MODELDAT	*m;

	for (i=0; i<num_active_cars; i++)
		{
		node = model[i].slot;
		m = &model[node];

		if (m->resurrect.moving_state == -2)
			{
			m->fastin.modelrun	= 0;
			m->resurrect.moving_state = -1;

			/* copy position and uvs */
			MOVMEM((char *)((F32 *)m->resurrect.pos), (char *)((F32 *)m->initin.pos), 12);
			MOVMEM((char *)((F32 *)m->resurrect.uvs), (char *)((F32 *)m->initin.uvs), 12*3);

			syminit(m);

			if (node == gThisNode)
				{
				recenter_flag = false;
				gSWforce = 0;
				frictwheel();
				}

			m->V[XCOMP] = m->resurrect.velocity * .4;

			for (j=0; j<4; j++)
				{
				m->TIREV[j][XCOMP] = m->resurrect.velocity * .4;
				m->tires[j].angvel = m->TIREV[j][XCOMP]/m->tires[j].tradius;
				}

			multigo (node);
			}
		}
}

S16  quat_slot;

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Move a resurrecting car the next step closer to its destination
*         note: this routine runs during model task
*         Upon entry: m -> points to model structure entry                   */

void update_resurrecting_car (MODELDAT *m)
{
	F32			pos[3], pos2[3], f32_state, temp, scale, uvs[3][3], quat[4];
	S16			i,j, s16_state, node = m->net_node;
	CAR_DATA	*gc = &game_car[m->net_node];

	/* initialize data for hand of god */
	if (m->resurrect.moving_state == 0)
		{
		/* get high point to bring car up to */
		vecsub (m->initin.pos, m->resurrect.pos, pos2);
		temp = magnitude (pos2) / 8;
		if (temp > 100)
			temp = 100;

		interpolate (m->initin.pos, m->resurrect.pos, 3, 0.0, RESURRECT_TIME, RESURRECT_TIME/2, save_pos[node]);

		save_pos[node][2] -= temp;
		m->appearance |= App_M_TRANSLUCENT;
		}

	m->resurrect.moving_state++;

	/* set current time offsets (float and int versions) */
	f32_state = s16_state = IRQTIME - m->resurrect.resurrect_time;

	/* we're done, shut off hand of god */
	if (s16_state > RESURRECT_TIME)
		{
		m->resurrect.moving_state = -2;

		if (node == this_node || m->we_control)
			m->hide_car = false;

		if (m->net_node == this_node)
			ctl_mod_latch (~LA_ABORT);
		}
	else
		{
		if (m->net_node == this_node)
			ctl_mod_latch (LA_ABORT);

		/*	Turn off hulk bit while being moved.  This takes care of the player hitting
		 *	ABORT before explosion turns car into hulk. */
		m->appearance &= ~App_M_HULK;
#if 0
		interpolate (m->initin.pos, m->resurrect.pos, 3, 0.0, RESURRECT_TIME, RESURRECT_TIME/2, pos2);
		pos2[2] -= save_dist[node];
#endif

		if (s16_state < RESURRECT_TIME/2)
			{
			scale = 2 * f32_state * f32_state / RESURRECT_TIME;

			interpolate (m->initin.pos, save_pos[node], 3, 0.0, RESURRECT_TIME/2, scale, pos);
			interpolate (m->resurrect.quat_start, m->resurrect.quat_end, 4, 0.0, RESURRECT_TIME/2, scale, quat);

			make_uvs_from_quat (quat, uvs);

			scale = 1 - 2 * scale / RESURRECT_TIME;

			if (node == this_node)
				/* adjust velocities for VIEW 3 */
				{
				for (i=0; i<3; i++)
					if (abs(model[this_node].RWV[i] *= scale) < .01)
						model[this_node].RWV[i] = 0;
				}

#if 0
			if (m->net_node == gThisNode)
				{
				char buf[60];

				for (i=0; i<3; i++)
					{
					sprintf (buf, "(%7.4f %7.4f %7.4f)   ",
							 (double)uvs[i][0],
							 (double)uvs[i][1],
							 (double)uvs[i][2]
							 );
					txt_str (15, 27+i, buf, WHT_PALB);
					}

				quat_slot = (quat_slot + 1) & 0x03;

				sprintf (buf, "(%7.4f %7.4f %7.4f %7.4f)   ",
						 (double)quat[0],
						 (double)quat[1],
						 (double)quat[2],
						 (double)quat[3]
						 );
				txt_str (15, 31+quat_slot, buf, WHT_PALB);
				}
#endif

			for(i=0;i<3;i++)
				for(j=0;j<3;j++)
					m->UV.fpuvs[i][j] = uvs[i][j];
			makesuvs(&m->UV);
			}
		else
			interpolate (save_pos[node], m->resurrect.pos, 3, RESURRECT_TIME/2, RESURRECT_TIME, f32_state, pos);

		for (i=0; i<3; i++)
			m->RWR[i] = pos[i];
		}
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Update transparent state of resurrecting cars                        */

void blink_resurrecting_cars()
{
    S16  		i;
    S32  		delta_time;
	CAR_DATA  	*gc;
	MODELDAT	*m;

	for (i=0; i<MAX_LINKS; i++)
		if (model[i].in_game && game_car[i].data_valid)
			{
			char buf[60];

			gc 	= &game_car[i];
			m	= &model[i];
#if 0
			sprintf (buf, "gc=%3d mc=%3d gcs=%3d gcc=%3d ms=%3d %7ld   ",
					 gc->collidable,
					 m->collidable,
					 gc->collide_state,
					 gc->collide_count,
					 m->resurrect.moving_state,
					 gc->collide_time);
			txt_str (5, 30+i, buf, WHT_PALB);
#endif

			if (gc->collidable)
				/* car is collidable, make solid if necessary */
				{
				if (!(gc->collide_state & 1) || gc->objnum < 0)
					{
					gc->collide_state |= 1;
					SetupCar (i, m->body_type, true);
					m->appearance &= ~(App_M_HULK | App_M_TRANSLUCENT | App_M_ANY_DAMAGE);
					}
				}
			else
				{
				/* get time since we were first non-collidable */
				delta_time = IRQTIME - gc->collide_time;

				/* if car is being resurrected, continually set time stamp */
				if (m->resurrect.moving_state > -1)
					{
					gc->collide_time = IRQTIME;
					delta_time = 0;
					gc->collide_count = 1;
					}

				/* if its been more than 5 secs, make only ourself collidable */
				if (delta_time > 5 * ONE_SEC)
					{
					if (i==this_node || m->we_control)
						{
						gc->collidable = true;
						gc->collide_time = 0;
						m->collidable = true;
						gc->collide_state |= 1;

						/* make it solid */
						m->appearance &= ~App_M_TRANSLUCENT;
						}
					}
				else
					/* still not-collidable */
					if (--gc->collide_count < 0)
						{
						if ((++gc->collide_state) & 1)
							/* every odd state transition is here */
							{
							gc->collide_count = 0;
							m->appearance &= ~App_M_TRANSLUCENT;
							}
						else
							/* even/first state */
							{
							switch (delta_time/1000)
								{
								case 0:
									gc->collide_count = 33;
									break;

								case 1:
									gc->collide_count = 8;
									break;

								case 2:
									gc->collide_count = 5;
									break;

								case 3:
									gc->collide_count = 2;
									break;

								default:
									gc->collide_count = 0;
									break;
								}

							m->appearance |= App_M_TRANSLUCENT;
							}
						}
				}
			}
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Abort car and initialize resurrection
*         Upon entry: drone_index -> index of car to abort                   */

void abort_car (S16 drone_index)
{
	/* do nothing if race started less than 5 seconds ago */
	if (GetElapsedTime() < 5 * ONE_SEC)
		return;

	/* heal any damage on abort. */
	model[drone_index].appearance &= ~(App_M_ANY_DAMAGE);

	set_resurrection_data (Do_it, &model[drone_index]);
	game_car[drone_index].we_died = true;
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Jump your car to next checkpoint                                     */

void jump_to_next_checkpoint(void)
{
	/*step to next checkpoint (for debug)*/
	JumpToNextCP();
	model[this_node].collidable = false;
	model[this_node].appearance |= App_M_TRANSLUCENT;
	game_car[this_node].collidable = false;
	game_car[this_node].collide_time = IRQTIME;
	game_car[this_node].collide_state = -1;
	game_car[this_node].collide_count = -1;
	set_resurrection_data (Initialize, &model[this_node]);

	game_car[this_node].we_died = true;
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*      Determine if any drones need to abort due to lack of forward progress */

void detect_drone_abort(void)
{
	S16			i, node, abort_me, index1, index2;
	F32			dist1, dist2, pos[3], pos2[3], delta[3], angle;
	S32			wait_time;
	MODELDAT	*m;
	CAR_DATA	*gc;

	/* do nothing if race started less than 5 seconds ago */
	if (GetElapsedTime() < 5 * ONE_SEC)
		{
		/* just clear related time stamps */
		for(i=0; i<MAX_LINKS; i++)
			{
			model[i].stuck_time = 0;
			game_car[i].fwd_progress_t = 0;
			}

		gStrayAngle = 0;

		return;
		}

	/*-----------------------------------------------------*/

    for(i=0; i<num_active_cars; i++)
		{
		node = model[i].slot;
		if (model[node].we_control || node==gThisNode)
			{
			m = &model[node];
			gc = &game_car[node];

			abort_me = m->please_abort_me;

			/* check if stuck */
			if ((   gstate==PLAYGAME || demo_game)
				&& (((dlevels & SW_DEBUG10)==0 || node!=gThisNode))
				&& (end_game_flag == false)
				&& (coast_flag == false)
				&& (gc->we_died == false)
				&& (m->crashflag == false)
				&& (gc->place_locked != 1)
				&& (m->resurrect.moving_state == -1)
				&& (m->collidable == true)
				&& (m->drone_scale > .1)
				&& (m->magvel<25 || abort_me==true))
				{
				if (abort_me == false)
					{
					if (m->stuck_time == 0)
						m->stuck_time = IRQTIME;
					else
						{
						wait_time = ((gc->shortcut_flags & YANK_MASK) >> YANK_SHIFT) + 1;

						if (levels & SW_REVERSE)
							wait_time++;

						/* if = 4 then never abort */
						if (wait_time != 4)
							{
							if (IRQTIME - m->stuck_time > 3*ONE_SEC * wait_time)
								abort_me = true;
							}
						else
							m->stuck_time = IRQTIME;
						}
					}

				if (abort_me)
					{
					set_resurrection_data (Do_it, m);
					gc->we_died = true;
					m->stuck_time = 0;
					}
				}
			else
				m->stuck_time = 0;
			}
		}

	/* now check if car in point-of-view needs help arrows */
	m = &model[this_node];
	gc = &game_car[this_node];

	if (gc->distance > gc->last_distance)
		{
		gc->fwd_progress_t = 0;
		gc->last_distance = gc->distance;
		gStrayAngle = 0;
		}
	else if (gc->we_died ||
			 m->crashflag ||
			 m->resurrect.moving_state!=-1 ||
			 m->collidable == false ||
			 (gc->shortcut_flags & NOWRONGWAY_MASK))
		{
		gc->fwd_progress_t = 0;
		gStrayAngle = 0;
		}
	else
		{
		if (gc->fwd_progress_t == 0)
			gc->fwd_progress_t = IRQTIME;
		else if (IRQTIME - gc->fwd_progress_t > ONE_SEC/2)
			{
			i = gc->mpath.mpi + 5;

#if 1
			/* get maxpath pos in display coord. */
			veccopyM2D(mpath[i].pos, pos2);

			/* save offset in relation to car in pos[] */
			vecsub (pos2, gc->dr_pos, delta);
			frwtobod (delta, pos2, gc->dr_uvs[0]);

			gStrayAngle = fatan2 (pos2[0], pos2[2]);
#else
			pos[1] = gc->dr_pos[0];   pos[2] = -gc->dr_pos[1];   pos[0] = gc->dr_pos[2];
			pos2[0] = mpath[i].pos[0];  pos2[1] = mpath[i].pos[1];   pos2[2] = mpath[i].pos[2];

			/* save offset in relation to car in pos[] */
			vecsub (pos2, pos, delta);
			rwtobod (delta, pos2, &gc->UV);

			gStrayAngle = fatan2 (pos2[1], pos2[0]);
#endif
			}
		}
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Is car dead, dying or resurrecting
*         Upon entry: node -> car in question
*         Upon exit: returned -> true = dead or dying etc.                   */

BOOL are_we_dead_or_dying (S16 node)
{
	CAR_DATA  *gc = &game_car[node];
	MODELDAT  *m  = &model[node];

	return ( (gc->we_died) ||
			 (m->crashflag) ||
			 (m->resurrect.moving_state > -1));
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Is car solid and not dead, dying or resurrecting
*         Upon entry: node -> car in question
*         Upon exit: returned -> true = car is solid and being counted       */

BOOL are_we_solid (S16 node)
{
	CAR_DATA  *gc = &game_car[node];
	MODELDAT  *m  = &model[node];

	return ( (gc->we_died == false) &&
			 (m->crashflag == false) &&
			 (m->resurrect.moving_state == -1) &&
			 (m->collidable == true) &&
			 (m->hide_car == false));
}

