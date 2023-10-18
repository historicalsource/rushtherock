/*
 *  resurrect.h -- include this file for any file that calls resurrect.c
 *
 *	======================================================================
 *	$Author: geraci $						$Date: 1996/10/22 09:26:18 $
 *	$Revision: 3.2 $						$Locker:  $
 *	======================================================================
 *	Change Log:
 *
 *	$Log: resurrect.h,v $
 * Revision 3.2  1996/10/22  09:26:18  geraci
 * bumped crash delay by 1/2 second but first crash will be 3 secs still
 *
 * Revision 3.1  1996/09/09  02:23:24  gray
 * JAMMA version
 *
 * Revision 2.2  1996/08/09  04:50:44  geraci
 * 3 second death instead of 4 second
 *
 * Revision 2.1  1996/05/04  02:56:18  petrick
 * Focus 2 release
 *
 * Revision 1.1  1996/04/24  02:56:33  geraci
 * Initial revision
 *
 *
 *
 *	======================================================================
 */

#ifndef __RESURRECT_H
#define __RESURRECT_H

#define POLEHDIST		17.0
#define POLEVDIST		20.0
#define CRASHDEL		3500 /* millisecs */
#define RESURRECT_TIME	1500 /* millisecs */

#include "Pro/resurrect.pro"

#endif
