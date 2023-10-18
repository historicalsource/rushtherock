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
** $Header: /engr/devel/sst1/GLIDE/DOS/SRC/GLFB.C 14    6/12/96 4:16p Dow $
** $Log: /engr/devel/sst1/GLIDE/DOS/SRC/GLFB.C $
 * 
 * 14    6/12/96 4:16p Dow
 * Made grLfbConstanAlpha actually do something
 * 
 * 13    5/09/96 7:04a Paik
 * Glide DLL support
 * 
 * 12    4/16/96 10:21a Tarolli
 * fixed bugs in grLfbBypassMode()
 * 
 * 11    3/25/96 7:58p Paik
 * Move fxglide.h out of glide.h
 * 
 * 10    3/20/96 8:07a Paik
 * Use single macro for TMUs
 * 
 * 9     3/01/96 1:56p Paik
 * State optimization
 * 
 * 12    3/01/96 4:16a Paik
 * 
 * 11    3/01/96 3:48a Paik
 * 
 * 10    2/29/96 11:58p Paik
 * 
 * 9     2/29/96 10:33p Paik
 * 
 * 8     2/09/96 4:42p Dow
 * Added guFbWriteRegion
 * 
 * 4     1/09/96 9:39a Dow
 * tabs->spaces && added Rev. Control keywords
**
*/
#include <3dfx.h>
#define FX_DLL_DEFINITION
#include <fxdll.h>
#include <glide.h>
#include "fxglide.h"

/*
** grLfbBypassMode
*/
FX_EXPORT void FX_CSTYLE 
grLfbBypassMode( GrLfbBypassMode_t mode ) {
   FxU32 lfbmode = _SST->state.fbi_config.lfbMode;

   if ( mode == GR_LFBBYPASS_ENABLE )
   {
      lfbmode &= ~( SST_LFB_ENPIXPIPE );
   }
   else
   {
      lfbmode |= SST_LFB_ENPIXPIPE;
   }

   grSstWriteConfigFbi( SSTR_LFBMODE, lfbmode );

   _SST->state.fbi_config.lfbMode = lfbmode;
} /* grLfbBypassMode */

/*
** grLfbGetReadPtr
*/
FX_EXPORT const FxU32 * FX_CSTYLE 
grLfbGetReadPtr( GrBuffer_t buffer ) {
   FxU32 lfbmode = _SST->state.fbi_config.lfbMode;

   lfbmode &= ~( SST_LFB_READBUFSELECT );

   if ( buffer == GR_BUFFER_FRONTBUFFER )
   {
      lfbmode |= SST_LFB_READFRONTBUFFER;
   }
   else if ( buffer == GR_BUFFER_BACKBUFFER )
   {
      lfbmode |= SST_LFB_READBACKBUFFER;
   }
   else if ( buffer & GR_BUFFER_AUXBUFFER )
   {
      lfbmode |= SST_LFB_READAUXBUFFER;
   }
   #ifdef GLIDE_DEBUG
   else
   {
      GrErrorCallback( "grLfbGetReadPtr:  illegal buffer parameter passed", FXTRUE );
   }
   #endif

   grSstWriteConfigFbi( SSTR_LFBMODE, lfbmode );
   _SST->state.fbi_config.lfbMode = lfbmode;
   return _SST->lfb_ptr;
} /* grLfbGetReadPtr */

/*
** grLfbGetWritePtr
*/
FX_EXPORT void * FX_CSTYLE 
grLfbGetWritePtr( GrBuffer_t buffer ) {
   FxU32 lfbmode = _SST->state.fbi_config.lfbMode;

   lfbmode &= ~( SST_LFB_WRITEBUFSELECT );

   if ( buffer == GR_BUFFER_FRONTBUFFER )
   {
      lfbmode |= SST_LFB_WRITEFRONTBUFFER;
   }
   else if ( buffer == GR_BUFFER_BACKBUFFER )
   {
      lfbmode |= SST_LFB_WRITEBACKBUFFER;
   }
   else if ( buffer & GR_BUFFER_AUXBUFFER )
   {
      lfbmode |= SST_LFB_WRITEAUXBUFFER;
   }
   #ifdef GLIDE_DEBUG
   else
   {
      GrErrorCallback( "grLfbGetWritePtr:  illegal buffer parameter passed", FXTRUE );
   }
   #endif

   grSstWriteConfigFbi( SSTR_LFBMODE, lfbmode );
   _SST->state.fbi_config.lfbMode = lfbmode;

   return _SST->lfb_ptr;
} /* grLfbGetWritePtr */

