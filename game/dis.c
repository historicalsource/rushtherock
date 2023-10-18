/*
 *  dis.c   -- Functions for handling DIS Protocol.
 *
 *  ======================================================================
 *  $Author: gray $                     $Date: 1997/10/16 02:49:07 $
 *  $Revision: 3.98 $                       $Locker:  $
 *  ======================================================================
 *  Change Log:
 *
 *  $Log: dis.c,v $
 * Revision 3.98  1997/10/16  02:49:07  gray
 * syncronize netTime in net receive ISR.
 *
 * Revision 3.97  1997/10/09  05:11:17  hightower
 * Hopefully fixed solo mode bug
 *
 * Revision 3.96  1997/10/02  21:44:50  hightower
 * Made Join Now message disappear if there's a network problem
 *
 * Revision 3.95  1997/10/01  19:06:31  hightower
 * Fix to make cars all start at same time
 *
 * Revision 3.94  1997/09/26  06:16:11  hightower
 * Now looking at gamenet stuff to determine if Join Now should be displayed
 *
 * Revision 3.93  1997/09/26  00:13:50  hightower
 * Removed gjoin_now in favor of logic that uses gamenet
 *
 * Revision 3.92  1997/09/25  05:53:46  hightower
 * Gamenet functional, but disabled -- to enable define GAMENET to 1 in gamenet.h
 *
 * Revision 3.91  1997/09/25  02:07:40  grjost
 * Fixed tourney mode for 12 tracks, 8 cars, and menus in GUTS.
 *
 * Revision 3.90  1997/09/24  01:09:20  hightower
 * Reformatted
 *
 * Revision 3.89  1997/09/15  22:27:35  gray
 * Removed trackset hack - trackno now 0 to NTRACKS.
 *
 * Revision 3.88  1997/08/22  01:38:13  gray
 * Working on replacing link control with LinkStatus.
 *
 * Revision 3.87  1997/07/15  17:42:33  gray
 * Fixed another hard-coded 3 tracks.
 *
 * Revision 3.86  1997/03/31  03:10:37  gray
 * Nucleus gone - got rid of include and nucleus types.
 *
 * Revision 3.85  1997/03/27  23:56:31  gray
 * dont need half second sleep before Net_Open_Interface any more.
 *
 * Revision 3.84  1997/03/27  23:09:16  gray
 * Changed from nucleus queue to local queue handling.
 *
 * Revision 3.83  1997/03/21  04:44:55  gray
 * Fix join now exit.
 *
 * Revision 3.82  1997/03/20  13:47:20  geraci
 * new message type for use in linking
 *
 * Revision 3.81  1997/03/18  19:43:05  geraci
 * if two stacks have the same mcast channel then both try to find a new one
 * broadcast countdown message supported
 *
 * Revision 3.80  1997/03/13  04:02:41  geraci
 * added multi-cast channel selection and negotiation
 *
 * Revision 3.79  1997/02/28  00:01:08  gray
 * hack to allow solo fall through without everyone falling through.
 *
 * Revision 3.78  1997/02/26  03:40:41  geraci
 * hopefully fixed the 'waiting for others' bug
 *
 * Revision 3.77  1997/02/21  00:07:03  geraci
 * msec parameter removed from tourney message
 *
 * Revision 3.76  1997/02/15  01:45:50  geraci
 * syncs up join now time, tourney tracks each have own laps
 *
 * Revision 3.75  1997/01/17  23:29:45  geraci
 * picks default value for number of laps if value not passed (old version)
 *
 * Revision 3.74  1997/01/15  20:01:41  geraci
 * number of laps for track 2 and 3 can be changed by operator
 *
 * Revision 3.73  1996/12/13  13:25:09  geraci
 * common marker flags
 *
 * Revision 3.72  1996/12/13  01:42:24  geraci
 * marks inactive players correctly
 *
 * Revision 3.71  1996/12/12  08:29:15  geraci
 * players can be dropped out in attract and track select
 *
 * Revision 3.70  1996/12/08  04:44:25  geraci
 * made track select fall through work correctly in all cases
 *
 * Revision 3.69  1996/12/06  19:23:00  geraci
 * ends tracks select if all possible nodes are locked
 *
 * Revision 3.68  1996/12/06  17:29:38  geraci
 * don't move to countdown from tranny select, wait 1/2 sec in select
 *
 * Revision 3.67  1996/12/05  00:03:44  geraci
 * end of game score removed from entity state, drone position fixed
 *
 * Revision 3.66  1996/12/04  11:27:29  geraci
 * randomly choose drone position
 *
 * Revision 3.65  1996/12/04  05:20:59  geraci
 * allows remove from game only during car select
 *
 * Revision 3.64  1996/12/04  01:57:19  geraci
 * checkpoints and end of game scores are handled by entity state updates
 *
 * Revision 3.63  1996/11/28  01:48:19  petrick
 * Turned off debug msgs for production release.
 *
 * Revision 3.62  1996/11/27  23:41:49  petrick
 * Fixed tap-ahead during high score entry bug.
 *
 * Revision 3.61  1996/11/27  12:56:10  gray
 * Changed NetTime from F32 to 10microsec S32.
 *
 * Revision 3.60  1996/11/27  10:22:06  geraci
 * randomize pole positions
 *
 * Revision 3.59  1996/11/27  09:24:10  petrick
 * Changed types of error messages to production types.
 *
 * Revision 3.58  1996/11/27  02:21:43  petrick
 * Deactivated remove from game debug msg.
 *
 * Revision 3.57  1996/11/25  14:19:53  petrick
 * Fix for remove from game.
 *
 * Revision 3.56  1996/11/25  06:21:51  petrick
 * Join messages now send tourney status.
 *
 * Revision 3.55  1996/11/25  04:10:06  petrick
 * Got rid of Continue state.
 *
 * Revision 3.54  1996/11/24  10:56:46  petrick
 * Put some code in to stop tapping through track select if second locker is solo.
 *
 * Revision 3.53  1996/11/22  16:50:38  petrick
 * Doesn't force attract mode switch unless tourney mode has toggled.
 *
 * Revision 3.52  1996/11/22  12:44:28  petrick
 * Changes to get tourney mode squared away.
 *
 * Revision 3.51  1996/11/22  04:59:06  geraci
 * quats passed over link instead of uvs
 *
 * Revision 3.50  1996/11/21  13:45:57  petrick
 * Better rejection of join messages between tourney and non-tourney nodes.
 *
 * Revision 3.49  1996/11/18  12:38:14  petrick
 * Tells what node color is for multiple nodes.
 *
 * Revision 3.48  1996/11/17  14:30:31  petrick
 * Allows tourney join message only while in a state to receive it.
 *
 * Revision 3.47  1996/11/16  09:50:33  petrick
 * Debug stuff.
 *
 * Revision 3.46  1996/11/15  16:17:08  petrick
 * Tourney mode changes.
 *
 * Revision 3.45  1996/11/15  12:02:59  petrick
 * Added duplicate node detection.
 *
 * Revision 3.44  1996/11/15  09:48:56  petrick
 * Turned off more debug.
 *
 * Revision 3.43  1996/11/14  13:19:39  petrick
 * Turned off debug.
 *
 * Revision 3.42  1996/11/13  07:19:16  petrick
 * Made timeout longer when deciding whether to remove someone during countdown.
 *
 * Revision 3.41  1996/11/11  18:55:43  petrick
 * Got rid of 'ready' girl.
 *
 * Revision 3.40  1996/11/11  14:24:57  petrick
 * Turned off debug for release version.
 *
 * Revision 3.39  1996/11/11  10:49:03  petrick
 * External tourney mode starting.
 *
 * Revision 3.38  1996/11/09  13:10:45  petrick
 * Added transition screen.
 *
 * Revision 3.37  1996/11/06  11:04:05  petrick
 * Got rid of some debugging msgs.
 *
 * Revision 3.36  1996/11/05  12:22:55  petrick
 * Code to handle JoinIn and countdown skipping during track select.
 *
 * Revision 3.35  1996/11/02  09:56:32  petrick
 * More packet rejection fixes.
 *
 * Revision 3.34  1996/11/02  04:03:00  geraci
 * transmitted difficulty in car message
 *
 * Revision 3.33  1996/11/02  00:13:14  geraci
 * fix for difficulty data not being sent in some cases
 *
 * Revision 3.32  1996/11/01  23:28:48  petrick
 * Tourney mode change.
 *
 * Revision 3.31  1996/10/30  05:45:43  petrick
 * Got rid of EntityList.
 *
 * Revision 3.30  1996/10/30  04:43:49  petrick
 * Made coin entry during continue reset countdown.
 *
 * Revision 3.29  1996/10/29  08:28:48  petrick
 * Improved rejection of resent msgs.
 *
 * Revision 3.28  1996/10/25  11:06:25  petrick
 * Changes to ack handling.
 *
 * Revision 3.27  1996/10/25  09:43:09  petrick
 * Better Ack rejection.
 *
 * Revision 3.26  1996/10/25  06:47:57  petrick
 * Got rid of shared high score tables.
 *
 * Revision 3.25  1996/10/24  09:23:02  petrick
 * Made countdown timer use lower time during hurry up.
 *
 * Revision 3.24  1996/10/23  11:20:28  petrick
 * Tourney mode changes.
 *
 * Revision 3.23  1996/10/23  00:23:14  petrick
 * Made track_no a global.
 *
 * Revision 3.22  1996/10/22  10:34:19  petrick
 * Tourney mode changes.
 *
 * Revision 3.21  1996/10/18  10:58:30  petrick
 * Improved Dead game recognition, spiffed up Countdown processing.
 *
 * Revision 3.20  1996/10/12  11:12:32  petrick
 * Added timer msg.
 *
 * Revision 3.19  1996/10/10  09:16:34  petrick
 * Changes to handle older version 0x16 packets.
 *
 * Revision 3.18  1996/10/10  05:22:05  petrick
 * Got rid of hack offline/online msgs.
 *
 * Revision 3.17  1996/10/08  07:48:10  petrick
 * Changed debugging priorities around.
 *
 * Revision 3.16  1996/10/07  22:17:23  petrick
 * Turned debug off for release.
 *
 * Revision 3.15  1996/10/07  02:06:31  petrick
 * Added difficulty to track select message.
 *
 * Revision 3.14  1996/10/07  00:25:26  petrick
 * Temporarily turned on debug msgs.
 *
 * Revision 3.13  1996/10/03  09:09:06  petrick
 * Reenabled part of RemoveFromGame.
 *
 * Revision 3.12  1996/10/02  21:18:18  petrick
 * Changed params in PutEventReport PDU call.
 *
 * Revision 3.11  1996/10/02  03:33:48  petrick
 * Changed name of a field to data.
 *
 * Revision 3.10  1996/09/26  00:07:27  petrick
 * Hopefully fot rid of CAR256 bug.
 *
 * Revision 3.9  1996/09/25  02:05:07  petrick
 * Got rid of include file.
 *
 * Revision 3.8  1996/09/24  06:52:42  petrick
 * Temporarily deactivated RemoveFromGame.
 *
 * Revision 3.7  1996/09/24  02:56:14  petrick
 * Changed tracklocked mechanism.
 *
 * Revision 3.6  1996/09/23  10:01:26  gray
 * Reset dead reckon skip counts.
 *
 * Revision 3.5  1996/09/23  07:44:16  petrick
 * Added check for all tracks locked.
 *
 * Revision 3.4  1996/09/23  03:42:54  petrick
 * Got rid of more debugging stuff.
 *
 * Revision 3.3  1996/09/21  04:17:15  petrick
 * Implemented packet dumping, won't remove from game.
 *
 * Revision 3.2  1996/09/19  15:02:18  petrick
 * Changes too massive to document.
 *
 * Revision 3.1  1996/09/09  02:24:52  gray
 * JAMMA version
 *
 *  ======================================================================
 */

#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "net_link.h"
#include "net_smc.h"
#include "game.h"
#include "globals.h"
#include "stdtypes.h"
#include "controls.h"
#include "os_proto.h"
#include "mb.h"
#include "blit.h"
#include "checkpoint.h"
#include "debug.h"
#include "mdrive.h"
#include "attract.h"
#include "resurrect.h"
#include "targets.h"
#include "visuals.h"


#define EXTERN
#include "dis.h"

