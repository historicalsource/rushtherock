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
** $Header: /engr/devel/sst1/GLIDE/DOS/SRC/GU.C 29    6/11/96 7:36a Sellers $
** $Log: /engr/devel/sst1/GLIDE/DOS/SRC/GU.C $
 * 
 * 29    6/11/96 7:36a Sellers
 * re-wrote guFbReadRegion() for functionality and SLI support
 * 
 * 28    5/18/96 9:27p Tarolli
 * added:  float guFogTableIndexToW( int i );
 * changed the way the fog table is computed
 * 
 * 27    5/09/96 7:05a Paik
 * Glide DLL support
 * 
 * 26    4/05/96 2:24a Paik
 * Get rid of annoying warning about mismatched parameters
 * 
 * 25    3/25/96 7:58p Paik
 * Move fxglide.h out of glide.h
 * 
 * 24    2/28/96 4:43p Dow
 * Bug fixes in ReadRegion
 * 
 * 23    2/09/96 4:44p Dow
 * guFbWriteRegion
 * 
 * 22    1/12/96 2:28p Paik
 * [sw-glide/225, sw-glide/242]
 * Executive summary
 * 1. created grAlphaCombine and grColorCombine which fully expose acu and
 * ccu.
 * 2. renamed grColorCombineFunction and grAlphaSource to
 * guColorCombineFunction and guAlphaSource, reimplement on top of
 * grColorCombine and grAlphaCombine.
 * 
 * 18    1/09/96 9:42a Dow
 * tabs -> spaces and added Rev. Control keywords
**
*/
#ifdef GLIDE_DEBUG
#include <stdio.h>
#include <conio.h>
#endif

#include <stdlib.h>
#include <math.h>

#include <3dfx.h>
#define FX_DLL_DEFINITION
#include <fxdll.h>
#include <glide.h>
#include "fxglide.h"

#if (GLIDE_PLATFORM & GLIDE_SST_HW)
#include <sst1init.h>
#endif

GrVertex  __gu_prim_vertex_array[GU_PRIM_MAX_VERTICES];
GrVertex *__gu_prim_current_vtx;
int       __gu_prim_num_verts;

int __debugGets;

/*
**  guAlphaSource
*/
FX_EXPORT void
FX_CSTYLE guAlphaSource( GrAlphaSource_t mode ) {
  switch ( mode )
  {
  case GR_ALPHASOURCE_CC_ALPHA:
    grAlphaCombine( GR_COMBINE_FUNCTION_LOCAL, GR_COMBINE_FACTOR_NONE, GR_COMBINE_LOCAL_CONSTANT, GR_COMBINE_OTHER_NONE, FXFALSE );
    break;

  case GR_ALPHASOURCE_ITERATED_ALPHA:
    grAlphaCombine( GR_COMBINE_FUNCTION_LOCAL, GR_COMBINE_FACTOR_NONE, GR_COMBINE_LOCAL_ITERATED, GR_COMBINE_OTHER_NONE, FXFALSE );
    break;

  case GR_ALPHASOURCE_TEXTURE_ALPHA:
    grAlphaCombine( GR_COMBINE_FUNCTION_SCALE_OTHER, GR_COMBINE_FACTOR_ONE, GR_COMBINE_LOCAL_NONE, GR_COMBINE_OTHER_TEXTURE, FXFALSE );
    break;

  case GR_ALPHASOURCE_TEXTURE_ALPHA_TIMES_ITERATED_ALPHA:
    grAlphaCombine( GR_COMBINE_FUNCTION_SCALE_OTHER, GR_COMBINE_FACTOR_LOCAL, GR_COMBINE_LOCAL_ITERATED, GR_COMBINE_OTHER_TEXTURE, FXFALSE );
    break;

#ifdef GLIDE_DEBUG
  default:
    GrErrorCallback( "grAlphaSource:  unknown alpha source mode", FXTRUE );
    break;
#endif
  }

  /* xxx not needed at the moment, should update grFogxxx
  _grVerifyNeedForITAlpha();
  */
} /* guAlphaSource */

