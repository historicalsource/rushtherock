/*
 *  gamenet.c   -- Functions for game level network.
 *
 *  Copyright 1997 Atari Corporation.
 *  Unauthorized reproduction, adaptation, distribution, performance or
 *  display of this computer program or the associated audiovisual work
 *  is strictly prohibited.
 *
 *  ======================================================================
 *  $Author: hightower $                        $Date: 1997/10/16 20:13:02 $
 *  $Revision: 3.19 $                        $Locker:  $
 *  ======================================================================
 *  Change Log:
 *
 *  $Log: gamenet.c,v $
 * Revision 3.19  1997/10/16  20:13:02  hightower
 * Took out sound in case of production release
 *
 * Revision 3.18  1997/10/16  10:02:12  hightower
 * Added screaming to help know when links get severed
 *
 * Revision 3.17  1997/10/16  09:39:46  hightower
 * Remove network spew from seqid off by 1
 *
 * Revision 3.16  1997/10/16  08:26:39  hightower
 * More reliable gamenet
 *
 * Revision 3.15  1997/10/15  04:10:24  hightower
 * Added countdown timer diffs
 *
 * Revision 3.14  1997/10/10  23:12:04  hightower
 * Added gLink..flags to gamenet
 *
 * Revision 3.13  1997/10/01  21:03:13  hightower
 * Removed tabs
 *
 * Revision 3.12  1997/10/01  03:37:37  hightower
 * Changed countdown timers to be more accurate
 *
 * Revision 3.11  1997/09/29  20:20:35  hightower
 * Removed warning due to bug in cproto(1)
 *
 * Revision 3.10  1997/09/29  08:13:09  hightower
 * Removed force countdown set - causing network spew
 *
 * Revision 3.9  1997/09/29  07:28:01  hightower
 * Extracted checksum funcs from gamenet.c to cksum.c
 *
 * Revision 3.8  1997/09/26  21:45:13  hightower
 * Dont propogate negative time
 *
 * Revision 3.7  1997/09/26  20:28:06  hightower
 * Join Now message now counts down based on timers of people already in track select
 *
 * Revision 3.6  1997/09/26  00:14:55  hightower
 * Turned GAMENET on, and debugging off
 *
 * Revision 3.5  1997/09/25  19:07:58  hightower
 * Added track, enabled low-level fitering, added generic cksum function
 *
 * Revision 3.4  1997/09/25  05:53:46  hightower
 * Gamenet functional, but disabled -- to enable define GAMENET to 1 in gamenet.h
 *
 * Revision 3.3  1997/09/17  08:52:10  hightower
 * Reformatted
 *
 * Revision 3.2  1997/09/13  05:35:12  hightower
 * FIxed bug in init loop
 *
 * Revision 3.1  1997/08/22  01:38:13  gray
 * Working on replacing link control with LinkStatus.
 *
 * Revision 3.0  1997/08/10  08:52:54  gray
 * Initial version.
 *
 *  ======================================================================
 */

#include "globals.h"
#include "gamenet.h"
#include "sounds.h"

#include "Pro/sounds.pro"

#include "select.h"
#include "cksum.h"

/*===============================  DEFINES  =================================*/

const netadd BROADCAST_COM_CHAN = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

#define MAX_LSQUEUE 256

#define GN_1SEC          1000           /* Time is in milliseconds (IRQTIME) */

/* How much time before sending again (even if no data has changed)
 * this can be adjusted as necessary -- bigger values will take longer to
 * figure out that someone has fallen off the net.  Smaller values will eat
 * up more bandwidth with redundant info
 *
 * NOTE: GN_IS_ALIVE must be (significantly) larger than the refresh time.
*/

/* May adjust later -- should be larger than net delay
 * Time before assuming last sent packet was lost
*/
#define GN_VERIFY_TIME   (50)
#define GN_REFRESH_TIME  (GN_1SEC*4)
#define GN_IS_ALIVE      (GN_REFRESH_TIME*2)
#define GN_GRACE_PERIOD  (GN_1SEC/10)

/*=================================  ENUMS  =================================*/

typedef enum MSGtype
{
    MSG_LinkState = 0,
    MSG_PositionUpdate,
    MSG_AppearanceUpdate
}
MSGtype;

/*===============================  TYPEDEFS  ================================*/


typedef struct gnFilter
{
    U8      version;          /* Is this gamenet stuff or dis stuff        */
    U8      node;             /* Which stack did this come from?           */
    MSGtype type;             /* What kind of stuff is this?               */
    U8      dummy;
}
gnFilter;

