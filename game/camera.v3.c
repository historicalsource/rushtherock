/*
 *	camera.c -- handles camera controls.
 *
 *		Copyright 1996 Atari Games.
 *	Unauthorized reproduction, adaptation, distribution, performance or 
 *	display of this computer program or the associated audiovisual work
 *	is strictly prohibited.
 *
 *	======================================================================
 *	$Author: gray $						$Date: 1997/09/18 01:19:43 $
 *	$Revision: 3.35 $						$Locker: gray $
 *	======================================================================
 *	Change Log:
 *
 *	$Log: camera.c,v $
 * Revision 3.35  1997/09/18  01:19:43  gray
 * Keep car out of road for all views.
 *
 * Revision 3.34  1997/09/02  09:03:31  gray
 * Patch for view3 camera to prevent bad uvs.
 *
 * Revision 3.33  1997/08/30  03:00:53  gray
 * New view 3 - needs polishing.
 *
 * Revision 3.32  1997/05/13  20:46:23  gray
 * Coordinate utils name change.
 *
 * Revision 3.31  1997/05/13  18:28:35  gray
 * Moded coordinate conversion routines to math.c .
 *
 * Revision 3.30  1997/05/13  04:20:09  gray
 * Made update_rear_camera use dead reckoned uvs.
 *
 * Revision 3.29  1997/05/11  07:55:11  gray
 * Removed uvs from game car data (now everything uses dr_uvs).
 *
 * Revision 3.28  1997/05/11  04:37:27  gray
 * Cleaning model interface.
 *
 * Revision 3.27  1997/05/11  03:37:10  gray
 * Cleaning model interface.
 *
 * Revision 3.26  1996/12/14  06:18:57  geraci
 * tweaked view 3 some more
 *
 * Revision 3.25  1996/12/13  14:26:33  geraci
 * more alan-like view 3
 *
 * Revision 3.24  1996/11/27  17:02:29  geraci
 * camera is more loose in view 3
 *
 * Revision 3.23  1996/11/27  00:56:09  petrick
 * Changed objnum processing to handle objnum == 0.
 *
 * Revision 3.22  1996/11/22  05:05:21  geraci
 * fixed view 3 / reverse cam, hide resurrecting car from others
 *
 * Revision 3.21  1996/11/21  15:51:19  gray
 * Keep view1 above road.
 *
 * Revision 3.20  1996/11/21  03:57:06  geraci
 * view 2 tweaked
 *
 * Revision 3.19  1996/11/20  21:38:52  geraci
 * being of race pan to car
 *
 * Revision 3.18  1996/11/19  23:17:17  geraci
 * lots of new camera stuff
 *
 * Revision 3.17  1996/11/03  05:55:27  geraci
 * split up debug switches and regular game switches
 *
 * Revision 3.16  1996/11/02  09:11:10  geraci
 * new drone switch for debug stuff
 *
 * Revision 3.15  1996/11/01  06:27:12  geraci
 * put dead reckoning on camera
 *
 * Revision 3.14  1996/10/31  06:54:54  geraci
 * car data is in upper case now
 *
 * Revision 3.13  1996/10/30  01:48:45  geraci
 * fixed 'velocity on only up/down vector' bug
 *
 * Revision 3.12  1996/10/15  04:24:37  petrick
 * Cleanup.
 *
 * Revision 3.11  1996/10/10  05:19:24  petrick
 * Cleaned out some unused globals.
 *
 * Revision 3.10  1996/10/09  05:48:37  petrick
 * Cleanup.
 *
 * Revision 3.9  1996/10/03  06:59:38  petrick
 * Moved garage door camera more into view.
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
 * Revision 3.5  1996/09/25  02:08:58  petrick
 * Got rid of include file.
 *
 * Revision 3.4  1996/09/24  23:37:03  gray
 * ZGLU purge.
 *
 * Revision 3.3  1996/09/18  10:05:18  geraci
 * beginnings of car shake in view 2
 *
 * Revision 3.2  1996/09/10  21:31:08  geraci
 * new death cam
 *
 * Revision 3.1  1996/09/09  02:24:52  gray
 * JAMMA version
 *
 * Revision 2.29  1996/09/06  00:13:23  petrick
 * Got rid of swoop in attract mode.
 *
 * Revision 2.28  1996/09/04  20:34:54  gray
 * Adding dead reckon data to game_car. Killing mainout.
 *
 * Revision 2.27  1996/09/01  09:16:10  gray
 * Model types cleanup. First pass - removed fptyp.
 *
 * Revision 2.26  1996/08/29  00:10:09  petrick
 * Minor cleanup.
 *
 * Revision 2.25  1996/08/27  05:57:09  petrick
 * Camera view changes randomly during demo game.
 *
 * Revision 2.24  1996/08/18  21:24:01  gray
 * Removing zoid stuff. No more zoid.h.
 *
 * Revision 2.23  1996/08/18  03:31:48  gray
 * Removing ZOID stuff.
 *
 * Revision 2.22  1996/08/17  04:24:14  petrick
 * Can no longer change views when in demo game.
 *
 * Revision 2.21  1996/08/17  03:58:06  geraci
 * car does not go through world when upside down in view 2
 *
 * Revision 2.20  1996/08/02  05:02:10  petrick
 * Moved camera for view 2.
 *
 * Revision 2.19  1996/07/31  12:02:22  petrick
 * Updated view 2 for lower COG on car.
 *
 * Revision 2.18  1996/07/30  21:57:27  geraci
 * minor fix for drone training/viewing
 *
 * Revision 2.17  1996/07/30  11:49:00  petrick
 * Disabled view switching while in DeathCam.
 *
 * Revision 2.16  1996/07/27  08:17:19  geraci
 * camera for demo game
 *
 * Revision 2.15  1996/07/24  11:05:23  petrick
 * Lights up buttons.
 *
 * Revision 2.14  1996/07/12  12:26:30  petrick
 * New car select changes, workaround hack to get all stacks working.
 *
 * Revision 2.13  1996/07/08  08:24:47  petrick
 * Changed appearance and car body visual handling.
 *
 * Revision 2.12  1996/06/19  15:55:56  gray
 * still gatta have mbox.h until no ZOID.
 *
 * Revision 2.11  1996/06/19  15:51:06  gray
 * mbox.h now mb.h.
 *
 * Revision 2.10  1996/06/15  01:12:37  gray
 * Phoenix matrix stuff.
 *
 * Revision 2.9  1996/06/12  19:33:40  geraci
 * you're able to view any other car in the race using any view or new
 * surround cam view
 *
 * Revision 2.8  1996/05/23  06:05:49  geraci
 * more better quaternions
 *
 * Revision 2.7  1996/05/21  19:52:01  geraci
 * added length parameter to interpolation function
 *
 * Revision 2.6  1996/05/17  19:20:51  petrick
 * New visual handling.
 *
 * Revision 2.5  1996/05/14  09:38:11  geraci
 * no longer removing cars or creating mulitple ones
 *
 * Revision 2.4  1996/05/11  01:15:29  geraci
 * fixed view 1 and view 2
 *
 * Revision 2.3  1996/05/08  21:35:06  geraci
 * pulled camera in closer to 'right sized' car
 *
 * Revision 2.2  1996/05/08  08:57:08  petrick
 * Changed view 2 camera slightly.
 *
 * Revision 2.1  1996/05/04  02:56:18  petrick
 * Focus 2 release
 *
 * Revision 1.24  1996/05/02  01:08:13  geraci
 * start of view2 with fixed hood to screen
 *
 * Revision 1.23  1996/04/30  01:40:46  petrick
 * Better view 2 camera.
 *
 * Revision 1.22  1996/04/25  01:14:35  petrick
 * First cut at view 2 with hood.
 *
 * Revision 1.21  1996/04/24  02:57:21  geraci
 * resurrection code removed
 *
 * Revision 1.20  1996/04/20  07:09:40  geraci
 * adjusted rubberband for resurrections
 *
 * Revision 1.19  1996/04/16  23:03:44  petrick
 * Commented out unused stuff.
 *
 * Revision 1.18  1996/04/11  01:53:55  geraci
 * changed reverse function
 *
 * Revision 1.17  1996/04/02  10:21:56  petrick
 * Minor changes.
 *
 * Revision 1.16  1996/04/02  03:13:05  geraci
 * changed flicker when not collidable
 *
 * Revision 1.15  1996/04/01  13:55:11  geraci
 * flashes when not collidable
 *
 * Revision 1.14  1996/04/01  06:06:15  petrick
 * Made LookInDir non-static.
 *
 * Revision 1.13  1996/03/27  00:59:33  gray
 * Removed vehoff.
 *
 * Revision 1.12  1996/03/26  04:08:41  petrick
 * New explosion method.
 *
 * Revision 1.11  1996/03/21  06:43:31  petrick
 * Added explosion.
 *
 * Revision 1.10  1996/03/20  03:45:20  geraci
 * moved max_vel to here
 *
 * Revision 1.9  1996/03/14  17:46:14  gray
 * Start of zglue removal.
 *
 * Revision 1.8  1996/03/09  09:46:10  petrick
 * Consolidated car creation out of recdrones.
 *
 * Revision 1.7  1996/03/09  08:05:39  geraci
 * make camera move slower when in reverse
 *
 * Revision 1.6  1996/03/07  23:48:21  geraci
 * added reverse view switch
 *
 * Revision 1.5  1996/03/07  02:42:55  geraci
 * put car view stuff in
 *
 *
 *	======================================================================
 */

