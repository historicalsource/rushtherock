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
** $Header: /engr/devel/sst1/GLIDE/DOS/SRC/GXDRAW.C 34    6/21/96 7:10a Paik $
** $Log: /engr/devel/sst1/GLIDE/DOS/SRC/GXDRAW.C $
 * 
 * 34    6/21/96 7:10a Paik
 * Correctly cull triangles (I hope)
 * 
 * 33    6/21/96 5:52a Paik
 * Define gxdraw.c to be cdecl
 * 
 * 32    6/11/96 7:40p Dow
 * Cleaned up MSVC warnings
 * Made performance optimization that Miriam suggested, which also made it
 * a better spec for xdraw.asm.
 * 
 * 31    6/08/96 4:19p Dow
 *      Added support for hints
 * 
 * 30    5/29/96 10:00p Dow
 * cleaned up packer hack
 * 
 * 29    5/23/96 10:46a Dow
 * fixed packer bug workaround.
 * 
 * 28    5/22/96 1:42a Dow
 * Moved some constants into fxglide.h, made trisetup return an FxBool
 * 
 * 27    5/14/96 8:42a Dow
 * Made changes reflective of changes in asm code
 * 
 * 26    5/13/96 9:02a Dow
 * Further optimized packer hack.
 * 
 * 25    5/09/96 2:54p Dow
 * better packer bug workaround
 * 
 * 24    5/03/96 10:23a Dow
 * Added better comments, and removed area reversal.
 * 
 * 23    5/03/96 9:30a Dow
 * Fixed triangle sorting and other calculations.
 * 
 * 22    3/25/96 7:58p Paik
 * Move fxglide.h out of glide.h
 * 
 * 21    3/03/96 3:59p Dow
 * Vertex sorting stuff
 * 
 * 20    2/28/96 4:43p Dow
 * Fixed vertex sorting
 * 
 * 19    2/05/96 11:42a Dow
 * alt regmap
 * 
 * 18    1/24/96 3:48a Paik
 * Function is named incorrectly wrt gdraw.c
 * 
 * 17    1/22/96 10:41a Dow
 * changed to reflect changes in the asm trisetup stuff
 * 
 * 16    1/19/96 5:12p Paik
 * Hardware bringup modifications
 * 
 * 15    1/14/96 12:30p Dow
 * AAMode stuff & GLIDE_STATE_PAD_SIZE fix
 * 
 * 14    1/14/96 9:31a Dow
 * 
 * 10    1/09/96 9:42a Dow
 * tabs -> spaces and added Rev. Control keywords
**
*/

#include <stdio.h>

#include <3dfx.h>
#include <glide.h>
#include "fxglide.h"


#if !defined(GLIDE_DONT_OPTIMIZE_TRISETUP) && ( (GLIDE_PLATFORM & GLIDE_SST_HW) || defined(GLIDE_BENCHMARK) )

