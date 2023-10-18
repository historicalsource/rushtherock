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
** $Revision: 2 $ 
** $Date: 6/15/96 11:34a $ 
**
** Utility routines for SST-1 Initialization code
**
*/
#pragma optimize ("",off)
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sst.h>
#define FX_DLL_DEFINITION
#include <fxdll.h>
#include <sst1vid.h>
#include <sst1init.h>

/*
** sst1InitIdle():
**  Return idle condition of SST-1
**
**    Returns:
**      FXTRUE if SST-1 is idle (fifos are empty, graphics engines are idle)
**      FXFALSE if SST-1 has not been mapped
**
*/
FX_EXPORT FxBool FX_CSTYLE sst1InitIdle(FxU32 *sstbase)
{
    if(!sstbase)
        return(FXFALSE);

    if(!sst1InitSliEnabled)
        sst1InitIdleLoop(sstbase);
    else {
        FxU32 j, n;

        if(sst1InitCheckBoard(sstbase) == FXFALSE)
            return(FXFALSE);
        /* Check idle for Master... */
        sst1InitIdleLoop(sstbase);

        /* Cause slave to drive PCI bus */
        PCICFG_RD(SST1_PCI_INIT_ENABLE, j);
        PCICFG_WR(SST1_PCI_INIT_ENABLE, j | SST_SCANLINE_SLV_OWNPCI);
        if(sst1InitCheckBoard(sst1InitSliSlaveVirtAddr) == FXFALSE)
            return(FXFALSE);
        PCICFG_RD(SST1_PCI_INIT_ENABLE, j);
        PCICFG_WR(SST1_PCI_INIT_ENABLE, j | SST_SCANLINE_SLV_OWNPCI);

        /* Check idle for Slave... */
        sst1InitIdleLoop(sstbase);

        /* Restore normal SLI conditions */
        PCICFG_RD(SST1_PCI_INIT_ENABLE, j);
        PCICFG_WR(SST1_PCI_INIT_ENABLE, j & ~SST_SCANLINE_SLV_OWNPCI);
        if(sst1InitCheckBoard(sstbase) == FXFALSE)
            return(FXFALSE);
        PCICFG_RD(SST1_PCI_INIT_ENABLE, j);
        PCICFG_WR(SST1_PCI_INIT_ENABLE, j & ~SST_SCANLINE_SLV_OWNPCI);
    }
    return(FXTRUE);
}

void sst1InitIdleLoop(FxU32 *sstbase)
{
    FxU32 cntr;
    volatile Sstregs *sst = (Sstregs *) sstbase;

    sst->nopCMD = 0x0;
    cntr = 0;
    while(1) {
        if(!(sst1InitReturnStatus(sstbase) & SST_BUSY)) {
            if(++cntr > 3)
                break;
        } else
            cntr = 0;
    }
}

/*
** sst1InitIdleFBI():
**  Return idle condition of FBI (ignoring idle status of TMU)
**
**    Returns:
**      FXTRUE if FBI is idle (fifos are empty, graphics engines are idle)
**      FXFALSE if FBI has not been mapped
**
*/
FX_EXPORT FxBool FX_CSTYLE sst1InitIdleFBI(FxU32 *sstbase)
{
    FxU32 cntr;
    volatile Sstregs *sst = (Sstregs *) sstbase;

    if(!sst)
        return(FXFALSE);

    sst->nopCMD = 0x0;
    cntr = 0;
    while(1) {
        if(!(sst1InitReturnStatus(sstbase) & SST_FBI_BUSY)) {
            if(++cntr > 3)
                break;
        } else
            cntr = 0;
    }
    return(FXTRUE);
}

