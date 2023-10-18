#include <stdlib.h>
#include <string.h>
#include "stdtypes.h"
#include "strings.h"
#include "blit.h"
#include "error.h"
#include "mb.h"
#include "mb_blit.h"

#define MAXOVERLAYBLITS MBBLIT_MAX
#define MAXBLITS	MAXOVERLAYBLITS

Blit	Blits[MAXBLITS];
Blit	*BlitPtrs[MAXBLITS];
int		NumBlits;

static GrowInfo	grow;

/*********************************************************************
 *********************************************************************/
void InitBlits(void)
{
	int		i;

	for(i=0; i < MAXBLITS;i++) BlitPtrs[i] = Blits+i;
	NumBlits = 0;
	memset(&grow,0,sizeof(GrowInfo));
}

/*********************************************************************
 *********************************************************************/
void RemoveBlit(Blit *dbp)
{
	int i;

	if (!dbp) return;
	for(i=0;i<NumBlits;i++)
	{
		if(dbp == BlitPtrs[i])
			break; 
	}

	if((i == NumBlits) || (NumBlits <= 0)) {
		Errorf(GENERIC,"REMOVE BLIT, NumBlits = %ld", NumBlits);
		return;
	}
	/**** Now remove from moveable list ****/
	ZOID_RemoveBlit(dbp->BLIdx);
	NumBlits--;
	BlitPtrs[i] = BlitPtrs[NumBlits];
	BlitPtrs[NumBlits] = dbp;

	if(dbp->child)
		RemoveBlit(dbp->child);
}

/*********************************************************************
 *********************************************************************/
void DrawBlits(void)
{
	int i;
	Blit	*blit;

	for(i=0;i<NumBlits;i++)
	{
		blit = BlitPtrs[i];
		if (blit->AnimFunc)
		{
			if (!blit->AnimFunc(blit))
			{
				RemoveBlit(blit);
				i--;
				continue;
			}
		}
		if (blit->Hide) continue;
#if 0
		put_poly(&blit->Polygon);
#endif
	}
}

/*********************************************************************
 *********************************************************************/
void InitBlit(Blit *blit)
{
	TEXDEF	*ti;

	ti = MBOX_FindTexture_Err(blit->Name,&blit->TexIndex,MBOX_WARN);
#if 0 /* Fatal errors suck! */
	if (!ti) FatalMsg("Can't find texture: %s",blit->Name);
#endif
	blit->Width 	= -1; /* ti->Width; */
	blit->Height	= -1; /* ti->Height; */
	blit->Z			= 0;
	blit->Alpha		= 0;
	blit->Info		= ti;
	blit->Top = -1;
	blit->Bot = -1;
	blit->Left = -1;
	blit->Right = -1;
	blit->color = 0x7FFF;
}

/*********************************************************************
 *********************************************************************/
void UpdateBlit(Blit *blit)
{
	extern int NumBlits;
	ZOID_UpdateBlit(blit->BLIdx,blit->TexIndex,
					blit->X,blit->Y,blit->Width,blit->Height);
	ZOID_CropBlit(blit->BLIdx, blit->Top, blit->Bot, blit->Left, blit->Right, blit->color);
	ZOID_SetBlitData(blit->BLIdx,blit->Hide,blit->Alpha,blit->Z);
	ZOID_FlipBlit(blit->BLIdx,blit->Flip);
#if 0
	FatalMsg("%s %d %d X%d Y%d %d %d %d",blit->Name,
			 blit->BLIdx,blit->Info->Index,
			 blit->X,383-blit->Y,blit->Width,blit->Height,NumBlits);
#endif
	
#if 0
	init_rect(&blit->Polygon,blit->X,383-blit->Y,blit->Width,blit->Height,
			  blit->Info->Addr/1024,blit->Info->Width,blit->Info->Height,blit->Alpha);
#endif
}

/*********************************************************************
 *********************************************************************/
Blit *NewBlit(const char *name,int x,int y)
{
	Blit	*blit;

	if(NumBlits >= MAXBLITS)
	{
		FatalError(GENERIC,"NewBlit - table full");
		return(NULL);
	}

	blit = BlitPtrs[NumBlits];
	NumBlits++;

	blit->Name		= name;
	blit->X			= x;
	blit->Y			= y;
	blit->Hide		= 0;
	blit->Flip		= 0;
	blit->Init		= 1;
	blit->child		= (Blit *)0;
	blit->AnimFunc	= 0;
	blit->AnimID	= -1;
	blit->AnimDTA	= -1;
	blit->data		= (void *)0;
	InitBlit(blit);
	blit->BLIdx		= ZOID_AddBlit(blit->TexIndex,x,y,-1,-1);
	UpdateBlit(blit);
	return(blit);
}

