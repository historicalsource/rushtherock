/*
 *	tourney.c -- Tournament mode functions
 *
 *		Copyright 1996 Time Warner Interactive.
 *	Unauthorized reproduction, adaptation, distribution, performance or 
 *	display of this computer program or the associated audiovisual work
 *	is strictly prohibited.
 *
 *	======================================================================
 *	$Author: grjost $						$Date $
 *	$Revision $						$Locker:  $
 *	======================================================================
 *	Change Log:
 *
 *	$Log: tourney.c,v $
 * Revision 1.35  1997/10/06  22:05:02  grjost
 * Added track 4 to remote switch box
 *
 * Revision 1.34  1997/09/25  02:09:12  grjost
 * Fixed tourney mode for 12 tracks, 8 cars, and menus in GUTS.
 *
 * Revision 1.33  1997/06/28  01:42:52  gray
 * Extended to 4 tracks.
 *
 * Revision 1.32  1997/05/30  19:02:58  gray
 * Got rid of FLAGSTAFF conditionals.
 *
 * Revision 1.31  1997/03/18  19:38:24  geraci
 * checks all parameters more thoroughly
 *
 * Revision 1.30  1997/02/21  23:29:25  geraci
 * normal laps is used only to set to default, real numbers are always passed
 *
 * Revision 1.29  1997/02/21  00:11:46  geraci
 * msec parameter removed from tourney message
 *
 * Revision 1.28  1997/02/15  01:52:57  geraci
 * join quick in tourney implemented
 *
 * Revision 1.27  1997/01/31  23:34:45  geraci
 * removed difficulty from tourney setup screen
 *
 * Revision 1.26  1997/01/24  02:18:10  geraci
 * new checkpoint scheme
 *
 * Revision 1.25  1996/12/06  17:30:55  geraci
 * detect edge diff on track1 switch box position
 *
 * Revision 1.24  1996/12/02  04:57:26  geraci
 * limit num of laps to 30 for tourney
 *
 * Revision 1.23  1996/11/25  14:21:02  petrick
 * Got rid of 'active' flag in tourney and 'active' menu entry.
 *
 * Revision 1.22  1996/11/25  06:32:50  petrick
 * Added Reset tourney status func.
 *
 * Revision 1.21  1996/11/25  04:19:06  petrick
 * Changed tourney mode to tourney setup.
 *
 * Revision 1.20  1996/11/24  06:19:55  petrick
 * Doesn't dim out unused cabinets, just doesn't put glow around them.
 *
 * Revision 1.19  1996/11/22  10:19:15  petrick
 * Wording changes to final release verion text, added tourneynode function.
 *
 * Revision 1.18  1996/11/21  13:57:38  petrick
 * Can now change settings without affecting local stack; cabinet glows now mean
 * stack is in link or not.
 *
 * Revision 1.17  1996/11/17  14:28:59  petrick
 * Makes sure graphics are in memory before trying to use them.
 *
 * Revision 1.16  1996/11/15  16:14:35  petrick
 * Allows selection of 8 car types.
 *
 * Revision 1.15  1996/11/13  11:57:38  petrick
 * Made sure all loads can abort OK.
 *
 * Revision 1.14  1996/11/11  14:28:39  petrick
 * Made loadmodel smarter about allowing aborts.
 *
 * Revision 1.13  1996/11/05  01:33:33  petrick
 * Now checks all tourney mode nvram data for bad values and resets if bad.
 *
 * Revision 1.12  1996/11/01  11:19:31  geraci
 * difficulty is used during tournament mode
 *
 * Revision 1.11  1996/10/31  09:33:55  petrick
 * Length menu initial selection based on laps.
 *
 * Revision 1.10  1996/10/31  04:49:26  petrick
 * Got '+' for adding laps.
 *
 * Revision 1.9  1996/10/25  09:47:34  petrick
 * Fixed prototype problem.
 *
 * Revision 1.8  1996/10/25  09:40:28  petrick
 * Changes to lap counting.
 *
 * Revision 1.7  1996/10/25  06:49:55  petrick
 * Fixed declaration warning msg.
 *
 * Revision 1.6  1996/10/25  00:32:20  petrick
 * Made tourney laps correct values.
 *
 * Revision 1.5  1996/10/23  12:28:19  petrick
 * Coupla minor changes.
 *
 * Revision 1.4  1996/10/23  11:16:51  petrick
 * First 'working' version.
 *
 * Revision 1.3  1996/10/22  10:34:19  petrick
 * Tourney mode changes.
 *
 * Revision 1.2  1996/10/18  11:04:11  petrick
 * Changed load model.
 *
 * Revision 1.1  1996/10/16  09:32:31  petrick
 * Initial revision
 *
 *
 *	======================================================================
 */

#include "attract.h"
#include "debug.h"
#include "dis.h"
#include "globals.h"
#include <options.h>
#include "eer_defs.h"

#include "Pro/dnl.pro"
#include "Pro/select.pro"


/*===============================  DEFINES  =================================*/

#define LOGO_LETTERS	0		/*	1 == Show moving 3D letters in logo screen. */
#define TEST_HIGH_SCORE	0		/*	1 == enter a high score name in attract mode. */
#define NEW_SCORE_ENTRY 0		/*	1 == New high score entry method. */

#define MAX_MSG_CHARS 	256
#define MOVIESIZE 		(256*256*2)

#define SUBY			18
#define NXTY			28


enum { MENU_COIN = 0, MENU_CAB, MENU_JOIN, MENU_TSEL, MENU_CSEL, MENU_TRANS, 
	   MENU_LEN1, MENU_LEN2, MENU_LEN3, MENU_DRONE, MENU_SPEED, MENU_EXIT,
	   NUM_MENUS };

