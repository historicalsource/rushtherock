/*************************************************************************************************
3D Hockey : anim.c
Programmers : Steven Bennets
Copyright 1994,1995 Time Warner Interactive.  Unauthorized reproduction, adaptation,
distribution, performance or display of this computer program or the associated
audiovisual work is strictly prohibited.
*************************************************************************************************/

#include <string.h>
#include "timer.h"
#include "stdtypes.h"
#include "anim.h"
#include "fmath.h"
#include "strings.h"
#include "error.h"
#include "mb.h"
#include "io.h"

static ANIMOBJ AnimObjs[MAXANIMOBJS];
ANIMOBJ *NewAnimobjs[NEWANIMOBJMAX];
static S32 NumAnimObjs;

static F32 AnimTimeScale,AnimInfoTimeScale;
static const int TypeFlags[] = {0x1, 0x2, 0x4, 0x10, 0x20, 0x40};

F32 ATime;
ANIMINFO *AnimDebugInfo;
ANIMOBJ *AnimDebugObj;

#ifndef DONTDOANIM
# define DONTDOANIM 0
#endif
#ifndef ANIMDEBUG
# define ANIMDEBUG 1
#endif
#ifndef ANIMDEBUG1
# define ANIMDEBUG1 0
#endif
#ifndef ANIMDEBUG2
# define ANIMDEBUG2 0
#endif
#ifndef ANIMCOLOR
# define ANIMCOLOR VIO_PAL
#endif
#ifndef ANIMLINE
# define ANIMLINE 20
#endif
#ifndef ANIMTIMESCALE
# define ANIMTIMESCALE (1.0 / 30.0)
#endif
#ifndef ANIMDEBUGOBJ
# define ANIMDEBUGOBJ(x) (x == AnimDebugObj)
#endif
#ifndef ANIMDEBUGINFO
# define ANIMDEBUGINFO(x) (x == AnimDebugInfo)
#endif
#ifndef XYPRINTF
# define XYPRINTF(x) xyprintfc##x
#endif
#ifndef FATALERRORF
# define FATALERRORF(x) FatalErrorf##x
#endif
#ifndef ERRORF
# define ERRORF(x) Errorf##x
#endif

#ifndef __INLINE__
# define __INLINE__ __inline__
#endif

#define ANIMANGLE(x) (x)
#define ANIMFOOT(x) (x)

#define QUATERNIANS 1
#if QUATERNIANS
# define QUAT 0x08
#else
# define QUAT 0x0
#endif

/*** MACROS FOR SIMPLIFYING VALUES ***/
__INLINE__ static F32 SimpleAng(F32 ang)
{
	if (ang > RAD(180))
		ang -= RAD(360);
	else if (ang <= RAD(-180))
		ang += RAD(360);
	return ang;
}

__INLINE__ static F32 SimpleQuat(F32 *quat)
{
	int i;
	for (i=0;i<4; i++) {
		if (quat[i] > 1.0)
			quat[i] -= 2.0;
		else if (quat[i] <= -1.0)
			quat[i] += 2.0;
	}
}

/*** MACROS FOR EXTRACTING ANIMATION DATA ***/
/* THREE TYPES
 *  TYPE_STATIC: One value which does not change across the animation
 *  TYPE_COMP: Compressed data, first 4 bytes = F32 base value,
 *				subsequent bytes are indicies into a compression table.
 *  DEFAULT: Uncompressed data, subsequent F32 values.
 * NOTE: Values are stored by keyframe, not by chanel, so for PYRX: (pitch, yaw, roll, and dX)
 * 			P1 Y1 R1 X1 P2 Y2 R2 X2 ...
 *		 where [PYRX]1 = F32 base value, and [PYRX][2...] = U08 index (if compressed) or F32 value.
 */
#define COPYQUAT(q1,q2) (q2[0] = q1[0], q2[1] = q1[1], q2[2] = q1[2], q2[3] = q1[3])
#define GETANIMQUAT(q,anim) (q[0]=(anim+0)->value, q[1]=(anim+1)->value, q[2]=(anim+2)->value, q[3]=(anim+3)->value)

__INLINE__ static int GetPYR(F32 *pyr, ANIM *anim,S16 type)
{
	int i;
	int count = 0;
	for(i=0; i<3; i++) {
		if (type & TypeFlags[i]) 
			pyr[i] = (anim + count)->value, count++;
		else
			pyr[i] = 0.0;
	}
	return count;
}
__INLINE__ static void GetXYZ(F32 *xyz, ANIM *anim,S16 type,int count)
{
	int i;
	for(i=0; i<3; i++) {
		if (type & TypeFlags[3+i])
			xyz[i] = (anim + count)->value,	count++;
		else
			xyz[i] = 0.0;
	}
}

__INLINE__ static
void GetAnimQuatXYZVal(ANIM *anim,ANIMINFO *info,F32 *q,F32 *pq,F32 *xyz,F32 *pxyz,
					   S16 type,S16 count0,S16 idx,S16 pidx)
{
	int count,i,j;
	U08 *tptr;
	U08 tableidx;
	int prev;
	
	if (type & TYPE_STATIC || idx == 0) {
		GETANIMQUAT(q,anim);
		GetXYZ(xyz,anim,type,4);
	}
	else if (type & TYPE_COMP)
	{
		if (pidx >= 0 && pidx <= idx) {
			if (pq)	COPYQUAT(pq,q);
			if (pxyz) CopyVector(pxyz,xyz);
			prev = pidx+1;
		} else if (pidx == idx+1) {
			if (pq)	COPYQUAT(pq,q);
			if (pxyz) CopyVector(pxyz,xyz);
			prev = -1;
		} else {
			GETANIMQUAT(q,anim);
			GetXYZ(xyz,anim,type,4);
			prev = 1;
		}
		if (prev == -1) {
			tptr = (U08 *)(anim+count0) + (idx)*count0;
			for (i=0; i<4; i++) {
				tableidx = *(tptr+i);
				q[i] -= info->CompressUnit[tableidx];
			}
			count = 4;
			for (i=0; i<3; i++) {
				if (type & TypeFlags[3+i]) {
					tableidx = *(tptr + count);
					xyz[i] -= info->CompressPos[tableidx];
					count++;
				}
			}
		} else {
			for (j=prev; j<=idx; j++) {
				tptr = (U08 *)(anim+count0) + (j-1)*count0;
				for (i=0; i<4; i++) {
					tableidx = *(tptr+i);
					q[i] += info->CompressUnit[tableidx];
				}
				count = 4;
				for (i=0; i<3; i++) {
					if (type & TypeFlags[3+i]) {
						tableidx = *(tptr + count);
						xyz[i] += info->CompressPos[tableidx];
						count++;
					}
				}
			}
		}
		SimpleQuat(q);
	}
	else
	{
		anim += idx*count0;
		GETANIMQUAT(q,anim);
		GetXYZ(xyz,anim,type,4);
	}
}

