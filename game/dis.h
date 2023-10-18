/*
 *  dis.h   -- Functions and definitiions for handling DIS Protocol.
 *
 *  Copyright 1996 Time Warner Interactive.
 *  Unauthorized reproduction, adaptation, distribution, performance or
 *  display of this computer program or the associated audiovisual work
 *  is strictly prohibited.
 *
 *  ======================================================================
 *  $Author: gray $                       $Date: 1997/09/26 09:36:49 $
 *  $Revision: 3.52 $                       $Locker:  $
 *  ======================================================================
 *  Change Log:
 *
 *  $Log: dis.h,v $
 * Revision 3.52  1997/09/26  09:36:49  gray
 * Define bits for mirroring car appearance.
 *
 * Revision 3.51  1997/09/26  00:14:17  hightower
 * Made prettier
 *
 * Revision 3.50  1997/09/25  05:53:46  hightower
 * Gamenet functional, but disabled -- to enable define GAMENET to 1 in gamenet.h
 *
 * Revision 3.49  1997/09/17  08:12:36  hightower
 * Reformatted
 *
 * Revision 3.48  1997/09/13  01:47:24  grjost
 * Fixed for 12 cars.
 *
 * Revision 3.47  1997/03/20  13:47:28  geraci
 * new message type for use in linking
 *
 * Revision 3.46  1997/03/18  19:39:34  geraci
 * new broadcast version of countdown
 *
 * Revision 3.45  1997/03/13  04:01:51  geraci
 * added multi-cast channel selection and negotiation
 *
 * Revision 3.44  1997/02/28  02:54:22  geraci
 * added one second to track lock delay
 *
 * Revision 3.43  1997/02/15  00:47:04  geraci
 * new tourney quick mode implemented
 *
 * Revision 3.42  1997/01/15  20:01:49  geraci
 * number of laps for track 2 and 3 can be changed by operator
 *
 * Revision 3.41  1997/01/15  00:00:40  geraci
 * added number of laps for linked messages
 *
 * Revision 3.40  1996/12/13  12:27:50  geraci
 * added report marker flag
 *
 * Revision 3.39  1996/12/13  12:14:58  geraci
 * added marker flag to link info
 *
 * Revision 3.38  1996/12/13  12:05:30  geraci
 * more new lap stuff
 *
 * Revision 3.37  1996/12/13  01:45:42  geraci
 * removed net bandaid
 *
 * Revision 3.36  1996/12/05  00:03:20  geraci
 * end of game score removed from entity state
 *
 * Revision 3.35  1996/12/04  01:57:12  geraci
 * checkpoints and end of game scores are handled by entity state updates
 *
 * Revision 3.34  1996/11/27  12:56:10  gray
 * Changed NetTime from F32 to 10microsec S32.
 *
 * Revision 3.33  1996/11/24  05:52:26  petrick
 * Got rid of flag girl, added flags.
 *
 * Revision 3.32  1996/11/22  04:57:59  geraci
 * hide resurrecting car defined in look field, quats passed instead of uvs
 *
 * Revision 3.31  1996/11/21  13:44:59  petrick
 * Made select screens 1 seconf longer.
 *
 * Revision 3.30  1996/11/13  07:18:20  petrick
 * Fixed comments for EventTypes.
 *
 * Revision 3.29  1996/11/11  10:49:03  petrick
 * External tourney mode starting.
 *
 * Revision 3.28  1996/11/09  08:52:01  petrick
 * Added flag girl objs.
 *
 * Revision 3.27  1996/11/05  12:22:55  petrick
 * Code to handle JoinIn and countdown skipping during track select.
 *
 * Revision 3.26  1996/11/05  01:35:04  petrick
 * Changed cone to cone1 for consistancy, fixed vanished cone bug.
 *
 * Revision 3.25  1996/11/02  10:57:35  petrick
 * Crowds are supposed to have 4 frames each.
 *
 * Revision 3.24  1996/11/01  23:27:46  petrick
 * Added crowds.
 *
 * Revision 3.23  1996/10/30  05:45:43  petrick
 * Got rid of EntityList.
 *
 * Revision 3.22  1996/10/25  09:42:17  petrick
 * Changed order of some packet data for better Ack rejection.
 *
 * Revision 3.21  1996/10/25  06:47:57  petrick
 * Got rid of shared high score tables.
 *
 * Revision 3.20  1996/10/24  09:22:01  petrick
 * Added multiple tree type objects.
 *
 * Revision 3.19  1996/10/22  10:34:19  petrick
 * Tourney mode changes.
 *
 * Revision 3.18  1996/10/17  08:21:13  petrick
 * Labeled values of PDU types.
 *
 * Revision 3.17  1996/10/12  11:09:53  petrick
 * Got rid of object-based marker, added Timer msg.
 *
 * Revision 3.16  1996/10/11  10:11:54  petrick
 * Added marker flag.
 *
 * Revision 3.15  1996/10/10  09:16:34  petrick
 * Changes to handle older version 0x16 packets.
 *
 * Revision 3.14  1996/10/10  05:22:05  petrick
 * Got rid of hack offline/online msgs.
 *
 * Revision 3.13  1996/10/09  05:45:07  petrick
 * Cleanup.
 *
 * Revision 3.12  1996/10/07  02:06:31  petrick
 * Added difficulty to track select message.
 *
 * Revision 3.11  1996/10/04  06:53:35  petrick
 * Moved some simple macro funcs here.
 *
 * Revision 3.10  1996/10/02  21:18:18  petrick
 * Cleaned out more unused stuff.
 *
 * Revision 3.9  1996/10/02  03:32:58  petrick
 * Added more knockdown targets.
 *
 * Revision 3.8  1996/09/23  08:06:40  petrick
 * Added check for all tracks locked.
 *
 * Revision 3.7  1996/09/21  04:14:32  petrick
 * Added splash and 4 cars.
 *
 * Revision 3.6  1996/09/19  15:00:15  petrick
 * Added Offline message and slot data type.
 *
 * Revision 3.5  1996/09/17  15:07:23  petrick
 * Got rid of some stuff from the hud and cleaned up more.
 *
 * Revision 3.4  1996/09/16  02:15:34  geraci
 * added a car dead flag to entity state's look field
 *
 * Revision 3.3  1996/09/12  10:51:49  petrick
 * Made window reflections permanent.
 *
 * Revision 3.2  1996/09/12  09:57:43  petrick
 * Now passing roadcode across link.
 *
 * Revision 3.1  1996/09/09  02:23:24  gray
 * JAMMA version
 *
 *  ======================================================================
 */