enum { EXIT_CANCEL = 0, EXIT_RESTORE, EXIT_SAVE };

static const S8	gNumMenuChoices[NUM_MENUS] = { 2,8,3,4,9,3,5,5,5,3,3,3 };


/*==============================  PROTOTYPES  ===============================*/

static BlitFunc	AnimateMenu, AnimateColor, AnimateTTrkSel, AnimateTCarSel,  
                AnimateTTranSel, AnimateTDrones,
                AnimateTCount1, AnimateTCount2, AnimateTCount3, 
                AnimateTLength1, AnimateTLength2, AnimateTLength3,
	            AnimateTExit, AnimateTJoin, AnimateTCoin, AnimateTGlow, 
				AnimateBlack, AnimateTCatchup;

static int		AnimateTLength(BLIT *blt, S16 m_item, S16 track);
static int 		updateMenu(BLIT *blt, BOOL isCur, BOOL red);
static void 	SaveTourneySettings(void);
static void 	HandleLights(void);
static BOOL 	BadTourneyData(Tourney *t);

extern U32  	ctl_mod_latch(int x);
extern void   	eer_write_byte(U8 *where,U8 *unlock,int what);


/*===============================  EXTERN  =================================*/

extern SFONTINFO      sfontX;
extern BOOL           join_during_load;

/*===============================  GLOBALS  =================================*/

Tourney				gTourney;
static BOOL			gInTourney[MAX_LINKS];
static S32			TourneyLaps[NTRACKS];
static S8			gCurMenu, gCurSelect[NUM_MENUS];
static BLIT			*TourneyBlit;

static MULTIBLIT const TourneyMultiBlit[] = 
{	/* name			 		       x            y whtblrfz alpha animFunc        id */
	{"CARICONGLOW",              155,          29, MUNUSED, 0x00,AnimateTGlow,    0},
	 {"CARICONGLOW",             185,          29, MUNUSED, 0x00,AnimateTGlow,    1},
	 {"CARICONGLOW",             215,          29, MUNUSED, 0x00,AnimateTGlow,    2},
	 {"CARICONGLOW",             245,          29, MUNUSED, 0x00,AnimateTGlow,    3},
	 {"CARICONGLOW",             275,          29, MUNUSED, 0x00,AnimateTGlow,    4},
	 {"CARICONGLOW",             305,          29, MUNUSED, 0x00,AnimateTGlow,    5},
	 {"CARICONGLOW",             335,          29, MUNUSED, 0x00,AnimateTGlow,    6},
	 {"CARICONGLOW",             365,          29, MUNUSED, 0x00,AnimateTGlow,    7},
	{"CARICON",                  160,          33, MUNUSED, 0x00,AnimateColor,    0},
	 {"CARICON",                 190,          33, MUNUSED, 0x00,AnimateColor,    1},
	 {"CARICON",                 220,          33, MUNUSED, 0x00,AnimateColor,    2},
	 {"CARICON",                 250,          33, MUNUSED, 0x00,AnimateColor,    3},
	 {"CARICON",                 280,          33, MUNUSED, 0x00,AnimateColor,    4},
	 {"CARICON",                 310,          33, MUNUSED, 0x00,AnimateColor,    5},
	 {"CARICON",                 340,          33, MUNUSED, 0x00,AnimateColor,    6},
	 {"CARICON",                 370,          33, MUNUSED, 0x00,AnimateColor,    7},
	{"HSICON1",                  225,         130, MUNUSED, 0x00,AnimateTCarSel,0x11},
	 {"HSICON2",                 260,         130, MUNUSED, 0x00,AnimateTCarSel,0x12},
	 {"HSICON3",                 295,         130, MUNUSED, 0x00,AnimateTCarSel,0x13},
	 {"HSICON4",                 330,         130, MUNUSED, 0x00,AnimateTCarSel,0x14},
	 {"HSICON5",                 365,         130, MUNUSED, 0x00,AnimateTCarSel,0x15},
	 {"HSICON6",                 400,         130, MUNUSED, 0x00,AnimateTCarSel,0x16},
	 {"HSICON7",                 435,         130, MUNUSED, 0x00,AnimateTCarSel,0x17},
	 {"HSICON8",                 470,         130, MUNUSED, 0x00,AnimateTCarSel,0x18},
};


/* ----------------------------------------------------------------------------- *
 * Initializes Tournament mode.
 * ----------------------------------------------------------------------------- */
void InitTourney(void)
{
	gCurMenu	= 0;
	TourneyBlit = nil;

	LoadTourneySettings();

}	/*	end InitTourney */


/* ----------------------------------------------------------------------------- *
 * Saves Tournament mode settings.
 * ----------------------------------------------------------------------------- */
static BOOL BadTourneyData(Tourney *t)
{
#if 1
	if(t->sanity != 0xDEADBEEF)			return 1;
	if(t->ext_start > 2 || t->ext_start < 0)	return 1;
	if(t->cabinets >= (1 << MAX_LINKS))		return 1;
	if(t->laps > 99 || t->laps < 0)    return 1;
	if(t->track >= NTRACKS || t->track < -1)	return 1;
	if(t->car >= NCARS || t->car < -1)		return 1;
	if(t->tranny > 1 || t->tranny < -1)		return 1;
	if(t->drones > 1 || t->drones < -1)		return 1;
	if(t->catchup > 1 || t->catchup < -1)		return 1;

	return 0;
#else
	return (t->sanity != 0xDEADBEEF ||
			t->ext_start > 2 || t->ext_start < 0 ||
			t->cabinets >= (1 << MAX_LINKS) ||
			t->laps[0] > 30 || t->laps[0] < 0 ||
			t->laps[1] > 30 || t->laps[1] < 0 ||
			t->laps[2] > 30 || t->laps[2] < 0 ||
			t->track >= NTRACKS || t->track < -1 ||
			t->car >= NCARS || t->car < -1 ||
			t->tranny > 1 || t->tranny < -1 ||
			t->drones > 1 || t->drones < -1 ||
			t->catchup > 1 || t->catchup < -1);
#endif
}	/* end BadTourneyData */


