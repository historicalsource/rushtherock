#ifndef __SST1INIT_H__
#define __SST1INIT_H__

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
** $Revision: 43 $
** $Date: 8/07/96 2:43p $
**
** SST-1 Initialization routine protypes
**
** If all initialization routines are called, it is assumed they are called
** in the following order:
**   1. sst1InitMapBoard();
**   2. sst1InitRegisters();
**   3. sst1InitGamma();
**   4. sst1InitVideo();
**
** sst1InitShutdown() is called at the end of an application to turn off
**  the SST-1 graphics subsystem
**
*/

/* sst1init.h assumes "glide.h" and "sst.h" are already included */

/* Allow SourceSafe to track Revision */
#define INIT_H_REV "$Revision: 43 $"

/* Init code debug print routine */
#ifdef INIT_DOS /* Glide version... */
#define INIT_OUTPUT
#define INIT_PRINTF(a) sst1InitPrintf a
#define INIT_INFO(A)
#endif

#ifndef DIRECTX
#undef GETENV
#undef ATOI
#undef ATOF
#undef SSCANF
#undef POW
#if !FAKE_IO
#define GETENV(A) sst1InitGetenv A
#define ATOI(A) atoi(A)
#define ATOF(A) atof(A)
#define SSCANF( A, B, C ) sscanf( A, B, C )
#else
#define GETENV(A) (0)
#define ATOI(A) (0)
#define ATOF(A) (0.0)
#define SSCANF( A, B, C ) (0)
#endif
#define POW powf
#if NOISY
extern void shims_printf(const char *ctl, ...);
extern void shims_info(int indent, const char *ctl, ...);
# define INIT_PRINTF(x) shims_printf x
# define INIT_INFO(x) shims_info x
#endif

/* Video resolution declarations */
#include "sst1vid.h"

#else /* DIRECTX */
#include "ddglobal.h"
#pragma optimize ("",off)             /* ddglobal.h tuns this on for retail builds */
#undef INIT_PRINTF
#undef INIT_INFO
#undef GETENV
#undef ATOI
#undef ATOF
#undef FTOL
#undef ITOF_INV
#undef SSCANF
#undef POW
/*#define INIT_PRINTF(a) */
#ifdef FXTRACE
  #define INIT_PRINTF DDPRINTF
#else
  #define INIT_PRINTF 1 ? (void) 0 : (void)
#endif
#define INIT_INFO(A)
#define GETENV(A)  ddgetenv(A)
#define ATOI(A) ddatoi(A)
#define ATOF(A) ddatof(A)
#define FTOL(A) ddftol(A)
#define ITOF_INV(A) dd_itof_inv(A)
#define SSCANF( A, B, C ) ddsscanf( A, B, C )
#define POW( A, B ) ddpow( A, B )

#endif /* DIRECTX */

#ifndef _FXPCI_H_
#include <fxpci.h>
#endif

#define MAX_NUM_TMUS 3

/* Following defines need to go in "sst.h" eventually */
/*--------- SST PCI Configuration Command bits -------------- */
#define SST_PCIMEM_ACCESS_EN               BIT(1)

/*------- SST PCI Configuration Register defaults ----------- */
#define SST_PCI_INIT_ENABLE_DEFAULT        0x0
#define SST_PCI_BUS_SNOOP_DEFAULT          0x0

/*--- SST PCI Init Enable Configuration Register defaults --- */
#define SST_SLI_OWNPCI                     SST_SCANLINE_SLV_OWNPCI
#define SST_SLI_MASTER_OWNPCI              0x0
#define SST_SLI_SLAVE_OWNPCI               SST_SCANLINE_SLV_OWNPCI

/*----------------- SST fbiinit0 bits ----------------------- */
#define SST_FBIINIT0_DEFAULT               0x00000411
#define SST_GRX_RESET                      BIT(1)
#define SST_PCI_FIFO_RESET                 BIT(2)

/*----------------- SST fbiinit1 bits ----------------------- */
#define SST_FBIINIT1_DEFAULT               0x00201102
#define SST_SLI_DETECT                     BIT(2)

/*----------------- SST fbiinit2 bits ----------------------- */
#define SST_FBIINIT2_DEFAULT               0x80000040
#define SST_DRAM_REFRESH_16MS              (0x30 << SST_DRAM_REFRESH_CNTR_SHIFT)

/*----------------- SST fbiinit3 bits ----------------------- */
#define SST_FBIINIT3_DEFAULT               0x001E4000
#define SST_TEXMAP_DISABLE                 BIT(6)
#define SST_FBI_MEM_TYPE_SHIFT             8
#define SST_FBI_MEM_TYPE                   (0x7<<SST_FBI_MEM_TYPE_SHIFT)
#define SST_FBI_VGA_PASS_POWERON           BIT(12)
#define SST_FT_CLK_DEL_ADJ_SHIFT           13
#define SST_FT_CLK_DEL_ADJ                 (0xF<<SST_FT_CLK_DEL_ADJ_SHIFT)
#define SST_TF_FIFO_THRESH_SHIFT           17
#define SST_TF_FIFO_THRESH                 (0x1F<<SST_TF_FIFO_THRESH_SHIFT)

/*----------------- SST fbiinit4 bits ----------------------- */
#define SST_FBIINIT4_DEFAULT               0x00000001
#define SST_PCI_RDWS_1                     0x0
#define SST_PCI_RDWS_2                     BIT(0)
#define SST_EN_LFB_RDAHEAD                 BIT(1)
#define SST_MEM_FIFO_LWM_SHIFT             2
#define SST_MEM_FIFO_LWM                   (0x3F<<SST_MEM_FIFO_LWM_SHIFT)
#define SST_MEM_FIFO_ROW_BASE_SHIFT        8
#define SST_MEM_FIFO_ROW_BASE              (0x3FF<<SST_MEM_FIFO_ROW_BASE_SHIFT)
#define SST_MEM_FIFO_ROW_ROLL_SHIFT        18
#define SST_MEM_FIFO_ROW_ROLL              (0x3FF<<SST_MEM_FIFO_ROW_ROLL_SHIFT)

/*----------------- SST trexInit0 bits ----------------------- */
#define SST_EN_TEX_MEM_REFRESH             BIT(0)
#define SST_TEX_MEM_REFRESH_SHIFT          1
#define SST_TEX_MEM_REFRESH                (0x1FF<<SST_TEX_MEM_REFRESH_SHIFT)
#define SST_TEX_MEM_PAGE_SIZE_SHIFT        10
#define SST_TEX_MEM_PAGE_SIZE_8BITS        (0x0<<SST_TEX_MEM_PAGE_SIZE_SHIFT)
#define SST_TEX_MEM_PAGE_SIZE_9BITS        (0x1<<SST_TEX_MEM_PAGE_SIZE_SHIFT)
#define SST_TEX_MEM_PAGE_SIZE_10BITS       (0x2<<SST_TEX_MEM_PAGE_SIZE_SHIFT)
#define SST_TEX_MEM_SECOND_RAS_BIT_SHIFT   12
#define SST_TEX_MEM_SECOND_RAS_BIT_BIT17   (0x0<<SST_TEX_MEM_SECOND_RAS_BIT_SHIFT)
#define SST_TEX_MEM_SECOND_RAS_BIT_BIT18   (0x1<<SST_TEX_MEM_SECOND_RAS_BIT_SHIFT)
#define SST_EN_TEX_MEM_SECOND_RAS          BIT(14)
#define SST_TEX_MEM_TYPE_SHIFT             15
#define SST_TEX_MEM_TYPE_EDO               (0x0<<SST_TEX_MEM_TYPE_SHIFT)
#define SST_TEX_MEM_TYPE_SYNC              (0x1<<SST_TEX_MEM_TYPE_SHIFT)
#define SST_TEX_MEM_DATA_SIZE_16BIT        0x0
#define SST_TEX_MEM_DATA_SIZE_8BIT         BIT(18)
#define SST_TEX_MEM_DO_EXTRA_CAS           BIT(19)
#define SST_TEX_MEM2                       BIT(20)