#ifndef _DIS_H_
#define _DIS_H_

#include "config.h"
#include "globals.h"
#include "net_smc.h"
#include "net_link.h"


/*===============================  DEFINES  =================================*/

#define rng(v,l,h)  ( ((v) < (l)) ? (l) : (((v) > (h)) ? (h) : (v)) )
#define min(a,b)    ( ((a) < (b)) ? (a) : (b) )
#define max(a,b)    ( ((a) > (b)) ? (a) : (b) )
#define abs(a)      ( ((a) > 0) ? (a) : -(a) )
#define in(l,m,h)   ( ((l) <= (m)) && ((m) <= (h)) )

#define ONE_SEC             1000L	/*  Whatever units make up one second. */
#define NET_SEC             100000L	/* net timer tics for one second. */
#define TRK_SELECT_TIME     (16 * ONE_SEC)	/*  Counts before track select times out */
#define QCK_SELECT_TIME     ( 5 * ONE_SEC)	/*  Quick select time */
#define CAR_SELECT_TIME     (16 * ONE_SEC)	/*  Counts before car select times out */
#define COUNTDOWN_TIME      3	/*  Number of seconds for countdown to race. */
#define JOIN_ANY            0	/*  Normal tourney join in */
#define JOIN_REMOTE         1	/*  Tourney join on remote button */
#define JOIN_QUICK          2	/*  Remote join in with no pauses */

#ifndef EXTERN
#define EXTERN extern
#endif

#define NULL_DATA           1

#define nil                 ((void *)0)
#define MAX_NAME_LEN        9	/*  Max chars in a high score name */
#define HULK_TYPE           NCARS	/*  Body type of a hulk car. */
#define MAX_ENTITIES        300	/*  Maximum nuber of entities in a game */
#define MAX_DATA_ITEMS      20	/*  Max number of items in any single Data PDU */
#define MAX_TOT_COLLISIONS  32	/*  Max simultaneous collisions possible over all entities */
#define MAX_COLLISIONS      1	/*  Max simultaneous collisons for a single entity */

#define LINK_ACTIVE         0x80	/*  If set, car is active and used */
#define LINK_BLINK          0x40	/*  If set, car should blink */
#define LINK_TLOCK          0x20	/*  If set, track is locked */
#define LINK_CLOCK          0x10	/*  If set, car is locked */
#define LINK_SOLO           0x08	/*  If set, node is playing solo */
#define LINK_RACERX         0x04	/*  If set, drone is racer-X */
#define LINK_DRONES         0x02	/*  If set, drones active */
#define LINK_CATCHUP        0x01	/*  If set, catchup mode desired/active */
#define LINK_COM_FLAGS      0x7F	/*  Mask of flags to send across link */
#define LINK_ON_FLAGS       (LINK_DRONES | LINK_CATCHUP)	/* Flags that are inverted on */
#define DEFAULT_LINK_FLAGS  (LINK_DRONES | LINK_CATCHUP)

#define DIS_VERSION         0x18	/*  miniDIS v1, DIS v7, (little endian). */
#define PDU_ALL_ENTITIES    0xFFFF	/*  Used in Entity ID field to refer to everyone.    */

#define PDU_NO_ERROR        0	/*  Everything's peachy. */
#define PDU_TIMEOUT         -1	/*  Driver timed out. */
#define PDU_PARAM_ERROR     -2	/*  Problem with a PDU parameter detected. */
#define PDU_MEM_ERROR       -3	/*  Couldn't allocate memory. */
#define PDU_TRUNC_ERROR     -4	/*  Buffer allocated for PDU is too small. */
#define PDU_NO_PDU          -5	/*  No more PDUs in the queue. */
#define PDU_CANT_INIT       -6	/*  Can't initialize the network connection. */
#define PDU_SEND_ERROR      -7	/*  Error sending a packet. */
#define PDU_NUCLEUS_ERROR   -8	/*  Problem involving Nucleus */
#define PDU_ASSERT_FAILURE  -9	/*  Assert failed */

#define PDU_VISUAL_BIT      0x20	/*  Bitfields for the Put/Parse  */
#define PDU_AUDIO_BIT       0x02	/*    Entity Appearance flag param. */

/*  These are bitfield masks used in the Entity Appearance visual fields of the  */
/*  Entity Appearance PDU and the Entity State PDU. */
#define App_M_PAINT         0x00000001	/*  1 = team, 0 = normal. (currently unused). */
#define App_M_DAMAGE        0x00000018	/*  Generic damage. (used for hulk and translucency) */
#define App_M_SMOKE         0x00000060	/*  Engine smoke. */
#define App_M_TRAILS        0x00000180	/*  Trailing effects. (Used for L&R sparks) */
#define App_M_HATCH         0x00000C00	/*  Hatch. (Used for bottom sparks) */
#define App_M_LIGHTS        0x00007000	/*  Any Lights. (high bit unused) */
#define  App_M_HEAD_LIGHTS  0x00002000	/*    Head lights.  (unused) */
#define  App_M_BRAKE_LIGHTS 0x00001000	/*    Brake lights. */
#define App_M_FLAMES        0x00008000	/*  Flames rising from entity. (currently unused) */
#define App_M_FROZEN        0x00200000	/*  Entity is frozen. (currently unused) */
#define App_M_DEACTIVATED   0x00400000	/*  Entity is deactivated. (modifed from DIS value) */

