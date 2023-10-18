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
** $Header: /engr/devel/sst1/GLIDE/DOS/SRC/G3DF.C 12    6/10/96 12:29a Jdt $
** $Log: /engr/devel/sst1/GLIDE/DOS/SRC/G3DF.C $
 * 
 * 12    6/10/96 12:29a Jdt
 * Added ReadDataLong()
 * Added support for P8 and AP88 formats
 * Reconciled with changes to Gu3DfInfo structure
 * 
 * 11    5/20/96 11:55a Dow
 * Fixed up NCC texture stuff.
 * 
 * 10    5/09/96 7:04a Paik
 * Glide DLL support
 * 
 * 9     3/25/96 7:58p Paik
 * Move fxglide.h out of glide.h
 * 
 * 5     1/09/96 9:39a Dow
 * tabs->spaces && added Rev. Control keywords
**
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef getc
# undef getc	/* use the libc version */
#endif

#include <3dfx.h>
#define FX_DLL_DEFINITION
#include <fxdll.h>
#include <glide.h>
#include "fxglide.h"

extern const int atable[GR_ASPECT_1x8+1][GR_LOD_1+1][2];

static FxU16 ReadDataShort( FILE * );
static FxU32 ReadDataLong( FILE * );
static void  Read8Bit( FxU8 *dst, FILE *image, int lod_min, int lod_max, GrAspectRatio_t aspect );
static void  Read16Bit( FxU16 *dst, FILE *image, int lod_min, int lod_max, GrAspectRatio_t aspect );

/* strupr()
**
** Summary: UNPUBLISHED - upper case the provided string in place.
**          Provided because strupr isn't provided in all clib implementations.
*/
static void strupr(char *ptr) {
    char c;
    while ((c=*ptr)) {
	if (c >= 'a' && c <= 'z') *ptr = c - 'a' + 'A';
	++ptr;
    }    
}

#if ( ( GLIDE_PLATFORM & ( GLIDE_OS_DOS32 | GLIDE_OS_WIN32 ) ) != 0 )
const char *openmode = "rb";
#else
const char *openmode = "r";
#endif

typedef struct
{
  const char        *name;
  GrTextureFormat_t  fmt;
  FxBool             valid;
} CfTableEntry;

