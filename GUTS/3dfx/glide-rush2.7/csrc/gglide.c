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
** $Header: /engr/devel/sst1/GLIDE/DOS/SRC/GGLIDE.C 88    6/19/96 3:05p Sellers $
** $Log: /engr/devel/sst1/GLIDE/DOS/SRC/GGLIDE.C $
 * 
 * 88    6/19/96 3:05p Sellers
 * fixed problems with splash when users repeatedly call glideInit() and
 * glideShutdown()
 * now grQueryHardware() properly returns FXFALSE and doesn't die if 3Dfx
 * hardware is not detected
 * 
 * 87    6/18/96 6:55p Sellers
 * fixed for SLI support and splash screen
 * 
 * 86    6/18/96 4:59p Dow
 * now restores state to correct tmus
 * 
 * 85    6/17/96 8:56a Dow
 * updated version string
 * 
 * 84    6/16/96 12:19p Sellers
 * fixed splash stuff for SLI (yet again...)
 * 
 * 83    6/14/96 6:26p Sellers
 * Fixed bogus setting of _SST->* before _SST was set
 * 
 * 82    6/14/96 5:17p Sellers
 * fixed for splash screen and SLI
 * fixed grBufferSwap() to be independent of .5 or .6 micron FBI
 * 
 * 81    6/14/96 10:04a Dow
 * Added grGlideShamelessPlug(FxBool on);
 * 
 * 80    6/13/96 10:06a Dow
 * Added boundschecking in debug mode
 * 
 * 78    6/12/96 4:16p Dow
 * Added splash and banner
 * Cleaned up stale (#if 0'd) code
 * Cleaned up clipWindow code
 * Disallowed depth buffering if 3 buffers were specified
 * 
 * 77    6/10/96 12:30a Jdt
 * Code to initialize state in _SST->state.tmu_config
 * 
 * 76    6/08/96 4:16p Dow
 *      Cleaned up #if 0'd code
 *      Added paramIndex validation code
 *      Changed the way paramIndex is generated to support hints
 *      Modified hints routine
 * 
 * 75    5/29/96 9:59p Dow
 * Added ENDITHERSUBTRACT, got rid of some warnings
 * 
 * 74    5/28/96 6:16a Dow
 * grFogTable:  Changed modification of const GrFog_t to local mod.
 * 
 * 73    5/28/96 8:29a Tarolli
 * rewrote fog table download code, a lot simpler and faster
 * 
 * 72    5/22/96 5:32p Dow
 * Upped ident string
 * 
 * 71    5/22/96 4:20p Dow
 * Made release tag ident string.
 * 
 * 70    5/22/96 11:24a Dow
 * fixed bug in grDepthBiasLevel
 * 
 * 69    5/20/96 12:58p Dow
 * added grBufferNumPending stall, and explained two calls to grSstStatus
 * around swap buffer command write
 * 
 * 68    5/14/96 8:14a Dow
 * Added grHints()
 * 
 * 67    5/09/96 4:39p Paik
 * Fix grColorMask/grDepthMask aux buffer write mask trash problem
 * 
 * 66    5/09/96 7:04a Paik
 * Glide DLL support
 * 
 * 65    5/01/96 2:21p Dow
 * removed fflush from grGlideShutdown
 * 
 * 64    4/27/96 10:16a Sellers
 * added workaround to fix hardware bug transitioning between
 * enabling/disabling texture mapping
 * 
 * 63    4/18/96 2:16p Paik
 * Latest change to sst1init.h requires sst.h be included before.
 * 
 * 62    4/06/96 3:36p Dow
 * Fixed fogging bug caused by state optimization.
 * 
 * 61    4/05/96 5:43p Dow
 * fixed captital g
 * 
 * 59    4/05/96 10:58a Tarolli
 * fixed last fog table entry bug
 * 
 * 58    3/25/96 7:58p Paik
 * Move fxglide.h out of glide.h
 * 
 * 57    3/22/96 10:22p Paik
 * Loop over TMUs instead of unrolled code
 * 
 * 56    3/22/96 10:03p Paik
 * Fix silly crash when no sst
 * 
 * 55    3/20/96 8:06a Paik
 * Use single macro for TMUs.
 * Fix glide bug01.
 * Fix nit about buffers pending.
 * 
 * 54    3/03/96 3:58p Dow
 * added sst1INitShutdown
 * 
 * 53    3/01/96 1:56p Paik
 * State optimization
 * 
 * 55    3/01/96 3:48a Paik
 * 
 * 54    2/29/96 11:58p Paik
 * 
 * 52    2/28/96 4:48p Dow
 * drawbuffer -> renderbuffer
 * 
 * 51    2/13/96 2:59p Dow
 * grDrawBuffer now takes a grBuffer_t
 * 
 * 50    2/09/96 5:00p Dow
 * Glide state stuff
 * 
 * 49    2/09/96 4:57p Dow
 * GrState size stuff
 * 
 * 48    2/06/96 6:34a Dow
 * Added buffernumpending, and changed the check for the grState pad a bit
 * 
 * 47    2/02/96 11:36p Paik
 * 
 * 48    1/30/96 10:19p Paik
 * Alpha buffer wasn't enabled properly, needed to also set
 * SST_ZAWRMASK.
 * 
 * 45    1/30/96 2:43a Dow
 * Removed ref to single_precision_asm
 * 
 * 44    1/24/96 3:48a Paik
 * Add grDrawBuffer
 * 
 * 43    1/22/96 11:05p Paik
 * Fixups for 2.0.a BL0
 * 
 * 42    1/22/96 10:40a Dow
 * Changed declaration of asm fixed point init stuff
 * 
 * 41    1/14/96 12:29p Dow
 * AAMode stuff & GLIDE_STATE_PAD_SIZE fix
 * 
 * 40    1/14/96 9:25a Dow
 * Chanded the function declarations to better suit emacs, changed the way
 * multiple tmu configurations are handled.
 * 
 * 39    1/13/96 2:43p Dow
 * Random fixes, and emacs helping stuff
 * 
 * 38    1/12/96 2:28p Paik
 * [sw-glide/225, sw-glide/242]
 * Executive summary
 * 1. created grAlphaCombine and grColorCombine which fully expose acu and
 * ccu.
 * 2. renamed grColorCombineFunction and grAlphaSource to
 * guColorCombineFunction and guAlphaSource, reimplement on top of
 * grColorCombine and grAlphaCombine.
 * 
 * 34    1/11/96 9:39p Dow
 * Got rid of case for alpha disable
 * 
 * 33    1/11/96 12:48p Dow
 * c:\apps\cwright\cwrsp000%
 * 
 * 32    1/11/96 2:26a Dow
 * Added GrTMU_t, changed functions taking tmus to use it.
 * Added tmuMask to state, use it to better control state setting when
 * moving between texture mapped and non texture mapped color combine
 * modes.
 * 
 * 31    1/10/96 11:12p Dow
 * more 1 & 3 tmu, virtual tmu removal fixes
 * 
 * 30    1/09/96 9:39a Dow
 * tabs->spaces && added Rev. Control keywords
**
*/

#if 0
static const char glideIdent[] = "@#%Glide Version 2.03";
#endif

#include <stdlib.h>
#include <stdio.h>

#include <3dfx.h>
#include <glidesys.h>
#if (GLIDE_PLATFORM & GLIDE_SST_HW)
#include <sst.h>
#include <sst1vid.h>
#include <sst1init.h>
#endif
#define FX_DLL_DEFINITION
#include <fxdll.h>
#include <glide.h>
#include "fxglide.h"

static FxBool shamelessPlug;

#ifdef GLIDE_DEBUG
FxBool
  _boundsCheck;
#endif /* GLIDE_DEBUG */

GrGlideState       _GLIDE_STATE;

GrSST             *_SST;
GrSST              _SSTs[MAX_NUM_SST];

const FxU32              _minMemFIFOFree = 0xffff;
const FxU32              _minPCIFIFOFree = 0x3f;

FxU32              _lastAddress;

extern grSstOpenLastRez;
extern grSstOpenLastRefresh;

/*
** _grUpdateParamIndex
**
** Updates the paramIndex bits based Glide state and the hints.
**
*/
void
_grUpdateParamIndex( void ) {
  FxU32
    paramIndex = 0,
    hints = _SST->state.paramHints,
    /* Squirrel away some SST registers */
    fbzColorPath = _SST->state.fbi_config.fbzColorPath,
    fogMode = _SST->state.fbi_config.fogMode,
    fbzMode = _SST->state.fbi_config.fbzMode;

  /*
  ** First, turn on every bit that we think we need. We can prune them
  ** back later.
  */

  /* Turn on the texture bits based on what grTexCombin set */
  if (fbzColorPath & SST_ENTEXTUREMAP) {
    /* we need texture, so get the bitfield that grTexCombineFunction sets */
    switch (_SST->num_tmu) {
    case 3:
      if (_SST->state.tmuMask & GR_TMUMASK_TMU2) {
        paramIndex |= STATE_REQUIRES_W_TMU2;
        paramIndex |= STATE_REQUIRES_ST_TMU2;
      }
    case 2:
      if (_SST->state.tmuMask & GR_TMUMASK_TMU1) {
        paramIndex |= STATE_REQUIRES_W_TMU1;
        paramIndex |= STATE_REQUIRES_ST_TMU1;
      }
    case 1:
      if (_SST->state.tmuMask & GR_TMUMASK_TMU0) {
        paramIndex |= STATE_REQUIRES_W_TMU0;
        paramIndex |= STATE_REQUIRES_ST_TMU0;
      }
      break;
    default:
      break;
    }
  }  

  /* See if we need iteterated RGB) */
  if ( _SST->state.cc_requires_it_rgb && !_SST->state.cc_delta0mode )
    paramIndex |= STATE_REQUIRES_IT_DRGB;

  /* See if we need to iterate alpha based on the value of
     ac_requires_it_alpha */ 
  if (_SST->state.ac_requires_it_alpha)
    paramIndex |= STATE_REQUIRES_IT_ALPHA;

  /* See what fbzMode contributes */
  if (fbzMode & SST_ENDEPTHBUFFER) {
    if (fbzMode & SST_WBUFFER)
      paramIndex |= STATE_REQUIRES_OOW_FBI;
    else
      paramIndex |= STATE_REQUIRES_OOZ;
  }

  /* See what fogMode contributes */
  if (fogMode & SST_ENFOGGING) {
    if (fogMode & SST_FOG_Z) {
      paramIndex |= STATE_REQUIRES_OOZ;
    } else {
      if (fogMode & SST_FOG_ALPHA)
        paramIndex |= STATE_REQUIRES_IT_ALPHA;
      else 
        paramIndex |= STATE_REQUIRES_OOW_FBI;
    }
  }

  /*
  **  Now we know everything that needs to be iterated.  Prune back
  **  the stuff that isn't explicitly different
  */
  /* Turn off W for TMU0 if we have W for FBI, and don't have a hint */
  if (paramIndex & STATE_REQUIRES_W_TMU0) {
    if (
        !(hints & GR_STWHINT_W_DIFF_TMU0) &&
        (paramIndex & STATE_REQUIRES_OOW_FBI)
        )
      paramIndex &= ~STATE_REQUIRES_W_TMU0;
  }
  
  /* Turn off ST for TMU1 if it's not different */
  if (
      (paramIndex & STATE_REQUIRES_ST_TMU1) &&
      !(hints & GR_STWHINT_ST_DIFF_TMU1) &&
      ((paramIndex & STATE_REQUIRES_ST_TMU0))
      )
    paramIndex &= ~STATE_REQUIRES_ST_TMU1;
  
  /* Turn off W for TMU1 if we have a previous w, and don't
     have a hint */ 
  if (
      (paramIndex & STATE_REQUIRES_W_TMU1) &&
      !(hints & GR_STWHINT_W_DIFF_TMU1) &&
      (
       (paramIndex & STATE_REQUIRES_OOW_FBI) ||
       (paramIndex & STATE_REQUIRES_W_TMU0)
       )
      )
    paramIndex &= ~STATE_REQUIRES_W_TMU1;

#if (GLIDE_NUM_TMU > 2)
  /*
     Turn off ST for TMU1 if it's not different & any other is set up.
     */ 
  if (
      (paramIndex & STATE_REQUIRES_ST_TMU2) &&
      !(hints & GR_STWHINTS_ST_DIFF_TMU2) &&
      (
       (paramIndex & STATE_REQUIRES_ST_TMU0) ||
       (paramIndex & STATE_REQURES_ST_TMU1)
       )
      )
    paramIndex &= ~STATE_REQUIRES_ST_TMU2;
  
  /* Turn off W for TMU1 if we have W for FBI (or TMU0), and don't
     have a hint */ 
  if (
      (paramIndex & STATE_REQUIRES_W_TMU2) &&
      !(hints & GR_STWHINTS_W_DIFF_TMU2) &&
      (
       (paramIndex & STATE_REQUIRES_OOW_FBI) ||
       (paramIndex & STATE_REQUIRES_OOW_TMU0) ||
       (paramIndex & STATE_REQURES_OOW_TMU1)
       )
      )
    paramIndex &= ~STATE_REQUIRES_W_TMU2;
#endif
  
  _SST->state.paramIndex = paramIndex;

}/* _grUpdateParamIndex */

#ifdef GLIDE_DEBUG
static FxBool
_grCanSupportDepthBuffer( void ) {
  if ( _SST->state.screen_height == 640 ) {
    if ( _SST->fbuf_size == 1 ) {
      return FXFALSE;
    }
  } else if ( _SST->state.screen_width == 800 ) {
    if ( ( _SST->fbuf_size == 1 ) ||
        ( _SST->fbuf_size == 2 ) ) {
      return FXFALSE;
    }
  }
  return FXTRUE;
} /* _grCanSupportDepthBuffer */
#endif

void 
_grSwizzleColor( GrColor_t *color )
{
  unsigned long red, green, blue, alpha;
  
  switch( _SST->state.color_format ) {
  case GR_COLORFORMAT_ARGB:
    break;
  case GR_COLORFORMAT_ABGR:
    blue = *color & 0x00ff;
    red = ( *color >> 16 ) & 0xff;
    *color &= 0xff00ff00;
    *color |= ( blue << 16 );
    *color |= red;
    break;
  case GR_COLORFORMAT_RGBA:
    red    = ( *color & 0x0000ff00 ) >> 8;
    green  = ( *color & 0x00ff0000 ) >> 16;
    blue   = ( *color & 0xff000000 ) >> 24;
    alpha  = ( *color & 0x000000ff );
    *color = ( alpha << 24 ) | ( blue << 16 ) | ( green << 8 ) | red;
    break;
  case GR_COLORFORMAT_BGRA:
    red    = ( *color & 0xff000000 ) >> 24;
    green  = ( *color & 0x00ff0000 ) >> 16;
    blue   = ( *color & 0x0000ff00 ) >> 8;
    alpha  = ( *color & 0x000000ff );
    *color = ( alpha << 24 ) | ( blue << 16 ) | ( green << 8 ) | red;
    break;
  }
}/* _grSwizzleColor */

/*
** grAlphaBlendFunction
**
*/
FX_EXPORT void FX_CSTYLE 
grAlphaBlendFunction( GrAlphaBlendFnc_t rgb_sf,   GrAlphaBlendFnc_t rgb_df,
                     GrAlphaBlendFnc_t alpha_sf, GrAlphaBlendFnc_t alpha_df )
{
  FxU32 alphamode = _SST->state.fbi_config.alphaMode;

#if defined GLIDE_DEBUG && 0
  if ( ( rgb_sf == GR_BLEND_DST_ALPHA ) || ( rgb_df == GR_BLEND_DST_ALPHA ) ||
       ( rgb_sf == GR_BLEND_ONE_MINUS_DST_ALPHA ) || ( rgb_df == GR_BLEND_ONE_MINUS_DST_ALPHA ) ||
      ( rgb_sf == GR_BLEND_ALPHA_SATURATE ) || ( rgb_df == GR_BLEND_ALPHA_SATURATE ) )
    {
      GrErrorCallback( "grAlphaBlendFunction: alpha buffer currently unsupported", FXTRUE );
    }
#endif
  
  if ( rgb_sf   == GR_BLEND_ONE && rgb_df   == GR_BLEND_ZERO &&
      alpha_sf == GR_BLEND_ONE && alpha_df == GR_BLEND_ZERO )
    alphamode &= ~SST_ENALPHABLEND;
  else
    alphamode |=  SST_ENALPHABLEND;
  
  alphamode &= ~( SST_RGBSRCFACT | SST_RGBDSTFACT | SST_ASRCFACT | SST_ADSTFACT );
  
  alphamode |= ( ( ( FxU32 ) rgb_sf ) << SST_RGBSRCFACT_SHIFT ) |
    ( ( ( FxU32 ) rgb_df ) << SST_RGBDSTFACT_SHIFT ) |
      ( ( ( FxU32 ) alpha_sf ) << SST_ASRCFACT_SHIFT ) |
        ( ( ( FxU32 ) alpha_df ) << SST_ADSTFACT_SHIFT );
  
  grSstWriteConfigFbi( SSTR_ALPHAMODE, alphamode );

  _SST->state.fbi_config.alphaMode = alphamode;
} /* grAlphaBlendFunction */

/*
** grAlphaCombine
*/
FX_EXPORT void FX_CSTYLE
grAlphaCombine(GrCombineFunction_t function, GrCombineFactor_t factor,
               GrCombineLocal_t local, GrCombineOther_t other,
               FxBool invert ) {
  FxU32 fbzColorPath = _SST->state.fbi_config.fbzColorPath;
  FxU32 oldTextureEnabled, newTextureEnabled;

  oldTextureEnabled = fbzColorPath & SST_ENTEXTUREMAP;

#ifdef GLIDE_DEBUG
  if ( function < GR_COMBINE_FUNCTION_ZERO ||
      function > GR_COMBINE_FUNCTION_SCALE_MINUS_LOCAL_ADD_LOCAL_ALPHA ) {
    GrErrorCallback( "grAlphaCombine:  unsupported alpha combine function", FXFALSE );
    return;
  }
  if ( (factor & 0x7) < GR_COMBINE_FACTOR_ZERO ||
      (factor & 0x7) > GR_COMBINE_FACTOR_TEXTURE_ALPHA ||
      factor > GR_COMBINE_FACTOR_ONE_MINUS_TEXTURE_ALPHA ) {
    GrErrorCallback( "grAlphaCombine:  unsupported alpha combine scale factor", FXFALSE );
    return;
  }
  if ( local < GR_COMBINE_LOCAL_ITERATED ||
      local > GR_COMBINE_LOCAL_DEPTH )
  {
    GrErrorCallback( "grAlphaCombine:  unsupported alpha combine local color", FXFALSE );
    return;
  }
  if ( other < GR_COMBINE_OTHER_ITERATED ||
      other > GR_COMBINE_OTHER_CONSTANT )
    {
      GrErrorCallback( "grAlphaCombine:  unsupported color combine other color", FXFALSE );
    return;
    }
#endif
  fbzColorPath &= ~( SST_ASELECT |
                    SST_ALOCALSELECT |
                    SST_CCA_ZERO_OTHER |
                    SST_CCA_SUB_CLOCAL |
                    SST_CCA_MSELECT |
                    SST_CCA_REVERSE_BLEND |
                    SST_CCA_ADD_CLOCAL |
                    SST_CCA_ADD_ALOCAL |
                    SST_CCA_INVERT_OUTPUT );
  
  /* setup requires flags */
  
  _SST->state.ac_requires_texture = FXFALSE;
  _SST->state.ac_requires_it_alpha = FXFALSE;
  
  if ( !_SST->state.cc_requires_texture )
    fbzColorPath &= ~( SST_ENTEXTUREMAP );
  
  /* setup scale factor bits */
  
  fbzColorPath |= ( factor & 0x7 ) << SST_CCA_MSELECT_SHIFT;
  
  if ( !( factor & 0x8 ) )
    fbzColorPath |= SST_CCA_REVERSE_BLEND;
  
  if ( ( factor & 0x7 ) == GR_COMBINE_FACTOR_TEXTURE_ALPHA ) {
    _SST->state.ac_requires_texture = FXTRUE;
    fbzColorPath |= SST_ENTEXTUREMAP;
  }
  
  /* setup local color bits */
  
  fbzColorPath |= local << SST_ALOCALSELECT_SHIFT;
  
  if ( local == GR_COMBINE_LOCAL_ITERATED )
    _SST->state.ac_requires_it_alpha = FXTRUE;
  
  /* xxx should clean up requires ooz handling -- future
     else if ( local == GR_COMBINE_LOCAL_DEPTH )
     _SST->state.ac_requires_ooz = FXTRUE; */
  
  /* setup other color bits */
  
  fbzColorPath |= other << SST_ASELECT_SHIFT;
  
  if ( other == GR_COMBINE_OTHER_TEXTURE ) {
      _SST->state.ac_requires_texture = FXTRUE;
      fbzColorPath |= SST_ENTEXTUREMAP;
    }
  
  else if ( other == GR_COMBINE_OTHER_ITERATED )
    _SST->state.ac_requires_it_alpha = FXTRUE;
  
  /* setup invert output bits */
  
  if ( invert )
    fbzColorPath |= SST_CCA_INVERT_OUTPUT;
  
  /* setup core color combine unit bits */
  
  switch ( function ) {
  case GR_COMBINE_FUNCTION_ZERO:
    fbzColorPath |= SST_CCA_ZERO_OTHER;
    break;
    
  case GR_COMBINE_FUNCTION_LOCAL:
  case GR_COMBINE_FUNCTION_LOCAL_ALPHA:
    fbzColorPath |= SST_CCA_ZERO_OTHER | SST_CCA_ADD_ALOCAL;
    break;
    
  case GR_COMBINE_FUNCTION_SCALE_OTHER:
    break;
    
  case GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL:
  case GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL_ALPHA:
    fbzColorPath |= SST_CCA_ADD_ALOCAL;
    break;
    
  case GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL:
    fbzColorPath |= SST_CCA_SUB_CLOCAL;
    break;
    
  case GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL_ADD_LOCAL:
  case GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL_ADD_LOCAL_ALPHA:
    fbzColorPath |= SST_CCA_SUB_CLOCAL | SST_CCA_ADD_ALOCAL;
    break;
    
  case GR_COMBINE_FUNCTION_SCALE_MINUS_LOCAL_ADD_LOCAL:
  case GR_COMBINE_FUNCTION_SCALE_MINUS_LOCAL_ADD_LOCAL_ALPHA:
    fbzColorPath |= SST_CCA_ZERO_OTHER | SST_CCA_SUB_CLOCAL | SST_CCA_ADD_ALOCAL;
    break;
  }
  
  /* update register */
  newTextureEnabled = fbzColorPath & SST_ENTEXTUREMAP;

  /* transition into/out of texturing ... add nopCMD */
  if(oldTextureEnabled != newTextureEnabled)
    grSstWriteConfigFbi( SSTR_NOPCMD, 0x0 );
  
  grSstWriteConfigFbi( SSTR_FBZCOLORPATH, fbzColorPath );
  
  _SST->state.fbi_config.fbzColorPath = fbzColorPath;
  
  /* setup paramIndex bits */
  _grUpdateParamIndex();
  
} /* grAlphaCombine */

/*
** grAlphaControlsITRGBLighting
** 
** Determines whether the LSB of alpha controls what lighting is used--
** Specifically whether grConstantColorValu or the interated RGB values are used
** during TEXTURE_TIMES_itrgb & TEXTURE_TIMES_ITRGB_DELTA0 color combine modes.
**
*/  
FX_EXPORT void FX_CSTYLE
grAlphaControlsITRGBLighting(FxBool enable) {
  FxU32 fbzColorPath = _SST->state.fbi_config.fbzColorPath;

  if ( enable ) {
    fbzColorPath |= SST_LOCALSELECT_OVERRIDE_WITH_ATEX;
  } else {
    fbzColorPath &= ~SST_LOCALSELECT_OVERRIDE_WITH_ATEX;
  }

  grSstWriteConfigFbi(SSTR_FBZCOLORPATH, fbzColorPath);

  _SST->state.fbi_config.fbzColorPath = fbzColorPath;
} /* grAlphaControlsITRGBLighting() */

/*
** grAlphaTestFunction
*/
FX_EXPORT void FX_CSTYLE
grAlphaTestFunction( GrCmpFnc_t fnc ) {
  FxU32 alphamode = _SST->state.fbi_config.alphaMode;

  alphamode &= ~(SST_ALPHAFNC | SST_ENALPHAFUNC);

  if ( fnc != GR_CMP_ALWAYS )
    alphamode |= ( ( fnc << SST_ALPHAFNC_SHIFT ) | SST_ENALPHAFUNC );

  grSstWriteConfigFbi( SSTR_ALPHAMODE, alphamode );

  _SST->state.fbi_config.alphaMode = alphamode;
} /* grAlphaTestFunction */

/*
** grAlphaTestReferenceValue
*/
FX_EXPORT void FX_CSTYLE
grAlphaTestReferenceValue( GrAlpha_t value ) {
  FxU32 alphamode = _SST->state.fbi_config.alphaMode;

  alphamode &= ~SST_ALPHAREF;
  alphamode |= ( ( ( FxU32 ) value ) << SST_ALPHAREF_SHIFT );
  grSstWriteConfigFbi( SSTR_ALPHAMODE, alphamode );

  _SST->state.fbi_config.alphaMode = alphamode;
} /* grAlphaTestReferenceValue */

/*
** grBufferClear
*/
FX_EXPORT void FX_CSTYLE
grBufferClear( GrColor_t color, GrAlpha_t alpha, FxU16 depth ) {
  GrColor_t oldc1  = _SST->state.fbi_config.color1;
  FxU32 oldzacolor = _SST->state.fbi_config.zaColor;
  FxU32 zacolor    = oldzacolor;

  /*
  ** Setup source registers
  */
  if ( _SST->state.fbi_config.fbzMode & SST_RGBWRMASK )
  {
    _grSwizzleColor( &color );
    grSstWriteConfigFbi( SSTR_C1, color );
  }
  if ( ( _SST->state.fbi_config.fbzMode & ( SST_ENALPHABUFFER | SST_ZAWRMASK ) ) == ( SST_ENALPHABUFFER | SST_ZAWRMASK ) )
  {
    zacolor &= ~SST_ZACOLOR_ALPHA;
    zacolor |= ( ( ( FxU32 ) alpha ) << SST_ZACOLOR_ALPHA_SHIFT );
    grSstWriteConfigFbi( SSTR_ZACOLOR, zacolor );
  }
  if ( ( _SST->state.fbi_config.fbzMode & ( SST_ENDEPTHBUFFER | SST_ZAWRMASK ) ) == ( SST_ENDEPTHBUFFER | SST_ZAWRMASK ) )  {
    zacolor &= ~SST_ZACOLOR_DEPTH;
    zacolor |= ( ( ( FxU32 ) depth ) << SST_ZACOLOR_DEPTH_SHIFT );
    grSstWriteConfigFbi( SSTR_ZACOLOR, zacolor );
  }

  /*
  ** Execute the FASTFILL command
  */
  grSstWriteConfigAll( SSTR_FASTFILLCMD, 1 );

  /*
  ** Restore C1 and ZACOLOR
  */
  grSstWriteConfigFbi( SSTR_C1, oldc1 );
  grSstWriteConfigFbi( SSTR_ZACOLOR, oldzacolor );
} /* grBufferClear */

/*
** grBufferSwap
*/
FX_EXPORT void FX_CSTYLE
grBufferSwap( int swap_interval ) {
#ifndef GLIDE_SHAME
  void grBanner();
#endif 
  int interval = swap_interval;
#ifdef GLIDE_DEBUG
  if ( ( swap_interval > 255 ) || ( swap_interval < 0 ) )
    GrErrorCallback( "grBufferSwap: swap_interval out of range", FXTRUE );
#endif
#ifndef GLIDE_SHAME
  if (shamelessPlug)
    grBanner();
#endif 
  while (grBufferNumPending() > 6);
  if((interval > 0) && _HW_CONFIGURATION.SSTs[_GLIDE_STATE.current_sst].sstBoard.VoodooConfig.fbiRev == 1) {
    /* Adjust for .6 micron FBI */
    if(interval == 2)
        interval = 3;
    else if(interval != 1)
        interval <<= 1;
  } else {
    if (interval > 1) interval = swap_interval - 1;
  }

  /*
     The two calls to grSstStatus work around a 'creep' 'feature' in the
     buffernumpending stuff in the chip.
   */
  grSstStatus();
  grSstWriteConfigAll( SSTR_SWAPBUFFERCMD, ( interval << 1 ) | (interval > 0) );
  grSstStatus();
} /* grBufferSwap */

/*
** grBufferNumPending
*/
FX_EXPORT int FX_CSTYLE 
grBufferNumPending(void) {
  FxU32
    sstStatus;
  int
    bufSwapPending;

  sstStatus = grSstGetConfigFbi(SSTR_STATUS);

  bufSwapPending = (sstStatus & SST_SWAPBUFPENDING) >> SST_SWAPBUFPENDING_SHIFT;

  return bufSwapPending;

}/* grBufferNumPending */

/*
** grChromakeyMode
*/
FX_EXPORT void FX_CSTYLE
grChromakeyMode( GrChromakeyMode_t mode ) {

  FxU32 fbzMode = _SST->state.fbi_config.fbzMode;

  if ( mode == GR_CHROMAKEY_ENABLE )
    fbzMode |= SST_ENCHROMAKEY;
  else
    fbzMode &= ~SST_ENCHROMAKEY;

  grSstWriteConfigFbi( SSTR_FBZMODE, fbzMode );

  _SST->state.fbi_config.fbzMode = fbzMode;
} /* grChromaKeyMode */

/*
** grChromakeyValue
*/
FX_EXPORT void FX_CSTYLE
grChromakeyValue( GrColor_t color ) {
  _grSwizzleColor( &color );
  grSstWriteConfigFbi( SSTR_CHROMAKEY, color );
  _SST->state.fbi_config.chromaKey = color;
}/* grChromaKeyValue */

/*
** grClipWindow
*/
FX_EXPORT void FX_CSTYLE
grClipWindow( int minx, int miny, int maxx, int maxy ) {

  FxU32
    clipLeftRight,              /* SST Clipping Registers */
    clipBottomTop;

  clipLeftRight =
    ( minx << SST_CLIPLEFT_SHIFT ) | ( maxx << SST_CLIPRIGHT_SHIFT ); 

  clipBottomTop =
    ( miny << SST_CLIPBOTTOM_SHIFT ) | ( maxy << SST_CLIPTOP_SHIFT );

  grSstWriteConfigFbi( SSTR_CLIPLEFTRIGHT, clipLeftRight);
  grSstWriteConfigFbi( SSTR_CLIPBOTTOMTOP, clipBottomTop);

  _SST->state.fbi_config.clipLeftRight = clipLeftRight;
  _SST->state.fbi_config.clipBottomTop = clipBottomTop;

  _SST->state.clipwindowf_xmin = ( float ) minx;
  _SST->state.clipwindowf_xmax = ( float ) maxx;
  _SST->state.clipwindowf_ymin = ( float ) miny;
  _SST->state.clipwindowf_ymax = ( float ) maxy;

} /* grClipWindow */

/*
** grColorCombine
*/
FX_EXPORT void FX_CSTYLE
grColorCombine( GrCombineFunction_t function, GrCombineFactor_t factor, GrCombineLocal_t local, GrCombineOther_t other, FxBool invert )
{
  FxU32 fbzColorPath = _SST->state.fbi_config.fbzColorPath;
  FxU32 oldTextureEnabled, newTextureEnabled;

  oldTextureEnabled = fbzColorPath & SST_ENTEXTUREMAP;

#ifdef GLIDE_DEBUG
  if ( function < GR_COMBINE_FUNCTION_ZERO ||
       function > GR_COMBINE_FUNCTION_SCALE_MINUS_LOCAL_ADD_LOCAL_ALPHA )
  {
    GrErrorCallback( "grColorCombine:  unsupported color combine function", FXFALSE );
    return;
  }
  if ( (factor & 0x7) < GR_COMBINE_FACTOR_ZERO ||
       (factor & 0x7) > GR_COMBINE_FACTOR_TEXTURE_ALPHA ||
       factor > GR_COMBINE_FACTOR_ONE_MINUS_TEXTURE_ALPHA )
  {
    GrErrorCallback( "grColorCombine:  unsupported color combine scale factor", FXFALSE );
    return;
  }
  if ( local < GR_COMBINE_LOCAL_ITERATED ||
       local > GR_COMBINE_LOCAL_CONSTANT )
  {
    GrErrorCallback( "grColorCombine:  unsupported color combine local color", FXFALSE );
    return;
  }
  if ( other < GR_COMBINE_OTHER_ITERATED ||
       other > GR_COMBINE_OTHER_CONSTANT )
  {
    GrErrorCallback( "grColorCombine:  unsupported color combine other color", FXFALSE );
    return;
  }
#endif

  fbzColorPath &= ~( SST_RGBSELECT |
                     SST_LOCALSELECT |
                     SST_CC_ZERO_OTHER |
                     SST_CC_SUB_CLOCAL |
                     SST_CC_MSELECT |
                     SST_CC_REVERSE_BLEND |
                     SST_CC_ADD_CLOCAL |
                     SST_CC_ADD_ALOCAL |
                     SST_CC_INVERT_OUTPUT );

  /* this is bogus, it should be done once, somewhere. */

  fbzColorPath |= SST_PARMADJUST;

  /* setup requires flags */

  _SST->state.cc_requires_texture = FXFALSE;
  _SST->state.cc_requires_it_rgb = FXFALSE;

  if ( !_SST->state.ac_requires_texture )
    fbzColorPath &= ~( SST_ENTEXTUREMAP );

  /* setup scale factor bits */

  fbzColorPath |= ( factor & 0x7 ) << SST_CC_MSELECT_SHIFT;

  if ( !( factor & 0x8 ) )
    fbzColorPath |= SST_CC_REVERSE_BLEND;

  if ( ( factor & 0x7 ) == GR_COMBINE_FACTOR_TEXTURE_ALPHA )
  {
     _SST->state.cc_requires_texture = FXTRUE;
    fbzColorPath |= SST_ENTEXTUREMAP;
  }
    
  /* setup local color bits */

  fbzColorPath |= local << SST_LOCALSELECT_SHIFT;

  if ( local == GR_COMBINE_LOCAL_ITERATED )
    _SST->state.cc_requires_it_rgb = FXTRUE;

  /* setup other color bits */

  fbzColorPath |= other << SST_RGBSELECT_SHIFT;

  if ( other == GR_COMBINE_OTHER_TEXTURE )
  {
     _SST->state.cc_requires_texture = FXTRUE;
     fbzColorPath |= SST_ENTEXTUREMAP;
  }

  else if ( other == GR_COMBINE_OTHER_ITERATED )
    _SST->state.cc_requires_it_rgb = FXTRUE;

  /* setup invert output bits */

  if ( invert )
    fbzColorPath |= SST_CC_INVERT_OUTPUT;

  /* setup core color combine unit bits */

  switch ( function )
  {
  case GR_COMBINE_FUNCTION_ZERO:
    fbzColorPath |= SST_CC_ZERO_OTHER;
    break;

  case GR_COMBINE_FUNCTION_LOCAL:
    fbzColorPath |= SST_CC_ZERO_OTHER | SST_CC_ADD_CLOCAL;
    break;

  case GR_COMBINE_FUNCTION_LOCAL_ALPHA:
    fbzColorPath |= SST_CC_ZERO_OTHER | SST_CC_ADD_ALOCAL;
    break;

  case GR_COMBINE_FUNCTION_SCALE_OTHER:
    break;

  case GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL:
    fbzColorPath |= SST_CC_ADD_CLOCAL;
    break;

  case GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL_ALPHA:
    fbzColorPath |= SST_CC_ADD_ALOCAL;
    break;

  case GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL:
    fbzColorPath |= SST_CC_SUB_CLOCAL;
    break;

  case GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL_ADD_LOCAL:
    fbzColorPath |= SST_CC_SUB_CLOCAL | SST_CC_ADD_CLOCAL;
    break;

  case GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL_ADD_LOCAL_ALPHA:
    fbzColorPath |= SST_CC_SUB_CLOCAL | SST_CC_ADD_ALOCAL;
    break;

  case GR_COMBINE_FUNCTION_SCALE_MINUS_LOCAL_ADD_LOCAL:
    fbzColorPath |= SST_CC_ZERO_OTHER | SST_CC_SUB_CLOCAL | SST_CC_ADD_CLOCAL;
    break;

  case GR_COMBINE_FUNCTION_SCALE_MINUS_LOCAL_ADD_LOCAL_ALPHA:
    fbzColorPath |= SST_CC_ZERO_OTHER | SST_CC_SUB_CLOCAL | SST_CC_ADD_ALOCAL;
    break;
  }

  /* update register */
  newTextureEnabled = fbzColorPath & SST_ENTEXTUREMAP;
  if(oldTextureEnabled != newTextureEnabled)
    /* transition into/out of texturing ... add nopCMD */
    grSstWriteConfigFbi( SSTR_NOPCMD, 0x0 );

  grSstWriteConfigFbi( SSTR_FBZCOLORPATH, fbzColorPath );

  _SST->state.fbi_config.fbzColorPath = fbzColorPath;

  /* setup paramIndex bits */

  _grUpdateParamIndex();

}/* grColorCombine */

void 
_grColorCombineDelta0Mode( FxBool delta0mode )
{
  _SST->state.cc_delta0mode = delta0mode;
}/* _grColorCombineDeltaMode */

FX_EXPORT void FX_CSTYLE
grColorMask( FxBool rgb, FxBool alpha ) {
  FxU32 fbzmode = _SST->state.fbi_config.fbzMode;

#ifdef GLIDE_DEBUG
  if ( ( fbzmode & SST_ENDEPTHBUFFER ) && alpha )
  {
     GrErrorCallback( "grColorMask:  alpha writes enabled even though depth buffering", FXFALSE );
  }
  if ( _SST->state.num_buffers == 3 && alpha )
  {
     GrErrorCallback( "grColorMask:  alpha writes enabled while triple buffering", FXFALSE );
  }
#endif

  if ( rgb )
    fbzmode |= SST_RGBWRMASK;
  else
    fbzmode &= ~SST_RGBWRMASK;

  /* Don't do anything if depth buffering on */
  /* XXX Should check for triple buffering too */

  if ( !(fbzmode & SST_ENDEPTHBUFFER) && (_SST->state.num_buffers != 3) ) {
    if ( alpha )
      fbzmode |= SST_ENALPHABUFFER | SST_ZAWRMASK;
    else
      fbzmode &= ~(SST_ENALPHABUFFER | SST_ZAWRMASK);
  }

  grSstWriteConfigFbi( SSTR_FBZMODE, fbzmode );

  _SST->state.fbi_config.fbzMode = fbzmode;
}/* grColorMask */

FX_EXPORT void FX_CSTYLE
grConstantColorValue( GrColor_t color ) {
  _grSwizzleColor( &color );
  grSstWriteConfigFbi( SSTR_C0, color );
  grSstWriteConfigFbi( SSTR_C1, color );
  _SST->state.fbi_config.color0 = color;
  _SST->state.fbi_config.color1 = color;
}/* grConstanColorValue */

FX_EXPORT void FX_CSTYLE
grConstantColorValue4( float a, float r, float g, float b ) {
  _SST->state.a = a;
  _SST->state.r = r;
  _SST->state.g = g;
  _SST->state.b = b;
}/* grConstantColorValue4 */

FX_EXPORT void FX_CSTYLE
grCullMode( GrCullMode_t mode ) {
  _SST->state.cull_mode = mode;
}/* grCullMode */

/*
** grDepthBiasLevel
**
** Sets the depth bias level.
*/
FX_EXPORT void FX_CSTYLE
grDepthBiasLevel( FxI16 level ) {
  FxU32 zacolor = _SST->state.fbi_config.zaColor;

  zacolor = ( zacolor & ~SST_ZACOLOR_DEPTH ) | (level & SST_ZACOLOR_DEPTH);
  grSstWriteConfigFbi( SSTR_ZACOLOR, zacolor );
  _SST->state.fbi_config.zaColor = zacolor;
}/* grDepthBiasLevel */

FX_EXPORT void FX_CSTYLE
grDepthBufferFunction( GrCmpFnc_t fnc ) {
  FxU32 fbzmode = _SST->state.fbi_config.fbzMode;

  fbzmode &= ~SST_ZFUNC;
  fbzmode |= ( fnc << SST_ZFUNC_SHIFT );
  grSstWriteConfigFbi( SSTR_FBZMODE, fbzmode );
    
  _SST->state.fbi_config.fbzMode = fbzmode;
}/* grDepthBufferFunction */

/*
** grDepthBufferMode
*/
FX_EXPORT void FX_CSTYLE
grDepthBufferMode( GrDepthBufferMode_t mode ) {
  FxU32 fbzmode = _SST->state.fbi_config.fbzMode;

#ifdef GLIDE_DEBUG
  if ( mode != GR_DEPTHBUFFER_DISABLE ) {
     /*
     ** depth buffering cannot be enabled at the same time as triple buffering
     */
     if ( _SST->state.num_buffers == 3 ) {
        GrErrorCallback( "grDepthBufferMode:  cannot enable depth buffer when triple buffering", FXTRUE );
     }

     /*
     ** depth buffering cannot be enabled on systems running at:
     **      800x600 w/ 2MB
     **      640x480 w/ 1MB
     */
     if ( !_grCanSupportDepthBuffer() ) {
       GrErrorCallback( "grDepthBufferMode:  cannot enable depthbuffer with configuration", FXTRUE );
     }
   }
#endif

  if ( mode == GR_DEPTHBUFFER_DISABLE ) {
    fbzmode &= ~SST_ENDEPTHBUFFER;
  } else {
    fbzmode |= SST_ENDEPTHBUFFER;
    if ( mode == GR_DEPTHBUFFER_WBUFFER ) 
      fbzmode |= SST_WBUFFER;
    else
      fbzmode &= ~SST_WBUFFER;
  }

  /*
  ** Update hardware and Glide state
  */
  grSstWriteConfigFbi( SSTR_FBZMODE, fbzmode );

  _SST->state.fbi_config.fbzMode = fbzmode;

  _grUpdateParamIndex();

}/* grDepthBufferMode */

/*
** grDepthMask
*/
FX_EXPORT void FX_CSTYLE
grDepthMask( FxBool enable ) {
  FxU32 fbzmode = _SST->state.fbi_config.fbzMode;

#ifdef GLIDE_DEBUG
  if ( !( fbzmode & SST_ENDEPTHBUFFER ) && enable )
  {
     GrErrorCallback( "grDepthMask:  depth mask writes enabled even though no depth buffering", FXFALSE );
  }
  if ( ( fbzmode & SST_ENALPHABUFFER ) && enable )
  {
     GrErrorCallback( "grDepthMask:  depth mask writes enabled even though alpha buffering", FXFALSE );
  }
  if ( _SST->state.num_buffers == 3 )
  {
     GrErrorCallback( "grDepthMask:  executed while triple buffering", FXFALSE );
  }
  if ( enable && ( !_grCanSupportDepthBuffer() ) )
  {
     GrErrorCallback( "grDepthMask:  executed in a non-depthbufferable configuration", FXTRUE );
  }
#endif

  if ( enable )
    fbzmode |= SST_ZAWRMASK;
  else
    fbzmode &= ~SST_ZAWRMASK;

  grSstWriteConfigFbi( SSTR_FBZMODE, fbzmode );

  _SST->state.fbi_config.fbzMode = fbzmode;
}/* grDepthMask */

/*
** grDisableAllEffects
*/
FX_EXPORT void FX_CSTYLE
grDisableAllEffects( void ) {
  grAlphaBlendFunction( GR_BLEND_ONE, GR_BLEND_ZERO, GR_BLEND_ONE, GR_BLEND_ZERO );
  grAlphaTestFunction( GR_CMP_ALWAYS );
  grChromakeyMode( GR_CHROMAKEY_DISABLE );
  grDepthBiasLevel( 0 );
  grDepthBufferMode( GR_DEPTHBUFFER_DISABLE );
  grFogMode( GR_FOG_DISABLE );
}

/*
** grDitherMode
*/
FX_EXPORT void FX_CSTYLE
grDitherMode( GrDitherMode_t mode ) {
  FxU32 fbzMode = _SST->state.fbi_config.fbzMode;

  fbzMode &= ~( SST_ENDITHER | SST_DITHER2x2 | SST_ENDITHERSUBTRACT );

  switch ( mode )
  {
     case GR_DITHER_DISABLE:
        break;
     case GR_DITHER_2x2:
        fbzMode |= ( SST_ENDITHER | SST_DITHER2x2 );
        break;
     case GR_DITHER_4x4:
        fbzMode |= (SST_ENDITHER | SST_ENDITHERSUBTRACT);
        break;
  }

  grSstWriteConfigFbi( SSTR_FBZMODE, fbzMode );

  _SST->state.fbi_config.fbzMode = fbzMode;

}/* grDitherMode */

/*
** grFogMode
*/
FX_EXPORT void FX_CSTYLE
grFogMode( GrFogMode_t mode ) {
  FxU32 fogmode = _SST->state.fbi_config.fogMode;

  fogmode &= ~( SST_ENFOGGING | SST_FOGADD | SST_FOGMULT | SST_FOG_ALPHA | SST_FOG_Z | SST_FOG_CONSTANT );

  switch ( mode & 0xff ) {
  case GR_FOG_DISABLE:
    break;
  case GR_FOG_WITH_ITERATED_ALPHA:
    fogmode |= ( SST_ENFOGGING | SST_FOG_ALPHA );
    break;
  case GR_FOG_WITH_TABLE:
    fogmode |= ( SST_ENFOGGING );
  }
  /*
  ** Update the hardware and Glide state
  */
  grSstWriteConfigFbi( SSTR_FOGMODE, fogmode );
  _SST->state.fbi_config.fogMode = fogmode;

  _grUpdateParamIndex();
}/* grFogMode */

/*
** grFogColorValue
*/
FX_EXPORT void FX_CSTYLE 
grFogColorValue( GrColor_t color ) {      
  _grSwizzleColor( &color );
  grSstWriteConfigFbi( SSTR_FOGCOLOR, color );
  _SST->state.fbi_config.fogColor = color;
}/* grFogColorValue */

/*
** grFogTable
*/
FX_EXPORT void FX_CSTYLE
grFogTable( const GrFog_t fogtable[GR_FOG_TABLE_SIZE] ) {
  int i, iend = GR_FOG_TABLE_SIZE / 2 - 1;
  const GrFog_t *locTable = fogtable;

  for ( i = 0; i <= iend; i++ ) {
    GrFog_t e0,e1,d0,d1;

    e0 = locTable[0];                   /* lower entry */
    e1 = locTable[1];                   /* upper entry */
    d0 = (e1-e0)<<2;                    /* delta0 in .2 format */
    d1 = (i==iend) ? e1 : locTable[2];  /* don't access beyond end of table */
    d1 = (d1-e1)<<2;                    /*delta1 in .2 format */

    grSstWriteConfigFbi( SSTR_FOGTABLE + i, (e1<<24) | (d1<<16) | (e0<<8) | d0);
    locTable += 2;
  }
} /* grFogTable */

/*
** grRenderBuffer
**
**  Although SST1 supports triple buffering, it's a hack in the hardware,
**  and the only drawbuffer modes supported by the fbzMode register are 0
**  (back) and 1 (front)
*/
FX_EXPORT void FX_CSTYLE 
grRenderBuffer( GrBuffer_t buffer ) {
  FxU32 fbzMode = _SST->state.fbi_config.fbzMode;
  int
    lbuffer;                    /* eventual drawbuffer bits of fbzMode */
#ifdef GLIDE_DEBUG
  if (buffer > GR_BUFFER_AUXBUFFER)
    GrErrorCallback("(grDrawBuffer) Error:  invalid buffer\n", FXTRUE);
#endif

  fbzMode &= ~( SST_DRAWBUFFER );

  if (buffer == GR_BUFFER_FRONTBUFFER)
    lbuffer = 0;
  else
    lbuffer = 1;

  fbzMode |= lbuffer << SST_DRAWBUFFER_SHIFT;

  grSstWriteConfigFbi( SSTR_FBZMODE, fbzMode );

  _SST->state.fbi_config.fbzMode = fbzMode;

}/* grDrawBuffer */

/*
** grGlideInit
*/
FX_EXPORT void FX_CSTYLE 
grGlideInit( void ) {
  extern void _grErrorDefaultCallback( const char *s, FxBool fatal );
  static FxBool calledBefore /*= FXFALSE*/;
  GrErrorCallback = _grErrorDefaultCallback;


#ifdef GLIDE_DEBUG
  if ( sizeof( GrState ) > GLIDE_STATE_PAD_SIZE ) {
    char string[256];
    sprintf(string, "grGlideInit - (GLIDE_STATE_PAD == %d) < (sizeof(GrState == %d)\n", GLIDE_STATE_PAD_SIZE, sizeof(GrState));
    GrErrorCallback(string, FXTRUE );
  }

  if (getenv("FX_GLIDE_BOUNDS_CHECK"))
    _boundsCheck = FXTRUE;

#endif

  /* Hoopti Splash Screen Stuff */
#ifndef GLIDE_NO_SPLASH  
  if ((!(getenv("FX_GLIDE_NO_SPLASH"))) && (calledBefore == FXFALSE)) {
    void grSplash(void);
    GrHwConfiguration hwconfig;
    int jj;

    calledBefore = FXTRUE;
    if( grSstQueryHardware( &hwconfig ) == FXFALSE )
      return;
    grSstSelect(0);
    
    grSstOpen( GR_RESOLUTION_640x480, GR_REFRESH_60Hz, GR_COLORFORMAT_ABGR,
              GR_ORIGIN_LOWER_LEFT, GR_SMOOTHING_ENABLE, 2);

    guColorCombineFunction( GR_COLORCOMBINE_DECAL_TEXTURE );
    grTexCombineFunction( GR_TMU0, GR_TEXTURECOMBINE_DECAL );
    grCullMode( GR_CULL_NEGATIVE );
    grConstantColorValue( 0xffffffff );
    grDepthBufferMode( GR_DEPTHBUFFER_WBUFFER );
    grDepthBufferFunction( GR_CMP_LESS );
    grDepthMask( FXTRUE );

    grDepthBufferMode(GR_DEPTHBUFFER_WBUFFER);
    /* Split buffer swaps into separate calls so SLI works properly (hack!) */
    for(jj=0; jj<60; jj++)
        grBufferSwap(1);
    grSplash();
    grSstOpenLastRez = GR_RESOLUTION_640x480;
    grSstOpenLastRefresh = GR_REFRESH_60Hz;
    grSstIdle();
  }
#endif

#ifndef GLIDE_SHAME
  if (getenv("FX_GLIDE_SHAMELESS_PLUG")) {
    shamelessPlug = FXTRUE;
#ifdef GLIDE_DEBUG
    GrErrorCallback("Plugging...\n", FXFALSE);
#endif
  }
#endif    
}/* grGlideInit */

/*
** grGlideShutdown
*/
FX_EXPORT void FX_CSTYLE
grGlideShutdown( void ) {
#if ( GLIDE_PLATFORM & GLIDE_SST_SIM )
  extern void
    gsim_winclose( void );

  gsim_winclose();
#endif
#if (GLIDE_PLATFORM & GLIDE_SST_HW)
  if (_SST && _SST->base_ptr)
    sst1InitShutdown(_SST->base_ptr);
#endif
  
#ifdef GLIDE_DEBUG
  printf("grGlideShutdown:  Min PCI FIFO free:    0x%x\n", _minPCIFIFOFree);
  printf("grGlideShutdown:  Min Memory FIFO free: 0x%x\n", _minMemFIFOFree);
#endif
  grSstOpenLastRez = GR_RESOLUTION_NONE;
  grSstOpenLastRefresh = GR_REFRESH_NONE;

}/* grGlideShutdown */

FX_EXPORT void FX_CSTYLE
grGlideGetState( GrState *state ) {
  *state = _SST->state;
}/* grGlideGetState */

FX_EXPORT void FX_CSTYLE
grHints(GrHint_t hintType, FxU32 hints) {
#if ( (GLIDE_PLATFORM & GLIDE_SST_HW) && !defined(GLIDE_DONT_OPTIMIZE_TRISETUP) )
  extern GrVertexReg
    *_paramRegs,
    _regStructByHints[],
    *_paramRegsPtr,
    _regStruct;
#ifdef GLIDE_DEBUG
  if (hintType != GR_HINT_STWHINT)
    GrErrorCallback("grHints:  Warning invalid hints type.\n", FXFALSE );
  if (hints >= GR_MAX_STW_HINTS)
    GrErrorCallback("grHints:  Warning invalid hints set.\n", FXFALSE );
#endif
  _SST->state.paramHints = hints;

  _regStruct = _regStructByHints[hints];
  _paramRegs = &_regStructByHints[hints];
  _paramRegsPtr = &_regStruct;
  
#endif
}/* grHints */

/*
** grGlideSetState
*/
FX_EXPORT void FX_CSTYLE
grGlideSetState( const GrState *state ) {
  int i;

  /* Update the hardware state from the saved state */

  grSstWriteConfigFbi( SSTR_FBZCOLORPATH, state->fbi_config.fbzColorPath );
  grSstWriteConfigFbi( SSTR_FOGMODE, state->fbi_config.fogMode );
  grSstWriteConfigFbi( SSTR_ALPHAMODE, state->fbi_config.alphaMode );
  grSstWriteConfigFbi( SSTR_FBZMODE, state->fbi_config.fbzMode );
  grSstWriteConfigFbi( SSTR_LFBMODE, state->fbi_config.lfbMode );
  grSstWriteConfigFbi( SSTR_CLIPLEFTRIGHT, state->fbi_config.clipLeftRight );
  grSstWriteConfigFbi( SSTR_CLIPBOTTOMTOP, state->fbi_config.clipBottomTop );
  grSstWriteConfigFbi( SSTR_FOGCOLOR, state->fbi_config.fogColor );
  grSstWriteConfigFbi( SSTR_ZACOLOR, state->fbi_config.zaColor );
  grSstWriteConfigFbi( SSTR_CHROMAKEY, state->fbi_config.chromaKey );
  grSstWriteConfigFbi( SSTR_STIPPLE, state->fbi_config.stipple );
  grSstWriteConfigFbi( SSTR_C0, state->fbi_config.color0 );
  grSstWriteConfigFbi( SSTR_C1, state->fbi_config.color1 );

  for ( i = 0; i < _SST->num_tmu; i += 1 ) {
    grSstWriteConfigTMU( SST_TMU_INDEX_TO_CHIP(i), SSTR_TEXTUREMODE,
                        state->tmu_config[i].textureMode ); 
    grSstWriteConfigTMU( SST_TMU_INDEX_TO_CHIP(i), SSTR_TLOD,
                        state->tmu_config[i].tLOD ); 
    grSstWriteConfigTMU( SST_TMU_INDEX_TO_CHIP(i), SSTR_TDETAIL,
                        state->tmu_config[i].tDetail ); 
    grSstWriteConfigTMU( SST_TMU_INDEX_TO_CHIP(i), SSTR_TEXBASEADDR,
                        state->tmu_config[i].texBaseAddr ); 
    grSstWriteConfigTMU( SST_TMU_INDEX_TO_CHIP(i), SSTR_TEXBASEADDR1,
                        state->tmu_config[i].texBaseAddr_1 ); 
    grSstWriteConfigTMU( SST_TMU_INDEX_TO_CHIP(i), SSTR_TEXBASEADDR2,
                        state->tmu_config[i].texBaseAddr_2 ); 
    grSstWriteConfigTMU( SST_TMU_INDEX_TO_CHIP(i), SSTR_TEXBASEADDR38,
                        state->tmu_config[i].texBaseAddr_3_8 ); 
  }

  _grUpdateParamIndex();

  _SST->state = *state;
}/* grGlideSetState */



/*
**  grGlideShamelessPlug - grGlideShamelessPlug
**
**  Returns:
**
**  Notes:
**
*/
FX_EXPORT void FX_CSTYLE
grGlideShamelessPlug( const FxBool on ) {
#define FN_NAME "grGlideShamelessPlug"
  shamelessPlug = on;
#undef FN_NAME
} /* grGlideShamelessPlug */