/*  The following fields are also defined for certain specific types of entities. */
/*  Car Entities. */
#define App_M_ANY_DAMAGE    0xFFC00000	/*  Any damage to car. */
#define  App_M_LF_DAMAGE    0xC0000000	/*  4 levels, left-front. */
#define  App_M_RF_DAMAGE    0x30000000	/*  4 levels, right-front. */
#define  App_M_LR_DAMAGE    0x0C000000	/*  4 levels, left-rear. */
#define  App_M_RR_DAMAGE    0x03000000	/*  4 levels, right-rear. */
#define  App_M_TOP_DAMAGE   0x00C00000	/*  4 levels, top. */
#define App_M_TIRE_SMOKE    0x000F0000	/*  Any Tire Smoke. */
#define  App_M_LF_SMOKE     0x00080000	/*    Left Front Smoke. */
#define  App_M_RF_SMOKE     0x00040000	/*    Right Front Smoke. */
#define  App_M_LR_SMOKE     0x00020000	/*    Left Rear Smoke */
#define  App_M_RR_SMOKE     0x00010000	/*    Right Rear Tire Smoke. */
#define App_M_SKID_MARK     0x00000606	/*  Any Tire leaving skid marks. */
#define  App_M_LF_SKID      0x00000002	/*    Left Front Skid. */
#define  App_M_RF_SKID      0x00000004	/*    Right Front Skid. */
#define  App_M_LR_SKID      0x00000200	/*    Left Rear Skid */
#define  App_M_RR_SKID      0x00000400	/*    Right Rear Tire Skid. */
#define App_M_SPARKS        0x00000980	/*  Any spark. */
#define  App_M_LSPARK       0x00000100	/*    Left side sparks. */
#define  App_M_RSPARK       0x00000080	/*    Right side sparks. */
#define  App_M_BSPARK       0x00000800	/*    Bottom sparks. */
#define App_M_HULK          0x00000010	/*  Car is a hulk. */
#define App_M_TRANSLUCENT   0x00000008	/*  Car is translucent. */
#define App_M_UNUSED        0x0030E001	/*  bits not currently in use. */

/* The following extracts bits for mirroring appearance right for left. */
#define App_SR2BITS (App_M_LF_DAMAGE|App_M_LR_DAMAGE)
#define App_SL2BITS (App_M_RF_DAMAGE|App_M_RR_DAMAGE)
#define App_SH2BITS (App_SR2BITS|App_SL2BITS)
#define App_SR1BITS (App_M_LF_SMOKE|App_M_LR_SMOKE|App_M_RF_SKID|App_M_RR_SKID|App_M_LSPARK)
#define App_SL1BITS (App_M_RF_SMOKE|App_M_RR_SMOKE|App_M_LF_SKID|App_M_LR_SKID|App_M_RSPARK)
#define App_SH1BITS (App_SR1BITS|App_SL1BITS)
#define App_SHBITS (App_SH2BITS|App_SH1BITS)

#define CAR_TYPE_MASK       0x0f	/*  Mask used for look field to get car type. */
#define CAR_TYPE_OFFSET     0	/*  How far to shift Car type to normalize it. */
#define ENG_TYPE_MASK       0x07	/*  Mask used for size of engine type field. */
#define ENG_TYPE_OFFSET     4	/*  How far to shift engine type to normalize it. */
#define CAR_DEAD_MASK       0x01	/*  Mask used for size of 'car is dead' field. */
#define CAR_DEAD_OFFSET     7	/*  How far to shift 'car is dead' to normalize it. */
#define COL_TYPE_MASK       0x01	/*  Mask used for collidable bit */
#define COL_TYPE_OFFSET     8	/*  Use ninth bit in field for data */
#define HIDE_CAR_MASK       0x01	/*  Mask used for hide car bit */
#define HIDE_CAR_OFFSET     9	/*  Use 10th bit in field for data */
#define ROADCODE_MASK       0xfff	/*  Mask used for road code data */
#define ROADCODE_OFFSET     10	/*  Use 11th-22nd bit for road code data */

/* Bits for mirroring roadcode */
#define Look_LF_ROADCODE (0xe00 << ROADCODE_OFFSET)
#define Look_RF_ROADCODE (0x1c0 << ROADCODE_OFFSET)
#define Look_LR_ROADCODE (0x038 << ROADCODE_OFFSET)
#define Look_RR_ROADCODE (0x007 << ROADCODE_OFFSET)

#define BROADCAST_COM_CHAN  ((U8)0xFF)
#define MULTICAST_COM_CHAN  ((U8)0xed)

/*================================  ENUMS  =================================*/

typedef enum PDUType		/*  Types of PDUs in use.  These are used  */
{				/*  to index into the callback array. */
    kOtherPDU = 0,
    kEntityStatePDU = 1,
    kFirePDU, kDetonationPDU, kCollisionPDU, kServiceRequestPDU, kResupplyOfferPDU,	/* Unused */
    kResupplyReceivedPDU, kResupplyCanceled, kRepairCompletePDU, kRepairResponsePDU,	/* Unused */
    kCreateEntityPDU, kRemoveEntityPDU, kStartPDU, kStopPDU,	/* Unused */
    kAcknowledgePDU = 15,
    kActionRequestPDU, kActionResponsePDU, kDataQueryPDU, kSetDataPDU, kDataPDU,	/* Unused */
    kEventReportPDU = 21,
    kMessagePDU = 22,
    kEMEmmisionPDU, kDesignatorPDU, kTransmitterPDU, kSignalPDU, kReceiverPDU,	/* Unused */
    kNewtonianCollisionPDU,	/* Unused */
    kEntityAppearancePDU = 29,
    kEnvironmentPDU, NUM_PDUS	/* Unused */
}
PDUType;

typedef enum AckFlag		/*  Values for the Acknowledge Flag field  */
{				/*  of the Acknowledge PDU. */
    AckCreate = 1, AckRemove, AckStart, AckStop
}
AckFlag;

typedef enum AckResponse	/*  Values for the Response Flag field  */
{				/*  of the Acknowledge PDU. */
    AckOther = 0, AckOK, AckFail
}
AckResponse;

