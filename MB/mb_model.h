#ifndef _MB_MODEL_H
#define _MB_MODEL_H

#include "stdtypes.h"
#include "mb_struct.h"

typedef U32 ModelList[16], MODELLIST[16];

typedef struct {
	char name[16];
	F32 bndrad;
	S16 nanims;
	S16 animidx;
}OBJDEF;

typedef struct
{
	OBJDEF	*ObjDefs;
	S32		Count;
} OBJDEF_LIST;

typedef struct {
	S16 type;		/* type of animation */
	S16 id;			/* id - i.e. polygon id */
	S16 lod;		/* which lod to apply animation to */
	S16 fid;		/* function id */
	S16 odata[2];	/* data - comes from object */
	S16 udata[2];	/* data - comes from description */
}OBJANIM;

typedef struct {
	OBJANIM *ObjAnims;
	S32		Count;
} OBJANIM_LIST;

typedef struct {
	void (*animfunc)(S32 mbidx, S32 dlidx,OBJANIM *oanim);
}MBOXANIMFUNC;

/* TEXDEF is defined in mb_struct.h */
typedef struct
{
	TEXDEF	*TexDefs;
	S32		Count;
	int		PartialLoad;
	char	Name[16];
} TEXDEF_LIST;

#define MBIDX_BITS	12
#define MODEL_MAX	18
#define OBJDEF_MAX	512
#define OBJANIM_MAX	1024
#define TEXDEF_MAX	1280

#define SETMBIDX(id,mdl)	(id | (mdl << MBIDX_BITS))
#define GETMODELID(idx)		(idx >> MBIDX_BITS)
#define GETOBJID(idx)		(idx & ((1 << MBIDX_BITS) - 1))
#define GETTEXID(idx)		(idx & ((1 << MBIDX_BITS) - 1))

#include "fmath.h"

extern OBJDEF_LIST		objdef_ptrs[];
extern OBJANIM_LIST		objanim_ptrs[];
extern TEXDEF_LIST		texdef_ptrs[];
extern RomObjectList	*model_addrs[];
extern MBOXANIMFUNC 	*mboxanimfuncs;
extern S32				nmboxanimfuncs;
extern int				model_count;

#include "Pro/mb_model.pro"

#endif
