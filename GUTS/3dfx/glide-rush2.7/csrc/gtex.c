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
** $Header: /engr/devel/sst1/GLIDE/DOS/SRC/GTEX.C 62    6/18/96 4:59p Dow $
** $Log: /engr/devel/sst1/GLIDE/DOS/SRC/GTEX.C $
 * 
 * 62    6/18/96 4:59p Dow
 * resett state message now based on contents of state structure
 * 
 * 61    6/16/96 5:05p Jdt
 * Fixed grTexMipoMapMode and grTexSource, mmlevelmask was in wrong
 * function.
 * 
 * 60    6/14/96 10:42p Jdt
 * 
 * 59    6/13/96 7:15p Jdt
 * Removed yet more debug spew.
 * 
 * 58    6/13/96 7:11p Jdt
 * Removed some debugging spew
 * 
 * 57    6/13/96 6:50p Jdt
 * Fixed a bug in grMipMapMode.  Had to grab some state from guTexSource
 * this dependancy will vanish when mmid based management is re-written on
 * the new texture
 * 
 * 56    6/10/96 12:41a Jdt
 * Export aspect_bit_table
 * Replaced grTxMipMapMode with new state management new arguments
 * Added:
 *   grTexNCCTable
 *   grTexMinAddress
 *   grTexMaxAddress
 *   grTexTextureMemRequired
 *   grTexSource
 *   grTexMultibase( stub )
 *   grTexMultibaseAddress( stub )
 * 
 * 55    6/08/96 4:17p Dow
 *      Changed the way paramIndex is updated, based on hints & validation
 * routine.
 *      Added conditional compilation for some 3TMU code
 * 
 * 54    6/03/96 1:17p Dow
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
 * 53    5/20/96 11:55a Dow
 * Fixed up NCC texture stuff.
 * 
 * 52    5/09/96 7:05a Paik
 * Glide DLL support
 * 
 * 51    5/08/96 3:07p Paik
 * Clean up of build warnings
 * 
 * 50    3/27/96 3:44p Paik
 * Fix Texture memory reset problem
 * 
 * 49    3/25/96 7:58p Paik
 * Move fxglide.h out of glide.h
 * 
 * 48    3/20/96 8:07a Paik
 * Use single macro for TMUs
 * 
 * 47    3/01/96 1:57p Paik
 * State optimization
 * 
 * 48    3/01/96 3:49a Paik
 * 
 * 47    3/01/96 1:15a Paik
 * 
 * 46    2/29/96 11:57p Paik
 * Fix errors and warnings
 * 
 * 45    2/29/96 6:00p Dow
 * freemem_base in reset
 * 
 * 44    2/20/96 2:34p Paik
 * Fix function declarations to match prototypes.
 * 
 * 43    2/11/96 4:21p Dow
 * fixed bug dorking gary mct
 * 
 * 42    2/02/96 11:56p Paik
 * Add new texture combine.
 * Change old texture combine to use new.  Move out into a gu file later.
 * 
 * 43    1/31/96 6:04p Paik
 * Add new texture combine and change old texture combine to use new one.
 * 
 * 42    1/31/96 3:05a Paik
 * Working on texture combine
 * 
 * 41    1/24/96 8:20a Paik
 * Split texture maps (trilinear) and some 3 TMU fixups.
 * 
 * 40    1/22/96 11:05p Paik
 * Fixups for 2.0.a BL0
 * 
 * 39    1/14/96 5:49p Dow
 * backed out last revision
 * 
 * 37    1/14/96 9:30a Dow
 * Changed the way multiple TMU configs are handled.
 * 
 * 36    1/13/96 2:43p Dow
 * Random fixes, and emacs helping stuff
 * 
 * 32    1/11/96 9:41p Dow
 * Changed the filtermode and clamp mode semantics to work with sst.h
 * 
 * 31    1/11/96 2:26a Dow
 * Added GrTMU_t, changed functions taking tmus to use it.
 * Added tmuMask to state, use it to better control state setting when
 * moving between texture mapped and non texture mapped color combine
 * modes.
 * 
 * 30    1/11/96 12:56a Dow
 * 3 tmu fix
 * 
 * 29    1/10/96 11:12p Dow
 * more 1 & 3 tmu, virtual tmu removal fixes
 * 
 * 28    1/09/96 9:39a Dow
 * tabs->spaces && added Rev. Control keywords
**
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <3dfx.h>

#define FX_DLL_DEFINITION
#include <fxdll.h>
#include <glide.h>
#include "fxglide.h"

const FxU32 aspect_bit_table[] =
{
   3,
   2,
   1,
   0,
   1,
   2,
   3
};

static FxU16
_grTexFloatLODToFixedLOD( float value )
{
  float num_quarters;
  int   new_value;

  num_quarters = ( value + .125F ) / .25F;
  new_value    = ( int ) num_quarters;

  new_value   &= 0x003F;

  return new_value;
}

static void
_guTexRebuildRegisterShadows( GrMipMapId_t mmid );

/*
** guTexMemQueryAvail
**
** returns the amount of available texture memory on a specified TMU.
*/
FX_EXPORT FxU32 FX_CSTYLE
guTexMemQueryAvail( GrChipID_t tmu ) {
  FxU32 memavail = 0;
#ifdef GLIDE_DEBUG
  if ( tmu < GR_TMU0 || tmu >= SST_TMU_INDEX_TO_CHIP(_SST->num_tmu) )
  {
    GrErrorCallback( "guTexMemQueryAvaile: invalid tmu specified", FXTRUE );
  }
#endif
  memavail = _SST->tmu_state[SST_TMU_CHIP_TO_INDEX(tmu)].total_mem - _SST->tmu_state[SST_TMU_CHIP_TO_INDEX(tmu)].freemem_base;
  return memavail;
} /* guTexQueryMemAvail */