typedef enum EventTypeID	/*  Values for the event_type parameter  */
{				/*  of the Event Report PDU. */
    /*  These messages only occur during game startup. */
    Event_CanPlay = 0x100,	/*   (256) Node is available to link with (node,track). */
    Event_CanJoin,		/*   (257) Node is available to link with (node,track,carType,RaceFlags). */
    Event_TrackLocked,		/*   (258) Node is locked into a given track (node,track). */
    Event_CarLocked,		/*   (259) Initial race configuration preferences (node,carType,RaceFlags) */
    Event_Countdown,		/*   (260) Seconds until race starts, start race at 0 (seconds,RaceFlags,channel). */
    Event_Timer,		/*   (261) Select Timer is at given value (node, msec). */
    Event_Negotiate_Track,	/*   (262) Send Track negotiation data */
    Event_Negotiate_Race,	/*   (263) Send Race negotiation data */
    /*  These messages occur during play. */
    Event_Highlight,		/*   (264) Turn on/off highlight recording (node, activate). */
    Event_CheckPoint,		/*   (265) Player has passed a checkpoint (node, checkpoint). */
    Event_GameOver,		/*   (266) Player run out of time and has coasted to a stop (node). */
    /*  These messages may occur any time */
    Event_Heartbeat,		/*   (267) Node is still alive (node) */
    Event_Coin,			/*   (268) A coin was received (node) */
    Event_Tourney,		/*   (269) Tourney Setup msg. (node,active,coin,join,track,car,
				   tranny, laps, drones, catchup). */
    Event_TourneyStart,		/*   (270) Tourney has started via external switch (node,track). */
    /*  These messages are passed in Data/SetData/DataQuery's */
    Event_EntityData,		/*   (271) Entity Data follows */
    Event_EntityIDReq,		/*   (272) Request to allocate Entity ID numbers */
    Event_Stats,		/*   (273) Game statistics follow */
    /*  These messages use the same event code but are different PDU type */
    Message_MCast_Channel,	/*   (274) Multicast channel snifter */
    Message_BCast_Countdown	/*   (275) Initial countdown message */
}
EventTypeID;

typedef enum EventReportItem	/*  Values for the DataItems parameter  */
{				/*  of the Event Report PDU. */
    Report_Other = 0,		/*  Unknown EventReport */
    Report_Node,		/*  ( 1) Node number (0-7) */
    Report_Track,		/*  ( 2) Track number (0-2) */
    Report_Channel,		/*  ( 3) Mcast address (0-63) */
    Report_CarType,		/*  ( 4) Car Type (0-3) */
    Report_RaceFlags,		/*  ( 5) Various Race flag settings */
    Report_Count,		/*  ( 6) Seconds for countdown (0-10?) */
    Report_Highlight,		/*  ( 7) Start/End of a highlight section (Boolean)  */
    Report_Score,		/*  ( 8) Score value (0-?) */
    Report_ScoreFlags,		/*  ( 9) Score flags (0=new score, 1=synch) */
    Report_CheckPoint,		/*  (10) Checkpoint number (0-?) */
    Report_Flags,		/*  (11) Checkpoint flags (various) */
    Report_SimID,		/*  (12) Simulation ID (U32) */
    Report_Drones,		/*  (13) Encoded drone responsibilites */
    Report_PolePos,		/*  (14) Pole position of this car (0-7) */
    Report_Time,		/*  (15) Reports a time value. (0-?) */
    Report_Slot,		/*  (16) Reports a slot number. (0-8) */
    Report_Difficulty,		/*  (17) Difficulty factor game option. (0-3) */
    Report_TourneyFlags,	/*  (18) Various tourney mode flags. (various) */
    Report_TourneyLaps,		/*  (19) Laps in tourney race. (0-99). */
    Report_State,		/*  (20) Node State info (0-4) */
    Report_MarkerFlag,		/*  (21) Marker Flag (0-1) */
    Report_NumberLaps,		/*  (22) Number of Laps for track (2-3) or 0 */
    HIGHEST_REPORT_NUM		/*  Keep last, use for error checking. */
}
EventReportItem;

#define CAR_PARTS   29		/*  Number of parts per car. */
#define MAX_DAMAGE  3		/*  Number of damage levels. */

