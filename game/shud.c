/*
 *	hud.c	-- Functions for handling the Head's Up Display.
 *
 *	======================================================================
 *	$Author: gray $						$Date: 1997/10/10 01:59:53 $
 *	$Revision: 3.61 $						$Locker:  $
 *	======================================================================
 *	Change Log:
 *
 *	$Log: hud.c,v $
 * Revision 3.61  1997/10/10  01:59:53  gray
 * Keep hud from getting clipped by smoke and fogged.
 *
 * Revision 3.60  1997/10/01  03:44:04  grjost
 * Added mirror hud track maps.
 *
 * Revision 3.59  1997/09/30  02:20:06  grjost
 * Added new lap font
 *
 * Revision 3.58  1997/09/19  00:38:46  grjost
 * Did HUD track map for track 7.
 *
 * Revision 3.57  1997/09/17  07:53:20  grjost
 * Fixed HUD track maps for tracks 5 & 6.
 *
 * Revision 3.56  1997/09/15  22:30:23  gray
 * Removed trackset hack - trackno now 0 to NTRACKS.
 *
 * Revision 3.55  1997/08/13  21:01:28  gray
 * Made radio blit use max_radio constant.
 *
 * Revision 3.54  1997/07/01  18:58:32  gray
 * Made alternate tracks (additional 4 tracks) view 1 selectable.
 *
 * Revision 3.53  1997/06/28  01:41:43  gray
 * Extended to 4 tracks.
 *
 * Revision 3.52  1997/05/30  19:02:58  gray
 * Got rid of FLAGSTAFF conditionals.
 *
 * Revision 3.51  1997/05/29  21:08:03  gray
 * Clean up mph stuff.
 *
 * Revision 3.50  1997/05/13  19:18:40  gray
 * Use dead reckoned pos and uvs for radar map.
 *
 * Revision 3.49  1997/05/11  03:37:10  gray
 * Cleaning model interface.
 *
 * Revision 3.48  1997/02/22  02:07:27  gray
 * Make markers use dead reckoned pos.
 *
 * Revision 3.47  1996/11/28  04:35:13  petrick
 * Disabled radio if rank is on the screen.
 *
 * Revision 3.46  1996/11/27  20:44:40  geraci
 * fixed bad hiscore car blit problem
 *
 * Revision 3.45  1996/11/27  17:06:25  geraci
 * coords of finish line can be gotten without turning on flag
 *
 * Revision 3.44  1996/11/27  10:30:19  geraci
 * markers don't appear until initialized
 *
 * Revision 3.43  1996/11/27  00:55:22  petrick
 * Cleaned up marker hadnling a bit, got rid of tach needle code.
 *
 * Revision 3.42  1996/11/25  13:54:59  petrick
 * Got a little carried away with marker offing -- fixed.
 *
 * Revision 3.41  1996/11/25  04:12:03  petrick
 * Hid markers if game option sez so.
 *
 * Revision 3.40  1996/11/24  09:05:52  petrick
 * Fixes place vanishing problem.
 *
 * Revision 3.39  1996/11/23  02:52:18  geraci
 * made wrong way a global
 *
 * Revision 3.38  1996/11/22  20:13:38  petrick
 * Made remove hud safer.
 *
 * Revision 3.37  1996/11/22  15:51:52  petrick
 * Deactivated wrong way if not in playgame state.
 *
 * Revision 3.36  1996/11/22  05:04:33  geraci
 * flash resurrecting dot, don't show marker when resurrecting.
 *
 * Revision 3.35  1996/11/20  23:54:15  petrick
 * Added param to addjoinnow.
 *
 * Revision 3.34  1996/11/19  17:51:18  petrick
 * Centralized loading.
 *
 * Revision 3.33  1996/11/19  13:39:21  petrick
 * Dynamic texture loading.
 *
 * Revision 3.32  1996/11/18  11:52:43  petrick
 * Made markers permanent.
 *
 * Revision 3.31  1996/11/03  06:00:17  geraci
 * split off debug switches, hid players being resurrected on 2d maps
 *
 * Revision 3.30  1996/11/02  00:15:49  geraci
 * place does not display unless car is solid
 *
 * Revision 3.29  1996/11/01  12:24:37  geraci
 * drone and human types are now defines
 *
 * Revision 3.28  1996/10/31  07:44:14  petrick
 * Tied Checkpoint display to switch 6.
 *
 * Revision 3.27  1996/10/31  06:55:54  geraci
 * car data is in upper case now
 *
 * Revision 3.26  1996/10/31  05:00:54  petrick
 * Fixed size of radar display.
 *
 * Revision 3.25  1996/10/29  08:23:17  petrick
 * Stopped elapsed timer at end of game.
 *
 * Revision 3.24  1996/10/23  08:25:58  petrick
 * Bigger hud.
 *
 * Revision 3.23  1996/10/23  00:24:25  petrick
 * Made track_no a global.
 *
 * Revision 3.22  1996/10/22  10:00:08  geraci
 * fixed markers and dots
 *
 * Revision 3.21  1996/10/16  09:39:38  petrick
 * Markers complete, stuff hidden if solo, checkpoint time is now elapsed time.
 *
 * Revision 3.20  1996/10/15  04:25:08  petrick
 * Blit markers installed.
 *
 * Revision 3.19  1996/10/12  11:15:45  petrick
 * Added timer to Join Now, first cut at blit markers.
 *
 * Revision 3.18  1996/10/10  05:22:45  petrick
 * Cleaned out previously defined externs.
 *
 * Revision 3.17  1996/10/08  05:20:58  gray
 * Flagstaff port.
 *
 * Revision 3.16  1996/10/08  02:02:24  petrick
 * Cleanup.
 *
 * Revision 3.15  1996/10/07  03:05:27  petrick
 * Newer auto transmission blit.
 *
 * Revision 3.14  1996/10/05  09:59:09  petrick
 * Automatic shifter display.
 *
 * Revision 3.13  1996/10/05  06:28:15  geraci
 * hud only clear abort light once after it doesn't need it on any more
 *
 * Revision 3.12  1996/10/05  05:35:05  petrick
 * Moved mph to the right space.
 *
 * Revision 3.11  1996/10/04  23:58:22  petrick
 * New KPH/MPH blit.
 *
 * Revision 3.10  1996/10/04  10:40:04  petrick
 * Added Join Now func.
 *
 * Revision 3.9  1996/09/28  04:37:59  petrick
 * Made number functions relocate textures that may have been reloaded.
 *
 * Revision 3.8  1996/09/28  01:50:59  petrick
 * Made Crop Blit nonstatic.
 *
 * Revision 3.7  1996/09/27  02:12:13  petrick
 * If a number texture can't be found it looks for it again.
 *
 * Revision 3.6  1996/09/26  10:03:22  petrick
 * Changes for new high score screens.
 *
 * Revision 3.5  1996/09/25  06:15:39  petrick
 * Temporary check-in to make Alan happy.
 *
 * Revision 3.4  1996/09/14  11:20:46  geraci
 * demo game is now done in attract state
 *
 * Revision 3.3  1996/09/14  08:57:04  petrick
 * Fixed Elapsed time going to 0 at end of game.
 *
 * Revision 3.2  1996/09/14  08:40:10  petrick
 * Got rid of unused green digits.
 *
 * Revision 3.1  1996/09/09  02:24:52  gray
 * JAMMA version
 *
 *	======================================================================
 */

#include <stdarg.h>
#include "attract.h"
#include "debug.h"
#include "dis.h"
#include "game.h"
#include "globals.h"
#include "mb.h"
#include "modeldat.h"
#include "options.h"
#include "resurrect.h"
#include "sounds.h"