/*
** guTexAllocateMemory
*/
FX_EXPORT GrMipMapId_t FX_CSTYLE
guTexAllocateMemory( GrChipID_t tmu,
                    FxU8 odd_even_mask,
                    int width, int height,
                    GrTextureFormat_t format,
                    GrMipMapMode_t mipmap_mode,
                    GrLOD_t lod_min, GrLOD_t lod_max,
                    GrAspectRatio_t aspect_ratio,
                    GrTextureClampMode_t s_clamp_mode,
                    GrTextureClampMode_t t_clamp_mode,
                    GrTextureFilterMode_t minfilter_mode,
                    GrTextureFilterMode_t magfilter_mode,
                    float lod_bias,
                    FxBool trilinear
                    ) {
   FxU32        memrequired;
   FxU32        memavail;

   GrMipMapId_t mmid = (GrMipMapId_t) GR_NULL_MIPMAP_HANDLE;

   FxU32 base_address;

   int          int_lod_bias;
   FxU32        s_is_wider;
   FxU32        aspect_mask;

   FxU32 tlod = 0;
   FxU32 
     texturemode = 0,
     filterMode,        /* filter mode bits */
     clampMode;         /* clamp mode bits */
   

   FxI32         i;

#ifdef GLIDE_DEBUG
  /*
  ** The constants are actually reverse of each other so the following
  ** test IS valid!
  */
  if ( lod_min < lod_max )
    GrErrorCallback( "guTexAllocateMemory: smallest_lod is larger than largest_lod", FXTRUE );
#endif

  memrequired = 0;

  for ( i = lod_max; i <= lod_min; i++ )
  {
    if ( i & 1 )
    {
      if ( odd_even_mask & GR_MIPMAPLEVELMASK_ODD )
        memrequired += grTexCalcMemRequired( i, i, aspect_ratio, format );
    }
    else
    {
      if ( odd_even_mask & GR_MIPMAPLEVELMASK_EVEN )
        memrequired += grTexCalcMemRequired( i, i, aspect_ratio, format );
    }
  }

  /*
  ** If we have enough memory and a free mip map handle then go for it
  */
  memavail = guTexMemQueryAvail( tmu );

  if ( memavail < memrequired )
    return (GrMipMapId_t) GR_NULL_MIPMAP_HANDLE;

  if (_SST->mm_table.free_mmid >= MAX_MIPMAPS_PER_SST )
    return (GrMipMapId_t) GR_NULL_MIPMAP_HANDLE;

  /*
  ** Allocate the mip map id
  */
  mmid = _SST->mm_table.free_mmid++;

  /*
  ** calculate baseaddress
  */
  base_address = _grTexCalcBaseAddress( _SST->tmu_state[SST_TMU_CHIP_TO_INDEX(tmu)].freemem_base, 
                 lod_max, 
                 aspect_ratio, 
                 format, 
                 odd_even_mask ) >> 3;

  /*
  ** reduce available memory to reflect allocation
  */
  _SST->tmu_state[SST_TMU_CHIP_TO_INDEX(tmu)].freemem_base += memrequired;

  /*
  ** Create the tLOD register value for this mip map
  */
  int_lod_bias = _grTexFloatLODToFixedLOD( lod_bias );

  if ( aspect_ratio < GR_ASPECT_1x1 )
    s_is_wider = SST_LOD_S_IS_WIDER;
  else
    s_is_wider = 0;

  aspect_mask = aspect_bit_table[aspect_ratio];

  if ( mipmap_mode == GR_MIPMAP_DISABLE )
    tlod = ( ( s_is_wider ) | ( aspect_mask << SST_LOD_ASPECT_SHIFT ) | ( int_lod_bias << SST_LODBIAS_SHIFT ) | ( ( lod_max << 2 ) << SST_LODMAX_SHIFT ) | ( lod_max << 2 ) );
  else
    tlod = ( ( s_is_wider ) | ( aspect_mask << SST_LOD_ASPECT_SHIFT ) | ( int_lod_bias << SST_LODBIAS_SHIFT ) | ( ( lod_min << 2 ) << SST_LODMAX_SHIFT ) | ( lod_max << 2 ) );
  
  filterMode = (
    (minfilter_mode == GR_TEXTUREFILTER_BILINEAR ? SST_TMINFILTER : 0) |
    (magfilter_mode == GR_TEXTUREFILTER_BILINEAR ? SST_TMAGFILTER : 0)
  );
  
  clampMode = (
    (s_clamp_mode == GR_TEXTURECLAMP_CLAMP ? SST_TCLAMPS : 0) |
    (t_clamp_mode == GR_TEXTURECLAMP_CLAMP ? SST_TCLAMPT : 0)
  );  
  
  /*
  ** Create the tTextureMode register value for this mip map
  */
  texturemode |= ( format << SST_TFORMAT_SHIFT );
  texturemode |= SST_TCLAMPW;
  texturemode |= SST_TPERSP_ST;
  texturemode |= filterMode;
  texturemode |= clampMode;

  if ( mipmap_mode == GR_MIPMAP_NEAREST_DITHER )
    texturemode |= SST_TLODDITHER;

  if ( trilinear )
  {
    texturemode |= SST_TRILINEAR;

    if ( odd_even_mask & GR_MIPMAPLEVELMASK_ODD )
      tlod |= SST_LOD_ODD;

    if ( odd_even_mask != GR_MIPMAPLEVELMASK_BOTH )
      tlod |= SST_LOD_TSPLIT;
  }

  /*
  ** Fill in the mm_table data for this mip map
  */
  _SST->mm_table.data[mmid].format         = format;
  _SST->mm_table.data[mmid].mipmap_mode    = mipmap_mode;
  _SST->mm_table.data[mmid].magfilter_mode = magfilter_mode;
  _SST->mm_table.data[mmid].minfilter_mode = minfilter_mode;
  _SST->mm_table.data[mmid].s_clamp_mode   = s_clamp_mode;
  _SST->mm_table.data[mmid].t_clamp_mode   = t_clamp_mode;
  _SST->mm_table.data[mmid].tLOD           = tlod;
  _SST->mm_table.data[mmid].tTextureMode   = texturemode;
  _SST->mm_table.data[mmid].lod_bias       = int_lod_bias;
  _SST->mm_table.data[mmid].lod_min        = lod_min;
  _SST->mm_table.data[mmid].lod_max        = lod_max;
  _SST->mm_table.data[mmid].tmu            = tmu;
  _SST->mm_table.data[mmid].odd_even_mask  = odd_even_mask;
  _SST->mm_table.data[mmid].tmu_base_address = base_address;
  _SST->mm_table.data[mmid].trilinear      = trilinear;
  _SST->mm_table.data[mmid].aspect_ratio   = aspect_ratio;
  _SST->mm_table.data[mmid].data           = 0;
/*   _SST->mm_table.data[mmid].ncc_table      = 0; */
  _SST->mm_table.data[mmid].sst            = _GLIDE_STATE.current_sst;
  _SST->mm_table.data[mmid].valid          = FXTRUE;
  _SST->mm_table.data[mmid].width          = width;
  _SST->mm_table.data[mmid].height         = height;

  return mmid;
} /* guTexAllocateMemory */

/*
** grTexClampMode
*/
FX_EXPORT void FX_CSTYLE
grTexClampMode( GrChipID_t tmu,
               GrTextureClampMode_t s_clamp_mode,
               GrTextureClampMode_t t_clamp_mode ) {
  FxU32 texturemode;
  FxU32 clampMode = (
    (s_clamp_mode == GR_TEXTURECLAMP_CLAMP ? SST_TCLAMPS : 0) |
    (t_clamp_mode == GR_TEXTURECLAMP_CLAMP ? SST_TCLAMPT : 0)
  );

#ifdef GLIDE_DEBUG
  if ( tmu < GR_TMU0 || tmu >= SST_TMU_INDEX_TO_CHIP(_SST->num_tmu) )
  {
    GrErrorCallback( "grTexClampMode: invalid tmu specified", FXTRUE );
  }
#endif
  
  texturemode  = _SST->state.tmu_config[SST_TMU_CHIP_TO_INDEX(tmu)].textureMode;
  texturemode &= ~( SST_TCLAMPS | SST_TCLAMPT );
  texturemode |=  clampMode;
  grSstWriteConfigTMU( tmu, SSTR_TEXTUREMODE, texturemode );
  _SST->state.tmu_config[SST_TMU_CHIP_TO_INDEX(tmu)].textureMode = texturemode;
}/* grTexClampMode */

