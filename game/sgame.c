/*
 *	game.c  main game loop
 *
 *	Copyright 1996 Time Warner Interactive
 *	Unauthorized reproduction, adaptation, distribution, performance or 
 *	display of this computer program or the associated audiovisual work
 *	is strictly prohibited.
 *
 *	======================================================================
 *	$Author: hightower $						$Date: 1997/10/15 03:49:52 $
 *	$Revision: 3.121 $						$Locker:  $
 *	======================================================================
 *	Change Log:
 *
 *	$Log: game.c,v $
 * Revision 3.121  1997/10/15  03:49:52  hightower
 * Synchonize countdown timers during race
 *
 * Revision 3.120  1997/10/06  07:50:13  gray
 * Now doing fog in a sane way.
 *
 * Revision 3.119  1997/10/03  21:18:33  grjost
 * Fixed hanging woman movie start
 *
 * Revision 3.118  1997/10/01  22:21:35  hightower
 * Fix to make cars all start at same time
 *
 * Revision 3.117  1997/10/01  17:45:56  grjost
 * Added new "Number of Laps" menu in GUTS
 *
 * Revision 3.116  1997/09/27  01:02:44  grjost
 * Put in mirror mode in hiscore table.
 *
 * Revision 3.115  1997/09/18  22:36:33  grjost
 * Fixed difficulty and laps for track 7
 *
 * Revision 3.114  1997/09/18  02:07:13  grjost
 * Made it so soundtracks don't play during countdown
 *
 * Revision 3.113  1997/09/18  01:34:25  grjost
 * Changed radio station selection so that mute is the first selection.
 *
 * Revision 3.112  1997/09/17  19:35:05  grjost
 * Hardcoded in laps and difficulty for T5 & T6
 *
 * Revision 3.111  1997/09/15  22:30:23  gray
 * Removed trackset hack - trackno now 0 to NTRACKS.
 *
 * Revision 3.110  1997/09/05  22:33:43  gray
 * Got rid of starting announcer for all tracks.
 *
 * Revision 3.109  1997/09/05  00:49:01  gray
 * Audio bug workaround - no announcer on track 1
 *
 * Revision 3.108  1997/09/02  05:17:32  gray
 * Big ol hack to get heartbeat going.
 *
 * Revision 3.107  1997/08/27  22:42:56  grjost
 * Added track and car histogram updating.
 *
 * Revision 3.106  1997/08/13  21:08:08  gray
 * Made music ON the default and radio station based on track.
 *
 * Revision 3.105  1997/07/27  22:29:14  gray
 * Checkpoint set select for consumer track dev.
 *
 * Revision 3.104  1997/07/01  18:58:32  gray
 * Made alternate tracks (additional 4 tracks) view 1 selectable.
 *
 * Revision 3.103  1997/06/28  01:40:05  gray
 * Extended to 4 tracks.
 *
 * Revision 3.102  1997/06/04  02:44:44  gray
 * Zapped ithrottle.
 *
 * Revision 3.101  1997/06/04  02:15:46  gray
 * Restructuring controls (gas,brake,clutch,etc) handling.
 *
 * Revision 3.100  1997/05/30  19:02:58  gray
 * Got rid of FLAGSTAFF conditionals.
 *
 * Revision 3.99  1997/03/31  03:19:15  gray
 * Nucleus gone so prc_get_HISR_time removed.
 *
 * Revision 3.98  1997/03/20  13:48:37  geraci
 * new countdown method
 *
 * Revision 3.97  1997/03/18  19:40:47  geraci
 * picks sim manager right before preplay
 *
 * Revision 3.96  1997/03/13  04:03:10  geraci
 * added quick game end, free game change, multi-cast channel call
 *
 * Revision 3.95  1997/02/21  00:13:29  geraci
 * set attract music state sooner
 *
 * Revision 3.94  1997/02/15  01:51:23  geraci
 * join quick in tourney implemented
 *
 * Revision 3.93  1997/02/05  02:33:05  geraci
 *  tracks can be set from 1 to 4 laps
 *
 * Revision 3.92  1997/01/31  23:36:49  geraci
 *  removed difficulty from tourney setup screen
 *
 * Revision 3.91  1997/01/30  23:11:40  geraci
 * checks if hiscore table needs to be reset then for factory default
 *
 * Revision 3.90  1997/01/30  20:29:04  geraci
 * new drone difficulty setting
 *
 * Revision 3.89  1997/01/28  22:39:03  geraci
 * got rid of text message during screen dump
 *
 * Revision 3.88  1997/01/25  01:23:45  geraci
 * fixed screen dump stuff
 *
 * Revision 3.87  1997/01/24  02:17:16  geraci
 * new checkpoint scheme
 * screen capture ability added
 *
 * Revision 3.86  1997/01/15  20:01:57  geraci
 * number of laps for track 2 and 3 can be changed by operator
 *
 * Revision 3.85  1996/12/13  13:25:22  geraci
 * common marker flags
 *
 * Revision 3.84  1996/12/12  09:42:41  geraci
 * targets hit in game task
 *
 * Revision 3.83  1996/12/05  10:20:10  geraci
 * house cleaning
 *
 * Revision 3.82  1996/12/05  00:04:33  geraci
 * end of game score removed from entity state
 *
 * Revision 3.81  1996/12/04  01:57:51  geraci
 * checkpoints and end of game scores are handled by entity state updates
 *
 * Revision 3.80  1996/12/02  04:53:33  gray
 * Never cheat modes for production version.
 *
 * Revision 3.79  1996/11/27  17:06:56  geraci
 * laps to go is displayed
 *
 * Revision 3.78  1996/11/27  08:22:28  petrick
 * Fixed manual shifter disable.
 *
 * Revision 3.77  1996/11/27  08:12:29  geraci
 * volume adjust is out of flagstaff
 *
 * Revision 3.76  1996/11/25  14:04:35  petrick
 * Turned off some debug for release version.
 *
 * Revision 3.75  1996/11/25  12:36:53  petrick
 * Added tourney indication to debug.
 *
 * Revision 3.74  1996/11/25  11:37:14  geraci
 * old engine is gone
 *
 * Revision 3.73  1996/11/25  04:13:35  petrick
 * Got rid of continue state, fixed hud loss after continue.
 *
 * Revision 3.72  1996/11/24  10:55:29  gray
 * Adjust Game or Attract volume with aud_dyn_adjvol.
 *
 * Revision 3.71  1996/11/23  22:03:38  petrick
 * win_opts field is only 1 bit now.
 *
 * Revision 3.70  1996/11/23  02:58:06  geraci
 * fix for winner light
 *
 * Revision 3.69  1996/11/22  21:45:29  petrick
 * Turns off winner light at end of game.
 *
 * Revision 3.68  1996/11/22  20:33:53  geraci
 * held music doesn't slow down game task
 *
 * Revision 3.67  1996/11/22  20:11:45  petrick
 * made check buttons static.
 *
 * Revision 3.66  1996/11/21  13:50:23  petrick
 * Made has_shifter a game option.
 *
 * Revision 3.65  1996/11/19  23:17:47  geraci
 * tweaked music flash to 10 secs
 *
 * Revision 3.64  1996/11/19  23:12:20  geraci
 * music button flashes at start of race
 *
 * Revision 3.63  1996/11/17  14:26:36  petrick
 * Checks for external join button in main game loop.
 *
 * Revision 3.62  1996/11/17  04:30:39  petrick
 * Implemented 'reset to factory settings' game option.
 *
 * Revision 3.61  1996/11/16  18:56:13  geraci
 * removed old way to end tourney game
 *
 * Revision 3.60  1996/11/15  15:40:42  geraci
 * fixed bug when sw 1 is up and drones want to race again
 *
 * Revision 3.59  1996/11/15  08:04:34  geraci
 * tests for existence of sound
 *
 * Revision 3.58  1996/11/15  06:10:24  petrick
 * Fixed game crash at end if switches 1&2 up.
 *
 * Revision 3.57  1996/11/15  06:02:02  gray
 * Game stats clearing removed form game options.
 *
 * Revision 3.56  1996/11/14  12:47:47  geraci
 * kills sounds after select screen
 *
 * Revision 3.55  1996/11/13  13:13:29  petrick
 * Made sure loads are abortable.
 *
 * Revision 3.54  1996/11/13  11:55:28  geraci
 * killed some reinit zoids and made a better hiscore debug mode
 *
 * Revision 3.53  1996/11/11  18:55:43  petrick
 * Got rid of 'ready' girl.
 *
 * Revision 3.52  1996/11/11  14:27:39  petrick
 * Made loadmodel smarter about allowing aborts.
 *
 * Revision 3.51  1996/11/11  09:19:09  geraci
 * new way to deal with pots
 *
 * Revision 3.50  1996/11/09  13:09:50  petrick
 * Changes to support abortable downloads.
 *
 * Revision 3.49  1996/11/05  12:22:55  petrick
 * Code to handle JoinIn and countdown skipping during track select.
 *
 * Revision 3.48  1996/11/03  05:58:46  geraci
 * split up debug switches and regular game switches
 *
 * Revision 3.47  1996/11/01  11:20:07  geraci
 * difficulty is used during tournament mode and game ends properly during a tourney
 *
 * Revision 3.46  1996/10/31  04:46:29  petrick
 * Game Over handling for tournement mode.
 *
 * Revision 3.45  1996/10/30  05:47:14  petrick
 * Changed win options to coin menu.
 *
 * Revision 3.44  1996/10/30  05:41:43  geraci
 * added time to drone test mode
 *
 * Revision 3.43  1996/10/29  08:36:46  petrick
 * Got rid of debug stuff.
 *
 * Revision 3.42  1996/10/29  08:31:30  petrick
 * Added game continue and cleanup stuff.
 *
 * Revision 3.41  1996/10/28  10:06:14  petrick
 * Got rid of some debug.
 *
 * Revision 3.40  1996/10/25  11:02:57  geraci
 * removed time stamp stuff
 *
 * Revision 3.39  1996/10/25  06:45:08  petrick
 * Took out shared high score tables.
 *
 * Revision 3.38  1996/10/24  09:26:40  petrick
 * Added win options.
 *
 * Revision 3.37  1996/10/24  08:44:05  geraci
 * coin sound work during attract, race announcer fades out after 4 secs.
 *
 * Revision 3.36  1996/10/23  04:12:36  petrick
 * Doesn't turn off tourney mode setup at startup.
 *
 * Revision 3.35  1996/10/23  00:22:45  petrick
 * Made track_no a global.
 *
 * Revision 3.34  1996/10/22  09:40:15  geraci
 * fixed play it again / what's your name bug
 *
 * Revision 3.33  1996/10/22  08:55:05  geraci
 * new music for high score entry
 *
 * Revision 3.32  1996/10/18  11:01:59  petrick
 * Changed load model.
 *
 * Revision 3.31  1996/10/17  08:23:28  petrick
 * Moved switch 6 display over so fps display can be seen, added resetgamelink to demo endgame.
 *
 * Revision 3.30  1996/10/17  01:01:20  geraci
 * new sputter sound when game time is over
 *
 * Revision 3.29  1996/10/16  07:46:55  geraci
 * initialize moob data and other sound stuff for each game
 *
 * Revision 3.28  1996/10/12  11:13:54  petrick
 * Added pulse display.
 *
 * Revision 3.27  1996/10/11  10:08:19  petrick
 * Added marker and no damage flags.
 *
 * Revision 3.26  1996/10/10  07:27:08  petrick
 * Made tourney mode global.
 *
 * Revision 3.25  1996/10/09  02:16:28  geraci
 * removed start line announcer
 *
 * Revision 3.24  1996/10/08  05:20:58  gray
 * Flagstaff port.
 *
 * Revision 3.23  1996/10/07  22:44:20  petrick
 * Got rid of gray screen at end of game.
 *
 * Revision 3.22  1996/10/07  16:14:36  geraci
 * moved some game options to globals
 *
 * Revision 3.21  1996/10/05  09:10:38  geraci
 * new starting sound bites
 *
 * Revision 3.20  1996/10/04  07:37:34  geraci
 * attract sound is now split into attract effects and attract music
 * got rid of gthisnode debug stuff
 *
 * Revision 3.19  1996/10/04  06:37:35  petrick
 * Removed time setting, added tourney mode switch.
 *
 * Revision 3.18  1996/10/02  21:15:52  petrick
 * Changed params in PutEventReport PDU call.
 *
 * Revision 3.17  1996/09/28  01:52:22  petrick
 * Moved fog stuff out.
 *
 * Revision 3.16  1996/09/27  03:43:31  gray
 * Changes for static model.
 *
 * Revision 3.15  1996/09/27  03:26:51  gray
 * Made camera control local. Eliminated CamFunc.
 *
 * Revision 3.14  1996/09/26  10:02:23  petrick
 * Changes for new high score screens.
 *
 * Revision 3.13  1996/09/26  00:23:30  gray
 * Killed ztypes.h
 *
 * Revision 3.12  1996/09/25  02:04:24  petrick
 * Got rid of include file.
 *
 * Revision 3.11  1996/09/25  00:58:41  gray
 * Removed old ZOID download stuff.
 *
 * Revision 3.10  1996/09/25  00:39:51  petrick
 * Activated fog.
 *
 * Revision 3.9  1996/09/24  22:59:02  petrick
 * Debug msg changes.
 *
 * Revision 3.8  1996/09/23  20:34:06  gray
 * Changed the force_frac to gFeedback x gOffset table.
 * /
 *
 * Revision 3.7  1996/09/20  00:49:16  petrick
 * Changed view angle test to switch 5.
 *
 * Revision 3.6  1996/09/19  14:59:25  petrick
 * Added another bit of data to game over msgs.
 *
 * Revision 3.5  1996/09/19  04:05:43  petrick
 * Made view angle non-static.
 *
 * Revision 3.4  1996/09/18  10:10:36  geraci
 * start line announcer sound effect
 *
 * Revision 3.3  1996/09/14  11:21:30  geraci
 * demo game is now done in attract state
 *
 * Revision 3.2  1996/09/13  23:01:38  geraci
 * play it again tune after game over and into attract
 *
 * Revision 3.1  1996/09/09  02:24:52  gray
 * JAMMA version
 *
 *	======================================================================
 */

