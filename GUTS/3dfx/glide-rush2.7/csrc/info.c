/*
** Copyright (c) 1996, 3Dfx Interactive, Inc.
** All Rights Reserved.
**
** This is UNPUBLISHED PROPRIETARY SOURCE CODE of 3Dfx Interactive, Inc.;
** the contents of this file may not be disclosed to third parties, copied or
** duplicated in any form, in whole or in part, without the prior written
** permission of 3Dfx Interactive, Inc.
**
** RESTRICTED RIGHTS LEGEND:
** Use, duplication or disclosure by the Government is subject to restrictions
** as set forth in subdivision (c)(1)(ii) of the Rights in Technical Data
** and Computer Software clause at DFARS 252.227-7013, and/or in similar or
** successor clauses in the FAR, DOD or NASA FAR Supplement. Unpublished  -
** rights reserved under the Copyright Laws of the United States.
**
**
** $Revision: 18 $ 
** $Date: 8/15/96 10:12a $ 
**
** Routines to detect memory size, strapping pin, and other initialization
** configuration information.
**
*/
#pragma optimize ("",off)
#include <stdio.h>
#include <stdlib.h>
#include <sst.h>
#define FX_DLL_DEFINITION
#include <fxdll.h>
#include <sst1vid.h>
#include <sst1init.h>

#define XY_ONE (1<<SST_XY_FRACBITS)
#ifdef SET
#undef SET
#endif
#define SET(d,s) d=s

static FxBool 
readAndSum4x4(FxU32 *sstbase, FxU32 x, FxU32 y, 
    		FxU32 *r_sum, FxU32 *g_sum, FxU32 *b_sum)
{
    FxU32 rd_x, rd_y;
    FxU32 rd_col;
    FxU32 rd_r, rd_g, rd_b;
    volatile Sstregs *sst = (Sstregs *) sstbase;

    /* wait for idle board */
    sst->lfbMode = SST_LFB_RGBALANES_ARGB | SST_LFB_READFRONTBUFFER;
    sst1InitIdle(sstbase);
    if (x & 1) {
	INIT_PRINTF(("ERROR: readAndSum4x4 must have an even X (%d)\n", x));
	return(FXFALSE);
    }

    /* get 16 pixels (4 x 4 array) from frame buffer and sum the colors */
    *r_sum = 0;
    *g_sum = 0;
    *b_sum = 0;

    for (rd_y = 0; rd_y < 4; rd_y++) {		/* read 4 scanlines */
	for (rd_x = 0; rd_x < 4; rd_x ++) {
	    if ((rd_x & 1)==0) {		/* read 2 pixels at a time */
		rd_col = sstbase[(SST_LFB_ADDR + (y+rd_y)*2048 + (x+rd_x)*2) >> 2];
	    }
	    else rd_col >>= 16;
	    rd_r = ((rd_col >> 11) & 0x1f) << 3;
	    rd_g = ((rd_col >>  5) & 0x3f) << 2;
	    rd_b = ((rd_col >>  0) & 0x1f) << 3;
	    *r_sum += rd_r;
	    *g_sum += rd_g;
	    *b_sum += rd_b;
	    INIT_INFO((4,"%d,%d = rd_col: 0x%04x   rgb: %02x %02x %02x\n",
		    rd_x, rd_y, (rd_col & 0xffff), rd_r, rd_g, rd_b));
	}
    }
    INIT_INFO((3,"sums:  r_sum=0x%03x  g_sum=0x%03x  b_sum=0x%03x\n",
		    *r_sum, *g_sum, *b_sum));
	return(FXTRUE);
}

/* xxx - Give these guys some meaningful comments */
static FxI32 rb_tbl[0xFFF+1];
static FxI32 g_tbl[0xFFF+1];

/* draw a right angle triangle */
static void
drawTriangle(volatile Sstregs *sst, int x, int y, int tSize)
{
    SET(sst->vA.x,x);
    SET(sst->vA.y,y);
    SET(sst->vB.x,x+XY_ONE*tSize);
    SET(sst->vB.y,y);
    SET(sst->vC.x,x);
    SET(sst->vC.y,y+XY_ONE*tSize);
    SET(sst->s,0);
    SET(sst->t,0);
    SET(sst->w,0);
    SET(sst->dsdx,1<<SST_ST_FRACBITS);
    SET(sst->dtdx,0);
    SET(sst->dwdx,0);
    SET(sst->dsdy,0);
    SET(sst->dtdy,1<<SST_ST_FRACBITS);
    SET(sst->dwdy,0);
    SET(sst->triangleCMD,0);
}

