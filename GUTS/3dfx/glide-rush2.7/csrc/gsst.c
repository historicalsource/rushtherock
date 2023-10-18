/*
** Copyright (c) 1995, 3Dfx Interactive, Inc.
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
** successor clauses in the FAR, DOD or NASA FAR Supplement. Unpublished -
** rights reserved under the Copyright Laws of the United States.
**
** $Header: /engr/devel/sst1/GLIDE/DOS/SRC/GSST.C 108   6/20/96 5:30p Dow $
** $Log: /engr/devel/sst1/GLIDE/DOS/SRC/GSST.C $
 * 
 * 108   6/20/96 5:30p Dow
 * Changed grGammaCorrectionValue to call init code.
 * 
 * 107   6/19/96 3:05p Sellers
 * fixed problems with splash when users repeatedly call glideInit() and
 * glideShutdown()
 * now grQueryHardware() properly returns FXFALSE and doesn't die if 3Dfx
 * hardware is not detected
 * 
 * 106   6/18/96 6:55p Sellers
 * fixed for SLI support and splash screen
 * 
 * 105   6/17/96 9:24a Dow
 * Filled in sliDetect element of VoodooConfig 
 * 
 * 104   6/16/96 5:05p Jdt
 * Fixed for new grTexMipMapMode signature
 * 
 * 103   6/16/96 1:31p Sellers
 * fixed SLI detection for older Obsidian boards with .5 micron FBIs
 * 
 * 102   6/15/96 11:36a Sellers
 * removed grSstSetPassthruMode
 * fixed grSstPassthruMode to call the init code function
 * sst1InitVgaPassCtrl()
 * 
 * 101   6/14/96 6:26p Sellers
 * Fixed bogus setting of _SST->* before _SST was set
 * 
 * 100   6/12/96 5:27p Dow
 * Removed grSstInVerticalBlank
 * Added grSstVRetraceOn
 * 
 * 98    6/11/96 7:39p Dow
 * Added pixel performance metrics routines, removed old ones.
 * Fixed bug that I noticed in grSstPassthruMode();
 * Fixed bug Scott noticed in grSstIsBusy();
 * 
 * 97    6/10/96 12:38a Jdt
 * Reconciled calls to grTexMipMapMode with new function signature
 * 
 * 96    6/08/96 4:17p Dow
 *      Removed stuff for wireframe
 *      Removed #if 0'd code
 *      Changed initial alpha value
 *      Scott's SLI changes
 * 
 * 95    6/05/96 10:38p Dow
 * Changed default state of TMU
 * 
 * 94    6/03/96 1:17p Dow
 * s/_grTexRebuildRegisterShadows/_guTexRebuildRegisterShadows/g
 * s/grTexAllocateMemory/guTexAllocateMemory/g
 * s/grTexGetMipMapInfo/guTexGetMipMapInfo/g
 * s/grTexMemReset/guTexMemReset/g
 * s/grTexSource/guTexSource/g
 * s/grTexChangeAttributes/guTexChangeAttributes/g
 * s/grTexDownloadMipMap/guTexDownloadMipMap/g
 * s/grTexDownloadMipMapLevel/guTexDownloadMipMapLevel/g
 * s/grTexMemQueryAvail/guTexMemQueryAvail/g
 * 
 * 93    5/29/96 9:59p Dow
 * Moved some initiazation code around.
 * 
 * 92    5/28/96 8:24a Tarolli
 * declared tmuNum (it was previously undefined)
 * 
 * 91    5/22/96 10:23a Dow
 * Changed default state to be more desireable per Track # 15
 * 
 * 90    5/22/96 10:07a Dow
 * Adjustments for hardware config info structure.
 * 
 * 89    5/22/96 1:40a Dow
 * Added configuration structure.
 * 
 * 87    5/20/96 11:55a Dow
 * Fixed up NCC texture stuff.
 * 
 * 86    5/09/96 7:05a Paik
 * Glide DLL support
 * 
 * 84    5/07/96 7:18p Sellers
 * added 640x400 resolution
 * added 856x480 resolution
 * added 960x720 resolution
 * 
 * 83    4/19/96 12:59a Tarolli
 * 
 * 82    4/05/96 1:33p Dow
 * added alt regmap notification
 * 
 * 81    3/25/96 7:58p Paik
 * Move fxglide.h out of glide.h
 * 
 * 80    3/22/96 10:03p Paik
 * Fix typo when compiling for simulator
 * 
 * 79    3/20/96 8:07a Paik
 * Use single macro for TMUs
 * 
 * 78    3/03/96 3:59p Dow
 * HW setup stuff
 * 
 * 77    3/01/96 1:57p Paik
 * State optimization
 * 
 * 79    3/01/96 3:49a Paik
 * 
 * 78    2/29/96 11:58p Paik
 * 
 * 77    2/29/96 8:51p Paik
 * 
 * 76    2/29/96 3:58p Dow
 * HW Config stuff
 * 
 * 75    2/28/96 5:15p Paik
 * Get simulator to compile
 * 
 * 74    2/28/96 4:52p Dow
 * Now only maps sst once
 * 
 * 73    2/13/96 3:01p Dow
 * Changed grSstIdle to work better
 * 
 * 72    2/13/96 5:40a Dow
 * FIxed bug preventing texture allocation on TMUs > 0
 * 
 * 71    2/11/96 4:21p Dow
 * added grSstIdle()  (this is faster than a while(grSstIsBusy()) loop) &
 * support for two TMUs
 * 
 * 70    2/09/96 4:44p Dow
 * Updated GetConfig*
 * 
 * 69    2/06/96 9:13p Garymct
 * grSstGetConfigFbi was not defined when compiling for the simulator, so
 * I added it.
 * 
 * 68    2/06/96 8:41p Garymct
 * Changed a '&&' to a '&' when testing a bitfield. . .was causing glide
 * to not compiler for the simulator.
 * 
 * 67    2/06/96 6:35a Dow
 * Modified queryHardware and detectResources to support TMU bringup.
 * 
 * 66    2/05/96 5:47p Dow
 * num_tmu == 1
 * 
 * 63    2/05/96 5:22p Dow
 * FIB -> FBI
 * 
 * 60    2/05/96 11:39a Dow
 * Fixed grSstGetStatus
 * 
 * 59    1/19/96 5:12p Paik
 * Hardware bringup modifications
 * 
 * 58    1/15/96 6:29p Dow
 * Hardware Bringup mods
 * 
 * 57    1/14/96 6:04p Dow
 * updated init code
 * 
 * 56    1/14/96 12:29p Dow
 * AAMode stuff & GLIDE_STATE_PAD_SIZE fix
 * 
 * 54    1/13/96 2:43p Dow
 * Random fixes, and emacs helping stuff
 * 
 * 53    1/12/96 3:23p Dow
 * removed spurious j
 * 
 * 52    1/12/96 2:28p Paik
 * [sw-glide/225, sw-glide/242]
 * Executive summary
 * 1. created grAlphaCombine and grColorCombine which fully expose acu and
 * ccu.
 * 2. renamed grColorCombineFunction and grAlphaSource to
 * guColorCombineFunction and guAlphaSource, reimplement on top of
 * grColorCombine and grAlphaCombine.
 * 
 * 51    1/12/96 1:02p Dow
 * _grSstDetermineResources -> _grSstDetectResources (in call)
 * 
 * 48    1/12/96 12:56p Dow
 * Hardware bringup mods
 * 
 * 46    1/11/96 9:39p Dow
 * Changed to new alpha semantics
 * Changed bous tmu type ref to GrChipID_t
 * 
 * 45    1/11/96 1:14p Dow
 * c:\apps\cwright\cwrsp000%
 * 
 * 44    1/11/96 2:26a Dow
 * Added GrTMU_t, changed functions taking tmus to use it.
 * Added tmuMask to state, use it to better control state setting when
 * moving between texture mapped and non texture mapped color combine
 * modes.
 * 
 * 43    1/11/96 12:57a Dow
 * Mostly using sst.h for bit definitions.  Still using gsstdef.c for
 * register defs, though.
 * 
 * 42    1/10/96 11:12p Dow
 * more 1 & 3 tmu, virtual tmu removal fixes
 * 
 * 41    1/09/96 9:39a Dow
 * tabs->spaces && added Rev. Control keywords
**
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include <3dfx.h>

#include <glidesys.h>
#if (GLIDE_PLATFORM & GLIDE_SST_HW)
#include <sst1vid.h>
#include <sst1init.h>
#endif

#include <fxpci.h>

#define FX_DLL_DEFINITION
#include <fxdll.h>
#include <glide.h>
#include "fxglide.h"

#define GLIDE_NO_PRODUCTION

#ifdef GLIDE_NO_PRODUCTION
static FxBool ignore_sst_writes;
#endif

#define GLIDE_LFB_OFFSET 0x400000
#define GLIDE_TEX_OFFSET 0x800000

#if ( GLIDE_PLATFORM & GLIDE_SST_SIM )

extern float gw_sw, gw_sh;
#ifndef __SST_H__
extern Sstregs *sstGinit( const char *, int );
#endif
extern void   sstTrexChips( int num_trex );
extern void   gsim_prefposition( FxU32 x1, FxU32 x2, FxU32 y1, FxU32 y2 );
       float  gw_sw, gw_sh;

#endif

#if ( GLIDE_PLATFORM & GLIDE_SST_HW)
static int    initFindNumSSTs(void);
#endif

GrHwConfiguration   _HW_CONFIGURATION;
GrScreenResolution_t grSstOpenLastRez;
GrScreenRefresh_t    grSstOpenLastRefresh;

#if (GLIDE_PLATFORM & GLIDE_SST_HW)
/*
** _grSstDetectResources
*/
FxBool 
_grSstDetectResources(void) {
  int i,j,k;
  sst1DeviceInfoStruct sstInfo;

	grSstOpenLastRez = GR_RESOLUTION_NONE;	/* from above */
	grSstOpenLastRefresh = GR_REFRESH_NONE;	/* from above */

  /* determine amount of FBI memory */
  _HW_CONFIGURATION.num_sst = 0;

  /* Find & Map the SST boards */
  for (k = 0, i = 0; i < MAX_NUM_SST; i++) {
    if ((_SSTs[k].base_ptr = sst1InitMapBoard(i))!= NULL) {
      /* XXXX Sam & Jim - this needs to change for SST-96 and AT3D */
      _HW_CONFIGURATION.SSTs[k].type = GR_SSTTYPE_VOODOO;
      _SSTs[k].lfb_ptr = _SSTs[k].base_ptr + ( GLIDE_LFB_OFFSET >> 2 );
      _SSTs[k].tex_ptr = _SSTs[k].base_ptr + ( GLIDE_TEX_OFFSET >> 2 );
      _SSTs[k].slave_ptr = (FxU32 *) NULL;

      /*
       **  Hardware Initialization
       */
      if (sst1InitRegisters(_SSTs[k].base_ptr) == FXFALSE) {
#ifdef GLIDE_DEBUG
        GrErrorCallback("grSstOpen:  Register initialization failed!\n", FXTRUE);
#endif
        return FXFALSE;
      }
      /* Scanline interleaved subsystem looks like a single SST board
         from an application level perspective...*/
      if ( (k & 1) && (_SSTs[k-1].scanline_interleaved == FXTRUE) ) {
        /* SLI Slaves are every other board in the system... */
        /* Save Slave address... */
        _SSTs[k-1].slave_ptr = _SSTs[k].base_ptr;
        continue;
      }

      ++_HW_CONFIGURATION.num_sst;
      if ( !sst1InitGetDeviceInfo( _SSTs[k].base_ptr, &sstInfo ) ) {
#ifdef GLIDE_DEBUG
        char str[255];

        sprintf(str, "_grSstDetectResources: Failed to query board %d\n", i);
        GrErrorCallback( str, FXTRUE );
#endif
        return FXFALSE;
      }
      /* TMUs Present */
      _SSTs[k].num_tmu = sstInfo.numberTmus;
      _HW_CONFIGURATION.SSTs[k].sstBoard.VoodooConfig.nTexelfx = sstInfo.numberTmus;
      /* FBI Revision */
      _HW_CONFIGURATION.SSTs[k].sstBoard.VoodooConfig.fbiRev = sstInfo.fbiRevision;
      /* FBI Ram */
      _HW_CONFIGURATION.SSTs[k].sstBoard.VoodooConfig.fbRam =  sstInfo.fbiMemSize;
      /* Scanline Interleaved? */
      _SSTs[k].scanline_interleaved = sstInfo.sstSliDetect;
      _HW_CONFIGURATION.SSTs[k].sstBoard.VoodooConfig.sliDetect =
        sstInfo.sstSliDetect; 

      /* TMU RAM */
      for (j = 0; j < _SSTs[k].num_tmu; j++) {
        _HW_CONFIGURATION.SSTs[k].sstBoard.VoodooConfig.tmuConfig[j].tmuRam =
          sstInfo.tmuMemSize[j];
        _HW_CONFIGURATION.SSTs[k].sstBoard.VoodooConfig.tmuConfig[j].tmuRev =
            sstInfo.tmuRevision;
          
        memset(&_SSTs[k].tmu_state[j], 0, sizeof(_SSTs[k].tmu_state[j]));
        _SSTs[k].tmu_state[j].ncc_mmids[0] = GR_NULL_MIPMAP_HANDLE;
        _SSTs[k].tmu_state[j].ncc_mmids[1] = GR_NULL_MIPMAP_HANDLE;
        _SSTs[k].tmu_state[j].total_mem = sstInfo.tmuMemSize[j]<<20;
      }
      _SSTs[k].fbuf_size = sstInfo.fbiMemSize;
      k++;
    } else {
      break;
    }
  }

  /* Sanity check for SLI detection */
  for (k = 0; i < MAX_NUM_SST; i++) {
      if((_SSTs[k].scanline_interleaved = sstInfo.sstSliDetect == FXTRUE) &&
         (_SSTs[k].slave_ptr == (FxU32 *) NULL)) {
           _SSTs[k].scanline_interleaved = sstInfo.sstSliDetect = FXFALSE;
           _HW_CONFIGURATION.SSTs[k].sstBoard.VoodooConfig.sliDetect = FXFALSE;
      }
  }

  if ( _HW_CONFIGURATION.num_sst == 0 ) {
#ifdef GLIDE_DEBUG
    GrErrorCallback( "_grSstDetectResources: Failed to map any board\n", FXTRUE );
#endif
    return FXFALSE;
  }
  return FXTRUE;

} /* grSstDetectResources */
#endif