#include "globals.h"
#include "checkpoint.h"
#include "debug.h"
#include "dis.h"
#include "maxpath.h"
#include "mb.h"
#include "mdrive.h"
#include "model.h"
#include "os_proto.h"
#include "resurrect.h"
#include "visuals.h"

#include "Pro/camera.pro"
#include "Pro/math.pro"
#include "Pro/unitvecs.pro"
#include "Pro/vecmath.pro"

/*===============================  DEFINES  =================================*/

#define ANGCVT 			(PIANG/0x8000)
#define DEFAULT_VIEW 	3
#define MAX_VEL   		80
#define get_max(a,b)	((a>b)?a:b)
#define sign(a)			((a<0)?-1:1)

/*------------------------------  CONSTANTS  --------------------------------*/

const F32	wormoff_y		= (1.172);			/*	Up/down */
const F32	wormoff_z		= (3.906);			/* Forward/back */

const S32	driveroff_x		= (-0.547);			/* Left/right */
const S32	driveroff_y		= (3.203);			/*	Up/down */
const S32	driveroff_z		= (0.0);			/* Forward/back */

const F32	acc_elasticity	= .85;
const F32	acc_elasticity2	= .35;

const F32	fixed_uvs[3][3] = { {0, 1, 0}, {-1, 0, 0}, {0, 0, 1} };

/*------------------------------  PROTOTYPES  -------------------------------*/

static void	UpdateDroneObjects(void);
static F32	update_rear_camera (F32 ab, F32 AB, F32 *camoff);
static F32	view3_position (F32 ab, F32 AB, F32 *camoff);
static void	FollowDir(F32 backval, F32 lookdir[3], MATRIX *mat);
extern void bodtorw(register F32 *v, register F32 *vprime, struct uvect *uvst);
extern U32  ctl_mod_latch(int x);
extern void frwtobod(F32 *vprime, F32 *v, F32 *uvp);
extern void fbodtorw(F32 *v, F32 *vprime, F32 *uvp);
extern void fmatcopy(F32 *ap, F32 *bp);

/*-------------------------------  GLOBALS  ---------------------------------*/

MATRIX	 		fcam;
S8	 			view,saveview;
F32				cur_acc[3], rear_x, rear_y, old_rear_x, mcam[3], old_vec0;
S32				view_3_time, rear_view_time;

static F32		elasticity, elastic_factor;			/* 0 = rigid, 1 = wavy gravy */

/*-------------------------------  EXTERNS  ---------------------------------*/

extern F32		spring_save;
extern MPATH	*mpath;

/* ----------------------------------------------------------------------------- */
void init_view(void)
{
	S16  i, ppoint;
	F32  pos[3];

	init_view3();

	/* zoom-in-cam */
	view = 7;
	init_steady_move_cam();

}	/* end init_view */

/* ----------------------------------------------------------------------------- */
void init_view3(void)
{
	S16  i, ppoint;
	F32  pos[3];

	view = saveview = DEFAULT_VIEW;

	if (demo_game)
		{
		rear_x = 30;
		rear_y = 18;
		}
	else
		{
		rear_x = 16;
		rear_y = 6;
		}

	view_3_time = 0;
	rear_view_time = 0;
	old_vec0 = 0;
}	/* end init_view */

/* ----------------------------------------------------------------------------- */
void ZeroCamera(void)
{
	MBOX_UpdateCamera(CAMLINK0, zerovector, unituvs);
}

#define MAX_SELCAMPOS 3

/* ----------------------------------------------------------------------------- *
 *	Handles movement of camera during select screens.
 * ----------------------------------------------------------------------------- */
void SelectCam(void)
{
	MATRIX 		cam;
    F32 		rpos[3], pos[3], uvs[3][3], dc, p;
	S32			i;
	static BOOL	transition;
	static F32	dt, t;
	extern BOOL gCameraTransition;
	extern S16  gCurCam, gNextCam, gCamFrames;

	F32 gSelCamPos[MAX_SELCAMPOS][2][3] = 
	{
		/*	TRACK SELECT camera position	& look at positions. */
		/*         X      Z        Y               X      Z       Y */
		{	{  14.14,  9.14,  17.46 },		{  16.78,  9.59,  22.03 } }, 

		/*	CAR SELECT camera position. */
#if 0
		{	{ -28.32, 12.20, -15.01 },		{  29.16,  0.17,  16.16 } },
#elif 0	/* 75 degree view angle */
		{	{ -29.68, 13.30, -22.76 },		{   0.00,-20.00,   0.00 } },
#else	/* 45 degree view angle */
		{	{ -37.87, 16.50, -22.15 },		{   0.58,-13.70,   0.53 } },
#endif

		/*	DOOR camera position. */
#if 1
		{	{ -16.36, 10.92,  14.93 }, 		{ -37.80,  1.86,  24.07 } } 
#else	/* DEBUG POSITION */
		{	{     -8,    32,     14 },		{ -37.80,  7.86,  24.07 } } 
#endif
	};

#if 0
	SMSG( 10,1, "Eye %lf %lf %lf", gSelCamPos[0][0][0], gSelCamPos[0][0][1], gSelCamPos[0][0][2] );
#endif

	if (gCameraTransition)
		{
		gCameraTransition	= false;
		gNextCam 			= gCurCam + 1;
		gCamFrames 			= 20;
		dt 					= 1.0 / gCamFrames;
		t 					= 0;
		}

	/*	Interpolating */
	if (--gCamFrames >= 0)
		{
		t += dt;
		for (i=0; i<3; ++i)
			{
			cam.mat3.pos[i] = gSelCamPos[gCurCam][0][i] + t*(gSelCamPos[gNextCam][0][i] - gSelCamPos[gCurCam][0][i]);
			rpos[i]			= gSelCamPos[gCurCam][1][i] + t*(gSelCamPos[gNextCam][1][i] - gSelCamPos[gCurCam][1][i]) - cam.mat3.pos[i];
			}
		}
	/*	Not Interpolating */
	else
		{
		gCurCam = gNextCam;
		/* Point camera at target. */
		for(i=0;i<3;i++)
			rpos[i] = gSelCamPos[gCurCam][1][i] - (cam.mat3.pos[i] = gSelCamPos[gCurCam][0][i]);
		}

	LookInDir (rpos, &cam); 
	MBOX_UpdateCamera(CAMLINK0, cam.mat3.pos, cam.mat3.uvs);
}	/*	end SelectCam */

#if 1
/* ----------------------------------------------------------------------------- */
void UpdateCam(void)
{
	MBOX_UpdateCamera(CAMLINK0, zerovector, unituvs);
}
#endif

/* ----------------------------------------------------------------------------- */
void SetMCamera( S16 mode )
{
	S16 i,j;

	if (mode == Initialize)
		{
		gCamPos[0] = gCamPos[2] = 0.0;
		gCamPos[1] = 2.2;
		for(i=0;i<3;i++)
			for(j=0;j<3;j++)
				gCamUvs[i][j] = unituvs[i][j];
		}

	cur_acc[0] = cur_acc[1] = cur_acc[2] = 0;

}	/* end SetMCamera */


/* ----------------------------------------------------------------------------- *
 *	Check the buttons for a camera view change and changes view if needed.
 * ----------------------------------------------------------------------------- */