/*
** gu3dfGetInfo
*/
FX_EXPORT FxBool
FX_CSTYLE gu3dfGetInfo( const char *FileName, Gu3dfInfo *Info )
{
  FILE *image_file;
  FxU32 index, i;
  FxU32 newlines = 0;
  char  version[5];
  char  color_format[10];
  int   aspect_width, aspect_height;
  char  buffer[100];
  int   lod_min, lod_max;
  FxBool ratio_found = FXFALSE;
  FxBool format_found = FXFALSE;
  GrAspectRatio_t wh_aspect_table[] =
  {
    GR_ASPECT_1x1,
    GR_ASPECT_1x2,
    GR_ASPECT_1x4,
    GR_ASPECT_1x8
  };
  GrAspectRatio_t hw_aspect_table[] =
  {
    GR_ASPECT_1x1,
    GR_ASPECT_2x1,
    GR_ASPECT_4x1,
    GR_ASPECT_8x1
  };
  CfTableEntry cftable[] = 
  {
      { "I8",       GR_TEXFMT_INTENSITY_8,        FXTRUE },
      { "A8",       GR_TEXFMT_ALPHA_8,            FXTRUE },
      { "AI44",     GR_TEXFMT_ALPHA_INTENSITY_44, FXTRUE },
      { "YIQ",      GR_TEXFMT_YIQ_422,            FXTRUE },
      { "RGB332",   GR_TEXFMT_RGB_332,            FXTRUE },
      { "RGB565",   GR_TEXFMT_RGB_565,            FXTRUE },
      { "ARGB8332", GR_TEXFMT_ARGB_8332,          FXTRUE },
      { "ARGB1555", GR_TEXFMT_ARGB_1555,          FXTRUE },
      { "AYIQ8422", GR_TEXFMT_AYIQ_8422,          FXTRUE },
      { "ARGB4444", GR_TEXFMT_ARGB_4444,          FXTRUE },
      { "AI88",     GR_TEXFMT_ALPHA_INTENSITY_88, FXTRUE },
      { "P8",       GR_TEXFMT_P_8,                FXTRUE },
      { "AP88",     GR_TEXFMT_AP_88,              FXTRUE },
      { 0, 0, FXFALSE }
  };

  /*
  ** open the filen
  */
  if( ( image_file = fopen( FileName, openmode ) ) == NULL )
    return FXFALSE;

  /*
  ** check header
  */
  index=0;
  while(newlines<4)  /* Read the header into a buffer. */
    if((buffer[index++]=getc(image_file))=='\n')
      {
    newlines++;
    buffer[index-1] = ' ';
      }
  buffer[--index] = 0;

  /*
  ** grab statistics out of the header
  */
  if( sscanf(buffer,"3df v%s %s lod range: %i %i aspect ratio: %i %i\n",
         &version,
          color_format,
         &lod_min, &lod_max,
         &aspect_width, &aspect_height ) == 0 )
    return FXFALSE;

  /*
  ** determine aspect ratio, height, and width
  */
  i = 0;
  ratio_found = FXFALSE;
  while ( ( i < 4 ) && ( !ratio_found ) )
  {
    if ( ( aspect_width << i ) == aspect_height )
    {
      Info->header.aspect_ratio = wh_aspect_table[i];
      ratio_found = FXTRUE;
    }
    i++;
  }
  i = 0;
  while ( ( i < 4 ) && ( !ratio_found ) )
  {
    if ( ( aspect_height << i ) == aspect_width )
    {
      Info->header.aspect_ratio = hw_aspect_table[i];
      ratio_found = FXTRUE;
    }
    i++;
  }
  if ( !ratio_found )
    return FXFALSE;

  /*
  ** determine height and width of the mip map
  */
  if ( aspect_width >= aspect_height )
  {
    Info->header.width  = lod_max;
    Info->header.height = lod_max / aspect_width;
  }
  else
  {
    Info->header.height = lod_max;
    Info->header.width  = lod_max / aspect_height;
  }


  /*
  ** calculate proper LOD values
  */
  switch ( lod_min )
  {
     case 1:
       Info->header.small_lod = GR_LOD_1;
       break;
     case 2:
       Info->header.small_lod = GR_LOD_2;
       break;
     case 4:
       Info->header.small_lod = GR_LOD_4;
       break;
     case 8:
       Info->header.small_lod = GR_LOD_8;
       break;
     case 16:
       Info->header.small_lod = GR_LOD_16;
       break;
     case 32:
       Info->header.small_lod = GR_LOD_32;
       break;
     case 64:
       Info->header.small_lod = GR_LOD_64;
       break;
     case 128:
       Info->header.small_lod = GR_LOD_128;
       break;
     case 256:
       Info->header.small_lod = GR_LOD_256;
       break;
  }

  switch ( lod_max )
  {
     case 1:
       Info->header.large_lod = GR_LOD_1;
       break;
     case 2:
       Info->header.large_lod = GR_LOD_2;
       break;
     case 4:
       Info->header.large_lod = GR_LOD_4;
       break;
     case 8:
       Info->header.large_lod = GR_LOD_8;
       break;
     case 16:
       Info->header.large_lod = GR_LOD_16;
       break;
     case 32:
       Info->header.large_lod = GR_LOD_32;
       break;
     case 64:
       Info->header.large_lod = GR_LOD_64;
       break;
     case 128:
       Info->header.large_lod = GR_LOD_128;
       break;
     case 256:
       Info->header.large_lod = GR_LOD_256;
       break;
  }

  /*
  ** determine the color format of the input image
  */
  strupr( color_format );

  i = 0;
  format_found = FXFALSE;
  while ( ( cftable[i].name != 0 ) && ( !format_found ) )
  {
    if ( strcmp( color_format, cftable[i].name ) == 0 )
    {
      Info->header.format = cftable[i].fmt;
      format_found = FXTRUE;
    }
    i++;
  }

  /*
  ** close the input file
  */
  fclose( image_file );

  if ( format_found )
    Info->mem_required = grTexCalcMemRequired( Info->header.small_lod, Info->header.large_lod, Info->header.aspect_ratio, Info->header.format  );
  return format_found;
}

