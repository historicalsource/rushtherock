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
** $Header: /engr/devel/sst1/GLIDE/DOS/SRC/GDRAW.C 58    6/13/96 10:06a Dow $
** $Log: /engr/devel/sst1/GLIDE/DOS/SRC/GDRAW.C $
 * 
 * 58    6/13/96 10:06a Dow
 * Code Cleanup
 * 
 * 57    6/08/96 4:16p Dow
 *      Cleanup of unecessary, never-compiled code
 *      Changes for hints
 * 
 * 56    5/23/96 7:10a Dow
 * Fixed a bad array size.
 * 
 * 55    5/22/96 1:10p Dow
 * Rolled back hints mods, as they screwed things up.
 * 
 * 54    5/22/96 1:39a Dow
 * fixed delta0 mode
 * 
 * 53    5/18/96 9:31p Tarolli
 * changed sanity checks to allow biased numbers
 * expanded ifdef for C version of grDrawTriangle to include the
 * outofbounds check
 * 
 * 52    5/16/96 3:33p Dow
 * fixed error re alt/orig reg map
 * 
 * 51    5/14/96 8:42a Dow
 * made ctrisetup work.
 * 
 * 50    5/14/96 8:13a Dow
 * Added code to support hints.
 * 
 * 49    5/09/96 2:53p Dow
 * better packer bug fix.
 * 
 * 45    4/10/96 3:57p Jt
 * #if'd  bounds checking code from grDrawTrianged per C.Dow's direction.
 * 
 * 44    4/10/96 3:18p Dow
 * fixed points
 * 
 * 43    4/05/96 2:24a Paik
 * Quick fix for alternate register mapping
 * 
 * 42    3/25/96 7:58p Paik
 * Move fxglide.h out of glide.h
 * 
 * 41    3/20/96 8:07a Paik
 * Use single macro for TMUs
 * 
 * 40    3/01/96 1:53p Paik
 * State optimization
 * 
 * 41    3/01/96 3:48a Paik
 * 
 * 40    3/01/96 3:38a Paik
 * 
 * 39    2/28/96 4:42p Dow
 * Minor adjustments
 * 
 * 38    2/06/96 6:33a Dow
 * Changed drawing primitive semantics
 * 
 * 37    2/05/96 11:37a Dow
 * Change draw triangle
 * 
 * 36    1/29/96 10:04p Dow
 * #ifdefed inclusion of tsc.h
 * 
 * 35    1/24/96 2:47p Paik
 * Fix typos in 3 TMU and opt tri setup init
 * 
 * 34    1/22/96 11:05p Paik
 * Fixups for 2.0.a BL0
 * 
 * 33    1/22/96 10:39a Dow
 * Changed decl of asm trisetup stuff
 * 
 * 32    1/19/96 5:12p Paik
 * Hardware bringup modifications
 * 
 * 31    1/14/96 12:29p Dow
 * AAMode stuff & GLIDE_STATE_PAD_SIZE fix
 * 
 * 27    1/10/96 11:12p Dow
 * more 1 & 3 tmu, virtual tmu removal fixes
 * 
 * 26    1/09/96 9:39a Dow
 * tabs->spaces && added Rev. Control keywords
**
*/
#  include <stdio.h>
#include <3dfx.h>

#define FX_DLL_DEFINITION
#include <fxdll.h>

#include <glide.h>
#include "fxglide.h"

#if !defined(__GNUC__)
#include <memory.h>
#endif

#define XY_ONE  1.0F
#define XY_HALF  .5F

#define SST_XY_FRAC_BITS 4
#define SST_XY_HALF      ( 1 << ( SST_XY_FRAC_BITS - 1 ) )
#define SST_XY_ONE       ( 1 << SST_XY_FRAC_BITS )

#define SNAP(a) (a + (float) (1 << 19) - (float) (1 << 19)) 
#define OUTBOUNDSX(a) ((a->x < 0.f ? 1 : 0) || (a->x > _SST->state.screen_width ? 1 : 0))
#define OUTBOUNDSY(a) ((a->y < 0.f ? 1 : 0) || (a->y > _SST->state.screen_height ? 1 : 0))
#define OUTBOUNDS(a) (OUTBOUNDSX(a) || OUTBOUNDSY(a))

float __gdraw_ooarea, __gdraw_area;
float __gdraw_dxAB;
float __gdraw_dyAB;
float __gdraw_dxBC;
float __gdraw_dyBC;

#if ( (GLIDE_PLATFORM & GLIDE_SST_HW) && !defined(GLIDE_DONT_OPTIMIZE_TRISETUP) )

unsigned char
  _interpolantsByParamIndex[(1 << GLIDE_PARMINDEX_BITS)][(1<<GLIDE_PARMINDEX_BITS) + 1];

GrVertexReg 
  _regStruct,
  _regStructByHints[GR_MAX_STW_HINTS],
  _paramRegs, 
  *_paramRegsPtr;

FxU32
  _packerHackAddress;

#endif

extern FxBool aa_line_begin;
float         aa_dady, aa_dadx;
float         aa_start;


#if !defined(GLIDE_DONT_OPTIMIZE_TRISETUP) && ( (GLIDE_PLATFORM & GLIDE_SST_HW) || defined(GLIDE_BENCHMARK) )