/* ----------------------------------------------------------------------------- *
 * Saves Tournament mode settings.
 * ----------------------------------------------------------------------------- */
void LoadTourneySettings(void)
{
	Tourney    oldTourney;
	U8	*src	= (U8 *)&oldTourney;
	U32	*tbl	= TABLE_BASE + TABLE_SIZE * NTRACKS;
	U32	i, tourn_opt;
	S16 laps;

	/*	Load the old table from the nvram. */
	for (i=0; i < sizeof(Tourney); ++i)
		*src++ = (U8)(*tbl++ & 0xFF);
	
	/*	Load the new table from the nvram. */
	tourn_opt = eer_gets(EER_TRNOPT);
	laps = (tourn_opt & GO_TRNLAPS) >> GOS_TRNLAPS;
	ParseTourneyFlags(tourn_opt, laps);

	/*	Bad tourney data, reset it to defaults. */
	if (BadTourneyData(&gTourney))
		{
		MSG( "Init Tourney");

		gTourney.free		= 0;			/*	0 == pay each time, 1 == free play. */
		gTourney.ext_start	= 0;			/*	0 == each player select, 1 == Use external switch to activate join-in, 2 == Same as remote but remove most game delays. */
		gTourney.track		= -1;			/*	> 0, Use track; == 0, external switch, < 0 any. */
		gTourney.car		= -1;			/*	>= 0, Use given car; < 0, any car can play. */
		gTourney.tranny		= -1;			/*	>= 0, Use given tranny; < 0, any tranny can play. */
		gTourney.laps	    = 0;		/*	> 0, Number of laps; 0 <= Normal game laps. */
		gTourney.drones		= -1;			/*	1 == drones; 0 == No drones; < 0, Normal drone setting. */
		gTourney.catchup	= -1;			/*	1 == catchup; 0 == No catchup; < 0, Normal catchup setting. */
		gTourney.cabinets	= 0;			/*	1 << node is set if 'node' is in tourney. */
		SaveTourneySettings();
		}

	/*
	 * Check to see if tourney table was changed in GUTS.  If it was
	 * send it to everyone else.
	 */
    if ((memcmp(&gTourney, &oldTourney, sizeof(Tourney)) != 0) ||
        (tourn_opt & GO_SENDTOURN))
    {
        tourn_opt &= ~GO_SENDTOURN;        /* Clear send message bit now */
		PutEventReportPDU( Event_Tourney, 3,
    					   Report_Node,	        (U32)gThisNode,
						   Report_TourneyFlags, (U32)tourn_opt,
						   Report_TourneyLaps,  (S32)laps);
    }

	/*	Set up the tourney setup menu with the current values. */
	gCurSelect[MENU_COIN] 	= gTourney.free;
	gCurSelect[MENU_CAB] 	= gThisNode;
	gCurSelect[MENU_JOIN] 	= gTourney.ext_start;
	gCurSelect[MENU_TSEL] 	= (gTourney.track < 0) ? 3 : gTourney.track;
	gCurSelect[MENU_CSEL] 	= gTourney.car + 1;
	gCurSelect[MENU_TRANS]	= gTourney.tranny + 1;
	gCurSelect[MENU_DRONE] 	= gTourney.drones + 1;
	gCurSelect[MENU_SPEED] 	= gTourney.catchup + 1;
	gCurSelect[MENU_EXIT] 	= 0;
	gCurSelect[MENU_LEN1]	= (gTourney.laps <= 0) ? 0 : 3;
	gCurSelect[MENU_LEN2]	= (gTourney.laps <= 0) ? 0 : 3;
	gCurSelect[MENU_LEN3]	= (gTourney.laps <= 0) ? 0 : 3;

	for (i=0; i<MAX_LINKS; ++i)
		gInTourney[i] = ((1 << i) & gTourney.cabinets) != 0;
	for (i=0; i<NTRACKS; i++)
		TourneyLaps[i] = gTourney.laps;
}	/*	end LoadTourneySettings */


/* ----------------------------------------------------------------------------- *
 * Saves Tournament mode settings.
 * ----------------------------------------------------------------------------- */
static void SaveTourneySettings(void)
{
	U8	*src	= (U8 *)&gTourney;
	U32	*tbl	= TABLE_BASE + TABLE_SIZE * NTRACKS;
	U32	i;

	gTourney.sanity = 0xDEADBEEF;
	for (i=0; i < sizeof(Tourney); ++i)
		eer_write_byte( (U8 *)tbl++, (U8 *)BRAM_UNLK, 0xff & *src++ );		
	MSG( "Save: gTourney.Laps %ld", (S32)gTourney.laps );

}	/*	end SaveTourneySettings */


/* ----------------------------------------------------------------------------- *
 * Displays or removes the Tournament mode setup screen.
 * ----------------------------------------------------------------------------- */
