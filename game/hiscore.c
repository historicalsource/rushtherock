/*
 *	hiscore.c -- High Score mode functions
 *
 *		Copyright 1996 Time Warner Interactive.
 *	Unauthorized reproduction, adaptation, distribution, performance or 
 *	display of this computer program or the associated audiovisual work
 *	is strictly prohibited.
 *
 *	======================================================================
 *	$Author: grjost $						$Date $
 *	$Revision: 1.60 $						$Locker:  $
 *	======================================================================
 *	Change Log:
 *
 *	$Log: hiscore.c,v $
 * Revision 1.60  1997/10/16  02:52:52  grjost
 * Hacked in continue mode
 *
 * Revision 1.59  1997/10/15  07:46:22  hightower
 * Replace Jims name with Aaron
 *
 * Revision 1.58  1997/10/09  05:53:54  hightower
 * Blink stuff based on countdown timer to synchronize blinking
 *
 * Revision 1.57  1997/10/06  07:50:13  gray
 * Now doing fog in a sane way.
 *
 * Revision 1.56  1997/10/02  01:26:20  grjost
 * Added mirrored car icons
 *
 * Revision 1.55  1997/10/01  03:43:44  grjost
 * Added track 7 hi-score
 *
 * Revision 1.54  1997/09/30  02:19:44  grjost
 * Trackname is now 2 blits
 *
 * Revision 1.53  1997/09/27  01:01:59  grjost
 * Put in mirror mode in hiscore table.
 *
 * Revision 1.52  1997/09/25  02:08:06  grjost
 * *** empty log message ***
 *
 * Revision 1.51  1997/09/19  22:06:48  grjost
 * Added hiscore for T5 & T6
 *
 * Revision 1.50  1997/09/17  07:53:46  grjost
 * Added 4 new car icons and GUTS link count
 *
 * Revision 1.49  1997/09/12  21:22:25  gray
 * Prelim. tweaks to support 12 cars and 8 tracks.
 *
 * Revision 1.48  1997/08/27  22:43:46  grjost
 * Write hi-scores to the GUTS area now.
 *
 * Revision 1.47  1997/06/28  01:41:43  gray
 * Extended to 4 tracks.
 *
 * Revision 1.46  1997/02/21  00:16:48  geraci
 * make play it again and swirling happen correctly in tourney mode
 *
 * Revision 1.45  1997/02/15  01:50:08  geraci
 * join quick in tourney implemented
 *
 * Revision 1.44  1996/12/14  09:36:20  geraci
 * tourney won't blast attract at game volume
 *
 * Revision 1.43  1996/12/06  04:57:37  geraci
 * okay now it's really fixed
 *
 * Revision 1.42  1996/12/06  04:51:12  geraci
 * fixed it again
 *
 * Revision 1.41  1996/12/06  04:49:09  geraci
 * fixed end of long tourney time
 *
 * Revision 1.40  1996/12/02  04:57:47  geraci
 * final time can be up to 99 minutes
 *
 * Revision 1.39  1996/12/02  04:49:05  geraci
 * final time can be up to 99 minutes
 *
 * Revision 1.38  1996/11/27  20:44:14  geraci
 * fixed bad hiscore car blit problem
 *
 * Revision 1.37  1996/11/27  10:05:47  petrick
 * Fix for car icon wrong on high score table.
 *
 * Revision 1.36  1996/11/27  01:58:01  petrick
 * Gave less time to mess about in high score table.
 *
 * Revision 1.35  1996/11/26  14:17:16  petrick
 * Got rid of objectionable high score table entry.
 *
 * Revision 1.34  1996/11/25  15:20:28  petrick
 * Disabled continue msg if playing free.
 *
 * Revision 1.33  1996/11/25  04:14:44  petrick
 * Made continue part of high score screen.
 *
 * Revision 1.32  1996/11/24  06:11:32  petrick
 * Added allen name to default list.
 *
 * Revision 1.31  1996/11/20  23:54:55  petrick
 * Made join now appear over scores.
 *
 * Revision 1.30  1996/11/17  14:22:14  petrick
 * Makes sure models are loaded before using them.
 *
 * Revision 1.29  1996/11/17  04:52:00  petrick
 * Put fake names in when resetting high score table.
 *
 * Revision 1.28  1996/11/16  18:56:46  geraci
 * kills sounds after tourney is over
 *
 * Revision 1.27  1996/11/16  09:49:57  petrick
 * Uses random list of names to init hi score table.
 *
 * Revision 1.26  1996/11/15  15:40:22  geraci
 * fixed up high score entry a little
 *
 * Revision 1.25  1996/11/14  13:11:16  geraci
 * oops, left a debug thing in there
 *
 * Revision 1.24  1996/11/14  12:39:18  geraci
 * uses end section for FF to moderate speed
 *
 * Revision 1.23  1996/11/13  15:13:29  geraci
 * new hiscore name entry
 *
 * Revision 1.22  1996/11/11  14:28:00  petrick
 * Made loadmodel smarter about allowing aborts.
 *
 * Revision 1.21  1996/11/07  06:38:49  petrick
 * Deactivated highs score table when tourney is not a regular length game.
 *
 * Revision 1.20  1996/11/05  12:21:29  petrick
 * Code to auto-clear the high score table after it gets too hard to score.
 *
 * Revision 1.19  1996/10/29  08:24:38  petrick
 * Made cars and hud stick around longer at end of game.
 *
 * Revision 1.18  1996/10/25  06:47:57  petrick
 * Got rid of shared high score tables.
 *
 * Revision 1.17  1996/10/24  08:45:39  geraci
 * what's your name sound fades out
 *
 * Revision 1.16  1996/10/23  03:34:17  petrick
 * Made hi score screen show track it belongs to.
 *
 * Revision 1.15  1996/10/22  10:35:12  petrick
 * Changes for 8 car types.
 *
 * Revision 1.14  1996/10/22  09:41:00  geraci
 * fixed play it again / what's your name bug
 *
 * Revision 1.13  1996/10/22  08:55:41  geraci
 * new music for high score entry
 *
 * Revision 1.12  1996/10/18  11:02:41  petrick
 * Changed load model.
 *
 * Revision 1.11  1996/10/17  08:25:21  petrick
 * Made attract func a typedef so gdb would know the state names.
 *
 * Revision 1.10  1996/10/12  11:16:52  petrick
 * Added timer to Join Now.
 *
 * Revision 1.9  1996/10/10  05:19:24  petrick
 * Cleaned out some unused globals.
 *
 * Revision 1.8  1996/10/08  02:00:53  petrick
 * WhoCares gone.
 *
 * Revision 1.7  1996/10/08  02:00:18  petrick
 * WhoCares gone, now Whizzo.
 *
 * Revision 1.6  1996/10/07  02:24:19  petrick
 * Added more Japanese.
 *
 * Revision 1.5  1996/10/04  10:37:05  petrick
 * Added Join Now func.
 *
 * Revision 1.4  1996/10/04  07:33:29  geraci
 * demo game can have sound effects disabled
 *
 * Revision 1.3  1996/10/02  21:16:30  petrick
 * Changed params in PutEventReport PDU call.
 *
 * Revision 1.2  1996/10/01  08:43:54  petrick
 * Changes to support final fog handling.
 *
 * Revision 1.1  1996/09/28  02:38:15  petrick
 * Initial revision
 *
 *
 *	======================================================================
 */

#include "attract.h"
#include "controls.h"
#include "debug.h"
#include "dis.h"
#include <eer_defs.h>
#include "font1.h"
#include "font3.h"
#include "globals.h"
#include "os_proto.h"
#include "sounds.h"
#include "targets.h"
#include <string.h>
#include <options.h>