/*********************************************************************
 *********************************************************************/
Blit *NewBlitUnscaled(const char *name,int x,int y)
{
Blit	*blit;

	blit = NewBlit(name,x,y);
	blit->Height = (blit->Info->Height * 3) >> 1;
	UpdateBlit(blit);

	return blit;
}

/*********************************************************************
 *********************************************************************/
void RenameBlit(Blit *blit,const char *name,int preserve)
{
	TEXDEF *ti;
	
	blit->Name = (char *)name;
	if(preserve) {
		if(!(ti = MBOX_FindTexture(blit->Name,&blit->TexIndex)))
			FatalMsg("Can't find texture: %s",blit->Name);
		blit->Info		= ti;
	} else
		InitBlit(blit);
	UpdateBlit(blit);
}


/* ----------------------------------------------------------------------------- *
 *	Given a blit, selects a subsection of that blit based on index.
 *	hSize && vSize are sizes of the subsections.
 *	It's assumed that as rows of subsections are filled as much as possible, and
 *	index 0 is the first index, and sections are filled across, then down.
 *	Works with flipped blits too.
 * ----------------------------------------------------------------------------- */
void SelectBlit( BLIT *blt, int index, int hSize, int vSize )
{
	int row, col, secPerRow;

	if (!blt)
		return;

	/* Determine row and column */
	/*	Error check the data and try to relocate if there's a problem. */
	if (!blt->Info || !blt->Info->Width)
		{
		RenameBlit( blt, blt->Name, 1 );
		if (!blt->Info)
			{
			Errorf(4,"SelectBlit(%s) bad blit, blit:%p info:%p",blt->Name,blt,blt->Info);
			return;
			}
		}
	secPerRow	= blt->Info->Width / hSize;
	if (!secPerRow)
		{
		Errorf(4,"SelectBlit(%s) bad size, req:%ld actual:%ld", blt->Name,
			  (long)hSize,(long)blt->Info->Width);
		return;
		}
	row 		= index / secPerRow;
	col 		= index % secPerRow;

	/*	Set the texture coordinates. */
	blt->Width	= hSize;
	blt->Height = vSize;
	blt->Top	= blt->Info->Height - row*vSize - 1;
	blt->Bot	= blt->Top - vSize + 1;
	if (blt->Flip)
		{
		col			= secPerRow - col - 1;
		blt->Left	= (blt->Info->Width % hSize) + col * hSize;
		blt->Right	= blt->Left + hSize - 1;
		}
	else
		{
		blt->Left	= col * hSize;
		blt->Right	= blt->Left + hSize - 1;
		}
	UpdateBlit(blt);

}	/* end SelectBlit */


/*!*******************************************************************
 *********************************************************************/
GrowInfo *InitGrowMessage(const char *name,int x,int y,int width,int height,
					 int stretch,int hold,int shrink,int blink)
{
	if (grow.Blit) RemoveBlit(grow.Blit);
	if (!stretch) stretch = 1;
	grow.Timer		= 0;
	grow.X			= x;
	grow.Y			= y;
	grow.Width		= width;
	grow.Height		= height;
	grow.Stretch	= stretch;
	grow.Hold		= hold;
	grow.Shrink		= shrink;
	grow.Blink		= blink;
	grow.Blit		= NewBlit(name,x,y);
	return &grow;
}

/*!*******************************************************************
 *********************************************************************/
int GrowMessage()
{
	int		t;

	if (grow.Timer < grow.Stretch + grow.Hold)
	{
		t = MIN(grow.Timer,grow.Stretch);
		grow.Blit->Width = t * grow.Width / grow.Stretch;
		grow.Blit->Height = t * grow.Height / grow.Stretch;
		if (grow.Timer > grow.Stretch && grow.Blink)
			grow.Blit->Hide = (grow.Timer >> 2) & 1;
	}
	else
	{
		t = grow.Shrink - (grow.Timer - (grow.Stretch + grow.Hold));
		grow.Blit->Width = t * grow.Width / grow.Shrink;
		grow.Blit->Height = t * grow.Height / grow.Shrink;
	}
	grow.Blit->X = grow.X + (grow.Width - grow.Blit->Width) / 2;
	grow.Blit->Y = grow.Y + (grow.Height - grow.Blit->Height) / 2;
	UpdateBlit(grow.Blit);

	if (++grow.Timer >= grow.Stretch + grow.Hold + grow.Shrink)
	{
		RemoveBlit(grow.Blit);
		grow.Blit = 0;
		return(0);
	}
	return(grow.Timer);
}