__INLINE__ static
void GetAnimAngXYZVal(ANIM *anim,ANIMINFO *info,F32 *pyr,F32 *ppyr,F32 *xyz,F32 *pxyz,
					 S16 type,S16 count0,S16 idx,S16 pidx)
{
	int count,i,j;
	U08 *tptr;
	U08 tableidx;
	int prev;
	
	if (type & TYPE_STATIC || idx == 0)
	{
		count = GetPYR(pyr,anim,type);
		GetXYZ(xyz,anim,type,count);
	}
	else if (type & TYPE_COMP)
	{
		if (pidx >= 0 && pidx <= idx) {
			if (ppyr) CopyVector(ppyr,pyr);
			if (pxyz) CopyVector(pxyz,xyz);
			prev = pidx+1;
		} else if (pidx == idx+1) {
			if (ppyr) CopyVector(ppyr,pyr);
			if (pxyz) CopyVector(pxyz,xyz);
			prev = -1;
		} else {
			count = GetPYR(pyr,anim,type);
			GetXYZ(xyz,anim,type,count);
			prev = 1;
		}
		if (prev == -1) {
			tptr = (U08 *)(anim+count0) + (idx)*count0;
			count = 0;
			for (i=0; i<3; i++) {
				if (type & TypeFlags[i]) {
					tableidx = *(tptr + count);
					pyr[i] -= info->CompressAng[tableidx];
					count++;
				}
			}
			for (i=0; i<3; i++) {
				if (type & TypeFlags[3+i]) {
					tableidx = *(tptr + count);
					xyz[i] -= info->CompressPos[tableidx];
					count++;
				}
			}
		} else {
			for (j=prev; j<=idx; j++) {
				tptr = (U08 *)(anim+count0) + (j-1)*count0;
				count = 0;
				for (i=0; i<3; i++) {
					if (type & TypeFlags[i]) {
						tableidx = *(tptr + count);
						pyr[i] += info->CompressAng[tableidx];
						count++;
					}
				}
				for (i=0; i<3; i++) {
					if (type & TypeFlags[3+i]) {
						tableidx = *(tptr + count);
						xyz[i] += info->CompressPos[tableidx];
						count++;
					}
				}
			}
		}
		for (i=0; i<3; i++)
			pyr[i] = SimpleAng(pyr[i]);
	}
	else
	{
		anim += count0*idx;
		count = GetPYR(pyr,anim,type);
		GetXYZ(xyz,anim,type,count);
	}
}

/*** MACROS FOR INTERPRETING BETWEEN TWO VALUES */
__INLINE__ static
void InterpPYR(F32 frac,F32 *pyr0, F32 *pyr1, F32 *res)
{
	F32 x0,x1,dx;
	int i;

	for (i=0; i<3; i++)
	{
		x0 = pyr0[i];
		x1 = pyr1[i];
		if (x0 != x1) {
			dx = x1-x0;
			if (dx > PI) dx -= 2*PI;
			if (dx <= -PI) dx += 2*PI;
			res[i] = x0 + frac * dx;
		}
		else
			res[i] = x0;
	}
}

__INLINE__ static
void InterpXYZ(F32 frac,F32 *xyz0, F32 *xyz1, F32 *res)
{
	F32 x0,x1;
	int i;
	for (i=0; i<3; i++) {
		x0 = xyz0[i];
		x1 = xyz1[i];
		if (x0 != x1)
			res[i] = x0 + frac * (x1 - x0);
		else
			res[i] = x0;
	}
}

/* **********************************
 * ROUTINES FOR PLAYING ANIMATIONS *
 **********************************/

/* Calculates the MATRIX for animobj.
 * Interpolates between ANIM block (anim + idx0) and (anim + idx1) by frac */
static void CalcAnimation(ANIMOBJ *animobj,ANIM *anim,ANIMINFO *info,S16 type,S16 count)
{
	int pidx = animobj->pidx;
	F32 *pyrq = animobj->pyrq;
	F32 *ppos = animobj->ppos;
	S16 idx0 = info->idx0;
	S16 idx1 = info->idx1;
	F32 frac = info->frac;
	int mirror = info->mirror;
	F32 lpyrq[4],xyz[3];

	START_TIMER(timer_anim2);
		
	animobj->pidx = idx0;
	if (idx1 == 0)
		pidx = -1;
#if ANIMDEBUG
	if (ANIMDEBUG2 && ANIMDEBUGOBJ(animobj))
		xyprintfc(ANIMCOLOR,0,ANIMLINE+6, " CALC  FRAC:%f IDX0:%d IDX1:%d TYPE:%x PIDX:%d  ",
				  frac,idx0,idx1,type&0xFF,pidx);
#endif
	if (idx1 == 0 || frac == 0.0) /* if (idx1==0) idx0 MUST == 0 */
	{
#if QUATERNIANS
		if (type & 8) {
			GetAnimQuatXYZVal(anim,info,lpyrq,pyrq,xyz,ppos,type,count,idx0,pidx);
			CopyQuat(lpyrq,pyrq);
			if (mirror) {
				lpyrq[1] = -lpyrq[1];
				lpyrq[2] = -lpyrq[2];
			}
			START_TIMER(timer_anim3);
			CreateQuatMat(&animobj->UVSP[0][0],lpyrq,1);
			END_TIMER(timer_anim3);
		}
		else /* PYR */
#endif
		{
			GetAnimAngXYZVal(anim,info,lpyrq,pyrq,xyz,ppos,type,count,idx0,pidx);
			CopyVector(lpyrq,pyrq);
			if (mirror) {
				lpyrq[1] = -lpyrq[1];
				lpyrq[2] = -lpyrq[2];
			}
			if (type & TYPE_PYR)
				CreatePYRMatrix(animobj->UVSP,lpyrq);
			else
				CreateRYPMatrix(animobj->UVSP,lpyrq);
		}
		CopyVector(xyz,ppos);
		if (mirror)
			xyz[0] = -xyz[0];			
		AddVector(xyz,animobj->initpos,animobj->POSP);
	}
	else
	{
#if QUATERNIANS
		if (type & QUAT) {
			if (pidx != idx0)
				GetAnimQuatXYZVal(anim,info,pyrq,0,ppos,0,type,count,idx0,pidx);
			GetAnimQuatXYZVal(anim,info,lpyrq,pyrq,xyz,ppos,type,count,idx1,idx0);
#if 0&&ANIMDEBUG
			if (ANIMDEBUG2 && ANIMDEBUGOBJ(animobj))
				xyprintfc(ANIMCOLOR,0,ANIMLINE+4,
						  " Q0:%.3f %.3f %.3f %.3f Q1:%.3f %.3f %.3f %.3f  ",
						  pyrq[0],pyrq[1],pyrq[2],pyrq[3],lpyrq[0],lpyrq[1],lpyrq[2],lpyrq[3]);
#endif
			InterpQuats(frac,pyrq,lpyrq,lpyrq);
			if (mirror) {
				lpyrq[1] = -lpyrq[1];
				lpyrq[2] = -lpyrq[2];
			}
			START_TIMER(timer_anim3);
			CreateQuatMat(&animobj->UVSP[0][0],lpyrq,0);
			END_TIMER(timer_anim3);
		}
		else /* PYR */
#endif
		{
			if (pidx != idx0)
				GetAnimAngXYZVal(anim,info,pyrq,0,ppos,0,type,count,idx0,pidx);
			GetAnimAngXYZVal(anim,info,lpyrq,pyrq,xyz,ppos,type,count,idx1,idx0);
			InterpPYR(frac,pyrq,lpyrq,lpyrq);
			if (mirror) {
				lpyrq[1] = -lpyrq[1];
				lpyrq[2] = -lpyrq[2];
			}
			if (type & TYPE_PYR)
				CreatePYRMatrix(animobj->UVSP,lpyrq);
			else
				CreateRYPMatrix(animobj->UVSP,lpyrq);
		}
#if 0&&ANIMDEBUG
		if (ANIMDEBUG2 && ANIMDEBUGOBJ(animobj))
			xyprintfc(ANIMCOLOR,0,ANIMLINE+5,
					  " POS0:%.3f %.3f %.3f POS1:%.3f %.3f %.3f  ",
					  ppos[0],ppos[1],ppos[2],xyz[0],xyz[1],xyz[2]);
#endif
		InterpXYZ(frac,ppos,xyz,xyz);
		if (mirror)
			xyz[0] = -xyz[0];			
		AddVector(xyz,animobj->initpos,animobj->POSP);
	}
#if ANIMDEBUG
	if (ANIMDEBUG2 && ANIMDEBUGOBJ(animobj)) {
		if (type & QUAT)
			xyprintfc(ANIMCOLOR,0,ANIMLINE+7, " QUAT:%.3f %.3f %.3f %.3f   ",
					  lpyrq[0],lpyrq[1],lpyrq[2],lpyrq[3]);
		else
			xyprintfc(ANIMCOLOR,0,ANIMLINE+7, " ROT:(%.3f %.3f %.3f)    ",
					  lpyrq[0],lpyrq[1],lpyrq[2]);
		xyprintfc(ANIMCOLOR,32,ANIMLINE+7, " POS:(%.3f %.3f %.3f)    ",
				 animobj->POSP[0],animobj->POSP[1],animobj->POSP[2]);
	}
#endif

	END_TIMER(timer_anim2);	
}

