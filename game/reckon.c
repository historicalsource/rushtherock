/*
 *	reckon.c  dead reckoning
 *
 *	Copyright 1996 Time Warner Interactive
 *	Unauthorized reproduction, adaptation, distribution, performance or 
 *	display of this computer program or the associated audiovisual work
 *	is strictly prohibited.
 *
 *	======================================================================
 *	$Author: gray $						$Date: 1997/10/10 21:55:15 $
 *	$Revision: 3.26 $						$Locker:  $
 *	======================================================================
 *	Change Log:
 *
 *	$Log: reckon.c,v $
 * Revision 3.26  1997/10/10  21:55:15  gray
 * Limit upward velocity to 10ft/sec for net shadows.
 *
 * Revision 3.25  1997/10/10  02:02:02  gray
 * Move car below world and freeze if resurrecting.
 *
 * Revision 3.24  1997/03/13  03:58:29  geraci
 * added alan's fix to stop net updates during resurrect
 *
 * Revision 3.24  1997/03/10  01:09:31  gray
 * Suppress EntityState updates during resurrect.
 *
 * Revision 3.23  1996/12/05  00:04:40  geraci
 * end of game score removed from entity state
 *
 * Revision 3.22  1996/12/04  01:58:02  geraci
 * checkpoints and end of game scores are handled by entity state updates
 *
 * Revision 3.21  1996/11/27  14:30:02  gray
 * Dead reckon shadow.
 *
 * Revision 3.20  1996/11/27  12:56:10  gray
 * Changed NetTime from F32 to 10microsec S32.
 *
 * Revision 3.19  1996/11/23  02:59:44  geraci
 * initialized quats at start of race better
 *
 * Revision 3.18  1996/11/22  05:01:09  geraci
 * hide resurrecting car defined in look field, quats passed instead of uvs
 *
 * Revision 3.17  1996/11/19  05:55:34  gray
 * Freeze cars if more than 2 seconds from last update.
 *
 * Revision 3.16  1996/11/16  07:53:31  gray
 * Stop dead reckoning after 2 seconds, calc magvel on reckon velocity.
 *
 * Revision 3.15  1996/10/10  05:12:22  petrick
 * Made irqtime a true global in globals_h.
 *
 * Revision 3.14  1996/10/07  19:17:24  gray
 * Temp fix to smooth out local shadows.
 *
 * Revision 3.13  1996/10/02  03:30:46  petrick
 * Turned off debug msg.
 *
 * Revision 3.12  1996/09/30  16:35:51  gray
 * Extrapolate car positions with model data if local (game task).
 *
 * Revision 3.11  1996/09/30  08:18:50  gray
 * Getting rid of old model globals.
 *
 * Revision 3.10  1996/09/24  02:56:26  petrick
 * Deactivated debug msg.
 *
 * Revision 3.9  1996/09/23  10:02:17  gray
 * Fixing dead reckon control.
 *
 * Revision 3.8  1996/09/21  04:13:46  petrick
 * Lowered dead reckoning limits.
 *
 * Revision 3.7  1996/09/19  15:03:00  petrick
 * Cut down on dead reckon kludge time.
 *
 * Revision 3.6  1996/09/19  06:27:02  geraci
 * don't dead reckon messages going out for first three secs
 *
 * Revision 3.5  1996/09/17  11:19:37  petrick
 * Sends road visual codes now instead of road code across net.
 *
 * Revision 3.4  1996/09/16  02:34:33  petrick
 * Filtering implemented.
 *
 * Revision 3.3  1996/09/15  01:07:04  gray
 * *** empty log message ***
 *
 * Revision 3.2  1996/09/12  09:57:43  petrick
 * Now passing roadcode across link.
 *
 * Revision 3.1  1996/09/09  02:24:52  gray
 * JAMMA version
 *
 * Revision 1.5  1996/09/05  01:45:35  gray
 * Passing airdist (for shadow) across link.
 *
 * Revision 1.4  1996/09/04  20:37:22  gray
 * Adding dead reckon data to game_car. Killing mainout.
 *
 * Revision 1.3  1996/09/02  01:23:20  gray
 * Moved initialization of CPU count vars to globals.
 *
 * Revision 1.2  1996/09/01  23:57:28  gray
 * Real Dead Reckon structure.
 *
 * Revision 1.1  1996/09/01  09:12:42  gray
 * Initial revision
 *
 *
 *	======================================================================
 */

#include "dis.h"
#include "globals.h"
#include "modeldat.h"
#include "resurrect.h"
#include "select.h"

#include "Pro/reckon.pro"
#include "Pro/vecmath.pro"
#include "Pro/unitvecs.pro"

/*===============================  DEFINES  =================================*/


/*==============================  PROTOTYPES  ===============================*/

extern void SMSG(S32 r, S32 c, char *fmt, ...);


/*===============================  EXTERNS  =================================*/