void ShowTourneySetup(BOOL show)
{
	S16 	x, y;
	char 	s[2] = {0,0};

	/* If Over Select doesn't exist yet, create it. */
	if (!TourneyBlit && show) 
		{
		if (!CheckModelType(MT_ATTRACT))
			return;
		TourneyBlit = NewMultiBlit(0,0,TourneyMultiBlit,NBLITS(TourneyMultiBlit));
		InitMsgs(1, true);

		/*	Make sure the most current settings are in use. */
		LoadTourneySettings();

		/*	Put some text stuff on the screen. */
		/*						    ID               Data */
		y = 6;
		AddMsg( 10,  y, "Coin Mode:", MENU_COIN, AnimateMenu, MENU_COIN);
 		 AddMsg( 160, y, "Norm",	0, AnimateTCoin, MENU_COIN);
 		 AddMsg( 270, y, "Free",	1, AnimateTCoin, MENU_COIN);

		AddMsg( 10,  y+=NXTY, "Cabinets:", MENU_CAB, AnimateMenu, MENU_CAB);
		y += SUBY;

		AddMsg( 10,  y+=NXTY, "Join In:", MENU_JOIN, AnimateMenu, MENU_JOIN);
 		 AddMsg( 160, y, "Any",		0, AnimateTJoin, MENU_JOIN);
 		 AddMsg( 240, y, "Remote",	1, AnimateTJoin, MENU_JOIN);
 		 AddMsg( 360, y, "Quick",	2, AnimateTJoin, MENU_JOIN);

		AddMsg( 10,  y+=NXTY, "Track:", MENU_TSEL, AnimateMenu, MENU_TSEL);
 		 AddMsg( 160, y, "T1",		0, AnimateTTrkSel, MENU_TSEL);
 		 AddMsg( 220, y, "T2",		1, AnimateTTrkSel, MENU_TSEL);
 		 AddMsg( 280, y, "T3",		2, AnimateTTrkSel, MENU_TSEL);
 		 AddMsg( 340, y, "Remote",	3, AnimateTTrkSel, MENU_TSEL);

		AddMsg( 10, y+=NXTY+5, "Car:",  MENU_CSEL, AnimateMenu, MENU_CSEL);
 		y += SUBY; 
 		 AddMsg( 160, y, "Any",   	0, AnimateTCarSel, MENU_CSEL);
		 AddMsg( 225, y, "1",		1, AnimateTCarSel, MENU_CSEL);
 		 AddMsg( 260, y, "2",		2, AnimateTCarSel, MENU_CSEL);
 		 AddMsg( 295, y, "3",   	3, AnimateTCarSel, MENU_CSEL);
 		 AddMsg( 330, y, "4",  		4, AnimateTCarSel, MENU_CSEL);
		 AddMsg( 365, y, "5",		5, AnimateTCarSel, MENU_CSEL);
 		 AddMsg( 400, y, "6",		6, AnimateTCarSel, MENU_CSEL);
 		 AddMsg( 435, y, "7",   	7, AnimateTCarSel, MENU_CSEL);
 		 AddMsg( 470, y, "8",  		8, AnimateTCarSel, MENU_CSEL);

		AddMsg( 10, y+=NXTY, "Shifter:", MENU_TRANS, AnimateMenu, MENU_TRANS);
 		 AddMsg( 160, y, "Any",		0, AnimateTTranSel, MENU_TRANS);
 		 AddMsg( 250, y, "Manual",	1, AnimateTTranSel, MENU_TRANS);
 		 AddMsg( 380, y, "Auto",	2, AnimateTTranSel, MENU_TRANS);

		AddMsg( 10, y+=NXTY, "T1 Laps:",MENU_LEN1, AnimateMenu, MENU_LEN1);
 		 AddMsg( 210, y, "Norm",	0, AnimateTCount1, MENU_LEN1);
 		 AddMsg( 290, y, "-5",		1, AnimateTCount1, MENU_LEN1);
 		 AddMsg( 340, y, "-1",		2, AnimateTCount1, MENU_LEN1);
 		 AddMsg( 390, y, "+1",		3, AnimateTCount1, MENU_LEN1);
 		 AddMsg( 440, y, "+5",		4, AnimateTCount1, MENU_LEN1);

		 s[0] = (TourneyLaps[0] / 10) + '0';
 		 AddMsg( 160, y, s,MENU_LEN1,AnimateTLength1,(s[0] << 8) | 1);
		 s[0] = (TourneyLaps[0] % 10) + '0';
 		 AddMsg( 178, y, s,MENU_LEN1,AnimateTLength1,(s[0] << 8) | 0);

		AddMsg( 10, y+=NXTY, "T2 Laps:",MENU_LEN2, AnimateMenu, MENU_LEN2);
 		 AddMsg( 210, y, "Norm",	0, AnimateTCount2, MENU_LEN2);
 		 AddMsg( 290, y, "-5",		1, AnimateTCount2, MENU_LEN2);
 		 AddMsg( 340, y, "-1",		2, AnimateTCount2, MENU_LEN2);
 		 AddMsg( 390, y, "+1",		3, AnimateTCount2, MENU_LEN2);
 		 AddMsg( 440, y, "+5",		4, AnimateTCount2, MENU_LEN2);

		 s[0] = (TourneyLaps[1] / 10) + '0';
 		 AddMsg( 160, y, s,MENU_LEN2,AnimateTLength2,(s[0] << 8) | 1);
		 s[0] = (TourneyLaps[1] % 10) + '0';
 		 AddMsg( 178, y, s,MENU_LEN2,AnimateTLength2,(s[0] << 8) | 0);

		AddMsg( 10, y+=NXTY, "T3 Laps:",MENU_LEN3, AnimateMenu, MENU_LEN3);
 		 AddMsg( 210, y, "Norm",	0, AnimateTCount3, MENU_LEN3);
 		 AddMsg( 290, y, "-5",		1, AnimateTCount3, MENU_LEN3);
 		 AddMsg( 340, y, "-1",		2, AnimateTCount3, MENU_LEN3);
 		 AddMsg( 390, y, "+1",		3, AnimateTCount3, MENU_LEN3);
 		 AddMsg( 440, y, "+5",		4, AnimateTCount3, MENU_LEN3);

		 s[0] = (TourneyLaps[2] / 10) + '0';
 		 AddMsg( 160, y, s,MENU_LEN3,AnimateTLength3,(s[0] << 8) | 1);
		 s[0] = (TourneyLaps[2] % 10) + '0';
 		 AddMsg( 178, y, s,MENU_LEN3,AnimateTLength3,(s[0] << 8) | 0);

		AddMsg( 10, y+=NXTY, "Drones:",  MENU_DRONE, AnimateMenu, MENU_DRONE);
 		 AddMsg( 160, y, "Norm",	0, AnimateTDrones, MENU_DRONE);
 		 AddMsg( 250, y, "Off",		1, AnimateTDrones, MENU_DRONE);
 		 AddMsg( 320, y, "On",		2, AnimateTDrones, MENU_DRONE);

		AddMsg( 10,  y+=NXTY, "Catchup:",  MENU_SPEED, AnimateMenu, MENU_SPEED);
 		 AddMsg( 160, y, "Norm",	0, AnimateTCatchup, MENU_SPEED);
 		 AddMsg( 250, y, "Off",		1, AnimateTCatchup, MENU_SPEED);
 		 AddMsg( 320, y, "On",		2, AnimateTCatchup, MENU_SPEED);

		AddMsg( 10,  y+=NXTY, "Exit:",  MENU_EXIT, AnimateMenu, MENU_EXIT);
 		 AddMsg( 160, y, "Cancel",	0, AnimateTExit, MENU_EXIT);
 		 AddMsg( 280, y, "Restore",	1, AnimateTExit, MENU_EXIT);
 		 AddMsg( 410, y, "Save",	2, AnimateTExit, MENU_EXIT);

		ctl_mod_latch( (gCurMenu < NUM_MENUS-1) ? LA_VIEW2 : ~LA_VIEW2 );	
		ctl_mod_latch( (gCurMenu > 0)           ? LA_MUSIC : ~LA_MUSIC );	
		ctl_mod_latch( (gCurSelect[gCurMenu] == 0) ? ~LA_VIEW1 : LA_VIEW1);
		ctl_mod_latch( (gCurSelect[gCurMenu] == gNumMenuChoices[gCurMenu] - 1) ? ~LA_VIEW3 : LA_VIEW3);			
		ctl_mod_latch( (gCurMenu == MENU_CAB || gCurMenu == MENU_EXIT) ? LA_ABORT : ~LA_ABORT );	
		}

	if (!show && TourneyBlit)
		{
		/*	Remove the screen. */
		RemoveMsgs();
 		RemoveBlit( TourneyBlit );
		TourneyBlit = nil;
		ctl_mod_latch( ~(LA_VIEW1|LA_VIEW2|LA_VIEW3|LA_ABORT|LA_MUSIC) );
		}

}	/* end ShowTourneySetup */