typedef struct gnHeader
{
    S32     irqtime;          /* Local IRQTIME in here with each pkt sent  */
    U32     seqid;            /* Sequence ID of packet                     */
    ckSum   cksum[MAX_LINKS]; /* cksums of this type state from x-mit side */
}
gnHeader;

typedef struct gnStateMSG
{
    gnFilter   filter;
    gnHeader   header;
    gnData     data;
}
gnStateMSG;

S32 rcvtime[MAX_LINKS];

typedef struct MSG_QUEUE
{
    U32 *queue;
    S32 ndx_max;
    S32 ndx_bot;
    S32 ndx_top;
}
MSG_QUEUE;


/*==============================  PROTOTYPES  ===============================*/

static S32   filter( gnFilter *filt, Net_Buffer **nbpp, S32 plen );
static S32   receive( Net_Buffer *nb );
static ckSum getChecksum( gnState *state );
static void  processLSQueue (void);
static void  sendStateMSG (void);
static void  parseStateMSG ( gnStateMSG * msg);

static void Create_Queue (MSG_QUEUE * q, U32 * mem, U32 max);
static S32 Send_To_Queue (MSG_QUEUE * q, U32 p);
static S32 Receive_From_Queue (MSG_QUEUE * q, U32 * p);

/*===============================  GLOBALS  =================================*/

gnState gn_state[MAX_LINKS];
S32 GNerrcnt[HIGHEST_GNERR];

static U32 seqid = 1;
static U32 sendTime;
static BOOL gameNetInitialized;
static MSG_QUEUE LSqueue;       /*  Queue of LinkState messages received,  */
static U32 LSqueue_mem[MAX_LSQUEUE];

/* Kind of a "this" pointer -- state for this stack */
static gnState *gs;

/* XXX - Steal dis.c's network descriptor until we have our own */
extern network_descriptor gNetDesc;

/*===============================  EXTERNS  =================================*/

#define GAMENET_DEBUG

#ifdef  GAMENET_DEBUG

#include <config.h>
#include <string.h>
#include "os_proto.h"

#endif

void
gnInit (void)
{
#if GAMENET
    int i;
    gnState *sp;

    /* Zero all net error counts. */
    for (i = 0; i < HIGHEST_GNERR; i++)
        GNerrcnt[i] = 0;

    /* initialize gn_state for all nodes. */
    for (i = 0; i < MAX_LINKS; i++)
    {
        sp          = &gn_state[i];
        sp->d.state = gstate;
        sp->d.track = 0;
        sp->rcvtime = 1;
        sp->rcvcnt  = 0;
        sp->alive   = ( i == gThisNode ) ? GN_IS_ALIVE : 0;
    }

    /* Create and setup the LinkState queue. */
    Create_Queue (&LSqueue, (U32 *) & LSqueue_mem, MAX_LSQUEUE);

    gs = &gn_state[ gThisNode ];

    gameNetInitialized = 1;
#endif
}

/*
 * Network callback
 * Called with various 'func' parameters by the network layer to perform
 * the given function on it's behalf.
 * This function is registered with the network at startup via the
 * Net_Open_Interface function call.
 *
 *   func    - The function to perform. (an integral type (#defined) )
 *   nd      - The network descriptor originating this request.
 *   pkt_no  - An internal driver packet number.
 *   plen    - Total length of the packet in bytes.
 *   ...     - Optional parameters.
*/

S32
gnNetCallback (Net_CB_Func func, network_descriptor nd,
    packet_number pkt_no, S32 plen, U32 p1, U32 p2, U32 p3)
{
#ifdef GAMENET_DEBUG
    if( p1 < sizeof( gnFilter ) )
    {
        txt_str( 1, 1, "need to increase filter signature length", GRN_PAL );
    }
#endif
    if( CB_Filter == func )
    {
        return filter( ( gnFilter * ) p2, ( Net_Buffer ** ) p3, plen );
    }
    else if( CB_Receive == func )
    {
        return receive( ( Net_Buffer * ) p1 );
    }
    return CB_FAILED;
}

