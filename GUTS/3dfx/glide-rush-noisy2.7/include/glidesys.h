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
** $Header: /engr/DEVEL/SST1/GLIDE/DOS/SRC/GLIDESYS.H 2     6/08/96 4:16p Dow $
** $Log: /engr/DEVEL/SST1/GLIDE/DOS/SRC/GLIDESYS.H $
 * 
 * 2     6/08/96 4:16p Dow
 * 	Changed def of GLIDE_NUM_TMU (We're not planning a 3-TMU board)
 * 
 * 1     5/09/96 7:03a Paik
 * Glide system definitions
*/
#ifndef __GLIDESYS_H__
#define __GLIDESYS_H__

/*
** -----------------------------------------------------------------------
** COMPILER/ENVIRONMENT CONFIGURATION
** -----------------------------------------------------------------------
*/
#define GLIDE_ENDIAN_LITTLE   0x40000000
#define GLIDE_ENDIAN_BIG      0x80000000

#define GLIDE_OS_UNIX         0x00000001
#define GLIDE_OS_DOS32      ( 0x00000100 | GLIDE_ENDIAN_LITTLE )
#define GLIDE_OS_WIN32      ( 0x00000200 | GLIDE_ENDIAN_LITTLE )
#define GLIDE_OS_SYSTEM7    ( 0x00000400 | GLIDE_ENDIAN_BIG )
#define GLIDE_OS_OS2        ( 0x00000800 | GLIDE_ENDIAN_LITTLE )

#define GLIDE_SST_SIM       ( 0x01000000 )
#define GLIDE_SST_HW        ( 0x02000000 )
#define GLIDE_SST           ( GLIDE_SST_SIM | GLIDE_SST_HW )

#if defined(__GNUC__)
#define GLIDE_PLATFORM ( GLIDE_ENDIAN_LITTLE | GLIDE_SST_HW )
#else
/*
** Make sure we handle all instances of WIN32
*/
#ifndef __WIN32__
#  if defined ( _WIN32 ) || defined (WIN32) || defined(__NT__)
#    define __WIN32__
#  endif
#endif

#ifdef GLIDE_SIMULATOR
#if defined ( __IRIX__ )
#  define GLIDE_PLATFORM ( GLIDE_OS_UNIX | GLIDE_ENDIAN_BIG | GLIDE_SST_SIM )
#elif defined ( __sparc__ )
#  define GLIDE_PLATFORM ( GLIDE_OS_UNIX | GLIDE_ENDIAN_BIG | GLIDE_SST_SIM )
#elif defined ( __linux__ )
#  define GLIDE_PLATFORM ( GLIDE_OS_UNIX | GLIDE_ENDIAN_LITTLE | GLIDE_SST_SIM )
#elif defined ( __DOS__ )
#  if defined ( __WATCOMC__ )
#    define GLIDE_PLATFORM ( GLIDE_OS_DOS32 | GLIDE_SST_SIM )
#  else
#    error "32-bit DOS support currently available only under Watcom C++"
#  endif
#elif defined ( __WIN32__ )
#  define GLIDE_PLATFORM ( GLIDE_OS_WIN32 | GLIDE_SST_SIM )
#else
#  error "No platform defined"
#endif
#endif

#ifdef GLIDE_HARDWARE

#if defined ( __IRIX__ )
#  define GLIDE_PLATFORM ( GLIDE_OS_UNIX | GLIDE_ENDIAN_BIG | GLIDE_SST_HW )
#elif defined ( __sparc__ )
#  define GLIDE_PLATFORM ( GLIDE_OS_UNIX | GLIDE_ENDIAN_BIG | GLIDE_SST_HW )
#elif defined ( __linux__ )
#  define GLIDE_PLATFORM ( GLIDE_OS_UNIX | GLIDE_ENDIAN_LITTLE | GLIDE_SST_HW )
#elif defined ( __DOS__ )
#  if defined ( __WATCOMC__ )
#    define GLIDE_PLATFORM ( GLIDE_OS_DOS32 | GLIDE_SST_HW )
#  else
#    error "32-bit DOS support currently available only under Watcom C++"
#  endif
#elif defined ( __DOS32__ )
#  if defined ( __DJGPP__ )
#    define GLIDE_PLATFORM ( GLIDE_OS_DOS32 | GLIDE_SST_HW )
#  else
#    error "32-bit DOS support currently available only under DJGPP"
#  endif
#elif defined ( __WIN32__ )
#  define GLIDE_PLATFORM ( GLIDE_OS_WIN32 | GLIDE_SST_HW )
#else
#  error "No platform defined"
#endif

#endif
#endif
            
/*
** Control the number of TMUs
*/
#ifndef GLIDE_NUM_TMU
#  define GLIDE_NUM_TMU 2
#endif


#if ( ( GLIDE_NUM_TMU < 0 ) && ( GLIDE_NUM_TMU > 3 ) )
#  error "GLIDE_NUM_TMU set to an invalid value"
#endif

#endif /* __GLIDESYS_H__ */