#include "Pro/dnl.pro"
#include "Pro/select.pro"
#include "Pro/fswheel.pro"
#include "Pro/sounds.pro"

/*===============================  DEFINES  =================================*/

#define CATCH_BAD_NAMES	0		/*	1 == stop when an illegal name is being entered. */

#define NNAMES			10		/* Number of names on high score screen at one time. */
#define SSCORES			9		/* Number of scrolling scores on screen at one time. */
#define NUM_HENTRY_POS	13		/* Number of selectable slot in hiscore entry screen */
#define HSCORE_LWIDTH	32		/* width of letter on hiscore entry screen */

#define MAX_MSG_CHARS 	256
#define MIN_SCORE 		( 1L*60*ONE_SEC)	/*	 1 Minutes */
#define MAX_SCORE		(10L*60*ONE_SEC)	/*	10 Minutes */
#define MAX_HSCORE		(100L*60*ONE_SEC)	/* 100 Minutes */
#define HI_SCORE_X		 45					/* X position of high score display of high scores. */
#define LO_SCORE_X		360					/* X position of low score display of high scores. */


/*===============================  TYPEDEFS  ================================*/

typedef struct ScoreBlitEntry
{
	BLIT	*name[NLENGTH];
	BLIT	*score[8];			/* m:ss.hh format */
	S16		nlen, slen;
} ScoreBlitEntry;

typedef struct LoScoreBlitEntry
{
	BLIT	*name[4];
	BLIT	*score[8];			/* m:ss.hh format */
	S16		nlen, slen;
} LoScoreBlitEntry;


/*==============================  PROTOTYPES  ===============================*/

static BlitFunc AnimateTName,  AnimatePName, AnimateScore, AnimateFF,
				AnimateLetter, AnimateTitle, AnimateIcon, 
				AnimateHScroll,AnimateRandom, AnimatePanel;

static BOOL		checkword( char *wp );
static BOOL		fixword(char *op, char *wp);
static void 	FilterBadChars( char *str );

extern void 	eer_write_byte(U8 *where,U8 *unlock,int what);
extern void 	CleanupAfterGame(void);

/*===============================  EXTERNS  =================================*/

extern AttractMode		attractFunc;
extern S16 				gThisCar;
extern S32				playitagain_time, whats_your_name_time;
extern Tourney			gTourney;
extern S32 mirror_flag;

/*===============================  GLOBALS  =================================*/

BOOL					gEnteringName;					/*	True if entering Name in High Score */
BOOL					continue_flag;
BOOL					continue_carry;
BLIT					*ScoreBlit, *EntryBlit;
SFONTINFO 				sfontX;

static S16				gThisRank;						/*	Current score to highlight. */
static S16				gThisTrack;						

static char				gCurName[NLENGTH+5];			/*	Current name being entered. */
static S16				gNameIndex;						/*	Place to put next char. */
static BLIT				*gCurNameBlit[NLENGTH];
static S16				gCurLetter;
static U32				gThisScore;
static S16				gLetterTries;

static BLIT 			*blitList[MAX_MSG_CHARS];
static S16				nextBlit;
static HiScore			gScoreTable[NTRACKS][NSCORES];
static BOOL				InThisGame[NTRACKS][NSCORES];
static U8				strictcnt;
static BOOL				left_side_active, right_side_active, holdoff_force;
static S16				char_on_left, char_on_right, last_section;
static S16				delta_x, old_delta_x;

