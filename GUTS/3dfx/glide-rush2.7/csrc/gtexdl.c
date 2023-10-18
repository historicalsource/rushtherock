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
** $Header: /engr/DEVEL/SST1/GLIDE/DOS/SRC/GTEXDL.C 30    6/13/96 8:26p Dow $
** $Log: /engr/DEVEL/SST1/GLIDE/DOS/SRC/GTEXDL.C $
 * 
 * 30    6/13/96 8:26p Dow
 * Fixed a warning message.
 * 
 * 29    6/10/96 12:44a Jdt
 * Created size_table
 * Removed memcpy from guTexDownloadMipMap
 * Added
 *   grTexDownloadTable
 *   grTexDownloadMipMap
 *   grTexDownloadMipMapLevel
 *   _grTexDownloadPalette
 * 
 * 28    6/03/96 1:17p Dow
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
 * 27    5/20/96 11:55a Dow
 * Fixed up NCC texture stuff.
 * 
 * 26    5/09/96 7:05a Paik
 * Glide DLL support
 * 
 * 25    3/25/96 7:58p Paik
 * Move fxglide.h out of glide.h
 * 
 * 24    3/20/96 8:07a Paik
 * Use single macro for TMUs
 * 
 * 23    3/01/96 1:57p Paik
 * State optimization
 * 
 * 24    3/01/96 4:16a Paik
 * 
 * 23    3/01/96 3:59a Paik
 * 
 * 22    2/02/96 11:57p Paik
 * Fix problem with texture repeatedly replicated at lower lods.
 * Texture load address needs to be added to tmu base address, not or-ed
 * together.
 * 
 * 23    1/31/96 2:45a Paik
 * Fix prob with repeatedly replicated textures
 * 
 * 21    1/14/96 9:30a Dow
 * Changed the way multiple TMU configs are handled
 * 
 * 20    1/13/96 2:43p Dow
 * Random fixes, and emacs helping stuff
 * 
 * 16    1/11/96 9:42p Dow
 * Fixed problem with Sam and I being backwards to each other WRT the
 * sense of mipmap level mask.
 * 
 * 15    1/11/96 2:26a Dow
 * Added GrChipID_t, changed functions taking tmus to use it.
 * Added tmuMask to state, use it to better control state setting when
 * moving between texture mapped and non texture mapped color combine
 * modes.
 * 
 * 14    1/10/96 11:12p Dow
 * sort of backed out tdownload changes
 * 
 * 13    1/09/96 9:39a Dow
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

static const FxU32 size_table[GR_LOD_1+1][4] = 
{
    /* GR_LOD_256 */
    { 
        65536, /* GR_ASPECT_1x1 */
        32768, /* GR_ASPECT_2x1 */
        16384, /* GR_ASPECT_4x1 */
         8192  /* GR_ASPECT_8x1 */
    }, 
    /* GR_LOD_128 */
    { 
        16384, /* GR_ASPECT_1x1 */
         8192, /* GR_ASPECT_2x1 */
         4096, /* GR_ASPECT_4x1 */
         2048  /* GR_ASPECT_8x1 */
    }, 
    /* GR_LOD_64 */
    { 
         4096, /* GR_ASPECT_1x1 */
         2048, /* GR_ASPECT_2x1 */
         1024, /* GR_ASPECT_4x1 */
          512  /* GR_ASPECT_8x1 */
    }, 
    /* GR_LOD_32 */
    { 
         1024, /* GR_ASPECT_1x1 */
          512, /* GR_ASPECT_2x1 */
          256, /* GR_ASPECT_4x1 */
          128  /* GR_ASPECT_8x1 */
    }, 
    /* GR_LOD_16 */
    { 
          256, /* GR_ASPECT_1x1 */
          128, /* GR_ASPECT_2x1 */
           64, /* GR_ASPECT_4x1 */
           32  /* GR_ASPECT_8x1 */
    }, 
    /* GR_LOD_8 */
    { 
           64, /* GR_ASPECT_1x1 */
           32, /* GR_ASPECT_2x1 */
           16, /* GR_ASPECT_4x1 */
            8  /* GR_ASPECT_8x1 */
    }, 
    /* GR_LOD_4 */
    { 
           16, /* GR_ASPECT_1x1 */
            8, /* GR_ASPECT_2x1 */
            4, /* GR_ASPECT_4x1 */
            4  /* GR_ASPECT_8x1 */
    }, 
    /* GR_LOD_2 */
    { 
            2, /* GR_ASPECT_1x1 */
            1, /* GR_ASPECT_2x1 */
            1, /* GR_ASPECT_4x1 */
            1  /* GR_ASPECT_8x1 */
    },
    /* GR_LOD_1 */
    { 
            1, /* GR_ASPECT_1x1 */
            1, /* GR_ASPECT_2x1 */
            1, /* GR_ASPECT_4x1 */
            1  /* GR_ASPECT_8x1 */
    }
};