/*!*******************************************************************
 *********************************************************************/
int	IncGrowMessage(int amount)
{
	grow.Timer += amount;
	if (grow.Timer >= grow.Stretch + grow.Hold + grow.Shrink)
	{
		RemoveBlit(grow.Blit);
		grow.Blit = 0;
		return(0);
	}
	return(grow.Timer);
}

/*!*******************************************************************
 *********************************************************************/
void	RemoveGrowMessage()
{
	RemoveBlit(grow.Blit);
	grow.Blit = 0;
}

/**** NewMultiBlit(ulx,uly,mblit,nblits) :
  ulx = upper left hand x coordinate to translate all blits by.
  uly = upper left hand y coordinate to translate all blits by.
  mblit = pointer to a list of blit data.
****/
BLIT *NewMultiBlit(S16 ulx, S16 uly, const MULTIBLIT *mblit, S16 nblits) {
	BLIT *rootblit,*lastblit,*curblit;
	int i;

	if(nblits <= 0)
		return((BLIT *)0);

	rootblit = (BLIT *)0;
	lastblit = (BLIT *)0;
	
	for(i=0;i<nblits;i++) {
		curblit = NewBlit(mblit[i].texname,ulx+mblit[i].dulx,uly+mblit[i].duly);
		curblit->Alpha = mblit[i].alpha;
 		curblit->Z = mblit[i].zdepth;

		curblit->Left = mblit[i].left;
		curblit->Top = mblit[i].top;
		curblit->Right = mblit[i].right;
		curblit->Bot = mblit[i].bot;
		if(mblit[i].width >= 0)
			curblit->Width = mblit[i].width;
		if(mblit[i].height >= 0)
			curblit->Height = mblit[i].height;
		UpdateBlit(curblit);
#if (VIS_V_PIX == 256)
		YScaleBlit(curblit, mblit[i].noyscale);
#endif
		
		if(mblit[i].animfunc) {
			curblit->AnimFunc = mblit[i].animfunc;
			curblit->AnimID = mblit[i].animid;
			if(!curblit->AnimFunc(curblit)) {
				RemoveBlit(curblit);
				return(rootblit);
			}
		}

		if(i == 0)
			rootblit = curblit;
		else
			lastblit->child = curblit;
		
		lastblit = curblit;
	}

	return(rootblit);
}

void MultiBlitMove(BLIT *blt, S16 dx, S16 dy) {
	if(!blt)
		return;
	
	blt->X += dx;
	blt->Y += dy;
	UpdateBlit(blt);
	MultiBlitMove(blt->child,dx,dy);
}

void MultiBlitHide(BLIT *blt) {
	BLIT *curblt;

	curblt = blt;	
	while(curblt) {
		curblt->Hide = 1;
		UpdateBlit(curblt);
		if(curblt->child)
			curblt = curblt->child;
	}	
}

void AttatchBlit(BLIT *parent, BLIT *child) {
	while(parent->child)
		parent = parent->child;
	parent->child = child;
}

void ReduceBlit(BLIT *blit, F32 xamt, F32 yamt) {
	S16 width,height;
		
	if (blit->Width < 0)
		width = blit->Info->Width;
	else
		width = blit->Width;
	width = (S16)((F32)width * xamt + .5);
	
	if (blit->Height < 0)
		height = blit->Info->Height;
	else
		height = blit->Height;
	height = (S16)((F32)height * yamt + .5);
	
	blit->Width 	= width;
	blit->Height	= height;
}
	
void YScaleBlit(BLIT *blit, int noyscale) {
#if (VIS_V_PIX == 256)
	if (noyscale&2) {
		if (blit->Height < 0)
			blit->Height = blit->Info->Height;
		blit->Height *= (2.0/3.0);
		UpdateBlit(blit);
	}
#endif
	ZOID_YScaleBlit(blit->BLIdx, noyscale);
}

void YScaleBlits(BLIT *blit, int noyscale) {
	while(blit) {
		YScaleBlit(blit, noyscale);
		blit = blit->child;
	}
}
