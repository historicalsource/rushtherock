/*
 *  carsnd.h -  includes for carsnd.c moobs.c sounds.c
 *
 *	Copyright 1996 Time Warner Interactive
 *	Unauthorized reproduction, adaptation, distribution, performance or 
 *	display of this computer program or the associated audiovisual work
 *	is strictly prohibited.
 *
 *	======================================================================
 *	$Author: geraci $						$Date: 1996/09/17 08:42:10 $
 *	$Revision: 3.4 $						$Locker:  $
 *	======================================================================
 *	Change Log:
 *
 *	$Log: carsnd.h,v $
 * Revision 3.4  1996/09/17  08:42:10  geraci
 * cosmetic change
 *
 * Revision 3.3  1996/09/16  02:14:07  geraci
 * skids use a constant array
 *
 * Revision 3.2  1996/09/13  22:59:53  geraci
 * bumps tied to collision forces
 *
 * Revision 3.1  1996/09/09  02:23:24  gray
 * JAMMA version
 *
 * Revision 2.2  1996/07/19  09:55:45  geraci
 * new sound effects: collidable objects, car scrape
 *
 * Revision 2.1  1996/05/04  02:56:18  petrick
 * Focus 2 release
 *
 * Revision 1.12  1996/05/03  22:28:17  geraci
 * better tire squeals
 *
 * Revision 1.11  1996/04/27  02:16:37  geraci
 * added prototypes to .h file so only include .h
 *
 * Revision 1.10  1996/04/26  02:39:37  geraci
 * changed from hex to dec some equates
 *
 * Revision 1.9  1996/04/12  00:21:01  geraci
 * another new engine
 *
 * Revision 1.8  1996/04/11  01:56:36  geraci
 * new number of cars engines
 *
 * Revision 1.7  1996/02/28  21:08:54  geraci
 * better skids/screeches/road and wind noises
 *
 * Revision 1.6  1996/02/22  03:38:26  geraci
 * better tire squeals
 *
 * Revision 1.5  1996/02/06  03:37:23  geraci
 * put num engines in carsnd.h not sound.h
 *
 *
 *
 *	======================================================================
 */

#include "stdtypes.h"
#include "targets.h"

#define WIND_PITCH				0x400
#define WIND_MAX_VOL			0x0150
#define ROAD_MAX_PITCH			0x03ff
#define ROAD_MIN_PITCH			0x0100
#define ROAD_MAX_VOL			0x0380
#define GRAVEL_MAX_VOL			0x00A0

#define MAX_ENG_RPM_SCL			0x1E00

#define VOL_FADE_VAL			0x0020

#define FSCRCH_LOW_SPD			3584
#define FSCRCH_HI_SPD			2560
#define FSCRCH_SPEED_THRESHOLD	20
#define MIN_FSCRCH_VAL			0x003C
#define FSCRCH_MAX_PITCH		0x0800

#define RSCRCH_LOW_SPD			0x0C80
#define RSCRCH_PITCH			0x500
#define RSCRCH_SPEED_THRESHOLD	60
#define MIN_RSCRCH_VAL			0x0040
#define RSCRCH_MAX_PITCH		0x0800

typedef struct snd_state
{
	S16  volume;
	S16  pitch;
	U16  state_var;
	BOOL on;
} snd_state;

typedef struct bump_state
{
	F32  peak;
	F32  peak_vec[3];
	S32  time;
	BOOL bump_it;
} bump_state;

typedef struct skid_data
{
	S32  force_tab[5];
	S32  pitch_h;
	S32  pitch_l;
	S32  vol_h;
	S32  vol_l;
	S16  gr_skid_thresh;
	S16  gr_smoke_thresh;
} skid_data;

#include "Pro/carsnd.pro"
