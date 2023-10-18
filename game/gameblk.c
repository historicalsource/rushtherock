/*************************************************************************************************

Copyright 1994,1995 Time Warner Interactive.  Unauthorized reproduction, adaptation,
distribution, performance or display of this computer program or the associated
audiovisual work is strictly prohibited.
*************************************************************************************************/

#include "config.h"
#include "os_proto.h"
#include "optmenu.h"	/* this is the GAME-OPTIONS MENU */

/*Game entry point*/
extern void start(void);

/* The time the game was linked. The definition depends on whether an ANSI
 * compiler was used. Of course, both date.c and this file must be compiled
 * with the same compiler
 */
const char date[]=__DATE__ " " __TIME__;

/* The Menu below is encoded per the scheme outlined at dispopt() in stats.c
 *	Since it is actually one string,  no comments can be included.  The
 * 	octal values shown are translated as follows:
 *		\171 represents 1 bit starting at bit #15 (\017,$0F)
 *		\065 represents 5 bits starting at bit #6 (\006,$06)
 */

/* Note: Update file coin.c in os directory when changing this menu */

extern const unsigned char coinmenu[];

/* >>->	Pretty much all that GUTS knows about the game is passed via the following
 * 	struct.
 */
const struct pconfigb gameblk =  
{
	start,		/* RESET vector 								*/
	gamemenu,	/* Game options menu							*/
	coinmenu,	/* Coin menu 									*/	
	date,		/* MAIN linktime    							*/
	0,			/* trap type 0:STOP +:68K -:68010  				*/
	NO_ROM_TEST+NO_RAM_TEST+NO_LOG_RESET,	/*	Debug options. 	*/
	0, 0, 0, 0, 0, 0,
	trackmenu,	/* Game difficulty/track options menu			*/
	lapmenu,	/* number of laps options menu					*/
	tournmenu	/* Tournament menu			                    */
#ifdef GAME_SPECIFIC
	,(int(*)())no_op
#endif
};
				

const U32 PBASE[2] = { 0xDEADBEEF, (U32)&gameblk };
