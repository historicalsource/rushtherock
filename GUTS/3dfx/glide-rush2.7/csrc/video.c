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
** $Revision: 9 $ 
** $Date: 8/15/96 10:10a $ 
**
** Initialization code for initializing SST-1 video unit
**
*/
#pragma optimize ("",off)
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sst.h>
#define FX_DLL_DEFINITION
#include <fxdll.h>
#define SST1INIT_VIDEO_ALLOCATE /* allocate data structures for video timing */
#include <sst1vid.h>
#include <sst1init.h>

/*
** sst1InitVideo():
**  Initialize video (including DAC setup) for the specified resolution
**
**    Returns:
**      FXTRUE if successfully initializes specified SST-1 video resolution
**      FXFALSE if cannot initialize SST-1 to specified video resolution
**
*/
FX_EXPORT FxBool FX_CSTYLE sst1InitVideo(FxU32 *sstbase,
  GrScreenResolution_t screenResolution, GrScreenRefresh_t screenRefresh,
  const sst1VideoTimingStruct *altVideoTiming)
{
    FxU32 n, vtmp;
    volatile Sstregs *sst = (Sstregs *) sstbase;
    const sst1VideoTimingStruct *sstVideoRez;
    FxU32 sst1MonitorRefresh;
    FxU32 sst1MonitorRez;
    FxU32 video16BPP;
    FxU32 memFifoEntries;
    FxU32 memFifoLwm, memFifoHwm, pciFifoLwm;
    FxU32 vInClkDel, vOutClkDel;
    FxU32 tf0_clk_del, tf1_clk_del, tf2_clk_del;
    FxU32 ft_clk_del;

    if(!sst)
        return(FXFALSE);

    if(sst1InitCheckBoard(sstbase) == FXFALSE)
        return(FXFALSE);

    if(GETENV(("SST_IGNORE_INIT_VIDEO"))) {
        INIT_PRINTF(("WARNING: Ignoring sst1InitVideo()...\n"));
        sst1InitIdleFBINoNOP(sstbase);
        return(FXTRUE);
    }

    switch(screenResolution) {
        case(GR_RESOLUTION_336x240):
            sst1MonitorRez = 336240;
            sst1CurrentBoard->fbiVideoWidth = 336;
            sst1CurrentBoard->fbiVideoHeight = 240;
            break;
        case(GR_RESOLUTION_400x240):
            sst1MonitorRez = 400240;
            sst1CurrentBoard->fbiVideoWidth = 400;
            sst1CurrentBoard->fbiVideoHeight = 240;
            break;
        case(GR_RESOLUTION_512x256):
            sst1MonitorRez = 512256;
            sst1CurrentBoard->fbiVideoWidth = 512;
            sst1CurrentBoard->fbiVideoHeight = 256;
            break;
        case(GR_RESOLUTION_512x256x50i):
            sst1MonitorRez = 51225650;
            sst1CurrentBoard->fbiVideoWidth = 512;
            sst1CurrentBoard->fbiVideoHeight = 256;
            break;
        case(GR_RESOLUTION_512x384):
            sst1MonitorRez = 512;
            sst1CurrentBoard->fbiVideoWidth = 512;
            sst1CurrentBoard->fbiVideoHeight = 384;
            break;
        case(GR_RESOLUTION_640x400):
            sst1MonitorRez = 640400;
            sst1CurrentBoard->fbiVideoWidth = 640;
            sst1CurrentBoard->fbiVideoHeight = 400;
            break;
        case(GR_RESOLUTION_640x480):
            sst1MonitorRez = 640;
            sst1CurrentBoard->fbiVideoWidth = 640;
            sst1CurrentBoard->fbiVideoHeight = 480;
            break;
        case(GR_RESOLUTION_800x600):
            sst1MonitorRez = 800;
            sst1CurrentBoard->fbiVideoWidth = 800;
            sst1CurrentBoard->fbiVideoHeight = 600;
            break;
        case(GR_RESOLUTION_856x480):
            sst1MonitorRez = 856;
            sst1CurrentBoard->fbiVideoWidth = 856;
            sst1CurrentBoard->fbiVideoHeight = 480;
            break;
        case(GR_RESOLUTION_960x720):
            sst1MonitorRez = 960;
            sst1CurrentBoard->fbiVideoWidth = 960;
            sst1CurrentBoard->fbiVideoHeight = 720;
            break;
        default:
            INIT_PRINTF(("sst1InitVideo(): Unsupported Resolution...\n"));
            return(FXFALSE);
            break;
    }

    switch(screenRefresh) {
        case(GR_REFRESH_60Hz):
            sst1MonitorRefresh = 60;
            break;
        case(GR_REFRESH_70Hz):
            sst1MonitorRefresh = 70;
            break;
        case(GR_REFRESH_72Hz):
            sst1MonitorRefresh = 72;
            break;
        case(GR_REFRESH_75Hz):
            sst1MonitorRefresh = 75;
            break;
        case(GR_REFRESH_80Hz):
            sst1MonitorRefresh = 80;
            break;
        case(GR_REFRESH_85Hz):
            sst1MonitorRefresh = 85;
            break;
        case(GR_REFRESH_90Hz):
            sst1MonitorRefresh = 90;
            break;
        case(GR_REFRESH_100Hz):
            sst1MonitorRefresh = 90;
            break;
        case(GR_REFRESH_120Hz):
            sst1MonitorRefresh = 120;
            break;
        default:
            INIT_PRINTF(("sst1InitVideo(): Unsupported Refresh Rate...\n"));
            return(FXFALSE);
            break;
    }

    /* Override settings with environment variables */
    if(GETENV(("SST_SCREENREZ"))) {
        if(ATOI(GETENV(("SST_SCREENREZ"))) == 640) {
            sst1MonitorRez = 640;
            sst1CurrentBoard->fbiVideoWidth = 640;
            sst1CurrentBoard->fbiVideoHeight = 480;
        } else if(ATOI(GETENV(("SST_SCREENREZ"))) == 640400) {
            sst1MonitorRez = 640400;
            sst1CurrentBoard->fbiVideoWidth = 640;
            sst1CurrentBoard->fbiVideoHeight = 400;
        } else if(ATOI(GETENV(("SST_SCREENREZ"))) == 800) {
            sst1MonitorRez = 800;
            sst1CurrentBoard->fbiVideoWidth = 800;
            sst1CurrentBoard->fbiVideoHeight = 600;
        } else if(ATOI(GETENV(("SST_SCREENREZ"))) == 512256) {
            sst1MonitorRez = 512256;
            sst1CurrentBoard->fbiVideoWidth = 512;
            sst1CurrentBoard->fbiVideoHeight = 256;
        } else if(ATOI(GETENV(("SST_SCREENREZ"))) == 512) {
            sst1MonitorRez = 512;
            sst1CurrentBoard->fbiVideoWidth = 512;
            sst1CurrentBoard->fbiVideoHeight = 384;
        } else if(ATOI(GETENV(("SST_SCREENREZ"))) == 856) {
            sst1MonitorRez = 856;
            sst1CurrentBoard->fbiVideoWidth = 856;
            sst1CurrentBoard->fbiVideoHeight = 480;
        } else if(ATOI(GETENV(("SST_SCREENREZ"))) == 960) {
            sst1MonitorRez = 960;
            sst1CurrentBoard->fbiVideoWidth = 960;
            sst1CurrentBoard->fbiVideoHeight = 720;
        } else {
            sst1MonitorRez = 640;
            sst1CurrentBoard->fbiVideoWidth = 640;
            sst1CurrentBoard->fbiVideoHeight = 480;
        }
    }
    if(GETENV(("SST_SCREENREFRESH"))) {
        if(ATOI(GETENV(("SST_SCREENREFRESH"))) == 60)
            sst1MonitorRefresh = 60;
        else if(ATOI(GETENV(("SST_SCREENREFRESH"))) == 72)
            sst1MonitorRefresh = 72;
        else if(ATOI(GETENV(("SST_SCREENREFRESH"))) == 75)
            sst1MonitorRefresh = 75;
        else if(ATOI(GETENV(("SST_SCREENREFRESH"))) == 85)
            sst1MonitorRefresh = 85;
        else if(ATOI(GETENV(("SST_SCREENREFRESH"))) == 120)
            sst1MonitorRefresh = 120;
        else
            sst1MonitorRefresh = 60;
    }

    if(altVideoTiming && !GETENV(("SST_SCREENREZ")) &&
       !GETENV(("SST_SCREENREFRESH"))) {
        sstVideoRez = altVideoTiming;
        sst1CurrentBoard->fbiVideoRefresh = 60;
    } else {
        switch(sst1MonitorRez) {
            case(336240):
                sstVideoRez = &SST_VREZ_336X240_60;
                break;
            case(400240):
                sstVideoRez = &SST_VREZ_400X240_60;
                break;
            case(512256):
                sstVideoRez = &SST_VREZ_512X256_60;
                break;
            case(51225650):
                sstVideoRez = &SST_VREZ_512X256_60_50i;
                break;
            case(512):
                if(sst1MonitorRefresh == 120)
                    sstVideoRez = &SST_VREZ_512X384_120;
                else if(sst1MonitorRefresh == 85)
                    sstVideoRez = &SST_VREZ_512X384_85;
                else if(sst1MonitorRefresh == 75)
                    sstVideoRez = &SST_VREZ_512X384_75;
                else if(sst1MonitorRefresh == 60 /* && GETENV(("SST_ARCADE"))*/)
                    sstVideoRez = &SST_VREZ_512X384_60;
                else {
                    sst1MonitorRefresh = 72;
                    sstVideoRez = &SST_VREZ_512X384_72;
                }
                break;
            case(640400):
                if(sst1MonitorRefresh == 120)
                    sstVideoRez = &SST_VREZ_640X400_120;
                else if(sst1MonitorRefresh == 85)
                    sstVideoRez = &SST_VREZ_640X400_85;
                else if(sst1MonitorRefresh == 75)
                    sstVideoRez = &SST_VREZ_640X400_75;
                else {
                    sst1MonitorRefresh = 60;
                    sstVideoRez = &SST_VREZ_640X400_60;
                }
                break;
            case(640):
                if(sst1MonitorRefresh == 120)
                    sstVideoRez = &SST_VREZ_640X480_120;
                else if(sst1MonitorRefresh == 85)
                    sstVideoRez = &SST_VREZ_640X480_85;
                else if(sst1MonitorRefresh == 75)
                    sstVideoRez = &SST_VREZ_640X480_75;
                else {
                    sst1MonitorRefresh = 60;
                    sstVideoRez = &SST_VREZ_640X480_60;
                }
                break;
            case(800):
                if(sst1MonitorRefresh == 85)
                    sstVideoRez = &SST_VREZ_800X600_85;
                else if(sst1MonitorRefresh == 75)
                    sstVideoRez = &SST_VREZ_800X600_75;
                else {
                    sst1MonitorRefresh = 60;
                    sstVideoRez = &SST_VREZ_800X600_60;
                }
                break;
            case(856):
                sst1MonitorRefresh = 60;
                sstVideoRez = &SST_VREZ_856X480_60;
                break;
            case(960):
                sst1MonitorRefresh = 60;
                sstVideoRez = &SST_VREZ_960X720_60;
                break;
            default:
                INIT_PRINTF(("sst1InitVideo(): Unsupported Resolution %d...\n",
                    screenResolution));
                return(FXFALSE);
                break;
        }
        sst1CurrentBoard->fbiVideoRefresh = sst1MonitorRefresh;
    }

    if(GETENV(("SST_VIDEO_24BPP")))
        sst1CurrentBoard->fbiVideo16BPP =
            (ATOI(GETENV(("SST_VIDEO_24BPP")))) ^ 0x1;
    else {
        if(sst1CurrentBoard->fbiVideo16BPP && (sstVideoRez->video16BPPIsOK ==
           FXFALSE))
               sst1CurrentBoard->fbiVideo16BPP = 0;
        if(!sst1CurrentBoard->fbiVideo16BPP && (sstVideoRez->video24BPPIsOK ==
           FXFALSE))
               sst1CurrentBoard->fbiVideo16BPP = 1;

        if(sst1CurrentBoard->sstSliDetect)
            sst1CurrentBoard->fbiVideo16BPP = 1;

#if 0
        if(sst1CurrentBoard->fbiRevision == 2 &&
           (altVideoTiming == (sst1VideoTimingStruct *) NULL) &&
           (((sst1MonitorRez > 640) && (sst1MonitorRez != 512256) &&
            (sst1MonitorRez != 640400)) || (sst1MonitorRefresh > 85)))
            sst1CurrentBoard->fbiVideo16BPP = 1;
#else
        if((sst1MonitorRefresh > 85) ||
           (sst1MonitorRez == 800 && sst1MonitorRefresh > 60))
            sst1CurrentBoard->fbiVideo16BPP = 1;
#endif
    }
    video16BPP = sst1CurrentBoard->fbiVideo16BPP;

    /* Reset Video Refresh Unit */
    sst->fbiInit1 |= SST_VIDEO_RESET;
    
    /* Enable video clock */
    if(SLI_DRIVEOK()) {
        INIT_PRINTF(("sst1InitVideo(): Enabling Video Clock...\n"));
        PCICFG_WR(SST1_PCI_VCLK_ENABLE, 0x0);
    }

    /* Setup SST video timing registers */
    if(GETENV(("SST_HSYNC"))) {
        SSCANF(GETENV(("SST_HSYNC")), "%i", &vtmp);
        INIT_PRINTF(("sst1InitVideo(): Using SST_HSYNC=0x%x\n", vtmp));
        sst->hSync = vtmp;
    } else
        sst->hSync = ((sstVideoRez->hSyncOff << SST_VIDEO_HSYNC_OFF_SHIFT) |
                      (sstVideoRez->hSyncOn << SST_VIDEO_HSYNC_ON_SHIFT));
    if(GETENV(("SST_VSYNC"))) {
        SSCANF(GETENV(("SST_VSYNC")), "%i", &vtmp);
        INIT_PRINTF(("sst1InitVideo(): Using SST_VSYNC=0x%x\n", vtmp));
        sst->vSync = vtmp;
    } else
        sst->vSync = ((sstVideoRez->vSyncOff << SST_VIDEO_VSYNC_OFF_SHIFT) |
                      (sstVideoRez->vSyncOn << SST_VIDEO_VSYNC_ON_SHIFT));
    if(GETENV(("SST_BACKPORCH"))) {
        SSCANF(GETENV(("SST_BACKPORCH")), "%i", &vtmp);
        INIT_PRINTF(("sst1InitVideo(): Using SST_BACKPORCH=0x%x\n", vtmp));
        sst->backPorch = vtmp;
    } else
        sst->backPorch =
                     ((sstVideoRez->vBackPorch << SST_VIDEO_VBACKPORCH_SHIFT) |
                      (sstVideoRez->hBackPorch << SST_VIDEO_HBACKPORCH_SHIFT));
#if 0
    if(GETENV(("SST_DIMENSIONS"))) {
        SSCANF(GETENV(("SST_DIMENSIONS")), "%i", &vtmp);
        INIT_PRINTF(("sst1InitVideo(): Using SST_DIMENSIONS=0x%x\n", vtmp));
        sstVideoRez->yDimension = (vtmp >>  SST_VIDEO_YDIM_SHIFT) & 0x3ff;
        sstVideoRez->xDimension = vtmp & 0x3ff;
    }
#endif
    sst->videoDimensions =
                     ((sstVideoRez->yDimension << SST_VIDEO_YDIM_SHIFT) |
                      ((sstVideoRez->xDimension-1) << SST_VIDEO_XDIM_SHIFT));
#if 0
    if(GETENV(("SST_MEMOFFSET"))) {
        SSCANF(GETENV(("SST_MEMOFFSET")), "%i", &vtmp);
        INIT_PRINTF(("sst1InitVideo(): Using video memOffset=0x%x\n", vtmp));
        sstVideoRez->memOffset = vtmp;
    }
    if(GETENV(("SST_TILESINX"))) {
        SSCANF(GETENV(("SST_TILESINX")), "%i", &vtmp);
        INIT_PRINTF(("sst1InitVideo(): Using video tilesInX=0x%x\n", vtmp));
        sstVideoRez->tilesInX_Over2 = vtmp;
    }
#endif

    /* Setup SST memory mapper for desired resolution */
    if(sst1CurrentBoard->fbiMemSize == 4)
        sst1InitSetResolution(sstbase, sstVideoRez, 1); 
    else
        sst1InitSetResolution(sstbase, sstVideoRez, 0); 

    if(GETENV(("SST_TRIPLE_BUFFER"))) {
        INIT_PRINTF(("sst1InitVideo(): Enabling Triple Buffering\n"));
        sst->fbiInit2 |= SST_EN_TRIPLE_BUFFERING;
    }

    /* Calculate graphics clock frequency */
    if(sst1InitCalcGrxClk(sstbase) == FXFALSE)
        return(FXFALSE);

    /* Setup video fifo */
    /* NOTE: Lower values for the video fifo threshold improve video fifo */
    /* underflow problems */
    if(GETENV(("SST_VFIFO_THRESH"))) {
        INIT_PRINTF(("sst1InitVideo(): Overriding Default Video Fifo Threshold %d and Storing %d\n",
            sstVideoRez->vFifoThreshold, ATOI(GETENV(("SST_VFIFO_THRESH")))));
        sst->fbiInit3 = (sst->fbiInit3 & ~SST_VIDEO_FIFO_THRESH) |
            ((ATOI(GETENV(("SST_VFIFO_THRESH"))))
             << SST_VIDEO_FIFO_THRESH_SHIFT);
    } else {
        FxU32 vFifoThresholdVal = sstVideoRez->vFifoThreshold;

        if(sst1CurrentBoard->grxClkFreq < 45)
            /* Lower threshold value for slower graphics clocks */
            vFifoThresholdVal = sstVideoRez->vFifoThreshold - 4;
        sst->fbiInit3 = (sst->fbiInit3 & ~SST_VIDEO_FIFO_THRESH) |
            (vFifoThresholdVal << SST_VIDEO_FIFO_THRESH_SHIFT);
    }

    INIT_PRINTF(("sst1InitVideo() Setting up video for resolution (%d, %d), Refresh:%d Hz...\n",
        sstVideoRez->xDimension, sstVideoRez->yDimension, sst1MonitorRefresh));
    INIT_PRINTF(("sst1InitVideo(): Video Fifo Threshold = %d\n",
        (sst->fbiInit3 & SST_VIDEO_FIFO_THRESH)>>SST_VIDEO_FIFO_THRESH_SHIFT));

    /* Initialize Y-Origin */
    sst1InitIdleFBINoNOP(sstbase);
    sst->fbiInit3 = (sst->fbiInit3 & ~SST_YORIGIN_TOP) |
        ((sstVideoRez->yDimension - 1) << SST_YORIGIN_TOP_SHIFT);

    sst1InitIdleFBINoNOP(sstbase);

    memFifoLwm = 23;
    memFifoHwm = 54;
    pciFifoLwm = 13;
#if 0
    if(GETENV(("SST_MEMFIFO_LWM")))
        SSCANF(GETENV(("SST_MEMFIFO_LWM")), "%i", &memFifoLwm);
    if(GETENV(("SST_MEMFIFO_HWM")))
        SSCANF(GETENV(("SST_MEMFIFO_HWM")), "%i", &memFifoHwm);
    if(GETENV(("SST_PCIFIFO_LWM")))
        SSCANF(GETENV(("SST_PCIFIFO_LWM")), "%i", &pciFifoLwm);
#endif
    INIT_PRINTF(("sst1InitVideo(): pciFifoLwm:%d  memFifoLwm:%d  memFifoHwm:%d\n",
        pciFifoLwm, memFifoLwm, memFifoHwm));

    /* Setup Memory FIFO */
    sst1InitIdleFBINoNOP(sstbase);
    sst->fbiInit4 &= ~(SST_MEM_FIFO_ROW_BASE | SST_MEM_FIFO_ROW_ROLL |
        SST_MEM_FIFO_LWM);
    sst1InitIdleFBINoNOP(sstbase);
    if(sst1CurrentBoard->fbiMemSize == 1)
        sst->fbiInit4 |= (0xff << SST_MEM_FIFO_ROW_ROLL_SHIFT);
    else if (sst1CurrentBoard->fbiMemSize == 2)
        sst->fbiInit4 |= (0x1ff << SST_MEM_FIFO_ROW_ROLL_SHIFT);
    else
        /* 4 MBytes frame buffer memory... */
        sst->fbiInit4 |= (0x3ff << SST_MEM_FIFO_ROW_ROLL_SHIFT);
    sst1InitIdleFBINoNOP(sstbase);

    if((sst1CurrentBoard->fbiMemSize == 1 && sst1MonitorRez == 512) ||
       (sst1CurrentBoard->fbiMemSize == 1 && sst1MonitorRez == 512256) ||
       (sst1CurrentBoard->fbiMemSize == 2 && sst1MonitorRez == 800) ||
       (sst1CurrentBoard->fbiMemSize == 2 && sst1MonitorRez == 856))
        sst->fbiInit4 |=
            ((2*sstVideoRez->memOffset) << SST_MEM_FIFO_ROW_BASE_SHIFT);
    else if(sst1CurrentBoard->fbiMemSize < 4 && sst1MonitorRez == 960 &&
        sst1CurrentBoard->sstSliDetect == 0)
        return(FXFALSE);
    else
        sst->fbiInit4 |=
            ((3*sstVideoRez->memOffset) << SST_MEM_FIFO_ROW_BASE_SHIFT);
    sst1InitIdleFBINoNOP(sstbase);
    sst->fbiInit4 |= (memFifoLwm << SST_MEM_FIFO_LWM_SHIFT);
    sst1InitIdleFBINoNOP(sstbase);

    /* Set PCI FIFO LWM */
    sst->fbiInit0 = (sst->fbiInit0 & ~SST_PCI_FIFO_LWM) |
            (pciFifoLwm << SST_PCI_FIFO_LWM_SHIFT);
    sst1InitIdleFBINoNOP(sstbase);

    /* Enable Memory Fifo... */
    if(GETENV(("SST_MEMFIFO")))
        n = ATOI(GETENV(("SST_MEMFIFO")));
    else
        n = 1;

    /* Prohibit illegal memory fifo settings... */
    if(sst1CurrentBoard->fbiMemSize == 1 && sstVideoRez->xDimension > 512)
        n = 0;
    
    if(n) {
        sst1CurrentBoard->fbiMemoryFifoEn = 1;
        if(sst1CurrentBoard->fbiMemSize == 1)
            memFifoEntries = sstVideoRez->memFifoEntries_1MB;
        else if(sst1CurrentBoard->fbiMemSize == 2)
            memFifoEntries = sstVideoRez->memFifoEntries_2MB;
        else
            memFifoEntries = sstVideoRez->memFifoEntries_4MB;
        if(GETENV(("SST_MEMFIFO_ENTRIES")))
            SSCANF(GETENV(("SST_MEMFIFO_ENTRIES")), "%i", &memFifoEntries);
        INIT_PRINTF(("sst1InitVideo(): Enabling Memory FIFO (Entries=0x%x)...\n",
            memFifoEntries));
        sst1CurrentBoard->memFifoStatusLwm = (memFifoEntries << 5) | 0x1f;
        sst->fbiInit0 = (sst->fbiInit0 & ~(SST_MEM_FIFO_EN |
            SST_MEM_FIFO_HWM | SST_PCI_FIFO_LWM | SST_MEM_FIFO_BURST_HWM)) |
            (memFifoEntries << SST_MEM_FIFO_HWM_SHIFT) |
            (pciFifoLwm << SST_PCI_FIFO_LWM_SHIFT) |
            (memFifoHwm << SST_MEM_FIFO_BURST_HWM_SHIFT) |
            SST_MEM_FIFO_EN;
    }

    vInClkDel = 0;
    if((sst1MonitorRez == 960 && !video16BPP) ||
       (sst1MonitorRez == 640 && !video16BPP && sst1MonitorRefresh == 120) ||
       (sst1MonitorRez == 800 && !video16BPP && sst1MonitorRefresh == 75) ||
       (sst1MonitorRez == 800 && !video16BPP && sst1MonitorRefresh == 85))
        vInClkDel = 2;
    if(sst1CurrentBoard->fbiRevision == 2)
        vInClkDel = 0;
    if(GETENV(("SST_VIN_CLKDEL")))
        SSCANF(GETENV(("SST_VIN_CLKDEL")), "%i", &vInClkDel);

    if(sst1CurrentBoard->fbiRevision == 2)
        vOutClkDel = 2;
    else
        vOutClkDel = 0;
    if(GETENV(("SST_VOUT_CLKDEL")))
        SSCANF(GETENV(("SST_VOUT_CLKDEL")), "%i", &vOutClkDel);

    INIT_PRINTF(("sst1InitVideo(): vInClkDel=0x%x  vOutClkDel=0x%x\n",
        vInClkDel, vOutClkDel));

    /* Drive dac output signals and select input video clock */
    if(video16BPP) {
        INIT_PRINTF(("sst1InitVideo(): Setting 16BPP video mode...\n"));
        sst->fbiInit1 = (sst->fbiInit1 & ~SST_VIDEO_MASK) |
            (SST_VIDEO_DATA_OE_EN |
             SST_VIDEO_BLANK_OE_EN |
             SST_VIDEO_HVSYNC_OE_EN |
             SST_VIDEO_DCLK_OE_EN |
             SST_VIDEO_VID_CLK_2X |
             (vInClkDel << SST_VIDEO_VCLK_DEL_SHIFT) |
             (vOutClkDel << SST_VIDEO_VCLK_2X_OUTPUT_DEL_SHIFT) |
             SST_VIDEO_VCLK_SEL);
        sst1InitIdleFBINoNOP(sstbase);
    } else {
        INIT_PRINTF(("sst1InitVideo(): Setting 24BPP video mode...\n"));
        sst->fbiInit1 = (sst->fbiInit1 & ~SST_VIDEO_MASK) |
            (SST_VIDEO_DATA_OE_EN |
             SST_VIDEO_BLANK_OE_EN |
             SST_VIDEO_HVSYNC_OE_EN |
             SST_VIDEO_DCLK_OE_EN |
             SST_VIDEO_VID_CLK_2X |
             SST_VIDEO_VCLK_DIV2 |
             (vInClkDel << SST_VIDEO_VCLK_DEL_SHIFT) |
             (vOutClkDel << SST_VIDEO_VCLK_2X_OUTPUT_DEL_SHIFT) |
             SST_VIDEO_24BPP_EN);
        sst1InitIdleFBINoNOP(sstbase);
        if(!GETENV(("SST_VIDEO_FILTER_DISABLE"))) {
            sst->fbiInit1 |= SST_VIDEO_FILTER_EN;
            if(GETENV(("SST_VIDEO_FILTER_THRESHOLD"))) {
                SSCANF(GETENV(("SST_VIDEO_FILTER_THRESHOLD")), "%i", &n);
                INIT_PRINTF(("sst1InitVideo(): Setting Video Filtering Treshold to 0x%x...\n", n));

                sst->videoFilterRgbThreshold = n;
            } else
                /* sst->videoFilterRgbThreshold = 0x180c18; */
                sst->videoFilterRgbThreshold = 0x080408;
        } else
            INIT_PRINTF(("sst1InitVideo(): Disabling Video Filtering...\n"));
    }

    /* Adjust FT-clock delay */
    if(sst1CurrentBoard->fbiRevision == 2) {
        /* .5 micron */
        if(sst1CurrentBoard->tmuRevision == 1)
            ft_clk_del = SST_FT_CLK_DEL_ADJ_DEFAULT_R21;
        else
            ft_clk_del = SST_FT_CLK_DEL_ADJ_DEFAULT_R20;
    } else {
        /* .6 micron */
        if(sst1CurrentBoard->tmuRevision == 1)
            ft_clk_del = SST_FT_CLK_DEL_ADJ_DEFAULT_R11;
        else
            ft_clk_del = SST_FT_CLK_DEL_ADJ_DEFAULT_R10;
    }

    /* Adjust TF-clock delay */
    if(sst1CurrentBoard->tmuRevision == 1) {
        if(sst1CurrentBoard->fbiRevision == 2) {
            tf0_clk_del = SST_TEX_TF_CLK_DEL_ADJ_DEFAULT_R21;
            tf1_clk_del = SST_TEX_TF_CLK_DEL_ADJ_DEFAULT_R21;
            tf2_clk_del = SST_TEX_TF_CLK_DEL_ADJ_DEFAULT_R21;
        } else {
            tf0_clk_del = SST_TEX_TF_CLK_DEL_ADJ_DEFAULT_R11;
            tf1_clk_del = SST_TEX_TF_CLK_DEL_ADJ_DEFAULT_R11;
            tf2_clk_del = SST_TEX_TF_CLK_DEL_ADJ_DEFAULT_R11;
        }
    } else {
        if(sst1CurrentBoard->fbiRevision == 2) {
            tf0_clk_del = SST_TEX_TF_CLK_DEL_ADJ_DEFAULT_R20;
            tf1_clk_del = SST_TEX_TF_CLK_DEL_ADJ_DEFAULT_R20;
            tf2_clk_del = SST_TEX_TF_CLK_DEL_ADJ_DEFAULT_R20;
        } else {
            tf0_clk_del = SST_TEX_TF_CLK_DEL_ADJ_DEFAULT_R10;
            tf1_clk_del = SST_TEX_TF_CLK_DEL_ADJ_DEFAULT_R10;
            tf2_clk_del = SST_TEX_TF_CLK_DEL_ADJ_DEFAULT_R10;
        }
    }

    /* Adjust clock delays for .5 micron to account for frequency dependencies */
    if(sst1CurrentBoard->fbiRevision == 2 && sst1CurrentBoard->tmuRevision == 1)
    {
        if(sst1CurrentBoard->grxClkFreq >= 55) {
            ft_clk_del = tf0_clk_del = tf1_clk_del = tf2_clk_del = 0x5;
        } else if(sst1CurrentBoard->grxClkFreq >= 48) {
            ft_clk_del = tf0_clk_del = tf1_clk_del = tf2_clk_del = 0x6;
        } else if(sst1CurrentBoard->grxClkFreq >= 45) {
            ft_clk_del = tf0_clk_del = tf1_clk_del = tf2_clk_del = 0x7;
        } else if(sst1CurrentBoard->grxClkFreq >= 43) {
            ft_clk_del = tf0_clk_del = tf1_clk_del = tf2_clk_del = 0x8;
        } else {
            ft_clk_del = tf0_clk_del = tf1_clk_del = tf2_clk_del = 0x9;
        }
    }

#if TEST_GAME
# define DIP_SWITCHES (*(unsigned short *)0x0B6000000)
# define SWITCHES(x,y) ((~DIP_SWITCHES>>(y))&(x))
    {
	int sw;
	sw = ~SWITCHES(7, 2);
	if ((sw&4)) sw |= -4;
	ft_clk_del += sw;
	sw = ~SWITCHES(7, 5);
	if ((sw&4)) sw |= -4;
	tf0_clk_del += sw;
	sw = ~SWITCHES(7, 8);
	if ((sw&4)) sw |= -4;
	tf1_clk_del += sw;
    }
#endif

    /* Override with environment variables */
    if(GETENV(("SST_FT_CLK_DEL")))
        SSCANF(GETENV(("SST_FT_CLK_DEL")), "%i", &ft_clk_del);
    if(GETENV(("SST_TF0_CLK_DEL")))
        SSCANF(GETENV(("SST_TF0_CLK_DEL")), "%i", &tf0_clk_del);
    if(GETENV(("SST_TF1_CLK_DEL")))
        SSCANF(GETENV(("SST_TF1_CLK_DEL")), "%i", &tf1_clk_del);
    if(GETENV(("SST_TF2_CLK_DEL")))
        SSCANF(GETENV(("SST_TF2_CLK_DEL")), "%i", &tf2_clk_del);

    INIT_PRINTF(("sst1InitVideo(): Setting FBI-to-TREX clock delay to 0x%x...\n", ft_clk_del));
    INIT_PRINTF(("sst1InitVideo(): Setting TREX#0 TREX-to-FBI clock delay to 0x%x\n",
        tf0_clk_del));
    INIT_PRINTF(("sst1InitVideo(): Setting TREX#1 TREX-to-FBI clock delay to 0x%x\n",
        tf1_clk_del));
    INIT_PRINTF(("sst1InitVideo(): Setting TREX#2 TREX-to-FBI clock delay to 0x%x\n",
        tf2_clk_del));

    sst->fbiInit3 = (sst->fbiInit3 & ~SST_FT_CLK_DEL_ADJ) |
        (ft_clk_del << SST_FT_CLK_DEL_ADJ_SHIFT);
    sst1InitIdleFBINoNOP(sstbase);

    sst1CurrentBoard->tmuInit1[0] = (sst1CurrentBoard->tmuInit1[0] &
        ~SST_TEX_TF_CLK_DEL_ADJ) | (tf0_clk_del<<SST_TEX_TF_CLK_DEL_ADJ_SHIFT);
    sst1CurrentBoard->tmuInit1[1] = (sst1CurrentBoard->tmuInit1[1] &
        ~SST_TEX_TF_CLK_DEL_ADJ) | (tf1_clk_del<<SST_TEX_TF_CLK_DEL_ADJ_SHIFT);
    sst1CurrentBoard->tmuInit1[2] = (sst1CurrentBoard->tmuInit1[2] &
        ~SST_TEX_TF_CLK_DEL_ADJ) | (tf2_clk_del<<SST_TEX_TF_CLK_DEL_ADJ_SHIFT);

    SST_TREX(sst,0)->trexInit1 = sst1CurrentBoard->tmuInit1[0];
    /* sst1InitIdleFBINoNOP(sstbase); */
    /* Can't use sst1InitIdleFbi because changing the tf clock delay may */
    /* incorrectly put data into the TREX-to-FBI fifo */
    for(n=0; n<100; n++) sst1InitReturnStatus(sstbase);
    SST_TREX(sst,1)->trexInit1 = sst1CurrentBoard->tmuInit1[1];
    for(n=0; n<100; n++) sst1InitReturnStatus(sstbase);
    SST_TREX(sst,2)->trexInit1 = sst1CurrentBoard->tmuInit1[2];
    for(n=0; n<100; n++) sst1InitReturnStatus(sstbase);

    /* Setup graphics clock */
    if(sst1InitGrxClk(sstbase) == FXFALSE)
        return(FXFALSE);

    /* Setup video mode */
    if(sst1InitSetVidMode(sstbase, video16BPP) == FXFALSE)
        return(FXFALSE);

    /* Adjust Video Clock */
#ifndef DIRECTX
    if(GETENV(("SST_VIDCLK2X"))) {
        float vidClkFreq;

        SSCANF(GETENV(("SST_VIDCLK2X")), "%f", &vidClkFreq);
        if(sst1InitSetVidClk(sstbase, vidClkFreq) == FXFALSE)
            return(FXFALSE);
    } else {
#endif
        if(sst1InitUseVoodooFile == FXTRUE) {
            if(sst1InitSetVidClkINI(sstbase, sst1CurrentBoard->fbiVideoWidth,
              sst1CurrentBoard->fbiVideoHeight,
              sst1CurrentBoard->fbiVideoRefresh, video16BPP) == FXFALSE)
                return(FXFALSE);
        } else {
            if(video16BPP) {
                if(sst1InitSetVidClk(sstbase, sstVideoRez->clkFreq16bpp) ==
                  FXFALSE)
                    return(FXFALSE);
            } else {
                if(sst1InitSetVidClk(sstbase, sstVideoRez->clkFreq24bpp) ==
                  FXFALSE)
                    return(FXFALSE);
            }
        }
#ifndef DIRECTX
    }
#endif

    /* Wait for video clock to stabilize */
    /* Estimate that FBI register reads are performed at 3M words/sec */
    /* Reading 250K times will stall for approx. 1/10 second */
    for(n=0; n<250000; n++)
        sst1InitReturnStatus(sstbase);

    /* Run Video Reset Module */
    sst->fbiInit1 &= ~SST_VIDEO_RESET;
    sst1InitIdleFBINoNOP(sstbase);

    sst->fbiInit2 |= SST_EN_DRAM_REFRESH;

    sst1InitIdleFBINoNOP(sstbase);

    if(!GETENV(("SST_VIDEO_NOCLEAR"))) {
        /* Clear Screen */
        FxU32 clearColor = 0x0;

        if(GETENV(("SST_VIDEO_CLEARCOLOR")))
            SSCANF(GETENV(("SST_VIDEO_CLEARCOLOR")), "%i", &clearColor);
        sst->c1 = clearColor;
        sst->c0 = clearColor;
        sst->zaColor = 0x0;
        sst->clipLeftRight = sstVideoRez->xDimension;
        sst->clipBottomTop = sstVideoRez->yDimension;
        sst->fbzMode = SST_RGBWRMASK | SST_ZAWRMASK;
        sst->fastfillCMD = 0x0;                             /* Frontbuffer & Z/A */
        sst->fbzMode = SST_RGBWRMASK | SST_DRAWBUFFER_BACK; /* Backbuffer */
        sst->fastfillCMD = 0x0;
        sst->nopCMD = 0x1;   /* Clear fbistat registers after clearing screen */
        sst1InitIdleFBINoNOP(sstbase);
    } else
        INIT_PRINTF(("sst1InitVideo(): Not Clearing Frame Buffer(s)...\n"));

    sst1InitVgaPassCtrl(sstbase, 0);

    if(GETENV(("SST_VIDEO_DISABLE"))) {
        INIT_PRINTF(("sst1InitVideo(): Disabling video timing...\n"));
        sst->fbiInit1 |= SST_VIDEO_RESET;
        sst1InitIdleFBINoNOP(sstbase);
    }
    sst1InitIdle(sstbase);

    if(((sst->status & SST_SWAPBUFPENDING) >> SST_SWAPBUFPENDING_SHIFT) > 0)
        sst1InitClearSwapPending(sstbase);

    INIT_PRINTF(("sst1InitVideo() exiting with status %d...\n", FXTRUE));
    return(FXTRUE);
}