static FxBool
initSumTables(FxU32 *sstbase)
{
    int x=0,y=0;
    FxU32 tst_color;
    FxU32 r_sum, g_sum, b_sum;
    volatile Sstregs *sst = (Sstregs *) sstbase;

    /* init sum array */
    for (r_sum = 0; r_sum <= 0xfff; r_sum++) {
	rb_tbl[r_sum] = -1;
	g_tbl[r_sum] = -1;
    }

    sst->fbzColorPath = SST_RGBSEL_C1 | SST_CC_PASS;
    sst->fbzMode = SST_DRAWBUFFER_FRONT | SST_RGBWRMASK | SST_ENDITHER;

    /* fill sum array */
    for (tst_color = 0; tst_color <= 255; tst_color++) {
	INIT_INFO((2,"tst_color=0x%02x\n", tst_color));
	sst->c1 = (tst_color << 16) | (tst_color << 8) | tst_color;

	drawTriangle(sst, x,y,36);
	if(readAndSum4x4(sstbase, x,y, &r_sum,&g_sum,&b_sum) == FXFALSE)
		return(FXFALSE);

	/* check sums for uniqueness and then store away */
	if (r_sum != b_sum) {
	    INIT_PRINTF(("ERROR:  b_sum=0x%03x  r_sum=0x%03x\n", r_sum, b_sum));
		return(FXFALSE);
	}
	if (rb_tbl[r_sum] != -1) {
	    INIT_PRINTF(("ERROR:  non-unique r/b_sum=0x%03x\n", r_sum));
		return(FXFALSE);
	}
	rb_tbl[r_sum] = tst_color;
	if (g_tbl[g_sum] != -1) {
	    INIT_PRINTF(("ERROR:  non-unique g_sum=0x%03x\n", g_sum));
		return(FXFALSE);
	}
	g_tbl[g_sum] = tst_color;
    }
	return(FXTRUE);
}

/* remove dither to derive actual 24-bit RGB value */
static FxBool
unDither(FxU32 r_sum, FxU32 g_sum, FxU32 b_sum, FxU32 *result)
{
    if (rb_tbl[r_sum] == -1 || g_tbl[g_sum] == -1 || rb_tbl[b_sum] == -1)
    {
	INIT_PRINTF(("ERROR: unDither: invalid color sum\n"));
	return(FXFALSE);
    }
    *result = (rb_tbl[r_sum] << 16) | (g_tbl[g_sum] << 8) | rb_tbl[b_sum];
	return(FXTRUE);
}

static FxBool
getTmuConfigData(FxU32 *sstbase, sst1DeviceInfoStruct *info)
{
    int x=0, y=0;
    FxU32 r_sum, g_sum, b_sum;
    volatile Sstregs *sst = (Sstregs *) sstbase;

    /* set trex's (all 3) to output configuration bits */
    SET(SST_TREX(sst,0)->trexInit1, info->tmuInit1[0] | (1 << 18));
    SET(SST_TREX(sst,1)->trexInit1, info->tmuInit1[1] | (1 << 18));
    SET(SST_TREX(sst,2)->trexInit1, info->tmuInit1[2] | (1 << 18));

    /* render into the frame buffer */
    sst->fbzColorPath = SST_RGBSEL_TREXOUT | SST_CC_PASS | SST_ENTEXTUREMAP;
    sst->texBaseAddr = 0;
    sst->textureMode = SST_AI88 | SST_TC_PASS | SST_TCA_PASS;
    sst->tLOD = 0;
    drawTriangle(sst,x,y,36);

    readAndSum4x4(sstbase, x,y, &r_sum,&g_sum,&b_sum);
    if(unDither(r_sum,g_sum,b_sum,&info->tmuConfig) == FXFALSE)
		return(FXFALSE);
	if(GETENV(("SST_TMUCFG")))
        SSCANF(GETENV(("SST_TMUCFG")), "%i", &info->tmuConfig);

    /* reset trex's init registers */
    SET(SST_TREX(sst,0)->trexInit1, info->tmuInit1[0]);
    SET(SST_TREX(sst,1)->trexInit1, info->tmuInit1[1]);
    SET(SST_TREX(sst,2)->trexInit1, info->tmuInit1[2]);

	return(FXTRUE);
}

