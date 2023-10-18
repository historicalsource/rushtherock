#ifndef _MB_BLIT_H
#define _MB_BLIT_H

#define MBBLIT_MAX 300
typedef struct {
    S32	Idxs[4];
    U8	alphas[4];
	S8	IdxCount;
	S8	Hide;
	U16	Flags;
    S16	X,Y;
    S16 Width,Height;
    F32 Z;
    F32 top,bot,left,right; /* Crop values */
	S32 color[4];			/* Color for verts if BLIT_COLORVERTS, else use color[0] for poly */
} BlitInstance, BLITINSTANCE;

#define BLIT_COLORVERTS 1 		/* If true, top,bot,left,right are COLOR values, NOT CROP values */
#define BLIT_HFLIP		2
#define BLIT_NOSCALE	4		/* If true, don't scale Y for low res */

#include "Pro/mb_blit.pro"

#endif