/*
**  guColorCombineFunction
*/
FX_EXPORT void
FX_CSTYLE guColorCombineFunction( GrColorCombineFnc_t fnc ) {
  /* gross hack to get ITRGB_DELTA0 modes working */

  _grColorCombineDelta0Mode( FXFALSE );

  switch ( fnc )
  {
  case GR_COLORCOMBINE_ZERO:
    grColorCombine( GR_COMBINE_FUNCTION_ZERO, GR_COMBINE_FACTOR_NONE, GR_COMBINE_LOCAL_NONE, GR_COMBINE_OTHER_NONE, FXFALSE );
    break;

  case GR_COLORCOMBINE_CCRGB:
    grColorCombine( GR_COMBINE_FUNCTION_LOCAL, GR_COMBINE_FACTOR_NONE, GR_COMBINE_LOCAL_CONSTANT, GR_COMBINE_OTHER_NONE, FXFALSE );
    break;

  case GR_COLORCOMBINE_ITRGB:
    grColorCombine( GR_COMBINE_FUNCTION_LOCAL, GR_COMBINE_FACTOR_NONE, GR_COMBINE_LOCAL_ITERATED, GR_COMBINE_OTHER_NONE, FXFALSE );
    break;

  case GR_COLORCOMBINE_ITRGB_DELTA0:
    _grColorCombineDelta0Mode( FXTRUE );
    grColorCombine( GR_COMBINE_FUNCTION_LOCAL, GR_COMBINE_FACTOR_NONE, GR_COMBINE_LOCAL_ITERATED, GR_COMBINE_OTHER_NONE, FXFALSE );
    break;

  case GR_COLORCOMBINE_DECAL_TEXTURE:
    grColorCombine( GR_COMBINE_FUNCTION_SCALE_OTHER, GR_COMBINE_FACTOR_ONE, GR_COMBINE_LOCAL_NONE, GR_COMBINE_OTHER_TEXTURE, FXFALSE );
    break;

  case GR_COLORCOMBINE_TEXTURE_TIMES_CCRGB:
    grColorCombine( GR_COMBINE_FUNCTION_SCALE_OTHER, GR_COMBINE_FACTOR_LOCAL, GR_COMBINE_LOCAL_CONSTANT, GR_COMBINE_OTHER_TEXTURE, FXFALSE );
    break;

  case GR_COLORCOMBINE_TEXTURE_TIMES_ITRGB:
    grColorCombine( GR_COMBINE_FUNCTION_SCALE_OTHER, GR_COMBINE_FACTOR_LOCAL, GR_COMBINE_LOCAL_ITERATED, GR_COMBINE_OTHER_TEXTURE, FXFALSE );
    break;

  case GR_COLORCOMBINE_TEXTURE_TIMES_ITRGB_DELTA0:
    _grColorCombineDelta0Mode( FXTRUE );
    grColorCombine( GR_COMBINE_FUNCTION_SCALE_OTHER, GR_COMBINE_FACTOR_LOCAL, GR_COMBINE_LOCAL_ITERATED, GR_COMBINE_OTHER_TEXTURE, FXFALSE );
    break;

  case GR_COLORCOMBINE_TEXTURE_TIMES_ITRGB_ADD_ALPHA:
    grColorCombine( GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL_ALPHA, GR_COMBINE_FACTOR_LOCAL, GR_COMBINE_LOCAL_ITERATED, GR_COMBINE_OTHER_TEXTURE, FXFALSE );
    break;

  case GR_COLORCOMBINE_TEXTURE_TIMES_ALPHA:
    grColorCombine( GR_COMBINE_FUNCTION_SCALE_OTHER, GR_COMBINE_FACTOR_LOCAL_ALPHA, GR_COMBINE_LOCAL_NONE, GR_COMBINE_OTHER_TEXTURE, FXFALSE );
    break;

  case GR_COLORCOMBINE_TEXTURE_TIMES_ALPHA_ADD_ITRGB:
    grColorCombine( GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL, GR_COMBINE_FACTOR_LOCAL_ALPHA, GR_COMBINE_LOCAL_ITERATED, GR_COMBINE_OTHER_TEXTURE, FXFALSE );
    break;

  case GR_COLORCOMBINE_TEXTURE_ADD_ITRGB:
    grColorCombine( GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL, GR_COMBINE_FACTOR_ONE, GR_COMBINE_LOCAL_ITERATED, GR_COMBINE_OTHER_TEXTURE, FXFALSE );
    break;

  case GR_COLORCOMBINE_TEXTURE_SUB_ITRGB:
    grColorCombine( GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL, GR_COMBINE_FACTOR_ONE, GR_COMBINE_LOCAL_ITERATED, GR_COMBINE_OTHER_TEXTURE, FXFALSE );
    break;

  case GR_COLORCOMBINE_CCRGB_BLEND_ITRGB_ON_TEXALPHA:
    grColorCombine( GR_COMBINE_FUNCTION_BLEND, GR_COMBINE_FACTOR_TEXTURE_ALPHA, GR_COMBINE_LOCAL_CONSTANT, GR_COMBINE_OTHER_ITERATED, FXFALSE );
    break;

  case GR_COLORCOMBINE_DIFF_SPEC_A:
    grColorCombine( GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL, GR_COMBINE_FACTOR_LOCAL_ALPHA, GR_COMBINE_LOCAL_ITERATED, GR_COMBINE_OTHER_TEXTURE, FXFALSE );
    break;

  case GR_COLORCOMBINE_DIFF_SPEC_B:
    grColorCombine( GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL_ALPHA, GR_COMBINE_FACTOR_LOCAL, GR_COMBINE_LOCAL_ITERATED, GR_COMBINE_OTHER_TEXTURE, FXFALSE );
    break;

  case GR_COLORCOMBINE_ONE:
    grColorCombine( GR_COMBINE_FUNCTION_ZERO, GR_COMBINE_FACTOR_NONE, GR_COMBINE_LOCAL_NONE, GR_COMBINE_OTHER_NONE, FXTRUE );
    break;
    
#ifdef GLIDE_DEBUG
  default:
    GrErrorCallback( "grColorCombineFunction:  unsupported color combine function", FXTRUE );
    break;
#endif
  }
} /* guColorCombineFunction */

