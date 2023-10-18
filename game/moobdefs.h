
/*
 *	moobdefs.h -- include file for moobs.c
 *
 *	======================================================================
 *	$Author: geraci $						$Date: 1996/12/05 01:50:04 $
 *	$Revision: 3.3 $						$Locker:  $
 *	======================================================================
 *
 *	Change Log:
 *	$Log: moobdefs.h,v $
 * Revision 3.3  1996/12/05  01:50:04  geraci
 * limits miscellaneous moobs to 2
 *
 * Revision 3.2  1996/10/16  07:55:13  geraci
 * new structure for miscellaneous moob objects
 *
 * Revision 3.1  1996/09/09  02:23:24  gray
 * JAMMA version
 *
 * Revision 2.2  1996/07/19  09:56:16  geraci
 * changed drone priority
 *
 * Revision 2.1  1996/05/04  02:56:18  petrick
 * Focus 2 release
 *
 * Revision 1.6  1996/03/26  07:03:48  geraci
 * general purpose sound effects based on track and position
 *
 * Revision 1.5  1996/03/21  03:45:39  geraci
 * moved sound defines to moobs.c
 *
 * Revision 1.4  1996/03/12  11:04:53  geraci
 * bug fix
 *
 * Revision 1.3  1996/02/22  03:43:35  geraci
 * moved stuff from moobs.c
 *
 * Revision 1.2  1996/01/11  21:03:05  geraci
 * changed priorities to <= 7f instead of <= ff
 *
 *
 * Revision 1.13  1996/01/04  23:48:14  gray
 * Default to NOT LINKING for ZOID20 (for now).
 *
 * Revision 1.12  1995/12/23  01:31:00  petrick
 * Removed spurious include files.
 *
 * Revision 1.11  1995/12/14  01:28:37  petrick
 * LINKING was commented out by mistake.
 *
 * Revision 1.10  1995/12/12  00:49:34  petrick
 * Linking fixes and cleanup.
 *
 *
 *	======================================================================
 */

#ifndef __MOOBDEFS_H
#define __MOOBDEFS_H

#define LOWEST_PRIORITY    0       /* Lowest priority value */
#define HIGHEST_PRIORITY   0x7f    /* Highest priority value */

#define DRONE_PRIORITY     0x50    /* Drone MOOB priority (for now, picked arbitrarily) */ 

#define MAX_MOOB_ANGLE     0x01FF  /* Maximum moob angle value (512 units/360 degrees) */
#define MAX_ACTIVE_MOOBS   2       /* allow up to 2 active miscellaneous moobs */

typedef struct
{
    U8   sound_index;   /* update sound flag (init has index of sound slot */
    U32  next_update;	/* next time to update sound */
	U32  cur_t_inc;		/* current time increment (init has update percent) */
} SOUND_T;

typedef struct
{
	U16	sound_id_on;	/* sound to make when in sphere */
	U16	sound_id_off;	/* sound to make when outside sphere */
	U8	track_num;		/* track that sound lives on */
	S32 pos[3];			/* center of sphere on track */
	S32 radius_sq;		/* radius (squared) of sphere */
}	MISC_SOUND;

typedef struct
{
	S16  moob_sound;	/* sound call to use */
	S32  pos[3];		/* center of sphere on track */
	S32  radius_sq;		/* radius (squared) of sphere */
	BOOL moob_state;	/* true=moob active, false=moob not active */
}	MISC_MOOBS;

typedef struct
{
	U8 index;			/* index into misc sound tab */
	U8 flag;			/* true if sound is on, false if off */
}	MISC_SND;

#include "Pro/moobs.pro"
#endif
