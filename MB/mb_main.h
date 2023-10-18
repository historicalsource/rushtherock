#ifndef _MB_MAIN_H
#define _MB_MAIN_H


typedef struct {
    U32 benchmark;		/* 0x00xx runs bench test xx, 0x01xx runs the game loop */
    U32 benchparam;		/* Parameter used in benchtests */
    U32 validword;		/* Must be 0xc0edbabe */
    S32 objhead[MBNUMWINDOWS];	/* ObjInstanceList index of first object in window (-1 = none) */
    S32 lighthead[MBNUMWINDOWS];/* LIGHT index of first light in window (-1 = none) */
    U32 CamRPY[3];		/* no longer unused */
    F32 CamPOS[3];		/* no longer unused */
} MBSTRUCT;

typedef struct {
    U32 frame;
    U32 objs;
    U32 polys;
    U32 visobjs;
    U32 vispolys;
    U32 objhead[MBNUMWINDOWS];
    U32 nextobjs;
    U32 childobjs;
    F32 fps;
    F32 texttime;
} MBSTATS;

extern MBSTATS MboxStats;
extern MBSTRUCT	MboxInfo;

#include "Pro/mb_main.pro"

#endif
