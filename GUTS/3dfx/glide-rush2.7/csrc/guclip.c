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
** $Header: /engr/DEVEL/SST1/GLIDE/DOS/SRC/GUCLIP.C 12    6/08/96 4:18p Dow $
** $Log: /engr/DEVEL/SST1/GLIDE/DOS/SRC/GUCLIP.C $
 * 
 * 12    6/08/96 4:18p Dow
 * Changes due to repercussions of hints.
 * 
 * 11    6/05/96 10:38p Dow
 * added guAADrawTriangleWithClip
 * 
 * 10    5/22/96 11:24a Dow
 * moved comparison to pixel center
 * 
 * 9     5/22/96 1:41a Dow
 * fixed guDrawTriangleWithClip
 * 
 * 8     5/09/96 7:05a Paik
 * Glide DLL support
 * 
 * 7     3/25/96 7:58p Paik
 * Move fxglide.h out of glide.h
 * 
 * 6     2/20/96 2:35p Paik
 * DJGPP fix
 * 
 * 2     1/09/96 9:42a Dow
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

static void
calcParams(const GrVertex *a, const GrVertex *b, GrVertex *isect, float d) {
  if (_SST->state.paramIndex & STATE_REQUIRES_IT_DRGB) {
    isect->r = a->r + d * ( b->r - a->r );
    isect->g = a->g + d * ( b->g - a->g );
    isect->b = a->b + d * ( b->b - a->b );
  }
    
  if (_SST->state.paramIndex & STATE_REQUIRES_IT_ALPHA) {
    isect->a        = a->a        + d * ( b->a - a->a );
  }

  if (_SST->state.paramIndex & STATE_REQUIRES_OOZ) {
    isect->ooz = a->ooz + d * ( b->ooz - a->ooz );
  }

  if (_SST->state.paramIndex & STATE_REQUIRES_OOW_FBI) {
    isect->oow = a->oow + d * ( b->oow - a->oow);
  }

  if (_SST->state.paramIndex & STATE_REQUIRES_ST_TMU0) {
    isect->tmuvtx[0].oow =
      a->tmuvtx[0].oow + d * ( b->tmuvtx[0].oow - a->tmuvtx[0].oow );
    isect->tmuvtx[0].sow =
      a->tmuvtx[0].sow + d * ( b->tmuvtx[0].sow - a->tmuvtx[0].sow );
    isect->tmuvtx[0].tow =
      a->tmuvtx[0].tow + d * ( b->tmuvtx[0].tow - a->tmuvtx[0].tow );
  }

  if (_SST->state.paramIndex & STATE_REQUIRES_ST_TMU1) {
    isect->tmuvtx[1].oow =
      a->tmuvtx[1].oow + d * ( b->tmuvtx[1].oow - a->tmuvtx[1].oow );
    isect->tmuvtx[1].sow =
      a->tmuvtx[1].sow + d * ( b->tmuvtx[1].sow - a->tmuvtx[1].sow );
    isect->tmuvtx[1].tow =
      a->tmuvtx[1].tow + d * ( b->tmuvtx[1].tow - a->tmuvtx[1].tow );
  }

#if (GLIDE_NUM_TMU > 2)
  if (_SST->state.paramIndex & STATE_REQUIRES_ST_TMU2) {
    isect->tmuvtx[2].oow =
      a->tmuvtx[2].oow + d * ( b->tmuvtx[2].oow - a->tmuvtx[2].oow );
    isect->tmuvtx[2].sow =
      a->tmuvtx[2].sow + d * ( b->tmuvtx[2].sow - a->tmuvtx[2].sow );
    isect->tmuvtx[2].tow =
      a->tmuvtx[2].tow + d * ( b->tmuvtx[2].tow - a->tmuvtx[2].tow );
  }
#endif

} /* calcParams */

static void 
intersectTop( const GrVertex *a, const GrVertex *b, GrVertex *intersect ) {
  float
    d = ( _SST->state.clipwindowf_ymin - a->y ) / ( b->y - a->y );
  
  intersect->x        = a->x        + d * ( b->x - a->x );
  intersect->y        = _SST->state.clipwindowf_ymin - 1.f;
  
  calcParams(a, b, intersect, d);

} /* intersectTop */

static void
intersectBottom( const GrVertex *a, const GrVertex *b, GrVertex *intersect ) {
  float
    d = ( _SST->state.clipwindowf_ymax - 1.0F - a->y ) / ( b->y - a->y );
  
  intersect->x        = a->x        + d * ( b->x - a->x );
  intersect->y        = _SST->state.clipwindowf_ymax - 1.f;

  calcParams(a, b, intersect, d);

} /* intersectBottom */

static void
intersectRight( const GrVertex *a, const GrVertex *b, GrVertex *intersect ) {
  float
    d = ( _SST->state.clipwindowf_xmax - 1.0F - a->x ) / ( b->x - a->x );
  
  intersect->x        = _SST->state.clipwindowf_xmax - 1.f;
  intersect->y        = a->y        + d * ( b->y - a->y );

  calcParams(a, b, intersect, d);

} /* intersectRight */

