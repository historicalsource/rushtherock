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
** $Revision: 4 $ 
** $Date: 7/19/96 7:03p $ 
**
** Initialization code for initializing scanline interleaving
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
** sst1InitSli():
**  Setup Multiple SST-1 subsystems for Scanline Interleaving
**  sstbase0 defined to be the SLI Master
**  sstbase1 defined to be the SLI Slave
**
*/
FX_EXPORT FxBool FX_CSTYLE sst1InitSli(FxU32 *sstbase0, FxU32 *sstbase1)
{
    FxU32 j, n, MasterPhysAddr, cntr;
    volatile Sstregs *sstMaster = (Sstregs *) sstbase0;
    volatile Sstregs *sstSlave = (Sstregs *) sstbase1;
    FxU32 MasterInit1, SlaveInit1;
    FxU32 memOffset;
    FxU32 masterVInClkDel, masterVOutClkDel;
    FxU32 slaveVInClkDel, slaveVOutClkDel;
    FxU32 masterPVOutClkDel, slavePVOutClkDel;
    FxU32 pciFifoLwm, memFifoLwm;

    INIT_PRINTF(("sst1InitSli(): Enabling Scanline Interleaving...\n"));

    /* Setup SLI Slave... */
    cntr = 0;
    while(1) {
        if(sst1InitCheckBoard(sstbase1) == FXFALSE)
            return(FXFALSE);
        PCICFG_RD(SST1_PCI_INIT_ENABLE, j);
        PCICFG_WR(SST1_PCI_INIT_ENABLE,
            (j | SST_SCANLINE_SLV_OWNPCI | SST_SCANLINE_SLI_SLV)); 
        sstSlave->fbiInit1 |= (SST_VIDEO_RESET | SST_EN_SCANLINE_INTERLEAVE);
        sst1InitIdleFBINoNOP(sstbase1);
        PCICFG_WR(SST1_PCI_VCLK_DISABLE, 0x0); /* Disable video clock */
        SST_TREX(sstSlave,0)->trexInit1 = sst1CurrentBoard->tmuInit1[0] |
            SST_EN_TEX_SCANLINE_INTERLEAVE | SST_TEX_SCANLINE_INTERLEAVE_SLAVE;
        sst1InitIdle(sstbase1);
        SST_TREX(sstSlave,1)->trexInit1 = sst1CurrentBoard->tmuInit1[1] |
            SST_EN_TEX_SCANLINE_INTERLEAVE | SST_TEX_SCANLINE_INTERLEAVE_SLAVE;
        sst1InitIdle(sstbase1);
        SST_TREX(sstSlave,2)->trexInit1 = sst1CurrentBoard->tmuInit1[2] |
            SST_EN_TEX_SCANLINE_INTERLEAVE | SST_TEX_SCANLINE_INTERLEAVE_SLAVE;
        sst1InitIdle(sstbase1);
        sst1InitIdleFBINoNOP(sstbase1);
        sstSlave->fbiInit2 = (sstSlave->fbiInit2 & ~(SST_EN_DRAM_BANKED |
            SST_SWAP_ALGORITHM)) | SST_SWAP_DACDATA0;
        sst1InitIdleFBINoNOP(sstbase1);
        /* Initialize Y-Origin */
        sstSlave->fbiInit3 = (sstSlave->fbiInit3 & ~SST_YORIGIN_TOP) |
            ((sst1CurrentBoard->fbiVideoHeight - 1) << SST_YORIGIN_TOP_SHIFT);
        sst1InitIdleFBINoNOP(sstbase1);
        memOffset = ((sstSlave->fbiInit2 & SST_VIDEO_BUFFER_OFFSET) >>
            SST_VIDEO_BUFFER_OFFSET_SHIFT) + 16;
        if(memOffset & 0x1) {
            if((memOffset + 1) & 0x3) /* not divisible by 4... */
                memOffset = (memOffset + 1) >> 1;
            else
                memOffset = (memOffset + 3) >> 1;
        } else {
            if(memOffset & 0x3) /* not divisible by 4... */
                memOffset = (memOffset + 2) >> 1;
            else
                memOffset = memOffset >> 1;
        }
        if(sst1CurrentBoard->fbiVideoWidth == 960) {
            memOffset = 170;
            sstSlave->fbiInit0 &= ~SST_MEM_FIFO_EN;
            INIT_PRINTF(("sst1InitSli(): Disabling memory fifo...\n"));
        }
        sstSlave->fbiInit2 = (sstSlave->fbiInit2 & ~SST_VIDEO_BUFFER_OFFSET) |
            (memOffset << SST_VIDEO_BUFFER_OFFSET_SHIFT);
        sst1InitIdleFBINoNOP(sstbase1);
        sstSlave->fbiInit0 = (sstSlave->fbiInit0 & ~SST_MEM_FIFO_HWM) |
            (0x100 << SST_MEM_FIFO_HWM_SHIFT);
        sst1InitIdleFBINoNOP(sstbase1);
        sstSlave->fbiInit4 = (sstSlave->fbiInit4 &  ~SST_MEM_FIFO_ROW_BASE) |
            ((3*memOffset) << SST_MEM_FIFO_ROW_BASE_SHIFT);
        sst1InitIdleFBINoNOP(sstbase1);
        slaveVInClkDel = 0;
        slaveVOutClkDel = 2;
        slavePVOutClkDel = 0;
        if(GETENV(("SST_SLIS_VOUT_CLKDEL")))
            SSCANF(GETENV(("SST_SLIS_VOUT_CLKDEL")), "%i", &slaveVOutClkDel);
        if(GETENV(("SST_SLIS_PVOUT_CLKDEL")))
            SSCANF(GETENV(("SST_SLIS_PVOUT_CLKDEL")), "%i", &slavePVOutClkDel);
        if(GETENV(("SST_SLIS_VIN_CLKDEL")))
            SSCANF(GETENV(("SST_SLIS_VIN_CLKDEL")), "%i", &slaveVInClkDel);
        INIT_PRINTF(("sst1InitSli(): slaveVinClkdel=0x%x, slaveVOutClkDel=0x%x, slavePVOutClkDel=0x%x\n",
            slaveVInClkDel, slaveVOutClkDel, slavePVOutClkDel));
        if(sst1CurrentBoard->fbiVideo16BPP)
            /* 16-bit Video Output */
            sstSlave->fbiInit1 = (sstSlave->fbiInit1 &
                    ~(SST_VIDEO_VCLK_2X_OUTPUT_DEL | SST_VIDEO_VCLK_DEL)) | 
                SST_EN_SCANLINE_INTERLEAVE |
                /*SST_VIDEO_VID_CLK_SLAVE | */
                SST_VIDEO_VID_CLK_2X |
                /*SST_VIDEO_INVERT_VID_CLK_2X | */
                SST_PCI_WRWS_1 |
                (slaveVInClkDel << SST_VIDEO_VCLK_DEL_SHIFT) |
                (slaveVOutClkDel << SST_VIDEO_VCLK_2X_OUTPUT_DEL_SHIFT);
        else
            /* 24-bit Video Output */
            sstSlave->fbiInit1 = (sstSlave->fbiInit1 &
                    ~(SST_VIDEO_VCLK_2X_OUTPUT_DEL | SST_VIDEO_VCLK_DEL |
                      SST_VIDEO_VCLK_SEL | SST_VIDEO_VCLK_2X_INPUT_DEL)) | 
                SST_EN_SCANLINE_INTERLEAVE |
                /*SST_VIDEO_VID_CLK_SLAVE | */
                SST_VIDEO_VID_CLK_2X |
                /*SST_VIDEO_INVERT_VID_CLK_2X | */
                (0x0 << SST_VIDEO_VCLK_SEL_SHIFT) |
                SST_PCI_WRWS_1 |
                (slaveVInClkDel << SST_VIDEO_VCLK_DEL_SHIFT) |
                (slaveVOutClkDel << SST_VIDEO_VCLK_2X_OUTPUT_DEL_SHIFT) |
                (slavePVOutClkDel << SST_VIDEO_VCLK_2X_INPUT_DEL_SHIFT);
        sst1InitIdleFBINoNOP(sstbase1);
        sstSlave->fbiInit1 &= ~(SST_VIDEO_DATA_OE_EN |
                                SST_VIDEO_BLANK_OE_EN |
                                /* SST_VIDEO_DCLK_OE_EN | */
                                SST_VIDEO_HVSYNC_OE_EN);
        sst1InitIdleFBINoNOP(sstbase1);
        /* Initialize pci and memory fifos... */
        pciFifoLwm = 16;
        memFifoLwm = 26;
        if(!GETENV(("SST_PCIFIFO_LWM")))
            sstSlave->fbiInit0 = (sstSlave->fbiInit0 & ~SST_PCI_FIFO_LWM) |
                    (pciFifoLwm << SST_PCI_FIFO_LWM_SHIFT);
        sst1InitIdleFBINoNOP(sstbase1);
        if(!GETENV(("SST_MEMFIFO_LWM")))
            sstSlave->fbiInit4 = (sstSlave->fbiInit4 & ~SST_MEM_FIFO_LWM) |
                    (memFifoLwm << SST_MEM_FIFO_LWM_SHIFT);
        sst1InitIdleFBINoNOP(sstbase1);
        INIT_PRINTF(("sst1InitSli(): Slave pciFifoLwm:%d, memFifoLwm:%d\n",
          ((sstSlave->fbiInit0 & SST_PCI_FIFO_LWM) >> SST_PCI_FIFO_LWM_SHIFT),
          ((sstSlave->fbiInit4 & SST_MEM_FIFO_LWM) >> SST_MEM_FIFO_LWM_SHIFT)));
        sst1InitIdleFBINoNOP(sstbase1);
        sst1InitIdleFBINoNOP(sstbase1);
        sstSlave->fbiInit1 |= (SST_EN_SCANLINE_INTERLEAVE | SST_VIDEO_RESET);
        sst1InitIdleFBINoNOP(sstbase1);
        sst1InitIdleFBINoNOP(sstbase1);
        sst1InitIdleFBINoNOP(sstbase1);
        sstSlave->fbiInit1 &= ~SST_VIDEO_RESET;
        sst1InitIdleFBINoNOP(sstbase1);
        sst1InitIdleFBINoNOP(sstbase1);
        if(sstSlave->fbiInit1 & SST_VIDEO_RESET) {
            printf("sst1InitSli(): Could not unReset Slave Video...\n");
            if(++cntr < 10)
                continue;
            else
                return(FXFALSE);
        }
        if(!(sstSlave->fbiInit1 & SST_EN_SCANLINE_INTERLEAVE)) {
            printf("sst1InitSli(): Could not setup SLI Slave...\n");
            if(++cntr < 10)
                continue;
            else
                return(FXFALSE);
        }
        SlaveInit1 = sstSlave->fbiInit1;
        break;
    }

    /* Setup SLI Master... */
    sstMaster->fbiInit1 |= SST_VIDEO_RESET;
    SST_TREX(sstMaster,0)->trexInit1 = sst1CurrentBoard->tmuInit1[0] |
        SST_EN_TEX_SCANLINE_INTERLEAVE;
    sst1InitIdle(sstbase0);
    SST_TREX(sstMaster,1)->trexInit1 = sst1CurrentBoard->tmuInit1[1] |
        SST_EN_TEX_SCANLINE_INTERLEAVE;
    sst1InitIdle(sstbase0);
    SST_TREX(sstMaster,2)->trexInit1 = sst1CurrentBoard->tmuInit1[2] |
        SST_EN_TEX_SCANLINE_INTERLEAVE;
    sst1InitIdle(sstbase0);
    sst1InitIdleFBINoNOP(sstbase0);
    sstMaster->fbiInit2 = (sstMaster->fbiInit2 & ~(SST_EN_DRAM_BANKED |
        SST_SWAP_ALGORITHM)) | SST_SWAP_DACDATA0;
    sst1InitIdleFBINoNOP(sstbase0);
    /* Initialize Y-Origin */
    sstMaster->fbiInit3 = (sstMaster->fbiInit3 & ~SST_YORIGIN_TOP) |
        ((sst1CurrentBoard->fbiVideoHeight) << SST_YORIGIN_TOP_SHIFT);
    sst1InitIdleFBINoNOP(sstbase0);
    if(sst1CurrentBoard->fbiVideoWidth == 960)
        sstMaster->fbiInit0 &= ~SST_MEM_FIFO_EN;
    sstMaster->fbiInit2 = (sstMaster->fbiInit2 & ~SST_VIDEO_BUFFER_OFFSET) |
        (memOffset << SST_VIDEO_BUFFER_OFFSET_SHIFT);
    sst1InitIdleFBINoNOP(sstbase0);
    sstMaster->fbiInit0 = (sstMaster->fbiInit0 & ~SST_MEM_FIFO_HWM) |
        (0x100 << SST_MEM_FIFO_HWM_SHIFT);
    sst1InitIdleFBINoNOP(sstbase0);
    sstMaster->fbiInit4 = (sstMaster->fbiInit4 &  ~SST_MEM_FIFO_ROW_BASE) |
        ((3*memOffset) << SST_MEM_FIFO_ROW_BASE_SHIFT);
    sst1InitIdleFBINoNOP(sstbase0);
    masterVInClkDel = 1;
    masterVOutClkDel = 2;
    masterPVOutClkDel = 0;
    if(GETENV(("SST_SLIM_VOUT_CLKDEL")))
        SSCANF(GETENV(("SST_SLIM_VOUT_CLKDEL")), "%i", &masterVOutClkDel);
    if(GETENV(("SST_SLIM_PVOUT_CLKDEL")))
        SSCANF(GETENV(("SST_SLIM_PVOUT_CLKDEL")), "%i", &masterPVOutClkDel);
    if(GETENV(("SST_SLIM_VIN_CLKDEL")))
        SSCANF(GETENV(("SST_SLIM_VIN_CLKDEL")), "%i", &masterVInClkDel);
    INIT_PRINTF(("sst1InitSli(): masterVinClkdel=0x%x, masterVOutClkDel=0x%x, masterPVOutClkDel=0x%x\n",
        masterVInClkDel, masterVOutClkDel, masterPVOutClkDel));
    if(sst1CurrentBoard->fbiVideo16BPP)
        /* 16-bit Video Output */
        sstMaster->fbiInit1 = (sstMaster->fbiInit1 &
                ~(SST_VIDEO_VCLK_2X_OUTPUT_DEL | SST_VIDEO_VCLK_DEL)) | 
            SST_EN_SCANLINE_INTERLEAVE |
            SST_VIDEO_VID_CLK_2X |
            /* SST_VIDEO_INVERT_VID_CLK_2X | */
            SST_PCI_WRWS_1 |
            (masterVInClkDel << SST_VIDEO_VCLK_DEL_SHIFT) |
            (masterVOutClkDel << SST_VIDEO_VCLK_2X_OUTPUT_DEL_SHIFT);
    else
        /* 24-bit Video Output */
        sstMaster->fbiInit1 = (sstMaster->fbiInit1 &
                ~(SST_VIDEO_VCLK_2X_OUTPUT_DEL | SST_VIDEO_VCLK_DEL |
                      SST_VIDEO_VCLK_SEL | SST_VIDEO_VCLK_2X_INPUT_DEL)) | 
            SST_EN_SCANLINE_INTERLEAVE |
            SST_VIDEO_VID_CLK_2X |
            /*SST_VIDEO_INVERT_VID_CLK_2X | */
            (0x0 << SST_VIDEO_VCLK_SEL_SHIFT) |
            SST_PCI_WRWS_1 |
            (masterVInClkDel << SST_VIDEO_VCLK_DEL_SHIFT) |
            (masterVOutClkDel << SST_VIDEO_VCLK_2X_OUTPUT_DEL_SHIFT) |
            (masterPVOutClkDel << SST_VIDEO_VCLK_2X_INPUT_DEL_SHIFT);
    sst1InitIdleFBINoNOP(sstbase0);
    /* Initialize pci and memory fifos... */
    pciFifoLwm = 16;
    memFifoLwm = 26;
    if(!GETENV(("SST_PCIFIFO_LWM")))
        sstMaster->fbiInit0 = (sstMaster->fbiInit0 & ~SST_PCI_FIFO_LWM) |
                (pciFifoLwm << SST_PCI_FIFO_LWM_SHIFT);
    sst1InitIdleFBINoNOP(sstbase0);
    if(!GETENV(("SST_MEMFIFO_LWM")))
        sstMaster->fbiInit4 = (sstMaster->fbiInit4 & ~SST_MEM_FIFO_LWM) |
                (memFifoLwm << SST_MEM_FIFO_LWM_SHIFT);
    sst1InitIdleFBINoNOP(sstbase0);
    INIT_PRINTF(("sst1InitSli(): Master pciFifoLwm:%d, memFifoLwm:%d\n",
      ((sstMaster->fbiInit0 & SST_PCI_FIFO_LWM) >> SST_PCI_FIFO_LWM_SHIFT),
      ((sstMaster->fbiInit4 & SST_MEM_FIFO_LWM) >> SST_MEM_FIFO_LWM_SHIFT)));
    sstMaster->fbiInit1 &= ~SST_VIDEO_RESET;
    sst1InitIdleFBINoNOP(sstbase0);
    MasterInit1 = sstMaster->fbiInit1;

    /* Master */
    if(sst1InitCheckBoard(sstbase0) == FXFALSE)
        return(FXFALSE);
    PCICFG_RD(SST1_PCI_INIT_ENABLE, j);
    PCICFG_WR(SST1_PCI_INIT_ENABLE,
        (j & ~(SST_SCANLINE_SLV_OWNPCI | SST_SCANLINE_SLI_SLV))); 
    MasterPhysAddr = sst1CurrentBoard->physAddr;
    sst1InitReturnStatus(sstbase0); /* flush pci packer with reads */
    sst1InitReturnStatus(sstbase0);
    sst1InitReturnStatus(sstbase0);

    /* Slave */
    if(sst1InitCheckBoard(sstbase1) == FXFALSE)
        return(FXFALSE);
    PCICFG_RD(SST1_PCI_INIT_ENABLE, j);
    PCICFG_WR(SST1_PCI_INIT_ENABLE,
        ((j & ~(SST_SCANLINE_SLV_OWNPCI)) | SST_SCANLINE_SLI_SLV)); 
    /* Map both boards to same Master physical address */
    PCICFG_WR(PCI_BASE_ADDRESS_0, MasterPhysAddr);
    sst1InitReturnStatus(sstbase0); /* flush pci packer with reads */
    sst1InitReturnStatus(sstbase0);
    sst1InitReturnStatus(sstbase0);

    /* Reset Video units simultaneously */
    sstMaster->fbiInit1 = MasterInit1 | SST_VIDEO_RESET;
    sst1InitIdleFBINoNOP(sstbase0);
    sst1InitIdleFBINoNOP(sstbase0);
    sst1InitIdleFBINoNOP(sstbase0);
    sstMaster->fbiInit1 = MasterInit1;
    if(sst1InitCheckBoard(sstbase0) == FXFALSE)
        return(FXFALSE);
    PCICFG_WR(SST1_PCI_INIT_ENABLE, 0x0);  /* Disable writes to init regs */
    sst1InitReturnStatus(sstbase0);
    sstMaster->fbiInit1 = SlaveInit1;
    sst1InitIdleFBINoNOP(sstbase0);
    /* Enable writes to init regs */
    PCICFG_WR(SST1_PCI_INIT_ENABLE, (SST_INITWR_EN | SST_PCI_FIFOWR_EN));
    sst1InitIdleFBINoNOP(sstbase0);

    if(sst1InitCheckBoard(sstbase0) == FXFALSE)
        return(FXFALSE);

    /* Clear buffers as a result of new buffer offsets... */
    if(!GETENV(("SST_VIDEO_NOCLEAR"))) {
        /* Clear Screen */
        FxU32 clearColor = 0x0;

        if(GETENV(("SST_VIDEO_CLEARCOLOR")))
            SSCANF(GETENV(("SST_VIDEO_CLEARCOLOR")), "%i", &clearColor);
        sstMaster->c1 = clearColor;
        sstMaster->c0 = clearColor;
        sstMaster->zaColor = 0x0;
        sstMaster->clipLeftRight = sst1CurrentBoard->fbiVideoWidth;
        sstMaster->clipBottomTop = sst1CurrentBoard->fbiVideoHeight;
        sstMaster->fbzMode = SST_RGBWRMASK | SST_ZAWRMASK;
        sstMaster->fastfillCMD = 0x0;    /* Frontbuffer & Z/A */
        sstMaster->fbzMode = SST_RGBWRMASK | SST_DRAWBUFFER_BACK; /* Backbuffer */
        sstMaster->fastfillCMD = 0x0;
        sstMaster->nopCMD = 0x1;
    }
    sst1InitSliEnabled = 1;
    sst1InitSliSlaveVirtAddr = sstbase1;

    sst1InitIdle(sstbase0);

    return(FXTRUE);
}