/* Calculates the MATRIX for animobj.
 * Interpolates between previous pyrq (animobj->pyrq) and (anim + idx) by frac */
static void CalcTransAnimation(ANIMOBJ *animobj,ANIM *anim,ANIMINFO *info,S16 type1,S16 count1)
{
	F32 dx,x0,x1;
	int i;
	F32 *tpyrq = animobj->tpyrq;
	F32 *pyrq = animobj->pyrq;
	F32 *tpos = animobj->tpos;
	F32 *ppos = animobj->ppos;
	S16 pidx = animobj->pidx;
	S16 idx1 = info->idx1;
	F32 frac = info->frac;
	int mirror = info->mirror;
	F32 lpyrq[4],xyz[3];

#if ANIMDEBUG
	if (ANIMDEBUG2 && ANIMDEBUGOBJ(animobj))
		xyprintfc(ANIMCOLOR,0,ANIMLINE+6, " TRANS FRAC:%f (IDX0:%d) IDX1:%d TYPE1:%04x PIDX:%d  ",
				  frac,info->idx0,idx1,type1&0xffff,animobj->pidx);
#endif
	if (pidx < 0)
		animobj->pidx = idx1;

#if QUATERNIANS
	if (type1 & QUAT)
	{
		if (pidx < 0)
			GetAnimQuatXYZVal(anim,info,tpyrq,0,tpos,0,type1,count1,idx1,-1);
		if (pidx < -1) /* We don't know where we were */
			CopyQuat(tpyrq,pyrq);
		InterpQuats(frac,pyrq,tpyrq,lpyrq);
		if (mirror) {
			lpyrq[1] = -lpyrq[1];
			lpyrq[2] = -lpyrq[2];
		}
		CreateQuatMat(&animobj->UVSP[0][0],lpyrq,0);
	}
	else /*PYR */
#endif
	{
		if (pidx < 0)
			GetAnimAngXYZVal(anim,info,tpyrq,0,tpos,0,type1,count1,idx1,-1);
		if (pidx < -1) /* We don't know where we were */
			CopyVector(tpyrq,pyrq);
		InterpPYR(frac,pyrq,tpyrq,lpyrq);
		if (mirror) {
			lpyrq[1] = -lpyrq[1];
			lpyrq[2] = -lpyrq[2];
		}
		if (type1 & TYPE_PYR)
			CreatePYRMatrix(animobj->UVSP,lpyrq);
		else
			CreateRYPMatrix(animobj->UVSP,lpyrq);
	}
	InterpXYZ(frac,ppos,tpos,xyz);
	if (mirror)
		xyz[0] = -xyz[0];			
	AddVector(xyz,animobj->initpos,animobj->POSP);
	
#if ANIMDEBUG
	if (ANIMDEBUG2 && ANIMDEBUGOBJ(animobj)) {
		if (type1 & QUAT)
			xyprintfc(ANIMCOLOR,0,ANIMLINE+7, " QUAT:%.3f %.3f %.3f %.3f   ",
					  lpyrq[0],lpyrq[1],lpyrq[2],lpyrq[3]);
		else
			xyprintfc(ANIMCOLOR,0,ANIMLINE+7, " ROT:(%.3f %.3f %.3f)    ",
					  lpyrq[0],lpyrq[1],lpyrq[2]);
		xyprintfc(ANIMCOLOR,32,ANIMLINE+7, " POS:(%.2f %.2f %.2f)  ",
				  animobj->POSP[0],animobj->POSP[1],animobj->POSP[2]);
	}
#endif
}

/* Calculates the MATRIX for animobj.
 * Interpolates between ANIM block (anim0 + idx0) and (anim1 + idx1) by frac */
static void CalcAnimation2(ANIMOBJ *animobj,ANIMINFO *info,
						   ANIM *anim0,ANIM *anim1,S16 type0,S16 count0,S16 type1,S16 count1)
{
	F32 dx,x0,x1;
	int i;
	F32 *pyrq = animobj->pyrq;
	F32 *ppos = animobj->ppos;
	S16 idx0 = info->idx0;
	S16 idx1 = info->idx1;
	F32 frac = info->frac;
	int mirror = info->mirror;
	F32 lpyrq[4],xyz[3];

#if ANIMDEBUG
	if (ANIMDEBUG2 && ANIMDEBUGOBJ(animobj))
		xyprintfc(ANIMCOLOR,0,ANIMLINE+6, " CALC2 FRAC:%f IDX0:%d IDX1:%d TYPE:%x  ",
				  frac,idx0,idx1,(U32)(type1&0xFF));
#endif
	
#if QUATERNIANS
	if (type1 & QUAT)
	{
		GetAnimQuatXYZVal(anim0,info,lpyrq,0,xyz, 0,type0,count0,idx0,-1);
		GetAnimQuatXYZVal(anim1,info,pyrq, 0,ppos,0,type1,count1,idx1,-1);
		InterpQuats(frac,lpyrq,pyrq,lpyrq);
		CopyQuat(lpyrq,pyrq);
		if (mirror) {
			lpyrq[1] = -lpyrq[1];
			lpyrq[2] = -lpyrq[2];
		}
		CreateQuatMat(&animobj->UVSP[0][0],lpyrq,0);
	}
	else /*PYR */
#endif
	{
		GetAnimAngXYZVal(anim0,info,lpyrq,0,xyz, 0,type0,count0,idx0,-1);
		GetAnimAngXYZVal(anim1,info,pyrq, 0,ppos,0,type1,count1,idx1,-1);
		InterpPYR(frac,lpyrq,pyrq,lpyrq);
		CopyVector(lpyrq,pyrq);
		if (mirror) {
			lpyrq[1] = -lpyrq[1];
			lpyrq[2] = -lpyrq[2];
		}
		if (type1 & TYPE_PYR)
			CreatePYRMatrix(animobj->UVSP,pyrq);
		else
			CreateRYPMatrix(animobj->UVSP,pyrq);
	}
	InterpXYZ(frac,xyz,ppos,xyz);
	CopyVector(xyz,ppos);
	if (mirror)
		xyz[0] = -xyz[0];			
	AddVector(xyz,animobj->initpos,animobj->POSP);
	
#if ANIMDEBUG
	if (ANIMDEBUG2 && ANIMDEBUGOBJ(animobj)) {
		if (type1 & QUAT)
			xyprintfc(ANIMCOLOR,0,ANIMLINE+7, " QUAT:%.3f %.3f %.3f %.3f   ",
					  pyrq[0],pyrq[1],pyrq[2],pyrq[3]);
		else
			xyprintfc(ANIMCOLOR,0,ANIMLINE+7, " ROT:(%.3f %.3f %.3f)    ",
					  pyrq[0],pyrq[1],pyrq[2]);
		XYPRINTF((ANIMCOLOR,32,ANIMLINE+7, " POS:(%.3f %.3f %.3f)    ",ppos[0],ppos[1],ppos[2]));
	}
#endif
}

/* ************************************
 * ROUTINES FOR SEQUENCING ANIMATIONS * 
 * i.e. INCRAMENTING ANIMATION FRAMES *
 * AND CALCULATING TRANSITIONS		  *
 *************************************/

