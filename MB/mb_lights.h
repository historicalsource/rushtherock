#ifndef _MB_LIGHTS_H
#define _MB_LIGHTS_H

#include "mb_types.h"


#define MBLIGHT_MAX 4
#define MBLIGHT_WHITE 1
#define MBLIGHT_INFINITE 2
#define MBLIGHT_POINT 4
#define MBLIGHT_ZDIST 8		/* Use zdist in light radius calc (otherwise ignore z) */
typedef struct
{
    U32 flags;
    F32	x,y,z;
    F32 r,g,b;		/* r = intensity for white lights */
    F32 radius;
    S16 next[MBNUMWINDOWS];
} MBLIGHT;

extern MBLIGHT		RamLightList[];

#include "Pro/mb_lights.pro"

#endif