#define SST_TREXINIT0_DEFAULT \
  ( (SST_EN_TEX_MEM_REFRESH)  \
  | (0x020 << SST_TEX_MEM_REFRESH_SHIFT) \
  | (SST_TEX_MEM_PAGE_SIZE_9BITS) \
  | (SST_TEX_MEM_SECOND_RAS_BIT_BIT18) \
  | (SST_EN_TEX_MEM_SECOND_RAS) \
  | (SST_TEX_MEM_TYPE_EDO) \
  | (SST_TEX_MEM_DATA_SIZE_16BIT) \
  | (0 & SST_TEX_MEM_DO_EXTRA_CAS) \
  | (0 & SST_TEX_MEM2)  )

#define SST_TREX0INIT0_DEFAULT             SST_TREXINIT0_DEFAULT
#define SST_TREX1INIT0_DEFAULT             SST_TREXINIT0_DEFAULT
#define SST_TREX2INIT0_DEFAULT             SST_TREXINIT0_DEFAULT

/*----------------- SST trexInit1 bits ----------------------- */
#define SST_TEX_SCANLINE_INTERLEAVE_MASTER 0x0
#define SST_TEX_SCANLINE_INTERLEAVE_SLAVE  BIT(0)
#define SST_EN_TEX_SCANLINE_INTERLEAVE     BIT(1)
#define SST_TEX_FT_FIFO_SIL_SHIFT          2
#define SST_TEX_FT_FIFO_SIL                (0x1F<<SST_TEX_FT_FIFO_SIL_SHIFT)
#define SST_TEX_TT_FIFO_SIL_SHIFT          7
#define SST_TEX_TT_FIFO_SIL                (0xF<<SST_TEX_TT_FIFO_SIL_SHIFT)
#define SST_TEX_TF_CLK_DEL_ADJ_SHIFT       12
#define SST_TEX_TF_CLK_DEL_ADJ             (0xF<<SST_TEX_TF_CLK_DEL_ADJ_SHIFT)
#define SST_TEX_RG_TTCII_INH               BIT(16)
#define SST_TEX_USE_RG_TTCII_INH           BIT(17)
#define SST_TEX_SEND_CONFIG                BIT(18)
#define SST_TEX_RESET_FIFO                 BIT(19)
#define SST_TEX_RESET_GRX                  BIT(20)
#define SST_TEX_PALETTE_DEL_SHIFT          21
#define SST_TEX_PALETTE_DEL                (0x3<<SST_TEX_PALETTE_DEL_SHIFT)
#define SST_TEX_SEND_CONFIG_SEL_SHIFT      23
#define SST_TEX_SEND_CONFIG_SEL            (0x7<<SST_TEX_SEND_CONFIG_SEL_SHIFT)

/* After things stabilize, the fifo stall inputs levels should be backed off from */
/*      the max. conservative values that are being used now for better performance. */
/* SST_TEX_FT_FIFO_SIL =  ?? */
/* SST_TEX_TT_FIFO_SIL =  ??  (effects multi-trex only) */

/* for trex ver. 1 bringup, SST_TEX_PALETTE_DEL should be set to it's max (== 3) for <50 MHz bringup */

/* TF Clock delays for (FBI Revision, TMU Revision) */
#define SST_TEX_TF_CLK_DEL_ADJ_DEFAULT_R10  0xf
#define SST_TEX_TF_CLK_DEL_ADJ_DEFAULT_R11  0x6
#define SST_TEX_TF_CLK_DEL_ADJ_DEFAULT_R20  0xf
#define SST_TEX_TF_CLK_DEL_ADJ_DEFAULT_R21  0x5

/* FT Clock delays for (FBI Revision, TMU Revision) */
#define SST_FT_CLK_DEL_ADJ_DEFAULT_R10  0xa
#define SST_FT_CLK_DEL_ADJ_DEFAULT_R11  0xa
#define SST_FT_CLK_DEL_ADJ_DEFAULT_R20  0x5
#define SST_FT_CLK_DEL_ADJ_DEFAULT_R21  0x5

#define SST_TREXINIT1_DEFAULT \
  ( (SST_TEX_SCANLINE_INTERLEAVE_MASTER) \
  | (0 & SST_EN_TEX_SCANLINE_INTERLEAVE) \
  | (0x8 << SST_TEX_FT_FIFO_SIL_SHIFT) \
  | (0x8 << SST_TEX_TT_FIFO_SIL_SHIFT) \
  | (0xf << SST_TEX_TF_CLK_DEL_ADJ_SHIFT) \
  | (0 & SST_TEX_RG_TTCII_INH) \
  | (0 & SST_TEX_USE_RG_TTCII_INH) \
  | (0 & SST_TEX_SEND_CONFIG) \
  | (0 & SST_TEX_RESET_FIFO) \
  | (0 & SST_TEX_RESET_GRX) \
  | (0 << SST_TEX_PALETTE_DEL_SHIFT) \
  | (0 << SST_TEX_SEND_CONFIG_SEL_SHIFT) )

#define SST_TREX0INIT1_DEFAULT              SST_TREXINIT1_DEFAULT
#define SST_TREX1INIT1_DEFAULT              SST_TREXINIT1_DEFAULT
#define SST_TREX2INIT1_DEFAULT              SST_TREXINIT1_DEFAULT

/*----------------- SST clutData bits ----------------------- */
#define SST_CLUTDATA_INDEX_SHIFT           24
#define SST_CLUTDATA_RED_SHIFT             16
#define SST_CLUTDATA_GREEN_SHIFT           8
#define SST_CLUTDATA_BLUE_SHIFT            0

/*----------------- SST video setup shifts ------------------ */
#define SST_VIDEO_HSYNC_OFF_SHIFT          16
#define SST_VIDEO_HSYNC_ON_SHIFT           0
#define SST_VIDEO_VSYNC_OFF_SHIFT          16
#define SST_VIDEO_VSYNC_ON_SHIFT           0
#define SST_VIDEO_HBACKPORCH_SHIFT         0
#define SST_VIDEO_VBACKPORCH_SHIFT         16
#define SST_VIDEO_XDIM_SHIFT               0
#define SST_VIDEO_YDIM_SHIFT               16