void CheckCameraView(void)
{
	static S32 	lastSwitch;
	S32  		new_view3, block, delta, extra_lag;
	U32	 		edge = edges;
	F32  		b, m, view3_lim;

	/*	Swap views every once in a while in demo mode. */
	if (demo_game)
		{
		edge  = 0;
		if (IRQTIME - lastSwitch > 3 * ONE_SEC)
			{
			extra_lag = 0;
			switch ((long)Random(11.0))
				{
				case 1:
					edge = SW_VIEW1;
					break;

				case 2:
					edge = SW_VIEW2;
					break;

				case 3:
					edge = SW_VIEW3;
					break;

				case 4:
					/* circle-cam */
					view = 5;
					extra_lag = 2 * ONE_SEC;
					break;

				case 5:
					/* fixed-direction-cam */
					edge = SW_VIEW3;
					init_view3();
					view = 8;
					break;

				case 6:
					/* maxpath-cam */
					view = 9;
					init_maxpath_cam();
					break;

				case 7:
					/* backwards-cam */
					edge = SW_VIEW3;
					init_view3();
					view = 10;
					break;

				default:
					/* fixed-on-track-cam */
					view = 6;
					init_camera_on_track();
					extra_lag = 2 * ONE_SEC;
					break;
				}

			lastSwitch = IRQTIME + extra_lag;
			}
		}

	if (view != 4)	/*	Not death cam and not in a demo.  */
		{
		if ((edge & SW_VIEW1) && view != 1)
			{
			view = 1;
			ctl_mod_latch( ~(LA_VIEW2|LA_VIEW3) );
			ctl_mod_latch( LA_VIEW1 );
			}
		else if ((edge & SW_VIEW2) && view != 2)
			{
			/*	Make the car into the hood object. */
			view = 2;
			ctl_mod_latch( ~(LA_VIEW1|LA_VIEW3) );
			ctl_mod_latch( LA_VIEW2 );
			cur_acc[0] = cur_acc[1] = cur_acc[2] = 0;
			}
		else if ((edge & SW_VIEW3) && view != 3)
			{		
			if ((view != 8 && view != 10) || demo_game == 0)
				view = 3;

			ctl_mod_latch( ~(LA_VIEW1|LA_VIEW2) );
			ctl_mod_latch( LA_VIEW3 );

			fcam.mat3.pos[0] = game_car[gThisNode].dr_pos[0];
			fcam.mat3.pos[1] = game_car[gThisNode].dr_pos[1] + 1.0;
			fcam.mat3.pos[2] = game_car[gThisNode].dr_pos[2];

			rear_x = 16;
			rear_y = 6;		
			}

		if ((levels & SW_VIEW3))
			{
			if (view_3_time == 0)
				{
				view_3_time = IRQTIME;
				old_rear_x = rear_x - 14;
				}
			else
				if (cheats && (IRQTIME - view_3_time > ONE_SEC))
					{
					view3_lim = 30;

					new_view3 = (IRQTIME - view_3_time - ONE_SEC) * view3_lim / 1800 + old_rear_x;

					block = new_view3 / view3_lim;
					delta = new_view3 - block * view3_lim;

					if (block & 1)
						delta = view3_lim - 1 - delta;

					m = (30 - 6) / (40 - 16);
					b = 6 - m * 16;
					delta += 14;

					rear_x = delta;
					rear_y = delta * m + b;
					}
			}
		else
			view_3_time = 0;
		}

	setcamview();

}	/* end CheckCameraView */

/* ----------------------------------------------------------------------------- */
S32 view1_suscomp_offset(MODELDAT *m)
{
	F32 avg_suscomp, moveup;

	/* worms eye between front tires - just look at front suscomp. */
	avg_suscomp = (	m->reckon.suscomp[0] + m->reckon.suscomp[1]) * 0.5;

	/* rest front suscomp avg = 0.3 ft*/
	moveup = avg_suscomp - 0.3;
	if(moveup > 0.0)
		return(moveup);
	else
		return(0);
}

#define BOTTOM_OUT (0.5)
F32 gSusAvg;
S32 gTireCnt;

/* ----------------------------------------------------------------------------- */
void setcamview(void)
{
	S32  		i, j, luv, t, cur_update_time, delta_time;
	F32		pos_in[3], pos_out[3];
	S32		state;
	CAR_DATA  	*car = &game_car[this_node];
	MODELDAT	*m = &model[this_node];
	F32		outdist;

    if (view < 1 || view > 10)
        view = DEFAULT_VIEW;
    if(view >= 1 && view <= 3)
    	saveview = view;

	if (dedges & SW_DEBUG3 && dlevels & SW_DEBUG1)
		/* bump target drone */
		{
		if (++target_drone >= num_active_cars)
			target_drone = 0;

		this_node = model[target_drone].slot;
		}

	for(i=0;i<3;i++)
		{
		gCamPos[i] = car->dr_pos[i];
		for(j=0;j<3;j++)
			gCamUvs[i][j] = car->dr_uvs[j][i];
		}

	/* figure out suspension to keep camera out of road. */
	for(i=0,gTireCnt=0,gSusAvg=0.0;i<4;i++)
		{
		if(m->suscomp[i] > 0.0)
			{
			gTireCnt++;
			gSusAvg += m->suscomp[i];
			}
		}
	/* only get suspension if 3 or 4 tires touching. */
	gSusAvg = (gTireCnt >= 3) ? gSusAvg/(F32)gTireCnt : 0.0;
	if(gSusAvg > BOTTOM_OUT)
		{
		/* offset in car up dir (normal to road). */
		outdist = gSusAvg - BOTTOM_OUT;
		outdist *= 1.5;	/* fudge to keep above washboard */
		for(i=0;i<3;i++)
			gCamPos[i] += (car->dr_uvs[i][1] * outdist);
		}


	blink_resurrecting_cars();

    switch(view)
		{
		case 1:	/* worms eye */
			pos_in[0] = 0.0;
/*			pos_in[1] = wormoff_y + view1_suscomp_offset(m);*/
			pos_in[1] = wormoff_y;
			pos_in[2] = wormoff_z;

			ftransvec(pos_in, pos_out, car->dr_uvs);

			for(i=0;i<3;i++)
				gCamPos[i] += pos_out[i];

			break;

		case 2:	/* drivers eye */
			/* use for temporary storage */
			for (j=0; j<2; j++)
				{
				/* limit all collision forces */
				if (m->peak_center_force[0][j] > 100000)
					m->peak_center_force[0][j] = 100000;

				if (m->peak_center_force[1][j] < -100000)
					m->peak_center_force[1][j] = -100000;

				if (m->peak_body_force[0][j] > 100000)
					m->peak_body_force[0][j] = 100000;

				if (m->peak_body_force[1][j] < -100000)
					m->peak_body_force[1][j] = -100000;

				/* adjust view offset due to collision */
				pos_in[j]  = m->peak_center_force[0][j] * .00002;
				pos_in[j] += m->peak_center_force[1][j] * .00002;
				m->peak_center_force[0][j] = 0;
				m->peak_center_force[1][j] = 0;

				pos_in[j] += m->peak_body_force[0][j] * .00002;
				pos_in[j] += m->peak_body_force[1][j] * .00002;
				m->peak_body_force[0][j] = 0;
				m->peak_body_force[1][j] = 0;
				}

			/* add acceleration offset to up/down view */
			pos_in[2] = car->dr_acc[1] * -0.003;
			if (pos_in[2] < -0.003)
				pos_in[2] = -0.003;

			/* smooth left/right and forward/back bumps */
			for (i=0; i<2; i++)
				cur_acc[i] = (cur_acc[i]*acc_elasticity + pos_in[i]*(1.0-acc_elasticity));

			/* smooth up/down bumps and add road rumble */
			cur_acc[2] = (cur_acc[2]*acc_elasticity2 + pos_in[2]*(1.0-acc_elasticity2)) - spring_save*0.04;

			/* now save offset to pos_in */
			pos_in[0] = -cur_acc[1] + driveroff_x;
			pos_in[1] =  cur_acc[2] + driveroff_y /* - spring_save*200 */;
			pos_in[2] = -cur_acc[0] + driveroff_z;

			ftransvec(pos_in, pos_out, car->dr_uvs);

			/* adjust y as car turns upside down */
/*			if (pos_out[1] < 0)*/
			if ((gSusAvg == 0.0) && (pos_out[1] < 0))
				gCamPos[1] -= (pos_out[1] * 0.7);

			/* put car object at camera coords */
			update_car_object (gCamPos, gCamUvs);

			/* update camera position to view world from */
			for(i=0;i<3;i++)
				gCamPos[i] += pos_out[i];

			break;

		case 3:	/* hover eye */
			View3Cam(gCamPos, gCamUvs);
			veccopy (fcam.mat3.pos, gCamPos);
			fmatcopy ((F32 *)fcam.mat3.uvs, (F32 *)gCamUvs);
			break;
		case 8: /* point camera in fixed direction */
		case 10: /* backward-cam */
			UpdateCarObj (gCamPos, gCamUvs);
			veccopy (fcam.mat3.pos, gCamPos);
			fmatcopy ((F32 *)fcam.mat3.uvs, (F32 *)gCamUvs);
			break;

		case 4:	/* death eye */
			DeathCam (gCamPos, gCamUvs);
			veccopy (fcam.mat3.pos, gCamPos);
			fmatcopy ((F32 *)fcam.mat3.uvs, (F32 *)gCamUvs);
			break;

		case 5: /* circle-cam */
			circle_camera_around_car (gCamPos, gCamUvs);
			veccopy (fcam.mat3.pos, gCamPos);
			fmatcopy ((F32 *)fcam.mat3.uvs, (F32 *)gCamUvs);
			break;

		case 6: /* fixed-cam */
			fix_camera_in_space (Do_it, gCamPos, gCamUvs);
			veccopy (fcam.mat3.pos, gCamPos);
			fmatcopy ((F32 *)fcam.mat3.uvs, (F32 *)gCamUvs);
			break;

		case 7: /* fixed-cam */
			steady_move_cam (Do_it, gCamPos, gCamUvs);
			veccopy (fcam.mat3.pos, gCamPos);
			fmatcopy ((F32 *)fcam.mat3.uvs, (F32 *)gCamUvs);
			break;

		case 9: /* fixed-cam */
			maxpath_cam (Do_it, gCamPos, gCamUvs);
			veccopy (fcam.mat3.pos, gCamPos);
			fmatcopy ((F32 *)fcam.mat3.uvs, (F32 *)gCamUvs);
			break;
		}

	MBOX_UpdateCamera (CAMLINK0, gCamPos, gCamUvs);

	/* save off camera position for listener position during demo game */
	if (demo_game)
		{
		state = prc_set_ipl (INTS_OFF);
		veccopy (gCamPos, mcam);
		prc_set_ipl (state);
		}

	UpdateDroneObjects();

}	/* end setcamview */