/* ----------------------------------------------------------------------------- */
/* _Model_ task dead reckon. */
void dead_reckon_all(S32 time)
{
	MODELDAT *m;
	S32 i, j;
	F32 deltime,temp[3];

	for (i=0; i<MAX_LINKS; i++)
		{
		m = &model[i];
		if(m->in_game)
			{
			/* calc delta time since reckon base */
			deltime = (F32)(time - m->reckon.base_time) * gNetTimeToSec * m->reckon.base_fudge;
			dead_reckon_posuv(m, deltime);
			}
		}
}	/*	end dead_reckon_all */

/* ----------------------------------------------------------------------------- */
/* _Game_ task dead reckon. Assumes model task is suspended. */
void game_reckon_all(S32 time)
{
	MODELDAT *m;
	S32 i, j;
	F32 deltime,temp[3];

	for (i=0; i<MAX_LINKS; i++)
		{
		m = &model[i];
		if(m->in_game)
			{
			if(m->we_control || (i == gThisNode))	/*update from model data (more accurate)*/
				{
				/* calc delta time since last model */
				deltime = (F32)(time - m->fastin.lasttime) * gNetTimeToSec * m->time_fudge;

				/*just copy velocity*/
				veccopy(m->RWV, m->reckon.RWV);

				/* extrapolate position (R' = R0 + V0*dt) */
				scalmul(m->RWV, deltime, temp);
				vecadd(m->RWR, temp, m->reckon.RWR);

				/* dead reckon uvs */
				fmatcopy(m->UV.fpuvs[0], m->reckon.UV[0]);
				scalmul(m->W, deltime, temp);
				rotateuv(temp, m->reckon.UV);

				for(j=0;j<4;j++)
					{
					m->reckon.suscomp[j] = m->suscomp[j];
					m->reckon.airdist[j] = m->airdist[j] + (m->airvel[j] * deltime);
					}

				}
			else	/* update from net data */
				{
				/* calc delta time since reckon base */
				deltime = (F32)(time - m->reckon.base_time) * gNetTimeToSec * m->reckon.base_fudge;
				dead_reckon_posuv(m, deltime);
				for(j=0;j<4;j++)
				m->reckon.airdist[j] = m->reckon.base_airdist[j] + 
					(m->reckon.base_airvel[j] * deltime);
				}
			}
		}
}	/*	end game_reckon_all */

#define RECKON_FREEZE_TIME (2.0)

void dead_reckon_posuv(MODELDAT *m, F32 dt)
{
	S16 i;
	F32 temp[3];

	if(dt > RECKON_FREEZE_TIME)
		{
		/* set velocity to zero if frozen */
		m->reckon.RWV[0] = m->reckon.RWV[1] = m->reckon.RWV[2] = 0.0;
		
		/* dead reckon position to freeze pos (but move below track). */
		/* keep xy pos for race place.*/
		scalmul(m->reckon.base_RWV, RECKON_FREEZE_TIME, temp);
		vecadd(m->reckon.base_RWR, temp, m->reckon.RWR);
		m->reckon.RWR[2] = 200.0;	/* positive z is down for model */

#if 0
		/* dead reckon uvs to freeze uvs */
		fmatcopy(m->reckon.base_UV[0], m->reckon.UV[0]);
		scalmul(m->reckon.base_W, RECKON_FREEZE_TIME, temp);
		rotateuv(temp, m->reckon.UV);
#endif
		m->reckon.num_quat_steps = 0;
		for (i=0; i<4; i++)
			m->reckon.delta_quat[i] = 0;
		make_uvs_from_quat (m->reckon.base_quat, m->reckon.UV);
		}
	else
		{
		veccopy(m->reckon.base_RWV, m->reckon.RWV);

		/* dead reckon position (R' = R0 + V0*dt) */
		scalmul(m->reckon.base_RWV, dt, temp);
		vecadd(m->reckon.base_RWR, temp, m->reckon.RWR);

		if (m->reckon.num_quat_steps > 0)
			{
			m->reckon.num_quat_steps--;
			for (i=0; i<4; i++)
				m->reckon.base_quat[i] += m->reckon.delta_quat[i];
			make_uvs_from_quat (m->reckon.base_quat, m->reckon.UV);
			}


#if 0
		/* dead reckon uvs */
		fmatcopy(m->reckon.base_UV[0], m->reckon.UV[0]);
		scalmul(m->reckon.base_W, dt, temp);
		rotateuv(temp, m->reckon.UV);
#endif
		}
}

S32	skips[8], sends[8], lastSend[8];

void zero_reckon_pcts(void)
{
	S32 i;

	for(i=0;i<8;i++)
		{
		skips[i] = 0;
		sends[i] = 0;
		}
}


