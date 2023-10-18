/*
 * ======================================================================
 *
 * $RCSfile: cksum.h,v $ --
 *
 * Copyright (C) 1997 Atari Games
 * Unauthorized reproduction, adaptation, distribution, performance or
 * display of this computer program or the associated audiovisual work
 * is strictly prohibited.
 *
 * ======================================================================
 * $Author: hightower $            $Date: 1997/10/01 21:17:39 $
 * $Revision: 1.2 $            $Locker:  $
 * ======================================================================
 * Change Log:
 *
 * $Log: cksum.h,v $
 * Revision 1.2  1997/10/01  21:17:39  hightower
 * removed tabs
 *
 * Revision 1.1  1997/09/29  07:26:45  hightower
 * Initial revision
 *
 *
 * ======================================================================
*/

#ifndef __CKSUM_H__
#define __CKSUM_H__

/*===============================  TYPEDEFS  ================================*/

#include <sys/types.h> /* typedef for size_t  */
#include "config.h"    /* U[8/16/32] typedefs */

typedef struct ckSum
{
    union
    {
        U8  u8 [8];
        U16 u16[4];
        U32 u32[2];
    } u;
}
ckSum;


/*==============================  PROTOTYPES  ===============================*/


extern ckSum ckSumProduct( const ckSum a, const ckSum b );
extern ckSum ckSumGet    ( const void *const ptr, const size_t size );

#define ckSumEqual(a,b) \
        ( ( (a).u.u32[0] == (b).u.u32[0] ) && ( (a).u.u32[1] == (b).u.u32[1] ) )

#endif /* !__CKSUM_H__ */
