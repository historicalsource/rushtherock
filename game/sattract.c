/*
 *  attract.c -- attract mode functions
 *
 *      Copyright 1996 Time Warner Interactive.
 *  Unauthorized reproduction, adaptation, distribution, performance or
 *  display of this computer program or the associated audiovisual work
 *  is strictly prohibited.
 *
 *  ======================================================================
 *  $Author: hightower $                       $Date $
 *  $Revision: 3.146 $                     $Locker:  $
 *  ======================================================================
 *  Change Log:
 *
 *  $Log: attract.c,v $
 * Revision 3.146  1997/09/29  19:08:20  hightower
 * Polish for the countdown timers
 *
 * Revision 3.145  1997/09/26  20:28:06  hightower
 * Join Now message now counts down based on timers of people already in track select
 *
 * Revision 3.144  1997/09/26  07:01:12  hightower
 * Took out some debug stuff
 *
 * Revision 3.143  1997/09/26  04:52:47  hightower
 * Now looking at gamenet stuff to determine if Join Now should be displayed
 *
 * Revision 3.142  1997/09/26  00:12:41  hightower
 * Now using gamenet to fix ability to join within last few seconds
 *
 * Revision 3.141  1997/09/25  02:06:39  grjost
 * Fixed tourney mode for 12 tracks, 8 cars, and menus in GUTS.
 *
 * Revision 3.140  1997/09/22  22:44:59  gray
 * version 0.10 - still field test 2.
 *
 * Revision 3.139  1997/09/20  00:28:04  gray
 * version v0.09. Still field test 2.
 *
 * Revision 3.138  1997/09/19  22:06:06  grjost
 * Added hiscore for T5 & T6
 *
 * Revision 3.137  1997/09/19  02:03:21  gray
 * v0.08 field test2.
 *
 * Revision 3.136  1997/09/18  22:36:08  grjost
 * Fixed movie sounds
 *
 * Revision 3.135  1997/09/18  05:22:37  gray
 * v0.07 field test2. rename movie 5. movie 5 sound effects.
 *
 * Revision 3.134  1997/09/18  01:15:27  grjost
 * Fixed for new movie5
 *
 * Revision 3.133  1997/09/17  23:42:39  gray
 * the rock movie replace rush movie.
 *
 * Revision 3.132  1997/09/17  00:58:15  gray
 * version 0.06.
 *
 * Revision 3.131  1997/09/16  00:40:02  gray
 * Reenabled secret stat mode.
 *
 * Revision 3.130  1997/09/09  22:29:44  grjost
 * Bumped version up to 0.05 for GUTS modem fix.
 *
 * Revision 3.129  1997/09/05  23:18:14  gray
 * version 0.04 - new PIC chip plus sound fixes.
 *
 * Revision 3.128  1997/09/05  01:29:11  gray
 * Rock v0.03.
 *
 * Revision 3.127  1997/09/03  02:55:06  gray
 * version 0.02. The Rock Field Test.
 *
 * Revision 3.126  1997/09/01  23:56:28  gray
 * The Rock version 0.01.
 *
 * Revision 3.125  1997/07/02  18:47:35  gray
 * Version 1.38 - Eight tracks.
 *
 * Revision 3.124  1997/06/28  01:40:05  gray
 * Extended to 4 tracks.
 *
 * Revision 3.123  1997/06/04  02:15:46  gray
 * Restructuring controls (gas,brake,clutch,etc) handling.
 *
 * Revision 3.122  1997/05/30  19:02:58  gray
 * Got rid of FLAGSTAFF conditionals.
 *
 * Revision 3.121  1997/03/25  23:20:04  gray
 * Version 1.36.
 *
 * Revision 3.120  1997/03/21  04:41:55  gray
 * Version 1.34, fix rolling rush triangle, fix join now exit.
 *
 * Revision 3.119  1997/03/21  01:44:43  geraci
 * version 1.32, fixed some linking attract states
 *
 * Revision 3.118  1997/03/18  20:11:47  geraci
 * version 1.29
 *
 * Revision 3.117  1997/03/18  19:42:01  geraci
 * added two new blits for more than ten coins, fixed cherry light misfire
 *
 * Revision 3.116  1997/03/13  04:04:11  geraci
 * waits for multi-cast channel to be assigned before starting attract
 *
 * Revision 3.115  1997/03/04  00:44:56  gray
 * version 1.19
 *
 * Revision 3.114  1997/02/28  02:55:08  geraci
 * version 1.18
 *
 * Revision 3.113  1997/02/27  00:16:03  geraci
 * version 1.16 for test
 *
 * Revision 3.112  1997/02/26  03:30:06  geraci
 * sound queue during join now
 *
 * Revision 3.111  1997/02/25  23:54:54  gray
 * Fix zippy attract bug.
 *
 * Revision 3.110  1997/02/25  19:28:24  gray
 * Get Team credits by holding music button.
 *
 * Revision 3.109  1997/02/22  03:06:19  geraci
 * starts with girl but ensures that credits will show on first pass
 *
 * Revision 3.108  1997/02/20  23:53:33  geraci
 * version 1.15, credits screen shows first, no free game advert in tourney
 *
 * Revision 3.107  1997/02/15  01:51:46  geraci
 * fixed whats your name music bug, join now syncs better
 *
 * Revision 3.106  1997/02/05  02:33:15  geraci
 * version 1.12
 *
 * Revision 3.105  1997/02/04  02:26:13  geraci
 * version 1.11
 *
 * Revision 3.104  1997/01/10  03:40:18  geraci
 * version 1.10
 *
 * Revision 3.103  1996/12/17  04:02:29  geraci
 * version 1.09
 *
 * Revision 3.102  1996/12/17  02:41:57  geraci
 * version 1.08
 *
 * Revision 3.101  1996/12/17  01:24:11  geraci
 * version 1.07
 *
 * Revision 3.100  1996/12/14  22:58:37  gray
 * Bump version to 1.06 for 11th hour shortcut fix.
 *
 * Revision 3.99  1996/12/14  10:48:05  gray
 * version 1.05.
 *
 * Revision 3.98  1996/12/13  14:26:11  geraci
 * version 1.04
 *
 * Revision 3.97  1996/12/13  13:27:58  gray
 * Get transition screen working without texture corruption.
 *
 * Revision 3.96  1996/12/08  05:53:55  geraci
 * version 1.03
 *
 * Revision 3.95  1996/12/06  21:11:47  geraci
 * bumped version
 *
 * Revision 3.94  1996/12/05  12:20:17  geraci
 * new version number
 *
 * Revision 3.93  1996/12/02  04:46:45  geraci
 * final time can be up to 99 minutes
 *
 * Revision 3.92  1996/11/27  12:56:10  gray
 * Changed NetTime from F32 to 10microsec S32.
 *
 * Revision 3.91  1996/11/27  08:15:37  geraci
 * girl is faster
 *
 * Revision 3.90  1996/11/27  01:59:06  petrick
 * Syntax fix.
 *
 * Revision 3.89  1996/11/27  01:46:38  petrick
 * Fixed possible rank display bug.
 *
 * Revision 3.88  1996/11/25  16:02:43  geraci
 * girl comes up less frequently over time, date stamp is on sw 11
 *
 * Revision 3.87  1996/11/25  06:33:35  petrick
 * Put link time on seperate line.
 *
 * Revision 3.86  1996/11/25  04:15:30  petrick
 * Made continue part of high score screens.
 *
 * Revision 3.85  1996/11/24  10:58:02  petrick
 * Added advert screen and second demo.
 *
 * Revision 3.84  1996/11/24  05:51:14  petrick
 * Merged e-mail address with advisory graphic.
 *
 * Revision 3.83  1996/11/23  21:57:46  petrick
 * Free game had been deactivated if playing solo.
 *
 * Revision 3.82  1996/11/23  20:31:27  petrick
 * Added continued game flag.
 *
 * Revision 3.81  1996/11/23  07:05:52  petrick
 * Slowed frame rate on girl movie to 15 fps.
 *
 * Revision 3.80  1996/11/23  01:12:06  gray
 * Removed EER_DRAWS increment - gone from GUTS.
 *
 * Revision 3.79  1996/11/22  21:46:38  petrick
 * Turns off winner light at end of game and doesn't allow high score entry mode
 * unless actually entering high score.
 *
 * Revision 3.78  1996/11/22  20:12:50  petrick
 * Made end rank changeable until locked down.
 *
 * Revision 3.77  1996/11/22  15:50:54  petrick
 * Made sure tourney join flag cleared when game starts.
 *
 * Revision 3.76  1996/11/22  10:12:49  petrick
 * Changed free game awarding and changed the way tourney mode flag works.
 *
 * Revision 3.75  1996/11/21  13:44:07  petrick
 * Made Game over bigger, added model tracing.
 *
 * Revision 3.74  1996/11/20  23:58:19  petrick
 * Added more time to demo game modes.
 *
 * Revision 3.73  1996/11/20  08:33:08  petrick
 * Fixed bug when coming in < 5th place.
 *
 * Revision 3.72  1996/11/19  21:45:37  geraci
 * added rush sound to logo screen
 *
 * Revision 3.71  1996/11/19  17:48:50  petrick
 * Dynamic texture loading, centralized loading, return of Japanese.
 *
 * Revision 3.70  1996/11/19  13:38:57  petrick
 * Dynamic texture loading.
 *
 * Revision 3.69  1996/11/18  10:37:32  petrick
 * Fixed crash of join-in while on last frame of movie.
 *
 * Revision 3.68  1996/11/17  15:54:38  petrick
 * Bumped version number.
 *
 * Revision 3.67  1996/11/17  14:25:51  petrick
 * Changes for tourney mode and to make sure graphics are loaded before used.
 *
 * Revision 3.66  1996/11/16  13:02:46  geraci
 * fixed weird music playing during attract (due to explosion)
 *
 * Revision 3.65  1996/11/16  09:48:51  petrick
 * Start of adding black background under join now.
 *
 * Revision 3.64  1996/11/15  23:25:38  petrick
 * Made Rush logo movie 1 long movie.
 *
 * Revision 3.63  1996/11/15  19:29:58  petrick
 * Added e-mail address, sound to movie5.
 *
 * Revision 3.62  1996/11/15  16:18:03  petrick
 * Tourney mode changes.
 *
 * Revision 3.61  1996/11/14  12:35:50  geraci
 * don't have to do stop all sounds more than once now
 *
 * Revision 3.60  1996/11/13  15:13:03  geraci
 * 20 seconds initial time for hiscore screen
 *
 * Revision 3.59  1996/11/13  11:56:41  petrick
 * Made sure all loads can abort OK, turn off winner light, fixed movie endings.
 *
 * Revision 3.58  1996/11/11  22:54:00  petrick
 * Fixed Flagstaff spelling error.
 *
 * Revision 3.57  1996/11/11  18:55:43  petrick
 * Got rid of 'ready' girl.
 *
 * Revision 3.56  1996/11/11  13:45:42  petrick
 * Stats mode change.
 *
 * Revision 3.55  1996/11/11  13:21:47  geraci
 * cleaned unused demo game state
 *
 * Revision 3.54  1996/11/11  13:06:47  geraci
 * external switch box support
 *
 * Revision 3.53  1996/11/11  10:49:03  petrick
 * External tourney mode starting.
 *
 * Revision 3.52  1996/11/10  14:07:24  petrick
 * Slowed down movies to 30 fps.
 *
 * Revision 3.51  1996/11/10  11:42:09  petrick
 * Made rolling rush letters start farther away.
 *
 * Revision 3.50  1996/11/09  13:07:53  petrick
 * Buncha new attract screens.
 *
 * Revision 3.49  1996/11/06  03:10:43  petrick
 * Looping movies and team pics.
 *
 * Revision 3.48  1996/11/05  12:27:22  petrick
 * Code for handling join-in correctly.
 *
 * Revision 3.47  1996/11/05  07:14:02  petrick
 * Credits now display 0/? if 0 credits.
 *
 * Revision 3.46  1996/11/02  09:55:50  petrick
 * Added 2 car movies.
 *
 * Revision 3.45  1996/11/01  12:25:23  geraci
 * drone and human types are now defines
 *
 * Revision 3.44  1996/10/31  09:34:45  petrick
 * Moved tourney banner to bottom of screen for game over screen.
 *
 * Revision 3.43  1996/10/31  08:22:53  petrick
 * Turned off who won display if solo game.
 *
 * Revision 3.42  1996/10/31  08:21:17  petrick
 * Got rid of more white text at game over.
 *
 * Revision 3.41  1996/10/31  04:47:47  petrick
 * Tourney mode changes.
 *
 * Revision 3.40  1996/10/30  04:42:20  petrick
 * Made credit display work with discount-to-continue.
 *
 * Revision 3.39  1996/10/29  08:33:42  petrick
 * Tiny bit more Game over stuff.
 *
 * Revision 3.38  1996/10/29  08:24:12  petrick
 * Free game and continue stuff.
 *
 * Revision 3.37  1996/10/25  09:59:05  geraci
 * fixed what your name bug
 *
 * Revision 3.36  1996/10/25  06:48:42  petrick
 * Got rid of shared high score tables, added tourney mode visual reminder.
 *
 * Revision 3.35  1996/10/24  09:43:08  petrick
 * Change way calculating who won.
 *
 * Revision 3.34  1996/10/24  08:38:22  geraci
 * coin sounds now work during attract, what's your name has an end tag
 * music playing when game is entered after demo game is fixed
 *
 * Revision 3.33  1996/10/23  11:18:09  petrick
 * Tourney mode changes.
 *
 * Revision 3.32  1996/10/23  03:33:26  petrick
 * Made hi score screens show the track they belong to.
 *
 * Revision 3.31  1996/10/22  08:51:12  geraci
 * what's your name and play it again play at game volume
 *
 * Revision 3.30  1996/10/18  11:19:04  petrick
 * Added Advisory screen.
 *
 * Revision 3.29  1996/10/17  08:24:27  petrick
 * Made attract mode a typedef so gdb would know the state names.
 *
 * Revision 3.28  1996/10/16  09:43:10  petrick
 * Moved tourney stuff to it's own file, hid rank if playing solo.
 *
 * Revision 3.27  1996/10/12  11:21:10  petrick
 * Added timer to Join Now.
 *
 * Revision 3.26  1996/10/11  10:21:53  petrick
 * Changes for Tourney mode.
 *
 * Revision 3.25  1996/10/10  08:30:26  petrick
 * Tourney mode additions.
 *
 * Revision 3.24  1996/10/10  05:26:11  petrick
 * Changes to give immediate feedback during disk loads.
 *
 * Revision 3.23  1996/10/09  05:46:47  petrick
 * Atari Logo movie installed.
 *
 * Revision 3.22  1996/10/08  05:20:58  gray
 * Flagstaff port.
 *
 * Revision 3.21  1996/10/07  16:12:27  geraci
 * moved some game options to globals
 *
 * Revision 3.20  1996/10/07  02:23:29  petrick
 * Made join-in Japanese bigger.
 *
 * Revision 3.19  1996/10/05  09:57:44  petrick
 * Preparations for Movies.
 *
 * Revision 3.18  1996/10/05  09:07:32  geraci
 * lower steering wheel recentering force during attract
 *
 * Revision 3.17  1996/10/05  05:38:51  petrick
 * Atari Logo static screen.
 *
 * Revision 3.16  1996/10/04  10:40:40  petrick
 * Added Join Now func.
 *
 * Revision 3.15  1996/10/04  07:30:23  geraci
 * music during attract bugs fixed
 *
 * Revision 3.14  1996/10/03  07:45:37  petrick
 * Fixed end of game freezeup bug.
 *
 * Revision 3.13  1996/10/01  08:42:00  petrick
 * Cleanup.
 *
 * Revision 3.12  1996/09/28  02:51:07  petrick
 * New high score entry screen.
 *
 * Revision 3.11  1996/09/27  10:37:19  petrick
 * Minor adjustment to Random animate.
 *
 * Revision 3.10  1996/09/27  10:29:25  petrick
 * Unexploding high scores.
 *
 * Revision 3.9  1996/09/27  02:15:20  petrick
 * Improved High Score screen, static model.
 *
 * Revision 3.8  1996/09/26  10:00:28  petrick
 * New High score screens, made entire attract more modular.
 *
 * Revision 3.7  1996/09/25  02:11:23  petrick
 * Got rid of include file.
 *
 * Revision 3.6  1996/09/24  01:09:57  petrick
 * Adjusted screen positions of some blits that were off by 1.
 *
 * Revision 3.5  1996/09/23  21:10:00  petrick
 * Added 3dfx logo.
 *
 * Revision 3.4  1996/09/16  02:15:02  geraci
 * stopped all sounds always after a demo game
 *
 * Revision 3.3  1996/09/14  11:21:39  geraci
 * demo game is now done in attract state
 *
 * Revision 3.2  1996/09/13  22:58:35  geraci
 * play it again tune killed if new game is started
 *
 * Revision 3.1  1996/09/09  02:24:52  gray
 * JAMMA version
 *
 *  ======================================================================
 */

