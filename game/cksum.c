/*
 * ======================================================================
 *
 * $RCSfile: cksum.c,v $ -- do checksum stuff
 *
 * Copyright (C) 1997 Atari Games
 * Unauthorized reproduction, adaptation, distribution, performance or
 * display of this computer program or the associated audiovisual work
 * is strictly prohibited.
 *
 * ======================================================================
 * $Author: hightower $            $Date: 1997/10/16 08:26:35 $
 * $Revision: 1.3 $            $Locker:  $
 * ======================================================================
 * Change Log:
 *
 * $Log: cksum.c,v $
 * Revision 1.3  1997/10/16  08:26:35  hightower
 * More reliable gamenet
 *
 * Revision 1.2  1997/10/01  21:12:05  hightower
 * removed tabs
 *
 * Revision 1.1  1997/09/29  07:26:30  hightower
 * Initial revision
 *
 *
 * ======================================================================
*/

#include "cksum.h"

/* This is a generic checksum routine -- it currently provides 64-bit checksums
 * without the need for a 64-bit integral type (which is apparently not a good
 * thing to use for gcc-compiled and/or ANSI strict code)
*/
static ckSum
_ckSumGet( ckSum cksum, const void *const ptr, const size_t size )
{
    if( ptr && size )
    {
        const char *const cptr = ( const char *const ) ptr;
        int i=0,j=0;

        do
        {
            cksum.u.u8[ j++ ] ^= cptr[ i++ ];
            if( j == sizeof( cksum.u.u8 ) ) j=0;
        }
        while( i<size );
    }
    else
    {
        cksum.u.u32[0] = cksum.u.u32[1] = 0;
    }

    return cksum;
}

ckSum
ckSumGet( const void *const ptr, const size_t size )
{
    ckSum cksum = { { { 1, 2, 3, 4, 5, 6, 7, 8 } } };

    return _ckSumGet( cksum, ptr, size );
}