/*
** grTexCombine
*/
FX_EXPORT void FX_CSTYLE
grTexCombine(
             GrChipID_t tmu,
             GrCombineFunction_t rgb_function,
             GrCombineFactor_t rgb_factor, 
             GrCombineFunction_t alpha_function,
             GrCombineFactor_t alpha_factor,
             FxBool rgb_invert, FxBool alpha_invert
             ) {
  FxU32 texmode;
  FxU32 tmuMask;

#ifdef GLIDE_DEBUG
  if ( tmu < GR_TMU0 || tmu >= SST_TMU_INDEX_TO_CHIP(_SST->num_tmu) )
  {
    GrErrorCallback( "grTexCombine: invalid tmu specified", FXTRUE );
  }
  if ( rgb_function < GR_COMBINE_FUNCTION_ZERO ||
       rgb_function > GR_COMBINE_FUNCTION_SCALE_MINUS_LOCAL_ADD_LOCAL_ALPHA ) {
    GrErrorCallback( "grTexCombine:  unsupported texture color combine function", FXFALSE );
    return;
  }
  if ( alpha_function < GR_COMBINE_FUNCTION_ZERO ||
       alpha_function > GR_COMBINE_FUNCTION_SCALE_MINUS_LOCAL_ADD_LOCAL_ALPHA ) {
    GrErrorCallback( "grTexCombine:  unsupported texture alpha combine function", FXFALSE );
    return;
  }
  if ( (rgb_factor & 0x7) < GR_COMBINE_FACTOR_ZERO ||
       (rgb_factor & 0x7) > GR_COMBINE_FACTOR_LOD_FRACTION ||
       rgb_factor > GR_COMBINE_FACTOR_ONE_MINUS_LOD_FRACTION ) {
    GrErrorCallback( "grTexCombine:  unsupported texture color combine scale factor", FXFALSE );
    return;
  }
  if ( (alpha_factor & 0x7) < GR_COMBINE_FACTOR_ZERO ||
       (alpha_factor & 0x7) > GR_COMBINE_FACTOR_LOD_FRACTION ||
       alpha_factor > GR_COMBINE_FACTOR_ONE_MINUS_LOD_FRACTION ) {
    GrErrorCallback( "grTexCombine:  unsupported texture alpha combine scale factor", FXFALSE );
    return;
  }
#endif

  /* tmuMask tells grColorCombineFunction what to turn off an on if 
     the ccFunc requires texture mapping */
  texmode  = _SST->state.tmu_config[SST_TMU_CHIP_TO_INDEX(tmu)].textureMode;
  tmuMask = GR_TMUMASK_TMU0 << SST_TMU_CHIP_TO_INDEX(tmu);

  texmode &= ~(SST_TCOMBINE | SST_TACOMBINE);

  _SST->state.tmuMask &= ~tmuMask;

  /* setup scale factor bits */

  texmode |= ( rgb_factor & 0x7 ) << SST_TC_MSELECT_SHIFT;

  if ( !( rgb_factor & 0x8 ) )
    texmode |= SST_TC_REVERSE_BLEND;

  if ( ( ( rgb_factor & 0x7 ) == GR_COMBINE_FACTOR_LOCAL ) ||
       ( ( rgb_factor & 0x7 ) == GR_COMBINE_FACTOR_LOCAL_ALPHA ) )
    _SST->state.tmuMask |= tmuMask;

  texmode |= ( alpha_factor & 0x7 ) << SST_TCA_MSELECT_SHIFT;

  if ( !( alpha_factor & 0x8 ) )
    texmode |= SST_TCA_REVERSE_BLEND;

  if ( ( ( alpha_factor & 0x7 ) == GR_COMBINE_FACTOR_LOCAL ) ||
       ( ( alpha_factor & 0x7 ) == GR_COMBINE_FACTOR_LOCAL_ALPHA ) )
    _SST->state.tmuMask |= tmuMask;

  /* setup invert output bits */

  if ( rgb_invert )
    texmode |= SST_TC_INVERT_OUTPUT;

  if ( alpha_invert )
    texmode |= SST_TCA_INVERT_OUTPUT;

  /* setup core color combine unit bits */
  
  switch ( rgb_function )
  {
  case GR_COMBINE_FUNCTION_ZERO:
    texmode |= SST_TC_ZERO_OTHER;
    break;

  case GR_COMBINE_FUNCTION_LOCAL:
    texmode |= SST_TC_ZERO_OTHER | SST_TC_ADD_CLOCAL;
    _SST->state.tmuMask |= tmuMask;
    break;

  case GR_COMBINE_FUNCTION_LOCAL_ALPHA:
    texmode |= SST_TC_ZERO_OTHER | SST_TC_ADD_ALOCAL;
    _SST->state.tmuMask |= tmuMask;
    break;

  case GR_COMBINE_FUNCTION_SCALE_OTHER:
    break;

  case GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL:
    texmode |= SST_TC_ADD_CLOCAL;
    _SST->state.tmuMask |= tmuMask;
    break;

  case GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL_ALPHA:
    texmode |= SST_TC_ADD_ALOCAL;
    _SST->state.tmuMask |= tmuMask;
    break;

  case GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL:
    texmode |= SST_TC_SUB_CLOCAL;
    _SST->state.tmuMask |= tmuMask;
    break;

  case GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL_ADD_LOCAL:
    texmode |= SST_TC_SUB_CLOCAL | SST_TC_ADD_CLOCAL;
    _SST->state.tmuMask |= tmuMask;
    break;

  case GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL_ADD_LOCAL_ALPHA:
    texmode |= SST_TC_SUB_CLOCAL | SST_TC_ADD_ALOCAL;
    _SST->state.tmuMask |= tmuMask;
    break;

  case GR_COMBINE_FUNCTION_SCALE_MINUS_LOCAL_ADD_LOCAL:
    texmode |= SST_TC_ZERO_OTHER | SST_TC_SUB_CLOCAL | SST_TC_ADD_CLOCAL;
    _SST->state.tmuMask |= tmuMask;
    break;

  case GR_COMBINE_FUNCTION_SCALE_MINUS_LOCAL_ADD_LOCAL_ALPHA:
    texmode |= SST_TC_ZERO_OTHER | SST_TC_SUB_CLOCAL | SST_TC_ADD_ALOCAL;
    _SST->state.tmuMask |= tmuMask;
    break;
  }
  
  switch ( alpha_function )
  {
  case GR_COMBINE_FUNCTION_ZERO:
    texmode |= SST_TCA_ZERO_OTHER;
    break;

  case GR_COMBINE_FUNCTION_LOCAL:
    texmode |= SST_TCA_ZERO_OTHER | SST_TCA_ADD_CLOCAL;
    _SST->state.tmuMask |= tmuMask;
    break;

  case GR_COMBINE_FUNCTION_LOCAL_ALPHA:
    texmode |= SST_TCA_ZERO_OTHER | SST_TCA_ADD_ALOCAL;
    _SST->state.tmuMask |= tmuMask;
    break;

  case GR_COMBINE_FUNCTION_SCALE_OTHER:
    break;

  case GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL:
    texmode |= SST_TCA_ADD_CLOCAL;
    _SST->state.tmuMask |= tmuMask;
    break;

  case GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL_ALPHA:
    texmode |= SST_TCA_ADD_ALOCAL;
    _SST->state.tmuMask |= tmuMask;
    break;

  case GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL:
    texmode |= SST_TCA_SUB_CLOCAL;
    _SST->state.tmuMask |= tmuMask;
    break;

  case GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL_ADD_LOCAL:
    texmode |= SST_TCA_SUB_CLOCAL | SST_TCA_ADD_CLOCAL;
    _SST->state.tmuMask |= tmuMask;
    break;

  case GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL_ADD_LOCAL_ALPHA:
    texmode |= SST_TCA_SUB_CLOCAL | SST_TCA_ADD_ALOCAL;
    _SST->state.tmuMask |= tmuMask;
    break;

  case GR_COMBINE_FUNCTION_SCALE_MINUS_LOCAL_ADD_LOCAL:
    texmode |= SST_TCA_ZERO_OTHER | SST_TCA_SUB_CLOCAL | SST_TCA_ADD_CLOCAL;
    _SST->state.tmuMask |= tmuMask;
    break;

  case GR_COMBINE_FUNCTION_SCALE_MINUS_LOCAL_ADD_LOCAL_ALPHA:
    texmode |= SST_TCA_ZERO_OTHER | SST_TCA_SUB_CLOCAL | SST_TCA_ADD_ALOCAL;
    _SST->state.tmuMask |= tmuMask;
    break;
  }

  /* update register */
  grSstWriteConfigTMU( tmu, SSTR_TEXTUREMODE, texmode);
  _SST->state.tmu_config[SST_TMU_CHIP_TO_INDEX(tmu)].textureMode = texmode;

  /* update paramIndex */
  _grUpdateParamIndex();

} /* grTexCombine */

/*
** grTexCombineFunction
**                              
** Sets the texture combine function.  For a dual TMU system this function
** will configure the SSTR_TEXTUREMODE registers as appropriate.  For a
** single TMU system this function will configure SSTR_TEXTUREMODE if
** possible, or defer operations until grDrawTriangle() is called.
*/
FX_EXPORT void FX_CSTYLE
grTexCombineFunction(GrChipID_t tmu, GrTextureCombineFnc_t tc) {
  switch ( tc )  {
  case GR_TEXTURECOMBINE_ZERO:
    grTexCombine( tmu, GR_COMBINE_FUNCTION_ZERO, GR_COMBINE_FACTOR_NONE,
                  GR_COMBINE_FUNCTION_ZERO, GR_COMBINE_FACTOR_NONE, FXFALSE, FXFALSE );
    break;

  case GR_TEXTURECOMBINE_DECAL:
    grTexCombine( tmu, GR_COMBINE_FUNCTION_LOCAL, GR_COMBINE_FACTOR_NONE,
                  GR_COMBINE_FUNCTION_LOCAL, GR_COMBINE_FACTOR_NONE, FXFALSE, FXFALSE );
    break;

  case GR_TEXTURECOMBINE_ONE:
    grTexCombine( tmu, GR_COMBINE_FUNCTION_ZERO, GR_COMBINE_FACTOR_NONE,
                  GR_COMBINE_FUNCTION_ZERO, GR_COMBINE_FACTOR_NONE, FXTRUE, FXTRUE );
    break;

  case GR_TEXTURECOMBINE_ADD:
    grTexCombine( tmu, GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL, GR_COMBINE_FACTOR_ONE,
                  GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL, GR_COMBINE_FACTOR_ONE, FXFALSE, FXFALSE );
    break;

  case GR_TEXTURECOMBINE_MULTIPLY:
    grTexCombine( tmu, GR_COMBINE_FUNCTION_SCALE_OTHER, GR_COMBINE_FACTOR_LOCAL,
                  GR_COMBINE_FUNCTION_SCALE_OTHER, GR_COMBINE_FACTOR_LOCAL, FXFALSE, FXFALSE );
    break;

  case GR_TEXTURECOMBINE_DETAIL:
    grTexCombine( tmu, GR_COMBINE_FUNCTION_BLEND, GR_COMBINE_FACTOR_ONE_MINUS_DETAIL_FACTOR,
                  GR_COMBINE_FUNCTION_BLEND, GR_COMBINE_FACTOR_ONE_MINUS_DETAIL_FACTOR, FXFALSE, FXFALSE );
    break;

  case GR_TEXTURECOMBINE_DETAIL_OTHER:
    grTexCombine( tmu, GR_COMBINE_FUNCTION_BLEND, GR_COMBINE_FACTOR_DETAIL_FACTOR,
                  GR_COMBINE_FUNCTION_BLEND, GR_COMBINE_FACTOR_DETAIL_FACTOR, FXFALSE, FXFALSE );
    break;

  case GR_TEXTURECOMBINE_TRILINEAR_ODD:
    grTexCombine( tmu, GR_COMBINE_FUNCTION_BLEND, GR_COMBINE_FACTOR_ONE_MINUS_LOD_FRACTION,
                  GR_COMBINE_FUNCTION_BLEND, GR_COMBINE_FACTOR_ONE_MINUS_LOD_FRACTION, FXFALSE, FXFALSE );
    break;

  case GR_TEXTURECOMBINE_TRILINEAR_EVEN:
    grTexCombine( tmu, GR_COMBINE_FUNCTION_BLEND, GR_COMBINE_FACTOR_LOD_FRACTION,
                  GR_COMBINE_FUNCTION_BLEND, GR_COMBINE_FACTOR_LOD_FRACTION, FXFALSE, FXFALSE );
    break;

  case GR_TEXTURECOMBINE_SUBTRACT:
    grTexCombine( tmu, GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL, GR_COMBINE_FACTOR_ONE,
                  GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL, GR_COMBINE_FACTOR_ONE, FXFALSE, FXFALSE );
    break;

  case GR_TEXTURECOMBINE_OTHER:
    grTexCombine( tmu, GR_COMBINE_FUNCTION_SCALE_OTHER, GR_COMBINE_FACTOR_ONE,
                  GR_COMBINE_FUNCTION_SCALE_OTHER, GR_COMBINE_FACTOR_ONE, FXFALSE, FXFALSE );
    break;

  default:
    GrErrorCallback( "grTexCombineFunction:  Unsupported function", FXTRUE );
    break;
  }
} /* grTexCombineFunction */