/* ----------------------------------------------------------------------------- *
 * Turns on/off switch lights depending on menu selections.
 * ----------------------------------------------------------------------------- */
static void HandleLights(void)
{
	ctl_mod_latch( (gCurMenu < NUM_MENUS-1) ? LA_VIEW2 : ~LA_VIEW2 );	
	ctl_mod_latch( (gCurMenu > 0)           ? LA_MUSIC : ~LA_MUSIC );	
	ctl_mod_latch( (gCurSelect[gCurMenu] == 0) ? ~LA_VIEW1 : LA_VIEW1);			
	ctl_mod_latch( (gCurSelect[gCurMenu] == gNumMenuChoices[gCurMenu] - 1) ? ~LA_VIEW3 : LA_VIEW3);			
	ctl_mod_latch( (gCurMenu == MENU_CAB || gCurMenu == MENU_EXIT || gCurMenu == MENU_LEN1
					|| gCurMenu == MENU_LEN2 || gCurMenu == MENU_LEN3) ?
				   LA_ABORT : ~LA_ABORT );	
}	/*	end HandleLights */


/* ----------------------------------------------------------------------------- *
 * Displays or removes the Tournament mode setup screen.
 * ----------------------------------------------------------------------------- */
void HandleTourneySetup(void)
{
	S16		i, laps, track;
	U32		flags;
	Tourney t;

	/*	Handle controls. */
	if (edges & SW_VIEW2) 
		{
		if (++gCurMenu >= NUM_MENUS)
			gCurMenu = NUM_MENUS - 1;
		HandleLights();
		}

	if (edges & SW_MUSIC)
		{
		if (--gCurMenu < 0)
			gCurMenu = 0;
		HandleLights();
		}

	if (edges & SW_VIEW1)
		{
		if (gCurMenu == MENU_CAB)
			{
			while ( (gLiveNodes & ((1 << gCurSelect[gCurMenu]) - 1)) != 0 &&
					gCurSelect[gCurMenu] >= 0 && !((1 << --gCurSelect[gCurMenu])& gLiveNodes))
				continue;
			}
		else if (--gCurSelect[gCurMenu] < 0)
			gCurSelect[gCurMenu] = 0;
		HandleLights();
		}
	if (edges & SW_VIEW3)
		{
		if (gCurMenu == MENU_CAB)
			{
			while ( (gLiveNodes & ~((1 << (gCurSelect[gCurMenu]+1)) - 1)) != 0 &&
					!((1 << ++gCurSelect[gCurMenu]) & gLiveNodes) )
				continue;
			}
			else if (++gCurSelect[gCurMenu] >= gNumMenuChoices[gCurMenu])
				gCurSelect[gCurMenu] = gNumMenuChoices[gCurMenu] - 1;
		HandleLights();
		}

	/*	Made a selection. */
	if (edges & SW_ABORT)
		{
		track = 0;
		switch (gCurMenu)
			{
			case MENU_CAB:				/*	Cabinets 		*/
				gInTourney[gCurSelect[MENU_CAB]] ^= 1;
				break;

			case MENU_LEN3:				/*	Race laps 		*/
				track++;
			case MENU_LEN2:
				track++;
			case MENU_LEN1:
				laps = TourneyLaps[track];
				if (laps == 0)
					TourneyLaps[track] = track_len_save[track];
				switch (gCurSelect[gCurMenu])
					{
					case 0:	TourneyLaps[track] = track_len_save[track];   break;
					case 1: TourneyLaps[track] = max(laps-5,1);           break;
					case 2: TourneyLaps[track] = max(laps-1,1);           break;
					case 3: TourneyLaps[track] = min(laps+1,30);          break;
					case 4: TourneyLaps[track] = min(laps+5,30);          break;
					}
					break;

			case MENU_EXIT:				/*	Exit 			*/
				if (gCurSelect[MENU_EXIT] == EXIT_SAVE)
					{
					t.free		= gCurSelect[MENU_COIN];
					t.ext_start	= gCurSelect[MENU_JOIN];
					t.track		= (gCurSelect[MENU_TSEL] < NTRACKS) ? gCurSelect[MENU_TSEL] : -1;
					t.car		= gCurSelect[MENU_CSEL] - 1;
					t.tranny	= gCurSelect[MENU_TRANS]- 1;
					t.laps		= TourneyLaps[0];
					t.drones	= gCurSelect[MENU_DRONE]- 1;
					t.catchup	= gCurSelect[MENU_SPEED]- 1;
					
					for (t.cabinets=i=0; i<MAX_LINKS; ++i)
						if (gInTourney[i])
							t.cabinets	|= 1 << i;

					/*	Tell all the other nodes that are in the tourney. */
					flags = (/*((t.active   & 1) << 0) |*/ ((t.free   & 1) << 0) | 
							 ((t.ext_start& 3) << 1) | ((t.track  & 7) << 3) |
							 ((t.car      &15) << 6) | ((t.tranny & 3) <<10) |
							 ((t.drones   & 3) <<12) | ((t.catchup& 3) <<14) |
							 ((t.cabinets & 0xff) << 16));

					PutEventReportPDU( Event_Tourney, 3,
									   Report_Node,	        (U32)gThisNode,
									   Report_TourneyFlags, (U32)flags,
									   Report_TourneyLaps,  (S32)t.laps);
					}

				if (gCurSelect[MENU_EXIT] == EXIT_RESTORE)
					LoadTourneySettings();
				else
					{
					tourney_setup = 0;
					game_opt &= ~GO_TOURNEY;
					eer_puts(EER_GMOPT,game_opt);	/* turn off setup-request	*/
					}
				break;

			default:	
				break;
			}
		}

}	/* end HandleTourneySetup */