/*
** guEndianSwapWords
*/
FX_EXPORT FxU32
FX_CSTYLE guEndianSwapWords( FxU32 value ) {
   return ( ( value & 0xFFFF0000 ) >> 16 ) | ( value << 16 );
}

/*
** guEndianSwapBytes
*/
FX_EXPORT FxU16
FX_CSTYLE guEndianSwapBytes( FxU16 value ) {
   return ( ( value & 0xFF00 ) >> 8 ) | ( value << 8 );
}

/*
** guFogTableIndexToW
*/
FX_EXPORT float
FX_CSTYLE guFogTableIndexToW( int i )
{
#if 0
    return ( float ) pow( 2.0, .25 * i);	/* old way */
#else
    int mant = i&3;				/* new way (GMT) */
    double oow;

    oow = pow(2.0,(double)-(i>>2));
    if (mant != 0)
	oow *= 0.5*(1.0 + .25*(4-(mant&3)));
    return (float)(1.0/oow); 
#endif
}

/*
** guFogGenerateExp
*/
FX_EXPORT void
FX_CSTYLE guFogGenerateExp( GrFog_t fogtable[GR_FOG_TABLE_SIZE], float density ) {
  int   i;
  float f;
  float scale;
  float dp;

  dp = density * guFogTableIndexToW( GR_FOG_TABLE_SIZE - 1 );
  scale = 1.0F / ( 1.0F - ( float ) exp( -dp ) );

  for ( i = 0; i < GR_FOG_TABLE_SIZE; i++ )
  {
     dp = density * guFogTableIndexToW( i );
     f = ( 1.0F - ( float ) exp( -dp ) ) * scale;

     if ( f > 1.0F )
        f = 1.0F;
     else if ( f < 0.0F )
        f = 0.0F;

     f *= 255.0F;
     fogtable[i] = ( GrFog_t ) f;
  }
} /* guFogGenerateExp */