static const char 		font_list[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!?.{}";


static MULTIBLIT const	ScoreMultiBlit[] = 
{	/* name			           x          y whtblrfz alpha animFunc        id */
	{"XLNTDRIVER1",            0, SCR_T-  2, MUNUSED, 0x00,AnimateTitle,    0}, 
	 {"XLNTDRIVER2",         256, SCR_T-  2, MUNUSED, 0x00,AnimateTitle,    1}, 
	{"TRACKNAME4",    SCR_MH-256, SCR_T+ 56, MUNUSED, 0x00,AnimateTName,    5}, 
	{"TRACKNAME4A",       SCR_MH, SCR_T+ 56, MUNUSED, 0x00,AnimateTName,    5}, 
	{"HSICON1",        SCR_L+275, SCR_T+ 70, MUNUSED, 0x00, AnimateIcon,    0}, 
	 {"HSICON1",       SCR_L+275, SCR_T+ 99, MUNUSED, 0x00, AnimateIcon,    1}, 
	 {"HSICON1",       SCR_L+275, SCR_T+126, MUNUSED, 0x00, AnimateIcon,    2}, 
	 {"HSICON1",       SCR_L+275, SCR_T+155, MUNUSED, 0x00, AnimateIcon,    3}, 
	 {"HSICON1",       SCR_L+275, SCR_T+184, MUNUSED, 0x00, AnimateIcon,    4}, 
	 {"HSICON1",       SCR_L+275, SCR_T+213, MUNUSED, 0x00, AnimateIcon,    5}, 
	 {"HSICON1",       SCR_L+275, SCR_T+242, MUNUSED, 0x00, AnimateIcon,    6}, 
	 {"HSICON1",       SCR_L+275, SCR_T+271, MUNUSED, 0x00, AnimateIcon,    7}, 
	 {"HSICON1",       SCR_L+275, SCR_T+300, MUNUSED, 0x00, AnimateIcon,    8}, 
	 {"HSICON1",       SCR_L+275, SCR_T+329, MUNUSED, 0x00, AnimateIcon,    9}, 
	{"DIGIT8",         SCR_L+ 10, SCR_T+ 70, MUNUSED, 0x00,AnimatePName, 0x81}, 
 	 {"DIGIT8",        SCR_L+ 10, SCR_T+ 99, MUNUSED, 0x00,AnimatePName, 0x82}, 
 	 {"DIGIT8",        SCR_L+ 10, SCR_T+126, MUNUSED, 0x00,AnimatePName, 0x83}, 
 	 {"DIGIT8",        SCR_L+ 10, SCR_T+155, MUNUSED, 0x00,AnimatePName, 0x84}, 
 	 {"DIGIT8",        SCR_L+ 10, SCR_T+184, MUNUSED, 0x00,AnimatePName, 0x85}, 
 	 {"DIGIT8",        SCR_L+ 10, SCR_T+213, MUNUSED, 0x00,AnimatePName, 0x86}, 
 	 {"DIGIT8",        SCR_L+ 10, SCR_T+242, MUNUSED, 0x00,AnimatePName, 0x87}, 
 	 {"DIGIT8",        SCR_L+ 10, SCR_T+271, MUNUSED, 0x00,AnimatePName, 0x88}, 
 	 {"DIGIT8",        SCR_L+ 10, SCR_T+300, MUNUSED, 0x00,AnimatePName, 0x89}, 
 	 {"DIGIT8",        SCR_L-  3, SCR_T+329, MUNUSED, 0x00,AnimatePName, 0x8B}, 
 	 {"DIGIT8",        SCR_L+ 17, SCR_T+329, MUNUSED, 0x00,AnimatePName, 0x8A}, 	
};

static MULTIBLIT const	EntryMultiBlit[] = 
{	/* name			           x          y whtblrfz alpha animFunc        id */
	{"XLNTDRIVER1",            0, SCR_T-  2, MUNUSED, 0x00,AnimateTitle,    0}, 
	 {"XLNTDRIVER2",         256, SCR_T-  2, MUNUSED, 0x00,AnimateTitle,    1}, 
	{"TRACKNAME4",    SCR_MH-256, SCR_T+ 60, MUNUSED, 0x00,AnimateTName,    5}, 
	{"TRACKNAME4A",      SCR_MH, SCR_T+ 60, MUNUSED, 0x00,AnimateTName,    5}, 
	{"PANEL02",                0,    22+256, MUNUSED, 0x00,AnimatePanel,    0},
	{"PANEL03",              256,    22+256, MUNUSED, 0x00,AnimatePanel,    0},
	{"HSICON1",              400, SCR_T+ 70, MUNUSED, 0x00, AnimateIcon, 0x10},
	{"HUDARROW",      SCR_MH-180,SCR_MV+ 20, MUNUSED, 0x00,   AnimateFF, 0x01},
	{"HUDARROW",      SCR_MH-160,SCR_MV+ 20, MUNUSED, 0x00,   AnimateFF, 0x02},
	{"HUDARROW",      SCR_MH+160,SCR_MV+ 20, MUNUSED, 0x00,   AnimateFF, 0x04},
	{"HUDARROW",      SCR_MH+180,SCR_MV+ 20, MUNUSED, 0x00,   AnimateFF, 0x03},
	HELP_DISPLAY(2)
	CREDITS_DISPLAY
};


/* ----------------------------------------------------------------------------- *
 * Called every frame to update the current score.
 *	Id	= (0x80 & current game score) | (0x40 & enter from left)
 * ----------------------------------------------------------------------------- */
static int AnimateScore(BLIT *blt)
{
	S32 time = GetCountdownTime();
	U32	alpha = ((time>>1) & 0xFF);

	if (blt->AnimID & 0x80)
		{
		if (!(time & 0x200))
			alpha = 255 - alpha;

		blt->Alpha = alpha;
		UpdateBlit(blt);
		}

	 AnimateRandom(blt);

	return 1;
}	/* end AnimateScore */

/* ----------------------------------------------------------------------- *
 * Reverses a string
 * ----------------------------------------------------------------------- */
void revcpy(char *dest, char *src)
{
    int i, len;

    len = strlen(src);
    for (i = 0; i < len; i++)
        dest[len - 1 - i] = src[i];

    dest[len] = '\0';
}

/* ----------------------------------------------------------------------------- *
 * Displays or removes the High Score screen.
 * ----------------------------------------------------------------------------- */
void ShowHiScore(BOOL show, S16 track)
{
	char 	digits[8], str[20];
	S32		i, j, start, id;
	BOOL	thisScore;

	/* If Over Select doesn't exist yet, create it. */
	if (!ScoreBlit && show) 
		{
		trackno   = track;
		CleanupAfterGame();
		if (!play_demogame(show,false,trackno, true))
			return;
		if (!CheckModelType(MT_HISCORE))
			return;
		SetFog(FOG_TRACK_TINT, 120, 0);
		InitDigitList();
		ScoreBlit = NewMultiBlit(0,0,ScoreMultiBlit,NBLITS(ScoreMultiBlit));
		InitMsgs(3, true);

		/*	Make the name list. */
		for (i=0; i<NNAMES; ++i)
		{
			thisScore = (i == gThisRank && track == gThisTrack);
			id = i | (thisScore ? 0x80 : 0);
			if (thisScore)
			{
				sfontX.scale_x = sfontX.scale_y = 1.25;
				InitMsgs(4,false);
			}
			else
			{
				InitMsgs((InThisGame[track][i]) ? 5:1, false);
			}

		    if (gScoreTable[track][i].mirror) 
		    {
			    sfontX.scale_x = -sfontX.scale_x;
			    revcpy(str, gScoreTable[track][i].name);
            }
            else
            {
			    strcpy(str, gScoreTable[track][i].name);
            }
			AddMsg( HI_SCORE_X,80+i*29, str, id, AnimateScore,-1 );

			cvt_time_str( gScoreTable[track][i].score, str, 'h' );
			AddMsg( HI_SCORE_X+135,80+i*29,str,id,AnimateScore,-1 );
			sfontX.scale_x = sfontX.scale_y = 1.0;
		}

		/*	Make the scrolling list. First determine where list should start. */
		if (track != gThisTrack || gThisRank <= 10+SSCORES)
			start = NNAMES;
		else if (gThisRank == 100)
			start = NSCORES - SSCORES;
		else if (gThisRank > NSCORES - SSCORES)
			start = NSCORES - SSCORES - 1;
		else
			start = gThisRank - (SSCORES / 2);
			
		for (i=start; i<start+SSCORES; ++i)
			{
			thisScore = (i == gThisRank && track == gThisTrack);
			id = (i-start) | (thisScore ? 0xc0 : 0x40);
			InitMsgs((!InThisGame[track][i]) ? 1 : (thisScore ? 4 : 5), false );
			if (i < NSCORES-1)
				{
				sprintf( str, "%2ld",(long)(i+1));
				AddMsg( LO_SCORE_X,98+(i-start)*19,str,id,AnimateScore,-1 );
				cvt_time_str( gScoreTable[track][i].score, str, 'h' );
				}
			else
				{
				AddMsg( LO_SCORE_X,98+(i-start)*19,"??",id,AnimateScore,-1  );
				cvt_time_str( gThisScore, str, 'h' );
				}
			AddMsg( LO_SCORE_X+45,98+(i-start)*19,str,id,AnimateScore,-1  );
			}


		/*	Handle continue options. */
		if (!TourneyOn() && continue_opts && continue_flag && cn_price() != 0)
			{
			AddJoinNow(ScoreBlit, 3, false);
			AddMsg( -256, 140, "CONTINUE", 0,AnimateBlink,0 );
			}
		else
			AddJoinNow(ScoreBlit, 1, false);
		}

	if (!show && ScoreBlit)
		{
		RemoveMsgs();

		/*	Remove the screen. */
		RemoveBlit( ScoreBlit );
		ScoreBlit = nil;
		continue_carry = continue_flag;
		continue_flag = 0;
		play_demogame(false,false, trackno, false);
		}

}	/* end ShowHiScore */


/* ----------------------------------------------------------------------------- *
 * Called at high score entry startup to crop the panels to the right size.
 * ----------------------------------------------------------------------------- */
static int	AnimatePanel(BLIT *blt)
{
	CropBlit( blt, 0,22, blt->Info->Width, blt->Info->Height );
	blt->AnimFunc = nil;
	return 1;
}	/* end AnimatePanel */


/* ----------------------------------------------------------------------------- *
 * Called every frame to update the Track Name in the high Score display.
 * ----------------------------------------------------------------------------- */
static int	AnimateTName(BLIT *blt)
{
#if 0
	if (language == LANG_JAPANESE)
		{
		RenameBlit( blt, "JTRACKNAME", true );
		SelectBlit( blt, trackno, 256,21 );
		}
	else
#endif
		SelectBlit( blt, trackno, 256,16 );
	blt->AnimFunc = nil;
	return 1;
}	/* end AnimateTName */


/* ----------------------------------------------------------------------------- *
 * Called every frame to update the Place Numbers in the High Score display.
 * ----------------------------------------------------------------------------- */
static int	AnimatePName(BLIT *blt)
{
	UpdateNumber( blt, (char)((blt->AnimID & 0xF) % 10) );
	blt->AnimFunc = nil;
	return 1;
}	/* end AnimatePName */


/* ----------------------------------------------------------------------------- *
 * Called every frame to update the Car icons in the High Score display.
 * ----------------------------------------------------------------------------- */
static int	AnimateIcon(BLIT *blt)
{
	/*	High score entry mode. */
	if (blt->AnimID & 0x10)
		{
		updateList2( blt, gThisCar, NCARS, 
					 "HSICON1", "HSICON2", "HSICON3", "HSICON4",
					 "HSICON5", "HSICON6", "HSICON7", "HSICON8",
					 "HSICON9", "HSICON10", "HSICON11", "HSICON12" );
		blt->Width 		= blt->Info->Width * 2;
		blt->Height 	= blt->Info->Height * 2;
		blt->AnimFunc	= nil;
		UpdateBlit(blt);
		return 1;
		}

	/*	High score display mode. */
	if (!(blt->AnimID & 0x20))
		{
		if (gScoreTable[trackno][blt->AnimID & 0xf].mirror) 
            blt->Flip = TRUE;
        else
            blt->Flip = FALSE;

		updateList2( blt, gScoreTable[trackno][blt->AnimID & 0xf].car & 0xf, NCARS, 
					 "HSICON1", "HSICON2", "HSICON3", "HSICON4",
					 "HSICON5", "HSICON6", "HSICON7", "HSICON8",
					 "HSICON9", "HSICON10", "HSICON11", "HSICON12" );
		blt->AnimID |= 0x20;
		}
	else
		AnimateRandom(blt);
	return 1;
}	/* end AnimateIcon */


/* ----------------------------------------------------------------------------- *
 * cvt_time_str - converts a long int into a time format MM:SS.hh or H:MM:SS.hh
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
U8 cvt_time_str( S32 t, U8 *dest, char format )
{
    S32	sec, min, digit = 0;
	U8	ret = 0;

    if (t < 0)
		t = 0;
	
    switch (format)
        {
    case 'c':
    	sec = t;
		if (sec > 999)					/* Maximum counter of 999 */
		    sec = 999;
		dest[0] = (U8)((sec / 100) % 10) + '0';
		dest[1] = (U8)((sec /  10) % 10) + '0';
		dest[2] = (U8)(        sec % 10) + '0';
		ret		= 3;
		break;
			
    case 'f':
		if (t >= MAX_HSCORE)
			{
			strcpy( dest, "99:59.99" );
			return 8;
			}
		dest[5] = '.';
		dest[6] = (U8)((t / 100) % 10) + '0';
		dest[7] = (U8)((t /  10) % 10) + '0';
		dest[ret=8] = 0;
		sec 	= t / 1000;
		min		= sec / 60;
        sec	   %= 60;
		dest[0]	= (U8)(min / 10) % 10 + '0';
		dest[1]	= (U8)(min % 10) + '0';
		dest[2] = ':';
		dest[3]	= (U8)(sec / 10) + '0';
		dest[4]	= (U8)(sec % 10) + '0';
		break;

    case 'h':
		if (t >= MAX_SCORE)
			{
			strcpy( dest, "9:99.99" );
			return 7;
			}
		dest[4] = '.';
		dest[5] = (U8)((t / 100) % 10) + '0';
		dest[6] = (U8)((t /  10) % 10) + '0';
		dest[ret=7] = 0;
		/* no break; fall through to do seconds */
		
    case 's':				
		sec 	= t / 1000;
		min		= (sec / 60) % 60;
        sec	   %= 60;
		dest[0]	= (U8)(min % 10) + '0';
		dest[1] = ':';
		dest[2]	= (U8)(sec / 10) + '0';
		dest[3]	= (U8)(sec % 10) + '0';
		if (!ret)
			dest[ret = 4] = 0;
		break;
		}

	return ret;
}	/* end cvt_time_str */


