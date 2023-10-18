/*
 *	money.c -- Money handling functions
 *
 *		Copyright 1996 Time Warner Interactive.
 *	Unauthorized reproduction, adaptation, distribution, performance or 
 *	display of this computer program or the associated audiovisual work
 *	is strictly prohibited.
 *
 *	======================================================================
 *	$Author: geraci $						$Date $
 *	$Revision $						$Locker:  $
 *	======================================================================
 *	Change Log:
 *
 *	$Log: money.c,v $
 * Revision 3.17  1997/02/15  01:50:42  geraci
 * join quick in tourney implemented
 *
 * Revision 3.16  1996/11/28  04:36:00  petrick
 * Made chk_start code clearer.
 *
 * Revision 3.15  1996/11/25  10:03:57  petrick
 * Deactivates continue for tourney mode.
 *
 * Revision 3.14  1996/11/25  04:10:06  petrick
 * Got rid of Continue state.
 *
 * Revision 3.13  1996/11/23  20:31:27  petrick
 * Added continued game flag.
 *
 * Revision 3.12  1996/11/22  12:40:07  petrick
 * Made continue mode credits display work correctly.
 *
 * Revision 3.11  1996/11/22  10:11:24  petrick
 * Changed free game awarding so only 1 free game per paid game can be awarded.
 *
 * Revision 3.10  1996/11/17  14:27:57  petrick
 * Tourney mode changes.
 *
 * Revision 3.9  1996/11/15  16:16:50  petrick
 * Tourney mode changes for external switch.
 *
 * Revision 3.8  1996/11/11  13:07:07  geraci
 * external switch box support
 *
 * Revision 3.7  1996/11/11  11:56:09  petrick
 * Tourney mode external start check.
 *
 * Revision 3.6  1996/10/30  04:44:33  petrick
 * Tied discount-to-continue into credit functions.
 *
 * Revision 3.5  1996/10/29  08:25:08  petrick
 * Free game and continue stuff.
 *
 * Revision 3.4  1996/10/23  11:17:33  petrick
 * Tourney mode changes.
 *
 * Revision 3.3  1996/10/10  05:12:22  petrick
 * Made irqtime a true global in globals_h.
 *
 * Revision 3.2  1996/10/02  21:15:02  petrick
 * Changed params in PutEventReport PDU call.
 *
 * Revision 3.1  1996/09/09  02:24:52  gray
 * JAMMA version
 *
 * Revision 2.5  1996/08/30  16:29:49  petrick
 * Adding a coin at any node gives all track selecting nodes more time.
 *
 * Revision 2.4  1996/08/30  14:39:17  petrick
 * Cleaned up.
 *
 * Revision 2.3  1996/08/18  21:25:20  gray
 * Removing zoid stuff. No more zoid.h.
 *
 * Revision 2.2  1996/08/17  03:39:08  petrick
 * Coin support for Rush.
 *
 *
 *	Adapted from Dennis Harper's Rage code. (1/25/94)
 *
 *	======================================================================
 */

#include "attract.h"
#include "controls.h"
#include "dis.h"
#include "globals.h"
#include "mb.h"
#include "os_proto.h"
#include "sounds.h"

#include "Pro/money.pro"
#include "Pro/sounds.pro"
#include "Pro/tourney.pro"


/*===============================  GLOBALS  =================================*/

U16				credits;				/* total whole credits				*/
U8				numer, denom;			/* credits: numerator, denominator	*/

static S32		credval;				/* credits: raw	data				*/
static U8		bongs;
static U32		lastBong;				/* IRQtime of last bong. 			*/


/*===============================  EXTERNS  =================================*/

extern Tourney	gTourney;
extern BOOL		gFreeGame, gPlayingFree, gTourneyJoin, gContinuedGame;
BOOL			continue_flag;


/*==============================  PROTOTYPES  ===============================*/