/*
** grSstOpen
*/
FX_EXPORT FxBool FX_CSTYLE
grSstOpen( GrScreenResolution_t screen_resolution,
          GrScreenRefresh_t    refresh_rate,
          GrColorFormat_t      color_format,
          GrOriginLocation_t   origin_location,
          GrSmoothingMode_t    smoothing_mode,
          int                  num_buffers ) {
  int
    i;
  FxU32
    mask    = 0,
    lfbmode;                    /* Contents of lfbMode register */
  
#ifdef GLIDE_DEBUG
  if ( _SST == 0 )
    GrErrorCallback( "grSstOpen:  no SST selected as current (_SST==NULL)", FXTRUE );
#endif
  
  /*
   ** Set up other configuration variable
   */
  _SST->state.color_format    = color_format;
  _SST->state.num_buffers     = num_buffers;
  
  /*
   ** Handle big-endian specific issues
   */
#if ( GLIDE_PLATFORM & GLIDE_ENDIAN_BIG )
#endif
  
  /*
   ** Make sure we have a legal screen resolution vs. mem configuration
   */
  if( (_SST->fbuf_size <= 2 &&
       screen_resolution == GR_RESOLUTION_800x600 &&
       _SST->state.num_buffers == 3 ) ||
     (_SST->fbuf_size <= 2 &&
      screen_resolution == GR_RESOLUTION_856x480 &&
      _SST->state.num_buffers == 3 ) ||
     (_SST->fbuf_size <= 2 &&
      screen_resolution == GR_RESOLUTION_960x720 ) )
    {
      GrErrorCallback( "grSstOpen:  not enough memory to run this configuration", FXTRUE );
    }
  
#ifdef GLIDE_NO_PRODUCTION
  if ( getenv( "GLIDE_IGNORE_SST" ) != 0 )
    ignore_sst_writes = FXTRUE;
  else
    ignore_sst_writes = FXFALSE;
#endif
  
  /*
   ** configure screen dimensions
   */
#if ( GLIDE_PLATFORM & GLIDE_SST_SIM )
  if ( gw_sw == 0 )  {
#endif
    switch ( screen_resolution ){
    case GR_RESOLUTION_320x200:
      _SST->state.screen_width  = 320.0F;
      _SST->state.screen_height = 200.0F;
#if (!(GLIDE_PLATFORM & GLIDE_OS_WIN32) && (GLIDE_PLATFORM & GLIDE_SST_SIM))
      GrErrorCallback( "grSstOpen:  320x200 not implemented", FXTRUE );
#endif
      break;
    case GR_RESOLUTION_320x240:
      _SST->state.screen_width  = 320.0F;
      _SST->state.screen_height = 240.0F;
#if (!(GLIDE_PLATFORM & GLIDE_OS_WIN32) && (GLIDE_PLATFORM & GLIDE_SST_SIM))
      GrErrorCallback( "grSstOpen:  320x240 not implemented", FXTRUE );
#endif
      break;
    case GR_RESOLUTION_336x240:
      _SST->state.screen_width  = 336.0F;
      _SST->state.screen_height = 240.0F;
#if (!(GLIDE_PLATFORM & GLIDE_OS_WIN32) && (GLIDE_PLATFORM & GLIDE_SST_SIM))
      GrErrorCallback( "grSstOpen:  336x240 not implemented", FXTRUE );
#endif
      break;
    case GR_RESOLUTION_400x240:
      _SST->state.screen_width  = 400.0F;
      _SST->state.screen_height = 240.0F;
#if (!(GLIDE_PLATFORM & GLIDE_OS_WIN32) && (GLIDE_PLATFORM & GLIDE_SST_SIM))
      GrErrorCallback( "grSstOpen:  400x240 not implemented", FXTRUE );
#endif
      break;
    case GR_RESOLUTION_400x256:
      _SST->state.screen_width = 400.0F;
      _SST->state.screen_height = 256.0F;
#if (!(GLIDE_PLATFORM & GLIDE_OS_WIN32) && (GLIDE_PLATFORM & GLIDE_SST_SIM))
      GrErrorCallback( "grSstOpen:  400x256 not implemented", FXTRUE );
#endif
      break;

    case GR_RESOLUTION_512x256:
      _SST->state.screen_width  = 512.0F;
      _SST->state.screen_height = 256.0F;
#if (!(GLIDE_PLATFORM & GLIDE_OS_WIN32) && (GLIDE_PLATFORM & GLIDE_SST_SIM))
      GrErrorCallback( "grSstOpen:  512x256 not implemented", FXTRUE );
#endif
      break;
    case GR_RESOLUTION_512x384:
      _SST->state.screen_width  = 512.0F;
      _SST->state.screen_height = 384.0F;
#if (!(GLIDE_PLATFORM & GLIDE_OS_WIN32) && (GLIDE_PLATFORM & GLIDE_SST_SIM))
      GrErrorCallback( "grSstOpen:  512x384 not implemented", FXTRUE );
#endif
      break;
    case GR_RESOLUTION_640x400:
      _SST->state.screen_width  = 640.0F;
      _SST->state.screen_height = 400.0F;
#if (!(GLIDE_PLATFORM & GLIDE_OS_WIN32) && (GLIDE_PLATFORM & GLIDE_SST_SIM))
      GrErrorCallback( "grSstOpen:  640x400 not implemented", FXTRUE );
#endif
      break;
    case GR_RESOLUTION_640x480:
      _SST->state.screen_width  = 640.0F;
      _SST->state.screen_height = 480.0F;
      break;
    case GR_RESOLUTION_800x600:
      _SST->state.screen_width  = 800.0F;
      _SST->state.screen_height = 600.0F;
      break;
    case GR_RESOLUTION_856x480:
      _SST->state.screen_width  = 856.0F;
      _SST->state.screen_height = 480.0F;
#if (!(GLIDE_PLATFORM & GLIDE_OS_WIN32) && (GLIDE_PLATFORM & GLIDE_SST_SIM))
      GrErrorCallback( "grSstOpen:  856x480 not implemented", FXTRUE );
#endif
      break;
    case GR_RESOLUTION_960x720:
      _SST->state.screen_width  = 960.0F;
      _SST->state.screen_height = 720.0F;
#if (!(GLIDE_PLATFORM & GLIDE_OS_WIN32) && (GLIDE_PLATFORM & GLIDE_SST_SIM))
      GrErrorCallback( "grSstOpen:  960x720 not implemented", FXTRUE );
#endif
      break;
    default:
      GrErrorCallback( "grSstOpen:  unknown resolution specified", FXTRUE );
      break;
    }

#if (GLIDE_PLATFORM & GLIDE_SST_HW)
    /* The splash stuff sets the screen up in 648x480x60Hz, so check to
       see if we really need to set it again...*/
    if((screen_resolution != grSstOpenLastRez) ||
       (refresh_rate != grSstOpenLastRefresh)) {

       if((grSstOpenLastRez == GR_RESOLUTION_640x480) &&
          (grSstOpenLastRefresh == GR_REFRESH_60Hz) &&
          (_SST->scanline_interleaved == FXTRUE) &&
          (_SST->slave_ptr != (FxU32 *) NULL)) {
                         /* SLI setup for Splash screen previously */
          /* Shutdown SLI before changing video resolutions */
          grSstIdle();
          sst1InitShutdownSli(_SST->base_ptr);
       }
       grSstOpenLastRez = screen_resolution;
       grSstOpenLastRefresh = refresh_rate;

#if 1 || VERSION_20B3_5
#  define INITVIDEO_EXTRA ,0
#else
#  define INITVIDEO_EXTRA
#endif

      if (sst1InitVideo(_SST->base_ptr, screen_resolution, refresh_rate INITVIDEO_EXTRA) == FXFALSE)
        GrErrorCallback("grSstOpen:  Video Timing initialization failed!\n", FXTRUE);
      if (sst1InitGamma(_SST->base_ptr, 1.7) == FXFALSE)
        GrErrorCallback("grSstOpen:  Gamma initialization failed!\n", FXFALSE);
      
      if ((_SST->scanline_interleaved == FXTRUE) && (_SST->slave_ptr)) {
        if (sst1InitVideo(_SST->slave_ptr, screen_resolution, refresh_rate INITVIDEO_EXTRA) ==
            FXFALSE)
          GrErrorCallback("grSstOpen:  SLI Slave Video Timing initialization failed!\n", FXTRUE);
        if (sst1InitGamma(_SST->slave_ptr, 1.7) == FXFALSE)
          GrErrorCallback("grSstOpen:  SLI Slave Gamma initialization failed!\n", FXFALSE);
        if(sst1InitSli(_SST->base_ptr, _SST->slave_ptr) == FXFALSE)
          GrErrorCallback("grSstOpen:  SLI Initialization failed!\n", FXTRUE);
      } else if(_SST->scanline_interleaved == FXTRUE)
          /* Older Obsidian boards incorrectly detect SLI */
          _SST->scanline_interleaved = FXFALSE;
    }

    /* Initialize the read/write registers with grSstGetConfig */
    _SST->state.fbi_config.fbzColorPath = grSstGetConfigFbi(SSTR_FBZCOLORPATH);
    _SST->state.fbi_config.fogMode = grSstGetConfigFbi(SSTR_FOGMODE);
    _SST->state.fbi_config.alphaMode = grSstGetConfigFbi(SSTR_ALPHAMODE);
    _SST->state.fbi_config.fbzMode = grSstGetConfigFbi(SSTR_FBZMODE);
    _SST->state.fbi_config.lfbMode = grSstGetConfigFbi(SSTR_LFBMODE);
    _SST->state.fbi_config.clipLeftRight = grSstGetConfigFbi(SSTR_CLIPLEFTRIGHT);
    _SST->state.fbi_config.clipBottomTop = grSstGetConfigFbi(SSTR_CLIPBOTTOMTOP);
    _SST->state.fbi_config.fogColor = 0x00000000;
    _SST->state.fbi_config.zaColor = 0x00000000;
    _SST->state.fbi_config.chromaKey = 0x00000000;
    _SST->state.fbi_config.stipple = grSstGetConfigFbi(SSTR_STIPPLE);
    _SST->state.fbi_config.color0 = grSstGetConfigFbi(SSTR_C0);
    _SST->state.fbi_config.color1 = grSstGetConfigFbi(SSTR_C1);

    for (i = 0; i < _SST->num_tmu; i += 1) {
      _SST->state.tmu_config[i].textureMode = 0x00000000;
      _SST->state.tmu_config[i].tLOD = 0x00000000;
      _SST->state.tmu_config[i].tDetail = 0x00000000;
      _SST->state.tmu_config[i].texBaseAddr = 0x00000000;
      _SST->state.tmu_config[i].texBaseAddr_1 = 0x00000000;
      _SST->state.tmu_config[i].texBaseAddr_2 = 0x00000000;
      _SST->state.tmu_config[i].texBaseAddr_3_8 = 0x00000000;
      _SST->state.tmu_config[i].mmMode   = GR_MIPMAP_NEAREST;
      _SST->state.tmu_config[i].smallLod = GR_LOD_1;
      _SST->state.tmu_config[i].largeLod = GR_LOD_1;
      _SST->state.tmu_config[i].evenOdd  = GR_MIPMAPLEVELMASK_BOTH;
      _SST->state.tmu_config[i].nccTable = GR_NCCTABLE_NCC0;
    }
#endif /* we're on the hardware */

#if ( GLIDE_PLATFORM & GLIDE_SST_SIM )
  } else {
    _SST->state.screen_width  = gw_sw;
    _SST->state.screen_height = gw_sh;
  }
#endif
  
#if ( GLIDE_PLATFORM & GLIDE_SST_SIM )
#ifdef __DOS__
  gsim_prefposition(
                    0, ( unsigned long ) ( _SST->state.screen_width - 1 ),
                    0, ( unsigned long ) ( _SST->state.screen_height - 1 )
                    );
#else
  gsim_prefposition(
                    100,(unsigned long)(_SST->state.screen_width - 1 + 100),
                    100,(unsigned long)( _SST->state.screen_height - 1 + 100)
                    );
#endif
  
  
#ifdef GLIDE_BENCHMARK
  _SST->base_ptr = calloc(1024 * 1024 * 10, 1);
#else   
  _SST->base_ptr = (FxU32 *) sstGinit( "Glide", -1 );
#endif
  
#ifndef GLIDE_BENCHMARK
  _SST->lfb_ptr  = _SST->base_ptr + ( GLIDE_LFB_OFFSET >> 2 );
  _SST->tex_ptr  = _SST->base_ptr + ( GLIDE_TEX_OFFSET >> 2 );
#endif
  
#ifdef GLIDE_BENCHMARK
  _SST->lfb_ptr  = _SST->tex_ptr  = _SST->base_ptr;
#endif
  
#ifndef GLIDE_BENCHMARK
  sstTrexChips( GLIDE_NUM_TMU );
#endif
  
#endif /* GLIDE_PLATFORM & GLIDE_SST_SIM */
  
#if !defined(GLIDE_DONT_OPTIMIZE_TRISETUP) && ( (GLIDE_PLATFORM & GLIDE_SST_HW) || defined(GLIDE_BENCHMARK) )
  /* Now that we have _SST, we can build the offests and register
     lists for the  optimized triangle setup code */
  _grInitTriSetupCode();
#endif
  
  /* Initialize FBZMODE register */
  mask = SST_ENRECTCLIP | SST_ENZBIAS | SST_DRAWBUFFER_BACK;
  grSstWriteConfigFbi( SSTR_FBZMODE, mask );
  _SST->state.fbi_config.fbzMode = mask;
  
  /* Initialize origin */
  grSstOrigin(origin_location);

  /* Initialize ALPHAMODE register  */
  grSstWriteConfigFbi( SSTR_ALPHAMODE, 0 );
  _SST->state.fbi_config.alphaMode = 0;
  
  /* Go through and set up default values for everything */
  grAlphaBlendFunction( GR_BLEND_ONE , GR_BLEND_ZERO, GR_BLEND_ONE, GR_BLEND_ZERO );
  grAlphaTestFunction( GR_CMP_ALWAYS );
  grAlphaTestReferenceValue( 0 );
  
  grChromakeyMode( GR_CHROMAKEY_DISABLE );
  grConstantColorValue( ( FxU32 ) ~0 );
  grClipWindow( 0, 0, ( int ) _SST->state.screen_width, ( int ) _SST->state.screen_height );
  _grColorCombineDelta0Mode( FXFALSE );
  
  grColorCombine(GR_COMBINE_FUNCTION_SCALE_OTHER,
                 GR_COMBINE_FACTOR_ONE,
                 GR_COMBINE_LOCAL_ITERATED,
                 GR_COMBINE_OTHER_ITERATED,
                 FXFALSE);
  grAlphaCombine(GR_COMBINE_FUNCTION_SCALE_OTHER,
                 GR_COMBINE_FACTOR_ONE,
                 GR_COMBINE_LOCAL_NONE,
                 GR_COMBINE_OTHER_CONSTANT,
                 FXFALSE);
  
  grCullMode( GR_CULL_DISABLE );
  
  grColorMask( FXTRUE, FXFALSE );
  
  grDepthBiasLevel( 0 );
  grDepthMask( FXFALSE );
  grDepthBufferMode( GR_DEPTHBUFFER_DISABLE );
  grDepthBufferFunction( GR_CMP_LESS );
  grDepthBiasLevel( 0 );
  
  grDitherMode( GR_DITHER_4x4 );
  
  grFogMode( GR_FOG_DISABLE );
  grFogColorValue( 0x00000000 );
  
  guTexMemReset();
  
  switch (_SST->num_tmu) {
  case 3:
    grTexClampMode( GR_TMU2, GR_TEXTURECLAMP_CLAMP, GR_TEXTURECLAMP_CLAMP );
    grTexDetailControl( GR_TMU2, 0, 1, 1.0F );
    grTexFilterMode( GR_TMU2, GR_TEXTUREFILTER_POINT_SAMPLED, GR_TEXTUREFILTER_POINT_SAMPLED );

    grTexLodBiasValue( GR_TMU2, 0.0F);
    grTexMipMapMode( GR_TMU2, GR_MIPMAP_DISABLE, FXFALSE );
    grTexCombineFunction(GR_TMU2, GR_TEXTURECOMBINE_ZERO )
      ;

  case 2:
    grTexClampMode( GR_TMU1, GR_TEXTURECLAMP_CLAMP, GR_TEXTURECLAMP_CLAMP );
    grTexDetailControl( GR_TMU1, 0, 1, 1.0F );
    grTexFilterMode( GR_TMU1, GR_TEXTUREFILTER_POINT_SAMPLED, GR_TEXTUREFILTER_POINT_SAMPLED );
    grTexLodBiasValue( GR_TMU1, 0.0F);
    grTexMipMapMode( GR_TMU1, GR_MIPMAP_DISABLE, FXFALSE );
    grTexCombineFunction(GR_TMU1, GR_TEXTURECOMBINE_ZERO );

  case 1:
    grTexClampMode( GR_TMU0, GR_TEXTURECLAMP_CLAMP, GR_TEXTURECLAMP_CLAMP );
    grTexDetailControl( GR_TMU0, 0, 1, 1.0F );
    grTexFilterMode( GR_TMU0, GR_TEXTUREFILTER_POINT_SAMPLED, GR_TEXTUREFILTER_POINT_SAMPLED );
    grTexLodBiasValue( GR_TMU0, 0.0F);
    grTexMipMapMode( GR_TMU0, GR_MIPMAP_DISABLE, FXFALSE );
    grTexCombineFunction(GR_TMU0, GR_TEXTURECOMBINE_ZERO );
  }
  
  /*
   ** Configure linear frame buffer access stuff
   */
  lfbmode = 0;
  if ( origin_location == GR_ORIGIN_LOWER_LEFT )
    {
      lfbmode |= SST_LFB_YORIGIN;
    }
  switch ( color_format ) {
  case GR_COLORFORMAT_ABGR:
    lfbmode |= SST_LFB_RGBALANES_ABGR;
    break;
  case GR_COLORFORMAT_ARGB:
    lfbmode |= SST_LFB_RGBALANES_ARGB;
    break;
  case GR_COLORFORMAT_RGBA:
    lfbmode |= SST_LFB_RGBALANES_RGBA;
    break;
  case GR_COLORFORMAT_BGRA:
    lfbmode |= SST_LFB_RGBALANES_BGRA;
    break;
  }
  grSstWriteConfigFbi( SSTR_LFBMODE, lfbmode );
  _SST->state.fbi_config.lfbMode = lfbmode;
  
  grLfbBypassMode( GR_LFBBYPASS_DISABLE );
  grLfbConstantAlpha( 0xFF );
  grLfbConstantDepth( 0 );
  grLfbWriteMode( GR_LFBWRITEMODE_8888 );
  
  return FXTRUE;

} /* grSstOpen */