/*
** sst1InitShutdownSli():
**  Shutdown SLI configuration
**
*/
FX_EXPORT FxBool FX_CSTYLE sst1InitShutdownSli(FxU32 *sstbase)
{
    volatile Sstregs *sstMaster = (Sstregs *) sstbase;
    volatile Sstregs *sstSlave = (Sstregs *) sst1InitSliSlaveVirtAddr;
    FxU32 n, j, cntr;

    if(sst1InitSliEnabled) {
        INIT_PRINTF(("sst1InitShutdownSli(): Disabling Scanline Interleaving...\n"));
        sst1InitSliEnabled = 0;

        cntr = 0;
        while(1) {
            if(sst1InitCheckBoard(sst1InitSliSlaveVirtAddr) == FXFALSE)
                return(FXFALSE);
            /* Remap slave to original physical address  */
            PCICFG_WR(PCI_BASE_ADDRESS_0, sst1CurrentBoard->physAddr);
            /* Turn off scanline interleaving in slave */
            PCICFG_RD(SST1_PCI_INIT_ENABLE, j);
            PCICFG_WR(SST1_PCI_INIT_ENABLE,
                (j | SST_SCANLINE_SLV_OWNPCI | SST_SCANLINE_SLI_SLV)); 
            PCICFG_RD(SST1_PCI_INIT_ENABLE, j); /* delay */
            sstSlave->fbiInit1 &= ~SST_EN_SCANLINE_INTERLEAVE;
            PCICFG_RD(SST1_PCI_INIT_ENABLE, j);
            PCICFG_WR(SST1_PCI_INIT_ENABLE,
                (j & ~(SST_SCANLINE_SLV_OWNPCI | SST_SCANLINE_SLI_SLV))); 
            PCICFG_RD(SST1_PCI_INIT_ENABLE, j); /* delay */
            sst1InitIdle((FxU32 *) sstSlave);


            if(sstSlave->fbiInit1 & SST_EN_SCANLINE_INTERLEAVE) {
                if(++cntr < 10)
                    continue;
                else {
                    INIT_PRINTF(("sst1InitShutdown(): Could not disable Slave SLI...\n"));
                    return(FXFALSE);
                }
            }
            break;
        }
        sstMaster->fbiInit1 &= ~SST_EN_SCANLINE_INTERLEAVE;
        sst1InitIdle((FxU32 *) sstMaster);
    }
    return(FXTRUE);
}