/* ----------------------------------------------------------------------------- */
static void UpdateDroneObjects(void)
{
	S16 		i,j,k;
	CAR_DATA  	*car;
	F32			pos[3],mat[3][3];

	for (i=0; i<MAX_LINKS; i++)
		if (model[i].in_game && game_car[i].objnum >= 0)
			{
			if (i == this_node || (i != this_node && model[i].hide_car))
				continue;

 			car = &game_car[i];

			for(j=0;j<3;j++)
				pos[j] = car->dr_pos[j];
				
			for (k = 0; k < 3; k++)
				for (j = 0; j < 3; j++)
					/*transpose*/
					mat[k][j] = car->dr_uvs[j][k];

			ZOID_UpdateObject(game_car[i].objnum,pos,mat);
			}

}	/* end UpdateDroneObjects */

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Draw car to screen*/

void update_car_object (F32 pos[3], F32 uvs[3][3])
{
	if (game_car[this_node].objnum >= 0)
		ZOID_UpdateObject (game_car[this_node].objnum,pos,uvs);
}

void shitcam(F32 res[3])
{
	F32 rel[3];

	rel[0] = 0.0;
	rel[1] = 6.0;
	rel[2] = -16.0;

	fbodtorw (rel, res, game_car[this_node].dr_uvs[0]);
}

#define V3_MAX_MAGVEL (100.0)
#define V3_MAX_STRECH (0.125)
#define V3_VEL_SCALE ((F32)((F32)V3_MAX_STRECH/(F32)V3_MAX_MAGVEL))
F32 glast_yuv[3];

/* */
#define BACK_SUSAVG (0.5)

/* ----------------------------------------------------------------------------- */
void View3Cam(F32 pos[3], F32 uvs[3][3])
{
	MODELDAT *m = &model[this_node];
	CAR_DATA *car = &game_car[this_node];
	S32	i,tcnt;
	F32	scale, magvel, rad, len;
	U32	dtime;
	static F32	f;	/* 0.0 = flymode, 1.0=backvec, else interpolate. */
	static U32	lasttime;
	F32 backvec[3], flyvec[3], camoff[3], cary[3];
	F32 rear_dist = rear_x;	/*16*/
	F32 up_dist = rear_y;	/*6*/
	F32 upscale;
	static F32 flyup[3];
	static F32 fu;
	BOOL on_road;
	static U32 flytime;
	static F32 lastx[3];

/*xxx*/
	update_car_object (pos, uvs);

	/* use velocity to scale distance from car. */
	magvel = magnitude(car->dr_vel);
	scale = (magvel > V3_MAX_MAGVEL) ? V3_MAX_STRECH : magvel * V3_VEL_SCALE;
	rad = fsqrt(rear_dist*rear_dist + up_dist*up_dist);
	rad *= (1.0 + scale);
	rear_dist *= (1.0 + scale);
	up_dist *= (1.0 + scale);

	on_road = gTireCnt >= 3;

	/* udpate interpolation value. */
	dtime = IRQTIME - lasttime;
	lasttime = IRQTIME;
	if(magvel < 20.0)
		f += dtime/1000.0;      /* one second transition to back view. */
	else if(susavg > 1.5)
		f += dtime/100.0;	/* fast transition to back view. */
	else if(susavg > 0.5)
		f += dtime/(100.0 + ((1.5 - susavg) * 900.0));	/* variable rate to back view. */
	else
		f -= dtime/1000.0;      /* one second transition to fly view. */
	if(f < 0.0)
		f = 0.0;
	else if(f > 1.0)
		f = 1.0;

	/* get back z vector. */
	for(i=0;i<3;i++)
		bvec[i] = -car->dr_uvs[i][2] * rear_dist;

	/* get fly z vector (if needed). */
	if(f < 1.0)
		{
		if(magvel < 1.0)	/* no velocity - use backvec. */
			CopyVector(bvec, fvec);
		else	/* use negitive velocity vector. */
			ScaleVector(car->dr_vel, -1.0/magvel, fvec);
		}

	/* copy or interpolate z vec. */
	if(f == 0.0)
		CopyVector(fvec, zvec);
	else if(f == 1.0)
		CopyVector(bvec, zvec);
	else
		interpolate(fvec, bvec, 3, 0.0, 1.0, f, zvec);

	/* get back y vector. */
	for(i=0;i<3;i++)
		bvec[i] = car->dr_uvs[i][1];

	/* get fly z vector (if needed). */
	if(f < 1.0)
		CopyVector(uvector, fvec);

	/* copy or interpolate z vec. */
	if(f == 0.0)
		CopyVector(fvec, yvec);
	else if(f == 1.0)
		CopyVector(bvec, yvec);
	else
		interpolate(fvec, bvec, 3, 0.0, 1.0, f, yvec);


	/* scale z vec */
	len = magnitude(zvec);
	if(len < 0.1)
		{
		for(i=0;i<3;i++)
			zvec[i] = -car->dr_uvs[i][2] * rear_dist;
		}
	else
		ScaleVector(zvec, rear_dist, zvec);

	/* cross yvec with zvec to get xvec. */
	CrossVector (fcam.matv.zuv, fcam.matv.xuv, fcam.matv.yuv);


	/*scale to view radius. */
	direction(camoff, camoff);
	ScaleVector(camoff, rad, camoff);

	/* set cam position */
	AddVector(pos, camoff, fcam.mat3.pos);

	/* get camz*/
	len = magnitude(camoff);
	ScaleVector(camoff, -1.0/len, fcam.matv.zuv);

#if 1
	CopyVector(flyup, fcam.matv.yuv);
#else
	/* get temp camy */
	for(i=0;i<3;i++)
		cary[i] = car->dr_uvs[i][1];
	if(f == 0.0)
		CopyVector(uvector, fcam.matv.yuv);
	else if(f > 0.0)
/*	else if(f == 1.0)*/
		CopyVector(cary, fcam.matv.yuv);
	else if((fcam.matv.zuv[1] > 0.7) || (fcam.matv.zuv[1] < -0.7))
		CopyVector(cary, fcam.matv.yuv);
	else
		{
		/* interpolate yuv*/
		ScaleVector(cary, f, cary);
		ScaleAddVector(uvector, 1.0-f, cary, fcam.matv.yuv);
		}
#endif

	/* get camx */
	CrossVector (fcam.matv.yuv, fcam.matv.zuv, fcam.matv.xuv);
	len = magnitude(fcam.matv.xuv);
	if(len < 0.001)
		CopyVector(lastx, fcam.matv.xuv);
	else
		{
		ScaleVector(fcam.matv.xuv, 1.0/len, fcam.matv.xuv);
		CopyVector(fcam.matv.xuv, lastx);
		}

	/* get real camy */
	CrossVector (fcam.matv.zuv, fcam.matv.xuv, fcam.matv.yuv);

#if 0
	CopyVector(rvector, fcam.matv.xuv);
	CopyVector(uvector, fcam.matv.yuv);
	CopyVector(fvector, fcam.matv.zuv);
#endif

}	/* end View3Cam */

