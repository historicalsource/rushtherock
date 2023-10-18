/*
 *  mdrive.c -- handles all car controls
 *
 *      Copyright 1996 Atari Games.
 *  Unauthorized reproduction, adaptation, distribution, performance or
 *  display of this computer program or the associated audiovisual work
 *  is strictly prohibited.
 *
 *  ======================================================================
 *  $Author: gray $                       $Date: 1997/10/10 02:05:54 $
 *  $Revision: 3.102 $                       $Locker:  $
 *  ======================================================================
 *  Change Log:
 *
 *  $Log: mdrive.c,v $
 * Revision 3.102  1997/10/10  02:05:54  gray
 * Move car below world and send message when starting resurrect.
 *
 * Revision 3.101  1997/10/07  18:57:11  grjost
 * Changed array from [3] to [MAX_TRACKS]
 *
 * Revision 3.100  1997/10/07  08:48:22  hightower
 * Removed tabs and put in call to update sounds in model task
 *
 * Revision 3.99  1997/10/03  21:18:14  grjost
 * Fixed hanging woman movie start
 *
 * Revision 3.98  1997/10/02  02:01:28  gray
 * No more car damage from major suspension compression.
 *
 * Revision 3.97  1997/09/29  19:24:53  gray
 * Calculate mph for remote cars.
 *
 * Revision 3.96  1997/09/25  05:53:46  hightower
 * Gamenet functional, but disabled -- to enable define GAMENET to 1 in gamenet.h
 *
 * Revision 3.95  1997/09/25  03:41:11  gray
 * better var name for entity state pointer.
 *
 * Revision 3.94  1997/09/25  02:08:20  grjost
 * *** empty log message ***
 *
 * Revision 3.93  1997/09/18  07:33:45  gray
 * audry engine on new VW slots.
 *
 * Revision 3.92  1997/09/15  22:30:23  gray
 * Removed trackset hack - trackno now 0 to NTRACKS.
 *
 * Revision 3.91  1997/09/04  17:59:59  gray
 * Audrey car engine audio only on bus now.
 *
 * Revision 3.90  1997/09/02  00:19:04  gray
 * Removed experimental engine audio. Back to original two.
 *
 * Revision 3.89  1997/08/28  20:59:46  gray
 * Third engine sound for car 2. 4th engine not ready for prime time.
 *
 * Revision 3.88  1997/08/22  01:33:54  gray
 * Hook into gamenet, messing with different engine types.
 *
 * Revision 3.87  1997/06/28  01:41:43  gray
 * Extended to 4 tracks.
 *
 * Revision 3.86  1997/06/13  18:44:10  gray
 * Audio mods to be compatible with old and new sounds.
 *
 * Revision 3.85  1997/06/04  02:44:44  gray
 * Zapped ithrottle.
 *
 * Revision 3.84  1997/06/04  02:15:46  gray
 * Restructuring controls (gas,brake,clutch,etc) handling.
 *
 * Revision 3.83  1997/05/30  18:17:27  gray
 * pruning.
 *
 * Revision 3.82  1997/05/30  07:53:15  gray
 * fastout is history.
 *
 * Revision 3.81  1997/05/30  04:05:35  gray
 * mainout is history!
 *
 * Revision 3.80  1997/05/30  01:25:14  gray
 * Clean up crashflag stuff.
 *
 * Revision 3.79  1997/05/29  21:08:03  gray
 * Clean up mph stuff.
 *
 * Revision 3.78  1997/05/21  22:11:28  gray
 * road codes removed from mainout.
 *
 * Revision 3.77  1997/05/21  18:13:16  gray
 * Removed rollv, pitchv, and yawv from mainout.
 *
 * Revision 3.76  1997/05/21  06:04:56  gray
 * Killed xv,yv,zv in mainout (again).
 *
 * Revision 3.75  1997/05/21  00:10:16  gray
 * caruvs finally exterminated.
 *
 * Revision 3.74  1997/05/13  22:08:01  gray
 * Only one uv type in CAR_DATA now!!!
 *
 * Revision 3.73  1997/05/11  07:55:11  gray
 * Removed uvs from game car data (now everything uses dr_uvs).
 *
 * Revision 3.72  1997/05/11  04:37:27  gray
 * Cleaning model interface.
 *
 * Revision 3.71  1997/05/11  03:37:10  gray
 * Cleaning model interface.
 *
 * Revision 3.70  1997/03/31  03:10:37  gray
 * Nucleus gone - got rid of include and nucleus types.
 *
 * Revision 3.69  1997/03/31  00:04:09  gray
 * update_game_data blocks model ast with gModeldatLock.
 *
 * Revision 3.68  1997/03/20  13:48:11  geraci
 * send countdown message out in model task
 *
 * Revision 3.67  1997/03/13  04:04:58  geraci
 * multi-cast channel is handled differently
 *
 * Revision 3.66  1997/02/15  01:51:02  geraci
 * inited difficulty setting
 *
 * Revision 3.65  1997/01/30  20:33:36  geraci
 * expanded game difficulty to eight levels
 *
 * Revision 3.64  1997/01/24  02:17:45  geraci
 * new checkpoint scheme
 *
 * Revision 3.63  1997/01/15  20:02:49  geraci
 * number of laps for track 2 and 3 can be changed by operator
 *
 * Revision 3.62  1996/12/13  13:25:34  geraci
 * common marker flags
 *
 * Revision 3.61  1996/12/13  12:06:10  geraci
 * more new lap stuff
 *
 * Revision 3.60  1996/12/13  08:15:58  geraci
 * new lap stuff
 *
 * Revision 3.59  1996/12/05  12:19:26  geraci
 * made quaternions less smooth / faster snapping
 *
 * Revision 3.58  1996/12/05  00:04:03  geraci
 * end of game score removed from entity state
 *
 * Revision 3.57  1996/12/04  01:58:08  geraci
 * checkpoints and end of game scores are handled by entity state updates
 *
 * Revision 3.56  1996/11/27  17:05:59  geraci
 * each car has its own in tunnel flag
 *
 * Revision 3.55  1996/11/27  14:30:02  gray
 * Dead reckon shadow.
 *
 * Revision 3.54  1996/11/27  12:56:10  gray
 * Changed NetTime from F32 to 10microsec S32.
 *
 * Revision 3.53  1996/11/26  23:34:39  geraci
 * clears out old junk in model structure
 *
 * Revision 3.52  1996/11/25  12:41:09  geraci
 * fixed wacky uvs at start of race
 *
 * Revision 3.51  1996/11/25  11:38:56  geraci
 * old engine is gone
 *
 * Revision 3.50  1996/11/25  03:47:42  geraci
 * sound and short-cut codes ignore 0 data when in air
 *
 * Revision 3.49  1996/11/23  23:09:32  gray
 * Made interrupt safe net time for game task.
 *
 * Revision 3.48  1996/11/23  02:58:45  geraci
 * allows skids during attract with no sound
 *
 * Revision 3.47  1996/11/22  05:01:55  geraci
 * hide resurrecting car defined in look field, quats passed instead of uvs
 *
 * Revision 3.46  1996/11/21  03:27:18  geraci
 * using stop-interrupt-method for data copying
 *
 * Revision 3.45  1996/11/19  18:48:25  gray
 * Different size collision boxes.
 *
 * Revision 3.44  1996/11/19  09:54:30  geraci
 * drones start at preset spots on each track during attract
 *
 * Revision 3.43  1996/11/18  13:35:08  geraci
 * re calculate magvel on all linked stacks
 *
 * Revision 3.42  1996/11/18  10:13:27  gray
 * Made model time interrrupt safe with GetNetTime.
 *
 * Revision 3.41  1996/11/16  18:57:10  geraci
 * forrest's way to stop model task during updates
 *
 * Revision 3.40  1996/11/16  07:53:31  gray
 * Stop dead reckoning after 2 seconds, calc magvel on reckon velocity.
 *
 * Revision 3.39  1996/11/14  13:34:28  geraci
 * sets car to one of four models (not eight)
 *
 * Revision 3.38  1996/11/13  14:56:48  geraci
 * don't re-center wheel in attract
 *
 * Revision 3.37  1996/11/11  22:17:21  geraci
 * cleaned out old drone model slicing stuff, updated new slices
 *
 * Revision 3.36  1996/11/09  12:28:09  geraci
 * pot read routine is here now, no drone bug fixed
 *
 * Revision 3.35  1996/11/07  14:37:04  geraci
 * drone always takes advanced car
 *
 * Revision 3.34  1996/11/06  13:48:27  geraci
 * drone model scheduling for link, model version of edges supported
 *
 * Revision 3.33  1996/11/03  05:59:37  geraci
 * split up debug switches and regular game switches
 *
 * Revision 3.32  1996/11/01  12:25:08  geraci
 * drone and human types are now defines
 *
 * Revision 3.31  1996/10/31  08:04:35  geraci
 * set distance tracker path to 0
 *
 * Revision 3.30  1996/10/30  05:45:43  petrick
 * Got rid of EntityList.
 *
 * Revision 3.29  1996/10/30  05:42:05  geraci
 * cleaned some unused stuff
 *
 * Revision 3.28  1996/10/25  10:58:28  geraci
 * removed time stamp stuff, made car data all caps
 *
 * Revision 3.27  1996/10/24  09:24:24  petrick
 * Moved human/drone counting code here.
 *
 * Revision 3.26  1996/10/23  00:14:33  petrick
 * Made track_no a global.
 *
 * Revision 3.25  1996/10/22  09:41:20  geraci
 * drones will drive intermediate or beginner models only
 *
 * Revision 3.24  1996/10/18  23:58:59  petrick
 * MAde Car a typedef.
 *
 * Revision 3.23  1996/10/16  09:07:00  geraci
 * initialized car data whether or not in race
 *
 * Revision 3.22  1996/10/15  20:47:38  geraci
 * starts engine sound model for demo car
 *
 * Revision 3.21  1996/10/10  05:19:24  petrick
 * Cleaned out some unused globals.
 *
 * Revision 3.20  1996/10/07  16:16:03  geraci
 * set game difficultly due to easiest common setting
 *
 * Revision 3.19  1996/10/05  09:18:09  geraci
 * lessened recentering force during attract
 *
 * Revision 3.18  1996/10/04  07:32:28  geraci
 * attract sound is now split into attract effects and attract music
 * recentering during countdown works again
 *
 * Revision 3.17  1996/10/02  21:15:02  petrick
 * Changed params in PutEventReport PDU call.
 *
 * Revision 3.16  1996/09/30  16:35:51  gray
 * Extrapolate car positions with model data if local (game task).
 *
 * Revision 3.15  1996/09/30  08:18:50  gray
 * Getting rid of old model globals.
 *
 * Revision 3.14  1996/09/25  03:23:49  gray
 * Dont need trunc stuff any more.
 *
 * Revision 3.13  1996/09/25  02:00:44  petrick
 * Got rid of include file.
 *
 * Revision 3.12  1996/09/25  01:25:59  gray
 * romaddr not used now.
 *
 * Revision 3.11  1996/09/23  10:02:17  gray
 * Fixing dead reckon control.
 *
 * Revision 3.10  1996/09/20  02:09:41  geraci
 * short cut codes have histerisis
 *
 * Revision 3.9  1996/09/18  10:11:26  geraci
 * made a majority rules function for road and sound flags
 *
 * Revision 3.8  1996/09/17  12:36:56  gray
 * Add roadboost to time_fudge.
 *
 * Revision 3.7  1996/09/17  11:18:11  petrick
 * Gets vis road info now instead of road code.
 *
 * Revision 3.6  1996/09/17  08:44:26  geraci
 * cars can rev at the start line
 *
 * Revision 3.5  1996/09/16  02:32:51  petrick
 * Dead reckoning now puts out entity state packets.
 *
 * Revision 3.4  1996/09/14  11:21:23  geraci
 * demo game is now done in attract state
 *
 * Revision 3.3  1996/09/13  23:02:52  geraci
 * drones can skid
 *
 * Revision 3.2  1996/09/12  09:57:43  petrick
 * Now passing roadcode across link.
 *
 * Revision 3.1  1996/09/09  02:24:52  gray
 * JAMMA version
 *
 *  ======================================================================
 */