/*
** guFogGenerateExp2
*/
FX_EXPORT void
FX_CSTYLE guFogGenerateExp2( GrFog_t fogtable[GR_FOG_TABLE_SIZE], float density ) {
  int   i;
  float f;
  float scale;
  float dp;

  dp = density * guFogTableIndexToW( GR_FOG_TABLE_SIZE - 1 );
  scale = 1.0F / ( 1.0F - ( float ) exp( -( dp * dp ) ) );

  for ( i = 0; i < GR_FOG_TABLE_SIZE; i++ )
  {
     dp = density * guFogTableIndexToW( i );
     f = ( 1.0F - ( float ) exp( -( dp * dp ) ) ) * scale;

     if ( f > 1.0F )
        f = 1.0F;
     else if ( f < 0.0F )
        f = 0.0F;

     f *= 255.0F;
     fogtable[i] = ( GrFog_t ) f;
  }
} /* guFogGenerateExp2 */

/*
** guFogGenerateLinear
*/
FX_EXPORT void
FX_CSTYLE guFogGenerateLinear( GrFog_t fogtable[GR_FOG_TABLE_SIZE], float nearZ, float farZ ) {
   int i;
   float world_w;
   float f;

   for ( i = 0; i < GR_FOG_TABLE_SIZE; i++ )
   {
      world_w = guFogTableIndexToW( i );
      if ( world_w > 65535.0F )
         world_w = 65535.0F;

      f = ( world_w - nearZ ) / ( farZ - nearZ );
      if ( f > 1.0F )
         f = 1.0F;
      else if ( f < 0.0F )
         f = 0.0F;
      f *= 255.0F;
      fogtable[i] = ( GrFog_t ) f;
   }
} /* guFogGenerateLinear */

/*
** setlevel
*/
static void
setlevel( FxU16 *data, FxU16 color, int width, int height ) {
   int s, t;

   for ( t = 0; t < height; t++ )
   {
      for ( s = 0; s < width; s++ )
      {
         *data = color;
          data++;
      }
   }
}/* setlevel */

#if !defined(__GNUC__)
/*
**  guTexCreateColorMipMap
*/
FX_EXPORT FxU16 *
FX_CSTYLE guTexCreateColorMipMap( void ) {
   FxU32 memrequired;
   FxU16 *data;
   FxU16 *start;

   memrequired = 2 * ( 256 * 256 + 128 * 128 + 64 * 64 + 32 * 32 + 16 * 16 + 8 * 8 + 4 * 4 + 2 * 2 + 1 * 1 );
   start = data = malloc( memrequired );
   if ( !data )
      return 0;

   setlevel( data,            0xF800, 256, 256 );
   setlevel( data += 256*256, 0x07e0, 128, 128 );
   setlevel( data += 128*128, 0x001F, 64, 64 );
   setlevel( data += 64*64,   0xFFFF, 32, 32);
   setlevel( data += 32*32,   0x0000, 16, 16 );
   setlevel( data += 16*16,   0xF800, 8, 8);
   setlevel( data += 8*8,     0x07e0, 4, 4 );
   setlevel( data += 4*4,     0x001f, 2, 2 );
   setlevel( data += 2*2,     0xFFFF, 1, 1 );

   return start;
} /* guTexCreateColoMipMap */
#endif

