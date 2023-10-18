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
*/

/* Implements multipass drawing */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <3dfx.h>
#define FX_DLL_DEFINITION
#include <fxdll.h>
#include <glide.h>
#include "fxglide.h"
#include "gump.h"

static GrMPState _gumpState;

FX_EXPORT void
FX_CSTYLE guMPInit( void )
{
  int i;

  for (i = 0; i < GLIDE_NUM_VIRTUAL_TMU; i += 1)
  {
    _gumpState.mmid[i] = (GrMipMapId_t) GR_NULL_MIPMAP_HANDLE;
  }
  _gumpState.tc_fnc = GR_MPTEXTURECOMBINE_ADD;
}

FX_EXPORT void
FX_CSTYLE guMPTexCombineFunction( GrMPTextureCombineFnc_t tc_fnc )
{
  _gumpState.tc_fnc = tc_fnc;
}

FX_EXPORT void
FX_CSTYLE guMPTexSource( GrChipID_t virtual_tmu, GrMipMapId_t mmid )
{
#ifdef GLIDE_DEBUG
  if ( virtual_tmu < GR_TMU0 || virtual_tmu >= GR_TMU0 + GLIDE_NUM_VIRTUAL_TMU )
  {
      GrErrorCallback( "guMPTexSource: bogus virtual TMU specified!", FXFALSE );
      return;
  }
#endif

  _gumpState.mmid[virtual_tmu] = mmid;
}