/*----------------- SST dacData constants ------------------- */
#define SST_DACREG_WMA                     0x0
#define SST_DACREG_LUT                     0x1
#define SST_DACREG_RMR                     0x2
#define SST_DACREG_RMA                     0x3
#define SST_DACREG_ICS_PLLADDR_WR          0x4 /* ICS only */
#define SST_DACREG_ICS_PLLADDR_RD          0x7 /* ICS only */
#define SST_DACREG_ICS_PLLADDR_DATA        0x5 /* ICS only */
#define SST_DACREG_ICS_CMD                 0x6 /* ICS only */
#define SST_DACREG_ICS_COLORMODE_16BPP     0x50 /* ICS only */
#define SST_DACREG_ICS_COLORMODE_24BPP     0x70 /* ICS only */
#define SST_DACREG_ICS_PLLADDR_VCLK0       0x0 /* ICS only */
#define SST_DACREG_ICS_PLLADDR_VCLK1       0x1 /* ICS only */
#define SST_DACREG_ICS_PLLADDR_VCLK7       0x7 /* ICS only */
#define SST_DACREG_ICS_PLLADDR_VCLK1_DEFAULT 0x55 /* ICS only */
#define SST_DACREG_ICS_PLLADDR_VCLK7_DEFAULT 0x71 /* ICS only */
#define SST_DACREG_ICS_PLLADDR_GCLK0       0xa /* ICS only */
#define SST_DACREG_ICS_PLLADDR_GCLK1       0xb /* ICS only */
#define SST_DACREG_ICS_PLLADDR_GCLK1_DEFAULT 0x79 /* ICS only */
#define SST_DACREG_ICS_PLLADDR_CTRL        0xe /* ICS only */
#define SST_DACREG_ICS_PLLCTRL_CLK1SEL     BIT(4)
#define SST_DACREG_ICS_PLLCTRL_CLK0SEL     BIT(5)
#define SST_DACREG_ICS_PLLCTRL_CLK0FREQ    0x7
#define SST_DACREG_INDEXADDR               SST_DACREG_WMA
#define SST_DACREG_INDEXDATA               SST_DACREG_RMR
#define SST_DACREG_INDEX_RMR               0x0
#define SST_DACREG_INDEX_CR0               0x1
#define SST_DACREG_INDEX_MIR               0x2
#define SST_DACREG_INDEX_MIR_ATT_DEFAULT   0x84   /* AT&T */
#define SST_DACREG_INDEX_MIR_TI_DEFAULT    0x97   /* TI */
#define SST_DACREG_INDEX_DIR               0x3
#define SST_DACREG_INDEX_DIR_ATT_DEFAULT   0x9    /* AT&T */
#define SST_DACREG_INDEX_DIR_TI_DEFAULT    0x9    /* TI */
#define SST_DACREG_INDEX_TST               0x4
#define SST_DACREG_INDEX_CR1               0x5
#define SST_DACREG_INDEX_CC                0x6
#define SST_DACREG_INDEX_AA0               0xff  /* can't access */
#define SST_DACREG_INDEX_AA1               0xff  /* can't access */
#define SST_DACREG_INDEX_AB0               0xff  /* can't access */
#define SST_DACREG_INDEX_AB1               0xff  /* can't access */
#define SST_DACREG_INDEX_AB2               0xff  /* can't access */
#define SST_DACREG_INDEX_AC0               0x48
#define SST_DACREG_INDEX_AC1               0x49
#define SST_DACREG_INDEX_AC2               0x4a
#define SST_DACREG_INDEX_AD0               0x4c
#define SST_DACREG_INDEX_AD1               0x4d
#define SST_DACREG_INDEX_AD2               0x4e
#define SST_DACREG_INDEX_BA0               0xff  /* can't access */
#define SST_DACREG_INDEX_BA1               0xff  /* can't access */
#define SST_DACREG_INDEX_BB0               0xff  /* can't access */
#define SST_DACREG_INDEX_BB1               0xff  /* can't access */
#define SST_DACREG_INDEX_BB2               0xff  /* can't access */
#define SST_DACREG_INDEX_BC0               0xff  /* can't access */
#define SST_DACREG_INDEX_BC1               0xff  /* can't access */
#define SST_DACREG_INDEX_BC2               0xff  /* can't access */
#define SST_DACREG_INDEX_BD0               0x6c
#define SST_DACREG_INDEX_BD1               0x6d
#define SST_DACREG_INDEX_BD2               0x6e

#define SST_DACREG_CR0_INDEXED_ADDRESSING  BIT(0)
#define SST_DACREG_CR0_8BITDAC             BIT(1)
#define SST_DACREG_CR0_SLEEP               BIT(3)
#define SST_DACREG_CR0_COLOR_MODE_SHIFT    4
#define SST_DACREG_CR0_COLOR_MODE         (0xF<<SST_DACREG_CR0_COLOR_MODE_SHIFT)
#define SST_DACREG_CR0_COLOR_MODE_16BPP   (0x3<<SST_DACREG_CR0_COLOR_MODE_SHIFT)
#define SST_DACREG_CR0_COLOR_MODE_24BPP   (0x5<<SST_DACREG_CR0_COLOR_MODE_SHIFT)

#define SST_DACREG_CR1_BLANK_PEDASTAL_EN  BIT(4)

#define SST_DACREG_CC_BCLK_SEL_SHIFT       0
#define SST_DACREG_CC_BCLK_SELECT_BD       BIT(3)
#define SST_DACREG_CC_ACLK_SEL_SHIFT       4
#define SST_DACREG_CC_ACLK_SELECT_AD       BIT(7)

#define SST_DACREG_CLKREG_MSHIFT           0
#define SST_DACREG_CLKREG_PSHIFT           6
#define SST_DACREG_CLKREG_NSHIFT           0
#define SST_DACREG_CLKREG_LSHIFT           4
#define SST_DACREG_CLKREG_IBSHIFT          0

#define SST_FBI_DACTYPE_ATT                0
#define SST_FBI_DACTYPE_ICS                1
#define SST_FBI_DACTYPE_TI                 2

/* Definitions for parsing voodoo.ini file */
#define DACRDWR_TYPE_WR                    0
#define DACRDWR_TYPE_RDMODWR               1
#define DACRDWR_TYPE_RDNOCHECK             2
#define DACRDWR_TYPE_RDCHECK               3
#define DACRDWR_TYPE_RDPUSH                4
#define DACRDWR_TYPE_WRMOD_POP             5

/* Other useful defines */
#define PCICFG_WR(ADDR, DATA)                                         \
    n = DATA;                                                         \
    if(pciSetConfigData(ADDR, sst1InitDeviceNumber, &n) == FXFALSE)   \
        return(FXFALSE)
#define PCICFG_RD(ADDR, DATA)                                          \
    if(pciGetConfigData(ADDR, sst1InitDeviceNumber, &DATA) == FXFALSE) \
        return(FXFALSE)
#define DAC_INDEXWRADDR(ADDR)                                         \
    sst1InitDacWr(sstbase, SST_DACREG_INDEXADDR, ADDR)
#define DAC_INDEXWR(DATA)                                             \
    sst1InitDacWr(sstbase, SST_DACREG_INDEXDATA, (DATA))
#define DAC_INDEXRD()                                                 \
    sst1InitDacRd(sstbase, SST_DACREG_INDEXDATA)
#define SLI_DRIVEOK()                                                 \
      (!(sst1CurrentBoard->numBoardInsystem & 0x1) ||                 \
       ((sst1CurrentBoard->numBoardInsystem & 0x1) &&                 \
        (sst1InitSliDetect(sstbase) == 0)))

/*----------------------------------------------------------- */

/*
** SST-1 Initialization typedefs
**
*/
typedef struct {
    float freq;
    FxU32 clkTiming_M;
    FxU32 clkTiming_P;
    FxU32 clkTiming_N;
    FxU32 clkTiming_L;
    FxU32 clkTiming_IB;
} sst1ClkTimingStruct;

