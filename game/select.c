/*
 *  select.c    -- Functions for handling the select screens.
 *
 *  Copyright 1996 Time Warner Interactive.
 *  Unauthorized reproduction, adaptation, distribution, performance or
 *  display of this computer program or the associated audiovisual work
 *  is strictly prohibited.
 *
 *  ======================================================================
 *  $Author: hightower $                       $Date: 1997/10/16 02:09:11 $
 *  $Revision: 3.147 $                      $Locker:  $
 *  ======================================================================
 *  Change Log:
 *
 *  $Log: select.c,v $
 * Revision 3.147  1997/10/16  02:09:11  hightower
 * Attempt to fix link bug
 *
 * Revision 3.146  1997/10/15  05:59:23  hightower
 * Terrible last minute hack to sync timers in game
 *
 * Revision 3.145  1997/10/15  02:58:46  hightower
 * Synchonize countdown timers during race
 *
 * Revision 3.144  1997/10/11  21:07:00  hightower
 * Made drone cars the same
 *
 * Revision 3.143  1997/10/11  10:40:35  hightower
 * Timer more tolerant of time diffs
 *
 * Revision 3.142  1997/10/11  07:35:22  hightower
 * Hold off on track negotiation all stacks are track locked
 *
 * Revision 3.141  1997/10/11  01:43:44  grjost
 * Fixed car select track map & disabled tourney transmission preselect
 * for certain cases.
 *
 * Revision 3.140  1997/10/11  00:19:39  hightower
 * Prevent waiting for drivers sound when someone starts a solo race
 *
 * Revision 3.139  1997/10/10  01:05:19  hightower
 * Had _SELECT_TIME instead of _DELAY_TIME
 *
 * Revision 3.138  1997/10/10  00:48:14  hightower
 * dont ask
 *
 * Revision 3.137  1997/10/10  00:16:54  hightower
 * lock trigger change
 *
 * Revision 3.136  1997/10/09  23:37:21  hightower
 * Fixed bad blit on wheel and pedal
 *
 * Revision 3.135  1997/10/09  23:36:47  grjost
 * Fixed tourney car select not showing up
 *
 * Revision 3.134  1997/10/09  20:39:42  hightower
 * Prevent waiting for drivers between tourney and non-tourney
 *
 * Revision 3.133  1997/10/09  05:39:54  hightower
 * Blink stuff based on countdown timer to synchronize blinking
 *
 * Revision 3.132  1997/10/09  05:11:23  hightower
 * Hopefully fixed solo mode bug
 *
 * Revision 3.131  1997/10/09  03:02:55  hightower
 * Got rid of inappropriate 8-way message
 *
 * Revision 3.130  1997/10/08  18:39:54  hightower
 * Fixed tourney mode attract mode cycling
 *
 * Revision 3.129  1997/10/07  20:02:33  hightower
 * Fixed bug when announcer was saying waiting for players at wrong time
 *
 * Revision 3.128  1997/10/06  19:20:19  hightower
 * bug: sounds stopped in attract after game - link fix attempt
 *
 * Revision 3.127  1997/10/06  19:16:42  grjost
 * Changed CAR_DELAY_TIME to one second
 *
 * Revision 3.126  1997/10/03  13:52:28  hightower
 * Fix music play bug and allow punch down if no slowpokes
 *
 * Revision 3.125  1997/10/02  20:40:00  hightower
 * Made more better
 *
 * Revision 3.124  1997/10/02  19:37:37  hightower
 * Fixed link bug due to timers getting too far off
 *
 * Revision 3.123  1997/10/02  19:03:40  hightower
 * Hack to prevent music from playing in attract mode
 *
 * Revision 3.122  1997/10/02  01:26:31  grjost
 * Fixed mirrored track map offset
 *
 * Revision 3.121  1997/10/01  21:06:28  hightower
 * Removed tabs
 *
 * Revision 3.120  1997/10/01  03:37:41  hightower
 * Changed countdown timers to be more accurate
 *
 * Revision 3.119  1997/09/30  02:51:16  hightower
 * Made the track announce work and added random sounds
 *
 * Revision 3.118  1997/09/29  19:08:22  hightower
 * Polish for the countdown timers
 *
 * Revision 3.117  1997/09/27  01:22:00  grjost
 * scaled down "waiting for others"
 *
 * Revision 3.116  1997/09/26  22:25:14  hightower
 * Synchro set to one sec
 *
 * Revision 3.115  1997/09/26  21:30:09  hightower
 * Synchronized timers in track select mode
 *
 * Revision 3.114  1997/09/26  02:24:53  grjost
 * Added mirror mode selection
 *
 * Revision 3.113  1997/09/26  00:13:10  hightower
 * Now using gamenet to fix ability to join within last few seconds
 *
 * Revision 3.112  1997/09/25  03:03:12  gray
 * hack in mirror mode select.
 *
 * Revision 3.111  1997/09/17  19:40:58  grjost
 * Put redraw hack back in.
 *
 * Revision 3.110  1997/09/17  07:54:17  grjost
 * Fixed blits positions with newest select graphics.
 *
 * Revision 3.109  1997/09/16  22:02:57  grjost
 * Works with new select screen w/ 7 tracks
 *
 * Revision 3.108  1997/09/15  22:30:23  gray
 * Removed trackset hack - trackno now 0 to NTRACKS.
 *
 * Revision 3.107  1997/09/13  01:48:49  grjost
 * Fixed for 12 cars.
 * But still can't show more than 8 cars.  Looks like game_car[]
 * isn't being initiated for the 4 new cars.
 *
 * Revision 3.106  1997/09/04  18:00:58  gray
 * Now says The Rock instead of Track 4.
 *
 * Revision 3.105  1997/09/02  00:21:31  gray
 * Made tracks 5-8 available only if switch 11 up.
 *
 * Revision 3.104  1997/08/28  21:14:19  gray
 * Added sound calls for track-picked and wiating-for-others for 8 tracks.
 *
 * Revision 3.103  1997/08/28  21:09:03  grjost
 * Fixed placement of track map.
 *
 * Revision 3.102  1997/08/27  22:44:09  grjost
 * Fixed blit offsets for new select screen
 *
 * Revision 3.101  1997/08/18  20:20:36  gray
 * Added track select maps for T5-8.
 *
 * Revision 3.100  1997/07/27  22:29:14  gray
 * Checkpoint set select for consumer track dev.
 *
 * Revision 3.99  1997/07/01  18:58:32  gray
 * Made alternate tracks (additional 4 tracks) view 1 selectable.
 *
 * Revision 3.98  1997/06/28  01:42:52  gray
 * Extended to 4 tracks.
 *
 * Revision 3.97  1997/06/04  02:44:44  gray
 * Zapped ithrottle.
 *
 * Revision 3.96  1997/06/04  02:15:46  gray
 * Restructuring controls (gas,brake,clutch,etc) handling.
 *
 * Revision 3.95  1997/05/30  19:02:58  gray
 * Got rid of FLAGSTAFF conditionals.
 *
 * Revision 3.94  1997/03/21  21:53:53  gray
 * Fixed cars switching on view1 after car lock.
 *
 * Revision 3.93  1997/03/20  13:47:48  geraci
 * new message type for use in linking
 *
 * Revision 3.92  1997/03/18  20:09:47  geraci
 * fixed soloing out during tournament, sim manager selected at different time
 * negotiation adds players properly
 *
 * Revision 3.91  1997/03/13  04:02:52  geraci
 * added multi-cast channel selection and negotiation
 *
 * Revision 3.90  1997/03/04  00:45:29  gray
 * Make quick tourney time work.
 *
 * Revision 3.89  1997/02/28  02:55:28  geraci
 * tourney car select sound shut-off used wrong value
 *
 * Revision 3.88  1997/02/28  00:01:08  gray
 * hack to allow solo fall through without everyone falling through.
 *
 * Revision 3.87  1997/02/27  23:52:21  geraci
 * fixed black screen during track select problem
 *
 * Revision 3.86  1997/02/26  03:49:57  geraci
 * sound cue during join now
 *
 * Revision 3.85  1997/02/22  02:08:00  gray
 * Brute force keep car select objects from dissappearing.
 *
 * Revision 3.84  1997/02/15  01:46:43  geraci
 * join quick remote tourney mode
 *
 * Revision 3.83  1997/01/31  23:35:10  geraci
 *  removed difficulty from tourney setup screen
 *
 * Revision 3.82  1997/01/24  02:18:02  geraci
 * new checkpoint scheme
 *
 * Revision 3.81  1997/01/15  20:03:03  geraci
 * number of laps for track 2 and 3 can be changed by operator
 *
 * Revision 3.80  1996/12/14  10:34:51  geraci
 * waits between links sounds
 *
 * Revision 3.79  1996/12/14  08:50:38  gray
 * Hack to keep cars on pads in car select.
 *
 * Revision 3.78  1996/12/13  13:25:52  geraci
 * *** empty log message ***
 *
 * Revision 3.77  1996/12/06  17:28:53  geraci
 * select wait for track and transmission select to be locked for 1/2 sec
 *
 * Revision 3.76  1996/11/27  20:44:59  geraci
 * lowered force on car select
 *
 * Revision 3.75  1996/11/27  08:09:52  petrick
 * Fixed shifter disabler.
 *
 * Revision 3.74  1996/11/25  06:34:21  petrick
 * Added tourney status to join msgs.
 *
 * Revision 3.73  1996/11/25  04:16:04  petrick
 * Got rid of reverse button toggling view angle.
 *
 * Revision 3.72  1996/11/22  10:17:52  petrick
 * Disables shifter select if has_shifter is false.
 *
 * Revision 3.71  1996/11/21  13:54:05  petrick
 * Car glow remains on after selection.
 *
 * Revision 3.70  1996/11/20  22:48:20  geraci
 * 8 way works
 *
 * Revision 3.69  1996/11/20  21:37:49  geraci
 * added sound effect on initial track track select
 *
 * Revision 3.68  1996/11/19  17:50:12  petrick
 * Centralized loading.
 *
 * Revision 3.67  1996/11/18  12:37:22  petrick
 * Force track lock if timer is < 1 sec.
 *
 * Revision 3.66  1996/11/14  01:31:14  petrick
 * Changed hidden options to V2 & V3 instead of V1 & V2.
 *
 * Revision 3.65  1996/11/13  11:58:31  petrick
 * Made sure all loads can abort OK.
 *
 * Revision 3.64  1996/11/13  07:33:29  petrick
 * Gives more time before timing others out during countdown.
 *
 * Revision 3.63  1996/11/11  22:06:54  geraci
 * temp debug stuff
 *
 * Revision 3.62  1996/11/11  18:55:43  petrick
 * Got rid of 'ready' girl.
 *
 * Revision 3.61  1996/11/11  14:26:42  petrick
 * Made loadmodel smarter about allowing aborts.
 *
 * Revision 3.60  1996/11/11  10:50:52  petrick
 * External track selection switch.
 *
 * Revision 3.59  1996/11/09  13:09:02  petrick
 * Added a transition screen after clutch select.
 *
 * Revision 3.58  1996/11/07  08:18:24  gray
 * Car shading and reflection stuff.
 *
 * Revision 3.57  1996/11/07  00:40:16  petrick
 * Hood no longer appears in car select.
 *
 * Revision 3.56  1996/11/06  13:49:26  geraci
 * uses one channel for each additional player in link
 *
 * Revision 3.55  1996/11/05  12:22:55  petrick
 * Code to handle JoinIn and countdown skipping during track select.
 *
 * Revision 3.54  1996/11/05  10:56:19  geraci
 * new linking sound effects
 *
 * Revision 3.53  1996/11/03  06:05:06  geraci
 * kills turn table sound once
 *
 * Revision 3.52  1996/11/02  04:03:28  geraci
 * transmitted difficulty in car message
 *
 * Revision 3.51  1996/11/01  11:22:31  geraci
 * fixed difficulty settings not getting to late link players bug
 *
 * Revision 3.50  1996/10/31  09:32:27  petrick
 * Tourney mode fixes for catchup and drones flags.
 *
 * Revision 3.49  1996/10/31  04:47:03  petrick
 * Tourney mode changes.
 *
 * Revision 3.48  1996/10/25  11:03:25  geraci
 * all cars have their own beeping sound during select
 *
 * Revision 3.47  1996/10/25  06:51:04  petrick
 * Added tourney mode visual reminder.
 *
 * Revision 3.46  1996/10/24  11:42:05  geraci
 * added beep for second set of cars
 *
 * Revision 3.45  1996/10/24  09:28:56  petrick
 * Made car icons have fixed slots.
 *
 * Revision 3.44  1996/10/23  12:29:08  petrick
 * Fixed tourney mode clutch locking.
 *
 * Revision 3.43  1996/10/23  11:19:36  petrick
 * Tourney mode changes.
 *
 * Revision 3.42  1996/10/23  08:34:24  petrick
 * New method of displaying car icons.
 *
 * Revision 3.41  1996/10/23  00:24:59  petrick
 * Made track_no a global.
 *
 * Revision 3.40  1996/10/22  10:36:38  petrick
 * Changes for 8 car types.
 *
 * Revision 3.39  1996/10/19  02:14:58  petrick
 * Avoids track not-lock problem.
 *
 * Revision 3.38  1996/10/18  11:19:26  petrick
 * Changed Load model.
 *
 * Revision 3.37  1996/10/17  11:12:55  petrick
 * More track select icon placement.
 *
 * Revision 3.36  1996/10/17  08:18:53  petrick
 * Put car icons in the right spot in track select.
 *
 * Revision 3.35  1996/10/16  09:35:25  petrick
 * Made updateCar non-static.
 *
 * Revision 3.34  1996/10/12  11:23:39  petrick
 * Added Timer to Join Now, consolidated help display.
 *
 * Revision 3.33  1996/10/10  05:22:05  petrick
 * Got rid of hack offline/online msgs.
 *
 * Revision 3.32  1996/10/09  05:49:22  petrick
 * Cleanup.
 *
 * Revision 3.31  1996/10/08  05:20:58  gray
 * Flagstaff port.
 *
 * Revision 3.30  1996/10/07  16:18:16  geraci
 * send default difficulty across net to all linked players.
 *
 * Revision 3.29  1996/10/07  02:08:09  petrick
 * Added difficulty to track lock msg, japanese 'grip'.
 *
 * Revision 3.28  1996/10/05  10:15:10  petrick
 * New transmission select.
 *
 * Revision 3.27  1996/10/05  09:20:19  geraci
 * calls raw wheel section routine at appropriate places
 *
 * Revision 3.26  1996/10/04  10:39:05  petrick
 * Added delay before locking choice, moved msgs around for better visibility.
 *
 * Revision 3.25  1996/10/02  21:20:15  petrick
 * Changed params in PutEventReport PDU call.
 *
 * Revision 3.24  1996/10/01  08:20:11  petrick
 * Cleanup.
 *
 * Revision 3.23  1996/09/28  01:48:33  petrick
 * Got rid of unused extern reference.
 *
 * Revision 3.22  1996/09/27  03:26:51  gray
 * Made camera control local. Eliminated CamFunc.
 *
 * Revision 3.21  1996/09/27  01:04:20  petrick
 * Resets unlocked models only.
 *
 * Revision 3.20  1996/09/26  10:01:42  petrick
 * Changes for new high score screens.
 *
 * Revision 3.19  1996/09/26  00:23:30  gray
 * Killed ztypes.h
 *
 * Revision 3.18  1996/09/25  02:10:34  petrick
 * Got rid of include file.
 *
 * Revision 3.17  1996/09/24  01:12:36  petrick
 * Moved a few blits that were off by 1 pixel. Turned off View 3 light if !KETCHUP.
 *
 * Revision 3.16  1996/09/23  20:31:18  petrick
 * Restored offcenter_wheel.
 *
 * Revision 3.15  1996/09/23  07:46:15  petrick
 * Widened car and track select ranges.
 *
 * Revision 3.14  1996/09/21  05:22:26  petrick
 * Brought back danger msg.
 *
 * Revision 3.13  1996/09/21  04:53:57  petrick
 * 4 cars.
 *
 * Revision 3.12  1996/09/20  00:50:22  petrick
 * Changed view angle test switch to switch 5.
 *
 * Revision 3.11  1996/09/19  15:08:03  petrick
 * Fixed clutch locked timeout problem.
 *
 * Revision 3.10  1996/09/19  06:47:32  petrick
 * Referenced select.h.
 *
 * Revision 3.9  1996/09/19  04:52:05  petrick
 * Added danger bars.
 *
 * Revision 3.8  1996/09/18  07:59:40  petrick
 * Blit wheel, dims out panel when waiting.
 *
 * Revision 3.7  1996/09/17  21:41:03  petrick
 * Added a cage around the mustang.
 *
 * Revision 3.6  1996/09/17  15:47:30  petrick
 * Better geometry steering wheel.
 *
 * Revision 3.5  1996/09/17  15:08:31  petrick
 * Geometry steering wheel, temporary edition.
 *
 * Revision 3.4  1996/09/14  08:57:27  petrick
 * New track select.
 *
 * Revision 3.3  1996/09/13  23:13:24  geraci
 * three two one rush countdowd, extreme car klaxon and crowd scream
 *
 * Revision 3.2  1996/09/10  21:31:17  geraci
 * temp hack to make extreme car hidden during select
 *
 * Revision 3.1  1996/09/09  02:24:52  gray
 * JAMMA version
 *
 *  ======================================================================
 */