/* Retrieves the next key (time) value from keys. ANIMKEY is a char.
 * If keyflags is set, 16 bits are read instead of 8 (two 'keys' are combined).
 * Keys are in frames (normally 30ths of a second, AnimTimeScale = 1.0/30.0).
 * AnimInfoTimeScale = AnimTimeScale * animinfo->scale (see CalcAnimInfo). */
static F32 GetKey(int keyflags, int idx, ANIMKEY *keys)
{
	F32 res;
	
	if (keyflags)
		res = (F32)((U32)((keys + idx*2)->key)*256 + ((keys + idx*2 + 1)->key));
	else
		res = (F32)((keys + idx)->key);
	res *= AnimInfoTimeScale;
	return res;
}

/*********************************/
/* ANIMATION PROCESSING ROUTINES */
/*********************************/
U16 AnimSeqType(ANIMOBJ *animobj,ANIMINFO *info)
{
	ANIMSEQINFO *animseq;
	ANIM *anim;
	animseq = animobj->seq;
	animseq += info->animseq;
	anim = (ANIM *)((U08 *)info->blocks + animseq->animidx);	/* Base of ANIM array */
	return animseq->type;
}

static int DoAnimation(ANIMOBJ *animobj,ANIMINFO *info)
{
	S16 type,count;
	ANIMSEQINFO *animseq;
	ANIM *anim;
	int i;
	int res=1;
	
	animseq = animobj->seq;
	if (!animseq)
	{
		if (ANIMDEBUG2 && ANIMDEBUGOBJ(animobj))
			XYPRINTF((ANIMCOLOR,0,ANIMLINE+6, " NO ANIMSEQ    "));
		return 0;
	}

	START_TIMER(timer_anim1);
	
	animseq += info->animseq;
	anim = (ANIM *)((U08 *)info->blocks + animseq->animidx);	/* Base of ANIM array */
	type = animseq->type;
	count = animseq->size;
	
	if (info->setpanim & 0x3)
	{
		if (info->setpanim & 0x2)
			animobj->pidx = -2;
		else
			animobj->pidx = -1;
		info->setpanim |= 0x80;
	}

	if (info->transkey < 0)
	{
		S16 type0,count0;
		ANIMSEQINFO *seq0;
		ANIM *anim0;
	
		seq0 = animobj->seq;
		seq0 += info->animseq0;
		anim0 = (ANIM *)((U08 *)info->blocks + seq0->animidx);	/* Base of ANIM array */
		type0 = seq0->type;
		count0 = seq0->size;
		CalcAnimation2(animobj,info,anim0,anim,type0,count0,type,count);
	}		
	else if (info->transkey)
	{
		CalcTransAnimation(animobj, anim, info, type, count);
	}
	else
	{	
		if (!(type & 0xFF))	/* No animation for this object */
		{
			if (ANIMDEBUG2 && ANIMDEBUGOBJ(animobj))
				XYPRINTF((ANIMCOLOR,0,ANIMLINE+6, " KF:%.0f TYPE:%.0f",(F32)info->idx1,(F32)(type&0xFF)));
			CopyUV(unituvs,animobj->UVSP);
			for (i=0; i<3; i++)
				animobj->POSP[i] = animobj->initpos[i];
			animobj->pidx = -1;
			res = 0;
		}
		else
			CalcAnimation(animobj, anim, info, type, count);
	}

	END_TIMER(timer_anim1);
	return res;
}

int AnimateObj(ANIMOBJ *animobj,ANIMINFO *info,int update)
{
	int res;
	
	if (ANIMDEBUG2 && ANIMDEBUGOBJ(animobj))
		XYPRINTF((ANIMCOLOR,0,ANIMLINE+6, " DO ANIM  "));
	
	if (info && (!update || !DONTDOANIM))
		res = DoAnimation(animobj,info);
	else
		res = 0;
	
	if (update)
		ZOID_UpdateObjectMat(animobj->dlidx);
	
	if (animobj->next)
		AnimateObj(animobj->next,info,update);
	if (animobj->child)
		AnimateObj(animobj->child,info,update);
	return res;
}

int AnimateTreeFrameInterp(ANIMINFO *info,ANIMOBJ *root,
						   S16 seq, S16 frame0, S16 frame1, F32 frac, int update)
{
	if (seq != info->animseq) {
		if (seq >= info->numseqs)
			return 0;
		info->setpanim = 0x1;
		info->transkey = 1;
		info->animseq = seq;
	} else {
		info->transkey = 0;
	}
	info->frac = frac;
	info->idx0 = frame0;
	info->idx1 = frame1;

	AnimateObj(root,info,update);
	return 1;
}	

int AnimateTreeFrameInterp2(ANIMINFO *info,ANIMOBJ *root,
							S16 seq0, S16 seq1, S16 frame0, S16 frame1, F32 frac, int update)
{
	if (seq0 != seq1) {
		if (seq0 >= info->numseqs || seq1 >= info->numseqs)
			return 0;
		info->transkey = -1;
		info->animseq0 = seq0;
		info->animseq = seq1;
	} else {
		info->transkey = 0;
		info->animseq = seq1;
	}
	info->frac = frac;
	info->idx0 = frame0;
	info->idx1 = frame1;

	AnimateObj(root,info,update);
	return 1;
}

int AnimateTreeFrame(ANIMINFO *info,ANIMOBJ *root, S16 seq, S16 frame, int update)
{
	return AnimateTreeFrameInterp(info,root, seq, frame, frame, 0.0, update);
}

/*********************** ANIMATION SEQUENCER ***********************/
/* These functions automatically step through an animation sequence.
 * The animinfo->stage variable is set to indicate the stage of the
 * animation. If animinfo->repeat is set the sequence will repeat
 * automatiocally.
 */

/* Calculates where within the current sequence (or between sequences)
   the animation should be.
   If info->transkey != 0, a transition is happening. info->idx1 is set to the transition key.
   Otherwise, idx0 and idx1 are set to set to prev/next frame (frac = % between).
   */
