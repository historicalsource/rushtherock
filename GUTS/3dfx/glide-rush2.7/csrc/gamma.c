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
** $Date: 8/07/96 4:51p $ 
**
** Initialization code for loading SST-1 gamma tables
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

FxBool AGCGamma(FxU32 *sstbase, float gammaR, float gammaG, float gammaB, float percent, float base)
{
    FxU32 x;
    FxBool sstVideoIsReset;
    volatile Sstregs *sst = (Sstregs *) sstbase;

    /* SST-1 video reset must be inactive to load gamma tables */
    sst1InitIdleFBINoNOP(sstbase);
    if(sst->fbiInit1 & SST_VIDEO_RESET)
        sstVideoIsReset = FXTRUE;
    else
        sstVideoIsReset = FXFALSE;
    sst->fbiInit1 &= ~SST_VIDEO_RESET;
    sst1InitIdleFBINoNOP(sstbase); /* wait for video reset to be deasserted */

    /* SST-1 requires every eighth entry of the gamma table to be loaded, */
    /* so only 32 basic writes are required.  A 33rd write is used to load  */
    /* the top entry of the gamma table.  The 33rd entry is necessary because */
    /* SST-1 performs linear interpolation between each gamma table entry to */
    /* generate 256 unique gamma-corrected values. */
    for(x=0; x <= 256; x +=8) {
	int r, g, b, i;
	r = base + (POW(x/255., 1.0/gammaR) * percent * 255.0 + 0.5);
	if (r > 255) r = 255;
	g = base + (POW(x/255., 1.0/gammaG) * percent * 255.0 + 0.5);
	if (g > 255) g = 255;
	b = base + (POW(x/255., 1.0/gammaB) * percent * 255.0 + 0.5);
	if (b > 255) b = 255;
	i = x >> 3;
        sst->clutData = 
    		(r << SST_CLUTDATA_RED_SHIFT) |
        	(g << SST_CLUTDATA_GREEN_SHIFT) |
        	(b << SST_CLUTDATA_BLUE_SHIFT) |
    		(i << SST_CLUTDATA_INDEX_SHIFT);
    }

    sst1InitIdleFBINoNOP(sstbase); /* wait for gamma table writes to complete */

    /* Return state of video reset to original value */
    if (sstVideoIsReset == FXTRUE)
        sst->fbiInit1 |= SST_VIDEO_RESET;

    return(FXTRUE);
}

/*
** sst1InitGamma():
**  Load the video color-lookup tables with the specified gamma function
**
**    Returns:
**      FXTRUE if successfully initializes SST-1 gamma tables
**      FXFALSE if cannot initialize SST-1 gamma tables
**
*/
FX_EXPORT FxBool FX_CSTYLE sst1InitGamma(FxU32 *sstbase, double gamma)
{
    int sts;
#if !defined(__GNUC__)
    {
	static FxBool calledBefore /*= FXFALSE*/;
	static FxBool overRideR /*= FXFALSE*/;
	static FxBool overRideG /*= FXFALSE*/;
	static FxBool overRideB /*= FXFALSE*/;
	static double gammaR, gammaG, gammaB;

	if(!sstbase)
	    return(FXFALSE);

	if(sst1InitCheckBoard(sstbase) == FXFALSE)
	    return(FXFALSE);

	if(overRideR == FXFALSE)
	    gammaR = gamma;
	if(overRideG == FXFALSE)
	    gammaG = gamma;
	if(overRideB == FXFALSE)
	    gammaB = gamma;

	if(calledBefore == FXFALSE) {
	    calledBefore = FXTRUE;
	    if(GETENV(("SST_RGAMMA"))) {
		overRideR = FXTRUE;
		gammaR = (double) ATOF(GETENV(("SST_RGAMMA")));
	    }
	    if(GETENV(("SST_GGAMMA"))) {
		overRideG = FXTRUE;
		gammaG = (double) ATOF(GETENV(("SST_GGAMMA")));
	    }
	    if(GETENV(("SST_BGAMMA"))) {
		overRideB = FXTRUE;
		gammaB = (double) ATOF(GETENV(("SST_BGAMMA")));
	    }
	    if(GETENV(("SST_GAMMA"))) {
		overRideR = FXTRUE;
		overRideG = FXTRUE;
		overRideB = FXTRUE;
		gammaR = (double) ATOF(GETENV(("SST_GAMMA")));
		gammaG = gammaR;
		gammaB = gammaR;
	    }
	}
	sts = sst1InitGammaRGB(sstbase, gammaR, gammaG, gammaB);
    }
#else
#ifndef GAMMA_BASE
# define GAMMA_BASE 28.0
#endif
#ifndef GAMMA_RATIO
# define GAMMA_RATIO (0.67) /* (256.0-GAMMA_BASE)/256.0) */
#endif
    sts = AGCGamma(sstbase, 1.4, 1.4*0.85, 1.4, GAMMA_RATIO, GAMMA_BASE);
/*        AGCGamma(sstbase, 1.4, 1.4,      1.4, GAMMA_RATIO, GAMMA_BASE) for white tracking. */
#endif
    return sts;
}