#include "attract.h"
#include "carsnd.h"
#include "checkpoint.h"
#include "controls.h"
#include "debug.h"
#include "dis.h"
#include "game.h"
#include "globals.h"
#include "hud.h"
#include "mdrive.h"
#include "os_proto.h"
#include "sounds.h"
#include "stats.h"
#include "gamenet.h"
#include <options.h>
#include <eer_defs.h>
#include <wms_proto.h>

#include "Pro/fswheel.pro"
#include "Pro/dnl.pro"
#include "Pro/money.pro"
#include "Pro/moobs.pro"
#include "Pro/select.pro"
#include "Pro/sounds.pro"
#include "Pro/timint.pro"
#include "Pro/tourney.pro"


/*===============================  DEFINES  =================================*/

#define LOGO_LETTERS    0 /*  1 == Show moving 3D letters in logo screen.   */
#define TEST_HIGH_SCORE 0 /*  1 == enter a high score name in attract mode. */
#define NEW_SCORE_ENTRY 0 /*  1 == New high score entry method.             */

#define MAX_MSG_CHARS   256
#define MOVIESIZE       (256*256*2)


/*==============================  PROTOTYPES  ===============================*/

static BlitFunc AnimateRank;
static BlitFunc AnimateOver;
static BlitFunc AnimateLogo;
static BlitFunc AnimateJoin;
static BlitFunc AnimateGOScore;
static BlitFunc AnimateWin;
static BlitFunc AnimateStart;
static BlitFunc AnimateAdvert;