/*
**  guFbReadRegion
*/
FX_EXPORT void
FX_CSTYLE guFbReadRegion(
    const int
      srcX,                     /* X-ordinate of ULC */
    const int
      srcY,                     /* Y ordinate of ULC */
    const int w, const int h,   /* Width & Height of rect to read */
    const void
      *dst,                     /* buffer in which result is stored */
    const int
    strideInBytes               /* distance from (x,y) to (x,y+1) in bytes */
    ) {

    int x, y;
    FxU32 *lfbPtr, lfbReadAddr;
    FxU32 *longPtr;
    unsigned char *charPtr;
    unsigned short *shortPtr;

#ifdef GLIDE_DEBUG
  if ((srcX < 0) || ((srcX + w) > _SST->state.screen_width))
    GrErrorCallback("guFbReadRegion:  invalid combination of y & h\n",FXTRUE);
  if ((srcY < 0) || ((srcY + h) > _SST->state.screen_height))
    GrErrorCallback("guFbReadRegion: invalid combination of x & w\n",FXTRUE);
#endif

#if (GLIDE_PLATFORM & GLIDE_SST_HW)
    sst1InitIdle(_SST->base_ptr);
#endif
    charPtr = (unsigned char *) dst;
    lfbReadAddr = (FxU32) _SST->lfb_ptr;
    for(y=srcY; y<(srcY+h); y++) {
#if (GLIDE_PLATFORM & GLIDE_SST_HW)
      if(_SST->scanline_interleaved == FXTRUE) {
        if(y & 0x1)
          sst1InitSliPciOwner(_SST->base_ptr, SST_SLI_SLAVE_OWNPCI);
        else
          sst1InitSliPciOwner(_SST->base_ptr, SST_SLI_MASTER_OWNPCI);
      }
#endif
      x = srcX;
      shortPtr = (unsigned short *) charPtr;
      lfbPtr = (FxU32 *) (lfbReadAddr + (y << 11) + ((x&0x3fe) << 1));

      /* Left Edge */
      if(x & 1) {
        x++;
        *shortPtr++ = (unsigned short) (*lfbPtr++ >> 16);
      }

      /* Middle */
      longPtr = (FxU32 *) shortPtr;
      for( ; x < (srcX+w-1); x+=2)
        *longPtr++ = *lfbPtr++;

      /* Right Edge */
      if(x < (srcX+w)) {
        shortPtr = (unsigned short *) longPtr;
        *shortPtr = (unsigned short) (*lfbPtr & 0xffff);
      }
      charPtr = (unsigned char *) ((FxU32) charPtr + (FxU32) strideInBytes);
    } /* Y loop */
#if (GLIDE_PLATFORM & GLIDE_SST_HW)
    if(_SST->scanline_interleaved == FXTRUE)
      sst1InitSliPciOwner(_SST->base_ptr, SST_SLI_MASTER_OWNPCI);
#endif
} /* guFbReadRegion */