typedef enum DynamicObjName
{				/* Make sure the first entries correspond to Car types above. */
    OBJ_CAR1BODY, OBJ_CAR1WHEEL, OBJ_CAR1BWHEEL, OBJ_CAR1BRAKE,
    OBJ_CAR1WIN0, OBJ_CAR1WIN1, OBJ_CAR1WIN2, OBJ_CAR1WIN3,
    OBJ_CAR1H0L, OBJ_CAR1H0R, OBJ_CAR1H1L, OBJ_CAR1H1R, OBJ_CAR1H2L, OBJ_CAR1H2R,
    OBJ_CAR1Q0D0, OBJ_CAR1Q0D1, OBJ_CAR1Q0D2, OBJ_CAR1Q1D0, OBJ_CAR1Q1D1, OBJ_CAR1Q1D2,
    OBJ_CAR1Q2D0, OBJ_CAR1Q2D1, OBJ_CAR1Q2D2, OBJ_CAR1Q3D0, OBJ_CAR1Q3D1, OBJ_CAR1Q3D2,
    OBJ_CAR1Q4D0, OBJ_CAR1Q4D1, OBJ_CAR1Q4D2,
    OBJ_CAR2BODY, OBJ_CAR2WHEEL, OBJ_CAR2BWHEEL, OBJ_CAR2BRAKE,
    OBJ_CAR2WIN0, OBJ_CAR2WIN1, OBJ_CAR2WIN2, OBJ_CAR2WIN3,
    OBJ_CAR2H0L, OBJ_CAR2H0R, OBJ_CAR2H1L, OBJ_CAR2H1R, OBJ_CAR2H2L, OBJ_CAR2H2R,
    OBJ_CAR2Q0D0, OBJ_CAR2Q0D1, OBJ_CAR2Q0D2, OBJ_CAR2Q1D0, OBJ_CAR2Q1D1, OBJ_CAR2Q1D2,
    OBJ_CAR2Q2D0, OBJ_CAR2Q2D1, OBJ_CAR2Q2D2, OBJ_CAR2Q3D0, OBJ_CAR2Q3D1, OBJ_CAR2Q3D2,
    OBJ_CAR2Q4D0, OBJ_CAR2Q4D1, OBJ_CAR2Q4D2,
    OBJ_CAR3BODY, OBJ_CAR3WHEEL, OBJ_CAR3BWHEEL, OBJ_CAR3BRAKE,
    OBJ_CAR3WIN0, OBJ_CAR3WIN1, OBJ_CAR3WIN2, OBJ_CAR3WIN3,
    OBJ_CAR3H0L, OBJ_CAR3H0R, OBJ_CAR3H1L, OBJ_CAR3H1R, OBJ_CAR3H2L, OBJ_CAR3H2R,
    OBJ_CAR3Q0D0, OBJ_CAR3Q0D1, OBJ_CAR3Q0D2, OBJ_CAR3Q1D0, OBJ_CAR3Q1D1, OBJ_CAR3Q1D2,
    OBJ_CAR3Q2D0, OBJ_CAR3Q2D1, OBJ_CAR3Q2D2, OBJ_CAR3Q3D0, OBJ_CAR3Q3D1, OBJ_CAR3Q3D2,
    OBJ_CAR3Q4D0, OBJ_CAR3Q4D1, OBJ_CAR3Q4D2,
    OBJ_CAR4BODY, OBJ_CAR4WHEEL, OBJ_CAR4BWHEEL, OBJ_CAR4BRAKE,
    OBJ_CAR4WIN0, OBJ_CAR4WIN1, OBJ_CAR4WIN2, OBJ_CAR4WIN3,
    OBJ_CAR4H0L, OBJ_CAR4H0R, OBJ_CAR4H1L, OBJ_CAR4H1R, OBJ_CAR4H2L, OBJ_CAR4H2R,
    OBJ_CAR4Q0D0, OBJ_CAR4Q0D1, OBJ_CAR4Q0D2, OBJ_CAR4Q1D0, OBJ_CAR4Q1D1, OBJ_CAR4Q1D2,
    OBJ_CAR4Q2D0, OBJ_CAR4Q2D1, OBJ_CAR4Q2D2, OBJ_CAR4Q3D0, OBJ_CAR4Q3D1, OBJ_CAR4Q3D2,
    OBJ_CAR4Q4D0, OBJ_CAR4Q4D1, OBJ_CAR4Q4D2,
    OBJ_CAR5BODY, OBJ_CAR5WHEEL, OBJ_CAR5BWHEEL, OBJ_CAR5BRAKE,
    OBJ_CAR5WIN0, OBJ_CAR5WIN1, OBJ_CAR5WIN2, OBJ_CAR5WIN3,
    OBJ_CAR5H0L, OBJ_CAR5H0R, OBJ_CAR5H1L, OBJ_CAR5H1R, OBJ_CAR5H2L, OBJ_CAR5H2R,
    OBJ_CAR5Q0D0, OBJ_CAR5Q0D1, OBJ_CAR5Q0D2, OBJ_CAR5Q1D0, OBJ_CAR5Q1D1, OBJ_CAR5Q1D2,
    OBJ_CAR5Q2D0, OBJ_CAR5Q2D1, OBJ_CAR5Q2D2, OBJ_CAR5Q3D0, OBJ_CAR5Q3D1, OBJ_CAR5Q3D2,
    OBJ_CAR5Q4D0, OBJ_CAR5Q4D1, OBJ_CAR5Q4D2,
    OBJ_CAR6BODY, OBJ_CAR6WHEEL, OBJ_CAR6BWHEEL, OBJ_CAR6BRAKE,
    OBJ_CAR6WIN0, OBJ_CAR6WIN1, OBJ_CAR6WIN2, OBJ_CAR6WIN3,
    OBJ_CAR6H0L, OBJ_CAR6H0R, OBJ_CAR6H1L, OBJ_CAR6H1R, OBJ_CAR6H2L, OBJ_CAR6H2R,
    OBJ_CAR6Q0D0, OBJ_CAR6Q0D1, OBJ_CAR6Q0D2, OBJ_CAR6Q1D0, OBJ_CAR6Q1D1, OBJ_CAR6Q1D2,
    OBJ_CAR6Q2D0, OBJ_CAR6Q2D1, OBJ_CAR6Q2D2, OBJ_CAR6Q3D0, OBJ_CAR6Q3D1, OBJ_CAR6Q3D2,
    OBJ_CAR6Q4D0, OBJ_CAR6Q4D1, OBJ_CAR6Q4D2,
    OBJ_CAR7BODY, OBJ_CAR7WHEEL, OBJ_CAR7BWHEEL, OBJ_CAR7BRAKE,
    OBJ_CAR7WIN0, OBJ_CAR7WIN1, OBJ_CAR7WIN2, OBJ_CAR7WIN3,
    OBJ_CAR7H0L, OBJ_CAR7H0R, OBJ_CAR7H1L, OBJ_CAR7H1R, OBJ_CAR7H2L, OBJ_CAR7H2R,
    OBJ_CAR7Q0D0, OBJ_CAR7Q0D1, OBJ_CAR7Q0D2, OBJ_CAR7Q1D0, OBJ_CAR7Q1D1, OBJ_CAR7Q1D2,
    OBJ_CAR7Q2D0, OBJ_CAR7Q2D1, OBJ_CAR7Q2D2, OBJ_CAR7Q3D0, OBJ_CAR7Q3D1, OBJ_CAR7Q3D2,
    OBJ_CAR7Q4D0, OBJ_CAR7Q4D1, OBJ_CAR7Q4D2,
    OBJ_CAR8BODY, OBJ_CAR8WHEEL, OBJ_CAR8BWHEEL, OBJ_CAR8BRAKE,
    OBJ_CAR8WIN0, OBJ_CAR8WIN1, OBJ_CAR8WIN2, OBJ_CAR8WIN3,
    OBJ_CAR8H0L, OBJ_CAR8H0R, OBJ_CAR8H1L, OBJ_CAR8H1R, OBJ_CAR8H2L, OBJ_CAR8H2R,
    OBJ_CAR8Q0D0, OBJ_CAR8Q0D1, OBJ_CAR8Q0D2, OBJ_CAR8Q1D0, OBJ_CAR8Q1D1, OBJ_CAR8Q1D2,
    OBJ_CAR8Q2D0, OBJ_CAR8Q2D1, OBJ_CAR8Q2D2, OBJ_CAR8Q3D0, OBJ_CAR8Q3D1, OBJ_CAR8Q3D2,
    OBJ_CAR8Q4D0, OBJ_CAR8Q4D1, OBJ_CAR8Q4D2,

    OBJ_CAR9BODY, OBJ_CAR9WHEEL, OBJ_CAR9BWHEEL, OBJ_CAR9BRAKE,
    OBJ_CAR9WIN0, OBJ_CAR9WIN1, OBJ_CAR9WIN2, OBJ_CAR9WIN3,
    OBJ_CAR9H0L, OBJ_CAR9H0R, OBJ_CAR9H1L, OBJ_CAR9H1R, OBJ_CAR9H2L, OBJ_CAR9H2R,
    OBJ_CAR9Q0D0, OBJ_CAR9Q0D1, OBJ_CAR9Q0D2, OBJ_CAR9Q1D0, OBJ_CAR9Q1D1, OBJ_CAR9Q1D2,
    OBJ_CAR9Q2D0, OBJ_CAR9Q2D1, OBJ_CAR9Q2D2, OBJ_CAR9Q3D0, OBJ_CAR9Q3D1, OBJ_CAR9Q3D2,
    OBJ_CAR9Q4D0, OBJ_CAR9Q4D1, OBJ_CAR9Q4D2,

    OBJ_CAR10BODY, OBJ_CAR10WHEEL, OBJ_CAR10BWHEEL, OBJ_CAR10BRAKE,
    OBJ_CAR10WIN0, OBJ_CAR10WIN1, OBJ_CAR10WIN2, OBJ_CAR10WIN3,
    OBJ_CAR10H0L, OBJ_CAR10H0R, OBJ_CAR10H1L, OBJ_CAR10H1R, OBJ_CAR10H2L, OBJ_CAR10H2R,
    OBJ_CAR10Q0D0, OBJ_CAR10Q0D1, OBJ_CAR10Q0D2, OBJ_CAR10Q1D0, OBJ_CAR10Q1D1, OBJ_CAR10Q1D2,
    OBJ_CAR10Q2D0, OBJ_CAR10Q2D1, OBJ_CAR10Q2D2, OBJ_CAR10Q3D0, OBJ_CAR10Q3D1, OBJ_CAR10Q3D2,
    OBJ_CAR10Q4D0, OBJ_CAR10Q4D1, OBJ_CAR10Q4D2,

    OBJ_CAR11BODY, OBJ_CAR11WHEEL, OBJ_CAR11BWHEEL, OBJ_CAR11BRAKE,
    OBJ_CAR11WIN0, OBJ_CAR11WIN1, OBJ_CAR11WIN2, OBJ_CAR11WIN3,
    OBJ_CAR11H0L, OBJ_CAR11H0R, OBJ_CAR11H1L, OBJ_CAR11H1R, OBJ_CAR11H2L, OBJ_CAR11H2R,
    OBJ_CAR11Q0D0, OBJ_CAR11Q0D1, OBJ_CAR11Q0D2, OBJ_CAR11Q1D0, OBJ_CAR11Q1D1, OBJ_CAR11Q1D2,
    OBJ_CAR11Q2D0, OBJ_CAR11Q2D1, OBJ_CAR11Q2D2, OBJ_CAR11Q3D0, OBJ_CAR11Q3D1, OBJ_CAR11Q3D2,
    OBJ_CAR11Q4D0, OBJ_CAR11Q4D1, OBJ_CAR11Q4D2,

    OBJ_CAR12BODY, OBJ_CAR12WHEEL, OBJ_CAR12BWHEEL, OBJ_CAR12BRAKE,
    OBJ_CAR12WIN0, OBJ_CAR12WIN1, OBJ_CAR12WIN2, OBJ_CAR12WIN3,
    OBJ_CAR12H0L, OBJ_CAR12H0R, OBJ_CAR12H1L, OBJ_CAR12H1R, OBJ_CAR12H2L, OBJ_CAR12H2R,
    OBJ_CAR12Q0D0, OBJ_CAR12Q0D1, OBJ_CAR12Q0D2, OBJ_CAR12Q1D0, OBJ_CAR12Q1D1, OBJ_CAR12Q1D2,
    OBJ_CAR12Q2D0, OBJ_CAR12Q2D1, OBJ_CAR12Q2D2, OBJ_CAR12Q3D0, OBJ_CAR12Q3D1, OBJ_CAR12Q3D2,
    OBJ_CAR12Q4D0, OBJ_CAR12Q4D1, OBJ_CAR12Q4D2,

    OBJ_HULK,
    OBJ_FINISH, OBJ_CHECKPOINT,
    OBJ_NEEDLE, OBJ_REDPOST, OBJ_GREENPOST, OBJ_BLUEPOST, OBJ_UVARROWS,
    OBJ_BLAST01, OBJ_BLAST02, OBJ_BLAST03, OBJ_BLAST04, OBJ_BLAST05, OBJ_BLAST06,
    OBJ_BLAST07, OBJ_BLAST08, OBJ_BLAST09, OBJ_BLAST10, OBJ_BLAST11, OBJ_BLAST12,
    OBJ_BLAST13, OBJ_BLAST14, OBJ_BLAST15,
    OBJ_SPARKS,
    OBJ_SMOKE01, OBJ_SMOKE02, OBJ_SMOKE03, OBJ_SMOKE04,
    OBJ_SMOKE05, OBJ_SMOKE06, OBJ_SMOKE07, OBJ_SMOKE08,
    OBJ_SMOKE09, OBJ_SMOKE10, OBJ_SMOKE11, OBJ_SMOKE12,
    OBJ_SMOKE13, OBJ_SMOKE14, OBJ_SMOKE15, OBJ_SMOKE16,
    OBJ_FSMOKE01, OBJ_FSMOKE02, OBJ_FSMOKE03, OBJ_FSMOKE04,
    OBJ_FSMOKE05, OBJ_FSMOKE06, OBJ_FSMOKE07, OBJ_FSMOKE08,
    OBJ_FSMOKE09, OBJ_FSMOKE10, OBJ_FSMOKE11, OBJ_FSMOKE12,
    OBJ_FSMOKE13, OBJ_FSMOKE14, OBJ_FSMOKE15, OBJ_FSMOKE16,
    OBJ_BSMOKE01, OBJ_BSMOKE02, OBJ_BSMOKE03, OBJ_BSMOKE04,
    OBJ_BSMOKE05, OBJ_BSMOKE06, OBJ_BSMOKE07, OBJ_BSMOKE08,
    OBJ_BSMOKE09, OBJ_BSMOKE10, OBJ_BSMOKE11, OBJ_BSMOKE12,
    OBJ_BSMOKE13, OBJ_BSMOKE14, OBJ_BSMOKE15, OBJ_BSMOKE16,
    OBJ_DUST01, OBJ_DUST02, OBJ_DUST03, OBJ_DUST04,
    OBJ_DUST05, OBJ_DUST06, OBJ_DUST07, OBJ_DUST08,
    OBJ_DUST09, OBJ_DUST10, OBJ_DUST11, OBJ_DUST12,
    OBJ_DUST13, OBJ_DUST14, OBJ_DUST15, OBJ_DUST16,
    OBJ_SPLASH01, OBJ_SPLASH02, OBJ_SPLASH03, OBJ_SPLASH04,
    OBJ_SPLASH05, OBJ_SPLASH06, OBJ_SPLASH07, OBJ_SPLASH08,
    OBJ_SPLASH09, OBJ_SPLASH10, OBJ_SPLASH11, OBJ_SPLASH12,
    OBJ_SPLASH13, OBJ_SPLASH14, OBJ_SPLASH15, OBJ_SPLASH16,
    NUM_DYN_OBJS
}
DynamicObjName;

