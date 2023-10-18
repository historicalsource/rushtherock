/*
 *	visuals.h	-- Definitiions for visual effects attached to objects.
 *
 *	Copyright 1996 Time Warner Interactive.
 *	Unauthorized reproduction, adaptation, distribution, performance or 
 *	display of this computer program or the associated audiovisual work
 *	is strictly prohibited.
 *
 *	======================================================================
 *	$Author: gray $						$Date: 1996/12/11 11:34:50 $
 *	$Revision: 3.7 $						$Locker:  $
 *	======================================================================
 *	Change Log:
 *
 *	$Log: visuals.h,v $
 * Revision 3.7  1996/12/11  11:34:50  gray
 * Complete rework of skids. Now handled seperatly from Environs.
 *
 * Revision 3.6  1996/10/12  11:19:43  petrick
 * Got rid of object-based markers.
 *
 * Revision 3.5  1996/10/11  10:09:31  petrick
 * Added marker flag.
 *
 * Revision 3.4  1996/10/09  05:45:57  petrick
 * Cleanup.
 *
 * Revision 3.3  1996/09/17  15:07:23  petrick
 * Got rid of some stuff from the hud and cleaned up more.
 *
 * Revision 3.2  1996/09/12  10:52:45  petrick
 * Made relfections permanent.
 *
 * Revision 3.1  1996/09/09  02:23:24  gray
 * JAMMA version
 *
 * Revision 2.11  1996/09/06  02:46:34  petrick
 * Defined REFLECTIONS.
 *
 * Revision 2.10  1996/09/06  02:45:10  petrick
 * Removed dis.h
 *
 * Revision 2.9  1996/09/06  02:44:11  petrick
 * Added dis.h to includes.
 *
 * Revision 2.8  1996/09/06  02:41:47  petrick
 * Reflective windows.
 *
 * Revision 2.7  1996/07/31  12:07:32  petrick
 * Leaner HUD.
 *
 * Revision 2.6  1996/07/08  23:10:28  geraci
 * backward compatible for zoid
 *
 * Revision 2.5  1996/07/08  08:24:47  petrick
 * Changed appearance and car body visual handling.
 *
 * Revision 2.4  1996/07/07  01:22:59  petrick
 * New car structure and knock down targets.
 *
 * Revision 2.3  1996/07/01  05:38:24  petrick
 * Added needed include file.
 *
 * Revision 2.2  1996/05/30  06:01:19  petrick
 * Made hulkiness and translucency part of appearance.
 *
 * Revision 2.1  1996/05/04  02:56:18  petrick
 * Focus 2 release
 *
 * Revision 1.6  1996/04/30  03:40:56  petrick
 * Added wheels, shadows.
 *
 * Revision 1.5  1996/04/17  00:00:39  petrick
 * Added timeStamp and next fields.
 *
 * Revision 1.4  1996/04/10  00:08:24  petrick
 * Added smoke and arrows.
 *
 * Revision 1.3  1996/04/02  11:52:37  petrick
 * Added cleanup code.
 *
 * Revision 1.2  1996/03/28  03:14:27  petrick
 * Added sparks.
 *
 * Revision 1.1  1996/03/25  19:45:31  petrick
 * Initial revision
 *
 *
 *	======================================================================
 */
#ifndef _VISUALS_H_
#define _VISUALS_H_

#include "config.h"
#include "fmath.h"


/*===============================  DEFINES  =================================*/


enum VisualTypes { VIS_BLAST = 0, VIS_BRAKES, VIS_LSPARK, VIS_RSPARK, VIS_BSPARK,
				   VIS_TIRE1, VIS_TIRE2, VIS_TIRE3, VIS_TIRE4, 
				   VIS_SHADOW, 
				   VIS_QUAD0, VIS_QUAD1, VIS_QUAD2, VIS_QUAD3, VIS_QUAD4,
				   VIS_WINDOW0, VIS_WINDOW1, VIS_WINDOW2, VIS_WINDOW3, 
				   VIS_FRAME,
				   NUM_VISUALS };


/*===============================  TYPEDEFS  ================================*/

/*	Used for visual overlays of objects. */
typedef struct Visual
{
	struct Visual *next;			/* Next node in linked list. */
	S32			index;				/* Index of animation sequence. */
	S32			objnum;				/* Object number of overlay. */
	U32			slot;				/* Slot number of the visual owner's car. */
	U32			data;				/* Used to store arbitrary data. */
	U32			timeStamp;			/* Used to set last time updated. */
	void 		(*func)(struct Visual *v,S16 op);	/* Controller function. */
} Visual;

typedef struct Skid
{
	struct	Skid *prev;	/* Links for GSkidFree and gSkidList. */
	struct	Skid *next;	/* Links for GSkidFree and gSkidList. */
	U32	lastTime;	/* IRQTIME of last update. */
	S32	objnum;		/* Mathbox polygon index. */
	F32	pos[3];		/* Skid center. Used for removing most distant skid. */
	U08	xlu;		/* skid translucency level (0x00 to 0xff). */
} Skid;

typedef struct NewSkid
{
	Skid	*skid;			/* Unlinked Skid for creation. skid==nil if no skid in progress. */
	F32		start[3], end[3];	/*	Start/end positions of poly. */
	F32		lensq;		/* Skid length squared. lensq=0.0 when initally created.  */
	F32		dir[3];		/* Skid direction vector. Valid if lensq > 0.0. */
	F32		vert[4][3];	/* Skid polygon verticies. (0,1=start 2,3=end) */
} NewSkid;

typedef void (VisFunc)(struct Visual *v, S16 op);

#include "Pro/visuals.pro"
#endif _VISUALS_H_
