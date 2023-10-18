#ifndef _BLIT_H
#define _BLIT_H

#include "stdtypes.h"
#include "mb.h"


enum
{
	TEX_MIP			= 1,
	TEX_PALETTE		= 2,
	TEX_HALFRES		= 4,
	TEX_USEPALETTE	= 8
};

#ifndef GLISTMAX
#define GLISTMAX 16
#endif

#define BLITPLANE(x) (x)

typedef struct _Blit
{
	const	char	*Name;
	S16		X,Y;
	S16		Width,Height;
	S08		Hide,Init;
	U08		Alpha,Flip;
	F32		Z;
	S16		Top,Bot,Left,Right;
	TEXDEF	*Info;
	int		(* AnimFunc)(struct _Blit *blit);
	struct _Blit *child;
	U16		BLIdx;
	S32		TexIndex;
	U32		AnimID;
	S32 	AnimDTA;
	S16		color; /* color for clipped blits */
	S16 	dummy;
	void 	*data;	/* a pointer to any piece of data you like */
} Blit,BLIT;

typedef struct
{
	int		Timer;
	int		X,Y;
	int		Width,Height;
	int		Stretch,Hold,Shrink,Blink;
	Blit	*Blit;
} GrowInfo,GROWINFO;

typedef struct {
	const char *texname;			/* texture name */
	S16 dulx,duly,width,height;		/* destination upper left coord and width/height */
	S16 top,bot,left,right;			/* source upper left and bottom right coordinates in texture */
	S32 noyscale;					/* Don't scale blit based on resolution */
	U32 zdepth;						/* z-buffer depth : used for sorting */
	U32 alpha;						/* translucency setting for this blit */
	int (* animfunc)(Blit *blit);	/* animation routine to call during DrawBlits() */
	S32 animid;						/* used to identify the blit when animfunc is called */
}MULTIBLIT;

#include "Pro/blit.pro"

extern TEXDEF * const TexTable;

#define		CLEARBLITS(b) \
{ \
int		_i; \
 \
	for(_i=0;_i<ARRAY_SIZE(b);_i++) \
	{ \
		if (b[_i]) \
		{ \
			RemoveBlit(b[_i]); \
			b[_i] = 0; \
		} \
	} \
} 

#endif