/*
** grDrawTriangle
*/
FX_EXPORT void  FX_CSTYLE 
grDrawTriangle(
               const GrVertex *a, const GrVertex *b,
               const GrVertex *c )
{
#ifdef GLIDE_DEBUG
  extern FxU32
    _minMemFIFOFree,
    _minPCIFIFOFree;
  extern FxBool
    _boundsCheck;

  FxU32
    statBits,                   /* bits we care about in status register */
    status;                     /* contents of status register */

  if ( !a || !b || !c )
    GrErrorCallback( "grDrawTriangle:  NULL pointer passed", FXFALSE );

  status = grSstGetConfigFbi(SSTR_STATUS);

  if ((statBits = status & SST_FIFOLEVEL) < _minPCIFIFOFree)
    _minPCIFIFOFree = statBits;

  statBits = (status >> SST_MEMFIFOLEVEL_SHIFT) & 0xffff;
  
  if (statBits < _minMemFIFOFree)
    _minMemFIFOFree = statBits;

  if (_boundsCheck) {
    if (OUTBOUNDS(a) || OUTBOUNDS(b) || OUTBOUNDS(c)) {
      printf("Triangle out of bounds:\n");
      printf("a->x = %3.2f, a->y = %3.2f\n", a->x, a->y);
      printf("b->x = %3.2f, b->y = %3.2f\n", b->x, b->y);
      printf("c->x = %3.2f, c->y = %3.2f\n", c->x, c->y);
      printf("Culling triangle based on these bogus values.\n");
      return;
    }
  }
#endif /* GLIDE_DEBUG */

  if ( _SST->state.cc_delta0mode ) {
    *_regStruct.r = _SST->state.r;
    *((float *) (((FxU32) _regStruct.r) + DPDX_OFFSET)) = 0.f;
    *((float *) (((FxU32) _regStruct.r) + DPDY_OFFSET)) = 0.f;    
    *_regStruct.g = _SST->state.g;
    *((float *) (((FxU32) _regStruct.g) + DPDX_OFFSET)) = 0.f;
    *((float *) (((FxU32) _regStruct.g) + DPDY_OFFSET)) = 0.f;    
    *_regStruct.b = _SST->state.b;
    *((float *) (((FxU32) _regStruct.b) + DPDX_OFFSET)) = 0.f;
    *((float *) (((FxU32) _regStruct.b) + DPDY_OFFSET)) = 0.f;    
  }
  
#ifdef GLIDE_USE_C_TRISETUP
  _trisetup(
            (void *) a, (void *) b, (void *) c, 
            _interpolantsByParamIndex[_SST->state.paramIndex], 
            _SST->state.cull_mode, _SST->state.paramHints
            );
#else
  _trisetup_asm(
                (void *) a, (void *) b, (void *) c, 
                _interpolantsByParamIndex[_SST->state.paramIndex], 
                _SST->state.cull_mode, _SST->state.paramHints
                );
#endif /* GLIDE_USE_C_TRISETUP */
  
}/* grDrawTriangle */


#else  /* GLIDE_DONT_OPTIMIZE... */
# if !defined(__GNUC__)
/*
** _grDrawTriangle
*/
static void
_grDrawTriangle( const GrVertex *a, const GrVertex *b, const GrVertex *c )
{
  const GrVertex *tv;

  /*
  ** sort the vertices
  */
  if ( a->y < b->y ) {
    if ( b->y > c->y) {
      if ( a->y < c->y ) {
         tv = b;
         b = c;
         c = tv;
         __gdraw_area = -__gdraw_area;
      } else {
         tv = a;
         a = c;
         c = b;
         b = tv;
      }
    }
  } else {
    if ( b->y < c->y ) {
      if ( a->y < c->y) {
        tv = a;
        a = b;
        b = tv;
        __gdraw_area = -__gdraw_area;
      } else {
        tv = a;
        a = b;
        b = c;
        c = tv;
      }
    } else {
      tv = a;
      a = c;
      c = tv;
      __gdraw_area = -__gdraw_area;
    }
  }

  /*
  ** write vertices to the hardware
  */
  grSstWriteFloatAll( SSTR_FVAX, a->x );
  grSstWriteFloatAll( SSTR_FVAY, a->y );
  grSstWriteFloatAll( SSTR_FVBX, b->x );
  grSstWriteFloatAll( SSTR_FVBY, b->y );
  grSstWriteFloatAll( SSTR_FVCX, c->x );
  grSstWriteFloatAll( SSTR_FVCY, c->y );

  if ( _SST->state.cc_delta0mode )
  {
    grSstWriteFloatAll( SSTR_FR, _SST->state.r );
    grSstWriteFloatAll( SSTR_FG, _SST->state.g );
    grSstWriteFloatAll( SSTR_FB, _SST->state.b );
  }
  if ( _SST->state.paramIndex & STATE_REQUIRES_IT_DRGB )
  {
    grSstWriteFloatAll( SSTR_FR, a->r );
    grSstWriteFloatAll( SSTR_FG, a->g );
    grSstWriteFloatAll( SSTR_FB, a->b );
  }
  if ( _SST->state.paramIndex & STATE_REQUIRES_IT_ALPHA )
  {
    if ( _SST->state.paramIndex & STATE_REQUIRES_IT_DRGB )
    {
       grSstWriteFloatAll( SSTR_FA, a->a );
    }
    else
    {
       grSstWriteFloatAll( SSTR_FA, _SST->state.a );
    }
  }
  if ( _SST->state.paramIndex & STATE_REQUIRES_OOZ )
  {
    grSstWriteFloatFbi( SSTR_FZ, a->ooz );
  }
  if ( _SST->state.paramIndex & STATE_REQUIRES_STW_TMU0 )
  {
    grSstWriteFloatAll( SSTR_FW, a->tmuvtx[0].oow );
    grSstWriteFloatAll( SSTR_FS, a->tmuvtx[0].sow );
    grSstWriteFloatAll( SSTR_FT, a->tmuvtx[0].tow );
  }
  if ( _SST->state.paramIndex & STATE_REQUIRES_STW_TMU1 )
  {
    grSstWriteFloatTMU( GR_TMU1, SSTR_FW, a->tmuvtx[1].oow );
    grSstWriteFloatTMU( GR_TMU1, SSTR_FS, a->tmuvtx[1].sow );
    grSstWriteFloatTMU( GR_TMU1, SSTR_FT, a->tmuvtx[1].tow );
  }
#if (GLIDE_NUM_TMU > 2)
  if ( _SST->state.paramIndex & STATE_REQUIRES_STW_TMU2 )
  {
    grSstWriteFloatTMU( GR_TMU2, SSTR_FW, a->tmuvtx[2].oow );
    grSstWriteFloatTMU( GR_TMU2, SSTR_FS, a->tmuvtx[2].sow );
    grSstWriteFloatTMU( GR_TMU2, SSTR_FT, a->tmuvtx[2].tow );
  }
#endif /* GLIDE_NUM_TMU > 2 */
  if ( _SST->state.paramIndex & STATE_REQUIRES_OOW_FBI )
  {
    grSstWriteFloatFbi( SSTR_FW, a->oow );
  }
  grSstWriteFloatAll( SSTR_FTRIANGLECMD, __gdraw_area );
}/* _grDrawTriangle */