typedef enum TrackObjName
{
    OBJ_CONE1 = NUM_DYN_OBJS, OBJ_CONE2,
    OBJ_METER1, OBJ_METER2, OBJ_METER3, OBJ_METER4, OBJ_METER5,
    OBJ_TREEA1, OBJ_TREEA2, OBJ_TREEA3, OBJ_TREEA4, OBJ_TREEA5, OBJ_TREEA6,
    OBJ_TREEB1, OBJ_TREEB2, OBJ_TREEB3,
    OBJ_TREEC1, OBJ_TREEC2, OBJ_TREEC3,
    OBJ_TREED1, OBJ_TREED2, OBJ_TREED3,
    OBJ_TREEE1, OBJ_TREEE2, OBJ_TREEE3,
    OBJ_WINDOWA1, OBJ_WINDOWA2, OBJ_WINDOWA3, OBJ_WINDOWA4,
    OBJ_WINDOWA5, OBJ_WINDOWA6, OBJ_WINDOWA7,
    OBJ_WINDOWB1, OBJ_WINDOWB2, OBJ_WINDOWB3, OBJ_WINDOWB4,
    OBJ_WINDOWB5, OBJ_WINDOWB6, OBJ_WINDOWB7,
    OBJ_TLIGHT1, OBJ_TLIGHT2,
    OBJ_SLIGHT1, OBJ_SLIGHT2,
    OBJ_POLE1, OBJ_POLE2,
    OBJ_FENCE1, OBJ_FENCE2, OBJ_FENCE3, OBJ_FENCE4, OBJ_FENCE5, OBJ_FENCE6,
    OBJ_FENCE7, OBJ_FENCE8, OBJ_FENCE9, OBJ_FENCE10, OBJ_FENCE11, OBJ_FENCE12,
    OBJ_CROWDA1, OBJ_CROWDA2, OBJ_CROWDA3, OBJ_CROWDA4,
    OBJ_CROWDB1, OBJ_CROWDB2, OBJ_CROWDB3, OBJ_CROWDB4,
    OBJ_CROWDC1, OBJ_CROWDC2, OBJ_CROWDC3, OBJ_CROWDC4,
    OBJ_FLAGA1, OBJ_FLAGA2, OBJ_FLAGA3, OBJ_FLAGA4, OBJ_FLAGA5,
    OBJ_FLAGA6, OBJ_FLAGA7, OBJ_FLAGA8, OBJ_FLAGA9, OBJ_FLAGA10,
    OBJ_FLAGB1, OBJ_FLAGB2, OBJ_FLAGB3, OBJ_FLAGB4, OBJ_FLAGB5,
    OBJ_FLAGB6, OBJ_FLAGB7, OBJ_FLAGB8, OBJ_FLAGB9, OBJ_FLAGB10,
    OBJ_FLAGC1, OBJ_FLAGC2, OBJ_FLAGC3, OBJ_FLAGC4, OBJ_FLAGC5,
    OBJ_FLAGC6, OBJ_FLAGC7, OBJ_FLAGC8, OBJ_FLAGC9, OBJ_FLAGC10,
    MAX_TRK_OBJS
}
TrackObjName;