/*
** _grTexDetailControl
*/
void 
_grTexDetailControl( GrChipID_t tmu, FxU32 detail )
{
#ifdef GLIDE_DEBUG
  if ( tmu < GR_TMU0 || tmu >= SST_TMU_INDEX_TO_CHIP(_SST->num_tmu) )
  {
    GrErrorCallback( "_grTexDetailControl: invalid tmu specified", FXTRUE );
  }
#endif

  grSstWriteConfigTMU( tmu, SSTR_TDETAIL, detail );
  _SST->state.tmu_config[SST_TMU_CHIP_TO_INDEX(tmu)].tDetail = detail;
} /* _grTexDetailControl */

/*
** grTexDetailControl
*/
FX_EXPORT void
FX_CSTYLE grTexDetailControl( GrChipID_t tmu, int lod_bias, FxU8 detail_scale, float detail_max )
{
  FxU32 tDetail = 0;
  FxU32 dmax    = ( FxU32 ) ( detail_max * 255.0F );
  FxU32 dscale  = detail_scale;
  
#ifdef GLIDE_DEBUG
  if ( tmu < GR_TMU0 || tmu >= SST_TMU_INDEX_TO_CHIP(_SST->num_tmu) )
  {
    GrErrorCallback( "grTexDetailControl: invalid tmu specified", FXTRUE );
  }
  if ( lod_bias < -32 || lod_bias > 31 ) {
    GrErrorCallback( "grTexDetailControl:  lod_bias out of range\n", FXTRUE );
  }
  if ( detail_scale > 7 ) {
    GrErrorCallback( "grTexDetailControl:  detail_scale out of range\n", FXTRUE );
  }
  if ( detail_max < 0.0 || detail_max > 1.0 ) {
    GrErrorCallback( "grTexDetailControl:  detail_max out of range\n", FXTRUE );
  }
#endif

  tDetail |= ( ( lod_bias << SST_DETAIL_BIAS_SHIFT ) & SST_DETAIL_BIAS );
  tDetail |= ( ( dmax << SST_DETAIL_MAX_SHIFT ) & SST_DETAIL_MAX );
  tDetail |= ( ( dscale << SST_DETAIL_SCALE_SHIFT ) & SST_DETAIL_SCALE );
  
  _grTexDetailControl( tmu, tDetail );
}/* grTexDetailControl */

/*
** _grTexDownloadNccTable
**
** Downloads an ncctable to the specified _physical_ TMU(s).  This
** function is called internally by Glide and should not be executed
** by an application.
*/
void
_grTexDownloadNccTable( GrChipID_t tmu, FxU32 which, const GuNccTable *table )
{
  int i;
  FxU32 tmuNCCTable;
  
  if ( table == 0 )
    return;
  
  if (which == 0)
    tmuNCCTable = SSTR_NCCTABLE0;
  else
    tmuNCCTable = SSTR_NCCTABLE1;  
          
#ifdef GLIDE_DEBUG
  if (tmu < GR_TMU0 || tmu >= SST_TMU_INDEX_TO_CHIP(_SST->num_tmu))
    GrErrorCallback( "_grTexDownloadNccTable:  bogus TMU specified", FXTRUE );
#endif    

  if (_SST->tmu_state[SST_TMU_CHIP_TO_INDEX(tmu)].ncc_table[which] != table ) {
    for ( i = 0; i < 12; i++ )
      grSstWriteConfigTMU( tmu, tmuNCCTable + i, table->packed_data[i] );
    _SST->tmu_state[SST_TMU_CHIP_TO_INDEX(tmu)].ncc_table[which] = table;
  }
} /* _grTexDownloadNccTable */

/*
** grTexGetCurrentMipMap
*/
FX_EXPORT GrMipMapId_t 
FX_CSTYLE grTexGetCurrentMipMap( GrChipID_t tmu )
{
#ifdef GLIDE_DEBUG
  if ( tmu < GR_TMU0 || tmu >= SST_TMU_INDEX_TO_CHIP(_SST->num_tmu) )
  {
     GrErrorCallback( "grTexGetCurrentMipMap:  invalid tmu", FXTRUE );
  }
#endif
  return _SST->state.current_mm[SST_TMU_CHIP_TO_INDEX(tmu)];
}

/*
** grTexFilterMode
*/
FX_EXPORT void
FX_CSTYLE grTexFilterMode( GrChipID_t tmu, GrTextureFilterMode_t minfilter, GrTextureFilterMode_t magfilter )
{
  FxU32 tmode;
  FxU32 filterMode = (
    (minfilter == GR_TEXTUREFILTER_BILINEAR ? SST_TMINFILTER : 0) |
    (magfilter == GR_TEXTUREFILTER_BILINEAR ? SST_TMAGFILTER : 0)
  );

#ifdef GLIDE_DEBUG
  if (tmu < GR_TMU0 || tmu >= SST_TMU_INDEX_TO_CHIP(_SST->num_tmu))
    GrErrorCallback("grTexFilterMode:  Bogus TMU number!\n", FXTRUE);
#endif

  tmode = _SST->state.tmu_config[SST_TMU_CHIP_TO_INDEX(tmu)].textureMode;
  tmode &= ~( SST_TMINFILTER | SST_TMAGFILTER );
  tmode |= filterMode;
  grSstWriteConfigTMU( tmu, SSTR_TEXTUREMODE, tmode );
  _SST->state.tmu_config[SST_TMU_CHIP_TO_INDEX(tmu)].textureMode = tmode;
}/* grTexFilterMode */

/*
** _grTexForceLod
**
** Forces tLOD to a specific LOD level.  This is useful only for
** debugging purposes.
*/
void 
_grTexForceLod( GrChipID_t tmu, int value )
{
  FxU32 tlod;

#ifdef GLIDE_DEBUG
  if (tmu < GR_TMU0 || tmu >= SST_TMU_INDEX_TO_CHIP(_SST->num_tmu))
    GrErrorCallback("_grTexForceLod: bogus tmu specified!\n", FXTRUE);
#endif

  tlod = _SST->state.tmu_config[0].tLOD;
  tlod &= ~(
            ( ( 0x3F << 2 ) << SST_LODMIN_SHIFT ) |
            ( ( 0x3F << 2 ) << SST_LODMAX_SHIFT )
           );
  tlod |= (
           ( ( value << 2 ) << SST_LODMIN_SHIFT ) |
           ( ( value << 2 ) << SST_LODMAX_SHIFT )
          );
    grSstWriteConfigTMU( tmu, SSTR_TLOD, tlod );
    _SST->state.tmu_config[SST_TMU_CHIP_TO_INDEX(tmu)].tLOD = tlod;
}/* _grTexForceLod */

/*
** guTexGetMipMapInfo
*/
FX_EXPORT GrMipMapInfo * FX_CSTYLE
guTexGetMipMapInfo( GrMipMapId_t mmid ) {
  return &( _SST->mm_table.data[mmid] );
} /* guTexGetMipMapInfo */

/*
** _grTexLodBiasValue
*/
void 
_grTexLodBiasValue( GrChipID_t tmu, FxU32 lod_bias )
{
   FxU32 tlod;

#ifdef GLIDE_DEBUG
   if (tmu < GR_TMU0 || tmu >= SST_TMU_INDEX_TO_CHIP(_SST->num_tmu))
     GrErrorCallback("_grTexLodBiasValue:  bogus tmu specified!\n", FXTRUE);
#endif

   tlod = _SST->state.tmu_config[SST_TMU_CHIP_TO_INDEX(tmu)].tLOD;
   tlod &= ~( SST_LODBIAS );
   tlod |= lod_bias << SST_LODBIAS_SHIFT;
   grSstWriteConfigTMU( tmu, SSTR_TLOD, tlod );
   _SST->state.tmu_config[SST_TMU_CHIP_TO_INDEX(tmu)].tLOD = tlod;
} /* _grTexLodBiasValue */