/*
** grLfbOrigin
*/
FX_EXPORT void FX_CSTYLE 
grLfbOrigin(GrOriginLocation_t origin) {
  FxU32 lfbMode = _SST->state.fbi_config.lfbMode;

  if (origin == GR_ORIGIN_UPPER_LEFT)
    lfbMode &= ~SST_LFB_YORIGIN;
  else
    lfbMode |= SST_LFB_YORIGIN;

  grSstWriteConfigFbi(SSTR_LFBMODE, lfbMode);
  _SST->state.fbi_config.lfbMode = lfbMode;

} /* grLfbOrigin */

/*
** grLfbWriteMode
*/
FX_EXPORT GrLfbWriteMode_t FX_CSTYLE 
grLfbWriteMode( GrLfbWriteMode_t mode ) {
    FxU32 oldmode, lfbmode = _SST->state.fbi_config.lfbMode;
    oldmode = lfbmode;

   lfbmode &= ~( SST_LFB_FORMAT );
   lfbmode |= ( mode << SST_LFB_FORMAT_SHIFT );
   grSstWriteConfigFbi( SSTR_LFBMODE, lfbmode );
   _SST->state.fbi_config.lfbMode = lfbmode;
   return oldmode;
} /* grLfbWriteMOde */

/*
** grLfbConstantAlpha
*/
FX_EXPORT void FX_CSTYLE 
grLfbConstantAlpha( GrAlpha_t alpha ) {
   _SST->state.lfb_constant_alpha = alpha;
} /* grLfbConstantAlpha */

/*
** grLfbConstantDepth
*/
FX_EXPORT void FX_CSTYLE 
grLfbConstantDepth( FxU16 depth ) {
   _SST->state.lfb_constant_depth = depth;
} /* grLfbConstantDepth */

/*
** grLfbBegin
**
** Informs Glide that LFB accesses are desired.  This allows Glide
** set the ZACOLOR_DEPTH register to constant depth and to disable
** depthbiasing (which also uses ZACOLOR_DEPTH).
*/
FX_EXPORT void FX_CSTYLE
grLfbBegin( void ) {
   FxU32 zaColor  = _SST->state.fbi_config.zaColor;
   FxU32 fbzMode = _SST->state.fbi_config.fbzMode;

   /*
   ** Set up the constant depth register because it may have been
   ** trashed by a call to grDepthBiasLevel (depthbiaslevel and
   ** constant depth use the same register)
   */
   zaColor &= ~SST_ZACOLOR_DEPTH;
   zaColor |=
     ((( FxU32 ) _SST->state.lfb_constant_depth ) << SST_ZACOLOR_DEPTH_SHIFT );
   zaColor |=
     (_SST->state.lfb_constant_alpha << SST_ZACOLOR_ALPHA_SHIFT);

   grSstWriteConfigFbi( SSTR_ZACOLOR, zaColor );

   /*
   ** disable depth biasing
   */
   fbzMode &= ~( SST_ENZBIAS );
   grSstWriteConfigFbi( SSTR_FBZMODE, fbzMode );

} /* grLfbBegin */

/*
** grLfbEnd
**
** Informs Glide that LFB accesses are no longer desired.  This allows
** Glide to reset the ZACOLOR_DEPTH to the depth bias level and to
** reenable depthbiasing (which also uses ZACOLOR_DEPTH).
*/
FX_EXPORT void FX_CSTYLE 
grLfbEnd( void ) {
  /*
   ** Restore depth bias level
   */
  grSstWriteConfigFbi( SSTR_ZACOLOR, _SST->state.fbi_config.zaColor );
  
  /*
   ** turn back on depth biasing
   */
  grSstWriteConfigFbi( SSTR_FBZMODE, _SST->state.fbi_config.fbzMode );
} /* grLfbEnd */