typedef struct {
    FxU32 hSyncOn;
    FxU32 hSyncOff;
    FxU32 vSyncOn;
    FxU32 vSyncOff;
    FxU32 hBackPorch;
    FxU32 vBackPorch;
    FxU32 xDimension;
    FxU32 yDimension;
    FxU32 memOffset;
    FxU32 memFifoEntries_1MB;
    FxU32 memFifoEntries_2MB;
    FxU32 memFifoEntries_4MB;
    FxU32 tilesInX_Over2;
    FxU32 vFifoThreshold;
    FxBool video16BPPIsOK;
    FxBool video24BPPIsOK;
    float clkFreq16bpp;
    float clkFreq24bpp;
} sst1VideoTimingStruct;

typedef struct {
    FxU32 *virtAddr;        /* virtual memory base address */
    FxU32 physAddr;         /* physical memory base address */
    FxU32 deviceNumber;     /* PCI device number */
    FxU32 vendorID;         /* PCI vendor ID */
    FxU32 deviceID;         /* PCI device ID */
    FxU32 fbiRevision;      /* FBI revision number */
    FxU32 fbiBoardID;       /* FBI board ID (poweron strapping bits) */
    FxU32 fbiVideo16BPP;    /* FBI video display mode */
    FxU32 fbiVideoWidth;    /* FBI video display X-resolution */
    FxU32 fbiVideoHeight;   /* FBI video display Y-resolution */
    FxU32 fbiMemoryFifoEn;  /* FBI memory fifo enabled */
    FxU32 sstSliDetect;     /* Scanline interleave detected */
    FxU32 tmuRevision;      /* TMU revision number (for all TMUs) */
    FxU32 numberTmus;       /* number of TMUs installed */
    FxU32 tmuConfig;        /* TMU configuration bits */
    FxU32 fbiMemSize;       /* FBI frame buffer memory (in MBytes) */
    FxU32 tmuMemSize[MAX_NUM_TMUS]; /* TMU texture memory (in MBytes) */

    /* These cannot be read from the hardware, so we shadow them here */
    FxU32 tmuInit0[MAX_NUM_TMUS];
    FxU32 tmuInit1[MAX_NUM_TMUS];

    /* Misc */
    FxU32 initGrxClkDone;
    FxU32 vgaPassthruDisable;
    FxU32 vgaPassthruEnable;
    FxU32 fbiDacType;
    FxU32 memFifoStatusLwm;
    FxU32 numBoardInsystem;
    FxU32 grxClkFreq;
    FxU32 fbiMemSpeed;
    FxU32 tmuMemSpeed;
    FxU32 tmuClkFixed;
    FxU32 fbiConfig;        /* FBI strapping pins */
    FxU32 fbiVideoRefresh;  /* FBI video refresh rate */
    FxU32 fbiRegulatorPresent;  /* FBI voltage regulator detected */
} sst1DeviceInfoStruct;

typedef struct {
    unsigned char type;
    unsigned char addr;
    FxU32 data;
    FxU32 mask;
    void *nextRdWr;
} sst1InitDacRdWrStruct;

typedef struct {
    FxU32 width;
    FxU32 height;
    FxU32 refresh;
    FxU32 video16BPP;
    sst1InitDacRdWrStruct *setVideoRdWr;
    void *nextSetVideo;
} sst1InitDacSetVideoStruct;

typedef struct {
    FxU32 frequency;
    sst1InitDacRdWrStruct *setMemClkRdWr;
    void *nextSetMemClk;
} sst1InitDacSetMemClkStruct;

typedef struct {
    FxU32 video16BPP;
    sst1InitDacRdWrStruct *setVideoModeRdWr;
    void *nextSetVideoMode;
} sst1InitDacSetVideoModeStruct;

typedef struct {
    char dacManufacturer[100];
    char dacDevice[100];
    sst1InitDacRdWrStruct *detect;
    sst1InitDacSetVideoStruct *setVideo;
    sst1InitDacSetMemClkStruct *setMemClk;
    sst1InitDacSetVideoModeStruct *setVideoMode;
    void *nextDac;
} sst1InitDacStruct;

typedef struct {
    char envVariable[100];
    char envValue[256];
    void *nextVar;
} sst1InitEnvVarStruct;

extern FxU32 *  sst1InitMapBoard(FxU32);
extern FxBool  sst1InitRegisters(FxU32 *);
extern FxBool  sst1InitGamma(FxU32 *, double);
extern FxBool  sst1InitGammaRGB(FxU32 *, double, double, double);
extern FxBool  sst1InitVideo(FxU32 *, GrScreenResolution_t,
  GrScreenRefresh_t, const sst1VideoTimingStruct *);
extern FxBool  sst1InitShutdown(FxU32 *);
extern FxBool  sst1InitShutdownSli(FxU32 *);
extern FxBool  sst1InitSli(FxU32 *, FxU32 *);
extern FxBool  sst1InitGetDeviceInfo(FxU32 *, sst1DeviceInfoStruct *);

/* Miscellaneous routines */
extern FxBool  sst1InitIdle(FxU32 *);
extern FxBool  sst1InitIdleFBI(FxU32 *);
extern FxBool  sst1InitIdleFBINoNOP(FxU32 *);
extern FxU32   sst1InitReturnStatus(FxU32 *);
extern FxU32  sst1InitDacRd(FxU32 *, FxU32);
extern void  sst1InitDacWr(FxU32 *, FxU32, FxU32);
FxBool sst1InitExecuteDacRdWr(FxU32 *, sst1InitDacRdWrStruct *);
extern void  sst1InitSetResolution(FxU32 *, const sst1VideoTimingStruct *,
    FxU32);
extern FxBool  sst1InitDacIndexedEnable(FxU32 *, FxU32);
extern FxBool  sst1InitGrxClk(FxU32 *);
extern FxBool  sst1InitCalcGrxClk(FxU32 *);
extern FxBool  sst1InitDacDetect(FxU32 *);
FxBool sst1InitDacDetectATT(FxU32 *);
FxBool sst1InitDacDetectTI(FxU32 *);
FxBool sst1InitDacDetectICS(FxU32 *);
FxBool sst1InitDacDetectINI(FxU32 *);
extern FxBool  sst1InitSetGrxClk(FxU32 *, sst1ClkTimingStruct *);
FxBool sst1InitComputeClkParams(float, sst1ClkTimingStruct *);
FxBool sst1InitSetGrxClkATT(FxU32 *, sst1ClkTimingStruct *);
FxBool sst1InitSetGrxClkICS(FxU32 *, sst1ClkTimingStruct *);
FxBool sst1InitSetGrxClkINI(FxU32 *, sst1ClkTimingStruct *);
extern FxBool  sst1InitSetVidClk(FxU32 *, float);
FxBool sst1InitSetVidClkATT(FxU32 *, sst1ClkTimingStruct *);
FxBool sst1InitSetVidClkICS(FxU32 *, sst1ClkTimingStruct *);
FxBool sst1InitSetVidClkINI(FxU32 *, FxU32, FxU32, FxU32, FxU32);
FxBool sst1InitSetVidMode(FxU32 *, FxU32);
FxBool sst1InitSetVidModeATT(FxU32 *, FxU32);
FxBool sst1InitSetVidModeICS(FxU32 *, FxU32);
FxBool sst1InitSetVidModeINI(FxU32 *, FxU32);
extern FxBool  sst1InitCheckBoard(FxU32 *);
extern FxBool  sst1InitGetFbiInfo(FxU32 *, sst1DeviceInfoStruct *);
extern FxBool  sst1InitGetTmuInfo(FxU32 *, sst1DeviceInfoStruct *);
extern void  sst1InitRenderingRegisters(FxU32 *);
extern FxBool  sst1InitGetTmuMemory(FxU32 *sstbase,
    sst1DeviceInfoStruct *info, FxU32 tmu, FxU32 *TmuMemorySize);