/*static void   ShowLogo(BOOL show); */
static void ShowMovie (BOOL show, S32 num);
static void ShowTeam (BOOL show);
static void ShowJoin (BOOL show);
static void ShowCredits (BOOL show);
static void Show3dfxLogo (BOOL show);
static void ShowAdvisory (BOOL show);
static void ShowAdvert (BOOL show);
static void AttractMovie (S32 num, BOOL init, BOOL loop);

extern U32 ctl_mod_latch (int x);
extern void CleanupAfterGame (void);


/*===============================  EXTERNS  =================================*/

extern U16 credits;             /* total whole credits          */
extern U8 numer, denom;         /* fractional credits.         */
extern S32 playitagain_time, whats_your_name_time;
extern S8 demo_station;
extern const S16 max_radio;
extern BOOL gEnteringName;      /*  True if entering Name in High Score */
extern BLIT *ScoreBlit, *EntryBlit;
extern SFONTINFO sfontX;
extern BOOL gClutchLocked;
extern Tourney gTourney;
extern S16 mBlitIndex;
extern BOOL continue_flag;
extern const char date[];
extern BOOL join_during_load;
extern U32 current_mcast_channel;


/*===============================  GLOBALS  =================================*/

const F32 gVersion = 0.11;

AttractMode attractFunc;

U8   gStartDuringLoad;
BOOL gFreeGame;                 /*  Set if player has won a free game. */
BOOL gPlayingFree;              /*  Set if player is playing a free game. */
BOOL gContinuedGame;            /*  Set if player is playing a continued game. */
BOOL explosion_state;
BOOL gTourneyJoin;
S32  explosion_time;
U8   skip;

static BLIT *OverBlit;
static BLIT *LogoBlit;
static BLIT *CreditsBlit;
static BLIT *Logo3Blit;
static BLIT *JoinInBlit;
static BLIT *JoinBlit;
static BLIT *AdvisoryBlit;
static BLIT *ContinueBlit;
static BLIT *MovieBlit;
static BLIT *TeamBlit;
static BLIT *TransitBlit;
static BLIT *AdvertBlit;
static S16   gLastFunc;
static BOOL  gDemoInited;
static S16   gAttractTimes[NUM_ATTRACTS];
static S32   gLogoObj[4];
static U32   radio_time; /* How many msec music button has been held */

static MULTIBLIT const JoinInMultiBlit[] =
{   /* name     x          y whtblrfz alpha  animFunc          id */
    {"START",   0, 0,                      MUNUSED, 0x00, AnimateStart, 0},
    {"JOINNOW", SCR_MH - 128, SCR_MV - 40, MUNUSED, 0x00, AnimateJoin, 0},
    {"DIGIT6",  218, 242,                  MUNUSED, 0x00, AnimateTimer, 0x1060},
    {"DIGIT6",  258, 242,                  MUNUSED, 0x00, AnimateTimer, 0x1061},
    CREDITS_DISPLAY
};

static MULTIBLIT const JoinMultiBlit[] =
{   /* name    x  y whtblrfz alpha animFunc id */
    {"BLACK2", 0, 0, MUNUSED, 0x00, nil, 1},
};

static MULTIBLIT const MovieMultiBlit[] =
{   /* name    x  y whtblrfz alpha animFunc id */
    {"BLACK2", 0, 0, MUNUSED, 0x00, nil, 1},
};

static MULTIBLIT const LogoMultiBlit[] =
{   /* name     x    y    whtblrfz alpha animFunc id */
    {"TITLE00", 0,   0,   MUNUSED, 0x00, AnimateLogo, 0},
    {"TITLE01", 256, 0,   MUNUSED, 0x00, AnimateLogo, 1},
    {"TITLE02", 0,   256, MUNUSED, 0x00, AnimateLogo, 2},
    {"TITLE03", 256, 256, MUNUSED, 0x00, AnimateLogo, 3},
};

static MULTIBLIT const TransitMultiBlit[] =
{   /* name    x  y whtblrfz alpha animFunc id */
    {"BLACK2", 0, 0, MUNUSED, 0x00, AnimateTransit, 0},
};

static MULTIBLIT const TeamMultiBlit[] =
{   /* name      x    y    whtblrfz alpha animFunc id */
    {"TEAMPIC0", 0,   0,   MUNUSED, 0x00, nil, 0},
    {"TEAMPIC1", 256, 0,   MUNUSED, 0x00, nil, 1},
    {"TEAMPIC2", 0,   256, MUNUSED, 0x00, nil, 2},
    {"TEAMPIC3", 256, 256, MUNUSED, 0x00, nil, 3},
};

static MULTIBLIT const Logo3MultiBlit[] =
{   /* name  x  y whtblrfz alpha animFunc id */
    {"3DFX", 0, 0, MUNUSED, 0x00, nil, 1},
};

static MULTIBLIT const AdvisoryMultiBlit[] =
{   /* name      x  y                        whtblrfz alpha animFunc id */
    {"ADVISORY", SCR_MH - 128, SCR_MV - 175, MUNUSED, 0x00, nil, 1},
    {"LEGAL0",   0,            SCR_B - 205,  MUNUSED, 0x00, nil, 1},
    {"LEGAL1",   256,          SCR_B - 205,  MUNUSED, 0x00, nil, 1},
};

static MULTIBLIT const AdvertMultiBlit[] =
{   /* name     x             y            whtblrfz alpha animFunc       id */
    {"WINRACE", SCR_MH - 128, SCR_T + 0,   MUNUSED, 0x00, AnimateAdvert, 0},
    {"TOURNEY", SCR_MH - 128, SCR_MV - 40, MUNUSED, 0x00, AnimateAdvert, 1},
};

static MULTIBLIT const CreditsMultiBlit[] =
{   /* name    x    y    whtblrfz alpha animFunc id */
    {"CRED00", 0,   0,   MUNUSED, 0x00, nil, 1},
    {"CRED01", 256, 0,   MUNUSED, 0x00, nil, 1},
    {"CRED02", 0,   256, MUNUSED, 0x00, nil, 1},
    {"CRED03", 256, 256, MUNUSED, 0x00, nil, 1},
};

static MULTIBLIT const OverMultiBlit[] =
{   /* name      x            y         whtblrfz alpha animFunc     id */
    {"GAMEOVER", SCR_MH-128, SCR_MV-52, MUNUSED, 0x00, AnimateOver, 0 },
    {"M8F05",    0, 227,                MUNUSED, 0x00, AnimateRank, 0 },
    {"GAMEOVER", 0, SCR_T + 50,         MUNUSED, 0x00, AnimateWin,  0xFF },
#if WIN_BLITS
    {"WIN_CAR",  SCR_MH-128, SCR_T+100, MUNUSED, 0x00, AnimateWin,  0xFE },
#endif
    {"DIGIT6",   218, 282,              MUNUSED, 0x00, AnimateTimer, 0x4060 },
    {"DIGIT6",   258, 282,              MUNUSED, 0x00, AnimateTimer, 0x4061 },
};


/* ----------------------------------------------------------------------------- *
 * Inits attract mode globals.
 * ----------------------------------------------------------------------------- */