/* ----------------------------------------------------------------------------- */
/* Returns 1 if difference between dead reckon and real model data too big, */
/* otherwise returns 0. */
/* ----------------------------------------------------------------------------- */
BOOL reckon_check(S32 slot)
{
	MODELDAT 	*m 		= &model[slot];
	S32			i;
	F32			diff;
	F32			epos, evel;

#define DIST_ERR_LIMIT		(0.25*0.25)			/* .5 Square of maximum position error allowed (ft^2). */
#define VEL_ERR_LIMIT		(0.25*0.25)			/* .25 Square of maximum position error allowed (fps^2). */
#define DEAD_RECKON_TIMEOUT	200				/* Force an update if one not sent after this much time (msec). */


	/* Force a send if just starting to resurrect. */
	if(m->resurrect.moving_state == 0)
		return 1;

	/* suppress network updates during resurrect. */
	if(m->resurrect.moving_state > 0)
		return 0;

	if (IRQTIME - lastSend[slot] > DEAD_RECKON_TIMEOUT)
		evel = epos = DIST_ERR_LIMIT;
	else
		for (epos=evel=i=0; i<3; ++i)
			{
			/*	Check for positional drift. */
			diff	 = m->reckon.RWR[i] - m->RWR[i];
			if ((epos += diff * diff) > DIST_ERR_LIMIT)
				break;

			/*	Check for velocity drift. */
			diff	 = m->reckon.RWR[i] - m->RWR[i];
			if ((evel += diff * diff) > VEL_ERR_LIMIT)
				break;
			}

	/* if position or velocity error too big, flag for update. */
	if (epos >= DIST_ERR_LIMIT || evel > VEL_ERR_LIMIT)
		{
		++sends[slot];
		lastSend[slot] = IRQTIME;
		return 1;
		}

	++skips[slot];
	return 0;

}	/*	end reckon_check */


/* ----------------------------------------------------------------------------- */
void update_reckon_base(S32 slot, S32 time)
{
	MODELDAT *m = &model[slot];
	S32 	i;
	U32 	look, roadlook;

	for (i=roadlook=0; i<4; ++i)
		roadlook = (m->sviscode[i] & 7) | (roadlook << 3);

	look = ( ((m->engine_type      & ENG_TYPE_MASK) << ENG_TYPE_OFFSET) |
			 ((m->body_type        & CAR_TYPE_MASK) << CAR_TYPE_OFFSET) |
			 ((m->collidable       & COL_TYPE_MASK) << COL_TYPE_OFFSET) |
			 ((roadlook            & ROADCODE_MASK) << ROADCODE_OFFSET) |
			 ((m->hide_car         & HIDE_CAR_MASK) << HIDE_CAR_OFFSET));

	m->reckon.base_time = time;
	m->reckon.base_fudge = m->time_fudge;
	veccopy(m->RWA, m->reckon.base_RWA);
	veccopy(m->RWV, m->reckon.base_RWV);
	veccopy(m->RWR, m->reckon.base_RWR);
	veccopy(m->W, m->reckon.base_W);
	fmatcopy(m->UV.fpuvs[0], m->reckon.base_UV[0]);
	fmatcopy(m->reckon.base_UV[0], m->reckon.UV[0]);
	make_quat_from_uvs (m->reckon.base_UV, m->reckon.base_quat);
	for (i=0; i<4; i++)
		m->reckon.delta_quat[i] = 0;
	m->reckon.torque = m->engtorque;
	m->reckon.rpm = m->rpm;
	m->reckon.steerangle = m->steerangle;
	for(i=0;i<4;i++)
		{
		m->reckon.tireW[i] = m->tires[i].angvel;
		m->reckon.suscomp[i] = m->suscomp[i];
		m->reckon.base_airdist[i] = m->airdist[i];
		/* limit upward velocity to 10ft/sec for net shadows. */
		if(m->airvel[i] < -10.0)
			m->reckon.base_airvel[i] = -10.0;
		else
			m->reckon.base_airvel[i] = m->airvel[i];
		}
	m->reckon.look = look;
	m->reckon.appearance = m->appearance;
	for(i=0;i<3;i++)
		m->reckon.base_RWV[i] = m->RWV[i] * m->time_fudge;

	/* if resurrecting, zero velocities. */
	if(m->resurrect.moving_state >= 0)
		{
		for(i=0;i<3;i++)
			{
			m->reckon.base_RWA[i] = 0.0;
			m->reckon.base_RWV[i] = 0.0;
			m->reckon.base_W[i] = 0.0;
			}
		for(i=0;i<4;i++)
			m->reckon.base_airvel[i] = 0.0;
		/* move car to 200 ft _below_ sea level. */
		/* keep xy pos for race place. */
		m->reckon.base_RWR[2] = 200.0;	/* positive z is down for model */
		}

	if ( !solo_flag && (m->we_control || slot == gThisNode) )
		PutEntityStatePDU(slot);

}	/*	end update_dead_reckon */


