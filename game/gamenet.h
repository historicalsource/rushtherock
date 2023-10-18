/*
 *  gamenet.h   -- Functions and definitiions for game level network.
 *
 *  Copyright 1997 Atari Corporation.
 *  Unauthorized reproduction, adaptation, distribution, performance or
 *  display of this computer program or the associated audiovisual work
 *  is strictly prohibited.
 *
 *  ======================================================================
 *  $Author: hightower $                     $Date: 1997/10/16 05:30:55 $
 *  $Revision: 3.13 $                        $Locker:  $
 *  ======================================================================
 *  Change Log:
 *
 *  $Log: gamenet.h,v $
 * Revision 3.13  1997/10/16  05:30:55  hightower
 * More reliable gamenet
 *
 * Revision 3.12  1997/10/11  20:59:30  hightower
 * Made drone cars the same
 *
 * Revision 3.11  1997/10/10  23:11:28  hightower
 * Added gLink..flags to gamenet
 *
 * Revision 3.10  1997/10/01  03:37:39  hightower
 * Changed countdown timers to be more accurate
 *
 * Revision 3.9  1997/09/30  02:50:20  hightower
 * Incremented version number to prevent bad crosstalk
 *
 * Revision 3.8  1997/09/29  07:28:02  hightower
 * Extracted checksum funcs from gamenet.c to cksum.c
 *
 * Revision 3.7  1997/09/26  21:45:21  hightower
 * Dont propogate negative time
 *
 * Revision 3.6  1997/09/26  20:28:08  hightower
 * Join Now message now counts down based on timers of people already in track select
 *
 * Revision 3.5  1997/09/26  00:14:55  hightower
 * Turned GAMENET on, and debugging off
 *
 * Revision 3.4  1997/09/25  19:06:56  hightower
 * Added track
 *
 * Revision 3.3  1997/09/25  05:53:46  hightower
 * Gamenet functional, but disabled -- to enable define GAMENET to 1 in gamenet.h
 *
 * Revision 3.2  1997/09/17  08:15:14  hightower
 * Reformatted
 *
 * Revision 3.1  1997/08/22  01:38:13  gray
 * Working on replacing link control with LinkStatus.
 *
 * Revision 3.0  1997/08/10  08:52:20  gray
 * Initial version.
 *
 *  ======================================================================
 */

#ifndef __GAMENET_H__
#define __GAMENET_H__

#include "config.h"
#include "net_smc.h"
#include "net_link.h"
#include "cksum.h"
#include "debug.h"

/*===============================  DEFINES  =================================*/

#define GAMENET          1              /* "Use gamenet" */
#define NET_VERSION      3

/*================================  ENUMS  =================================*/

typedef enum GNerr
{
    GNerr_NetInitialize = 0,
    GNerr_OpenInterface,
    GNerr_GetBuffer,
    GNerr_FreeBuffer,
    GNerr_SendPacket,
    HIGHEST_GNERR
}
GNerr;

/*===============================  TYPEDEFS  ================================*/

/*
 * If cksum[this_node] != cksum[x] and alive[x] for any x send state to network
 *
 * DESIGN:
 *   Each frame, getChecksum( gs ) is compared against the checksum that the
 *   other stacks have computed for your data.  Each packet that gets sent
 *   out also sends out the checksum of gn_state[i] into the packet header.
 *   This is parsed out at the receiving end to know what the remote checksum
 *   of your data is (so you can know if the remote side has outdated info)
 *
 *   This is a little bit difficult to follow, but the important thing is to
 *   know that gn_state[i].cksum is __NOT__ from getChecksum( &gn_state[i] )
 *   rather this is what remote statck "i" computed from his copy of your
 *   local stack's state information  (and this might be outdated).
 *   Asserting that getChecksum( gs ) != gn_state[i].cksum means that stack
 *   "i" has outdated state information for your local stack...
 *
 *   The design allows gnData to be updated along with the getChecksum()
 *   routine and additional state information may be shared as simply as that..
*/

typedef struct gnData
{
    GState state;            /* state of x-mitting stack                  */
    U8     track;            /* Which track is selected                   */
    U8     flags;            /* Flags from gLink[i].flags                 */
    S32    start_time;       /* set from start_time in select.c           */
    S32    play_time;        /* set from play_time  in select.c           */
    U32    seed;             /* When u need net-uniform pseudo-random #'s */
}
gnData;

typedef struct gnState
{
    ckSum cksum;             /* cksum for remote copy of gn_state[gThisNode] */
    U32  alive;              /* Counts down to 0 .. 0==dead, else alive      */
    U32  rcvtime;            /* Time last packet for this state was rcv'ed   */
    U32  rcvcnt;             /* Count of number of packets received          */
    S32  irqdiff;            /* How much bigger is IRQTIME than remote?      */
    U32  seqid;              /* Sequence id of last packet from this node    */
    U32  pktloss;            /* Count of how many packets lost from this node*/

    gnData d;                /* The part that is actually x-mitted           */
}
gnState;

/*==============================  PROTOTYPES  ===============================*/

#include "Pro/gamenet.pro"

/*===============================  GLOBALS  =================================*/

extern gnState            gn_state[MAX_LINKS];     /* State of each stack */
extern S32                GNerrcnt[HIGHEST_GNERR];
extern network_descriptor netDesc;

#endif /* ! __GAMENET_H__ */
