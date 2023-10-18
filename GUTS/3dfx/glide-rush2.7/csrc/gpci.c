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
** $Header: /engr/devel/sst1/GLIDE/DOS/SRC/GPCI.C 14    3/25/96 7:58p Paik $
** $Log: /engr/devel/sst1/GLIDE/DOS/SRC/GPCI.C $
 * 
 * 14    3/25/96 7:58p Paik
 * Move fxglide.h out of glide.h
 * 
 * 13    1/14/96 6:04p Dow
 * removed unused stuff
 * 
 * 9     1/09/96 9:39a Dow
 * tabs->spaces && added Rev. Control keywords
**
*/
#include <stdlib.h>
#include <string.h>
#include <3dfx.h>
#include <glide.h>
#include "fxglide.h"

/*
** _grPciFindSST
*/
FxU32 *_grPciFindSST( void )
{
#if ( GLIDE_PLATFORM & GLIDE_SST_SIM )
  extern float gw_sw, gw_sh;

  if ( getenv( "GW_RENDER_WIDTH" ) != 0 )
  {
     float sw = ( float ) atof( getenv( "GW_RENDER_WIDTH" ) );
     float sh;

     sh = sw * .75F;

     gw_sw = sw;
     gw_sh = sh;
  }
#endif
  return 0;
}