/*
** sst1InitIdleFBINoNOP():
**  Return idle condition of FBI (ignoring idle status of TMU)
**  sst1InitIdleFBINoNOP() differs from sst1InitIdleFBI() in that no NOP command
**  is issued to flush the graphics pipeline.
**
**    Returns:
**      FXTRUE if FBI is idle (fifos are empty, graphics engines are idle)
**      FXFALSE if FBI has not been mapped
**
*/
FX_EXPORT FxBool FX_CSTYLE sst1InitIdleFBINoNOP(FxU32 *sstbase)
{
    FxU32 cntr;
    volatile Sstregs *sst = (Sstregs *) sstbase;

    if(!sst)
        return(FXFALSE);

    /* sst->nopCMD = 0x0; */
    cntr = 0;
    while(1) {
        if(!(sst1InitReturnStatus(sstbase) & SST_FBI_BUSY)) {
            if(++cntr > 5)
                break;
        } else
            cntr = 0;
    }
    return(FXTRUE);
}

/* Included so compiler doesn't optimize out loop code waiting on status bits */
FX_EXPORT FxU32 FX_CSTYLE sst1InitReturnStatus(FxU32 *sstbase)
{
    volatile Sstregs *sst = (Sstregs *) sstbase;

    return(sst->status);
}


/*
** sst1InitClearSwapPending():
**  Clear any swaps pending in the status register
**  NOTE: The video unit of FBI must be initialized before calling this routine
**
**    Returns:
**      FXTRUE
**
*/
FX_ENTRY FxBool FX_CALL sst1InitClearSwapPending(FxU32 *sstbase)
{
    volatile Sstregs *sst = (Sstregs *) sstbase;
    FxU32 displayedBuffer, i;

    INIT_PRINTF(("sst1InitClearSwapPending() WARNING: Clearing pending swapbufferCMDs...\n"));

    sst1InitIdle(sstbase);
    displayedBuffer =
        (sst->status & SST_DISPLAYED_BUFFER) >> SST_DISPLAYED_BUFFER_SHIFT;

    /* Wait until vsync is inactive to guarantee that swaps queue in the */
    /* PCI fifo properly */
    while(!(sst->status & SST_VRETRACE) || ((sst->vRetrace & 0xfff) > 100) ||
        ((sst->vRetrace & 0xfff) < 10))
        ;

    /* First swap syncs to Vsync...Subsequent ones do not... */
    sst->swapbufferCMD = 0x1;
    sst->nopCMD = 0x0;
    for(i=0; i<17; i++) {
        sst->swapbufferCMD = 0x0;
        sst->nopCMD = 0x0;
    }
    if(displayedBuffer) {
        sst->swapbufferCMD = 0x0;
        sst->nopCMD = 0x0;
    }
    sst1InitIdle(sstbase);

    return(FXTRUE);
}

/*
** sst1InitVgaPassCtrl():
**  Control VGA passthrough setting
**
**
*/
FX_EXPORT FxBool FX_CSTYLE sst1InitVgaPassCtrl(FxU32 *sstbase, FxU32 enable)
{
    volatile Sstregs *sst = (Sstregs *) sstbase;

    if(sst1InitCheckBoard(sstbase) == FXFALSE)
        return(FXFALSE);

    if(enable) {
        /* VGA controls monitor */
        sst->fbiInit0 = (sst->fbiInit0 & ~SST_EN_VGA_PASSTHRU) | 
            sst1CurrentBoard->vgaPassthruEnable;
        sst->fbiInit1 |= SST_VIDEO_BLANK_EN;
    } else {
        /* SST-1 controls monitor */
        sst->fbiInit0 = (sst->fbiInit0 & ~SST_EN_VGA_PASSTHRU) | 
            sst1CurrentBoard->vgaPassthruDisable;
        sst->fbiInit1 &= ~SST_VIDEO_BLANK_EN;
    }

    return(FXTRUE);
}