#include "attract.h"
#include "blit.h"
#include "carsnd.h"
#include "checkpoint.h"
#include "controls.h"
#include "debug.h"
#include "dis.h"
#include "drones.h"
#include "eer_defs.h"
#include "fmath.h"
#include "maxpath.h"
#include "mdrive.h"
#include "menus.h"
#include "model.h"
#include "options.h"
#include "optmenu.h"
#include "os_proto.h"
#include "resurrect.h"
#include "sounds.h"
#include "stats.h"
#include "world.h"
#include "gamenet.h"

/*==============================  PROTOTYPES  ===============================*/

#include "Pro/camera.pro"
#include "Pro/dnl.pro"
#include "Pro/fswheel.pro"
#include "Pro/hud.pro"
#include "Pro/init.pro"
#include "Pro/math.pro"
#include "Pro/moobs.pro"
#include "Pro/money.pro"
#include "Pro/pots.pro"
#include "Pro/select.pro"
#include "Pro/sounds.pro"
#include "Pro/timint.pro"

/*-----------------------------  PROTOTYPES  --------------------------------*/

extern U32	prc_get_exception_time( int );
extern U32	ctl_mod_latch(int x);
extern U32	factory_setting(const U08 *menu); 
extern void ReadShifter(void);		/* ?? This doesn't show up in pots.pro */

