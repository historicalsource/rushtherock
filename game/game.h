/*
 *	game.h -- defines for game.c
 *
 *	Copyright 1996 Time Warner Interactive
 *	Unauthorized reproduction, adaptation, distribution, performance or 
 *	display of this computer program or the associated audiovisual work
 *	is strictly prohibited.
 *
 *	======================================================================
 *	$Author: petrick $						$Date: 1996/11/25 04:10:06 $
 *	$Revision: 3.9 $						$Locker:  $
 *	======================================================================
 *	Change Log:
 *
 *	$Log: game.h,v $
 * Revision 3.9  1996/11/25  04:10:06  petrick
 * Got rid of Continue state.
 *
 * Revision 3.8  1996/11/16  13:03:30  geraci
 * removed some unused game states
 *
 * Revision 3.7  1996/11/01  12:24:03  geraci
 * added drone and human type defines
 *
 * Revision 3.6  1996/10/29  08:22:12  petrick
 * Added 'continue' state.
 *
 * Revision 3.5  1996/10/17  08:22:17  petrick
 * Made Game state a typedef so gdb would know the names of the states.
 *
 * Revision 3.4  1996/10/10  05:24:08  petrick
 * Goet rid of some unused stuff.
 *
 * Revision 3.3  1996/09/26  00:22:33  gray
 * Killed ztypes.h
 *
 * Revision 3.2  1996/09/14  06:37:49  geraci
 * added game's protypes
 *
 * Revision 3.1  1996/09/09  02:23:24  gray
 * JAMMA version
 *
 * Revision 2.1  1996/05/04  02:56:18  petrick
 * Focus 2 release
 *
 * Revision 1.10  1996/02/07  02:45:07  geraci
 * put defines from gstate.h into game.h; made PLAYGAME state
 *
 * Revision 1.9  1996/02/02  05:28:13  geraci
 * added true/false defines
 *
 * Revision 1.8  1996/01/24  22:43:28  geraci
 * needed ztypes.h to be stand-alone
 *
 * Revision 1.7  1996/01/20  05:00:58  petrick
 * Cleanup.
 *
 * Revision 1.6  1996/01/19  02:57:22  geraci
 * got rid of sndcall.h and added some useful equates
 *
 *
 *	======================================================================
 */


#ifndef _GAME_H
#define _GAME_H

#include "stdtypes.h"

#if 0
typedef struct {
    long  objindex;
    short objtype;
    short entry;
    short exit;
    long  spitch;	/* start pitch	*/
    long  dpitch;	/* delta pitch	*/
    long  syaw;		/* start yaw	*/
    long  dyaw;		/* delta yaw	*/
    long  slow;
} ROADSEG;

#define REC16 0
#endif

typedef enum
{ 
	Initialize,
    Allocate,
	Do_it,
	Undo_it,
	Deallocate,
	Cleanup,
	NUM_COMMANDS
} CmdFunc;

#define true		1
#define false		0

#define DRONE		1
#define HUMAN		2

typedef enum GState
{ 
	ATTRACT,
	TRKSEL,
	CARSEL,
	PLAYGAME,
	ENDGAME,
	GAMEOVER,
	HISCORE,
	PREPLAY,
	PREPLAY2,
	COUNTDOWN,
	NUM_GAME_STATES
} GState;

#include "Pro/game.pro"

#endif	/* _GAME_H */