#define SENSE2 0x92F56EB0
#define SENSE1 0xF2A916B5
#define SENSE0 0xBADBEEF1

static FxU32 sense(FxU32 *sstbase, sst1DeviceInfoStruct *info, FxU32 tmu,
		   FxU32 mem, FxU32 init)
{
    volatile Sstregs *sst = (Sstregs *) sstbase;
    FxU32 *texAddr = (tmu<<(21-2)) + (FxU32 *)SST_TEX_ADDRESS(sst);

    /* set the Init0 register to enable ? MBytes of memory */
    sst1InitIdle(sstbase);
    SST_TREX(sst,tmu)->trexInit0 = init | (info->tmuInit0[tmu] & ~0x7000);
    sst1InitIdle(sstbase);

    sst->texBaseAddr = 0x200000>>3;	/* set to 2 MB */
    texAddr[0] = SENSE2;		/* write a random value */
    sst->texBaseAddr = 0x100000>>3;	/* set to 1 MB */
    texAddr[0] = SENSE1;		/* write a random value */
    sst->texBaseAddr = 0x000000>>3;	/* set to 0 MB */
    texAddr[0] = SENSE0;		/* write a random value */

    sst->texBaseAddr = mem>>3;		/* reset to 2 MB */
    drawTriangle(sst,0,0,4);		/* draw a 4x4 right triangle */
    sst1InitIdle(sstbase);

    mem = sstbase[SST_LFB_ADDR>>2];
    INIT_INFO((2,"data=0x%08x\n", mem));

    /* reset the Init0 register back to its previous value */
    sst1InitIdle(sstbase);
    SST_TREX(sst,tmu)->trexInit0 = info->tmuInit0[tmu];
    sst1InitIdle(sstbase);
    return mem;
}

FX_EXPORT FxBool FX_CSTYLE
sst1InitGetTmuMemory(FxU32 *sstbase, sst1DeviceInfoStruct *info, FxU32 tmu,
	FxU32 *TmuMemorySize)
{
    FxU32 i,data;
    volatile Sstregs *sst = (Sstregs *) sstbase;

    INIT_INFO((1,"sst1InitGetTmuMemory(0x%x, , %d)\n", sstbase,tmu));

	if(GETENV(("SST_TMUMEM_SIZE"))) {
		*TmuMemorySize = ATOI(GETENV(("SST_TMUMEM_SIZE")));
		return(FXTRUE);
	}

    sst->lfbMode = SST_LFB_RGBALANES_ARGB | SST_LFB_READFRONTBUFFER;
    sst->fbzMode = SST_DRAWBUFFER_FRONT | SST_RGBWRMASK;
    sst->fbzColorPath = SST_RGBSEL_TREXOUT | SST_CC_PASS | SST_ENTEXTUREMAP;
    sst->textureMode = SST_RGB565 | SST_TC_REPLACE | SST_TCA_REPLACE;
    sst->tLOD = 0;

    /* setup all downstream TMUs to be in pass-thru mode */
    for (i=0; i<tmu; i++)
	SST_TREX(sst,i)->textureMode = SST_TC_PASS | SST_TCA_PASS;

    /* first see if we have 4 Mbytes by writing a texel at 2MB followed by */
    /* a texel at 1MB and 0MB and then rendering using the texel at 2MB */
    /* if we have less memory it should not render correctly since we trash */
    /* the memory locations it would wrap to */
    data = sense(sstbase,info,tmu,0x200000, 0x5000);
    if (data == SENSE2) {*TmuMemorySize = 4; return(FXTRUE);}

    /* set the Init0 register to enable 2 MBytes of memory and repeat test */
    data = sense(sstbase,info,tmu,0x100000, 0x2000);
    if (data == SENSE1) {*TmuMemorySize = 2; return(FXTRUE);}

    /* set the Init0 register to enable 1 MBytes of memory and repeat test */
    data = sense(sstbase,info,tmu,0x000000, 0x2000);
    if (data == SENSE0) {*TmuMemorySize = 1; return(FXTRUE);}

    INIT_PRINTF(("sst1InitGetTmuMemory() ERROR: Could not detect memory size.\n"));
	return(FXFALSE);
}