#include "model.h"
#include "modeldat.h"
#include "modelcom.h"

#include "blit.h"
#include "carsnd.h"
#include "checkpoint.h"
#include "controls.h"
#include "debug.h"
#include "dis.h"
#include "drivsym.h"
#include "drones.h"
#include "game.h"
#include "gamenet.h"
#include "globals.h"
#include "sounds.h"
#include "os_proto.h"
#include "mb.h"
#include "maxpath.h"
#include "sounds.h"
#include "resurrect.h"
#include "vecmath.h"
#include "visuals.h"

#include "Pro/camera.pro"
#include "Pro/checkpoint.pro"
#include "Pro/collision.pro"
#include "Pro/communic.pro"
#include "Pro/controls.pro"
#include "Pro/d3math.pro"
#include "Pro/drivetra.pro"
#include "Pro/drivsym.pro"
#include "Pro/fswheel.pro"
#include "Pro/initiali.pro"
#include "Pro/math.pro"
#include "Pro/mathsubs.pro"
#include "Pro/mdrive.pro"
#include "Pro/moobs.pro"
#include "Pro/pots.pro"
#include "Pro/reckon.pro"
#include "Pro/road.pro"
#include "Pro/select.pro"
#include "Pro/sounds.pro"
#include "Pro/timint.pro"
#include "Pro/unitvecs.pro"
#include "Pro/vecmath.pro"

/*-------------------------------  DEFINES  ---------------------------------*/