/*
** grSstQueryHardware
**
** Under the SST-1 C-simulator this emulates either a 1 or 2 TMU
** configuration.  By default the SST hardware emulates 2 TMU,
** but this can be controlled by an environment variable.
*/
FX_EXPORT FxBool 
FX_CSTYLE grSstQueryHardware( GrHwConfiguration *hw )
{
#if (  GLIDE_PLATFORM & GLIDE_SST_SIM )
  int i;
  char *fbi_mem, *tmu_mem;
  
  _HW_CONFIGURATION.num_sst   = 1;
  _SSTs[0].fbuf_size          = 2;
  
  _SSTs[0].base_ptr           = _grPciFindSST();
  _SSTs[0].lfb_ptr            = _SSTs[0].base_ptr + ( GLIDE_LFB_OFFSET >> 2 );
  _SSTs[0].tex_ptr            = _SSTs[0].base_ptr + ( GLIDE_TEX_OFFSET >> 2 );
  
  _SSTs[0].scanline_interleaved = FXFALSE;
  
  memset( &_SSTs[0].state, 0, sizeof( _SSTs[0].state ) );
  
  for ( i = 0; i < GLIDE_NUM_TMU; i++ ) {
    /* note that this sets next_ncc_table to 0, as it should */
    memset( &_SSTs[0].tmu_state[i], 0, sizeof( _SSTs[0].tmu_state[i] ) );
    _SSTs[0].state.current_mm[i] = GR_NULL_MIPMAP_HANDLE;
    _SSTs[0].tmu_state[i].ncc_mmids[0] = GR_NULL_MIPMAP_HANDLE;
    _SSTs[0].tmu_state[i].ncc_mmids[1] = GR_NULL_MIPMAP_HANDLE;
  }

  _SSTs[0].num_tmu = GLIDE_NUM_TMU;
  _SSTs[1].num_tmu = GLIDE_NUM_TMU;
  
  _SSTs[0].tmu_state[0].total_mem = 1L << 21;
  _SSTs[0].tmu_state[1].total_mem = 1L << 21;
  _SSTs[0].tmu_state[2].total_mem = 1L << 21;  
  
  _SSTs[0].mm_table.free_mmid = 0;
  
  /*
   ** check environment variables
   */
  if ( ( fbi_mem = getenv( "GLIDE_FBI_RAM" ) ) != 0 )
    {
      long fbi_ram = atol( fbi_mem );

      switch ( fbi_ram )
        {
        case 1:
        case 2:
        case 4:
          _SSTs[0].fbuf_size = fbi_ram;
          break;
        default:
          GrErrorCallback( "grSstQueryHardware:  invalid GLIDE_FBI_RAM setting (1, 2, 4)", FXTRUE );
          break;
        }
    }
  if ( ( tmu_mem = getenv( "GLIDE_TMU_RAM" ) ) != 0 )
    {
      long tmu_ram = atol( tmu_mem );

      switch ( tmu_ram )
        {
        case 1:
          _SSTs[0].tmu_state[0].total_mem = 1L << 20;
          _SSTs[0].tmu_state[1].total_mem = 1L << 20;
          break;
        case 2:
          _SSTs[0].tmu_state[0].total_mem = 1L << 21;
          _SSTs[0].tmu_state[1].total_mem = 1L << 21;
          break;
        case 4:
          _SSTs[0].tmu_state[0].total_mem = 1L << 22;
          _SSTs[0].tmu_state[1].total_mem = 1L << 22;
          break;
        default:
          GrErrorCallback( "grSstQueryHardware:  invalid GLIDE_TMU_RAM setting (1, 2, 4)", FXTRUE );
          break;
        }
    }
  
#else
# if 0
  static FxBool
    initialized /*= FXFALSE*/;
# endif
  static FxBool
    retVal;
  int i;

# if 0
  if (initialized == FXFALSE) {
    initialized = FXTRUE;
# endif
    if((retVal = _grSstDetectResources()) == FXFALSE)
      return FXFALSE;
    
    for (i = 0; i < _HW_CONFIGURATION.num_sst; i++) {
      _SSTs[i].mm_table.free_mmid = 0;
    }
    
#endif
    
    *hw = _HW_CONFIGURATION;
    
#ifdef GLIDE_DEBUG
    for (i = 0; i < hw->num_sst; i++) {
      int tmuNum;
      if (hw->SSTs[i].type == GR_SSTTYPE_VOODOO) {
        printf("SST board %d: 3Dfx Voodoo Graphics\n", i);
        printf("\tPixelfx rev %d with %d MB Frame Buffer\n",
               hw->SSTs[i].sstBoard.VoodooConfig.fbiRev,
               hw->SSTs[i].sstBoard.VoodooConfig.fbRam
               );
        printf("\t%d Texelfx chips:\n",
               hw->SSTs[i].sstBoard.VoodooConfig.nTexelfx
               );
        if (hw->SSTs[i].sstBoard.VoodooConfig.sliDetect)
          printf("\tScanline Interleaved\n");
        for (
             tmuNum = 0;
             tmuNum < hw->SSTs[i].sstBoard.VoodooConfig.nTexelfx;
             tmuNum++ ) {
          printf("\t\tTexelfx %d: Rev %d, %d MB Texture\n", tmuNum,
                 hw->SSTs[i].sstBoard.VoodooConfig.tmuConfig[tmuNum].tmuRev,
                 hw->SSTs[i].sstBoard.VoodooConfig.tmuConfig[tmuNum].tmuRam
                 );
        }
      } else {
        GrErrorCallback("grSstQueryHardware: Fatal Invalid SST type!\n", FXTRUE);
      }
    }
#endif
# if 0
  }
# endif
  return retVal;

} /* grSstQueryHardware */