FX_EXPORT FxBool FX_CSTYLE sst1InitGammaRGB(FxU32 *sstbase, double gammaR,
  double gammaG, double gammaB)
{
    FxU32 x, n;
    FxU32 gammaTableR[256];
    FxU32 gammaTableG[256];
    FxU32 gammaTableB[256];
    FxBool sstVideoIsReset;
    static FxBool calledBefore /*= FXFALSE*/;
    volatile Sstregs *sst = (Sstregs *) sstbase;

    if(!sstbase)
        return(FXFALSE);

    if(sst1InitCheckBoard(sstbase) == FXFALSE)
        return(FXFALSE);

    /* Enable video clock */
    if(calledBefore == FXFALSE) {
        if(SLI_DRIVEOK()) {
            INIT_PRINTF(("sst1InitGammaRGB(): Enabling Video Clock...\n"));
            PCICFG_WR(SST1_PCI_VCLK_ENABLE, 0x0);
        }
        INIT_PRINTF(("sst1InitGammaRGB(): Setting GammaRGB = (%.2f,%.2f,%.2f)\n",
          gammaR, gammaG, gammaB));
    }

    /* Initialize the gamma table */
    for(x=0; x<256; x++) {
        gammaTableR[x] = (FxU32) (POW(x/255., 1.0/gammaR) * 255.0 + 0.5);
        gammaTableG[x] = (FxU32) (POW(x/255., 1.0/gammaG) * 255.0 + 0.5);
        gammaTableB[x] = (FxU32) (POW(x/255., 1.0/gammaB) * 255.0 + 0.5);
    }

    /* SST-1 video reset must be inactive to load gamma tables */
    sst1InitIdleFBINoNOP(sstbase);
    if(sst->fbiInit1 & SST_VIDEO_RESET)
        sstVideoIsReset = FXTRUE;
    else
        sstVideoIsReset = FXFALSE;
    sst->fbiInit1 &= ~SST_VIDEO_RESET;
    sst1InitIdleFBINoNOP(sstbase); /* wait for video reset to be deasserted */

    /* SST-1 requires every eighth entry of the gamma table to be loaded, */
    /* so only 32 basic writes are required.  A 33rd write is used to load  */
    /* the top entry of the gamma table.  The 33rd entry is necessary because */
    /* SST-1 performs linear interpolation between each gamma table entry to */
    /* generate 256 unique gamma-corrected values. */
    for(x=0; x<32; x++) {
        FxU32 gcR = gammaTableR[(x<<3)];
        FxU32 gcG = gammaTableG[(x<<3)];
        FxU32 gcB = gammaTableB[(x<<3)];
        sst->clutData = ((x<<SST_CLUTDATA_INDEX_SHIFT) | 
                         (gcR<<SST_CLUTDATA_RED_SHIFT) |
                         (gcG<<SST_CLUTDATA_GREEN_SHIFT) |
                         (gcB<<SST_CLUTDATA_BLUE_SHIFT));
    }
    /* Last entry in the gamma table is stored as 0x0 to perform proper linear */
    /* interpolation of the last 8 entries */
    sst->clutData = (32<<SST_CLUTDATA_INDEX_SHIFT);

    sst1InitIdleFBINoNOP(sstbase); /* wait for gamma table writes to complete */

    /* Return state of video reset to original value */
    if(sstVideoIsReset == FXTRUE)
        sst->fbiInit1 |= SST_VIDEO_RESET;

    if(calledBefore == FXFALSE) {
        calledBefore = FXTRUE;
        INIT_PRINTF(("sst1InitGammaRGB() exiting with status %d...\n", FXTRUE));
    }
    return(FXTRUE);
}

#pragma optimize ("",on)