const int atable[GR_ASPECT_1x8+1][GR_LOD_1+1][2] =
{
   { 
     256 , 32, 
     128 , 16, 
     64  , 8, 
     32  , 4, 
     16  , 2, 
     8   , 1, 
     4   , 1, 
     2   , 1, 
     1, 1 
   },
   { 
     256 , 64, 
     128 , 32, 
     64  , 16, 
     32  , 8, 
     16  , 4, 
     8   , 2,
     4   , 1, 
     2   , 1, 
     1   , 1 
   },
   { 
     256 , 128, 
     128 , 64, 
     64  , 32, 
     32  , 16,
     16  , 8,
     8   , 4, 
     4   , 2, 
     2   , 1, 
     1   , 1 
   },
   { 
     256 , 256, 
     128 , 128, 
     64  , 64,
     32  , 32,
     16  , 16,
     8   , 8, 
     4   , 4, 
     2   , 2, 
     1   , 1 
   },
   { 
     128, 256,
     64,  128,
     32,  64 ,
     16,  32 ,
     8,   16 , 
     4,   8  , 
     2,   4  , 
     1,   2  , 
     1,   1
   },
   {
     64,  256,
     32,  128,
     16,  64 ,
     8,   32 ,
     4,   16 ,
     2,   8  , 
     1,   4  , 
     1,   2  , 
     1,   1
   },
   { 
     32,  256,
     16,  128,
     8,   64 ,
     4,   32 ,
     2,   16 ,
     1,   8  , 
     1,   4  , 
     1,   2  ,
     1,   1
   }
};

/*
** guTexDownloadMipMap
**
** Downloads a mip map (previously allocated with guTexAllocateMemory) to
** the hardware using the given data and ncctble.  The "data" is assumed
** to be in row major order from largest mip map to smallest mip map.
*/
FX_EXPORT void FX_CSTYLE
guTexDownloadMipMap(
                    GrMipMapId_t mmid,
                    const void *src,
                    const GuNccTable *ncc_table ) {
  GrLOD_t     lod;
  const void *ptr = src;

#ifdef GLIDE_DEBUG
  if ( ( mmid == GR_NULL_MIPMAP_HANDLE ) || ( mmid >= _SST->mm_table.free_mmid ) )
    GrErrorCallback( "guTexDownloadMipMap: invalid mip map handle passed", FXTRUE );
#endif

  /*
  ** Bind data and ncc table to this mip map
  */
  _SST->mm_table.data[mmid].data      = (void *) ptr;
  _SST->mm_table.data[mmid].ncc_table = *ncc_table;

  /*
  ** Start downloading mip map levels
  */
  for ( lod = _SST->mm_table.data[mmid].lod_max; lod <= _SST->mm_table.data[mmid].lod_min; lod++ )
  {
     guTexDownloadMipMapLevel( mmid, lod, &ptr );
  }
}