/*--------------------------------------------------------------------------- */
/* NOTES: */
/*	assumes that board and registers are initialized */
/*	destroys part of the framebuffer */
/*--------------------------------------------------------------------------- */
FX_EXPORT FxBool FX_CSTYLE
sst1InitGetTmuInfo(FxU32 *sstbase, sst1DeviceInfoStruct *info)
{
    FxU32 trev;

    if(initSumTables(sstbase) == FXFALSE)
		return(FXFALSE);
    if(getTmuConfigData(sstbase,info) == FXFALSE)
		return(FXFALSE);

	/* TMU memory speed */
	/* tmuClkFixed is no longer used... */
	info->tmuClkFixed = 0x0;
	/* tmuMemSpeed is legacy, and is not used by either Obsidian GE or Pro Fab */
	info->tmuMemSpeed = 0;

    /* count the TMUs and verify that all TMUs are the same revision */
    info->tmuRevision = info->tmuConfig & 0x7;
    info->numberTmus = 1;
    if(sst1InitGetTmuMemory(sstbase, info, 0, &info->tmuMemSize[0]) == FXFALSE)
		return(FXFALSE);
    INIT_INFO((1,"TMU0 memory = %d MB\n", info->tmuMemSize[0]));
    if (info->tmuConfig & FXBIT(6)) {		/* if TMU 1 exists */
	info->numberTmus++;			/* increment TMU count */
	trev = (info->tmuConfig>>7) & 0x7;	/* get its revision */
	if (info->tmuRevision != trev) {
	    INIT_PRINTF(("sst1InitGetDeviceInfo: ERROR, multiple different TMU revision IDs detected\n"));
	    return(FXFALSE);
	}
    if(sst1InitGetTmuMemory(sstbase, info, 1, &info->tmuMemSize[1]) == FXFALSE)
		return(FXFALSE);
    }
    if (info->tmuConfig & FXBIT(13)) {		/* if TMU 2 exists */
	info->numberTmus++;			/* increment TMU count */
	trev = (info->tmuConfig>>14) & 0x7;	/* get its revision */
	if (info->tmuRevision != trev) {
	    INIT_PRINTF(("sst1InitGetDeviceInfo: ERROR, multiple different TMU revision IDs detected\n"));
	    return(FXFALSE);
	}
    if(sst1InitGetTmuMemory(sstbase, info, 2, &info->tmuMemSize[2]) == FXFALSE)
		return(FXFALSE);
    }
    INIT_INFO((1,"numberTMus = %d\n", info->numberTmus));
    return(FXTRUE);
}

/*
** fbiMemSize():
**  Returns size (in MBytes) of FBI frame buffer memory
**  Returns 0 on error
**  NOTE: fbiMemSize() destroys the contents in memory
**
*/
#define LFB_PUTPIXEL(X, Y, DATA)	\
	lfbptr[((SST_LFB_ADDR+(X<<1)+(Y<<11))>>1)] = DATA
#define LFB_GETPIXEL(X, Y)			\
	lfbptr[((SST_LFB_ADDR+(X<<1)+(Y<<11))>>1)]

