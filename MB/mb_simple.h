#ifndef _MB_SIMPLE_H
#define _MB_SIMPLE_H

#include "mb_types.h"
#include "mb_struct.h"
#define GLIDE_LIB
#define __FX64_H__
#include "glide.h"
#if GLIDE2
#include <fxglide.h>
#include <gsstdef.h>
#endif

#define vertex_snapper ((F32) (1L << 20))
#define SNAP_VERT(x) \
	x += vertex_snapper; \
	x -= vertex_snapper; 

#if 1
typedef struct simplexvert {
  float sx, sy, sz;            /* X, Y, and Z of scrn space -- Z is ignored */
  float ooz;                /* 65535/Z (used for Z-buffering) */
  float oow;                /* 1/W (used for W-buffering) */
  float r, g, b, a;         /* R, G, B, and A ([0..255.0]) */
  float pad;
  float tmu_oow;            /* 1/w (used mipmapping - really 0xfff/w) */
  float sow;                /* s texture ordinate (s over w) */
  float tow;                /* t texture ordinate (t over w) */  
  float x;            /* 1/w (used mipmapping - really 0xfff/w) */
  float y;                /* s texture ordinate (s over w) */
  float z;                /* t texture ordinate (t over w) */  
} SIMPLEXVERT;
#else
typedef struct
{
  float sx, sy, sz;                /* X, Y, and Z of scrn space -- Z is ignored */
  float r, g, b;                /* R, G, B, ([0..255.0]) */
  float ooz;                    /* 65535/Z (used for Z-buffering) */
  float a;                      /* Alpha [0..255.0] */
  float oow;                    /* 1/W (used for W-buffering, texturing) */
  float sow;                /* s texture ordinate (s over w) */
  float tow;                /* t texture ordinate (t over w) */  
  float tmu_oow;            /* 1/w (used mipmapping - really 0xfff/w) */
  float x;            /* 1/w (used mipmapping - really 0xfff/w) */
  float y;                /* s texture ordinate (s over w) */
  float z;                /* t texture ordinate (t over w) */  
  float pad;
} SIMPLEXVERT;
#endif
typedef struct simplexnorm {
	F32 x,y,z;
} SIMPLEXNORM;

#define COLOREDLIGHTS 	0
#define MANYLIGHTS		0
#define DYNMIPMAP		0
#define NOLIGHTING		1
#define CHECK_FIFO		0

#define NEW_TRI_CLIP 1

#define UNPACK_R(c) (((c) >> 10) << 2)
#define UNPACK_G(c) ((((c) >> 5) & 0x1F) << 2)
#define UNPACK_B(c) (((c) & 0x1F) << 2)
#define UNPACK_RGB(c,r,g,b) (r = UNPACK_R(c), g = UNPACK_G(c), b = UNPACK_B(c))

#define UNPACK_R5(c) ((c) >> 10)
#define UNPACK_G5(c) (((c) >> 5) & 31)
#define UNPACK_B5(c) ((c) & 31)
#define UNPACK_RGB5(c,r,g,b) (r = UNPACK_R5(c), g = UNPACK_G5(c), b = UNPACK_B5(c))

extern SIMPLEXVERT SimpleXVerts[];

#define CHECK_FIFO_LOOP(n) if (((n) & 128) == 128) CheckFIFO()

unsigned int prc_get_count(void);

#include "Pro/mb_simple.pro"

#endif