#include "gamenet.h"


#include "Pro/reckon.pro"
#include "Pro/select.pro"
#include "Pro/pdu.pro"
#include "Pro/checkpoint.pro"
#include "Pro/tourney.pro"
#include "Pro/timint.pro"


/*===============================  DEFINES  =================================*/

#define NETTIME_SYNC 1

#define NO_NETWORK      0
#if NO_NETWORK
#define Net_Send_Packet Jim_Send_Packet
#endif

#define DEBUG_DIS       2       /* Set to >= 1 to enable DIS debug messages in the code */
#define DEBUG_BUFFERS   0       /* 1 = save get/free buffer events with time stamps */

#define QUEUE_MSG_SIZE  (sizeof(void *) / sizeof(U32))
#define MAX_QUE_ENTRIES 256

#define MAX_ACKS        16      /*  Number of msgs to keep around. */
#define MAX_TOTAL_ACKS  (MAX_LINKS * MAX_ACKS)  /*  Total acks possible from all nodes. */
#define ACK_RESEND      200     /* msecs until message is resent. */
#define ACK_EXPIRE      (5*ACK_RESEND)  /* msecs until message expires. */


/*===============================  TYPEDEFS  =================================*/

typedef struct AckQ
{
    Net_Buffer *msg;            /*  The actual message awaiting acks. */
    netadd dest;                /*  Destination address for the packet. */
    U32 ExpireTime;             /*  When the msg will expire if not acknowledged. */
    U32 ResendTime;             /*  When the msg will be resent resent. */
    U32 OriginTime;             /*  When the msg will be resent resent. */
    U32 ack;                    /*  Bitmap of who has ack'd so far. */
}
AckQ;
typedef struct AG_QUEUE
{
    U32 *queue;
    S32 ndx_max;
    S32 ndx_bot;
    S32 ndx_top;
}
AG_QUEUE;


/*==============================  PROTOTYPES  ===============================*/


static S32 DISCallback (Net_CB_Func func, network_descriptor nd, packet_number pkt_no, S32 plen, U32 p1, U32 p2, U32 p3);
static S16 CopyToChain (U8 * pdu, Net_Buffer ** buf, U16 size);
static void SetPDUHeader (PDUHeader * header, PDUType type, U16 length);

static S16 EventReportCallback (PDU * pdu);
static S16 EntityStateCallback (PDU * pdu);
static S16 AcknowledgeCallback (PDU * pdu);
static S16 EntityAppearanceCallback (PDU * pdu);
static S16 EntityMessageCallback (PDU * pdu);

static void CalcPolePositions (void);

static BOOL SetupAck (Net_Buffer * netBuf, netadd dest, PDUType type);
static BOOL AcknowledgeReceive (PDU * msg);
static void ReceiveAck (U32 id, U16 entID, BOOL real);
static void CheckAcks (void);

/*===============================  GLOBALS  =================================*/

static S32 dLastMsg[MAX_LINKS];
static S32 dMsgsRecvd[MAX_LINKS], dMsgsMissed[MAX_LINKS], dMsgList[MAX_LINKS][16];

static char str[80];
static BOOL gDISInitialized;

static AG_QUEUE gPDUqueue, gFastQueue;  /*  Queue of PDUs received,  */
                                    /*  but not yet processed. */
static U32 gPDU_que_block[QUEUE_MSG_SIZE * MAX_QUE_ENTRIES];
static U32 gPDU_que_block2[QUEUE_MSG_SIZE * MAX_QUE_ENTRIES];
static PDUHandler gPDUCallback[NUM_PDUS];       /*  List of callback functions. */

static U32 gAckList[MAX_TOTAL_ACKS];    /*  Timestamps of msgs already ack'd using [index] */
static U8 gNextMsgID;           /*  ID number to use for next outgoing msg. */
static AckQ gAckQ[MAX_ACKS];    /*  List of messages awaiting Acks. */
static netadd gNetAdd[MAX_LINKS];
static BOOL gGotAdd[MAX_LINKS];

static const U16 pktLen[NUM_PDUS] =
{
    0,
    sizeof (EntityStatePDU), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    sizeof (AcknowledgePDU), 0, 0, 0, 0, 0,
    sizeof (EventReportPDU),
    sizeof (MessagePDU), 0, 0, 0, 0, 0, 0,
    sizeof (EntityAppearancePDU), 0
};

#if DEBUG_DIS > 3
static F32 gMinAck[MAX_LINKS], gMaxAck[MAX_LINKS], gAvgAck[MAX_LINKS],
  gAckSamp[MAX_LINKS];
#endif

BOOL gGameInited, mcast_holdoff;
U32 mcast_channel[MAX_LINKS * 2], mcast_channel_time[MAX_LINKS];
U32 current_mcast_channel, trying_mcast_channel, using_mcast_channel;
S32 nettime_synced;

/*===============================  EXTERNS  =================================*/

extern Target gTargetList[];    /*  List of targets,    targets.c   */
extern BOOL gPulse, gEnteringName;
extern Tourney gTourney;
extern S32 all_cars_locked_time;
extern const F32 gVersion;
extern S8 negotiate_track;      /* true = we are in track negotiation phase */
extern U32 negotiate_tdata[MAX_LINKS];  /* save track negotiation data here */
extern U32 negotiate_twant[MAX_LINKS];  /* save what each stack is looking for in link */
extern U32 negotiate_ttime[MAX_LINKS];  /* time each track message was received */
extern U32 negotiate_tphase[MAX_LINKS];         /* each new group of tries should have a different phase number */
extern U32 negotiate_tchan[MAX_LINKS * 2];      /* channel node has control of and is using */
extern S8 negotiate_race;       /* true = we are in race negotiation phase */
extern BOOL re_negotiate;       /* true if we encounter a higher negotiation phase than our own */
extern U8 countdown_state;      /* 0=ready to send 1st msg, 1=sending 1st msg, 2=sending countdown msgs */
extern S32 last_countdown_time; /* timer used for count down */

/* ----------------------------------------------------------------------------- */

#if DEBUG_BUFFERS
U32 buf_tab[10000][2];
S16 buf_index;

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
   *
   *       Save buffer to array
   *         Upon entry: buffer -> pointer to save
   *                     mask -> data to OR with time
   *                     some masks are: 80...get from slow queue (Process Pdus)
   *                                     81...free from slow queue (Process Pdus)
   *                                     C0...get from fast queue (Process Pdus)
   *                                     C1...free from fast queue (Process Pdus)
   *                                     20...get buffer (Send Pdu)
   *                                     21...free buffer (Send Pdu)
   *                                     22...buffer put on Ack queue (Send Pdu)
   *                                     44...buffer resent (CheckAcks)
   *                                     41...free buffer (CheckAcks)
   *                                     69...free buffer (ReceiveAck)
   *                                     10...get buffer (cb filter (call back))
   *                                     11...fail put on queue (cb receive (call back))
   *                                     12...get slow queue (cb receive (call back))
   *                                     14...get fast queue (cb receive (call back))
   *                                     16...fail copy to chain            */

static void
tag_buffer (U32 buffer, U32 mask)
{
    S32 state;
    U32 b_index;

    state = prc_set_ipl (INTS_OFF);
    if ((b_index = buf_index++) >= 10000)
        b_index = buf_index = 0;
    prc_set_ipl (state);

    buf_tab[b_index][0] = buffer;
    buf_tab[b_index][1] = IRQTIME | mask;
}
#define init_buf_index()    buf_index=0;
#else
#define tag_buffer(a,b)     while(0)
#define init_buf_index()    while(0)
#endif


static void
AG_Create_Queue (AG_QUEUE * q, U32 * mem, U32 max)
{
    q->queue = mem;
    q->ndx_max = max;
    q->ndx_bot = q->ndx_top = 0;
}

static S32
AG_Send_To_Queue (AG_QUEUE * q, U32 p)
{
    S32 newtop = q->ndx_top + 1;

    if (newtop >= q->ndx_max)
        newtop = 0;

    if (newtop == q->ndx_bot)
        return 1;               /* queue full. */

    q->queue[q->ndx_top] = p;
    q->ndx_top = newtop;

    return 0;

}

static S32
AG_Receive_From_Queue (AG_QUEUE * q, U32 * p)
{
    S32 ipl = prc_set_ipl (INTS_OFF);

    /* check for empty queue. */
    if (q->ndx_bot == q->ndx_top)
    {
        prc_set_ipl (ipl);
        return 1;
    }

    /* return queue bottom. */
    *p = q->queue[q->ndx_bot++];
    if (q->ndx_bot >= q->ndx_max)
        q->ndx_bot = 0;

    prc_set_ipl (ipl);
    return 0;
}


/*//////////////////////////////////////////////////////////////////////////////
   //  Initializes miniDIS.  Returns error code if unsuccessful.
   ////////////////////////////////////////////////////////////////////////////// */
S16
InitializeDIS (void)
{
    S16 i, j;
    S32 iErr;

    /* XXX - GAMENET -- Eventually call gnInit instead of InitializeDIS. */
    gnInit ();

    gErrOverride = 0;
    gLine = 9;
    gPal = GRN_PAL;
    gErrnum = 1;
    init_buf_index ();

    gPacketsSent = 0;
    gPacketsReject = 0;
    gPacketsRcvd = 0;

    for (i = 0; i < MAX_LINKS; ++i)
    {
        gLink[i].state = INACTIVE;
#if 1
        dLastMsg[i] = -1;
        dMsgsRecvd[i] = dMsgsMissed[i] = 0;
        for (j = 0; j < 16; ++j)
            dMsgList[i][j] = 15;
#endif
    }

    /*  Setup the broadcast channel address. */
    for (i = 0; i < 6; ++i)
        gBroadcast[i] = BROADCAST_COM_CHAN;

    /* Create and setup the PDU queue. */
    AG_Create_Queue (&gPDUqueue, (U32 *) & gPDU_que_block, MAX_QUE_ENTRIES);
    AG_Create_Queue (&gFastQueue, (U32 *) & gPDU_que_block2, MAX_QUE_ENTRIES);

    /*  Setup callbacks. */
    for (i = 0; i < NUM_PDUS; ++i)
        gPDUCallback[i] = 0;
    gPDUCallback[kEntityStatePDU] = EntityStateCallback;
    gPDUCallback[kEventReportPDU] = EventReportCallback;
    gPDUCallback[kAcknowledgePDU] = AcknowledgeCallback;
    gPDUCallback[kEntityAppearancePDU] = EntityAppearanceCallback;
    gPDUCallback[kMessagePDU] = EntityMessageCallback;

    /*  Initialize this machine's Entity ID. */
    gThisEntity.site = 69;      /*  STUB */
    gThisEntity.entity = gThisNode;     /*  STUB */
    gSimID = 0;
    gNetDelay = false;
    gJoinInPossible = 0;

    /*  Install the driver callback functions. */
    gNetDesc = Net_Open_Interface ("SMC0", (net_callback_fn *) & DISCallback,
                                   CB_Will_Filter | CB_Will_Receive |
    /* CB_Will_TXResult | */ CB_Non_Block,
                                   20, NET_PKT_HEADER_SIZE);    /*  20 byte sig, 0 offset */

    if (gNetDesc == 0)
        ReportPDUError (PDU_CANT_INIT);

    /*  Set the communication address to the general multicast address. */
    ResetGameLink ();
    gDISInitialized = true;

    return PDU_NO_ERROR;

}                               /*  end InitializeDIS */


/*//////////////////////////////////////////////////////////////////////////////
   //  Deletes a net buffer packet.
   ////////////////////////////////////////////////////////////////////////////// */
static void
DumpPack (Net_Buffer * p)
{
    S32 result;
    tag_buffer ((U32) p, 0xc1000000);
    result = Net_Free_Buffer (p, NETOPT_NONBLOCK);
    if (result != NETOK)
        ReportPDUError ((S16) PDU_MEM_ERROR, result, (S32) 8);
}                               /* end DumpPack */


/*//////////////////////////////////////////////////////////////////////////////
   //  This is the PDU processing function.  It removes PDUs received up to this
   //  point from the queue one at a time and calls a callback routine to handle
   //  the individual PDU data.
   ////////////////////////////////////////////////////////////////////////////// */