#include "Pro/mdrive.pro"
#include "Pro/sounds.pro"
#include "Pro/vecmath.pro"
#include "Pro/unitvecs.pro"
#include "Pro/select.pro"


/*===============================  DEFINES  =================================*/

#define DEG_TO_RADS		(PI / 180.0)
#define RAD_TO_DEGS		(180.0 / PI)

#define TCAR_H			8.0				/* H,W of Radar Map car blits including clear */	
#define TCAR_W			7.0
#define RADAR_X			SCR_L			/*	Position of radar on screen */
#define RADAR_Y			(SCR_T + 60)
#define RADAR_WIDE		64.0			/*	Width of radar frame */
#define RADAR_HIGH		256.0
#define RADAR_BWIDE		3.0				/*	Width of radar border */
#define RADAR_BHIGH		3.0
#define RADAR_MIN_X		(RADAR_X + RADAR_BWIDE)
#define RADAR_MAX_X 	(RADAR_X + RADAR_WIDE - RADAR_BWIDE - TCAR_W)
#define RADAR_MIN_Y 	(RADAR_Y + RADAR_BHIGH)
#define RADAR_MAX_Y 	(RADAR_Y + RADAR_HIGH - RADAR_BHIGH - TCAR_H)
#define RADAR_CENTER_X	(RADAR_WIDE/2.0)
#define RADAR_CENTER_Y	42.0

#define TRACK_OFFSET	5				/* Index of track number for TrackName */
#define TPART_OFFSET	7				/* Index of track part number TrackName */

#define TRAFFIC_NAME	"TRAFFIC"
#define DOT_NAME		"TMAPDOT"
#define STRAY_THRESHOLD (3.0 * PI / 8.0)	/* Radians of Stray allowed before arrow turns on. */


/*===============================  TYPEDEFS  ================================*/

typedef struct BlitList
{
	char 		Name[10];
	TexInfo		*ti;
	S32			TexIndex;
} BlitList;


/*==============================  PROTOTYPES  ===============================*/

static BlitFunc	AnimateTach, AnimateSpeed, AnimateNeedle, AnimateDot, 
				AnimateTraffic, AnimateRadio, AnimateShifter, AnimateCheckPt,
				AnimateElapsed, AnimatePlace, AnimateTrackMap, AnimateCTimer,
				AnimateRadar, AnimateFlag, AnimateBonus, AnimateWin, 
				AnimateMarker, AnimateLapDigit,
				AnimateWrongWay;

static void map_car_to_track(F32 x, F32 y, BLIT *blt);
static void set_track_mapping(BLIT *blt);
static void map_car_to_traffic(F32 x, F32 y, BLIT *blt);
static void	set_radar_mapping(F32 lx, F32 ly, F32 hx, F32 hy);

extern void frwtobod(F32 *vprime, F32 *v, F32 *uvp);
static void InitBlitList(BlitList list[], S16 num);
static void PickBlit(Blit *blit,BlitList list[], S16 index);

extern U32 ctl_mod_latch(int x);


/*===============================  GLOBALS  =================================*/

#if 0
static S32		gNeedleObj;	
#endif
S32				gHudMovie;
S16				mBlitIndex;
BOOL			wrong_way;

F32				gStrayAngle;			/* Angle of stray from desired course (radians). */

static S16		hud_mph, markers_valid;
static F32		gMTrackX, gMTrackY, gTTrackX, gTTrackY;
static F32		gMRadarX, gMRadarY, gTRadarX, gTRadarY;
static U32		gRadioTimeout;			/* Time when Radio display will go away. */
static U32 		gCheckPtTimeout;		/* Time when checkpoint delta time will go away. */	
U32 			gCheckPtTimeout2;
static S8		gRankList[MAX_LINKS], dot_order_list[MAX_LINKS];
F32		marker_pos[MAX_LINKS][3];
static char		TrackName[10];
static char 	TrackTimeStr[8], CountdownStr[8], CheckpointStr[8];
static BOOL		gFinishHid;
S16				gFinishX, gFinishY;

static BlitList gDigitList[NUM_DIGIT_TYPES];