/* ----------------------------------------------------------------------------- *
 * Returns true if tournament mode is in force at this node.
 * ----------------------------------------------------------------------------- */
BOOL TourneyOn(void)
{
	return TourneyNode((U32)gThisNode);
}	/*	end TourneyOn */


/* ----------------------------------------------------------------------------- *
 * Returns true if tournament mode is in force at this node.
 * ----------------------------------------------------------------------------- */
BOOL TourneyNode(U32 i)
{
	return /*gTourney.active &&*/ ((gTourney.cabinets & (1 << i)) != 0);
}	/*	end TourneyOn */


/* ----------------------------------------------------------------------------- *
 * Sets tourney status for a given cabinet.
 * ----------------------------------------------------------------------------- */
void ResetTourneyStatus( U32 node, BOOL val )
{
	if (val)
		gTourney.cabinets |= (1 << node);
	else 
		gTourney.cabinets &= ~(1 << node);
}	/* end ResetTourneyStatus */


/* ----------------------------------------------------------------------------- *
 * Parses network coded tourney settings flags.
 * ----------------------------------------------------------------------------- */
void CheckTourneyBox(void)
{
	static BOOL	gotEdge;
	static U32	lastTrack;

	U32 track = ((levels & SW_TRACK4) ? 3 : 
	             (levels & SW_TRACK3) ? 2 : 
				 (levels & SW_TRACK2) ? 1 : 0);

	if (edges & (SW_JOIN | SW_TRACK1 | SW_TRACK2 | SW_TRACK3 | SW_TRACK4))
		gotEdge = true;

	if (gTourney.ext_start && ((SW_JOIN & edges) || join_during_load))
		{
		join_during_load = false;
		MSG("Sending JOIN NOW");
		PutEventReportPDU( Event_TourneyStart, 3, 
						   Report_Node,  (U32)gThisNode, 
						   Report_Track, (U32)track,
						   Report_Flags, (U32)1 );
		}
	else if (track != lastTrack && gotEdge)
		{
		MSG("Sending Tourney mode setup");
		PutEventReportPDU( Event_TourneyStart, 3, 
						   Report_Node,  (U32)gThisNode, 
						   Report_Track, (U32)track,
						   Report_Flags, (U32)0 );
		}
	lastTrack = track;
}	/* end CheckTourneyBox */


