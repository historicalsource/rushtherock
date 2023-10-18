/*
 *  attract.h -- attract mode inlcudes
 *
 *      Copyright 1996 Time Warner Interactive.
 *  Unauthorized reproduction, adaptation, distribution, performance or
 *  display of this computer program or the associated audiovisual work
 *  is strictly prohibited.
 *
 *  ======================================================================
 *  $Author: grjost $                       $Date $
 *  $Revision $                     $Locker:  $
 *  ======================================================================
 *  Change Log:
 *
 *  $Log: attract.h,v $
 * Revision 3.37  1997/10/03  00:07:38  grjost
 * Added more rock and atari movies
 *
 * Revision 3.36  1997/10/02  22:13:34  gray
 * Credits display handles up to 12 coin/cedit and up to 99 credits.
 *
 * Revision 3.35  1997/10/02  02:04:22  grjost
 * Added old credit and team screens
 *
 * Revision 3.34  1997/10/02  01:26:04  grjost
 * Added and shuffled attract screens
 *
 * Revision 3.33  1997/10/01  03:42:58  grjost
 * Added track 7 hi-score
 *
 * Revision 3.32  1997/09/27  01:02:26  grjost
 * Put in mirror mode in hiscore table.
 *
 * Revision 3.31  1997/09/26  00:12:41  hightower
 * Now using gamenet to fix ability to join within last few seconds
 *
 * Revision 3.30  1997/09/25  02:07:23  grjost
 * Fixed tourney mode for 12 tracks, 8 cars, and menus in GUTS.
 *
 * Revision 3.29  1997/09/19  22:06:21  grjost
 * Added hiscore for T5 & T6
 *
 * Revision 3.28  1997/09/15  22:27:35  gray
 * Removed trackset hack - trackno now 0 to NTRACKS.
 *
 * Revision 3.27  1997/06/28  01:40:05  gray
 * Extended to 4 tracks.
 *
 * Revision 3.26  1997/03/18  19:41:22  geraci
 * added two new blits for more than ten coins
 *
 * Revision 3.25  1997/02/15  01:42:59  geraci
 * each track in tourney can have its own number of laps
 *
 * Revision 3.24  1997/01/31  23:35:34  geraci
 *  removed difficulty from tourney setup screen
 *
 * Revision 3.23  1996/11/25  14:19:16  petrick
 * Got rid of 'active' field of tourney.
 *
 * Revision 3.22  1996/11/25  04:09:01  petrick
 * Neaterized.
 *
 * Revision 3.21  1996/11/24  10:53:41  petrick
 * Added a second demo mode.
 *
 * Revision 3.20  1996/11/21  13:43:26  petrick
 * Added define to allow tracing of which models are loaded.
 *
 * Revision 3.19  1996/11/17  14:27:10  petrick
 * Defs for type of models.
 *
 * Revision 3.18  1996/11/16  09:49:19  petrick
 * Added atr_join.
 *
 * Revision 3.17  1996/11/15  23:25:38  petrick
 * Made Rush logo movie 1 long movie.
 *
 * Revision 3.16  1996/11/09  13:07:53  petrick
 * Buncha new attract screens.
 *
 * Revision 3.15  1996/11/06  03:11:23  petrick
 * Looping movies and team pic.
 *
 * Revision 3.14  1996/11/05  12:24:35  petrick
 * Auto-clear high score table defines.
 *
 * Revision 3.13  1996/11/02  09:52:53  petrick
 * Added 2 car movies.
 *
 *
 *  ======================================================================
 */

#include "globals.h"
#include "hud.h"
#include "font.h"

#include "Pro/attract.pro"
#include "Pro/hiscore.pro"
#include "Pro/tourney.pro"


/*===============================  DEFINES  =================================*/

#define TRACE_MODELS    0     /*  Set to 1 to trace model loading. */
#define NSCORES         100   /* Number of scores saved. */

#define TABLE_BASE      ((U32 *)(BRAM_BASE + 8*1024L))
#define TABLE_SIZE      (sizeof(HiScore) * NSCORES)

#define MAX_GAME_CNT    2000  /* max total games before AUTO HSCORE RESET     */
#define MAX_HI_CNT      200   /* max non-HS games before AUTO HSCORE RESET    */

#define NLENGTH         9   /* Max length of a high score name+1. */

typedef enum ModelTypes
{
    MT_NONE = 0,
    MT_ATTRACT,
    MT_HISCORE,
    MT_HUD,
    MT_STATIC
}
ModelType;