#define NUM_TRK_OBJS (MAX_TRK_OBJS - NUM_DYN_OBJS)

typedef enum NodeState
{
    INACTIVE = 0,
    IDLE,
    JOINING,
    LOCKED,
    BUSY,
    NOCHANGE,
    NUM_NODESTATES
}
NodeState;

/*===============================  TYPEDEFS  ================================*/

typedef F32 F32x3[3], F32x6[6], F32x3x3[3][3];
typedef F64 F64x3[3];

typedef struct LinkInfo
{
    U8 track;			/* Current track selection. */
    U8 car;			/* Current car selection. */
    U8 slot;			/* Current pole position. */
    U8 flags;			/* Drones, catchup, active, blink flags */
    U8 owner;
    S8 difficulty;		/* Difficulty setting from Game Options. */
    S8 marker_flag;		/* Marker flag (1=markers enabled) */
    S8 num_laps;		/* Number of laps for track */
    U32 join_time;		/* Local time of last CanJoin/CanPlay recv'd. */
    U32 heartbeat;		/* Local time of last msg of any type recv'd. */
    F32 version;		/* Version number of stack */
    NodeState state;
    MODELDAT *model;		/* Points to model. */
}
LinkInfo;

typedef struct EntityStateData
{				/*  Data involved with EntityState PDUs */
    F32x3 pos, vel, acc, angv;	/*  Position, velocity, acceleration, angular velocity */
    F32 quat[4];		/*  quaternion version of uvs */
    S16 torque;			/*  Engine torque */
    U16 rpm;			/*  Engine Rpm */
    F32 steerangle;		/*  Tire angle. */
    F32 suscomp[4];		/*  Tire compression (vert offset) */
    F32 tireV[4];		/*  Tire angular velocity. */
    U32 look;			/*  Save engine type here */
    U32 appearance;		/*  Visual appearance flags. */
    S32 timeStamp;		/*  Time stamp when data was created. */
    F32 timeFudge;		/*  time boost correction for time stamp */
    F32 shadow_h[4];		/*  Vertical offset of shadow from car. */
    F32 shadow_v[4];		/*  Vertical velocity shadow is moving for D.R. */
    S8 checkpoint;		/*  Current checkpoint */
    S8 lap;			/*  Current lap */
    S8 mpath_lap;		/*  Maxpath version of lap */
    BOOL fresh;			/*  If true, this data is fresh from network. */
}
EntityStateData;