/*
** sst1InitSetVidMode():
**  Set video Mode
**
*/
FxBool sst1InitSetVidMode(FxU32 *sstbase, FxU32 video16BPP)
{
    int helper = (GETENV(("SST_DEBUGDAC"))) ? 1 : 0;

    if(helper)
        INIT_PRINTF(("sst1InitSetVidMode(): Entered...\n"));

    if(sst1InitCheckBoard(sstbase) == FXFALSE)
        return(FXFALSE);

    if(sst1InitUseVoodooFile == FXTRUE) {
        return(sst1InitSetVidModeINI(sstbase, video16BPP));
    } else {
        if((sst1CurrentBoard->fbiDacType == SST_FBI_DACTYPE_ATT) ||
           (sst1CurrentBoard->fbiDacType == SST_FBI_DACTYPE_TI))
           return(sst1InitSetVidModeATT(sstbase, video16BPP));
        else if(sst1CurrentBoard->fbiDacType == SST_FBI_DACTYPE_ICS)
           return(sst1InitSetVidModeICS(sstbase, video16BPP));
        else
            return(FXFALSE);
    }
}

/*
** sst1InitSetResolution():
**  Setup FBI video resolution registers
**  This routine is used by sst1InitVideo()
**
**
*/
FX_EXPORT void FX_CSTYLE sst1InitSetResolution(FxU32 *sstbase, 
    const sst1VideoTimingStruct *sstVideoRez, FxU32 Banked)
{
    volatile Sstregs *sst = (Sstregs *) sstbase;

    if(Banked)
        sst->fbiInit2 = (sst->fbiInit2 & ~SST_VIDEO_BUFFER_OFFSET) |
            (sstVideoRez->memOffset << SST_VIDEO_BUFFER_OFFSET_SHIFT) |
            SST_DRAM_BANKING_CONFIG | SST_EN_DRAM_BANKED;
    else
        sst->fbiInit2 = (sst->fbiInit2 & ~SST_VIDEO_BUFFER_OFFSET) |
            (sstVideoRez->memOffset << SST_VIDEO_BUFFER_OFFSET_SHIFT);

    sst->fbiInit1 = (sst->fbiInit1 & ~SST_VIDEO_TILES_IN_X) |
        (sstVideoRez->tilesInX_Over2 << SST_VIDEO_TILES_IN_X_SHIFT);
}