static MULTIBLIT const HudMultiBlit[] = 
{	/* name			 		  x            y whtblrfz alpha animFunc            id */
	{"MARKERS",              0,            0, MUNUSED, 0x00,AnimateMarker,       0},
	 {"MARKERS",             0,            0, MUNUSED, 0x00,AnimateMarker,       1},
	 {"MARKERS",             0,            0, MUNUSED, 0x00,AnimateMarker,       2},
	 {"MARKERS",             0,            0, MUNUSED, 0x00,AnimateMarker,       3},
	 {"MARKERS",             0,            0, MUNUSED, 0x00,AnimateMarker,       4},
	 {"MARKERS",             0,            0, MUNUSED, 0x00,AnimateMarker,       5},
	 {"MARKERS",             0,            0, MUNUSED, 0x00,AnimateMarker,       6},
	 {"MARKERS",             0,            0, MUNUSED, 0x00,AnimateMarker,       7},
	{"NEWTACH15L", SCR_MH-  69,    SCR_T-  7, MUNUSED, 0x00,AnimateTach,         0},
	 {"NEWTACH15R",SCR_MH-69+128,  SCR_T-  7, MUNUSED, 0x00,AnimateTach,         1},
	 {"MPH",        SCR_MH- 15,    SCR_T+ 20, MUNUSED, 0x00,AnimateTach,         2},
	 {"DOT",      SCR_MH- 32/2,    SCR_T+ 50, MUNUSED, 0x00,AnimateNeedle,       0},
	{"DIGIT0", 	  SCR_MH-   33,    SCR_T+  0, MUNUSED, 0x00,AnimateSpeed,     0x00},
	 {"DIGIT0",	  SCR_MH-   11,    SCR_T+  0, MUNUSED, 0x00,AnimateSpeed,     0x01},
	 {"DIGIT0",	  SCR_MH+   11,    SCR_T+  0, MUNUSED, 0x00,AnimateSpeed,     0x02},
	{DOT_NAME,               0,            0, MUNUSED, 0x80,AnimateTrackMap,     1},
	 {DOT_NAME,              0,            0, MUNUSED, 0x80,AnimateTrackMap,     2},
	{DOT_NAME,               0,            0, MUNUSED, 0x00,AnimateDot,          0},
	 {DOT_NAME,              0,            0, MUNUSED, 0x00,AnimateDot,          1},
	 {DOT_NAME,              0,            0, MUNUSED, 0x00,AnimateDot,          2},
	 {DOT_NAME,              0,            0, MUNUSED, 0x00,AnimateDot,          3},
	 {DOT_NAME,              0,            0, MUNUSED, 0x00,AnimateDot,          4},
	 {DOT_NAME,              0,            0, MUNUSED, 0x00,AnimateDot,          5},
	 {DOT_NAME,              0,            0, MUNUSED, 0x00,AnimateDot,          6},
	 {DOT_NAME,              0,            0, MUNUSED, 0x00,AnimateDot,          7},
	{"MAPFRAME1",	   RADAR_X,      RADAR_Y, MUNUSED, 0x00,AnimateRadar,        1},
	 {"MAPFRAME2",	 1+RADAR_X,   32+RADAR_Y, MUNUSED, 0x00,AnimateRadar,        2},
	 {"MAPFRAME2",  60+RADAR_X,   32+RADAR_Y, MUNUSED, 0x00,AnimateRadar,        3},
	 {"MAPFRAME3",	   RADAR_X,  240+RADAR_Y, MUNUSED, 0x00,AnimateRadar,        4},
	 {TRAFFIC_NAME,          0,            0, MUNUSED, 0x00,AnimateTraffic,      0},
	 {TRAFFIC_NAME,	         0,            0, MUNUSED, 0x00,AnimateTraffic,      1},
	 {TRAFFIC_NAME,	         0,            0, MUNUSED, 0x00,AnimateTraffic,      2},
	 {TRAFFIC_NAME,		     0,            0, MUNUSED, 0x00,AnimateTraffic,      3},
	 {TRAFFIC_NAME,		     0,            0, MUNUSED, 0x00,AnimateTraffic,      4},
	 {TRAFFIC_NAME,		     0,            0, MUNUSED, 0x00,AnimateTraffic,      5},
	 {TRAFFIC_NAME,		     0,            0, MUNUSED, 0x00,AnimateTraffic,      6},
	 {TRAFFIC_NAME,		     0,            0, MUNUSED, 0x00,AnimateTraffic,      7},
	{"SHIFTER",      SCR_R- 50,    SCR_B- 64, MUNUSED, 0x00,AnimateShifter,     -1},
	{"DIGIT9",       SCR_R-105,    SCR_T+  0, MUNUSED, 0x00,AnimateCTimer,    0x90},
	 {"DIGIT9",      SCR_R- 72,    SCR_T+  0, MUNUSED, 0x00,AnimateCTimer,    0x91},
	 {"DIGIT9",      SCR_R- 39,    SCR_T+  0, MUNUSED, 0x00,AnimateCTimer,    0x92},
	{"OUTOF8",       SCR_R- 37,    SCR_T+ 40, MUNUSED, 0x00,AnimatePlace,        1},
	 {"DIGIT8",      SCR_R- 55,    SCR_T+ 40, MUNUSED, 0x00,AnimatePlace,     0x80},
    {"TIMETOP",      SCR_L+  1,    SCR_T+  0, MUNUSED, 0x80,nil,                 0},
	 {"DIGIT1",      SCR_L+  2,    SCR_T+  0, MUNUSED, 0x00,AnimateElapsed,   0x10},
	 {"DIGIT1",      SCR_L+ 17,    SCR_T+  0, MUNUSED, 0x00,AnimateElapsed,   0x11},
	 {"DIGIT1",      SCR_L+ 27,    SCR_T+  0, MUNUSED, 0x00,AnimateElapsed,   0x12},
	 {"DIGIT2",      SCR_L+ 45,    SCR_T+  6, MUNUSED, 0x00,AnimateElapsed,   0x23},
	 {"DIGIT2",      SCR_L+ 54,    SCR_T+  6, MUNUSED, 0x00,AnimateElapsed,   0x24},
	{"TIMEBOT",      SCR_L- 25,    SCR_T+ 27, MUNUSED, 0x80,AnimateCheckPt,      6},
 	 {"DIGIT3",      SCR_L+ 11,    SCR_T+ 27, MUNUSED, 0x00,AnimateCheckPt,   0x30},
	 {"DIGIT3",      SCR_L+ 28,    SCR_T+ 27, MUNUSED, 0x00,AnimateCheckPt,   0x31},
	 {"DIGIT3",      SCR_L+ 38,    SCR_T+ 27, MUNUSED, 0x00,AnimateCheckPt,   0x32},
	 {"DIGIT4",      SCR_L+ 56,    SCR_T+ 33, MUNUSED, 0x00,AnimateCheckPt,   0x43},
	 {"DIGIT4",      SCR_L+ 65,    SCR_T+ 33, MUNUSED, 0x00,AnimateCheckPt,   0x44},
	{"HUDARROW",     SCR_MH-128,   SCR_MV-32, MUNUSED, 0x60,AnimateWrongWay,     0},
	 {"HUDARROW",    SCR_MH+130,   SCR_MV-32, MUNUSED, 0x60,AnimateWrongWay,     1},
	 {"HUDARROW",    SCR_MH-150,   SCR_MV-32, MUNUSED, 0x60,AnimateWrongWay,     2},
	 {"HUDARROW",    SCR_MH-170,   SCR_MV-32, MUNUSED, 0x60,AnimateWrongWay,     3},
	{"BONUS",        SCR_MH-128,   SCR_MV+64, MUNUSED, 0x00,AnimateBonus,        0},
	{"DIGIT5",                0,           0, MUNUSED, 0x00,AnimateLapDigit,     1},
	{"DIGIT5",                0,           0, MUNUSED, 0x00,AnimateLapDigit,     0},
	{"FLAG",                  0,           0, MUNUSED, 0x00,AnimateFlag,         0},
/* {"SHADBLUR",       10, SCR_MV-32, MUNUSED, 0x00,nil,         0}, */
};

static MULTIBLIT const RadioMultiBlit[] = 
{	/* name			 		 x             y whtblrfz alpha animFunc            id */
	{"CDPLAYER",  SCR_MH-256/2,    SCR_B- 64, MUNUSED, 0x00,AnimateRadio,        0},
	 {"DIGIT2",   SCR_MH+110/2,    SCR_B- 34, MUNUSED, 0x00,AnimateRadio,     0x42}
};

static BLIT	*HudBlit, *RadioBlit;


/*===============================  EXTERNS  =================================*/

extern U32		play_time;		/*	Msecs of time to play. */
extern S16		mph,rpm;		/*	Speed, tach			model.h 	*/
extern U16		carnum;			/*	Car number,			model.h 	*/
extern U8     	radio_station;	/*	Radio station, 		carsnd.c 	*/
extern S16		which_engine;	/*	Current engine sound 			*/
extern U8		man_gear;		/*	gear,				mdrive.c	*/
extern S16		autotrans;		/*	true if using auto tranny		*/
extern S8		view;			/*  Curent camera view, camera.c	*/
extern MATRIX	fcam;			/*	Camera matrix for view 3. 		*/
extern const S16	max_radio;


/* ----------------------------------------------------------------------------- */
void InitHUD(void) 
{
	strcpy( TrackName,	"TRACK1_1" );

	gRadioTimeout	= 0;

	set_radar_mapping(-103.12,78.75,103.12,-401.25);

}	/* end InitHUD */


/* ----------------------------------------------------------------------------- */
void ShowHUD(void) 
{
	/* If HUD doesn't exist yet, create it. */
	if (!HudBlit) 
		{
		InitDigitList();
		HudBlit 		= NewMultiBlit(0,0,HudMultiBlit,NBLITS(HudMultiBlit));
#if 0
		gNeedleObj		= MBOX_NewObject( MBOX_FindObject("TACHNDLO1"), zerovector, unituvs, -1, ZATR_FRONTFACE);
#endif
		gFinishHid		= true;
		gRadioTimeout 	= IRQTIME;
		if (demo_game)
			AddJoinNow(HudBlit, 0, true);

		markers_valid = false;
		}

}	/* end ShowHUD */


/* ----------------------------------------------------------------------------- */
void RemoveHUD(void)
{
	if (RadioBlit)
		{
		RemoveBlit( RadioBlit );
		RadioBlit = nil;
		}

	if (HudBlit)
		{
		RemoveBlit( HudBlit );
		gFinishHid	= true;
		HudBlit 	= nil;
		}

	RemoveMsgs();

}	/* end RemoveHUD */


/* ----------------------------------------------------------------------------- *
 * Called every frame to update the tach frame.  Since this is really going to
 * do nothing to the actual frame, use it to update all the timer strings so
 * all those conversion only happen once per multiblit update.
 * ----------------------------------------------------------------------------- */