static void
intersectLeft( const GrVertex *a, const GrVertex *b, GrVertex *intersect ) {
  float
    d = ( _SST->state.clipwindowf_xmin - a->x ) / ( b->x - a->x );
  
  intersect->x        = _SST->state.clipwindowf_xmin;
  intersect->y        = a->y        + d * ( b->y - a->y );

  calcParams(a, b, intersect, d);

} /* intersectLeft */

static FxBool
aboveYMin(const GrVertex *p) {
  return (( p->y >= _SST->state.clipwindowf_ymin + 0.5f ) ? FXTRUE : FXFALSE);
} /* aboveYMin */
     

static FxBool
belowYMax(const GrVertex *p) {
  return (( p->y < _SST->state.clipwindowf_ymax + 0.5f ) ? FXTRUE : FXFALSE);
} /* belowYMax */

static FxBool
aboveXMin(const GrVertex *p) {
  return (( p->x >= _SST->state.clipwindowf_xmin + 0.5f ) ? FXTRUE : FXFALSE );
} /* aboveXMin */

static FxBool
belowXMax(const GrVertex *p) {
  return (( p->x < _SST->state.clipwindowf_xmax + 0.5f ) ? FXTRUE : FXFALSE );
} /* belowXMax */

/*
** shClipPolygon
*/
static void
shClipPolygon(
              const GrVertex invertexarray[],
              GrVertex outvertexarray[],
              int inlength, int *outlength,
              FxBool (*inside)(const GrVertex *p),
              void (*intersect)(
                                const GrVertex *a,
                                const GrVertex *b,
                                GrVertex *intersect )
              ) {
  GrVertex
    s, p, intersection;
  int
    j;
  
  *outlength = 0;
  
  s = invertexarray[inlength-1];
  for ( j = 0; j < inlength; j++ ) {
    p = invertexarray[j];
    if ( inside( &p ) ) {
      if ( inside( &s ) ) {
        outvertexarray[*outlength] = p;
        (*outlength)++;
      }else {
        intersect( &s, &p, &intersection );
        outvertexarray[*outlength] = intersection;
        (*outlength)++;
        outvertexarray[*outlength] = p;
        (*outlength)++;
      }
    } else {
      if ( inside( &s ) ) {
        intersect( &s, &p, &intersection );
        outvertexarray[*outlength] = intersection;
        (*outlength)++;
      }
    }
    s = p;
  }
} /* shClipPolygon */

/*
** guDrawTriangleWithClip
**
** NOTE:  This routine snaps vertices by adding a large number then
** subtracting that same number again.  In order for this to work
** you MUST set up the FPU to work in single precision mode.  Code
** to perform this is listed in the Appendix to the Glide Programmer's
** Guide.
*/
static const float vertex_snap_constant = ( float ) ( 1L << 19 );

FX_EXPORT void
FX_CSTYLE
guDrawTriangleWithClip(
                       const GrVertex *a,
                       const GrVertex *b,
                       const GrVertex *c ) {
  GrVertex
    output_array[8],
    output_array2[8],
    input_array[3];
  int
    i,
    outlength;

   /*
    ** perform trivial accept
    */
  if (
      ( a->x >= _SST->state.clipwindowf_xmin) &&
      ( a->x < _SST->state.clipwindowf_xmax ) &&
      ( a->y >= _SST->state.clipwindowf_ymin ) &&
      ( a->y < _SST->state.clipwindowf_ymax ) &&
      ( b->x >= _SST->state.clipwindowf_xmin ) &&
      ( b->x < _SST->state.clipwindowf_xmax ) &&
      ( b->y >= _SST->state.clipwindowf_ymin ) &&
      ( b->y < _SST->state.clipwindowf_ymax ) &&
      ( c->x >= _SST->state.clipwindowf_xmin ) &&
      ( c->x < _SST->state.clipwindowf_xmax ) &&
      ( c->y >= _SST->state.clipwindowf_ymin ) &&
      ( c->y < _SST->state.clipwindowf_ymax )
      ) {
    grDrawTriangle( a, b, c );
    return;
  }

  /*
   ** go ahead and clip and render
   */
  input_array[0] = *a;
  input_array[1] = *b;
  input_array[2] = *c;
  
  shClipPolygon( input_array,   output_array,  3,         &outlength, belowXMax, intersectRight );
  shClipPolygon( output_array,  output_array2, outlength, &outlength, belowYMax, intersectBottom );
  shClipPolygon( output_array2, output_array,  outlength, &outlength, aboveXMin, intersectLeft );
  shClipPolygon( output_array,  output_array2, outlength, &outlength, aboveYMin, intersectTop );
  
  /*
   ** snap vertices then decompose the n-gon into triangles
   */
#if defined ( __WATCOMC__ ) || defined ( __MSC__ ) || defined ( __DJGPP__ ) || defined(__GNUC__)
  for ( i = 0; i < outlength; i++ ) {
    output_array2[i].x += vertex_snap_constant;
    output_array2[i].x -= vertex_snap_constant;
    output_array2[i].y += vertex_snap_constant;
    output_array2[i].y -= vertex_snap_constant;
  }
#else
#  error VERTEX SNAPPING MUST BE IMPLEMENTED FOR THIS COMPILER
#endif
  grDrawPolygonVertexList( outlength, output_array2 );
} /* guDrawTriangleWithClip */