/*
** sst1InitSetVidClk():
**  Set video clock
**
*/
FX_EXPORT FxBool FX_CSTYLE sst1InitSetVidClk(FxU32 *sstbase, float vidClkFreq)
{
    sst1ClkTimingStruct vidClkTiming;
    int helper = (GETENV(("SST_DEBUGDAC"))) ? 1 : 0;

    if(helper)
        INIT_PRINTF(("sst1InitSetVidClk(): Entered...\n"));

    if(sst1InitCheckBoard(sstbase) == FXFALSE)
        return(FXFALSE);

    if(sst1InitComputeClkParams(vidClkFreq, &vidClkTiming) == FXFALSE)
        return(FXFALSE);

    INIT_PRINTF(("sst1InitSetVidClk(): Setting up %.2f MHz Video Clock...\n",
        vidClkFreq));

    if((sst1CurrentBoard->fbiDacType == SST_FBI_DACTYPE_ATT) ||
       (sst1CurrentBoard->fbiDacType == SST_FBI_DACTYPE_TI))
       return(sst1InitSetVidClkATT(sstbase, &vidClkTiming));
    else if(sst1CurrentBoard->fbiDacType == SST_FBI_DACTYPE_ICS)
       return(sst1InitSetVidClkICS(sstbase, &vidClkTiming));
    else
        return(FXFALSE);
}