/*
** sst1InitSliPciOwner():
**  Specify which SLI card owns PCI bus control
**
*/
FX_EXPORT FxBool FX_CSTYLE sst1InitSliPciOwner(FxU32 *sstbase, FxU32 owner)
{
    FxU32 j, n;
/*  static FxU32 masterIsOwner = 1; */
    static FxU32 notMasterIsOwner;

    if(!sst1InitSliEnabled)
        return(FXTRUE);

    /* Flush PCI packers... */
    sst1InitReturnStatus(sstbase);
    sst1InitReturnStatus(sstbase);
    sst1InitReturnStatus(sstbase);

    if(!(notMasterIsOwner)) {
        if(owner == SST_SLI_SLAVE_OWNPCI) {
            notMasterIsOwner = 1;
            if(sst1InitCheckBoard(sstbase) == FXFALSE)
                return(FXFALSE);
            PCICFG_RD(SST1_PCI_INIT_ENABLE, j);
            PCICFG_WR(SST1_PCI_INIT_ENABLE, ((j & ~SST_SLI_OWNPCI) |
                SST_SLI_SLAVE_OWNPCI));
            if(sst1InitCheckBoard(sst1InitSliSlaveVirtAddr) == FXFALSE)
                return(FXFALSE);
            PCICFG_RD(SST1_PCI_INIT_ENABLE, j);
            PCICFG_WR(SST1_PCI_INIT_ENABLE, ((j & ~SST_SLI_OWNPCI) |
                SST_SLI_SLAVE_OWNPCI));
        } else
            return(FXTRUE);
    } else {
        if(owner == SST_SLI_MASTER_OWNPCI) {
            notMasterIsOwner = 0;
            if(sst1InitCheckBoard(sst1InitSliSlaveVirtAddr) == FXFALSE)
                return(FXFALSE);
            PCICFG_RD(SST1_PCI_INIT_ENABLE, j);
            PCICFG_WR(SST1_PCI_INIT_ENABLE, ((j & ~SST_SLI_OWNPCI) |
                SST_SLI_MASTER_OWNPCI));
            if(sst1InitCheckBoard(sstbase) == FXFALSE)
                return(FXFALSE);
            PCICFG_RD(SST1_PCI_INIT_ENABLE, j);
            PCICFG_WR(SST1_PCI_INIT_ENABLE, ((j & ~SST_SLI_OWNPCI) |
                SST_SLI_MASTER_OWNPCI));
        } else
            return(FXTRUE);
    }
    return(FXTRUE);
}

/*
** sst1InitSliDetect():
**  Determine if scanline interleave has been detected (either through
**   power-up settings or through SST_SLIDETECT)
**
*/
FX_ENTRY FxU32 FX_CALL sst1InitSliDetect(FxU32 *sstbase)
{
#if !defined(__GNUC__)
/*  static int firstTime = 1;*/
    static int not_first_time;
    static FxU32 sliDetected /*= 0*/;
    FxU32 fbiRev;
    volatile Sstregs *sst;

    if( !(not_first_time) ) {
        not_first_time = 1;
        if(GETENV(("SST_SLIDETECT")))
            sliDetected = ATOI(GETENV(("SST_SLIDETECT")));
        else {
            PCICFG_RD(PCI_REVISION_ID, fbiRev);
            sst = (Sstregs *) sstbase;
            if((sst->fbiInit1 & SST_SLI_DETECT) && (fbiRev > 1) &&
               (boardsInSystem) > 1 &&
               ((sst->fbiInit3 & SST_FBI_MEM_TYPE) >>
                (SST_FBI_MEM_TYPE_SHIFT + 2)))
               sliDetected = 1;
        }
    }
    return(sliDetected);
#else
    return 0;			/* never allowed to do SLI */
#endif
}

#pragma optimize ("",on)