extern FxBool  sst1InitClearSwapPending(FxU32 *);
extern FxBool  sst1InitVgaPassCtrl(FxU32 *, FxU32);
FxBool sst1InitResetTmus(FxU32 *);
extern FxU32  sst1InitSliDetect(FxU32 *);
extern FxBool  sst1InitSliPciOwner(FxU32 *, FxU32);
extern FxBool  sst1InitVoodooFile(void);
extern char *  sst1InitGetenv(char *);
extern FxU32 *  sst1InitGetBaseAddr(FxU32);
FxBool sst1InitFillDeviceInfo(FxU32 *, sst1DeviceInfoStruct *);
void sst1InitIdleLoop(FxU32 *);
void sst1InitClearBoardInfo(void);

/* Info/Print routines */
#ifdef INIT_OUTPUT
extern void  sst1InitPrintf(const char *, ...);
#ifndef _FILE_DEFINED
#include <stdio.h>
#endif
#ifdef SST1INIT_ALLOCATE
FILE *sst1InitMsgFile /*= stdout*/;
#else
extern FILE *sst1InitMsgFile;
#endif
#endif

/* Maximum number of SST-1 boards supported in system */
#define SST1INIT_MAX_BOARDS 16

/* Maximum number of read pushes in "voodoo.ini" file */
#define DACRDWR_MAX_PUSH    16

#ifdef SST1INIT_ALLOCATE
# if 0
  const static char headersIdent[] = "@#%InitHeaders $Revision: 43 $";
# endif
  FxBool sst1InitUseVoodooFile /*= FXFALSE*/;
  sst1InitEnvVarStruct *envVarsBase /*= (sst1InitEnvVarStruct *) NULL*/;
  sst1InitDacStruct *dacStructBase /*= (sst1InitDacStruct *) NULL*/;
  sst1InitDacStruct *iniDac /*= (sst1InitDacStruct *) NULL*/;
  sst1InitDacSetVideoStruct *iniVideo /*= (sst1InitDacSetVideoStruct *) NULL*/;
  sst1InitDacSetMemClkStruct *iniMemClk /*= (sst1InitDacSetMemClkStruct *) NULL*/;
  FxU32 iniStack[DACRDWR_MAX_PUSH];
  int iniStackPtr /*= 0*/;
  sst1DeviceInfoStruct *sst1CurrentBoard;
  FxU32 sst1InitDeviceNumber;
  sst1DeviceInfoStruct sst1BoardInfo[SST1INIT_MAX_BOARDS];
  FxU32 boardsFound /*= 0*/;
  FxU32 boardsInSystem;
  FxU32 sst1InitSliEnabled /*= 0*/;
  FxU32 *sst1InitSliSlaveVirtAddr;
  const PciRegister SST1_PCI_INIT_ENABLE   = { 0x40, 4, READ_WRITE };
  const PciRegister SST1_PCI_BUS_SNOOP0    = { 0x44, 4, WRITE_ONLY };
  const PciRegister SST1_PCI_BUS_SNOOP1    = { 0x48, 4, WRITE_ONLY };
  const PciRegister SST1_PCI_CFG_STATUS    = { 0x4c, 4, READ_ONLY };
  const PciRegister SST1_PCI_VCLK_ENABLE   = { 0xc0, 4, WRITE_ONLY };
  const PciRegister SST1_PCI_VCLK_DISABLE  = { 0xe0, 4, WRITE_ONLY };

#else
  extern FxBool sst1InitUseVoodooFile;
  extern sst1InitEnvVarStruct *envVarsBase;
  extern sst1InitDacStruct *dacStructBase;
  extern sst1InitDacStruct *iniDac;
  extern sst1InitDacSetVideoStruct *iniVideo;
  extern sst1InitDacSetMemClkStruct *iniMemClk;
  extern FxU32 iniStack[];
  extern int iniStackPtr;
  extern sst1DeviceInfoStruct *sst1CurrentBoard;
  extern FxU32 sst1InitDeviceNumber;
  extern sst1DeviceInfoStruct sst1BoardInfo[SST1INIT_MAX_BOARDS];
  extern FxU32 boardsFound;
  extern FxU32 boardsInSystem;
  extern FxU32 sst1InitSliEnabled;
  extern FxU32 *sst1InitSliSlaveVirtAddr;
  extern PciRegister SST1_PCI_INIT_ENABLE;
  extern PciRegister SST1_PCI_BUS_SNOOP0;
  extern PciRegister SST1_PCI_BUS_SNOOP1;
  extern PciRegister SST1_PCI_CFG_STATUS;
  extern PciRegister SST1_PCI_VCLK_ENABLE;
  extern PciRegister SST1_PCI_VCLK_DISABLE;
#endif

#ifdef SST1INIT_VIDEO_ALLOCATE
/* SST1INIT_VIDEO_ALLOCATE is only #defined in video.c */
/* */
/* Define useful clock and video timings */
/* Clocks generated are follows: */
/*   Clock Freq. (MHz) = */
/*     [14.318 * (clkTiming_M+2)] / [(clkTiming_N+2) * (2^clkTiming_P)] */
/* */
/* Solving for clkTiming_M yields: */
/*   clkTiming_M = */
/*     [ [(Clock Freq (Mhz)) * (clkTiming_N+2) * (2^clkTiming_P)] / 14.318 ] - 2 */
/*    */
/* NOTE: [14.318 * (clkTiming_M+2)] / (clkTiming_N+2) should be between */
/*  120 and 240 */
/* NOTE: Max. M is 127 */
/* NOTE: Max. N is 31 */
/* NOTE: Max. P is 3 */
/* NOTE: Max. L is 15 */
/* NOTE: Max. IB is 15 */
/* */

/* 512x256@60 only syncs to Arcade-style monitors */
const sst1VideoTimingStruct SST_VREZ_512X256_60 = {
    39,        /* hSyncOn */
    605,       /* hSyncOff */
    4,         /* vSyncOn */
    276,       /* vSyncOff */
    74,        /* hBackPorch */
    13,        /* vBackPorch */
    512,       /* xDimension */
    256,       /* yDimension */
    64,        /* memOffset */
    0x410,     /* memFifoEntries_1MB  ... 32256 entries in memory fifo (no Z) */
    0x100,     /* memFifoEntries_2MB  ... 57344 entries in memory fifo */
    0x100,     /* memFifoEntries_4MB  ... 57344 entries in memory fifo */
    8,         /* tilesInX_Over2 */
    23,        /* vFifoThreshold */
    FXFALSE,   /* video16BPPIsOK */
    FXTRUE,    /* video24BPPIsOK */
    20.66F,    /* clkFreq16bpp */
    20.66F     /* clkFreq24bpp */
};

