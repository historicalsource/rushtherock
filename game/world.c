/*****************************************************************************************
  RUSH World/Object Viewer :

  Copyright 1994,1995 Time Warner Interactive.  Unauthorized reproduction, adaptation,
  distribution, performance or display of this computer program or the associated
  audiovisual work is strictly prohibited.
  *****************************************************************************************/
#include <stdlib.h>
#include "globals.h"
#include "stdtypes.h"
#include "strings.h"
#include "error.h"
#include "mb.h"
#include "world.h"
#include "modeldat.h"
#include "drivsym.h"
#include "hdrive.h"

#include "Pro/initiali.pro"

#define SFX_FRONTFACE 		0x1
#define SFX_ANIM_TEXTURE 	0x2

typedef int (*FW_FUNCT)(const void *, const void *);


char 		WorldData[WORLD_BSIZE];	/* allocate space for the world data */

WOBJ		*WorldBase;		/* base node for the current world */
WOBJ		*Current;
S16 		Worlddlidx;
WORLDHDR * const WorldTable = (WORLDHDR *)WORLD_HDRS;

static S16 	dlindex;				/* return value from display list, pass to children */
static S32 	mbobj;

/* ----------------------------------------------------------------------------- */
static void NewWorldObject(WOBJ *Wnode, S16 Parent) 
{
    while(1) 	
		{
#if 1
		mbobj = MBOX_FindObject(Wnode->name);
		dlindex = MBOX_NewObject(mbobj,Wnode->objpos,Wnode->matrix,Parent,Wnode->dlflags | ZATR_SORTALPHA);
#else
		if(Wnode->sfx1)
			dlindex = NewWorldMoveable(Wnode,Parent);
		else 
			{
			mbobj = MBOX_FindObject(Wnode->name);
			dlindex = MBOX_NewObject(mbobj,Wnode->objpos,Wnode->matrix,Parent,Wnode->dlflags);
			}
#endif

		if (Worlddlidx==-1)
			Worlddlidx=dlindex;

		if(Wnode->child >= 0)
			NewWorldObject((WorldBase+Wnode->child),dlindex);
		
		if(Wnode->next >= 0)
			Wnode=WorldBase+Wnode->next;
		else
			break;
		}
}

/* ----------------------------------------------------------------------------- */
static int worldcmp(const WORLDHDR *awhdr, const WORLDHDR *bwhdr) 
{
	return(strcmp(awhdr->wname,bwhdr->wname));
}

/* ----------------------------------------------------------------------------- */
static WORLDHDR *FindWorld(const char *wstr) 
{
	WORLDHDR *ptr;
	WORLDHDR whdr;

	strcpy(whdr.wname,wstr);
	
	ptr = BSEARCH(&whdr, WorldTable, NWORLDS, sizeof(WORLDHDR),(FW_FUNCT)worldcmp);
	
	if(ptr == (WORLDHDR *)0)
		FatalErrorf(GENERIC,"FindWorld(\"%s\") Failed",wstr);
	
	return(ptr);
}

/* ----------------------------------------------------------------------------- */
void LoadWorld(void)
{
	WORLDHDR *whdr;

	/* LIB routine to load all world objects to MB. */
	/* Expects WorldData to point to start of world data. */
	Worlddlidx = -1;
	whdr = FindWorld("TEMP");
	WorldBase = (WOBJ *)(WORLD_DATA + whdr->woff);
	NewWorldObject(WorldBase,-1);

	modelinit();	/* initialize road surface (roaddat.rom) */
}