#define MIRROR_HACK 1

#include "attract.h"
#include "controls.h"
#include "debug.h"
#include "dis.h"
#include "drivsym.h"
#include "fmath.h"
#include "game.h"
#include "globals.h"
#include "mb.h"
#include "model.h"
#include "modeldat.h"
#include "options.h"
#include "os_proto.h"
#include "select.h"
#include "sounds.h"
#include "targets.h"
#include "gamenet.h"

#include "Pro/camera.pro"
#include "Pro/dnl.pro"
#include "Pro/fmath.pro"
#include "Pro/fswheel.pro"
#include "Pro/init.pro"
#include "Pro/math.pro"
#include "Pro/mathsubs.pro"
#include "Pro/pots.pro"
#include "Pro/sounds.pro"
#include "Pro/tourney.pro"


/*===============================  DEFINES  =================================*/

#define KETCHUP         0       /* 1 == enable catchup/no catchup selection. */

#define TRACKMAP_X      (152)   /* X position of track map */

#define CARROW1         ( 12)   /* Y offset of car display from track display */
#define CARROW2         ( 36)   /* ditto 2nd row */
#define CARROW3         ( 78)   /* ditto 3rd row */
#define CARROW4         (102)   /* ditto 4th row */
#define CARROW5         (145)   /* ditto 5th row */
#define CARROW6         (169)   /* ditto 6th row */
#define CARROW7         (211)   /* ditto 7th row */
#define CARROW8         (235)   /* ditto 8th row */
#define CARROW_Y        (24)    /* y-dis between rows */
#define CARTRK_Y        (67)    /* y-dis between car displays */
#define LINKCOL1        12
#define LINKCOL2        448
#define DRONE_CARNUM    1       /* Selects which car is used for drones */
#define NPADS           4       /* Number of pads to hold cars in select. */

#define WHEEL_THIRD     (WHEEL_RANGE / 3)       /* Width of a section of wheel range */
#define WHEEL_SLICE     (WHEEL_RANGE / NPADS)   /* Width of a 1/4 wheel range section */
#define GAS_TRIGGER     (2 * (0x1000) / 3)      /* Position of gas pedal to trigger action. */
#define CLUTCH_TRIGGER  (2 * (0x1000) / 3)      /* Position of clutch pedal to trigger action. */
#define CLUTCH_SEL_TIME (11 * ONE_SEC)  /* Amount of time to select a clutch. */
#define TRK_DELAY_TIME  (ONE_SEC)       /* Delay before track lock is allowed. */
#define CAR_DELAY_TIME  (ONE_SEC)         /* Delay before car lock can occur */
#define TRANS_DELAY_TIME (ONE_SEC / 3)    /* Delay before trans lock can occur */

#define CAR_NAME        "CARICON"       /* Default car icon name. */

#define HURRY_SPEED     ONE_SEC /* How fast hurry-up coutdown works. */

/* Probably want this to be 0 in the final version. */
/* Leave as 1 for now so we can demonstrate ultra wimpy force. AG */
#define DEFAULT_FEEDBACK_LEVEL 1

#define MAX_SELCAMPOS   3       /*  Maximum number of camera positions. */
#define ROTOR_OFFSET    (100.0 * PI / 180.0)    /*  Angle to turn the rotor to line up with ramp. */
#define MAX_ACC         0.0025
#define MAX_VEL         0.05
#define BLINK_TIME      0x100   /*  How fast to blink. */

enum CarMoves
{
    CM_NOT = 0, CM_SETUP, CM_OFFPAD, CM_TURN, CM_FISHTAIL, CM_PEELOUT, CM_DONE
};

enum
{
    LS_INIT, LS_WAITING, LS_SOLO, LS_LINKED, LS_8WAY
};

/*==============================  PROTOTYPES  ===============================*/

static BlitFunc AnimateTrans, AnimateFeedback, AnimateBar, AnimateSolo,
  AnimateCar, AnimateCarSel, AnimateTrack, AnimateWait, AnimateWaitJ, AnimateDanger,
  AnimateTrkNum, AnimatePanel,
#if KETCHUP
  AnimateCatchup,
#endif
  AnimateGlow, AnimateDrone, AnimateSWheel, AnimateLink;

static void ClutchSel (void);
static S16 do_link_sound (S16 link_state);
static void ShowCountdown (BOOL active);

extern U32 ctl_mod_latch (int x);

/*===============================  EXTERNS  =================================*/

extern const F32 unituvs[3][3];
extern const F32 zerovector[3];
extern S8 view;
extern const F32 sethang[];
extern S32 angi;
extern Tourney gTourney;
extern BOOL has_shifter;
extern BOOL gGameInited;
extern U32 current_mcast_channel;
extern SFONTINFO sfontX;

/*===============================  GLOBALS  =================================*/

U32 start_time,                 /*  IRQTIME when game started */
  play_time;                    /*  Msecs of time to play. */
U8 gFeedbackLevel;
BOOL gWait4TrkPlaying;          /* "waiting for drivers on track n" playing */
S16 gThisCar;
BOOL gCarLocked;                /* Need this global for game stats */
BOOL gClutchLocked;
S16 num_pads;                   /* number of cars selectable */
S8 negotiate_track;             /* true = we are in track negotiation phase */
U32 negotiate_tdata[MAX_LINKS]; /* save track negotiation data here */
U32 negotiate_twant[MAX_LINKS]; /* save what each stack is looking for in link */
U32 negotiate_ttime[MAX_LINKS]; /* time each track message was received */
U32 negotiate_tphase[MAX_LINKS];        /* each new group of tries should have a different phase number */
U32 negotiate_tchan[MAX_LINKS * 2];     /* channel node has control of */
U32 negotiate_time;             /* timer used for negotiation */
U32 drop_out_time;              /* next time to drop out un-heard from stacks */
S8 negotiate_race;              /* true = we are in race negotiation phase */
BOOL re_negotiate;              /* true if we encounter a higher negotiation phase than our own */
U8 countdown_state;             /* 0=ready to send 1st msg, 1=sending 1st msg, 2=sending countdown msgs */
S32 last_countdown_time;        /* timer used for count down */

static F32 rotAngle, rotOffset;
static U8 drones_flag,          /*  Describe car-select settings */
  catchup_flag;

static S16 gLitPad;
static BLIT *TrackBlit, *CarBlit, *CountBlit, *ClutchBlit;
static BOOL GasReleased;

static short oldTrack, old_num_on;

static S32 gRotorObj, gScreenObj, gRoomObj, gDoorObj, gFanObj, gPadObj[NPADS];
static S32 gArrowObj, gLevelObj, gBackObj, gCageObj;
static S16 gLastCar, gPickPad, gCarMove, gCarFirstSel;
static U32 gPickTime, gBeginTime;
static F32 gPadPitch;
static BOOL gSoundOn, gAltCars;
static S32 tlock_sound_delay;
static S32 all_tracks_locked_time;
S32 all_cars_locked_time;
S32 mirror_flag;

S16 gCurCam, gNextCam, gCamFrames;      /*  Cur and next cam indices into gSelCamPos, frames until nextCam */
BOOL gCameraTransition;

const S16 car_type_sound[4] =
{S_BEEPWARN1, S_BEEPWARN2, S_BEEPWARN3, S_KLAXON};
const S16 kill_car_type_sound[4] =
{S_KBEEPWARN1, S_KBEEPWARN2, S_KBEEPWARN3, S_KKLAXON};
const S16 track_picked_sound[8] =
{S_YOU1, S_YOU2, S_YOU3, S_YOUROCK,
 S_YOU5, S_YOU6, S_YOU7, S_YOU8};
const S16 waiting_on_sound[8] =
{S_WAIT1, S_WAIT2, S_WAIT3, S_WAITROCK,
 S_WAIT5, S_WAIT6, S_WAIT7, S_WAIT8};
const S16 others_locked_sound[7] =
{S_2CARS, S_3CARS, S_4CARS, S_5CARS, S_6CARS, S_7CARS, S_8CARS};
const S16 kill_others_locked_sound[7] =
{S_K2CARS, S_K3CARS, S_K4CARS, S_K5CARS, S_K6CARS, S_K7CARS, S_K8CARS};

/* ----------------------------------------------------------------------------- */

static MULTIBLIT const TrackMultiBlit[] =
{                               /* name                             x         y whtblrfz alpha animFunc        id */
    {"PANEL00", 0, 0, MUNUSED, 0x00, nil, 0},
    {"SWHEEL01", 295, 54, MUNUSED, 0x00, AnimateSWheel, 0},
    {"PANEL01", 256, 0, MUNUSED, 0x00, AnimatePanel, 0x00},
    {"PANEL02", 0, 256, MUNUSED, 0x00, nil, 0},
    {"PANEL03", 256, 256, MUNUSED, 0x00, AnimatePanel, 0x01},
    {"CARICONGLOW", 0, 0, MUNUSED, 0x00, AnimateGlow, 0x00},
    {"LINKLIGHT", 80, 50, MUNUSED, 0x00, AnimateLink, 0},
    {"SOLO", 85, 46, MUNUSED, 0x00, AnimateSolo, 0},
    {"BLACK2", 0, 0, MUNUSED, 0xFF, AnimateTrans, 3},
    {"TRKPIC01", TRACKMAP_X, 12, MUNUSED, 0x00, AnimateTrack, -1},
    {"TRKNUMBER1", 0, 0, MUNUSED, 0x00, AnimateTrkNum, -1},
    {CAR_NAME, LINKCOL1 + 0, CARROW1, MUNUSED, 0x00, AnimateCar, 0x00},
    {CAR_NAME, LINKCOL1 + 14, CARROW1, MUNUSED, 0x00, AnimateCar, 0x01},
    {CAR_NAME, LINKCOL1 + 28, CARROW1, MUNUSED, 0x00, AnimateCar, 0x02},
    {CAR_NAME, LINKCOL1 + 42, CARROW1, MUNUSED, 0x00, AnimateCar, 0x03},
    {CAR_NAME, LINKCOL1 + 0, CARROW2, MUNUSED, 0x00, AnimateCar, 0x04},
    {CAR_NAME, LINKCOL1 + 14, CARROW2, MUNUSED, 0x00, AnimateCar, 0x05},
    {CAR_NAME, LINKCOL1 + 28, CARROW2, MUNUSED, 0x00, AnimateCar, 0x06},
    {CAR_NAME, LINKCOL1 + 42, CARROW2, MUNUSED, 0x00, AnimateCar, 0x07},
    HELP_DISPLAY (0)
    {"JWAITJOIN", SCR_MH - 128, SCR_B - 85, MUNUSED, 0x00, AnimateWaitJ, 0},
    CREDITS_DISPLAY
};                              /* end Track MultiBlit */