/*
** grTexLodBiasValue
*/
FX_EXPORT void 
FX_CSTYLE grTexLodBiasValue( GrChipID_t tmu, float value )
{
   FxU32 lod_bias;

#ifdef GLIDE_DEBUG
   if (tmu < GR_TMU0 || tmu >= SST_TMU_INDEX_TO_CHIP(_SST->num_tmu))
     GrErrorCallback("grTexLodBiasValue:  bogus tmu specified!\n", FXTRUE);
#endif

   lod_bias   = _grTexFloatLODToFixedLOD( value );
   _grTexLodBiasValue( tmu, lod_bias );
}/* grTexLodBiasValue */

/*
** guTexMemReset
**
** Clears out texture buffer memory.
*/
FX_EXPORT void FX_CSTYLE
guTexMemReset( void ) {
  int i;
  
  memset( _SST->mm_table.data, 0, sizeof( _SST->mm_table.data ) );
  _SST->mm_table.free_mmid = 0;
  
  for ( i = 0; i < _SST->num_tmu; i++ ) {
    _SST->state.current_mm[i] = (GrMipMapId_t) GR_NULL_MIPMAP_HANDLE;
    _SST->tmu_state[i].freemem_base = 0;
    _SST->tmu_state[i].ncc_mmids[0] = GR_NULL_MIPMAP_HANDLE;
    _SST->tmu_state[i].ncc_mmids[1] = GR_NULL_MIPMAP_HANDLE;    
   }
} /* guTexMemReset */

#if 0
/*
** grTexMipMapMode
**
** Configures the current mip mapping mode.
** This mode is in effect until the next call to grTexMipMapMode
** or guTexSource.
*/
FX_EXPORT void FX_CSTYLE
grTexMipMapMode( GrChipID_t tmu, GrMipMapMode_t mode ) {
  GrMipMapId_t    mmid;
  GrMipMapInfo   *mminfo;
  FxU32           tlod, tmode;

#ifdef GLIDE_DEBUG
  if (tmu < GR_TMU0 || tmu >= SST_TMU_INDEX_TO_CHIP(_SST->num_tmu))
    GrErrorCallback("grTexMipMapMode: bogus TMU specified!\n", FXTRUE);
#endif

  tlod  = _SST->state.tmu_config[SST_TMU_CHIP_TO_INDEX(tmu)].tLOD;
  tmode = _SST->state.tmu_config[SST_TMU_CHIP_TO_INDEX(tmu)].textureMode;

  tlod  &= ~( SST_LODMIN | SST_LODMAX );
  tmode &= ~SST_TLODDITHER;

  /*
  ** Get handle of the current mmid
  */
  mmid = grTexGetCurrentMipMap( tmu );
  mminfo = guTexGetMipMapInfo( mmid );

  /*
  ** Reset bits appropriately
  */
  switch ( mode ) {
  case GR_MIPMAP_DISABLE:
    /*
     ** Set LODMIN and LODMAX of the tLOD register to the highest
     ** LOD level of the current mmid
     */
    tlod |= ( ( mminfo->lod_max << 2 ) << SST_LODMAX_SHIFT ) | ( mminfo->lod_max << 2 );
    break;
  case GR_MIPMAP_NEAREST:
    /*
     ** Set LODMIN and LODMAX of the tLOD register to the min and max
     ** LOD levels of the current mmid.
     */
    tlod |= ( ( mminfo->lod_min << 2 ) << SST_LODMAX_SHIFT ) | ( mminfo->lod_max << 2 );
    break;
  case GR_MIPMAP_NEAREST_DITHER:
    tlod |= ( ( mminfo->lod_min << 2 ) << SST_LODMAX_SHIFT ) | ( mminfo->lod_max << 2 );
    tmode |= SST_TLODDITHER;
    break;
  default:
#ifdef GLIDE_DEBUG
    GrErrorCallback( "grTexMipMapMode:  invalid mode passed", FXFALSE );
#endif
    break;
  }

  grSstWriteConfigTMU( tmu, SSTR_TLOD, tlod );
  grSstWriteConfigTMU( tmu, SSTR_TEXTUREMODE, tmode );
  _SST->state.tmu_config[SST_TMU_CHIP_TO_INDEX(tmu)].tLOD = tlod;
  _SST->state.tmu_config[SST_TMU_CHIP_TO_INDEX(tmu)].textureMode = tmode;
}
#else
/*-------------------------------------------------------------------
  Function: grTexMipMapMode
  Date: 6/2
  Implementor(s): GaryMcT, Jdt
  Library: glide
  Description:
    Sets the mip map mode for the specified TMU
    "Ex" because glide's grTexMipMapMode is inadequate for 
         low level texture memory management
  Arguments:
    tmu       - tmu to update
    mmMode   - mipmap mode 
                One of:
                  GR_MIPMAP_DISABLE
                  GR_MIPMAP_NEAREST
                  GR_MIPMAP_NEAREST_DITHER
    lodBlend - enable lodBlending
                FXTRUE  - enabled
                FXFALSE - disabled
  Return:
    none
  -------------------------------------------------------------------*/
FX_EXPORT void FX_CSTYLE
grTexMipMapMode( GrChipID_t     tmu, 
                 GrMipMapMode_t mmMode, 
                 FxBool         lodBlend ) {
    FxU32  tlod;
    FxU32  tmode;
    
#ifdef GLIDE_DEBUG
    if (tmu < GR_TMU0 || tmu >= SST_TMU_INDEX_TO_CHIP(_SST->num_tmu))
        GrErrorCallback( "grTexMipMapMode: bogus TMU specified!\n", 
                         FXTRUE);
#endif

    /*--------------------------------------------------------------
      Get Current tlod and tmode register values
      --------------------------------------------------------------*/
    tlod  = _SST->state.tmu_config[SST_TMU_CHIP_TO_INDEX(tmu)].tLOD;
    tmode = _SST->state.tmu_config[SST_TMU_CHIP_TO_INDEX(tmu)].textureMode;

    /*--------------------------------------------------------------
      Clear LODMIN, LODMAX and LODDITHER
      --------------------------------------------------------------*/
    tlod  &= ~( SST_LODMIN|SST_LODMAX);
    tmode &= ~(SST_TLODDITHER|SST_TRILINEAR);

    /*--------------------------------------------------------------
      Encode Mipmap Mode Bits
      --------------------------------------------------------------*/
    switch ( mmMode ) {
        case GR_MIPMAP_DISABLE:
            /*----------------------------------------------------------
              To disable mipmapping set the min and max lods to the same
              value
              ----------------------------------------------------------*/
            tlod |= ( ( _SST->state.tmu_config[SST_TMU_CHIP_TO_INDEX(tmu)].largeLod << 2 ) << SST_LODMAX_SHIFT ) | 
                    ( ( _SST->state.tmu_config[SST_TMU_CHIP_TO_INDEX(tmu)].largeLod << 2 ) << SST_LODMIN_SHIFT );
            break;
        case GR_MIPMAP_NEAREST_DITHER:
            tmode |= SST_TLODDITHER;
            /* intentional fall-through to set lodmin and lodmax values */
        case GR_MIPMAP_NEAREST:
            /*----------------------------------------------------------
              Set LODMIN and LODMAX to in the tLod register to the 
              actual min and max LODs of the current texture.
              ----------------------------------------------------------*/
            tlod |= ( ( _SST->state.tmu_config[SST_TMU_CHIP_TO_INDEX(tmu)].largeLod << 2 ) << SST_LODMIN_SHIFT ) | 
                    ( ( _SST->state.tmu_config[SST_TMU_CHIP_TO_INDEX(tmu)].smallLod << 2 ) << SST_LODMAX_SHIFT );
            break;
        default:
            GrErrorCallback( "grTexMipMapMode:  invalid mode passed", 
                             FXFALSE );
            break;
    }
    _SST->state.tmu_config[SST_TMU_CHIP_TO_INDEX(tmu)].mmMode = mmMode;

    /*--------------------------------------------------------------
      Fix trilinear and evenOdd bits
      --------------------------------------------------------------*/
    if ( lodBlend ) {
        tmode |= SST_TRILINEAR;
    }
    
    /*--------------------------------------------------------------
      Write State To Hardware and Update Glide Shadow State
      --------------------------------------------------------------*/
    grSstWriteConfigTMU( tmu, SSTR_TLOD, tlod );
    grSstWriteConfigTMU( tmu, SSTR_TEXTUREMODE, tmode );
    _SST->state.tmu_config[SST_TMU_CHIP_TO_INDEX(tmu)].tLOD        = tlod;
    _SST->state.tmu_config[SST_TMU_CHIP_TO_INDEX(tmu)].textureMode = tmode;
    return;
}