static int AnimateTach(BLIT *blt) 
{
	S32			t, digit;
	S16 		cars_in_order[MAX_LINKS], index, indx, i, j;
	S16			pos[2];
	F32			cpos[3], bp[3], max_dist;
	MATRIX		mat;

	/*	Calculate this only once per frame.  Not used here, but elsewhere in the
	 *	HUD display. */
	if (!blt->AnimID && (gstate == PLAYGAME || gstate == ATTRACT))
		{
		hud_mph = (S16)game_car[this_node].mph;
		if (metric)	hud_mph *= 1.61;

		/*	Update all the  time strings.  First the elapsed time. */
		if (game_car[this_node].place_locked == 1)
			t = game_car[this_node].score;
		else			
			t = GetElapsedTime();

		if (gstate == PLAYGAME)
			cvt_time( t, TrackTimeStr, 'h' );

		/*	Now the remaining seconds. */
		t = GetCountdownTime()/ONE_SEC;
		cvt_time( t, CountdownStr, 'c' );


		for (i=0; i<MAX_LINKS; i++)
			{
			dot_order_list[i] = -1;
			gRankList[i] = -1;
			}

		/* do stuff for markers */
		/*	Sort the slots based on current ranking. */
		for (indx=0; indx<num_active_cars; indx++)
			{
			index = model[indx].slot;

			/*	Determine position of blit on screen. */
			for (i=0; i<3; ++i)
				cpos[i] = game_car[index].dr_pos[i];
/*				cpos[i] = game_car[index].pos[i]/40.0;*/
			cpos[1] += 4.0;

            MBOX_ProjectWindow(0, cpos, (MATRIX *)&ZoidCamInfo[0].mat, bp, pos);
			marker_pos[index][0] = pos[0] - 8;
			marker_pos[index][1] = pos[1] - 16;
			marker_pos[index][2] = bp[2];

			gRankList[indx] = index;
			}

		/* sort distances of cars (as drawn on screen) */
		for (i=0; i<num_active_cars; i++)
			{
			index = i;

			max_dist = -1;

			for (j=i; j<num_active_cars; j++)
				{
				indx = gRankList[j];

				if (marker_pos[indx][2] > marker_pos[gRankList[index]][2])
					{
					max_dist = marker_pos[indx][2];
					index = j;
					}
				}

			indx = gRankList[i];
			gRankList[i] = gRankList[index];
			gRankList[index] = indx;
			}

		/* do stuff for dots */
		/* make list of ordered cars by place */
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
			}

		/* place self */
		dot_order_list[num_active_cars-1] = this_node;

		/* place other humans */
		for (i=0,j=1; i<num_active_cars; i++)
			{
			index = cars_in_order[i];
			if (index == this_node)
				continue;

			if (model[index].drone_type == HUMAN)
				dot_order_list[num_active_cars-1-j++] = index;
			}

		/* place drones */
		for (i=0; i<num_active_cars; i++)
			{
			index = cars_in_order[i];
			if (model[index].drone_type == DRONE)
				dot_order_list[num_active_cars-1-j++] = index;
			}

		markers_valid = true;
		}
	else if (blt->AnimID == 2)
		updateBoolean( blt, metric, "KPH", "MPH" );

	return 1;

}	/* end AnimateTach */


/* ----------------------------------------------------------------------------- *
 * Called every frame to update the tach needle digits.
 * ----------------------------------------------------------------------------- */
static int AnimateNeedle(BLIT *blt) 
{
	F32			angle, temp, xyz[3];
	BOOL		update = false, flip = false;
	S32 		degrees, i,j;
	S16			pos[2];

	/*	Calculate the position of the needle on the tach. */
	temp = (game_car[this_node].rpm*1.35) / 1000.0;	/* 1.35 is fake rpm boost. */

	if (temp < 0.0)
		temp = -temp;
	if (temp > 10.0)
		temp = 10.0;

	angle	= DEG_TO_RADS * (180+ 35 + temp * (180.0 - (35*2))/11.0);
	blt->X	= SCR_MH -  4 + (60 * fcos(angle));	/* Don't forget 1/2 dot width */
	blt->Y	= SCR_T-7- 24 - (60 * fsin(angle));
	UpdateBlit(blt);
#if 0
	pos[0] = blt->X;
	pos[1] = blt->Y;
	for (i=0; i<3; ++i)
		for (j=0; j<3; ++j)
	MBOX_WindowTo3D(0, pos, (MATRIX *)&ZoidCamInfo[0].mat, 1.0, xyz);
	SMSG(15,30,"x %8.3f", xyz[0] );
	SMSG(16,30,"y %8.3f", xyz[1] );
	SMSG(17,30,"z %8.3f", xyz[2] );
	ZOID_UpdateObject( gNeedleObj, xyz, nil );
#endif
	return 1;
}	/* end AnimateNeedle */


/* ----------------------------------------------------------------------------- *
 * Called every frame to update the speedo digits.
 * ----------------------------------------------------------------------------- */
static int AnimateSpeed(BLIT *blt) 
{
	S16	digit, type;

	blt->AnimID = (blt->AnimID & 0xF) + (SpeedDigit << 4);
	blt->X 	= SCR_MH - 33    + 22 * (blt->AnimID & 0xF);
	digit = hud_mph;
	if (digit < 0)
		digit = -digit;
	switch (blt->AnimID & 0xF)
		{
	case 0:	digit /= 100;	break;		/* Hundreds */
	case 1:	digit /= 10;	break;		/* Tens */
	default:				break;		/* Ones */
		}

	return UpdateNumber( blt, (char)(digit % 10));
}	/* end AnimateSpeed */


/* ----------------------------------------------------------------------------- *
 * Called every frame to update the track map.
 * ----------------------------------------------------------------------------- */
static int AnimateTrackMap(BLIT *blt) 
{
#if TRKFIX
/* figure placement for t4-8 later. */
	static const S32	XSize[MAX_TRACKS] 		= {100,125,165,112,126,122,58,112},
				Xoffset[MAX_TRACKS]	= { 63,  0,128,  0, 0, 0, 0, 0},
				Yoffset[MAX_TRACKS]	= {128,  0,  0,  0, 0, 0, 0, 0};
#endif

	/* If the track has changed, reset the mapping of track to map */
	if (trackno+1 != blt->AnimDTA)
		{
		TrackName[TRACK_OFFSET] = (char)((trackno) + '1');
		TrackName[TPART_OFFSET] = (char)(blt->AnimID + '0');
		blt->AnimDTA = (S32)trackno+1;
		blt->Flip = gMirrorMode;
		if((trackno == 1 || trackno == 3 || (trackno > 3)) && blt->AnimID == 2)
			return Hidden(blt,true);
		RenameBlit(blt,TrackName,0);

		/*	Reposition so map is at the right edge of the screen. */
		if (gMirrorMode && (trackno == 0))
		    blt->X = SCR_R - 128;
		else
		    blt->X = SCR_R - XSize[trackno];
		blt->Y = SCR_T + 82;

        if (blt->AnimID != 1)
			blt->Y += Yoffset[trackno];

		if (blt->AnimID == 1)
		{
		    if (gMirrorMode && (trackno == 2))
		        blt->X += XSize[trackno] - Xoffset[trackno] - 5;
			set_track_mapping(blt);
        }
        else if (gMirrorMode && (trackno == 0))
			blt->X += Xoffset[trackno] - 5;
        else if (!gMirrorMode)
			blt->X += Xoffset[trackno];

		blt->Alpha = 0x80;
		blt->Z = 0.3;
		UpdateBlit(blt);
		}

	blt->AnimFunc = nil;
	return 1;

}	/* end AnimateTrackMap */


/* ----------------------------------------------------------------------------- *
 * Called every frame to update the dots on the track map.
 * ----------------------------------------------------------------------------- */