/* ----------------------------------------------------------------------------- */
static MULTIBLIT const CarMultiBlit[] =
{                               /* name                         x         y whtblrfz alpha        animFunc    id */
    {"BLACK2", 0, 0, MUNUSED, 0x00, AnimateCarSel, 0},
    {"PANEL02", 0, 256, MUNUSED, 0x00, nil, 0},
    {"PANEL03", 256, 256, MUNUSED, 0x00, nil, 0},
    {"SPEEDCON00", 0, 228, MUNUSED, 0x00, nil, 0x30},
    {"SPEEDCON01", 256, 228, MUNUSED, 0x00, nil, 0x30},
    {"COLORBAR01", 0, 250, MUNUSED, 0x00, AnimateBar, 0x00},
    {"COLORBAR02", 64, 250, MUNUSED, 0x00, AnimateBar, 0x01},
    {"COLORBAR03", 128, 250, MUNUSED, 0x00, AnimateBar, 0x02},
    {"COLORBAR04", 192, 250, MUNUSED, 0x00, AnimateBar, 0x03},
    {"COLORBAR05", 256, 250, MUNUSED, 0x00, AnimateBar, 0x04},
    {"COLORBAR06", 320, 250, MUNUSED, 0x00, AnimateBar, 0x05},
    {"COLORBAR07", 384, 250, MUNUSED, 0x00, AnimateBar, 0x06},
    {"COLORBAR08", 448, 250, MUNUSED, 0x00, AnimateBar, 0x07},
    {"COLORBAR09", 0, 261, MUNUSED, 0x00, AnimateBar, 0x10},
    {"COLORBAR10", 64, 261, MUNUSED, 0x00, AnimateBar, 0x11},
    {"COLORBAR11", 128, 261, MUNUSED, 0x00, AnimateBar, 0x12},
    {"COLORBAR12", 192, 261, MUNUSED, 0x00, AnimateBar, 0x13},
    {"COLORBAR13", 256, 261, MUNUSED, 0x00, AnimateBar, 0x14},
    {"COLORBAR14", 320, 261, MUNUSED, 0x00, AnimateBar, 0x15},
    {"COLORBAR15", 384, 261, MUNUSED, 0x00, AnimateBar, 0x16},
    {"COLORBAR16", 448, 261, MUNUSED, 0x00, AnimateBar, 0x17},
    {"BLACK2", 0, 0, MUNUSED, 0xFF, AnimateTrans, 2},
    {"DRONES", SCR_L + 0, SCR_T + 0, MUNUSED, 0x00, AnimateDrone, -1},
    {"FEEDREG", SCR_L + 70, SCR_T + 0, MUNUSED, 0x00, AnimateFeedback, -1},
#if KETCHUP
    {"KETCHUP", SCR_L + 140, SCR_B - 64, MUNUSED, 0x00, AnimateCatchup, -1},
#endif
    {"AUTOTRANS", SCR_MH - 142, 0, MUNUSED, 0x00, AnimateTrans, 0},
    {"DANGER", SCR_L + 255, SCR_MH - 150, MUNUSED, 0x00, AnimateDanger, 0},
    {"MANUAL", SCR_MH + 14, 0, MUNUSED, 0x00, AnimateTrans, 1},
    HELP_DISPLAY (0)
    CREDITS_DISPLAY
};                              /* end Car MultiBlit */


/* ----------------------------------------------------------------------------- *
 *  Called each time select mode is initially entered to load the appropriate
 *  models and prepare for the select process.
 * ----------------------------------------------------------------------------- */
void
InitSelect (void)
{
    extern S8 view;             /*  Current camera view     camera.c */
    F32 pos[3], npos[3], uv[3][3];
    S32 i, j;
    char str[20];

    /*  Give an audio cue when game starts. */
    SOUND (S_RUSHWHISPER);

    num_pads = 0;

    /*  Load the data from disk. */
    gTrackLocked = false;
    SetCountdownTimer( TRK_SELECT_TIME );
    gstate = TRKSEL;
    Load (3);

    InitVisuals (false);

    gPickTime = 0;
    gPadPitch = 0;
    gCameraTransition = false;
    gCarMove = CM_NOT;
    gLitPad = -1;
    gSoundOn = false;
    oldTrack = -1;
    all_tracks_locked_time = 0;
    all_cars_locked_time = 0;
    gGameInited = false;
    gMirrorMode = mirror_flag = 0;

    /*  Create the car select room. */
    gScreenObj = MBOX_NewObject (MBOX_FindObject ("CSELSCREEN"), zerovector, unituvs, -1, 0);
    gRotorObj = MBOX_NewObject (MBOX_FindObject ("CSELROTOR"), zerovector, unituvs, -1, 0);
    gDoorObj = MBOX_NewObject (MBOX_FindObject ("CSELDOOR"), zerovector, unituvs, -1, 0);
/*  gFanObj     = MBOX_NewObject( MBOX_FindObject("CSELFAN"),    zerovector, unituvs, -1, 0); */
    gArrowObj = MBOX_NewObject (MBOX_FindObject ("CSELPOSTS"), zerovector, unituvs, gRotorObj, 0);
    gLevelObj = MBOX_NewObject (MBOX_FindObject ("CSELCARLEVE"), zerovector, unituvs, gRotorObj, 0);
    gRoomObj = MBOX_NewObject (MBOX_FindObject ("CSELROOM"), zerovector, unituvs, -1, 0);
    gBackObj = MBOX_NewObject (MBOX_FindObject ("CSELSCR_BAK"), zerovector, unituvs, -1, 0);

    /*  Create the cars and place them on the pads. */
    view = 3;

    /*  Set up the camera. */
    gCurCam = gNextCam = gCamFrames = 0;
    SelectCam ();

    /*  Prepare to select. */
    SOUND (S_SELECT);

    /*  Turn off all the lamps. */
    ctl_mod_latch (~(LA_VIEW1 | LA_VIEW2 | LA_VIEW3 | LA_MUSIC | LA_LEADER));

}                               /* end InitSelect */


/* ----------------------------------------------------------------------------- *
 *  Called once at init time to initialize global variables to known states.
 * ----------------------------------------------------------------------------- */
void
InitTrkSel (void)
{
    gTrackLocked = false;
    gCarLocked = false;
    gClutchLocked = false;
    drones_flag = 1;            /*  Describe car-select settings */
    gFeedbackLevel = DEFAULT_FEEDBACK_LEVEL;
    catchup_flag = 1;
    solo_flag = 0;
    mirror_flag = 0;
}                               /* end InitTrkSel */


/* ----------------------------------------------------------------------------- */
void
ShowTrackSelect (BOOL active)
{
    /* If Track Select doesn't exist yet, create it. */
    if (!TrackBlit && active)
    {
        InitDigitList ();
        TrackBlit = NewMultiBlit (0, 0, TrackMultiBlit, NBLITS (TrackMultiBlit));
        /*  Add text messages */
        InitMsgs (3, true);
        if (language != LANG_JAPANESE)
        {
            sfontX.scale_x = sfontX.scale_y = 0.75;
            AddMsg (-256, SCR_B - 170, "WAITING FOR OTHERS", 0, AnimateWait, 0);
            AddMsg (-256, SCR_B - 137, "TO JOIN", 0, AnimateWait, 0);
        }

        if (TourneyOn ())
            AddMsg (-256, SCR_B - 203, "TOURNAMENT", 0, AnimateBlink, 0);

        sfontX.scale_x = sfontX.scale_y = 1.00;

        if (TourneyOn () && gTourney.ext_start == JOIN_QUICK)
            /* Quick */
            SetCountdownTimer (QCK_SELECT_TIME);
        else
            /* Normal */
            SetCountdownTimer (TRK_SELECT_TIME);
    }

    /*  Check if getting rid of this screen */
    if (TrackBlit && !active)
    {
        RemoveMsgs ();
        RemoveBlit (TrackBlit);
        TrackBlit = (void *) 0;
    }

}                               /* end ShowTrackSelect */


/* ----------------------------------------------------------------------------- */
void
TrackSelForce (void)
{
    static S32 last_time;
    S16 init;

    init = (IRQTIME - last_time < ONE_SEC);
    last_time = IRQTIME;

    if (!gTrackLocked)
        OverrideForce (NTRACKS, 40, 5, init, 1);
    else
        gSWforce = 0;
}                               /*  end TrackSelForce */

/* If anybody new joins in, this will reset everybody's timers */

static void
waitForNewbees (void)
{
    static U8 prev_state;
    U8 state = 0;
    S32 maxtime = 0;
    S32 slowPokes = 0;
    S32 i;

    if( gstate != TRKSEL ) return;

    for (i = 0; i < MAX_LINKS; i++)
    {
        /* Ignore deadbeats */
        if( !gn_state[i].alive || TourneyOn() != TourneyNode( i ) ) continue;

        state = ((state << 1) & 0xfe) |
            (gn_state[i].d.state == TRKSEL || gn_state[i].d.state == CARSEL);

        if( i != gThisNode &&
            gn_state[i].d.state == TRKSEL && gnGetCountdownTime( i ) > maxtime )
            maxtime = gnGetCountdownTime( i );

        /* Slowpokes are people who aren't locked yet */
        slowPokes += ( gn_state[i].d.state == TRKSEL &&
                       gLink[i].state != LOCKED );
    }

    /* Ok to drift by .1 seconds -- this should be larger than net delay */
    maxtime -= ONE_SEC/10;

    if (state != prev_state)
    {
        ResetCountdownTimer ();
        prev_state = state;
    }
    else if( maxtime > GetCountdownTime() && GetCountdownTime() > 0 )
    {
        if( slowPokes ) SetCountdownTimer( maxtime );
    }
}

