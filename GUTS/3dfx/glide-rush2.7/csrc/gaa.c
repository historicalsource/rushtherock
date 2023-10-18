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
** $Header: /engr/devel/sst1/GLIDE/DOS/SRC/GAA.C 28    6/15/96 6:23p Dow $
** $Log: /engr/devel/sst1/GLIDE/DOS/SRC/GAA.C $
 * 
 * 28    6/15/96 6:23p Dow
 * oops
 * 
 * 27    6/15/96 5:17p Dow
 * removed grAADrawEdges
 * 
 * 26    6/11/96 7:38p Dow
 * Cleaned up MSVC warnings
 * 
 * 25    6/10/96 12:38p Dow
 * Disabled writing if depth buffer when drawing edge triangles.
 * 
 * 24    6/10/96 12:08p Dow
 * Added grAADrawEdges to help GaryMcT with gsplash.c and made
 * grAADrawTriangle use it.
 * Changed grAADrawPolygon to avoid MSVC warnings
 * Changed grAADrawPolygonVertexList to avoid MSVC warnings
 * Fixed back-faced culling in grAADrawPolygon* routines
 * 
 * 23    6/08/96 4:16p Dow
 *      Changes due to repercussions of adding hints
 * 
 * 22    6/05/96 10:39p Dow
 * added grAADrawPolygonVertexList
 * 
 * 21    5/29/96 9:59p Dow
 * Changed packer hack a bit. got rid of some warnings
 * 
 * 20    5/23/96 12:21p Dow
 * Changed around grAADrawPoints & DrawLines
 * made grAA{Begin,End}{Points,Lines} internal routines.
 * 
 * 19    5/23/96 7:06a Dow
 * Fixed a warning
 * 
 * 18    5/22/96 1:38a Dow
 * got rid of grAADrawFirstTriangle (obviated by making trisetup return a
 * bool), 
 * switched code to use asm trisetup for first triangle,  
 * fixed edge sense caculation for edges of aa tris
 * made grAADrawPolygon work
 * 
 * 17    5/15/96 9:55a Dow
 * Changes to support hints.
 * 
 * 16    5/09/96 2:53p Dow
 * better packer bug fix
 * 
 * 13    5/06/96 4:02p Dow
 * Fixed some warnings and removed a spurious printf.
 * 
 * 12    5/06/96 10:33a Dow
 * Fixed antialiasing.
 * 
 * 11    4/06/96 3:37p Dow
 * fixed some warnings
 * 
 * 10    3/25/96 7:58p Paik
 * Move fxglide.h out of glide.h
 * 
 * 9     3/01/96 2:44p Dow
 * State changes
 * 
 * 9     2/29/96 11:58p Paik
 * 
 * 7     2/28/96 4:47p Dow
 * renaming stuff
 * 
 * 5     2/13/96 2:58p Dow
 * tweaks of graadrawtriangle.  BTW AA tris are WRONG to make them  right,
 * Change the grDrawAAEdges to grAADrawLines
 * 
 * 4     2/11/96 4:21p Dow
 * some (currently ifdef'd out) experimental antialiasing stuff
 * 
 * 3     2/06/96 6:33a Dow
 * Back to original semantics, b/w gr* instead of gu*
 * 
 * 9     1/13/96 2:43p Dow
 * Random fixes, and emacs helping stuff
 * 
 * 8     1/12/96 2:28p Paik
 * [sw-glide/225, sw-glide/242]
 * Executive summary
 * 1. created grAlphaCombine and grColorCombine which fully expose acu and
 * ccu.
 * 2. renamed grColorCombineFunction and grAlphaSource to
 * guColorCombineFunction and guAlphaSource, reimplement on top of
 * grColorCombine and grAlphaCombine.
 * 
 * 4     1/09/96 9:42a Dow
 * tabs -> spaces and added Rev. Control keywords
**
*/
#include <math.h>
#include <stdlib.h>

#include <3dfx.h>
#define FX_DLL_DEFINITION
#include <fxdll.h>
#include <glide.h>
#include "fxglide.h"

/*
** -----------------------------------------------------------------------
**
** Antialiasing utility functions
**
** -----------------------------------------------------------------------
*/

#define XY_ONE 1.f

static GrCullMode_t       aa_cull_mode;

FxBool                    aa_line_begin, aa_point_begin;
GrMipMapId_t              aa_texture;

extern float  aa_dady, aa_dadx;
extern float  aa_start;

static GrAlphaBlendFnc_t
  oldRGBSourceFunction,
  oldRGBDestFunction,
  oldAlphaSourceFunction,
  oldAlphaDestFunction;

static GrState aa_state;


/*
** grAABeginPoints
*/
static void 
grAABeginPoints( void ) {
   grGlideGetState( &aa_state);
   
   guAlphaSource( GR_ALPHASOURCE_ITERATED_ALPHA );
   grAlphaBlendFunction( GR_BLEND_SRC_ALPHA, GR_BLEND_ONE_MINUS_SRC_ALPHA, GR_BLEND_ONE, GR_BLEND_ZERO );

} /* grAABeginPoints */

/*
** grAAEndPoints
*/
static void
grAAEndPoints( void ) {
  grGlideSetState( &aa_state );
} /* grAAEndPoints */

/*
** grAADrawPoint
**
**   |-w-|
**   A---B -
**   |\ /| |
**   | E | h
**   |/ \| |
**   D --C -
**   w = h = 2pixels
*/
FX_EXPORT void FX_CSTYLE
grAADrawPoint( const GrVertex *pt )
{
   GrVertex
     a, b, c, d, e;
   grAABeginPoints();
   /*
   ** make corners of a square
   */
   a = b = c = d = e = *pt;

   a.tmuvtx[0].oow =
   b.tmuvtx[0].oow =
   c.tmuvtx[0].oow =
   d.tmuvtx[0].oow =
   e.tmuvtx[0].oow = 1.0F;

   a.x -= 1.0F;
   a.y -= 1.0F;
   a.a = 0.0F;

   b.x += 1.0F;
   b.y -= 1.0F;
   b.a = 0.0F;

   c.x += 1.0F;
   c.y += 1.0F;
   c.a = 0.0F;
    
   d.x -= 1.0F;
   d.y += 1.0F;
   d.a = 0.0F;

   e.a = 1.0F;
   
   grDrawTriangle(&a, &b, &e);
   grDrawTriangle(&a, &e, &d);
   grDrawTriangle(&b, &e, &c);
   grDrawTriangle(&e, &d, &c);

   /* clean up */
   grAlphaBlendFunction(
                          oldRGBSourceFunction, oldRGBDestFunction,
                          oldAlphaSourceFunction, oldAlphaDestFunction
                        );
   grAAEndPoints();

} /* grAADrawPoint */

/*
** grAABeginLines
*/
static void
grAABeginLines( void ) {
  aa_line_begin = FXTRUE;
  
  aa_cull_mode = _SST->state.cull_mode;

  grGlideGetState( &aa_state );
    
  guAlphaSource( GR_ALPHASOURCE_ITERATED_ALPHA );
  grAlphaBlendFunction( GR_BLEND_SRC_ALPHA, GR_BLEND_ONE_MINUS_SRC_ALPHA, GR_BLEND_ONE, GR_BLEND_ZERO );
  grAlphaTestFunction( GR_CMP_ALWAYS );
  grCullMode( GR_CULL_DISABLE );
  
}/* grAABeginLines */

/*
** grAAEndLines
*/
static void
grAAEndLines( void ) {
  aa_line_begin = FXFALSE;
  
  grCullMode( aa_cull_mode );
} /* grAAEndLines */

/*
** grAADrawLine
*/
FX_EXPORT void FX_CSTYLE 
grAADrawLine( const GrVertex *v1, const GrVertex *v2 ) {
  float           dx, dy;           /* delta X and Y */
  float           adx, ady;         /* |dX| and |dY| */
  GrVertex        a, b, c, d, e, f; /* four vertices outlining the line */
  const GrVertex *tv;
  
  grAABeginLines();

  /* draw from low Y to high Y */
  if ( v2->y < v1->y ) {
    tv = v1; v1 = v2; v2 = tv;
  }
  
  /* compute deltas and absolute deltas */
  dx = adx = v2->x - v1->x;
  dy = ady = v2->y - v1->y;
  if ( adx < 0 )
    adx = -adx;
  if ( ady < 0 )
    ady = -ady;
  
  a = *v1;
  d = *v1;
  e = *v1;
  b = *v2;
  c = *v2;
  f = *v2;
  
  if ( adx >= ady ) {           /* X major line */
    
    float m = 255.0F * dy / dx;
    
    a.y -= XY_ONE;
    a.a = 0.F;
    b.y -= XY_ONE;
    b.a = 0.F;
    e.y += XY_ONE;
    e.a = 0.F;
    f.y += XY_ONE;
    f.a = 0.F;

    aa_dadx  = -m;
    aa_dady  = 255.0F;
    aa_start = 0.0F;

    grDrawTriangle( &a, &c, &b );
    grDrawTriangle( &a, &c, &d );
    
    aa_dadx  = m;
    aa_dady  = -255.0F;
    aa_start = 255.0F;
    grDrawTriangle( &d, &f, &e );
    grDrawTriangle( &d, &c, &f );
    
  } else {                      /* Y major line */
    float m = 255.0F * dx / dy;
    
    a.x += XY_ONE;
    a.a = 0.F;
    b.x += XY_ONE;
    b.a = 0.F;
    e.x -= XY_ONE;
    e.a = 0.F;
    f.x -= XY_ONE;
    f.a = 0.F;
    
    aa_dadx  = -255.0F;
    aa_dady  = m;
    aa_start = 0.0F;
    grDrawTriangle( &a, &b, &c );
    grDrawTriangle( &d, &a, &c );
    
    aa_dadx  = 255.0F;
    aa_dady  = -m;
    aa_start = 0.0F;
    grDrawTriangle( &d, &f, &e );

    aa_start = 255.0F;
    grDrawTriangle( &d, &c, &f );
  }

  grAAEndLines();

}/* grAADrawLine */

typedef enum {
  aaEdgeSenseTop,
  aaEdgeSenseLeft,
  aaEdgeSenseBottom,
  aaEdgeSenseRight
} aaEdgeSense;
  

#ifdef GLIDE_USE_ALT_REGMAP
#define DPDX_OFFSET 0x4
#define DPDY_OFFSET 0x8
#else
#define DPDX_OFFSET 0x20
#define DPDY_OFFSET 0x40
#endif

extern unsigned char
_interpolantsByParamIndex[(1 << GLIDE_PARMINDEX_BITS)][(1<<GLIDE_PARMINDEX_BITS) + 1];
/*
**  grAADrawEdgeTriangle
*/
static void
grAADrawEdgeTriangle(float *va, float *vb, float *vc) {
  register float *fa = va;
  register float *fb = vb;
  register float *fc = vc;
  register float *ptr;
  register float area;
  float     ooa, dxAB, dxBC, dyAB, dyBC;
  register float dpdx;
  register float dpdy;
  extern GrVertex _regStruct;
  float **paramRegsPtr = (float **) &_regStruct;
  
  unsigned  i;
  float negateArea = -1.f;
 
  unsigned char *src;

  src = _interpolantsByParamIndex[_SST->state.paramIndex];

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
  if (area == 0.0f) return;

  ooa = 1.0f / area;

  /*
  ** Divide the deltas by the area for gradient calculation.
  */
  dxBC *= ooa;
  dyAB *= ooa;
  dxAB *= ooa;
  dyBC *= ooa;

  /* write out X & Y  for vertex A */
  grSstWriteFloatAll(SSTR_FVAX, fa[0]);
  grSstWriteFloatAll(SSTR_FVAY, fa[1]);

  /* write out X & Y  for vertex B */
  grSstWriteFloatAll(SSTR_FVBX, fb[0]);
  grSstWriteFloatAll(SSTR_FVBY, fb[1]);

  /* write out X & Y  for vertex C */
  grSstWriteFloatAll(SSTR_FVCX, fc[0]);
  grSstWriteFloatAll(SSTR_FVCY, fc[1]);


  /* Write the alpha data */
  i = GR_VERTEX_A_OFFSET;
  ptr = paramRegsPtr[i];
  *ptr = fa[i];
  
  ptr = (float *)((FxU32)paramRegsPtr[i] + DPDX_OFFSET);
  dpdx = (fa[i] - fb[i]) * dyBC - (fb[i] - fc[i]) * dyAB;
  *ptr = dpdx;
  
  ptr = (float *) ((FxU32) paramRegsPtr[i] + DPDY_OFFSET);
  dpdy = (fb[i] - fc[i]) * dxAB - (fa[i] - fb[i]) * dxBC;
  *ptr = dpdy;

  /* 
  ** The src vector contains offsets from fa, fb, and fc to for which
  **  gradients need to be calculated, and is null-terminated.
  */
  while (i = *src++) {
    PACKERHACK(ptr);

    ptr = paramRegsPtr[i];
    *ptr = (((float *)fa)[i]);
  }
 
  grSstWriteFloatAll( SSTR_FTRIANGLECMD, area);

  return;
} /* grAADrawEdgeTriangle */


/*
**  grAADrawTriEdge
*/
static void
grAADrawTriEdge(const GrVertex *a, const GrVertex *b, aaEdgeSense sense) {
  GrVertex c = *a, d = *b;

#if 0
  static char  src[] = {8, 0};
#endif
  c.a = 0.f;
  d.a = 0.f;

  switch (sense) {
  case aaEdgeSenseTop:
    c.y += XY_ONE;
    d.y += XY_ONE;
    break;

  case aaEdgeSenseLeft:
    c.x -= XY_ONE;
    d.x -= XY_ONE;
    break;

  case aaEdgeSenseBottom:
    c.y -= XY_ONE;
    d.y -= XY_ONE;
    break;

  case aaEdgeSenseRight:
    c.x += XY_ONE;
    d.x += XY_ONE;
    break;
  }

  if (a->y < b->y) {
#if 1 /* def GLIDE_USE_C_TRISETUP */
    grAADrawEdgeTriangle((float *) a, (float *) &c, (float *) &d);
    grAADrawEdgeTriangle((float *) a, (float *) &d, (float *) b);
#else
    _trisetup_asm((float *) a, (float *) &c, (float *) &d, src, GR_CULL_DISABLE);
    _trisetup_asm((float *) a, (float *) &d, (float *) b, src, GR_CULL_DISABLE);
#endif
  } else {
#if 1 /* def GLIDE_USE_C_TRISETUP */
    grAADrawEdgeTriangle((float *) b, (float *) &d, (float *) &c);
    grAADrawEdgeTriangle((float *) b, (float *) &c, (float *) a);
#else
    _trisetup_asm((float *) b, (float *) &d, (float *) &c, src, GR_CULL_DISABLE);
    _trisetup_asm((float *) b, (float *) &c, (float *) a, src, GR_CULL_DISABLE);
#endif    
  }

}/* grAADrawTriEdge */

/*
** aaCalculateSignOfSinTheta
**  
*/
float
aaCalculateSignOfSinTheta(GrVertex *a, GrVertex *b, GrVertex *c) {
  float
    x1, y1, x2, y2;

  /*
     Now, we make two vectors: ab [x1 y1] and ac [x2 y2]
     Fromt the cross product, we know that
     
     x1*y2 - x2*y1 = |ab|*|ac|*sin(theta), where theta is the angle
     between the two lines.  If sin(theta) > 0, c is above the line ab,
     and if it's negative, it's below ab.
     */
  x1 = b->x - a->x;
  y1 = b->y - a->y;
  
  x2 = c->x - a->x;
  y2 = c->y - a->y;

  return ((x1 * y2) - (x2 * y1));
  
} /* aaCalculateSignOfSinTheta */


#define DXSQUARED(i,j) ((i->x - j->x) * (i->x - j->x))
#define DYSQUARED(i,j) ((i->y - j->y) * (i->y - j->y))  
  
/*
**
*/
static aaEdgeSense
aaCalcEdgeSense(const GrVertex *a, const GrVertex *b, const GrVertex *c) {
  float
    yOnLine,                    /* y-ordinate of point on line AB at c->x */
    xOnLine,                    /* x-ordinage of point on line AB at c->y */
    dX,                         /* delta x AC */
    dXSquared, dYSquared,       /* |AB|^2 (x,y) */
    B,                          /* as in y = mx + b */
    slope;                      /* slope of line AB */
  aaEdgeSense
    ret;
  
  dX = a->x - b->x;

  /* Special case undefined slope */
  if (dX == 0.f) {
    if (c->x >  a->x)
      return aaEdgeSenseLeft;
    else
      return aaEdgeSenseRight;
  }
        
  slope = (a->y - b->y) / (a->x - b->x);

  dYSquared = DYSQUARED(a,b);
  dXSquared = DXSQUARED(a,b);

  B = a->y - (slope * a->x);

  yOnLine = (c->x * slope) + B;

	if (slope)
 	 xOnLine = (c->y - B) / slope;
	else xOnLine = (c->y - B) * 1000;

  if (dXSquared >= dYSquared) {  /* X-major line */
    if (yOnLine > c->y)
      ret = aaEdgeSenseTop;
    else
      ret = aaEdgeSenseBottom;
  } else {                      /* Y-major line */
    if (xOnLine < c->x)
      ret = aaEdgeSenseLeft;
    else
      ret = aaEdgeSenseRight;
  }

  return ret;

}/* aaCalcEdgeSense */

/*
** grAADrawTriangle
**
**  NOTE:  This doesn't quite work yet
*/
FX_EXPORT void FX_CSTYLE
grAADrawTriangle(
                 const GrVertex *a,
                 const GrVertex *b,
                 const GrVertex *c,
                 FxBool ab_antialias,
                 FxBool bc_antialias,
                 FxBool ca_antialias
    ) {
  FxU32
    fbzMode,                    /* What we write to fbzMode */
    fbzModeOld =                /* Squirrel away current fbzMode */
      _SST->state.fbi_config.fbzMode;
  aaEdgeSense
    sense;                      /* Edge sense for edge polygons */
  FxBool
    res;
  char *src = _interpolantsByParamIndex[_SST->state.paramIndex];

#ifdef GLIDE_USE_C_TRISETUP
  if ((res =
       _trisetup(&a->x, &b->x, &c->x, src,
                 _SST->state.cull_mode,
                 _SST->state.paramHints)) == FXFALSE)
#else
    if ((res =
         _trisetup_asm(
                       &a->x, &b->x, &c->x, src,
                       _SST->state.cull_mode,
                       _SST->state.paramHints)) == FXFALSE)
#endif
    return;

  /* Disable depth buffer writes for edge triangles */
  fbzMode = fbzModeOld;
  fbzMode &= ~(SST_ZAWRMASK);
  grSstWriteConfigFbi(SSTR_FBZMODE, fbzMode);

  grAABeginLines();             /* This actually sets up what we want for
                                   tris, too. Its name is purely histerical */
  if ( ab_antialias ) {
    sense = aaCalcEdgeSense(a, b, c);
    grAADrawTriEdge(a, b, sense);
  }
  
  if ( bc_antialias ) {
    sense = aaCalcEdgeSense(b, c, a);
    grAADrawTriEdge(b, c, sense);
  }
  
  if ( ca_antialias ) {
    sense = aaCalcEdgeSense(c, a, b);
    grAADrawTriEdge(c, a, sense);
  }
  
  grAAEndLines();

  /* Restore the old fbzMode */
  grSstWriteConfigFbi(SSTR_FBZMODE, fbzModeOld);

} /* grAADrawTriangle */

/*
**  grAADrawPolygon
**  NOTE:  This routine does not do backface culling!!!
*/
FX_EXPORT void FX_CSTYLE 
grAADrawPolygon(const int nverts, const int ilist[], const GrVertex vlist[]) {
  int
    i = 1;                      /* index into ilist */
  const GrVertex
    *a, *b, *c;                 /* What we end up sending */
  aaEdgeSense
    sense;                      /* Edge sense for edge polygons */
  char
    *src = _interpolantsByParamIndex[_SST->state.paramIndex];
  FxBool
    res;

  while (i < nverts - 1) {
    a = &vlist[ilist[0]];
    b = &vlist[ilist[i]];
    c = &vlist[ilist[i+1]];
    
    res = 
#ifdef GLIDE_USE_C_TRISETUP   
    _trisetup(&a->x, &b->x, &c->x, src, _SST->state.cull_mode,
              _SST->state.paramHints); 
#else
    _trisetup_asm(&a->x, &b->x, &c->x, src, _SST->state.cull_mode,
                  _SST->state.paramHints);
#endif
    if (res == FXFALSE)
      return;
    grAABeginLines();
    /*
    **  Always draw the BC edge.
    **  On the first triangle, additionally draw the AB edge.
    **  On the last triangle, draw the CA edge.
    */
    if (i == 1) {
      sense = aaCalcEdgeSense(a, b, c);
      grAADrawTriEdge(a, b, sense);

    } else if (i == (nverts - 2)) {
      sense = aaCalcEdgeSense(c, a, b);
      grAADrawTriEdge(c, a, sense);
    } 

    sense = aaCalcEdgeSense(b, c, a);
    grAADrawTriEdge(b, c, sense);

    ++i;
    grAAEndLines();
  }


} /* grAADrawPolygon */

/*
**  grAADrawPolygonVertexList
**  NOTE:  This routine does not do backface culling!!!
*/
FX_EXPORT void FX_CSTYLE 
grAADrawPolygonVertexList(const int nverts, const GrVertex vlist[]) {
  int
    i = 1;                      /* index into ilist */
  const GrVertex
    *a, *b, *c;                 /* What we end up sending */
  aaEdgeSense
    sense;                      /* Edge sense for edge polygons */
  char
    *src = _interpolantsByParamIndex[_SST->state.paramIndex];
  FxBool
    res;

  while (i < nverts - 1) {
    a = &vlist[0];
    b = &vlist[i];
    c = &vlist[i+1];
    
    res = 
#ifdef GLIDE_USE_C_TRISETUP   
    _trisetup(&a->x, &b->x, &c->x, src, _SST->state.cull_mode,
              _SST->state.paramHints); 
#else
    _trisetup_asm(&a->x, &b->x, &c->x, src, _SST->state.cull_mode,
                  _SST->state.paramHints);
#endif
    if (res == FXFALSE)
      return;

    grAABeginLines();
    /*
    **  Always draw the BC edge.
    **  On the first triangle, additionally draw the AB edge.
    **  On the last triangle, draw the CA edge.
    */
    if (i == 1) {
      sense = aaCalcEdgeSense(a, b, c);
      grAADrawTriEdge(a, b, sense);

    } else if (i == (nverts - 2)) {
      sense = aaCalcEdgeSense(c, a, b);
      grAADrawTriEdge(c, a, sense);
    } 

    sense = aaCalcEdgeSense(b, c, a);
    grAADrawTriEdge(b, c, sense);

    grAAEndLines();

    ++i;
  }

} /* grAADrawPolygonVertexList */