/* ----------------------------------------------------------------------------- *
 *	Clears out old players from high score table and resets for a new game.
 * ----------------------------------------------------------------------------- */
void InitGameScores( void )
{
	S32	i,j;

	for (j=0; j<NTRACKS; ++j)
		for (i=0; i<NSCORES; ++i)
			InThisGame[j][i] = false;
	gThisRank = -1;
	gThisTrack = -1;
}	/* InitGameScores */


/* ----------------------------------------------------------------------------- *
 *	Returns the rank a given score deserves on a given track.
 * ----------------------------------------------------------------------------- */
S16 HiScoreRank( U32 score, S16 track )
{
	S16	i;

	if (score == 0)
		return -1;

	for (i=0; i<NSCORES; ++i)
		if (gScoreTable[track][i].score >= score)
			return i;
	return -1;

}	/* HiScoreRank */

/* ----------------------------------------------------------------------------- *
 *	Writes the entire High Score Table to NVRAM.
 * ----------------------------------------------------------------------------- */
static void WriteTable( S16 t )
{
	U8	*src	= (U8 *)gScoreTable[t];
	U32	*tbl	= TABLE_BASE + TABLE_SIZE * t;
	U32	i;
	struct hst_ent hent;

	for (i=0; i < TABLE_SIZE; ++i)
		eer_write_byte( (U8 *)tbl++, (U8 *)BRAM_UNLK, 0xff & *src++ );		
	/* Write hi-score to the GUTS area, too */
	for (i = 0; i < 10; i++) {
	    hent.score = gScoreTable[t][i].score;
	    cvt_time_str(hent.score, hent.initials, 'h');
	    eer_hstw(i, &hent, HST_TRACK1_TOP + t);
	}

}	/* end WriteTable */


/* ----------------------------------------------------------------------------- *
 *	Clears a high score entry to default values.
 * ----------------------------------------------------------------------------- */
static void ClearAScore( HiScore *s, const char *name )
{
	s->score		= MAX_SCORE;
	strcpy( s->name, name);
	s->deaths		= 0;
	s->mirror		= 0;
	s->car			= gThisNode << 4;
}	/*	end ClearAScore */


/* ----------------------------------------------------------------------------- *
 *	Clears the high score table to default values.
 * ----------------------------------------------------------------------------- */
void ClearHighScores(void)
{
	S16 i, trk;
	const char * const	defNames[32] = 
	{ 	"CAPIZZI", "GIZMO", "SPOO", "ROBMAN", "GUMMER", "RAY", 
		"ALAN", "JOHN", "AARON", "SKRID", "SPENCE", "TOM", "ROB", "PETE", 
		"FORREST", "DAVE", "PAUL", "JRG", "JAM", "MR.PIXEL", "BINKLEY", 
		"FRENZY", "KAYA", "KENNA", "DECAL", "LATENITE", "ALIEN",
		"SCHRODER", "MAGGIE", "MARGE", "HOMER", "ZIGGY" 
	};

	/*	Initialize the HiScore times with High values so real entries will
	 *	force the fake entries out. */
	for (trk = 0; trk < NTRACKS; ++trk)
		{
		for (i=0; i<NSCORES; ++i)
			ClearAScore( &gScoreTable[trk][i], defNames[RandInt(32)] );
		WriteTable(trk);
		}

	InitGameScores();

	/*	Reset count of games since reset. */
	eer_puts( EER_GAME_CNT,0 );
	eer_puts( EER_HI_CNT,0 );

}	/*	end ClearHighScores */


/* ----------------------------------------------------------------------------- *
 *	Loads the high score table from NVRAM at game startup.
 * ----------------------------------------------------------------------------- */