#endif	/* __GNUC__ */
/*
** _grDrawSetupTriangle
*/
static FxBool 
_grDrawSetupTriangle( const GrVertex *a, const GrVertex *b, const GrVertex *c )
{
#ifdef GLIDE_DEBUG
   if ( ( a->x < -2047 ) ||
        ( a->x >  2047  && a->x < 524288) ||
        ( b->x < -2047 ) ||
        ( b->x >  2047  && b->x < 524288) ||
        ( c->x < -2047 ) ||
        ( c->x >  2047  && c->x < 524288) ||
        ( a->y < -2047 ) ||
        ( a->y >  2047  && a->y < 524288) ||
        ( b->y < -2047 ) ||
        ( b->y >  2047  && b->y < 524288) ||
        ( c->y < -2047 ) ||
        ( c->y >  2047  && c->y < 524288) )
   {
      char buffer[200];

      sprintf( buffer, "grDrawTriangle: bad triangle <%.2f, %.2f> <%.2f, %.2f> <%.2f, %.2f>",
               a->x, a->y,
               b->x, b->y,
               c->x, c->y );
      GrErrorCallback( buffer, FXTRUE );
   }
#endif /* GLIDE_DEBUG */

  /*
  ** Compute area of the triangle
  */
  __gdraw_dxAB = a->x - b->x;
  __gdraw_dyAB = a->y - b->y;
  __gdraw_dxBC = b->x - c->x;
  __gdraw_dyBC = b->y - c->y;
  __gdraw_area = __gdraw_dxAB * __gdraw_dyBC - __gdraw_dxBC * __gdraw_dyAB;

  /*
  ** Backface cull
  */
  if ( __gdraw_area < 0.0F )
  {
    if ( _SST->state.cull_mode == GR_CULL_NEGATIVE )
      return FXFALSE;
  }
  else
  {
    if ( _SST->state.cull_mode == GR_CULL_POSITIVE )
      return FXFALSE;
  }
  if ( __gdraw_area == 0.0F )
    return FXFALSE;

  /*
  ** compute parameter gradients
  */
  __gdraw_ooarea = ( float ) ( XY_ONE / __gdraw_area );

  if ( _SST->state.cc_delta0mode )
  {
    grSstWriteFloatAll( SSTR_FDRDX, 0.0F );
    grSstWriteFloatAll( SSTR_FDRDY, 0.0F );
    grSstWriteFloatAll( SSTR_FDGDX, 0.0F );
    grSstWriteFloatAll( SSTR_FDGDY, 0.0F );
    grSstWriteFloatAll( SSTR_FDBDX, 0.0F );
    grSstWriteFloatAll( SSTR_FDBDY, 0.0F );
  }
  else if ( _SST->state.paramIndex & STATE_REQUIRES_IT_DRGB )
  {
    /*
    ** OPTIMIZATION NOTES:
    **
    ** [BWH] -- I tried precomputing the ( a->p - b->p ) and
    **          ( b->p - c->p ) factors that are shared between
    **          adjacent routines, but this actually slowed down
    **          the code.  My only guess is that the code as it
    **          stands pipelines extremely well and by precomputing
    **          stuff we're only stalling the pipe.
    */
    float drdx, drdy, dgdx, dgdy, dbdx, dbdy;

    drdx = ( ( a->r - b->r ) * __gdraw_dyBC - ( b->r - c->r ) * __gdraw_dyAB ) * __gdraw_ooarea;
    drdy = ( ( b->r - c->r ) * __gdraw_dxAB - ( a->r - b->r ) * __gdraw_dxBC ) * __gdraw_ooarea;
    grSstWriteFloatAll( SSTR_FDRDX, drdx );
    dgdx = ( ( a->g - b->g ) * __gdraw_dyBC - ( b->g - c->g ) * __gdraw_dyAB ) * __gdraw_ooarea;
    grSstWriteFloatAll( SSTR_FDRDY, drdy );
    dgdy = ( ( b->g - c->g ) * __gdraw_dxAB - ( a->g - b->g ) * __gdraw_dxBC ) * __gdraw_ooarea;
    grSstWriteFloatAll( SSTR_FDGDX, dgdx );
    dbdx = ( ( a->b - b->b ) * __gdraw_dyBC - ( b->b - c->b ) * __gdraw_dyAB ) * __gdraw_ooarea;
    grSstWriteFloatAll( SSTR_FDGDY, dgdy );
    dbdy = ( ( b->b - c->b ) * __gdraw_dxAB - ( a->b - b->b ) * __gdraw_dxBC ) * __gdraw_ooarea;

    grSstWriteFloatAll( SSTR_FDBDX, dbdx );
    grSstWriteFloatAll( SSTR_FDBDY, dbdy );
    
  }
  if ( _SST->state.paramIndex & STATE_REQUIRES_IT_ALPHA )
  {
    float dadx, dady;

    dadx = ( ( a->a - b->a ) * __gdraw_dyBC - ( b->a - c->a ) * __gdraw_dyAB ) * __gdraw_ooarea;
    dady = ( ( b->a - c->a ) * __gdraw_dxAB - ( a->a - b->a ) * __gdraw_dxBC ) * __gdraw_ooarea;

    grSstWriteFloatAll( SSTR_FDADX, dadx );
    grSstWriteFloatAll( SSTR_FDADY, dady );
  }
  if ( _SST->state.paramIndex & STATE_REQUIRES_STW_TMU0 )
  {
    float dsow_dx, dsow_dy;
    float dtow_dx, dtow_dy;
    float doow_dx, doow_dy;

    dsow_dx = ( ( a->tmuvtx[0].sow - b->tmuvtx[0].sow ) * __gdraw_dyBC - ( b->tmuvtx[0].sow - c->tmuvtx[0].sow ) * __gdraw_dyAB ) * __gdraw_ooarea;
    dsow_dy = ( ( b->tmuvtx[0].sow - c->tmuvtx[0].sow ) * __gdraw_dxAB - ( a->tmuvtx[0].sow - b->tmuvtx[0].sow ) * __gdraw_dxBC ) * __gdraw_ooarea;
    grSstWriteFloatAll( SSTR_FDSDX, dsow_dx );
    dtow_dx = ( ( a->tmuvtx[0].tow - b->tmuvtx[0].tow ) * __gdraw_dyBC - ( b->tmuvtx[0].tow - c->tmuvtx[0].tow ) * __gdraw_dyAB ) * __gdraw_ooarea;
    grSstWriteFloatAll( SSTR_FDSDY, dsow_dy );
    dtow_dy = ( ( b->tmuvtx[0].tow - c->tmuvtx[0].tow ) * __gdraw_dxAB - ( a->tmuvtx[0].tow - b->tmuvtx[0].tow ) * __gdraw_dxBC ) * __gdraw_ooarea;
    grSstWriteFloatAll( SSTR_FDTDX, dtow_dx );
    doow_dx = ( ( a->tmuvtx[0].oow - b->tmuvtx[0].oow ) * __gdraw_dyBC - ( b->tmuvtx[0].oow - c->tmuvtx[0].oow ) * __gdraw_dyAB ) * __gdraw_ooarea;
    grSstWriteFloatAll( SSTR_FDTDY, dtow_dy );
    doow_dy = ( ( b->tmuvtx[0].oow - c->tmuvtx[0].oow ) * __gdraw_dxAB - ( a->tmuvtx[0].oow - b->tmuvtx[0].oow ) * __gdraw_dxBC ) * __gdraw_ooarea;

    grSstWriteFloatAll( SSTR_FDWDX, doow_dx );
    grSstWriteFloatAll( SSTR_FDWDY, doow_dy );
  }
  if ( _SST->state.paramIndex & STATE_REQUIRES_STW_TMU1 )
  {
    float dsow_dx, dsow_dy;
    float dtow_dx, dtow_dy;
    float doow_dx, doow_dy;

    dsow_dx = ( ( a->tmuvtx[1].sow - b->tmuvtx[1].sow ) * __gdraw_dyBC - ( b->tmuvtx[1].sow - c->tmuvtx[1].sow ) * __gdraw_dyAB ) * __gdraw_ooarea;
    dsow_dy = ( ( b->tmuvtx[1].sow - c->tmuvtx[1].sow ) * __gdraw_dxAB - ( a->tmuvtx[1].sow - b->tmuvtx[1].sow ) * __gdraw_dxBC ) * __gdraw_ooarea;
    grSstWriteFloatTMU( GR_TMU1, SSTR_FDSDX, dsow_dx );
    dtow_dx = ( ( a->tmuvtx[1].tow - b->tmuvtx[1].tow ) * __gdraw_dyBC - ( b->tmuvtx[1].tow - c->tmuvtx[1].tow ) * __gdraw_dyAB ) * __gdraw_ooarea;
    grSstWriteFloatTMU( GR_TMU1, SSTR_FDSDY, dsow_dy );
    dtow_dy = ( ( b->tmuvtx[1].tow - c->tmuvtx[1].tow ) * __gdraw_dxAB - ( a->tmuvtx[1].tow - b->tmuvtx[1].tow ) * __gdraw_dxBC ) * __gdraw_ooarea;
    grSstWriteFloatTMU( GR_TMU1, SSTR_FDTDX, dtow_dx );
    doow_dx = ( ( a->tmuvtx[1].oow - b->tmuvtx[1].oow ) * __gdraw_dyBC - ( b->tmuvtx[1].oow - c->tmuvtx[1].oow ) * __gdraw_dyAB ) * __gdraw_ooarea;
    grSstWriteFloatTMU( GR_TMU1, SSTR_FDTDY, dtow_dy );
    doow_dy = ( ( b->tmuvtx[1].oow - c->tmuvtx[1].oow ) * __gdraw_dxAB - ( a->tmuvtx[1].oow - b->tmuvtx[1].oow ) * __gdraw_dxBC ) * __gdraw_ooarea;

    grSstWriteFloatTMU( GR_TMU1, SSTR_FDWDX, doow_dx );
    grSstWriteFloatTMU( GR_TMU1, SSTR_FDWDY, doow_dy );
  }
#if (GLIDE_NUM_TMU > 2)
  if ( _SST->state.paramIndex & STATE_REQUIRES_STW_TMU2 )
  {
    float dsow_dx, dsow_dy;
    float dtow_dx, dtow_dy;
    float doow_dx, doow_dy;

    dsow_dx = ( ( a->tmuvtx[2].sow - b->tmuvtx[2].sow ) * __gdraw_dyBC - ( b->tmuvtx[2].sow - c->tmuvtx[2].sow ) * __gdraw_dyAB ) * __gdraw_ooarea;
    dsow_dy = ( ( b->tmuvtx[2].sow - c->tmuvtx[2].sow ) * __gdraw_dxAB - ( a->tmuvtx[2].sow - b->tmuvtx[2].sow ) * __gdraw_dxBC ) * __gdraw_ooarea;
    grSstWriteFloatTMU( GR_TMU2, SSTR_FDSDX, dsow_dx );
    dtow_dx = ( ( a->tmuvtx[2].tow - b->tmuvtx[2].tow ) * __gdraw_dyBC - ( b->tmuvtx[2].tow - c->tmuvtx[2].tow ) * __gdraw_dyAB ) * __gdraw_ooarea;
    grSstWriteFloatTMU( GR_TMU2, SSTR_FDSDY, dsow_dy );
    dtow_dy = ( ( b->tmuvtx[2].tow - c->tmuvtx[2].tow ) * __gdraw_dxAB - ( a->tmuvtx[2].tow - b->tmuvtx[2].tow ) * __gdraw_dxBC ) * __gdraw_ooarea;
    grSstWriteFloatTMU( GR_TMU2, SSTR_FDTDX, dtow_dx );
    doow_dx = ( ( a->tmuvtx[2].oow - b->tmuvtx[2].oow ) * __gdraw_dyBC - ( b->tmuvtx[2].oow - c->tmuvtx[2].oow ) * __gdraw_dyAB ) * __gdraw_ooarea;
    grSstWriteFloatTMU( GR_TMU2, SSTR_FDTDY, dtow_dy );
    doow_dy = ( ( b->tmuvtx[2].oow - c->tmuvtx[2].oow ) * __gdraw_dxAB - ( a->tmuvtx[2].oow - b->tmuvtx[2].oow ) * __gdraw_dxBC ) * __gdraw_ooarea;

    grSstWriteFloatTMU( GR_TMU2, SSTR_FDWDX, doow_dx );
    grSstWriteFloatTMU( GR_TMU2, SSTR_FDWDY, doow_dy );
  }
#endif /* GLIDE_NUM_TMU > 2 */
  if ( _SST->state.paramIndex & STATE_REQUIRES_OOZ )
  {
    float dooz_dx, dooz_dy;


    dooz_dx = ( ( a->ooz - b->ooz ) * __gdraw_dyBC - ( b->ooz - c->ooz ) * __gdraw_dyAB ) * __gdraw_ooarea;
    dooz_dy = ( ( b->ooz - c->ooz ) * __gdraw_dxAB - ( a->ooz - b->ooz ) * __gdraw_dxBC ) * __gdraw_ooarea;

    grSstWriteFloatFbi( SSTR_FDZDX, dooz_dx );
    grSstWriteFloatFbi( SSTR_FDZDY, dooz_dy );
  }
  if ( _SST->state.paramIndex & STATE_REQUIRES_OOW_FBI )
  {
    float doow_dx, doow_dy;

    doow_dx = ( ( a->oow - b->oow ) * __gdraw_dyBC - ( b->oow - c->oow ) * __gdraw_dyAB ) * __gdraw_ooarea;
    doow_dy = ( ( b->oow - c->oow ) * __gdraw_dxAB - ( a->oow - b->oow ) * __gdraw_dxBC ) * __gdraw_ooarea;

    grSstWriteFloatFbi( SSTR_FDWDX, doow_dx );
    grSstWriteFloatFbi( SSTR_FDWDY, doow_dy );
  }

  return FXTRUE;
}/* _grDrawSetupTriangle */