S16
ProcessPDUs (BOOL fast)
{
    PDU *pdu, *next;
    U8 type;
    S16 i, result = 0, ret = PDU_NO_ERROR;      /*  Assume no error. */
    S32 iErr;
    U32 num_entries, m_size;
    Net_Buffer *buf_ptr;
    static Net_Buffer *packs[MAX_LINKS];

    if (!gDISInitialized)
        return ret;

    if (fast)
    {
        /*  Remove PDUs from the queue in order and invoke callback functions to deal with them. */
        while (1)
        {
            if (AG_Receive_From_Queue (&gFastQueue, (U32 *) & buf_ptr))
                break;          /*queue empty */

            tag_buffer ((U32) buf_ptr, 0xc0000000);

            /* Got a message from the queue, keep track of most recent. */
            pdu = (PDU *) & buf_ptr->data;
            type = ((PDUHeader *) pdu->data)->type;
            if (type < NUM_PDUS && gPDUCallback[type])
            {
                EntityStatePDU *p = (EntityStatePDU *) & pdu->data;
                U16 slot = p->entityID.entity;

                /*  If it's a packet of interest save it. */
                if (slot < MAX_LINKS)
                {
                    /*  If there's already a packet in the slot, replace it with this one. */
                    if (packs[slot] != nil)
                        DumpPack (packs[slot]);
                    packs[slot] = buf_ptr;
                }

                /*  Uninteresting packet, should never happen. */
                else
                    DumpPack (buf_ptr);
            }
            else
                MSG ("Evil message recv'd pdu=%p, type=%ld", pdu, (S32) type);
        }

        /*  Now we have the most recent of each packet, process them. */
        for (i = 0; i < MAX_LINKS; ++i)
            if (packs[i])
            {
                pdu = (PDU *) & packs[i]->data;
                type = ((PDUHeader *) pdu->data)->type;
                result = gPDUCallback[type] (pdu);
                if (result != PDU_NO_ERROR)
                    ReportPDUError (ret = result);
                DumpPack (packs[i]);
                packs[i] = nil;
            }
    }
    else
        /*  Remove PDUs from the queue in order and invoke callback functions to deal with them. */
        while (1)
        {
            if (AG_Receive_From_Queue (&gPDUqueue, (U32 *) & buf_ptr))
                break;          /*queue empty */

            tag_buffer ((U32) buf_ptr, 0x80000000);

            /* Got a message from the queue, invoke callback */
            pdu = (PDU *) & buf_ptr->data;
            type = ((PDUHeader *) pdu->data)->type;
            if (type < NUM_PDUS && gPDUCallback[type])
            {
                result = gPDUCallback[type] (pdu);
                if (result != PDU_NO_ERROR)
                    ReportPDUError (ret = result);
            }
            else
                MSG ("Evil message recv'd pdu=%p, type=%ld", pdu, (S32) type);

            tag_buffer ((U32) buf_ptr, 0x81000000);
            result = Net_Free_Buffer (buf_ptr, NETOPT_NONBLOCK);
            if (result != NETOK)
                ReportPDUError ((S16) PDU_MEM_ERROR, (S32) result, (S32) 8);
        }
    return ret;

}                               /*  end ProcessPDUs */

S32 dbgBCastCnt;

/*//////////////////////////////////////////////////////////////////////////////
   //  This is the network callback function.  It gets called with various 'func'
   //  parameters by the network layer to perform the given function on it's behalf.
   //  This function is registered with the network at startup via the
   //  Net_Open_Interface function call.
   //
   //  func    - The function to perform.
   //  nd      - The network descriptor originating this request.
   //  pkt_no  - An internal driver packet number.
   //  plen    - Total length of the packet in bytes.
   //  ...     - Optional parameters.
   ////////////////////////////////////////////////////////////////////////////// */
static S32
DISCallback (Net_CB_Func func, network_descriptor nd,
             packet_number pkt_no, S32 plen, U32 p1, U32 p2, U32 p3)
{
    U32 length, ret;
    U8 *sig_buf;
    PDUType type;
    Net_Buffer **dest, *buffer;
    PDU *pdu;
    PDUHeader *h;
    S32 result;
    S32 err;

    switch (func)
    {
        case CB_Filter:
            /*  This is the PDU filter function.  It is called by the driver to decide
             *  if an incoming packet is acceptable and to relay the packet's size.
             *  This function is responsible for deciding if this packet is desirable,
             *  and for providing a memory buffer to place the packet in.
             *
             *  This func has 3 extra parameters:
             *  length  - length of buffer containing signature bytes.
             *  sig_buf - Buffer containing signature bytes.
             *  dest    - On return, points to a buffer to place the packet in.
             */
            length = p1;
            h = (PDUHeader *) p2;
            dest = (Net_Buffer **) p3;
            type = h->type;

            /* XXX - GAMENET -- Eventually call gnNetCallback directly. */
            if (h->version == NET_VERSION)
                return gnNetCallback (func, nd, pkt_no, plen, p1, p2, p3);

            /*  Check the packet's tags for a packet to reject, like ones with bad header info. */
            if (type > NUM_PDUS || !in (1, length, pktLen[type]) || h->version != DIS_VERSION)
            {
                MSG ("Filtering pkt:, len:%ld(%ld), version:%ld, type:%ld",
                     (long) length, (long) pktLen[type], (long) h->version, (long) type);
                ++gPacketsReject;
                return CB_Drop_Packet;
            }

            /*  Drop Entity States from the local node, or any packet from another game,
               or acknowledgements directed at other nodes. */
            if ((type == kEntityStatePDU && (h->id >> 4) == gThisNode) ||
                (in (100, h->simID, 115) && in (100, gSimID, 115) && h->simID != gSimID))
            {
                ++gPacketsReject;
#if DEBUG_DIS > 2
                if (type != kEntityStatePDU)
                    MSG ("Filtering pkt2: from %ld simID %ld gSimID %ld",
                       (long) (h->id >> 4), (long) h->simID, (long) gSimID);
#endif
                return CB_Drop_Packet;
            }

            if (type == kAcknowledgePDU && ((AcknowledgePDU *) h)->receivingID.entity != gThisNode)
            {
                ++gPacketsReject;
                MSG ("Filtering pkt3:, from %ld",
                     (long) (((AcknowledgePDU *) h)->receivingID.entity));
                return CB_Drop_Packet;
            }

            ret = Net_Get_Buffer (dest, plen + sizeof (PDU), NETOPT_NONBLOCK);

            tag_buffer ((U32) * dest, 0x10000000);

            /*  The packet is acceptable, allocate a buffer to stick it into. */
            if (ret != NETOK)
            {
                /*  This may be inappropriate for an ISR routine. */
                ReportPDUError (PDU_MEM_ERROR, ret, (S32) 2);
                return CB_FAILED;
            }

            /*  Create room at the start of the packet for the PDU info. */
            (*dest)->offset = 0;

            /*  Tell the driver to accept the packet. */
            return CB_Copy_Packet;


        case CB_Receive:
            /*  This is the PDU callback routine that gets called when a packet
             *  has been received from the net and copied from chip memory to
             *  the buffer provided by the above filter function.
             *
             *  The extra parameter to this function is:
             *  buffer  - Pointer to the packet data. */
            buffer = (Net_Buffer *) p1;
            pdu = (PDU *) & buffer->data;
            h = (PDUHeader *) pdu->data;
            type = h->type;

            /* XXX - GAMENET -- Eventually call gnNetCallback directly. */
            if (h->version == NET_VERSION)
                return gnNetCallback (func, nd, pkt_no, plen, p1, p2, p3);

#if NETTIME_SYNC
            if( !nettime_synced && (type == kMessagePDU))
            {
                if( ((MessagePDU *)h)->eventType == Message_BCast_Countdown)
                {
                sync_net_time(0);
                dbgBCastCnt++;
                nettime_synced = 1;
                }
            }
#endif

            /*  Place it in a queue, but which one? */
            if (type == kEntityStatePDU)
            {
                tag_buffer ((U32) buffer, 0x14000000);
                err = AG_Send_To_Queue (&gFastQueue, (U32) buffer);
            }
            else
            {
                tag_buffer ((U32) buffer, 0x12000000);
                err = AG_Send_To_Queue (&gPDUqueue, (U32) buffer);
                if (type != kAcknowledgePDU)    /* or any type that requres ack. */
                {
                    PDUHeader *head = (PDUHeader *) & pdu->data;
                    long id = head->id >> 4;
#if DEBUG_DIS > 2
                    MSG ("PutAck: %ld-%4s, ID 0x%lx type %ld", id, NodeName (id),
                         (long) head->id, (long) type);
#endif
                    PutAcknowledgePDU (pdu->src, (U16) id, head->id, head->type, AckStart, AckOK);
                }
            }

            if (err)
            {
                ReportPDUError (PDU_NUCLEUS_ERROR, err, (S16) 3);
                tag_buffer ((U32) buffer, 0x11000000);
                Net_Free_Buffer (buffer, NETOPT_NONBLOCK);
                return CB_FAILED;
            }
            ++gPacketsRcvd;
            return CB_OK;

        case CB_TXResult:       /* NOTE:  This NEVER occurs.  It's turned off in Net_Initialize */
            /*  This callback occurs when a previous Send has some results to report.

             *  There are 2 extra parameters to this function:
             *  result      - Enumeration of result code.
             *  i_am_free   - If not NULL, buffer being released back to the app. */
            result = p1;
            buffer = (Net_Buffer *) p2;

            if (result != NETOK && result != NETOK_SEND_PENDING && result != NETOK_SEND_COMPLETE)
                ReportPDUError (PDU_SEND_ERROR, (S32) result, (S16) 1);

            /*  Fill in any pdu fields that need initializing. */
            if (buffer)
                return Net_Free_Buffer (buffer, NETOPT_NONBLOCK);

            return NETOK;

        default:
            break;
    }

    /*  Only gets here if the packet wasn't a recognized type. */
    return CB_FAILED;

}                               /* end DISCallback */


/*//////////////////////////////////////////////////////////////////////////////
   //  This is the generic internal PDU sending function.  It just calls the driver
   //  to send the given PDU out onto the wire, then returns a status.
   //
   //  pdu     - The PDU to send.
   //  length  - Length of the PDU in bytes.
   ////////////////////////////////////////////////////////////////////////////// */
S16
SendPDU (PDU * pdu, U16 length, PDUType type, netadd dest, BOOL chain)
{
    S16 ret;
    BOOL useAck;
    PDUHeader *header;
    Net_Buffer *netBuf;

#if DEBUG_DIS > 2
    MSG ("SendPDU: type:%ld, len:%ld dest %lx%lx%lx:%lx%lx%lx", (S32) type, (long) length,
         (long) dest[0], (long) dest[1], (long) dest[2], (long) dest[3], (long) dest[4], (long) dest[5]);
#endif
    if (!chain)
        netBuf = (Net_Buffer *) pdu;
    else if ((ret = CopyToChain ((U8 *) pdu, &netBuf, length)) != PDU_NO_ERROR)
    {
        tag_buffer ((U32) netBuf, 0x16000000);
        return ret;
    }

    tag_buffer ((U32) netBuf, 0x20000000);

    /*  Now the PDU is chained, set the header part of it before sending. */
    pdu = (PDU *) & netBuf->data;
    header = (PDUHeader *) & pdu->data;
    SetPDUHeader (header, type, length);
    if (dest == gBroadcast)
        header->simID = 0;

    useAck = SetupAck (netBuf, dest, type);

    /*  Send the PDU, then check the return status. */
    ++gPacketsSent;
    ret = Net_Send_Packet (gNetDesc, dest, netBuf, NETOPT_NONBLOCK);
    if (ret & NETERR_MASK)
        ReportPDUError (PDU_SEND_ERROR, (S32) ret, (S16) 2);

    /* Delete packet if send completed already. */
    if (useAck)
        tag_buffer ((U32) netBuf, 0x22000000);

    else if (ret != NETOK_SEND_PENDING)
    {
        tag_buffer ((U32) netBuf, 0x21000000);
#if !NO_NETWORK
        ret = Net_Free_Buffer (netBuf, NETOPT_NONBLOCK);
#endif
    }

    return (ret == NETOK || ret == NETOK_SEND_PENDING) ? PDU_NO_ERROR : PDU_SEND_ERROR;

}                               /*  end SendPDU */