static int CalcAnimInfo(ANIMINFO *info)
{
 	ANIMSEQHEADER *seqheader;
	ANIMKEY *keys;
	int numkeys;
	F32 dtime,t0 = 0.0,t1 = 0.0;
	S32 keyflag;
	
	if (!info)
		return 0;
	if ((info->stage & 0xFF) == 0xFF)
	{
		return 0;
	}

	info->atime = ATime;
	
	if (info->setpanim >= 0x80)
		info->setpanim = 0;
	
	seqheader = info->seqheader + info->animseq;
	keyflag = seqheader->control & CTL_BIGKEYS ? 1 : 0;
	numkeys = seqheader->keys;
	keys = info->keys + seqheader->keyidx;

	if (!numkeys)
	{
		info->active = 0;
		info->stage |= 0xFF;
		info->starttime = info->atime;
		if (ANIMDEBUG1)
			XYPRINTF((ANIMCOLOR,0,ANIMLINE+4,"  ZERO KEYFRAME ANIMATION"));
		return 0;
	}
	AnimInfoTimeScale = info->scale * AnimTimeScale;

	if (info->transkey)
	{
		S16 transkey = info->transkey - 1;
		t1 = info->transtime;
		if (info->atime >= t1)
		{
			if (ANIMDEBUG2 && ANIMDEBUGINFO(info))
				XYPRINTF((ANIMCOLOR,50,ANIMLINE+2,"TRANS: END  "));
			if (numkeys == 1)	/* Done. */
			{
				t0 = info->starttime + GetKey(keyflag,0,keys);
					
				info->stage |= 0xFF;		/* Done */

				info->starttime = t0;
				info->transkey = 0;
				info->setpanim |= 0x1;
				info->frac = 0.0;
				info->idx0 = info->idx1;  /* Ignored in transition, but needed for replay */
				info->idx1 = 0;
				info->animseq0 = info->animseq;
				return 2;
			}
			else
			{
				t0 = GetKey(keyflag,transkey,keys);
				info->starttime = info->atime - t0;
				info->prevkey = transkey;
				info->transkey = 0;
				info->setpanim |= 0x1;
				info->idx0 = info->idx1;
				info->animseq0 = info->animseq;
				return 0xF;				/* End Transition */
			}
		}
		t0 = info->starttime;
		if (t1 < t0)
		{
			if (ANIMDEBUG1 && ANIMDEBUGINFO(info))
				xyprintfc(ANIMCOLOR,0,ANIMLINE+5,"Bad transtime0: %.3f %.3f at %.3f",
						  t0,t1,info->atime);
			info->transkey = 0;
			return 2;
		}

		if (ANIMDEBUG2 && ANIMDEBUGINFO(info))
			XYPRINTF((ANIMCOLOR,50,ANIMLINE+2,"TRANS: ->%d  ",transkey));

		if (t1 == t0)
			info->frac = 0.0;
		else
			info->frac = (info->atime - t0) / (t1 - t0);
		/* info->idx0 = info->idx1;*/	/* Ignored in transition, but needed for replay */
		info->idx1 = transkey;	/* info->anim + transkey points to new ANIM block */

		if (ANIMDEBUG2 && ANIMDEBUGINFO(info))
			xyprintfc(ANIMCOLOR,0,ANIMLINE+2, " TRANS %.0f: %.3f %.3f (%.4f)       ",
					 (F32)info->animseq,t0,t1,info->frac);
	}
	else
	{	
		int idx;
#if 1
		if (numkeys == 1)	/* Done. */
		{
			t0 = info->starttime + GetKey(keyflag,0,keys);

			info->stage |= 0xFF;		/* Done */
			info->starttime = t0;
			if (ANIMDEBUG2 && ANIMDEBUGINFO(info))
				XYPRINTF((ANIMCOLOR,50,ANIMLINE+2,"ONE KEY  "));
			return 0xF;
		}
#endif
		if (info->active == 1)	/* forward */
		{
			dtime = info->atime - info->starttime;

			for (idx=info->prevkey; idx<numkeys; idx++)
			{
				t1 = GetKey(keyflag,idx,keys);
				if (t1 >= dtime)
					break;
			}
			if (idx >= numkeys)	/* past last key */
			{
				t0 = info->starttime + GetKey(keyflag,numkeys-1,keys);

				info->stage |= 0x100;
				if (info->repeat == 1)
				{
/*					t0 += 1.0/30.0;	 */
					info->prevkey = 0;
					info->setpanim |= 0x1;
					if (ANIMDEBUG2 && ANIMDEBUGINFO(info))
						XYPRINTF((ANIMCOLOR,50,ANIMLINE+3,"RPT   "));
				}
				else if (info->repeat == 2)
				{
/*					t0 += 1.0/30.0;	 */
					info->prevkey = 0;
					info->active = 2;
				}
				else
				{
					info->stage |= 0xFF;		/* Done */
					info->frac = 0.0;
					info->idx0 = numkeys-1;
					info->idx1 = numkeys-1;
				}
				info->starttime = t0;
				if (ANIMDEBUG2 && ANIMDEBUGINFO(info))
					XYPRINTF((ANIMCOLOR,50,ANIMLINE+2,"FWD : END  "));
				return 0xF;
			}

			if (idx > 0)
				t0 = GetKey(keyflag,idx-1,keys);
			else
				t0 = 0.0;
			if (t1 < t0)
			{
				if (ANIMDEBUG1 && ANIMDEBUGINFO(info))
					xyprintfc(ANIMCOLOR,2,ANIMLINE+5,"  Bad transtime1: %.3f %.3f idx:%d/%d  ",
							  t0,t1,idx,numkeys);
				return 2;
			}
#if 1
			if (ANIMDEBUG2 && ANIMDEBUGINFO(info))
				XYPRINTF((ANIMCOLOR,50,ANIMLINE+2,"F IDX: %d-%d   ",idx-1,idx));
#endif
			if ((idx == 0) || (seqheader->control & CTL_EVERYKEY)
				/* || (t1 - t0 < .034) */ ) 	/* close enough to 1 frame */
			{
				info->frac = 0.0;
				info->idx0 = idx;
				info->idx1 = idx;
			}
			else
			{
				info->frac = (dtime - t0) / (t1 - t0);
				info->idx0 = idx-1;
				info->idx1 = idx;
			}
			info->prevkey = info->idx0;
		}
		else if (info->active == 2) /* backward */
		{
			F32 endtime;
			U16 prevkey;
				
			endtime = GetKey(keyflag,numkeys-1,keys);
			dtime = endtime - (info->atime - info->starttime);

			if (dtime <= 0)	/* past first key */
			{
				t0 = info->starttime + GetKey(keyflag,numkeys-1,keys);
				info->stage |= 0x100;
				if (info->repeat == 1)
				{
					t0 += 1.0/30.0;					
					info->prevkey = 0;
					info->setpanim |= 0x1;
				}
				else if (info->repeat == 2)
				{
					t0 += 1.0/30.0;					
					info->prevkey = 0;
					info->active = 1;
				}
				else
				{
					info->stage |= 0xFF;		/* Done */
				}

				info->starttime = t0;
				if (ANIMDEBUG2 && ANIMDEBUGINFO(info))
					XYPRINTF((ANIMCOLOR,50,ANIMLINE+2,"BACK: END  "));
				return 0xF;
			}
				
			prevkey = info->prevkey;
			if (!prevkey || (prevkey >= numkeys))
				prevkey = numkeys-1;

			for (idx=prevkey; idx>=0; idx--)
			{
				t0 = GetKey(keyflag,idx,keys);
				if (t0 <= dtime)
					break;
			}

			if (idx < 0) /*past first key (shouldn't happen) */
			{
				t0 = info->starttime + GetKey(keyflag,numkeys-1,keys);
				info->stage |= 0x100;
				if (info->repeat == 1)
				{
					t0 += 1.0/30.0;					
					info->prevkey = 0;
				}
				else if (info->repeat == 2)
				{
					t0 += 1.0/30.0;					
					info->prevkey = 0;
					info->active = 1;
				}
				else
				{
					info->stage |= 0xFF;		/* Done */
				}

				info->starttime = t0;
				return 0xF;
			}
			
#if 1
			if (ANIMDEBUG2 && ANIMDEBUGINFO(info))
				XYPRINTF((ANIMCOLOR,50,ANIMLINE+2,"B IDX: %d-%d",idx,idx+1));
#endif
			t1 = GetKey(keyflag,idx+1,keys);
			if (t1 < t0)
			{
				if (ANIMDEBUG1 && ANIMDEBUGINFO(info))
					xyprintfc(ANIMCOLOR,2,ANIMLINE+5,"  Bad transtime2: %.3f %.3f at %.3f",
							  t0,t1,info->atime);
				return 2;
			}
#if 1
			if ((idx == 0) || (t1 - t0 < .034))	/* close enough to 1 frame */
			{
				info->frac = 0.0;
				info->idx0 = idx;
				info->idx1 = idx;
			}
			else
#endif
			{
				info->frac = (dtime - t0) / (t1 - t0);
				info->idx0 = idx;
				info->idx1 = idx+1;
			}
			info->prevkey = info->idx1;
		}
		if (ANIMDEBUG2 && ANIMDEBUGINFO(info))
		{
			xyprintfc(ANIMCOLOR,0,ANIMLINE+2, " KEYF %.0f: %.0f-%.0f: %.3f %.3f (%f)    ",
					 (F32)info->animseq,(F32)info->idx0,(F32)info->idx1,t0,t1,info->frac);
		}
	}
	return 1;
}