#if 0
/* ----------------------------------------------------------------------------- */
void View3Cam(F32 pos[3], F32 uvs[3][3])
{
    S32 i,j;
	F32	rpos[3], carpos[3], fmat[3][3];
	F32 res[3];
	char buf[50];
	F32 interp = 0.0;

	update_car_object (pos, uvs);

	/* car position in floating point */
	for(i=0;i<3;i++)
		carpos[i] = pos[i];

	/* get offset relative to car */
	if (dlevels & SW_DEBUG4)
		update_rear_camera (-rear_x, rear_y, res);
	else
		interp = view3_position (rear_x, rear_y, res);

	if (model[this_node].resurrect.moving_state >= 0)
		elasticity = 0;
	else
		elasticity = .6 * elastic_factor;

elasticity = 0;

	for(i=0;i<3;i++)	/* follow the car */
		{
		fcam.mat3.pos[i] = (fcam.mat3.pos[i]*elasticity + (carpos[i]+res[i])*(1.0-elasticity));

		/* look at the car */
		/* dir_vect = dest_vect - source_vect */
		rpos[i] = carpos[i] - fcam.mat3.pos[i];
		}

	/* free body follow */
/*	LookInDir(rpos, &fcam);*/
	FollowDir(interp, rpos, &fcam);

}	/* end View3Cam */
#endif

/* ----------------------------------------------------------------------------- */
void UpdateCarObj (F32 pos[3], F32 uvs[3][3])
{
    S32 i,j;
	F32	rpos[3], carpos[3], fmat[3][3];
	F32 res[3];
	char buf[50];

	update_car_object (pos, uvs);

	/* car position in floating point */
	for(i=0;i<3;i++)
		carpos[i] = pos[i];

	/* get offset relative to car */
	if (dlevels & SW_DEBUG4 || view == 10)
		update_rear_camera (-rear_x, rear_y, res);
	else
		update_rear_camera (rear_x, rear_y, res);

	if (model[this_node].resurrect.moving_state >= 0)
		elasticity = 0;
	else
		elasticity = .6 * elastic_factor;

	for(i=0;i<3;i++)	/* follow the car */
		{
		fcam.mat3.pos[i] = (fcam.mat3.pos[i]*elasticity + (carpos[i]+res[i])*(1.0-elasticity));

		/* look at the car */
		/* dir_vect = dest_vect - source_vect */
		rpos[i] = carpos[i] - fcam.mat3.pos[i];
		}

	/* steady camera follow */
	LookInDir(rpos, &fcam);

}	/* end UpdateCarObj */



/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Update camera placement for view 3
*         Upon entry: ab -> distance from back of car to camera
*                     AB -> distance from bottom of car to camera
*                     camoff -> pointer to array of new camera offset    */

static F32 view3_position (F32 rear_dist, F32 up_dist, F32 *camoff)
{
	MODELDAT *m = &model[this_node];
	CAR_DATA *car = &game_car[this_node];
	S32	i,tcnt;
	F32	susavg, scale, magvel;
	F32	backvec[3], flyvec[3];
	U32	dtime;
	static F32	f;	/* 0.0 = flymode, 1.0=backvec, else interpolate. */
	static U32	lasttime;
/*xxx*/
F32 rad;

/* initialize properly later*/
if(lasttime == 0)
 CopyVector (uvector, glast_yuv);


	/* determine camera mode. */
	for(i=0,tcnt=0,susavg=0.0;i<4;i++)
		{
		if(m->suscomp[i] > 0.0)
			{
			tcnt++;
			susavg += m->suscomp[i];
			}
		}
	susavg = (tcnt >= 3) ? susavg/(F32)tcnt : 0.0;
	gSusAvg = susavg;

	/* udpate interpolation value. */
	dtime = IRQTIME - lasttime;
	lasttime = IRQTIME;
	if(susavg > 0.5)
		f += dtime/1000.0;
	else
		f -= dtime/1000.0;
	if(f < 0.0)
		f = 0.0;
	else if(f > 1.0)
		f = 1.0;


	/* use velocity to scale distance from car. */
	magvel = magnitude(car->dr_vel);
	scale = (magvel > V3_MAX_MAGVEL) ? V3_MAX_STRECH : magvel * V3_VEL_SCALE;
#if 0
	rear_dist += (scale * rear_dist);
	up_dist += (scale * 0.5 * up_dist);
#endif

rad = fsqrt(rear_dist*rear_dist + up_dist*up_dist);
rad *= (1.0 + scale);

	/* determine back pos if needed. */
	for(i=0;i<3;i++)
		backvec[i] = (car->dr_uvs[i][2] * -rear_dist) + (car->dr_uvs[i][1] * up_dist);

	direction( backvec, backvec);
	ScaleVector( backvec, rad, backvec);

	/* determine fly pos if needed. */
	if(f < 1.0)
		{
		if(magvel < 1.0)
			CopyVector(backvec, flyvec);	/* no velocity - use backvec. */
		else
			{
			/* use negitive velocity vector. */
			ScaleVector(car->dr_vel, -rear_dist/magvel, flyvec);
/*			ScaleAddVector(glast_yuv, up_dist, flyvec, flyvec);*/
			flyvec[1] += up_dist;
			direction(flyvec, flyvec);
			ScaleVector( flyvec, rad, flyvec);
			}
		}

	/* copy of interpolate cam pos. */
	if(f == 0.0)
		CopyVector(flyvec, camoff);
	else if(f == 1.0)
		CopyVector(backvec, camoff);
	else
		{
		interpolate(flyvec, backvec, 3, 0.0, 1.0, f, camoff);
		direction(camoff, camoff);
		ScaleVector(camoff, rad, camoff);
		}

	return(f);
}	/* end view3_position */

#if 0
static BOOL view3_position (F32 rear_dist, F32 up_dist, F32 *camoff)
{
	MODELDAT *m = &model[this_node];
	CAR_DATA *car = &game_car[this_node];
	S32	i,tcnt;
	F32	magvel, scale, susavg, hugvec[3], velvec[3], upvec[3];
	BOOL	on_road;
	static F32 last_pos[3];
	static F32 last_up[3];
/*xxx*/

	/* use velocity to scale distance from car. */
	magvel = magnitude(game_car[this_node].dr_vel);
	scale = (magvel > V3_MAX_MAGVEL) ? V3_MAX_STRECH : magvel * V3_VEL_SCALE;
	rear_dist += (scale * rear_dist);
	up_dist += (scale * 0.5 * up_dist);

	/* get camera hug position for high g situations. */
	for(i=0;i<3;i++)
		hugvec[i] = car->dr_uvs[i][2] * -rear_dist;

	/* get camera velocity position for flying. */
	if(magvel < 1.0)
		CopyVector(hugvec, velvec);	/* no velocity - use hugvec. */
	else
		{
		/* use negitive velocity vector. */
		direction(car->dr_vel, velvec);
		ScaleVector(velvec, -rear_dist, velvec);
		}

	/* determine in car is on road and the average suspension compression. */
	for(i=0,tcnt=0,susavg=0.0;i<4;i++)
		{
		if(m->suscomp[i] > 0.0)
			{
			tcnt++;
			susavg += m->suscomp[i];
			}
		}
	on_road = (tcnt >= 3);
	susavg = (on_road) ? susavg/(F32)tcnt : 0.0;
	gSusAvg = susavg;
	if(susavg < 0.0)
		susavg = 0.0;
	else if(susavg > 1.0)
		susavg = 1.0;


	/* interpolate between velvec and hug vec based on suscomp. */
	interpolate(velvec, hugvec, 3, 0.0, 1.0, susavg, camoff);

	/* add vertical component to camera position. */
	if(on_road)
		{
		/* offset in cars up direction. */
		for(i=0;i<3;i++)
			upvec[i] = car->dr_uvs[i][1] * up_dist;
		}
	else
		{
		/* flying - offset in world up direction. */
		upvec[0] = 0.0;
		upvec[1] = up_dist;
		upvec[2] = 0.0;
		interpolate(last_up, upvec, 3, 0.0, 60.0, 1.0, upvec);
		}
	CopyVector(upvec, last_up);

	AddVector(camoff, upvec, camoff);

	return(on_road);
}	/* end view3_position */
#endif