/*
**  __trisetup
**
**  This routine does all the setup needed for drawing a triangle.  It
**  is intended to be an exact specification for the mechanisim used
**  to pass vertices to the assembly language triangle setup code, and
**  as such has no optimizations at all.  Whenever a 'shortcut'
**  routine to draw triangles (such as for antialiasing, fast lines,
**  fast spans, polygons, etc) is needed, this code should be used as
**  the starting point.
**
**  While the vertex values passed to this routine are actually
**  pointers to grVertex data structures, we treat them as float
**  arrays, because the assembly language code does.
*/  
FxBool
__cdecl
_trisetup(
          const float *va, const float *vb, const float *vc, unsigned
          char *src, GrCullMode_t cullMode, FxU32 paramHints ) {
  register const float *fa = va;
  register const float *fb = vb;
  register const float *fc = vc;
  register float *ptr;
  register float area;
  float     ooa, dxAB, dxBC, dyAB, dyBC;
  unsigned  i;
  float negateArea = 1.f;
/*
**  Structure containing relavent SST1 addresses.  This structure
**  looks just like a grVertex, but contains the SST1 address of the
**  parameter registers.  The dpdx and dpdy offsets from these
**  registers are defined above.
*/ 
  extern GrVertexReg _regStruct;
  float **paramRegsPtr = (float **) &_regStruct;
   
  /*
  **  Sort the vertices.
  **  Whenever the radial order is reversed (from counter-clockwise to
  **  clockwise), we need to change the area of the triangle.  Note
  **  that we know the first two elements are X & Y by looking at the
  **  grVertex structure.  
  */
  if (va[1] < vb[1]) {
    if (vb[1] > vc[1]) {        /* acb */
      if (va[1] < vc[1]) {
        fa = va;
        fb = vc;
        fc = vb;
        negateArea *= -1.f;
      } else {                  /* cab */
        fa = vc;
        fb = va;
        fc = vb;
      }
      /* else it's already sorted */
    }
  } else {
    if (vb[1] < vc[1]) {        /* bac */
      if (va[1] < vc[1]) {
        fa = vb;
        fb = va;
        fc = vc;
        negateArea *= -1.f;
      } else {                  /* bca */
        fa = vb;
        fb = vc;
        fc = va;
      }
    } else {                    /* cba */
      fa = vc;
      fb = vb;
      fc = va;
      negateArea *= -1.f;
    }
  }

  /* Compute Area */
  dxAB = fa[0] - fb[0];
  dxBC = fb[0] - fc[0];

  dyAB = fa[1] - fb[1];
  dyBC = fb[1] - fc[1];

  area = dxAB * dyBC - dxBC * dyAB;

  /* Zero-area triangles are BAD!! */
  if (area == 0.0f)
    return FXFALSE;

  ooa = 1.0f / area;

  /* Backface culling */
  if (cullMode != GR_CULL_DISABLE) {
    if (
      ((cullMode == GR_CULL_NEGATIVE) && (area * negateArea < 0.0)) ||
      ((cullMode == GR_CULL_POSITIVE) && (area * negateArea > 0.0))
    )
      return FXFALSE;
  }

  /*
  ** Divide the deltas by the area for gradient calculation.
  */
  dxBC *= ooa;
  dyAB *= ooa;
  dxAB *= ooa;
  dyBC *= ooa;

  /* Work around the PCI Packer 'feature' */
  PACKERHACK(grSstGetAddressAll(SSTR_FVAX));
    
  /* write out X & Y  for vertex A */
  grSstWriteFloatAll(SSTR_FVAX, fa[0]);
  grSstWriteFloatAll(SSTR_FVAY, fa[1]);

  /* write out X & Y  for vertex B */
  grSstWriteFloatAll(SSTR_FVBX, fb[0]);
  grSstWriteFloatAll(SSTR_FVBY, fb[1]);

  /* write out X & Y  for vertex C */
  grSstWriteFloatAll(SSTR_FVCX, fc[0]);
  grSstWriteFloatAll(SSTR_FVCY, fc[1]);

  /* 
  ** The src vector contains offsets from fa, fb, and fc to for which
  **  gradients need to be calculated, and is null-terminated.
  */
  if (paramHints) {
    while (i = *src++) {
      register float
        dpAB, dpBC,
        dpdx, dpdy;
      FxU32 offset;

      dpAB = (fa[i] - fb[i]);
      dpBC = (fb[i] - fc[i]);

      /* SST registers look like: float *param, float *dpdx, float *dpdy */
      ptr = paramRegsPtr[i];
      
      offset = ((FxU32) ptr) - ((FxU32) _SST->base_ptr);

      /* Hack around the packer 'feature' */
      PACKERHACK(ptr);
      
      *ptr = fa[i];
      
      ptr = (float *)((FxU32)paramRegsPtr[i] + DPDX_OFFSET);
      dpdx = dpAB * dyBC - dpBC * dyAB;
      *ptr = dpdx;
      
      ptr = (float *) ((FxU32) paramRegsPtr[i] + DPDY_OFFSET);
      dpdy = dpBC * dxAB - dpAB * dxBC;
      *ptr = dpdy;
    }
  } else {
    while (i = *src++) {
      FxU32 offset;
      register float  dpdx, dpdy;
      
      /* SST registers look like: float *param, float *dpdx, float *dpdy */
      ptr = paramRegsPtr[i];

      offset = ((FxU32) ptr) - ((FxU32) _SST->base_ptr);

      *ptr = fa[i];
      
      ptr = (float *)((FxU32)paramRegsPtr[i] + DPDX_OFFSET);
      dpdx = (fa[i] - fb[i]) * dyBC - (fb[i] - fc[i]) * dyAB;
      *ptr = dpdx;
      
      ptr = (float *) ((FxU32) paramRegsPtr[i] + DPDY_OFFSET);
      dpdy = (fb[i] - fc[i]) * dxAB - (fa[i] - fb[i]) * dxBC;
      *ptr = dpdy;
    }
  }
 
  /* Draw the triangle by writing the area to the triangleCMD register */
  grSstWriteFloatAll( SSTR_FTRIANGLECMD, area);

  return FXTRUE;
} /* _trisetup */

#endif /* !defined(GLIDE_DONT_OPTIMIZE_TRISETUP) && ( (GLIDE_PLATFORM & GLIDE_SST_HW) || defined(GLIDE_BENCHMARK) ) */

