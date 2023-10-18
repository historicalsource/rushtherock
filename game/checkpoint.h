/*
 *	checkpoint.h - contains checkpoint defines and structures
 *
 *	Copyright 1996 Time Warner Interactive
 *	Unauthorized reproduction, adaptation, distribution, performance or 
 *	display of this computer program or the associated audiovisual work
 *	is strictly prohibited.
 *
 *	======================================================================
 *	$Author: gray $						$Date: 1997/09/15 22:27:35 $
 *	$Revision: 3.6 $						$Locker:  $
 *	======================================================================
 *	Change Log:
 *
 *	$Log: checkpoint.h,v $
 * Revision 3.6  1997/09/15  22:27:35  gray
 * Removed trackset hack - trackno now 0 to NTRACKS.
 *
 * Revision 3.5  1997/07/27  22:29:14  gray
 * Checkpoint set select for consumer track dev.
 *
 * Revision 3.4  1997/06/28  01:40:05  gray
 * Extended to 4 tracks.
 *
 * Revision 3.3  1997/01/30  20:33:19  geraci
 * four more difficulty settings
 *
 * Revision 3.2  1997/01/24  02:16:45  geraci
 * new checkpoint scheme
 *
 * Revision 3.1  1996/09/09  02:23:24  gray
 * JAMMA version
 *
 * Revision 2.3  1996/08/20  05:15:20  geraci
 * bonus times are now floats
 *
 * Revision 2.2  1996/08/17  07:10:14  geraci
 * up to three laps supported for bonus time
 *
 * Revision 2.1  1996/05/04  02:56:18  petrick
 * Focus 2 release
 *
 * Revision 1.6  1996/04/24  02:56:22  geraci
 * resurrection code removed
 *
 * Revision 1.5  1996/04/20  07:10:31  geraci
 * put resurrection move time here
 *
 * Revision 1.4  1996/03/20  03:47:15  geraci
 * supports two checkpoint bonus times and initial time for track
 *
 * Revision 1.3  1996/03/08  07:48:18  geraci
 * new checkpoint moob
 *
 * Revision 1.2  1996/03/06  10:32:57  petrick
 * Made more standalone.
 *
 * Revision 1.1  1996/02/02  05:31:30  geraci
 * Initial revision
 *
 *
 *	======================================================================
 */

#ifndef _CHECKPOINT_H
#define _CHECKPOINT_H

#include "stdtypes.h"
#include "modeldat.h"

#define CP_EDITOR		1
#define MAX_CPS			20

typedef struct		/* structure used to house initial data for checkpoints */
{
    F32 pos[3];
	S32 radius;     /* radius (not square-rooted) of check circle */
	S16 logical_cp;	/* value represents checkpoint order where checkpoint at finish is highest */
	F32 best_t[2];	/* extra time gained for crossing this checkpoint (also called par time) */
} InitCheckPoint;

typedef struct		/* structure used at run-time */
{
	F32 pos[3];
	F32 uvs[3][3];
	S32 radius;  	/* radius of check circle */
	S16 track_cent; /* track center index over checkpoint center */
	S16 logical_cp;	/* value represents checkpoint order where checkpoint at finish is highest */
	F32 bonus_t[2];	/* extra time gained for crossing this checkpoint (also called par time) */
} CheckPoint;

typedef struct			/* structure to hold data for each track */
{
	F32 start_time[8];	/* amount to give at start of race (8 difficulty levels) */
	F32	end_time[8];	/* target amount of time at end of race (8 levels) */
	S16 loop_chkpnt;	/* after passing last checkpoint in list, loop to this one */
	S16 finish_line;	/* checkpoint index of finish line */
	S16 before_finish;	/* checkpoint prior to finish line */
} Init_Track_Data;

typedef struct			/* structure to hold data for each track */
{
	F32 start_time[8];	/* amount to give at start of race (8 difficulty levels) */
	F32 end_time[8];	/* target amount of time at end of race (8 levels) */
	S16 loop_chkpnt;	/* after passing last checkpoint in list, loop to this one */
	S16 finish_line;	/* checkpoint index of finish line */
	S16 before_finish;	/* checkpoint prior to finish line */
	S16 number_of_laps;	/* actual number of laps for this race */
} Track_Data;

typedef struct
{
	CheckPoint  *chk_point;
	S16			num_checkpoints;
	S16			direction;
} Track;

/* only define CHKPOINT_DATA in module declaring checkpoints (ie. cpinit.c) */
#ifndef CHKPOINT_DATA
extern InitCheckPoint * const   InitCP_Track[];
extern const Init_Track_Data    init_track_data[];
#endif

#include "Pro/checkpoint.pro"

#endif	/* _CHECKPOINT_H */