/*
** grDrawTriangle
*/
FX_EXPORT void FX_CSTYLE 
grDrawTriangle(
                      const GrVertex *a, const GrVertex *b,
                      const GrVertex *c )
{
#ifdef GLIDE_DEBUG
  extern FxBool
    _boundsCheck;
  
  if ( !a || !b || !c )
    GrErrorCallback( "grDrawTriangle:  NULL pointer passed", FXFALSE );
  
  if (_boundsCheck) {
    if (OUTBOUNDS(a) || OUTBOUNDS(b) || OUTBOUNDS(c)) {
      printf("Triangle out of bounds:\n");
      printf("a->x = %3.2f, a->y = %3.2f\n", a->x, a->y);
      printf("b->x = %3.2f, b->y = %3.2f\n", b->x, b->y);
      printf("c->x = %3.2f, c->y = %3.2f\n", c->x, c->y);
      printf("Culling triangle based on these bogus values.\n");
      return;
    }
  }
#endif /* GLDIE_DEBUG */

  
   if ( _grDrawSetupTriangle( a, b, c ) )
   {
      _grDrawTriangle( a, b, c );
   }
}/* grDrawTriangle */
#endif /* GLIDE_DONT_OPTIMIZE... */

/*
** grDrawPoint
*/
FX_EXPORT void  FX_CSTYLE 
grDrawPoint( const GrVertex *p ) {
 int x = ( int ) ( p->x ); 
 int y = ( int ) ( p->y );

  GrVertex
    v1 = *p, v2 = *p, v3 = *p;

  v1.x = x + .5f;
  v1.y = y + .5f;

  v2.x = x + 1.f;
  v2.y = y + .5f;
 
  v3.x = x + 1.f;
  v3.y = y + 1.f;

  grDrawTriangle(&v1, &v2, &v3);
  
}/* grDrawPoint */