/*
** grSstSelect
*/
FX_EXPORT void 
FX_CSTYLE grSstSelect( int which )
{
  if ( which >= _HW_CONFIGURATION.num_sst )
    GrErrorCallback( "grSstSelect:  non-existent SST", FXTRUE );

  _SST = &_SSTs[which];

  _GLIDE_STATE.current_sst = which;
}/* grSstSelect */

/*
** The following routines are functions that access the SST-1
** C-simulator.  These routines are replaced by macros when 
** directly accessing the hardware.
*/
#if (\
    ( GLIDE_PLATFORM & GLIDE_SST_SIM ) && \
    (!(GLIDE_PLATFORM & GLIDE_SST_HW) )  && \
    !defined(GLIDE_BENCHMARK)              \
    )

void grSstWriteFixedAll( GrSstRegister r, FxI32 value )
{
#ifdef GLIDE_NO_PRODUCTION
   if ( !ignore_sst_writes )
#endif
      sstStore32( _SST->base_ptr + r, value );
}

void grSstWriteFloatAll( GrSstRegister r, float value )
{
#ifdef GLIDE_NO_PRODUCTION
   if ( !ignore_sst_writes )
#endif
      sstStore32f( _SST->base_ptr + r, value );
}

void grSstWriteConfigAll( GrSstRegister r, FxU32 value )
{
#ifdef GLIDE_NO_PRODUCTION
   if ( !ignore_sst_writes )
#endif
     sstStore32( _SST->base_ptr + r, value );
}