void LoadHighScores(void)
{
	U8		*src	= (U8 *)gScoreTable;
	U32		*tbl;
	U32		trk, i;
	HiScore	*hs;

	/*	See if the scores should be auto-cleared. */
	if ((game_opt & GO_AUTOCLEARHS) &&              
		eer_gets(EER_GAME_CNT) >= MAX_GAME_CNT && eer_gets(EER_HI_CNT) >= MAX_HI_CNT)
		{
		ClearHighScores();
		return;
		}

		/*	Filter out bad characters. */
	for (trk=0; trk<NTRACKS; ++trk)
		{
		src	= (U8 *)gScoreTable[trk];
		tbl	= TABLE_BASE + TABLE_SIZE * trk;

		/*	Load the tables from the nvram. */
		for (i=0; i < TABLE_SIZE; ++i)
			*src++ = (U8)(*tbl++ & 0xFF);

		/*	Remove any bad looking scores in case BRAM is flaky. */
		for (i=0; i<NSCORES; ++i)
			{
			hs = &gScoreTable[trk][i];
			if (hs->score < MIN_SCORE || hs->score > MAX_SCORE ||
				(i < NSCORES - 1 && hs->score > (hs+1)->score))
				{
				memmove( (void *)hs, (void *)(hs+1), sizeof(HiScore)*(NSCORES-1-i) );
				ClearAScore( &gScoreTable[trk][NSCORES-1], "DEFAULT" );
				--i;
				}
			else
				FilterBadChars( hs->name );
			checkword( hs->name );
			}
		}

	InitGameScores();

}	/*	end LoadHighScores */


/* ----------------------------------------------------------------------------- *
 *	Saves a given high score table entry.
 * ----------------------------------------------------------------------------- */
S32 SaveHighScore(char *name, U32 score, U32 track, U32 deaths, U32 mirror, U32 car, 
				  U32	flags)
{ 
	S32		rank = HiScoreRank( score, (S16)track );
	HiScore	*hs;
	S32		i,j;

	if (rank < 0)	
		return -1;

	/*	See if score already exists in table, and reject it if so. */
	hs = &gScoreTable[track][rank];
	for (i = rank; i < NSCORES; ++i, ++hs)	
		if (hs->score != score)
			break;
		else if (hs->mirror == mirror && hs->deaths == deaths && hs->car == car && 
				 !strncmp(hs->name, name, NLENGTH))
			return -1;

	/*	Check the score for illegal chars. */
	FilterBadChars( name );

	/*	Score is acceptable, enter it into the score table.  First, move other scores out of the way. */
	hs = &gScoreTable[track][rank];
	if (rank < NSCORES-1)
		{
		memmove( (void *)&gScoreTable[track][rank+1], (void *)hs, 
				 sizeof(HiScore)*(NSCORES-1-rank) );
		memmove( (void *)&InThisGame[track][rank+1], (void *)&InThisGame[track][rank], 
				 sizeof(BOOL)*(NSCORES-1-rank) );
		if (track == gThisTrack && rank < gThisRank)
			++gThisRank;
		}

	/*	Censor out evil words. */
	checkword( name );

	/*	Now insert the fresh entry. */
	strncpy( hs->name, name, NLENGTH-1);
	hs->name[NLENGTH-1] 	= 0;
	hs->score				= score;
	hs->deaths				= deaths;
	hs->mirror				= mirror;
	hs->car					= car;
	if (!(flags & 2))
		InThisGame[track][rank] = true;

	WriteTable(track);
	return rank;

}	/*	end SaveHighScore */


/* ----------------------------------------------------------------------------- *
 *	Takes a score and enters it into the high score table in the correct place.
 * ----------------------------------------------------------------------------- */
void EnterHighScore( S16 track, U32 score, char *name, U32 deaths, U32 mirror, U32 car )
{
	S32	rank;

	/*	Assume we're not entering a name. */
	gEnteringName	= false;
	gstate			= ATTRACT;
	continue_flag   = !demo_game;

	/*	If a real game, go to correct high score table. */
	if (!demo_game)
		{
		switch (track)
			{
			case 0:	attractFunc = ATR_HISCORE1-1; break;
			case 1:	attractFunc = ATR_HISCORE2-1; break;
			case 2:	attractFunc = ATR_HISCORE3-1; break;
			case 3:	attractFunc = ATR_HISCORE4-1; break;
			case 4:	attractFunc = ATR_HISCORE5-1; break;
			case 5:	attractFunc = ATR_HISCORE6-1; break;
			case 6:	attractFunc = ATR_HISCORE7-1; break;
			default:	attractFunc = ATR_HISCORE4-1; break;
			}
		SetCountdownTimer(0);
		}

	if (!game_car[gThisNode].place_locked || gIgnoreScore || demo_game)
		return;

/*---------------------------------------------------------------------------*/

	/* can't enter hiscores for most tourney cases */
	if (TourneyOn() &&
		( (gTourney.laps > 0 && track_len_save[trackno] != gTourney.laps) ||
		  gTourney.ext_start == JOIN_QUICK))
		{
		/* if we did finish, go from swirling to 'play it again' */
		if (game_car[gThisNode].place_locked)
			{
			SOUND (S_STOP_ALL_SNDS);
			SOUND (S_PLAYITAGAIN);
			playitagain_time = IRQTIME;
			whats_your_name_time = 0;
			}

		return;
		}

/*---------------------------------------------------------------------------*/

	/*	Only get here after place is finally locked. */
	SOUND (S_STOP_ALL_SNDS);

	/*	Count this game in the stats. */
	if ((U16)eer_incs(EER_GAME_CNT,1) > MAX_GAME_CNT)
		eer_puts(EER_GAME_CNT,MAX_GAME_CNT);

	/*	Also bump up the link counter */
	if (!solo_flag)
	    eer_incs(EER_LINKED,1);

	rank = HiScoreRank( score, track );

	/*	Don't do anything unless the score belongs on a table. */
	if (score == 0 || score >= MAX_SCORE)
		{
		SOUND (S_PLAYITAGAIN);
		playitagain_time = IRQTIME;
		return;
		}

/*---------------------------------------------------------------------------*/

	/*	For high score entry display purposes. */
	gThisRank		= rank;
	gThisTrack		= track;
	gThisScore		= score;
	if (rank < 0)
		{
		gThisRank 	= 100;

		SOUND (S_PLAYITAGAIN);
		playitagain_time = IRQTIME;

		/*	Count this as a failure game. */
		if( eer_incs(EER_HI_CNT,1) > MAX_HI_CNT )
			eer_puts(EER_HI_CNT,MAX_HI_CNT);
		return;
		}

/*---------------------------------------------------------------------------*/

	/*	Top ten name? */
	if (rank < NNAMES)
		{
		/*	Setup for entry of name. attract() will take over next time thru. */
		holdoff_force	= true;
		gEnteringName	= true;
		attractFunc 	= ATR_HSSETUP;
		SOUND (S_WHATSYERNAME);
		}

	/*	Only report score if it doesn't go into top ten; those are handled in GetHighScoreName */
	else if (rank >= NNAMES)
		{
		strcpy( gScoreTable[track][rank].name, "GIZMO" );
		SaveHighScore( "GIZMO", score, track, deaths, mirror, car|(gThisNode << 4), 0L );
		SOUND (S_PLAYITAGAIN);
		playitagain_time = IRQTIME;

		/*	Count this as a failure game. */
		if( eer_incs(EER_HI_CNT,1) > MAX_HI_CNT )
			eer_puts(EER_HI_CNT,MAX_HI_CNT);
		}

}	/* end EnterHighScore */


/* ----------------------------------------------------------------------------- *
 *	Handles steering wheel force while entering high score name.
 * ----------------------------------------------------------------------------- */
void HiScoreForce(void)
{
	if (!holdoff_force)
		OverrideForce (NUM_HENTRY_POS, 40, 5, 2, 1);
}	/*	end HiScoreForce */


/* ----------------------------------------------------------------------------- *
 * Displays or removes the high score entry screen.
 * ----------------------------------------------------------------------------- */
