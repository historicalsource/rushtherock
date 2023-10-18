#ifndef _HDRIVE_H
#define _HDRIVE_H

#include "stdtypes.h"
#include "fmath.h"
#include "mole_info.h"

#define FLAG1VAL 0x55A5A5AA
#define FLAG2VAL 0xAA5A5A55

typedef struct {
	U32 flag1;
	U32 dir_num_sect;
	U32 part_num_sect;
	U32 num_files;
	U32 next_free_lsn; /* Excluding gaps (only files of track len => track alligned)*/
	U32 free_sectors;  /* Including gaps */
	U32 largest_gap;
	U32 flag2;
} DirHdr;

typedef struct {
	U32 start_lsn;
	U32 num_sect; /* max */
	U32 num_bytes; /* DIRFLAG => subdirectory */
	U8  fname[MAXFNAMELEN]; /* null terminated (unless all 20 used?) */
} DirFile;

typedef union {
	DirHdr   hdr;
	DirFile  file;
} DirEntry;

extern U16 NumSec;
extern U16 NumHead;
extern U16 NumCyl;
extern U32 SecsPerCyl;
extern U32 NumLSN;
extern U32 LongsPerTrack;

#define DIRENTRIESPERSEC (BYTESPERSEC/sizeof(DirEntry)) /* 512/(8*4) == 16 */

#define LSN2TRACK(x)	((x) / NumSec)
#define LSN2CYL(x)		((x) / SecsPerCyl)
#define LSN2HEAD(x)		(LSN2TRACK(x) % NumHead)
#define LSN2SEC(x)		((x) % NumSec + 1)

#define CURTIME (ZCLOCK)

#define HDAllocFileERR  		10
#define HDAllocMakeDirERR     	20
#define HDDirInfoERR 	   		30
#define HDDirLSNERR 	   		40
#define HDMakeDirAfterAllocERR 	50
#define HDMakeRootDirERR		60
#define HDFileInfoERR   		70	
#define HDFindCachedFileERR		80
#define HDFindFileERR	 		90
#define HDFindGapERR			100
#define HDFindSpaceERR			110
#define HDGetCacheERR			120
#define HDReadDirERR  			130
#define HDRemoveFileERR	 		140
#define HDRemoveDirERR  		150
#define HDStartERR       		160
#define HDWriteDirERR			170
#define HD_DirInfERR			180
#define HD_DirPtrERR			190
#define HD_FileInfERR			200
#define HD_GetLSNCacheERR		210
#define HD_ListDirERR			220
#define HD_MkDirERR				230
#define HD_MkFileERR			240
#define HD_RdFileERR			250
#define HD_RmDirERR				260
#define HD_RmFileERR			270
#define HD_RmFromLSNCacheERR	280
#define HD_WrFileERR			290 /*20 long*/
#define HDRemoveDirFilesERR		310
#define HD_RmDirFilesERR		320

#include "Pro/hdrive.pro"

#endif