/*
** grDrawPlanarPolygon
**
** Brute force "triangle-fan" implementation of a convex polygon drawer.
*/
FX_EXPORT void  FX_CSTYLE 
grDrawPlanarPolygon( int nverts, const int ilist[], const GrVertex vlist[] )
{
  int i;

  for ( i = 1; i < nverts - 1; i++ )
  {
    grDrawTriangle( &vlist[ilist[0]], &vlist[ilist[i]], &vlist[ilist[i+1]] );
  }
}/* grDrawPlanarPolygon */

/*
** grDrawPolygon
**
** Brute force "triangle-fan" implementation of a convex polygon drawer.
*/
FX_EXPORT void  FX_CSTYLE 
grDrawPolygon( int nverts, const int ilist[], const GrVertex vlist[] )
{
  int i;

  for ( i = 1; i < nverts - 1; i++ )
  {
    grDrawTriangle( &vlist[ilist[0]], &vlist[ilist[i]], &vlist[ilist[i+1]] );
  }
}/* grDrawPolygon */

/*
** grDrawPolygonVertexList
**
** Brute force "triangle-fan" implementation of a convex polygon drawer.
*/
FX_EXPORT void  FX_CSTYLE 
grDrawPolygonVertexList( int nverts, const GrVertex vlist[] )
{
   int i;

   for ( i = 1; i < nverts - 1; i++ )
   {
      grDrawTriangle( &vlist[0], &vlist[i], &vlist[i+1] );
   }
}/* grDrawPolygonVertiexList */