void
InitAttract (void)
{
    S16 i;

    LogoBlit = OverBlit = ScoreBlit = EntryBlit = ContinueBlit = nil;
    CreditsBlit = AdvisoryBlit = Logo3Blit = TeamBlit = AdvertBlit = nil;
    MovieBlit = nil;
    attractFunc = ATR_MOVIE4;
    SetCountdownTimer (8 * ONE_SEC);
    gLastFunc = -1;
    gEnteringName = false;
    SetGasReleased (true);
    demo_game = 0;
    gDemoInited = false;
    gStartDuringLoad = 0;
    gFreeGame = false;
    skip = 4;

    gAttractTimes[ATR_HSENTRY] = 20;
    gAttractTimes[ATR_HISCORE1] = 20;
    gAttractTimes[ATR_HISCORE2] = 20;
    gAttractTimes[ATR_HISCORE3] = 20;
    gAttractTimes[ATR_HISCORE4] = 20;
    gAttractTimes[ATR_HISCORE5] = 20;
    gAttractTimes[ATR_HISCORE6] = 20;
    gAttractTimes[ATR_ATARILOGO] = 10;
    gAttractTimes[ATR_MOVIE2] = 6;      /*  Car 1 rotating */
    gAttractTimes[ATR_MOVIE3] = 6;      /*  Car 2 rotating */
    gAttractTimes[ATR_MOVIE4] = 10;     /*  Bimbo */
    gAttractTimes[ATR_MOVIE5] = 20;     /*  Rush logo A */
    gAttractTimes[ATR_CREDITS] = 8;
    gAttractTimes[ATR_ADVISORY] = 8;
    gAttractTimes[ATR_ADVERT] = 8;
    gAttractTimes[ATR_3DFXLOGO] = 3;
    gAttractTimes[ATR_DEMO] = 35;
    gAttractTimes[ATR_DEMO2] = 35;
    gAttractTimes[ATR_TEAM] = 5;
    gAttractTimes[ATR_JOIN] = 15;
    gAttractTimes[ATR_TOURNEY] = 999;
    gAttractTimes[ATR_TRANSIT] = 999;
    gAttractTimes[ATR_GAMESTAT] = 999;

#if 0
    InitTourney ();
#endif

}                               /* end InitAttract */


/* ----------------------------------------------------------------------------- *
 * Selects various attract modes based on func.
 * ----------------------------------------------------------------------------- */
static void
ShowAttract (S16 func, BOOL show)
{
    if (show)
        InitDigitList ();

    gDemoInited = false;

    ctl_mod_latch (~(LA_VIEW1 | LA_VIEW2 | LA_VIEW3 | LA_MUSIC | LA_ABORT | LA_LEADER | LA_WINNER));

    switch (func)
    {
        case ATR_HSENTRY: ShowScoreEntry (show); break;
        case ATR_HISCORE1: ShowHiScore (show, 0); break;
        case ATR_HISCORE2: ShowHiScore (show, 1); break;
        case ATR_HISCORE3: ShowHiScore (show, 2); break;
        case ATR_HISCORE4: ShowHiScore (show, 3); break;
        case ATR_HISCORE5: ShowHiScore (show, 4); break;
        case ATR_HISCORE6: ShowHiScore (show, 5); break;
        case ATR_CREDITS: ShowCredits (show); break;
        case ATR_3DFXLOGO: Show3dfxLogo (show); break;
        case ATR_TEAM: ShowTeam (show); break;
        case ATR_ATARILOGO: ShowMovie (show, 1); break;
        case ATR_MOVIE2: ShowMovie (show, 2); break;
        case ATR_MOVIE3: ShowMovie (show, 3); break;
        case ATR_MOVIE4: ShowMovie (show, 4); break;
        case ATR_MOVIE5: ShowMovie (show, 5); break;
        case ATR_ADVISORY: ShowAdvisory (show); break;
        case ATR_ADVERT: ShowAdvert (show); break;
        case ATR_DEMO:
        case ATR_DEMO2: play_demogame (show, true, -1, true); break;
        case ATR_TRANSIT: ShowTransit (show, 1); break;
        case ATR_TOURNEY: ShowTourneySetup (show); break;
        case ATR_JOIN: ShowJoin (show); break;

        /*  Keep this the highest numbered. */
        case ATR_GAMESTAT: ShowGameStats (show); break;
        default:
            break;
    }
    if (show)
        gLastFunc = func;
    else
        gLastFunc = -1;
}                               /* end ShowAttract */


/* ----------------------------------------------------------------------------- */
void
AttractForce (void)
{
    if (gEnteringName)
        HiScoreForce ();
    else
        force_wheel_to_center (0, 5, 11, 20);

}                               /* end AttractForce */

/* This function figures out if there is a race available for us to join
*/
int
raceAvailable( void )
{
    int i;

    if( gstate != ATTRACT ) return 0;

    for( i=0; i<MAX_LINKS; i++ )
    {
        if( i != gThisNode && gn_state[i].d.state == TRKSEL ) return 1;
    }

    return 0;
}

/* ----------------------------------------------------------------------------- *
 * Handles attract mode.
 * ----------------------------------------------------------------------------- */
void
attract (void)
{
    S16 i;
    BOOL tourney_start;
    BOOL start_game = false;
    static U8 lastTrack;

    /*  Handle steering wheel forces. */
    AttractForce ();

    /* wait until 'join' message is sent before we continue with attract */
    if (join_during_load)
        return;

    if (current_mcast_channel == 0xff)
        return;

/*---------------------------------------------------------------------------*/

    if (whats_your_name_time && (IRQTIME - whats_your_name_time > 3 * ONE_SEC))
    {
        SOUND (S_KWHATSYERNAME);
        SOUND (S_PLAYITAGAIN);
        whats_your_name_time = 0;
        playitagain_time = IRQTIME;
    }

    if (playitagain_time && (IRQTIME - playitagain_time > 23 * ONE_SEC) && attract_music)
    {
        if (demo_game)
            SOUND (S_KPLAYITAGAIN);
        else
            SOUND (S_STOP_ALL_SNDS);

        playitagain_time = 0;

        /* set to attract volume */
        SOUND (S_ATTRACT_MODE);

        StartRadio (demo_station + max_radio + 1);
    }

    if (explosion_state && abs (IRQTIME - explosion_time) > 6 * ONE_SEC)
    {
        explosion_state = false;
        choose_next_tune ();
    }

    /* High score entry is technically part of attract, but overrides the normal
     * flow of code. */
    if (gEnteringName)
    {
        /*  First time through? */
        if (attractFunc == ATR_HSSETUP)
        {
            attractFunc = ATR_HSENTRY;
            SetCountdownTimer (gAttractTimes[attractFunc] * ONE_SEC);
            ShowAttract (attractFunc, true);
        }
        GetHighScoreName ();
        play_demogame (true, false, trackno, false);
        return;
    }

    /*  Check for tourney mode setup. */
    if (tourney_setup)
    {
        if (attractFunc != ATR_TOURNEY)
        {
            attractFunc = ATR_TOURNEY;
            ShowTourneySetup (true);
        }
        HandleTourneySetup ();
        if (tourney_setup == 0)
        {
            attractFunc = ATR_MOVIE5;
            ShowAttract (attractFunc, true);
            SetCountdownTimer (gAttractTimes[attractFunc] * ONE_SEC);
        }
        return;
    }

    /*  Check stat mode or Move to the next function if time is out. */
    if (ChkGameStats ())
    {
        attractFunc = ATR_GAMESTAT;
        SetCountdownTimer (ONE_SEC);
    }
    else if (!TourneyOn ())
    {
        /*  Allow holding music to hold current screen, or toggling through screens. */
        /*  Holding music for > 2 seconds will get rush team credits when released. */
        if (levels & SW_RADIO)
        {
            SetCountdownTimer (ONE_SEC);
            if (edges & SW_RADIO)
                radio_time = IRQTIME;
        }
        else if (trailing_edges & SW_RADIO)
        {
            SetCountdownTimer (0);
            if ((IRQTIME - radio_time) > 2 * ONE_SEC)
            {
                attractFunc = ATR_TEAM - 1;     /* Will inc to ATR_TEAM. */
                skip = 0;
            }
        }
    }

    if (TimeOut () || attractFunc == ATR_HSENTRY ||
        ( raceAvailable() && attractFunc != ATR_JOIN ) )
    {
        ResetJoinIn ();
        gTourneyJoin = false;
        if ((gJoinInPossible || (TourneyOn () && gTourney.ext_start && gTourneyStart))
            && !(TourneyOn () && gTourney.ext_start == JOIN_QUICK && !EnoughCredit ()))
        {
            attractFunc = ATR_JOIN;
            gTourneyStart = false;
            gTourneyJoin = true;
        }
        else
        {
            /*  Don't display every screen each time through. */
            do
            {
                if (++attractFunc >= ATR_TOURNEY)
                {
                    ++skip;
#if TEST_HIGH_SCORE
                    attractFunc = ATR_HSSETUP;
                    gEnteringName = 1;
#else
                    attractFunc = ATR_HSENTRY + 1;
#endif
                }
            }
            while (((skip % 5) && (attractFunc == ATR_CREDITS || attractFunc == ATR_3DFXLOGO ||
                                   attractFunc == ATR_TEAM)) ||
                   (attractFunc == ATR_MOVIE4 && Random ((F32) IRQTIME) > 900 * ONE_SEC) ||
                   (attractFunc == ATR_ADVERT && TourneyOn ()));
        }
	reset_IRQTIME(); /* reset interrupt timer between attract segments. */
        SetCountdownTimer (gAttractTimes[attractFunc] * ONE_SEC);
        gTourneyStart = false;
    }

    if( attractFunc == ATR_JOIN && gstate == ATTRACT )
    {
	int i;
        U32 maxtime = 0;

	for (i = 0; i < MAX_LINKS; i++)
        {
	    if( gn_state[i].alive && gn_state[i].d.state == TRKSEL &&
		( gn_state[i].d.cdown > maxtime ) )
	        maxtime = gn_state[i].d.cdown;
	}

	SetCountdownTimer( maxtime );
    }

    /*  If changing functions, remove the old screen and display the new. */
    if (gLastFunc != attractFunc)
    {
        ShowAttract (gLastFunc, false);
        ShowAttract (attractFunc, true);
    }

    /* wait until message is sent before we continue with attract */
    if (join_during_load)
    {
        ShowAttract (attractFunc, false);
        attractFunc = ATR_ATARILOGO;
        return;
    }

/*---------------------------------------------------------------------------*/

    /*  Check for attract mode exit (GAME START). */
    if (gStartDuringLoad || chk_start ())
    {
        attract_music = false;
        explosion_state = false;

        /* stop sounds and set to game volume */
        SOUND (S_STOP_ALL_SNDS);
        SOUND (S_GAME_MODE);

        /*  Remove the screen. */
        ShowAttract (gLastFunc, false);
        ShowAttract (attractFunc = ATR_TRANSIT, true);

        /*  Set/reset linking variables as needed. */
        gSimID = 0;
        gTrackLocked = 0;
        gTourneyStart = false;
        gTourneyJoin = false;

        demo_game = 0;
        gDemoInited = false;
        gLastFunc = -1;
        this_node = gThisNode;

        playitagain_time = 0;
        whats_your_name_time = 0;
    }
    else
        /*  If doing a demo game, must keep calling the func.  Other attract
         *  modes are completely controlled by AnimateBlit functions which
         *  get called from the main game loop.
        */
        switch (attractFunc)
        {
            case ATR_DEMO:
            case ATR_DEMO2:
            case ATR_HISCORE1:
            case ATR_HISCORE2:
            case ATR_HISCORE3:
            case ATR_HISCORE4:
            case ATR_HISCORE5:
            case ATR_HISCORE6:
            case ATR_ADVERT:
            case ATR_ADVISORY:  play_demogame (true, false, -1, false); break;
            case ATR_ATARILOGO: AttractMovie (1, false, false);         break;
            case ATR_MOVIE2:    AttractMovie (2, false, true);          break;
            case ATR_MOVIE3:    AttractMovie (3, false, true);          break;
            case ATR_MOVIE4:    AttractMovie (4, false, false);         break;
            case ATR_MOVIE5:    AttractMovie (5, false, false);         break;
            default: break;
        }

}                               /* end attract */