/* ----------------------------------------------------------------------------- *
 * Parses network coded tourney settings flags.
 * ----------------------------------------------------------------------------- */
void ParseTourneyFlags(U32 flags, S16 laps)
{
	U32	cab 				= (flags >> 16 & 0xff);

	/*	Parse the flags, save the settings. */
#if 0
	gTourney.free		= ((flags >> 0)  &  1);
#else
	gTourney.free		= 0;
#endif
	gTourney.ext_start	= ((flags >> 1)  &  3);
	if ((gTourney.track	= ((flags >> 3)  &  7)) >= NTRACKS)
		gTourney.track 	= -1;
	if ((gTourney.car	= ((flags >> 6)  & 15)) >= NCARS)
		gTourney.car	= -1;
	if ((gTourney.tranny= ((flags >>10)  &  3)) > 1)
		gTourney.tranny = -1;
	if ((gTourney.drones= ((flags >>12)  &  3)) > 1)
		gTourney.drones = -1;
	if ((gTourney.catchup= ((flags >>14) &  3)) > 1)
		gTourney.catchup = -1;
	gTourney.cabinets	= cab;

	gTourney.laps		= rng(laps, 0, 99);
	MSG( "Parse: TourneyLaps %ld laps %ld", (S32)TourneyLaps[0], (long)laps );

    eer_puts(EER_TRNOPT, flags);
	SaveTourneySettings();

}	/*	end ParseTourneyFlags */


/* ----------------------------------------------------------------------------- *
 * Called to create a semi-transparent black overlay for the Tourney Setup screen.
 * ----------------------------------------------------------------------------- */
static int AnimateBlack(BLIT *blt)
{

	blt->Width		= SCR_W;
	blt->Height		= SCR_H;
	blt->Alpha		= 0x30;
	blt->AnimFunc	= nil;
	UpdateBlit(blt);
	return 1;

}	/* end AnimateBlack */


/* ----------------------------------------------------------------------------- *
 * Called to handle menu entries.
 * ----------------------------------------------------------------------------- */
static int updateMenu(BLIT *blt, BOOL isCur, BOOL red)
{
	BOOL	wasCur 	= (blt->AnimID & 0x100) != 0;

	if (isCur && !wasCur)
		{
		RenameBlit( blt, red ? "FONT1RED" : "FONT1YEL", 1);
		blt->AnimID |= 0x100;
		}
	else if (!isCur && wasCur)
		{
		RenameBlit( blt, "FONT1", 1);
		blt->AnimID &= ~0x100;
		}

	Hidden(blt, isCur && blt->AnimDTA == gCurMenu && (IRQTIME & 0x100));

	return 1;

}	/* end updateMenu */


/* ----------------------------------------------------------------------------- *
 * Called to handle the menu header blits for the Tourney Setup screen.
 * ----------------------------------------------------------------------------- */
static int AnimateMenu(BLIT *blt)
{
	return updateMenu( blt, gCurMenu == (blt->AnimID & 0xf), false );

}	/* end AnimateMenu */


/* ----------------------------------------------------------------------------- *
 * Called to handle the car color blits for the Tourney Setup screen.
 * ----------------------------------------------------------------------------- */
static int AnimateColor(BLIT *blt)
{
	U8	slot	= blt->AnimID & 0xF;
	S8	i;

	blt->AnimDTA	= MENU_CAB;

	/* Remove if this node doesn't exist. */
	if (Hidden( blt, !(gLiveNodes & (1 << slot)) || 
				(gCurMenu == MENU_CAB && (U8)gCurSelect[MENU_CAB] == slot && 
				 (IRQTIME & 0x100))))
		return 1;

/*	blt->Alpha =  gInTourney[slot] ? 0x00 : 0xA0; */
	return updateCar( blt, slot, gInTourney[slot] );

}	/* end AnimateColor */


/* ----------------------------------------------------------------------------- *
 * Called to handle the car color blits for the Tourney Setup screen.
 * ----------------------------------------------------------------------------- */
static int AnimateTGlow(BLIT *blt)
{
	U8	slot	= blt->AnimID & 0xF;

	blt->AnimDTA	= MENU_CAB;
	Hidden(blt, !(gLiveNodes & (1 << slot)) || !gInTourney[slot] || 
				(gCurMenu == MENU_CAB && (U8)gCurSelect[MENU_CAB] == slot && 
				 (IRQTIME & 0x100)));
	return 1;

}	/* end AnimateTGlow */


/* ----------------------------------------------------------------------------- *
 * Called to handle the Track Select blits for the Tourney Setup screen.
 * ----------------------------------------------------------------------------- */
static int AnimateTTrkSel(BLIT *blt)
{
	return updateMenu( blt, gCurSelect[MENU_TSEL] == (blt->AnimID & 0xf), true );

}	/* end AnimateTTrkSel */


/* ----------------------------------------------------------------------------- *
 * Called to handle the Car Select blits for the Tourney Setup screen.
 * ----------------------------------------------------------------------------- */
static int AnimateTCarSel(BLIT *blt)
{
	BOOL	on = (gCurSelect[MENU_CSEL] == (blt->AnimID & 0xf));

	/*	This is a car icon. */
	if (blt->AnimID & 0x10)
		{
		blt->Alpha = (on || !gCurSelect[MENU_CSEL]) ? 0x00 : 0xA0;
		UpdateBlit(blt);
		return 1;
		}
	return updateMenu( blt, on, true );

}	/* end AnimateTCarSel */


/* ----------------------------------------------------------------------------- *
 * Called to handle the Transmission Select blits for the Tourney Setup screen.
 * ----------------------------------------------------------------------------- */
