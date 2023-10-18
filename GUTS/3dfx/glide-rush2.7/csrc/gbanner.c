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
** $Header: /engr/devel/SST1/GLIDE/DOS/SRC/GBANNER.C 6     6/16/96 2:10a Jdt $
** $Log: /engr/devel/SST1/GLIDE/DOS/SRC/GBANNER.C $
 * 
 * 6     6/16/96 2:10a Jdt
 * fixed sizing problem
 * 
 * 5     6/15/96 9:32p Garymct
 * Now works with all color formats.
 * 
 * 4     6/15/96 5:37p Garymct
 * Updated gbanner with new image. . . also repositioned.
 * 
 * 3     6/12/96 10:15p Dow
 * Fixed bogus coordinates for shameless plug
**
*/
#include <glide.h>
#include "fxglide.h"
#include "banner.inc"

void grBanner( void )
{
  GrState state;
  volatile FxU32 *fb;
/*static int first_time = 1;*/
  static int not_first_time;
  unsigned char tmp;
  unsigned char sign_data[ sizeof(_sign_data) ];

  memcpy( sign_data, _sign_data, sizeof(sign_data) );

  if( !(not_first_time) )
    {
      long i;

#define COPY(a,b) sign_data[i*4+(a)] = sign_data[i*4+(b)]
#define SWAP(a,b) do { tmp = sign_data[i*4+(a)]; \
                       sign_data[i*4+(a)] = sign_data[i*4+(b)]; \
                       sign_data[i*4+(b)] = tmp;  } while( 0 )
#define ITEM(a) sign_data[i*4+(a)]

      /*
       * ARGB is what the image is stored at. . convert to whatever
       * is the current color format.
       */
      switch( _SST->state.color_format )
        {
        case GR_COLORFORMAT_ARGB:
          /*
           * Do nothing since the banner image is already in this format.
           */
          break;
        case GR_COLORFORMAT_ABGR:
          for( i = 0; i < ( long )sign_height * ( sign_width ); i++ )
            {
              /* ARGB -> ABGR */
              SWAP( 0, 2 );
            }
          break;
        case GR_COLORFORMAT_RGBA:
          for( i = 0; i < ( long )sign_height * ( sign_width ); i++ )
            {
              unsigned char tmp2;
              /* ARGB -> RGBA */
              tmp2 = ITEM( 3 );
              COPY( 3, 2 );
              COPY( 2, 1 );
              COPY( 1, 0 );
              ITEM( 0 ) = tmp2;
            }
          break;
        case GR_COLORFORMAT_BGRA:
          for( i = 0; i < ( long )sign_height * ( sign_width ); i++ )
            {
              /* ARGB -> BGRA */
              SWAP( 0, 3 );
              SWAP( 1, 2 );
            }
          break;
        }
      not_first_time = 1;
    }

  grGlideGetState( &state );
  grDisableAllEffects();
  grAlphaCombine( GR_COMBINE_FUNCTION_SCALE_OTHER,
                  GR_COMBINE_FACTOR_ONE,
                  GR_COMBINE_LOCAL_NONE,
                  GR_COMBINE_OTHER_TEXTURE, FXFALSE );
  grColorCombine( GR_COMBINE_FUNCTION_SCALE_OTHER,
                  GR_COMBINE_FACTOR_ONE,
                  GR_COMBINE_LOCAL_NONE,
                  GR_COMBINE_OTHER_TEXTURE,
                  FXFALSE );
  grAlphaBlendFunction( GR_BLEND_SRC_ALPHA,
                        GR_BLEND_ONE_MINUS_SRC_ALPHA,
                        GR_BLEND_ZERO, GR_BLEND_ZERO );
  grDepthBufferMode( GR_DEPTHBUFFER_DISABLE );
  grSstOrigin(GR_ORIGIN_LOWER_LEFT);
  grLfbBypassMode(GR_LFBBYPASS_DISABLE);
  grLfbWriteMode( GR_LFBWRITEMODE_8888 );
  grLfbBegin();
  fb = ( FxU32 * )grLfbGetWritePtr( GR_BUFFER_BACKBUFFER );
  guFbWriteRegion( (int) (_SST->state.screen_width - sign_width), (int)
                  0, sign_width, sign_height, sign_data,
                   sign_width * 4 );
  grLfbEnd();
  grGlideSetState( &state );
}