static int AnimateDot(BLIT *blt) 
{
	S16 b, flash, slot 	= dot_order_list[blt->AnimID & 0xf];
	MODELDAT	*m;
	CAR_DATA	*c;

	if (Hidden(blt,slot==-1))
		return 1;

	m = &model[slot];
	c = &game_car[slot];

	blt->X = (U16)(c->dr_pos[0] * 40.0 * gMTrackX + gTTrackX);
	blt->Y = (U16)(c->dr_pos[2] * 40.0 * gMTrackY + gTTrackY);
	
	flash = c->we_died || m->crashflag || !m->collidable || m->hide_car ||
		!(m->resurrect.moving_state == -1 || mpath_edit || gFlyMode);

	/*	Handle the local car. */
	if (slot == this_node)
		{
		blt->Z		= 0.0;
		blt->Alpha  = (IRQTIME & 0x100) ? 0xA0 : 0x00;
		SelectBlit( blt, 1+slot, 7, 7 );
		}

	/*	Handle other players. */
	else if (m->drone_type == HUMAN)
		{
		blt->Z	= 0.1;
		blt->Alpha  = (IRQTIME & 0x100) && flash ? 0xA0 : 0x00;
		SelectBlit( blt, 1+slot, 7, 7 );
		}

	/*	Handle a drone. */
	else
		{
		blt->Z = 0.2;
		blt->Alpha  = (IRQTIME & 0x100) && flash ? 0xA0 : 0x00;
		SelectBlit( blt, 0, 7, 7 );
		}

	UpdateBlit(blt);

	return 1;
}	/* end AnimateDot */


/* ----------------------------------------------------------------------------- *
 * Called every frame to update the Human player markers on the screen.
 * ----------------------------------------------------------------------------- */
static int AnimateMarker(BLIT *blt) 
{
	S16 		b, i,j;
	S16			pos[2];
	F32			cpos[3], bp[3];
	MATRIX		mat;
	S16			slot 	= gRankList[blt->AnimID & 0xf];
	MODELDAT	*m		= &model[slot];
	BOOL	offscreen;

	/* Prevent marker flashing. */
	offscreen = (	(marker_pos[slot][0] < -1000.0) ||
			(marker_pos[slot][0] > 1000.0) ||
			(marker_pos[slot][1] > -1000.0) ||
			(marker_pos[slot][1] > 1000.0) ||
			(marker_pos[slot][2] <= 3.3) );

/*SMSG( 10+slot, 2, "%8s: %8.3f",NodeName(slot), blt->Z );  */
	if (Hidden(blt, slot == -1 || !are_we_solid(slot) || slot == this_node
			     || offscreen || !marker_flag
			     || m->drone_type != HUMAN || solo_flag || !markers_valid))
		return 1;

	/*	Initialize the blit if needed. */
	SelectBlit( blt, slot, 16,16 );
	
	blt->X	= marker_pos[slot][0];
	blt->Y	= marker_pos[slot][1];
	blt->Z	= marker_pos[slot][2];

	UpdateBlit(blt);
	return 1;
}	/* end AnimateMarker */


/* ----------------------------------------------------------------------------- *
 * Called every frame to update the cars on the radar map.
 * ----------------------------------------------------------------------------- */
static int AnimateTraffic(BLIT *blt) 
{
	F32			x,y;
	S16			i, angle, slot, flip = 0;
	F32 		rpos[3], rdir[3], lpos[3], ldir[3];
	CAR_DATA	*gc;
	MODELDAT	*m;

	slot = blt->AnimID;
	m = &model[slot];
	gc = &game_car[slot];

	/*	Hide if not in game. */
	if (!m->in_game || (solo_flag && !(gLink[gThisNode].flags & LINK_DRONES)))
		{
		blt->AnimFunc = nil;
		return Hidden(blt,1);
		}
		
	/*	Handle the local car differently. */
	if (this_node == slot)
		{
		blt->X = (U16)gTRadarX;
		blt->Y = (U16)gTRadarY;
		angle = 3;	/* player_model always 0 degrees */
	    goto found;
		}

	if (Hidden(blt,!are_we_solid(slot)))
		return 1;

	/* get rel direction */
	ldir[0] = gc->dr_uvs[0][2];	/*column 0 is forward dir (unit) */
	ldir[1] = gc->dr_uvs[1][2];
	ldir[2] = gc->dr_uvs[2][2];
	frwtobod(ldir,rdir,game_car[this_node].dr_uvs[0]);

	/* determine angle */
	flip =  (rdir[0] * rdir[2] < 0.0);
	if(rdir[2] < 0.0)
		rdir[2] = -rdir[2];		/* map to quadrant 0 */
	if(rdir[2] > 0.966) 
		angle = 3;				/*  0 to 15 degrees */
	else if(rdir[2] > 0.707) 
		angle = 2;				/* 15 to 45 degrees */
	else if(rdir[2] > 0.259) 
		angle = 1;				/* 45 to 75 degrees */
	else 
		angle = 0;				/* 75 to 90 degrees */

	/* get rel position */
	vecsub(gc->dr_pos,game_car[this_node].dr_pos,lpos);
	frwtobod(lpos,rpos,game_car[this_node].dr_uvs[0]);

	/*	Now scale and translate them into place. */
	x	= blt->X = (U16)(rpos[0] * gMRadarX + gTRadarX);
	y 	= blt->Y = (U16)(rpos[2] * gMRadarY + gTRadarY);

	/* Check for clipping (cheating some to see further behind) */
	if (Hidden(blt,(x < RADAR_MIN_X || x > RADAR_MAX_X || 
					y < RADAR_MIN_Y || y > RADAR_MAX_Y)))
	    return 1;
found:
	if (flip)
		angle = 7 - angle;
	if (model[slot].drone_type == DRONE)
		SelectBlit( blt,  (S16)(angle * (MAX_LINKS+1)), TCAR_W,TCAR_H );
	else
		SelectBlit( blt, (S16)((angle * (MAX_LINKS+1)) + (slot+1)), TCAR_W,TCAR_H );
	return 1;

}	/* end AnimateTraffic */


/* ----------------------------------------------------------------------------- *
 * Turns on the radio display for a few seconds.
 * ----------------------------------------------------------------------------- */
void EnableRadioHUD(void)
{
	if (gstate != PLAYGAME || mBlitIndex == MBLIT_PLACE)
		return;

	if (!RadioBlit) 
		{
		RadioBlit = NewMultiBlit(0,0,RadioMultiBlit,NBLITS(RadioMultiBlit));
		mBlitIndex = MBLIT_CD;
		}

	gRadioTimeout 		= IRQTIME + ONE_SEC * 3;

}	/* end EnableRadioHUD */


/* ----------------------------------------------------------------------------- *
 * Called every frame to update the radio.
 * ----------------------------------------------------------------------------- */
static int AnimateRadio(BLIT *blt) 
{
	S8	digit;

	if (Hidden( blt, (IRQTIME > gRadioTimeout || gstate != PLAYGAME)))
		return 1;

	/*	Temporary fooling around with translucency of tach frame. */
	digit = blt->AnimID & 0xF;
	if (digit == 2)
		return UpdateNumber( blt, (char)(radio_station + 1) % (max_radio+1) );
	
	if (mBlitIndex != MBLIT_CD)
		{
		RenameBlit(blt,"CDPLAYER",1);
		mBlitIndex = MBLIT_CD;
		}

	return 1;
}	/* end AnimateRadio */


/* ----------------------------------------------------------------------------- *
 *	Crops the given blit to the given size.
 * ----------------------------------------------------------------------------- */