static void	do_gameover(void);
static void	do_endgame(void);
static void check_buttons(void);

/*-------------------------------  DEFINES  ---------------------------------*/

#define GAMEOVER_TIME		( 4 * ONE_SEC)
#define TOURNEYOVER_TIME	(30 * ONE_SEC)
#define QUICKOVER_TIME		(10 * ONE_SEC)

/*-------------------------------  GLOBALS  ---------------------------------*/

S16		which_engine, crash_delay;
U8		radio_station, next_radio_station, game_volume, old_game_volume;
S8		demo_station, in_game_mode;
S16		gForceOffset;
S32		playitagain_time, whats_your_name_time, flash_music_time;
BOOL	carsetup_flag[MAX_LINKS];
const F32 	sethang[] = {90.0, 75.0, 60.0, 45.0, 30.0, 10.0, 175.0, 150.0, 120.0, 110.0};
S32		angi;
BOOL	gPulse;				/* Displays a pulse beat in Switch 6 debug display. */
BOOL	gNeedCleanup;
BOOL	has_shifter;
S16		start_announcer;
const S16  announcer_sound[3] = { S_START1, S_START2, S_START3 };
const S16  kill_announcer[3] = { S_KSTART1, S_KSTART2, S_KSTART3 };
U32 gHeartTime;

