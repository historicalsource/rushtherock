#ifndef _MB_CAMERA_H
#define _MB_CAMERA_H


#define REV_CAMLINK 16		/* Add at head of list, instead of tail */
#define CAMLINK0 1
#define CAMLINK1 2
#define CAMLINK2 4
#define CAMLINK3 8

typedef struct {
    S32 update;
    F32 mat[4][3];
} MBCAMINFO;

extern MBCAMINFO	ZoidCamInfo[];

#include "Pro/mb_camera.pro"

#endif