void ShowScoreEntry(BOOL show)
{
	char 		digits[8], str[20];
	S32			i, j;

	/* If screen doesn't exist yet, create it. */
	if (!EntryBlit && show) 
		{
		if (!play_demogame(show,false,trackno,false))
			return;
		if (!CheckModelType(MT_HISCORE))
			return;

		/*	Reset the auto H.S. table clearing counter. */
		eer_puts( EER_HI_CNT,0 );
		SetFog(FOG_TRACK_TINT, 150, 0);
		gCurName[0]	= 0;
		gNameIndex	= 0;
		gLetterTries= 6;
		gCurLetter  = 'M' - 'A';
		char_on_left = gCurLetter - (NUM_HENTRY_POS-4-1) / 2;
		char_on_right = gCurLetter + (NUM_HENTRY_POS-4-1) / 2;
		last_section = (NUM_HENTRY_POS-4-1) / 2 + 1;
		old_delta_x = 0;

		SetGasReleased(true);

		/*	Create the fixed elements & stretch the background to cover the screen */
		EntryBlit = NewMultiBlit(0,0,EntryMultiBlit,NBLITS(EntryMultiBlit));

		/*	Make the name list. */
		InitMsgs(1, true);
		sfontX.scale_x 	= sfontX.scale_y = 2.0;
		
		/*	Make a score string. */
		cvt_time_str( gThisScore, str, 'h' );
		AddMsg( 170,90,str, 0, nil,0 );

		/*	Make a rank string. */
		sprintf( str,"%3d",gThisRank+1);
		AddMsg( 30,90,str, 0, nil,0 );

		/*	Create the alphabet. */
		str[1]	= 0;
		for (i=0; font_list[i]; ++i)
			{
			str[0] 			= font_list[i];
			AddMsg( SCR_MH + (i - gCurLetter)*HSCORE_LWIDTH,SCR_MV+23,str, i, AnimateLetter, font_list[i] );
			}
		}
	sfontX.scale_x 	= sfontX.scale_y = 1.0;

	if (!show && EntryBlit)
		{
		/*	Remove the strings. */
		RemoveMsgs();

		/*	Remove the screen. */
		RemoveBlit( EntryBlit );
		EntryBlit = nil;
		play_demogame(false,false,trackno,false);

		SOUND (S_WHATSTAG);
		whats_your_name_time = IRQTIME;
		}

}	/* end ShowScoreEntry */


/* ----------------------------------------------------------------------------- *
 * Called every frame to accept entry of a high score name.
 * ----------------------------------------------------------------------------- */
void GetHighScoreName(void)
{
	S16			i, sections = sizeof(font_list)-1;
	S32			this_section, cur_time;
	F32			wheel_pos, sect_size;
	static S32	last_time, time_to_wait;

	/*	Determine the currently selected letter. */
	this_section = WheelSection (NUM_HENTRY_POS,1) - 1;

	if (this_section < 0)
		this_section = 0;
	else if (this_section > NUM_HENTRY_POS-3)
		this_section = NUM_HENTRY_POS-3;

	holdoff_force = false;
	cur_time = IRQTIME;

	left_side_active = this_section == 0;
	right_side_active = this_section == NUM_HENTRY_POS-3;

	/* if new wheel section, update current letter and in time */
	if (this_section != last_section)
		{
		/* if not on an arrow, update current letter */
		if (left_side_active || last_section==NUM_HENTRY_POS-3)
			gCurLetter += this_section - last_section + 1;
		else if (right_side_active || last_section==0)
			gCurLetter += this_section - last_section - 1;
		else
			gCurLetter += this_section - last_section;

		last_time = cur_time;
		time_to_wait = ONE_SEC/20;
		}
	/* handle if on an arrow */
	else if (left_side_active | right_side_active)
		{
		if (abs (cur_time - last_time) > time_to_wait)
			{
			wheel_pos = PotWheelPos();
			sect_size = (F32)RAW_WHEEL_RANGE * 2 / NUM_HENTRY_POS;

			if (left_side_active)
				{
				gCurLetter--;
				wheel_pos = (wheel_pos + RAW_WHEEL_RANGE) / sect_size;
				}
			else
				{
				gCurLetter++;
				wheel_pos = (RAW_WHEEL_RANGE - wheel_pos) / sect_size;
				}

			/* wait time based on wheel pos */
			time_to_wait = (F32)ONE_SEC * wheel_pos * .5;

			if (time_to_wait < ONE_SEC / 20)
				time_to_wait = ONE_SEC / 20;

			last_time = cur_time;
			}
		}

	last_section = this_section;

	if (gCurLetter < 0)
		gCurLetter = 0;
	if (gCurLetter > sections-1)
		gCurLetter = sections-1;

	/*	If entered all letters, force to END (or DEL). */
	if (strlen(gCurName) == NLENGTH - 1)
		{
		last_section = WheelSection(2,1) + NUM_HENTRY_POS-5;

		gCurLetter = sections - 2 + last_section - (NUM_HENTRY_POS-5);
		left_side_active = false;
		right_side_active = false;

		/* ensure that END shows up */
		if (char_on_right == sections-2)
			{
			char_on_left++;
			char_on_right++;
			}
		}

	/* adjust bounds for current letter */
	if (gCurLetter < char_on_left)
		{
		char_on_left--;
		char_on_right--;
		}
	else if (gCurLetter > char_on_right)
		{
		char_on_left++;
		char_on_right++;
		}

	/* deactivate arrows if on a bound */
	if (char_on_left == 0)
		left_side_active = false;
	else if (char_on_right == sections-1)
		right_side_active = false;

	/*	See if the pedal has selected a character. */
	if (TimeOut() || ((GasPressed(true) || (edges & SW_ABORT)) && !(left_side_active | right_side_active)))
		{
		/*	Make the name list. */
		InitSFontInfo(&sfontX, &font1info);
		sfontX.color = 0x7E1F;

		for (i=0; gCurName[i]; ++i)
			{
			RemoveBlit( gCurNameBlit[i] );
			gCurNameBlit[i] = nil;
			}

		/*	Check for done. */
		if (font_list[gCurLetter] == '}' || TimeOut() || (edges & SW_ABORT))
			{
			gEnteringName	= false;
			switch (gThisTrack)
				{
				case 0:	attractFunc = ATR_HISCORE1-1; break;
				case 1:	attractFunc = ATR_HISCORE2-1; break;
				case 2:	attractFunc = ATR_HISCORE3-1; break;
				case 3:	attractFunc = ATR_HISCORE4-1; break;
				case 4:	attractFunc = ATR_HISCORE5-1; break;
				case 5:	attractFunc = ATR_HISCORE6-1; break;
				case 6:	attractFunc = ATR_HISCORE7-1; break;
				default:	attractFunc = ATR_HISCORE4-1; break;
				}
			SetCountdownTimer(0);
			SetGasReleased(false);

			SaveHighScore( gCurName, gThisScore, gThisTrack, 0, mirror_flag, 
						   gThisCar|(gThisNode << 4), 0L );
			gCurName[0] 	= 0;
			gNameIndex		= 0;
			return;
			}

		/*	Check for delete. */
		if (font_list[gCurLetter] == '{')
			{
			if (gNameIndex > 0)
				--gNameIndex;
			}

		/*	Grab this character and put it in the name. */
		else if (gNameIndex < NLENGTH-1)
			{
			gCurName[gNameIndex++] = font_list[gCurLetter];
			if (--gLetterTries < 0)
				gLetterTries = 0;
			}
		gCurName[gNameIndex] = 0;

		if (gLetterTries > 0)
			SetCountdownTimer( 15 * ONE_SEC );
		if (gCurName[0])
			{
			InitMsgs(4, false);
			sfontX.scale_x = sfontX.scale_y = 3.0;
			NewSFontBlitCenterPrint(gCurNameBlit,NLENGTH,&sfontX,SCR_MH,140,gCurName);
			sfontX.scale_x = sfontX.scale_y = 1.0;
			InitMsgs(1, false);
			}
		}

}	/* end GetHighScoreName */


