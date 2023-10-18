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
** $Header: /engr/devel/sst1/GLIDE/DOS/SRC/GSSTDEF.H 24    5/29/96 9:59p Dow $
** $Log: /engr/devel/sst1/GLIDE/DOS/SRC/GSSTDEF.H $
 * 
 * 24    5/29/96 9:59p Dow
 * got rid of forced broadcast stuff
 * 
 * 23    5/28/96 11:29a Dow
 * turned broadcast back on
 * 
 * 22    5/09/96 7:05a Paik
 * Glide DLL support
 * 
 * 21    3/20/96 8:07a Paik
 * Use single macro for TMUs
 * 
 * 20    2/09/96 4:44p Dow
 * got rid of c++ comment
 * 
 * 19    2/05/96 11:40a Dow
 * Alternat reg mapping stuff
 * 
 * 18    1/22/96 10:40a Dow
 * added alternate register set
 * 
 * 14    1/11/96 9:43a Dow
 * This file now uses sst.h for everything but register definitions.
 * 
 * 13    1/11/96 12:57a Dow
 * Mostly using sst.h for bit definitions.  Still using gsstdef.c for
 * register defs, though.
 * 
 * 12    1/09/96 9:39a Dow
 * tabs->spaces && added Rev. Control keywords
**
*/
#ifndef __GSSTDEF_H__
#define __GSSTDEF_H__

#include <sst.h>

