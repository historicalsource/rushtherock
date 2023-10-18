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
** $Revision: 1.3 $
** $Date: 1995/05/27 16:07:40 $
*/
#include <stdlib.h>
#include <stdio.h>

#include <fx64.h>

#ifndef NATIVE_64_SUPPORT

const FxI64  i64_one  = { 0, 1 };
const FxI64  i64_zero = { 0, 0 };

static FxBool fx64_cf;

FxBool __FX_ADD32( FxU32 a, FxU32 b );
#pragma aux __FX_ADD32 = \
   "mov fx64_cf, 0" \
   "add eax, edx"   \
   "adc fx64_cf, 0" \
   parm [eax] [edx] \
   value [eax];

FxI64 __FX_AND64( FxI64 a, FxI64 b )
{
   FxI64 result;

   result.hi = a.hi & b.hi;
   result.lo = a.lo & b.lo;

   return result;
}

FxI64 __FX_CREATE64( FxI32 a, FxU32 b )
{
   FxI64 result;

   result.hi = a;
   result.lo = b;

   return result;
}

/*
** __FX_MUL64
**
** The following code is NOT a full 64-bit multiply.  It only computes
** the low order 64-bits of the product since this is all our hardware
** needs.
*/
FxI64 __FX_MUL64( FxI64 a, FxI64 b )
{
   FxI64 result = { 0, 0 };

   FXUNUSED( a );
   FXUNUSED( b );

   puts( "__FX_MUL64:  not implemented" );
   exit( 1 );
   return result;
}

FxI64 __FX_OR64( FxI64 a, FxI64 b )
{
   FxI64 result;

   result.hi = a.hi | b.hi;
   result.lo = a.lo | b.lo;

   return result;
}

FxI64 __FX_COMP64( FxI64 a )
{
   FxI64 result;

   result.hi = ~a.hi;
   result.lo = ~a.lo;

   return result;
}

float __FX_64TOFLOAT( FxI64 a )
{
   double hi, lo;

   hi = a.hi * 4294967296.0;	/* 2**32 */
   lo = ( float ) a.lo;
   return (float)(hi + lo);
}

FxI64 __FX_FLOATTO64( float f )
{
    int n;
    FxU32 data;
    FxI64 result;

    data = *(FxU32 *)&f;		/* get into integer register */
    n = (data>>23) & 0xFF;		/* peel off exponent */
    n = 150 - n;			/* compute shift amount */
    data |= 0x800000;			/* add in the hidden bit */
    result.hi = 0;
    result.lo = (data&0xFFFFFF);
    if (n > 0) {			/* need to shift right, < 2**23 */
	if (n > 31) n = 31;
	result.lo >>= n;
    }
    else {
	n = -n;
	if (n > 63) n = 63;
        result = FX_SHL64( result, n );
    }
    /* negate after shift, this rounds down instead of to zero */
    if (data & 0x80000000)
      result = FX_NEG64(result);
    return result;
}


int __FX_CMP64( FxI64 a, FxI64 b )
{
   if ( a.hi < b.hi )
      return -1;
   if ( a.hi > b.hi )
      return 1;
   if ( a.lo < b.lo )
      return -1;
   else if ( a.lo > b.lo )
      return 1;
   return 0;
}

#endif