#endif

/*
** guTexSource
*/
FX_EXPORT void FX_CSTYLE
guTexSource( GrMipMapId_t mmid ) {
  FxU32               texmode, tlod;
  FxU32               oldtexmode;
  FxU32               baseaddress;
  FxU32               tmu;
  const GrMipMapInfo *mminfo;

  /*
  ** Make sure that mmid is not NULL
  */
  if ( mmid == GR_NULL_MIPMAP_HANDLE )
  {
    return;
  }

  /*
  ** get a pointer to the relevant GrMipMapInfo struct
  */
  mminfo = &_SST->mm_table.data[mmid];

  tmu = mminfo->tmu;

#ifdef GLIDE_DEBUG
  if ( mmid == _SST->state.current_mm[SST_TMU_CHIP_TO_INDEX(tmu)] ) {    
    GrErrorCallback( "gtTexSource:  setting same state twice", FXFALSE);
    return;
  }
#endif

  _SST->state.current_mm[SST_TMU_CHIP_TO_INDEX(tmu)] = mmid;

  /*
  ** Set up new glide state for this mmid
  */
  _SST->state.tmu_config[SST_TMU_CHIP_TO_INDEX(tmu)].mmMode = 
    mminfo->mipmap_mode;
  _SST->state.tmu_config[SST_TMU_CHIP_TO_INDEX(tmu)].smallLod = 
    mminfo->lod_min;
  _SST->state.tmu_config[SST_TMU_CHIP_TO_INDEX(tmu)].largeLod = 
    mminfo->lod_max;
  _SST->state.tmu_config[SST_TMU_CHIP_TO_INDEX(tmu)].evenOdd = 
    mminfo->odd_even_mask;
  _SST->state.tmu_config[SST_TMU_CHIP_TO_INDEX(tmu)].nccTable = 
    0;


  /*
  ** Set up base address, texmode, and tlod registers
  */
  baseaddress = mminfo->tmu_base_address;
  texmode     = mminfo->tTextureMode;
  tlod        = mminfo->tLOD;

  oldtexmode = _SST->state.tmu_config[SST_TMU_CHIP_TO_INDEX(tmu)].textureMode;
  oldtexmode &= ~( SST_TFORMAT | SST_TCLAMPT | SST_TCLAMPS | SST_TNCCSELECT | SST_TLODDITHER | SST_TCLAMPW | SST_TMAGFILTER | SST_TMINFILTER | SST_TRILINEAR );
  texmode |= oldtexmode;

 /* 
  **  Download the NCC table, if needed.  
  */
  if (
      (mminfo->format==GR_TEXFMT_YIQ_422) ||
      (mminfo->format==GR_TEXFMT_AYIQ_8422)
      ) {
    int
      table;                    /* ncc table we'll use */
    /* See if it's already down there */
    if (_SST->tmu_state[tmu].ncc_mmids[0] == mmid) {
      /* Table 0 has what we need, so make it current */
      table = 0;
    } else if (_SST->tmu_state[tmu].ncc_mmids[1] == mmid) {
      /* Table 1 has what we need, so make it current */
      table = 1;
    } else {
      /*
      **  it's not down there, so we need to pick the table and
      **  download it
      */
      /* Which table should we use? */
      table = _SST->tmu_state[SST_TMU_CHIP_TO_INDEX(tmu)].next_ncc_table;
      /* Download NCC table */
      _grTexDownloadNccTable( tmu, table, &mminfo->ncc_table );
      /* Set the mmid so we known it's down there */
      _SST->tmu_state[SST_TMU_CHIP_TO_INDEX(tmu)].ncc_mmids[table] = mmid;
      /* Set the state to know which table was the LRA */
      _SST->tmu_state[SST_TMU_CHIP_TO_INDEX(tmu)].next_ncc_table =
        (table == 0 ? 1 : 0);
    } /* we had to download it */
    /*
    **  Setting the TNCCSelect bit to 0 selects table 0, setting it to 1
    **  selects table 1
    */
    if (table == 0)
      texmode &= ~(SST_TNCCSELECT);
    else
      texmode |= SST_TNCCSELECT;
  } /* if it's an NCC texture */

  /* update shadows */
  _SST->state.tmu_config[SST_TMU_CHIP_TO_INDEX(tmu)].texBaseAddr = baseaddress;
  _SST->state.tmu_config[SST_TMU_CHIP_TO_INDEX(tmu)].textureMode = texmode;
  _SST->state.tmu_config[SST_TMU_CHIP_TO_INDEX(tmu)].tLOD = tlod;
 
  /* Write relevant registers out to hardware */
  grSstWriteConfigTMU( tmu, SSTR_TEXBASEADDR, baseaddress );
  grSstWriteConfigTMU( tmu, SSTR_TEXTUREMODE, texmode );
  grSstWriteConfigTMU( tmu, SSTR_TLOD, tlod );

} /* guTexSource */

/*
** The following code performs texture memory pointer arithmetic.  It
** is convoluted and complicated, so I wouldn't recommend modifying it
** unless you REALLY know what you're doing.
*/

static FxU32 GetPage25( GrLOD_t lod, GrAspectRatio_t aspect )
{
  FxU32 page25[GR_LOD_1+1][GR_ASPECT_1x8+1] =
  {
  { 1 << 14, 1 << 13, 1 << 12, 1 << 11 },
  { 1 << 12, 1 << 11, 1 << 10, 1 << 9 },
  { 1 << 10, 1 << 9,  1 << 8,  1 << 7 },
  { 1 << 8,  1 << 7,  1 << 6,  1 << 5 },
  { 1 << 6,  1 << 5,  1 << 4,  1 << 3 },
  { 1 << 4,  1 << 3,  1 << 2,  1 << 2 },
  { 1 << 2,  1 << 1,  1 << 1,  1 << 1 },
  { 1 << 0,  1 << 0,  1 << 0,  1 << 0 },
  { 1 << 0,  1 << 0,  1 << 0,  1 << 0 }
  };

  int aspect_index;

  switch ( aspect )
  {
  case GR_ASPECT_1x1:
    aspect_index = 0;
    break;
  case GR_ASPECT_2x1:
  case GR_ASPECT_1x2:
    aspect_index = 1;
    break;
  case GR_ASPECT_4x1:
  case GR_ASPECT_1x4:
    aspect_index = 2;
    break;
  case GR_ASPECT_8x1:
  case GR_ASPECT_1x8:
    aspect_index = 3;
    break;
  }

  return page25[lod][aspect_index];
}

/*
** _grTexCalcBaseAddress
*/
FxU32 _grTexCalcBaseAddress( FxU32 start, GrLOD_t lod_max, GrAspectRatio_t aspect, GrTextureFormat_t format, FxU32 odd_even_mask )
{
  FxU32 multiplier;
  FxU32 sum_of_lod_sizes = 0;
  GrLOD_t lod;

  if ( format < GR_TEXFMT_16BIT )
    multiplier = 4;
  else
    multiplier = 8;

  for ( lod = GR_LOD_256; lod < lod_max; lod++ )
  {
    if ( lod & 1 )
    {
       if ( odd_even_mask & GR_MIPMAPLEVELMASK_ODD )
          sum_of_lod_sizes += GetPage25( lod, aspect );
    }
    else
    {
       if ( odd_even_mask & GR_MIPMAPLEVELMASK_EVEN )
          sum_of_lod_sizes += GetPage25( lod, aspect );
    }
  }

  sum_of_lod_sizes *= multiplier;
  sum_of_lod_sizes &= ( ~7 );

  return ( start - sum_of_lod_sizes );
}

/*
** _grTexCalcNextAddress
*/
FxU32 _grTexCalcNextAddress( FxU32 base, GrLOD_t lod_min, GrAspectRatio_t aspect, GrTextureFormat_t format, FxU32 odd_even_mask )
{
  FxU32 multiplier;
  FxU32 sum_of_lod_sizes = 0;
  GrLOD_t lod;

  if ( format < GR_TEXFMT_16BIT )
    multiplier = 4;
  else
    multiplier = 8;

  for ( lod = GR_LOD_256; lod <= lod_min; lod++ )
  {
     if ( lod & 1 )
     {
        if ( odd_even_mask & GR_MIPMAPLEVELMASK_ODD )
           sum_of_lod_sizes += GetPage25( lod, aspect );
     }
     else
     {
        if ( odd_even_mask & GR_MIPMAPLEVELMASK_EVEN )
           sum_of_lod_sizes += GetPage25( lod, aspect );
     }
  }
  sum_of_lod_sizes *= multiplier;
  sum_of_lod_sizes += 4;
  sum_of_lod_sizes &= ( ~7 );

  return ( base + sum_of_lod_sizes );
}