static S32
filter( gnFilter *filt, Net_Buffer **nbpp, S32 plen )
{
    /* This is the PDU filter function.  It is called by the driver
     * to decide if an incoming packet is acceptable and to relay the
     * packet's size.  This function is responsible for deciding if
     * this packet is desirable, and for providing a memory buffer to
     * place the packet in.
    */

    /* Don't accept packets until stuff initialized. */
    /* Filter packets with different NET_VERSION. */
    /* Filter packets with sent from this node. */
    if (!gameNetInitialized ||
         filt->version != NET_VERSION )
    {
        return CB_Drop_Packet;
    }
    rcvtime[filt->node] = IRQTIME;

    /* We want the packet, allocate a buffer to read it into. */
    if( NETOK != Net_Get_Buffer ( (Net_Buffer **)nbpp, plen, NETOPT_NONBLOCK))
    {
        GNerrcnt[GNerr_GetBuffer]++;
        return CB_Drop_Packet;
    }

    /*  Tell the driver to accept the packet. */
    return CB_Copy_Packet;
}

/* This function puts the message on a local queue
*/
static S32
receive( Net_Buffer *nb )
{
    gnStateMSG *msg = (gnStateMSG *) & nb->data[NET_PKT_HEADER_SIZE];

    if (msg->filter.type == MSG_LinkState)
    {
        Send_To_Queue (&LSqueue, (U32) nb);
        return CB_OK;
    }
    else
    {
        if (NETOK != (Net_Free_Buffer (nb, NETOPT_NONBLOCK)))
        {
            GNerrcnt[GNerr_FreeBuffer]++;
        }
        return CB_FAILED;
    }
}

static void
processLSQueue (void)
{
    Net_Buffer *nb;
    S16 res;

    /* Process all LinkState meassages in LSqueue. */
    while (1)
    {
        /* Get next Net_buffer pointer from queue. */
        if (Receive_From_Queue (&LSqueue, (U32 *) & nb) == 0)
            break;              /*queue empty */

        /* Parse LinkState message. */
        parseStateMSG ( ( gnStateMSG * ) & nb->data[NET_PKT_HEADER_SIZE]);

        /* Done with Net_Buffer - free it. */
        res = Net_Free_Buffer (nb, NETOPT_NONBLOCK);
        if (res != NETOK)
            GNerrcnt[GNerr_FreeBuffer]++;
    }
}

static void
parseStateMSG (gnStateMSG * msg)
{
    U8 node = msg->filter.node;
    S32 diff;

    if( ( diff = ( ( gn_state[node].seqid ) ?
        ( msg->header.seqid - gn_state[node].seqid - 1 ) : 0 ) ) >= 0 )
    {
        gn_state[node].pktloss += diff;
    }
    else /* If the diff is negative, stack was reset so reset pktloss */
    {
        gn_state[node].pktloss = 0;
    }

    gn_state[node].seqid = msg->header.seqid;

    /* Only do the remaining stuff if from another stack */
    if( node != gThisNode )
    {
        gn_state[node].irqdiff = msg->header.irqtime-rcvtime[node];
        gn_state[node].d       = msg->data;   /* Copy over the data    */

        /* The (only) tricky part -- read comment in the .h file for more */
        gn_state[node].cksum = msg->header.cksum[gThisNode];
    }
    else
    {
        /* Maybe judge network delay here -- XXX (?) AMH */
    }

    gn_state[node].alive  = GN_IS_ALIVE; /* Assume alive for this many ticks */
    gn_state[node].rcvcnt++;             /* Yes, we received a packet        */
    gn_state[node].rcvtime = rcvtime[node];/* It was received at this time   */
}

static void
sendStateMSG (void)
{
    U32 res;
    Net_Buffer *nb;
    gnStateMSG *msg;
    const S32 pktlen = sizeof (gnStateMSG);

    res = Net_Get_Buffer (&nb, pktlen, NETOPT_NONBLOCK | NETOPT_PRIORITY);

    if (res != NETOK)
    {
        GNerrcnt[GNerr_GetBuffer]++;
        return;
    }

    /* Fill in net buffer data. */
    nb->len = pktlen;

    /* Fill in msg filter data. */
    msg = (gnStateMSG *) & nb->data;
    msg->filter.version = NET_VERSION; /* let DIS know that this is gamenet */
    msg->filter.type    = MSG_LinkState;
    msg->filter.node    = gThisNode;
    msg->filter.dummy   = 0;
    msg->data           = gn_state[gThisNode].d;

    /* The following will compute and include the checksums for each linkStat
     * so that on the receiving side, it can know who is up to date on state
    */
    {
        U16 i;

        msg->header.seqid = ++seqid;
        msg->header.irqtime = IRQTIME;

        for( i=0; i<MAX_LINKS; i++ )
        {
            msg->header.cksum[ i ] = getChecksum( &gn_state[i] );
        }
    }

    res = Net_Send_Packet (gNetDesc, BROADCAST_COM_CHAN, nb, NETOPT_NONBLOCK);

    sendTime = IRQTIME;

    if (res != NETOK_SEND_COMPLETE)
        GNerrcnt[GNerr_SendPacket]++;

    res = Net_Free_Buffer (nb, NETOPT_NONBLOCK);
    if (res != NETOK)
        GNerrcnt[GNerr_FreeBuffer]++;
}