/* ----------------------------------------------------------------------------- *
 * Called every frame to update the the current score.
 * ----------------------------------------------------------------------------- */
static int AnimateLetter(BLIT *blt)
{
	S16		offset	= blt->AnimID;
	BOOL	cur 	= blt->AnimID == gCurLetter && !(left_side_active | right_side_active);
	BOOL	init 	= (blt->AnimDTA & 0x200) == 0;
	BOOL	wasCur 	= (blt->AnimDTA & 0x100) != 0;
	S16		delta_delta_x;

	blt->Height	= blt->Info->Height / ((cur) ? 1 : 2);
	blt->Alpha 	= (cur && (IRQTIME & 0x80)) ? 0xFF : 0x00;

	/* initialize x coord on all letters */
	if (init)
		{
		delta_x = 0;
		blt->X = SCR_MH + HSCORE_LWIDTH * (offset - (char_on_right+char_on_left)/2);
		blt->AnimDTA |= 0x200;
		}
	/* do this once each time for all letters */
	else if (offset == 0)
		{
		/* get delta between where we are and where we need to go */
		delta_x = SCR_MH + HSCORE_LWIDTH * (0 - (char_on_right+char_on_left)/2) - blt->X;

		/* try to go half way there */
		if (delta_x < 0)
			{
			delta_x >>= 1;
			if (delta_x == 0)
				delta_x = -1;
			}
		else if (delta_x > 0)
			{
			delta_x >>= 1;
			if (delta_x == 0)
				delta_x = 1;
			}

		/* now check against last delta */
		delta_delta_x = old_delta_x - delta_x;

		/* adjust rate at which we can change */
		if (delta_delta_x < 0)
			{
			delta_delta_x >>= 1;
			if (delta_delta_x < -8)
				delta_delta_x = -8;
			else if (delta_delta_x == 0)
				delta_delta_x = -1;
			}
		else if (delta_delta_x > 0)
			{
			delta_delta_x >>= 1;
			if (delta_delta_x > 8)
				delta_delta_x = 8;
			else if (delta_delta_x == 0)
				delta_delta_x = 1;
			}

		/* get allowable change */
		delta_x = old_delta_x - delta_delta_x;

		old_delta_x = delta_x;
		}

	/* update x coord */
	blt->X += delta_x;

	/* don't display letters outside of selectable group */
	if (Hidden (blt, offset < char_on_left || offset > char_on_right))
		return 1;

	if (cur && (init || !wasCur))
		RenameBlit( blt, "FONT1YEL", 1);
	else if (wasCur && !cur)
		RenameBlit( blt, "FONT1", 1);

	blt->AnimDTA = 0x200 | (cur ? 0x100 : 0);
	UpdateBlit(blt);

	return 1;
}	/* end AnimateLetter */


/* ----------------------------------------------------------------------------- *
 * Called every frame to update the the current score.
 * ----------------------------------------------------------------------------- */
static int AnimateFF(BLIT *blt)
{
	S16		id	= blt->AnimID;

	/*	Make right side blits face right. */
	if (blt->AnimID <= 2)
		{
		blt->Flip = 1;
		UpdateBlit(blt);
		}

	/*	Flash them if they're active. */
	if (Hidden( blt, (IRQTIME & 0x100) && ((blt->AnimID <= 2 && left_side_active) || 
										   (blt->AnimID >= 3 && right_side_active))))
		return 1;

	/*  hide if all letters are shown on a boundary */
	if (Hidden( blt, ((char_on_left == 0 && blt->AnimID <= 2) ||
					  (char_on_right == sizeof(font_list)-2 && blt->AnimID >= 3))))
		return 1;

	/*	Make them enlarge if active. */
	if (blt->AnimID <= 2)
		if (left_side_active)
			{
			blt->Height = blt->Info->Height * 2;
			blt->Alpha = 0x00;
			UpdateBlit(blt);
			}
		else
			{
			blt->Height = blt->Info->Height;
			blt->Alpha = 0x80;
			UpdateBlit(blt);
			}
	else if (blt->AnimID >= 3)
		if (right_side_active)
			{
			blt->Height = blt->Info->Height * 2;
			blt->Alpha = 0x00;
			UpdateBlit(blt);
			}
		else
			{
			blt->Height = blt->Info->Height;
			blt->Alpha = 0x80;
			UpdateBlit(blt);
			}


	UpdateBlit(blt);

	return 1;
}	/* end AnimateFF */


/* ----------------------------------------------------------------------------- *
 * Called every frame to update the High score screen title.
 * ----------------------------------------------------------------------------- */
static int	AnimateTitle(BLIT *blt)
{
	static F32 vel;

	if (language == LANG_JAPANESE && !(blt->AnimID & 0x10))
		{
		char str[20];
		blt->AnimID |= 0x10;
		sprintf( str, "J%s", blt->Name );
		RenameBlit( blt, str, true );
		}

	/*	Scroll the title in from the side. AnimDTA is inited to -1 by NewMultiBlit */
	if (blt->AnimDTA < 0)
		{
		blt->AnimDTA = 512;
		vel = 1.0;
		}
	else if (blt->AnimDTA >= vel)
		{
		blt->AnimDTA -= vel;
		vel += 0.40;
		}
		
	blt->X = 16-blt->AnimDTA;
	if ((blt->AnimID & 0xF) == 1)
		blt->X += 242;

	UpdateBlit(blt);
	return 1;

}	/* end AnimateTitle */


/* ----------------------------------------------------------------------------- *
 * Called every frame to update the Horizontally scrolling msgs.
 *	expects ID = (0x100 & enter from right) | (0x400 & velocity controller).
 * ----------------------------------------------------------------------------- */
static int AnimateHScroll(BLIT *blt)
{	
	static F32 vel;
	static BOOL	control;
	S16		offset, pos;
	
	offset 	= (blt->AnimDTA >> 16) & 0xFFFF;
	pos   	= blt->AnimDTA & 0xFFFF;

	if (!control && offset < 0)
		{
		control = true;
		blt->AnimID |= 0x400;
		}
	else if (control && offset >= 0)
		control = false;

	/*	Scroll the title in from the left side. AnimDTA is inited to -1 by NewMultiBlit */
	if (offset < 0)
		{
		offset 	= 512;
		pos		= blt->X;
		if (blt->AnimID & 0x400)
			vel = 1.0;
		}
	else if (offset >= vel)
		{
		offset -= vel;
		if (blt->AnimID & 0x400)
			vel += 0.60;
		}

	if (!(blt->AnimID & 0x100))
		blt->X = pos+offset;
	else
		blt->X = pos-offset;

	blt->AnimDTA = (offset << 16) | (pos & 0xFFFF);
	UpdateBlit(blt);
	return 1;

}	/* end AnimateHScroll */


/* ----------------------------------------------------------------------------- *
 * Called every frame to update the Horizontally scrolling msgs.
 *	uses ID = (ox & 0xFFC00000) | (oy & 0x3FF000) | (0x200 & inited) | 
 *			  (0x100 & done) | (0x7FF & reserved).
 *	uses DTA= (dx & 0xFF800000) | (dy & 0x7FC000) | (steps & 0x3fff).
 * ----------------------------------------------------------------------------- */