/*-------------------------------  EXTERNS  ---------------------------------*/

extern S16          which_engine;
extern S16          gThisCar;
extern S8           view,saveview;
extern Car          *carlist[];
extern COLLSIZE         collsize[];
extern MPCTL        mpctl[];
extern F32          gStrayAngle;
extern U32          number_checkpoints;
extern const S16    max_radio;
extern S8           demo_station;
extern short        num_cars;  /* Number of cars in data base */
extern Track        CP_Track[];
extern PATHTYP      *path;
extern U16          path_start, path_end, path_loop;
extern S16          path_dist_index[];
extern S16          path_index[];
extern U32          current_mcast_channel;

/*-------------------------------  GLOBALS  ---------------------------------*/

S16   cur_drone, sched_index, sched_to_use;
S16   our_drones[MAX_LINKS], num_our_drones;
S16   demo_path_point, demo_checkpoint;
S32   last_m_levels, last_m_dlevels;

const S16 use_sched[8] = { 4, 3, 2, 1, 1, 1, 1, 0 };
const S16 drone_sched[5][12] = { { 2, 2, 2, 1, -1 },
                                 { 1, 1, 1, 0, -1 },
                                 { 1, 0, 1, 0, -1 },
                                 { 0, 0, 1, 0, -1 },
                                 { 0, -1 } };

const S16 demo_t1_path_points[][3] = { {-1630,  276,  5972},
                                       {-1257,   59,  1469},
                                       {-2414,   74,  -994},
                                       {-2729,   75, -2268},
                                       {  -18,  -86, -2066},
                                       { 1454, -117,   798} };


const S16 demo_t2_path_points[][3] = { {1140, 143, 2340},
                                       {-5256, 0, 1362},
                                       {-3921, 0, -452} };


const S16 demo_t3_path_points[][3] = { {-5417, -31, 67},
                                       {-7341, 190, 3245},
                                       {-6627, 111, 974},
                                       {-3548, 259, 2426},
                                       {-1834, 322, 2031} };

const S16 demo_t4_path_points[][3] = { {1140, 143, 2340},
                                       {-5256, 0, 1362},
                                       {-3921, 0, -452} };

#if TRKFIX
/* temp - use t4 points for t5-8. */
S16 * const demo_path_points[MAX_TRACKS] = {    (U16 * const) &demo_t1_path_points,
                        (U16 * const) &demo_t2_path_points,
                        (U16 * const) &demo_t3_path_points,
                        (U16 * const) &demo_t4_path_points,
                        (U16 * const) &demo_t4_path_points,
                        (U16 * const) &demo_t4_path_points,
                        (U16 * const) &demo_t4_path_points,
                        (U16 * const) &demo_t4_path_points };

const S16 num_demo_ppoints[MAX_TRACKS] = {  sizeof (demo_t1_path_points)/3/2,
                        sizeof (demo_t2_path_points)/3/2,
                        sizeof (demo_t3_path_points)/3/2,
                        sizeof (demo_t4_path_points)/3/2,
                        sizeof (demo_t4_path_points)/3/2,
                        sizeof (demo_t4_path_points)/3/2,
                        sizeof (demo_t4_path_points)/3/2,
                        sizeof (demo_t4_path_points)/3/2 };
#endif


/*------------------------------  PROTOTYPES  --------------------------------*/

static void multiinit (S16 mode, S16 drone_index);
static void CheckQuadDamage( MODELDAT *m, S32 q );

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Set up flags and stuff due to glink flags                            */

void munge_gLink_data (void)
{
    S32      i, j, index;
    MODELDAT *m;
    CAR_DATA *gc;

    /* set up list that has you, your drones and other drones */
    num_active_cars = 0;
    difficulty[trackno] = 7;
    marker_flag = false;
    track_len[trackno] = 99;

    for (i=0; i<MAX_LINKS; i++)
        {
        m = &model[i];
        gc = &game_car[i];

        m->we_control       = (gLink[i].owner == gThisNode);
        m->drone_type       = 0; /* initially set to no car */

        /* if slot is other player or drone; process */
        m->in_game  = (gLink[i].flags & LINK_ACTIVE) || (gLink[i].owner<MAX_LINKS);
        if (m->in_game)
            {
            model[num_active_cars++].slot   = i;
            gc->place                       = gLink[i].slot;
            m->drone_type                   = (gLink[i].owner<MAX_LINKS) ? DRONE : HUMAN;

            if (m->drone_type == HUMAN)
                {
                if (gLink[i].difficulty < difficulty[trackno])
                    difficulty[trackno] = gLink[i].difficulty;

                if (gLink[i].marker_flag)
                    marker_flag = true;

                if (gLink[i].num_laps < track_len[trackno])
                    track_len[trackno] = gLink[i].num_laps;
                }

            m->net_node = i;
            }
        }

    num_humans = num_drones = num_our_drones = 0;

    /*  Calculate lists of humans and drones. */
    for (i=0; i<num_active_cars; i++)
        {
        index = model[i].slot;

        /* place car into proper array */
        if (model[index].drone_type==HUMAN)
            humans[num_humans++] = index;
        else
            {
            drones[num_drones++] = index;
            if (model[index].we_control)
                our_drones[num_our_drones++] = index;
            }

        }
}

/* ----------------------------------------------------------------------------
*
*       Initialize data for all cars                                         */