void grSstWriteAbsolute16( FxU32 address, FxU16 value )
{
#ifdef GLIDE_NO_PRODUCTION
   if ( !ignore_sst_writes )
#endif
      sstStore16( ( FxU16 * ) address, value );
}

void grSstWriteAbsolute32( FxU32 address, FxU32 value )
{
#ifdef GLIDE_NO_PRODUCTION
   if ( !ignore_sst_writes )
#endif
      sstStore32( ( FxU32 * ) address, value );
}

/*
** grSstWriteXXXFbi
*/
void grSstWriteFixedFbi( GrSstRegister r, FxI32 value )
{
#ifdef GLIDE_NO_PRODUCTION
   if ( !ignore_sst_writes )
#endif
      sstStore32( _SST->base_ptr + r + SST_FBI_ADDR, value );
}

void grSstWriteFloatFbi( GrSstRegister r, float value )
{
#ifdef GLIDE_NO_PRODUCTION
   if ( !ignore_sst_writes )
#endif
      sstStore32f( _SST->base_ptr + r + SST_FBI_ADDR, value );
}

void grSstWriteConfigFbi( GrSstRegister r, FxU32 value )
{
#ifdef GLIDE_DEBUG
  if ( r >= SSTR_END_OF_REGISTER_SET )
    GrErrorCallback( "grSstWriteConfig: register value out of range", FXFALSE );
#endif

#ifdef GLIDE_NO_PRODUCTION
   if ( !ignore_sst_writes )
#endif
      sstStore32( _SST->base_ptr + r + SST_FBI_ADDR, value );
}