/* 336x240@60 only syncs to Arcade-style monitors */
const sst1VideoTimingStruct SST_VREZ_336X240_60 = {
    34,        /* hSyncOn */
    422,       /* hSyncOff */
    4,         /* vSyncOn */
    276,       /* vSyncOff */
    68,        /* hBackPorch */
    13,        /* vBackPorch */
    336,       /* xDimension */
    240,       /* yDimension */
    64,        /* memOffset */
    0x410,     /* memFifoEntries_1MB  ... 32256 entries in memory fifo (no Z) */
    0x100,     /* memFifoEntries_2MB  ... 57344 entries in memory fifo */
    0x100,     /* memFifoEntries_4MB  ... 57344 entries in memory fifo */
    8,         /* tilesInX_Over2 */
    23,        /* vFifoThreshold */
    FXFALSE,   /* video16BPPIsOK */
    FXTRUE,    /* video24BPPIsOK */
    16.0F,    /* clkFreq16bpp */
    16.0F     /* clkFreq24bpp */
};

/* 400x240@60 only syncs to Arcade-style monitors */
const sst1VideoTimingStruct SST_VREZ_400X240_60 = {
    41,        /* hSyncOn */
    502,       /* hSyncOff */
    4,         /* vSyncOn */
    276,       /* vSyncOff */
    81,        /* hBackPorch */
    13,        /* vBackPorch */
    400,       /* xDimension */
    240,       /* yDimension */
    64,        /* memOffset */
    0x410,     /* memFifoEntries_1MB  ... 32256 entries in memory fifo (no Z) */
    0x100,     /* memFifoEntries_2MB  ... 57344 entries in memory fifo */
    0x100,     /* memFifoEntries_4MB  ... 57344 entries in memory fifo */
    8,         /* tilesInX_Over2 */
    23,        /* vFifoThreshold */
    FXFALSE,   /* video16BPPIsOK */
    FXTRUE,    /* video24BPPIsOK */
    19.0F,    /* clkFreq16bpp */
    19.0F     /* clkFreq24bpp */
};

/* 512x256@60 50" monitor */
const sst1VideoTimingStruct SST_VREZ_512X256_60_50i = {
    42,        /* hSyncOn */
    665,       /* hSyncOff */
    4,         /* vSyncOn */
    278,       /* vSyncOff */
    86,        /* hBackPorch */
    16,        /* vBackPorch */
    512,       /* xDimension */
    256,       /* yDimension */
    64,        /* memOffset */
    0x410,     /* memFifoEntries_1MB  ... 32256 entries in memory fifo (no Z) */
    0x100,     /* memFifoEntries_2MB  ... 57344 entries in memory fifo */
    0x100,     /* memFifoEntries_4MB  ... 57344 entries in memory fifo */
    8,         /* tilesInX_Over2 */
    23,        /* vFifoThreshold */
    FXFALSE,   /* video16BPPIsOK */
    FXTRUE,    /* video24BPPIsOK */
    22.62F,    /* clkFreq16bpp */
    22.62F     /* clkFreq24bpp */
};

/* 512x384@60 only syncs to Arcade-style monitors */
const sst1VideoTimingStruct SST_VREZ_512X384_60 = {
#if DMS_TIMING
    63,        /* hSyncOn */
    575,       /* hSyncOff */
    5,         /* vSyncOn */
    411,       /* vSyncOff */
    32,        /* hBackPorch (s/b 49) */
    25,        /* vBackPorch */
    512,       /* xDimension */
    384,       /* yDimension */
    96,        /* memOffset */
    0x0,       /* memFifoEntries_1MB  ... 32256 entries in memory fifo (no Z) */
    0x100,     /* memFifoEntries_2MB  ... 57344 entries in memory fifo */
    0x100,     /* memFifoEntries_4MB  ... 57344 entries in memory fifo */
    8,         /* tilesInX_Over2 */
    23,        /* vFifoThreshold */
    FXTRUE,    /* video16BPPIsOK */
    FXTRUE,    /* video24BPPIsOK */
    16.0F,     /* clkFreq16bpp */
    32.0F      /* clkFreq24bpp */
#else
    23,        /* hSyncOn */
    640,       /* hSyncOff */
    3,         /* vSyncOn */
    411,       /* vSyncOff */
# if !SDMS_TIMING
    90,        /* hBackPorch */
# else
    73,        /* hBackPorch */
# endif
    24,        /* vBackPorch */
    512,       /* xDimension */
    384,       /* yDimension */
    96,        /* memOffset */
    0x410,     /* memFifoEntries_1MB  ... 32256 entries in memory fifo (no Z) */
    0x100,     /* memFifoEntries_2MB  ... 57344 entries in memory fifo */
    0x100,     /* memFifoEntries_4MB  ... 57344 entries in memory fifo */
    8,         /* tilesInX_Over2 */
    23,        /* vFifoThreshold */
    FXFALSE,   /* video16BPPIsOK */
    FXTRUE,    /* video24BPPIsOK */
    33.0F,     /* clkFreq16bpp */
    33.0F      /* clkFreq24bpp */
#endif
};

const sst1VideoTimingStruct SST_VREZ_512X384_72 = {
    38,        /* hSyncOn */
    610,       /* hSyncOff */
    2,         /* vSyncOn */
    415,       /* vSyncOff */
    55,        /* hBackPorch */
    18,        /* vBackPorch */
    512,       /* xDimension */
    384,       /* yDimension */
    96,        /* memOffset */
    0x410,     /* memFifoEntries_1MB  ... 32256 entries in memory fifo (no Z) */
    0x100,     /* memFifoEntries_2MB  ... 57344 entries in memory fifo */
    0x100,     /* memFifoEntries_4MB  ... 57344 entries in memory fifo */
    8,         /* tilesInX_Over2 */
    23,        /* vFifoThreshold */
    FXTRUE,    /* video16BPPIsOK */
    FXTRUE,    /* video24BPPIsOK */
    20.0F,     /* clkFreq16bpp .. 72 Hz refresh rate */
    40.0F      /* clkFreq24bpp */
};

const sst1VideoTimingStruct SST_VREZ_512X384_75 = {
    45,        /* hSyncOn */
    610,       /* hSyncOff */
    1,         /* vSyncOn */
    418,       /* vSyncOff */
    55,        /* hBackPorch */
    25,        /* vBackPorch */
    512,       /* xDimension */
    384,       /* yDimension */
    96,        /* memOffset */
    0x410,     /* memFifoEntries_1MB  ... 32256 entries in memory fifo (no Z) */
    0x100,     /* memFifoEntries_2MB  ... 57344 entries in memory fifo */
    0x100,     /* memFifoEntries_4MB  ... 57344 entries in memory fifo */
    8,         /* tilesInX_Over2 */
    23,        /* vFifoThreshold */
    FXTRUE,    /* video16BPPIsOK */
    FXTRUE,    /* video24BPPIsOK */
    20.5F,     /* clkFreq16bpp .. 72 Hz refresh rate */
    41.0F      /* clkFreq24bpp */
};

const sst1VideoTimingStruct SST_VREZ_512X384_85 = {
    45,        /* hSyncOn */
    610,       /* hSyncOff */
    1,         /* vSyncOn */
    418,       /* vSyncOff */
    80,        /* hBackPorch */
    25,        /* vBackPorch */
    512,       /* xDimension */
    384,       /* yDimension */
    96,        /* memOffset */
    0x410,     /* memFifoEntries_1MB  ... 32256 entries in memory fifo (no Z) */
    0x100,     /* memFifoEntries_2MB  ... 57344 entries in memory fifo */
    0x100,     /* memFifoEntries_4MB  ... 57344 entries in memory fifo */
    8,         /* tilesInX_Over2 */
    23,        /* vFifoThreshold */
    FXTRUE,    /* video16BPPIsOK */
    FXTRUE,    /* video24BPPIsOK */
    23.2F,     /* clkFreq16bpp .. 72 Hz refresh rate */
    46.5F      /* clkFreq24bpp */
};

