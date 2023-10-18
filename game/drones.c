/*
 *  drones.c - Drone control functions           
 *
 *	Copyright 1996 Time Warner Interactive
 *	Unauthorized reproduction, adaptation, distribution, performance or 
 *	display of this computer program or the associated audiovisual work
 *	is strictly prohibited.
 *
 *	======================================================================
 *	$Author: gray $						$Date: 1997/09/24 22:32:02 $
 *	$Revision: 3.27 $						$Locker:  $
 *	======================================================================
 *	Change Log:
 *
 *	$Log: drones.c,v $
 * Revision 3.27  1997/09/24  22:32:02  gray
 * Clean catchup code. Give solo player avg catchup.
 *
 * Revision 3.26  1997/09/15  22:30:23  gray
 * Removed trackset hack - trackno now 0 to NTRACKS.
 *
 * Revision 3.25  1997/09/05  00:46:40  gray
 * Freed debug switch 2 for screen grabs.
 *
 * Revision 3.24  1997/06/28  01:40:05  gray
 * Extended to 4 tracks.
 *
 * Revision 3.23  1997/02/26  03:41:13  geraci
 * tuned free game pacer drones
 *
 * Revision 3.22  1997/02/21  00:12:44  geraci
 * lead drone now disregards all but his plan when free game is in effect
 *
 * Revision 3.21  1997/02/15  01:49:08  geraci
 * drones stay with humans, lead drone has own difficulty
 *
 * Revision 3.20  1997/01/30  23:19:01  geraci
 * uses drone difficulty correctly
 *
 * Revision 3.19  1997/01/30  21:20:47  geraci
 * make drone difficulty 0 the default in the code
 *
 * Revision 3.18  1997/01/30  20:32:07  geraci
 * new drone difficulty setting
 *
 * Revision 3.17  1996/12/05  10:19:34  geraci
 * more betterer drones
 *
 * Revision 3.16  1996/11/25  16:03:59  geraci
 * drones are tweaked up again
 *
 * Revision 3.15  1996/11/23  04:02:37  geraci
 * default path assign is back
 *
 * Revision 3.14  1996/11/15  05:12:36  geraci
 * auto changing of default path
 *
 * Revision 3.13  1996/11/14  13:35:11  geraci
 * tweaked drones up a little
 *
 * Revision 3.12  1996/11/11  22:13:35  geraci
 * more friendly drones
 *
 * Revision 3.11  1996/11/07  14:38:32  geraci
 * better drone regulation
 *
 * Revision 3.10  1996/10/31  07:08:24  geraci
 * multi max paths supported
 *
 * Revision 3.9  1996/10/25  10:57:58  geraci
 * change car data to all caps
 *
 * Revision 3.8  1996/10/24  09:42:45  petrick
 * Doesn't clear out num humans/drones.
 *
 * Revision 3.7  1996/10/24  09:23:47  petrick
 * Moved human/drone counting routine to mdrive.
 *
 * Revision 3.6  1996/10/23  00:14:33  petrick
 * Made track_no a global.
 *
 * Revision 3.5  1996/10/22  08:54:43  geraci
 * new drone regulation
 *
 * Revision 3.4  1996/10/07  16:13:51  geraci
 * drones spread themselves out more
 *
 * Revision 3.3  1996/09/24  23:37:03  gray
 * ZGLU purge.
 *
 * Revision 3.2  1996/09/20  02:11:57  geraci
 * end drones will lag further behind humans
 *
 * Revision 3.1  1996/09/09  02:24:52  gray
 * JAMMA version
 *
 * Revision 2.17  1996/09/06  22:05:48  geraci
 * no cheating drones
 *
 * Revision 2.16  1996/09/01  22:19:05  geraci
 * drones a little less docile, more aggressive on beginner track/expert car
 *
 * Revision 2.15  1996/08/31  22:50:08  geraci
 * faster drones for track 1
 *
 * Revision 2.14  1996/08/30  17:07:16  geraci
 * more docile drones
 *
 * Revision 2.13  1996/08/27  05:33:53  geraci
 * cleaned up some unused variables
 *
 * Revision 2.12  1996/08/21  16:36:05  geraci
 * drones cheat until they catch you
 *
 * Revision 2.11  1996/08/09  04:55:55  geraci
 * made the drones drive a bit slower but with a little more boost fudge
 *
 * Revision 2.10  1996/08/02  05:40:24  geraci
 * removed faked rpm boost due to old catchup
 *
 * Revision 2.9  1996/07/31  15:57:56  geraci
 * new time boost for faster cars
 *
 * Revision 2.8  1996/07/27  08:19:25  geraci
 * new demo game mode support
 *
 * Revision 2.7  1996/07/20  04:38:48  geraci
 * fixed drone pacing due to slow human
 *
 * Revision 2.6  1996/07/19  10:01:52  geraci
 * fixed drone 'wait for human' window
 *
 * Revision 2.5  1996/07/13  06:56:40  geraci
 * fixed bug in drone assignment
 *
 * Revision 2.4  1996/07/09  20:38:58  geraci
 * fake out rpm loss due to catch-up
 *
 * Revision 2.3  1996/07/06  01:04:18  geraci
 * better drone tracking of players
 *
 * Revision 2.2  1996/06/08  01:47:18  geraci
 * start of drone data collection
 *
 * Revision 2.1  1996/05/04  02:56:18  petrick
 * Focus 2 release
 *
 * Revision 1.30  1996/05/02  01:13:55  geraci
 * switch 10 and view1 control record again
 *
 * Revision 1.29  1996/04/30  01:28:21  geraci
 * freed up debug switch 1
 *
 * Revision 1.28  1996/04/24  23:45:58  geraci
 * moved drone abort to resurrect module
 *
 * Revision 1.27  1996/04/24  02:56:52  geraci
 * added include for resurrect module
 *
 * Revision 1.26  1996/04/20  07:11:08  geraci
 * drones can't abort during resurrection move
 *
 * Revision 1.25  1996/04/05  03:44:47  geraci
 * move drone personality to here
 *
 * Revision 1.24  1996/03/30  02:04:05  geraci
 * moved drone abort code to here
 *
 * Revision 1.23  1996/03/28  12:31:18  gray
 * Changed Maxpath record switches, look at gUseCatchup for catchup mode.
 *
 * Revision 1.22  1996/03/25  19:08:14  petrick
 * Put in catchup speed handling, but commented it out until we want it
 * off a switch.
 *
 * Revision 1.21  1996/03/12  12:35:42  gray
 * Call real catchup routine. Disable catchup on SW_DEBUG10.
 *
 * Revision 1.20  1996/03/05  03:33:03  gray
 * Start to get real drone code into stubs.
 *
 * Revision 1.19  1996/02/13  18:51:43  gray
 * Killed old drone stuff. Left shell to start building real drone code.
 *
 * Revision 1.18  1996/02/01  04:08:16  petrick
 * Changed around inlcude files a bit.
 *
 * Revision 1.17  1996/01/20  04:30:56  petrick
 * Cleanup, changed trackno to S16.
 *
 * Revision 1.16  1996/01/16  22:30:04  geraci
 * freed SW_DEBUG2
 *
 *
 *	======================================================================
 */