void init_cars()
{
    S32      i, j, index;
    MODELDAT *m;
    CAR_DATA *gc;
    Car      *carp = carlist[0];
    COLLSIZE *sizep;
    F32 maxlen;

    /* set up list that has you, your drones and other drones */
    num_active_cars = 0;
    difficulty[trackno] = 7;
    marker_flag = false;

    for (i=0; i<MAX_LINKS; i++)
        {
        m = &model[i];
        gc = &game_car[i];

        /* clear out old data */
        for (j=0; j<sizeof(MODELDAT); j++)
            ((S8 *)m)[j] = 0;

        for (j=0; j<sizeof(CAR_DATA); j++)
            ((S8 *)gc)[j] = 0;

        m->we_control       = (gLink[i].owner == gThisNode);
        m->data_valid       = 0;
        m->drone_type       = 0; /* initially set to no car */
        m->appearance       = 0;
        gc->data_valid      = 0;
        gc->objnum          = -1;
        gc->appearance      = 0;
        gc->score           = 0;
        gc->mpath.mpi       = 0;
        gc->mpath.xrel      = 0;
        gc->mpath.yrel      = 0;
        gc->mpath.mpath_index = 0;
        m->can_we_cheat     = false;

        m->fastin.modelrun              = 0;
        gc->distance                    = 1;
        gc->place_locked                = false;
        gc->crashtime                   = 0;
        m->stuck_time                   = 0;
        m->collidable                   = true;
        gc->collidable                  = true;
        gc->collide_time                = 0;
        m->drone_target                 = gThisNode;
        m->drone_scale                  = 1.0;
        m->time_boost                   = 1.0;
        m->please_abort_me              = 0;
        gc->last_distance               = -100;
        m->catchup                      = 1.0;
        m->resurrect.moving_state       = -1;
        m->crashflag                    = 0;
        gc->we_died                     = false;
        gc->just_crashed                = 0;
        gc->crashtime                   = 0;
        gc->old_shortcut                = -1;
        gc->sound_flag_time             = 0;
        gc->scut_flag_time              = 0;
        m->hide_car                     = false;
        gc->difficulty                  = 0;

        /* if slot is other player or drone; process */
        m->in_game  = (gLink[i].flags & LINK_ACTIVE) || (gLink[i].owner<MAX_LINKS);
        if (m->in_game)
            {
            model[num_active_cars++].slot   = i;
            gc->place                       = gLink[i].slot;
            m->drone_type                   = (gLink[i].owner<MAX_LINKS) ? DRONE : HUMAN;

            if (m->drone_type == HUMAN)
                {
                if (gLink[i].difficulty < difficulty[trackno])
                    difficulty[trackno] = gLink[i].difficulty;

                if (gLink[i].marker_flag)
                    marker_flag = true;
                }

            if (m->we_control)
                {
                m->carnum           = 1;
                m->body_type        = gLink[i].car;
                m->engine_type      = 2;
                m->autotrans        = 1;
                m->mainin.autotrans = 1;
                m->data_valid       = 1;
                gc->data_valid      = 1;
                }

            if (i == this_node)
                {
                m->carnum       = gThisCar % num_cars;
                if ((gThisCar == 1) || (gThisCar == 6) || (gThisCar == 8))
                    which_engine = 0;
                else
                    which_engine = 1;

                if (demo_game==0 || (demo_game && attract_effects))
                    StartEngineSound();
                m->body_type    = gThisCar;
                m->engine_type  = which_engine;
                m->data_valid   = 1;
                gc->data_valid  = 1;
                }

            m->net_node = i;

            if (demo_game)
                m->last_cp_index = (U16)demo_checkpoint % (U16)number_checkpoints;
            else
                m->last_cp_index = 0;

            m->next_cp_index = get_next_checkpoint (m->last_cp_index);
            gc->mpath_laps = 0;
            gc->laps = 0;
            gc->lap_sync_time = 0;
            m->laps = 0;
            m->mlap = 0;
            m->mmpath_lap = 0;
            gc->syncmpath_laps = 0;
            m->mcheckpoint = m->last_cp_index;

            /*  Set up tires for other node's cars. */
            if (i != gThisNode && !m->we_control)
                {
                for (j=0; j<4; ++j)
                    {
                    veccopy( carp->TIRER[j], m->TIRER[j] );
                    m->tires[j].tradius = carp->tires[j].tradius;
                    }
                sizep = &collsize[m->body_type];
                m->BODYR[0][0] = m->BODYR[1][0] = sizep->colfront;  /* Front X */
                m->BODYR[2][0] = m->BODYR[3][0] = -sizep->colrear;  /* Back X */
                m->BODYR[0][1] = m->BODYR[2][1] = sizep->colside;   /* Right Y */
                m->BODYR[1][1] = m->BODYR[3][1] = -sizep->colside;  /* Left Y */
                m->BODYR[0][2] = m->BODYR[1][2] = -sizep->colheight;    /* Top Z */
                m->BODYR[2][2] = m->BODYR[3][2] = -sizep->colheight;    /* Top Z */
                maxlen = (sizep->colfront > sizep->colrear) ? sizep->colfront : sizep->colrear;
                m->colrad = sqrt(   (maxlen*maxlen) +
                            (sizep->colside*sizep->colside) +
                            (sizep->colheight*sizep->colheight));
                }
            }
        }

    lap_flag = false;

    sched_to_use = use_sched[num_our_drones];
    sched_index = 0;
    cur_drone = 0;
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Fake out initialization for attract mode                             */

void fake_init(S16 trk)
{
    S16         i, slot;
    static S16  fake_track;

    if (0 <= trk && trk < NTRACKS)
        trackno = trk;
    else
        {
        if (++fake_track>=NTRACKS || fake_track<0)
            fake_track = 0;

        trackno = fake_track;
        }

    for (i=0,slot=0; i<MAX_LINKS; i++)
        {
        if (i == gThisNode)
            {
            gLink[i].flags = 0;
            gLink[i].owner = MAX_LINKS+i;
            }
        else
            {
            gLink[i].owner = gThisNode;
            gLink[i].slot = slot++;
            gLink[i].flags = LINK_TLOCK | LINK_CLOCK | LINK_SOLO | LINK_DRONES | LINK_CATCHUP;
            gLink[i].track = trackno;
            }
        }

    this_node++;
    this_node %= MAX_LINKS;

    if (this_node == gThisNode)
        {
        this_node++;
        this_node %= MAX_LINKS;
        }

    /* get a pseudo random number */
    demo_path_point = -1;

    autotrans = true;
    solo_flag = true;
    gUseCatchup = true;

    SetComChannel (current_mcast_channel);
    gSimID = current_mcast_channel+200;
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Find index of demo path point start and preceding checkpoint         */

S16 get_demo_path_point (void)
{
    S16        i, j, px, py, pz, dx, dy, dz, close_chkpnt;
    S16        *demo_points, pindex, dindex, start, end;
    S32        dist, closest_dist;
    static S16 cur_demo_ppoint[MAX_LINKS];
    CheckPoint *chkpnt;

    if (demo_path_point == -1)
        {
        dindex = cur_demo_ppoint[trackno] + 1;
        if (dindex < 0 || dindex >= num_demo_ppoints[trackno])
            dindex = 0;
        cur_demo_ppoint[trackno] = dindex;

        demo_points = demo_path_points[trackno];

        px = demo_points[dindex*3 + 0];
        py = demo_points[dindex*3 + 1];
        pz = demo_points[dindex*3 + 2];

        closest_dist = 99999999;

        /* determine closest track center (linear search method) */
        for (j=0; j<number_checkpoints+1; j++)
            {
            start = path_dist_index[j];
            end = path_dist_index[j+1];

            for (pindex=start; pindex<end; pindex++)
                {
                i = path_index[pindex];

                dx =  path[i].fpos[1] - px;
                dy = -path[i].fpos[2] - py;
                dz =  path[i].fpos[0] - pz;

                dist = dx*dx + dy*dy + dz*dz;

                if (dist < closest_dist)
                    {
                    closest_dist = dist;
                    demo_path_point = pindex;
                    demo_checkpoint = j;
                    }
                }
            }
        }

    return (demo_path_point);
}

/* ----------------------------------------------------------------------------
*
*       Initialize car model
*       Upon entry: mode -> Initialize = once at start of race
*                           Do_it = all other times for resurrection
*                   drone_index -> drone/car to initialize                   */

void Init_MDrive (S16 mode, S16 drone_index)
    {
    volatile long temp;
    S32 i, color;

    /*  Make the hulk back into a real car if we've crashed. */
    if (mode == Do_it)
        SetupCar (drone_index, model[drone_index].body_type, true);

    model[drone_index].fastin.modelrun = 0;

    if (drone_index == gThisNode)
        {
        gSWpos          = 0;
        gear            = 1;
        brake           = 0;
        ignition        = 1;
        startermotor    = 1;
        crashthreshold  = 0x7fff;
        gStrayAngle     = 0;
        }

    if (drone_index == this_node)
        {
        view = saveview;
        SetMCamera (mode);
        }

    /* do this for drones, once */
    /* this should be cleaned and put right when model is revamped */
    if (mode == Initialize)
        {
        gSWpos          = 0;
        gear            = 1;
        brake           = 0;
        ignition        = 1;
        startermotor    = 1;
        crashthreshold  = DEFCRASHTHRESH;
        }

    modelrun = 1;

    multiinit (mode, drone_index);
}   /* end Init_MDrive */

/* ----------------------------------------------------------------------------- */
void Update_MDrive(void)
{
    S16  i, old_view;
    S32  cur_msb, cur_lsb;

    ReadGasAndBrake();
    ignition    = 1;
    startermotor= 1;

    /* check cars/drones that are being resurrected */
    check_if_finished_resurrecting();

    /* update all models */
    multicomm();

    /* update game task's version of model data */
    update_game_data();
    CheckCameraView();
}   /* end Update_MDrive */

/* ----------------------------------------------------------------------------- */
/* create uv object */
long InitUvObj(S32 objnum)
{
    /*  Same as InitChildObh with no parent. */
    return InitChildObj( objnum, -1 );
}   /* end InitUvObj */


/* ----------------------------------------------------------------------------- */
static void multiinit (S16 mode, S16 drone_index)
{
    S32 i;
    MODELDAT *m;

    m = &model[drone_index];

    if (mode == Initialize)
        {
        m->initin.polepos = gLink[drone_index].slot;
        set_resurrection_data (Initialize, m);
        }
    else
        m->initin.polepos = drone_index + 8;

    /* use initialized/stored resurrection data to place car */
    resurrect_car (m);

    game_car[drone_index].we_died       = false;
    game_car[drone_index].just_crashed  = 0;
    game_car[drone_index].crashtime     = 0;
}

/* ----------------------------------------------------------------------------- */
void multicomm(void)
{
    S16       i;
    MODELDAT  *m;

    communication();    /* chat with the model proc */

}

/* ----------------------------------------------------------------------------- */
/* -------------------
      COMMUNICATION
   -------------------
 */
void communication(void)
{
    short ang, sang, cang, i, vel;
    long unimove[3], modloc[3];
    S32 xv,yv,zv;

    crashthreshold = DEFCRASHTHRESH;

    PutModelInfo(&model[gThisNode]);
    model[gThisNode].mainin.clutch = ReadGear( &model[gThisNode].mainin.gear );

    rpm         = model[this_node].rpm;
    gas         = 8 * 4;

/*This should be in a better place */
    /* keep screwy scaling for now. */
    xv = model[this_node].RWV[XCOMP] * 2.56;
    yv = model[this_node].RWV[YCOMP] * 2.56;
    zv = model[this_node].RWV[ZCOMP] * 2.56;
    velsq = xv*xv + yv*yv + zv*zv;

}

/* ----------------------------------------------------------------------------- */

void init_model_task (void)
{
    last_m_levels = get_levels();
    last_m_dlevels = get_dlevels();
}

/* ----------------------------------------------------------------------------- */

void model_iteration(void)
{
    S16 i;
    S32 curtime;

    /* */
    gnUpdate();
    sndUpdate();

    PotsRead();
    handle_mcast_channel (Do_it, -1);
    send_model_countdown();

    /* This is the time snapshot for this model iteration.       */
    /* The player model will be time stamped with this time.     */
    /* Any drone models run will be time stamped with this time. */
    /* All models are dead reckoned to the time.                 */
    curtime = GetNetTime(); /* interrupt safe. */

    /*global scaled steering wheel pos. Update once per model.*/
    gSWpos = wheelpos();

    m_levels = get_levels();
    m_dlevels = get_dlevels();
    m_edges = ~last_m_levels & m_levels;
    m_dedges = ~last_m_dlevels & m_dlevels;

    last_m_levels = m_levels;
    last_m_dlevels = m_dlevels;

    if (gstate==COUNTDOWN || gstate==PREPLAY2)
        force_wheel_to_center (0, 4, 11, 80);

    if (gstate != PLAYGAME && demo_game==0 && gstate!=COUNTDOWN)
        {
        /* clear message from fast queue */
        ProcessPDUs(true);

        for (i=0; i<MAX_LINKS; i++)
            carz[i].fresh = false;

        return;
        }

    if (modelrun == 0)
        return;

    /* update data from network cars */
    update_link_cars();

    /* generate dead reckon data for all in_game*/
    dead_reckon_all(curtime);

    /* update local models */
    update_player_model(curtime);
    update_drone_models(curtime);

    UpdateListener (true, !(demo_game && attract_effects));
}

/*---------------------------------------------------------------------------*/

void update_player_model(S32 curtime)
{
    MODELDAT *m = &model[gThisNode];

#if KILLMAININ
    m->throttle = gGasVal;
#endif
    if (gstate == PLAYGAME)
        {
        wheel = (gSWpos)*(INVCTLSCALE / 0x800);
        m->fastin.wheel = wheel;
        m->fastin.swvel = rawswvel;
        }

    /* This should be after model runs. */
    if (gstate == PLAYGAME)
        {
        gSWtorque = m->swtorque;
        gSWdamp = m->swdamp;
        gSWfrict = m->swfrict;
        }

    update_model(gThisNode, curtime);

}


/*---------------------------------------------------------------------------*/

void update_drone_models(S32 curtime)
{
    S16 i, num_drones_to_do;

    /* if at end of list, wrap back to slot 0 */
    if (drone_sched[sched_to_use][++sched_index] == -1)
        {
        sched_index = 0;
        cur_drone = 0;
        }

    /* get number of drones to do this model slice */
    num_drones_to_do = drone_sched[sched_to_use][sched_index];

    if (num_drones_to_do == 0)
        return;

    for (i=0; i<num_drones_to_do; i++)
        {
        update_model (our_drones[cur_drone], curtime);

        if (okay_to_check_skid())
            DoTireSqueals (our_drones[cur_drone]);
        cur_drone++;
        }
}

/*---------------------------------------------------------------------------*/

void update_model(S32 slot, S32 curtime)
{
    MODELDAT    *m = &model[slot];
    F32         modeltime;
    S16         result;
    BOOL        player = (slot == gThisNode);

    modeltime = (F32)(curtime - m->fastin.lasttime) * gNetTimeToSec;
    if(modeltime > 0.1)
        modeltime = 0.1;
    if(modeltime < 0.001)
        modeltime = 0.001;
    m->fastin.lasttime = curtime;

    /* scaled roadboost = 1.0 to 2.0*/
    m->time_fudge = m->catchup * m->time_boost  * (1.0 + (F32)m->roadboost[0] * 0.06667);
    m->fastin.modeltime = modeltime * m->time_fudge;

    if (m->resurrect.moving_state > -1)
        {
        if(player)
            {
            if (gFlyMode)
                flyaround (Do_it);
            else if (mpath_edit)
                update_mpath_edit (Do_it);
            else
		{
		/* freeze model when starting resurrect. */
		if(m->resurrect.moving_state == 0)
			update_reckon_base(slot, curtime);
                update_resurrecting_car (m);
		}

            UpdateListener (false, false);

            /*set steering wheel force based on new model state.*/
            if (gstate == PLAYGAME)
                ModelSteerForce();
            }
        else    /*drone*/
		{
		/* freeze model when starting resurrect. */
		if(m->resurrect.moving_state == 0)
			update_reckon_base(slot, curtime);
		update_resurrecting_car (m);
		}
        }
    else if (m->fastin.modelrun)
        {
        if(player)
            {
            sym(m); /* Call model main loop */
            UpdateListener (true, false);

            /*set steering wheel force based on new model state.*/
            if (gstate == PLAYGAME)
                ModelSteerForce();
            }
        else    /*drone*/
            {
            MaxPathControls(m); /* set wheel,accel,brake */
            sym(m); /* Call model main loop */
            }
        if(reckon_check(slot))
            update_reckon_base(slot, curtime);
        }
}

S16  quat_index;

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Update drones from other network games                               */

void update_link_cars (void)
{
    S32             i, j, state;
    F32             num_steps;
    U32             roadlook;
    MODELDAT        *m;
    MPCTL           *cp;
    BOOL            fresh[MAX_LINKS];
    EntityStateData *es;

    ProcessPDUs(true);

    for (i=0; i<MAX_LINKS; i++)
        {
        fresh[i] = 0;
        m = &model[i];
        if (m->in_game)
            {
            es = &carz[i];
            if ((fresh[i] = !(m->we_control || i==gThisNode) && es->fresh) != 0)
                {
                state = prc_set_ipl (INTS_OFF);

                m->reckon.base_time = es->timeStamp;
                m->reckon.base_fudge = es->timeFudge;
                veccopy(es->acc, m->reckon.base_RWA);
                veccopy(es->vel, m->reckon.base_RWV);
                veccopy(es->pos, m->reckon.base_RWR);
                veccopy(es->angv, m->reckon.base_W);

                if (m->data_valid)
                    {
                    find_best_quat (m->reckon.base_quat, es->quat);

                    num_steps = 30;
                    m->reckon.num_quat_steps = num_steps;

                    for (j=0; j<4; j++)
                        m->reckon.delta_quat[j] = (es->quat[j] - m->reckon.base_quat[j]) * (1.0/num_steps);
                    }
                else
                    {
                    make_uvs_from_quat (es->quat, m->reckon.base_UV);
                    fmatcopy(m->reckon.base_UV[0], m->reckon.UV[0]);
                    m->reckon.num_quat_steps = 1;
                    for (j=0; j<4; j++)
                        {
                        m->reckon.delta_quat[j] = 0;
                        m->reckon.base_quat[j] = es->quat[j];
                        }
                    }

                m->reckon.torque = es->torque;
                m->reckon.rpm = es->rpm;
                m->reckon.steerangle = es->steerangle;
                for(j=0;j<4;j++)
                    {
                    m->reckon.tireW[j] = es->tireV[j];
                    m->reckon.suscomp[j] = es->suscomp[j];
                    m->reckon.base_airdist[j] = es->shadow_h[j];
                    m->reckon.base_airvel[j] = es->shadow_v[j];
                    }

                m->mcheckpoint = es->checkpoint;
                m->mlap = es->lap;
                m->mmpath_lap = es->mpath_lap;

                m->reckon.look = es->look;
                m->reckon.appearance = es->appearance;

                /*TEMP - stuff where expected*/
                veccopy(m->reckon.base_RWR, m->RWR);
                veccopy(m->reckon.base_RWV, m->RWV);
                veccopy(m->reckon.base_W, m->W);
                m->rpm = m->reckon.rpm;
                m->steerangle = m->reckon.steerangle;
                for(j=0;j<4;j++)
                    {
                    m->tires[j].angvel = m->reckon.tireW[j];
                    m->suscomp[j] = m->reckon.suscomp[j];
                    }
                m->engtorque            =   m->reckon.torque;
                m->engine_type          =   (m->reckon.look >> ENG_TYPE_OFFSET)   & ENG_TYPE_MASK;
                m->body_type            =   (m->reckon.look >> CAR_TYPE_OFFSET)   & CAR_TYPE_MASK;
                m->collidable           =   (m->reckon.look >> COL_TYPE_OFFSET)   & COL_TYPE_MASK;
                roadlook                =   (m->reckon.look >> ROADCODE_OFFSET)   & ROADCODE_MASK;
                m->hide_car             =   (m->reckon.look >> HIDE_CAR_OFFSET)   & HIDE_CAR_MASK;
                for (j=0; j<4; ++j)
                    m->sviscode[j]      = (roadlook >> (3-j)*3) & 7;
                m->appearance           =   m->reckon.appearance;
                m->time_fudge           =   m->reckon.base_fudge;
                /*TEMP - stuff where expected*/

                es->fresh               =   false;
                m->data_valid           =   true;
                prc_set_ipl (state);
                }
            }
        }

    for (i=0; i<MAX_LINKS; i++)
        {
        m = &model[i];

        if (!(i == gThisNode || m->we_control))
            {
            m->magvel = sqrt( m->reckon.RWV[0]*m->reckon.RWV[0] +
                              m->reckon.RWV[1]*m->reckon.RWV[1] +
                              m->reckon.RWV[2]*m->reckon.RWV[2]);

            /* compute mph from rear tire velocity (assume 1 ft tire radius). */
            m->mph = (m->tires[2].angvel + m->tires[3].angvel) * .5 * (3600.0/5280.0);
            }
        }
}



/* ----------------------------------------------------------------------------- */
/* --------------------
      PUT MODEL INFO
   --------------------
 */
void PutModelInfo(MODELDAT *m)
{
    m->mainin.clutch            = clutch;
    m->mainin.brake             = brake;
#if !KILLMAININ
    m->mainin.throttle          = gGasInt;
#endif
    m->mainin.gear              = gear;
    m->mainin.ignition          = ignition; /* set ignition */
    m->mainin.startermotor      = startermotor;
    m->mainin.crashthreshold    = crashthreshold;
}

/* ----------------------------------------------------------------------------- */
/* ----------------
      MODEL STOP
   ----------------
 */
void modelstop(void)
{
    modelrun = 0;
}

/* ----------------------------------------------------------------------------- */
void multigo (S16 drone_index)
{
    S32 i;


    PutModelInfo (&model[drone_index]);

    model[drone_index].fastin.modelrun = 1;
}

/* ----------------------------------------------------------------------------- */
/* ----------------------------------
      MODEL TO DISPLAY UNIT VECTOR
   ----------------------------------
 */
void modtodispuv(short moduv[3][3], short camuv[3][3])
{
    camuv[0][0] = moduv[1][1];
    camuv[0][1] = -moduv[1][2];
    camuv[0][2] = moduv[1][0];
    camuv[1][0] = -moduv[2][1];
    camuv[1][1] = moduv[2][2];
    camuv[1][2] = -moduv[2][0];
    camuv[2][0] = moduv[0][1];
    camuv[2][1] = -moduv[0][2];
    camuv[2][2] = moduv[0][0];
}

/* ----------------------------------------------------------------------------- */
void fmodtodispuv(F32 moduv[3][3], F32 camuv[3][3])
{
    camuv[0][0] = moduv[1][1];
    camuv[0][1] = -moduv[1][2];
    camuv[0][2] = moduv[1][0];
    camuv[1][0] = -moduv[2][1];
    camuv[1][1] = moduv[2][2];
    camuv[1][2] = -moduv[2][0];
    camuv[2][0] = moduv[0][1];
    camuv[2][1] = -moduv[0][2];
    camuv[2][2] = moduv[0][0];
}

F32 dr_campos[3],dr_camuvs[3][3];

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Update game task's version of model data                             */

/* update dead reckon to currect time. copy (and coord. chage) to game_car. */
void update_game_data (void)
{
    S16       i, j, k, l, high_index, same_count[4];
    U16       snd_flags;
    S32       curtime;
    MODELDAT  *m;
    CAR_DATA  *gc;
    F32       temp[3];

    /* Hold model task off while accessing. */
    gModeldatLock = 1;

    /* dead reckon all cars to current time. */
    curtime = GetNetTime(); /* interrupt safe. */
    game_reckon_all(curtime);

    /*copy to game_car*/
    for (i=0; i<MAX_LINKS; i++)
        {
        if (model[i].in_game)
            {
            m = &model[i];
            gc = &game_car[i];

            /* copy and change coord. */
            gc->dr_pos[0] =  m->reckon.RWR[YCOMP];
            gc->dr_pos[1] = -m->reckon.RWR[ZCOMP];
            gc->dr_pos[2] =  m->reckon.RWR[XCOMP];

            /*copy and change coord.*/
            gc->dr_uvs[0][0] =  m->reckon.UV[1][1];
            gc->dr_uvs[0][1] = -m->reckon.UV[1][2];
            gc->dr_uvs[0][2] =  m->reckon.UV[1][0];
            gc->dr_uvs[1][0] = -m->reckon.UV[2][1];
            gc->dr_uvs[1][1] =  m->reckon.UV[2][2];
            gc->dr_uvs[1][2] = -m->reckon.UV[2][0];
            gc->dr_uvs[2][0] =  m->reckon.UV[0][1];
            gc->dr_uvs[2][1] = -m->reckon.UV[0][2];
            gc->dr_uvs[2][2] =  m->reckon.UV[0][0];


            /* tire positions - not dead reckoned*/
            MOVMEM((char *)(m->TIRER),   (char *)(gc->TIRER),   sizeof(F32)*4*3);

            /*generate tire pos from dead reckoned data.*/
            for(j=0;j<4;j++)
                {
                temp[0] =  m->TIRER[j][YCOMP];
                temp[1] = -m->TIRER[j][ZCOMP];
                temp[2] =  m->TIRER[j][XCOMP];
                dbodtorw(temp, gc->dr_tirepos[j], gc->dr_uvs);
                vecadd(gc->dr_tirepos[j], gc->dr_pos, gc->dr_tirepos[j]);
                }

            gc->dr_acc[0] =  m->reckon.base_RWA[1];
            gc->dr_acc[1] = -m->reckon.base_RWA[2];
            gc->dr_acc[2] =  m->reckon.base_RWA[0];

            gc->dr_vel[0] = m->reckon.RWV[YCOMP];
            gc->dr_vel[1] = -m->reckon.RWV[ZCOMP];
            gc->dr_vel[2] = m->reckon.RWV[XCOMP];

            veccopy(m->reckon.RWV, gc->RWV);
            veccopy(m->reckon.RWR, gc->RWR);

            /*need to figure out skid and shadow later*/
            for (j=0; j<4; j++)
                {
                gc->tireW[j] = m->reckon.tireW[j];
                for (k=0; k<3; k++)
                    gc->TIRERWR[j][k] = m->TIRERWR[j][k];
/*                  gc->TIRERWR[j][k] = m->TIRERWR[j][k] + m->RWV[k] * reckon_factor;*/;
                }


            gc->body_type   = (m->reckon.look >> CAR_TYPE_OFFSET) & CAR_TYPE_MASK;
            gc->engine_type = (m->reckon.look >> ENG_TYPE_OFFSET) & ENG_TYPE_MASK;
            gc->rpm         = m->reckon.rpm;
            gc->crashflag   = m->crashflag;


            /*  Update appearance flags that depend on other factors. */
            if (m->we_control || i == gThisNode)
                {
                /* first do sound codes */
                /* go through all tires */
                for (k=0; k<4; k++)
                    same_count[k] = 0;

                for (k=0; k<4; k++)
                    {
                    if ((m->sviscode[k] != AIR) || (m->sound_flags[k]))
                        {
                        for (l=0; l<4; l++)
                            {
                            if (m->sound_flags[k]==m->sound_flags[l])
                                same_count[k]++;
                            }
                        }
                    }

                high_index = 0;

                /* find most common type under all tires */
                for (k=1; k<4; k++)
                    {
                    if (same_count[k] > same_count[high_index])
                        high_index = k;
                    }

                if (same_count[high_index] != 0 && gc->sound_flags != m->sound_flags[high_index])
                    gc->sound_flags = m->sound_flags[high_index];

                /* update tunnel flag */
                snd_flags = gc->sound_flags & (ALL_REVERB_MASK | ALL_DIST_MASK);
                gc->in_tunnel = (((snd_flags & UP2_REVERB_MASK) != 0) << 1) + ((snd_flags & UP_REVERB_MASK) != 0);

                /* now do shorcut codes */
                /* go through all tires */
                for (k=0; k<4; k++)
                    same_count[k] = 0;

                for (k=0; k<4; k++)
                    {
                    if ((m->sviscode[k] != AIR) || (m->shortcut_flags[k]))
                        {
                        for (l=0; l<4; l++)
                            {
                            if (m->shortcut_flags[k]==m->shortcut_flags[l])
                                same_count[k]++;
                            }
                        }
                    }

                high_index = 0;

                /* find most common type under all tires */
                for (k=1; k<4; k++)
                    {
                    if (same_count[k] > same_count[high_index])
                        high_index = k;
                    }

                if (same_count[high_index] != 0 && gc->shortcut_flags != m->shortcut_flags[high_index])
                    {
                    if (gc->scut_flag_time)
                        {
                        if (IRQTIME - gc->scut_flag_time > ONE_SEC/5)
                            {
                            gc->shortcut_flags = m->shortcut_flags[high_index];
                            gc->scut_flag_time = 0;
                            }
                        }
                    else
                        gc->scut_flag_time = IRQTIME;
                    }
                else
                    gc->scut_flag_time = 0;

                if (m->crashflag)
                    m->appearance |= App_M_HULK | App_M_SMOKE;
                else if (!(m->appearance & App_M_HULK))
                    m->appearance &= ~App_M_SMOKE;

                if (m->brake)
                    m->appearance |= App_M_BRAKE_LIGHTS;
                else
                    m->appearance &= ~App_M_BRAKE_LIGHTS;

                if ((m->magvel>20) && m->BODYFORCE[0][1] && m->BODYFORCE[2][1])
                    m->appearance |= App_M_RSPARK;
                else
                    m->appearance &= ~App_M_RSPARK;

                if ((m->magvel>20) && m->BODYFORCE[1][1] && m->BODYFORCE[3][1])
                    m->appearance |= App_M_LSPARK;
                else
                    m->appearance &= ~App_M_LSPARK;

                if (m->thumpflag > 2)
                    m->appearance |= App_M_BSPARK;
                else if (m->thumpflag == 0)
                    m->appearance &= ~App_M_BSPARK;

                if (gc->collidable == false && m->collidable == true)
                    m->appearance |= App_M_TRANSLUCENT;

                else if (gc->collidable == true && m->collidable == false)
                    m->appearance &= ~App_M_TRANSLUCENT;
                for (j=0; j<4; ++j)
                    CheckQuadDamage(m,j);

                /* not dead reckoned */
                gc->V[YCOMP] = m->V[YCOMP];
                gc->V[ZCOMP] = m->V[ZCOMP];
                gc->V[XCOMP] = m->V[XCOMP];

                gc->mph = m->mph;

                for (j=0; j<4; j++)
                    {
                    gc->road_norm[j][0] = -m->roaduvs[j].fpuvs[1][2];
                    gc->road_norm[j][1] =  m->roaduvs[j].fpuvs[2][2];
                    gc->road_norm[j][2] = -m->roaduvs[j].fpuvs[0][2];
                    }

                m->mlap = m->laps;
                m->mmpath_lap = gc->mpath_laps;
                m->mcheckpoint = m->last_cp_index;
                } /* end: this_node and our drones */

            gc->laps = m->mlap;
            gc->syncmpath_laps = m->mmpath_lap;
            gc->checkpoint = m->mcheckpoint;
            gc->mph = m->mph;

            if ((   gc->we_died == false)
                && (m->crashflag == false)
                && (m->resurrect.moving_state == -1))
                /* update maxpath interval */
                update_mpath_index (i);

            gc->appearance = m->appearance;
            gc->collidable = m->collidable;

            if (gc->collidable==false && gc->collide_time==0)
                {
                gc->collide_time = IRQTIME;
                gc->collide_state = -1;
                gc->collide_count = -1;
                }

            if (gc->collidable == true)
                gc->collide_time = 0;

            gc->data_valid = m->data_valid;
            }
        }

    gModeldatLock = 0;

}   /*  end update_game_data */

/* ----------------------------------------------------------------------------- */

void dbodtorw(F32 ivec[3], F32 ovec[3], F32 mat[3][3])
{
    S32 i,j;

    for(i=0;i<3;i++)
        {
        ovec[i]=0.0;
        for(j=0;j<3;j++)
            ovec[i] += (ivec[j] * mat[i][j]);
        }
}

/* ----------------------------------------------------------------------------- */
#define FORCE_DAMAGE_THRESHOLD 1.0e9
static void CheckQuadDamage( MODELDAT *m, S32 q )
{
    S16 damage;
    F32 mag_force = m->BODYFORCE[q][0]*m->BODYFORCE[q][0] + m->BODYFORCE[q][1]*m->BODYFORCE[q][1] +
                    m->BODYFORCE[q][2]*m->BODYFORCE[q][2];
/*
    if (m->slot == this_node)
        {
        static F32 max[4];
        if (mag_force > max[q])
            max[q] = mag_force;
        SMSG( 10+q, 2, "F:%15.1lf Max:%15.1lf", mag_force, max[q] );
        }
*/

    if (mag_force < FORCE_DAMAGE_THRESHOLD || !m->collidable)
        return;

    /* dont damage car when suspension compresses to top points. */
    if(m->suscomp[q] > 2.0)
        return;

    damage = 1 + ((m->appearance & gDamageMask[q]) >> gDamageShift[q]);
    if (damage < MAX_DAMAGE)
        m->appearance = (m->appearance & ~gDamageMask[q]) | (damage << gDamageShift[q]);
}   /*  end CheckQuadDamage */


/* ----------------------------------------------------------------------------- */
void printslip(void)
{
    MODELDAT    *m;
    F32         sfmax,sforce,sliprat;
    char        xxnumbuf[80];

    m = &model[gThisNode];
#if 0
    sfmax = m->TIREFORCE[0][2] * m->tires[0].Cfmax;
    sforce = m->TIREFORCE[0][1];
#else
    sfmax = m->TIREFORCE[0][2];
    sforce = m->TIREFORCE[0][1] * m->tires[0].Cfmax;
#endif
    sliprat = ((sfmax < 0.01)&&(sfmax > -0.01)) ? 0.0 : sforce/sfmax;

    sprintf(xxnumbuf,"slipratio = %0.6f    ",(double)sliprat);
    txt_str(34,34,xxnumbuf,RED_PALB);
}   /* end printslip */

/* ----------------------------------------------------------------------------- */
void End_MDrive (S16 drone_index)
{
    if (drone_index == gThisNode)
        swforce = 0;

    model[drone_index].fastin.modelrun  = 0;
/*  model[drone_index].appearance       = 0; */
}   /* end End_MDrive */

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Check if all cars have stopped moving
*         Upon exit: returned -> false = someone's still moving
*                                true = all stopped                          */

S16 are_all_cars_stopped (void)
{
    S16 i, node, done_flag;

    /* assume all stopped */
    done_flag = true;

    for (i=0; i<num_active_cars; i++)
        {
        node = model[i].slot;

        if (model[node].drone_type==HUMAN && model[node].magvel>4)
            {
            done_flag = false;
            break;
            }
        }

    return (done_flag);
}