typedef enum AttractMode
{
    ATR_HSSETUP=-1,
    ATR_HSENTRY=0,
    ATR_HISCORE1,
    ATR_ATARILOGO,
    ATR_HISCORE2,
    ATR_MOVIE5,
    ATR_HISCORE3,
    ATR_TEAM,
    ATR_OTEAM,
    ATR_ATARILOGO2,
    ATR_ADVERT,
    ATR_MOVIE2,
    ATR_HISCORE4,
    ATR_MIRROR,
    ATR_HISCORE5,
    ATR_MOVIE52,
    ATR_HISCORE6,
    ATR_OCREDITS,
    ATR_3DFXLOGO,
    ATR_ATARILOGO3,
    ATR_HISCORE7,
    ATR_MOVIE4,
    ATR_DEMO,
    ATR_CREDITS,
    ATR_MOVIE53,
    ATR_ADVISORY,
    ATR_MOVIE3,
    ATR_DEMO2,
    ATR_CARS,
    /* Any Modes after here are special. Place new ones after ATR_TOURNEY. */
    ATR_TOURNEY,
    ATR_TRANSIT,
    ATR_JOIN,
    ATR_GAMESTAT,
    NUM_ATTRACTS
}
AttractMode;

/* Used as a shortcut to init MULTIBLIT arrays */
#define MUNUSED -1,-1, -1,-1,-1,-1, 0,0

#define CREDITS_DISPLAY \
    {"CREDITS", SCR_MH - 65,SCR_B - 19, MUNUSED, 0x00, AnimateCreds, 0x00},  \
     {"DIGIT0", SCR_MH + 20,SCR_B - 21, MUNUSED, 0x00, AnimateCreds, 0x01},  \
     {"DIGIT0", SCR_MH + 40,SCR_B - 21, MUNUSED, 0x00, AnimateCreds, 0x02},  \
     {"DIGIT0", SCR_MH + 60,SCR_B - 21, MUNUSED, 0x00, AnimateCreds, 0x03},  \
     {"DIGIT0", SCR_MH + 80,SCR_B - 21, MUNUSED, 0x00, AnimateCreds, 0x04},  \
     {"DIGIT0", SCR_MH +100,SCR_B - 21, MUNUSED, 0x00, AnimateCreds, 0x05},  \
     {"DIGIT0", SCR_MH +120,SCR_B - 21, MUNUSED, 0x00, AnimateCreds, 0x06},  \
     {"DIGIT0", SCR_MH +140,SCR_B - 21, MUNUSED, 0x00, AnimateCreds, 0x07},  \
     {"DIGIT0", SCR_MH +160,SCR_B - 21, MUNUSED, 0x00, AnimateCreds, 0x08},

#define HELP_DISPLAY(type)  \
     {"WHEEL",      SCR_L+ 5,SCR_B - 72, MUNUSED, 0x00, AnimateWheel, 0},    \
     {"TURNCHOOSE", SCR_L+50,SCR_B -100, MUNUSED, 0x00, AnimateText, 0},     \
     {"PEDAL",      SCR_R-62,SCR_B - 72, MUNUSED, 0x00, AnimatePedal, 0},    \
     {"PRESSSELECT",SCR_R -175,SCR_B -100, MUNUSED, 0x00, AnimateText, 1},   \
     {"DIGIT6",     218, 292, MUNUSED, 0x00, AnimateTimer,(type)*0x1000+0x60},\
     {"DIGIT6",     258, 292, MUNUSED, 0x00, AnimateTimer,(type)*0x1000+0x61},

/*===============================  TYPEDEFS  ================================*/

typedef struct HiScore
{
    U32     score;
    char    name[NLENGTH];
    S8      deaths;
    U8      mirror;      /* TRUE means race was done in mirror mode */
    S8      car;         /* High nibble contains node where score came from. */
}
HiScore;

typedef struct Tourney
{
    BOOL free;       /* 0 == pay each time, 1 == free play.                 */
    S8   ext_start;  /* 0 == each player select,                            */
                     /* 1 == Use external switch to activate join-in,       */
		     /* 2 == Same as remote but remove most game delays.    */
    U32  cabinets;   /* 1 << node is set if 'node' is in tourney.           */
    S16  laps;       /* > 0, Number of laps; 0 == No limit; < 0, Normal.    */
    S8   track;      /* >= 0, Use given track; < 0, use external switch.    */
    S8   car;        /* >= 0, Use given car; < 0, any car can play.         */
    S8   tranny;     /* >= 0, Use given tranny; < 0, any tranny.            */
    S8   drones;     /* 1 == drones; 0 == No drones; < 0, Normal drones.    */
    S8   catchup;    /* 1 == catchup; 0 == No catchup; < 0, Normal catchup. */
    U32  sanity;     /* If != DEADBEEF, settings are invalid. */
}
Tourney;
