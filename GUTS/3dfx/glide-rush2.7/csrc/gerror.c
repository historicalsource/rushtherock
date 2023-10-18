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
** successor clauses in the FAR, DOD or NASA FAR Supplement. Unpublished  -
** rights reserved under the Copyright Laws of the United States.
**
**
** $Header: /engr/devel/sst1/GLIDE/DOS/SRC/gerror.c 8     5/09/96 7:04a Paik $
** $Log: /engr/devel/sst1/GLIDE/DOS/SRC/gerror.c $
 * 
 * 8     5/09/96 7:04a Paik
 * Glide DLL support
 * 
 * 7     3/25/96 7:58p Paik
 * Move fxglide.h out of glide.h
 * 
 * 3     1/09/96 9:39a Dow
 * tabs->spaces && added Rev. Control keywords
**
*/
#include <stdlib.h>
#include <stdio.h>
#ifdef __DOS__
#  include <malloc.h>
#endif

#include <3dfx.h>
#define FX_DLL_DEFINITION
#include <fxdll.h>
#include <glide.h>
#include "fxglide.h"

void (*GrErrorCallback)( const char *string, FxBool fatal );

FX_EXPORT void
FX_CSTYLE grErrorSetCallback( void (*function) ( const char *string, FxBool fatal ) )
{
  GrErrorCallback = function;
}

void
_grErrorDefaultCallback( const char *s, FxBool fatal )
{
  if ( fatal )
  {
    grGlideShutdown();
    puts( s );
    exit( 1 );
  }
  else
  {
    puts( s );
  }
}

#ifdef __DOS__
int _guHeapCheck( void )
{
  int i = _heapchk();

  if ( i != _HEAPOK )
    printf( "heapchk: %d\n", i );

  return ( i == 0 );
}
#endif
