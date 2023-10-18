#ifndef _STRUCT_H
#define _STRUCT_H
#include <config.h>

#if (SST_GAME == SST_HOCKEY)
#define GLIDE2 1
#else
#define GLIDE2 1
#endif

#define PL_PPW_RGB1                   (1<<21)
#define PL_PPW_ZCOMPARE               (1<<18)
#define PL_PPW_ZWRITE				(1<<17)
#define PL_PPW_ZBUFFER                (PL_PPW_ZCOMPARE | PL_PPW_ZWRITE)
#define PL_TEMP_2SIDED                (1<<24)
#define PL_TEMP_TEXTURE               (1<<26)
#define PL_TEMP_TEXPAL                (1<<27)
#define PL_TEMP_SMOOTHSHADE			(1<<28)
#define PL_TEMP_VERTEXCOLOR			(1<<29)

#ifndef _TYPEDEF_VEC3
typedef F32 Vec3[3];
#define _TYPEDEF_VEC3
#endif
#if (SST_GAME == SST_HOCKEY)
#define MAXOBJVERTS	(1024)
#define MAXOBJNORMS	(1024)
#else
#define MAXOBJVERTS	(1024 + 1024)
#define MAXOBJNORMS	(1024 + 1024)
#endif
/*
 * Alphabetically sorted list of object names (same order as RomObjects)
 * Index in array corresponds to RomObject index
 */
typedef struct
{
	char	Name[16];
	F32		Radius;
	S16		Nanims;
	S16		AnimIndex;
} ObjDef;

typedef struct
{
	S16		type;		/* type of animation */
	S16		id;			/* id - i.e. polygon id */
	S16		lod;		/* which lod to apply animation to */
	S16		fid;		/* function id */
	S16		odata[2];	/* data - comes from object */
	S16		udata[2];	/* data - comes form description file */
} AnimEntry,ANIMENTRY;

/*
 * Alphabetically sorted list of texture names
 * Index in array corresponds to RomTex index
 * NOTE - these aren't stored with the RomTex's because they are
 * infrequently used, and there's no point clogging the cache.
 */
typedef struct
{
	char	Name[16];
	U16		Width,Height;
} TexDef, TEXDEF, TexInfo;	/* TexInfo kept for historical reasons; HOCKEY no longer uses ist */

typedef struct
{
	U16		Vidxs[3];
	U16		TexIndex;
} RomTriangle;

typedef struct
{
	U16		Nidxs[3];
	U16		Spam;
} RomTriNorm;

typedef struct
{
	Vec3	Pos;
	F32		S,T;
	S16		PrevPos;
	U8		Shade;
	U8		Extra;
} RomVert;

/*
 * ROMTEX contains the data that is unique to each texture
 */
typedef struct
{
	U08		HighLOD;
	U08		LowLOD;
	U08		Format;
	U08		Flags;
	U16		Width;
	U16		Height;
	U32		TexBase;	/* Base pointer to texture data			*/
	U32		TexBaseSST;	/* SST base register					*/
	U32		TexModeSST;	/* SST base register					*/
	U32		TexLodSST;	/* SST base register					*/
	S32		ChromaKey;	/* Transparent color					*/
	U32		TexBaseSSTReloc;
	U32		TexPal[12];
} RomTex, ROMTEX;

enum
{
	TEX_NCC_PAL = 1
};

/*
 * All indices in ROMLOD are in bytes and relative to the base of 
 * the entire object list
 * ie; 	pverts = (ROMPOLYVERT *) ((U8 *) RomObjList + lod->PVIndex);
 */
typedef struct
{
	F32		LodRad;		/* Switching radius for this LOD	*/
	U32		Flags;
	S32		VertCount;		/* Number of vertices in object		*/
	S32		VertIndex;		/* Vertex pool index				*/
	S32		TriCount;
	S32		TriIndex;
	S32		Spam;
	S32		NormIndex;
} RomLOD, ROMLOD;

enum
{
	LOD_ALPHA	= 1,
	LOD_NORMALS	= 2
};

/*
 * Info about each object
 */
typedef struct
{
	F32		InvRad;				/* Invisbilty radius - don't know what its for */
	F32		BndRad;				/* Bounding Radius of the object	*/
	S32		LodCount;			/* How many of the (max=4) LOD's are used (usually 1) */
	ROMLOD	LOD[4];
} RomObject, ROMOBJECT;

/*
 * Header for the object list
 * All data is in ROM-ready format.
 * Immediately following this header are:
 * 	Object header list		- ROMOBJECT[ObjCount] (includes ROMLOD[4])
 *	Texture header list		- ROMTEX[TexCount]
 *	Object data list		(ObjCount of them)
 * 		Vertices				- F32[VCount][3]
 *		Normals					- F32[NCount][3]
 *		Polygons				- ROMPOLY[VCount]
 *		PolyVerts				- ROMPOLYVERT[PVCount]
 *	Object def list			- ObjDef[ObjCount]			-Used by host
 *  Animation list			- AnimEntry[AnimCount]		-Used by host
 *	Texture def list		- TexDef[TexCount]			-Used by host
 */
typedef struct
{
	char	TexFileName[32];	/* Name of the texture file this object list was linked with */
	char	PalFileName[32];	/* Name of the palette associated with the texture file */
	char	Spare[12];			/* spare struct padding		 */
	U32		ObjAnimIdx;			/* Index int Object def animation table */
	S32		ObjAnimCount;		/* Number of Animation entries in animation table */
	U32		MaxTexSize;			/* Size of the largest texture - used for base address padding */
	U32		ObjDefIdx;			/* Index into Object def table		*/
	U32		TexDefIdx;			/* Index into Texture def table		*/
	S32		ObjCount;			/* Number of Objects			*/
	S32		TexCount;			/* Number of Texture indices	*/
} RomObjectList, ROMOBJECTLIST;



extern ROMOBJECTLIST *RomObjList;

/*extern void	*LoadFile(char *fname);*/
/*extern void	StructureData(U32 *base);*/

#include "Pro/mb_struct.pro"

#endif