#include "globals.h"
#include "model.h"
#include "maxpath.h"
#include "checkpoint.h"
#include "debug.h"
#include "drones.h"
#include "dis.h"
#include "mdrive.h"
#include "os_proto.h"
#include "resurrect.h"
#include "select.h"

#define  get_max(a,b) ((a>b)?a:b)

/***********************************************************************/
/*      VARS                                                           */
/***********************************************************************/

extern Track		CP_Track[];
extern S16			gThisCar;
extern MPCTL 		mpctl[];
extern const char   hint_types[][20];

/***********************************************************************/

F32 old_drone_scale[MAX_LINKS], old_boost[MAX_LINKS];

/***********************************************************************/

void InitDrones(void)
{
	S32  i, record;

	if ((dlevels & SW_DEBUG10) && (levels & SW_VIEW1))
		{
		record  = (SW_DEBUG12 & dlevels) ? 4 : 0;
		record += (SW_DEBUG13 & dlevels) ? 2 : 0;
		record += (SW_DEBUG14 & dlevels) ? 1 : 0;
		}
	else
		record = -1;

	InitMaxPath (record);

	for (i=0; i<MAX_LINKS; i++)
		{
		old_drone_scale[i] = 1;
		old_boost[i] = 1;
		}
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*      Housekeeping on drones (in game task)                                 */

void DoDrones(void)
{
	if(!gUseCatchup)	/* Can set no catchup for tourneys. */
		no_catchup();
	else
		set_catchup();

	MaxPath (&model[gThisNode]);

	assign_drones();
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*      Stop doing nothing                                                    */

void EndDrones(void)
{
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*      Assign drone tasks (personalities) and speed adjustment               */

void assign_drones (void)
{
	S16        i, j, k, indx, index, index2, index3, temp, high_index;
	S16        high_place, rel_drone, old_index;
	S16        total_drones[MAX_LINKS], humans[MAX_LINKS], drones[MAX_LINKS];
	S16        car_in_conflict[MAX_LINKS], cars_in_order[MAX_LINKS], my_place;
	S16        ttype, num_humans, num_drones, num_to_assign;
	BOOL       okay_to_cheat;
	F32        scale, delta_dist, delta_dist2, slope, offset, delta[3];
	F32        time_scale, target_dist, car_distance[MAX_LINKS][MAX_LINKS];
	F32        max_brake, min_brake, max_boost, min_boost, temp1;
	F32        place_scale, diff_scale;
	CheckPoint *cp_tmp;
	MODELDAT   *m;
	CAR_DATA   *gc;

	cp_tmp = CP_Track[trackno].chk_point;

	/* make a drone list and human list */
	for (i=0; i<num_active_cars; i++)
		{
		index = model[i].slot;
		total_drones[index] = 0;
		}

	place_cars_in_order (cars_in_order, humans, drones, &num_humans, &num_drones);

	for (i=0,j=0; i<num_drones; i++)
		game_car[drones[i]].weight_index = i;

	assign_default_paths (num_drones, drones);

	/* if training, ignore other cars */
	if ((SW_DEBUG1 & dlevels) || demo_game)
		{
		for (i=0; i<num_active_cars; i++)
			{
			index = model[i].slot;

			if (model[index].drone_type == DRONE)
				model[index].drone_scale = 1;

			if (dlevels & SW_DEBUG1 && index == this_node)
				{
				char buf[50];
				sprintf (buf, "Mpath = %d    %s", mpctl[index].mpath_index, get_cur_hint(index));
				txt_str (20, 46, buf, GRN_PAL);
				sprintf (buf, "%ld ", mpctl[index].mpi);
				txt_str (20, 47, buf, GRN_PAL);
				}
			}

		return;
		}

	/*-----------------------------------------------------*/

	if (num_drones)
		{
		/* spread them out at start of race */
		if (GetElapsedTime() < 5 * ONE_SEC)
			{
			/* now assign drones to humans */
			for (i=0; i<num_drones; i++)
				{
				j = num_drones - i - 1;
				model[drones[i]].time_boost = ((F32)(j) * .02) + .98;
				model[drones[i]].drone_scale = 1;
				}
			}
		else
			{
			/* assign difficulty settings based on human's place (0=hard, 7=easy) */
			/* get number to assign to each human */
			num_to_assign = (F32)num_drones / (F32)num_humans + .5;
			for (i=0,j=0; i<num_humans; i++)
				{
				for (k=0; k<num_to_assign; k++)
					{
					if (j < num_drones)
						game_car[drones[j++]].difficulty = game_car[humans[i]].place;
					}
				}

			/* get distances between all cars */
			for (i=0; i<num_active_cars; i++)
				{
				index = model[i].slot;
				car_in_conflict[index] = 0;
				gc = &game_car[index];

				for (j=i; j<num_active_cars; j++)
					{
					index2 = model[j].slot;
					if (index == index2)
						car_distance[index][index2] = 99999999;
					else
						{
						for (k=0; k<3; k++)
							delta[k] = gc->dr_pos[k] - game_car[index2].dr_pos[k];

						delta_dist = delta[0]*delta[0] + delta[1]*delta[1] + delta[2]*delta[2];

						car_distance[index][index2] =
							car_distance[index2][index] = delta_dist;
						}
					}
				}

#if 0
			for (i=0; i<num_active_cars; i++)
				{
				index = model[i].slot;

				for (j=0; j<num_active_cars; j++)
					{
					char buf[50];

					index2 = model[j].slot;



					sprintf (buf, "%7.1f", (double)car_distance[index][index2]);
					txt_str (1+index*8, 20+index2, buf, WHT_PALB);
					}
				}
#endif

			/* init drone targets */
			for (i=0; i<num_drones; i++)
				model[drones[i]].drone_target = drones[i];

			/* check for human to human conflicts */
			for (i=0; i<num_humans; i++)
				{
				for (j=i+1; j<num_humans; j++)
					{
					if (car_distance[humans[i]][humans[j]] < 70*70)
						{
						car_in_conflict[humans[i]]+=10;
						car_in_conflict[humans[j]]+=10;
						}
					}
				}

			/* don't assign lead drone if free game is enabled */
			if (win_opts)
				car_in_conflict[drones[0]] = 10;

			/* now assign drone ahead or behind to human not in a conflict */
			for (i=0; i<num_humans; i++)
				{
				if (car_in_conflict[humans[i]] < 10)
					/* human not in conflict, go get drone */
					{
					/* init to self */
					index = humans[i];
					my_place = game_car[index].place;

					index2 = index3 = -1;

					if (my_place > 0)
						/* we can look behind for one */
						{
						j = cars_in_order[my_place-1];
						if (model[j].drone_type==DRONE && car_in_conflict[j]==0)
							index2 = j;
						}

					if (my_place < num_active_cars-1)
						/* we can look ahead for one */
						{
						j = cars_in_order[my_place+1];
						if (model[j].drone_type==DRONE && car_in_conflict[j]==0)
							index3 = j;
						}

					if (index2!=-1 && index3!=-1)
						{
						if (car_distance[index3][index] < car_distance[index2][index])
							index2 = index3;
						}
					else if (index2 == -1)
						index2 = index3;

					if (index2 != -1)
						/* we got a winner */
						{
						car_in_conflict[index]++;
						car_in_conflict[index2]++;
						model[index2].drone_target = index;
						}
					}
				}

			/* now try to assign another drone to each human not in a conflict */
			if (trackno == 0)
				{
				for (i=0; i<num_humans; i++)
					{
					if (car_in_conflict[humans[i]] < 10)
						/* human not in conflict, go get drone */
						{
						/* init to self */
						index = humans[i];
						my_place = game_car[index].place;

						index2 = index3 = -1;

						if (my_place > 0)
							/* we can look behind for one */
							{
							j = cars_in_order[my_place-1];
							if (model[j].drone_type==DRONE && car_in_conflict[j]==0)
								index2 = j;
							}

						if (my_place < num_active_cars-1)
							/* we can look ahead for one */
							{
							j = cars_in_order[my_place+1];
							if (model[j].drone_type==DRONE && car_in_conflict[j]==0)
								index3 = j;
							}

						if (index2!=-1 && index3!=-1)
							{
							if (car_distance[index3][index] < car_distance[index2][index])
								index2 = index3;
							}
						else if (index2 == -1)
							index2 = index3;

						if (index2 != -1)
							/* we got a winner */
							{
							car_in_conflict[index]++;
							car_in_conflict[index2]++;
							model[index2].drone_target = index;
							}
						}
					}
				}

			/* move drones close to target human */
			for (indx=0; indx<num_active_cars; indx++)
				{
				index = model[indx].slot;

				ttype = -1;

				/* mess with only our drones */
				if (model[index].we_control)
					{
					index2 = model[index].drone_target;
					my_place = game_car[index].place;

					/* first place = 0, last_place = 1 */
					place_scale = (F32)my_place / (num_active_cars-1);

					/* difficulty scale (0=hard, 1=easy) */
					diff_scale = (F32)game_car[index].difficulty / (F32)(MAX_LINKS-1);

					/* set max brake and boost values */
					max_boost = (1.0-1.1)*diff_scale+1.1 - place_scale * ((.15-.05)*diff_scale+.05);
					min_boost = 1.0 - place_scale * ((.06-.02)*diff_scale+.02);
					max_brake =  .9 - place_scale * ((.6-.1)*diff_scale+.1);
					min_brake = (.96-1.0)*diff_scale+1.0 - place_scale * ((.05-.02)*diff_scale+.02);

					/* get nearest humans ahead */
					for (i=my_place-1; i>=0; i--)
						{
						if (model[cars_in_order[i]].drone_type == HUMAN)
							break;
						}

					okay_to_cheat = true;

#if 0

					/* if we found one ahead see if further than 500 ft */
					if (i != -1)
						{
						if (game_car[index].distance - game_car[cars_in_order[i]].distance > -500)
							okay_to_cheat &= false;
						}

					/* get nearest humans behind */
					for (i=my_place; i<num_active_cars; i++)
						{
						if (model[cars_in_order[i]].drone_type == HUMAN)
							break;
						}

					/* if we found one behind, see if further than 500 ft */
					if (i != num_active_cars)
						{
						if (game_car[index].distance - game_car[cars_in_order[i]].distance < 500)
							okay_to_cheat &= false;
						}

					/* if drone is ahead of his difficulty (place he is trying */
					/* to be at) then don't cheat ahead */
					if (my_place < game_car[index].difficulty)
						okay_to_cheat &= false;
#endif

#if 0
					{
					char buf[50];

					nsprintf (buf, 50, "%d %7.3f xt %7.3f nt %7.3f xk %7.3f nk ", game_car[index].difficulty, 	max_boost, min_boost, max_brake, min_brake);
					txt_str (10, 35+game_car[index].place, buf, WHT_PALB);
					}
#endif

					/* if free game enabled and this is pacer drone, scales are already set */
					if (win_opts && drones[0]==index)
						{
#if TRKFIX
/* figure out scale for t4-8 later. */
						F32 tscale[MAX_TRACKS][4] = {	{.986, .993, 1.0, 1.06},
										{.98,  .99, .996, 1.02},
										{.985, .99, .997, 1.03},
										{.985, .99, .997, 1.03},
										{.985, .99, .997, 1.03},
										{.985, .99, .997, 1.03},
										{.985, .99, .997, 1.03},
										{.985, .99, .997, 1.03} };
#endif

						ttype = 0;

						/* if drone is in first place and 700+ feet ahead, slow down */
						if (drones[0] == 0)
							{
							index2 = cars_in_order[1];
							delta_dist = game_car[drones[0]].distance - game_car[index2].distance;

							if (delta_dist < 700)
								{
								time_scale = tscale[trackno][drone_diff];
								scale = 1.0;
								}
							else
								{
								time_scale = 1.0;

								/* to far ahead of car behind, slow down */
								scale = linear_interp (1500, 700, 1.0, .5, delta_dist);
								}
							}
						else
							{
							delta_dist = 0;
							time_scale = tscale[trackno][drone_diff];
							scale = 1.0;
							}
						}
					/* if drone has human target, modify toward human */
					else if (index2 != index)
						{
						delta_dist = game_car[index].distance - game_car[index2].distance;

						if (delta_dist < -20)
							{
							/*11111111111111*/
							ttype = 1;
							if (delta_dist < -300 && okay_to_cheat)
								max_boost *= 1.2;
							/* human in front of drone, speed up */
							time_scale = linear_interp (-300, -60, max_boost, min_boost, delta_dist);
							scale = min_brake;
							}
						else
							{
							/*22222222222222*/
							ttype = 2;
							/* if only one drone assigned, okay to pull back */
							if (car_in_conflict[index2] <= 1)
								{
								scale = linear_interp (200, 60, max_brake, min_brake, delta_dist);
								time_scale = min_boost;
								}
							else
								{
								scale = min_brake;
								time_scale = min_boost;
								}
							}
						} /* end: drone has human target */
					else
						/* spread all other drones out */
						{
						my_place = game_car[index].place;

						if (my_place == 0)
							/* first place */
							{
							index2 = cars_in_order[1];
							delta_dist = game_car[index].distance - game_car[index2].distance;

							if (delta_dist < 200)
								{
								/*33333333333333*/
								ttype = 3;
								/* too close to car behind, speed up */
								time_scale = linear_interp (200, 0, min_boost, max_boost, delta_dist);
								scale = min_brake;
								}
							else
								{
								/*44444444444444*/
								ttype = 4;
								/* to far ahead of car behind, slow down */
								scale = linear_interp (500, 200, max_brake, min_brake, delta_dist);
								time_scale = min_boost;
								}
							}
						else if (my_place == num_active_cars-1)
							/* last place */
							{
							index2 = cars_in_order[num_active_cars-2];

							delta_dist = game_car[index].distance - game_car[index2].distance;

							if (delta_dist < -150)
								{
								/*55555555555555*/
								ttype = 5;
								if (delta_dist < -300 && okay_to_cheat)
									max_boost *= 1.2;
								/* too far behind, speed up */
								time_scale = linear_interp (-150, -300, min_boost, max_boost, delta_dist);
								scale = min_brake;
								}
							else
								{
								/*66666666666666*/
								ttype = 6;
								/* too close to car in front, slow down */
								scale = linear_interp (-150, 0, min_brake, max_brake, delta_dist);
								time_scale = min_boost;
								}
							}
						else
							/* in middle of pack */
							{
							/* get car ahead */
							index2 = cars_in_order[my_place-1];
							delta_dist = game_car[index].distance - game_car[index2].distance;

							/* get car behind */
							index2 = cars_in_order[my_place+1];
							delta_dist2 = game_car[index].distance - game_car[index2].distance;

							/* if car ahead and behind are both within 150 ft, stay between them */
							if (delta_dist > -150 && delta_dist2 < 150)
								{
								target_dist = (delta_dist + delta_dist2) / 2;

								if (target_dist < 0)
									{
									/*77777777777777*/
									ttype = 7;
									/* closer to car behind, speed up */
									time_scale = linear_interp (delta_dist-target_dist, -200, min_boost, max_boost, delta_dist);
									scale = min_brake;
									}
								else
									{
									/*88888888888888*/
									ttype = 8;
									/* closer to car ahead, slow down */
									scale = linear_interp (delta_dist2-target_dist, 200, min_brake, max_brake, delta_dist2);
									time_scale = min_boost;
									}
								}
							else
								/* go toward target human */



								/* go toward most humans */
								{
								/* get number of humans ahead */
								for (i=0,j=0; i<my_place; i++)
									{
									if (model[cars_in_order[i]].drone_type == HUMAN)
										j++;
									}

								/* get number of humans behind */
								for (i=my_place,k=0; i<num_active_cars; i++)
									{
									if (model[cars_in_order[i]].drone_type == HUMAN)
										k++;
									}

								/* more humans are ahead */
								if (j >= k)
									{
									/*99999999999999*/
									ttype = 9;
									if (delta_dist < -150)
										{
										if (delta_dist < -300 && okay_to_cheat)
											max_boost *= 1.2;

										/* behind humans, speed up */
										time_scale = linear_interp (-150, -300, min_boost, max_boost, delta_dist);
										scale = min_brake;
										}
									else
										{
										/* too close to car ahead, slow down */
										/*10 10 10 10 10 10 10*/
										ttype = 10;
										scale = linear_interp (-150, 0, min_brake, max_brake, delta_dist);
										time_scale = min_boost;
										}
									}
								else
									/* more are behind */
									{
									if (delta_dist2 < 150)
										{
										/*11 11 11 11 11 11 11*/
										ttype = 11;

										/* closer to car behind, speed up */
										time_scale = linear_interp (150, 0, min_boost, max_boost, delta_dist2);
										scale = min_brake;
										}
									else
										{
										/*12 12 12 12 12 12 12*/
										ttype = 12;

										/* closer to car ahead, slow down */
										scale = linear_interp (500, 150, max_brake, min_brake, delta_dist2);
										time_scale = min_boost;
										}
									}
								}
							}
						} /* end: spread all other drones out */

					if (coast_flag || end_game_flag)
						time_scale = min_boost;	

					if (lap_flag==true && game_car[humans[num_humans-1]].place>game_car[index].place)
						{
						scale = min_brake;
						time_scale = min_boost;
						}

					temp1 = model[index].drone_scale;

					if (temp1 > scale)
						{
						temp1 -= .01;
						if (temp1 < scale)
							temp1 = scale;
						}
					else
						{
						temp1 += .01;
						if (temp1 > scale)
							temp1 = scale;
						}

					model[index].drone_scale = temp1;

					temp1 = model[index].time_boost;

					if (temp1 > time_scale)
						{
						temp1 -= .01;
						if (temp1 < time_scale)
							temp1 = time_scale;
						}
					else
						{
						temp1 += .01;
						if (temp1 > time_scale)
							temp1 = time_scale;
						}

					model[index].time_boost = temp1;

#if 0
					if (dlevels & SW_DEBUG2)
						{
						char buf[80];

						nsprintf (buf, 80, "%8.8s %d %2d %d %6.2f %6.2f  %8.3f  %6.2f %6.2f ",
								  NodeName(index), game_car[index].place,
								  ttype, okay_to_cheat,
								  scale,
								  model[index].drone_scale,
								  delta_dist,
								  time_scale,
								  model[index].time_boost);
						txt_str (1, 39+index, buf, GRN_PAL);
						}
#endif
					}

#if 0
				for (i=0; i<num_active_cars; i++)
					{
					char buf[50];

					index = model[i].slot;

					sprintf (buf, "%d ", car_in_conflict[index]);
					txt_str (20+index*4, 30, buf, WHT_PALB);
					}

				for (i=0; i<num_active_cars; i++)
					{
					char buf[50];

					index = model[i].slot;

					sprintf (buf, "%d ", model[index].drone_target);
					txt_str (20+index*4, 32, buf, WHT_PALB);
					}
#endif

				}
			}
		}
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Find where 'input' lies between in_bound1 and in_bound2
*         then use the ratio to scale between out_bound1 and 2 to get output */

F32 linear_interp (F32 in_bound1, F32 in_bound2, F32 out_bound1, F32 out_bound2, F32 input)
{
	F32 slope, offset;

	if (in_bound1 < in_bound2)
		{
		if (input < in_bound1)
			return (out_bound1);
		else if (input > in_bound2)
			return (out_bound2);
		}
	else
		{
		if (input < in_bound2)
			return (out_bound2);
		else if (input > in_bound1)
			return (out_bound1);
		}

	/* if input range is too small, just return center of output */
	if (fabs(in_bound1-in_bound2) < 1e-3)
		return ((out_bound1+out_bound2)*.5);

	slope = (out_bound1 - out_bound2) / (in_bound1 - in_bound2);
	offset = out_bound1 - slope * in_bound1;

	return (slope * input + offset);
}