/*
** guTexDownloadMipMapLevel
**
** Downloads a single mip map level to a mip map.  "src" is considered to be
** row major data of the correct aspect ratio and format.
*/
FX_EXPORT void FX_CSTYLE
guTexDownloadMipMapLevel(
                         GrMipMapId_t mmid,
                         GrLOD_t lod,
                         const void **src_base
                         ) {
  const  FxU8  *src8  = ( const FxU8  * ) * ( const FxU8  ** ) src_base;
  const  FxU16 *src16 = ( const FxU16 * ) * ( const FxU16 ** ) src_base;

  const  GrMipMapInfo *mminfo;

  GrChipID_t tmu;
  FxU32  oldtexmode, oldtlod;
  FxU32  texmode, tlod;
  FxU32  oldbaseaddress;
  FxU32  baseaddress;
  FxU32  tmu_baseaddress;
  FxU32  tex_address;
  int    width;                 /* Texture map's width in texels */
  int    max_s;                 /* Texture map's width in 32-bit words */
  int    max_t;                 /* Texture map's height */
  int    s, t;

#ifdef GLIDE_DEBUG
  if ( ( lod > _SST->mm_table.data[mmid].lod_min ) || ( lod < _SST->mm_table.data[mmid].lod_max ) )
    GrErrorCallback( "guTexDownloadMipMapLevel:  specified lod is out of range", FXTRUE );
#endif

  /*
  ** retrieve a const * to the GrMipMapInfo struct
  */
  mminfo = &_SST->mm_table.data[mmid];

  /*
  ** Determine max_s and max_t.  max_s is determined in terms of
  ** number of 32-bit writes per horizontal texture map line.
  */
  if ( _SST->mm_table.data[mmid].format < GR_TEXFMT_16BIT )
  {
    width = atable[_SST->mm_table.data[mmid].aspect_ratio][lod][0];
    max_s = width >> 2;
    if ( max_s <= 0 )
       max_s = 1;
  }
  else
  {
    width = atable[_SST->mm_table.data[mmid].aspect_ratio][lod][0];
    max_s = width >> 1;
    if ( max_s <= 0 )
       max_s = 1;
  }
  max_t = atable[_SST->mm_table.data[mmid].aspect_ratio][lod][1];

  /*
  ** skip if not in odd/even mask
  */

  if ( lod & 0x1 )
  {
    if ( ! (mminfo->odd_even_mask & GR_MIPMAPLEVELMASK_ODD) )
    {
      if ( mminfo->format < GR_TEXFMT_16BIT )
        *src_base = (void*) ( src8 + ( width * max_t ) );
      else
        *src_base = (void*) ( src16 + ( width * max_t ) );

      return;
    }
  }
  else
  {
    if ( ! (mminfo->odd_even_mask & GR_MIPMAPLEVELMASK_EVEN) )
    {
      if ( mminfo->format < GR_TEXFMT_16BIT )
        *src_base = (void*) ( src8 + ( width * max_t ) );
      else
        *src_base = (void*) ( src16 + ( width * max_t ) );

      return;
    }
  }

  /*
  ** determine TMU of this mip map
  */
  tmu = mminfo->tmu;

#ifdef GLIDE_DEBUG
  if ( tmu < GR_TMU0 || tmu >= SST_TMU_INDEX_TO_CHIP(_SST->num_tmu) )
  {
    GrErrorCallback( "guTexDownloadMipMapLevel: invalid tmu specified", FXTRUE );
  }
#endif

  /*
  ** Determine base address of this mip map
  */
  baseaddress = _SST->mm_table.data[mmid].tmu_base_address;

  tlod = _SST->mm_table.data[mmid].tLOD;
  texmode = _SST->mm_table.data[mmid].tTextureMode;

  /*
  ** Calculate linear address of texture memory we're interested in.
  */
  tmu_baseaddress   = ( FxU32 ) _SST->tex_ptr;
  tmu_baseaddress  += ( ( ( FxU32 ) tmu ) << 21 );
  tmu_baseaddress  += ( ( ( FxU32 ) lod ) << 17 );
  tex_address       = tmu_baseaddress;

  /*
  ** Set the hardware's texture mode, lod, and base address
  */
  oldbaseaddress = _SST->state.tmu_config[SST_TMU_CHIP_TO_INDEX(tmu)].texBaseAddr;
  oldtexmode     = _SST->state.tmu_config[SST_TMU_CHIP_TO_INDEX(tmu)].textureMode;
  oldtlod        = _SST->state.tmu_config[SST_TMU_CHIP_TO_INDEX(tmu)].tLOD;

  grSstWriteConfigTMU( tmu, SSTR_TEXBASEADDR, baseaddress );
  grSstWriteConfigTMU( tmu, SSTR_TEXTUREMODE, texmode );
  grSstWriteConfigTMU( tmu, SSTR_TLOD, tlod );

  /*
  ** Handle 8-bit textures
  */
  if ( _SST->mm_table.data[mmid].format < GR_TEXFMT_16BIT )
  {
    /*
    ** Handle 1xN textures
    */
    if ( width == 1 )
    {
      for ( t = 0; t < max_t; t++ )
      {
        tex_address = tmu_baseaddress + ( t << 9 );
        for ( s = 0; s < max_s; s++ )
        {
          grSstWriteAbsolute32( tex_address + ( s << 3 ), *src8 );
          src8 += 1;
        }
      }
    }
    /*
    ** Handle 2xN textures
    */
    else if ( width == 2 )
    {
      for ( t = 0; t < max_t; t++ )
      {
        tex_address = tmu_baseaddress + ( t << 9 );
        for ( s = 0; s < max_s; s++ )
        {
          grSstWriteAbsolute32( tex_address + ( s << 3 ), (src8[1]<<8) | src8[0] );
          src8 += 2;
        }
      }
    }

    /*
    ** Handle all other textures
    */
    else
    {
      for ( t = 0; t < max_t; t++ )
      {
        tex_address = tmu_baseaddress + ( t << 9 );
        for ( s = 0; s < max_s; s++ )
        {
          grSstWriteAbsolute32( tex_address + ( s << 3 ), (src8[3]<<24) | (src8[2]<<16) | (src8[1]<<8) | src8[0] );
          src8 += 4;
        }
      }
    }
  }
  /*
  ** Handle 16-bit textures
  */
  else
  {
    /*
    ** Handle 1xN textures
    */
    if ( width == 1 )
    {
      for ( t = 0; t < max_t; t++ )
      {
        tex_address = tmu_baseaddress + ( t << 9 );
        for ( s = 0; s < max_s; s += 2 )
        {
          grSstWriteAbsolute32( tex_address + ( s << 2 ), *src16 );
          src16 += 1;
        }
      }
    }
    /*
    ** Handle non-1xN textures
    */
    else
    {
      for ( t = 0; t < max_t; t++ )
      {
        tex_address = tmu_baseaddress + ( t << 9 );
        for ( s = 0; s < max_s; s++ )
        {
          grSstWriteAbsolute32( tex_address + ( s << 2 ), (src16[1] << 16) | src16[0] );
          src16 += 2;
        }
      }
    }
  }

  /*
  ** Restore trashed registers
  */
  grSstWriteConfigTMU( tmu, SSTR_TEXBASEADDR, oldbaseaddress );
  grSstWriteConfigTMU( tmu, SSTR_TEXTUREMODE, oldtexmode );
  grSstWriteConfigTMU( tmu, SSTR_TLOD, oldtlod );

  /*
  ** update the source pointer
  */
  if ( _SST->mm_table.data[mmid].format < GR_TEXFMT_16BIT )
    *src_base = src8;
  else
    *src_base = src16;
}