#if 0
/* ----------------------------------------------------------------------------- *
 * Displays or removes the logo screen.
 * ----------------------------------------------------------------------------- */
static void
ShowLogo (BOOL show)
{
    S32 i;

    /* If Logo doesn't exist yet, create it. */
    if (!LogoBlit && show)
    {
        if (!CheckModelType (MT_ATTRACT))
            return;
        LogoBlit = NewMultiBlit (0, 0, LogoMultiBlit, NBLITS (LogoMultiBlit));
        AddJoinNow (LogoBlit, 0, true);
    }

    if (!show && LogoBlit)
    {
        /*  Remove the screen. */
        RemoveMsgs ();
        RemoveBlit (LogoBlit);
        LogoBlit = nil;
    }

}                               /* end ShowLogo */
#endif

/* ----------------------------------------------------------------------------- *
 * Called every frame to animate the logo.
 * ----------------------------------------------------------------------------- */
static int
AnimateLogo (BLIT * blt)
{
#if LOGO_LETTERS
    S16 i;
    F32 roll = 0.1;
    static F32 totalRoll;

    if (blt->Z == 0)
    {
        blt->Z = 15;
        UpdateBlit (blt);
        totalRoll = 2.0 * (PI - roll);
    }

    if (blt->AnimID == 0)
    {
        totalRoll += roll;
        if (totalRoll < 2.0 * PI)
        {
            for (i = 0; i < 4; ++i)
            {
                RollUV (roll, ObjInstanceList[gLogoObj[i]].mat);
                ObjInstanceList[gLogoObj[i]].mat[3][2] = 20 * fcos (totalRoll + PI / 3.0);
            }
        }
        else if (totalRoll > 3.5 * PI)
            totalRoll = 0;

        for (i = 0; i < 4; ++i)
        {
            ObjInstanceList[gLogoObj[i]].mat[3][1] = 2.5;
            ZOID_UpdateObjectMat (gLogoObj[i]);
        }
    }
#endif /* LOGO_LETTERS */
    return 1;
}                               /* end AnimateLogo */


/* ----------------------------------------------------------------------------- *
 * Displays or removes the credits screen.
 * ----------------------------------------------------------------------------- */
static void
ShowCredits (BOOL show)
{
    /* If blit doesn't exist yet, create it. */
    if (!CreditsBlit && show)
    {
        if (!CheckModelType (MT_ATTRACT))
            return;

        CreditsBlit =
          NewMultiBlit (0, 0, CreditsMultiBlit, NBLITS (CreditsMultiBlit));

        AddJoinNow (CreditsBlit, 0, true);
    }

    if (!show && CreditsBlit)
    {
        /*  Remove the screen. */
        RemoveMsgs ();
        RemoveBlit (CreditsBlit);
        CreditsBlit = nil;
    }

}                               /* end ShowCredits */


/* ----------------------------------------------------------------------------- *
 * Displays or removes the credits screen.
 * ----------------------------------------------------------------------------- */
static void
ShowTeam (BOOL show)
{
    /* If blit doesn't exist yet, create it. */
    if (!TeamBlit && show)
    {
        if (!CheckModelType (MT_ATTRACT))
            return;
        TeamBlit = NewMultiBlit (0, 0, TeamMultiBlit, NBLITS (TeamMultiBlit));
        AddJoinNow (TeamBlit, 0, true);
    }

    if (!show && TeamBlit)
    {
        /*  Remove the screen. */
        RemoveMsgs ();
        RemoveBlit (TeamBlit);
        TeamBlit = nil;
    }

}                               /* end ShowTeam */


/* ----------------------------------------------------------------------------- *
 * Displays or removes the credits screen.
 * ----------------------------------------------------------------------------- */
static void
Show3dfxLogo (BOOL show)
{
    /* If blit doesn't exist yet, create it. */
    if (!Logo3Blit && show)
    {
        if (!CheckModelType (MT_ATTRACT))
            return;
        Logo3Blit = NewMultiBlit (0, 0, Logo3MultiBlit, NBLITS (Logo3MultiBlit));
        Logo3Blit->Width = SCR_W;
        Logo3Blit->Height = SCR_H;
        UpdateBlit (Logo3Blit);
        AddJoinNow (Logo3Blit, 0, true);
    }

    if (!show && Logo3Blit)
    {
        /*  Remove the screen. */
        RemoveMsgs ();
        RemoveBlit (Logo3Blit);
        Logo3Blit = nil;
    }

}                               /* end Show3dfxLogo */


/* ----------------------------------------------------------------------------- *
 * Displays or removes the Join In screen.
 * ----------------------------------------------------------------------------- */
static void
ShowJoin (BOOL show)
{
    /* If blit doesn't exist yet, create it. */
    if (!JoinBlit && show)
    {
        JoinBlit = NewMultiBlit (0, 0, JoinMultiBlit, NBLITS (JoinMultiBlit));
        JoinBlit->Width = SCR_W;
        JoinBlit->Height = SCR_H;
        UpdateBlit (JoinBlit);
        AddJoinNow (JoinBlit, 0, true);
    }

    if (!show && JoinBlit)
    {
        /*  Remove the screen. */
        RemoveMsgs ();
        RemoveBlit (JoinBlit);
        JoinBlit = nil;
    }

}                               /* end ShowJoin */


/* ----------------------------------------------------------------------------- *
 * Displays or removes the transition screen.
 * ----------------------------------------------------------------------------- */