/*//////////////////////////////////////////////////////////////////////////////
   //  Takes a variable sized PDU and copies it to a chain of buffers suitable for
   //  sending out to the SMC driver.
   ////////////////////////////////////////////////////////////////////////////// */
static S16
CopyToChain (U8 * pdu, Net_Buffer ** nb, U16 size)
{
    Net_Buffer *buf;
    U8 *bp;
    S32 bytes;
    U32 result;

    /*  Leave room in first buffer for my internal PDU data that needs to be */
    /*  sent, but isn't actually part of a DIS PDU */
    if ((result = Net_Get_Buffer (nb, size, NETOPT_NONBLOCK)) != NETOK)
    {
        ReportPDUError (PDU_MEM_ERROR, result, (S32) 100);
        return PDU_MEM_ERROR;
    }
    buf = *nb;
    buf->offset = NET_PKT_HEADER_SIZE;
    buf->len = 0;
    bytes = NET_BUFFER_SIZE - buf->offset;
    bp = (U8 *) & buf->data[buf->offset];
    while (size > 0)
    {
        /*  Copy buf sized chunks */
        if (bytes > size)
            bytes = size;
        memcpy ((void *) bp, (void *) pdu, bytes);
        buf->len = bytes;

        if ((size -= bytes) <= 0)
            break;

        /* Filled a buffer, advance to next link in chain */
        if (!buf->next)
            if ((result = Net_Get_Buffer (&buf->next, 1, NETOPT_NONBLOCK)) != NETOK)
            {
                ReportPDUError (PDU_MEM_ERROR, result, (S32) 101);
                return PDU_MEM_ERROR;
            }
        buf = buf->next;
        buf->offset = buf->len = 0;
        bp = (U8 *) & buf->data[buf->offset];
        bytes = NET_BUFFER_SIZE;
    }

    /*  All full, send away */
    return PDU_NO_ERROR;

}                               /*  end CopyToChain */



/*//////////////////////////////////////////////////////////////////////////////
   //  Sets the values of an EntityID record.
   ////////////////////////////////////////////////////////////////////////////// */
static void
SetPDUHeader (PDUHeader * header, PDUType type, U16 length)
{
    header->version = DIS_VERSION;
    header->type = type;
    header->timeStamp = GetTimeStamp ();
    header->length = length;
    header->simID = gSimID;
    header->id = (gThisNode << 4);
}                               /*  end SetPDUHeader */


/*//////////////////////////////////////////////////////////////////////////////
   //  Sets the communication channel address.
   ////////////////////////////////////////////////////////////////////////////// */
void
SetComChannel (U8 val)
{
    U8 i;

    /* First release any we may have aquired. If we haven't, then it */
    /* doesn't matter, the function handles it...                    */
    for (i = 0; i < 16; i++)
        Net_Release_MC (gNetDesc, i, gComAddress);

    if (val == BROADCAST_COM_CHAN)
        for (i = 0; i < 6; ++i)
            gComAddress[i] = gBroadcast[i];
    else
    {
        /* A new channel address is needed. Ask for one, based on  */
        /* our node id. The function is limitied to returning one  */
        /* of 10 channels, so only 10 node id's are unique...      */
        Net_Aquire_MC (gNetDesc, val, &gComAddress);
    }
}                               /*  end SetComChannel */


/*//////////////////////////////////////////////////////////////////////////////
   //  Calculate the pole positions for the active cars on the given track,
   //  based on time of selection.
   ////////////////////////////////////////////////////////////////////////////// */
static void
CalcPolePositions (void)
{
    S32 i, j, index, track;
    U8 nextSlot;
    U32 maxtime;
    BOOL found;
    LinkInfo *lp;

    /*  First clear pole positions all around. */
    for (lp = gLink, i = 0; i < MAX_LINKS; ++i, ++lp)
        lp->slot = 0;

    /*  Handle solo special. */
    lp = &gLink[gThisNode];
    if (lp->flags & LINK_SOLO)
    {
        if (lp->flags & LINK_DRONES)
            lp->slot = 7;
        else
            lp->slot = 0;
        return;
    }

    /*  Check pole positions for each track. */
    for (track = 0; track < NTRACKS; ++track)
    {
        /*  Flag all slots for this track. */
        for (lp = gLink, i = 0; i < MAX_LINKS; ++i, ++lp)
            if (lp->track == track && (lp->flags & LINK_ACTIVE) &&
                !(lp->flags & LINK_SOLO))
                lp->slot = MAX_LINKS;

        /*  The local player's slot is always active. */
        if (track == gLink[gThisNode].track)
            gLink[gThisNode].slot = MAX_LINKS;

        /*  Sort the used slots by time. */
        nextSlot = MAX_LINKS - 1;
        for (i = 0; i < MAX_LINKS; ++i)
        {
            index = -1;
            for (lp = gLink, maxtime = j = 0; j < MAX_LINKS; ++j, ++lp)
                if (lp->slot == MAX_LINKS && lp->join_time > maxtime)
                {
                    maxtime = lp->join_time;
                    index = j;
                }
            if (index < 0)
                break;

            /*  Assign the slot to the oldest index. */
            gLink[index].slot = nextSlot--;
        }

    }

}                               /* end CalcPolePositions */


/* ----------------------------------------------------------------------------- */

/*//////////////////////////////////////////////////////////////////////////////
   //  Callback for the EventReportPDU.
   ////////////////////////////////////////////////////////////////////////////// */