/*
** grSstWriteXXXTMU
*/
void grSstWriteFixedTMU( GrChipID_t tmu, GrSstRegister r, FxI32 value )
{
#ifdef GLIDE_NO_PRODUCTION
    if ( !ignore_sst_writes )
#endif
    sstStore32( _SST->base_ptr + r + SST_TMU_ADDR(tmu), value );
}

void grSstWriteFloatTMU( GrChipID_t tmu, GrSstRegister r, float value )
{
#ifdef GLIDE_NO_PRODUCTION
    if ( !ignore_sst_writes )
#endif
    sstStore32f( _SST->base_ptr + r + SST_TMU_ADDR(tmu), value );
}

void grSstWriteConfigTMU( GrChipID_t tmu, GrSstRegister r, FxU32 value )
{
#ifdef GLIDE_NO_PRODUCTION
    if ( !ignore_sst_writes )
#endif
    sstStore32( _SST->base_ptr + r + SST_TMU_ADDR(tmu), value );
}

/*
** grSstWriteXXXTMU1
*/
#if (GLIDE_NUM_TMU > 1)
void grSstWriteFixedTMU1( GrSstRegister r, FxI32 value )
{
#ifdef GLIDE_NO_PRODUCTION
   if ( !ignore_sst_writes )
#endif
      sstStore32( _SST->base_ptr + r + SST_TMU1_ADDR, value );
}

