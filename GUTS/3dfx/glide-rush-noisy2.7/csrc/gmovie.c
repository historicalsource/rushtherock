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
** $Header: /engr/devel/sst1/GLIDE/DOS/SRC/gmovie.c 9     5/09/96 7:05a Paik $
** $Log: /engr/devel/sst1/GLIDE/DOS/SRC/gmovie.c $
 * 
 * 9     5/09/96 7:05a Paik
 * Glide DLL support
 * 
 * 8     3/25/96 7:58p Paik
 * Move fxglide.h out of glide.h
 * 
 * 4     1/09/96 9:39a Dow
 * tabs->spaces && added Rev. Control keywords
**
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <3dfx.h>
#define FX_DLL_DEFINITION
#include <fxdll.h>
#include <glide.h>
#include "fxglide.h"

#if ( GLIDE_PLATFORM & GLIDE_SST_SIM )

extern void gsim_picsave( int );

FX_EXPORT void
FX_CSTYLE guMovieStart( void )
{
  gsim_picsave( 1 );
}

FX_EXPORT void
FX_CSTYLE guMovieStop( void )
{
  gsim_picsave( 0 );
}

FX_EXPORT void
FX_CSTYLE guMovieSetName( const char *name )
{
   static char buffer[255];
   extern char *picsave;

   strcpy( buffer, name );
   picsave = buffer;
}

#else

FX_EXPORT void
FX_CSTYLE guMovieStart( void )
{
   GrErrorCallback( "guMovieStart:  unsupported on SST hardware", FXFALSE );
}

FX_EXPORT void
FX_CSTYLE guMovieStop( void )
{
   GrErrorCallback( "guMovieStop:  unsupported on SST hardware", FXFALSE );
}

FX_EXPORT void
FX_CSTYLE guMovieSetName( const char *name )
{
   GrErrorCallback( "guMovieSetName:  unsupported on SST hardware", FXFALSE );
}

#endif