/*
** sst1InitSetGrxClk():
**  Set graphics clock
**  NOTE: sst1InitSetGrxClk() resets the PCI fifo and the graphics subsystem
**  of FBI
**
*/
FX_EXPORT FxBool FX_CSTYLE sst1InitSetGrxClk(FxU32 *sstbase,
  sst1ClkTimingStruct *sstGrxClk)
{
    FxBool retVal = FXFALSE;
    int helper = (GETENV(("SST_DEBUGDAC"))) ? 1 : 0;

    if(helper)
        INIT_PRINTF(("sst1InitSetGrxClk(): Entered...\n"));

    if(sst1InitCheckBoard(sstbase) == FXFALSE)
        return(FXFALSE);

    if(sst1InitUseVoodooFile == FXTRUE) {
        retVal = sst1InitSetGrxClkINI(sstbase, sstGrxClk);
    } else {
        if((sst1CurrentBoard->fbiDacType == SST_FBI_DACTYPE_ATT) ||
           (sst1CurrentBoard->fbiDacType == SST_FBI_DACTYPE_TI))
           retVal = sst1InitSetGrxClkATT(sstbase, sstGrxClk);
        else if(sst1CurrentBoard->fbiDacType == SST_FBI_DACTYPE_ICS)
           retVal = sst1InitSetGrxClkICS(sstbase, sstGrxClk);
    }
    if(retVal == FXFALSE)
        return(FXFALSE);

    if(sst1InitReturnStatus(sstbase) & SST_TREX_BUSY) {
        INIT_PRINTF(("sst1InitSetGrxClk(): Resetting TMUs after clock change...\n"));
        return(sst1InitResetTmus(sstbase));
    }
    return(FXTRUE);
}


#pragma optimize ("",on)