/*
** grDrawLine
*/
FX_EXPORT void  FX_CSTYLE 
grDrawLine( const GrVertex *a, const GrVertex *b )
{
   float           dx, dy;           /* delta X and Y */
   float           adx, ady;         /* |dX| and |dY| */
   GrVertex        c, d, e, f;       /* four vertices outlining the line */
   const GrVertex *tv;
   FxBool          cull_mode = _SST->state.cull_mode;


   /*
   ** disable backface culling
   */
   grCullMode( GR_CULL_DISABLE );

   /*
   ** draw from low Y to high Y
   */
   if ( b->y < a->y )
   {
     tv = a; a = b; b = tv;
   }

   /*
   ** compute deltas and absolute deltas
   */
   dx = adx = b->x - a->x;
   dy = ady = b->y - a->y;
   if ( adx < 0 )
     adx = -adx;
   if ( ady < 0 )
     ady = -ady;

   c = *a;
   d = *a;
   e = *b;
   f = *b;

   /*
   ** X major line
   */
   if ( adx >= ady )
   {
     /*
     ** NOTE: 1. this will not fill the last pixel in line because
     **          B2 or C is on the right edge and the right edge is not
     **          drawn.
     ** (0,0)
     **
     **  A(x1,y1-0.5)+
     **              |  \
     **              |  \  \
     **       (x1,y1)*         \
     **              |  *           \
     **              |     *\           \
     ** B1(x1,y1+0.5)+         *             \
     **                 \        \ *            +B2(x2,y2-0.5)
     **                     \          *        |
     **                        \       \    *   |
     **                             \     \     +  (x2,y2)
     **                                 \       |
     **                                      \  |
     **                                         +C(x2,y2+0.5)
     */
     c.y -= XY_HALF;
     d.y += XY_HALF;
     e.y -= XY_HALF;
     f.y += XY_HALF;
     grDrawTriangle( &c, &d, &f );
     grDrawTriangle( &f, &e, &c );
   }
   /*
   ** Y major line
   */
   else
   {
     c.x -= XY_HALF;
     d.x += XY_HALF;
     e.x -= XY_HALF;
     f.x += XY_HALF;
     grDrawTriangle( &c, &d, &f );
     grDrawTriangle( &f, &e, &c );
   }

   /*
   ** restore backface culling
   */
   grCullMode( cull_mode );
}/* grDrawLine */

