/*
 *	targets.c	-- Functions for handling collidable objects.
 *
 *	Copyright 1996 Time Warner Interactive.
 *	Unauthorized reproduction, adaptation, distribution, performance or 
 *	display of this computer program or the associated audiovisual work
 *	is strictly prohibited.
 *
 *	======================================================================
 *
 *	This stuff is set up by entering a comment in the comment field for
 *	an object or polygon in MultiGen.  The format of the comment is:
 *
 *		ANIM name p1 p2
 *
 *	where:	name 	= Name of the type of Animation or object.
 *			p1,p2	= User defined parameters.
 *
 *	The tools process the comment fields and extract lines like the above,
 *	and then the linker uses info contained in another file (specified by
 *	a linker option) to map the name to an id number.  The format of that
 *	file is:
 *	
 *		[OANIM|PANIM] name type u1 u2
 *
 *	where:	name 	= the same as above.
 *			type	= the function type to assign to that name.
 *			u1,u2	= more user defined parameters. (found in odata[0] and [1])
 *			OANIM	= Object animation.
 *			PANIM	= Polygon animation.
 *
 *	All the above serves to set up an OBJANIM struct for each animation.
 *	The tie-in to the code occurs when loading a world.  Before loading,
 *	call MBOX_ResetModels(funcs,numfuncs) which maps the above type to a
 *	function from the 'funcs' list.  During a LoadWorld, 'type' is used as 
 *	an index into the 'funcs' list, and the resulting func is called once
 *	only, but this gives you a chance to get the object/poly's number and
 *	do with it what you will.
 *
 *	======================================================================
 *	$Author: gray $						$Date: 1997/10/06 07:48:50 $
 *	$Revision: 3.71 $						$Locker: gray $
 *	======================================================================
 *	Change Log:
 *
 *	$Log: targets.c,v $
 * Revision 3.71  1997/10/06  07:48:50  gray
 * Completely rewrite fog stuff.
 *
 * Revision 3.70  1997/09/30  02:20:38  grjost
 * Added new sounds.
 *
 * Revision 3.69  1997/09/18  01:15:48  grjost
 * Turned off SIREN MOOB
 *
 * Revision 3.68  1997/09/15  22:30:23  gray
 * Removed trackset hack - trackno now 0 to NTRACKS.
 *
 * Revision 3.67  1997/09/05  22:39:21  gray
 * debug switch 0 is now debug switch 15.
 *
 * Revision 3.66  1997/08/22  01:35:52  gray
 * Experimenting with static fog for tracks 5-8.
 *
 * Revision 3.65  1997/05/13  21:07:48  gray
 * Removed UV from game car data (now everything uses dr_uvs).
 *
 * Revision 3.64  1997/05/11  03:37:57  gray
 * Cleaning model interface.
 *
 * Revision 3.63  1997/02/21  00:10:00  geraci
 * time box targets can no longer give out more time than regular game
 *
 * Revision 3.62  1997/01/30  20:34:21  geraci
 * added names to targets that weren't already setting them
 *
 * Revision 3.61  1996/12/17  04:01:55  geraci
 * fixed breakaway fence
 *
 * Revision 3.60  1996/12/13  10:19:33  gray
 * Pass targets over the link again.
 *
 * Revision 3.59  1996/12/12  10:02:45  geraci
 * flags wave again
 *
 * Revision 3.58  1996/12/12  09:43:04  geraci
 * targets hit in game task
 *
 * Revision 3.57  1996/12/07  22:13:48  geraci
 * Disable TargetCheck.
 *
 * Revision 3.56  1996/12/06  17:30:28  geraci
 * only collide with your targets
 *
 * Revision 3.55  1996/12/04  05:20:25  geraci
 * tidied up target pole routine
 *
 * Revision 3.54  1996/12/03  06:15:56  petrick
 * Flags are always on.
 *
 * Revision 3.53  1996/11/27  01:50:05  petrick
 * Updated to work with objnum == 0.
 *
 * Revision 3.52  1996/11/25  06:34:49  petrick
 * Turned off debug msgs.
 *
 * Revision 3.51  1996/11/25  04:18:24  petrick
 * Made fences not reset, flags and crowds rotatable and not triggered by drones.
 *
 * Revision 3.50  1996/11/24  10:52:53  petrick
 * Got rid of setmintexbase call.
 *
 * Revision 3.49  1996/11/24  06:17:06  petrick
 * Removed flag woman, added flags.
 *
 * Revision 3.48  1996/11/21  13:52:20  petrick
 * Added model tracing.
 *
 * Revision 3.47  1996/11/18  12:40:03  petrick
 * Upped max targets to 300.
 *
 * Revision 3.46  1996/11/17  13:29:17  petrick
 * Trimmed down excess memory usage.
 *
 * Revision 3.45  1996/11/15  16:27:06  geraci
 * new time target for getting more time
 *
 * Revision 3.44  1996/11/14  01:32:08  petrick
 * Inits fog group to nil every new game.
 *
 * Revision 3.43  1996/11/13  11:29:53  petrick
 * Turned off debug.
 *
 * Revision 3.42  1996/11/11  19:34:47  petrick
 * Finish line checkpoint texture swap.
 *
 * Revision 3.41  1996/11/11  12:20:28  petrick
 * Deactivated flag girl.
 *
 * Revision 3.40  1996/11/10  14:08:26  petrick
 * Fog now updated in game task.
 *
 * Revision 3.39  1996/11/09  13:12:01  petrick
 * Added flag women.
 *
 * Revision 3.38  1996/11/07  05:06:21  geraci
 * made bush sound work correctly, disabled gong for now
 *
 * Revision 3.37  1996/11/06  03:12:31  petrick
 * Gong and bush added.
 *
 * Revision 3.36  1996/11/05  01:35:04  petrick
 * Changed cone to cone1 for consistancy, fixed vanished cone bug.
 *
 * Revision 3.35  1996/11/03  05:57:34  geraci
 * split up debug switches and regular game switches
 *
 * Revision 3.34  1996/11/02  10:56:02  petrick
 * Crowds are supposed to have 4 frames.
 *
 * Revision 3.33  1996/11/02  10:49:14  petrick
 * Crowds version 2.
 *
 * Revision 3.32  1996/11/02  03:17:29  petrick
 * Added one more frame to windowB animation.
 *
 * Revision 3.31  1996/11/01  23:27:46  petrick
 * Added crowds.
 *
 * Revision 3.30  1996/10/31  08:04:59  geraci
 * new big cheer with different radius
 *
 * Revision 3.29  1996/10/31  07:36:57  geraci
 * big crowd cheer with different radius
 *
 * Revision 3.28  1996/10/30  04:41:51  petrick
 * Made targets handleing more generic.
 *
 * Revision 3.27  1996/10/25  11:05:31  petrick
 * Makes damn sure the fog is turned off.
 *
 * Revision 3.26  1996/10/25  09:55:01  petrick
 * Multiple people types.
 *
 * Revision 3.25  1996/10/24  09:30:00  petrick
 * Added marker targets and multiple tree types.
 *
 * Revision 3.24  1996/10/24  08:33:39  geraci
 * update to six announcer moobs can be chosen
 *
 * Revision 3.23  1996/10/23  03:32:48  petrick
 * Got rid of some unneeded code.
 *
 * Revision 3.22  1996/10/22  10:35:40  petrick
 * Knockdown fence added to track 1.
 *
 * Revision 3.21  1996/10/18  09:31:18  geraci
 * fixed switch in radii for moob targets
 *
 * Revision 3.20  1996/10/17  08:51:27  geraci
 * crowd clap and cheer are really there now
 *
 * Revision 3.19  1996/10/17  01:25:43  geraci
 * crowd cheer and crowd clap moob objects added
 *
 * Revision 3.18  1996/10/16  07:56:55  geraci
 * three new targets which are used for moob sound effects (cable car, speaker, fire cracker)
 *
 * Revision 3.17  1996/10/15  04:24:50  petrick
 * Turned off debug msg.
 *
 * Revision 3.16  1996/10/10  05:12:22  petrick
 * Made irqtime a true global in globals_h.
 *
 * Revision 3.15  1996/10/08  18:56:02  geraci
 * lockup at end of game due to fog target is fixed
 *
 * Revision 3.14  1996/10/08  01:59:36  petrick
 * More debug for cones.
 *
 * Revision 3.13  1996/10/07  22:45:03  petrick
 * Added more debug msgs.
 *
 * Revision 3.12  1996/10/07  03:04:45  petrick
 * Fixed pole rotation.
 *
 * Revision 3.11  1996/10/05  10:57:35  petrick
 * Better fog handling.
 *
 * Revision 3.10  1996/10/04  10:44:49  petrick
 * Temporary tuning stuff for black fog.
 *
 * Revision 3.9  1996/10/03  09:22:52  petrick
 * Better fog, new knockdown poles, front-facing targets.
 *
 * Revision 3.8  1996/10/02  03:36:02  petrick
 * Temporary fog reworking.
 *
 * Revision 3.7  1996/10/01  08:44:58  petrick
 * Changes to support final fog handling.
 *
 * Revision 3.6  1996/09/28  02:50:29  petrick
 * Moved fog to a central location.
 *
 * Revision 3.5  1996/09/27  02:12:52  petrick
 * Resets Unlocked models only.
 *
 * Revision 3.4  1996/09/25  06:15:39  petrick
 * Temporary check-in to make Alan happy.
 *
 * Revision 3.3  1996/09/24  22:57:36  petrick
 * Got rid of last vestiges of reusable objects.
 *
 * Revision 3.2  1996/09/21  04:11:59  petrick
 * Got rid of reusable objects.
 *
 * Revision 3.1  1996/09/09  02:24:52  gray
 * JAMMA version
 *
 *	======================================================================
 */