/*
** _gumpTexCombineFunction
**                                                          
** Sets the texture combine function.  For a dual TMU system this function
** will configure the SSTR_TEXTUREMODE registers as appropriate.  For a
** single TMU system this function will configure SSTR_TEXTUREMODE if
** possible, or defer operations until grDrawTriangle() is called.
*/
static void _gumpTexCombineFunction( int virtual_tmu )
{
  FxU32 texmode = _SST->state.tmu_config[0].textureMode;
  GrMPTextureCombineFnc_t tc;

  texmode &= ~( SST_TCOMBINE | SST_TACOMBINE );
  tc = _gumpState.tc_fnc;

  switch ( tc )
  {
  case GR_MPTEXTURECOMBINE_ADD:
    /* tmu0: other + local */
    /* tmu1: local */
    /* pass0(tm0): local */
    /* pass1(tm1): local */
    if ( virtual_tmu == 0 )
    {
      texmode |= ( SST_TC_REPLACE | SST_TCA_REPLACE );
    }
    else
    {
      texmode |= ( SST_TC_REPLACE | SST_TCA_REPLACE );
    }
    break;
  case GR_MPTEXTURECOMBINE_MULTIPLY:
    /* tmu0: other * local */
    /* tmu1: local */
    /* pass0(tm0): local */
    /* pass1(tm1): local */
    if ( virtual_tmu == 0 )
    {
      texmode |= ( SST_TC_REPLACE | SST_TCA_REPLACE );
    }
    else
    {
      texmode |= ( SST_TC_REPLACE | SST_TCA_REPLACE );
    }
    break;
  case GR_MPTEXTURECOMBINE_DETAIL0:
    /* tmu0: (other - local) * lod + local */
    /*     = lod * other + (1 - lod) * local */
    /* tmu1: local */
    /* pass0(tm0): (-local) * lod + local */
    /*           = (1 - lod) * local */
    /* pass1(tm1): (-local) * (1 - lod) + local */
    /*           = lod * local */
    if ( virtual_tmu == 0 )
    {
      texmode |= ( SST_TC_BLEND_LOD | SST_TCA_BLEND_LOD |
		   SST_TC_ZERO_OTHER | SST_TCA_ZERO_OTHER );
    }
    else
    {
      texmode |= ( SST_TC_BLEND_LOD | SST_TCA_BLEND_LOD |
		   SST_TC_REVERSE_BLEND | SST_TCA_REVERSE_BLEND |
		   SST_TC_ZERO_OTHER | SST_TCA_ZERO_OTHER );      
    }
    break;
  case GR_MPTEXTURECOMBINE_DETAIL1:
    /* tmu0: (other - local) * (1 - lod) + local */
    /*     = (1 - lod) * other + lod * local */
    /* tmu1: local */
    /* pass0(tm0): (-local) * (1 - lod) + local */
    /*           = lod * local */
    /* pass1(tm1): (-local) * lod + local */
    /*           = (1 - lod) * local */
    if ( virtual_tmu == 0 )
    {
      texmode |= ( SST_TC_BLEND_LOD | SST_TCA_BLEND_LOD |
		   SST_TC_REVERSE_BLEND | SST_TCA_REVERSE_BLEND |
		   SST_TC_ZERO_OTHER | SST_TCA_ZERO_OTHER );
    }
    else
    {
      texmode |= ( SST_TC_BLEND_LOD | SST_TCA_BLEND_LOD |
		   SST_TC_ZERO_OTHER | SST_TCA_ZERO_OTHER );
    }
    break;
  case GR_MPTEXTURECOMBINE_TRILINEAR0:
    /* tmu0: (other - local) * lodbfrac + local */
    /*     = lodbfrac * other + (1 - lodbfrac) * local */
    /* tmu1: local */
    /* pass0(tm0): (-local) * lodbfrac + local */
    /*           = (1 - lodbfrac) * local */
    /* pass1(tm1): (-local) * (1 - lodbfrac) + local */
    /*           = lodbfrac * local */
    if ( virtual_tmu == 0 )
    {
      texmode |= ( SST_TC_BLEND_LODFRAC | SST_TCA_BLEND_LODFRAC |
		   SST_TC_ZERO_OTHER | SST_TCA_ZERO_OTHER );
    }
    else
    {
      texmode |= ( SST_TC_BLEND_LODFRAC | SST_TCA_BLEND_LODFRAC |
		   SST_TC_REVERSE_BLEND | SST_TCA_REVERSE_BLEND |
		   SST_TC_ZERO_OTHER | SST_TCA_ZERO_OTHER );
    }
    break;
  case GR_MPTEXTURECOMBINE_TRILINEAR1:
    /* tmu0: (other - local) * (1 - lodbfrac) + local */
    /*     = (1 - lodbfrac) * other + lodbfrac * local */
    /* tmu1: local */
    /* pass0(tm0): (-local) * (1 - lodbfrac) + local */
    /*           = lodbfrac * local */
    /* pass1(tm1): (-local) * lodbfrac + local */
    /*           = (1 - lodbfrac) * local */
    if ( virtual_tmu == 0 )
    {
      texmode |= ( SST_TC_BLEND_LODFRAC | SST_TCA_BLEND_LODFRAC |
		  SST_TC_REVERSE_BLEND | SST_TCA_REVERSE_BLEND |
		  SST_TC_ZERO_OTHER | SST_TCA_ZERO_OTHER );
    }
    else
    {
      texmode |= ( SST_TC_BLEND_LODFRAC | SST_TCA_BLEND_LODFRAC |
		   SST_TC_ZERO_OTHER | SST_TCA_ZERO_OTHER );
    }
    break;
  case GR_MPTEXTURECOMBINE_SUBTRACT:
    /* tmu0: other - local */
    /* tmu1: local */
    /* doesn't work, alpha blender can't subtract */
    GrErrorCallback( "_gumpTexCombineFunction:  TEXTURE_SUBTRACT not supported", FXFALSE );
    return;
    break;
    if ( virtual_tmu == 0 )
    {
      texmode |= ( SST_TC_REPLACE | SST_TCA_REPLACE );
    }
    else
    {
      texmode |= ( SST_TC_REPLACE | SST_TCA_REPLACE );
    }
    break;
  default:
    GrErrorCallback( "_gumpTexCombineFunction:  Unsupported function", FXFALSE );
    return;
    break;
  }

  grSstWriteConfigTMU( GR_TMU0, SSTR_TEXTUREMODE, texmode );
}