/*-------------------------------------------------------------------
  Function: grTexDownloadTable
  Date: 6/3
  Implementor(s): jdt, GaryMcT
  Library: glide
  Description:
    download look up table data to a tmu
  Arguments:
    tmu - which tmu
    type - what type of table to download
        One of:
            GR_TEXTABLE_NCC0
            GR_TEXTABLE_NCC1
            GR_TEXTABLE_PALETTE
    void *data - pointer to table data
  Return:
    none
  -------------------------------------------------------------------*/
FX_EXPORT void FX_CSTYLE
grTexDownloadTable( GrChipID_t   tmu,
                    GrTexTable_t type, 
                    void         *data ) {
#ifdef GLIDE_DEBUG
    if (tmu < GR_TMU0 || tmu >= SST_TMU_INDEX_TO_CHIP(_SST->num_tmu))
        GrErrorCallback( "grTexDownloadTable: bogus TMU specified!\n", 
                         FXTRUE);
    if ( type > 0x2 )
        GrErrorCallback( "grTexDownloadTable: invalid table specified.\n",
                         FXTRUE );
    if ( !data ) 
        GrErrorCallback( "grTexDownloadTable: invalid data pointer.\n",
                         FXTRUE );
#endif
    if ( type == GR_TEXTABLE_PALETTE ) {
        /* Need Palette Download Code */
        _grTexDownloadPalette( tmu, (GuTexPalette *)data );
    } else { /* Type is an ncc table */
        _grTexDownloadNccTable( tmu, type, (GuNccTable*)data );
    }
}


