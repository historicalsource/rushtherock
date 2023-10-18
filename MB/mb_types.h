#ifndef	_MB_TYPES_H
#define	_MB_TYPES_H

#include "stdtypes.h"
#include "mb_struct.h"

typedef struct window {
	S32 flags;
	F32 width, height;		/* window width and height (in pixels) */
	F32 hviewang,vviewang;	/* horizontal and vertical viewing angles */
	F32 hvam,vvam;			/* view angle multilier = tan(viewang/2) */
	F32 rhvam,rvvam;		/* reciprocal view angle multilier = 1.0/tan(viewang/2) */
	F32 hcos,vcos;			/* cos(viewang/2) used for object trivial accept/reject */
	F32 frontclip;
	F32 backclip;
	F32 hcenter,vcenter;	/* window screen x, y */
	F32 wwidth,wheight;		/* window screen horizontal (+vert) width/2 * rhvam */
	F32 cammat[4][3];		/* camera transformation matrix  */
	F32	hazestart;
	F32	hazefull;
	F32 hazemult;
	S32	hazecolor;			/* xxxxrrrrggggbbbb */
	F32 hazergb[3];			/* 0.0 - 255.0 */
	F32	ambientlevel;		/* 0.0 to 1.0 */
	F32	ambientrgb[3];		/* already adjusted for level (0.0 - 1.0) */
	S32	startobj;			/* object link list start index */
	S32 zmod;				/* Add to z value of all verts */
	F32 invunitscale;		/* 1/unitscale, where unitscale is a scale factor for Z/W values
							   of each vertex. i.e. unitscale=100 (invunitscale = .01) turns
							   feet into 1/100ths of a foot. The 3dfx range of Z/W is
							   [-4096,61439] units, so max range = 61439*invunitscale */
	S32 grDepthBufferFunc;	/* 3dfx depth-buffer function */
	S32 guColorCombineFunc;	/* 3dfx color-combine function */
	S32 grTexCombineFunc;	/* 3dfx texture-combine function */
    S32 guAlphaSource;		/* 3dfx alpha-source function */	
} WINDOW;

typedef struct
{
	U32 flags;
	F32	x,y,z;
	F32 r,g,b;		/* rgb[0] = intensity for white lights */
	F32 radius;
} LOCAL_LIGHT;

#define MBNUMWINDOWS	4
#define MBLIGHT_MAX		4

typedef struct
{
	/*** PARAMETERS & FLAGS ***/
	S32 NumVerts;
	S32 PolyFlags;

	/*** WINDOWS ***/
	S32	 CurWinNum;
	WINDOW Window[MBNUMWINDOWS];
	WINDOW *CurWinPtr;

	ROMOBJECTLIST * RomObjList;

	int	DrawBlitsFirst;
	S32 ClipMask;
	S32 ZMod;
	int	ZoomX;
	int	ZoomY;
	int	ZoomOffX;
	int	ZoomOffY;

	/*** Stats ***/
	S32	FrameCtr;
	S32	NumObjects; 
	S32	NumPolys; 
	S32	NumClipPolys;
	S32	NumSimpleClipPolys;
	S32	NumVisObjs;
	S32	NumVisPolys;
	S32	NumXluPolys;
	S32	NumSimpleObjs;
	S32 NumNextObjs;
	S32 NumChildObjs;
	S32	NumSortObjs;

	F32 FPS;
	F32 FrameTime;
	F32 MBTime;
	F32 DrawTime;
	F32 TextTime;
	
	U32	FIFOSTallTime;
	U32	TexSetupTime;
	U32	ClipTime;
	U32	LynchDrawTime;
	U32	TriDrawTime;
	U32	PolyDrawTime;
	U32	BlitDrawTime;
	U32	ObjectLevelTime;
	U32	VertXformTime;
	U32	NormXformTime;
	U32	VertCount;
} MB_Global;

extern MB_Global mb_global;

#endif
