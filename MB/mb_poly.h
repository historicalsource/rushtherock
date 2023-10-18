#ifndef _MB_POLY_H
#define _MB_POLY_H

#include "mb_types.h"
#include "mb_struct.h"

#define MBPOLYINST_MAX 128
#define POLYINSTMAXVERTS 4

enum {
	POLY_TYPE_DEFAULT = 0,
	POLY_TYPE_SHADOW0,
	POLY_TYPE_SHADOW1,
	POLY_TYPE_MAX
};

typedef struct
{
	S08 update;
	S08 hide;
	S16 nverts;
	F32 normal[3];
	RomVert verts[POLYINSTMAXVERTS];
	S32 texidx;
	U16 type; /* if !=0, gets drawn only by call from special object */
	U16 alpha;
	U32 color;
	S16 zmod;
	S16 dzmod;
} POLYINST;

#include "Pro/mb_poly.pro"

#endif