static BOOL 	CanContinue(void);
static BOOL 	ContinueMode(void);
extern BOOL 	GasPressed(BOOL test);


/* ----------------------------------------------------------------------------- *
 *	Initializes coin variables.
 * ----------------------------------------------------------------------------- */
void init_credits(void)
{
	credval 	= cn_credits();	/* Init credit vars		*/
	bongs		= 0;
	lastBong	= 0;	
	gFreeGame 	= false;
	gPlayingFree= false;
}	/* end init_credits */


/* ----------------------------------------------------------------------------- *
 *	Handles coin input and makes sounds to acknowledge coin drops.
 * ----------------------------------------------------------------------------- */
void get_credits(void)
{
	U16	credits_old	= credits;
	S32	credval_old = credval;

	bongs += cn_bong();
	if (bongs && (IRQTIME - lastBong > 500) )
		{
		--bongs;
		lastBong = IRQTIME;
		SOUND( S_COIN1 );
		}

	credval	= (ContinueMode()) ? cn_continues() : cn_credits();		/* get current credits	*/
	if (credval != credval_old)	/* if changed...	*/
		{
		PutEventReportPDU( Event_Coin, 1, Report_Node, (U32)gThisNode);
		credits = (U16)(credval >> 16);
		numer   = (credval >> 8) & 0xFF;
		denom   = credval & 0xFF;
		if( credits > credits_old )		/* if full credit added...	*/
			SOUND(S_COIN2);				/*     'full coin' sound	*/
		}

}	/* end get_credits */


/* ----------------------------------------------------------------------------- *
 *	Checks for game start and returns true if a game can and should start.
 * ----------------------------------------------------------------------------- */
BOOL chk_start(void)
{
	BOOL useExtern	= (TourneyOn() && gTourney.ext_start);

	/*	Check for enough money to play. */
	if (!EnoughCredit() || (useExtern && !(gTourneyStart || gTourneyJoin)))
		return false;						/* Not enough money         */

	/*	Check for free game. */
	if (gFreeGame)
		{
		gContinuedGame	= false;
		gPlayingFree	= true;
		gFreeGame		= false;
		return true;
		}

	/*	Check for start button/condition. */
	if ((GasPressed(true) || (TourneyOn() && gTourney.ext_start==JOIN_QUICK) ||
		 (edges & (SW_ABORT|SW_VIEW1|SW_VIEW2|SW_VIEW3|SW_REVERSE))))
		{
		gPlayingFree	= false;
		gFreeGame 		= false;
		gContinuedGame	= false;
		if (TourneyOn() && gTourney.free)
			return true;

		if (CanContinue())
			{
			cn_dbcoin(cn_discount());
			gContinuedGame = true;
			}
		else
			cn_debit(1);						/* Take a credit            */
		return true;
		}

	return false;
}	/* end chk_start */


/* ----------------------------------------------------------------------------- *
 *	Returns true if it's OK to start a game.
 * ----------------------------------------------------------------------------- */
BOOL EnoughCredit(void)
{
	/*	Check for enough money to play. */
	return ((TourneyOn() && gTourney.free) || gFreeGame || CanContinue() || 
			cn_chkcoin(cn_price()));
}	/* end EnoughCredit */


/* ----------------------------------------------------------------------------- *
 *	Returns true if continue mode is in effect.
 * ----------------------------------------------------------------------------- */
static BOOL ContinueMode(void)
{
	/*	All these states are continue mode so credits display looks right
	 *	when continuing a game. */
	return (continue_opts && !TourneyOn() &&
			 (continue_flag || gstate == TRKSEL || gstate == CARSEL));
}	/*	end ContinueMode */

/* ----------------------------------------------------------------------------- *
 *	Returns true if it's OK to continue a game.
 * ----------------------------------------------------------------------------- */
static BOOL CanContinue(void)
{
	return (ContinueMode() && cn_chkcoin(cn_discount()));
}	/*	end CanContinue */
