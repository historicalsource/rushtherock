/*
 *	hud.h	-- Definitions for dealing with blits and hud type data.
 *
 *	Copyright 1996 Time Warner Interactive.
 *	Unauthorized reproduction, adaptation, distribution, performance or 
 *	display of this computer program or the associated audiovisual work
 *	is strictly prohibited.
 *
 *	======================================================================
 *	$Author: grjost $						$Date: 1997/09/30 02:20:16 $
 *	$Revision: 3.6 $						$Locker:  $
 *	======================================================================
 *	Change Log:
 *
 *	$Log: hud.h,v $
 * Revision 3.6  1997/09/30  02:20:16  grjost
 * Added new lap font
 *
 * Revision 3.5  1996/11/19  13:39:21  petrick
 * Dynamic texture loading.
 *
 * Revision 3.4  1996/10/12  11:18:03  petrick
 * Moved credits display to attract.h where it belonged.
 *
 * Revision 3.3  1996/09/26  09:56:11  petrick
 * Added Blit Func typedef.
 *
 * Revision 3.2  1996/09/14  08:39:30  petrick
 * Got rid of unused green digits.
 *
 * Revision 3.1  1996/09/09  02:23:24  gray
 * JAMMA version
 *
 * Revision 2.7  1996/08/30  18:48:30  geraci
 * checkpoint delay time global
 *
 * Revision 2.6  1996/08/30  16:32:18  petrick
 * Cleanup.
 *
 * Revision 2.5  1996/08/17  03:38:10  petrick
 * Updated credits display for fractional credits.
 *
 * Revision 2.4  1996/07/19  07:38:04  petrick
 * Moved credits display down to bottom of screen.
 *
 * Revision 2.3  1996/06/20  20:24:51  petrick
 * Moved Attract data to it's own world.
 *
 * Revision 2.2  1996/06/04  22:09:11  petrick
 * Got rid of small speed digits.
 *
 * Revision 2.1  1996/05/04  02:56:18  petrick
 * Focus 2 release
 *
 * Revision 1.7  1996/03/21  07:15:31  petrick
 * Explosions.
 *
 * Revision 1.6  1996/03/08  00:01:39  petrick
 * Moved right edge of screen out a bit.
 *
 * Revision 1.5  1996/03/01  10:32:20  petrick
 * Added new text messages and replaced many placeholders.
 *
 * Revision 1.4  1996/02/28  04:05:14  petrick
 * Made edges of the screen #defines so we can make blits show up on screen better.
 *
 * Revision 1.3  1996/02/07  08:51:32  petrick
 * Moved Digit typedef here to make it more global
 *
 * Revision 1.2  1996/02/02  11:53:35  petrick
 * Changes to support indexed texture map numbers.
 *
 * Revision 1.1  1996/02/01  07:49:31  petrick
 * Initial revision
 *
 *
 *	======================================================================
 */

#ifndef _HUD_H_
#define _HUD_H_

#include "blit.h"
#include "Pro/hud.pro"

/*===============================  DEFINES  =================================*/

#define NBLITS(x)		(sizeof(x)/sizeof(MULTIBLIT))

#define SCR_W			(VIS_H_PIX)		/* Screen width */
#define SCR_H			(VIS_V_PIX)		/* Screen height */

#define SCR_L			(5)					/* Safe start of Left edge of screen. */
#define SCR_R			(SCR_W - 10)		/* Safe start of Right edge of screen. */
#define SCR_T			(5)					/* Safe start of Top edge of screen. */
#define SCR_B			(SCR_H - 5)			/* Safe start of Bottom edge of screen. */
#define SCR_MH			((SCR_R - SCR_L)/2)	/*	Screen Middle Horizontal. */
#define SCR_MV			((SCR_B - SCR_T)/2)	/*	Screen Middle Vertical. */
#define CHECKPT_TIME	(3 * ONE_SEC)


/*===============================  TYPEDEFS  =================================*/

typedef enum mBlits 
{ 
	MBLIT_COUNT, MBLIT_WRONGWAY, MBLIT_PLACE, MBLIT_JOIN, MBLIT_WAIT, 
	MBLIT_PREP, MBLIT_CD
} MBlits;

typedef enum DigitType 
{ 
	SpeedDigit = 0, YelDigit, SmallYelDigit, RedDigit,   SmallRedDigit,
	LapDigit, HudTimerDigit, CountdownDigit, PlaceDigit, TimerDigit,
	NUM_DIGIT_TYPES 
} DigitType;

typedef int (BlitFunc)(BLIT *blt);

#endif /* _HUD_H_ */