void grSstWriteFloatTMU1( GrSstRegister r, float value )
{
#ifdef GLIDE_NO_PRODUCTION
   if ( !ignore_sst_writes )
#endif
      sstStore32f( _SST->base_ptr + r + SST_TMU1_ADDR, value );
}

void grSstWriteConfigTMU1( GrSstRegister r, FxU32 value )
{
#ifdef GLIDE_NO_PRODUCTION
   if ( !ignore_sst_writes )
#endif
      sstStore32( &_SST->base_ptr[r + SST_TMU1_ADDR], value );
}
#endif

/*
** grSstWriteXXXTMU2
*/
void grSstWriteFixedTMU2( GrSstRegister r, FxI32 value )
{

#ifdef GLIDE_NO_PRODUCTION
   if ( !ignore_sst_writes )
#endif
      sstStore32( _SST->base_ptr + r + SST_TMU2_ADDR, value );
}

void grSstWriteFloatTMU2( GrSstRegister r, float value )
{
#ifdef GLIDE_NO_PRODUCTION
   if ( !ignore_sst_writes )
#endif
      sstStore32f( _SST->base_ptr + r + SST_TMU2_ADDR, value );
}

void grSstWriteConfigTMU2( GrSstRegister r, FxU32 value )
{
#ifdef GLIDE_NO_PRODUCTION
   if ( !ignore_sst_writes )
#endif
      sstStore32( &_SST->base_ptr[r + SST_TMU2_ADDR], value );
}

/*
** grSstReadConfigXXXes
*/
FxU32 
grSstGetConfigFbi( GrSstRegister r ) {
  return  sstLoad32(_SST->base_ptr + r);
}
#endif

