/*
 * stree.h	Defines, Types,and Structures for stree.c
 *
 * Copyright (c) 1994-1995 Time Warner Interactive, Alan Gray
 *
 * 11/14/94 asg	Created
 *
 */

#ifndef _STREE_H
#define _STREE_H

#include "stdtypes.h"

#define SURF_MASK		0x000F
#define SURF_PAVE		0x0000
#define SURF_DIRT		0x0001
#define SURF_COLLIDE	0x0005

#define SVIS_MASK		0x00F0
#define SVIS_SHIFT		4
#define SVIS_PAVE		0x0000
#define SVIS_DIRT		0x0010
#define SVIS_WATER		0x0020
#define SVIS_COLLIDE	0x0050

#define SURF_BOOST_MASK	0x0F00
#define SURF_BOOST_SHIFT 8

#define INTERP_BIT		0x1000

#define S_CUT_MASK  	0x8000
#define NOWRONGWAY_MASK	0x4000
#define YANK_MASK		0x0300
#define YANK_SHIFT		8
#define SC_DIST_MASK	0x003F

#define UP2_REVERB_MASK 0x8000
#define UP_REVERB_MASK	0x4000
#define LF_REVERB_MASK	0x2000
#define RT_REVERB_MASK	0x1000
#define ALL_REVERB_MASK	(UP2_REVERB_MASK | UP_REVERB_MASK | LF_REVERB_MASK | RT_REVERB_MASK)
#define UP_DIST_MASK	0x0C00
#define UP_DIST_SHIFT	10
#define LF_DIST_MASK	0x0300
#define LF_DIST_SHIFT	8
#define RT_DIST_MASK	0x00C0
#define RT_DIST_SHIFT	6
#define ALL_DIST_MASK	(UP_DIST_MASK | LF_DIST_MASK | RT_DIST_MASK)

/*#define WH_Z_OFFSET 8*/
/*#define WH_Z_OFFSET 60*/
#define WH_Z_OFFSET 0
#if 0
#define MAXSURFHOP -80
#define MAXSURFPOS 0x7FFFFF
#else
#define MAXSURFHOP 120
#define MAXSURFPOS -10000
#endif


#define HPOS 1
#define VPOS 0

typedef struct stheadertyp {
	S32	treebytes;
	S32	tnum;
	S32	snum;
	S32	vnum;
	S32	pnum;
	S32	slnum;
	S32	vlnum;
	S32	startpos[3];
	S16	startuvs[3][3];
	S16	pathstart;
	S16	pathend;
	S16	pathloop;
} STHEADERTYP;

typedef struct streetyp {
	S32	parent;
	S32	minh,maxh,minv,maxv;
	S32	child[4];
} STREETYP;

typedef struct surftyp {
	U16	flags;		/*road surface flags */
	U16	snd_flags;	/* road sound flags */
	U16	cut_flags;	/* road shortcut flags */
	S16	uvs[3][3];
	S32	vli;
} SURFTYP;

typedef struct sverttyp {
	S32	pos[3];
} SVERTTYP;

typedef struct pathtyp {
#if 1
	F32	fpos[3];
	F32	fuvs[3][3];
	/* The following data is for surface smoothing. */
	F32 ve[3];	/* vector for edge0		*/
	F32 vl[3];	/* tangent vector (left)	*/
	F32 vr[3];	/* tangent vector (right)	*/
	F32 fwidth;	/* road width (left to right)	*/
	F32 w;		/* x width (at y=0)		*/
	F32 m0;		/* x/y slope (at pos)		*/
	F32 m1;		/* x/y slope (at next pos)	*/
	F32 wl;		/* y left width (at x=0)	*/
	F32 ml;		/* y/x left slope		*/
	F32 wr;		/* y right width (at x=0)	*/
	F32 mr;		/* y/x right slope		*/
	F32 m0l;	/* left surface loft (edge0)	*/
	F32 m0r;	/* right surface loft (edge0)	*/
	F32 m1l;	/* left surface loft (edge1)    */
	F32 m1r;	/* right surface loft (edge1)   */
#endif
	S32	pos[3];
	S16	uvs[3][3];
	S16	width;
} PATHTYP;


#endif
/* end #ifndef _STREE_H */