/*----------------- SST chip layout -----------------------*/
typedef enum
{
   SSTR_STATUS,
   SSTR_RESERVED0,
   SSTR_VAX,
   SSTR_VAY,
   SSTR_VBX,
   SSTR_VBY,
   SSTR_VCX,
   SSTR_VCY,
#ifdef GLIDE_USE_ALT_REGMAP
   SSTR_R,
   SSTR_DRDX,
   SSTR_DRDY,

   SSTR_G,
   SSTR_DGDX,
   SSTR_DGDY,

   SSTR_B,
   SSTR_DBDX,
   SSTR_DBDY,

   SSTR_Z,
   SSTR_DZDX,
   SSTR_DZDY,

   SSTR_A,
   SSTR_DADX,
   SSTR_DADY,

   SSTR_S,
   SSTR_DSDX,
   SSTR_DSDY,

   SSTR_T,
   SSTR_DTDX,
   SSTR_DTDY,

   SSTR_W,
   SSTR_DWDX,
   SSTR_DWDY,
#else
   SSTR_R,
   SSTR_G,
   SSTR_B,
   SSTR_Z,
   SSTR_A,
   SSTR_S,
   SSTR_T,
   SSTR_W,

   SSTR_DRDX,
   SSTR_DGDX,
   SSTR_DBDX,
   SSTR_DZDX,
   SSTR_DADX,
   SSTR_DSDX,
   SSTR_DTDX,
   SSTR_DWDX,

   SSTR_DRDY,
   SSTR_DGDY,
   SSTR_DBDY,
   SSTR_DZDY,
   SSTR_DADY,
   SSTR_DSDY,
   SSTR_DTDY,
   SSTR_DWDY,
#endif
   SSTR_TRIANGLECMD,
   SSTR_RESERVED1,

   SSTR_FVAX,
   SSTR_FVAY,
   SSTR_FVBX,
   SSTR_FVBY,
   SSTR_FVCX,
   SSTR_FVCY,
#ifdef GLIDE_USE_ALT_REGMAP
   SSTR_FR,
   SSTR_FDRDX,
   SSTR_FDRDY,
                
   SSTR_FG,
   SSTR_FDGDX,
   SSTR_FDGDY,

   SSTR_FB,
   SSTR_FDBDX,
   SSTR_FDBDY,

   SSTR_FZ,
   SSTR_FDZDX,
   SSTR_FDZDY,

   SSTR_FA,
   SSTR_FDADX,
   SSTR_FDADY,

   SSTR_FS,
   SSTR_FDSDX,
   SSTR_FDSDY,

   SSTR_FT,
   SSTR_FDTDX,
   SSTR_FDTDY,

   SSTR_FW,
   SSTR_FDWDX,
   SSTR_FDWDY,
#else 
   SSTR_FR,
   SSTR_FG,
   SSTR_FB,
   SSTR_FZ,
   SSTR_FA,
   SSTR_FS,
   SSTR_FT,
   SSTR_FW,

   SSTR_FDRDX,
   SSTR_FDGDX,
   SSTR_FDBDX,
   SSTR_FDZDX,
   SSTR_FDADX,
   SSTR_FDSDX,
   SSTR_FDTDX,
   SSTR_FDWDX,

   SSTR_FDRDY,
   SSTR_FDGDY,
   SSTR_FDBDY,
   SSTR_FDZDY,
   SSTR_FDADY,
   SSTR_FDSDY,
   SSTR_FDTDY,
   SSTR_FDWDY,
#endif
   SSTR_FTRIANGLECMD,
   SSTR_FBZCOLORPATH,
   SSTR_FOGMODE,
   SSTR_ALPHAMODE,
   SSTR_FBZMODE,
   SSTR_LFBMODE,
   SSTR_CLIPLEFTRIGHT,
   SSTR_CLIPBOTTOMTOP,

   SSTR_NOPCMD,
   SSTR_FASTFILLCMD,
   SSTR_SWAPBUFFERCMD,
   SSTR_FOGCOLOR,
   SSTR_ZACOLOR,
   SSTR_CHROMAKEY,
   SSTR_RESERVED2,
   SSTR_RESERVED3,

   SSTR_STIPPLE,
   SSTR_C0,
   SSTR_C1,

   SSTR_FBIPIXELSIN,
   SSTR_FBICHROMAFAIL,
   SSTR_FBIZFUNCFAIL,
   SSTR_FBIAFUNCFAIL,
   SSTR_FBIPIXELSOUT,

   SSTR_FOGTABLE,
   SSTR_RESERVED8 = SSTR_FOGTABLE + 32,

   SSTR_FBIINIT4 = SSTR_RESERVED8 + 8,
   SSTR_VRETRACE,
   SSTR_BACKPORCH,
   SSTR_VIDEODIMENSIONS,
   SSTR_FBIINIT0,
   SSTR_FBIINIT1,
   SSTR_FBIINIT2,
   SSTR_FBIINIT3,

   SSTR_HSYNC,
   SSTR_VSYNC,
   SSTR_CLUTDATA,
   SSTR_DACDATA,
   SSTR_MAX_RGB_DELTA,
   SSTR_RESERVED51,

   SSTR_TEXTUREMODE = SSTR_RESERVED51 + 51,
   SSTR_TLOD,
   SSTR_TDETAIL,
   SSTR_TEXBASEADDR,
   SSTR_TEXBASEADDR1,
   SSTR_TEXBASEADDR2,
   SSTR_TEXBASEADDR38,
   SSTR_TEXINIT0,
   SSTR_TEXINIT1,

   SSTR_NCCTABLE0,
   SSTR_NCCTABLE1 = SSTR_NCCTABLE0 + 12,
   SSTR_END_OF_REGISTER_SET
} GrSstRegister;

#ifdef GLIDE_USE_ALT_REGMAP
#define SST_ALT_REGMAP_SELECT BIT(21)
#endif  /* GLIDE_USE_ALT_REGMAP */

#define SST_LFB_OFFSET 0x400000
#define SST_TB_OFFSET  0x800000

#define SST_FBI_ADDR  ( 1L << 8  )
#define SST_TMU0_ADDR ( 1L << 9  )
#define SST_TMU1_ADDR ( 1L << 10 )
#define SST_TMU2_ADDR ( 1L << 11 )

#define SST_CHIP_SELECT_MASK 0x3c00
#define SST_CHIP_SELECT_SHIFT 8
#define SST_CHIP_ADDR(chip) ( 1L << (chip+SST_CHIP_SELECT_SHIFT) )
#define SST_TMU_ADDR(chip)  ( 1L << (chip+SST_CHIP_SELECT_SHIFT+1) )

/* The SST spec has chip nos starting at 0 for fbi and 1 for tmus,
   and I think we should switch to that.  This should insulate any new code. */

#define SST_TMU_INDEX_TO_CHIP(number) ((GrChipID_t)(number))
#define SST_TMU_CHIP_TO_INDEX(chip)   (chip)

#endif