/*-------------------------------  EXTERNS  ---------------------------------*/

extern U8 			ad8val[8];
extern S16			last_swforce;
extern S16 			rpm, num_engines;
extern const S16	max_radio;
extern U32			number_checkpoints;
extern Track		CP_Track[];
extern Track_Data	track_data[];
extern Tourney		gTourney;
extern U8			countdown_state;			/* 0=ready to send 1st msg, 1=sending 1st msg, 2=sending countdown msgs */
extern S16		gThisCar;
extern S32 mirror_flag;

/* ----------------------------------------------------------------------------- */

#if TRKFIX
const U32 GO_TDIFF[MAX_TRACKS] =
	{ GO_T1DIFF, GO_T2DIFF, GO_T3DIFF, GO_T4DIFF,
	  GO_T5DIFF, GO_T6DIFF, GO_T7DIFF, GO_T4DIFF };
const U32 GOS_TDIFF[MAX_TRACKS] =
	{ GOS_T1DIFF, GOS_T2DIFF, GOS_T3DIFF, GOS_T4DIFF,
	  GOS_T5DIFF, GOS_T6DIFF, GOS_T7DIFF, GOS_T4DIFF };

const U32 GO_TRKLAPS[MAX_TRACKS] =
	{ GO_TRK1LAPS, GO_TRK2LAPS, GO_TRK3LAPS, GO_TRK4LAPS,
	  GO_TRK5LAPS, GO_TRK6LAPS, GO_TRK7LAPS, GO_TRK4LAPS };
const U32 GOS_TRKLAPS[MAX_TRACKS] =
	{ GOS_TRK1LAPS, GOS_TRK2LAPS, GOS_TRK3LAPS, GOS_TRK4LAPS,
	  GOS_TRK5LAPS, GOS_TRK6LAPS, GOS_TRK7LAPS, GOS_TRK4LAPS };
#endif