void AnimFixPos(ANIMOBJ *child,ANIMOBJ *parent,ANIMINFO *info)
{
	F32 delta[3];
		
	WorldVector(child->POSP,delta,parent->UVSP);
	delta[1] = 0.0;
	AddVector(parent->POSP,delta,parent->POSP);

	child->POSP[0] = 0.0;
	child->POSP[2] = 0.0;
	child->ppos[0] = 0.0;
	child->ppos[2] = 0.0;
	info->setpanim |= 0x1;
}

static int GetKeyFrame(ANIMINFO *info,int seq,int transframe)
{
	ANIMSEQHEADER *seqheader;
	ANIMKEY *keys;
	int numkeys,i;
	F32 keyframe,keyf;
	int keyflags;
	
	seqheader = info->seqheader + seq;
	numkeys = seqheader->keys;
	keys = info->keys + seqheader->keyidx;
	keyflags = info->seqheader->control & CTL_BIGKEYS;

	keyf = transframe * (1.0/30.0);
	
	for (i=0; i<numkeys; i++)
	{
		keyframe = GetKey(keyflags, i, keys);		
		if (keyframe >= keyf)
			break;
	}
	
	if (i < numkeys)
		return i;
	else
		return 0;
}

int InitAnim(ANIMINFO *info,int seq,int transframe,F32 transtime,int active)
{
 	ANIMSEQHEADER *seqheader;
	S16 numkeys;
	S16 rpt;
	int keyf=0;
	S32 oldkeyflag;
	ANIMKEY *oldkeys;
	
	info->atime = ATime;
	
	/* Get oldkeyflag and oldkeys from old (current) seq */
	seqheader = info->seqheader + info->animseq;
	oldkeyflag = seqheader->control & CTL_BIGKEYS ? 1 : 0;
	oldkeys = info->keys + seqheader->keyidx;

	/* Set seqheader to new seq & get numkeys in new seq */
	seqheader = info->seqheader + seq;
	numkeys = seqheader->keys;
	rpt = seqheader->control & 0x3;

	info->active = active;
	if (seq >= info->numseqs)
		return 0;

	if (transframe)	/* (transtime) */
	{
		keyf = GetKeyFrame(info,seq,transframe) + 1;
		/* info->starttime is set in AnimateTree to ATime - 1/30 sec */
		info->transtime = info->starttime + transtime;

		if (keyf > numkeys)
			FATALERRORF((ANIMERROR,"Init Anim Failed: SEQ= %d  %d > %d",
						 seq,keyf,numkeys));
		info->transkey = keyf;
		info->setpanim |= 0x1;
		info->idx0 = info->idx1;
		info->idx1 = 0;
		info->frac = 0.0;
		info->animseq = seq;
		info->animseq0 = info->animseq;
	}
	else
	{
		info->starttime = info->atime;
		info->transtime = 0.0;
		info->transkey = 0;
		info->setpanim |= 0x1;
		info->idx0 = 0;
		info->idx1 = 0;
		info->frac = 0.0;
		info->animseq = seq;
		info->animseq0 = seq;
	}

	if (ANIMDEBUG2 && ANIMDEBUGINFO(info))
		xyprintfc(ANIMCOLOR,0,ANIMLINE+5,"  INIT key %.0f(%.0f)/%.0f start %.3f trans %.3f at %.3f rpt %.0f  ",
				 (F32)keyf,(F32)transframe,(F32)numkeys-1,
				 info->starttime,transtime,info->atime,(F32)rpt);

	info->stage &= ~0xFFF;
	info->prevkey = 0;
	info->repeat = rpt;
	return 1;
}

void FreezeAnim(ANIMINFO *info)
{
	/* Use info->atime as a temporary holder */
	info->atime = ATime - info->starttime; 
}

void UnFreezeAnim(ANIMINFO *info)
{
	info->starttime = ATime - info->atime;;
}

int AnimateTree(ANIMINFO *info, ANIMOBJ *root,int nextseq, int frame, F32 time,
				int interrupt, int update)
{
	int dotrans = 0;
	S16 stage,curseq;
	int animres=0;
	int done;
	int control;
	
#if 1
	if (ANIMDEBUG2 && ANIMDEBUGINFO(info)) {
		XYPRINTF((ANIMCOLOR,0,ANIMLINE+2, " NO ANIMATION PROCESSED [%d]    ",nextseq));
		XYPRINTF((ANIMCOLOR,50,ANIMLINE+2,"NO ANIM   "));
		XYPRINTF((ANIMCOLOR,50,ANIMLINE+3,"NO RPT  "));
	}
#endif

	animres = CalcAnimInfo(info);

	stage = info->stage;
	curseq = info->animseq;
	done = ((info->stage & 0xFF) == 0xFF) ? 1 : 0;
	
	if (nextseq >= 0 && (done || (interrupt & 0xF)))
	{
		int initres = 0;
		
		if (!done || info->starttime < (info->atime - (1.0/30.0)))
			info->starttime = info->atime - (1.0/30.0);

		if (ANIMDEBUG2 && ANIMDEBUGINFO(info))
			xyprintfc(ANIMCOLOR,0,ANIMLINE+4,"  NEWSEQ:%.0f FRAME:%.0f TIME %.3f START:%.3f  ",
					  (F32)nextseq,(F32)frame,time,info->starttime);

		initres = InitAnim(info,nextseq,frame,time,1);
		
		if (initres <= 0)
			FATALERRORF((ANIMERROR,"Init Anim Failed: %d NEXT: %d MAX: %d",
						 initres,nextseq,info->numseqs));
		control = ((info->seqheader)+curseq)->control;
#if 1
		if((control & (CTL_FIXPOS)) && (!(interrupt & 4)) ) {
			AnimFixPos(root->child,root,info);
			dotrans |= 8;
		}
#endif
		dotrans |= 1;
	}

	if ((dotrans&1) || animres == 0xF)
	{
		animres = CalcAnimInfo(info);
		if (animres == 0xF)
			animres = CalcAnimInfo(info);
	}

	if (dotrans && (done || ((info->stage & 0xFF) == 0xFF)))
		dotrans |= 2;
	
#if 1
	if (ANIMDEBUG1 && ANIMDEBUGINFO(info))
	{
		control = (info->seqheader + info->animseq)->control;
		xyprintfc(ANIMCOLOR,0,ANIMLINE,"ACTV:%d SEQ:%d STG:%x RPT:%d CTL:0x%x FP:%d SC:%.2f SP:%d  ",
				 info->active,info->animseq,info->stage,info->repeat,
				 control,(control & (CTL_FIXPOS))?1:0,info->scale,info->setpanim);
		xyprintfc(ANIMCOLOR,0,ANIMLINE+1,
				  "STARTIME:%06.2f ATime:%06.2f IDX0:%02.0f IDX1:%02.0f FRAC:%04.2f ",
				 info->starttime,info->atime,(F32)info->idx0,(F32)info->idx1,info->frac);
	}
#endif

	if (update >= 0)
		AnimateObj(root,info,update);
	else
		ZOID_UpdateObjectMat(root->dlidx);

	return dotrans;
}
/******************** END ANIMATION SEQUENCER ***********************/


/***********************************/
/* ANIMOBJ INITIALIZATION ROUTINES */
/***********************************/

void InitAnimObjs()
{
	int i;
	for (i=0; i<MAXANIMOBJS; i++) {
		AnimObjs[i].mbidx = -1;
		AnimObjs[i].dlidx = -1;
		AnimObjs[i].mat = 0;
	}
	NumAnimObjs = 0;
	AnimTimeScale = ANIMTIMESCALE;
}

