/*
 *	globals.c	-- Declares and initializes global variables.
 *
 *	Copyright 1996 Time Warner Interactive.
 *	Unauthorized reproduction, adaptation, distribution, performance or 
 *	display of this computer program or the associated audiovisual work
 *	is strictly prohibited.
 *
 *	======================================================================
 *	$Author: gray $						$Date: 1997/03/31 00:04:09 $
 *	$Revision: 3.10 $						$Locker:  $
 *	======================================================================
 *	Change Log:
 *
 *	$Log: globals.c,v $
 * Revision 3.10  1997/03/31  00:04:09  gray
 * update_game_data blocks model ast with gModeldatLock.
 *
 * Revision 3.9  1996/11/27  12:56:10  gray
 * Changed NetTime from F32 to 10microsec S32.
 *
 * Revision 3.8  1996/11/11  09:19:26  geraci
 * new way to deal with pots
 *
 * Revision 3.7  1996/11/09  12:26:54  geraci
 * pot limit read routine doesn't get a parameter passed to it
 *
 * Revision 3.6  1996/10/10  05:20:15  petrick
 * Made RUSHGLOBAL so globals only need be defined in globals_h and not globals.c too.
 *
 * Revision 3.5  1996/10/07  16:15:02  geraci
 * moved some game options to globals
 *
 * Revision 3.4  1996/09/28  01:36:36  gray
 * Made global camera pos and uvs (gCamPos gCamUvs).
 *
 * Revision 3.3  1996/09/27  03:26:51  gray
 * Made camera control local. Eliminated CamFunc.
 *
 * Revision 3.2  1996/09/26  00:23:30  gray
 * Killed ztypes.h
 *
 * Revision 3.1  1996/09/09  02:24:52  gray
 * JAMMA version
 *
 *	======================================================================
 */

/* Globals get declared non-extern in this file. */
#define RUSHGLOBAL
#include "globals.h"
#undef RUSHGLOBAL

#include <eer_defs.h>
#include "mb.h"
#include "debug.h"
#include "error.h"
#include "model.h"
#include "string.h"
#include "dis.h"
#include "options.h"

#include "Pro/globals.pro"
#include "Pro/pots.pro"
#include "Pro/mdrive.pro"
#include "Pro/timint.pro"


/*===============================  DEFINES  =================================*/

#define USE_GAME_OPTIONS  1		/*	Define to use NVRAM Game Options menu setting to set stack. */


/*==============================  PROTOTYPES  ===============================*/

extern void reent_init(void);


/*===============================  GLOBALS  =================================*/

VS16	ResetLine;
VU32	*record_buffer;

/*-------------------------------  EXTERNS  ---------------------------------*/


/* ----------------------------------------------------------------------------- *
 *	Initializes global variables.
 *	init_glob should be the first subroutine to run.
 * ----------------------------------------------------------------------------- */
void init_glob(void)
{
	gModeldatLock = 0;

	gSecPerCount 	 = 2.0/(F32)CPU_SPEED;		/*seconds per CPU count*/
	gMsecPerCount 	 = (2.0 * 1000.0)/(F32)CPU_SPEED;	/*msec per CPU count*/
	ginvMsecPerCount = 1.0 / gMsecPerCount;
	gNetTimeToSec	= 1.0/100000.0;		/* net timer (10microsec) to seconds */

	get_switches (Initialize);
	get_switches (Do_it);

	set_gthisnode();

	gFlyMode = 0;
	gSWforce = 0;

}	/* end init_glob */

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/

U8 set_gthisnode()
{
#if USE_GAME_OPTIONS
	gThisNode = ((eer_gets( EER_GMOPT ) & GO_COLOR) >> GOS_COLOR) % MAX_LINKS;
#else /* USE_SERIAL_NUMBER */
	{
	    struct hst_ent hsp = *(eer_hstr(0,HST_SERIAL_NUM));

		if (strncmp((char *)hsp.initials, "SFR", 3) == 0)
			gThisNode = hsp.score % MAX_LINKS;
		else
			FatalError (GENERIC, "Serial Number must be: SFR0#### (# is 0-9)");
	}
#endif

	return (gThisNode);
}
