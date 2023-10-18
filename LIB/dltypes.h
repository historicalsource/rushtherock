/*
 *  dltypes.h
 */

#ifndef _DLTYPES_H
#define _DLTYPES_H

#if 0

#include "stdtypes.h"

#define MAXCAMERAS 4

/*
**		Flag bits
*/
/* Set by mathbox */
#define ZATR_LOD		0x00000003	/* LOD saved in these two bits   */
#define ZATR_VIS		0x00000004	/* Set if object is visible	   */
/* Set by host */
#define ZATR_OFF		0x00000100	/* Don't show this object */
#define ZATR_ALL_OFF	0x00000200	/* Don't show this object or any below it */

typedef struct
{
	U32 flags;
	F32	mat[3][4]; 
	S32	index; 		/* Index into ROM object data */	
	S16 child;		/* Child index into ROM obj data (-1 = none) */
	S16 next[MAXCAMERAS];
} OBJINSTANCE;

#define MAXLIGHTS 16

#define LIGHT_WHITE 1
#define LIGHT_INFINITE 2
#define LIGHT_POINT 4
typedef struct
{
	U32 flags;
	F32	x,y,z;
	F32 r,g,b;		/* rgb[0] = intensity for white lights */
    F32 radius;
} LIGHT;

typedef struct
{
    S32 winindex;
	F32	mat[3][4]; 
} CAMERA;

#endif
#endif