/*
** grTexCalcMemRequired
*/
FX_EXPORT FxU32
FX_CSTYLE grTexCalcMemRequired( GrLOD_t lodmin, GrLOD_t lodmax, GrAspectRatio_t aspect, GrTextureFormat_t format )
{
  FxU32 memrequired;
  FxU32 base, next;

  base = _grTexCalcBaseAddress( 0, lodmax, aspect, format, GR_MIPMAPLEVELMASK_BOTH );
  next = _grTexCalcNextAddress( base, lodmin, aspect, format, GR_MIPMAPLEVELMASK_BOTH );

  memrequired = next;

  memrequired += 4;
  memrequired &= ~7;

  return memrequired;
}

/*
**  guTexChangeAttributes 
*/
FX_EXPORT FxBool FX_CSTYLE
guTexChangeAttributes( GrMipMapId_t mmid,
                      int width, int height,
                      GrTextureFormat_t fmt,
                      GrMipMapMode_t mm_mode,
                      GrLOD_t smallest_lod, GrLOD_t largest_lod,
                      GrAspectRatio_t aspect,
                      GrTextureClampMode_t s_clamp_mode,
                      GrTextureClampMode_t t_clamp_mode,
                      GrTextureFilterMode_t minFilterMode,
                      GrTextureFilterMode_t magFilterMode
                      ) {
  GrMipMapInfo *mminfo;

  /*
  ** Make sure that mmid is not NULL
  */
  if ( mmid == GR_NULL_MIPMAP_HANDLE ) {
    return FXFALSE;
  }

  mminfo = &_SST->mm_table.data[mmid];

  /*
  ** Fill in the mm_table data for this mip map
  */
  if ( fmt != -1 )
     mminfo->format         = fmt;

  if ( mm_mode != -1 )
     mminfo->mipmap_mode    = mm_mode;

  if ( smallest_lod != -1 )
     mminfo->lod_min        = smallest_lod;;
  if ( largest_lod != -1 )
     mminfo->lod_max        = largest_lod;
  if ( minFilterMode != -1 )
     mminfo->minfilter_mode = minFilterMode;
  if ( magFilterMode != -1 )
     mminfo->magfilter_mode = magFilterMode;
  if ( s_clamp_mode != -1 )
     mminfo->s_clamp_mode   = s_clamp_mode;
  if ( t_clamp_mode != -1 )
     mminfo->t_clamp_mode   = t_clamp_mode;
  if ( aspect != -1 )
     mminfo->aspect_ratio   = aspect;
  if ( width != -1 )
     mminfo->width          = width;
  if ( height != -1 )
     mminfo->height         = height;

  _guTexRebuildRegisterShadows( mmid );

  return FXTRUE;
} /* grTexResetAttributes */

static void
_guTexRebuildRegisterShadows( GrMipMapId_t mmid ) {
   GrMipMapInfo *mminfo = &_SST->mm_table.data[mmid];
   int texturemode      = 0;
   int tlod             = 0;
   int s_is_wider       = 0;
   int int_lod_bias;
   int aspect_mask;
   FxU32 
     filterMode,        /* filter mode bits of texturemode */
     clampMode;         /* clamp mode bits of texturemode */

   /*
   ** build up tTextureMode
   */
   /* build filterMode */
   filterMode = (
     (mminfo->minfilter_mode == GR_TEXTUREFILTER_BILINEAR ? SST_TMINFILTER : 0) |
     (mminfo->magfilter_mode == GR_TEXTUREFILTER_BILINEAR ? SST_TMAGFILTER : 0)
   );
   clampMode = (
     (mminfo->s_clamp_mode == GR_TEXTURECLAMP_CLAMP ? SST_TCLAMPS : 0) |
     (mminfo->t_clamp_mode == GR_TEXTURECLAMP_CLAMP ? SST_TCLAMPT : 0)
   );
   
   texturemode |= ( mminfo->format << SST_TFORMAT_SHIFT );
   texturemode |= SST_TCLAMPW;
   texturemode |= SST_TPERSP_ST;
   texturemode |= filterMode;
   texturemode |= clampMode;

   if ( mminfo->mipmap_mode == GR_MIPMAP_NEAREST_DITHER )
      texturemode |= SST_TLODDITHER;

   if ( mminfo->trilinear )
      texturemode |= SST_TRILINEAR;

   /*
   ** build up tLOD
   */
   int_lod_bias = mminfo->lod_bias;
   if ( mminfo->aspect_ratio < GR_ASPECT_1x1 )
     s_is_wider = SST_LOD_S_IS_WIDER;
   else
     s_is_wider = 0;

   aspect_mask = aspect_bit_table[mminfo->aspect_ratio];
   if ( mminfo->mipmap_mode == GR_MIPMAP_DISABLE )
      tlod = ( ( s_is_wider ) | ( aspect_mask << SST_LOD_ASPECT_SHIFT ) | ( int_lod_bias << SST_LODBIAS_SHIFT ) | ( ( mminfo->lod_max << 2 ) << SST_LODMAX_SHIFT ) | ( mminfo->lod_max << 2 ) );
   else
      tlod = ( ( s_is_wider ) | ( aspect_mask << SST_LOD_ASPECT_SHIFT ) | ( int_lod_bias << SST_LODBIAS_SHIFT ) | ( ( mminfo->lod_min << 2 ) << SST_LODMAX_SHIFT ) | ( mminfo->lod_max << 2 ) );

   /*
   ** assign them
   */
   mminfo->tTextureMode = texturemode;
   mminfo->tLOD         = tlod;
}/* grTExRebuildRegisterShadows */


/*-------------------------------------------------------------------
  Function: grTexNCCTable
  Date: 6/3
  Implementor(s): jdt
  Library: glide
  Description:
    select one of the two NCC tables
  Arguments:
    tmu - which tmu
    table - which table to select
        One of:
            GR_TEXTABLE_NCC0
            GR_TEXTABLE_NCC1
            GR_TEXTABLE_PALETTE
  Return:
    none
  -------------------------------------------------------------------*/
FX_EXPORT void FX_CSTYLE
grTexNCCTable( GrChipID_t tmu, GrNCCTable_t table ) {
    FxU32 texMode;

#ifdef GLIDE_DEBUG
    if (tmu < GR_TMU0 || tmu >= SST_TMU_INDEX_TO_CHIP(_SST->num_tmu))
        GrErrorCallback( "grTexNCCTable: bogus TMU specified!\n", 
                         FXTRUE);
    if ( table > 0x1 )
        GrErrorCallback( "grTexNCCTable: invalid ncc table specified.\n",
                         FXTRUE );
#endif

    /*------------------------------------------------------------------
      Update local state
      ------------------------------------------------------------------*/
    _SST->state.tmu_config[SST_TMU_CHIP_TO_INDEX(tmu)].nccTable = table;

    /*------------------------------------------------------------------
      Grab shadow texMode, update TexMode, update shadow/real register
      ------------------------------------------------------------------*/
    texMode = _SST->state.tmu_config[SST_TMU_CHIP_TO_INDEX(tmu)].textureMode;
    texMode &= ~( SST_TNCCSELECT );
    if ( table )
        texMode |= SST_TNCCSELECT;
    else 
        texMode &= ~(SST_TNCCSELECT);
    _SST->state.tmu_config[SST_TMU_CHIP_TO_INDEX(tmu)].textureMode = texMode;
    grSstWriteConfigTMU( tmu, SSTR_TEXTUREMODE, texMode );
    return;
}


/*-------------------------------------------------------------------
  Function: grTexMinAddress
  Date: 6/2
  Implementor(s): GaryMcT, Jdt
  Library: glide
  Description:
    Returns address of start of texture ram for a TMU
  Arguments:
    tmu
  Return:
    integer texture base address, this pointer is not to be dereferenced
    by the application, it is on to be used by grTexDownload(),
    and grTExDownloadLevel()
  -------------------------------------------------------------------*/
FX_EXPORT FxU32 FX_CSTYLE
grTexMinAddress( GrChipID_t tmu ) {
    FXUNUSED( tmu );
    return 0;
}


/*-------------------------------------------------------------------
  Function: grTexMaxAddress
  Date: 6/2
  Implementor(s): GaryMcT, Jdt
  Library: glide
  Description:
    Returns address of maximum extent of texture ram for a given TMU
  Arguments:
    tmu
  Return:
    integer upper bound on texture memory, paired with a texMinAddress
  -------------------------------------------------------------------*/
FX_EXPORT FxU32 FX_CSTYLE
grTexMaxAddress( GrChipID_t tmu ) {
    return guTexMemQueryAvail( tmu );
}