static int fbiMemSize(FxU32 *sstbase)
{
	volatile Sstregs *sst = (Sstregs *) sstbase;
	volatile unsigned short *lfbptr = (unsigned short *) sstbase;
	FxU32 init0Save = sst->fbiInit0;
	FxU32 init1Save = sst->fbiInit1;
	FxU32 init2Save = sst->fbiInit2;
	int retval = 0;

	if(GETENV(("SST_FBIMEM_SIZE")))
		return(ATOI(GETENV(("SST_FBIMEM_SIZE"))));

	/* Enable dram refresh, disable memory fifo, and setup memory */
	/* for rendering */
	sst->fbiInit0 &= ~SST_MEM_FIFO_EN;
    sst->fbiInit2 |= SST_EN_DRAM_REFRESH;
	sst1InitIdleFBI(sstbase);

	/* Setup Basic rendering datapath */
	sst->fbzColorPath = SST_CC_MONE;
	sst->fogMode = 0x0;
	sst->fbzMode = SST_RGBWRMASK | SST_ZAWRMASK | SST_DRAWBUFFER_FRONT;
	sst1InitIdleFBI(sstbase);

	if(sst1InitSliDetect(sstbase))
		/* SLI cannot support 4 MBytes... */
		goto check2MByte;
    sst1InitSetResolution(sstbase, &SST_VREZ_800X600_60, 1);
	sst1InitIdleFBI(sstbase);

	sst->lfbMode = SST_LFB_ZZ | SST_LFB_WRITEFRONTBUFFER |
		SST_LFB_READDEPTHABUFFER;
	sst1InitIdleFBI(sstbase);

	/* Check for 4 MBytes... */
	/* Write to Zbuffer in 800x600 resolution in upper 2 MBytes of memory */
	LFB_PUTPIXEL(128, 100, 0xdead);	/* maps to row:0x216, col:0x80, bank:0x1 */
	LFB_PUTPIXEL(0, 0, 0x0);
	LFB_PUTPIXEL(798, 599, 0xffff);
	LFB_PUTPIXEL(200, 200, 0x55aa);	/* maps to row:0x23d, col:0x104, bank:0x0 */
	LFB_PUTPIXEL(20, 20, 0xffff);
	LFB_PUTPIXEL(400, 400, 0x0);
	sst1InitIdleFBI(sstbase);
	if((LFB_GETPIXEL(128, 100) == 0xdead) &&
	   (LFB_GETPIXEL(200, 200) == 0x55aa)) {
   		retval = 4;
		goto fbiMemSizeDone;
	}

check2MByte:
	/* Check for 2 MBytes... */
	/* Write to color buffer in 640x480 resolution */
    sst1InitSetResolution(sstbase, &SST_VREZ_640X480_60, 0);
	sst->lfbMode = SST_LFB_565 | SST_LFB_WRITEFRONTBUFFER |
		SST_LFB_READFRONTBUFFER;
	sst1InitIdleFBI(sstbase);
	LFB_PUTPIXEL(50, 100, 0xdead);	/* maps to row:0x1e, col:0x99, bank:0x0 */
	LFB_PUTPIXEL(0, 0, 0x0);
	LFB_PUTPIXEL(638, 479, 0xffff);
	sst->lfbMode = SST_LFB_565 | SST_LFB_WRITEBACKBUFFER |
		SST_LFB_READFRONTBUFFER;
	sst1InitIdleFBI(sstbase);
	LFB_PUTPIXEL(178, 436, 0xaa55);	/* maps to row:0x11e, col:0x99, bank:0x0 */
	LFB_PUTPIXEL(20, 20, 0x0);
	LFB_PUTPIXEL(400, 400, 0xffff);
	sst1InitIdleFBI(sstbase);
	if(LFB_GETPIXEL(50, 100) != 0xdead)
		goto check1MByte;
	sst->lfbMode = (SST_LFB_565 | SST_LFB_READBACKBUFFER);
	sst1InitIdleFBI(sstbase);
	if(LFB_GETPIXEL(178, 436) == 0xaa55) {
   		retval = 2;
		goto fbiMemSizeDone;
	}

check1MByte:
	sst->lfbMode = SST_LFB_565 | SST_LFB_WRITEFRONTBUFFER |
		SST_LFB_READFRONTBUFFER;
	sst1InitIdleFBI(sstbase);
	LFB_PUTPIXEL(10, 10, 0xdead);	/* maps to row:0x0, col:0x145, bank:0x0 */
	LFB_PUTPIXEL(8, 8, 0x0);
	LFB_PUTPIXEL(340, 340, 0xffff);
	LFB_PUTPIXEL(100, 200, 0x5a5a);	/* maps to row:0x3c, col:0x112, bank:0x1 */
	LFB_PUTPIXEL(66, 0, 0x0);
	LFB_PUTPIXEL(360, 360, 0xffff);
	sst1InitIdleFBI(sstbase);
	if((LFB_GETPIXEL(10, 10) == 0xdead) &&
	   (LFB_GETPIXEL(100, 200) == 0x5a5a))
   		retval = 1;

fbiMemSizeDone:
	/* Restore init registers to original state */
	sst->fbiInit0 = init0Save;
	sst->fbiInit1 = init1Save;
	sst->fbiInit2 = init2Save;
	sst1InitIdleFBI(sstbase);

	return(retval);
}