void
ShowTransit (BOOL show, S16 num)
{
    S16 i;
    extern int (*gTransitionFunc) (BLIT *);

    /* If blit doesn't exist yet, create it. */
    if (!TransitBlit && show)
    {
        /*  Load in the transition screen. */
        if (num == 1)
        {
            /*  Load the static stuff and the transition, then finish loading during
             *  InitSelect.  This lets the transition get started. */
            if (!Load (1))
                return;
        }
        else
            Load (2);

        TransitBlit = NewMultiBlit (0, 0, TransitMultiBlit, NBLITS (TransitMultiBlit));
        gLogoObj[0] = MBOX_NewObject (MBOX_FindObject ("LOGOR"), zerovector, unituvs, -1, 0);
        gLogoObj[1] = MBOX_NewObject (MBOX_FindObject ("LOGOU"), zerovector, unituvs, -1, 0);
        gLogoObj[2] = MBOX_NewObject (MBOX_FindObject ("LOGOS"), zerovector, unituvs, -1, 0);
        gLogoObj[3] = MBOX_NewObject (MBOX_FindObject ("LOGOH"), zerovector, unituvs, -1, 0);

        for (i = 0; i < 4; ++i)
        {
            YawUV (-PI / 2.0, ObjInstanceList[gLogoObj[i]].mat);
            RollUV (i * PI / 4.0, ObjInstanceList[gLogoObj[i]].mat);
            ObjInstanceList[gLogoObj[i]].mat[3][2] = 75;
        }

        /*  Start loading the select world. */
        gTransitionFunc = AnimateTransit;
        if (num == 1)
            InitSelect ();
    }

    if (!show && TransitBlit)
    {
        /*  Remove the letters. */
        for (i = 0; i < 4; ++i)
        {
            MBOX_RemoveObject (gLogoObj[i]);
            gLogoObj[i] = 0;
        }

        /*  Remove the screen. */
        RemoveMsgs ();
        RemoveBlit (TransitBlit);
        TransitBlit = nil;
    }

}                               /* end ShowTransit */

/* ----------------------------------------------------------------------------- *
 * Called every frame to animate the transition.
 * ----------------------------------------------------------------------------- */
int
AnimateTransit (BLIT * blt)
{
    S16 i;
    F32 roll, dt;
    static F32 totalTime;
    static U32 lastTime;

    /*  Happens on initial call. */
    if ((U32) blt > 100)
    {
        lastTime = IRQTIME;
        totalTime = 0;
        blt->Z = 60;
        blt->Hide = 1;
        blt->AnimFunc = nil;
        UpdateBlit (blt);
    }

    dt = (F32) (IRQTIME - lastTime) / (F32) (ONE_SEC);
    lastTime = IRQTIME;
    totalTime += dt;
    roll = 10.0 * dt;
    for (i = 0; i < 4; ++i)
    {
        RollUV (roll, ObjInstanceList[gLogoObj[i]].mat);
        ObjInstanceList[gLogoObj[i]].mat[3][2] = 75 - totalTime * 30.0;
        ZOID_UpdateObjectMat (gLogoObj[i]);
    }

    return 1;

}                               /* end AnimateTransit */

/* ----------------------------------------------------------------------------- *
 * Displays or removes the Parent's Advisory screen.
 * ----------------------------------------------------------------------------- */
static void
ShowAdvisory (BOOL show)
{
    char str[80];
    S16 i;

    /* If blit doesn't exist yet, create it. */
    if (!AdvisoryBlit && show)
    {
        if (!play_demogame (show, false, -1, true))
            return;
        ConstantFog (120, 0);
        if (!CheckModelType (MT_HISCORE))
            return;
        AdvisoryBlit = NewMultiBlit (0, 0, AdvisoryMultiBlit, NBLITS (AdvisoryMultiBlit));
        AddJoinNow (AdvisoryBlit, 2, true);
        InitMsgs (5, false);
        nsprintf (str, 80, "Version %3.2f           %s%s", gVersion,
                  GetMFG ()[MFG_TYPE], GetMFG ()[MFG_SERIAL]);
        for (i = 0; str[i]; ++i)
            if (str[i] == '/')
                str[i] = '.';
        AddMsg (-256, SCR_B - 60, str, 0, nil, 0);

        if (dlevels && SW_DEBUG11)
        {
            strcpy (str, date);
            str[11] = 0;
            AddMsg (10, SCR_B - 30, str, 0, nil, 0);
            AddMsg (10, SCR_B - 14, &date[12], 0, nil, 0);
        }
    }

    if (!show && AdvisoryBlit)
    {
        /*  Remove the screen. */
        RemoveMsgs ();
        RemoveBlit (AdvisoryBlit);
        AdvisoryBlit = nil;
        play_demogame (false, false, -1, false);
    }

}                               /* end ShowAdvisory */


/* ------------------------------------------------------------------------- */
BOOL
play_demogame (BOOL show, BOOL allow_sound, S16 trk, BOOL canAbort)
{
    if (show)
    {
        if (!gDemoInited)
        {
            gDemoInited = true;
            demo_game = 1;
            fake_init (trk);

            if (GetNetTime () > (500 * NET_SEC))        /* 500 seconds */
                sync_net_time (0);      /*Need reasonable NetTime for demo_game. */

            if (allow_sound)
            {
                check_sound_time (Initialize);
                attract_effects = attract_effects_save;
            }
            else
                attract_effects = false;

            if (!preplay (attractFunc == ATR_DEMO || attractFunc == ATR_DEMO2, canAbort))
                return false;

            Update_MDrive ();
            do_car_setup ();
            init_cp_time ();
        }

        playgame ();
    }
    else
    {
        gDemoInited = false;
        demo_endgame ();
        demo_game = 0;

        if (allow_sound)
            choose_next_tune ();
    }
    return true;
}                               /* end play_demogame */

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
   *
   *       Make next musical selection                                          */

void
choose_next_tune (void)
{
    SOUND (S_STOP_ALL_SNDS);

    if (attract_music && playitagain_time == 0)
    {
        if (++demo_station >= max_radio)
            demo_station = 0;

        StartRadio (demo_station + max_radio + 1);
    }
}


/* ----------------------------------------------------------------------------- *
 * Displays or removes the game over screen.
 * ----------------------------------------------------------------------------- */
void
ShowGameOver (BOOL show)
{
    S32 i, t, x, lock;
    static char str[20], s[2];

    /* If Over Select doesn't exist yet, create it. */
    if (!OverBlit && show)
    {
        if (!CheckModelType (MT_HUD))
            return;
        OverBlit = NewMultiBlit (0, 0, OverMultiBlit, NBLITS (OverMultiBlit));

        /*  Make a score string. */
        lock = game_car[gThisNode].place_locked == 1;
        if (lock)
            t = game_car[this_node].score;
        else
            t = GetElapsedTime ();
        cvt_time_str (t, str, 'f');

        InitMsgs (4, true);
        sfontX.scale_x = sfontX.scale_y = 2.0;
        AddMsg (-256, 120, str, 0, AnimateGOScore, lock ? 0x10 : -1);
        sfontX.scale_x = sfontX.scale_y = 1.0;
        InitMsgs (3, true);
        if (TourneyOn ())
            AddMsg (-256, SCR_B - 50, "TOURNAMENT", 0, AnimateBlink, 0);
    }

    if (!show)
    {
        /*  RemoveHUD also removes msgs. */
        RemoveHUD ();
        ctl_mod_latch (~LA_WINNER);
        if (OverBlit)
        {
            /*  Remove the screen. */
            RemoveBlit (OverBlit);
            OverBlit = nil;
        }
    }

}                               /* end ShowGameOver */


/* ----------------------------------------------------------------------------- *
 * Called every frame to the winner message.
 * ----------------------------------------------------------------------------- */
static int
AnimateWin (BLIT * blt)
{
    char str[20];
    S32 i, slot;
    CAR_DATA *c = game_car;
    MODELDAT *m;

    for (i = 0; i < num_active_cars; ++i)
    {
        slot = model[i].slot;
        m = &model[slot];
        c = &game_car[slot];
        if (c->place_locked == 1 && c->place == 0 && m->drone_type == HUMAN)
            break;
    }

    if (Hidden (blt, i == num_active_cars || (solo_flag && !win_opts)))
        return 1;

    i = c - game_car;
#if WIN_BLITS
    if (blt->AnimID == 0xFF)
    {
        sprintf (str, "WIN_%s", NodeName (i));
        RenameBlit (blt, str, 0);
        blt->X = SCR_MH - ((i == 1 || i == 3 || i == 6) ? 64 : 128);
        blt->AnimID = i;
    }
#else
    Hidden (blt, 1);
    sprintf (str, "%s car wins!", NodeName (i));
    InitMsgs (4, false);
    sfontX.scale_x = sfontX.scale_y = 1.5;
    AddMsg (-256, SCR_T + 50, str, 0, nil, 0);
    sfontX.scale_x = sfontX.scale_y = 1.0;
#endif

    /*  Check for a free game winner. */
    if (win_opts && num_humans >= (1 << (win_opts - 1)) && i == gThisNode && !TourneyOn () &&
        (gLink[gThisNode].flags & LINK_DRONES) && !gPlayingFree)
    {
        /*  Add a bonus credit to a game. */
        InitMsgs (3, false);
        AddMsg (-256, SCR_B - 60, "FREE GAME", 0, AnimateBlink, 0);
        gFreeGame = true;
/* EER_DRAWS no longer exists in the latest GUTS */
/*      eer_incs( EER_DRAWS, 1); */
    }

    blt->AnimFunc = nil;
    return 1;

}                               /* end AnimateWin */