void SetAnimObjParent(ANIMOBJ *child, ANIMOBJ *parent)
{
	ANIMOBJ *link;

	if (child == parent)
		FATALERRORF((ANIMERROR,"SetAnimObjParent with same objs (idx=%ld)",
					 child->objnum));
	else if (!child || !parent)
		FATALERRORF((ANIMERROR,"SetAnimObjParent with NULL obj (chld=%lx par=%lx)",
					 (U32)child,(U32)parent));

	if (parent->child)
	{
		link = parent->child;
		while (link->next)
			link = link->next;
		link->next = child;
	}
	else
		parent->child = child;
	child->parent = parent;
}

/* NOTE: Assumes animobj->parent is set correctly
   (should be if parenting is done through functions below) */
static void DeParentAnimObj(ANIMOBJ *animobj)
{
	ANIMOBJ *parent = animobj->parent;
	ANIMOBJ *next = animobj->next;
	ANIMOBJ *child;
	
	if (!parent)
		return;
			
	if (parent->child == animobj) {
		parent->child = next;
		animobj->next = 0;
	}
	else if (parent->child) {
		for (child=parent->child; child; child = child->next) {
			if (child->next == animobj) {
				child->next = next;
				animobj->next = 0;
				break;
			}
		}
	}
	animobj->parent = 0;
}

/* NOTE: Assumes animobj->parent is set correctly
   (should be if parenting is done through functions below) */
void ReParentAnimObj(ANIMOBJ *child, ANIMOBJ *newparent)
{
	DeParentAnimObj(child);
	
	SetAnimObjParent(child, newparent);
	ZOID_SetObjectParent( child->dlidx, newparent->dlidx, 0x1 );
}


/* rootdlidx is the dlidx of the parent of the parent
   (which might not be an animobj and therefore can not be retrieved
   with animobj->parent->parent). Use -1 if animobj->parent
   has no parent. */
void RemoveAnimObjParent(ANIMOBJ *animobj, S16 parentdlidx)
{
	ANIMOBJ *parent;
	MATRIX mat;

	parent = animobj->parent;
	if (!parent)
		return;
#if 1
	MulMat(parent->mat, animobj->mat, &mat);
	*animobj->mat = mat;
#else
	AddVector(parent->POSP,animobj->POSP,animobj->POSP);
#endif
	DeParentAnimObj(animobj);
	ZOID_SetObjectParent(animobj->dlidx,parentdlidx,1);
	ZOID_UpdateObjectMat(animobj->dlidx);
}

/* Removes an anim obj. If a parent exists, links to obj are removed.
   Any children or neighbors are unaffected (and may become 'orphans') */
void RemoveAnimObj(ANIMOBJ *obj)
{
	ANIMOBJ *parent, *next;
	if(obj) {
		DeParentAnimObj(obj);
		MBOX_RemoveObject(obj->dlidx);
		obj->dlidx = -1;
		obj->mbidx = -1;
	}
	else
		ERRORF((GENERIC,"REMOVE ANIM OBJECT ERROR"));
}

/* Remove animobj without deparenting */
static void RemoveAnimTreeSub(ANIMOBJ *obj)
{
	if (obj->child)
		RemoveAnimTreeSub(obj->child);
	if (obj->next)
		RemoveAnimTreeSub(obj->next);
	MBOX_RemoveObject(obj->dlidx);
	obj->dlidx = -1;
	obj->mbidx = -1;
	obj->parent = 0;
	obj->child = 0;
	obj->next = 0;
}
/* Remove all children (no need to deparent them so use RemoveAnimTreeSub)
 * Remove and deparent the root.
 * NOTE: Does not remive the root's neighbors. */
void RemoveAnimTree(ANIMOBJ *root)
{
	if (!root)
		ERRORF((GENERIC,"REMOVE ANIM TREE ERROR"));
	if (root->child)
		RemoveAnimTreeSub(root->child);
	RemoveAnimObj(root); /* Deparent root */
}

void AddAnimObj(ANIMOBJ *obj, S32 parentidx, U32 dlflags)
{
	if(obj)
		obj->dlidx = MBOX_NewObjectMat(obj->mbidx,(F32**)(&obj->mat),parentidx,dlflags);
	else
		ERRORF((GENERIC,"ADD ANIM OBJECT ERROR"));
}

void AddAnimTree(ANIMOBJ *obj, ANIMOBJ *parent, U32 dlflags,F32 *parpos)
{
	S16 dlidx,i;
	F32 tpos[3];

	for (i=0; i<3; i++)
		tpos[i] = obj->initpos[i];
	
	if (parent)
	{
		dlidx = parent->dlidx;
		if (parpos)
		{
			for (i=0; i<3; i++)
			{
				obj->initpos[i] -= parpos[i];
			}
		}
	}
	else
		dlidx = -1;
	
	AddAnimObj(obj, dlidx, dlflags);

	if (obj->next)
		AddAnimTree(obj->next, parent, dlflags, parpos);
	if (obj->child)
		AddAnimTree(obj->child, obj, dlflags, tpos);
}

void ResetAnimObj(ANIMOBJ *obj)
{
	int j;

	if (obj->mat) {
		for (j=0; j<3; j++)
		{
			obj->POSP[j] = 0.0;
			obj->ppos[j] = 0.0;
			obj->tpos[j] = 0.0;
			obj->pyrq[j] = 0.0;
			obj->tpyrq[j] = 0.0;
		}
		CopyUV(unituvs,obj->UVSP);
	}
	obj->pyrq[3] = 1.0;
	obj->tpyrq[3] = 1.0;
}

void ResetAnimTree(ANIMOBJ *obj)
{
	if (obj) {
		ResetAnimObj(obj);
		if (obj->child)
			ResetAnimTree(obj->child);
		if (obj->next)
			ResetAnimTree(obj->next);
	}
}

void ZmodAnimTree(ANIMOBJ *obj, S32 zmod)
{
	if (obj) {
		ZOID_ZmodObject(obj->dlidx, zmod);
		if (obj->child)
			ZmodAnimTree(obj->child, zmod);
		if (obj->next)
			ZmodAnimTree(obj->next, zmod);
	}
}

ANIMOBJ *NewAnimObj(char *objname,S16 objnum,ANIMHEADER *header,int model_num)
{
	int j,idx;
	ANIMOBJ *new;

	for (idx=0; idx<NumAnimObjs; idx++)
		if (AnimObjs[idx].mbidx < 0)
			break;
	if (idx == NumAnimObjs) {
		if (NumAnimObjs >= MAXANIMOBJS) {
			ERRORF((GENERIC,"NEW ANIM OBJ ERROR"));
			return 0;
		}
		NumAnimObjs++;
	}	
			
	new = &AnimObjs[idx];
#if HOCKEY
	new->mbidx = MBOX_FindObject_Err(objname,MBOX_WARN);
#else
	new->mbidx = MBOX_FindObject_Sub(objname, model_num, model_num, MBOX_NOERR);
#endif
	if (new->mbidx < 0)
		new->dlidx = 0;
	new->next = 0;
	new->child = 0;
	new->parent = 0;
	
	if (objnum >= 0 && header)
		new->seq = header->seqs + (objnum * header->numseqs);
	else
		new->seq = 0;
	new->pidx = -1;
	new->objnum = objnum;
	new->objinfo = 0;
	for (j=0; j<3; j++)
		new->initpos[j] = 0.0;
	ResetAnimObj(new);
	return new;
}

void InsertAnimObj(ANIMOBJ *animobj, ANIMHEADER *header, S32 idx)
{
	int j;
	animobj->seq = header->seqs + (idx * header->numseqs);
	animobj->pidx = -1;
}	

/* This function builds the tree of ANIMOBJS from:
   parts: 	{F32 pos[3]; char desc[32]; char parent[32]; char objname[32];}
     A structure which defines the tree structure. Objname is used to get
     the mbox dlidx in NewAnimObj.
   header: the animation header structure associated with this tree.
   */