void CropBlit( BLIT *blt, int l, int t, int r, int b )
{
	blt->Top	= blt->Info->Height - t;
	blt->Bot	= blt->Info->Height - b + 1;
	blt->Height	= b - t + 1;
	blt->Width	= r - l + 1;

	/* Note: Does not handle flipped blits. */
	blt->Left	= l;
	blt->Right	= r + 1;
	UpdateBlit(blt);

}	/* end CropBlit */


/* ----------------------------------------------------------------------------- *
 * Called every frame to update the radar frame.
 * ----------------------------------------------------------------------------- */
static int AnimateRadar(BLIT *blt) 
{
	blt->AnimFunc = nil;

	if (solo_flag && !(gLink[gThisNode].flags & LINK_DRONES))
		return Hidden(blt,1);

	if (blt->AnimID == 1)
		blt->Height = 48;
	else if (blt->AnimID == 2 || blt->AnimID == 3)
		{
		blt->Y		= RADAR_Y + 48;
		blt->Height = RADAR_HIGH - 16 - 48;
		blt->Width  = blt->Info->Width;
		}
	UpdateBlit(blt);
	return 1;
}	/* end AnimateRadar */


/* ----------------------------------------------------------------------------- *
 * Called every frame to update the shifter.
 * ----------------------------------------------------------------------------- */
static int AnimateShifter(BLIT *blt) 
{
	S32	index;

	updateBoolean( blt, autotrans, "AUTOSHIFT", "SHIFTER" );
	if (autotrans)
		{
		if (gstate == COUNTDOWN || (edges & SW_REVERSE) || (trailing_edges & SW_REVERSE))
			index	= 1;
		else
			index	= (levels & SW_REVERSE) ? 0 : 2;
		if (index != blt->AnimID)
			SelectBlit(blt, index, 40,64);
		blt->AnimID = index;
		}
	else if (man_gear != blt->AnimID)
		{
		SelectBlit(blt, man_gear, 51,64 );
		blt->AnimID = man_gear;
		}
	return 1;
}	/* end AnimateShifter */


/* ----------------------------------------------------------------------------- *
 * Called every frame to update the countdown timer.
 * ----------------------------------------------------------------------------- */
static int AnimateCTimer(BLIT *blt) 
{
	S32 sec;
	static S32 old_sec;

	if (gstate != PLAYGAME && gstate != ATTRACT)
		return UpdateNumber( blt, 0 );

	if ((sec = GetCountdownTime() / 1000) < 0)
		{
		play_time -= sec * ONE_SEC;
		sec = 0;
		}

	if ((sec < 6) && (old_sec != sec))
		{
		old_sec = sec;
		if (demo_game == 0)
			SOUND (S_BEEP1);
		}

	if (Hidden(blt, sec <= 5 && (IRQTIME & 0x100)))
		return 1;

	return UpdateNumber( blt, CountdownStr[blt->AnimID & 0xF] );
}	/* end AnimateCTimer */


/* ----------------------------------------------------------------------------- *
 * Called every frame to update the Elapsed time.
 * ----------------------------------------------------------------------------- */
static int AnimateElapsed(BLIT *blt) 
{

	if (gstate != PLAYGAME && gstate != ENDGAME && gstate != GAMEOVER && gstate != ATTRACT)
		return UpdateNumber( blt, 0 );

	return UpdateNumber( blt, TrackTimeStr[blt->AnimID & 0xF] );
}	/* end AnimateElapsed */


/* ----------------------------------------------------------------------------- *
 *	Called to display the checkpoint delta time.  Only call once to start the
 *	display, shuts off automatically.
 * ----------------------------------------------------------------------------- */
void ShowCheckpointTime(S32	delta) 
{
	S32	t;

	/*	Set the go away time. */
	gCheckPtTimeout = IRQTIME + CHECKPT_TIME;

	/*	Convert the value to a string. */
	if (dlevels & SW_DEBUG6)
		cvt_time( delta, CheckpointStr, 'h' );
	else
		{
		t = GetElapsedTime();
		cvt_time( t, CheckpointStr, 'h' );
		}
}	/* end ShowCheckpointTime */


/* ----------------------------------------------------------------------------- *
 * Called every frame to update the Checkpoint time.
 * ----------------------------------------------------------------------------- */
static int AnimateCheckPt(BLIT *blt) 
{
	S8 		hide;
	S8		digit 	= blt->AnimID & 0xF;
	S32		remain	= gCheckPtTimeout - IRQTIME;

	hide = remain < 0;
	if (Hidden(blt,hide))
		return 1;

	blt->Alpha = (remain > ONE_SEC) ? 0 : 255 - 255 * remain / ONE_SEC;

	if (digit < 5)
		UpdateNumber( blt, (U8)CheckpointStr[digit] );

	/* Handle AnimID == 6 (background) special. */
	if (blt->Alpha != blt->AnimDTA)
		{
		blt->AnimDTA = blt->Alpha;
		UpdateBlit(blt);
		}

	return 1;

}	/* end AnimateCheckPt */


/* ----------------------------------------------------------------------------- *
 * Called every frame to the time extension message.
 * ----------------------------------------------------------------------------- */
static int AnimateBonus(BLIT *blt) 
{
	S32		remain	= (gCheckPtTimeout2 - IRQTIME) - ONE_SEC;
	BOOL hide = remain < 0 || (IRQTIME & 0x80);

	if (Hidden(blt,hide))
		return 1;

	blt->Alpha = (remain > ONE_SEC) ? 0 : 255 - 255 * remain / CHECKPT_TIME;
	if (blt->Alpha != blt->AnimDTA)
		{
		blt->AnimDTA = blt->Alpha;
		UpdateBlit(blt);
		}
	return 1;
}	/* end AnimateBonus */


/* ---------------------------------------------------------------------- *
 * Called every frame to update the lap digit.
 * ---------------------------------------------------------------------- */
    S16    gLapsLeft;
static int AnimateLapDigit(BLIT *blt) 
{
    S16    digit, i;

	if (Hidden(blt,(!gFinishHid || demo_game)))
		return 1;

	blt->X	= gFinishX + 6 - 12 * (blt->AnimID & 0xF);
	blt->Y	= gFinishY - 8;

	blt->AnimID = (blt->AnimID & 0xF) + (LapDigit << 4);

	digit = 1;
	for (i = 0; i < (blt->AnimID & 0xF); i++)
	    digit *= 10;

	digit = gLapsLeft / digit;
	if (Hidden(blt,(!digit)))
		return 1;

	digit = digit % 10;

	UpdateNumber(blt, digit);
	return 1;
}	/* end AnimateLapDigit */

/* ----------------------------------------------------------------------------- *
 * Called every frame to update the finish line flag.
 * ----------------------------------------------------------------------------- */
static int AnimateFlag(BLIT *blt) 
{
	if (Hidden(blt,gFinishHid))
		return 1;

	blt->X	= gFinishX;
	blt->Y	= gFinishY;
	if (blt->AnimDTA != IRQTIME / 125)
		{
		blt->AnimDTA = IRQTIME / 125;
		blt->AnimID = (1 + blt->AnimID) % 5;
		}
	SelectBlit(blt, blt->AnimID, 15,16 );
	return 1;
}	/* end AnimateFlag */


/* ----------------------------------------------------------------------------- *
 * Called every frame to update the finish line flag.
 * ----------------------------------------------------------------------------- */
void ShowFinishFlag( F32 pos[3], BOOL hide )
{
	gFinishHid	= hide;
	gFinishX	= (S16)(40*pos[0] * gMTrackX + gTTrackX) - 14;
	gFinishY	= (S16)(40*pos[2] * gMTrackY + gTTrackY) - 14;

}	/* end ShowFinishFlag */