/* ----------------------------------------------------------------------------- *
 * Called every frame to update the player's rank in the game over screen.
 * ----------------------------------------------------------------------------- */
static int
AnimateRank (BLIT * blt)
{
    S16 place = game_car[gThisNode].place;
    extern S16 mBlitIndex;

    if (place == 0 && (TourneyOn () || game_car[gThisNode].place_locked))
        ctl_mod_latch (LA_WINNER);
    else
        ctl_mod_latch (~LA_WINNER);

    if (Hidden (blt, solo_flag && !(gLink[gThisNode].flags & LINK_DRONES)))
        return 1;

    /*  Handle the rank part of the string. */
    if (mBlitIndex != MBLIT_PLACE || place != blt->AnimDTA)
    {
        char name[20];
        sprintf (name, "M8F%02ld", place + 5L);
        RenameBlit (blt, name, 1);
        mBlitIndex = MBLIT_PLACE;
        blt->AnimDTA = place;
    }
    blt->X = SCR_MH - 256 / 2;

    return AnimateBlink (blt);
}                               /* end AnimateRank */


/* ----------------------------------------------------------------------------- *
 * Called every frame to update the Press Start/Insert Coins Messages.
 * ----------------------------------------------------------------------------- */
static int
AnimateStart (BLIT * blt)
{
    BOOL type = blt->AnimID & 0xf;
    BOOL enuff;

    if (Hidden (blt, (gstate == PLAYGAME || gstate == COUNTDOWN || gstate == PREPLAY2) && !demo_game))
        return 1;

    if (Hidden (blt, TourneyOn () && gTourney.ext_start && EnoughCredit ()))
        return 1;

    if (!(blt->AnimID & 0x10))
    {
        if (blt->AnimID == 1)   /*  type 1 */
        {
            blt->X = SCR_R - 145;
            blt->Y = SCR_B - 84;
        }
        else if (blt->AnimID == 2)      /*  type 2 */
        {
            blt->X = SCR_MH - 64;
            blt->Y = SCR_B - 85;
        }
        else
            /*  type 0 */
        {
            blt->X = SCR_MH - 64;
            blt->Y = SCR_B - 100;
        }

        blt->AnimID |= 0x10;
        UpdateBlit (blt);
    }

    if (language == LANG_JAPANESE)
        updateBoolean (blt, EnoughCredit (), "JPRESSSELECT", "INSCOINS");
    else
        updateBoolean (blt, EnoughCredit (), "START", "INSCOINS");
    return AnimateBlink (blt);
}                               /* end AnimateStart */


/* ----------------------------------------------------------------------------- *
 * Called every frame to update the Countdown timer.
 *  ID = (timer_type << 12) | (inited << 8) | (digit_type << 4) | (digit_position)
 * ----------------------------------------------------------------------------- */
int
AnimateTimer (BLIT * blt)
{
    S32 timer = GetCountdownTime ();
    S32 sec = GetCountdownTime () / ONE_SEC;
    BOOL hid = ( (sec   <= 5 && (IRQTIME & 0x100)) || timer < (ONE_SEC/4) );
    S32 type = (blt->AnimID >> 12) & 0xF;

    if (sec < 0)
        sec = 0;

    switch (type)
    {
        case 0:         /* Select */
            hid = hid || gClutchLocked;
            break;
        case 1:         /* Attract */
            if (TourneyOn () && gTourney.ext_start)
                hid = hid || !gTourneyJoin;
            else
                hid = hid || !gJoinInPossible;

            if ((attractFunc == ATR_JOIN) && !gJoinInPossible)
                SetCountdownTimer (0);
            break;
        case 3:
            blt->Y = 220;
            break;
        case 4:
            hid = hid || !TourneyOn ();
            break;

        default:
            break;              /* Hi Score Entry */
    }


    if (Hidden (blt, hid))
        return 1;

    return UpdateNumber (blt, ((blt->AnimID & 0xF) ? sec : (sec / 10)) % 10);
}                               /* end AnimateTimer */


/* ----------------------------------------------------------------------------- *
 * Called every frame to update the 'Join Now' Messages.
 * ----------------------------------------------------------------------------- */
static int
AnimateJoin (BLIT * blt)
{
    ResetJoinIn();

    if (TourneyOn () && gTourney.ext_start)
        return (Hidden (blt, !gTourneyJoin)) ? 1 : AnimateBlink (blt);

    if (Hidden (blt, !gJoinInPossible ||
                ((gstate == PLAYGAME || gstate == COUNTDOWN || gstate == PREPLAY2) && !demo_game)))
        return 1;

    waiting_for_others (0);

    if (!(blt->AnimID & 0x10))
    {
        if (language == LANG_JAPANESE)
            RenameBlit (blt, "JJOINNOW", 1);
        blt->Width = blt->Info->Width * 2;
        blt->Height = blt->Info->Height * 2;
        blt->X = SCR_MH - blt->Info->Width;
        blt->AnimID |= 0x10;
    }

    return AnimateBlink (blt);
}                               /* end AnimateJoin */


/* ----------------------------------------------------------------------------- *
 * Called every frame to update the Start/Over Messages.
 * ---------------------------------------------------------------------------- */
int
AnimateBlink (BLIT * blt)
{
    U32 alpha = ((IRQTIME >> 1) & 0xFF);

    if (IRQTIME & 0x200)
        alpha = 255 - alpha;

    blt->Alpha = alpha;
    UpdateBlit (blt);

    return 1;
}                               /* end AnimateBlink */


/* ----------------------------------------------------------------------------- *
 * Called every frame to update the Game Over Message.
 * ----------------------------------------------------------------------------- */
static int
AnimateOver (BLIT * blt)
{
    U32 alpha;

    if (Hidden (blt, gstate != ENDGAME && gstate != GAMEOVER))
        return 1;

    alpha = ((IRQTIME >> 1) & 0xFF);
    if (IRQTIME & 0x200)
        alpha = 255 - alpha;

    blt->Width = blt->Info->Width * 2;
    blt->Height = blt->Info->Height * 2;

    blt->X = 256 - blt->Width / 2;
    blt->Alpha = alpha;
    UpdateBlit (blt);

    return 1;
}                               /* end AnimateOver */


/* ----------------------------------------------------------------------------- *
 * Called every frame to update the score in the Game Over screen.
 * ----------------------------------------------------------------------------- */
static int
AnimateGOScore (BLIT * blt)
{
    static S32 index;
    char str[10];
    U32 alpha = ((IRQTIME >> 1) & 0xFF);

    /*  Assign initial index for this blit. */
    if (blt->AnimDTA >= 0)
        index = 0;
    else
    {
        blt->AnimID = index++;
        blt->AnimDTA = 0;
    }

    if (!(IRQTIME & 0x200))
        alpha = 255 - alpha;
    blt->Alpha = alpha;

    if (Hidden (blt, !TourneyOn () && game_car[gThisNode].place_locked != 1))
        return 1;

    /*  Just locked. */
    if (!(blt->AnimDTA & 0x10) && game_car[gThisNode].place_locked == 1)
    {
        blt->AnimDTA |= 0x10;
        cvt_time_str (game_car[gThisNode].score, str, 'f');
        InitMsgs (4, false);
        sfontX.scale_x = sfontX.scale_y = 2.0;
        RenameFontBlit (blt, 1, &sfontX, str[blt->AnimID]);
        sfontX.scale_x = sfontX.scale_y = 1.0;
    }

    UpdateBlit (blt);

    return 1;
}                               /* end AnimateGOScore */


/* ----------------------------------------------------------------------------- *
 * Called every frame to update the Credits display.
 * ----------------------------------------------------------------------------- */