void game_init(void)
{
	U08	day,hour,min,attract_sound;
	U32	i, coin_opt;

	/*	Read Option menu settings. */
	guts_opt	= eer_gets( EER_GUTS_OPT );
	game_opt	= eer_gets( EER_GMOPT );
	track_opt	= eer_gets( EER_TRKOPT );
	lap_opt		= eer_gets( EER_LAPOPT );
	coin_opt	= eer_gets( EER_CNOPT );

	/* see if need to clear high scores */
	if (game_opt & GO_CLEARHS)
		{
		ClearHighScores();
		game_opt &= ~GO_CLEARHS;
		eer_puts(EER_GMOPT,game_opt);	/* turn off clear-request	*/
		}

	/*	Set game options to default? */
	if (game_opt & GO_FACTORY)
		{
		eer_puts(EER_GMOPT,factory_setting(gamemenu));
		game_opt = eer_gets( EER_GMOPT );
		}

	/*	Set game difficulty to default? */
	if (track_opt & GO_TRFACTORY)
		{
		eer_puts(EER_TRKOPT,factory_setting(trackmenu));
		track_opt = eer_gets( EER_TRKOPT );
		}

	/* handle all game difficulty settings */
	drone_diff = (track_opt & GO_FREEDIFF) >> GOS_FREEDIFF;
	drone_diff_save = drone_diff;

	for (i=0; i<MAX_TRACKS; i++)
		difficulty_save[i] = difficulty[i] = (track_opt & GO_TDIFF[i]) >> GOS_TDIFF[i];

	for (i=0; i<MAX_TRACKS; i++)
		track_len_save[i] = track_len[i] = ((lap_opt & GO_TRKLAPS[i]) >> GOS_TRKLAPS[i]) + 1;

#if 0                            /* Hack to change T5, T6 & T7 */
    difficulty_save[4] = difficulty[4] = 4;
    track_len_save[4] = track_len[4] = 3;
    difficulty_save[5] = difficulty[5] = 4;
    track_len_save[5] = track_len[5] = 2;
    difficulty_save[6] = difficulty[6] = 4;
    track_len_save[6] = track_len[6] = 2;
#endif

	/* handle all game option settings */
	metric 			= (game_opt & GO_METRIC) >> GOS_METRIC;
#if PRODUCTION_RELEASE
	/* Disable cheat mode (regardless of BRAM setting) for production. */
	cheats			= 0;
#else
	cheats			= (game_opt & GO_CHEATS) != 0;
#endif
	attract_sound 	= (game_opt & GO_ATTRACT_AUDIO) >> GOS_ATTRACT_AUDIO;
	language		= (game_opt & GO_LANG) >> GOS_LANG;
	gForceOffset	= (game_opt & GO_FORCE) >> GOS_FORCE;
	tourney_setup	= (game_opt & GO_TOURNEY) >> GOS_TOURNEY;
	marker_flag		= (game_opt & GO_MARKER) != 0;
	damage_flag		= (game_opt & GO_DAMAGE) != 0;
	has_shifter		= (game_opt & GO_SHIFTER) != 0;
	HAS_SHIFTER		= 1;
#if 0
	win_opts		= (coin_opt & 0x10000) >> 16;
#else
	win_opts		= (track_opt & GO_FREE_GAME) >> GOS_FREE_GAME;
#endif
	continue_opts	= cn_discount() > 0;
	gNeedCleanup	= false;

	game_volume		= aud_setvol (-1);
	demo_station	= 0;
	playitagain_time= 0;
	whats_your_name_time = 0;

	switch (attract_sound)
		{
		case 0: attract_effects=false;  attract_music=false;  break;
		case 1: attract_effects=true;   attract_music=false;  break;
		case 2: attract_effects=true;   attract_music=true;   break;
		case 3: attract_effects=false;  attract_music=true;   break;
		}

	attract_effects_save = attract_effects;
	attract_music_save = attract_music;
	marker_flag_save = marker_flag;

	in_game_mode = false;

	InitHUD();
	InitializeDIS();	
	handle_mcast_channel (Initialize, 0xff);

	LoadHighScores();
	LoadTourneySettings();

	gSWforce = 0;

	gHeartTime = 0;

}	/* end game_init */

extern S32 nettime_synced;

