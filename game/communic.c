/*
 *	communic.c	-- Functions for handling model communication.
 *
 *	Copyright 1996 Time Warner Interactive.
 *	Unauthorized reproduction, adaptation, distribution, performance or 
 *	display of this computer program or the associated audiovisual work
 *	is strictly prohibited.
 *
 *	======================================================================
 *	$Author: gray $						$Date: 1997/05/30 07:53:15 $
 *	$Revision: 3.11 $						$Locker:  $
 *	======================================================================
 *	Change Log:
 *
 *	$Log: communic.c,v $
 * Revision 3.11  1997/05/30  07:53:15  gray
 * fastout is history.
 *
 * Revision 3.10  1997/05/30  04:05:35  gray
 * mainout is history!
 *
 * Revision 3.9  1997/05/30  01:25:14  gray
 * Clean up crashflag stuff.
 *
 * Revision 3.8  1997/05/29  21:08:03  gray
 * Clean up mph stuff.
 *
 * Revision 3.7  1997/05/21  22:11:28  gray
 * road codes removed from mainout.
 *
 * Revision 3.6  1997/05/21  18:13:16  gray
 * Removed rollv, pitchv, and yawv from mainout.
 *
 * Revision 3.5  1997/05/21  06:04:56  gray
 * Killed xv,yv,zv in mainout (again).
 *
 * Revision 3.4  1997/05/21  00:10:16  gray
 * caruvs finally exterminated.
 *
 * Revision 3.3  1997/05/11  03:37:10  gray
 * Cleaning model interface.
 *
 * Revision 3.2  1996/09/25  03:23:49  gray
 * Dont need trunc stuff any more.
 *
 * Revision 3.1  1996/09/09  02:24:52  gray
 * JAMMA version
 *
 * Revision 2.8  1996/09/05  00:09:33  gray
 * Backed out xyzv mod, evil audio bug.
 *
 * Revision 2.7  1996/09/04  23:32:27  gray
 * Removed xv, yv, zv from mainout.
 *
 * Revision 2.6  1996/09/04  20:34:54  gray
 * Adding dead reckon data to game_car. Killing mainout.
 *
 * Revision 2.5  1996/08/30  17:04:39  geraci
 * cleaned unused stuff
 *
 * Revision 2.4  1996/08/20  08:59:12  geraci
 * better rpm display value
 *
 * Revision 2.3  1996/07/09  20:11:18  geraci
 * fake out rpm loss due to catch-up
 *
 * Revision 2.2  1996/05/08  21:35:34  geraci
 * removed old screech code
 *
 * Revision 2.1  1996/05/04  02:56:18  petrick
 * Focus 2 release
 *
 * Revision 1.17  1996/04/12  08:26:01  petrick
 * Added header, changed mph back to a real number.
 *
 *
 *	======================================================================
 */

#include "drivsym.h"
#include "modelcom.h"
#include "road.h"

#include "Pro/communic.pro"
#include "Pro/unitvecs.pro"
#include "Pro/d3math.pro"


/* ----------------------------------------------------------------------------- */
void mcommunication(MODELDAT *m)
{


	/* spedometer reading (in mph). */
	m->mph = (m->tires[2].angvel*m->tires[2].tradius +
		  m->tires[3].angvel*m->tires[3].tradius) * .5 *
			(3600.0/5280.0); 

	/* calculate magnitude of velocity */
	m->rpm = (S16)(m->engangvel*rdpstorpm*.9); 

}	/* end mcommunication */


/* ----------------------------------------------------------------------------- *
 *	This subroutine converts a set of unit vectors from simulation coordinates
 *	to display coordinates.
 * ----------------------------------------------------------------------------- */
void mmodtodispuv(short moduv[3][3], short camuv[3][3])
{
	camuv[0][0] =  moduv[1][1];
	camuv[0][1] = -moduv[1][2];
	camuv[0][2] =  moduv[1][0];
	camuv[1][0] = -moduv[2][1];
	camuv[1][1] =  moduv[2][2];
	camuv[1][2] = -moduv[2][0];
	camuv[2][0] =  moduv[0][1];
	camuv[2][1] = -moduv[0][2];
	camuv[2][2] =  moduv[0][0];
}	/* end mmodtodispuv */

void disptomoduv(short d[3][3],uvect *m)
{
	m->uvs[0][0] =  d[2][2];
	m->uvs[0][1] =  d[2][0];
	m->uvs[0][2] = -d[2][1];
	m->uvs[1][0] =  d[0][2];
	m->uvs[1][1] =  d[0][0];
	m->uvs[1][2] = -d[0][1];
	m->uvs[2][0] = -d[1][2];
	m->uvs[2][1] = -d[1][0];
	m->uvs[2][2] =  d[1][1];
	makefpuvs(m);
}
