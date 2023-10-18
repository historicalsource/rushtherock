/*
 *	targets.h	-- Definitiions for dynamic target objects.
 *
 *	Copyright 1996 Time Warner Interactive.
 *	Unauthorized reproduction, adaptation, distribution, performance or 
 *	display of this computer program or the associated audiovisual work
 *	is strictly prohibited.
 *
 *	======================================================================
 *	$Author: gray $						$Date: 1997/10/06 07:50:13 $
 *	$Revision: 3.18 $						$Locker:  $
 *	======================================================================
 *	Change Log:
 *
 *	$Log: targets.h,v $
 * Revision 3.18  1997/10/06  07:50:13  gray
 * Now doing fog in a sane way.
 *
 * Revision 3.17  1997/09/30  02:20:53  grjost
 * Added new sounds.
 *
 * Revision 3.16  1996/11/24  06:10:55  petrick
 * Added Flag targets.
 *
 * Revision 3.15  1996/11/15  16:27:28  geraci
 * new time target for getting more time
 *
 * Revision 3.14  1996/11/11  19:34:47  petrick
 * Finish line checkpoint texture swap.
 *
 * Revision 3.13  1996/11/06  03:11:52  petrick
 * Gong and bush added.
 *
 * Revision 3.12  1996/11/01  23:28:04  petrick
 * Added crowds.
 *
 * Revision 3.11  1996/10/31  07:29:56  geraci
 * big crowd cheer with different radius
 *
 * Revision 3.10  1996/10/31  07:15:03  geraci
 * added big crowd cheer moob
 *
 * Revision 3.9  1996/10/30  04:41:34  petrick
 * Added some debug stuff.
 *
 * Revision 3.8  1996/10/24  09:21:18  petrick
 * Added marker targets.
 *
 * Revision 3.7  1996/10/17  01:25:21  geraci
 * crowd cheer and crowd clap moob objects added
 *
 * Revision 3.6  1996/10/16  07:58:20  geraci
 * three new targets which are used for moob sound effects (cable car, speaker, fire cracker)
 *
 * Revision 3.5  1996/10/02  03:34:53  petrick
 * Added dist field, changed frame to data, added Frontface.
 *
 * Revision 3.4  1996/10/01  08:45:22  petrick
 * Changes to support final fog handling.
 *
 * Revision 3.3  1996/09/24  22:58:02  petrick
 * Got rid of last vestiges of reusable objects.
 *
 * Revision 3.2  1996/09/21  04:15:56  petrick
 * Got rid of reusable objects.
 *
 * Revision 3.1  1996/09/09  02:23:24  gray
 * JAMMA version
 *
 * Revision 1.5  1996/08/18  03:31:10  gray
 * Removing ZOID stuff.
 *
 * Revision 1.4  1996/08/02  11:45:49  petrick
 * Added WindowB.
 *
 * Revision 1.3  1996/07/12  12:26:30  petrick
 * New car select changes, workaround hack to get all stacks working.
 *
 * Revision 1.2  1996/07/07  01:22:59  petrick
 * New car structure and knock down targets.
 *
 * Revision 1.1  1996/07/01  05:37:01  petrick
 * Initial revision
 *
 *
 *	======================================================================
 */
#ifndef _TARGETS_H_
#define _TARGETS_H_

#include "mb.h"
#include "config.h"
#include "modeldat.h"


/*===============================  DEFINES  =================================*/

enum TargetType 
{
	TARG_NONE = 0, TARG_CONE, TARG_METER, TARG_FENCE, TARG_POLE,
	TARG_TREE, TARG_CROWD, TARG_WINDOWA, TARG_WINDOWB, TARG_FOG,
	TARG_FRONTFACE, TARG_CABLECAR, TARG_SPEAKER, TARG_FIRECRACKER,
	TARG_SMALLHOOT, TARG_SMALLCLAP, TARG_MARKER, TARG_BIGCHEER,
	TARG_BIGCHEER2, TARG_GONG, TARG_BUSH, TARG_FINISH, TARG_TIME,
	TARG_FLAG, TARG_SIREN,
	NUM_TARGTYPES 
};

/* Fog Modes */
#define FOG_INIT       0
#define FOG_OFF        1
#define FOG_TINT       2
#define FOG_TRACK      3
#define FOG_TRACK_TINT 4

/*===============================  TYPEDEFS  ================================*/

typedef void (MBAnimFunc)(S32 mbidx, S32 dlidx,OBJANIM *oanim);

typedef struct TargObj
{
	struct TargObj	*next;		/*	Next obj in list. */
	S32				objnum;		/*	ZOID object number. */
} TargObj;

typedef struct Target
{
	struct Target	*next;		/*	next target in this group. */
	OBJANIM			*oanim;		/*	Animation data. */
	S32				objnum;		/*	Objnum for this target. */
	F32				pos[3];		/*	Position of this target. */
	F32				uv[3][3];	/*	Orientation matrix of target. */	
	F32				vel[3];		/*	Linear velocities. */
	F32				angV[3];	/*	Roll, pitch, yaw, velocities. */
	S16				data;		/*	Used for type-specific data storage. */
	S16				type;		/*	Index into gObjList of this target's representation. */
	F32				radius;		/*	Collision radius of target. */
	void			(*func)(struct Target *t, U32 data);	/* Kick off function. */
	VisFunc			*VisFunc;	/* 	Animation controller function. */
	BOOL			active;		/*	True if this target is still strikable. */
#if 1	/* DEBUG STUFF */
	char			name[15];	/*	Name for this type of target. */
	S16				otype;		/*	Original starting type of object. */
	S16				frames;		/*	Number of frames for this type. */
#endif
} Target;

typedef void (TargFunc)(struct Target *t, U32 data);

typedef struct TargetGroup
{
	S16				type;			/*	Type of targets in this group (special groups only). */
	S16				count;			/*	Number of targets in this group. */
	F32				radius;			/*	Radius of influence for this group. */
	F32				dist;			/*	Distance of this group from the camera. */	
	F32				pos[3];			/*	Position of the center of this group. */
	F32				min[3], max[3];	/*	Min and max bounding box for this group. */
	Target			*list;			/*	List of targets in this group. */
	Target			*last;			/*	Points to last target in this group. */
} TargetGroup;

#include "Pro/targets.pro"
#endif _TARGETS_H_