#if ((GLIDE_PLATFORM & GLIDE_SST_HW) || defined(GLIDE_BENCHMARK)) && !defined(GLIDE_DONT_OPTIMIZE_TRISETUP)
/*
** This code sets up the arrays passed to _trisetup and _trisetup_asm
** The array regRowSizes simply represents the number of bits set in 
** each value between 0 and 63.  This tells how many registers have to be
** written (one set of registers per kind of interpolant).
*/ 
void
_grInitTriSetupCode(void) {
  int
    hint, i, j;                 /* various array indeces */

  /* Zero out interpolants */
  memset((void *) _interpolantsByParamIndex, 0, sizeof(_interpolantsByParamIndex));
  
  for (i = 0; i < (1 << GLIDE_PARMINDEX_BITS); i++) {
    
    j = 0;
    
    if (i & STATE_REQUIRES_IT_DRGB) {
      _interpolantsByParamIndex[i][j++] = GR_VERTEX_R_OFFSET;
      _interpolantsByParamIndex[i][j++] = GR_VERTEX_G_OFFSET;
      _interpolantsByParamIndex[i][j++] = GR_VERTEX_B_OFFSET;
    }
    
    if (i & STATE_REQUIRES_OOZ)
      _interpolantsByParamIndex[i][j++] = GR_VERTEX_OOZ_OFFSET;
    
    if (i & STATE_REQUIRES_IT_ALPHA)
      _interpolantsByParamIndex[i][j++] = GR_VERTEX_A_OFFSET;
    
    if (i & STATE_REQUIRES_ST_TMU0) {
      _interpolantsByParamIndex[i][j++] = GR_VERTEX_SOW_TMU0_OFFSET;
      _interpolantsByParamIndex[i][j++] = GR_VERTEX_TOW_TMU0_OFFSET;
    }

    if (i & STATE_REQUIRES_OOW_FBI)
      _interpolantsByParamIndex[i][j++] = GR_VERTEX_OOW_OFFSET;
    
    if (i & STATE_REQUIRES_W_TMU0)
      _interpolantsByParamIndex[i][j++] = GR_VERTEX_OOW_TMU0_OFFSET;
    
    if (i & STATE_REQUIRES_ST_TMU1) {
      _interpolantsByParamIndex[i][j++] = GR_VERTEX_SOW_TMU1_OFFSET;
      _interpolantsByParamIndex[i][j++] = GR_VERTEX_TOW_TMU1_OFFSET;
    }

    if (i & STATE_REQUIRES_W_TMU1)
      _interpolantsByParamIndex[i][j++] = GR_VERTEX_OOW_TMU1_OFFSET;
    
#if GLIDE_NUM_TMU > 2
    if (i & STATE_REQUIRES_ST_TMU2) {
      _interpolantsByParamIndex[i][j++] = GR_VERTEX_SOW_TMU2_OFFSET;
      _interpolantsByParamIndex[i][j++] = GR_VERTEX_TOW_TMU2_OFFSET;
    }

    if (i & STATE_REQUIRES_W_TMU2)
      _interpolantsByParamIndex[i][j++] = GR_VERTEX_OOW_TMU2_OFFSET;

#endif /* GLDIE_NUM_TMU > 2 */
  }

    /* Set up the __paramRegs structure */
  for (hint = 0; hint < GR_MAX_STW_HINTS; hint++) {
#ifdef GLIDE_USE_ALT_REGMAP
    FxU32
      temp;
#endif /* GLIDE_USE_ALT_REGMAP */

    _regStructByHints[hint].x =
      (float *) grSstGetAddressAll(SSTR_FVAX); 
    _regStructByHints[hint].y =
      (float *) grSstGetAddressAll(SSTR_FVAY);
    
    _regStructByHints[hint].ooz =
      (float *) grSstGetAddressAll(SSTR_FZ);
    _regStructByHints[hint].oow =
      (float *) grSstGetAddressAll(SSTR_FW);
    
    _regStructByHints[hint].r =
      (float *) grSstGetAddressAll(SSTR_FR);
    _regStructByHints[hint].g =
      (float *) grSstGetAddressAll(SSTR_FG);
    _regStructByHints[hint].b =
      (float *) grSstGetAddressAll(SSTR_FB);
    
    _regStructByHints[hint].a =
      (float *) grSstGetAddressAll(SSTR_FA);
    
    if (hint & GR_STWHINT_W_DIFF_TMU0) 
      _regStructByHints[hint].tmuvtx[0].oow =
        (float *) grSstGetAddressTMU(GR_TMU0, SSTR_FW);
    else 
      _regStructByHints[hint].tmuvtx[0].oow =
        (float *) grSstGetAddressAll(SSTR_FW);
    
    if (hint & GR_STWHINT_ST_DIFF_TMU0) {
      _regStructByHints[hint].tmuvtx[0].sow =
        (float *) grSstGetAddressTMU(GR_TMU0, SSTR_FS);
      _regStructByHints[hint].tmuvtx[0].tow =
        (float *) grSstGetAddressTMU(GR_TMU0, SSTR_FT);
    } else {
      _regStructByHints[hint].tmuvtx[0].sow =
        (float *) grSstGetAddressAll(SSTR_FS);
      _regStructByHints[hint].tmuvtx[0].tow =
        (float *) grSstGetAddressAll(SSTR_FT);
    }
    
    if (hint & GR_STWHINT_W_DIFF_TMU1)
      _regStructByHints[hint].tmuvtx[1].oow =
        (float *) grSstGetAddressTMU(GR_TMU1, SSTR_FW);
    else
      _regStructByHints[hint].tmuvtx[1].oow =
        (float *) grSstGetAddressAll(SSTR_FW);
    
    if (hint & GR_STWHINT_ST_DIFF_TMU1) {
      _regStructByHints[hint].tmuvtx[1].sow =
        (float *) grSstGetAddressTMU(GR_TMU1, SSTR_FS);
      _regStructByHints[hint].tmuvtx[1].tow =
        (float *) grSstGetAddressTMU(GR_TMU1, SSTR_FT);
    } else {
      _regStructByHints[hint].tmuvtx[1].sow =
        (float *) grSstGetAddressAll(SSTR_FS);
      _regStructByHints[hint].tmuvtx[1].tow =
        (float *) grSstGetAddressAll(SSTR_FT);
    }
#if (GLIDE_NUM_TMU > 2)
    if (hint & GR_STWHINT_W_DIFF_TMU2)
      _regStructByHints[hint].tmuvtx[2].oow =
        (float *) grSstGetAddressTMU(GR_TMU2, SSTR_FW);
    else
      _regStructByHints[hint].tmuvtx[2].oow =
        (float *) grSstGetAddressAll(SSTR_FW);
    
    if (hint & GR_STWHINT_ST_DIFF_TMU2) {
      _regStructByHints[hint].tmuvtx[2].sow =
        (float *) grSstGetAddressTMU(GR_TMU2, SSTR_FS);
      _regStructByHints[hint].tmuvtx[2].tow =
        (float *) grSstGetAddressTMU(GR_TMU2, SSTR_FT);
    } else {
      _regStructByHints[hint].tmuvtx[2].sow =
        (float *) grSstGetAddressAll(SSTR_FS);
      _regStructByHints[hint].tmuvtx[2].tow =
        (float *) grSstGetAddressAll(SSTR_FT);
    }
#endif /* GLIDE_NUM_TMU > 2 */
    
#ifdef GLIDE_USE_ALT_REGMAP
    temp = (FxU32) _regStructByHints[hint].x;
    temp += SST_ALT_REGMAP_SELECT;
    _regStructByHints[hint].x = (float *) temp;
    
    temp = (FxU32) _regStructByHints[hint].y;
    temp += SST_ALT_REGMAP_SELECT;
    _regStructByHints[hint].y = (float *) temp;
    
    temp = (FxU32) _regStructByHints[hint].ooz ;
    temp += SST_ALT_REGMAP_SELECT;
    _regStructByHints[hint].ooz  = (float *) temp;
    
    temp = (FxU32) _regStructByHints[hint].oow ;
    temp += SST_ALT_REGMAP_SELECT;
    _regStructByHints[hint].oow  = (float *) temp;
    
    temp = (FxU32) _regStructByHints[hint].r ;
    temp += SST_ALT_REGMAP_SELECT;
    _regStructByHints[hint].r  = (float *) temp;
    
    temp = (FxU32) _regStructByHints[hint].g ;
    temp += SST_ALT_REGMAP_SELECT;
    _regStructByHints[hint].g  = (float *) temp;
    
    temp = (FxU32) _regStructByHints[hint].b ;
    temp += SST_ALT_REGMAP_SELECT;
    _regStructByHints[hint].b  = (float *) temp;
    
    temp = (FxU32) _regStructByHints[hint].a ;
    temp += SST_ALT_REGMAP_SELECT;
    _regStructByHints[hint].a  = (float *) temp;
    
    temp = (FxU32) _regStructByHints[hint].tmuvtx[0].oow ;
    temp += SST_ALT_REGMAP_SELECT;
    _regStructByHints[hint].tmuvtx[0].oow  = (float *) temp;
    
    temp = (FxU32) _regStructByHints[hint].tmuvtx[0].sow ;
    temp += SST_ALT_REGMAP_SELECT;
    _regStructByHints[hint].tmuvtx[0].sow  = (float *) temp;
    
    temp = (FxU32) _regStructByHints[hint].tmuvtx[0].tow ;
    temp += SST_ALT_REGMAP_SELECT;
    _regStructByHints[hint].tmuvtx[0].tow  = (float *) temp;
    
    temp = (FxU32) _regStructByHints[hint].tmuvtx[1].oow ;
    temp += SST_ALT_REGMAP_SELECT;
    _regStructByHints[hint].tmuvtx[1].oow  = (float *) temp;
    
    temp = (FxU32) _regStructByHints[hint].tmuvtx[1].sow ;
    temp += SST_ALT_REGMAP_SELECT;
    _regStructByHints[hint].tmuvtx[1].sow  = (float *) temp;
    
    temp = (FxU32) _regStructByHints[hint].tmuvtx[1].tow ;
    temp += SST_ALT_REGMAP_SELECT;
    _regStructByHints[hint].tmuvtx[1].tow  = (float *) temp;
    
    temp = (FxU32) _regStructByHints[hint].tmuvtx[2].oow ;
    temp += SST_ALT_REGMAP_SELECT;
    _regStructByHints[hint].tmuvtx[2].oow  = (float *) temp;
    
    temp = (FxU32) _regStructByHints[hint].tmuvtx[2].sow ;
    temp += SST_ALT_REGMAP_SELECT;
    _regStructByHints[hint].tmuvtx[2].sow  = (float *) temp;
    
    temp = (FxU32) _regStructByHints[hint].tmuvtx[2].tow ;
    temp += SST_ALT_REGMAP_SELECT;
    _regStructByHints[hint].tmuvtx[2].tow  = (float *) temp;
#endif /* GLIDE_USE_ALT_REGMAP */
  } /* build paramRegs based on hints */ 

  _paramRegs = _regStructByHints[0];
  _regStruct = _regStructByHints[0];

  _packerHackAddress  = ( FxU32 ) _SST->tex_ptr;
  _packerHackAddress  += ( ( ( FxU32 ) 3 ) << 21 );
  _packerHackAddress  += ( ( ( FxU32 ) 1 ) << 17 );

} /* _grInitTriSetupCode */
#endif /* HARDWARE || BENCHMARK && !DONT_OPTIMIZE... */