static void
Create_Queue (MSG_QUEUE * q, U32 * mem, U32 max)
{
    q->queue = mem;
    q->ndx_max = max;
    q->ndx_bot = q->ndx_top = 0;
}

/* Returns: 0=QUEUE_FULL, 1=MSG_QUEUED */
static S32
Send_To_Queue (MSG_QUEUE * q, U32 p)
{
    S32 newtop = q->ndx_top + 1;

    if (newtop >= q->ndx_max)
        newtop = 0;

    if (newtop == q->ndx_bot)
        return 0;               /* queue full. */

    q->queue[q->ndx_top] = p;
    q->ndx_top = newtop;

    return 1;
}

/* Returns: 0=QUEUE_EMPTY, 1=MSG_DEQUEUED */
static S32
Receive_From_Queue (MSG_QUEUE * q, U32 * p)
{
    S32 ipl = prc_set_ipl (INTS_OFF);

    /* check for empty queue. */
    if (q->ndx_bot == q->ndx_top)
    {
        prc_set_ipl (ipl);
        return 0;
    }

    /* return queue bottom. */
    *p = q->queue[q->ndx_bot++];
    if (q->ndx_bot >= q->ndx_max)
        q->ndx_bot = 0;

    prc_set_ipl (ipl);
    return 1;
}

/* Return a different result based on the contents of * the link status.
 * Ignore changes in some fields (like the cksum) -- should return diff
 * result to indicate that the packet needs retransmitting...
 */
static ckSum
getChecksum ( gnState *s )
{
    /* This will kindly inform others if they had an message dropped
     * within a maximum period of 2*REFRESH_TIME + GRACE_PERIOD
    */
    if( GN_IS_ALIVE - s->alive < GN_REFRESH_TIME + GN_GRACE_PERIOD )
    {
        return ckSumGet( &s->d, sizeof( s->d ) );
    }
    else
    {
        return ckSumGet( 0, 0 );
    }
}

/* Send out link information if any of the stacks have not already received
 * it...
 */
static void
sendChanged (void)
{
    ckSum cksum;
    int i, sendit;
    static U32 pulse = 0;

    if( !gs ) return;

    cksum = getChecksum (gs);
    sendit = 0; /* Assume it doesn't need to be sent */

    for ( i = 0; i < MAX_LINKS; i++)
    {
        /* If we send it, assume they get it -- autoupdate will fix if they
         * don't get it (when they REFRESH after hitting GN_REFRESH_TIME
         * their checksums will be xmitted, and the out-of-date state will be
         * caught) -- NOTE: this method assumes that packets generally don't
         * get dropped (IE: if a packet gets dropped, it will be GN_REFRESH
         * amount of time before we know about it since there is no ack's)
        */
        if( gn_state[i].alive && !ckSumEqual( gn_state[i].cksum, cksum ) )
        {
            sendit = 1;

            gn_state[i].cksum = cksum;
        }
    }

    /* This GN_REFRESH_TIME can be adjusted if desired in gamenet.h ..
     * this refresh is what keeps gn_state[i].alive set on the other side
     *
     * This will also acknowledge a dropped packet and correct the problem
     * by resending the packet
    */
    if ( ( IRQTIME - pulse > GN_REFRESH_TIME ) ||
       ( seqid != gn_state[gThisNode].seqid &&
       ( IRQTIME - sendTime ) > GN_VERIFY_TIME ) )
    {
        sendit = 1;
    }

    if( sendit )
    {
        pulse = IRQTIME;
        sendStateMSG ();
    }
}

/* This function checks the "pulse" of each stack on the network.  If the
 * last message was heard within a certain threshold of time, then assume
 * that the stack is "alive."
 */
static void
checkPulse (void)
{
    static U32 prevtime;
    U32 curtime = IRQTIME;

    /* Don't do anything but update prevtime on the first update frame */
    if( prevtime )
    {
        BOOL sound_fired = 0; /* Prevent multiple yells at same frame */
        U16 i;

        for (i = 0; i < MAX_LINKS; i++)
        {
            gnState *tmp = &gn_state[i];

            if ( gThisNode != i )
            {
                if( tmp->alive < curtime - prevtime )
                {
                    if( tmp->alive && !sound_fired )
                    {
                        /* If lost somebody, yell! */
#if ( PRODUCTION_RELEASE == 0 )
                        SOUND( S_YELL );
#endif
                    }
                    tmp->alive = 0;
                }
                else tmp->alive -= ( curtime - prevtime );
            }
        }

        sound_fired = 0;
    }

    prevtime = curtime;
}