/*
** gu3dfLoad
*/
FX_EXPORT FxBool
FX_CSTYLE gu3dfLoad( const char *filename, Gu3dfInfo *info )
{
  FILE *image_file  = 0;
  FxU32 index       = 0;
  FxU32 newlines    = 0;
  char  buffer[100] = "";

  /*
  ** open the file
  */
  if ( ( image_file = fopen( filename, openmode ) ) == NULL )
    return FXFALSE;

  /*
  ** examine the header
  */
  index = 0;
  while ( newlines < 4 )
  {
    if ( ( buffer[index++] = getc( image_file ) ) == '\n')
    {
      newlines++;
      buffer[index-1] = ' ';
    }
  }
  buffer[--index] = 0;

  /*
  ** If necessary, read in the YIQ decompression table
  */
  if ( ( info->header.format == GR_TEXFMT_YIQ_422 ) ||
       ( info->header.format == GR_TEXFMT_AYIQ_8422 ) )
  {
    /*
    ** read in Y
    */
    for ( index = 0; index < 16; index++ )
      info->table.nccTable.yRGB[index] = ( ( FxI16 ) ReadDataShort( image_file ) ) & 0xFF;

    /*
    ** read in I
    */
    for ( index = 0; index < 4; index++ )
    {
      info->table.nccTable.iRGB[index][0] = ( ( FxI16 ) ReadDataShort( image_file ) ) & 0x1FF;
      info->table.nccTable.iRGB[index][1] = ( ( FxI16 ) ReadDataShort( image_file ) ) & 0x1FF;
      info->table.nccTable.iRGB[index][2] = ( ( FxI16 ) ReadDataShort( image_file ) ) & 0x1FF;
    }

    /*
    ** read in Q
    */
    for ( index = 0; index < 4; index++ )
    {
      info->table.nccTable.qRGB[index][0] = ( ( FxI16 ) ReadDataShort( image_file ) ) & 0x1FF;
      info->table.nccTable.qRGB[index][1] = ( ( FxI16 ) ReadDataShort( image_file ) ) & 0x1FF;
      info->table.nccTable.qRGB[index][2] = ( ( FxI16 ) ReadDataShort( image_file ) ) & 0x1FF;
    }

    /*
    ** pack the table Y entries
    */
    for ( index = 0; index < 4; index++ )
    {
       FxU32 packedvalue;

       packedvalue  = ( ( FxU32 ) info->table.nccTable.yRGB[index*4+0] );
       packedvalue |= ( ( FxU32 ) info->table.nccTable.yRGB[index*4+1] ) << 8;
       packedvalue |= ( ( FxU32 ) info->table.nccTable.yRGB[index*4+2] ) << 16;
       packedvalue |= ( ( FxU32 ) info->table.nccTable.yRGB[index*4+3] ) << 24;
                                           
       info->table.nccTable.packed_data[index] = packedvalue;
    }

    /*
    ** pack the table I entries
    */
    for ( index = 0; index < 4; index++ )
    {
       FxU32 packedvalue;

       packedvalue  = ( ( FxU32 ) info->table.nccTable.iRGB[index][0] ) << 18;
       packedvalue |= ( ( FxU32 ) info->table.nccTable.iRGB[index][1] ) << 9;
       packedvalue |= ( ( FxU32 ) info->table.nccTable.iRGB[index][2] ) << 0;

       info->table.nccTable.packed_data[index+4] = packedvalue;
    }

    /*
    ** pack the table Q entries
    */
    for ( index = 0; index < 4; index++ )
    {
       FxU32 packedvalue;

       packedvalue  = ( ( FxU32 ) info->table.nccTable.qRGB[index][0] ) << 18;
       packedvalue |= ( ( FxU32 ) info->table.nccTable.qRGB[index][1] ) << 9;;
       packedvalue |= ( ( FxU32 ) info->table.nccTable.qRGB[index][2] ) << 0;

       info->table.nccTable.packed_data[index+8] = packedvalue;
    }
  }

  /*
  ** If necessary, read in the Palette
  */
  if ( ( info->header.format == GR_TEXFMT_P_8 ) ||
       ( info->header.format == GR_TEXFMT_AP_88 ) ) {
      FxU32 i;
      for( i = 0; i < 256; i++ )
          info->table.palette.data[i] = ReadDataLong( image_file );
  }

  /*
  ** Read in the image
  */
  switch ( info->header.format )
  {
  case GR_TEXFMT_INTENSITY_8:
  case GR_TEXFMT_ALPHA_8:
  case GR_TEXFMT_ALPHA_INTENSITY_44:
  case GR_TEXFMT_YIQ_422:
  case GR_TEXFMT_RGB_332:
  case GR_TEXFMT_P_8:
    Read8Bit( info->data, image_file, info->header.small_lod, info->header.large_lod, info->header.aspect_ratio );
    break;
  case GR_TEXFMT_RGB_565:
  case GR_TEXFMT_ARGB_8332:
  case GR_TEXFMT_ARGB_1555:
  case GR_TEXFMT_AYIQ_8422:
  case GR_TEXFMT_ARGB_4444:
  case GR_TEXFMT_ALPHA_INTENSITY_88:
  case GR_TEXFMT_AP_88:
    Read16Bit( info->data, image_file, info->header.small_lod, info->header.large_lod, info->header.aspect_ratio );
    break;

  default:
    return FXFALSE;
  }

  /*
  ** close the file
  */
  fclose( image_file );

  return FXTRUE;
}