/* ----------------------------------------------------------------------------- */
void
TrackSel (void)
{
    static S32 lastSend;
    static S16 link_state = LS_INIT;
    static U8 old_flag = 0;
    S16 i, wait_track, num_on;

    TrackSelForce ();
    SelectCam ();
    waitForNewbees ();

    /*  Check for initialization need. */
    if (oldTrack == -1)
    {
        gn_state[gThisNode].d.seed = prc_get_count();
        ResetGameLink ();
        gIgnoreScore = false;
        gCarLocked = false;
        gClutchLocked = false;
        autotrans = true;       /* Initialize default */
        gThisCar = -1;
        gCarFirstSel = 0;
        drones_flag = (TourneyOn () && gTourney.drones >= 0) ? gTourney.drones : 1;
        catchup_flag = (TourneyOn () && gTourney.catchup >= 0) ? gTourney.catchup : 1;
        lastSend = 0;
        gFeedbackLevel = DEFAULT_FEEDBACK_LEVEL;
        old_num_on = 0;
        tlock_sound_delay = 0;
        link_state = LS_INIT;

        SetCountdownTimer (TRK_SELECT_TIME);

        old_flag = 0;
        solo_flag = ( (levels & SW_REVERSE) != 0 );

        /*  Check current track selection */
        if (TourneyOn ())
            trackno = (gTourney.track >= 0) ? gTourney.track : gTourneyTrack;
        else
            trackno = WheelSection (NTRACKS, 1);

        oldTrack = trackno;

        /*  Tie car selection to wheel so it'll point correctly immediately. */
        gPickPad = gThisCar = RawWheelSection (NPADS, 1);
        gThisCar += gCarFirstSel;

        gInThisGame = (1 << gThisNode);
        gBeginTime = IRQTIME;

        gLink[gThisNode].flags = LINK_ACTIVE | LINK_BLINK | DEFAULT_LINK_FLAGS;
        PutEventReportPDU (Event_CanPlay, 4,
                        Report_Node, gThisNode, Report_Track, (U32) trackno,
                           Report_RaceFlags, (U32) gLink[gThisNode].flags,
                           Report_RaceFlags, (U32) TourneyOn ());

        num_pads = NPADS;
        create_cars ();

        negotiate_track = 0;
        negotiate_race = 0;
        negotiate_time = 0;
        drop_out_time = 0;
        re_negotiate = false;
        for (i = 0; i < MAX_LINKS; i++)
        {
            negotiate_tdata[i] = 0;
            negotiate_twant[i] = 0;
            negotiate_ttime[i] = 0;
            negotiate_tphase[i] = 0;
            negotiate_tchan[i] = 0xff;
        }
    }

    /*  If track is already locked, allow tap-through. */
    if (gTrackLocked)
    {
        ShowTrackSelect (true);

        if (solo_flag)
        {
            /* immediately get out of track select */
            if( !TourneyOn() ) make_node_solo( gThisNode );
            init_car_select ();
            return;
        }

        /* neg_phase: 0=not locked yet, 1=locked, 2=in negotiation, 3=holdoff */
        if (negotiate_track > 1)
        {
            track_negotiation ();
            return;
        }
        else
            negotiate_track = 1;

        /*  Turn on speedup buttons only. */
        ctl_mod_latch (~(LA_ABORT | LA_LEADER));
        BlinkUnlessPressed (LA_MUSIC, SW_RADIO);

        /*  Check if everybody possible is linked and skip countdown if so. */
        if (gAllTracksLocked && GetCountdownTime () > ONE_SEC)
            SetCountdownTimer (ONE_SEC);

        /*  Some buttons can speed the clock up. */
        if (edges & SW_RADIO)
        {
            PutEventReportPDU (Event_TrackLocked, 4,
                               Report_Node, (U32) gThisNode,
                               Report_Track, (U32) trackno,
                             Report_RaceFlags, (U32) gLink[gThisNode].flags,
                            Report_Time, GetCountdownTime () - HURRY_SPEED);
            lastSend = IRQTIME;
        }

        /*  Tie car selection to wheel so it'll point correctly immediately. */
        gPickPad = gThisCar = RawWheelSection (NPADS, 1);
        gThisCar += gCarFirstSel;

        if (!solo_flag && !TourneyOn () && (levels & SW_REVERSE) && (GasPressed (true) || (edges & SW_ABORT)))
        {
            solo_flag = true;
            goto go_solo;
        }

        /* check if link message should sound */
        link_state = do_link_sound (link_state);

        if ( GetCountdownTime() <= 0 &&
             link_state != LS_WAITING && link_state != LS_INIT )
        {
            negotiate_track = 2;

            /*  Turn off all lights. */
            ctl_mod_latch (~(LA_VIEW1 | LA_VIEW2 | LA_VIEW3 | LA_ABORT));
        }

        return;
    }

    /*  Check for lock trigger. */
    if ( (IRQTIME - gBeginTime > TRK_DELAY_TIME ) &&
         (TourneyOn () || (GasPressed (true) || (edges & SW_ABORT) ||
         (GetCountdownTime () <= (ONE_SEC/2) ))))
    {
        if (TourneyOn ())
        {
            ShowTrackSelect (true);
            trackno = (gTourney.track >= 0) ? gTourney.track : gTourneyTrack;
        }

        /*  Track is newly locked, inform the net that it's locked */
      go_solo:
        for (i = 1; i <= old_num_on; i++)
        {
            SOUND (kill_others_locked_sound[i - 1]);
            wait (10);
        }

        if (!TourneyOn ())
            tlock_sound_delay = IRQTIME;

        if (solo_flag)
        {
/* hack to allow solo fall through without everyone falling through. */
            gLink[gThisNode].flags |= LINK_SOLO | LINK_TLOCK;

            PutEventReportPDU (Event_TrackLocked, 4,
                               Report_Node, (U32) gThisNode,
                               Report_Track, (U32) trackno,
                             Report_RaceFlags, (U32) gLink[gThisNode].flags,
                               Report_Time, GetCountdownTime ());

            if (gTrackLocked == false)
                SOUND (S_TRANSELECT);

            gTrackLocked = true;
            make_node_solo (gThisNode);

            return;
        }
        else
        {
            gLink[gThisNode].flags &= ~LINK_SOLO;

            link_state = do_link_sound (link_state);
        }

        gTrackLocked = true;

      send:
        PutEventReportPDU (Event_TrackLocked, 4,
                           Report_Node, (U32) gThisNode,
                           Report_Track, (U32) trackno,
                           Report_RaceFlags, (U32) gLink[gThisNode].flags,
                           Report_Time, GetCountdownTime ());
        lastSend = IRQTIME;
        return;
    }

    /*  Didn't lock in selection yet, allow selection changes. */
    /*  Make sure display is turned on. */
    ShowTrackSelect (true);

    /*  Check current track selection */
    if (TourneyOn ())
        trackno = (gTourney.track >= 0) ? gTourney.track : gTourneyTrack;
    else
        trackno = WheelSection (NTRACKS, 1);

    /*  Tie car selection to wheel so it'll point correctly immediately. */
    gPickPad = gThisCar = RawWheelSection (NPADS, 1);
    gThisCar += gCarFirstSel;

#if MIRROR_HACK
    if (edges & SW_VIEW1)
        mirror_flag = (mirror_flag) ? 0 : 1;
#endif

    /*  Check for changing toggle items. */
    /*  REVERSE toggles Solo/Linked. */
    solo_flag = !TourneyOn () && ((levels & SW_REVERSE) != 0);

    if (trackno != oldTrack || old_flag != solo_flag)
    {
        /*  Track selection has changed, inform the net */
        if (solo_flag)
            gLink[gThisNode].flags |= LINK_SOLO;
        else
            gLink[gThisNode].flags &= ~LINK_SOLO;
        PutEventReportPDU (Event_CanJoin, 4,
                           Report_Node, (U32) gThisNode,
                           Report_Track, (U32) trackno,
                           Report_RaceFlags, (U32) gLink[gThisNode].flags,
                           Report_RaceFlags, (U32) TourneyOn ());
        oldTrack = trackno;
        old_flag = solo_flag;

        SOUND (S_TRANSELECT);
    }

    /* now make the crowd sound for locked cars on track */
    num_on = 0;
    for (i = 0; i < MAX_LINKS; i++)
    {
        if ((trackno == gLink[i].track) &&
            (i != gThisNode) &&
            (gLink[i].flags & (LINK_TLOCK | LINK_BLINK)))
            num_on++;
    }

    if (num_on != old_num_on)
    {
        if (num_on < old_num_on)
        {
            for (i = old_num_on; i > num_on; i--)
            {
                SOUND (kill_others_locked_sound[i - 1]);
                wait (10);
            }
        }
        else
        {
            for (i = old_num_on + 1; i <= num_on; i++)
            {
                SOUND (others_locked_sound[i - 1]);
                wait (10);
            }
        }
        old_num_on = num_on;
    }

    waiting_for_others (1);

}                               /* end TrackSel */

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
   *
   *       Negotiate with all stacks for who's in the link                      */