#include "dis.h"

#ifdef  GAMENET_DEBUG

/*
 * Gamenet debug/print code
*/
static const char *
flags( U8 flags )
{
    S32 i;
    static char f[9];

    f[0] = flags & LINK_BLINK ? 'b' : ' ';
    f[1] = flags & LINK_TLOCK ? 't' : ' ';
    f[2] = flags & LINK_CLOCK ? 'c' : ' ';
    f[3] = flags & LINK_SOLO  ? 's' : ' ';
    f[4] = flags & LINK_RACERX? 'r' : ' ';
    f[5] = flags & LINK_DRONES? 'd' : ' ';
    f[6] = flags & LINK_CATCHUP?'k' : ' ';
    f[7] = flags & LINK_ACTIVE? 'a' : ' ';
    f[8] = 0;

    return f;
}

static void
gnDebug( void )
{
    int i;
    static char *names[] =
    {
        "Attr", "TSel", "CSel", "Play", "End ",
        "GmOv", "HiSc", "Pply", "Ppl2", "Cdwn"
    };

    /* Don't do anything if not yet initted */
    if( !gs || !(dlevels & SW_DEBUG4)) return;

    for (i = 0; i < MAX_LINKS; i++)
    {
        S32 color;
        char tmp[40];

        sprintf (tmp, "%d%d%s ",
              (int)(gn_state[i].d.track),
              (int)(gn_state[i].rcvtime)&7, names[gn_state[i].d.state]);

        if( !gn_state[i].alive )
        {
            color = RED_PAL;
        }
        else if( GN_IS_ALIVE - gn_state[i].alive > GN_REFRESH_TIME )
        {
            color = YEL_PAL;
        }
        else
        {
            color = GRN_PAL;
        }

        txt_str (i * 8, 0, tmp, color );

        tmp[0] = '0' + ( gn_state[i].cksum.u.u8[0] & 7 );
        tmp[1] = '0' + ( gn_state[i].d.start_time  & 7 );
        tmp[2] = '0' + ((gn_state[i].d.play_time/1000)& 7 );
        tmp[3] = 0;

        txt_str (i * 8, 1, tmp, color );

        sprintf( tmp, "%-10d", ( int ) gnGetCountdownTime( i ) );
        txt_str (i * 8, 2, tmp, color );
        txt_str (i * 8, 3, flags(gn_state[i].d.flags), color );
        txt_str (i * 8, 4, flags(gLink[i].flags), color );
        sprintf( tmp, "%-10d",
          ( int ) (gnGetCountdownTime( i )-GetCountdownTime() ));
        txt_str (i * 8, 5, tmp, color );
        sprintf( tmp, "%d %d      ", ( int ) gn_state[i].pktloss, ( int )
          ( ( 100 * gn_state[i].pktloss ) / ( gn_state[i].seqid + 1 ) ) );
        txt_str (i * 8, 6, tmp, WHT_PAL );
    }
}
#endif

void
gnUpdate (void)
{
#if GAMENET
    /* If init has been called, and static local "gs" is this stack's state */
    if( gs )
    {
        gs->d.state      = gstate;
        gs->d.track      = trackno;
        gs->d.play_time  = GetPlayTime();
        gs->d.start_time = GetStartTime();
        gs->d.flags      = gLink[gThisNode].flags;

        checkPulse ();              /* Update state of who is alive and dead */
        processLSQueue ();  /* process packets queued up by the IRQ callback */
        sendChanged ();   /* Send any state that's changed since last update */

#ifdef GAMENET_DEBUG
        gnDebug(); /* print stuff out as a crutch to the mentally deficient */
#endif
    }
#endif

    if( 0 )
    {
        S32 i;

        for( i=0; i<MAX_LINKS; i++ )
        {
            if( i != gThisNode )
                gLink[i].flags = gn_state[i].d.flags;
        }
    }
}

/* Return stack i's countdown time
*/
S32
gnGetCountdownTime( S32 i )
{
    S32 rval = gn_state[i].d.play_time -
        ( IRQTIME + gn_state[i].irqdiff - gn_state[i].d.start_time );

    return rval > 0 ? rval : 0;
}
