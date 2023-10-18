#ifndef _MB_OBJECTS_H
#define _MB_OBJECTS_H

#include "mb_types.h"
#include "mb_struct.h"
#include "mb_model.h"

/*********************************************************************
 * These are the data structures used to communicate with the mathbox
 * library code.  The actual declarations are outside the library.
 * (ie; mb_wrap.c for mb-slave, z15lib.c for mb-host)
 *********************************************************************/

#define ZATR_OFF		0x0100	/* Don't show this object */
#define ZATR_ALL_OFF	0x0200	/* Don't show this object or any below it */
#define ZATR_NOZCOMP	0x0400  /* Don't do a Z compare on this object */
#define ZATR_NOZWRITE	0x0800	/* Don't write Z for this object */
#define ZATR_COLOROBJ	0x1000	/* Use color field to color this object */
#define ZATR_ALPHAOBJ	0x2000	/* Use xlucency field to alpha blend this object */
#define ZATR_DISTALPHA	0x4000	/* Becomes more xlucent as it gets closer to camera */
#define ZATR_SORTALPHA	0x8000	/* If this object has alpha, draw it last sorted against Z with others of its type */
#define ZATR_FRONTFACE	0x10000	/* Object yaws to face camera */
#define ZATR_ADDFIRST	0x20000	/* Add at beginning of object list, instead of end */
#define ZATR_REFLECT	0x40000	/*  */
#define ZATR_CAR_WINDOW	0x80000	/*  */
#define ZATR_CAMERAFACE	0x100000	/* Object always faces camera */
#define ZATR_CAR_BODY	0x200000	/*  */

#if (SST_GAME == SST_HOCKEY)
#define MBOBJINST_MAX 512
#else
#define MBOBJINST_MAX 2048
#endif
typedef struct
{
	S32	color;		/* Use with flag to color all polys in object */
	U32	flags;		/* Only use bits 8-15 so that OBJINSTFLAGS can be or'd in */
	F32	mat[4][3];
	S32	zmod;		/* Modifier to Z-buffer value for this object */
					/* BE CAREFUL! CHECKING FOR (Z + ZMODFOR)<0 IS TOO EXPENSIVE */
	S32	index; 		/* Index into ROM object data */	
	S16	parent;		/* Parent index into ROM obj data (-1 = none) */
	S16	child;		/* Child index into ROM obj data (-1 = none) */
	S16	dopolytype;	/* Draw polys of type dopolytype when this object is processed */
	U08	xlucency;	/* Use with flag to alpha blend all polys in object */
	U08	ambientsub;	/* % reduction in ambient for this obj, 0=none 255=100% (no ambient) */
	F32	reflect_speed;
	ROMTEX	*reflect_rtex;
	F32	sort_offset;
	S16 	next[MBNUMWINDOWS];
	F32	ambient_add;
} OBJINSTANCE;

/* OBJINSTFLAGS flag bits (Set by mathbox) */
/* NOTE: MUST be separated form OBJINSTANCE structure so that host does not overwrite */
#define ZATR_LOD		0x0003	/* LOD saved in these two bits   */
#define ZATR_VIS		0x0004	/* Set if object is visible	   */
typedef struct
{
    U32 flags;		/* only 8 lsb's are used, but must be 32 bit aligned for host */
} OBJINSTFLAGS;

extern ROMOBJECTLIST *RomObjList;

extern RomObjectList   *model_addrs[];
extern OBJINSTANCE		ObjInstanceList[];
extern S16	 			ObjHead[MBNUMWINDOWS];


#include "Pro/mb_objects.pro"

#endif