#include "globals.h"
#include "carsnd.h"
#include "debug.h"
#include "dis.h"
#include "sounds.h"
#include "targets.h"
#include "visuals.h"
#include "attract.h"
#include "glide.h"
#include "moobdefs.h"

#include "Pro/sounds.pro"
#include "Pro/vecmath.pro"
#include "Pro/d3math.pro"
#include "Pro/math.pro"
#include "Pro/unitvecs.pro"

/*===============================  DEFINES  =================================*/

#define TDEBUG 			0
#define RESET_TARGETS	0					/*	1 == targets reset 10 sec after hit. */

#define MAX_TARG_GROUPS	30					/*	Maximum target groups per track. */
#define MAX_TARGETS		300					/*	Maximum targets per track. */

#define CAR_RADIUS		11.2				/*	Car bounding radius (collision.c) */

#define vcopy120( d, s )	{ d[0] = s[1]; d[1] = -(s[2]); d[2] = s[0]; }

/*==============================  TYPEDEFS  ================================*/


/*==============================  PROTOTYPES  ===============================*/

static Target 		*AddToAnimList( OBJANIM *oanim, S32 mbidx, S32 dlidx );
static BOOL 		OverlapTarget(S16 slot, F32 pos[3], F32 radius, F32 *dist);
static BOOL 		CheckTargetReset( Target *t, Visual *v, S32 msec );
static BOOL 		AnimateGeneric(Visual *v, S16 op, S32 msec, BOOL frontface);
static F32 			distance(S16 slot, F32 pos[3]);
static void 		MakeFogTable(float density, S32 clip, U32 color);

static TargFunc		StartCone,   StartKnockdown, StartMarker, StartGeneric;
static TargFunc		give_more_time;
static VisFunc		AnimatePole, AnimateTree,  AnimateCrowd, AnimateFlag;
static VisFunc		AnimateCone, AnimateMeter, AnimateFence, AnimateWindow;
static MBAnimFunc	TargetCone,  TargetMeter,  TargetFence,  TargetWindowA, TargetWindowB;
static MBAnimFunc	TargetPole,  TargetTree,   TargetCrowd, TargetFinish;
static MBAnimFunc	TargetFog,   TargetFrontFace, TargetVoid, TargetMarker;
static MBAnimFunc	TargetCableCar, TargetSpeaker, TargetFireCracker;
static MBAnimFunc	TargetSmallHoot, TargetSmallClap, TargetBigCheer;
static MBAnimFunc	TargetBigCheer2, TargetGong, TargetWoman, TargetTime, TargetFlag;
static MBAnimFunc	TargetSiren;

/*===============================  GLOBALS  =================================*/

Target				gTargetList[MAX_TARGETS];		/*	List of targets for entire track. */
TargetGroup 		*gFogGroup;						/*	Current active fog group. */
U32 				gFinishObject[10];				/*	Object number of the finish line. */
S32 				gFinishPoly[10];				/*	Object number of the finish line. */
S32 				num_finish_polys;				/*	Object number of the finish line. */

static TargetGroup	gTargetGroups[MAX_TARG_GROUPS];	/*	List of groups for this entire track. */
static S16			gGroupCount;					/*	Number of groups for this track. */
static S16			gTargCount;						/*	Number of targets for this track. */

static GrFog_t		fogTable[GR_FOG_TABLE_SIZE];
static F32			lastDensity;
static U32			lastClip, lastColor;

static const MBOXANIMFUNC RTanimation[NUM_TARGTYPES]= 
{
	{TargetVoid},
	{TargetCone},
	{TargetMeter},
	{TargetFence},
	{TargetPole},
	{TargetTree},
	{TargetCrowd},
	{TargetWindowA},
	{TargetWindowB},
	{TargetFog},
	{TargetFrontFace},
	{TargetCableCar},
	{TargetSpeaker},
	{TargetFireCracker},
	{TargetSmallHoot},
	{TargetSmallClap},
	{TargetMarker},
	{TargetBigCheer},
	{TargetBigCheer2},
	{TargetGong},
	{TargetWoman},
	{TargetFinish},
	{TargetTime},
	{TargetFlag},
	{TargetSiren},
};


/*===============================  EXTERNS  =================================*/

extern const F32 	unituvs[3][3];
extern const F32 	zerovector[3];
extern U32			play_time;
extern U32			time_to_be_given, total_time_given, total_time_box_time;
extern void frwtobod(F32 *vprime, F32 *v, F32 *uvp);

/* ----------------------------------------------------------------------------- *
 *	Handles target list setup for the current track.  
 *	Called right before a track is loaded.
 * ----------------------------------------------------------------------------- */
void SetupTargets(S16 track)
{
	S16	i,j;

	MBOX_ResetUnlockedModels(RTanimation,NUM_TARGTYPES);
#if TRACE_MODELS
	MSG("--- MBOX_ResetUnlockedModels(targ)");
#endif

	/*	Setup targets. */
	gTargCount		= 0;
	num_finish_polys= 0;
	for (i=0; i<MAX_TARGETS; ++i)
		{
		gTargetList[i].next		= nil;
		gTargetList[i].oanim	= nil;
		gTargetList[i].objnum	= -1;
		gTargetList[i].type		= -1;
		gTargetList[i].otype	= -1;
		gTargetList[i].active	= false;
		gTargetList[i].name[0]	= 0;
		gTargetList[i].frames	= 0;
		}

	/*	Setup groups. */
	gGroupCount		= 0;
	gFogGroup		= nil;
	for (i=0; i<MAX_TARG_GROUPS; ++i)
		{
		gTargetGroups[i].type		= 0;
		gTargetGroups[i].count		= 0;
		gTargetGroups[i].list 		= nil;
		gTargetGroups[i].last 		= nil;
		gTargetGroups[i].radius 	= 0.0;
		gTargetGroups[i].dist	 	= 9999999.0;
		for (j=0; j<3; ++j)
			gTargetGroups[i].pos[j] = gTargetGroups[i].min[j] = gTargetGroups[i].max[j]= 0.0;
		}
}	/*	end SetupTargets */


/* ----------------------------------------------------------------------------- */
void SetTargets(void)
{
	S32 	i;
	U32		flags;
	Target	*t;

	for (t = gTargetList, i=0; i<gTargCount; ++i, ++t)
		if (t->objnum >= 0 && t->type >= TARG_NONE)
			{
			ZOID_SetObjectDef( t->objnum, gObjList[t->otype] );
			if (t->otype == OBJ_METER1)
				{
				flags = ZOID_ObjectFlags(t->objnum);
				ZOID_SetObjectFlags(t->objnum, flags|ZATR_FRONTFACE);
				}
			}
#if TDEBUG > 2
{
TargetGroup *g;
	for (g = &gTargetGroups[i=0]; i<gGroupCount; ++g,++i)
		if (g->list)
			MSG( "Group %ld used, %ld targets", (long)i, (long)g->count);

}
MSG( "Total targets %ld Total groups %ld", gTargCount, gGroupCount );
#endif
}	/* end SetTargets */