static S16
EventReportCallback (PDU * pdu)
{
    U8 i, cnt, num_not_locked, num_in_attract;
    S16 result, laps;
    U32 flags, drone_list, pole_list, node, slot, phase;
    U32 num_items, oflags, msec;
    U16 entityID;
    EventTypeID event_type;
    PDUDataItem item_list[15];  /* Make sure enough items to hold largest msg */

    /*  Parse the packet and abort if not legal */
    result = ParseEventReportPDU (pdu, &entityID, &event_type, &num_items,
                                  item_list);
    if (result != PDU_NO_ERROR)
        return result;

    if (!AcknowledgeReceive (pdu))
        return result;

    /*  Deal with the individual messages */
    node = 0;
    switch (event_type)
    {
        default:
            MSG ("ER: %ld-%4s Unexpected event_type %ld", (long) node, NodeName (node), (long) event_type);
            break;

/*---------------------------------------------------------------------------*/

        case Event_Heartbeat:
            /*  Node is alive. */
            MarkAlive (node = item_list[0].datum, (NodeState) item_list[1].datum);

            /*  Check for node number collisions. */
            if (!gGotAdd[node])
            {
                for (i = 0; i < 6; ++i)
                    gNetAdd[node][i] = pdu->src[i];
                gGotAdd[node] = true;
            }
            else
            {
                for (i = 0; i < 6; ++i)
                    if (gNetAdd[node][i] != pdu->src[i])
                    {
                        SEMSG (6 + node, 5, RED_PALB,
                               "ERROR: Local cabinet %s, but multiple %s cabs detected",
                               NodeName (gThisNode), NodeName (node));
                        break;
                    }
            }

            if (num_items == 3)
                gLink[node].version = (F32) item_list[2].datum * .001;
            else
                gLink[node].version = 1.06;

            gPulse = true;
#if DEBUG_DIS > 2
            MSG ("ER: %ld-%4s Heartbeat: Live nodes: %02lx, state %1ld",
                 (long) node, NodeName (node), (long) gLiveNodes, (long) item_list[1].datum);
#endif
            break;

/*---------------------------------------------------------------------------*/

        case Event_CanPlay:
            /*  Someone just pressed start, respond to tell them this node is active */
            MarkAlive (node = item_list[0].datum, JOINING);
            ResetTourneyStatus (node, item_list[3].datum != 0);

            /*  Ignore this msg if not in track select already or if it's a tournement and
             *  sending node isn't in the tournament, or if not in a tourney and it's a tourney
             *  start message. */
            if (gstate != TRKSEL || TourneyOn () != TourneyNode (node))
                break;

            if (node != gThisNode)
                PutEventReportPDU (Event_CanJoin, 4,
                                   Report_Node, (U32) gThisNode,
                                 Report_Track, (U32) gLink[gThisNode].track,
                                   Report_RaceFlags, (U32) (gLink[gThisNode].flags & LINK_COM_FLAGS),
                                   Report_RaceFlags, (U32) TourneyOn ());

            /*  Fall through to the CanJoin case. */

/*---------------------------------------------------------------------------*/

        case Event_CanJoin: /* other guys changed the track or solo mode */
            /*  The given node is currently selecting the given track, but not locked in */
            MarkAlive (node = item_list[0].datum, JOINING);
            ResetTourneyStatus (node, item_list[3].datum != 0);

            /*  Ignore this msg if not in track select already or if it's a tournement and
             *  sending node isn't in the tournament, or if not in a tourney and it's a tourney
             *  start message. */
            if (gstate != TRKSEL || TourneyOn () != TourneyNode (node))
                break;

            /*  Record settings from this node. */
            gLink[node].track = item_list[1].datum;
            gLink[node].join_time = IRQTIME;
            gLink[node].flags = LINK_ACTIVE | item_list[2].datum;
            if (gLink[node].flags & LINK_TLOCK)
                MarkAlive (node, LOCKED);

            if (gLink[node].flags & LINK_SOLO)
            {
                MarkAlive (node, BUSY);
                if (node != gThisNode)
                    gLink[node].flags &= ~LINK_ACTIVE;
            }
            CalcPolePositions ();
#if DEBUG_DIS > 1
            MSG ("ER: %ld-%4s Can%s   T:%ld P:%ld F:%s", (long) node,
            NodeName (node), (event_type == Event_CanPlay) ? "Play" : "Join",
                 (long) gLink[node].track, (long) gLink[node].slot, FlagName (gLink[node].flags));
#endif
            break;

/*---------------------------------------------------------------------------*/

        case Event_Timer:
            /*  The given node is moving on to car select. */
            msec = item_list[1].datum;
            MarkAlive (node = item_list[0].datum, (msec == 0) ? BUSY : LOCKED);

            /*  Ignore this msg if not in track select already or if it's a tournement and
             *  sending node isn't in the tournament, or if not in a tourney and it's a tourney
             *  start message. */
            if (gstate != ATTRACT || TourneyOn () != TourneyNode (node) || gEnteringName)
                break;

#if DEBUG_DIS > 1
            MSG ("ER: %ld-%4s Timer msec:%ld Join:%02lx", (long) node, NodeName (node),
                 msec, (long) gJoinInPossible);
#endif
            if (msec < GetCountdownTime () && GetCountdownTime () > ONE_SEC / 2)
                SetCountdownTimer (msec);
            break;

/*---------------------------------------------------------------------------*/

        case Event_TrackLocked:
            node = item_list[0].datum;

            if (gLink[node].flags & LINK_SOLO)
                MarkAlive (node, BUSY);
            else
                MarkAlive (node, LOCKED);

            if (negotiate_track > 1)
                break;

#if DEBUG_DIS > 1
            MSG ("JG: %ld-%4s TrackLock T:%ld     F:%s", (long) node, NodeName (node),
                 (long) gLink[node].track, FlagName (gLink[node].flags));
#endif

            if (gstate != TRKSEL || TourneyOn () != TourneyNode (node))
                break;

            /*  The given node has locked in to the given track. */
            /*  Record settings from this node. */
            gLink[node].track = item_list[1].datum;
            gLink[node].flags = item_list[2].datum;

            /*  Remove other solo players from this game, and this solo player
             *  from other games.
             */
            if (gLink[node].flags & LINK_SOLO)
            {
                if (node != gThisNode)
                    gLink[node].flags &= ~LINK_ACTIVE;
                else
                    for (i = 0; i < MAX_LINKS; ++i)
                        if (i != gThisNode)
                            gLink[i].flags &= ~LINK_ACTIVE;
            }
            gLink[node].flags |= LINK_TLOCK;    /*  Track is locked */
            gLink[node].flags &= ~LINK_BLINK;   /*  Turn off the blink flag */

#if DEBUG_DIS > 1
            MSG ("ER: %ld-%4s TrackLock T:%ld     F:%s", (long) node, NodeName (node),
                 (long) gLink[node].track, FlagName (gLink[node].flags));
#endif
            /*  See if all active players are locked into a track. */
            for (i = 0, num_in_attract = 0; i < MAX_LINKS; ++i)
            {
                if (gLink[i].state == IDLE && TourneyOn () == TourneyNode (i) &&
                    !(TourneyOn () && gTourney.ext_start == JOIN_QUICK))
                    num_in_attract++;
            }

            for (i = 0, num_not_locked = 0; i < MAX_LINKS; ++i)
            {
                if (gLink[i].state == JOINING && TourneyOn () == TourneyNode (i))
                    num_not_locked++;
            }

            /*  Only allow tap-through if everybody's locked. */
            if (num_not_locked == 0 || (gLink[gThisNode].flags & LINK_SOLO))
            {
                /* hack to allow solo fall through without everyone falling through. */
                if (gLink[gThisNode].flags & LINK_SOLO)
                    SetCountdownTimer (msec = 0);
                else if ((msec = GetCountdownTime ()) > item_list[3].datum)
                    SetCountdownTimer (msec = item_list[3].datum);

                PutEventReportPDU (Event_Timer, 2, Report_Node, (U32) gThisNode, Report_Time, msec);

                /*  Determine who's in this game. */
                for (gInThisGame = i = 0; i < MAX_LINKS; ++i)
                {
                    if ((gLink[i].flags & LINK_ACTIVE) && (gLink[i].flags & LINK_TLOCK))
                    {
                        if (gLink[i].track == gLink[gThisNode].track)
                            gInThisGame |= 1 << i;
                        else
                            gLink[i].flags &= ~LINK_ACTIVE;
                    }
                }

                gAllTracksLocked = (num_in_attract == 0 && num_not_locked == 0);
            }

            break;

/*---------------------------------------------------------------------------*/

        case Event_Negotiate_Track:
            node = item_list[0].datum;
            if (gstate == ATTRACT)
                MarkAlive (node, BUSY);

            /* we are not negotiating, get out */
            if (negotiate_track == 0 || negotiate_track == 3)
                break;

            /* wrong track, get out */
            if (node != gThisNode &&
                (item_list[1].datum != trackno ||
                 TourneyOn () != TourneyNode (node) ||
                 ((1 << node) & gInThisGame) == 0))
            {
                negotiate_tdata[node] = 0;
                negotiate_ttime[node] = 0;
                negotiate_tphase[node] = 0;
                negotiate_twant[node] = 0;
                negotiate_tchan[node] = 0xff;

                break;
            }

            /* if somebody has started to negotiate, force into negotiation */
            if (negotiate_track == 1)
                negotiate_track = 2;

            phase = item_list[3].datum;

            /* if we're out of sync, ignore data and force a re-negotiation */
            if (phase > negotiate_tphase[gThisNode])
                re_negotiate = true;
            else if (phase == negotiate_tphase[gThisNode])
            {
                /* save data and stamp it */
                negotiate_tdata[node] = item_list[2].datum;
                negotiate_ttime[node] = IRQTIME;
                negotiate_tphase[node] = item_list[3].datum;
                negotiate_twant[node] = item_list[4].datum;
                negotiate_tchan[node] = item_list[5].datum;

#if DEBUG_DIS > 1
                MSG ("%ld-%6s T Negotiate T:%2.2lx D:%2.2lx C:%2ld P:%2ld",
                     (S32) node,
                     NodeName (node),
                     (S32) negotiate_tdata[node],
                     (S32) negotiate_twant[node],
                     (S32) negotiate_tchan[node],
                     (S32) negotiate_tphase[node]);
#endif
            }
            break;

/*---------------------------------------------------------------------------*/

        case Event_CarLocked:
            MarkAlive (node = item_list[0].datum, BUSY);

            if (negotiate_race > 1)
                break;

            if (!(gInThisGame & (1 << node)))
            {
                gLink[node].flags &= ~LINK_ACTIVE;
                break;
            }

            if (gstate != CARSEL || TourneyOn () != TourneyNode (node))
                break;

            /*  The given node has locked in to the given car, and is also reporting
             *  desired option settings. */

            /*  Record settings from this node. */
            gLink[node].car = item_list[1].datum;
            gLink[node].flags = item_list[2].datum | LINK_CLOCK | LINK_ACTIVE;
            gLink[node].difficulty = item_list[3].datum;
            gLink[node].marker_flag = item_list[4].datum;

            if (num_items == 6)
                gLink[node].num_laps = item_list[5].datum;
            else
                gLink[node].num_laps = 0;

            /*  Remove other solo players from this game, and this solo player
             *  from other games. */
            if (gLink[node].flags & LINK_SOLO)
            {
                if (node != gThisNode)
                    gLink[node].flags &= ~LINK_ACTIVE;
                else
                    for (i = 0; i < MAX_LINKS; ++i)
                        if (i != gThisNode)
                            gLink[i].flags &= ~LINK_ACTIVE;
            }

#if DEBUG_DIS > 1
            MSG ("%ld-%4s CarLocked car %ld   F:%s", (long) node, NodeName (node),
                 (long) gLink[node].car, FlagName (gLink[node].flags));
#endif
            /*  See if all cars are locked, and count votes for flag settings */
            flags = LINK_COM_FLAGS & ~LINK_ON_FLAGS;
            oflags = 0;
            for (cnt = i = 0; i < MAX_LINKS; ++i)
                if (gLink[i].flags & LINK_ACTIVE)
                    if (gLink[i].track == gLink[gThisNode].track)
                    {
                        flags &= (gLink[i].flags & ~LINK_ON_FLAGS);
                        oflags |= (gLink[i].flags & LINK_ON_FLAGS);
                        ++cnt;
                    }
                    else
                        gLink[i].flags &= ~LINK_ACTIVE;

            flags = (flags & ~LINK_ON_FLAGS) | (oflags & LINK_ON_FLAGS);

            if (flags & LINK_CLOCK && all_cars_locked_time == 0)
                all_cars_locked_time = IRQTIME;

            break;

/*---------------------------------------------------------------------------*/

        case Event_Negotiate_Race:
            /* we are not negotiating, get out */
            if (negotiate_race == 0 || negotiate_race == 3)
                break;

            node = item_list[0].datum;

            /* wrong track, get out */
            if (node != gThisNode &&
                (item_list[1].datum != trackno ||
                 TourneyOn () != TourneyNode (node) ||
                 ((1 << node) & gInThisGame) == 0))
            {
                negotiate_tdata[node] = 0;
                negotiate_ttime[node] = 0;
                negotiate_tphase[node] = 0;
                negotiate_twant[node] = 0;

                break;
            }

            /* if somebody has started to negotiate, force into negotiation */
            if (negotiate_race == 1)
                negotiate_race = 2;

            phase = item_list[3].datum;

            /* if we're out of sync, ignore data and force a re-negotiation */
            if (phase > negotiate_tphase[gThisNode])
                re_negotiate = true;
            else if (phase == negotiate_tphase[gThisNode])
            {
                /* save data and stamp it */
                negotiate_tdata[node] = item_list[2].datum;
                negotiate_ttime[node] = IRQTIME;
                negotiate_tphase[node] = item_list[3].datum;
                negotiate_twant[node] = item_list[4].datum;

                /*  Record settings from this node. */
                gLink[node].car = item_list[5].datum;
                gLink[node].flags = item_list[6].datum | LINK_CLOCK | LINK_ACTIVE;
                gLink[node].difficulty = item_list[7].datum;
                gLink[node].marker_flag = item_list[8].datum;
                gLink[node].num_laps = item_list[9].datum;

#if DEBUG_DIS > 1
                MSG ("%ld-%6s R Negotiate T:%2.2lx D:%2.2lx P:%ld",
                     (S32) node,
                     NodeName (node),
                     (S32) negotiate_tdata[node],
                     (S32) negotiate_twant[node],
                     (S32) negotiate_tphase[node]);
#endif
            }
            break;

/*---------------------------------------------------------------------------*/

        case Event_Countdown:
            MarkAlive (node = item_list[0].datum, BUSY);

            if (demo_game ||
                !(gInThisGame & (1 << node)) ||
                countdown_state < 2 ||
                item_list[1].datum != trackno ||
            (gstate != CARSEL && gstate != COUNTDOWN && gstate != PREPLAY) ||
                TourneyOn () != TourneyNode (node))
                break;

#if DEBUG_DIS > 1
            if (gCountDown != item_list[2].datum)
                MSG ("ER: %ld-%s Countdown gstate = %s, num %ld", (long) node, NodeName (node),
                     GameState (gstate), (long) num_items);
#endif
            /*  Set the countdown timer. */
            gCountDown = item_list[2].datum;
            flags = item_list[3].datum;

#if 0
            if (GetCountdownTime () > (gCountDown + 1) * ONE_SEC - 1)
                SetCountdownTimer ((gCountDown + 1) * ONE_SEC - 1);
#else
            SetCountdownTimer( gnGetCountdownTime( node ) );
#endif

            break;

/*---------------------------------------------------------------------------*/

        case Event_CheckPoint:  /*  Car has just passed a checkpoint. */
            MarkAlive (node = item_list[0].datum, BUSY);
            if (gstate == PLAYGAME)
            {
                S32 checkpoint = item_list[2].datum;
                slot = item_list[1].datum;
                flags = item_list[3].datum;
#if DEBUG_DIS > 0
                SMSG (1, 1, "ER: %ld-%4s Checkpoint slot:%ld, ckpt:%lx flags:%ld", (long) node, NodeName (node),
                      slot, checkpoint, flags);
#endif
                CarPassedCheckpoint (slot, checkpoint, flags);
            }
            break;

/*---------------------------------------------------------------------------*/

        case Event_GameOver:    /*  Car has just ended game. */
            MarkAlive (node = item_list[0].datum, BUSY);
            if (gstate == PLAYGAME)
            {
                U32 score = item_list[2].datum;
                slot = item_list[1].datum;
                flags = item_list[3].datum;
#if DEBUG_DIS
                MSG ("ER: %ld-%4s GameOver slot:%ld score:%ld flags:%ld", (long) node, NodeName (node),
                     slot, score, flags);
#endif
                CarReportsGameOver (slot, score, flags);
            }
            break;

/*---------------------------------------------------------------------------*/

        case Event_Tourney:     /*   Reports a Tournament setup msg (node, flags, laps) */
            /*  Acceptable at any time. */
            MarkAlive (node = item_list[0].datum, NOCHANGE);

            /*  Get the settings, determine if they're for us. */
            oflags = TourneyOn ();
            flags = item_list[1].datum;
            laps = item_list[2].datum;
            pole_list = (flags >> 16) & 0xff;

#if DEBUG_DIS > 1
            MSG ("ER: %ld-%4s Tourney flags 0x%08lx laps %ld ", (long) node, NodeName (node),
                 flags, (S32) laps);
#endif

            ParseTourneyFlags (flags, laps);

            /*  Kill the clock to force a mode switch. */
            if (TourneyOn () && !oflags)
                SetCountdownTimer (0);
            break;

/*---------------------------------------------------------------------------*/

        case Event_TourneyStart:
            /*  Acceptable at any time. */
            MarkAlive (node = item_list[0].datum, NOCHANGE);

            gTourneyTrack = item_list[1].datum;
            flags = item_list[2].datum;
            gTourneyStart = TourneyOn () && flags && gstate == ATTRACT;
#if DEBUG_DIS > 1
            MSG ("ER: %ld-%4s T.Start flags 0x%08lx Start %ld, track %ld", (long) node, NodeName (node),
                 flags, (S32) gTourneyStart, (S32) gTourneyTrack);
#endif
            if (TourneyOn () && gstate == ATTRACT)
                SetCountdownTimer (0);
            break;

/*---------------------------------------------------------------------------*/

        case Event_Coin:        /*   Reports a coin drop (node) */
            /*  Acceptable at any time. */
            MarkAlive (node = item_list[0].datum, NOCHANGE);

#if DEBUG_DIS > 1
            MSG ("EventReport: %ld-%4s Coin", (long) node, NodeName (node));
#endif
            /*  Reset counter if someone is trying to join. */
            if (gstate == TRKSEL || gstate == ATTRACT)
                SetCountdownTimer (TRK_SELECT_TIME);
            break;
    }

    return result;

}                               /*  end EventReportCallback */

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
   *
   *       Remove node from link
   *         Upon entry: node -> node to make solo                              */