void
track_negotiation (void)
{
    S16 i, j, count, node_count[MAX_LINKS], same_count[MAX_LINKS];
    U32 whom_ive_heard_from_mask, new_mask;

    /* are we ready to send another message out? */
    if (negotiate_time == 0)
        /* yes */
    {
        whom_ive_heard_from_mask = 0;
        for (i = 0; i < MAX_LINKS; i++)
        {
            /* have we heard from this stack yet? */
            if (negotiate_ttime[i] != 0)
                whom_ive_heard_from_mask |= 1 << i;
        }

        /* tell all who we have heard from */
        PutEventReportPDU (Event_Negotiate_Track, 6,
                           Report_Node, gThisNode,
                           Report_Track, (U32) trackno,
                           Report_RaceFlags, (U32) whom_ive_heard_from_mask,
                        Report_RaceFlags, (U32) negotiate_tphase[gThisNode],
                           Report_RaceFlags, (U32) gInThisGame,
                           Report_RaceFlags, (U32) current_mcast_channel);

        negotiate_time = IRQTIME;
    }

    if (IRQTIME - negotiate_time > ONE_SEC / 5)
        negotiate_time = 0;

    /* first clear out of date messages */
#if 0
    for (i = 0; i < MAX_LINKS; i++)
    {
        if (negotiate_ttime[i] != 0 && IRQTIME - negotiate_ttime[i] > 1 * ONE_SEC)
        {
            negotiate_ttime[i] = 0;
            negotiate_tdata[i] = 0;
            negotiate_tphase[i] = 0;
            negotiate_twant[i] = 0;
            negotiate_tchan[i] = 0xff;
        }
    }
#endif

    if (re_negotiate == false)
    {
        /* now see if we all agree */
        for (i = 0; i < MAX_LINKS; i++)
        {
            /* have we heard from this stack yet? */
            if (negotiate_ttime[i] != 0)
            {
                if (negotiate_tdata[i] != gInThisGame ||
                    negotiate_twant[i] != gInThisGame ||
                    negotiate_tchan[i] == 0xff)
                    break;
            }
            else if ((1 << i) & gInThisGame)
                break;
        }

        /* do we completely agree? */
        if (i == MAX_LINKS)
        {
            /*  Everyone agrees, start the car select. */
            negotiate_track = 3;
            init_car_select ();

            return;
        }
    }

/*---------------------------------------------------------------------------*/

    if (drop_out_time == 0)
        drop_out_time = IRQTIME;

    if (IRQTIME - drop_out_time > 2 * ONE_SEC || re_negotiate)
    {
        /* now find the most common grouping with the most nodes */

        /* hold off new messages for a while */
        negotiate_track = 3;

        /* bump negotiation phase */
        negotiate_tphase[gThisNode]++;
        re_negotiate = false;

        for (i = 0; i < MAX_LINKS; i++)
        {
            /* count all members of each node's "i've heard from" data */
            count = 0;
            if (negotiate_ttime[i] != 0)
            {
                for (j = 0; j < MAX_LINKS; j++)
                {
                    if (negotiate_tdata[i] & (1 << j))
                        count++;
                }
            }

            node_count[i] = count;

            /* now count all like matches */
            count = 0;
            if (negotiate_ttime[i] != 0)
            {
                for (j = 0; j < MAX_LINKS; j++)
                {
                    if (negotiate_ttime[j] != 0 && negotiate_tdata[i] == negotiate_tdata[j])
                        count++;
                }
            }

            same_count[i] = count;
        }

        /* now find the highest matches (may be more than one group) */
        count = 0;
        for (i = 0; i < MAX_LINKS; i++)
        {
            if (same_count[i] > same_count[count])
                count = i;
        }

        /* now find the biggest group */
        j = count;
        for (i = 0; i < MAX_LINKS; i++)
        {
            if (same_count[i] == same_count[count] && node_count[i] > node_count[j])
                j = i;
        }

        /* new_mask will have all that should be in link */
        new_mask = negotiate_tdata[j] | (1 << gThisNode);

#if 1
        MSG ("ER: max_cnt:%ld   new_mask:%lx", (S32) same_count[j], (S32) new_mask);
#endif

        for (i = 0; i < MAX_LINKS; i++)
        {
            if (new_mask & (1 << i))
            {
                gLink[i].flags |= LINK_ACTIVE;
                gLink[i].track = trackno;
            }
            else if (gInThisGame & (1 << i))
                RemoveFromGame (5, i);
        }

#if 0
        new_mask = (negotiate_tdata[gThisNode] & gInThisGame) | (1 << gThisNode);
        MSG ("new_mask:%lx", (S32) new_mask);

        for (i = 0; i < MAX_LINKS; i++)
        {
            if (!(new_mask & (1 << i)) && (gInThisGame & (1 << i)))
                RemoveFromGame (5, i);
        }
#endif

        /* let's hear from everyone again */
        for (i = 0; i < MAX_LINKS; i++)
        {
            negotiate_tdata[i] = 0;
            negotiate_ttime[i] = 0;
        }

        gInThisGame = new_mask;
        drop_out_time = IRQTIME;
        negotiate_track = 2;
    }
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
   *
   *       Initialize stuff for car select                                      */

void
init_car_select (void)
{
    S16 i;

    gstate = CARSEL;
    SetCountdownTimer (CAR_SELECT_TIME);
    ShowTrackSelect (false);
    gCameraTransition = true;
    ShowCarSelect (true);

    negotiate_race = 0;
    negotiate_track = 3;
    negotiate_time = 0;
    drop_out_time = 0;
    re_negotiate = false;
    for (i = 0; i < MAX_LINKS; i++)
    {
        negotiate_tdata[i] = 0;
        negotiate_ttime[i] = 0;
        negotiate_tphase[i] = 0;
        negotiate_twant[i] = 0;
    }
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
   *
   *       Create all cars for car select                                       */

void
create_cars (void)
{
    extern Car *carlist[];
    Car *carp;
    F32 pos[3], npos[3], uv[3][3];
    S32 i, j, num_pads;
    char str[20];

    num_pads = NPADS;

    /*  Create the cars and place them on the pads. */
    gLastCar = -1;
    for (i = 0; i < 3; ++i)
        for (j = 0; j < 3; ++j)
            uv[i][j] = unituvs[i][j];
    rotAngle = -PI / (float) num_pads;
    pos[0] = 0;
    pos[1] = 0;                 /* 1.5 */
    pos[2] = 15;

    for (i = 0; i < num_pads; ++i)
    {
        CreateCar ((S16) i, false);
        SetupCar ((S16) i, (S16) i, false);
        StartCar (i);

        /*  Make the alternate cars too. */
        CreateCar ((S16) i + NPADS, false);
        SetupCar ((S16) i + NPADS, (S16) i + NPADS, false);
        StartCar (i + NPADS);
        MBOX_HideObject (game_car[i + NPADS].objnum, HIDE_EACHCHILD);

        CreateCar ((S16) i + 2 * NPADS, false);
        SetupCar ((S16) i + 2 * NPADS, (S16) i + 2 * NPADS, false);
        StartCar (i + 2 * NPADS);
        MBOX_HideObject (game_car[i + 2 * NPADS].objnum, HIDE_EACHCHILD);

        /* Place the tires in the correct positions. */
        carp = carlist[0];
        for (j = 0; j < 4; ++j)
        {
            npos[0] = carp->TIRER[j][1];
            npos[1] = -carp->TIRER[j][2] + 1.6;
            npos[2] = carp->TIRER[j][0];
            ZOID_UpdateObject (game_car[i].visuals[VIS_TIRE1 + j].objnum, npos, unituvs);
            ZOID_UpdateObject (game_car[i + NPADS].visuals[VIS_TIRE1 + j].objnum, npos, unituvs);
            ZOID_UpdateObject (game_car[i + 2 * NPADS].visuals[VIS_TIRE1 + j].objnum, npos, unituvs);
            npos[0] = npos[2] = 0;
            npos[1] = 1.5;
            ZOID_UpdateObject (game_car[i].objnum, npos, unituvs);
            ZOID_UpdateObject (game_car[i + NPADS].objnum, npos, unituvs);
            ZOID_UpdateObject (game_car[i + 2 * NPADS].objnum, npos, unituvs);
        }
        WorldVector (pos, npos, uv);
        gPadObj[i] = MBOX_NewObject (MBOX_FindObject ("CSELPAD"), npos, uv, gRotorObj, 0);
        ZOID_SetObjectParent (game_car[i].objnum, gPadObj[i], 1);
        ZOID_SetObjectParent (game_car[i + NPADS].objnum, gPadObj[i], 1);
        ZOID_SetObjectParent (game_car[i + 2 * NPADS].objnum, gPadObj[i], 1);

        if (i == num_pads - 1)
        {
            npos[1] += 3;
            gCageObj = MBOX_NewObject (MBOX_FindObject ("CSELCAGE"), npos, uv, gRotorObj, 0);
        }

        YawUV (rotAngle, uv);
    }
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
   *
   *       Waiting for others sound
   *         Upon entry: type -> 0 = check during join now (attract)
   *                             1 = check during track select (select)         */

void
waiting_for_others (S16 type)
{
    static S32 r_firetime; /* Time that last random  sound fired */
    static S32 w_firetime; /* Time that last waiting sound was fired */
    static S32 cdown;

    S32 i;
    S16 wait_track = -1;
    S32 curtime = IRQTIME;

    for (i = 0; i < MAX_LINKS; i++)
    {
        if( i == gThisNode || TourneyOn() != TourneyNode( i ) )
        {
            continue;
        }

        if( gn_state[i].d.state == TRKSEL &&
            (gn_state[i].d.flags & LINK_TLOCK ) &&
            (!(gn_state[i].d.flags & LINK_SOLO)))
        {
            if (wait_track == -1)
                wait_track = gn_state[i].d.track;
            else if (wait_track != gn_state[i].d.track)
            {
                w_firetime = 0; /* Allow immediate response later */
                return;
            }
        }
    }

    /* Last minute hack to prevent music from playing in attract mode
     * after the announcer says, "waiting for drivers on track n"
    */
    if( gWait4TrkPlaying )
    {
        if( gstate != ATTRACT )
        {
            gWait4TrkPlaying = 0;
        }
        else if( curtime - w_firetime > ONE_SEC * 3 )
        {
            SOUND( S_STOP_ALL_SNDS );
            gWait4TrkPlaying = 0;
        }
    }

    if (wait_track >= 0 && GetCountdownTime () > ONE_SEC * 3 )
    {
        /* If less than 5 seconds since last sound was fired off, skip it */
        if( curtime - w_firetime < ONE_SEC * 5 )
            return;

        if( gstate == ATTRACT ) SOUND (S_STOP_ALL_SNDS);
        SOUND (waiting_on_sound[wait_track]);
        gWait4TrkPlaying = ( gstate == ATTRACT );
        w_firetime = r_firetime = curtime;
    }
    else if( gstate != TRKSEL && GetCountdownTime () > ONE_SEC )
    {
        static int sndid = -1;
        static snds[] =
        {
            S_YELL,
            S_ITSFAT,
            S_CHIPS,
            S_YOUGOTTA,
            S_ITSINSANE,
            S_LOOKOUT,
            S_BADLICIOUS
        };

        if( curtime - r_firetime < ONE_SEC * 5 )
            return;

        /* Start with random sound, but deterministic from there */
        if( sndid == -1 )
        {
            sndid = Random( sizeof( snds ) / sizeof( snds[0] ) );
        }

        if( cdown <= 0 )
        {
            if( ++sndid > sizeof(snds)/sizeof(snds[0]) ) sndid = 0;

            SOUND ( snds[sndid] );
            r_firetime = curtime;
        }
    }
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
   *
   *       See if others are linked with you                                    */

static S16
do_link_sound (S16 link_state)
{
    S16 i;
    S16 notlocked, lockcount;

    /* count players other than yourself that have locked on your track */
    for (i = notlocked = lockcount = 0; i < MAX_LINKS; i++)
    {
        if( i == gThisNode || !gn_state[i].alive ||
            ( gn_state[i].d.state != TRKSEL &&
              gn_state[i].d.state != CARSEL ) ||
            ( gn_state[i].d.flags & LINK_SOLO ) )
        {
            continue;
        }

        if( gn_state[i].d.flags & LINK_TLOCK )
        {
            if( gn_state[i].d.track == trackno )
            {
                lockcount++;
            }
        }
        else
        {
            notlocked++;
        }
    }

    /* If someone alive on our track but not locked on, wait for them */
    if( notlocked )
    {
        if( link_state != LS_INIT )
        {
            link_state = LS_WAITING;
        }
    }
    else if (lockcount == (MAX_LINKS-1) && link_state != LS_8WAY )
    {
        SOUND (S_8WAY);
        link_state = LS_8WAY;
    }
    else if( link_state == LS_INIT || link_state == LS_WAITING )
    {
        if( lockcount )
        {
            if( link_state != LS_WAITING )
            {
                SOUND (S_LINKED);
            }
            link_state = LS_LINKED;
        }
        else
        {
            if( link_state != LS_WAITING )
            {
                SOUND( S_TRANSELECT );
            }
            link_state = LS_SOLO;
        }
    }

    return (link_state);
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
   *
   *       Wait a number of milliseconds
   *         Upon entry: millisecs -> number of milliseconds to wait            */

void
wait (S32 millisecs)
{
    S32 cur_time = IRQTIME;

    while (IRQTIME - cur_time < millisecs);
}

/* ----------------------------------------------------------------------------- *
 * Called every frame to highlight the currently selected car type.
 * ----------------------------------------------------------------------------- */
static int
AnimateCarSel (BLIT * blt)
{
    U16 i, j, objnum;
    S32 index;
    static S32 carframe, fanframe;
    static F32 rotorPos, acc, vel, v, t, d, bounce;
    static BOOL justPicked;
    char name[20];
    F32 deg, mat[3][3], rpy[3], pos[3] =
    {0, 0, 0};


    /*This is a hack to get the missing car parts to appear. */
    {
        extern S32 NumObjects;
        /*Turn all objects on. */
        for (i = 0; i < NumObjects; i++)
            MBOX_ShowObject (i, SHOW_ALL);
        /*Hide appropriate car parts. */
        for (i = 0; i < NPADS; i++)
        {
            SetupCar ((S16) i, (S16) i, false);
            SetupCar ((S16) i + NPADS, (S16) i + NPADS, false);
            SetupCar ((S16) i + (2 * NPADS), (S16) i + (2 * NPADS), false);

#if 0
            if ((!gCarLocked && (gCarFirstSel)) || (gCarLocked && (gThisCar >= NPADS)))
#endif
                for (j = 0; j < NCARS; j += NPADS)
                    if (gCarFirstSel != j)
                        MBOX_HideObject (game_car[i + j].objnum, HIDE_EACHCHILD);
        }
    }
    /*This is a hack to get the missing car parts to appear. */


    rpy[0] = rpy[2] = 0.0;
    /*  Position manually if player has locked on a car. */
    if (gPickTime)
    {
        /*  Highlight the locked car's pad. */
        if (gLitPad != gPickPad)
        {
            ZOID_SetObjectDef (gPadObj[gPickPad], MBOX_FindObject ("CSELPAD1"));
            if (gLitPad >= 0)
                ZOID_SetObjectDef (gPadObj[gLitPad], MBOX_FindObject ("CSELPAD"));

            gLitPad = gPickPad;
        }

        /*  Make sure the car is aligned with the ramp. */
        if (justPicked)
        {
            t = 1;
            justPicked = false;
        }

        if ((t -= (1 / 20.0)) < 0)
            t = 0;

        /*  Handle wraparound since pos 0.0 is equivalent to pos 1.0 */
        d = gPickPad / (num_pads * 2.0);
        if (abs (d - rotorPos) > abs (1 + d - rotorPos))
            d += 1;

        rpy[1] = -(2.0 * PI) * ((d * (1 - t) + rotorPos * t)) + ROTOR_OFFSET;
        if (t <= 0 && gSoundOn)
        {
            SOUND (S_KTURNTABLE);
            gSoundOn = false;
        }
    }

    /* Hasn't locked yet. */
    else
    {
        justPicked = true;

        /*  Highlight the currently selected car's pad. */
        if (gLitPad != gPickPad)
        {
            ZOID_SetObjectDef (gPadObj[gPickPad], MBOX_FindObject ("CSELPAD1"));
            if (gLitPad >= 0)
            {
                ZOID_SetObjectDef (gPadObj[gLitPad], MBOX_FindObject ("CSELPAD"));
                SOUND (kill_car_type_sound[gLitPad]);
            }

            if (!TourneyOn () || gTourney.car < 0)
                SOUND (car_type_sound[gPickPad]);

            gLitPad = gPickPad;
            SOUND (S_LIGHTSOUND);
        }

        /*  Move the arrow to the currently selected pad. */
        rpy[1] = -rotAngle * gPickPad + ROTOR_OFFSET - 100.0 * PI / 180.0;
        CreatePYRMatrix (mat, rpy);
        Transpose3x3 (mat);
        pos[1] = fsin (bounce += 0.6) - 1.0;
        ZOID_UpdateObject (gArrowObj, pos, mat);
        pos[1] = pos[1] / 10.0 - 7.5;   /* was 4 */
        ZOID_UpdateObject (gLevelObj, pos, mat);

        /*  Change the textures on the arrow and level indicator to match the current car. */
        sprintf (name, "GRAD%02ld", (long) gPickPad);
        MBOX_FindTexture (name, &index);
        ZOID_ChangeTexture (MBOX_FindObject ("CSELPOSTS"), 0L, -1L, index);
        if (language == LANG_JAPANESE)
            sprintf (name, "JCARLEVEL%02ld", (long) gPickPad);
        else
            sprintf (name, "CARLEVEL%02ld", (long) gPickPad);
        MBOX_FindTexture (name, &index);
        ZOID_ChangeTexture (MBOX_FindObject ("CSELCARLEVE"), 0L, -1L, index);

        /*  If the car isn't lined up, move it towards the current selection. */
        if (gLastCar == -1)
        {
            gLastCar = gPickPad;
            t = gPickPad;
            v = 0.0;
        }

        /*  Determine direction and distance yet to move. */
        if ((d = gPickPad - t) > 0)
        {
            if (d > num_pads)
                d -= num_pads * 2;
        }
        else if (d < -num_pads)
            d += num_pads * 2;

        /*  Set acceleration based on direction, velocity based on acceleration, and position
         *  based on velocity. */
        /*  Handle decceleration. */
        if (abs (v) < 0.05)
            if (abs (d) < 0.05)
            {
                if (gSoundOn)
                    SOUND (S_KTURNTABLE);
                gSoundOn = false;
            }
            else if (!gSoundOn)
            {
                SOUND (S_TURNTABLE);
                gSoundOn = true;
            }

        if (d > 0)              /* Moving CCW */
            if (d >= 0.5)       /*  Far away, max accelerate */
                acc = MAX_ACC;
            else if (v > MAX_ACC)       /*  Close in, but still moving too fast, hard slow. */
                acc = -MAX_ACC;
            else if (v > d)     /*  Real close, just accelerate slightly. */
                acc = -d / 4;
            else if (v < 0)     /*  Velocity went too far. */
                acc = d / 4;
            else                /*  We're there! */
                acc = 0;
        else if (d <= -0.5)     /*  Far away, max accelerate */
            acc = -MAX_ACC;
        else if (v < -MAX_ACC)  /*  Close in, but still moving too fast, hard slow. */
            acc = MAX_ACC;
        else if (v < d)         /*  Real close, just accelerate slightly. */
            acc = -d / 4;
        else if (v > 0)         /*  Velocity went too far. */
            acc = d / 4;
        else                    /*  We're there! */
            acc = 0;

        v += acc;
        if (v > MAX_VEL)
            v = MAX_VEL;
        else if (v < -MAX_VEL)
            v = -MAX_VEL;

        t += v;
        if (t > num_pads * 2.0)
            t -= num_pads * 2.0;
        else if (t < 0)
            t += num_pads * 2.0;

        /*  Check if we've arrived at destination. */
        for (i = 0; i < (U16) num_pads; ++i)
            if (t - MAX_VEL <= i && i <= t + MAX_VEL)
                gLastCar = i;

        rotorPos = t / (num_pads * 2.0);
        rpy[1] = -(rotorPos) * (2.0 * PI) + ROTOR_OFFSET;
    }


    MBOX_HideObject (gBackObj, HIDE_ONLY);

    CreatePYRMatrix (mat, rpy);
    Transpose3x3 (mat);
    ZOID_UpdateObject (gRotorObj, zerovector, mat);
#if 0
    /*  Handle fan rotation. */
    fanframe = 1 + (fanframe % 4);
    sprintf (name, "FAN%03ld", fanframe);
    MBOX_FindTexture (name, &index);
    ZOID_ChangeTexture (MBOX_FindObject ("CSELFAN"), 0L, -1L, (U32) index);
#endif
    /*  Handle pad. */
    if (gPickTime)
    {
        F32 pitch = -1.0 / 200.0;

        /*  Make sure the pad doesn't pitch too far. */
        gPadPitch += pitch;
        if (gPadPitch <= -0.12 && gPadPitch >= -0.12 + pitch)
            gCameraTransition = gCarMove = CM_SETUP;
        if (gPadPitch <= -0.10)
            pitch = 0;
        if (gPickPad >= 0 && gPickPad < num_pads && pitch != 0.0)
        {
            PitchUV (pitch, ObjInstanceList[gPadObj[gPickPad]].mat);
            ZOID_UpdateObjectMat (gPadObj[gPickPad]);

            if (gPickPad == NPADS - 1)
            {
                ObjInstanceList[gCageObj].mat[3][1] -= 0.20;
                ZOID_UpdateObjectMat (gCageObj);
            }
        }

        /*  Handle door. */
        pos[1] = (IRQTIME - gPickTime) / 500.0;
        if (pos[1] > 10)
            pos[1] = 10;
        ZOID_UpdateObject (gDoorObj, pos, nil);
    }
    objnum = game_car[gThisCar].objnum;
    switch (gCarMove)
    {
        case CM_NOT:
            break;

        case CM_SETUP:          /*  Setup initial move off the pad. */
            ZOID_UpdateObjectMat (objnum);
            carframe = 27;
            gCarMove = CM_OFFPAD;
            acc = 0.03;
            vel = 0.0;
            break;

        case CM_OFFPAD: /*  Move to the turn position. */
            if (--carframe > 0)
            {
                ObjInstanceList[objnum].mat[3][2] += vel;
                vel += acc;
            }
            else
            {
                gCarMove = CM_TURN;
                carframe = 20;
                acc = -0.02;
            }
            break;

        case CM_TURN:           /* Do a turn to the right. */
            if (--carframe > 0)
            {
                YawUV ((110.0 / 20.0) * PI / 180.0, ObjInstanceList[objnum].mat);
                ObjInstanceList[objnum].mat[3][2] += vel;
                vel += acc;
                ZOID_UpdateObjectMat (objnum);
            }
            else
            {
                gCarMove = CM_FISHTAIL;
                carframe = 10;
                acc = 0.1;
            }
            break;

        case CM_FISHTAIL:       /* Fishtail. */
            if (--carframe > 0)
            {
                YawUV (-(10.0 / 5.0) * PI / 180.0, ObjInstanceList[objnum].mat);
                ObjInstanceList[objnum].mat[3][0] += vel;
                vel += acc;
                ZOID_UpdateObjectMat (objnum);
            }
            else
            {
                gCarMove = CM_PEELOUT;
                carframe = 40;  /* 5 */
                acc = 0.32;     /* -0.1 */
            }
            break;

        case CM_PEELOUT:        /*  Move to the garage door. */
            if (--carframe > 0)
            {
                ObjInstanceList[objnum].mat[3][0] += vel;
                vel += acc;
            }
            else
            {
                gCarMove = CM_DONE;
                carframe = 70;
            }
            break;

        case CM_DONE:           /*  Wait at garage door. */
            break;
    }

#if 0
    if (TourneyOn () && gTourney.car >= 0 && gTourney.tranny >= 0)
    {
        blt->Width = SCR_W;
        blt->Height = SCR_H;
        UpdateBlit (blt);
        return 1;
    }
#endif

    return Hidden (blt, true);
}                               /* end AnimateCarSel */

/* ----------------------------------------------------------------------------- *
 * Called every frame to update the non-glowing panel track numbers.
 * ----------------------------------------------------------------------------- */
static int
AnimatePanel (BLIT * blt)
{
    /* Leftover from second bank of tracks - Gerrett gets to fix. */
#if 0
    S32 bltnum = (trackno > 3) ? blt->AnimID | 0x2 : blt->AnimID & 0x1;
#else
    S32 bltnum = blt->AnimID & 0x1;
#endif

    /* 0=p011-4, 1=p031-4, 2=p015-8, 3=p035-8 */
    return updateList (blt, bltnum, 4, "PANEL01", "PANEL03", "PANEL01A", "PANEL03A");

}                               /* end AnimatePanel */

/* ----------------------------------------------------------------------------- *
 * Called every frame to update the track select glowing numbers.
 * ----------------------------------------------------------------------------- */
static int
AnimateTrkNum (BLIT * blt)
{
    S32 bltnum = trackno;

    if (trackno < 4)
    {
        blt->X = 35;
        blt->Y = 4 + (3 - trackno) * 68;
        if (trackno == 3)       /* "The Rock" is a bit different */
        {
            blt->X += 6;
            blt->Y -= 1;
        }
    }
    else
    {
        blt->X = 400;
        blt->Y = 4 + (trackno % 4) * 68;
    }

    UpdateBlit (blt);
    return updateList (blt, bltnum, 8, "TRKNUMBER1", "TRKNUMBER2", "TRKNUMBER3", "TRKNUMBER4",
                    "TRKNUMBER5", "TRKNUMBER6", "TRKNUMBER7", "TRKNUMBER8");
}                               /* end AnimateTrkNum */


/* ----------------------------------------------------------------------------- *
 * Called every frame to update the track select steering wheel.
 * ----------------------------------------------------------------------------- */
static int
AnimateSWheel (BLIT * blt)
{
    char str[20];
    F32 mat[3][3], rpy[3] =
    {0, 0, 0};

    if (Hidden (blt, gTrackLocked))
        return 1;

    sprintf (str, "SWHEEL%02ld", RawWheelSection (10, 1) + 1L);
    RenameBlit (blt, str, 0);
    blt->Y = 14;
    blt->X = 275;
    blt->Height = 180;
    blt->Width = 85;
    UpdateBlit (blt);
    return 1;
}                               /* end AnimateSWheel */


/* ----------------------------------------------------------------------------- *
 * Called every frame to update the Solo/Linked play button.
 * ----------------------------------------------------------------------------- */
static int
AnimateSolo (BLIT * blt)
{
    Hidden (blt, !((gLink[gThisNode].flags & LINK_SOLO) ));
    return 1;
}                               /* end AnimateSolo */


/* ----------------------------------------------------------------------------- *
 * Called every frame to update the Drones/No Drones button.
 * ----------------------------------------------------------------------------- */
static int
AnimateDrone (BLIT * blt)
{
    if (Hidden (blt, ((gCarLocked && (gLink[gThisNode].flags & (LINK_DRONES | LINK_RACERX)) == LINK_DRONES) ||
              (!gCarLocked && !(levels & SW_RADIO) && (drones_flag == 1)))))
        return 1;
    return updateList (blt, drones_flag, 3, "NODRONES", "DRONES", "RACERX");
}                               /* end AnimateDrone */


/* ----------------------------------------------------------------------------- *
 * Called every frame to update the transmission select button.
 * ----------------------------------------------------------------------------- */
static int
AnimateTrans (BLIT * blt)
{
    static F32 vel;

    if (Hidden (blt, (!gCarLocked && blt->AnimID < 3) || (!gTrackLocked && blt->AnimID == 3)))
        return 1;

    /*  Handle the black background. */
    if (blt->AnimID == 2 || blt->AnimID == 3)
    {
        if (blt->Alpha > 0x40)
        {
            blt->Alpha -= 8;
            UpdateBlit (blt);
        }

        if (blt->AnimDTA != -5)
        {
            blt->AnimDTA = -5;
            blt->Height = SCR_H;
            blt->Width = SCR_W;
            UpdateBlit (blt);
        }
        return 1;
    }

    /*  Drop the Transmission select from the top. AnimDTA is inited to -1 by NewMultiBlit */
    if (blt->AnimDTA < 0)
    {
        blt->AnimDTA = SCR_MV + 60;
        vel = 1.0;
    }
    else if (blt->AnimDTA >= vel)
    {
        blt->AnimDTA -= vel;
        vel += 0.25;
    }

    if (blt->AnimID == 0)
        updateBoolean (blt, language == LANG_JAPANESE, "JAUTOTRANS", "AUTOTRANS");
    else if (!has_shifter)
        return Hidden (blt, true);
    else
        updateBoolean (blt, language == LANG_JAPANESE, "JMANUAL", "MANUAL");

    if ((blt->AnimID == 0 && autotrans) || (blt->AnimID == 1 && !autotrans))
    {
        blt->Alpha = 0x00;
        blt->Y = (SCR_MV + 30 - blt->AnimDTA) - 135;
    }
    else
    {
        blt->Alpha = 0x90;
        blt->Y = SCR_MV + 30 - 110 - blt->AnimDTA;
        if (gClutchLocked)
            return Hidden (blt, 1);
    }

    UpdateBlit (blt);
    return 1;
}                               /* end AnimateTrans */


/* ----------------------------------------------------------------------------- *
 * Called every frame to update the feedback strength button.
 * ----------------------------------------------------------------------------- */
static int
AnimateFeedback (BLIT * blt)
{
    if (Hidden (blt, ((gCarLocked && gFeedbackLevel == DEFAULT_FEEDBACK_LEVEL) ||
                      (!gCarLocked && !(levels & SW_RADIO) &&
                       gFeedbackLevel == DEFAULT_FEEDBACK_LEVEL))))
        return 1;
    return updateList (blt, gFeedbackLevel, 3, "FEEDREG", "FEEDMORE", "FEEDFULL");
}                               /* end AnimateFeedback */


#if KETCHUP
/* ----------------------------------------------------------------------------- *
 * Called every frame to update the Drones/No Drones button.
 * ----------------------------------------------------------------------------- */
static int
AnimateCatchup (BLIT * blt)
{
    if (Hidden (blt, ((gCarLocked && catchup_flag != 0) ||
                    (!gCarLocked && !(levels & SW_RADIO) && catchup_flag))))
        return 1;
    return updateBoolean (blt, (BOOL) (catchup_flag != 0), "KETCHUP", "NOKETCHUP");
}                               /* end AnimateCatchup */
#endif


/* ----------------------------------------------------------------------------- *
 * Called every frame to update the "Wait for others to select" blit.
 * ----------------------------------------------------------------------------- */
static int
AnimateWait (BLIT * blt)
{
    BOOL hide = (blt->AnimID == 0 && !gTrackLocked) ||
    (blt->AnimID == 1 && !gClutchLocked);

    if (Hidden (blt, hide))
        return 1;

    blt->Alpha = (GetCountdownTime() >> 2) & 0xFF;
    UpdateBlit (blt);
    return 1;
}                               /* end AnimateWait */


/* ----------------------------------------------------------------------------- *
 * Called every frame to update the "Wait for others to select" blit.
 * ----------------------------------------------------------------------------- */
static int
AnimateWaitJ (BLIT * blt)
{
    BOOL hide = (blt->AnimID == 0 && !gTrackLocked) ||
    (blt->AnimID == 1 && !gClutchLocked);

    if (Hidden (blt, hide || language != LANG_JAPANESE))
        return 1;

    blt->Alpha = (GetCountdownTime() >> 2) & 0xFF;
    UpdateBlit (blt);
    return 1;
}                               /* end AnimateWaitJ */


/* ----------------------------------------------------------------------------- *
 * Called every frame to update the "Danger" blit.
 * ----------------------------------------------------------------------------- */
static int
AnimateDanger (BLIT * blt)
{
    if (Hidden (blt, gPickPad != NPADS - 1 || gCarLocked))
        return 1;

    blt->Alpha = (GetCountdownTime() >> 2) & 0xFF;
    UpdateBlit (blt);
    return 1;
}                               /* end AnimateDanger */


/* ----------------------------------------------------------------------------- *
 * Called every frame to update the 'link' blit.
 * ----------------------------------------------------------------------------- */
static int
AnimateLink (BLIT * blt)
{
    S16 i;
    U8 track;
    LinkInfo *p;
    BOOL hid;

    /* Handle local solo play. */
    p = &gLink[gThisNode];
    hid = (p->flags & LINK_SOLO) != 0;
    track = p->track;

    /*  Not playing solo, determine check for others using this track. */
    if (!hid)
        for (p = gLink, i = 0; i < MAX_LINKS; ++i, ++p)
            if ((hid = !(p->track == track && (p->flags & LINK_ACTIVE) &&
                         !(p->flags & LINK_SOLO) && i != gThisNode)) == 0)
                break;

    Hidden (blt, hid || (GetCountdownTime() & 0x80));
    return 1;
}                               /* end AnimateLink */


/* ----------------------------------------------------------------------------- *
 * Called every frame to update instructional text.
 * ----------------------------------------------------------------------------- */
int
AnimateText (BLIT * blt)
{
    if (Hidden (blt, (gstate == TRKSEL && gTrackLocked) ||
                (gstate == CARSEL && gClutchLocked)))
        return 1;

    /*  Turn wheel text strings. */
    if (blt->AnimID == 0)
    {
        updateBoolean (blt, language == LANG_JAPANESE, "JTURNCHOOSE", "TURNCHOOSE");
        blt->Alpha = (GetCountdownTime() >> 2) & 0xFF;
        UpdateBlit (blt);
        return 1;
    }

    /*  Press Gas text strings. */
    if (blt->AnimID == 1)
    {
        updateBoolean (blt, language == LANG_JAPANESE, "JPRESSSELECT", "PRESSSELECT");
        blt->Alpha = (GetCountdownTime() >> 2) & 0xFF;
        UpdateBlit (blt);
        return 1;
    }

    return 1;
}                               /* end AnimateText */


/* ----------------------------------------------------------------------------- *
 * Called every frame to update the turning wheel blit.
 * ----------------------------------------------------------------------------- */
int
AnimateWheel (BLIT * blt)
{
    U16 timer = GetCountdownTime();
    S16 step = (timer >> 7) % 10;

    if (Hidden (blt, (gstate == TRKSEL && gTrackLocked) ||
                (gstate == CARSEL && gClutchLocked)))
        return 1;

    if (step >= 7)
        step = 0;
    else if (step >= 4)
        step = 6 - step;
    SelectBlit (blt, step, 64, 64);
    return 1;
}                               /* end AnimateWheel */


/* ----------------------------------------------------------------------------- *
 * Called every frame to update the pedal blit.
 * ----------------------------------------------------------------------------- */
int
AnimatePedal (BLIT * blt)
{
    U16 timer = GetCountdownTime();
    S16 step = (timer >> 7) % 10;

    if (Hidden (blt, (gstate == TRKSEL && gTrackLocked) ||
                (gstate == CARSEL && gClutchLocked)))
        return 1;

    if (step >= 7)
        step = 0;
    else if (step >= 4)
        step = 6 - step;
    SelectBlit (blt, step, 64, 64);
    return 1;
}                               /* end AnimatePedal */


/* ----------------------------------------------------------------------------- *
 * Called every frame to update the Track selection.
 * ----------------------------------------------------------------------------- */
static int
AnimateTrack (BLIT * blt)
{
    S32 index;
    char trkpic[16];

    if (language == LANG_JAPANESE)
        sprintf (trkpic, "JTRKPIC0%d", trackno + 1);
    else
        sprintf (trkpic, "TRKPIC0%d", trackno + 1);

    if (mirror_flag)
    {
        blt->Flip = TRUE;
        blt->X = TRACKMAP_X + 3;
    }
    else
    {
        blt->Flip = FALSE;
        blt->X = TRACKMAP_X;
    }
    UpdateBlit(blt);

    MBOX_FindTexture (trkpic, &index);
    ZOID_ChangeTexture (MBOX_FindObject ("CSELSCREEN"), 0, -1, index);

    if (language == LANG_JAPANESE)
        return updateList (blt, trackno, 8, "JTRKPIC01", "JTRKPIC02", "JTRKPIC03", "JTRKPIC04",
                        "JTRKPIC05", "JTRKPIC06", "JTRKPIC07", "JTRKPIC08");
    else
        return updateList (blt, trackno, 8, "TRKPIC01", "TRKPIC02", "TRKPIC03", "TRKPIC04",
                           "TRKPIC05", "TRKPIC06", "TRKPIC07", "TRKPIC08");
}                               /* end AnimateTrack */


/* ----------------------------------------------------------------------------- *
 * Called every frame show car icons under the track they've selected (Track Select)
 * ----------------------------------------------------------------------------- */
static int
AnimateCar (BLIT * blt)
{
    U8 slot = (blt->AnimID & 0xF);
    LinkInfo *p;

    /*  Determine if this car is visible. */
    p = &gLink[slot];
    if (Hidden (blt, !(p->flags & LINK_ACTIVE) || (solo_flag && slot != gThisNode)))
        return 1;

    /*  It's visible, where does it belong? */
    blt->X = (slot % 4) * 14;
    if (p->track < 4)
    {
        blt->X += LINKCOL1;
        blt->Y = CARROW_Y * (slot / 4) + CARTRK_Y * (3 - p->track) + CARROW1;
    }
    else
    {
        blt->X += LINKCOL2;
        blt->Y = CARROW_Y * (slot / 4) + CARTRK_Y * (p->track % 4) + CARROW1;
    }

    return updateCar (blt, slot, ((p->flags & LINK_BLINK) && (GetCountdownTime() & 0x100)) ? 1 : 0);

}                               /* end AnimateCar */


/* ----------------------------------------------------------------------------- *
 * Called every frame show Color bars for speed/control.
 * data is
 * ----------------------------------------------------------------------------- */
static int
AnimateBar (BLIT * blt)
{
    U8 bar = (blt->AnimID >> 4) & 0xF;
    U8 segment = (blt->AnimID & 0xF);
    S16 t, car;
    static const S16 carSpeed[] =
    {100, 200, 350, 500};
    static const S16 carControl[] =
    {500, 350, 200, 100};

    if (Hidden (blt, gCarLocked))
        return 1;

    if (language == LANG_JAPANESE && bar == 1 &&
        (segment == 0 || segment == 1) && !(blt->AnimID & 0x100))
    {
        char str[20];

        blt->AnimID |= 0x100;
        sprintf (str, "J%s", blt->Name);
        RenameBlit (blt, str, true);
    }

    car = (gPickPad < 0 || gPickPad > 3) ? 0 : gPickPad;
    if (bar == 0)
    {
        if (blt->AnimDTA < 0)
            blt->AnimDTA = 0;
        if (carSpeed[car] != blt->AnimDTA)
        {
            blt->AnimDTA += (carSpeed[car] - blt->AnimDTA) / 2;
            t = blt->AnimDTA - blt->X;
            if (!Hidden (blt, t <= 0))
                SelectBlit (blt, 0, ((t < 64) ? t : 64), 8);
        }
    }
    else
    {
        if (blt->AnimDTA < 0)
            blt->AnimDTA = 0;
        if (carControl[car] != blt->AnimDTA)
        {
            blt->AnimDTA += (carControl[car] - blt->AnimDTA) / 2;
            t = blt->AnimDTA - blt->X;
            if (!Hidden (blt, t <= 0))
                SelectBlit (blt, 0, ((t < 64) ? t : 64), 8);
        }
    }
    return 1;
}                               /* end AnimateBar */


/* ----------------------------------------------------------------------------- *
 * Utility routine to set a track select car blit to the correct color.
 * ----------------------------------------------------------------------------- */
int
updateCar (BLIT * blt, S8 index, S8 dark)
{
    blt->Hide = false;
    SelectBlit (blt, 1 + index + dark * 9, 12, 24);
    return 1;
}                               /* end updateCar */


/* ----------------------------------------------------------------------------- *
 * Called every frame to update the Car icon highlight blit.
 * ----------------------------------------------------------------------------- */
static int
AnimateGlow (BLIT * blt)
{
    S16 col, slot, trk;

    blt->Alpha = (gTrackLocked || (GetCountdownTime() & 0x100)) ? 0xFF : 0x00;

    slot = gThisNode;
    trk = gLink[gThisNode].track;
    blt->X = (slot % 4) * 14 - 5;
    if (trk < 4)
    {
        blt->X += LINKCOL1;
        blt->Y = CARROW_Y * (slot / 4) + CARTRK_Y * (3 - trk) + CARROW1 - 4;
    }
    else
    {
        blt->X += LINKCOL2;
        blt->Y = CARROW_Y * (slot / 4) + CARTRK_Y * (trk % 4) + CARROW1 - 4;
    }

    SelectBlit (blt, 0, 22, 32);
    UpdateBlit (blt);
    return 1;
}                               /* end AnimateGlow */


/* ----------------------------------------------------------------------------- *
 * Utility routine to update a boolean button.
 * ----------------------------------------------------------------------------- */
int
updateBoolean (BLIT * blt, BOOL val, char *t, char *f)
{
    if (val != blt->AnimDTA)
    {
        blt->AnimDTA = val;
        RenameBlit (blt, val ? t : f, 0);
    }
    return 1;
}                               /* end updateBoolean */


/* ----------------------------------------------------------------------------- */
void
ShowCarSelect (BOOL active)
{
    /* If Car Select doesn't exist yet, create it. */
    if (!CarBlit && active)
    {
        MBOX_SetWindow (60.0, 0);
        InitDigitList ();

        CarBlit = NewMultiBlit (0, 0, CarMultiBlit, NBLITS (CarMultiBlit));
        InitMsgs (3, true);
        if (solo_flag)
            AddMsg (-256, SCR_B - 120, "PREPARING CAR", 1, AnimateWait, 0);
        else
        {
            AddMsg (-256, SCR_B - 120, "WAITING FOR OTHERS", 1, AnimateWait, 0);
            AddMsg (-256, SCR_B - 87, "TO CHOOSE", 1, AnimateWait, 0);
        }
        SetCountdownTimer (CAR_SELECT_TIME);
        gBeginTime = IRQTIME;

        SOUND (S_LOWERVOL);
        SOUND (S_GARAGEAMB);
    }

    /*  Check if getting rid of this screen */
    if (CarBlit && !active)
    {
        RemoveMsgs ();
        RemoveBlit (CarBlit);
        CarBlit = (void *) 0;
        MBOX_SetWindow (sethang[0], 0);
    }

}                               /* end ShowCarSelect */


/* ----------------------------------------------------------------------------- */
void
CarSelForce (void)
{
    static S32 last_time1, last_time2;
    S16 init;

    if (!gCarLocked)
    {
        init = (IRQTIME - last_time1 < ONE_SEC);
        last_time1 = IRQTIME;

        OverrideForce (num_pads, 30, 5, init, 1);
    }
    else if (!gClutchLocked)
    {
        init = (IRQTIME - last_time2 < ONE_SEC);
        last_time2 = IRQTIME;

        OverrideForce (2, 45, 5, init, 3);
    }
    else
        gSWforce = 0;
}                               /*  end CarSelForce */


/* ----------------------------------------------------------------------------- *
 * CarSel (selects your vehicle)
 * ----------------------------------------------------------------------------- */
void
CarSel (void)
{
    S16 i, tourney_lock;

    CarSelForce ();
    SelectCam ();

    if (dedges & SW_DEBUG5)
    {
        angi = (angi + 1) % 10;
        MBOX_SetWindow (sethang[angi], 0);
    }

    /*  If car is already locked, just wait here. */
    if (gCarLocked)
    {
        ClutchSel ();
        return;
    }

    /*  Handle tourney mode overrides. */
    tourney_lock = false;
    if (TourneyOn ())
    {
        if (gTourney.drones >= 0)
            drones_flag = gTourney.drones;
        if (gTourney.catchup >= 0)
            catchup_flag = gTourney.catchup;
        if (gTourney.car >= 0)
        {
            gThisCar = gTourney.car;
            gPickPad = gThisCar % 4;

            if (IRQTIME - gBeginTime > CAR_DELAY_TIME)
                tourney_lock = true;
        }
    }

    /*  Check for lock trigger. */
    if ((IRQTIME - gBeginTime > CAR_DELAY_TIME) &&
    (GasPressed (true) || (edges & SW_ABORT) || TimeOut () || tourney_lock))
    {
        gCarLocked = true;
        gBeginTime = IRQTIME;
        gPickTime = IRQTIME;

        /*  Turn off all the lamps. */
        ctl_mod_latch (~(LA_VIEW1 | LA_VIEW2 | LA_VIEW3 | LA_ABORT | LA_MUSIC));

        SetCountdownTimer (CLUTCH_SEL_TIME + GetCountdownTime ());

        SOUND (kill_car_type_sound[gPickPad]);

        if (gPickPad == NPADS - 1)
        {
            SOUND (S_DANGEROUS);
            SOUND (S_CROWDSCARE);
        }
        else
            SOUND (S_CARSELECT);

        if (gSoundOn)
        {
            SOUND (S_KTURNTABLE);
            gSoundOn = false;
        }

        switch (drones_flag)
        {
            default:
                gLink[gThisNode].flags &= ~LINK_DRONES;
                gLink[gThisNode].flags &= ~LINK_RACERX;
                break;
            case 1:
                gLink[gThisNode].flags |= LINK_DRONES;
                gLink[gThisNode].flags &= ~LINK_RACERX;
                break;
            case 2:
                gLink[gThisNode].flags &= ~LINK_DRONES;
                gLink[gThisNode].flags |= LINK_RACERX;
                break;
        }

        if (catchup_flag)
            gLink[gThisNode].flags |= LINK_CATCHUP;
        else
            gLink[gThisNode].flags &= ~LINK_CATCHUP;

        return;
    }

    /*  Didn't lock in selection yet, allow selection changes. */
    /*  Check for changing car selection */
    if (!(TourneyOn () && gTourney.car >= 0))
    {
        gPickPad = gThisCar = WheelSection (NPADS, 1);
        if (edges & SW_VIEW1)
            if ((gCarFirstSel += NPADS) == NCARS)
                gCarFirstSel = 0;
        gThisCar += gCarFirstSel;
    }

    /* Big ol' hack. To keep cars on pads. */
    for (i = 0; i < NCARS; ++i)
    {
        F32 pos[3] =
        {0.0, 1.5, 0.0};
        ZOID_UpdateObject (game_car[i].objnum, pos, unituvs);
    }

    /*  Toggle between car sets if need be. */
    if (edges & SW_VIEW1)
    {
        S16 hidepad;

        hidepad = gCarFirstSel - NPADS;
        if (hidepad < 0)
            hidepad = NCARS - NPADS;
        for (i = 0; i < NPADS; ++i)
        {
            MBOX_HideObject (game_car[i + hidepad].objnum, HIDE_EACHCHILD);
            MBOX_ShowObject (game_car[i + gCarFirstSel].objnum, SHOW_EACHCHILD);
            /*  Don't show the hood part. */
            ShowHood (i + gCarFirstSel, false);
        }

        SOUND (S_BEEPCAR);
    }

    /*  Check for changing toggle items. */
    if ((levels & SW_RADIO) && (!TourneyOn () || (gTourney.drones < 0 || gTourney.catchup < 0)))
    {
        ctl_mod_latch (LA_MUSIC);
        if (!TourneyOn () || gTourney.drones < 0)
            BlinkUnlessPressed (LA_VIEW1, SW_VIEW1);
        BlinkUnlessPressed (LA_VIEW2, SW_VIEW2);
#if KETCHUP
        if (!TourneyOn () || gTourney.catchup < 0)
            BlinkUnlessPressed (LA_VIEW3, SW_VIEW3);
#endif
        /*  View 2 toggles drones/no drones/Racer X. */
        if ((edges & SW_VIEW2) && (!TourneyOn () || gTourney.drones < 0))
        {
            drones_flag = (drones_flag + 1) % 3;

            /*  Temporarily deactivate racer-X */
            if (drones_flag == 2)
                drones_flag = 0;
        }

        /*  View 3 changes feedback force. */
        if (edges & SW_VIEW3)
            gFeedbackLevel = (gFeedbackLevel + 1) % 3;
#if 0
        /*  View 3 changes catchup/no catchup speed. */
        if ((edges & SW_VIEW3) && (!TourneyOn () || gTourney.catchup < 0))
            catchup_flag = (catchup_flag + 1) % 2;
#endif
    }
    else
        ctl_mod_latch (~(LA_MUSIC | LA_VIEW1 | LA_VIEW2 | LA_VIEW3));


    if (tlock_sound_delay != 0 && IRQTIME - tlock_sound_delay > ONE_SEC)
    {
        SOUND (track_picked_sound[trackno]);
        tlock_sound_delay = 0;
    }

}                               /* end CarSel */


/* ----------------------------------------------------------------------------- *
 * ClutchSel (selects your transmission type)
 * ----------------------------------------------------------------------------- */
static void
ClutchSel (void)
{
    S16 i, tourney_lock;
    static U32 lockTime;

    /*  If car is already locked, just wait here. */
    if (gClutchLocked)
    {
        if (negotiate_race == 3)
        {
            if (gGameInited)
            {
                if (gstate != PREPLAY)
                {
                    gstate = PREPLAY;
                    ShowCarSelect (false);
                    ShowTransit (true, 2);
                }
            }
            else
                start_countdown ();

            return;
        }

        /* neg_race: 0=not locked yet, 1=locked, 2=in negotiation, 3=holdoff */
        if (negotiate_race > 1)
        {
            race_negotiation ();
            return;
        }

        countdown_state = 0;

        negotiate_race = 1;

        if (TimeOut () || all_cars_locked_time)
        {
            all_cars_locked_time = 0;

            if (solo_flag)
                negotiate_race = 3;
            else
                negotiate_race = 2;
        }

        return;
    }

    /*  Handle tourney mode overrides. */
    tourney_lock = false;
    if (TourneyOn () && gTourney.tranny >= 0 && (IRQTIME - gBeginTime > TRANS_DELAY_TIME))
    {
        /* HACK: Force player to select if not quick mode or car not preselected */
        if ((gTourney.ext_start == JOIN_QUICK) || (gTourney.car >= 0))
        {
            autotrans = gTourney.tranny || !has_shifter;
            SetCountdownTimer (ONE_SEC / 2);
            tourney_lock = true;
        }
    }

    /*  Check for lock trigger. */
    if ((IRQTIME - gBeginTime > TRANS_DELAY_TIME) &&
        ((GasPressed (true)) ||
         (edges & SW_ABORT) ||
         (GetCountdownTime () < ONE_SEC) ||
         (!has_shifter) ||
         (TimeOut ()) ||
         (tourney_lock)))
    {
        /*  Check for changing clutch selection */
        if (!tourney_lock)
            autotrans = !WheelSection (2, 3) || !has_shifter;

        model[gThisNode].mainin.autotrans = autotrans;
        gClutchLocked = true;
        gSWforce = 0;
        lockTime = IRQTIME;

        /*  Turn off all the lamps. */
        ctl_mod_latch (~(LA_VIEW1 | LA_VIEW2 | LA_VIEW3 | LA_ABORT | LA_MUSIC));

        SOUND (S_TRACKBROWSE);

        /*  Hit the switch to lock the car */
        PutEventReportPDU (Event_CarLocked, 6,
                           Report_Node, (U32) gThisNode,
                           Report_CarType, (U32) gThisCar,
          Report_RaceFlags, (U32) (gLink[gThisNode].flags & LINK_COM_FLAGS),
                           Report_Difficulty, (U32) difficulty_save[trackno],
                           Report_MarkerFlag, (U32) marker_flag_save,
                           Report_NumberLaps, (U32) track_len_save[trackno]);
        return;
    }

    /*  Didn't lock in selection yet, allow selection changes. */
    /*  Check for auto/manual transmission. */
    if (!gClutchLocked)
    {
        ReadGasAndBrake ();
        autotrans = !WheelSection (2, 3) || !has_shifter;
    }

}                               /* end ClutchSel */


/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
   *
   *       Negotiate with all stacks for all race conditions                    */

void
race_negotiation (void)
{
    S16 i, j, count, node_count[MAX_LINKS], same_count[MAX_LINKS];
    U32 whom_ive_heard_from_mask, negotiate_or, negotiate_and, new_mask;

    if (negotiate_time == 0)
    {
        whom_ive_heard_from_mask = 0;
        for (i = 0; i < MAX_LINKS; i++)
        {
            /* have we heard from this stack yet? */
            if (negotiate_ttime[i] != 0)
                whom_ive_heard_from_mask |= 1 << i;
        }

        /* tell all who we have heard from */
        PutEventReportPDU (Event_Negotiate_Race, 10,
                           Report_Node, gThisNode,
                           Report_Track, (U32) trackno,
                           Report_RaceFlags, (U32) whom_ive_heard_from_mask,
                        Report_RaceFlags, (U32) negotiate_tphase[gThisNode],
                           Report_RaceFlags, (U32) gInThisGame,
                           Report_CarType, (U32) gThisCar,
          Report_RaceFlags, (U32) (gLink[gThisNode].flags & LINK_COM_FLAGS),
                           Report_Difficulty, (U32) difficulty_save[trackno],
                           Report_MarkerFlag, (U32) marker_flag_save,
                           Report_NumberLaps, (U32) track_len_save[trackno]);

        negotiate_time = IRQTIME;
    }

    if (IRQTIME - negotiate_time > ONE_SEC / 5)
        negotiate_time = 0;

    /* first clear out of date messages */
#if 0
    for (i = 0; i < MAX_LINKS; i++)
    {
        if (negotiate_ttime[i] != 0 && IRQTIME - negotiate_ttime[i] > 1 * ONE_SEC)
        {
            negotiate_ttime[i] = 0;
            negotiate_tdata[i] = 0;
            negotiate_tphase[i] = 0;
            negotiate_twant[i] = 0;
        }
    }
#endif

    if (re_negotiate == false)
    {
        /* now see if we all agree */
        for (i = 0; i < MAX_LINKS; i++)
        {
            /* have we heard from this stack yet? */
            if (negotiate_ttime[i] != 0)
            {
                if (negotiate_tdata[i] != gInThisGame ||
                    negotiate_twant[i] != gInThisGame)
                    break;
            }
            else if ((1 << i) & gInThisGame)
                break;
        }

        /* do we completely agree? */
        if (i == MAX_LINKS)
        {
            /*  Everyone agrees, pick a Simulation Manager and start the countdown. */
            negotiate_race = 3;
            return;
        }
    }

/*---------------------------------------------------------------------------*/

    if (drop_out_time == 0)
        drop_out_time = IRQTIME;

    if (IRQTIME - drop_out_time > 2 * ONE_SEC || re_negotiate)
    {
        /* now find the most common grouping with the most nodes */

        /* hold off new messages for a while */
        negotiate_race = 3;

        /* bump negotiation phase */
        negotiate_tphase[gThisNode]++;
        re_negotiate = false;

        new_mask = 1 << gThisNode;
        for (i = 0; i < MAX_LINKS; i++)
        {
            if (negotiate_ttime[i] != 0 && negotiate_tdata[i] == gInThisGame)
                new_mask |= 1 << i;
        }

        MSG ("new_mask:%lx", (S32) new_mask);

        for (i = 0; i < MAX_LINKS; i++)
        {
            if (!(new_mask & (1 << i)) && (gInThisGame & (1 << i)))
                RemoveFromGame (6, i);
        }

        /* let's hear from everyone again */
        for (i = 0; i < MAX_LINKS; i++)
        {
            negotiate_tdata[i] = 0;
            negotiate_ttime[i] = 0;
        }

        gInThisGame = new_mask;
        drop_out_time = IRQTIME;
        negotiate_race = 2;
    }
}

/* ----------------------------------------------------------------------------- */
static void
ShowCountdown (BOOL active)
{
    static GrowInfo *grow;
    char str[20];

    /* If Count Select doesn't exist yet, create it. */
    if (active)
    {
        if (!grow)
        {
            grow = InitGrowMessage ("COUNTDOWN3", SCR_MH - 64 / 2, SCR_MV - 128 / 2, 64, 128, 5, 45, 2, 1);
            grow->Blit->AnimDTA = 3;
            GrowMessage ();
            SOUND (S_THREE);
        }
        else if (!grow->Blit || grow->Blit->AnimDTA != gCountDown || !GrowMessage ())
        {
            sprintf (str, "COUNTDOWN%ld", (long) gCountDown);
            grow = InitGrowMessage (str, SCR_MH - 64 / 2, SCR_MV - 128 / 2, 64, 128, 5, 45, 2, 1);
            grow->Blit->AnimDTA = gCountDown;
            GrowMessage ();
            SOUND ((gCountDown == 2) ? S_TWO : S_ONE);
        }
    }

    /*  Check if getting rid of this screen */
    else if (grow)
    {
        RemoveGrowMessage ();
        grow = nil;
    }

}                               /* end ShowCountdown */


/* ----------------------------------------------------------------------------- */
/* CountDown - counts down to start of race */
void
CountDown (void)
{
    S32 t = GetCountdownTime () / ONE_SEC;
    S32 i;

    ctl_mod_latch ((view == 1) ? LA_VIEW1 : ~LA_VIEW1);
    ctl_mod_latch ((view == 2) ? LA_VIEW2 : ~LA_VIEW2);
    ctl_mod_latch ((view == 3) ? LA_VIEW3 : ~LA_VIEW3);

    /*  If not responding after 2 seconds, don't wait any more. */
#if 1
    if (t + 4 <= gCountDown)
    {
        MSG ("Tired of waiting, giving up on SimManager %s", NodeName (gSimManager));
#if 0
        RemoveFromGame (4, gSimManager);
        SetCountdownTimer ((--gCountDown + 1) * ONE_SEC - 1);
#endif
    }
#endif

    if (gCountDown <= 0 || TimeOut ())
    {
        gCountDown = 0;
        ShowCountdown (0);

        gstate = PREPLAY2;
    }
    else
    {
        send_countdown ();
        ShowCountdown (gCountDown > 0);
    }

}                               /* end CountDown */


/* ----------------------------------------------------------------------------- */
/*  Returns the countdown timer in milliseconds. */
S32
GetCountdownTime (void)
{
    return ( (S32) play_time ) - ( IRQTIME - ( (S32) start_time ) );
}                               /* end GetCountdownTime */

S32
GetPlayTime (void)
{
    return play_time;
}

/* effectively does a SetCountdownTimer(msecs) at time t=0 from previous timer
*/
void
SetCountdownTimerAt0( S32 msecs )
{
    S32 timer = GetCountdownTime();

    SetCountdownTimer( msecs );
    start_time += timer - ONE_SEC;
}

S32
GetStartTime (void)
{
    return start_time;
}

/* ----------------------------------------------------------------------------- */
/*  Returns the elapsed time in milliseconds. */
U32
GetElapsedTime (void)
{
    return IRQTIME - start_time;
}                               /* end GetCountdownTime */

/* ----------------------------------------------------------------------------- */
/*  Reset the countdown timer the last thing passed to SetCountdownTimer() */
void
ResetCountdownTimer (void)
{
    start_time = IRQTIME;
}

/* ----------------------------------------------------------------------------- */
/*  Sets the countdown timer to the given number of milliseconds. */
void
SetCountdownTimer (long msec)
{
    play_time = msec;
    ResetCountdownTimer ();
}                               /* end SetCountdownTimer */


/* ----------------------------------------------------------------------------- */
/*  Returns true if the countdown timer has timed out. */
BOOL
TimeOut (void)
{
    return GetCountdownTime () <= 0;
}                               /*  end Timeout */


/* ----------------------------------------------------------------------------- *
 * Returns true if gas has been pressed.
 * ----------------------------------------------------------------------------- */
BOOL
GasPressed (BOOL test)
{
    BOOL ret;

    if (!test)
        return GasReleased = true;

    ReadGasAndBrake ();

    if (gGasInt < GAS_TRIGGER / 10)
        GasReleased = true;

    if (GasReleased && gGasInt > GAS_TRIGGER)
    {
        GasReleased = false;
        return true;
    }

    return false;
}                               /* end GasPressed */


/* ----------------------------------------------------------------------------- *
 * Returns true if gas has been pressed.
 * ----------------------------------------------------------------------------- */
void
SetGasReleased (BOOL released)
{
    GasReleased = released;
}                               /* end SetGasReleased */


/* ----------------------------------------------------------------------------- *
 *  Blinks the given light unless the given switch is set, in which case the
 *  light stays on.  Note: will not work for Race Leader.
 * ----------------------------------------------------------------------------- */
void
BlinkUnlessPressed (U32 light, U32 button)
{
    U32 cur = ctl_mod_latch (0);
    BOOL on = (levels & button) || (GetCountdownTime() & BLINK_TIME);

    if (on && !(cur & light))
        ctl_mod_latch (light);
    else if (!on && (cur & light))
        ctl_mod_latch (~light);

}                               /* end BlinkUnlessPressed */