/* ----------------------------------------------------------------------------- */
static Target *AddToAnimList( OBJANIM *oanim, S32 mbidx, S32 dlidx )
{
	Target 		*t;
	TargetGroup *g;
	S16			i,j, group;
	F32			tr;

	/*	Add to list of targets.  Complain if there's too many. */
	if (gTargCount >= MAX_TARGETS)
		{
		static BOOL sentMsg;
		if (!sentMsg)
			EMSG( "Warning: too many targets, max is %ld", (long)MAX_TARGETS );
		sentMsg = 1;
		return nil;
		}

	t 			= &gTargetList[gTargCount++];
	t->oanim	= oanim;
	t->data		= -1;
	t->next		= nil;
	t->active	= true;
	t->objnum	= dlidx;
	t->radius	= tr = MBOX_GetObjectRadius(mbidx)*2.0;
	t->pos[0]	= ObjInstanceList[dlidx].mat[3][2];
	t->pos[1]	= ObjInstanceList[dlidx].mat[3][0];
	t->pos[2]	= -ObjInstanceList[dlidx].mat[3][1];

	for (i=0; i<3; ++i)
		for (j=0; j<3; ++j)
			t->uv[i][j] = ObjInstanceList[dlidx].mat[i][j];

	/*	Now add the target to a group.  The groups are set up within Multigen, 
	 *	so the odata field has a reference to which group it belongs. */
	group = oanim->odata[0];
	if (group < 0 || group >= MAX_TARG_GROUPS)
		{
		EMSG( "Warning: bad group (%ld) for object %s, index %ld, max:%ld", 
			  (long)group, MBOX_GetObjectName(mbidx), (long)oanim->odata[1], 
			  (long)MAX_TARG_GROUPS );
		return nil;
		}
	else if (group+1 > gGroupCount)
		gGroupCount = group+1;

#if TDEBUG > 4
 MSG( " Added t:%ld, T.Count:%ld, group %ld, g.count:%ld",
	  (long)dlidx, (long)gTargCount, (long)group, gGroupCount);
#endif
	g	= &gTargetGroups[group];
	if (!g->list)
		{
		/* With the first entry, set size and position info directly. */
		g->list 	= g->last = t;
		g->radius	= tr;
		for (i=0; i<3; ++i)
			{
			g->pos[i] = t->pos[i];
			g->min[i] = g->pos[i] - g->radius;
			g->max[i] = g->pos[i] + g->radius;
			}
		}
	else
		{
		/*	Subsequent targets need special handling to correctly determine the 
		 *	group's bounds. */
		g->last->next	= t;
		g->last 		= t;

		/*	Recalculate the group's size and position. */
		for (i=0; i<3; ++i)
			{
			if (g->min[i] > t->pos[i] - tr)
				g->min[i] = t->pos[i] - tr;
			if (g->max[i] < t->pos[i] + tr)
				g->max[i] = t->pos[i] + tr;
			g->pos[i] = (g->max[i] + g->min[i]) / 2.0;

			if (g->radius*2.0 < g->max[i] - g->min[i])
				g->radius = (g->max[i] - g->min[i]) / 2.0;
			}
		}
	++g->count;
	return t;
}	/*	end AddToAnimList */


/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Routine called from game task when object has been hit
*         Upon entry: m -> pointer to model slot to check against
*                     t -> pointer to object structure
*         Upon exit: returned -> true = object inside car
*                                false = object outside car                  */
BOOL car_inside_target (MODELDAT *m, Target *t)
{

	F32      pos[3], pos2[3], delta[3];
	S16      slot = m->net_node;
	CAR_DATA *gc = &game_car[slot];

	/* get target pos in display coord. */   
	veccopyM2D(t->pos, pos);

	/* save offset in relation to car in pos[] */
	vecsub (pos, gc->dr_pos, delta);
	frwtobod (delta, pos2, gc->dr_uvs[0]);

	return ((fabs(pos2[2]) < 7) && (fabs(pos2[0]) < 3));
}	/* end car_inside_target */


/* ----------------------------------------------------------------------------- *
 *	Checks the given car against the target list and fires off any animations
 *	that show a collision.
 * ----------------------------------------------------------------------------- */
void TargetCheck (S16 slot)
{
	TargetGroup *g, *fogGroup;
	Target		*t;
	BOOL		vis, overlap;
	F32			minDist;
	S16			i,j, type;
	MODELDAT	*m = &model[slot];

#if TDEBUG > 1
static BOOL wasIn[MAX_TARG_GROUPS];
#endif


	/*	Check this car against all groups. */
	minDist 	= 1.0e16;
	fogGroup 	= nil;
	type 		= -1;
	for (g = &gTargetGroups[i=0]; i<gGroupCount; ++g,++i)
		{
		/*	Determine the distance of car from this group. */
		type = (g->list) ? g->list->oanim->udata[0] : -1;
		if (slot == this_node && type == TARG_FOG)
			{
			/* really distance squared. */
			g->dist = distance(this_node, g->pos);
			/* * 0.25 since radius is really diameter. */
			if((g->dist <= (g->radius * g->radius * 0.25)) && (g->dist < minDist))
				{
				minDist  	= g->dist;
				fogGroup 	= g;
				}
			overlap = 0;	/* dont check fog objects any further. FYJP. */
			}
		else
			overlap = OverlapTarget(slot, g->pos, g->radius, nil);

		/*	Check car against each group, and only look inside the group if car
		 *	and group intersect. */
		if (overlap)
			{
#if TDEBUG > 1
			if (slot == this_node && !wasIn[i])
				{
				int j;
				for (j=0, t = g->list; t; t = t->next)
					++j;
				MSG( "Inside group %ld, size %ld", (long)i, (long)j);
				wasIn[i] = true;
				}
#endif
			/*	For crowds, being within the group radius is enough to start them going. */
			if (type == TARG_CROWD || type == TARG_FLAG)
				{
				for (t = g->list; t; t = t->next)
					if (t->active)
						t->func(t,(U32)(m-model));
				}

			/*	Car is within this group's radius, check individual targets */
			else
				for (t = g->list; t; t = t->next)
					if (t->active && OverlapTarget(slot, t->pos, t->radius, nil))
						{
						if (target_inside_box (slot, t) || 
							((t->type == OBJ_WINDOWA1 || t->type == OBJ_WINDOWB1) &&
							 OverlapTarget(slot, t->pos, t->radius/3.0, nil)) )
							{
							/*	We have a collision!  Send a message to everyone that this has
							 *	occurred. Animation gets started when the message is recv'd. */
#if TDEBUG > 0
							MSG( "Sending appearance 0x%04lx, %ld", (long)(t-gTargetList)|0x8000, (U32)(m-model));
#endif
#if 1
							PutEntityAppearancePDU( (U16)(t-gTargetList)|0x8000, PDU_VISUAL_BIT,
													(U32)(m-model), 0L );
#else
							TargetAppearance( (U16)(t-gTargetList), PDU_VISUAL_BIT,
													(U32)(m-model), 0L );
							/*	Mark the object inactive so no more collisions happen until the 
							 *	animation sequence says it's OK. */
							t->active = false;
#endif

							}
						}
			}
#if TDEBUG > 1
		else if (slot == this_node && wasIn[i])
			{
			MSG( "Out of group %ld", (long)i);
			wasIn[i] = false;
			}
#endif
		}

	if (slot == this_node)
		gFogGroup = fogGroup;

}	/*	end TargetCheck */


/* ----------------------------------------------------------------------------- *
 *	Checks the given model against a position and returns true if it overlaps.
 *  Dist, if present gets the gap value.
 * ----------------------------------------------------------------------------- */
static BOOL OverlapTarget(S16 slot, F32 pos[3], F32 radius, F32 *dist)
{
	F32       dsq,gap;
	F32       vec[3];
	CAR_DATA  *gc = &game_car[slot];
	MODELDAT  *m = &model[slot];

	/* check if car is collidable at this time */
	if (m->collidable == false)
		return false;

	/* see if bounding spheres intersect */
	vecsub(gc->RWR,pos,vec);	/*vector from other to us*/
	dsq = vec[0]*vec[0] + vec[1]*vec[1] + vec[2]*vec[2];
	radius += CAR_RADIUS;
	gap = dsq - radius*radius;

	if (dist)
		*dist = gap;
	return (gap <= 0.0);
}	/*	end OverlapTarget */


/* ----------------------------------------------------------------------------- *
 *	Handles Target appearance changes received from the net. 
 * ----------------------------------------------------------------------------- */
void TargetAppearance(S16 ID, U8 flag, U32 dataV, U32 appearA)
{
	Target *t = &gTargetList[ID];

	/*	Kick off whatever animation needs doing.
	 *	This has to be done regardless if the target is visible or not
	 *	in case it becomes visible during the animation and to have it
	 *	end up properly dealt with. */
#if TDEBUG > 2
	MSG( "Got Targ Appearnce %ld, v:%ld", (long)ID, dataV);
#endif
	if (t->active && t->func)
		{
		t->func(t,dataV);
		t->active = false;
		}
	
}	/*	end TargetAppearance */


/* ----------------------------------------------------------------------------- *
 *	Initiates a knockdown target sequence.
 *	slot	- index of car model that struck this target.
 * ----------------------------------------------------------------------------- */
