#ifndef _WORLD_H
#define _WORLD_H

#include "stdtypes.h"

/************* worlds.h *****************/
extern char WorldData[];
#define WORLD_DATA	WorldData

typedef struct {
	U32 woff;        /** offset from 0 **/
	char wname[16];  /** name of the world **/
}WORLDHDR;

#define WORLD_HDRS	(WORLD_DATA + sizeof(S32))
#define NWORLDS	(*((S32 *)WORLD_DATA))
/************* worlds.h *****************/

#define WORLD_BSIZE (0x20000)
#define BSEARCH(a,b,c,d,e) (bsearch((a),(b),(c),(d),(e)))


typedef struct {			/* WORLD OBJECT */
	char name[16];				/* name of the object */
	F32 matrix[3][3];			/* convert to short matrix format */
	F32 objpos[3];				/* position in parents frame of ref */
	S32 dlflags;				/* Mathbox display list flags for this object */
	S16 next;					/* id of next link, - index in list of objects */
	S16 child;					/* id of child, - index in list of objects */
		
	S16 sfx1;					/* special effects flag 1 */
	S16 sfx2;					/* special effects flag 2 */
}WOBJ;

typedef int (*WD_FUNCT)(S16 init);


#include "Pro/world.pro"
#endif