/*-------------------------------------------------------------------
  Function: grTexDownloadMipMapLevel
  Date: 6/2
  Implementor(s): GaryMcT, Jdt
  Library: glide
  Description:
    Downloads a mipmap level to the specified tmu at the given
    texture start address
  Arguments:
    tmu           - which tmu
    startAddress - starting address for texture download,
                     this should be some value between grTexMinAddress()
                     and grTexMaxAddress()
    thisLod      - lod constant that describes the mipmap level
                    to be downloaded
    largeLod     - largest level of detail in complete mipmap to 
                   be downloaded at startAddress of which level to
                   be downloaded is a part
    aspectRatio  - aspect ratio of this mipmap
    format        - format of mipmap image data
    evenOdd      - which set of mipmap levels have been downloaded for
                    the selected texture
                    One of:
                      GR_MIPMAPLEVELMASK_EVEN 
                      GR_MIPMAPLEVELMASK_ODD
                      GR_MIPMAPLEVELMASK_BOTH
    data          - pointer to mipmap data
  Return:
    none
  -------------------------------------------------------------------*/
FX_ENTRY void
FX_CSTYLE grTexDownloadMipMapLevel( GrChipID_t        tmu,
                                    FxU32             startAddress,
                                    GrLOD_t           thisLod,
                                    GrLOD_t           largeLod,
                                    GrAspectRatio_t   aspectRatio,
                                    GrTextureFormat_t format,
                                    FxU32             evenOdd,
                                    void              *data ) {
    const  FxU8  *src8  = ( const FxU8  * ) data;
    const  FxU16 *src16 = ( const FxU16 * ) data;
    FxU32 max_s, max_t, s, t, width, tex_address, tmu_baseaddress;
    FxU32 tLod, texMode, baseAddress;
    FxU32 oldTLod, oldTexMode, oldBaseAddress;

#ifdef GLIDE_DEBUG
    if ( tmu > 0x2 )
        GrErrorCallback( "grTexDownloadLevel: Bad tmu.\n", FXTRUE );
    if ( startAddress > grTexMaxAddress( tmu ) )
        GrErrorCallback( "grTexDownloadLevel: invalid startAddress.\n",
                         FXTRUE );
    if ( thisLod > GR_LOD_1 )
        GrErrorCallback( "grTexDownloadLevel: thisLod invalid.\n", FXTRUE );
    if ( largeLod > GR_LOD_1 )
        GrErrorCallback( "grTexDownloadLevel: largeLod invalid.\n", FXTRUE );
    if ( thisLod < largeLod )
        GrErrorCallback( "grTexDownloadLevel: thisLod may not be"
                         " larger than largeLod.\n", FXTRUE );
    if ( aspectRatio > GR_ASPECT_1x8 || aspectRatio < GR_ASPECT_8x1 ) 
        GrErrorCallback( "grTexDownloadLevel: aspectRatio invalid.\n", FXTRUE );
    if ( evenOdd > 0x3 || evenOdd == 0 )
        GrErrorCallback( "grTexDownloadLevel: evenOdd mask invalid.\n", FXTRUE );
    if ( !data ) 
        GrErrorCallback( "grTexDownloadLevel: data invalid.\n", FXTRUE );
#endif        

    /*------------------------------------------------------------
      Determine max_s and max_t  
      ------------------------------------------------------------*/
    if ( format < GR_TEXFMT_16BIT ) { /* 8-bit texture */
        width = atable[aspectRatio][thisLod][0];
        max_s = width >> 2;
        if ( max_s <= 0 )
        max_s = 1;
    } else { /* 16-bit texture */
        width = atable[aspectRatio][thisLod][0];
        max_s = width >> 1;
        if ( max_s <= 0 )
            max_s = 1;
    }
    max_t = atable[aspectRatio][thisLod][1];

    /*------------------------------------------------------------
      Skip this level entirely if not in odd/even mask
      ------------------------------------------------------------*/
    if ( thisLod & 0x1 ) {
        if ( ! (evenOdd & GR_MIPMAPLEVELMASK_ODD) ) {
            return;
        }
    } else {
        if ( ! (evenOdd & GR_MIPMAPLEVELMASK_EVEN) ) {
            return;
        }
    }

    /*------------------------------------------------------------
      Compute Base Address Given Start Address Offset
      ------------------------------------------------------------*/
    baseAddress = _grTexCalcBaseAddress( startAddress,
                                         largeLod, 
                                         aspectRatio,
                                         format,
                                         evenOdd ) >> 3;


    /*------------------------------------------------------------
      Compute Physical Write Pointer
      ------------------------------------------------------------*/
    tmu_baseaddress = (FxU32)_SST->tex_ptr;
    tmu_baseaddress += (((FxU32)tmu)<<21) + (((FxU32)thisLod)<<17);

    /*------------------------------------------------------------
      Compute pertinant contents of tLOD and texMode registers 
      ------------------------------------------------------------*/
    tLod = ( ( GR_LOD_1 << 2 ) << SST_LODMAX_SHIFT) |
           ( ( largeLod << 2 ) << SST_LODMIN_SHIFT);
    if ( evenOdd != GR_MIPMAPLEVELMASK_BOTH ) {
       tLod |= SST_LOD_TSPLIT;
       if ( evenOdd & GR_MIPMAPLEVELMASK_ODD )
            tLod |= SST_LOD_ODD;
    }
    if ( aspectRatio < GR_ASPECT_1x1 )
        tLod |= SST_LOD_S_IS_WIDER;
    tLod |= aspect_bit_table[aspectRatio] << SST_LOD_ASPECT_SHIFT;
    texMode = format << SST_TFORMAT_SHIFT;

    /*------------------------------------------------------------
      Save TLOD, texMode, baseAddress
      ------------------------------------------------------------*/
    oldTexMode     = _SST->state.tmu_config[SST_TMU_CHIP_TO_INDEX(tmu)].textureMode;
    oldTLod        = _SST->state.tmu_config[SST_TMU_CHIP_TO_INDEX(tmu)].tLOD;
    oldBaseAddress = _SST->state.tmu_config[SST_TMU_CHIP_TO_INDEX(tmu)].texBaseAddr;

    /*------------------------------------------------------------
      Update TLOD, texMode, baseAddress
      ------------------------------------------------------------*/
    grSstWriteConfigTMU( tmu, SSTR_TEXTUREMODE, texMode );
    grSstWriteConfigTMU( tmu, SSTR_TLOD, tLod );
    grSstWriteConfigTMU( tmu, SSTR_TEXBASEADDR, baseAddress );

    /*------------------------------------------------------------
      Handle 8-bit Textures
      ------------------------------------------------------------*/
    if ( format < GR_TEXFMT_16BIT ) {
        if ( width == 1 ) { /* 1xN textures */
            for ( t = 0; t < max_t; t++ ) {
                tex_address = tmu_baseaddress + ( t << 9 );
                for ( s = 0; s < max_s; s++ ) {
                    grSstWriteAbsolute32( tex_address + ( s << 3 ), 
                                          *src8 );
                    src8 += 1;
                }
            }
        } else if ( width == 2 ) { /* 2xN textures */
            for ( t = 0; t < max_t; t++ ) {
                tex_address = tmu_baseaddress + ( t << 9 );
                for ( s = 0; s < max_s; s++ ) {
                    grSstWriteAbsolute32( tex_address + ( s << 3 ), 
                                          (src8[1]<<8) | src8[0] );
                    src8 += 2;
                }
            }
        } else { /* All other textures */
            for ( t = 0; t < max_t; t++ ) {
                tex_address = tmu_baseaddress + ( t << 9 );
                for ( s = 0; s < max_s; s++ ) {
                    grSstWriteAbsolute32( tex_address + ( s << 3 ), 
                                          (src8[3]<<24)|(src8[2]<<16)|(src8[1]<<8)|src8[0] );
                    src8 += 4;
                }
            }
        }
    /*------------------------------------------------------------
      Handle 16-bit Textures
      ------------------------------------------------------------*/
    } else { 
        if ( width == 1 ) { /* 1xN textures */
            for ( t = 0; t < max_t; t++ ) {
                tex_address = tmu_baseaddress + ( t << 9 );
                for ( s = 0; s < max_s; s += 2 ) {
                      grSstWriteAbsolute32( tex_address + ( s << 2 ), 
                                            *src16 );
                      src16 += 1;
                }
            }
        } else { /* All other textures */
            for ( t = 0; t < max_t; t++ ) {
                tex_address = tmu_baseaddress + ( t << 9 );
                for ( s = 0; s < max_s; s++ ) {
                    grSstWriteAbsolute32( tex_address + ( s << 2 ), 
                                          (src16[1]<<16) | src16[0] );
                    src16 += 2;
                }
            }
        }
    }

    /*------------------------------------------------------------
      Restore TLOD, texMode, baseAddress
      ------------------------------------------------------------*/
    grSstWriteConfigTMU( tmu, SSTR_TEXTUREMODE, oldTexMode );
    grSstWriteConfigTMU( tmu, SSTR_TLOD, oldTLod );
    grSstWriteConfigTMU( tmu, SSTR_TEXBASEADDR, oldBaseAddress );
    return;
}