static void   StartKnockdown( Target *t, U32 slot )
{
	Visual 		*v;
	MATRIX		tmat;
	F32			lv[3], pos[3];
	CAR_DATA  	*car;	
	S32			i,j;

	if (!(v=GrabEnvEntry()))
		return;

	v->index 	= 0;
	v->func		= t->VisFunc;
	v->data		= (U32)t;
	v->timeStamp= IRQTIME;
	t->active 	= false;

	/* Point target in direction of velocity. */
	car			= &game_car[slot];
	vcopy120( lv, car->RWV );
	PointInDir( lv, &tmat );
	for (i = 0; i < 3; i++)	
		for (j = 0; j < 3; j++)
			t->uv[i][j] = tmat.mat3.uvs[i][j];

	vcopy120( pos, t->pos );
	ZOID_UpdateObject(t->objnum, pos, t->uv );
	/*	Put it on the env list. */
	AddToEnvList(v);

	/*	Make some sound for the target. */
	target_sound( t, slot );

}	/*	end StartKnockdown */


/* ----------------------------------------------------------------------------- */
static void TargetVoid(S32 mbidx, S32 dlidx, OBJANIM *oanim) 
{
	return;
}	/*	end TargetVoid */


/* ----------------------------------------------------------------------------- */
static void TargetFence(S32 mbidx, S32 dlidx, OBJANIM *oanim) 
{
	Target *t = AddToAnimList( oanim, mbidx, dlidx );

	if (!t)
		return;

	t->type 	= t->otype 	= OBJ_FENCE1;
	t->frames	= 12;
	t->func		= StartGeneric;
	t->VisFunc	= AnimateFence;
	strcpy( t->name, "Fence" );

#if TDEBUG > 2
	MSG( "Added a %s", t->name);
#endif
}	/*	end TargetFence */


/* ----------------------------------------------------------------------------- *
 *	Makes parking meter sequence animate.
 * ----------------------------------------------------------------------------- */
static void AnimateFence(Visual *v, S16 op)
{
	/*	Since this is a one-shot, once it's done, no more anim needed. */
	if (AnimateGeneric( v, op, 5*ONE_SEC, false ))
		{
#if RESET_TARGETS
		Target 		*t	= (Target *)v->data;
		TargetGroup *g 	= &gTargetGroups[t->oanim->odata[0]];

#if TDEBUG > 2
		MSG( "Resetting %s", t->name);
#endif
		/*	Reset the markers in the fence groups too. */
		for (t = g->list; t; t = t->next)
			t->active 	= true;
#endif
		}
		
}	/*	end AnimateFence */


/* ----------------------------------------------------------------------------- */
static void TargetWindowB(S32 mbidx, S32 dlidx, OBJANIM *oanim) 
{
	Target *t = AddToAnimList( oanim, mbidx, dlidx );

	if (!t)
		return;

	t->type 	= t->otype = OBJ_WINDOWB1;
	t->frames	= 7;
	t->func		= StartGeneric;
	t->VisFunc	= AnimateWindow;
	strcpy( t->name, "WindowB" );

#if TDEBUG > 2
	MSG( "Added a %s", t->name);
#endif

}	/*	end TargetWindowB */


/* ----------------------------------------------------------------------------- */
static void TargetWindowA(S32 mbidx, S32 dlidx, OBJANIM *oanim) 
{
	Target *t = AddToAnimList( oanim, mbidx, dlidx );

	if (!t)
		return;

	t->type 	= t->otype = OBJ_WINDOWA1;
	t->frames	= 7;
	t->func		= StartGeneric;
	t->VisFunc	= AnimateWindow;
	strcpy( t->name, "WindowA" );

#if TDEBUG > 2
	MSG( "Added a %s", t->name);
#endif

}	/*	end TargetWindowA */


/* ----------------------------------------------------------------------------- *
 *	Makes a window sequence animate.
 * ----------------------------------------------------------------------------- */
static void AnimateWindow(Visual *v, S16 op)
{
	/*	Wait short time between frames. */
	if (op && v->timeStamp + v->index*ONE_SEC/16 > IRQTIME)
		return;

	AnimateGeneric( v, op, ONE_SEC, false );
		
}	/*	end AnimateWindow */


/* ----------------------------------------------------------------------------- */
static void TargetPole(S32 mbidx, S32 dlidx, OBJANIM *oanim) 
{
	Target *t = AddToAnimList( oanim, mbidx, dlidx );

	if (!t)
		return;

	switch (oanim->odata[1] % 10)
		{
		default:
		case 1:	t->type = OBJ_TLIGHT1;	break;	/*	Traffic light. */
		case 2:	t->type = OBJ_SLIGHT1;	break;	/*	Street light. */
		case 3:	t->type = OBJ_POLE1;	break;	/*	Telephone pole. */
		}

	YawUV( PI*(oanim->odata[1]/10.0)/180.0, t->uv);
	ZOID_UpdateObject(t->objnum, nil, t->uv );
	t->data		= t->otype = t->type;
	t->frames	= 2;
	t->func		= StartKnockdown;
	t->VisFunc	= AnimatePole;
	strcpy( t->name, "Pole" );

#if TDEBUG > 2 
	MSG( "Added a Pole rot %ld, type %ld, radius:%6.2f, Z=%f",(long)oanim->odata[1]/10, 
		 (long)oanim->odata[1]%10, t->radius, t->pos[1] );
#endif
}	/*	end TargetPole */


/* ----------------------------------------------------------------------------- *
 *	Makes pole sequence animate.
 * ----------------------------------------------------------------------------- */
static void AnimatePole(Visual *v, S16 op)
{
	AnimateGeneric( v, op, ONE_SEC, false );
}	/*	end AnimatePole */


/* ----------------------------------------------------------------------------- */
static void TargetFog(S32 mbidx, S32 dlidx, OBJANIM *oanim) 
{
	Target *t = AddToAnimList( oanim, mbidx, dlidx );

	if (!t  || (dlevels & SW_DEBUG15))
		return;

#if TDEBUG > 2 
	MSG( "Fog group: %ld color %08lx: max:%ld",(long)oanim->odata[0],oanim->odata[1]/1000L, 
		 oanim->odata[1]%256L );
#endif
	t->type 	= t->otype = -1;
	t->frames	= 0;
	t->func		= nil;
	t->VisFunc	= nil;
	strcpy( t->name, "Fog" );
}	/*	end TargetFog */


/* ----------------------------------------------------------------------------- */
static void TargetMarker(S32 mbidx, S32 dlidx, OBJANIM *oanim) 
{
	Target *t = AddToAnimList( oanim, mbidx, dlidx );

	if (!t)
		return;

#if TDEBUG > 2 
	MSG( "Added Marker: group %ld p2:%ld",
		 (long)oanim->odata[0],(long)oanim->odata[1]);
#endif
	t->type 	= t->otype = -1;
	t->frames 	= 0;
	t->data		= oanim->odata[1];
	t->func		= StartMarker;
	t->VisFunc	= nil;
	strcpy( t->name, "Marker" );
}	/*	end TargetMarker */


/* ----------------------------------------------------------------------------- *
 *	Initiates a Fence hit sequence.
 *	slot	- index of car model that struck this target.
 * ----------------------------------------------------------------------------- */
static void StartMarker( Target *t, U32 slot )
{
	Target		*rt;
	TargetGroup *g 		= &gTargetGroups[t->oanim->odata[0]];
	void *		vfunc 	= nil;

	/*	Look through the group and determine what the markers mark. */
	for (rt = g->list; rt; rt = rt->next)
		if (rt->func != StartMarker)
			vfunc = (void *)rt->VisFunc;

	if ((vfunc == AnimateCrowd || vfunc == AnimateFlag) && slot != this_node)
		return;
#if TDEBUG > 0
	MSG( "slot %ld  vfunc %p", slot, vfunc);
#endif

	for (rt = g->list; rt; rt = rt->next)
		{
		/*	If the actual target is found, trigger it. */
		if (rt->func != StartMarker)
			{
			if (rt->VisFunc == AnimateCrowd || rt->VisFunc == AnimateFlag)
				{
#if TDEBUG > 0
				MSG( "Kicking off %s", rt->name);
#endif
				if (rt->active)
					rt->func(rt,slot);
				}
			else
				{
#if TDEBUG > 0
				MSG( "Sending appearance 0x%04lx, %ld", (long)(rt-gTargetList)|0x8000, (U32)slot);
#endif
				/*	This might cause multiple starts for the same target. */
				PutEntityAppearancePDU( (U16)(rt-gTargetList)|0x8000, PDU_VISUAL_BIT, 
										(U32)slot, 0L );
				}
			}
		else
			/* Make all markers inactive. */
			/* if (vfunc != AnimateCrowd && vfunc != AnimateFlag) */
			rt->active 	= false;
		}

}	/*	end StartMarker */


