/*
 *	timint.c -- game functions done at timer interrupt
 *
 *		Copyright 1996 Atari Games.
 *	Unauthorized reproduction, adaptation, distribution, performance or 
 *	display of this computer program or the associated audiovisual work
 *	is strictly prohibited.
 *
 *	======================================================================
 *	$Author: gray $						$Date: 1997/10/02 00:54:17 $
 *	$Revision: 3.9 $						$Locker:  $
 *	======================================================================
 *	Change Log:
 *
 *	$Log: timint.c,v $
 * Revision 3.9  1997/10/02  00:54:17  gray
 * Added seconds-since-poweron variable (gUpSeconds).
 *
 * Revision 3.8  1997/10/01  22:06:35  gray
 * IRQTIME reset routine.
 *
 * Revision 3.7  1996/11/27  12:56:10  gray
 * Changed NetTime from F32 to 10microsec S32.
 *
 * Revision 3.6  1996/11/27  00:49:31  gray
 * Fixed GetNetTime bug - never reenabled interrupts.
 *
 * Revision 3.5  1996/11/18  10:13:27  gray
 * Made model time interrrupt safe with GetNetTime.
 *
 * Revision 3.4  1996/11/09  12:26:37  geraci
 * moved pots read to model task
 *
 * Revision 3.3  1996/10/25  09:54:55  geraci
 * zapped all time stamp stuff
 *
 * Revision 3.2  1996/10/10  05:12:22  petrick
 * Made irqtime a true global in globals_h.
 *
 * Revision 3.1  1996/09/09  02:24:52  gray
 * JAMMA version
 *
 * Revision 2.17  1996/09/02  09:40:20  gray
 * Make net time stuff global.
 *
 * Revision 2.16  1996/09/02  01:24:50  gray
 * Start of NetTime stuff.
 *
 * Revision 2.15  1996/08/30  16:25:03  geraci
 * new way to do irq time
 *
 * Revision 2.14  1996/08/27  05:32:44  geraci
 * more accurate version of irq time
 *
 * Revision 2.13  1996/08/18  21:25:20  gray
 * Removing zoid stuff. No more zoid.h.
 *
 * Revision 2.12  1996/08/18  20:53:09  gray
 * Made Nucleus default, removed flag.
 *
 * Revision 2.11  1996/08/18  19:36:33  gray
 * Make PHOENIX_PORT only, removed flag.
 *
 * Revision 2.10  1996/07/20  00:19:44  gray
 * Slimmed timer interrrupt routine. wheelpos() and forcewheel() moved.
 *
 * Revision 2.9  1996/06/21  19:14:36  geraci
 * uses alan's cpu speed method for time stamping
 *
 * Revision 2.8  1996/06/19  02:53:50  geraci
 * temp fix to time stamp for phoenix port
 *
 * Revision 2.7  1996/06/03  20:58:28  gray
 * Phoenix port.
 *
 * Revision 2.6  1996/05/21  19:25:55  petrick
 * Got rid of turning off interrupts.
 *
 * Revision 2.5  1996/05/17  20:12:41  geraci
 * name tags on time stamps
 *
 * Revision 2.4  1996/05/15  00:52:37  geraci
 * added name stamps to time stamps
 *
 * Revision 2.3  1996/05/14  00:56:23  geraci
 * time stamps only need to define first stamp, all others are handled automatically
 * also stamps are in microsecond increments
 *
 * Revision 2.2  1996/05/11  01:28:54  geraci
 * improved time stamp code
 *
 * Revision 2.1  1996/05/04  02:56:18  petrick
 * Focus 2 release
 *
 * Revision 1.19  1996/03/26  23:20:06  geraci
 * time stamp stuff always initializes
 *
 * Revision 1.18  1996/03/15  22:06:36  petrick
 * Header fix.
 *
 *
 *	======================================================================
 */

#include "mb.h"
#include "debug.h"
#include "dis.h"
#include "globals.h"
#include "controls.h"
#include "menus.h"
#include "os_proto.h"

#include "Pro/fswheel.pro"
#include "Pro/pots.pro"
#include "Pro/timint.pro"

/*-------------------------------  DEFINES  ---------------------------------*/

/*-------------------------------  EXTERNS  ---------------------------------*/

extern U32 prc_get_count(void);

/*-------------------------------  GLOBALS  ---------------------------------*/

U32		last_irqtime, last_irq_delta;

static S32 NetTime;
static U32 NetTimeCount;
static S32 CountPer10Usec;
S32 upsec_inc;

/*---------------------------------------------------------------------------*/

/* Set to a small number (dont set to zero). */
#define IRQTIME_RESET_VAL (0x10000)

void Init_TimInt(void)
{
	S16 i;

	CountPer10Usec = CPU_SPEED/(2 * 100000);

	sync_net_time(0);
	IRQTIME = IRQTIME_RESET_VAL;
	gUpSeconds = 0;
	upsec_inc = IRQTIME + 1000;
	reset_IRQTIME();
}

void reset_IRQTIME(void)
{
	S32 state;

	/* need ints off while diddling with timer vars. */
	state = prc_set_ipl (INTS_OFF);

	upsec_inc = IRQTIME_RESET_VAL + (upsec_inc - IRQTIME);

	IRQTIME = IRQTIME_RESET_VAL;
	last_irqtime = prc_get_count();
	last_irq_delta = 0;

	prc_set_ipl (state);
}

/*---------------------------------------------------------------------------*/


void TimInt(void)
{
	U32  cur_count, wrap_count, irqtime_delta;

	update_net_time();

#if 1
	cur_count = prc_get_count();
	wrap_count = cur_count - last_irqtime;
	wrap_count += last_irq_delta;
	irqtime_delta = (F32)wrap_count * gMsecPerCount;
	IRQTIME += irqtime_delta;
	last_irq_delta = wrap_count - (F32)irqtime_delta * ginvMsecPerCount;
	last_irqtime = cur_count;
#else
	IRQTIME++;
#endif

	/* bump second counter if time. */
	if(IRQTIME >= upsec_inc)
		{
		gUpSeconds++;
		upsec_inc = IRQTIME + 1000;;
		}

	/*update motor amp with latest value.*/
	setswforce(gSWforce);
}

S32 GetNetTime(void)
{
	S32 state;
	S32 time;


	/* need ints off since both NetTime and NetTimeCount updated in timer int. */
	state = prc_set_ipl (INTS_OFF);
	time = NetTime + ((prc_get_count() - NetTimeCount)/CountPer10Usec);
	prc_set_ipl (state);

	return(time);
}

void sync_net_time(S32 time)
{
	NetTime = time;
	NetTimeCount = prc_get_count();
}

void update_net_time(void)
{
	U32 cur_count;

	cur_count = prc_get_count();
	NetTime += ((cur_count - NetTimeCount)/CountPer10Usec);
	NetTimeCount = cur_count;
}