/*-------------------------------------------------------------------
  Function: grTexDownloadMipMap
  Date: 6/2
  Implementor(s): GaryMcT, Jdt
  Library: glide
  Description:
    Downloads a texture mipmap to the specified tmu at the specified
    base address.
  Arguments:
    tmu          - which tmu
    startAddress - starting address for texture download,
                     this should be calculated using grTexMinStartAddress()
    evenOdd  - which set of mipmap levels have been downloaded for
                the selected texture
                One of:
                  GR_MIPMAPLEVELMASK_EVEN 
                  GR_MIPMAPLEVELMASK_ODD
                  GR_MIPMAPLEVELMASK_BOTH
    info       - pointer to GrTexInfo structure defining dimension of
                 texture to be downloaded and containing texture data
  Return:
    none
  -------------------------------------------------------------------*/
FX_EXPORT void 
FX_CSTYLE grTexDownloadMipMap( GrChipID_t tmu,
                               FxU32      startAddress,
                               FxU32      evenOdd,
                               GrTexInfo  *info ) {
    GrLOD_t lod;
    char *src_base;
#ifdef GLIDE_DEBUG
    if ( tmu > 0x2 )
        GrErrorCallback( "grTexDownload: Bad tmu.\n", FXTRUE );
    if ( startAddress > grTexMaxAddress( tmu ) )
        GrErrorCallback( "grTexDownload: invalid startAddress.\n",
                         FXTRUE );
    if ( startAddress + grTexTextureMemRequired( evenOdd, info ) >
         grTexMaxAddress( tmu ) )
        GrErrorCallback( "grTexDownload: insufficient texture ram at startAddress.",
                         FXTRUE );
    if ( evenOdd > 0x3 )
        GrErrorCallback( "grTexDownload: evenOdd mask invalid.\n", FXTRUE );
    if ( !info ) 
        GrErrorCallback( "grTexDownload: info invalid.\n", FXTRUE );
#endif

    src_base = (char *)info->data;

    /*---------------------------------------------------------------
      Download one mipmap level at a time
      ---------------------------------------------------------------*/
    for( lod = info->largeLod; lod <= info->smallLod; lod++ ) {
        grTexDownloadMipMapLevel( tmu, 
                                  startAddress, 
                                  lod,
                                  info->largeLod,
                                  info->aspectRatio,
                                  info->format,
                                  evenOdd,
                                  src_base );
        src_base += 
            ((size_table[lod][aspect_bit_table[info->aspectRatio]])
              <<(info->format>=GR_TEXFMT_16BIT));
    }
    return;
}


/*-------------------------------------------------------------------
  Function: _grTexDownloadPalette
  Date: 6/9
  Implementor(s): jdt
  Library: Glide
  Description:
    Private function to download a palette to the specified tmu
  Arguments:
    tmu - which tmu to download the palette to
  Return:
    none
  -------------------------------------------------------------------*/
void _grTexDownloadPalette( GrChipID_t   tmu, 
                            GuTexPalette *pal ) {
    FxU32 burst;
    FxU32 line;
    FxU32 entry;

    entry = 0;
    for( burst = 0; burst < 32; burst++ ) {
        for( line = 0; line < 8; line++ ) {
            FxU32 data = 0x80000000 |
                         (( entry << 23 )&0x7F000000) |
                         pal->data[entry];
            grSstWriteConfigTMU( tmu, 
                                 SSTR_NCCTABLE0+4+line,
                                 data );
            entry++;
        }
    }
}
