/*
** Copyright (c) 1995, 1996, 3Dfx Interactive, Inc.
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
** $Revision: 1 $
** $Date: 4/04/96 3:20p $
*/
/* preprocessor defines for libraries to support DLL creation */

/* in header file, use FX_ENTRY return-type FX_CALL function-name ( ... ) */
/* in source file, use FX_EXPORT return-type FX_CSTYLE function-name (... ) */

/* in source file, set FX_DLL_DEFINITION, include this file, then include
   header file for library. */

/* we need to use two macros per declaration/definition because MSVC
   requires __stdcall and __declspec( dllexport ) be in different parts
   of the prototype! */

/* I use two sets in case we need to control declarations and definitions
   differently.  If it turns out we don't, it should be easy to do a search
   and replace to eliminate one set */

#if 0
#ifdef FX_ENTRY
#undef FX_ENTRY
#endif

#ifdef FX_CALL
#undef FX_CALL
#endif

#ifdef FX_EXPORT
#undef FX_EXPORT
#endif

#ifdef FX_CSTYLE
#undef FX_CSTYLE
#endif

#if defined(FX_DLL_DEFINITION)

#if defined(FX_DLL_ENABLE)

#if defined(__MSC__)

#define FX_ENTRY __declspec( dllexport )
#define FX_CALL __stdcall

#define FX_EXPORT __declspec( dllexport )
#define FX_CSTYLE __stdcall

#elif defined(__WATCOMC__)

#define FX_ENTRY
#define FX_CALL __stdcall __export

#define FX_EXPORT
#define FX_CSTYLE __stdcall __export

#else /* compiler */

#error define FX_ENTRY,FX_CALL & FX_EXPORT,FX_CSTYLE for your compiler

#endif /* compiler */

#else /* FX_DLL_ENABLE */

#define FX_ENTRY
#define FX_CALL __stdcall

#define FX_EXPORT
#define FX_CSTYLE __stdcall

#endif /* FX_DLL_ENABLE */

#else /* FX_DLL_DEFINITION */

#define FX_ENTRY extern
#define FX_CALL __stdcall

#endif /* FX_DLL_DEFINITION */
#endif