const sst1VideoTimingStruct SST_VREZ_512X384_120 = {
    25,        /* hSyncOn */
    660,       /* hSyncOff */
    1,         /* vSyncOn */
    418,       /* vSyncOff */
    110,        /* hBackPorch */
    25,        /* vBackPorch */
    512,       /* xDimension */
    384,       /* yDimension */
    96,        /* memOffset */
    0x410,     /* memFifoEntries_1MB  ... 32256 entries in memory fifo (no Z) */
    0x100,     /* memFifoEntries_2MB  ... 57344 entries in memory fifo */
    0x100,     /* memFifoEntries_4MB  ... 57344 entries in memory fifo */
    8,         /* tilesInX_Over2 */
    20,        /* vFifoThreshold */
    FXTRUE,    /* video16BPPIsOK */
    FXTRUE,    /* video24BPPIsOK */
    32.8F,     /* clkFreq16bpp .. 72 Hz refresh rate */
    65.5F      /* clkFreq24bpp */
};

const sst1VideoTimingStruct SST_VREZ_512X400_60 = {	/* Midway's monitors */
    25,        /* hSyncOn */
    155,       /* hSyncOff */
    3,         /* vSyncOn */
    33,        /* vSyncOff */
    114,       /* hBackPorch */
    27,        /* vBackPorch */
    512,       /* xDimension */
    400,       /* yDimension */
    100,       /* memOffset */
    0,         /* memFifoEntries_1MB */
    0x100,     /* memFifoEntries_2MB */
    0x100,     /* memFifoEntries_4MB */
    8,         /* tilesInX_Over2 */
    16,        /* vFifoThreshold */
    FXTRUE,    /* video16BPPIsOK */
    FXTRUE,    /* video24BPPIsOK */
    12.80f,    /* clkFreq16bpp */
    25.60f     /* clkFreq24bpp */
};

const sst1VideoTimingStruct SST_VREZ_640X400_60 = {
    96,        /* hSyncOn */
    704,       /* hSyncOff */
    30,        /* vSyncOn */
    465,       /* vSyncOff */
    35,        /* hBackPorch */
    25,        /* vBackPorch */
    640,       /* xDimension */
    400,       /* yDimension */
    150,       /* memOffset */
    0x0,       /* memFifoEntries_1MB  ... memory fifo not supported */
    0x430,     /* memFifoEntries_2MB  ... 31232 entries in memory fifo */
    0x100,     /* memFifoEntries_4MB  ... 57344 entries in memory fifo */
    10,        /* tilesInX_Over2 */
    21,        /* vFifoThreshold */
    FXTRUE,    /* video16BPPIsOK */
    FXTRUE,    /* video24BPPIsOK */
    24.5F,     /* clkFreq16bpp */
    49.1F      /* clkFreq24bpp  */
};

const sst1VideoTimingStruct SST_VREZ_640X400_75 = {
    75,        /* hSyncOn */
    730,       /* hSyncOff */
    20,        /* vSyncOn */
    475,       /* vSyncOff */
    45,        /* hBackPorch */
    15,        /* vBackPorch */
    640,       /* xDimension */
    400,       /* yDimension */
    150,       /* memOffset */
    0x0,       /* memFifoEntries_1MB  ... memory fifo not supported */
    0x430,     /* memFifoEntries_2MB  ... 31232 entries in memory fifo */
    0x100,     /* memFifoEntries_4MB  ... 57344 entries in memory fifo */
    10,        /* tilesInX_Over2 */
    19,        /* vFifoThreshold */
    FXTRUE,    /* video16BPPIsOK */
    FXTRUE,    /* video24BPPIsOK */
    30.7F,     /* clkFreq16bpp */
    61.4F      /* clkFreq24bpp  */
};

const sst1VideoTimingStruct SST_VREZ_640X400_85 = {
    90,        /* hSyncOn */
    800,       /* hSyncOff */
    20,        /* vSyncOn */
    425,       /* vSyncOff */
    105,       /* hBackPorch */
    15,        /* vBackPorch */
    640,       /* xDimension */
    400,       /* yDimension */
    150,       /* memOffset */
    0x0,       /* memFifoEntries_1MB  ... memory fifo not supported */
    0x430,     /* memFifoEntries_2MB  ... 31232 entries in memory fifo */
    0x100,     /* memFifoEntries_4MB  ... 57344 entries in memory fifo */
    10,        /* tilesInX_Over2 */
    18,        /* vFifoThreshold */
    FXTRUE,    /* video16BPPIsOK */
    FXTRUE,    /* video24BPPIsOK */
    34.8F,     /* clkFreq16bpp */
    69.6F      /* clkFreq24bpp  */
};

const sst1VideoTimingStruct SST_VREZ_640X400_120 = {
    90,        /* hSyncOn */
    800,       /* hSyncOff */
    20,        /* vSyncOn */
    425,       /* vSyncOff */
    105,       /* hBackPorch */
    15,        /* vBackPorch */
    640,       /* xDimension */
    400,       /* yDimension */
    150,       /* memOffset */
    0x0,       /* memFifoEntries_1MB  ... memory fifo not supported */
    0x430,     /* memFifoEntries_2MB  ... 31232 entries in memory fifo */
    0x100,     /* memFifoEntries_4MB  ... 57344 entries in memory fifo */
    10,        /* tilesInX_Over2 */
    18,        /* vFifoThreshold */
    FXTRUE,    /* video16BPPIsOK */
    FXTRUE,    /* video24BPPIsOK */
    49.1F,     /* clkFreq16bpp */
    98.3F      /* clkFreq24bpp  */
};

const sst1VideoTimingStruct SST_VREZ_640X480_60 = {
    96,        /* hSyncOn */
    704,       /* hSyncOff */
    2,         /* vSyncOn */
    523,       /* vSyncOff */
    50,        /* hBackPorch */
    18,        /* vBackPorch */
    640,       /* xDimension */
    480,       /* yDimension */
    150,       /* memOffset */
    0x0,       /* memFifoEntries_1MB  ... unsupported resolution */
    0x430,     /* memFifoEntries_2MB  ... 31232 entries in memory fifo */
    0x100,     /* memFifoEntries_4MB  ... 57344 entries in memory fifo */
    10,        /* tilesInX_Over2 */
    23,        /* vFifoThreshold */
    FXTRUE,    /* video16BPPIsOK */
    FXTRUE,    /* video24BPPIsOK */
    25.175F,   /* clkFreq16bpp */
    50.350F    /* clkFreq24bpp  */
};