/*
** Read8Bit
**
** Read in an 8-bit texture map, unpacked.
*/
static void Read8Bit( FxU8 *data, FILE *image_file, int lod_min, int lod_max, GrAspectRatio_t aspect_ratio )
{
  int index;
  int lod;
  int width, height;

  for ( lod = lod_max; lod <= lod_min; lod++ )
  {
    width  = atable[aspect_ratio][lod][0];
    height = atable[aspect_ratio][lod][1];

    for ( index = 0; index < ( width * height ); index++ )
    {
      *data = getc( image_file );
      data++;
    }
  }
}

/*
** Read16Bit
**
** Read in a 16-bit texture map, unpacked.
*/
static void Read16Bit( FxU16 *data, FILE *image_file, int lod_min, int lod_max, GrAspectRatio_t aspect_ratio )
{
  int index;
  int lod;
  int width, height;

  for ( lod = lod_max; lod <= lod_min; lod++ )
  {
    width  = atable[aspect_ratio][lod][0];
    height = atable[aspect_ratio][lod][1];

    for ( index = 0; index < ( width * height ); index++ )
    {
      *data = ReadDataShort( image_file );
      data++;
    }
  }
}

/*
** FxU16 ReadDataShort
*/
static FxU16 ReadDataShort( FILE *fp )
{
  FxU16 value;

  /*
  ** read in the MSB
  */
  value = ( ( FxU16 ) getc( fp ) ) << 8;

  /*
  ** read in the LSB
  */
  value |= ( getc( fp ) & 0x00FF );

  return value;
}

/*
** ReadDataLong
*/
static FxU32 ReadDataLong( FILE *fp ) {
    FxU32 data;
    FxU8 byte[4];

    fread(byte, 4, 1, fp);
    data = (((FxU32) byte[0]) << 24) |
           (((FxU32) byte[1]) << 16) |
           (((FxU32) byte[2]) <<  8) |
            ((FxU32) byte[3]);

    return data;
}



