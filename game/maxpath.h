/*
 *	maxpath.h	-- Include file for maxpath.c
 *
 *	Copyright 1996 Atari Corporation
 *	Unauthorized reproduction, adaptation, distribution, performance or 
 *	display of this computer program or the associated audiovisual work
 *	is strictly prohibited.
 *
 *	======================================================================
 *	$Author: geraci $						$Date: 1997/01/15 20:02:31 $
 *	$Revision: 3.8 $						$Locker:  $
 *	======================================================================
 *	Change Log:
 *
 *	$Log: maxpath.h,v $
 * Revision 3.8  1997/01/15  20:02:31  geraci
 * defined number of max paths
 *
 * Revision 3.7  1996/11/11  22:13:02  geraci
 * linked priority and solo priority replaced by priority
 *
 * Revision 3.6  1996/11/07  14:37:51  geraci
 * weights are defined
 *
 * Revision 3.5  1996/10/31  07:08:08  geraci
 * multi max paths supported
 *
 * Revision 3.4  1996/10/25  11:07:37  geraci
 * start of multi maxpath support
 *
 * Revision 3.3  1996/09/25  00:45:34  gray
 * Removed old RamRom recording stuff.
 *
 * Revision 3.2  1996/09/24  23:35:27  gray
 * Removed MAXPATHDBG stuff (part of ZGLU purge).
 *
 * Revision 3.1  1996/09/09  02:23:24  gray
 * JAMMA version
 *
 * Revision 2.12  1996/09/05  23:47:53  geraci
 * changed filler to spare
 *
 * Revision 2.11  1996/09/05  08:12:28  geraci
 * new maxpath data structure (widths and hints)
 *
 * Revision 2.10  1996/08/18  19:35:00  gray
 * Make PHOENIX_PORT only, removed flag.
 *
 * Revision 2.9  1996/08/18  03:30:29  gray
 * Removing ZOID stuff.
 *
 * Revision 2.8  1996/07/20  04:40:42  geraci
 * move mpctl structure to modeldat since it is used by the model
 *
 * Revision 2.7  1996/07/19  23:37:51  geraci
 * fixed drone abort due to fast model time
 *
 * Revision 2.6  1996/07/16  21:49:53  gray
 * Load drone path data into RAM at track load time.
 *
 * Revision 2.5  1996/06/28  05:14:25  geraci
 * new counter to determine when to abort
 *
 * Revision 2.4  1996/06/17  20:28:18  geraci
 * added header
 *
 *
 *	======================================================================
 */

#ifndef _MAXPATH_H
#define _MAXPATH_H

#include "stdtypes.h"

typedef struct MPHEADER
{
	S32 num_points;
	S32 mpath_active;
	S8  priority;
	S8  dummy;
	S16 lap_start;
	S16 lap_end;
	S16 num_in_lap;
	S32 spare[2];
} MPHEADER;

typedef struct MPATH
{
    F32	pos[3];
    F32	speed;
	S32 hints;
	S16 base_to_path;
	S16 path_to_base;
} MPATH;

typedef struct MPWEIGHT
{
	F32 distance_weight;
	F32 direction_weight;
	F32 speed_weight;
	F32 own_path_weight;
	F32 priority_weight;
	F32 hint_weight;
} MPWEIGHT;

#define MAXMPATH    2000
#define MAX_MPATHS  8

#include "Pro/maxpath.pro"

#endif