const sst1VideoTimingStruct SST_VREZ_640X480_75 = {
    64,        /* hSyncOn */
    750,       /* hSyncOff */
    3,         /* vSyncOn */
    503,       /* vSyncOff */
    60,        /* hBackPorch */
    18,        /* vBackPorch */
    640,       /* xDimension */
    480,       /* yDimension */
    150,       /* memOffset */
    0x0,       /* memFifoEntries_1MB  ... unsupported resolution */
    0x430,     /* memFifoEntries_2MB  ... 31232 entries in memory fifo */
    0x100,     /* memFifoEntries_4MB  ... 57344 entries in memory fifo */
    10,        /* tilesInX_Over2 */
    21,        /* vFifoThreshold */
    FXTRUE,    /* video16BPPIsOK */
    FXTRUE,    /* video24BPPIsOK */
    31.5F,     /* clkFreq16bpp */
    63.0F      /* clkFreq24bpp */
};

const sst1VideoTimingStruct SST_VREZ_640X480_85 = {
    74,        /* hSyncOn */
    765,       /* hSyncOff */
    3,         /* vSyncOn */
    503,       /* vSyncOff */
    100,        /* hBackPorch */
    18,        /* vBackPorch */
    640,       /* xDimension */
    480,       /* yDimension */
    150,       /* memOffset */
    0x0,       /* memFifoEntries_1MB  ... unsupported resolution */
    0x430,     /* memFifoEntries_2MB  ... 31232 entries in memory fifo */
    0x100,     /* memFifoEntries_4MB  ... 57344 entries in memory fifo */
    10,        /* tilesInX_Over2 */
    21,        /* vFifoThreshold */
    FXTRUE,    /* video16BPPIsOK */
    FXTRUE,    /* video24BPPIsOK */
    35.7F,     /* clkFreq16bpp */
    71.3F      /* clkFreq24bpp */
};

const sst1VideoTimingStruct SST_VREZ_640X480_120 = {
    74,        /* hSyncOn */
    765,       /* hSyncOff */
    3,         /* vSyncOn */
    503,       /* vSyncOff */
    100,        /* hBackPorch */
    18,        /* vBackPorch */
    640,       /* xDimension */
    480,       /* yDimension */
    150,       /* memOffset */
    0x0,       /* memFifoEntries_1MB  ... unsupported resolution */
    0x430,     /* memFifoEntries_2MB  ... 31232 entries in memory fifo */
    0x100,     /* memFifoEntries_4MB  ... 57344 entries in memory fifo */
    10,        /* tilesInX_Over2 */
    17,        /* vFifoThreshold */
    FXTRUE,    /* video16BPPIsOK */
    FXTRUE,    /* video24BPPIsOK */
    50.4F,     /* clkFreq16bpp */
    100.8F     /* clkFreq24bpp */
};

const sst1VideoTimingStruct SST_VREZ_800X600_60 = {
    128,       /* hSyncOn */
    890,       /* hSyncOff */
    4,         /* vSyncOn */
    630,       /* vSyncOff */
    40,        /* hBackPorch */
    23,        /* vBackPorch */
    800,       /* xDimension */
    600,       /* yDimension */
    247,       /* memOffset */
    0x0,       /* memFifoEntries_1MB  ... unsupported resolution */
    0x6e2,     /* memFifoEntries_2MB  ... 9152 entries in memory fifo (no Z) */
    0x100,     /* memFifoEntries_4MB  ... 57344 entries in memory fifo */
    13,        /* tilesInX_Over2 */
    14,        /* vFifoThreshold */
    FXTRUE,    /* video16BPPIsOK */
    FXTRUE,    /* video24BPPIsOK */
    40.0F,     /* clkFreq16bpp */
    80.0F      /* clkFreq24bpp */
};

const sst1VideoTimingStruct SST_VREZ_800X600_75 = {
    110,       /* hSyncOn */
    920,       /* hSyncOff */
    2,         /* vSyncOn */
    652,       /* vSyncOff */
    90,        /* hBackPorch */
    35,        /* vBackPorch */
    800,       /* xDimension */
    600,       /* yDimension */
    247,       /* memOffset */
    0x0,       /* memFifoEntries_1MB  ... unsupported resolution */
    0x6e2,     /* memFifoEntries_2MB  ... 9152 entries in memory fifo (no Z) */
    0x100,     /* memFifoEntries_4MB  ... 57344 entries in memory fifo */
    13,        /* tilesInX_Over2 */
    14,        /* vFifoThreshold */
    FXTRUE,    /* video16BPPIsOK */
    FXTRUE,    /* video24BPPIsOK */
    50.0F,     /* clkFreq16bpp */
    100.0F     /* clkFreq24bpp */
};

const sst1VideoTimingStruct SST_VREZ_800X600_85 = {
    110,       /* hSyncOn */
    920,       /* hSyncOff */
    4,         /* vSyncOn */
    630,       /* vSyncOff */
    90,        /* hBackPorch */
    23,        /* vBackPorch */
    800,       /* xDimension */
    600,       /* yDimension */
    247,       /* memOffset */
    0x0,       /* memFifoEntries_1MB  ... unsupported resolution */
    0x6e2,     /* memFifoEntries_2MB  ... 9152 entries in memory fifo (no Z) */
    0x100,     /* memFifoEntries_4MB  ... 57344 entries in memory fifo */
    13,        /* tilesInX_Over2 */
    14,        /* vFifoThreshold */
    FXTRUE,    /* video16BPPIsOK */
    FXTRUE,    /* video24BPPIsOK */
    56.7F,     /* clkFreq16bpp */
    113.3F     /* clkFreq24bpp */
};

const sst1VideoTimingStruct SST_VREZ_856X480_60 = {
    136,       /* hSyncOn */
    1008,      /* hSyncOff */
    2,         /* vSyncOn */
    523,       /* vSyncOff */
    100,       /* hBackPorch */
    23,        /* vBackPorch */
    856,       /* xDimension */
    480,       /* yDimension */
    210,       /* memOffset */
    0x0,       /* memFifoEntries_1MB  ... unsupported resolution */
    0x430,     /* memFifoEntries_2MB  ... 31232 entries in memory fifo */
    0x100,     /* memFifoEntries_4MB  ... 57344 entries in memory fifo */
    14,        /* tilesInX_Over2 */
    14,        /* vFifoThreshold */
    FXTRUE,    /* video16BPPIsOK */
    FXTRUE,    /* video24BPPIsOK */
    36.0F,     /* clkFreq16bpp */
    72.0F      /* clkFreq24bpp */
};

const sst1VideoTimingStruct SST_VREZ_960X720_60 = {
    200,       /* hSyncOn */
    1013,      /* hSyncOff */
    4,         /* vSyncOn */
    750,       /* vSyncOff */
    30,        /* hBackPorch */
    23,        /* vBackPorch */
    960,       /* xDimension */
    720,       /* yDimension */
    338,       /* memOffset (really 337.5) */
    0x0,       /* memFifoEntries_1MB  ... unsupported resolution */
    0x0,       /* memFifoEntries_2MB  ... unsupported resolution */
    0x765,     /* memFifoEntries_4MB  ... 4959 entries in memory fifo */
    15,        /* tilesInX_Over2 */
    14,        /* vFifoThreshold */
    FXTRUE,    /* video16BPPIsOK */
    FXTRUE,    /* video24BPPIsOK */
    56.0F,     /* clkFreq16bpp */
    112.0F     /* clkFreq24bpp */
};

#else /* SST1INIT_VIDEO_ALLOCATE */
extern sst1VideoTimingStruct SST_VREZ_640X480_60;
extern sst1VideoTimingStruct SST_VREZ_800X600_60;

#endif /* SST1INIT_VIDEO_ALLOCATE */

#endif /* !__SST1INIT_H__ */