/* ----------------------------------------------------------------------------- *
 *	Makes the given object front-facing.
 * ----------------------------------------------------------------------------- */
static void TargetFrontFace(S32 mbidx, S32 dlidx, OBJANIM *oanim) 
{
	U32 flags = ZOID_ObjectFlags(dlidx);
	ZOID_SetObjectFlags(dlidx, flags|ZATR_FRONTFACE);

}	/*	end TargetFrontFace */


/* ----------------------------------------------------------------------------- *
 *	Makes the marks the finish line object/poly.
 * ----------------------------------------------------------------------------- */
static void TargetFinish(S32 mbidx, S32 dlidx, OBJANIM *oanim) 
{
	gFinishPoly[num_finish_polys]		= oanim->id;
	gFinishObject[num_finish_polys++]	= mbidx;
}	/*	end TargetFinish */


/* ----------------------------------------------------------------------------- *
 *	Changes a checkpoint into the finish line.
 * ----------------------------------------------------------------------------- */
void ChangeFinishLine(void) 
{
	S32 tex, i;
	MBOX_FindTexture("FINISH",&tex);
	for (i=0; i<num_finish_polys; ++i)
		ZOID_ChangeTexture(gFinishObject[i],0,gFinishPoly[i],tex);
	num_finish_polys = 0;
}	/*	end ChangeFinishLine */


/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Tag cable car moobs
*         Upon entry: mbidx -> math box index
*                     dlidx -> local object index
*                     oanim -> pointer to object's data block                */

static void TargetCableCar (S32 mbidx, S32 dlidx, OBJANIM *oanim)
{
	add_moob_to_list (S_CABLECARBELL, ObjInstanceList[dlidx].mat[3], 400);
}	/*	end TargetCableCar */


/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Tag speaker tower moobs
*         Upon entry: mbidx -> math box index
*                     dlidx -> local object index
*                     oanim -> pointer to object's data block                */

static void TargetSpeaker (S32 mbidx, S32 dlidx, OBJANIM *oanim)
{
	S16  sound_code;

	if (oanim->odata[0] == 5)
	{
	    sound_code = S_PRISONMOOB;
	    return;
	}
	else
	{
	    switch (oanim->odata[1])
		{
		default:
		case 1:	sound_code = S_ANN1;	break;
		case 2:	sound_code = S_ANN2;	break;
		case 3:	sound_code = S_ANN3;	break;
		case 4:	sound_code = S_ANN4;	break;
		case 5:	sound_code = S_ANN5;	break;
		case 6:	sound_code = S_ANN6;	break;
		}
    }
	add_moob_to_list (sound_code, ObjInstanceList[dlidx].mat[3], 1000);
}	/* end TargetSpeaker */


/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Tag firecracker moobs
*         Upon entry: mbidx -> math box index
*                     dlidx -> local object index
*                     oanim -> pointer to object's data block                */

static void TargetFireCracker (S32 mbidx, S32 dlidx, OBJANIM *oanim)
{
	add_moob_to_list (S_FIRECRACKER, ObjInstanceList[dlidx].mat[3], 400);
}	/* end TargetFireCracker */


/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Tag small crowd cheering moob
*         Upon entry: mbidx -> math box index
*                     dlidx -> local object index
*                     oanim -> pointer to object's data block                */

static void TargetSmallHoot (S32 mbidx, S32 dlidx, OBJANIM *oanim)
{
	add_moob_to_list (S_SMALLHOOT, ObjInstanceList[dlidx].mat[3], 200);
}	/* end TargetSmallHoot */

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Tag small crowd clapping moob
*         Upon entry: mbidx -> math box index
*                     dlidx -> local object index
*                     oanim -> pointer to object's data block                */

static void TargetSmallClap (S32 mbidx, S32 dlidx, OBJANIM *oanim)
{
	add_moob_to_list (S_SMALLCLAP, ObjInstanceList[dlidx].mat[3], 200);
}	/* end TargetSmallClap */

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Tag large crowd cheering moob
*         Upon entry: mbidx -> math box index
*                     dlidx -> local object index
*                     oanim -> pointer to object's data block                */

static void TargetBigCheer (S32 mbidx, S32 dlidx, OBJANIM *oanim)
{
	add_moob_to_list (S_FINISHMOOB, ObjInstanceList[dlidx].mat[3], 800);
}	/* end TargetBigCheer */

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Tag large crowd cheering moob
*         Upon entry: mbidx -> math box index
*                     dlidx -> local object index
*                     oanim -> pointer to object's data block                */

static void TargetBigCheer2 (S32 mbidx, S32 dlidx, OBJANIM *oanim)
{
	add_moob_to_list (S_FINISHMOOB, ObjInstanceList[dlidx].mat[3], 600);
}	/* end TargetBigCheer2 */


/* ----------------------------------------------------------------------------- */
static void TargetGong(S32 mbidx, S32 dlidx, OBJANIM *oanim)
{
#if 0
	add_moob_to_list (S_CHINACYM, ObjInstanceList[dlidx].mat[3], 600);
#endif
}	/* end TargetGong */