/* ----------------------------------------------------------------------------- */
void game(void)
{
	char	numbuf[80];
	S32 	i, j, slot;
	U32		delta_time;
	static int all_in_cdown;

#if DO_PERF
	DBG_perf_frame_init();
	delta_time = prc_get_count();
	prc_get_exception_time(1);	/*clear exception counter*/
	/* Model time added to DBG_perf.ModelTime[PERF_STATE_GAME] */
	DBG_perf.State = PERF_STATE_GAME;
#endif

	if (in_game_mode == false)
		{
		test_sounds();
		SOUND(S_ATTRACT_MODE);

		if (attract_music && !tourney_setup)
			StartRadio (demo_station+max_radio+1);

		in_game_mode = true;

		MarkAlive (gThisNode, IDLE);
		}

	ProcessPDUs(false);

	get_switches (Do_it);

	/*	Check for tourney mode start and external track setting changes. */
	CheckTourneyBox();

	/* hack to force a heartbeat */
	if((IRQTIME - gHeartTime) > ONE_SEC)
		{
		gHeartTime = IRQTIME;
		Heartbeat(true);
		}

#if CP_EDITOR
	check_menu (Do_it, main_menu);
#endif

	/* set car to follow with camera */
	if (dlevels & SW_DEBUG1)
		this_node = model[target_drone].slot;
	else if (trailing_dedges & SW_DEBUG1)
		this_node = gThisNode;

	if (dedges & SW_DEBUG9)
		flyaround (Initialize);
	else if (trailing_dedges & SW_DEBUG9)
		flyaround (Cleanup);

	get_credits();	/* Do money stuff */
    
	/* If we want foreground dispatch routines, (a list of functions, one to be executed
	   each frame, cycle every so many frames) put them here */
#if 1
#if 0
 for (i=0; i<MAX_LINKS; ++i)
	SMSG( 1+i,1,"%ld: %s %s %c", (long)i, FlagName(gLink[i].flags), NodeStat(gLink[i].state),
		  (TourneyNode(i) ? 'T' : '.'));
#elif 0
	 {
	 extern void net_stats( int col, int row, int mask );
	 static S32 lastTime;
	 if ((SW_DEBUG6 & dlevels) && lastTime+ONE_SEC < IRQTIME)
		 {
		 net_stats(5,10, 0x06);
		 lastTime = IRQTIME;
		 }
	 }
#endif
	SMSG(44,1, "STEERING= %04x (%5ld)  FORCE= %4ld  RPM:%4ld  Vang:%4.1f", (gSWpos & 0xffff), (long)gSWpos, 
		 (long)last_swforce, (long)rpm, sethang[angi] );
	SMSG(45,5, "POTS= BRAKE:%02x (%4x)   GAS:%02x (%4x)   CLUTCH:%02x (%4x)", 
		  ad8val[ADC_BRAKE],brake, ad8val[ADC_ACCEL],gGasInt, ad8val[ADC_CLUTCH],clutch );
	SMSG(46,4, "EDGES= %08x  LEVELS:%08x  TRAIL:%08x", edges, levels, trailing_edges );
	SMSG(47,40, "LIVE: %02x%c IN: %02x", gLiveNodes, (gPulse) ? '*' : ' ', gInThisGame );
	gPulse = false;
	SMSG(47,27, "gSimID:%3ld", (long)gSimID );
	if (trailing_dedges & SW_DEBUG7)
		ClearText();
#endif

	/* Adjust Game or Attract volume from buttons. */
	aud_dyn_adjvol( gstate == ATTRACT );

	game_stats();	/*log stats from current game state*/

#if 1
	if (cheats && (dedges & SW_DEBUG14) && (dlevels & SW_DEBUG2))
		{
#if 0
		S16 res = dlevels & SW_DEBUG13 ? 1 : 0;
#endif
		S16 res = 0;

		MBOX_ScreenDump (res);
#if 0
		txt_str (20, 20, res ? "High Res" : "Low Res ", WHT_PALB);
#endif
		}
#endif

	switch (gstate) 	/* Main game state (see file gstate.h for definitions) */
		{
		case ATTRACT:
			attract();
			break;
	
		case TRKSEL:	/* Track select */
			TrackSel();
nettime_synced = 0;
			break;
		
		case CARSEL:	/* Car select */
			CarSel();
			break;

		case PREPLAY:	/* Once per game initialization stuff. */
			preplay(true, false);

			SetCountdownTimer (10 * ONE_SEC);
			check_sound_time (Initialize);
			flash_music_time = IRQTIME;
			countdown_state = 2;
			gstate = COUNTDOWN;
			all_in_cdown = 0;
			break;
		
		case COUNTDOWN:
			if (HAS_SHIFTER)
				ReadShifter(); /* For lack of a better place to put this.. */

			Update_MDrive();

			if( !all_in_cdown )
			{
				U32 state = 0;
				S32 i;

				for( i=0; i<MAX_LINKS; i++ )
				{
					if( (gInThisGame & (1<<i) ) &&
						( gn_state[i].d.state == COUNTDOWN ||
						  !gn_state[i].alive ) )
					{
						state |= (1<<i);
					}
				}
				if( state == gInThisGame )
				{
					all_in_cdown = 1;
				}
				else
				{
					ResetCountdownTimer();
				}
			}

 			if( all_in_cdown ) CountDown();
			do_car_setup();

			check_buttons();

			if (!demo_game)
				{
				if (!carsetup_flag[gThisNode])
					SetupCar( gThisNode, game_car[gThisNode].body_type, true );
				carsetup_flag[gThisNode] = true;
				UpdateVisuals(gThisNode); 
				}

			break;

		case PREPLAY2:	/*PREPLAY2 should only contain time critical pregame stuff.*/
			if (dlevels & SW_DEBUG1)
				SetCountdownTimer (700 * ONE_SEC);
			else
				{
				SetCountdownTimerAt0( track_data[trackno].start_time[difficulty[trackno]] );
				SOUND (S_RUSH);
				}

			init_cp_time();
			gstate = PLAYGAME;

#if 1
			/* Audio bug workaround - no starting announcer. */
			start_announcer = -1;
#else
			start_announcer = Random(3.0);
			if (start_announcer >= 3.0)
				start_announcer = 2;

			SOUND (announcer_sound[start_announcer]);
#endif
			StartRadio (radio_station);
			EnableRadioHUD();
			break;

		case PLAYGAME:

			if (HAS_SHIFTER)
				ReadShifter(); /* For lack of a better place to put this.. */

			playgame();

			if (start_announcer != -1 && (GetElapsedTime() > 4 * ONE_SEC))
				{
				SOUND (kill_announcer[start_announcer]);
				start_announcer = -1;
				}

			check_buttons();

			if (coast_flag)
				{
				if (are_all_cars_stopped())
					{
					/* report game over to all */
					PutEventReportPDU( Event_GameOver, 4,
									   Report_Node, gThisNode,
									   Report_Slot, gThisNode,
									   Report_Score, 0, Report_ScoreFlags, 1 );
					gstate = ENDGAME;
					}

				if (!TimeOut())
					{
					coast_flag = false;
					SOUND (S_KSPUTTER);
					}
				}
			else if (TimeOut())
				{
				coast_flag = true;
				SOUND (S_SPUTTER);
				}

			break;

		case ENDGAME:
			do_endgame();
			break;
		
		case GAMEOVER:
			if (dlevels & SW_DEBUG1)
				{
 				update_maxpath_data();
				gstate = PREPLAY;
				ShowGameOver(false);
				demo_game = 1;
				for (i=0; i<8; i++)
					gLink[i].heartbeat = 0;
				}
			else
				do_gameover();
			break;
		
		case HISCORE:
			ResetGameLink();
			EnterHighScore (trackno, game_car[gThisNode].score, "???????", 0,
			                mirror_flag, game_car[gThisNode].body_type);

			/*	Turn off all lights. */
			ctl_mod_latch( ~(LA_VIEW1|LA_VIEW2|LA_VIEW3|LA_MUSIC|LA_ABORT|LA_LEADER) );

			gstate = ATTRACT;
			break;
		
		default:
			break;	
		} /* end: Main game state */

	DrawBlits();

	UpdateEnvirons();

#if DO_PERF
	/* Model time added to DBG_perf.ModelTime[PERF_STATE_MBOX] */
	DBG_perf.State = PERF_STATE_MBOX;
#endif


	ZOID_EndFrame();

#if DO_PERF
	/* Model time added to DBG_perf.ModelTime[PERF_STATE_SPIN] */
	DBG_perf.State = PERF_STATE_SPIN;
	/* Dont count DBG_print_debug time in FrameTime. */
	DBG_perf.FrameTime = prc_get_count() - delta_time;
	DBG_perf.IntTime = prc_get_exception_time(1);
#endif

	DBG_print_debug();
}	/* end game */


