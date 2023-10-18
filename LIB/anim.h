#ifndef _ANIM_H
#define _ANIM_H

typedef struct anim ANIM;
typedef struct animseqinfo ANIMSEQINFO;
typedef struct animkey ANIMKEY;
typedef struct animseqheader ANIMSEQHEADER;
typedef struct animdesc ANIMDESC;

typedef struct animheader ANIMHEADER;
typedef struct animinfo ANIMINFO;
typedef struct animobj ANIMOBJ;

typedef struct partdesc PARTDESC;

/** includes needed for local prototypes **/
#include "stdtypes.h"
#include "fmath.h"
/******************************************/

#define NULLOBJ "AAANULLOBJ"
#define MAXANIMOBJS 256		/* TOTAL NUMBER OF ANIMOBJS AVAILABLE */
#define NEWANIMOBJMAX 72	/* MAX NUMBER OF ANIMOBJS IN A TREE */

#define TYPE_PYR	0x8000
#define TYPE_STATIC	0x4000
#define TYPE_COMP	0x2000
#define TYPE_PYRMASK 0x0007
#define TYPE_PYRQMASK 0x000F
#define TYPE_XYZMASK 0x0070

#define CTL_RPTFWD	0x01
#define CTL_RPTBAK	0x02
#define CTL_REPEAT	0x03
#define CTL_FIXUVS	0x04
#define CTL_FIXPOS	0x08
#define CTL_MIRROR	0x10
#define CTL_BIGKEYS	0x20
#define CTL_ROT180	0x40
#define CTL_EVERYKEY 0x80

/* AMIM BLOCK */
/*** One/type/key/seq/obj ***/
struct anim		
{
	F32	value;		/* end position of animation (position key) */
};

/* Each sequence represents a type of motion, eg standing, walking, running, etc */
/*** One/seq/obj ***/
struct animseqinfo
{
	U16	type;		/* Type of animation  ( | 0x8000 = PYR, not RYP )
					   (pitch|=1,yaw|=2,roll|=4,[|0xF = QUATERNIAN], dx|=0x10,dy|=0x20,dz|=0x40) */
	U16 size;		/* Precalculated # of entries in type */
	U32 animidx;	/* Index of first ANIMBLOCK */
};

/*** One/seq ***/
struct animseqheader
{
	U08 keys;		/* Number of keys in sequence */
	U08 control;	/* control bits 1|=rpt fwd,2|=rpt bak,8|=fixpos,  0x20|=16 bit keys */
	U16 keyidx;		/* First key in ...AnimKeys[] */
};

/*** One/key/seq ***/
struct animkey
{
	U08 key;
};

struct animdesc
{
	char desc[32];
};

struct partdesc
{
	F32 pos[3];
	char desc[32];
	char parent[32];
	char objname[32];
};

/* HEADER */
/*** One/series of seqs ***/
struct animheader {
	F32 *CompressAng;
	F32 *CompressPos;
	F32 *CompressUnit;
	ANIM *blocks;
	ANIMSEQINFO *seqs;
	ANIMSEQHEADER *seqheader;
	ANIMKEY *keys;
	ANIMDESC *objdescs;
	S32 numseqs;
	S32 numobjs;
	S32 numjoints;
	S32 numsites;
};

/*** One/tree/series of seqs ***/
struct animinfo {
	F32 *CompressAng;
	F32 *CompressPos;
	F32 *CompressUnit;
	ANIM *blocks;				/* Array of anim blocks */
	ANIMSEQHEADER *seqheader;	/* Array of key info for each sequence */
	ANIMKEY *keys;				/* Array of keys for each sequence */
	ANIMHEADER *header;			/* Animation keyframes header info */
	S16 numseqs;				/* number of animations associated with object */
	S16 animseq0;				/* Previous animation sequence */
	S16 animseq;				/* Current animation sequence */
	S16 transkey;				/* used to set up transition animations */
	F32 frac;					/* Stored fraction between idx0 and idx1 */
	S16 idx0;					/* Keyframe before current anim value */
	S16 idx1;					/* Keyframe after current anim value */
	S16 setpanim;				/* flag used to set up previos anim key */
	S16 mirror;					/* true = mirror animation across YZ plane */
	/* SEQUENCE INFO, ONLY USED IN SEQUENCER */
	S16 active;					/* TRUE if animation is active . 1=forward,2=back  */
	U16 prevkey;				/* Index of most recent key passed */
	F32 starttime;				/* Time which current block started */
	F32 transtime;				/* Time at which to end transition */
	F32 scale;					/* Scale factor for animation */
	F32 atime;	
	S16 repeat;					/* Repeat current sequence? */
	U16 stage;					/* Current stage of current animation block */
};

struct animobj {
	S32 mbidx;					/* object definition index for math box */
	S16 dlidx;					/* index in the display list for this object */
	MATRIX *mat;					/* current matrix of object (relative to parent) */
	F32 initpos[3];				/* Initial position (xyz) (relative to parent) */
	struct animobj *next;		/* Next object in tree */
	struct animobj *child;		/* Child object in tree */
	struct animobj *parent;		/* Parent object in tree */
	ANIMSEQINFO	*seq;			/* Array of animation object sequence headers */
	S16 pidx;					/* Index of last anim block */
	F32 pyrq[4];				/* Last known pitch,yaw,roll of object, OR Quaternian */
	F32 tpyrq[4];				/* Transiton pitch,yaw,roll of object, OR Quaternian */
	F32 ppos[3];				/* Last known position (xyz) (if appropriate) */
	F32 tpos[3];				/* Last known position (xyz) (if appropriate) */
	S32 objnum;					/* Which animation object */
	struct objinfo *objinfo;
};

#include "Pro/anim.pro"

extern F32 ATime;
extern ANIMINFO *AnimDebugInfo;
extern ANIMOBJ *AnimDebugObj;
extern ANIMOBJ *NewAnimobjs[NEWANIMOBJMAX];
#endif