FX_EXPORT void
FX_CSTYLE
guAADrawTriangleWithClip(
                         const GrVertex *a,
                         const GrVertex *b,
                         const GrVertex *c ) {
  GrVertex
    output_array[8],
    output_array2[8],
    input_array[3];
  int
    i,
    ilist[10],
    outlength;

   /*
    ** perform trivial accept
    */
  if (
      ( a->x >= _SST->state.clipwindowf_xmin) &&
      ( a->x < _SST->state.clipwindowf_xmax ) &&
      ( a->y >= _SST->state.clipwindowf_ymin ) &&
      ( a->y < _SST->state.clipwindowf_ymax ) &&
      ( b->x >= _SST->state.clipwindowf_xmin ) &&
      ( b->x < _SST->state.clipwindowf_xmax ) &&
      ( b->y >= _SST->state.clipwindowf_ymin ) &&
      ( b->y < _SST->state.clipwindowf_ymax ) &&
      ( c->x >= _SST->state.clipwindowf_xmin ) &&
      ( c->x < _SST->state.clipwindowf_xmax ) &&
      ( c->y >= _SST->state.clipwindowf_ymin ) &&
      ( c->y < _SST->state.clipwindowf_ymax )
      ) {
    grAADrawTriangle( a, b, c, FXTRUE, FXTRUE, FXTRUE );
    return;
  }

  /*
   ** go ahead and clip and render
   */
  input_array[0] = *a;
  input_array[1] = *b;
  input_array[2] = *c;
  
  shClipPolygon( input_array,   output_array,  3,         &outlength, belowXMax, intersectRight );
  shClipPolygon( output_array,  output_array2, outlength, &outlength, belowYMax, intersectBottom );
  shClipPolygon( output_array2, output_array,  outlength, &outlength, aboveXMin, intersectLeft );
  shClipPolygon( output_array,  output_array2, outlength, &outlength, aboveYMin, intersectTop );
  
  /*
   ** snap vertices then decompose the n-gon into triangles
   */
#if defined ( __WATCOMC__ ) || defined ( __MSC__ ) || defined ( __DJGPP__ ) || defined(__GNUC__)
  for ( i = 0; i < outlength; i++ ) {
    output_array2[i].x += vertex_snap_constant;
    output_array2[i].x -= vertex_snap_constant;
    output_array2[i].y += vertex_snap_constant;
    output_array2[i].y -= vertex_snap_constant;
    ilist[i] = i;
  }

  ilist[outlength] = 0;
#else
#  error VERTEX SNAPPING MUST BE IMPLEMENTED FOR THIS COMPILER
#endif
  grAADrawPolygon( outlength, ilist, output_array2 );
} /* guAADrawTriangleWithClip */


FX_EXPORT void
FX_CSTYLE
guDrawPolygonVertexListWithClip( int nverts, const GrVertex vlist[] ) {
  GrVertex
    output_array[GU_PRIM_MAX_VERTICES+8],
    output_array2[GU_PRIM_MAX_VERTICES+8];
   int
     i,
     outlength;

  /*
   ** go ahead and clip and render
   */
  shClipPolygon( vlist, output_array,  nverts, &outlength, belowXMax, intersectRight );
  shClipPolygon( output_array,  output_array2, outlength, &outlength, belowYMax, intersectBottom );
  shClipPolygon( output_array2, output_array,  outlength, &outlength, aboveXMin, intersectLeft );
  shClipPolygon( output_array,  output_array2, outlength, &outlength, aboveYMin, intersectTop );

  /*
   ** snap vertices then decompose the n-gon into triangles
   */
#if defined ( __WATCOMC__ ) || defined ( __MSC__ ) || defined ( __DJGPP__ ) || defined(__GNUC__)
  for ( i = 0; i < outlength; i++ ) {
    output_array2[i].x += vertex_snap_constant;
    output_array2[i].x -= vertex_snap_constant;
    output_array2[i].y += vertex_snap_constant;
    output_array2[i].y -= vertex_snap_constant;
  }
#else
#  error VERTEX SNAPPING MUST BE IMPLEMENTED FOR THIS COMPILER
#endif
  grDrawPolygonVertexList( outlength, output_array2 );
} /* guDrawPolygonVertexListWithClip */