/* ------------------------------------------------------------------------- */

BOOL preplay(BOOL useHud, BOOL canAbort)
{
	S32 	i, slot;
	BOOL	loaded;

	init_moobs();
	munge_gLink_data();

	for (i=0; i<MAX_LINKS; i++)        /* Clear out objnums in case aborted */
		game_car[i].objnum = -1;

	gNetDelay = true;			/*	Delay acknowledgement processing until after load. */
	loaded = loadTrack(trackno,useHud,canAbort);
	if (!loaded)
		return false;

	if (dlevels & SW_DEBUG1)
		demo_game = 0;

	if (!demo_game)
		ShowTransit(false,2);

	gNetDelay = false;			/*	Restart acknowledgement processing from here. */
	last_track_loaded = trackno;

	radio_station = trackno % max_radio;    /* Initial Radio staion based on track. */
	next_radio_station = -1;                /* Want next station to be mute */

	if (demo_game == 0)
		{
		SOUND (S_STOP_ALL_SNDS);
		SOUND (S_FANFARE2);
		}

	reinit_zoid();	/*reset zoid, set rush viewangle and ambient.*/

	InitSW();

	/* Initialize checkpoints */
	InitCPS();
	FPutCheckpoints(trackno);

	update_mpath_edit (Allocate);

	init_cars();
	for (i=0; i<MAX_LINKS; i++)
		carsetup_flag[i] = false;

	InitDrones();
	link_maxpath_and_pathpoints();

	/* initialize data for all cars */
	coast_flag = false;
	crash_delay = CRASHDEL;
	for (i=0; i<num_active_cars; i++)
		{
		slot = model[i].slot;
		if ((slot==gThisNode) || (model[slot].we_control))
			Init_MDrive (Initialize, slot);
		}

	StartEnvironment();

	init_view();
	for (i=0; i<MAX_LINKS; i++)
		InitCarSnds(i);
	frictwheel();

	if (!demo_game)
		InitGameScores( );

	if (useHud)
		ShowHUD();

	update_lap_counter (Do_it, -1);

	/* save track and car statistics */
	if (!demo_game) {
	    eer_tally_hist(HIST_CAR, gThisCar);
	    eer_tally_hist(HIST_TRK, trackno);
		if (gMirrorMode)
			eer_incs(EER_MIRROR,1);
	}

	SetFog(FOG_TRACK, 0, 0);

	return true;
}	/* end preplay */


/* ------------------------------------------------------------------------- */

void playgame (void)
{
	S32 	i, slot;

	if (dedges & SW_DEBUG5)
		{
		angi = (angi+1) % 10;
		MBOX_SetWindow(sethang[angi],0);
		}

#if 0 /* JRG: immediately stops game after start then enter hiscore table */
	if (dlevels & SW_DEBUG13)
		{
		PutEventReportPDU( Event_GameOver, 4,
						   Report_Node, gThisNode,
						   Report_Slot, gThisNode,
						   Report_Score, GetElapsedTime(),
						   Report_Flags, 0 );
		return;
		}
#endif

	Update_MDrive();
	DoDrones();
	CheckCPs();

	for (i=0; i<num_active_cars; i++)
		{
		slot = model[i].slot;
		CheckCrash (slot);

		/*	See if it's time to update the visual effects. */
		UpdateVisuals (slot);

		if (slot==this_node || model[slot].we_control)
			TargetCheck (slot);

		if (game_car[slot].we_died)
			{
			if (slot == gThisNode)
				{
				gSWforce = 0;
				}

			End_MDrive (slot);
			Init_MDrive (Do_it, slot);

			if (slot == gThisNode)
				{
				frictwheel();
				}
			}
		}

	/* now detect abort for drones we control */
	detect_drone_abort();
}