#if 0
static F32 view3_position (F32 rear_dist, F32 up_dist, F32 *camoff)
{
	MODELDAT *m = &model[this_node];
	CAR_DATA *car = &game_car[this_node];
	S32	i,tcnt;
	F32	magvel, scale, susavg, hugvec[3], velvec[3];
	BOOL	on_road;
	static F32 last_pos[3];
extern F32 dbgFogMinDist[];
extern F32 dbgFogMaxDist[];
/*xxx*/

#if 0
	rear_dist = dbgFogMaxDist[trackno] * 0.01;
	up_dist = dbgFogMinDist[trackno] * 0.01;
#endif


	/* get camera hug position for high g situations. */
	for(i=0;i<3;i++)
		hugvec[i] = (car->dr_uvs[i][1] * up_dist) + (car->dr_uvs[i][2] * -rear_dist);

	/* determine z (back) part of velocity position. */
	magvel = magnitude(game_car[this_node].dr_vel);
	scale = (magvel > V3_MAX_MAGVEL) ? 1.0 + V3_MAX_STRECH : 1.0 + (magvel * V3_VEL_SCALE);
	rear_dist *= scale;
	up_dist *= scale;
	if(magvel < 1.0)
		{
		/* no velocity - put directly behind car. */
		for(i=0;i<3;i++)
			velvec[i] = game_car[this_node].dr_uvs[i][2] * -rear_dist;
		}
	else
		{
		/* use negitive velocity vector. */
		direction(game_car[this_node].dr_vel, velvec);
		ScaleVector(velvec, -rear_dist, velvec);
		}

	/* determine y (up) part of velocity position. */
	/* if 3 or more tires on a surface, assume driving (not flying). */
	for(i=0,tcnt=0,susavg=0.0;i<4;i++)
		{
		if(m->suscomp[i] > 0.0)
			{
			tcnt++;
			susavg += m->suscomp[i];
			}
		}
	on_road = (tcnt >= 3);
	if(on_road)
		{
		/* offset in cars up direction. */
		for(i=0;i<3;i++)
			velvec[i] += car->dr_uvs[i][1] * up_dist;
		}
	else
		{
		/* flying - offset in world up direction. */
		velvec[1] += up_dist;
		}

	/* interpolate between velvec and hug vec based on suscomp. */
	susavg = (tcnt) ? susavg/(F32)tcnt : 0.0;
gSusAvg = susavg;
	if(susavg < 0.0)
		susavg = 0.0;
	else if(susavg > 1.0)
		susavg = 1.0;
	interpolate(velvec, hugvec, 3, 0.0, 1.0, susavg, camoff);

	/* position smoothing. */
	interpolate(last_pos, camoff, 3, 0.0, 10.0, 1.0, camoff);
	CopyVector(camoff, last_pos);

}	/* end view3_position */
#endif

/* ----------------------------------------------------------------------------- */
void DeathCam (F32 pos[3], F32 uvs[3][3])
{
    S32 i,j;
	F32	rpos[3], carpos[3], res[3];
	
	/* update car object */
	ZOID_UpdateObject(game_car[this_node].objnum,pos,uvs);

	/* car position in floating point */
	for(i=0;i<3;i++)
		carpos[i] = pos[i];

	elasticity = .13;

	/* get offset relative to car */
	update_rear_camera (30, 8, res);

	for(i=0;i<3;i++)	/* follow the car */
		fcam.mat3.pos[i] = (fcam.mat3.pos[i]*(1.0-elasticity) + (carpos[i]+res[i])*elasticity);

	/* look at the car */
	/* dir_vect = dest_vect - source_vect */
	for(i=0;i<3;i++)
		rpos[i] = carpos[i] - fcam.mat3.pos[i];

	/* steady camera follow */
	LookInDir(rpos, &fcam);
}	/* end DeathCam */

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Update rear view camera
*         Upon entry: ab -> distance from back of car to camera
*                     AB -> distance from bottom of car to camera
*                     camoff -> pointer to array of new camera offset    */

static F32 update_rear_camera (F32 ab, F32 AB, F32 *camoff)
{
	F32  vec[3], res[3], temp[3], A, a, B, b, fact, magvel, uvs[3][3];
	S16  fixed_cam, flag;
	char buf[60];
	
	fixed_cam = (dlevels & (SW_DEBUG1 | SW_DEBUG2)) == (SW_DEBUG1 | SW_DEBUG2) || view==8;

	/* get velocity of car */
	if (fixed_cam)
		magvel = 0;
	else
		{
		temp[0] = game_car[this_node].dr_vel[2];
		temp[1] = game_car[this_node].dr_vel[0];
		temp[2] = 0;

		magvel = magnitude (temp);
		}

	/* get camera position from directly behind car */
	if (magvel <= MAX_VEL)
		{
		vec[0] = 0;
		vec[1] = AB;
		vec[2] = -ab;

		if (fixed_cam)
			veccopy( vec, temp);
		else
			ftransvec((F32 *)vec, (F32 *)temp, (F32x3 *)game_car[this_node].dr_uvs);

		temp[1] = -fabs (temp[1]);
		}

	/* if car is moving a little, move camera due to velocity */
	if (magvel > 1)
		{
		/* death cam */
		if (view == 4)
			{
			vec[0] = -game_car[this_node].V[0];
			vec[1] = game_car[this_node].V[2];
			vec[2] = 0;

			ftransvec((F32 *)vec, (F32 *)res, (F32x3 *)game_car[this_node].dr_uvs);
			vecD2M(res);
			}
		else
			{
			/* prepare data for behind the car view */
			res[0] = game_car[this_node].dr_vel[0];
			res[1] = game_car[this_node].dr_vel[1];
			res[2] = game_car[this_node].dr_vel[2];

			if (ab < 0)
				res[1] = -res[1];

			frwtobod (res, vec, game_car[this_node].dr_uvs[0]);
			vecD2M(vec);

			flag = true;

			/* determine if camera is moving abruptly */
			if (magvel < 20)
				rear_view_time = 0;

			if (rear_view_time)
				{
				old_vec0 = vec[0];

				if (IRQTIME - rear_view_time > 5 * ONE_SEC)
					rear_view_time = 0;

				flag = false;
				}

			if (abs (vec[0] - old_vec0) > 5)
				{
				rear_view_time = IRQTIME;
				flag = false;
				}

			old_vec0 = vec[0];

			/* if set, okay to let camera just follow car direction */
			if (flag)
				vec[0] = fabs (vec[0]);

			vecM2D(vec);
			fbodtorw (vec, res, game_car[this_node].dr_uvs[0]);
			vecD2M(res);

			res[0] = -res[0];
			res[1] = -res[1];
			}

		fact = ab / magvel;
		scalmul (res, fact, res);

		a = res[2];
		b = fsqrt (res[0]*res[0] + res[1]*res[1]);

		if (b > .001)
			{
			A = a * AB / ab;

			fact = (b - A) / b;

			res[0] *= fact;
			res[1] *= fact;
			}

		B = b * AB / ab;

		res[2] = fabs (res[2]+B);

		vecM2D(res);

		/* if velocity is below max, scale using behind the car view */
		if (magvel < MAX_VEL)
			interpolate (temp, res, 3, 1, MAX_VEL, magvel, res);
		}
	else
		/* no velocity, just use behind the car view */
		{
		res[0] = temp[0];
		res[1] = temp[1];
		res[2] = temp[2];
		}

	if (magvel > MAX_VEL)
		elastic_factor = 0;
	else
		elastic_factor = 1 - magvel/MAX_VEL;

	if (fixed_cam)
		elastic_factor = 0;

	camoff[0] = res[0];
	camoff[1] = -res[1];
	camoff[2] = res[2];
}	/* end update_rear_camera */

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Point camera at target car
*         Upon entry: pos -> coords of car to point at
*                     uvs -> uvs of target car                               */