/*-------------------------------------------------------------------
  Function: grTexTextureMemRequired
  Date: 6/2
  Implementor(s): GaryMcT, Jdt
  Library: glide
  Description:
    Returns the tmu memory required to store the specified mipmap    
    ( Gary and I don't like the name of this function, but are 
      a little backed into a corner because of the existence 
      of grTexMemRequired() which does not imply any distinction
      between texture memory and system ram )
  Arguments:
    evenOdd  - which set of mipmap levels are to be stored
                One of:
                  GR_MIPMAPLEVELMASK_EVEN 
                  GR_MIPMAPLEVELMASK_ODD
                  GR_MIPMAPLEVELMASK_BOTH
    info      - pointer to GrTexInfo structure defining dimensions
                of texture
  Return:
    offset to be added to current texture base address to calculate next 
    valid texture memory download location
  -------------------------------------------------------------------*/
FX_EXPORT FxU32 FX_CSTYLE
grTexTextureMemRequired( FxU32     evenOdd,
                         GrTexInfo *info   ) {
    FxU32 base, memrequired;

#ifdef GLIDE_DEBUG
    if ( evenOdd > 0x3 || evenOdd == 0 ) 
        GrErrorCallback( "grTexTextureMemRequired: invalid evenOdd mask.\n",
                         FXTRUE );
    /* This should do per member validation on info */
    if ( !info ) 
        GrErrorCallback( "grTexTextureMemRequired: info invalid.\n",
                         FXTRUE );
#endif



    /*--------------------------------------------------------
      First calculate base address for the texture if 
      the texture had a start address of 0
      --------------------------------------------------------*/
    base = _grTexCalcBaseAddress( 0, 
                                  info->largeLod, 
                                  info->aspectRatio,
                                  info->format,
                                  evenOdd );
    /*--------------------------------------------------------
      Next get memory required by finding the next starting 
      address offset from the base address just calculated
      --------------------------------------------------------*/
    memrequired = _grTexCalcNextAddress( base,
                                         info->smallLod, 
                                         info->aspectRatio,
                                         info->format,
                                         evenOdd );
    /*--------------------------------------------------------
      Snap address up to next 8 byte boundary
      ( I don't understand why it was done this way
        in grTexCalcMemRequired, but I'll just 
        leave it for now ) why not += 7?
      --------------------------------------------------------*/
    memrequired += 4;
    memrequired &= ~7;
    return memrequired;
}

/*-------------------------------------------------------------------
  Function: grTexSource
  Date: 6/2
  Implementor(s): GaryMcT, Jdt
  Library: glide
  Description:
    Sets up the current texture for texture mapping on the specified
    TMU.
  Arguments:
    tmu          - which tmu
    startAddress - texture start address
    evenOdd  - which set of mipmap levels have been downloaded for
                the selected texture
                One of:
                  GR_MIPMAPLEVELMASK_EVEN 
                  GR_MIPMAPLEVELMASK_ODD
                  GR_MIPMAPLEVELMASK_BOTH
    info         - pointer to GrTexInfo structure containing
                   texture dimensions
  Return:
    none
  -------------------------------------------------------------------*/
FX_EXPORT void 
FX_CSTYLE grTexSource( GrChipID_t tmu ,
                       FxU32      startAddress,
                       FxU32      evenOdd,
                       GrTexInfo  *info ) {
    FxU32 baseAddress, texMode, tLod;

#ifdef GLIDE_DEBUG
    if ( tmu > 0x2 )
        GrErrorCallback( "grTexSource: bad tmu number.\n", FXTRUE );
    if ( startAddress > grTexMaxAddress( tmu ) )
        GrErrorCallback( "grTexSource: invalid startAddress.\n",
                         FXTRUE );
    if ( startAddress + 
         grTexTextureMemRequired( evenOdd, info ) >
         grTexMaxAddress( tmu ) )
        GrErrorCallback( "grTexSource: insufficient texture ram at startAddress.",
                         FXTRUE );
    if ( !info ) 
        GrErrorCallback( "grTexSource: Invalid info pointer.\n", FXTRUE );
#endif

    /*-------------------------------------------------------------
      Update Texture Unit State
      -------------------------------------------------------------*/
    _SST->state.tmu_config[SST_TMU_CHIP_TO_INDEX(tmu)].smallLod = info->smallLod;
    _SST->state.tmu_config[SST_TMU_CHIP_TO_INDEX(tmu)].largeLod = info->largeLod;
    _SST->state.tmu_config[SST_TMU_CHIP_TO_INDEX(tmu)].evenOdd  = evenOdd;

    /*-------------------------------------------------------------
      Calculate Base Address
      -------------------------------------------------------------*/
    baseAddress = _grTexCalcBaseAddress( startAddress,
                                         info->largeLod, 
                                         info->aspectRatio,
                                         info->format,
                                         evenOdd ) >> 3;

    /*-------------------------------------------------------------
      Update Texture Mode
      -------------------------------------------------------------*/
    texMode = _SST->state.tmu_config[SST_TMU_CHIP_TO_INDEX(tmu)].textureMode;
    texMode &= ~( SST_TFORMAT );
    texMode |= info->format << SST_TFORMAT_SHIFT;
    
    /*-------------------------------------------------------------
      Compute TLOD
      -------------------------------------------------------------*/
    switch ( _SST->state.tmu_config[SST_TMU_CHIP_TO_INDEX(tmu)].mmMode ) {
        case GR_MIPMAP_DISABLE:
            /*----------------------------------------------------------
              To disable mipmapping set the min and max lods to the same
              value
              ----------------------------------------------------------*/
            tLod = ( ( info->largeLod << 2 ) << SST_LODMAX_SHIFT ) | 
                   ( ( info->largeLod << 2 ) << SST_LODMIN_SHIFT );
            break;
        default:
            /*----------------------------------------------------------
              Set LODMIN and LODMAX to in the tLod register to the 
              actual min and max LODs of the current texture.
              ----------------------------------------------------------*/
            tLod = ( ( info->largeLod << 2 ) << SST_LODMIN_SHIFT ) | 
                   ( ( info->smallLod << 2 ) << SST_LODMAX_SHIFT );
            break;
    }

    if ( evenOdd != GR_MIPMAPLEVELMASK_BOTH ) {
       tLod |= SST_LOD_TSPLIT;
       if ( evenOdd & GR_MIPMAPLEVELMASK_ODD )
           tLod |= SST_LOD_ODD;
    }

    if ( info->aspectRatio < GR_ASPECT_1x1 )
        tLod |= SST_LOD_S_IS_WIDER;
    tLod |= aspect_bit_table[info->aspectRatio] << SST_LOD_ASPECT_SHIFT;

    /* update shadows */
    _SST->state.tmu_config[SST_TMU_CHIP_TO_INDEX(tmu)].texBaseAddr = baseAddress;
    _SST->state.tmu_config[SST_TMU_CHIP_TO_INDEX(tmu)].textureMode = texMode;
    _SST->state.tmu_config[SST_TMU_CHIP_TO_INDEX(tmu)].tLOD        = tLod;
 
    /* Write relevant registers out to hardware */
    grSstWriteConfigTMU( tmu, SSTR_TEXBASEADDR, baseAddress );
    grSstWriteConfigTMU( tmu, SSTR_TEXTUREMODE, texMode );
    grSstWriteConfigTMU( tmu, SSTR_TLOD,        tLod );

    return;
}


/*-------------------------------------------------------------------
  Function: grTexMultibase
  Date: 4/4
  Implementor(s): jdt
  Library: Glide
  Description:
    Enable multiple base addresses for texturing.
  Arguments:
    tmu    - which tmu
    enable - flag which enables/disables multibase
  Return:
    none
  -------------------------------------------------------------------*/
FX_EXPORT void
FX_CSTYLE grTexMultibase( GrChipID_t tmu,
                          FxBool     enable ) {

    FXUNUSED( tmu );
    FXUNUSED( enable );
    GrErrorCallback( "grTexMultibase: Multi-base is not yet implemented in glide.\n", FXTRUE );
    return;
}


/*-------------------------------------------------------------------
  Function: grTexMultibaseAddress
  Date: 4/4
  Implementor(s): jdt
  Library: Glide
  Description:
    Set the base address for a particular set of mipmap
    lods when multibase is enabled.
  Arguments:
    tmu    - which tmu
    range  - range of lods that are based at this starting address
             One of:
             GR_TEXBASE_0
             GR_TEXBASE_1
             GR_TEXBASE_2
             GR_TEXBASE_3_TO_8
    startAddress - start address that data was downloaded to 
                    hardware with using grTexDownload/Level
    info   - pointer to GrTexInfo structure containing dimensions
             of complete texture map that has been split up in 
             texture RAM.
  Return:
    none
  -------------------------------------------------------------------*/
FX_EXPORT void
FX_CSTYLE grTexMultibaseAddress( GrChipID_t       tmu,
                                 GrTexBaseRange_t range,
                                 FxU32            startAddress,
                                 GrTexInfo        *info ) {
    FXUNUSED( tmu );
    FXUNUSED( range );
    FXUNUSED( startAddress );
    FXUNUSED( info );
    GrErrorCallback( "grTexMultibaseAddress: Multi-base is not yet implemented in glide.\n", FXTRUE );
    return;
}