/*
** grSstScreenWidth
*/
FX_EXPORT int FX_CSTYLE 
grSstScreenWidth (void)
{

  return ( int ) _SST->state.screen_width;
}

/*
** grSstScreenHeight
*/
FX_EXPORT int FX_CSTYLE 
grSstScreenHeight (void)
{
  return ( int ) _SST->state.screen_height;
}

/***************** Register Debugging Stuff **********************/
static FILE *_sstRegDebugFile;
/*
** _grSstInitRegDebug
*/
int
_grSstInitRegDebug(void) {
  char *fname;

#if !defined(__GNUC__)
  fname = getenv("GR_REG_FILE_NAME");

  if ((_sstRegDebugFile = fopen(fname, "wab")) == NULL) {
    perror(fname);
    return 0;
  }
#endif
  return 1;
} /* _grSstInitRegDebug */

/*
** _grSstWriteRegs
*/
void
_grSstWriteRegs(void) {
#if !defined(__GNUC__)
  fwrite(_SST->base_ptr, SSTR_END_OF_REGISTER_SET, sizeof(long), _sstRegDebugFile);
#endif
} /* _grSstWriteRegs */

/*
**  _grSstShutDownRegDebug
*/
void
_grSstShutDownRegDebug(void) {
#if !defined(__GNUC__)
  fclose(_sstRegDebugFile);
#endif
} /* _grSstShutDownRegDebug */


/*
**  grSstPerfStats
*/ 
FX_EXPORT void FX_CSTYLE
grSstPerfStats(GrSstPerfStats_t *pStats) {
#ifdef GLIDE_DEBUG
  if (pStats == NULL) {
    GrErrorCallback(
                    "Error:  NULL pointer sent to grSstPerfStats",
                    FXTRUE);
  }
#endif

  pStats->pixelsIn = grSstGetConfigFbi(SSTR_FBIPIXELSIN);
  pStats->chromaFail = grSstGetConfigFbi(SSTR_FBICHROMAFAIL);
  pStats->zFuncFail = grSstGetConfigFbi(SSTR_FBIZFUNCFAIL);
  pStats->aFuncFail = grSstGetConfigFbi(SSTR_FBIAFUNCFAIL);
  pStats->pixelsOut = grSstGetConfigFbi(SSTR_FBIPIXELSOUT);

} /* grSstPerfStats */


/*
**  grStatsResetPerfStats
*/
FX_EXPORT void FX_CSTYLE
grSstResetPerfStats(void) {
  grSstWriteConfigAll(SSTR_NOPCMD, 1);
} /* grSstResetPerfStats */


/*
**  grSstPassthruMode - set the board in vga passthru mode or 
**  take it out of it.
*/
FX_EXPORT void FX_CSTYLE 
grSstPassthruMode( GrPassthruMode_t mode) {
#if !defined(__GNUC__)
printf("PassThruMode!!\n");
fflush(stdout);
#endif
  if (mode == GR_PASSTHRU_SHOW_VGA) {
    sst1InitVgaPassCtrl(_SST->base_ptr, FXTRUE);
  } else {
    sst1InitVgaPassCtrl(_SST->base_ptr, FXFALSE);
  }
} /* grSstSetPassthruMode */


/*
** grSstConfigPipeline
*/
FX_EXPORT void FX_CSTYLE 
grSstConfigPipeline(GrChipID_t chip, GrSstRegister reg, FxU32 value) {

  switch (chip) {
  case GR_FBI:
    grSstWriteConfigFbi(reg, value);
    break;
  case GR_TMU0:
  case GR_TMU1: 
  case GR_TMU2:
    grSstWriteConfigTMU(chip, reg, value);
    break;
  default:
#ifdef GLIDE_DEBUG
    {
    char outString[256];

    sprintf(outString, "grSstConfigPipeline: Bogus chip ID specified (%d)\n", chip);
    GrErrorCallback(outString, FXTRUE);
    }
#endif
    break;
  }
} /* grSstConfigPipeline */

/*
**  grSstStatus - return contents of status register
*/
FX_EXPORT FxU32 FX_CSTYLE 
grSstStatus(void) {

  FxU32 status = grSstGetConfigFbi(SSTR_STATUS);

  return status;

}/* grSstStatus */
  
/*
**  grSstVRetrace - return contents of SST_VRETRACE bit of status register;
*/
FX_EXPORT FxBool FX_CSTYLE 
grSstVRetraceOn(void) {
  if (grSstGetConfigFbi(SSTR_STATUS) & SST_VRETRACE)
    return FXFALSE;
  else
    return FXTRUE;
}/* grSstVRetrace */

/*
**  grSstIsBusy - find out if the SST is busy or not
*/
FX_EXPORT FxBool
FX_CSTYLE grSstIsBusy(void) {
#if (GLIDE_PLATFORM & GLIDE_SST_HW)
  if (grSstGetConfigFbi(SSTR_STATUS) & SST_BUSY)
    return FXTRUE;

  return FXFALSE;

#else
  return FXFALSE;
#endif
}/* grSstIsBusy */

/*
**  grGammaCorrectionValue - set the gamma correction value
*/
FX_EXPORT void
FX_CSTYLE grGammaCorrectionValue(float value) {
#if (GLIDE_PLATFORM & GLIDE_SST_HW)
    sst1InitGamma(_SST->base_ptr, (double) value);
#endif
} /* grGammaCorrectionValue */



/*
** grSstIdle
*/
FX_EXPORT void 
FX_CSTYLE grSstIdle(void) {
#if (GLIDE_PLATFORM & GLIDE_SST_HW)
  sst1InitIdle(_SST->base_ptr);
#else
  ;;
#endif
}/* grSstIdle */


/*
**  grSstOrigin - Set the orgin orientation of the screen.
**
**  Returns:
**
**  Notes:
**
*/
FX_EXPORT void FX_CSTYLE
grSstOrigin(GrOriginLocation_t origin )
{
#define FN_NAME "grSstOrigin"
  FxU32
    fbzMode = _SST->state.fbi_config.fbzMode;

  /* Initialize FBZMODE register */
  if (origin == GR_ORIGIN_LOWER_LEFT)
    fbzMode |= SST_YORIGIN;
  else
    fbzMode &= ~(SST_YORIGIN);

  grSstWriteConfigFbi(SSTR_FBZMODE, fbzMode);
  _SST->state.fbi_config.fbzMode = fbzMode;
  
#undef FN_NAME
} /* grSstOrigin */