void circle_camera_around_car (F32 pos[3], F32 uvs[3][3])
{
	F32 res[3], ang;

	/* get offset relative to car */
	ang = 2 * 3.1415926535 * ((F32)(IRQTIME % (6 * ONE_SEC))/(6 * ONE_SEC));

	res[0] = fcos (ang) * 30;
	res[1] = 20;
	res[2] = fsin (ang) * 30;

	point_at_car (pos, uvs, res);
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Init camera on track                                                 */

void init_camera_on_track (void)
{
	S16  i, ppoint;
	F32  pos[3], uvs[3][3];

	ppoint = game_car[this_node].mpath.mpi;

	for (i=0; i<20; i++)
		ppoint = NextMaxPath (ppoint, 0);

	uvs[0][0] = 0;

	pos[0] =  mpath[ppoint].pos[1];
	pos[1] = -mpath[ppoint].pos[2] + 2.0 + Random(18.0);
	pos[2] =  mpath[ppoint].pos[0];

	fix_camera_in_space (Initialize, pos, uvs);
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Point camera at target car
*         Upon entry: mode -> Initialize = set fixed camera position
*                             Doit = update camera uvs
*                     pos -> coords of car to point at
*                     uvs -> uvs of target car                               */

void fix_camera_in_space (S16 mode, F32 pos[3], F32 uvs[3][3])
{
	F32     res[3];
	static  F32 cam_pos[3];

	if (mode == Initialize)
		veccopy (pos, cam_pos);
	else
		{
		vecsub (cam_pos, pos, res);
		point_at_car (pos, uvs, res);
		}
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Initialize steady-move-cam data                                      */

void init_steady_move_cam (void)
{
	S16  i, ppoint;
	F32  pos[3], uvs[3][3];

	ppoint = game_car[this_node].mpath.mpi;

	for (i=0; i<4; i++)
		ppoint = NextMaxPath (ppoint, 0);

	uvs[0][0] = 0;

	pos[0] =  mpath[ppoint].pos[1];
	pos[1] = -mpath[ppoint].pos[2] + 5.0;
	pos[2] =  mpath[ppoint].pos[0];

	steady_move_cam (Initialize, pos, uvs);
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Point camera at target car
*         Upon entry: mode -> Initialize = set fixed camera positions
*                             Doit = update camera uvs
*                     pos -> coords of car to point at
*                     uvs -> uvs of target car                               */

void steady_move_cam (S16 mode, F32 pos[3], F32 uvs[3][3])
{
	F32           res[3], pos2[3];
	static  S16   count;
	static  F32   cam_pos[3], delta[3];
	static  BOOL  leg2;

	if (mode == Initialize)
		{
		veccopy (pos, cam_pos);
		veccopy (pos, fcam.mat3.pos);
		veccopy (model[this_node].RWR, res);
		vecsub (pos, res, pos);
		LookInDir (pos, &fcam); 
		count = -1;
		leg2 = false;
		}
	else
		{
		if (count == 0 && leg2 == true)
			init_view3();
		else
			{
			if (count == -1)
				{
				count = 30;
				update_rear_camera (rear_x, rear_y, res);
				vecadd (pos, res, res);
				vecsub (res, cam_pos, delta);
				scalmul (delta, 4.0/(F32)count, delta);

				vecadd (res, delta, pos2);
				pos2[0] += delta[2];
				pos2[1] += 30;
				pos2[2] -= delta[0];

				scalmul (delta, 2, delta);
				vecsub (pos2, cam_pos, delta);
				scalmul (delta, 1.0/(F32)count, delta);
				}
			else if (count == 0)
				{
				count = 10;

				update_rear_camera (rear_x, rear_y, res);
				vecadd (pos, res, res);
				vecsub (res, cam_pos, delta);
				scalmul (delta, 1.0/(F32)count, delta);
				leg2 = true;
				}

			count--;
			vecadd (cam_pos, delta, cam_pos);
			vecsub (cam_pos, pos, res);
			point_at_car (pos, uvs, res);
			}
		}
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Initialize maxpath-cam data                                          */

void init_maxpath_cam (void)
{
	S16  i, ppoint;
	F32  pos[3], uvs[3][3];

	ppoint = game_car[this_node].mpath.mpi;

	for (i=0; i<20; i++)
		ppoint = PrevMaxPath (ppoint, 0);

	uvs[0][0] = 0;
	pos[0] =  ppoint;

	maxpath_cam (Initialize, pos, uvs);
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Point camera at target car
*         Upon entry: mode -> Initialize = set fixed camera positions
*                             Doit = update camera uvs
*                     pos -> coords of car to point at
*                     uvs -> uvs of target car                               */

void maxpath_cam (S16 mode, F32 pos[3], F32 uvs[3][3])
{
	F32           res[3], new_pos[3];
	static  S16   ppoint, prev_ppoint, count;
	static  F32   cam_pos[3], delta[3];

	if (mode == Initialize)
		{
		ppoint = pos[0];
		count = 0;
		}
	else
		{
		if (count == 0)
			{
			count = 2;

			cam_pos[0] =  mpath[ppoint].pos[1];
			cam_pos[1] = -mpath[ppoint].pos[2] + 10.0;
			cam_pos[2] =  mpath[ppoint].pos[0];

			prev_ppoint = NextMaxPath (ppoint, 0);

			new_pos[0] =  mpath[prev_ppoint].pos[1];
			new_pos[1] = -mpath[prev_ppoint].pos[2] + 10.0;
			new_pos[2] =  mpath[prev_ppoint].pos[0];

			ppoint = prev_ppoint;

			vecsub (new_pos, cam_pos, delta);
			scalmul (delta, 1.0/(F32)count, delta);
			}

		count--;
		vecadd (cam_pos, delta, cam_pos);
		vecsub (cam_pos, pos, res);
		point_at_car (pos, uvs, res);
		}
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Point camera at target car
*         Upon entry: pos -> coords of car to point at
*                     uvs -> uvs of target car
*                     res -> offset to place camera at                       */

void point_at_car (F32 pos[3], F32 uvs[3][3], F32 res[3])
{
    S32 i,j;
	F32	rpos[3], carpos[3], fmat[3][3];
	char buf[50];

	update_car_object (pos, uvs);

	/* car position in floating point */
	for(i=0;i<3;i++)
		carpos[i] = pos[i];

	if (model[this_node].resurrect.moving_state >= 0)
		elasticity = 0;
	else
		elasticity = .6 * elastic_factor;

	for(i=0;i<3;i++)	/* follow the car */
		{
		fcam.mat3.pos[i] = (fcam.mat3.pos[i]*elasticity + (carpos[i]+res[i])*(1.0-elasticity));

		/* look at the car */
		/* dir_vect = dest_vect - source_vect */
		rpos[i] = carpos[i] - fcam.mat3.pos[i];
		}

	/* steady camera follow */
	LookInDir (rpos, &fcam);
}

void SuckInDir(F32 lookdir[3], MATRIX *mat)
{
	CAR_DATA  	*car = &game_car[this_node];

	CopyVector (lookdir, mat->matv.zuv);
	direction (mat->matv.zuv, mat->matv.zuv);

	mat->matv.xuv[0] = car->dr_uvs[0][0];
	mat->matv.xuv[1] = car->dr_uvs[1][0];
	mat->matv.xuv[2] = car->dr_uvs[2][0];

	CrossVector (mat->matv.zuv,mat->matv.xuv,mat->matv.yuv);
	direction (mat->matv.yuv, mat->matv.yuv);

	CrossVector (mat->matv.yuv,mat->matv.zuv,mat->matv.xuv);
}

/* ----------------------------------------------------------------------------- *
 *	o lookdir[3] is the direction vector you want to face
 *	  it does not have to be a unit vector.
 *	o mat is the matrix you want to make into lookdir
 * ----------------------------------------------------------------------------- */
static void FollowDir(F32 backval, F32 lookdir[3], MATRIX *mat) 
{
/*xxx*/
	MATRIX	backmat, flymat;
	F32	fly_quat[4], back_quat[4], cur_quat[4];
	S32 i;
F32 tempuvs[3][3];
static F32 last_backval;

F32 cary[3];

CopyVector (lookdir, backmat.matv.zuv);
direction (backmat.matv.zuv, backmat.matv.zuv);

for(i=0;i<3;i++)
	cary[i] = game_car[this_node].dr_uvs[i][1];

if(backval == 0.0)
	CopyVector(uvector, backmat.matv.yuv);
else if(backval == 1.0)
	CopyVector(cary, backmat.matv.yuv);
else
	{
	/* interpolate yuv*/
	ScaleVector(cary, backval, cary);
	ScaleAddVector(uvector, 1.0-backval, cary, backmat.matv.yuv);
	}

CrossVector (backmat.matv.yuv, backmat.matv.zuv, backmat.matv.xuv);
direction (backmat.matv.xuv, backmat.matv.xuv);

/* get orthogonal up vector. */
CrossVector (backmat.matv.zuv, backmat.matv.xuv, backmat.matv.yuv);

fmatcopy((F32 *)backmat.mat3.uvs, (F32 *)mat->mat3.uvs);

return;

	if(backval > 0.0)
		{
		/* get normalized forward vector. */
		CopyVector (lookdir, backmat.matv.zuv);
		direction (backmat.matv.zuv, backmat.matv.zuv);

		/* use cars up vector to get normalized right vector. */
		for(i=0;i<3;i++)
			backmat.matv.yuv[i] = game_car[this_node].dr_uvs[i][1];
		CrossVector (backmat.matv.yuv, backmat.matv.zuv, backmat.matv.xuv);
		direction (backmat.matv.xuv, backmat.matv.xuv);

		/* get orthogonal up vector. */
		CrossVector (backmat.matv.zuv, backmat.matv.xuv, backmat.matv.yuv);
		}

	if(backval < 1.0)
		{
		CopyVector (lookdir, flymat.matv.zuv);
		direction (flymat.matv.zuv, flymat.matv.zuv);

		if(backval == 0.0)	/* pure fly */
			{
			/* transition car up to univ up. */
			glast_yuv[0] = glast_yuv[0] * 0.01;
			glast_yuv[1] = glast_yuv[1] + ((1.0 - glast_yuv[1]) * 0.01);
			glast_yuv[2] = glast_yuv[2] * 0.01;
			direction(glast_yuv, glast_yuv);
			
			/* cross univ straight up with look dir. */
			flymat.matv.yuv[0] = 0.0;
			flymat.matv.yuv[1] = (glast_yuv[1] >= 0.0) ? 1.0 : -1.0;
			flymat.matv.yuv[2] = 0.0;
			CrossVector (flymat.matv.yuv, flymat.matv.zuv, flymat.matv.xuv);
			direction (flymat.matv.xuv, flymat.matv.xuv);

			/* get orthogonal up vector. */
			CrossVector (flymat.matv.zuv, flymat.matv.xuv, flymat.matv.yuv);
			}
		else
			{
			/* transitioning to back val - maintian up dir. */
#if 0
			CopyVector( glast_xuv, flymat.matv.xuv);
			CrossVector (flymat.matv.zuv, flymat.matv.xuv, flymat.matv.yuv);
			direction (flymat.matv.yuv, flymat.matv.yuv);
			CrossVector (flymat.matv.yuv, flymat.matv.zuv, flymat.matv.xuv);
#else
			CopyVector( glast_yuv, flymat.matv.yuv);
			CrossVector (flymat.matv.yuv, flymat.matv.zuv, flymat.matv.xuv);
			direction (flymat.matv.xuv, flymat.matv.xuv);
			CrossVector (flymat.matv.zuv, flymat.matv.xuv, flymat.matv.yuv);
#endif
			}


		}

	if(backval == 1.0)
		fmatcopy((F32 *)backmat.mat3.uvs, (F32 *)mat->mat3.uvs);
	else if(backval == 0.0)
		fmatcopy((F32 *)flymat.mat3.uvs, (F32 *)mat->mat3.uvs);
	else
		{
#if 0
matcopyD2M(flymat.uvs, tempuvs);
make_quat_from_uvs(tempuvs, fly_quat);
matcopyD2M(backmat.uvs, tempuvs);
make_quat_from_uvs(tempuvs, back_quat);
find_best_quat(fly_quat, back_quat);
interpolate(fly_quat, back_quat, 4, 0.0, 1.0, backval, cur_quat);
make_uvs_from_quat(cur_quat, tempuvs);
matcopyM2D(tempuvs, mat->mat3.uvs);
#else
		make_quat_from_uvs(flymat.uvs, fly_quat);
		make_quat_from_uvs(backmat.uvs, back_quat);
		find_best_quat(fly_quat, back_quat);

		interpolate(fly_quat, back_quat, 4, 0.0, 1.0, backval, cur_quat);

		make_uvs_from_quat(cur_quat, mat->mat3.uvs);
#endif
		}

	if(backval > 0.0)
		CopyVector (mat->matv.yuv, glast_yuv);
	else
		{
		/* transition to up. */
		glast_yuv[0] = mat->matv.yuv[0] * 0.05;
		glast_yuv[1] = (mat->matv.yuv[1] * (1.0 - 0.05)) + 0.05;
		glast_yuv[2] = mat->matv.yuv[2] * 0.05;
		direction(glast_yuv, glast_yuv);
		CopyVector (uvector, glast_yuv);
		}

if(levels & SW_MUSIC)
 CopyVector (uvector, glast_yuv);
else
 CopyVector (mat->matv.yuv, glast_yuv);

	last_backval = backval;
}	/* end LookInDir */

#if 0
static void FollowDir(F32 backval, F32 lookdir[3], MATRIX *mat) 
{
/*xxx*/
	S32 i;
	F32 cur_quat[4];
	static F32 last_quat[4];

/*	if(backval == 1.0)*/
	if(1)
		{
		/* get normalized forward vector. */
		CopyVector (lookdir, mat->matv.zuv);
		direction (mat->matv.zuv, mat->matv.zuv);

		/* use cars up vector to get normalized right vector. */
		for(i=0;i<3;i++)
			mat->matv.yuv[i] = game_car[this_node].dr_uvs[i][1];
		CrossVector (mat->matv.yuv, mat->matv.zuv, mat->matv.xuv);
		direction (mat->matv.xuv, mat->matv.xuv);

		/* get orthogonal up vector. */
		CrossVector (mat->matv.zuv, mat->matv.xuv, mat->matv.yuv);
		}
	else	/* flying */
		LookInDir(lookdir, mat);

#if 0
	make_quat_from_uvs(mat->mat3.uvs, cur_quat);
	find_best_quat(cur_quat, last_quat);

	interpolate(last_quat, cur_quat, 4, 0.0, 10.0, 1.0, cur_quat);

	make_uvs_from_quat(cur_quat, mat->mat3.uvs);
#endif
}	/* end LookInDir */
#endif

/* ----------------------------------------------------------------------------- *
 *	o lookdir[3] is the direction vector you want to face
 *	  it does not have to be a unit vector.
 *	o mat is the matrix you want to make into lookdir
 * ----------------------------------------------------------------------------- */
void LookInDir(F32 lookdir[3], MATRIX *mat) 
{
	F32 length;

	CopyVector (lookdir, mat->matv.zuv);
	direction (mat->matv.zuv, mat->matv.zuv);
   
	mat->matv.xuv[0] = lookdir[2];
	mat->matv.xuv[1] = 0.0;
	mat->matv.xuv[2] = -lookdir[0];

	length = magnitude (mat->matv.xuv);

	if (length <= 0.1)
		CopyVector (rvector,mat->matv.xuv);

	direction (mat->matv.xuv, mat->matv.xuv);

	CrossVector (mat->matv.zuv,mat->matv.xuv,mat->matv.yuv);
	CrossVector (mat->matv.yuv,mat->matv.zuv,mat->matv.xuv);
}	/* end LookInDir */