/* ----------------------------------------------------------------------------- *
 * Called every frame to update ranking.
 * ----------------------------------------------------------------------------- */
static int AnimatePlace(BLIT *blt) 
{
	char digit;
	static char lastDigit;

	if (Hidden(blt,(solo_flag && !(gLink[gThisNode].flags & LINK_DRONES)) || 
		!are_we_solid (this_node)) || blt->AnimID == 1)
		return 1;

	digit = game_car[this_node].place + 1;
	if (digit < 1) digit = 1;
	if (digit > 8) digit = 8;

	/*	Update the race leader lamp. */
	if (digit == 1 && lastDigit != 1 && demo_game==false)
		ctl_mod_latch( LA_LEADER );
	else if (lastDigit == 1 && digit != 1)
		ctl_mod_latch( ~LA_LEADER );
	lastDigit = digit;

	return UpdateNumber( blt, digit );
}	/* end AnimatePlace */


/* ----------------------------------------------------------------------------- *
 * Called every frame to update wrongway indicator.
 * ----------------------------------------------------------------------------- */
static int AnimateWrongWay(BLIT *blt) 
{
	BOOL		wrong;
	S32			time, x,x2,y, offset,w,h, id;
	F32			t, scale;
	static BOOL	WasStrayed;

	/* If haven't strayed, turn off display. */
	id		= blt->AnimID & 0xF;
	wrong 	= !in(-STRAY_THRESHOLD,gStrayAngle/2,STRAY_THRESHOLD);
	wrong_way = wrong;
	if (Hidden(blt, (in(-STRAY_THRESHOLD,gStrayAngle,STRAY_THRESHOLD) ||
					 (wrong && id) || (!wrong && !id) || gstate != PLAYGAME)))
		{
		blt->AnimDTA = IRQTIME;
		if (!id && WasStrayed)
			{
			ctl_mod_latch( ~LA_ABORT );
			WasStrayed = false;
			}
		return 1;
		}

	/*	Going wrong way, decide on what parts to display. */
	if (id == 0)
		{
		if (Hidden(blt, (IRQTIME < gRadioTimeout)))
			return 1;
		if (mBlitIndex != MBLIT_WRONGWAY)
			{
			RenameBlit( blt, "WRONGWAYTEXT", 1 );
			mBlitIndex = MBLIT_WRONGWAY;
			}
		ctl_mod_latch( (IRQTIME & 0x100) ? LA_ABORT : ~LA_ABORT );
		WasStrayed = true;
		}

	/*	Place either in center, moving or at lower left corner based on time. */
	time = IRQTIME - blt->AnimDTA;
	if (time < 3000)
		t = 0.0;
	else if (time < 3500)
		t = (time - 3000) / 500.0; 
	else
		t = 1.0;
	scale = 1.0 - t/2.0;

	if (wrong)
		{
		blt->Alpha	= (time & 0x100) ? 0xD0 : 0x00;
		blt->Y		= (SCR_MV - 64) + t * (SCR_B - SCR_MV + 32);
		offset		= scale * 128;
		x2			= SCR_L;
		}
	else
		{
		if (!(blt->Flip = gStrayAngle < 0))
			blt->Alpha	= ((time%350)/50 < (4-id)) ? 0xD0 : 0x00;
		else
			blt->Alpha	= ((time%350)/50 < id) ? 0xD0 : 0x00;
		blt->Y		= (SCR_MV - 64) + t * (SCR_B - SCR_MV + 32) + (16 * scale);
		offset		= scale * 20 * id;
		x2			= SCR_L + 40 - offset;
		}

	x			= SCR_MH - offset;
	blt->X		= x + t * (x2 - x);
	blt->Width	= blt->Info->Width  * scale;
	blt->Height	= blt->Info->Height * scale;
	UpdateBlit( blt );
	return 1;
}	/* end AnimateWrongWay */


/* ----------------------------------------------------------------------------- *
 * Returns true if blit is hidden and sets it hidden if hide is != 0.
 * ----------------------------------------------------------------------------- */
BOOL Hidden(BLIT *blt, S32 hide)
{
	if (hide != blt->Hide)
		{
		blt->Hide = hide;
		UpdateBlit(blt);
		}
	return blt->Hide;
}	/* end Hidden */


/* ----------------------------------------------------------------------------- *
 * 	Handles blits that contain digits.
 *	Blit fields are used as follows:
 *		AnimID	= initialized << 8  | desired type/font << 4 | id
 *		AnimDTA	= numeric value << 4 | actual type/font
 * ----------------------------------------------------------------------------- */
int UpdateNumber( BLIT *blt, U8 digit )
{
	S16 	type 	= (S16)((blt->AnimID >> 4) & 0xF);
	S16		val	 	= (S16)((blt->AnimDTA >> 4) & 0xF);
	BOOL	update 	= ((blt->AnimID >> 8) & 0xF) == 0;
	
	/*	This gives the size of each digit set's characters. */
	static const S16 DigitSize[NUM_DIGIT_TYPES][2] =	
	{	/* w   h */
		{ 22, 17 },		/* 0 - SpeedDigit 		*/
		{ 12, 32 },		/* 1 - YelDigit			*/
		{  8, 16 },		/* 2 - SmallYelDigit	*/
		{ 12, 32 },		/* 3 - RedDigit			*/
		{  8, 16 },		/* 4 - SmallRedDigit	*/
		{ 12, 32 },		/* 5 - LapDigit         */
		{ 32, 38 },		/* 6 - HudTimerDigit	*/
		{ 32, 64 },		/* 7 - CountdownDigit	*/
		{ 24, 32 },		/* 8 - PlaceDigit		*/
		{ 32, 32 },		/* 9 - TimerDigit		*/
	};

	/*	Check for a font change. */
	if (type != (blt->AnimDTA & 0xF) || !gDigitList[type].ti)
		{
		blt->AnimDTA 	= (blt->AnimDTA & 0xFFF0) | (type & 0xF);
		blt->Info		= gDigitList[type].ti;
		if (!blt->Info)
			{
			/*	Maybe got reloaded since last looked for. */
			if (!(gDigitList[type].ti = MBOX_FindTexture(gDigitList[type].Name,&gDigitList[type].TexIndex)))	
				{
				EMSG( "Can't find texture: %s", blt->Name );
				return 1;
				}
			blt->Info	= gDigitList[type].ti;
			}
		blt->Name		= gDigitList[type].Name;
		blt->TexIndex	= gDigitList[type].TexIndex;
		update	= 1;
		}
	
	if ((digit & 0xF) != val || update)
		{
		blt->AnimDTA 	= (blt->AnimDTA & 0xFF0F) | ((digit & 0xF) << 4);
		blt->AnimID		|= 0x100;
		if (type == 0 && digit == 10)		
			SelectBlit(blt, (S16)digit, DigitSize[type][0],DigitSize[type][1] );
		else
			SelectBlit(blt, (S16)(digit % 10), DigitSize[type][0],DigitSize[type][1] );
		}

	return 1;
}	/* end UpdateNumber */


/* ----------------------------------------------------------------------------- *
 *	Initializes a list of predefined texture blits.
 * ----------------------------------------------------------------------------- */
static void InitBlitList(BlitList list[], S16 num)
{
	BlitList	*bl;

	for (--num; num >= 0; --num)
		{
		bl = &list[num];
		bl->ti = MBOX_FindTexture(bl->Name,&bl->TexIndex);
		}
}	/*	end InitBlitList */