FX_EXPORT FxBool FX_CSTYLE
sst1InitGetFbiInfo(FxU32 *sstbase, sst1DeviceInfoStruct *info)
{
	volatile Sstregs *sst = (Sstregs *) sstbase;

    info->fbiMemSize = fbiMemSize(sstbase);

/* Detect board identification and memory speed */
#if 0
    if(GETENV(("SST_FBICFG")))
        SSCANF(GETENV(("SST_FBICFG")), "%i", &info->fbiConfig);
    else
#endif
	info->fbiConfig = (sst->fbiInit3 & SST_FBI_MEM_TYPE) >>
	    SST_FBI_MEM_TYPE_SHIFT;

    info->fbiBoardID = (info->fbiConfig >> 2) & 0x1;
    /* fbiMemSpeed is legacy, and is not used by either Obsidian GE or Pro Fab */
    info->fbiMemSpeed = 0;

    /* Detect scanline interleaving */
    info->sstSliDetect = sst1InitSliDetect(sstbase);

    return FXTRUE;
}

/*
** sst1InitGetDeviceInfo():
**  Read device specific information
**  NOTE: info pointer must point to an Info structure which has already
**        been allocated
**
*/
FX_EXPORT FxBool FX_CSTYLE sst1InitGetDeviceInfo(FxU32 *sstbase,
  sst1DeviceInfoStruct *info)
{
    FxBool retval;

    if((retval = sst1InitCheckBoard(sstbase)) == FXTRUE)
        *info = *sst1CurrentBoard;
    return(retval);
}

/*
** sst1InitFillDeviceInfo():
**  Fill in device information
**  NOTE: This routine destroys current contents in frame buffer memory
**
**
*/
FxBool sst1InitFillDeviceInfo(FxU32 *sstbase, sst1DeviceInfoStruct *info)
{
    if(!sstbase)
        return(FXFALSE);

    if(info->tmuRevision != 0xdead)
        return FXTRUE;  /* if already got it, return */

    if(GETENV(("SST_NODEVICEINFO"))) {
        /* fill device info struct with sane values... */
        info->tmuRevision = 0;
        info->numberTmus = 1;
        info->tmuConfig = 0x0;
        info->fbiMemSize = 2;
        info->tmuMemSize[0] = 2;
    } else {
        /* GetFbiInfo() must be called before GetTmuInfo() */
        if(sst1InitGetFbiInfo(sstbase, info) == FXFALSE)
            return(FXFALSE);
        /* get the revision ID of each TMU and verify that they are all the same */
        if(sst1InitGetTmuInfo(sstbase, info) == FXFALSE)
            return(FXFALSE);
    }
    INIT_PRINTF(("sst1DeviceInfo: Board ID: Obsidian %s\n",
        (info->fbiBoardID) ? "PRO" : "GE"));
    INIT_PRINTF(("sst1DeviceInfo: FbiConfig:0x%x, TmuConfig:0x%x\n",
        info->fbiConfig, info->tmuConfig));
    INIT_PRINTF(("sst1DeviceInfo: FBI Revision:%d, TMU Revison:%d, Num TMUs:%d\n",
        info->fbiRevision, info->tmuRevision, info->numberTmus));
    INIT_PRINTF(("sst1DeviceInfo: FBI Memory:%d, TMU[0] Memory:%d",
        info->fbiMemSize, info->tmuMemSize[0]));
    if(info->numberTmus > 1)
        INIT_PRINTF((", TMU[1] Memory:%d", info->tmuMemSize[1]));
    if(info->numberTmus > 2)
        INIT_PRINTF((", TMU[2] Memory:%d", info->tmuMemSize[2]));
    INIT_PRINTF(("\n"));
    if(sst1InitUseVoodooFile == FXTRUE) {
        if(iniDac == (sst1InitDacStruct *) NULL)
			INIT_PRINTF(("sst1DeviceInfo: Dac Type: Unknown"));
		else
	    	INIT_PRINTF(("sst1DeviceInfo: Dac Type: %s %s\n", 
			  iniDac->dacManufacturer, iniDac->dacDevice));
	} else {
	    INIT_PRINTF(("sst1DeviceInfo: Dac Type: "));
	    if(info->fbiDacType == SST_FBI_DACTYPE_ATT)
	        INIT_PRINTF(("AT&T ATT20C409\n"));
	    else if(info->fbiDacType == SST_FBI_DACTYPE_ICS)
	        INIT_PRINTF(("ICS ICS5342\n"));
	    else if(info->fbiDacType == SST_FBI_DACTYPE_TI)
	        INIT_PRINTF(("TI TVP3409\n"));
	    else
	        INIT_PRINTF(("Unknown\n"));
	}
    INIT_PRINTF(("sst1DeviceInfo: SliDetect:%d\n", info->sstSliDetect));

    return(FXTRUE);
}
#pragma optimize ("",on)