FX_EXPORT void
FX_CSTYLE guMPDrawTriangle( const GrVertex *a, const GrVertex *b, const GrVertex *c )
{
#ifdef GLIDE_DEBUG
  if ( !a || !b || !c )
    GrErrorCallback( "guDrawTriangleM:  NULL pointer passed", FXFALSE );
#endif

  /* check for multipass texture modes */
  /* xxx complex multipass mode checks should be moved into gtex/gglide */
  /* specific color combine modes use texture */
  /* this is equivalent to
       fbzColorPath & SST_ENTEXTREMAP
       cc_state & STATE_CC_REQUIRES_DECAL_STW */
  /* specific texture combine modes */
  /* xxx don't even check anymore, because you can't */

  {
    /* check for not possible on one tmu modes */
    /* alpha blend enabled (grAlphaBlendMode)
	 xxx some can work, with some cheats, even more
       low bit alpha switches between iterated and constant rgb
	 (grAlphaControlsITRGBLighting)
       alpha test (grAlphaTestFunction)
	 xxx actually, NEVER is ok too
       chromakey
    */

    if ( ( _SST->state.fbi_config.alphaMode & SST_ENALPHABLEND ) ||
	 ( _SST->state.fbi_config.fbzColorPath & SST_LOCALSELECT_OVERRIDE_WITH_ATEX ) ||
	 ( _SST->state.fbi_config.alphaMode & SST_ENALPHAFUNC ) ||
	 ( _SST->state.fbi_config.fbzMode & SST_ENCHROMAKEY )
       )
    {
      GrErrorCallback( "guMPDrawTriangle:  Illegal state", FXFALSE );
    }

    if ( _gumpState.tc_fnc == GR_MPTEXTURECOMBINE_ADD ||
	 _gumpState.tc_fnc == GR_MPTEXTURECOMBINE_DETAIL0 ||
	 _gumpState.tc_fnc == GR_MPTEXTURECOMBINE_DETAIL1 ||
	 _gumpState.tc_fnc == GR_MPTEXTURECOMBINE_TRILINEAR0 ||
	 _gumpState.tc_fnc == GR_MPTEXTURECOMBINE_TRILINEAR1 )
    {
      /* render pass with all effects */
      /* select other texture map */
      /* set alpha blend to add to dest buffer */
      /* if depth buffering, save depth buffer function,
	 set to z-equal */
      /* disable fog and color combine bias */
      /* render other pass */
      /* restore depth buffer function, restore alpha blend, enable fog
	 and color combine bias */

      FxU32 alphamode, alphamode_orig;
      FxU32 fbzcolorpath, fbzcolorpath_orig;
      FxU32 fbzmode, fbzmode_orig;
      FxU32 fogmode, fogmode_orig;

      alphamode = alphamode_orig = _SST->state.fbi_config.alphaMode;
      fbzcolorpath = fbzcolorpath_orig = _SST->state.fbi_config.fbzColorPath;

      /* first pass */

      /* tmu setup */

      guTexSource( _gumpState.mmid[0] );
      _gumpTexCombineFunction( 0 );

      /* render first pass */

      grDrawTriangle( a, b, c );

      /* second pass */

      /* xxx may need to copy texture coordinates, see
	 grTexCombineFunction's second parameter.  if so,
	 we have to disable the second coordinate when drawing. */

      /* tmu setup */

      guTexSource( _gumpState.mmid[1] );
      _gumpTexCombineFunction( 1 );

      /* enable alpha blend to add to destination buffers */

      alphamode &= ~(SST_RGBSRCFACT | SST_RGBDSTFACT | SST_ASRCFACT | SST_ADSTFACT);
      alphamode |= SST_ENALPHABLEND |
		   (SST_A_ONE << SST_RGBSRCFACT_SHIFT) |
		   (SST_A_ONE << SST_RGBDSTFACT_SHIFT) |
		   (SST_A_ONE << SST_ASRCFACT_SHIFT) |
		   (SST_A_ONE << SST_ADSTFACT_SHIFT);

      grSstWriteConfigFbi( SSTR_ALPHAMODE, alphamode );

      /* if depth buffering, set to z= mode and disable writes */

      if ( _SST->state.fbi_config.fbzMode & SST_ENDEPTHBUFFER )
      {
	fbzmode = fbzmode_orig = _SST->state.fbi_config.fbzMode;

	fbzmode &= ~(SST_ZAWRMASK | SST_ZFUNC);
	fbzmode |= GR_CMP_EQUAL;

	grSstWriteConfigFbi( SSTR_FBZMODE, fbzmode );
      }

      /* disable (fogta * fogColor) bias */
      /* xxx setting ADD_FOG with fog disabled is harmless */

      if ( _SST->state.fbi_config.fogMode & SST_ENFOGGING )
      {
	fogmode = fogmode_orig = _SST->state.fbi_config.fogMode;

	fogmode |= SST_FOGADD;

	grSstWriteConfigFbi( SSTR_FOGMODE, fogmode );
      }

      /* disable biasing in color combine */
      /* this can change the parameters output */
      /* xxx the equivalent of GR_COLORCOMBINE_CCRGB_BLEND_ITRGB_ON_TEXALPHA
	     does not work, you need to do this instead of below.

      if ( _SST->state.cc_fnc == GR_COLORCOMBINE_CCRGB_BLEND_ITRGB_ON_TEXALPHA )
      {
	fbzcolorpath &= ~SST_CC_ADD_CLOCAL;
      }
      */

      fbzcolorpath &= ~(SST_CC_ADD_CLOCAL | SST_CC_ADD_ALOCAL | SST_CC_SUB_CLOCAL);
      
      grSstWriteConfigFbi( SSTR_FBZCOLORPATH, fbzcolorpath );

      /* render other pass */

      grDrawTriangle( a, b, c );

      /* restore */

      /* restore alpha blending state */

      grSstWriteConfigFbi( SSTR_ALPHAMODE, alphamode_orig );

      /* restore depth buffer state */

      if ( _SST->state.fbi_config.fbzMode & SST_ENDEPTHBUFFER )
      {
	grSstWriteConfigFbi( SSTR_FBZMODE, fbzmode_orig );
      }

      /* restore fog state */

      if ( _SST->state.fbi_config.fogMode & SST_ENFOGGING )
      {
	grSstWriteConfigFbi( SSTR_FOGMODE, fogmode_orig );
      }

      /* restore ccu/acu state */

      grSstWriteConfigFbi( SSTR_FBZCOLORPATH, fbzcolorpath_orig );

      return;
    }

    else if ( _gumpState.tc_fnc == GR_MPTEXTURECOMBINE_MULTIPLY )
    {
      /* disable fog and color combine bias */
      /* render pass with all other effects */
      /* select other texture map */
      /* set alpha blend to multiply to dest buffer */
      /* if depth buffering, save depth buffer function,
	 set to z-equal */
      /* disable fog and color combine factor */
      /* render other pass */
      /* if fog or color combine w/bias */
	/* disable tmu */
	/* set alpha blend to add to dest buffer */
	/* enable fog and color combine bias */
	/* render bias pass */
      /* restore depth buffer function, restore alpha blend, enable fog
	 and color combine bias */

      FxU32 alphamode, alphamode_orig;
      FxU32 fbzcolorpath, fbzcolorpath_orig;
      FxU32 fbzmode, fbzmode_orig;
      FxU32 fogmode, fogmode_orig;

      alphamode = alphamode_orig = _SST->state.fbi_config.alphaMode;
      fbzcolorpath = fbzcolorpath_orig = _SST->state.fbi_config.fbzColorPath;

      /* first pass */

      /* tmu setup */

      guTexSource( _gumpState.mmid[0] );
      _gumpTexCombineFunction( 0 );

      /* disable bias */

      /* disable (fogta * fogColor) bias */
      /* xxx setting ADD_FOG with fog disabled is harmless */

      if ( _SST->state.fbi_config.fogMode & SST_ENFOGGING )
      {
	fogmode = fogmode_orig = _SST->state.fbi_config.fogMode;

	fogmode |= SST_FOGADD;

	grSstWriteConfigFbi( SSTR_FOGMODE, fogmode );
      }

      /* disable biasing in color combine */
      /* this can change the parameters output */
      /* xxx consult add path for switch version */
      /* xxx the equivalent of GR_COLORCOMBINE_CCRGB_BLEND_ITRGB_ON_TEXALPHA
	     does not work, you need to do this instead of below.

      if ( _SST->state.cc_fnc == GR_COLORCOMBINE_CCRGB_BLEND_ITRGB_ON_TEXALPHA )
      {
	fbzcolorpath &= ~SST_CC_ADD_CLOCAL;
      }
      */

      fbzcolorpath &= ~(SST_CC_ADD_CLOCAL | SST_CC_ADD_ALOCAL | SST_CC_SUB_CLOCAL);

      grSstWriteConfigFbi( SSTR_FBZCOLORPATH, fbzcolorpath );

      /* render first pass */

      grDrawTriangle( a, b, c );

      /* second pass */

      /* xxx may sometimes need to copy texture coordinates */

      /* tmu setup */

      guTexSource( _gumpState.mmid[1] );
      _gumpTexCombineFunction( 1 );

      /* enable alpha blend to multiply to destination buffers */
      /* xxx alpha component blender can only handle factors of
	 0 and 1 */

      alphamode &= ~(SST_RGBSRCFACT | SST_RGBDSTFACT | SST_ASRCFACT | SST_ADSTFACT);
      alphamode |= SST_ENALPHABLEND |
		   (SST_A_COLOR << SST_RGBSRCFACT_SHIFT) |
		   (SST_A_ZERO << SST_RGBDSTFACT_SHIFT) |
		   (SST_A_ONE << SST_ASRCFACT_SHIFT) |
		   (SST_A_ZERO << SST_ADSTFACT_SHIFT);

      grSstWriteConfigFbi( SSTR_ALPHAMODE, alphamode );

      /* if depth buffering, set to z= mode and disable writes */

      if ( _SST->state.fbi_config.fbzMode & SST_ENDEPTHBUFFER )
      {
	fbzmode = fbzmode_orig = _SST->state.fbi_config.fbzMode;

	fbzmode &= ~(SST_ZAWRMASK | SST_ZFUNC);
	fbzmode |= GR_CMP_EQUAL;

	grSstWriteConfigFbi( SSTR_FBZMODE, fbzmode );
      }

      /* disable fog */

      if ( _SST->state.fbi_config.fogMode & SST_ENFOGGING )
      {
	grSstWriteConfigFbi( SSTR_FOGMODE, 0 );
      }

      /* disable factor and bias in color combine--
	 decal, except for the cases that need texture alpha */
      /* this can change the parameters output */

      fbzcolorpath  = fbzcolorpath_orig;
      fbzcolorpath &= ~( SST_RGBSELECT |
			 SST_LOCALSELECT |
			 SST_CC_ZERO_OTHER |
			 SST_CC_SUB_CLOCAL |
			 SST_CC_MSELECT |
			 SST_CC_REVERSE_BLEND |
			 SST_CC_ADD_CLOCAL |
			 SST_CC_ADD_ALOCAL |
			 SST_CC_INVERT_OUTPUT |
			 SST_CC_REVERSE_BLEND );

      /* xxx the equivalent of GR_COLORCOMBINE_CCRGB_BLEND_ITRGB_ON_TEXALPHA
	     does not work, you need to do this instead of below.

      if ( _SST->state.cc_fnc == GR_COLORCOMBINE_CCRGB_BLEND_ITRGB_ON_TEXALPHA )
      {
	xxx see implementation notes on why this isn't implemented yet
	GrErrorCallback( "guDrawTriangleMP:  MULTIPLY and BLEND_ITRGB_ON_TEXALPHA not implemented yet!", FXFALSE );
	return;
      }
      */

      fbzcolorpath |= SST_RGBSEL_TREXOUT;

      grSstWriteConfigFbi( SSTR_FBZCOLORPATH, fbzcolorpath );

      /* render second pass */

      grDrawTriangle( a, b, c );

      /* if bias, third pass */

      if ( ( _SST->state.fbi_config.fogMode & SST_ENFOGGING )
      /* xxxXXXxxx it isn't very easy to check following any more!
	   ||
	   _SST->state.cc_fnc == GR_COLORCOMBINE_TEXTURE_TIMES_ITRGB_ADD_ITALPHA ||
	   _SST->state.cc_fnc == GR_COLORCOMBINE_TEXTURE_TIMES_ITALPHA_ADD_ITRGB ||
	   _SST->state.cc_fnc == GR_COLORCOMBINE_TEXTURE_ADD_ITRGB ||
	   _SST->state.cc_fnc == GR_COLORCOMBINE_TEXTURE_SUB_ITRGB ||
	   _SST->state.cc_fnc == GR_COLORCOMBINE_CCRGB_BLEND_ITRGB_ON_TEXALPHA ||
	   _SST->state.cc_fnc == GR_COLORCOMBINE_DIFF_SPEC_A ||
	   _SST->state.cc_fnc == GR_COLORCOMBINE_DIFF_SPEC_B */ )
      {
	/* enable alpha blend to add to destination buffers */

	alphamode &= ~(SST_RGBSRCFACT | SST_RGBDSTFACT | SST_ASRCFACT | SST_ADSTFACT);
	alphamode |= SST_ENALPHABLEND |
		     (SST_A_ONE << SST_RGBSRCFACT_SHIFT) |
		     (SST_A_ONE << SST_RGBDSTFACT_SHIFT) |
		     (SST_A_ONE << SST_ASRCFACT_SHIFT) |
		     (SST_A_ONE << SST_ADSTFACT_SHIFT);

	grSstWriteConfigFbi( SSTR_ALPHAMODE, alphamode );

	/* disable fog factor, leave fog bias enabled */
	/* xxx setting ADD_MULT with fog disabled is harmless */

	if ( _SST->state.fbi_config.fogMode & SST_ENFOGGING )
	{
	  fogmode = fogmode_orig;

	  fogmode |= SST_FOGMULT;

	  grSstWriteConfigFbi( SSTR_FOGMODE, fogmode );
	}

	/* disable factor in color combine, enable bias */
	/* this can change the parameters output */

	fbzcolorpath  = fbzcolorpath_orig;
	fbzcolorpath &= ~( SST_ENTEXTUREMAP |
			   SST_RGBSELECT |
			   SST_LOCALSELECT |
			   SST_CC_ZERO_OTHER |
			   SST_CC_SUB_CLOCAL |
			   SST_CC_MSELECT |
			   SST_CC_REVERSE_BLEND |
			   SST_CC_ADD_CLOCAL |
			   SST_CC_ADD_ALOCAL |
			   SST_CC_INVERT_OUTPUT |
			   SST_CC_REVERSE_BLEND );

#if 0
	/* xxx the following doesn't work anymore! */

	switch ( _SST->state.cc_fnc )
	{
	case GR_COLORCOMBINE_TEXTURE_TIMES_ITRGB_ADD_ITALPHA:
	  fbzcolorpath |= SST_CC_ADD_ALOCAL | SST_CC_ZERO_OTHER;
	  break;

	  /* unimplemented in Glide */
	case GR_COLORCOMBINE_TEXTURE_TIMES_ITALPHA_ADD_ITRGB:
	  fbzcolorpath |= SST_CC_ADD_CLOCAL | SST_CC_ZERO_OTHER;
	  break;

	case GR_COLORCOMBINE_TEXTURE_ADD_ITRGB:
	  fbzcolorpath |= SST_CC_ADD_CLOCAL | SST_CC_ZERO_OTHER;
	  break;

	case GR_COLORCOMBINE_TEXTURE_SUB_ITRGB:
	  /* Can't do this on SST1, can't subtract in alpha blender */
	  GrErrorCallback( "guDrawTriangleM:  GR_COLORCOMBINE_TEXTURE_SUB_ITRGB does not work with GR_MPTEXTURECOMBINE_MULTIPLY.", FXFALSE );
	  fbzcolorpath |= SST_CC_ZERO_OTHER;
	  break;

	case GR_COLORCOMBINE_CCRGB_BLEND_ITRGB_ON_TEXALPHA:
	  fbzcolorpath |= SST_CC_ZERO_OTHER;
	  break;

	case GR_COLORCOMBINE_DIFF_SPEC_A:
	  fbzcolorpath |= SST_CC_ADD_CLOCAL;
	  break;

	case GR_COLORCOMBINE_DIFF_SPEC_B:
	  fbzcolorpath |= SST_CC_ADD_ALOCAL;
	  break;
	}
#endif

	/* render third pass */

	grDrawTriangle( a, b, c );
      }

      /* restore alpha blending state */

      grSstWriteConfigFbi( SSTR_ALPHAMODE, alphamode_orig );

      /* restore depth buffer state */

      if ( _SST->state.fbi_config.fbzMode & SST_ENDEPTHBUFFER )
      {
	grSstWriteConfigFbi( SSTR_FBZMODE, fbzmode_orig );
      }

      /* restore fog state */

      if ( _SST->state.fbi_config.fogMode & SST_ENFOGGING )
      {
	grSstWriteConfigFbi( SSTR_FOGMODE, fogmode_orig );
      }

      /* restore ccu/acu state */

      grSstWriteConfigFbi( SSTR_FBZCOLORPATH, fbzcolorpath_orig );

      return;
    }

    else if ( _gumpState.tc_fnc == GR_MPTEXTURECOMBINE_SUBTRACT )
    {
      GrErrorCallback( "gumpDrawTriangle:  GR_MPTEXCOMBINE_SUBTRACT not implemented", FXFALSE );
      return;
    }
  }
}