/*
** sst1InitResetTmus():
**  Reset TMUs after changing graphics clocks
**  Occasionally when changing the frequency of the graphics clock, the TMUs
**  get in an unknown state.  sst1InitResetTmus() cleans up the problem.
**
*/
FxBool sst1InitResetTmus(FxU32 *sstbase)
{
    volatile Sstregs *sst = (Sstregs *) sstbase;
    FxU32 n;

    /* Clear FBI registers */
    sst->fbzColorPath = 0x0;
    sst->fogMode = 0x0;
    sst->alphaMode = 0x0;
    sst->fbzMode = 0x0;
    sst->lfbMode = 0x0;
    sst->fogColor = 0x0;
    sst->zaColor = 0x0;
    sst->chromaKey = 0x0;
    sst->stipple = 0x0;
    sst->c0 = 0x0;
    sst->c1 = 0x0;

    /* clear TMU registers */
    sst->textureMode = 0x0;
    sst->tLOD = 0x0;
    sst->tDetail = 0x0;
    sst->texBaseAddr = 0x0;
    sst->texBaseAddr1 = 0x0;
    sst->texBaseAddr2 = 0x0;
    sst->texBaseAddr38 = 0x0;

    /* Set downstream TMU to intentionally overflow TT Fifo... */
    SST_TREX(sst,0)->trexInit1 = sst1CurrentBoard->tmuInit1[0] &
        ~SST_TEX_TT_FIFO_SIL;
    sst1InitIdleFBINoNOP(sstbase);

    /* Draw 256-pixel textured triangle to overflow TT fifo in downstream */
    /* TMU.  Create numerous page misses in downstream TMU so upstream TMU */
    /* is always running faster */
    sst->fbzColorPath = SST_ENTEXTUREMAP;
    sst->fbzMode = SST_DRAWBUFFER_FRONT;
    sst->vA.x = 0;
    sst->vA.y = 0;
    sst->vB.x = (25<<SST_XY_FRACBITS);
    sst->vB.y = 0;
    sst->vC.x = 0;
    sst->vC.y = (25<<SST_XY_FRACBITS);
    sst->s = 0x0;
    sst->t = 0x0;
    sst->w = 0x0;
    sst->dwdx = 0x0;
    sst->dwdy = 0x0;
    SST_TREX(sst,0)->dsdx = (69<<SST_ST_FRACBITS);
    SST_TREX(sst,0)->dsdy = (69<<SST_ST_FRACBITS);
    SST_TREX(sst,0)->dsdx = (69<<SST_ST_FRACBITS);
    SST_TREX(sst,0)->dsdy = (69<<SST_ST_FRACBITS);
    SST_TREX(sst,1)->dsdx = (0<<SST_ST_FRACBITS);
    SST_TREX(sst,1)->dsdy = (0<<SST_ST_FRACBITS);
    SST_TREX(sst,1)->dsdx = (0<<SST_ST_FRACBITS);
    SST_TREX(sst,1)->dsdy = (0<<SST_ST_FRACBITS);
    sst->triangleCMD = 0x0;
    sst->nopCMD = 0x1;  /* This will reset pixel counter registers... */

    /* Wait for command to execute... */
    for(n=0; n<25000; n++)
        sst1InitReturnStatus(sstbase);

    /* Upstream TMU is now idle */
    /* Set downstream TMU to always accept upstream data */
    /* This will flush pending pixels in the downstream TMU */
    SST_TREX(sst,0)->trexInit1 = sst1CurrentBoard->tmuInit1[0] |
        SST_TEX_RG_TTCII_INH | SST_TEX_USE_RG_TTCII_INH;
    for(n=0; n<100; n++)
        sst1InitReturnStatus(sstbase);

    /* Wait for command to execute... */
    for(n=0; n<25000; n++)
        sst1InitReturnStatus(sstbase);

    /* Restore registers */
    SST_TREX(sst,0)->trexInit1 = sst1CurrentBoard->tmuInit1[0];
    sst->fbzColorPath = 0x0;
    sst->fbzMode = 0x0;
    for(n=0; n<100; n++)
        sst1InitReturnStatus(sstbase);

    if(sst1InitReturnStatus(sstbase) & SST_TREX_BUSY) {
        INIT_PRINTF(("sst1InitResetTmus(): Could not reset TMUs...\n"));
        return(FXFALSE);
    } else
        return(FXTRUE);
}

#pragma optimize ("",on)
