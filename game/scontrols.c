/*
 *  controls.c: this file copies control data to model
 *
 *	Copyright 1996 Time Warner Interactive
 *	Unauthorized reproduction, adaptation, distribution, performance or 
 *	display of this computer program or the associated audiovisual work
 *	is strictly prohibited.
 *
 *	======================================================================
 *	$Author: gray $						$Date: 1997/06/04 02:15:46 $
 *	$Revision: 3.6 $						$Locker:  $
 *	======================================================================
 *	Change Log:
 *
 *	$Log: controls.c,v $
 * Revision 3.6  1997/06/04  02:15:46  gray
 * Restructuring controls (gas,brake,clutch,etc) handling.
 *
 * Revision 3.5  1997/02/15  01:43:32  geraci
 * end of game braking is hard when crossing finish line normal otherwise
 *
 * Revision 3.4  1996/11/17  09:07:05  geraci
 * skip ahead fixed
 *
 * Revision 3.3  1996/11/16  18:58:15  geraci
 * stop cars that finish race
 *
 * Revision 3.2  1996/09/17  08:43:20  geraci
 * cars can rev at the start line
 *
 * Revision 3.1  1996/09/09  02:24:52  gray
 * JAMMA version
 *
 * Revision 2.10  1996/09/01  21:58:17  geraci
 * everyone slows down at the same rate
 *
 * Revision 2.9  1996/09/01  09:16:10  gray
 * Model types cleanup. First pass - removed fptyp.
 *
 * Revision 2.8  1996/08/20  08:57:45  geraci
 * cars stops creeping down hills after play time has expired
 *
 * Revision 2.7  1996/08/17  07:08:32  geraci
 * stops faster after someone crosses finish line
 *
 * Revision 2.6  1996/08/07  20:15:57  geraci
 * time bound check is now done in fast comm
 *
 * Revision 2.5  1996/08/02  18:53:46  geraci
 * time boost is now in fastcomm
 *
 * Revision 2.4  1996/07/31  15:58:34  geraci
 * new time boost for faster cars
 *
 * Revision 2.3  1996/07/31  07:56:45  gray
 * Floating point model time.
 *
 * Revision 2.2  1996/07/13  09:21:00  geraci
 * initialized gear differently
 *
 * Revision 2.1  1996/05/04  02:56:18  petrick
 * Focus 2 release
 *
 * Revision 1.20  1996/04/23  22:52:53  gray
 * Removed the feeble anti-spin attempt for focus I.
 *
 * Revision 1.19  1996/04/11  01:56:52  geraci
 * coasting to a stop adds braking and kills throttle
 *
 * Revision 1.18  1996/04/01  08:53:19  gray
 * First hack at feeb assist.
 *
 * Revision 1.17  1996/03/26  19:09:40  geraci
 * added header
 *
 *
 *	======================================================================
 */

#include "drivsym.h"
#include "globals.h"
#include "modelcom.h"
#include "Pro/controls.pro"
#include "Pro/unitvecs.pro"
#include "Pro/initiali.pro"
#include "road.h"
#include "Pro/road.pro"
#include "Pro/vecmath.pro"

void controls(MODELDAT *m)
{
	F32	rolloff,colvel[3],elasticity;
	S32		intswangle,i;
	short	flag;
	struct tiredes *td;
	CAR_DATA *gc = &game_car[m->net_node];

	m->dt = m->fastin.modeltime;
	m->idt = 1.0/m->dt;
	intswangle = m->fastin.wheel;
/*	intswangle = m->fastin.wheel + 
		((m->fastin.swvel * m->fastin.tp128model)/(2*4));*/
			
	m->steerangle=m->steergain*CTLSCALE*intswangle;

	m->clutch=m->mainin.clutch*CTLSCALE;

#if !KILLMAININ
	if (game_car[m->net_node].place_locked == 1)
		m->throttle = 0;
	else
		m->throttle=m->mainin.throttle*CTLSCALE;
#endif

	if (gc->place_locked == 1)
		{
		m->brake = 1 - (1 - m->brake) * pow (.99, m->dt*200);
		m->throttle = 0;
		m->clutch = 1;
		}
	else if (coast_flag || end_game_flag)
		{
		m->brake = 1 - (1 - m->brake) * pow (.999, m->dt*200);
		m->throttle = 0;
		m->clutch = 1;
		}
	else
		m->brake = m->mainin.brake*CTLSCALE;

	if (m->brake > .99)
		m->brake = 1;

	m->autotrans=m->mainin.autotrans;

	if (!m->autotrans)
		m->commandgear = m->gear=m->mainin.gear;
	else
		m->commandgear = m->mainin.gear;
				
	if (gstate == COUNTDOWN)
		{
		m->commandgear = 0;
		m->gear = 0;
		m->clutch = 1;
		m->brake = 1;
		}

	m->ignition = m->mainin.ignition == 1;
	m->gameover = m->mainin.ignition == 2;
	m->startermotor = m->mainin.startermotor;
	m->crashthreshold = m->mainin.crashthreshold*100;

	elasticity = 0; /* Just so this gets initialized */

	if(m->gear < -1)
		m->gear = 0;
	if(m->gear > 4)
		m->gear = 0;

	/* Calculate brake torque */
	for(i=0,td = &m->tires[0];i<4;++i,++td){
		if(td->angvel > 0){
			if(td->angvel < 10){
				if(td->angvel < 2) rolloff = 0.2;
				else rolloff = td->angvel *.1;
			}
			else
				rolloff = 1;
		}
		else{
			if(td->angvel > -10){
				if(td->angvel > -2) rolloff = -0.2;
				else rolloff = td->angvel *.1;
			}
			else
				rolloff = -1;
		}
		
		m->torque[i] = -m->brakegain[i] * m->brake * rolloff;
	}
	
}

/* can remove when all controls set before sym. */
void initcontrols(MODELDAT *m)
{

	m->fastin.wheel = 0;
	m->mainin.clutch=0;
	m->mainin.brake=0;
#if !KILLMAININ
	m->mainin.throttle=1.0/CTLSCALE;
#endif
	m->mainin.gear=1;
}