static int AnimateTTranSel(BLIT *blt)
{
	return updateMenu( blt, gCurSelect[MENU_TRANS] == (blt->AnimID & 0xf), true );

}	/* end AnimateTTranSel */


/* ----------------------------------------------------------------------------- *
 * Called to handle the Race Length blits for the Tourney Setup screen.
 * ----------------------------------------------------------------------------- */
static int AnimateTLength1(BLIT *blt)
{
	return (AnimateTLength (blt, MENU_LEN1, 0));
}

/* ----------------------------------------------------------------------------- *
 * Called to handle the Race Length blits for the Tourney Setup screen.
 * ----------------------------------------------------------------------------- */
static int AnimateTLength2(BLIT *blt)
{
	return (AnimateTLength (blt, MENU_LEN2, 1));
}
	
/* ----------------------------------------------------------------------------- *
 * Called to handle the Race Length blits for the Tourney Setup screen.
 * ----------------------------------------------------------------------------- */
static int AnimateTLength3(BLIT *blt)
{
	return (AnimateTLength (blt, MENU_LEN3, 2));
}
	
/* ----------------------------------------------------------------------------- *
 * Called to handle the Race Length blits for the Tourney Setup screen.
 * ----------------------------------------------------------------------------- */
static int AnimateTLength(BLIT *blt, S16 m_item, S16 track)
{
	char	c;
	BOOL	t,isCur,wasCur;
	S16		laps;

	laps = TourneyLaps[track];

	isCur	= (gCurMenu == m_item);
	wasCur 	= (blt->AnimID & 0x100) != 0;
	if (isCur && !wasCur)
		blt->AnimID |= 0x100;
	else if (!isCur && wasCur)
		blt->AnimID &= ~0x100;

	RenameBlit( blt, "FONT1RED", 1);

	if (laps == 0)
		laps = track_len_save[track];

	t = (blt->AnimDTA & 1);
	if (Hidden(blt, ((isCur && (IRQTIME & 0x100)) || (!t && laps < 10))))
		return 1;

	/* get ten's digit */
	c = (laps / 10) + '0';

	/*	See if the number changed. */
	if (t)
		/* character to the left */
		{
		if (c == '0')
			/* no tens, just display one's digit */
			c = (laps % 10) + '0';
		}
	else
		/* character to the right */
		{
		if (c != '0')
			/* display one's digit to right of ten's */
			c = (laps % 10) + '0';
		}

	if (c != (char)((blt->AnimDTA >> 8) & 0xff))
		{
		RenameFontBlitChar(blt, &sfontX, c);
		blt->AnimDTA = (c << 8) | t;
		}

	return 1;

}	/* end AnimateTLength */


/* ----------------------------------------------------------------------------- *
 * Called to handle the Race Length blits for the Tourney Setup screen.
 * ----------------------------------------------------------------------------- */
static int AnimateTCount1(BLIT *blt)
{
	return updateMenu( blt, gCurSelect[MENU_LEN1] == (blt->AnimID & 0xf), false );

}	/* end AnimateTCount */

/* ----------------------------------------------------------------------------- *
 * Called to handle the Race Length blits for the Tourney Setup screen.
 * ----------------------------------------------------------------------------- */
static int AnimateTCount2(BLIT *blt)
{
	return updateMenu( blt, gCurSelect[MENU_LEN2] == (blt->AnimID & 0xf), false );

}	/* end AnimateTCount */

/* ----------------------------------------------------------------------------- *
 * Called to handle the Race Length blits for the Tourney Setup screen.
 * ----------------------------------------------------------------------------- */
static int AnimateTCount3(BLIT *blt)
{
	return updateMenu( blt, gCurSelect[MENU_LEN3] == (blt->AnimID & 0xf), false );

}	/* end AnimateTCount */


/* ----------------------------------------------------------------------------- *
 * Called to handle the Drones setting blits for the Tourney Setup screen.
 * ----------------------------------------------------------------------------- */
static int AnimateTDrones(BLIT *blt)
{
	return updateMenu( blt, gCurSelect[MENU_DRONE] == (blt->AnimID & 0xf), true );

}	/* end AnimateTDrones */


/* ----------------------------------------------------------------------------- *
 * Called to handle the Catchuo setting blits for the Tourney Setup screen.
 * ----------------------------------------------------------------------------- */
static int AnimateTCatchup(BLIT *blt)
{
	return updateMenu( blt, gCurSelect[MENU_SPEED] == (blt->AnimID & 0xf), true );

}	/* end AnimateTCatchup */

/* ----------------------------------------------------------------------------- *
 * Called to handle the Exit blits for the Tourney Setup screen.
 * ----------------------------------------------------------------------------- */
static int AnimateTExit(BLIT *blt)
{
	return updateMenu( blt, gCurSelect[MENU_EXIT] == (blt->AnimID & 0xf), true );

}	/* end AnimateTExit */


/* ----------------------------------------------------------------------------- *
 * Called to handle the Join In blits for the Tourney Setup screen.
 * ----------------------------------------------------------------------------- */
static int AnimateTJoin(BLIT *blt)
{
	return updateMenu( blt, gCurSelect[MENU_JOIN] == (blt->AnimID & 0xf), true );

}	/* end AnimateTJoin */


/* ----------------------------------------------------------------------------- *
 * Called to handle the Coin Mode blits for the Tourney Setup screen.
 * ----------------------------------------------------------------------------- */
static int AnimateTCoin(BLIT *blt)
{
	return updateMenu( blt, gCurSelect[MENU_COIN] == (blt->AnimID & 0xf), true );

}	/* end AnimateTCoin */