int
AnimateCreds (BLIT * blt)
{
    BOOL freePlay;
    U8 id = blt->AnimID & 0xf;

    if (Hidden (blt, ((gstate == PLAYGAME && !demo_game) ||
                      (TourneyOn () && gTourney.free))))
        return 1;

    freePlay = (cn_price () == 0);

    if (id && freePlay)
        blt->AnimFunc = nil;

    if (denom < 10)
    {
        switch (id)
        {
            case 0:             /*  Label */
                blt->X = SCR_MH - ((freePlay) ? 50 : 65);
                updateBoolean (blt, freePlay, "FREEPLAY", "CREDITS");
                blt->AnimFunc = nil;
                break;

            case 1:             /*  Whole credits digit. */
                if (Hidden (blt, freePlay || credits == 0))
                    return 1;
                return UpdateNumber (blt, credits > 9 ? 9 : credits);

            case 2:             /*  Numerator credits digit. */
                if (Hidden (blt, freePlay || (numer == 0 && credits != 0)))
                    return 1;
                return UpdateNumber (blt, numer % 10);

            case 3:             /*  Slash. */
                if (Hidden (blt, freePlay || (numer == 0 && credits != 0)))
                    return 1;
                return UpdateNumber (blt, 10);

            case 4:             /*  Denominator. */
                if (Hidden (blt, freePlay || (numer == 0 && credits != 0)))
                    return 1;
                return UpdateNumber (blt, denom % 10);

            case 5:             /* unused */
            case 6:
                return (Hidden (blt, true));
        }
    }
    else
    {
        switch (id)
        {
            case 0:             /*  Label */
                blt->X = SCR_MH - ((freePlay) ? 50 : 65);
                updateBoolean (blt, freePlay, "FREEPLAY", "CREDITS");
                blt->AnimFunc = nil;
                break;

            case 1:             /*  Whole credits digit. */
                if (Hidden (blt, freePlay || credits == 0))
                    return 1;
                return UpdateNumber (blt, credits > 9 ? 9 : credits);

            case 2:             /*  Numerator credits ten's digit. */
                if (Hidden (blt, freePlay || numer < 10))
                    return 1;
                return UpdateNumber (blt, numer / 10);

            case 3:             /*  Numerator credits one's digit. */
                if (Hidden (blt, freePlay || (numer == 0 && credits != 0)))
                    return 1;
                return UpdateNumber (blt, numer % 10);

            case 4:             /*  Slash. */
                if (Hidden (blt, freePlay || (numer == 0 && credits != 0)))
                    return 1;
                return UpdateNumber (blt, 10);

            case 5:             /*  Denominator. ten's digit */
                if (Hidden (blt, freePlay || (numer == 0 && credits != 0)))
                    return 1;
                return UpdateNumber (blt, denom / 10);

            case 6:             /*  Denominator. one's digit */
                if (Hidden (blt, freePlay || (numer == 0 && credits != 0)))
                    return 1;
                return UpdateNumber (blt, denom % 10);
        }
    }

    return 1;
}                               /* end AnimateCreds */


/* ----------------------------------------------------------------------------- *
 * Adds the 'JOIN NOW' message to the screen.
 * ----------------------------------------------------------------------------- */
void
AddJoinNow (BLIT * parent, S16 type, BOOL reset)
{
    BLIT *blt;

    InitMsgs (3, reset);
    if (!CheckModelType (MT_STATIC))
        return;
    JoinInBlit = NewMultiBlit (0, 0, JoinInMultiBlit, NBLITS (JoinInMultiBlit));
    JoinInBlit->AnimID = type;
    mBlitIndex = MBLIT_JOIN;

    if (TourneyOn ())
        AddMsg (-256, SCR_MV - 100, "TOURNAMENT", 0, AnimateBlink, 0);

    AttatchBlit (parent, JoinInBlit);
}                               /* end AddJoinNow */


/* ----------------------------------------------------------------------------- *
 * Displays or removes a generic movie screen.
 * ----------------------------------------------------------------------------- */
static void
ShowMovie (BOOL show, S32 num)
{
    /* If blit doesn't exist yet, create it. */
    if (!MovieBlit && show)
    {
        Load (6);
        AttractMovie (num, true, true);
        MovieBlit = NewMultiBlit (0, 0, MovieMultiBlit, NBLITS (MovieMultiBlit));
        MovieBlit->Hide = 1;
        UpdateBlit (MovieBlit);
        AddJoinNow (MovieBlit, 0, true);

        if (num == 5 && attract_effects_save)
            SOUND (S_RUSHWHISPER);
    }

    if (!show && MovieBlit)
    {
        /*  Remove the screen. */
        RemoveMsgs ();
        RemoveBlit (MovieBlit);
        MovieBlit = nil;
        if (explosion_state == true)
            explosion_time = IRQTIME + 10 * ONE_SEC;
    }

}                               /* end ShowMovie */


/* ----------------------------------------------------------------------------- *
 * Plays the given movie.
 * ----------------------------------------------------------------------------- */
static void
AttractMovie (S32 num, BOOL init, BOOL loop)
{
    static Blit *scr;
    static S32 model, frame, frameTime;
    static const char *const mNames[8] =
    {
/*      "ATARI01", "BUG0001", "CON0001", "WOMAN01", "LOGO0001", "X", "X","X" }; */
     "ATARI01", "BUG0001", "CON0001", "WOMAN01", "rock0001", "X", "X", "X"};
    static const S32 mFrames[8] =
    {
        76, 74, 74, 60, 125, 0, 0, 0};
/*             76,        74,        74,        60,        120,   0,  0,  0  }; */

    char name[40];

    if (init)
    {
        sprintf (name, "MOVIES/MOVIE%ld", num);
        model = MBOX_LoadMovie (name, MOVIESIZE, 1);
#if TRACE_MODELS
        MSG ("M%2ld MBOX_LoadMovie(%s)", (long) model, name);
        ZOID_EndFrame ();
#endif
        scr = NewBlit (mNames[num - 1], 0, 0);
        scr->Width = SCR_W;
        scr->Height = SCR_H;
        UpdateBlit (scr);
        frame = -1;
        frameTime = 0;
    }

    /*  Make sure the movie doesn't play too fast. */
    if (num == 4 && (IRQTIME - frameTime < (ONE_SEC / 24)))     /* 24 fps */
        return;
    else if (IRQTIME - frameTime < (ONE_SEC / 22))      /* 22 fps */
        return;
    frameTime = IRQTIME;

    if (++frame >= mFrames[num - 1])
    {
        if (loop)
            frame = 0;
        else
        {
            if (!(gJoinInPossible || (TourneyOn () && gTourney.ext_start && gTourneyStart)))
                SetCountdownTimer (2);
            return;
        }
    }

#if 0
    if (num == 5 && frame == 62 && attract_effects_save)
    {
        SOUND (S_EXPLO);
        explosion_state = true;
        explosion_time = IRQTIME;
    }
#else
    if (num == 5 && frame == 2 && attract_effects_save)
    {
        SOUND (S_PRISONSIREN);
    }
    if (num == 5 && frame == 80 && attract_effects_save)
    {
        SOUND (S_EXPLO);
        explosion_state = true;
        explosion_time = IRQTIME;
    }
    if (num == 5 && frame == 100 && attract_effects_save)
    {
        SOUND (S_JAILDOORLOOP);
    }
    if (num == 5 && frame == 124 && attract_effects_save)
    {
        SOUND (S_KJAILDOORLOOP);
        SOUND (S_JAILDOORCLANG);
    }
#endif

    MBOX_MovieFrame (model, frame, 1);
}                               /*  end AttractMovie */


/* ----------------------------------------------------------------------------- *
 * Displays or removes the Tourney Advertisement screen.
 * ----------------------------------------------------------------------------- */
static void
ShowAdvert (BOOL show)
{
    char str[80];
    S16 i;

    /* If blit doesn't exist yet, create it. */
    if (!AdvertBlit && show)
    {
        if (!play_demogame (show, false, -1, true))
            return;
        ConstantFog (120, 0);
        if (!CheckModelType (MT_HISCORE))
            return;
        AdvertBlit = NewMultiBlit (0, 0, AdvertMultiBlit, NBLITS (AdvertMultiBlit));
        AddJoinNow (AdvertBlit, 2, true);
        InitMsgs (5, false);
        nsprintf (str, 80, "Version %3.2f           %s%s", gVersion,
                  GetMFG ()[MFG_TYPE], GetMFG ()[MFG_SERIAL]);
        for (i = 0; str[i]; ++i)
            if (str[i] == '/')
                str[i] = '.';
        AddMsg (-256, SCR_B - 60, str, 0, nil, 0);
    }

    if (!show && AdvertBlit)
    {
        /*  Remove the screen. */
        RemoveMsgs ();
        RemoveBlit (AdvertBlit);
        AdvertBlit = nil;
        play_demogame (false, false, -1, false);
    }
}                               /* end ShowAdvert */

/* ----------------------------------------------------------------------------- *
 * Called every frame to update the free game and tourney advert Messages.
 * ----------------------------------------------------------------------------- */
static int
AnimateAdvert (BLIT * blt)
{
    if (blt->AnimID == 0)       /*  Free game */
        Hidden (blt, !win_opts);

    if (blt->AnimID == 1)       /*  Tourney mode */
        Hidden (blt, !(game_opt & GO_ADVERT));

    return 1;
}                               /* end AnimateAdvert */