void
make_node_solo (U32 node)
{
    S16 i;

    gLink[node].flags |= LINK_SOLO;

    if (node != gThisNode)
    {
        gLink[node].flags &= ~LINK_ACTIVE;
        gInThisGame &= ~(1 << node);
    }
    else
    {
        for (i = 0; i < MAX_LINKS; ++i)
        {
            if (i != gThisNode)
            {
                gLink[i].flags &= ~LINK_ACTIVE;
            }
        }

        gInThisGame = (1<<node);
    }
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& */

static U32 link_flags, drone_list, pole_list;

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
   *
   *       Have Sim manager send a start countdown message                      */

void
start_countdown (void)
{
    U8 i, j, min, drones[MAX_LINKS], next_pole;
    U32 oflags;
    S32 countdown;
    S16 humans[MAX_LINKS], num_humans, done_flag;
    S16 num_drones, drone_pole[MAX_LINKS];

    if (countdown_state == 0)
    {
        gCountDown = COUNTDOWN_TIME;
        SetCountdownTimer (10 * ONE_SEC);
        last_countdown_time = 0;

        if (find_sim_manager () == gThisNode)
        {
            /* set this data up once */

            /* Count votes for flag settings */
            link_flags = LINK_COM_FLAGS & ~LINK_ON_FLAGS;
            oflags = 0;
            for (i = 0; i < MAX_LINKS; ++i)
                if ((1 << i) & gInThisGame)
                {
                    link_flags &= (gLink[i].flags & ~LINK_ON_FLAGS);
                    oflags |= (gLink[i].flags & LINK_ON_FLAGS);
                }

            link_flags = (link_flags & ~LINK_ON_FLAGS) | (oflags & LINK_ON_FLAGS);

            /* set humans and potential drones */
            for (i = num_humans = num_drones = 0; i < MAX_LINKS; i++)
            {
                /*  Anyone not active is a drone, and anyone not a drone is a human  */
                if (!(drones[i] = !(gLink[i].flags & LINK_ACTIVE)))
                    humans[num_humans++] = i;
                else
                    drone_pole[num_drones++] = i;
            }

            drone_list = pole_list = min = next_pole = 0;
            done_flag = false;
            solo_flag = (num_humans == 1);

            if (link_flags & LINK_DRONES)
            {
                for (i = 0; i < MAX_LINKS; i++)
                {
                    /* non human? */
                    if (drones[i])
                    {
                        /* yes, assign human's stack to control it */
                        drones[i] = humans[min++] + 2;
                        if (min >= num_humans)
                            min = 0;

                        drone_list |= (drones[i] - 2) << (i * 4);
                    }
                    else
                        drone_list |= (i + MAX_LINKS) << (i * 4);
                }
                for (i = 0; i < num_drones; i++)
                {
                    if ((j = Random (num_drones - i)) >= num_drones - i - 1)
                        j = num_drones - i - 1;

                    pole_list |= next_pole++ << (drone_pole[j + i] * 4);

                    drone_pole[j + i] = drone_pole[i];
                }
            }
            else                /*  Not using drones */
                for (i = 0; i < MAX_LINKS; i++)
                    drone_list |= (i + MAX_LINKS) << (i * 4);

            for (i = 0; i < num_humans; i++)
            {
                if ((j = Random (num_humans - i)) >= num_humans - i - 1)
                    j = num_humans - i - 1;

                pole_list |= next_pole++ << (humans[j + i] * 4);

                humans[j + i] = humans[i];
            }

            /*  Set LINK_ACTIVE flag this time so the rest of the countdown data won't be ignored. */
            link_flags |= LINK_ACTIVE;
        }

        countdown_state = 1;
    }

#if 0
    if (TimeOut ())
    {
        countdown_state = 0;
        solo_flag = true;
    }
#endif
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
   *
   *       Have Sim manager send a start countdown message from model task      */

void
send_model_countdown (void)
{
    /*  Start the countdown.  Remember to send the rest of the countdown somewhere else. */
    if (countdown_state == 1 && find_sim_manager () == gThisNode)
    {
        if (last_countdown_time != 0)
        {
            if (IRQTIME - last_countdown_time > ONE_SEC / 5)
                last_countdown_time = 0;
            else
                return;
        }

/*---------------------------------------------------------------------------*/

        /*  Inform everyone else of these decisions. */
        PutEventReportPDU (Message_BCast_Countdown, 6,
                           Report_Node, (U32) gThisNode,
                           Report_Track, (U32) trackno,
                           Report_Count, (U32) gCountDown,
                           Report_RaceFlags, (U32) link_flags,
                           Report_Drones, (U32) drone_list,
                           Report_PolePos, (U32) pole_list);
    }
}


/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
   *
   *       Have Sim manager send a countdown message                            */

void
send_countdown (void)
{
    U8 i, j, min, drones[MAX_LINKS], next_pole;
    U32 flags, oflags, drone_list, pole_list;
    S32 countdown;
    S16 humans[MAX_LINKS], num_humans, done_flag;
    S16 num_drones, drone_pole[MAX_LINKS];


    if (countdown_state == 2)
    {
        gCountDown = COUNTDOWN_TIME;
        SetCountdownTimer ((gCountDown + 1) * ONE_SEC - 1);
        countdown_state = 3;
        last_countdown_time = 0;
    }

    /*  Start the countdown.  Remember to send the rest of the countdown somewhere else. */
    if (find_sim_manager () == gThisNode)
    {
        if (last_countdown_time != 0)
        {
            if (IRQTIME - last_countdown_time > ONE_SEC / 5)
                last_countdown_time = 0;
            else
                return;
        }

/*---------------------------------------------------------------------------*/

        countdown = GetCountdownTime () / ONE_SEC;

        PutEventReportPDU (Event_Countdown, 3,
                           Report_Node, (U32) gThisNode,
                           Report_Track, (U32) trackno,
                           Report_Count, (U32) countdown);
    }
}

/*//////////////////////////////////////////////////////////////////////////////
   //  Callback for the EntityState PDU.
   ////////////////////////////////////////////////////////////////////////////// */
static S16
EntityStateCallback (PDU * pdu)
{
    S16 result = PDU_NO_ERROR;
    EntityStateData d, *c;
    U16 ID, i;
    S32 ipl;
    Target *t;

    /*  Parse the packet and abort if not legal */
    result = ParseEntityStatePDU (pdu, &ID, &d);
    if (result != PDU_NO_ERROR)
        return result;

#if 0
    {
        PDUHeader *ph = (PDUHeader *) pdu->data;
        if (ph->simID != gSimID)
        {
            MSG ("Bad SIMID recvd %ld", (long) ph->simID);
            return result;
        }
    }
#endif
    /*  Copy to the correct location */
    if (!(ID & 0x8000) && model[ID].we_control)
        return result;
    ipl = prc_set_ipl (INTS_OFF);
    if (!(ID & 0x8000))
    {
        c = &carz[ID];
        memcpy ((void *) c, (void *) &d, sizeof (EntityStateData));
        c->fresh = true;
    }
    else
    {
        /*  Handle synchronization problems. ??? */
        ID &= 0x7FFF;
        t = &gTargetList[ID];
        t->type = d.look;
        t->data = d.appearance;
        for (i = 0; i < 3; ++i)
        {
            t->pos[i] = d.pos[i];
            t->vel[i] = d.vel[i];
            make_uvs_from_quat (d.quat, t->uv);
            t->angV[i] = d.angv[i];
        }
    }

    ipl = prc_set_ipl (ipl);
    return result;

}                               /*  end EntityStateCallback */


/*//////////////////////////////////////////////////////////////////////////////
   //  Callback for the EntityState PDU.
   ////////////////////////////////////////////////////////////////////////////// */
static S16
EntityAppearanceCallback (PDU * pdu)
{
    S16 result = PDU_NO_ERROR;
    EntityStateData d, *c;
    U16 ID;
    S32 ipl, node;
    U8 flag;
    U32 appearV, appearA;

    /*  Parse the packet and abort if not legal */
    result = ParseEntityAppearancePDU (pdu, &ID, &flag, &appearV, &appearA);
    if (result != PDU_NO_ERROR)
        return result;

#if DEBUG_DIS > 2
    node = appearV;
    MSG ("Appearance: %ld-%4s ID:0x%lx flag:%ld vis:%ld aud:%ld",
    (long) node, NodeName (node), (long) ID, (long) flag, appearV, appearA);
#endif
    /*  Copy to the correct location */
    if (ID & 0x8000)
        TargetAppearance (ID & 0x7fff, flag, appearV, appearA);
    return result;

}                               /*  end EntityAppearanceCallback */


/*//////////////////////////////////////////////////////////////////////////////
   //  Callback for the Acknowledge PDU.
   ////////////////////////////////////////////////////////////////////////////// */
static S16
AcknowledgeCallback (PDU * pdu)
{
    S16 result = PDU_NO_ERROR;
    U16 entID, i, origin;
    U32 reqID;
    AckFlag ack_flag;
    AckResponse response;
    EntityStateData *esd;

    /*  Parse the packet and abort if not legal */
    result = ParseAcknowledgePDU (pdu, &entID, &reqID, &origin, &ack_flag, &response);
    if (result != PDU_NO_ERROR)
        return result;

    MarkAlive (origin, NOCHANGE);

    ReceiveAck (reqID, origin, true);
    return result;

}                               /*  end AcknowledgeCallback */


/* ----------------------------------------------------------------------------- */
void
ResetGameLink (void)
{
    S16 i, j;
    LinkInfo *lp;

    /*  Init some global variables. */
    gSimManager = 0xFF;         /*  No sim manager yet. */
    gNextRequest = 0;           /*  Keeps track of next request ID to use */
    gNetDelay = false;
    gSimID = 0;
    gAllTracksLocked = false;
    zero_reckon_pcts ();

    /*  Clear out active players. */
    for (lp = gLink, i = 0; i < MAX_LINKS; ++i, ++lp)
    {
        lp->join_time = lp->flags = lp->slot = lp->car = lp->track = 0;
        carz[i].fresh = 0;
        lp->owner = 0xF0;
        if (gInThisGame & (1 << i))
            MarkAlive (i, IDLE);
    }

    /*  Start out accepting broadcasts */
    SetComChannel (BROADCAST_COM_CHAN);
    handle_mcast_channel (Allocate, 0xff);
    Heartbeat (true);
}                               /* end ResetGameLink */


/*//////////////////////////////////////////////////////////////////////////////
   //  Returns the name associated with this stack (debug only).
   ////////////////////////////////////////////////////////////////////////////// */
char *
FlagName (U32 flag)
{
    static char str[9];

    str[0] = (flag & LINK_ACTIVE) ? 'A' : '_';
    str[1] = (flag & LINK_BLINK) ? 'B' : '_';
    str[2] = (flag & LINK_TLOCK) ? 'T' : '_';
    str[3] = (flag & LINK_CLOCK) ? 'C' : '_';
    str[4] = (flag & LINK_SOLO) ? 'S' : '_';
    str[5] = (flag & LINK_RACERX) ? 'X' : '_';
    str[6] = (flag & LINK_DRONES) ? 'D' : '_';
    str[7] = (flag & LINK_CATCHUP) ? 'K' : '_';
    str[8] = 0;

    return str;

}                               /* end FlagName */


/*//////////////////////////////////////////////////////////////////////////////
   //  Called at exit from Track Select to pick a simulation manager.
   ////////////////////////////////////////////////////////////////////////////// */
void
PickSimManager (BOOL use_current)
{
    S16 i;
    U32 channel;

    i = find_sim_manager ();

    if (solo_flag)
        channel = current_mcast_channel;
    else
        channel = negotiate_tchan[i];

    handle_mcast_channel (Allocate, channel);

#if DEBUG_DIS > 1
    MSG ("Sim Manager is %ld-%6s", (long) i, NodeName (i));
#endif

    gSimManager = i;
    gSimID = 100 + channel;
    SetComChannel (channel);

}                               /* end PickSimManager */


/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
   *
   *       Find a sim manager
   *         Upon exit: returned -> node number of appropriate sim manager      */
S16
find_sim_manager (void)
{
    S16 i;

    if (solo_flag)
        i = gThisNode;
    else
    {
        for (i = 0; i < MAX_LINKS; i++)
        {
            if ((1 << i) & gInThisGame)
                break;
        }
    }

    return (i);
}

/*//////////////////////////////////////////////////////////////////////////////
   //  Acknowledgement scheme.
   ////////////////////////////////////////////////////////////////////////////// */


/*//////////////////////////////////////////////////////////////////////////////
   //  Call this periodically from the game loop to send out a heartbeat and check
   //  the pulse of other nodes.
   ////////////////////////////////////////////////////////////////////////////// */
void
Heartbeat (BOOL force)
{
    U16 i;
    U32 state;

    /*  Send out heartbeat for this node, if it's time. */
    if (force || IRQTIME - gLink[gThisNode].heartbeat > ONE_SEC * 3)
    {
        state = gLink[gThisNode].state;
        if (state == INACTIVE)
            state = IDLE;

        PutEventReportPDU (Event_Heartbeat, 3,
                           Report_Node, (U32) gThisNode,
                           Report_State, (U32) state,
                           Report_Flags, (U32) (gVersion * 1000));
#if DEBUG_DIS > 2
        MSG ("Heartbeat: Sent");
#endif
    }

    /*  Check if any other nodes have expired. */
    for (i = 0; i < MAX_LINKS; ++i)
        if ((gLiveNodes & (1 << i)) && (IRQTIME - gLink[i].heartbeat > ONE_SEC * 20))
        {
#if DEBUG_DIS > 2
            MSG ("Heartbeat: %ld-%4s timed out", (long) i, NodeName (i));
#endif
            RemoveFromGame (1, i);
        }

    /*  Check for messages that need resending. */
#if 0
    CheckAcks ();
#endif
}                               /* end Heartbeat */


/*//////////////////////////////////////////////////////////////////////////////
   //  Called whenever a msg is received from a node, simply notes time last
   //  message was received from that node.
   ////////////////////////////////////////////////////////////////////////////// */
void
MarkAlive (U16 node, NodeState state)
{
    U32 mask = 1 << node;

    if (state == INACTIVE)
    {
        gLiveNodes &= ~mask;
        gLink[node].state = INACTIVE;
        ResetJoinIn ();
        return;
    }

    gLink[node].heartbeat = IRQTIME;
    gLiveNodes |= mask;

    if (state != NOCHANGE)
        gLink[node].state = state;

    ResetJoinIn ();
}                               /* end MarkAlive */


/*//////////////////////////////////////////////////////////////////////////////
   //  Call this at Send time to place the message on the Acknowledgement queue.
   ////////////////////////////////////////////////////////////////////////////// */
static BOOL
SetupAck (Net_Buffer * netBuf, netadd dest, PDUType type)
{
    PDU *msg = (PDU *) & netBuf->data;
    AckQ *ack;
    PDUHeader *header = (PDUHeader *) & msg->data;
    U32 thisMsg, msgID, timeStamp, mask, amask, i;
    BOOL useAck;

    return false;

#if NO_NETWORK
    return false;
#else
    useAck = type != kEntityStatePDU && type != kAcknowledgePDU &&
        !(demo_game && type == kEntityAppearancePDU);
    if (!useAck)
        return false;
#endif

    /*  Determine which nodes should respond.   */
    amask = (dest[0] == BROADCAST_COM_CHAN) ? gLiveNodes : gInThisGame;
    if (!amask)
        return false;

    /*  Setup the acknowledgement ID. */
    thisMsg = gNextMsgID;
    gNextMsgID = (gNextMsgID + 1) & 0xF;
    msgID = (gThisNode << 4) | thisMsg;
    ack = &gAckQ[thisMsg];
    timeStamp = IRQTIME;
    header->id = msgID;
    header->timeStamp = timeStamp;

    /*  Make sure this ID is OK. */
    if (gAckQ[thisMsg].msg)
    {
        /*  Somebody hasn't ack'd this msg yet, remove them from the game. */
        for (mask = 1, i = 0; i < MAX_LINKS; ++i, mask <<= 1)
            if (ack->ack & mask)
            {
                ReceiveAck ((i << 4) | thisMsg, (U16) 0xFF, false);
                RemoveFromGame (2, i);
            }
    }

    /*  Place the message on the Ack queue. */
    ack->msg = netBuf;          /* Make a copy of the packet so allocation works OK. */
    memcpy (ack->dest, dest, sizeof (netadd));
    ack->OriginTime = timeStamp;
    ack->ExpireTime = timeStamp + ACK_EXPIRE;
    ack->ResendTime = timeStamp + ACK_RESEND;

    ack->ack = amask;
#if DEBUG_DIS > 0
    if (type == kEntityAppearancePDU)
        MSG ("SetupAck: ID:%02lx chan %02lx mask %02lx type %ld",
             (long) msgID, (long) dest[0], (long) ack->ack, (long) type);
#endif
#if DEBUG_DIS > 3
    CMSG (1 + gThisNode, 25 + thisMsg, BLU_PALB, "%1lx", thisMsg);
    CMSG (2 + gThisNode, 25 + thisMsg, BLU_PALB, "%1lx", (long) (0xf & (ack->dest[0] >> 4)));
#endif
    return true;

}                               /* end SetupAck */


/*//////////////////////////////////////////////////////////////////////////////
   //  Call this at Receive time to acknowledge receipt of a message.
   //  Returns true if this is a new message, false if old.
   ////////////////////////////////////////////////////////////////////////////// */
static BOOL
AcknowledgeReceive (PDU * msg)
{
    PDUHeader *header = (PDUHeader *) & msg->data;
    long id = header->id >> 4;
    long mid = header->id & 0xF, i;

    return true;

    /*  Check if already received this msg. */
    if (gAckList[header->id] == header->timeStamp)
    {
/*      MSG("Multiple msg %ld (%ld) from %ld-%s", mid, (dLastMsg[id]+1L), id, NodeName(id) ); */
        return false;
    }

    /* New msg, acknowledge it. */
    gAckList[header->id] = header->timeStamp;

    if ((dLastMsg[id] + 1L) % 16 != mid && dLastMsg[id] != -1)
    {
        ++dMsgsMissed[id];
#if DEBUG_DIS > 0
        MSG ("Missed msg %ld (%ld) from %ld-%s", mid, (dLastMsg[id] + 1L), id, NodeName (id));
/*   CMSG( 1+id,20,RED_PALB,"%2ld", mid ); */
/*   CMSG( 1+id,43+mid, RED_PALB, "%1lx", mid ); */
/*   CMSG( 1+id,10,GRN_PALB, "%6.2f%", 100.0 * dMsgsMissed[id]/(dMsgsMissed[id]+dMsgsRecvd[id])); */
#endif
    }
    else
    {
        ++dMsgsRecvd[id];
#if DEBUG_DIS > 3
        CMSG (1 + id, 20, CYN_PALB, "%2ld", (long) mid);
        CMSG (1 + id, 10, GRN_PALB, "%6.2f%", 100.0 * dMsgsMissed[id] / (dMsgsMissed[id] + dMsgsRecvd[id]));
#endif
    }
    dLastMsg[id] = mid;
    for (i = 1; i < 16; ++i)
        dMsgList[id][i - 1] = dMsgList[id][i];
    dMsgList[id][15] = mid;
#if DEBUG_DIS > 3
    for (i = 0; i < 16; ++i)
        CMSG (1 + id, 43 + i, RED_PALB, "%1lx", dMsgList[id][i]);
#endif
    return true;
}                               /*  end AcknowledgeReceive */


/*//////////////////////////////////////////////////////////////////////////////
   //  Removes a node from active participation in the current game.
   ////////////////////////////////////////////////////////////////////////////// */
void
RemoveFromGame (S16 where, S16 node)
{
    S32 i, state;
    MODELDAT *m;
    CAR_DATA *gc;

    /*  Can't remove yourself from a game, don't bother with already dead   . */
    if (node == gThisNode || !(gLiveNodes & (1 << node)))
        return;

    if (gstate != CARSEL && gstate != ATTRACT && gstate != TRKSEL)
        return;

#if DEBUG_DIS > 0
    MSG ("RemoveFromGame%ld: %ld-%s  ack mask %02lx -> %02lx", (long) where,
       (long) node, NodeName (node), gLiveNodes, gLiveNodes & ~(1 << node));
#endif
    gLink[node].flags &= ~LINK_ACTIVE;
    gInThisGame &= ~(1 << node);
    MarkAlive (node, INACTIVE);

    state = prc_set_ipl (INTS_OFF);
    num_active_cars = 0;
    for (i = 0; i < MAX_LINKS; i++)
    {
        m = &model[i];
        gc = &game_car[i];

        m->in_game = (gLink[i].flags & LINK_ACTIVE) || (gLink[i].owner < MAX_LINKS);
        if (m->in_game)
            model[num_active_cars++].slot = i;
        else
        {
            m->data_valid = 0;
            m->appearance = 0;
            gc->data_valid = 0;
            gc->appearance = 0;
        }
    }
    prc_set_ipl (state);

}                               /* end RemoveFromGame */


/*//////////////////////////////////////////////////////////////////////////////
   //  Call this when an Ack message gets received.  Handles removal of message
   //  if last ack received.
   ////////////////////////////////////////////////////////////////////////////// */
static void
ReceiveAck (U32 id, U16 origin, BOOL real)
{
    U8 thisMsg, node;
    S16 result;
    F32 dt;
    AckQ *ack;

    /*  Ignore Acks for other nodes. */
    node = id >> 4;
    if (node != gThisNode)
        return;

    /*  Record the Ack. */
    thisMsg = id & 0xF;
    ack = &gAckQ[thisMsg];
    if (origin == 0xff)
        ack->ack = 0;
    else
        ack->ack &= ~(1 << origin);

    /*  See if the last Ack was received. */
    if (!ack->ack)
    {
        if (real)
        {
#if DEBUG_DIS >3
            CMSG (1 + gThisNode, 25 + thisMsg, WHT_PALB, " ");
            dt = IRQTIME - ack->OriginTime;
            if (dt < ACK_EXPIRE)
            {
                if (dt > gMaxAck[origin])
                    gMaxAck[origin] = dt;
                if (dt < gMinAck[origin])
                    gMinAck[origin] = dt;
                gAvgAck[origin] = gAvgAck[origin] * gAckSamp[origin] + dt;
                gAvgAck[origin] /= ++gAckSamp[origin];
                CMSG (1 + origin, 30, YEL_PALB, "%ld %5.0f %4.0f %6.2f %6.0f", (long) origin, gAckSamp[origin],
                      gMinAck[origin], gAvgAck[origin], gMaxAck[origin]);
            }
#if DEBUG_DIS > 1
            MSG ("Ack from: %ld-%4s ID:%lx O:%ld dt:%ld Free!",
               (long) (origin), NodeName (origin), (long) id, (long) origin,
                 (long) (IRQTIME - ack->OriginTime));
#endif
#endif
        }
#if DEBUG_DIS > 3
        else
        {
            CMSG (1 + gThisNode, 25 + thisMsg, RED_PALB, "%1lx", thisMsg);
            CMSG (2 + gThisNode, 25 + thisMsg, RED_PALB, "%1lx", thisMsg);
        }
#endif
        if (ack->msg)
        {
            tag_buffer ((U32) ack->msg, 0x69000000);
            result = Net_Free_Buffer (ack->msg, NETOPT_NONBLOCK);
            if (result != NETOK)
                ReportPDUError ((S16) PDU_MEM_ERROR, (S32) result, (S32) 80);
        }
        ack->msg = nil;
        ack->ResendTime = ack->ExpireTime = 0;
    }
    else if (real)
    {
#if DEBUG_DIS > 3
        CMSG (1 + gThisNode, 25 + thisMsg, YEL_PALB, "%1lx", thisMsg);
        dt = IRQTIME - ack->OriginTime;
        if (dt < ACK_EXPIRE)
        {
            if (dt > gMaxAck[origin])
                gMaxAck[origin] = dt;
            if (dt < gMinAck[origin])
                gMinAck[origin] = dt;
            gAvgAck[origin] = gAvgAck[origin] * gAckSamp[origin] + dt;
            gAvgAck[origin] /= ++gAckSamp[origin];
            CMSG (1 + origin, 30, YEL_PALB, "%ld %5.0f %4.0f %6.2f %6.0f", (long) origin, gAckSamp[origin],
                  gMinAck[origin], gAvgAck[origin], gMaxAck[origin]);
        }
#if DEBUG_DIS > 3
        MSG ("Ack from: %ld-%4s ID:%lx O:%ld Wait:%02lx dt:%ld",
             (long) (origin), NodeName (origin), (long) id, (long) origin, (long) ack->ack,
             (long) (IRQTIME - ack->OriginTime));
#endif
#endif
    }
}                               /* end ReceiveAck */


/*//////////////////////////////////////////////////////////////////////////////
   //  Checks pending ack list and resends message if need be.
   ////////////////////////////////////////////////////////////////////////////// */
static void
CheckAcks (void)
{
    S16 i, node, ret;
    AckQ *ack;

    for (ack = gAckQ, i = 0; i < MAX_ACKS; ++i, ++ack)
        if (ack->msg)
        {
#if 1
            if (gNetDelay)
            {
                ack->ExpireTime = IRQTIME + ACK_EXPIRE;
                ack->ResendTime = IRQTIME + ACK_RESEND;
            }
            else
#endif
            if (IRQTIME > ack->ExpireTime)
            {
                /*  Time has run out for this Ack, remove anyone still not responding. */
                for (node = 0; node < MAX_LINKS; ++node)
                    if ((ack->ack & (1 << node)))
                    {
#if DEBUG_DIS > 0
                        PDU *pdu = (PDU *) & (ack->msg)->data;
                        PDUHeader *header = (PDUHeader *) & pdu->data;
                        MSG ("ChkAck: Expire %ld-%s, ID%lx msk%02lx type%ld chan%02lx", (long) node, NodeName (node),
                             (long) (gThisNode << 4) | i, (long) ack->ack, (long) header->type, (long) ack->dest[0]);
#endif
                        RemoveFromGame (3, node);
                    }

                /*  Free up the ack. */
                tag_buffer ((U32) ack->msg, 0x41000000);
                ret = Net_Free_Buffer (ack->msg, NETOPT_NONBLOCK);
                if (ret != NETOK)
                    ReportPDUError ((S16) PDU_MEM_ERROR, (S32) ret, (S32) 81);
                ack->ack = 0;
                ack->ResendTime = ack->ExpireTime = 0;
                ack->msg = nil;
            }
#if 1
            else if (IRQTIME > ack->ResendTime)
            {
#if DEBUG_DIS > 3
                MSG ("CheckAcks: Resend ID %lx, mask %02lx", (long) (gThisNode << 4) | i, (long) ack->ack);
#endif
                /*  Resend the PDU, then check the return status. */
                ret = Net_Send_Packet (gNetDesc, ack->dest, ack->msg, NETOPT_NONBLOCK);
                if (ret & NETERR_MASK)
                    ReportPDUError (PDU_SEND_ERROR, (S32) ret, (S16) 200);

                tag_buffer ((U32) ack->msg, 0x44000000);
                ack->ResendTime = IRQTIME + ACK_RESEND;
            }
#endif
        }
}                               /*  end CheckAcks */


/*//////////////////////////////////////////////////////////////////////////////
   //  Returns a string describing the current node state.
   ////////////////////////////////////////////////////////////////////////////// */
char *
NodeStat (NodeState state)
{
    switch (state)
    {
        case INACTIVE:
            return "INACTIVE";
        case IDLE:
            return "IDLE    ";
        case JOINING:
            return "JOINING ";
        case LOCKED:
            return "LOCKED  ";
        case BUSY:
            return "BUSY    ";
        case NOCHANGE:
            return "NOCHANGE";
        default:
            return "????????";
    }
    return nil;
}                               /* end NodeStat */


/*//////////////////////////////////////////////////////////////////////////////
   //  Sets the JoinIn flag based on last known node states.
   ////////////////////////////////////////////////////////////////////////////// */
void
ResetJoinIn (void)
{
    BOOL ton = TourneyOn ();
    U32 i;

    gJoinInPossible = 0;

    for (i = 0; i < MAX_LINKS; i++)
    {
        gJoinInPossible |= (TourneyNode (i) == ton &&
                            gn_state[i].alive &&
                            (gn_state[i].d.state == TRKSEL));
    }
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
   *
   *        Handle all multi-cast channel requests and updates
   *          Upon entry: mode -> Initialize = clear and init stuff
   *                      Allocate -> set channel being used for current race
   *                      Do_it -> send out current mcast channel to everyone
   *          Upon exit: returned -> 0xff = no multicast channel assigned
   *                                 0-15 = valid multi-cast channel            */

U32
handle_mcast_channel (S16 mode, U32 force_channel)
{
    S16 i, num_channels;
    U32 channels[16], cur_channel, good_channels[16], alt_channel;
    static S32 last_broadcast_time, trying_time;

    switch (mode)
    {
        case Initialize:
            for (i = 0; i < 8; i++)
            {
                mcast_channel[i] = 0xff;
                mcast_channel[i + 8] = 0xff;
                mcast_channel_time[i] = 0;
            }
            current_mcast_channel = 0xff;
            using_mcast_channel = 0xff;
            trying_mcast_channel = 0xff;
            last_broadcast_time = IRQTIME + (gThisNode + 8) * ONE_SEC / 8;
            trying_time = 0;
            mcast_holdoff = false;
            break;

        case Allocate:
            using_mcast_channel = force_channel;
            break;

        case Do_it:
            if (mcast_holdoff)
                break;

            /* ready to send one out and see if we need to acquire one */
            if (last_broadcast_time == 0)
            {
                /* remove old ones */
                for (i = 0; i < MAX_LINKS; i++)
                {
                    if (mcast_channel_time[i] != 0 &&
                        IRQTIME - mcast_channel_time[i] > 1 * ONE_SEC)
                    {
                        /* clear data from that node */
                        mcast_holdoff = true;
                        mcast_channel[i] = 0xff;
                        mcast_channel[i + 8] = 0xff;
                        mcast_channel_time[i] = 0;
                        mcast_holdoff = false;
                    }
                }

                /* do we need to find one? */
                if (current_mcast_channel != 0xff)
                {
                    /* no, we already have one */
                    alt_channel = using_mcast_channel;

                    /* see if anyone is using ours */
                    for (i = 0; i < MAX_LINKS; i++)
                    {
                        if (i != gThisNode && mcast_channel[i] == current_mcast_channel)
                            break;
                    }

                    if (i != MAX_LINKS)
                    {
                        current_mcast_channel = 0xff;
                        trying_mcast_channel = 0xff;
                        MSG ("Changing Channel");
                    }
                }

                /* do we need to find one? */
                if (current_mcast_channel == 0xff)
                {
                    /* yes */
                    if (trying_mcast_channel == 0xff)
                        /* if this is the first time through pick one from the pool */
                    {
                        mcast_holdoff = true;

                        for (i = 0; i < 16; i++)
                            channels[i] = i;

                        for (i = 0; i < MAX_LINKS * 2; i++)
                            if (mcast_channel[i] < MAX_LINKS)
                                channels[mcast_channel[i]] = 0xff;

                        mcast_holdoff = false;

                        for (i = 0, num_channels = 0; i < 16; i++)
                        {
                            if (channels[i] != 0xff)
                                good_channels[num_channels++] = channels[i];
                        }

                        if (num_channels > 0)
                        {
                            /* now pick a random one */
                            i = Random (num_channels);
                            if (i >= num_channels)
                                i = num_channels - 1;

                            trying_mcast_channel = good_channels[i];
                            MSG ("Trying: %ld", (long) trying_mcast_channel);
                            trying_time = IRQTIME;
                        }
                    }

                    /* see if it's time to decide */
                    if (IRQTIME - trying_time > 2 * ONE_SEC)
                    {
                        /* see if anyone else is on my channel */
                        mcast_holdoff = true;

                        for (i = 0; i < MAX_LINKS; i++)
                        {
                            if (mcast_channel[i] == trying_mcast_channel &&
                                i != gThisNode)
                                break;
                        }

                        mcast_holdoff = false;

                        if (i == MAX_LINKS)
                            /* success */
                        {
                            current_mcast_channel = trying_mcast_channel;
                            MSG ("Got one: %ld", (long) current_mcast_channel);
                        }
                        else
                            /* failure */
                        {
                            trying_mcast_channel = 0xff;
                            trying_time = 0;
                        }
                    }

                    alt_channel = trying_mcast_channel;
                }

                /* tell all what channel we have */
                PutEventReportPDU (Message_MCast_Channel, 3,
                                   Report_Node, (U32) gThisNode,
                              Report_RaceFlags, (U32) current_mcast_channel,
                               Report_RaceFlags, (U32) using_mcast_channel);

                last_broadcast_time = IRQTIME;
            }

            if (IRQTIME - last_broadcast_time > ONE_SEC / 4)
                last_broadcast_time = 0;

            break;
    }

    return (current_mcast_channel);
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
   *
   *       Messages transmitted during model task                               */

static S16
EntityMessageCallback (PDU * pdu)
{
    U8 i, cnt, num_not_locked, num_in_attract;
    S16 result, laps[3];
    U32 flags, drone_list, pole_list, node, slot, phase;
    U32 num_items, oflags, msec;
    U16 entityID;
    EventTypeID event_type;
    PDUDataItem item_list[15];  /* Make sure enough items to hold largest msg */

    /*  Parse the packet and abort if not legal */
    result = ParseEventReportPDU (pdu, &entityID, &event_type, &num_items,
                                  item_list);
    if (result != PDU_NO_ERROR)
        return result;

    /*  Deal with the individual messages */
    node = 0;
    switch (event_type)
    {
        default:
            MSG ("ER: %ld-%4s Unexpected event_type %ld", (long) node, NodeName (node), (long) event_type);
            break;

/*---------------------------------------------------------------------------*/

        case Message_MCast_Channel:
            if (mcast_holdoff)
                break;

            node = item_list[0].datum;
            if (node >= MAX_LINKS)
                break;

            /* get channel node has control of and one it is currently using */
            mcast_channel[node] = item_list[1].datum;
            mcast_channel[node + 8] = item_list[2].datum;
            mcast_channel_time[node] = IRQTIME;

            /* if we're using an mcast channel then node is in a race */
            if (mcast_channel[node + 8] != 0xff)
                gLink[node].state = BUSY;

#if 0
            MSG ("%ld-%6s Mcast channel %ld", (long) node, NodeName (node), (long) mcast_channel[node]);
#endif
            break;

/*---------------------------------------------------------------------------*/

        case Message_BCast_Countdown:
            /* we've seen at least one broadcast message so far */
            node = item_list[0].datum;

            if (gGameInited ||
                demo_game ||
                !(gInThisGame & (1 << node)) ||
                countdown_state != 1 ||
                item_list[1].datum != trackno ||
            (gstate != CARSEL && gstate != COUNTDOWN && gstate != PREPLAY) ||
                TourneyOn () != TourneyNode (node))
                break;

            PickSimManager (solo_flag);

#if DEBUG_DIS > 1
            if (gCountDown != item_list[2].datum)
                MSG ("ER: %ld-%s Countdown gstate = %s, num %ld", (long) node, NodeName (node),
                     GameState (gstate), (long) num_items);
#endif
            /*  Set the countdown timer. */
            flags = item_list[3].datum;

            /*  Deal with the flag settings. */
            drone_list = item_list[4].datum;
            pole_list = item_list[5].datum;
            gUseCatchup = (flags & LINK_CATCHUP) != 0;
#if DEBUG_DIS > 1
            MSG ("Sim ID = %ld, drone_list = %08lx, pole_list: %08lx", (long) gSimID, drone_list, pole_list);
#endif
            /*  Handle drones under this node's control. */
            for (i = 0; i < MAX_LINKS; ++i)
            {
                gLink[i].owner = drone_list & 0xf;
                drone_list >>= 4;
                gLink[i].slot = pole_list & 0xf;
                pole_list >>= 4;
            }

            for (i = 0; i < MAX_LINKS; ++i)
                if (gLink[i].owner == gThisNode)
                {
                    /*  Create a drone and start it under this node's control. */
                    gLink[i].flags = flags;
                }

            /*  Allow the game to use the pole positions and drone settings. */
            gGameInited = true;

            break;

/*---------------------------------------------------------------------------*/

    }

    return result;
}