/* ----------------------------------------------------------------------------- *
 *	Initializes a list of predefined texture blits.
 * ----------------------------------------------------------------------------- */
void InitDigitList(void)
{
	S32 i;
	for (i=0; i<NUM_DIGIT_TYPES; ++i)
		sprintf( gDigitList[i].Name, "DIGIT%1ld", i);
	InitBlitList( gDigitList, NUM_DIGIT_TYPES );
}	/*	end InitDigitList */



/* ----------------------------------------------------------------------------- *
 * Utility routine to update a button with more than 2 states.
 * ----------------------------------------------------------------------------- */
int updateList( BLIT *blt, S32 val, S32 num, ...)
{
	va_list	ap;
	char 	*name;
	S16		i;

	if (val != blt->AnimID) 
		{
		blt->AnimID = val;
		va_start( ap, num );	
		i = 0;
		do	{
			name = va_arg(ap,char *);
			++i;
			}
		while (i <= val && i < num);
		RenameBlit( blt, name, 0 );
		va_end( ap );
		}
	return 1;
}	/* end updateList */

/* ----------------------------------------------------------------------------- *
 * Utility routine to update a button with more than 2 states.
 * ----------------------------------------------------------------------------- */
int updateList2( BLIT *blt, S32 val, S32 num, ...)
{
	va_list	ap;
	char 	*name;
	S16		i;

	blt->AnimID = val;
	va_start( ap, num );	
	i = 0;
	do	{
	name = va_arg(ap,char *);
	++i;
	}
	while (i <= val && i < num);
	RenameBlit( blt, name, 0 );
	va_end( ap );

	return 1;
}	/* end updateList */

/* ----------------------------------------------------------------------------- */
static void set_track_mapping(BLIT *blt)
{ 

#if TRKFIX
/* figure placement for t5-8 later. */
	/*	This offsets the dot within a given map */
	static const S32 TrackSlop[MAX_TRACKS][2] =	{ { 33, 56 },		/* Track 1 */
							  { 97, 75 },		/* Track 2 */
							  {184,-43 },		/* Track 3 */
							  { 65, 56 },		/* Track 4 */
							  {  0,-50 },		/* Track 5 */
							  { 54,-42 },		/* Track 6 */
							  {  2, 20 },		/* Track 7 */
							  { 65, 56 } };		/* Track 8 */

	static const S32 TrackSlopM[MAX_TRACKS] =	{ 
	                           -11,		        /* Track 1 */
							  -103,      		/* Track 2 */
							  -232,      		/* Track 3 */
							   -31,      		/* Track 4 */
							    15,      		/* Track 5 */
							  -106,      		/* Track 6 */
							     4,      		/* Track 7 */
							    65,      };		/* Track 8 */

	/*	This maps the track world coordinates to window coordinates of the track map. */
	static const F32 TrackMapping[MAX_TRACKS][2][4]	= { 
		{ { 1996.686,-6510.893, -3557.542,  3124.747 },			/* T1, world coords */
		  {      0.0,      0.0,     101.0,     174.0 } },		/* T1, window coords */
		{ {  -6050.0,    600.0,    1430.0,   -4540.0 },			/* T2, world coords */
		  {      0.0,      0.0,     128.0,      89.0 } },		/* T2, window coords */
		{ {  -8510.0,  -1680.0,   -1250.0,    4150.0 },			/* T3, world coords */
		  {      0.0,    136.0,     169.0,       0.0 } },		/* T3, window coords */
		{ {  -2360.0,   1998.0,     752.0,   -1238.0 },			/* T4, world coords */
		  {      0.0,      0.0,      90.0,      92.0 } },		/* T4, window coords */

		{ {     48.8,   2578.6,    3691.3,     258.9 },			/* T5, world coords */
		  {     96.0,     46.0,      12.0,       2.0 } },		/* T5, window coords */
		{ {   2900.0,   2320.0,      10.0,     550.0 },			/* T6, world coords */
		  {     57.0,     50.0,     116.0,      13.0 } },		/* T6, window coords */
		{ {   -205.3,   1725.6,    2173.3,   -6274.9 },			/* T7, world coords */
		  {     10.0,     17.0,      42.0,     121.0 } },		/* T7, window coords */
		{ {  -2360.0,   1998.0,     752.0,   -1238.0 },			/* T8, world coords */
		  {      0.0,      0.0,      90.0,      92.0 } } };		/* T8, window coords */
#endif

	gMTrackX	= (TrackMapping[trackno][1][2] - TrackMapping[trackno][1][0]) / 40 /
	   	          (TrackMapping[trackno][0][2] - TrackMapping[trackno][0][0]);

    if (gMirrorMode)
    {
    	gTTrackX	= TrackMapping[trackno][1][2] + gMTrackX * TrackMapping[trackno][0][0] + 
	    	          blt->X + TrackSlopM[trackno];
	}
	else
	{
    	gTTrackX	= TrackMapping[trackno][1][0] - gMTrackX * TrackMapping[trackno][0][0] + 
	    	          blt->X + TrackSlop[trackno][0];
	}

	gMTrackY	= (TrackMapping[trackno][1][3] - TrackMapping[trackno][1][1]) / 40 /
		          (TrackMapping[trackno][0][3] - TrackMapping[trackno][0][1]);
	gTTrackY	= TrackMapping[trackno][1][1] - gMTrackX * TrackMapping[trackno][0][1] + 
		          blt->Y + TrackSlop[trackno][1];

}	/* end set_track_mapping */


/* ----------------------------------------------------------------------------- */
static void set_radar_mapping(F32 lx, F32 ly, F32 hx, F32 hy)
{
	gMRadarX	= (RADAR_WIDE - RADAR_BWIDE*2) / (hx - lx);
	gMRadarY	= (RADAR_HIGH - RADAR_BHIGH*2) / (hy - ly);
	gTRadarX	= RADAR_X + RADAR_CENTER_X - TCAR_W/2;
	gTRadarY	= RADAR_Y + RADAR_CENTER_Y - TCAR_H/2;
}	/* end set_radar_mapping */


/* ----------------------------------------------------------------------------- *
 * cvt_time - converts a long int into a time format MM:SS.hh or H:MM:SS.hh
 *
 * t			- the time in 0.001 second units to convert
 * dest		- location of array size [4] (or [5]) to store time format result
 * format	- 's'	- MM:SS     seconds only (faster)
 *			- 'h'	- MM:SS.hh  default (else) case: displays hundredth second
 *			- 'c'	- SSS       count down timer (seconds only)
 *
 * Returns	- number of digits set.
 *
 * ----------------------------------------------------------------------------- */
U8 cvt_time( S32 t, U8 *dest, char format )
{
    S32	sec, min;
	U8	ret = 0;

    if (t < 0)
		t = 0;
	
    switch (format)
        {
    case 'c':
    	sec = t;
		if (sec > 999)					/* Maximum counter of 999 */
		    sec = 999;
		dest[0] = (U8)((sec / 100) % 10);
		dest[1]	= (U8)((sec /  10) % 10);
		dest[2]	= (U8)(        sec % 10);
		ret		= 3;
		break;
			
    case 'h':
		dest[3] = (U8)((t / 100) % 10);
		dest[4] = (U8)((t /  10) % 10);
		ret		= 5;
		/* no break; fall through to do seconds */
		
    case 's':				
		sec 	= t / 1000;
		min		= (sec / 60) % 60;
        sec	   %= 60;
		dest[0]	= (U8)(min % 10);
		dest[1]	= (U8)(sec / 10);
		dest[2]	= (U8)(sec % 10);
		if (!ret)
			ret	= 3;
		break;
		}

	return ret;
}	/* end cvt_time */
