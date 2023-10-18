#ifndef _BALLOON_H
#define _BALLOON_H

typedef struct
{
	F32		BasePos[3];	/* Center point of line 				*/
	F32		UnitVec[3];	/* Unit vec								*/
	F32		CurrPos[3];
	F32		CurrVec[3];
	F32		Length;		/* (Length of line)/2 + Radius			*/
	F32		Radius;		/* Radius of the skin of the balloon	*/
} Balloon;

typedef struct
{
	char	Name[16];
	F32		Radius;
#if HOCKEY
	S32 	Surface;
#endif
	F32		ShadowRadius;
	F32		Extend[2];
	F32 	LineSeg[2][3];
	F32 	ShadowSeg[2][3];
} BalloonInfo;

#include "Pro/balloon.pro"

extern F32 BCDVec[3];

#endif