/* ----------------------------------------------------------------------------- */
static void do_endgame(void)
{
	S16  i;

	update_lap_counter (Cleanup, 0);
	attract_music = attract_music_save;

	if (game_car[gThisNode].place_locked != 1)
		{
		SOUND (S_STOP_ALL_SNDS);
		SOUND (S_PLAYITAGAIN);
		playitagain_time = IRQTIME;
		whats_your_name_time = 0;
		}

	for (i=0; i<num_active_cars; i++)
		End_MDrive (model[i].slot);

	modelstop();

	freewheel();
	gSWforce = 0;
	gTourneyStart = 0;

	/* Stop engine sound here */
	StopEngineSound();

	ShowGameOver(true);
	SetCountdownTimer(TourneyOn() ? (gTourney.ext_start == JOIN_QUICK ? QUICKOVER_TIME : TOURNEYOVER_TIME) : GAMEOVER_TIME);

	gstate = GAMEOVER;
}	/* end do_endgame */


/* ----------------------------------------------------------------------------- */
static void do_gameover(void)
{
	S16 i;
	S32	msec;

	if (TimeOut() || (!TourneyOn() && (edges & SW_ABORT)))
		{
		gNeedCleanup = true;
		CleanupAfterGame();

		/* Load Hi Score screen model. */
		reinit_zoid();
		gstate = HISCORE;
		ctl_mod_latch( ~LA_ABORT );
		return;
		}

	if (!TourneyOn())
		BlinkUnlessPressed( LA_ABORT, SW_ABORT );
	else if ((edges & SW_MUSIC) && (msec = GetCountdownTime()) > 2*ONE_SEC)
		SetCountdownTimer( msec - ONE_SEC );
}	/* end do_gameover */


/* ------------------------------------------------------------------------- */

void demo_endgame (void)
{
	S16  i;

	if (playitagain_time==0 && attract_effects)
		SOUND (S_STOP_ALL_SNDS);

	for (i=0; i<num_active_cars; i++)
		End_MDrive (model[i].slot);

	modelstop();

	freewheel();
	gSWforce = 0;
	/* Stop engine sound here */
	StopEngineSound();

	ShowGameOver(false);

	/*reset index so player car gets recreated next game*/
	RemoveEnvirons();

	/*	Clean up cars and visuals. */
	for (i=0; i<MAX_LINKS; ++i)
		RemoveCar(i);

	/* Reload ATTRACT model. */
	reinit_zoid();
	ZeroCamera();
	ctl_mod_latch( ~(LA_ABORT|LA_WINNER) );
	ResetGameLink();

}	/* end do_gameover */


/* ------------------------------------------------------------------------- */

void do_car_setup (void)
{
	S16  i, slot;

	for (i=0; i<num_active_cars; i++)
		{
		slot = model[i].slot;	
		if (model[slot].data_valid)
			{
			if (!carsetup_flag[slot])
				SetupCar( slot, game_car[slot].body_type, true );
			carsetup_flag[slot] = true;
			UpdateVisuals(slot); 
			}
		}
}	/* end do_car_setup */


/* ------------------------------------------------------------------------- */
void CleanupAfterGame(void)
{
	S16 i;

	if (!gNeedCleanup)
		return;

	ShowGameOver(false);

	/*reset index so player car gets recreated next game*/
	RemoveEnvirons();
	
	/*	Clean up cars and visuals. */
	for (i=0; i<MAX_LINKS; ++i)
		RemoveCar(i);

	gNeedCleanup = false;
}	/* end CleanupAfterGame */

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Check for music and abort                                            */

static void check_buttons (void)
{
	S16  i;

	if (flash_music_time != 0)
		{
		if (IRQTIME - flash_music_time < 10 * ONE_SEC)
			{
			if (radio_station == max_radio)
				BlinkUnlessPressed (LA_MUSIC, SW_RADIO);
			else
				flash_music_time = 0;
			}
		else
			{
			flash_music_time = 0;
			ctl_mod_latch (~LA_MUSIC);
			}
		}

	if (edges & SW_RADIO) 
		{
#if 1           /* skip ahead and game abort are temporary debug goodies (JRG) */
		/*	Now the cheats can be (de)activated through the game options menu. */
		if (cheats && (levels & SW_ABORT))
			{
			if (levels & SW_VIEW1)
				{
				set_game_time_to_one_second();

				PutEventReportPDU( Event_GameOver, 4,
								   Report_Node, gThisNode,
								   Report_Slot, gThisNode,
								   Report_Score, 0, Report_ScoreFlags, 1 );
				}
			else
				jump_to_next_checkpoint();
			}
		else
#endif
			{
			StopRadio (radio_station);
			radio_station = next_radio_station;
			if (++next_radio_station > max_radio) 
				next_radio_station = 0;
			if (gstate != COUNTDOWN)
			    StartRadio (radio_station);
			EnableRadioHUD();
			}
		}

	else if (edges & SW_ABORT && gstate == PLAYGAME)
		{
		if ((  model[this_node].resurrect.moving_state == -1)
			&& (coast_flag == false)
			&& (end_game_flag == false))
			{
			abort_car (this_node);
			}
		}
}