typedef struct PDU
{
    netadd dst, src;		/*  Source and destination address */
    U8 junk[(NET_PKT_HEADER_SIZE - sizeof (netadd) * 2)];	/*  Used by driver only. */
    U8 data[NULL_DATA];		/*  Start of the real data. */
}
PDU;


typedef S16 (*PDUHandler) (PDU *);

typedef struct PDUHeader
{				/*  miniDIS */
    U8 version;
    U8 type;
    U16 length;
    U32 simID;			/*  Simulation ID or zero for any simulation. */
    U32 id;			/*  ID number for this message. */
    U32 timeStamp;
}
PDUHeader;

typedef struct EntityID
{				/*  miniDIS: Generic ID record, used for Entity ID's, etc. */
    U16 site;
    U16 entity;
}
EntityID;

typedef struct EntityType
{				/* miniDIS: Describes an entity. */
    U16 kind;
    U16 specific;
}
EntityType;

typedef struct EntityAppearancePDU
{				/*  miniDIS: Appearance */
    PDUHeader header;
    EntityID originID;
    U8 flag1;
    U8 flag2;
    U32 appearVis;
    U32 appearAudio;
}
EntityAppearancePDU;


typedef struct EntityStatePDU
{				/*  EntityState */
    PDUHeader header;
    EntityID entityID;
    EntityType entityType;
    F32x3 velocity;
    F32x3 location;
    F32 quat[4];
    S16 torque;
    U16 rpm;
    U32 appearance;
    U32 look;
    F32 steerangle, tireV[4];
    F32 suscomp[4];
    S32 timeStamp;
    F32 timeFudge;
    F32 shadow_h[4];		/*  Vertical offset of shadow from car. */
    F32 shadow_v[4];		/*  Vertical velocity shadow is moving for D.R. */
    F32x3 accel;
    F32x3 angularV;
    S8 checkpoint;		/*  Current checkpoint */
    S8 lap;			/*  Current lap */
    S8 mpath_lap;		/* Maxpath version of lap */
    S8 dummy;
}
EntityStatePDU;


/*------------------------------------------------------------------------ */

typedef struct AcknowledgePDU
{				/*  Acknowledge */
    PDUHeader header;
    EntityID receivingID;
    EntityID originID;
    AckFlag ackFlag;
    AckResponse responseFlag;
    U32 requestID;
}
AcknowledgePDU;

typedef struct PDUDataItem
{				/*  Used for fixed-length (32-bit) data items. */
    U32 datumID;		/*  See DatumID enum for valid values */
    U32 datum;
}
PDUDataItem;

typedef struct PDUVarDataItem
{				/*  Used in Data PDUs variable-length data items. */
    U32 datumID;		/*  See DatumID enum for valid values */
    U32 length;
    U32 data[NULL_DATA];
}
PDUVarDataItem;

typedef struct EventReportPDU
{				/*  Event Report  */
    PDUHeader header;
    EntityID receivingID;
    EntityID originID;
    U32 eventType;
    U32 numFixedRecs;
    U32 numVarRecs;
    U8 data[NULL_DATA];
}
EventReportPDU;

typedef struct MessagePDU
{				/*  Event Report  */
    PDUHeader header;
    EntityID receivingID;
    EntityID originID;
    U32 eventType;
    U32 numFixedRecs;
    U32 numVarRecs;
    U8 data[NULL_DATA];
}
MessagePDU;

/*==============================  PROTOTYPES  ===============================*/

#include "Pro/pdu.pro"
#include "Pro/dis.pro"

/*===============================  GLOBALS  =================================*/

EXTERN const U32 gDamageMask[5];	/*  Damage field of appearance flag masks. */
EXTERN const U8 gDamageShift[5];	/*  Damage field bit-shift amounts. */
EXTERN netadd gComAddress;	/*  Multicast address to talk/listen to. */
EXTERN netadd gBroadcast;	/*  Broadcast channel. */
EXTERN EntityID gThisEntity;	/*  Set to the entity ID of this machine. */
EXTERN S32 gCountDown;		/*  Amount of time until game starts. */

EXTERN EntityStateData carz[MAX_LINKS];		/*  Holds current pos */
EXTERN U32 gNextRequest;	/*  Keeps track of next request ID to use */
EXTERN U8 gSimManager;		/*  True if this node is the Simulation Manager */
EXTERN U32 gSimID;		/*  ID number of the current simulation. */

EXTERN LinkInfo gLink[MAX_LINKS];	/*  List of who's playing which track */
EXTERN BOOL gTrackLocked;	/*  True if a game has been locked down. */
EXTERN BOOL gAllTracksLocked;	/*  True all active players have locked tracks. */
EXTERN BOOL gUseCatchup;	/*  True if a game is using catchup. */
EXTERN BOOL gErrOverride;	/*  Used to override message supression. */
EXTERN BOOL gNetDelay;		/*  If set, delay Ack timeouts. */
EXTERN BOOL solo_flag;		/*  If set, only one human is playing. */
EXTERN BOOL gJoinInPossible;	/*  If set, a game is starting on the net. */
EXTERN BOOL gTourneyStart;	/*  True if external tourney start msg recvd. */
EXTERN BOOL gTourneyTrack;	/*  External track selection. */
EXTERN U8 gFeedbackLevel;	/*  Set to level of feedback */
EXTERN S32 gObjList[MAX_TRK_OBJS];	/*  ROM Object number of the various ROM objects. */

EXTERN BOOL gIgnoreScore;	/*  If true, don't enter score in HS table. */
EXTERN U32 gInThisGame;		/*  Mask with bits set for each node in this game. */
EXTERN U32 gLiveNodes;		/*  Mask with bits set for each node sending msgs. */

EXTERN U32 gPacketsSent;	/*  Stats for total packets sent so far. */
EXTERN U32 gPacketsReject;	/*  Stats for total packets rejected so far. */
EXTERN U32 gPacketsRcvd;	/*  Stats for total packets received so far. */
EXTERN network_descriptor gNetDesc; /* Network descriptor */

#undef EXTERN

#endif /* _DIS_H_ */