ANIMOBJ *BuildAnimTree(PARTDESC *parts,ANIMHEADER *header,int model_num)
{
	int i,j;
	ANIMOBJ *root;
	S16 parent;
	S16 numjoints;
	int objidx;

	root = NewAnimObj(NULLOBJ,-1,header,-1);

	numjoints = 0;
	for (i=0; i<NEWANIMOBJMAX; i++)
	{
		if (parts[i].desc[0] == 0)
			break;
		objidx = GetAnimObjIdx(header,parts[i].desc);
		NewAnimobjs[i] = NewAnimObj(parts[i].objname,objidx,header,model_num);

		for (j=0; j<3; j++)
		{
			NewAnimobjs[i]->initpos[j] = parts[i].pos[j];
		}
		numjoints++;
	}
	
	root->child = 0;
	root->parent = 0;
	for (i=0; i<numjoints; i++)
	{
		parent = GetPartIdx(parts,parts[i].parent);
		if (parent >= 0)
			SetAnimObjParent(NewAnimobjs[i],NewAnimobjs[parent]);
		else if (root->child)
			FATALERRORF((ANIMERROR,"BuildAnimTree: >1 NULL parents (idx=%ld)",
						 NewAnimobjs[i]->objnum));
		else
			root->child = NewAnimobjs[i];
	}

	return root;
}


/*******************************/
/* ANIMHEADER UTILITY ROUTINES */
/*******************************/


void SetupAnimHeader(U32 addr,ANIMHEADER *new)
{
	struct animheader *header = (struct animheader *)addr;

	if (!addr || !new)
		return;
	
#if 0
	int line = 10;
	
	XYPRINTF((ANIMCOLOR,1,line++,"BLOCKS:  0x%x",header->blocks));
	XYPRINTF((ANIMCOLOR,1,line++,"SEQS:    0x%x",header->seqs));
	XYPRINTF((ANIMCOLOR,1,line++,"SEQHDR:  0x%x",header->seqheader));
	XYPRINTF((ANIMCOLOR,1,line++,"KEYS:    0x%x",header->keys));
	XYPRINTF((ANIMCOLOR,1,line++,"DESCS:   0x%x",header->objdescs));

	XYPRINTF((ANIMCOLOR,1,line++,"NUMSEQS:   %d",header->numseqs));
	XYPRINTF((ANIMCOLOR,1,line++,"NUMOBJS:   %d",header->numobjs));
	XYPRINTF((ANIMCOLOR,1,line++,"NUMJOINTS: %d",header->numjoints));
	XYPRINTF((ANIMCOLOR,1,line++,"NUMSITES:  %d",header->numsites));	
#endif

	new->CompressAng = (F32 *)((U32)(header->CompressAng) + addr);
	new->CompressPos = (F32 *)((U32)(header->CompressPos) + addr);
	new->CompressUnit = (F32 *)((U32)(header->CompressUnit) + addr);
	new->blocks = 	(ANIM *)((U32)(header->blocks) + addr);
	new->seqs = 	(ANIMSEQINFO *)((U32)(header->seqs) + addr);
	new->seqheader =(ANIMSEQHEADER *)((U32)(header->seqheader) + addr);
	new->keys = 	(ANIMKEY *)((U32)(header->keys) + addr);
	new->objdescs = (ANIMDESC *)((U32)(header->objdescs) + addr);
	new->numseqs =	header->numseqs;
	new->numobjs =	header->numobjs;
	new->numjoints=	header->numjoints;
	new->numsites =	header->numsites;

#if 0
	{
		int i,cnt,ctl,idx;
		ANIMSEQHEADER *seqh;

		line = 4;
		XYPRINTF((ANIMCOLOR,1,line++,"ANIM KEYS: (0x%x)",(U32)new->seqheader));	
		seqh = new->seqheader;
		for (i=0; i<new->numseqs; i++)
		{
			cnt = seqh->keys;
			ctl = seqh->control;
			idx = seqh->keyidx;
			XYPRINTF((ANIMCOLOR,1,line++,"%d: [%d 0x%x %d]",i,cnt,ctl,idx));
			seqh++;
		}
	}
#endif
}

	
int GetAnimObjIdxErr(ANIMHEADER *header,const char *name, int err)
{
	int i;
	ANIMDESC *desc;

	if (!header)
		return 0;
	
	desc = header->objdescs;
	
	for (i=0; i<header->numobjs; i++)
	{
		if (strncmp(name,desc->desc,32) == 0)
			return i;
		desc++;
	}
	if (strncmp(name,"NULL",4) != 0 && name[0]) {
		if (err == 2)
			FATALERRORF((ANIMERROR,"GetAnimObjIdx Failed: \"%s\" (%ld)",
						 name,header->numobjs));
		else if (err == 1)
			ERRORF((ANIMERROR,"GetAnimObjIdx Failed: \"%s\" (%ld)",
					name,header->numobjs));
	}
	return -1;
}

int GetAnimObjIdx(ANIMHEADER *header,const char *name)
{
	return GetAnimObjIdxErr(header,name,1);
}

char *GetAnimObjName(ANIMHEADER *header,int idx)
{
	int i;
	ANIMDESC *desc;
	static char AnimString[74];

	if (!header)
		return 0;
	
/*	XYPRINTF((ANIMCOLOR,1,6,"GetAnimObjName idx = %d header = 0x%x",idx,(S32)header)); */
	desc = header->objdescs;

	if (idx < header->numobjs && idx >= 0)
		return (desc + idx)->desc;
	else
	{
		nsprintf(AnimString,74,"Bad AnimObjName Idx: %d",idx);
		return AnimString;
	}
}

int GetPartIdx(PARTDESC *parts,const char *name)
{
	int i;
	

	for (i=0; parts[i].desc[0] != 0; i++)
	{
		if (strncmp(name,parts[i].desc,32) == 0)
			return i;
	}
	if (strncmp(name,"NULL",4) != 0 && name[0])
		FATALERRORF((ANIMERROR,"GetPartIdx Failed: \"%s\" (%d)",name,i));
	return -1;
}

ANIMOBJ *AnimTreeObjIgnorePre(ANIMHEADER *header,ANIMOBJ *root, const char *name, int presize)
{
	ANIMOBJ *result;
	char *desc;

	desc = GetAnimObjName(header,root->objnum);
	if (strcmp(desc+presize, name) == 0)
		return root;
	if (root->next) {
		result = AnimTreeObjIgnorePre(header,root->next,name,presize);
		if (result)
			return result;
	}
	if (root->child) {
		result = AnimTreeObjIgnorePre(header,root->child,name,presize);
		if (result)
			return result;
	}
	return 0;
}

ANIMOBJ *AnimTreeObj(ANIMHEADER *header,ANIMOBJ *root, const char *name)
{
	return AnimTreeObjIgnorePre(header,root,name,0);
}

void InitAnimInfo(ANIMHEADER *header,ANIMINFO *info,int mirror)
{
	info->header = header;
	if (header)
	{
		info->blocks = header->blocks;
		info->seqheader = header->seqheader;
		info->keys = header->keys;
		info->numseqs = header->numseqs;
		info->CompressAng = header->CompressAng;
		info->CompressPos = header->CompressPos;
		info->CompressUnit = header->CompressUnit;
	}
	else
	{
		info->blocks = 0;
		info->seqheader = 0;
		info->keys = 0;
		info->numseqs = 1;
		info->CompressAng = 0;
		info->CompressPos = 0;
		info->CompressUnit = 0;
	}
	info->active = 0;
	info->animseq0 = 0;
	info->animseq = 0;
	info->transkey = 0;
	info->stage = 0;
	info->prevkey = 0;
	info->starttime = 0.0;
	info->transtime = 0.0;
	info->frac = 0.0;
	info->idx0 = 0;
	info->idx1 = 0;
	info->setpanim = 0x1;
	info->repeat = 0;
	info->scale = 1.0;
	info->mirror = mirror;
	
	if (header)
		InitAnim(info,0,0,0.0,1);
}