/* ----------------------------------------------------------------------------- */
static void TargetTime(S32 mbidx, S32 dlidx, OBJANIM *oanim)
{
	Target *t = AddToAnimList( oanim, mbidx, dlidx );

	if (!t)
		return;

	t->type		= t->otype	= -1;
	t->frames	= 0;
	t->func		= give_more_time;
	t->VisFunc	= nil;
	t->data		= oanim->odata[1];
	strcpy( t->name, "Timer" );
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Tag siren moobs
*         Upon entry: mbidx -> math box index
*                     dlidx -> local object index
*                     oanim -> pointer to object's data block                */

static void TargetSiren (S32 mbidx, S32 dlidx, OBJANIM *oanim)
{
	add_moob_to_list (S_PRISONMOOB, ObjInstanceList[dlidx].mat[3], 800);
}

/* ----------------------------------------------------------------------------- */
static void give_more_time( Target *t, U32 slot )
{
	U32  bonus_time;

	t->active 	= false;

	if (num_humans == 1)
		{
		MBOX_HideObject (t->objnum, HIDE_ONLY);

		bonus_time = t->data * ONE_SEC;

#if 0
			{
			char buf[50];
			nsprintf (buf, 50, "%7ld %7ld %7ld ", time_to_be_given, total_time_given, total_time_box_time);
			txt_str (10, 40, buf, WHT_PALB);
			}
#endif

		/* if we're about to be over, give balance of time */
		if (total_time_given + bonus_time > time_to_be_given)
			bonus_time = time_to_be_given - total_time_given;

		if (bonus_time)
			SOUND (S_BEEP1);

		total_time_given += bonus_time;
		total_time_box_time += bonus_time;
		play_time += bonus_time;

#if 0
			{
			char buf[50];
			nsprintf (buf, 50, "%7ld %7ld %7ld ", time_to_be_given, total_time_given, total_time_box_time);
			txt_str (10, 41, buf, WHT_PALB);
			}
#endif
		}

}	/*	end give_more_time */

/* ----------------------------------------------------------------------------- */
static void TargetTree(S32 mbidx, S32 dlidx, OBJANIM *oanim) 
{
	static const S8		TreeFrames[]	= {         6,         3,         3,         3,         3};
	static const S16	TreeObject[]	= {OBJ_TREEA1,OBJ_TREEB1,OBJ_TREEC1,OBJ_TREED1,OBJ_TREEE1};

	Target *t = AddToAnimList( oanim, mbidx, dlidx );

	if (!t)
		return;

	t->data		= oanim->odata[1];
	if (t->data > 4)
		t->data = 0;

	t->type		= t->otype = TreeObject[t->data];
	t->frames	= TreeFrames[t->data];
	t->func		= StartKnockdown;
	t->VisFunc	= AnimateTree;
	sprintf( t->name, "Tree%ld", (long)t->data );

#if TDEBUG > 2
	MSG( "Added a %s", t->name);
#endif

}	/*	end TargetTree */


/* ----------------------------------------------------------------------------- *
 *	Makes tree sequence animate.
 * ----------------------------------------------------------------------------- */
static void AnimateTree(Visual *v, S16 op)
{
	AnimateGeneric(v,op,ONE_SEC,false);
		
}	/*	end AnimateTree */


/* ----------------------------------------------------------------------------- */
static void TargetWoman(S32 mbidx, S32 dlidx, OBJANIM *oanim) 
{
	return;
}	/*	end TargetWoman */


/* ----------------------------------------------------------------------------- */
static void TargetCrowd(S32 mbidx, S32 dlidx, OBJANIM *oanim) 
{
	static const S8		kFrames[]	= {         4,          4,          4};
	static const S16	kObject[]	= {OBJ_CROWDA1,OBJ_CROWDB1,OBJ_CROWDC1};

	Target *t = AddToAnimList( oanim, mbidx, dlidx );

	if (!t)
		return;

	t->data		= oanim->odata[1];

	YawUV( PI*(oanim->odata[1]/10.0)/180.0, t->uv);
	ZOID_UpdateObject(t->objnum, nil, t->uv );

	t->type		= t->otype = kObject[t->data];
	t->frames	= kFrames[t->data];
	t->func		= StartGeneric;
	t->VisFunc	= AnimateCrowd;
	sprintf( t->name, "Crowd%ld", (long)t->data );

#if TDEBUG > 1
	MSG( "Added a %s", t->name);
#endif
}	/*	end TargetCrowd */


/* ----------------------------------------------------------------------------- *
 *	Makes Crowd sequence animate.
 * ----------------------------------------------------------------------------- */
static void AnimateCrowd(Visual *v, S16 op)
{
	Target	*t;
	S16		type;
	U32		flags;

	/*	Handle cleanup call. */
	if (op == 0)
		{
		ReleaseEnviron(v,true);
		return;
		}
#if 1
	/*	Wait short time between frames. */
	if (v->timeStamp + ONE_SEC/8 > IRQTIME)
		return;
	v->timeStamp = IRQTIME;
#endif

	/*	When done animating, loop it around. */
	t 	 = (Target *)v->data;
	if (++v->index >= t->frames)
		{
		TargetGroup *g 	= &gTargetGroups[t->oanim->odata[0]];

#if TDEBUG > 2
		MSG( "Resetting %s", t->name);
#endif

		ReleaseEnviron(v,true);

		/*	Reset the target. */
		t->type 	= t->otype;
		ZOID_SetObjectDef( t->objnum, gObjList[t->type] );
		flags 		= ZOID_ObjectFlags(t->objnum);
		ZOID_SetObjectFlags(t->objnum, flags & ~ZATR_FRONTFACE);

		/*	Reset the markers in the flag group too. */
		for (t = g->list; t; t = t->next)
			t->active 	= true;
		return;
		}

	if ((type = t->otype + v->index) != t->type)
		{
#if TDEBUG > 1
		MSG( "%s %ld, type %ld",t->name, t->objnum, type );
#endif
		ZOID_SetObjectDef( t->objnum, gObjList[type] );
		t->type = type;
		flags 	= ZOID_ObjectFlags(t->objnum);
		ZOID_SetObjectFlags(t->objnum, flags & ~ZATR_FRONTFACE);
		}
		
}	/*	end AnimateCrowd */


/* ----------------------------------------------------------------------------- */
static void TargetFlag(S32 mbidx, S32 dlidx, OBJANIM *oanim) 
{
	static const S8		kFrames[]	= {        10,        10,        10};
	static const S16	kObject[]	= {OBJ_FLAGA1,OBJ_FLAGB1,OBJ_FLAGC1};

	Target *t = AddToAnimList( oanim, mbidx, dlidx );

	if (!t)
		return;

	t->data		= oanim->odata[1];
	YawUV( PI*(oanim->odata[1]/10.0)/180.0, t->uv);
	ZOID_UpdateObject(t->objnum, nil, t->uv );

	t->type		= t->otype = kObject[t->data % 3];
	t->frames	= kFrames[t->data % 3];
	t->func		= StartGeneric;
	t->VisFunc	= AnimateFlag;
	sprintf( t->name, "Flag%ld", (long)t->data % 3 );

	/*	Kick the flags off immediately. */
	StartGeneric( t, this_node );

	t->active 	= true;

#if TDEBUG > 0
	MSG( "Added a %s, degrees %ld", t->name, (long)t->data / 10L);
#endif
}	/*	end TargetFlag */


/* ----------------------------------------------------------------------------- *
 *	Makes Flag sequence animate.
 * ----------------------------------------------------------------------------- */
static void AnimateFlag(Visual *v, S16 op)
{
	Target	*t;
	S16		type;
	U32		flags;

	/*	Handle cleanup call. */
	if (op == 0)
		{
		ReleaseEnviron(v,true);
		return;
		}

#if 1
	/*	Wait short time between frames. */
	if (v->timeStamp + ONE_SEC/16 > IRQTIME)
		return;
	v->timeStamp = IRQTIME;
#endif

	/*	When done animating, loop it around. */
	t 	 = (Target *)v->data;
	if (++v->index >= t->frames)
#if 1	/*	Flags are always on */
		v->index = 0;
#else	/*	Make flags reset. */
		{
		TargetGroup *g 	= &gTargetGroups[t->oanim->odata[0]];

#if TDEBUG > 2
		MSG( "Resetting %s", t->name);
#endif

		ReleaseEnviron(v,true);

		/*	Reset the target. */
		t->type 	= t->otype;
		ZOID_SetObjectDef( t->objnum, gObjList[t->type] );

		/*	Reset the markers in the flag group too. */
		for (t = g->list; t; t = t->next)
			t->active 	= true;
		return;
		}
#endif
	if ((type = t->otype + v->index) != t->type)
		{
#if TDEBUG > 1
		MSG( "%s %ld, type %ld",t->name, t->objnum, type );
#endif
		ZOID_SetObjectDef( t->objnum, gObjList[type] );
		t->type = type;
		}
		
}	/*	end AnimateFlag */


/* ----------------------------------------------------------------------------- */
static void TargetMeter(S32 mbidx, S32 dlidx, OBJANIM *oanim) 
{
	Target *t = AddToAnimList( oanim, mbidx, dlidx );

	if (!t)
		return;

	t->type 	= t->otype = OBJ_METER1;
	t->frames	= 5;
	t->func		= StartKnockdown;
	t->VisFunc	= AnimateMeter;
	strcpy( t->name, "Meter" );
}	/*	end TargetMeter */


/* ----------------------------------------------------------------------------- *
 *	Makes parking meter sequence animate.
 * ----------------------------------------------------------------------------- */
static void AnimateMeter(Visual *v, S16 op)
{
	AnimateGeneric(v,op,ONE_SEC,true);
		
}	/*	end AnimateMeter */


/* ----------------------------------------------------------------------------- */
static void TargetCone(S32 mbidx, S32 dlidx, OBJANIM *oanim) 
{
	Target *t = AddToAnimList( oanim, mbidx, dlidx );

	if (!t)
		return;
#if TDEBUG > 2 
	MSG( "Added a Cone group %ld radius:%6.2f",(long)oanim->odata[0], t->radius );
#endif
	t->type 	= t->otype = OBJ_CONE1;
	t->frames	= 2;
	t->func		= StartCone;
	t->VisFunc	= AnimateCone;
	strcpy( t->name, "Cone" );
}	/*	end TargetCone */


/* ----------------------------------------------------------------------------- *
 *	Initiates a Cone hit sequence.
 *	slot	- index of car model that struck this target.
 * ----------------------------------------------------------------------------- */
static void StartCone( Target *t, U32 slot )
{
	Visual 		*v;
	MATRIX		tmat;
	F32			pos[3];
	CAR_DATA  	*car;	
	S16			i,j;

	if (!(v=GrabEnvEntry()))
		return;

	v->index 	= 0;
	v->func		= t->VisFunc;
	v->data		= (U32)t;
	v->timeStamp= IRQTIME;
	t->active 	= false;

	car			= &game_car[slot];
	t->vel[0]	= car->RWV[0] * 0.125;
	t->vel[1]	= car->RWV[1] * 0.125;
	t->vel[2]	= car->RWV[2] * 0.125;
	PointInDir( t->vel, &tmat );
	t->angV[0]  = 0.5;
	t->angV[1]  = 0.4;
	t->angV[2]  = 0.3;

	for (i = 0; i < 3; i++)		
		for (j = 0; j < 3; j++)
			t->uv[i][j] = tmat.mat3.uvs[i][j];
	vcopy120( pos, t->pos );
	ZOID_UpdateObject(t->objnum, pos, t->uv );

	/*	Put it on the env list. */
	AddToEnvList(v);

	/*	Play the cone hit sound. */
	target_sound( t, slot );

}	/*	end StartCone */


/* ----------------------------------------------------------------------------- *
 *	Makes a cone animate.
 * ----------------------------------------------------------------------------- */
static void AnimateCone(Visual *v, S16 op)
{
	S16		type,i;
	Target	*t;
	F32		dt = 0.15;
	F32 	pos[3];
	static const F32 gravity[3] = {0.0, -0.5, +0.25};

	/*	Handle cleanup call. */
	if (op == 0)
		{
		ReleaseEnviron(v,true);
		return;
		}

	t 	 = (Target *)v->data;

	/*	Update the position of the cone based on timestamp. */
	for (i=0; i<3; ++i)
		{
		t->vel[i] += gravity[i] * dt;
		t->pos[i] += t->vel[i] + gravity[i] * dt;
		}

	/* Should also scale by dt */
	RotateUV(t->angV, t->uv);
	vcopy120( pos, t->pos );
	ZOID_UpdateObject(t->objnum, pos, t->uv );

	type = ((v->timeStamp + 5*ONE_SEC) > IRQTIME) ? OBJ_CONE2 : t->otype;
	if (type != t->type)
		{
		ZOID_SetObjectDef( t->objnum, gObjList[type] );

		t->type 	= type;
		t->active 	= type == t->otype;
		
		/*	Since this is a one-shot, once it's changed back, no more anim needed. */
		if (v->timeStamp + ONE_SEC < IRQTIME)
			ReleaseEnviron(v,true);
		}

}	/*	end AnimateCone */


/* ----------------------------------------------------------------------------- *
 *	Checks if a target needs to be reset or released and does it if needed.
 *	Returns true if reset happened.
 * ----------------------------------------------------------------------------- */
static BOOL CheckTargetReset( Target *t, Visual *v, S32 msec )
{
	if (v->timeStamp + msec < IRQTIME)
		{
		ReleaseEnviron(v,true);
#if RESET_TARGETS
		/*	This stuff just resets the target (for debug). */
		t->type 	= t->otype;
        t->active 	= true;
		ZOID_SetObjectDef( t->objnum, gObjList[t->type] );
#if TDEBUG > 2
		MSG( "%s %ld, type %ld (reset)",t->name, t->objnum, t->type );
#endif
#endif
		return true;
		}
	return false;
}	/* end CheckTargetReset */


/* ----------------------------------------------------------------------------- *
 *	Initiates a generic hit sequence.
 *	slot	- index of car model that struck this target.
 * ----------------------------------------------------------------------------- */
static void StartGeneric( Target *t, U32 slot )
{
	Visual 		*v;

	if (!(v=GrabEnvEntry()))
		return;

	v->index 	= 0;
	v->func		= t->VisFunc;
	v->data		= (U32)t;
	v->timeStamp= IRQTIME;
	t->active 	= false;

	/*	Put it on the env list. */
	AddToEnvList(v);

	/*	Play the proper hit sound. */
	target_sound( t, slot );

}	/*	end StartGeneric */


/* ----------------------------------------------------------------------------- *
 *	Makes generic sequences animate.
 * ----------------------------------------------------------------------------- */
static BOOL AnimateGeneric(Visual *v, S16 op, S32 msec, BOOL frontface)
{
	Target	*t;
	S16		i, j, type;
	U16		id;
	U32		flags;

	/*	Handle cleanup call. */
	if (op == 0)
		{
		ReleaseEnviron(v,true);
		return true;
		}
#if 0
	/*	Wait short time between frames. */
	if (v->timeStamp + ONE_SEC/8 > IRQTIME)
		return false;
	v->timeStamp = IRQTIME;
#endif

	/*	When done animating, leave it knocked down. */
	t 	 = (Target *)v->data;
	if (++v->index >= t->frames)
		v->index = t->frames-1;

	if ((type = t->otype + v->index) != t->type)
		{
#if TDEBUG > 2
		MSG( "%s %ld, type %ld",t->name, t->objnum, type );
#endif
		ZOID_SetObjectDef( t->objnum, gObjList[type] );
		if (frontface)
			{
			flags = ZOID_ObjectFlags(t->objnum);
			ZOID_SetObjectFlags(t->objnum, flags & ~ZATR_FRONTFACE);
			for(i=0; i<3;++i) 
				for (j=0;j<3;++j) 
					t->uv[i][j] = unituvs[i][j];
			}
		t->type 	= type;
		}

	/*	Since this is a one-shot, once it's done, no more anim needed. */
	return CheckTargetReset(t,v,ONE_SEC);
		
}	/*	end AnimateGeneric */


/* ----------------------------------------------------------------------------- *
 *	Returns distance squared of target from position.
 * ----------------------------------------------------------------------------- */
static F32 distance(S16 slot, F32 pos[3])
{
	F32	vec[3];

	/* see if bounding spheres intersect */
	vec[0] = game_car[slot].dr_pos[2] - pos[0];
	vec[1] = game_car[slot].dr_pos[0] - pos[1];
	vec[2] = -game_car[slot].dr_pos[1] - pos[2];
	return vec[0]*vec[0] + vec[1]*vec[1] + vec[2]*vec[2];
}	/*	end distance */

#define FOG_HACK 1

extern U32 dbgFogColor[MAX_TRACKS];
extern F32 dbgFogMinDist[MAX_TRACKS];
extern F32 dbgFogMaxDist[MAX_TRACKS];
extern F32 dbgFogMaxLevel[MAX_TRACKS];

static S32 fogMode, last_fogMode, save_fogMode;
static U32 fogDensity;
static U32 fogColor;

/* SetFog is called any time to modify fog settings. */
void SetFog(S32 mode, U32 density, U32 color)
{

	/* sanity check data */
	if((mode < FOG_OFF) || (mode > FOG_TRACK_TINT))
		mode = FOG_OFF;

	if(density < 0)
		density = 0;
	else if(density > 0xFF)
		density = 0xFF;

	color |= 0xFFFFFF;

	fogMode = mode;
	fogDensity = density;
	fogColor = color;

	/* save for SW_DEBUG15 */
	save_fogMode = fogMode;
}


/* UpdateFog is called right before EndFrame to update 3dfx fog parameters. */
void UpdateFog(void)
{
	/* disable fog on switch 15 */
	if (dlevels & SW_DEBUG15)
		fogMode = FOG_OFF;
	else if(trailing_dedges & SW_DEBUG15)
		fogMode = save_fogMode;

	switch(fogMode)
	{
		default:
			fogMode = FOG_OFF;
			/* fall through if bogus fogMode. */
		case FOG_OFF:
			FogOff();
			break;
		case FOG_TINT:
			FogTint();
			break;
		case FOG_TRACK:
			FogTrack();
			break;
		case FOG_TRACK_TINT:
			FogTrackTint();
			break;
	}

	last_fogMode = fogMode;
}

void FogOff(void)
{
	S32 i;

	if(last_fogMode == FOG_OFF)
		return;

	for (i = 0; i < GR_FOG_TABLE_SIZE; i++)
		fogTable[i] = 0;

	ZoidBGColor = 0;
	grFogColorValue(0);
	grFogTable(fogTable);
	grFogMode(GR_FOG_DISABLE);
}

void FogTint(void)
{
	static U32 last_density, last_color;
	S32	i;

	if((last_fogMode == FOG_TINT) &&
	   (last_density == fogDensity) &&
	   (last_color == fogColor))
		return;

	fogTable[0] = (GrFog_t)0;
	for (i = 1; i < GR_FOG_TABLE_SIZE; i++)
		fogTable[i] = (GrFog_t)fogDensity;

	ZoidBGColor = fogColor;
	grFogColorValue( fogColor );
	grFogMode(GR_FOG_WITH_TABLE);
	grFogTable(fogTable);

	last_density = fogDensity;
	last_color = fogColor;
}



/* Normal fog for game and non-darkened attract demo. */
void FogTrack(void)
{
	static U32 last_fogcolor;
	static F32 last_mindist, last_maxdist, last_minfog, last_maxfog;
	U32 fogcolor;
	F32 mindist, maxdist, minfog, maxfog;
	U32 fgcolor, fgmax, fogr, fogg, fogb;
	F32 dist,blend,fglevel;

	fogcolor = dbgFogColor[trackno];
	mindist = dbgFogMinDist[trackno];
	maxdist = dbgFogMaxDist[trackno];
	minfog = 0.0;
	maxfog = dbgFogMaxLevel[trackno];

	if(gFogGroup && !demo_game)	/* combine track fog and pocket fog. */
		{
		fgcolor 	= (gFogGroup->list->oanim->odata[1]/1000) ? 0xDCFFFF : 0;
		fgmax		= (gFogGroup->list->oanim->odata[1] % 1000);
		if(fgcolor)	/* white fog. */
			{

			/*	Check distance from fog group for fog conditions. */
			dist = distance(this_node, g->pos);
			if (dist > 1.0)
				blend = 1.0 / xxsqrt(dist);
			else
				blend = 1.0;

			/*	Clip maximum fog table values according to distance from fog group.
			 *	The farther away, the more clipped and less dense the fog. */
			dist /= 10000.0;
			clip = fgmax - min(dist,fgmax);
			}
		else if(in_tunnel)	/* black fog in tunnel only. */
			{
			blend = 1.0 - gFogGroup->dist/(gFogGroup->radius * gFogGroup->radius * 0.25);
			if(blend < 0.0) blend = 0.0;
			else if(blend > 1.0) blend = 1.0;
			}
		else	/* dont do black fog outside tunnel. */
			blend = 0.0;

		if(blend > 0.0)
			{
			fogr = (U32)((((fogcolor>>16)&0xFF)*(1.0-blend))+(((fgcolor>>16)&0xFF)*blend))&0xFF;
			fogg = (U32)((((fogcolor>> 8)&0xFF)*(1.0-blend))+(((fgcolor>> 8)&0xFF)*blend))&0xFF;
			fogb = (U32)((((fogcolor    )&0xFF)*(1.0-blend))+(((fgcolor    )&0xFF)*blend))&0xFF;
			fogcolor = (fogr << 16) | (fogg << 8) | fogb;
			fglevel = ((F32)fgmax/255.0) * blend;
			minfog = minfog*(1.0-blend) + fglevel*blend;
			maxfog = maxfog*(1.0-blend) + fglevel*blend;
			}
		}

	if((last_fogMode == FOG_TRACK) &&
	   (last_fogcolor == fogcolor) &&
	   (last_mindist == mindist) &&
	   (last_maxdist == maxdist) &&
	   (last_minfog == minfog) &&
	   (last_maxfog == maxfog))
		return;

	MakeFogTable(density, clip, color );


/*	MakeFogRamp(fogcolor, mindist, maxdist, minfog, maxfog);*/

	last_fogcolor = fogcolor;
	last_mindist = mindist;
	last_maxdist = maxdist;
	last_minfog = minfog;
	last_maxfog = maxfog;
}

#if 0
/* Normal fog for game and non-darkened attract demo. */
void FogTrack(void)
{
	static U32 last_fogcolor;
	static F32 last_mindist, last_maxdist, last_minfog, last_maxfog;
	U32 fogcolor;
	F32 mindist, maxdist, minfog, maxfog;
	U32 fgcolor, fgmax, fogr, fogg, fogb;
	F32 dist,blend,fglevel;

	fogcolor = dbgFogColor[trackno];
	mindist = dbgFogMinDist[trackno];
	maxdist = dbgFogMaxDist[trackno];
	minfog = 0.0;
	maxfog = dbgFogMaxLevel[trackno];

	if(gFogGroup && !demo_game)	/* combine track fog and pocket fog. */
		{
		fgcolor 	= (gFogGroup->list->oanim->odata[1]/1000) ? 0xDCFFFF : 0;
		fgmax		= (gFogGroup->list->oanim->odata[1] % 1000);
		if(fgcolor)	/* white fog. */
			{
			blend = 1.0 - gFogGroup->dist/(gFogGroup->radius * gFogGroup->radius * 0.25);
			if(blend < 0.0) blend = 0.0;
			else if(blend > 1.0) blend = 1.0;
			/* do stupid stuff. FYJP */
			blend = blend * blend;
			blend = blend * blend;
			blend = blend * blend;
			blend = blend * blend;
			blend = blend * blend;
			}
		else if(in_tunnel)	/* black fog in tunnel only. */
			{
			blend = 1.0 - gFogGroup->dist/(gFogGroup->radius * gFogGroup->radius * 0.25);
			if(blend < 0.0) blend = 0.0;
			else if(blend > 1.0) blend = 1.0;
			}
		else	/* dont do black fog outside tunnel. */
			blend = 0.0;

		if(blend > 0.0)
			{
			fogr = (U32)((((fogcolor>>16)&0xFF)*(1.0-blend))+(((fgcolor>>16)&0xFF)*blend))&0xFF;
			fogg = (U32)((((fogcolor>> 8)&0xFF)*(1.0-blend))+(((fgcolor>> 8)&0xFF)*blend))&0xFF;
			fogb = (U32)((((fogcolor    )&0xFF)*(1.0-blend))+(((fgcolor    )&0xFF)*blend))&0xFF;
			fogcolor = (fogr << 16) | (fogg << 8) | fogb;
			fglevel = ((F32)fgmax/255.0) * blend;
			minfog = minfog*(1.0-blend) + fglevel*blend;
			maxfog = maxfog*(1.0-blend) + fglevel*blend;
			}
		}

	if((last_fogMode == FOG_TRACK) &&
	   (last_fogcolor == fogcolor) &&
	   (last_mindist == mindist) &&
	   (last_maxdist == maxdist) &&
	   (last_minfog == minfog) &&
	   (last_maxfog == maxfog))
		return;

	MakeFogRamp(fogcolor, mindist, maxdist, minfog, maxfog);

	last_fogcolor = fogcolor;
	last_mindist = mindist;
	last_maxdist = maxdist;
	last_minfog = minfog;
	last_maxfog = maxfog;
}
#endif

/* Used for darkening track in attract mode. */
void FogTrackTint(void)
{
	static U32 last_fogcolor;
	static F32 last_mindist, last_maxdist, last_minfog, last_maxfog;
	U32 fogcolor;
	F32 mindist, maxdist, minfog, maxfog;

	fogcolor = 0;
	mindist = dbgFogMinDist[trackno];
	maxdist = dbgFogMaxDist[trackno];
	minfog = fogDensity/255.0;
	maxfog = dbgFogMaxLevel[trackno];
	/* If normal track fades to full fog, fade to black at same distance. */
	/* Otherwise, bring max fog up to tint level. */
	if(maxfog < minfog)
		maxfog = minfog;

	if((last_fogMode == FOG_TRACK_TINT) &&
	   (last_fogcolor == fogcolor) &&
	   (last_mindist == mindist) &&
	   (last_maxdist == maxdist) &&
	   (last_minfog == minfog) &&
	   (last_maxfog == maxfog))
		return;

	MakeFogRamp(fogcolor, mindist, maxdist, minfog, maxfog);

	last_fogcolor = fogcolor;
	last_mindist = mindist;
	last_maxdist = maxdist;
	last_minfog = minfog;
	last_maxfog = maxfog;
}

void MakeFogRamp(U32 color, F32 mindist, F32 maxdist, F32 minfog, F32 maxfog)
{
	S32 i;
	F32 rampdist,tabdist,delfog;

	/* sanity check and set up fog ramp distances. */
	if(mindist > maxdist)
		mindist = maxdist;
	rampdist = maxdist - mindist;

	/* sanity check min fog level. */
	minfog *= 255.0;
	if(minfog > 255.0)
		minfog = 255.0;
	else if(minfog < 0.0)
		minfog = 0.0;
	/* sanity check max fog level. */
	maxfog *= 255.0;
	if(maxfog > 255.0)
		maxfog = 255.0;
	else if(maxfog < 0.0)
		maxfog = 0.0;
	delfog = maxfog - minfog;

	for(i=0;i<GR_FOG_TABLE_SIZE;i++)
		{
		tabdist = guFogTableIndexToW(i);
		if(tabdist <= mindist)
			fogTable[i] = (GrFog_t)minfog;
		else if(tabdist >= maxdist)
			fogTable[i] = (GrFog_t)maxfog;
		else	/* in fog ramp */
			fogTable[i] = (GrFog_t)(minfog + (delfog * ((tabdist - mindist)/rampdist)));
		}
	fogTable[0] = (GrFog_t)0; /* keep blits from fogging. */

	ZoidBGColor = color;
	grFogColorValue( color );
	grFogTable(fogTable);
	grFogMode(GR_FOG_WITH_TABLE);
}

/* ----------------------------------------------------------------------------- *
 *	Builds a fog table of the given density and clip value.
 * ----------------------------------------------------------------------------- */
static void MakeFogTable(F32 density, S32 clip, U32 color)
{
	S32	i;
	F32	dp, scale, f;

	/*	Attempt to avoid making table if it's already set. */
	if (lastDensity == density && clip == lastClip && color == lastColor)
		return;

	if (clip > 255)
		clip = 255;
	if (clip <= 0)
		density = clip = 0;

	lastDensity = density;
	lastColor 	= color;
	lastClip	= clip;


	dp 		= density * guFogTableIndexToW(GR_FOG_TABLE_SIZE-1);
	scale 	= 1.0F / (1.0F - (float)safe_expf(-dp));

	for (i = 0; i < GR_FOG_TABLE_SIZE; i++)
		{
		dp	= density * guFogTableIndexToW(i);
		f 	= (1.0 - (float)safe_expf(-dp)) * scale;

		if (f > 1.0)
			f = 1.0;
		else if (f < 0.0)
			f = 0.0;

		f *= 255.0;
		fogTable[i] = (GrFog_t)((f > clip) ? clip : f);
		}

	grFogColorValue(color);
	grFogTable(fogTable);
	grFogMode(GR_FOG_WITH_TABLE);
}	/* end MakeFogTable */