static int AnimateRandom(BLIT *blt)
{	
	S32		ox, oy, dx, dy, step;
	F32		t;
	BOOL	inited, done;
	
	done	= (blt->AnimID & 0x100) != 0;
	if (done)
		return 1;

	inited	= (blt->AnimID & 0x200) != 0;
	if (!inited)
		{
		/*	Initialize start and end positions of this letter. */
		ox 			= Random(512.0 * 3.0) - 513;
		oy 			= Random(512.0 * 3.0) - 513;
		dx			= blt->X;
		dy			= blt->Y;
		step		= 0x3fff;
		if (ox < -511)
			ox = -511;
		if (oy < -511)
			oy = -511;
		blt->AnimID = ((ox & 0x7ff) << 21) | ((oy & 0x7ff) << 10) | 0x200 | (0x7ff & blt->AnimID);
		blt->AnimDTA= ((dx & 0x1ff) << 23) | ((dy & 0x1ff) << 14) | (step & 0x3fff);
		}

	/*	Unpack the data. */
	ox	= (blt->AnimID  >> 21) & 0x7ff;
	oy	= (blt->AnimID  >> 10) & 0x7ff;
	dx	= (blt->AnimDTA >> 23) & 0x1ff;
	dy	= (blt->AnimDTA >> 14) & 0x1ff;
	step= (blt->AnimDTA & 0x3fff);
	if (step == 0)
		blt->AnimID |= 0x100;

	t	= (float)step / (float)(0x3fff);
	step= (217 * step) / 256;

	/*	Restore sign of ox & oy */
	if (ox > 1023)
		ox = 1 - ox;
	if (oy > 1023)
		oy = 1 - oy;

	/*	Calculate the new X,Y position of the blit. */
	blt->X = dx + (ox - dx) * t;
	blt->Y = dy + (oy - dy) * t;

	/*	Now recompress the altered data. */
	blt->AnimDTA = (blt->AnimDTA & ~0x3fff) | (step & 0x3fff);
	UpdateBlit(blt);
	return 1;

}	/* end AnimateHScroll */


/* ----------------------------------------------------------------------------- */
void InitMsgs(S16 fontNum, BOOL reset)
{
	switch (fontNum)
		{
		case 1:	InitSFontInfo(&sfontX, &font1info);		break;
		case 3:	InitSFontInfo(&sfontX, &font3info);		break;
		case 4:	InitSFontInfo(&sfontX, &font1rinfo);	break;
		case 5:	InitSFontInfo(&sfontX, &font1yinfo);	break;
		}

	if (reset)
		nextBlit = 0;
}	/*	InitMsgs */


/* ----------------------------------------------------------------------------- */
void AddMsg( int x, int y, const char *txt, int AnimID, int	(* AnimFunc)(Blit *blit), S32 data )
{
	S16	i, len = strlen(txt);
	U8  flip = FALSE;

	if (sfontX.scale_x < 0.0) 
	{
	    sfontX.scale_x = -sfontX.scale_x;
	    flip = TRUE;
    }

	if (len < MAX_MSG_CHARS - nextBlit)
		{
		if (x <= -256 && y <= -256)
			len	= NewSFontBlitCenterPrint(&blitList[nextBlit],len,&sfontX,
										  SCR_MH,SCR_MV-sfontX.finfop->lheight/2,txt);
		else if (y <= -256)
			len	= NewSFontBlitCenterPrint(&blitList[nextBlit],len,&sfontX,
										  x,y-sfontX.finfop->lheight/2,txt);
		else if (x <= -256)
			len	= NewSFontBlitCenterPrint(&blitList[nextBlit],len,&sfontX,SCR_MH,y,txt);
		else
			len	= NewSFontBlitPrint(&blitList[nextBlit],len,&sfontX,x,y,txt);
		for (i=0; i<len; ++i)
			if (blitList[nextBlit+i])
				{
				blitList[nextBlit+i]->AnimID	= AnimID;
				blitList[nextBlit+i]->AnimFunc	= AnimFunc;
				blitList[nextBlit+i]->AnimDTA	= data;
				blitList[nextBlit+i]->Flip	    = flip;
				}
		nextBlit += len;
		}
}	/* end AddMsg */

/* ----------------------------------------------------------------------------- */
void RemoveMsgs(void)
{
	while (--nextBlit >= 0)
		if (blitList[nextBlit])
			RemoveBlit( blitList[nextBlit] );

	nextBlit = 0;
}	/*	end RemoveMsgs */


/* ----------------------------------------------------------------------------- */
static BOOL checkword( char *wp )
{
	char 				**op,retval;
	const char * const	obscenities[] = 
	{
		"ASSHOLE", "ASSWHOLE", "FUCK", "PHUK", "PHUCK", "FUK", "SHIT", "CUM", 
		"PISS", "CUNT", "PUSSY", "TWAT", "COCK", "NIGGER", "NIGGA", "FAGGOT", 
		"FAG", "CHINK", "GOOK", "BEANER", "WETBACK", "" 
	};

	if ( !(game_opt & GO_CENSOR) )	/* Option turned off	*/
		return 0;					/* just return	*/

	strictcnt	= 0;
	retval 		= 0;
	for(op = (char **)obscenities;**op;++op)
		retval |= fixword(*op,wp);
	return (BOOL)retval;
}	/* end checkword */


/* ----------------------------------------------------------------------------- */
static BOOL fixword(char *op, char *wp)
{
	static char *op1,*sp;
	static S16	cnt;

	op1 = op;
	while (1)
		{	
		/* See if first char is there */
		if(!*wp)
			return 0;
		if(*wp == *op)
			break;
		wp++;
		}

	/* Save position of first match */
	sp = wp;	

	/* allow repeats of the first character of our obscene word */
	while(*wp == *op)	
		wp++;
	
	/* next ignore first char repeats in our obscene word */
	while(*op && (op[0] == op[1]))
		op++;

	while(1)
		{
		/* Spaces don't count in our words */
		while(*++op == ' ');

		if(!*op)
			break;

		for(cnt=strictcnt;1;wp++,cnt--)
			{
			/* Spaces don't count in player's word either */
			while(*wp == ' ')
				wp++;

			if(*wp == *op)
				break;
	
			if((!*wp) || !cnt)
				return fixword(op1,sp+1); 
			}
		do
			{
			wp++;
			}
		while((*wp == *op) && (op[0] != op[1]));
		}

	/* Change offending letters */
	while(sp < wp)	
		*sp++ = '!';

	fixword(op1,wp);
	return 1;
}	/* end fixword */


/* ----------------------------------------------------------------------------- *
 * Removes illegal chars from the input string.
 * ----------------------------------------------------------------------------- */
static void FilterBadChars( char *str )
{
	S32	i,j;
	extern void FatalErrorf(S16 errid, char const *fmt, ...);

	for (i=0; i<NLENGTH && str[i]; ++i)
		{
		for (j=0; j<font1info.total; ++j)
			if (str[i] == font1[j].ch && !strchr(" {}",font1[j].ch) )
				break;

		if (j == font1info.total)
			{
#if CATCH_BAD_NAMES
			FatalErrorf( "Bad character '%c' (0x%02lx) in str[%ld] detected", 
						 str[i], (long)str[i], (long)i);
#else
			MSG( "Bad character '%c' (0x%02lx) in str[%ld] detected", 
				 str[i], (long)str[i], (long)i);
#endif
			str[i] = ' ';
			}
		}
}	/* end FilterBadChars */


/* ----------------------------------------------------------------------------- *
 * Returns true if tables should be auto-cleared.
 * ----------------------------------------------------------------------------- */
BOOL check_ctrs()
{
	return (game_opt & GO_AUTOCLEARHS)                 /*     Auto-HST-Reset Option is set */
		&& (eer_gets(EER_GAME_CNT) >= MAX_GAME_CNT)    /* AND Conditions have been met */
		&& (eer_gets(EER_HI_CNT) >= MAX_HI_CNT);
}