/*
**  guFbWriteRegion
*/
FX_EXPORT void
FX_CSTYLE guFbWriteRegion(
                const int
                  dstX,         /* X-ordinate of dest rect ULC */
                const int
                  dstY,         /* Y-ordinate of dest rect ULC */
                const int
                  w,            /* width of dest rect */
                const int
                  h,            /* height of dest rect */
                const void
                  *src,         /* Src pointer */
                const int
                  strideInBytes /* bytes btween (x,y) & (x, y+1) */
                ) {
  FxU32
    lfbMode;                    /* lfbMode SST-1 register */
  FxU32
    lfbFormat;                  /* Format of data from LFB reads */
  int
    i,j,
    maxX,                       /* Max value for X */
    startRun,                   /* x positionof beginning of aligned stores */
    maxRun,                     /* Max run of 32-bit stores */
    endRun,                     /* End of run */
    srcLine,                    /* Line of src we're blitting  */
    incStep,                    /* Size to increment index by */
    pixelSize;                  /* sizeof pixels gotten from lfb Reads */
  FxU32
    lfbWriteAddr;               /* used for write address calculation */
  FxU32
    *endPtr,                                    /* pointer to last pixel of run */
    *lfbPtr,                    /* pointer for storing to lfb */
    *srcPtr;                    /* Pointer for loading from src*/

#ifdef GLIDE_DEBUG
  if ((dstX < 0) || ((dstX + w) > _SST->state.screen_width))
    GrErrorCallback("guFbWriteRegion:  invalid combination of y & h\n",FXTRUE);
  if ((dstY < 0) || ((dstY + h) > _SST->state.screen_height))
    GrErrorCallback("guFbWriteRegion: invalid combination of x & w\n",FXTRUE);
#endif

  /*
  **  Get the format bits out of the lfbMode register to determine
  **  the size of pixels read back from the frame buffer
  */
  lfbMode = grSstGetConfigFbi(SSTR_LFBMODE);
  lfbFormat = lfbMode & 0xf;    /* Low 4 bits represent format */

  /*  We'll be reading an FxU32 at a time, so determine the pixel
  **  increment by dividing the pixelSize by the size of FxU32
  */
  if (lfbFormat < GR_LFBWRITEMODE_888) {    
    pixelSize = 2;
    incStep = 2;
  } else {
    pixelSize = 4;
    incStep = 1;
  }

  /*
  **  Figure out maxX, so if the pixelSize is 2, we know if we need
  **  to do one more 16-bit store after the loop, as we're doing as
  **  many 32-bit stores as we can
  */
  maxX = dstX + w;
  
  /*
  **  Figure out maximum run of aligned 32-bit words we can store:
  **
  **  Here are the cases:
  **    dstX is odd && width is even && pixelSize == 2:
  **      one 16-bit store
  **      w-2 32-bit stores
  **      one 16-bit store
  **    dstX is odd && width is odd && pixelSize == 2
  **      one 16-bit store
  **      w-1 32-bit stores
  **    dstX is even && width is odd && pixelSize == 2
  **      w-1 32-bit stores
  **      1 16-bit store
  **
  **  Otherwise, it's all aligned and we just go.
  */
  if (pixelSize == 2) {
    if ((dstX & 1) && (!(w & 1))) {
      startRun = dstX + 1;
      maxRun = w - 2;
    } else if ((dstX & 1) && (w & 1)) {
      startRun = dstX + 1;
      maxRun = w - 1;
    } else if ((!(dstX & 1)) && (w & 1))
      startRun = dstX;
      maxRun = w - 1;
  } else
    maxRun = w;
      
  endRun = startRun + maxRun;

  /* Get the lfb Write Address */
  lfbWriteAddr = (FxU32) _SST->lfb_ptr;
  srcLine = 0;
  for (i = dstY; i < dstY + h; i++, srcLine++) {
    lfbPtr = (FxU32 *) (lfbWriteAddr + (i * 1024 * pixelSize));
    lfbPtr = (FxU32 *) ((FxU32) lfbPtr + (dstX * pixelSize));
    srcPtr = (FxU32 *) ((FxU32) src + (srcLine * strideInBytes));
        endPtr = (FxU32 *) ((FxU32) srcPtr + ((endRun - startRun - 1) * pixelSize));
    /*
    **  If pixelSize == 2, then we want to start with a 16-bit store,
    **  then do as many 32-bit stores as we can;
    */
    j = dstX;

    /*
    ** if dstX is odd, and pixelSize == 2, do the first 16-bit store here,
    ** so we can then do aligned 32-bit stores
    */
     if ((j & 1) && pixelSize == 2) {
      *((FxU16 *)lfbPtr) = *((FxU16 *)srcPtr);
      lfbPtr = (FxU32 *) ((FxU32) lfbPtr + 2);
      srcPtr = (FxU32 *) ((FxU32) srcPtr + 2);
      j++;
    }

#if WATCOM_DIDNT_SUCK
    while (j < endRun) {
      *lfbPtr++ = *srcPtr++;
      j += incStep;
    }
#elsif IT_HAD_A_FUCKING_BRAIN 
    loopTop:
    *lfbPtr++ = *srcPtr++;
    j += incStep;
    if (j < endRun)
      goto loopTop;
#else /* too bad, it does suck and doesn't have a brain */
    while (srcPtr < endPtr) 
          *lfbPtr++ = *srcPtr++;
#endif

    /*
    **  if dstX is even, w is odd, and pixelSize == 2, then we still have
    **  one 16-bit pixel left to store
    */
    if (j < dstX + w)
      *((FxU16 *)lfbPtr) = *((FxU16 *)srcPtr);
  }
} /* guFbWriteRegion */
                
                

