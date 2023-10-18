/*
 *	model.c
 *
 *	Copyright 1996 Time Warner Interactive
 *	Unauthorized reproduction, adaptation, distribution, performance or 
 *	display of this computer program or the associated audiovisual work
 *	is strictly prohibited.
 *
 *	======================================================================
 *	$Author: gray $						$Date: 1997/10/14 03:18:19 $
 *	$Revision: 3.16 $						$Locker:  $
 *	======================================================================
 *	Change Log:
 *
 *	$Log: model.c,v $
 * Revision 3.16  1997/10/14  03:18:19  gray
 * Disable solo boost.
 *
 * Revision 3.15  1997/10/14  02:20:14  gray
 * Better documentation for solo catchup.
 *
 * Revision 3.14  1997/10/13  22:10:37  gray
 * 20 percent max catchup for T4-7.
 *
 * Revision 3.13  1997/10/08  22:40:35  gray
 * tweak catchup for T5-7.
 *
 * Revision 3.12  1997/09/24  22:32:02  gray
 * Clean catchup code. Give solo player avg catchup.
 *
 * Revision 3.11  1997/09/15  22:30:23  gray
 * Removed trackset hack - trackno now 0 to NTRACKS.
 *
 * Revision 3.10  1997/09/03  02:57:11  gray
 * Made T4 catchup between T1 and T2 catchup.
 *
 * Revision 3.9  1997/06/28  01:41:43  gray
 * Extended to 4 tracks.
 *
 * Revision 3.8  1996/11/01  12:24:59  geraci
 * drone and human types are now defines
 *
 * Revision 3.7  1996/11/01  11:21:48  geraci
 * use distance for catchup instead of place
 *
 * Revision 3.6  1996/10/23  00:14:33  petrick
 * Made track_no a global.
 *
 * Revision 3.5  1996/10/22  10:01:36  geraci
 * tweaked catchup speed
 *
 * Revision 3.4  1996/10/07  16:17:13  geraci
 * catchup only affects other humans
 *
 * Revision 3.3  1996/10/03  06:56:11  geraci
 * removed debug stuff
 *
 * Revision 3.2  1996/10/03  06:36:08  geraci
 * catchup is different
 *
 * Revision 3.1  1996/09/09  02:24:52  gray
 * JAMMA version
 *
 * Revision 2.3  1996/09/01  09:18:02  gray
 * Model types cleanup. First pass - removed fptyp.
 *
 * Revision 2.2  1996/07/31  13:42:22  gray
 * New time fudge catchup.
 *
 * Revision 2.1  1996/05/04  02:56:18  petrick
 * Focus 2 release
 *
 * Revision 1.11  1996/03/31  11:08:14  gray
 * 1st pass catchup tuning.
 *
 * Revision 1.10  1996/03/28  12:35:59  gray
 * Made catchup scale by track. 1.0 = full catchup. avg_catchup() for
 * No Ketchup selection. no_catchup() for full speed drone path record.
 *
 * Revision 1.9  1996/03/15  02:50:59  geraci
 * made index and dist arrays, local not global
 *
 *
 *	======================================================================
 */


#define DEFINE_GLOBALS
#include "model.h"
#undef DEFINE_GLOBALS

#include "globals.h"
#include "modeldat.h"
#include "checkpoint.h"
#include "dis.h"
#include "debug.h"

#include "Pro/vecmath.pro"
#include "Pro/unitvecs.pro"

/* cuzone = catchup zone (in feet behind leader).      */
/* cuscale = If in cuzone, scale catchup 0 to cuscale  */
/*           if outside cuzone, catchup = max catchup. */
/*							*/
/*				  trk1   trk2   trk3   trk4   trk5   trk6   trk7   trk8  */
/*				  -----  -----  -----  -----  -----  -----  -----  ----- */
const F32 cuzone[MAX_TRACKS] =  { 400.0, 800.0, 800.0, 600.0, 900.0, 600.0, 800.0, 600.0 };
const F32 cuscale[MAX_TRACKS] = { 0.300, 0.200, 0.100, 0.200, 0.200, 0.200, 0.200, 0.250 };
/*		maximum catchup	    30%    20%    10%    20%    20%    20%    20%    25%   */

/* So that solo players can get same hiscores as linked player, */
/* compensate for linked rubber-band catchup by giving solo player */
/*  a constant percentage of max track catchup. */
const F32 cusolo = 0.0; /* 0% of max catchup. (no solo boost) */


/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Rubber-band catchup algorithm                        */

void set_catchup(void)
{
	S16 i, high_index, num_players;
	F32 firstdist, lagdist, max_catchup, target_catchup;


	/* find leading human */
	num_players = 0;
	high_index = -1;
	for (i=0; i<MAX_LINKS; i++)
		{
		if ( (model[i].in_game) &&
			 (model[i].drone_type == HUMAN))
			{
			num_players++;
			if (high_index == -1)
				high_index = i;
			else if (game_car[i].distance > game_car[high_index].distance)
				high_index = i;
			}
		}

	if(num_players == 1)
		{
		/* give solo player compensating boost. */
		model[gThisNode].catchup = 1.0 + (cuscale[trackno] * cusolo);
		return;
		}

	max_catchup = cuscale[trackno];
	firstdist = game_car[high_index].distance;

	/* spread catchup speed evenly across catchup zone and max out all others */
	for(i=0; i<MAX_LINKS; i++)
		{
		if (model[i].in_game && model[i].drone_type == HUMAN)
			{
			lagdist = firstdist - game_car[i].distance;
			if (lagdist > cuzone[trackno])
				target_catchup = max_catchup + 1;
			else
				target_catchup = lagdist * max_catchup / cuzone[trackno] + 1;

			model[i].catchup = .97 * model[i].catchup + .03 * target_catchup;
			}

#if 0
			{
			char buf[50];
			sprintf (buf, "%d %8.3f %8.3f ", model[i].drone_type, (double)model[i].catchup, (double)model[i].time_boost);
			txt_str (20, 35+i, buf, WHT_PALB);
			}
#endif

		}
}

/* Set to no catchup. Still need for tourney mode. */
void no_catchup(void)
{
	S32 i;
	F32 scale;

	scale = 1.0;
	model[gThisNode].catchup = scale;
	for(i=0;i<MAX_LINKS;i++)
		if(model[i].we_control)
			model[i].catchup = scale;
}


