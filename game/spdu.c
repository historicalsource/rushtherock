/*
 *  pdu.c   -- Functions for handling DIS Protocal packets.
 *
 *  Copyright 1996 Time Warner Interactive.
 *  Unauthorized reproduction, adaptation, distribution, performance or
 *  display of this computer program or the associated audiovisual work
 *  is strictly prohibited.
 *
 *  ======================================================================
 *  $Author: hightower $                       $Date: 1997/09/25 03:30:18 $
 *  $Revision: 3.42 $                       $Locker:  $
 *  ======================================================================
 *  Change Log:
 *
 *  $Log: pdu.c,v $
 * Revision 3.42  1997/09/25  03:30:18  hightower
 * Deleted unused stuff
 *
 * Revision 3.41  1997/09/18  06:16:13  hightower
 * Reformatted
 *
 * Revision 3.40  1997/09/13  01:48:41  grjost
 * Fixed for 12 cars.
 *
 * Revision 3.39  1997/06/28  01:42:52  gray
 * Extended to 4 tracks.
 *
 * Revision 3.38  1997/05/30  01:24:05  gray
 * Removed unused mainout reference.
 *
 * Revision 3.37  1997/03/31  03:10:37  gray
 * Nucleus gone - got rid of include and nucleus types.
 *
 * Revision 3.36  1997/03/20  13:46:55  geraci
 * new message type for use in linking
 *
 * Revision 3.35  1997/03/18  19:40:07  geraci
 * new broadcast version of countdown message
 *
 * Revision 3.34  1997/03/13  03:59:07  geraci
 * allowed messages to be broadcast on broadcast channel after multi-channel has been selected
 *
 * Revision 3.33  1997/01/30  20:31:24  geraci
 * expanded game difficulty and number of laps to include more entries
 *
 * Revision 3.32  1997/01/15  20:02:56  geraci
 * number of laps for track 2 and 3 can be changed by operator
 *
 * Revision 3.31  1996/12/13  13:25:43  geraci
 * common marker flags
 *
 * Revision 3.30  1996/12/13  12:05:45  geraci
 * more new lap stuff
 *
 * Revision 3.29  1996/12/12  10:39:48  geraci
 * expanded quaterion boundary
 *
 * Revision 3.28  1996/12/12  08:28:37  geraci
 * variable range checking supported
 *
 * Revision 3.27  1996/12/05  00:03:28  geraci
 * end of game score removed from entity state
 *
 * Revision 3.26  1996/12/04  01:57:28  geraci
 * checkpoints and end of game scores are handled by entity state updates
 *
 * Revision 3.25  1996/12/02  03:34:08  geraci
 * made score of 400 minutes valid
 *
 * Revision 3.24  1996/11/27  14:30:02  gray
 * Dead reckon shadow.
 *
 * Revision 3.23  1996/11/27  12:56:10  gray
 * Changed NetTime from F32 to 10microsec S32.
 *
 * Revision 3.22  1996/11/27  09:24:59  petrick
 * Changed types of error messages to production types.
 *
 * Revision 3.21  1996/11/27  00:52:50  petrick
 * Deactivated param error msg, since it's handled elsewhere.
 *
 * Revision 3.20  1996/11/22  04:59:49  geraci
 * quats passed over link instead of uvs
 *
 * Revision 3.19  1996/11/17  08:10:03  petrick
 * Tourney mode messages are always broadcast.
 *
 * Revision 3.18  1996/11/15  12:02:40  petrick
 * Made 'bad data' msg hideable.
 *
 * Revision 3.17  1996/11/13  07:28:49  petrick
 * Limits Entity state PDU rate at gave start to 8/sec per entity.
 *
 * Revision 3.16  1996/11/05  12:25:19  petrick
 * Error checking for Report State data.
 *
 * Revision 3.15  1996/11/02  09:57:24  petrick
 * Put Ack now acks directly to sending node instead of multicasting.
 *
 * Revision 3.14  1996/10/25  09:43:38  petrick
 * Cleanup.
 *
 * Revision 3.13  1996/10/25  06:47:57  petrick
 * Got rid of shared high score tables.
 *
 * Revision 3.12  1996/10/23  08:35:19  petrick
 * Added tourney reports to bounds checking.
 *
 * Revision 3.11  1996/10/19  02:15:34  petrick
 * Prioritized msgs.
 *
 * Revision 3.10  1996/10/17  08:26:41  petrick
 * Made acks more likely to go to the broadcast channel during demo games.
 *
 * Revision 3.9  1996/10/16  09:34:43  petrick
 * Changed bounds check msg to show originating node.
 *
 * Revision 3.8  1996/10/12  11:22:07  petrick
 * Added bounds checking to Event Report Data.
 *
 * Revision 3.7  1996/10/11  07:32:32  petrick
 * Got rid of some unused funcs.
 *
 * Revision 3.6  1996/10/10  05:22:05  petrick
 * Got rid of hack offline/online msgs.
 *
 * Revision 3.5  1996/10/02  21:18:18  petrick
 * Changed params in PutEventReport PDU call.
 *
 * Revision 3.4  1996/10/02  03:31:27  petrick
 * Changed name of a field to data.
 *
 * Revision 3.3  1996/09/19  15:06:44  petrick
 * Error checking incoming data, added new offline msg.
 *
 * Revision 3.2  1996/09/16  02:33:35  petrick
 * moved solo_flag check farther up the call chain.
 *
 * Revision 3.1  1996/09/09  02:24:52  gray
 * JAMMA version
 *
 * Revision 2.22  1996/09/05  01:45:35  gray
 * Passing airdist (for shadow) across link.
 *
 * Revision 2.21  1996/09/04  22:10:05  petrick
 * Minor fix.
 *
 * Revision 2.20  1996/09/04  21:02:49  petrick
 * Added shadow data for Alan.
 *
 * Revision 2.19  1996/09/04  20:29:53  gray
 * Send reckon data for Entity state.
 *
 * Revision 2.18  1996/08/30  20:31:10  petrick
 * Made Coin message always broadcast.
 *
 * Revision 2.17  1996/08/30  17:08:02  geraci
 * passing floating point version of position and velocity over net
 *
 * Revision 2.16  1996/08/22  11:24:25  petrick
 * More hiscore hi-jinx.  Improved Heartbeat acknowledgement during demos.
 *
 * Revision 2.15  1996/08/22  05:07:28  petrick
 * Acks get broadcast for demo game to keep nodes alive.
 *
 * Revision 2.14  1996/08/20  05:33:18  petrick
 * Cleanup.
 *
 * Revision 2.13  1996/08/19  07:48:37  petrick
 * Hi score across net, hi score synchronizing.
 *
 * Revision 2.12  1996/08/09  04:50:23  geraci
 * time base valid bit is passed on net in look long word
 *
 * Revision 2.11  1996/08/07  20:39:17  geraci
 * time fudge for dead reckoning. net passes a better version of position
 *
 * Revision 2.10  1996/07/30  09:16:53  petrick
 * Made PDU errors show up in fixed screen location.
 *
 * Revision 2.9  1996/07/29  04:24:10  petrick
 * Changed err msg
 *
 * Revision 2.8  1996/07/27  01:51:41  petrick
 * Put Send error on UR corner, fixed High score bug.
 *
 * Revision 2.7  1996/07/23  06:24:39  petrick
 * Network ignores own packets, doesn't send in solo mode.
 *
 * Revision 2.6  1996/07/01  05:39:41  petrick
 * Added updates for dynamic objects.
 *
 * Revision 2.5  1996/06/04  10:14:06  petrick
 * Got rid of AllocateMemory and FreeMemory funcs.
 *
 * Revision 2.4  1996/05/16  03:46:49  petrick
 * Changed error message function.
 *
 * Revision 2.3  1996/05/11  09:06:38  petrick
 * Better Ack handling, added Brodacast channel variable.
 *
 * Revision 2.2  1996/05/07  22:28:33  petrick
 * True multicast channels, 1st cut at high score synch.
 *
 * Revision 2.1  1996/05/04  02:56:18  petrick
 * Focus 2 release
 *
 * Revision 1.31  1996/04/30  03:38:18  petrick
 * Added network wheel spinning code.
 *
 * Revision 1.30  1996/04/12  00:23:42  geraci
 * use engtorque instead of load
 *
 * Revision 1.29  1996/04/10  00:13:18  petrick
 * Error messages override switch settings.
 *
 * Revision 1.28  1996/04/01  13:54:09  geraci
 * added collision bit to entity state
 *
 * Revision 1.27  1996/03/30  08:27:16  petrick
 * Added more Mem error checking.
 *
 * Revision 1.26  1996/03/26  04:06:08  petrick
 * New method of doing explosions.  Brake lights.
 *
 * Revision 1.25  1996/03/16  01:49:58  petrick
 * New timer routine.
 *
 * Revision 1.24  1996/03/14  01:10:03  petrick
 * Got rid of linking #def.
 *
 * Revision 1.23  1996/03/09  09:49:44  petrick
 * Added timestamp and appearance data to EntityStatePDU.
 *
 * Revision 1.22  1996/03/08  01:09:20  petrick
 * moved debug stuff to debug.c
 *
 * Revision 1.21  1996/03/07  23:51:45  geraci
 * moved rpm, load, appearance and engine_type to same place in model struct
 *
 * Revision 1.20  1996/03/07  10:43:04  geraci
 * drone jitter
 *
 * Revision 1.19  1996/02/24  00:12:26  petrick
 * Fixed Clearing text when switch 7 is turned off.
 *
 * Revision 1.18  1996/02/22  01:19:53  petrick
 * Added checkpoint and game over message handling.
 *
 * Revision 1.17  1996/02/14  21:37:51  petrick
 * Fixed PutEntityStatePDU.  It had extra params no longer needed.
 *
 * Revision 1.16  1996/02/14  19:22:39  petrick
 * Changed PutEntityStatePDU to use model data directly.
 *
 * Revision 1.15  1996/02/07  09:06:25  petrick
 * Cleanup.
 *
 * Revision 1.14  1996/02/02  01:43:01  petrick
 * Moved DIS debug msgs to switch 7.
 *
 * Revision 1.13  1996/01/11  22:42:53  petrick
 * Cleanup of type sizes.
 *
 * Revision 1.11  1996/01/06  02:09:21  petrick
 * Moved SMSG control to switch 6 since some stacks only have 8 switches.
 *
 * Revision 1.10  1996/01/04  19:49:40  gray
 * Change _PALB to _PAL for Z20.\
 *
 * Revision 1.9  1995/12/23  02:21:39  petrick
 * Put PDU Error Msgs under debug switch 13 control, cleanup.
 *
 * Revision 1.8  1995/12/14  02:35:47  petrick
 * Reactivated switch 6 messages.
 *
 * Revision 1.7  1995/12/12  01:48:24  petrick
 * Temporarily deactivated switch 6 from controlling debug switches.
 *
 *  Revision 1.6  1995/12/08  04:09:15  petrick
 *  Added RCS header to see what it does.
 *
 *  ======================================================================
 */

#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "os_proto.h"
#include "dis.h"
#include "debug.h"
#include "globals.h"
#include "net_link.h"
#include "net_smc.h"
#include "resurrect.h"
#include "targets.h"
#include "visuals.h"

#include "Pro/select.pro"

/*===============================  DEFINES  =================================*/

#define assert(x)       { if (!(x)) ReportPDUError( PDU_ASSERT_FAILURE ); }
#define offsetof(t, f)  ((size_t) &((t *) 0)->f)

#if PRODUCTION_RELEASE
#define LTEST(sss,vvv, lo, hi)                                         \
    if (!in(lo,(vvv),hi))                                              \
    {                                                                  \
        vvv     = .1;                                                  \
        return PDU_PARAM_ERROR;  /* Reject the packet. */              \
    }    
#else
#define LTEST(sss,vvv, lo, hi)                                         \
    if (!in(lo,(vvv),hi))                                              \
    {                                                                  \
        static S16   hold;                                             \
        EMSG( "Bad %s[%ld] caught (%8.3g)", sss, i, (float)(vvv));     \
        hold = 1;                                                      \
        while (hold);                                                  \
        vvv     = .1;                                                  \
        hold    = 1;                                                   \
        return PDU_PARAM_ERROR;  /* Reject the packet. */              \
    }    
#endif

#define RTEST(sss,vvv)  LTEST( sss, vvv, -1.0e6, 1.0e6 )


/*==============================  PROTOTYPES  ===============================*/

static void SetPDUEntityID (EntityID * entity, U16 id);
static BOOL CompareEntityID (EntityID * a, EntityID * b, BOOL useEntity);
static U32 GetRequestID (void);


/*===============================  GLOBALS  =================================*/

static U32 totalErrors;

/*===============================  EXTERNS  =================================*/


/* ----------------------------------------------------------------------------- */

/*//////////////////////////////////////////////////////////////////////////////
   // This call is used to acknowledge various other PDUs.
   //
   // entID    - The ID of the entity to receive acknowledgement.
   // reqID    - ID Request being acknowledged.
   // ack_flag - The type of message acknowledged. [enum]
   // response - Indication of success or failure, and the reason. [enum]
   ////////////////////////////////////////////////////////////////////////////// */
S16
PutAcknowledgePDU (netadd dest, U16 entID, U32 reqID, U32 type, AckFlag ack_flag,
    AckResponse response)
{
    U16 length = sizeof (AcknowledgePDU);
    AcknowledgePDU p;

    /*  Initialize the pdu data fields. */
    SetPDUEntityID (&p.receivingID, entID);
    SetPDUEntityID (&p.originID, gThisEntity.entity);
    p.ackFlag = ack_flag;
    p.responseFlag = response;
    p.requestID = reqID;

    return SendPDU ((PDU *) & p, length, kAcknowledgePDU, dest, true);

}                               /*  end PutAcknowledgePDU */


/*//////////////////////////////////////////////////////////////////////////////
   // This call is used to parse an Acknowledgement PDU.
   //
   // pdu      - This is the pdu to parse.
   // entID    - The ID of the entity to receive acknowledgement.
   // reqID    - ID Request being acknowledged.
   // ack_flag - The type of message acknowledged. [enum]
   // response - Indication of success or failure, and the reason. [enum]
   ////////////////////////////////////////////////////////////////////////////// */
S16
ParseAcknowledgePDU (PDU * pdu, U16 * entID, U32 * reqID, U16 * origin,
    AckFlag * ack_flag, AckResponse * response)
{
    AcknowledgePDU *p = (AcknowledgePDU *) & pdu->data;
#if 0
    /*  Parse packet into individual fields and check them if possible. */
    if (!CompareEntityID (&p->receivingID, &gThisEntity, false))
        return PDU_PARAM_ERROR;
#endif
    *entID = p->receivingID.entity;
    *origin = p->originID.entity;
    *ack_flag = p->ackFlag;
    *response = p->responseFlag;
    *reqID = p->requestID;

    return PDU_NO_ERROR;

}                               /*  end ParseAcknowledgePDU */


/* ----------------------------------------------------------------------------- */

/*//////////////////////////////////////////////////////////////////////////////
   // This PDU is sent whenever something in the entity’s state changes.  This usually
   // updates Dead Reckoning, but may also update the appearance of an entity.
   //
   // entID            - ID for the entity.  Assigned when a Create Entity PDU is sent/received.
   // velocity         - Linear velocity in X,Y,Z components [fps].
   // location         - Current position in World Coordinates [ft].
   // orientation      - Euler angle [radians].
   // appearance       - Bitfield describing current appearance of an entity.
   // acceleration     - Linear acceleration of the entity [fps2] used in Dead Reckoning.
   // angular_velocity - Rate of rotation of the object in [radians/sec].
   ////////////////////////////////////////////////////////////////////////////// */
S16
PutEntityStatePDU (U16 entID)
{
    U16 i, length = sizeof (EntityStatePDU);
    EntityStatePDU p;
    U32 look;

    /*  entID refers to a car model index. */
    if (!(entID & 0x8000))
    {
        MODELDAT *m = &model[entID];
        static lastEID[MAX_LINKS];

        if (gstate == COUNTDOWN && (IRQTIME - lastEID[entID] < ONE_SEC / 8))
            return PDU_NO_ERROR;
        lastEID[entID] = IRQTIME;

        /*  Initialize the pdu data fields. */
        SetPDUEntityID (&p.entityID, entID);

        for (i = 0; i < 3; i++)
        {
            p.velocity[i] = m->reckon.base_RWV[i];
            p.location[i] = m->reckon.base_RWR[i];
            p.accel[i] = m->reckon.base_RWA[i];
            p.angularV[i] = m->reckon.base_W[i];
        }

        make_quat_from_uvs (m->reckon.base_UV, p.quat);

        p.checkpoint = m->mcheckpoint;
        p.lap = m->mlap;
        p.mpath_lap = m->mmpath_lap;

        for (i = 0; i < 4; ++i)
        {
            p.shadow_h[i] = m->reckon.base_airdist[i];
            p.shadow_v[i] = m->reckon.base_airvel[i];
            p.tireV[i] = m->reckon.tireW[i];
            p.suscomp[i] = m->reckon.suscomp[i];
        }
        p.torque = m->reckon.torque;
        p.rpm = m->reckon.rpm;
        p.steerangle = m->reckon.steerangle;
        p.appearance = m->reckon.appearance;
        p.look = m->reckon.look;
        p.timeStamp = m->reckon.base_time;
        p.timeFudge = m->reckon.base_fudge;

        if(gMirrorMode) /* If we are mirrored, unmirror and send. */
            {
            p.velocity[0] = -p.velocity[0];
            p.location[0] = -p.location[0];
            /* unmirror other stuff... */
            }
    }
    /*  This is a target index (| 0x8000) */
    else
    {
        extern Target gTargetList[];
        Target *t;

        SetPDUEntityID (&p.entityID, entID);

        t = &gTargetList[entID & 0x7FFF];
        p.look = t->type;
        p.appearance = t->data;
        for (i = 0; i < 3; ++i)
        {
            p.location[i] = t->pos[i];
            p.velocity[i] = t->vel[i];
            p.angularV[i] = t->angV[i];
        }

        make_quat_from_uvs (t->uv, p.quat);
    }

    return SendPDU ((PDU *) & p, length, kEntityStatePDU, gComAddress, true);

}                               /*  end PutEntityStatePDU */

/*//////////////////////////////////////////////////////////////////////////////
   // When this PDU is received, something in the entity's state changed.  This usually
   // updates Dead Reckoning, but may also update the appearance of an entity.
   //
   // pdu              - This is the pdu to parse.
   // entID            - Returns the ID for the entity.
   // orientation      - Returns Euler angle [radians].
   // appearance       - Returns Bitfield describing current appearance of entity.
   // acceleration     - Returns Linear acceleration of the entity [fps2] used in Dead Reckoning.
   // angularVelocity  - Returns Rate of rotation of the object [radians/sec].
   // appearance       - Returns the apearances flags.
   ////////////////////////////////////////////////////////////////////////////// */
S16
ParseEntityStatePDU (PDU * pdu, U16 * entID, register EntityStateData * d)
{
    register EntityStatePDU *p = (EntityStatePDU *) & pdu->data;
    S32 i;

    /*  Parse packet into individual fields and check them if possible. */
    if (!CompareEntityID (&p->entityID, &gThisEntity, false))
        return PDU_PARAM_ERROR;

    *entID = p->entityID.entity;

    /*  Error check the data.  This test should work for both floats and ints.
     *  See the quaternion case for an example of use. It's OK to do the test after
     *  the copy, since the copy location is only a temporary holding place. */
    for (i = 0; i < 3; ++i)
    {
        d->vel[i] = p->velocity[i];
        d->pos[i] = p->location[i];
        d->acc[i] = p->accel[i];
        d->angv[i] = p->angularV[i];
#if 1
        RTEST ("vel", d->vel[i]);
        RTEST ("pos", d->pos[i]);
        RTEST ("acc", d->acc[i]);
        RTEST ("angv", d->angv[i]);
#endif
    }

    for (i = 0; i < 4; ++i)
    {
        d->suscomp[i] = p->suscomp[i];
        d->tireV[i] = p->tireV[i];
        d->shadow_h[i] = p->shadow_h[i];
        d->shadow_v[i] = p->shadow_v[i];
        d->quat[i] = p->quat[i];
#if 1
        LTEST ("quat", d->quat[i], -6.0, 6.0);
        RTEST ("suscomp", d->suscomp[i]);
        RTEST ("tireV", d->tireV[i]);
        RTEST ("shadow_h", d->shadow_h[i]);
        RTEST ("shadow_v", d->shadow_v[i]);
#endif
    }
    d->torque = p->torque;
    d->rpm = p->rpm;
    d->steerangle = p->steerangle;
    d->appearance = p->appearance;
    d->look = p->look;
    d->timeStamp = p->timeStamp;
    d->timeFudge = p->timeFudge;
    d->checkpoint = p->checkpoint;
    d->lap = p->lap;
    d->mpath_lap = p->mpath_lap;
#if 1
    RTEST ("torque", d->torque);
    RTEST ("rpm", d->rpm);
    RTEST ("steerangle", d->steerangle);
/*  RTEST( "timeStamp",     d->timeStamp    ); */
    LTEST ("timeFudge", d->timeFudge, .1, 2.0);

    /* Added range checking on lap and checkpoint... */
    LTEST ("checkpt", d->checkpoint, 0, 15);
    LTEST ("lap", d->lap, -40, 40);
    LTEST ("mpath_lap", d->mpath_lap, -40, 40);
#endif

    return PDU_NO_ERROR;

}                               /*  end ParseEntityStatePDU */

/* ----------------------------------------------------------------------------- */

/*//////////////////////////////////////////////////////////////////////////////
   //  This PDU is sent whenever an entity’s apperance changes. Since it only
   //  reports visiual or audio information, no acknowledgement is needed.
   //
   // entID    - ID for the entity being sent the data.
   // flag     - PDU_VISUAL_BIT set if visual included, PDU_AUDIO_BIT set if audio.
   // appearV  - Visual appearance bit fields. [enum]
   // appearA  - Audio appearance bit fields. [enum, TBD]
   ////////////////////////////////////////////////////////////////////////////// */
S16
PutEntityAppearancePDU (U16 entID, U8 flag, U32 appearV, U32 appearA)
{
    U16 length = sizeof (EntityAppearancePDU);
    EntityAppearancePDU p;

    /*  Initialize the pdu data fields. */
    SetPDUEntityID (&p.originID, entID);
    p.flag1 = 0x80 | (flag & PDU_VISUAL_BIT);
    p.flag2 = flag & PDU_AUDIO_BIT;
    p.appearVis = appearV;
    p.appearAudio = appearA;

    return SendPDU ((PDU *) & p, length, kEntityAppearancePDU, gComAddress, true);

}                               /*  end PutEntityAppearancePDU */


/*//////////////////////////////////////////////////////////////////////////////
   // pdu      - This is the pdu to parse.
   // entID    - Returns the ID for the entity sending the data.
   // flag     - PDU_VISUAL_BIT set if visual included, PDU_AUDIO_BIT set if audio.
   // appearV  - Visual appearance bit fields. [enum]
   // appearA  - Audio appearance bit fields. [enum, TBD]
   ////////////////////////////////////////////////////////////////////////////// */
S16
ParseEntityAppearancePDU (PDU * pdu, U16 * entID, U8 * flag, U32 * appearV,
    U32 * appearA)
{
    EntityAppearancePDU *p = (EntityAppearancePDU *) & pdu->data;

    /*  Parse packet into individual fields and check them if possible. */
    *entID = p->originID.entity;
    *flag = (p->flag1 & PDU_VISUAL_BIT) | (p->flag2 & PDU_AUDIO_BIT);
    *appearV = p->appearVis;
    *appearA = p->appearAudio;

    return PDU_NO_ERROR;

}                               /*  end ParseEntityAppearancePDU */


/* ----------------------------------------------------------------------------- */

/*//////////////////////////////////////////////////////////////////////////////
   //  This PDU is sent to report significant events to other nodes.
   //
   // entID        - ID for the entity this data refers to.
   // event_type   - Identifies type of event that occurred. [enum]
   // num_items    - Number of entries in item_list.
   // item_list    - List of data ID items being reported.
   // ...          - List of data items being reported.  These are sent in pairs
   //                <EventReportItem, value>
   ////////////////////////////////////////////////////////////////////////////// */
S16
PutEventReportPDU (EventTypeID event_type, U32 num_items,...)
{
    va_list ap;
    U16 entID = PDU_ALL_ENTITIES;
    U16 length = sizeof (EventReportPDU) +
    (num_items * sizeof (PDUDataItem));
    EventReportPDU *p;
    Net_Buffer *nb, *pdu;
    S16 i, k, len, bytes, chunk, ret;
    PDUDataItem *iPtr;
    U32 id, result, oitems, nameLen = 0;
    char *str = 0, lStr[MAX_NAME_LEN + 5];

    /*  Prescan the argument list for non-U32 sized arguments */
    va_start (ap, num_items);
    oitems = num_items;
    for (i = 0; i < num_items; ++i)
    {
        if ((id = va_arg (ap, EventReportItem)) >= HIGHEST_REPORT_NUM)
            EMSG ("Sending foul packet");
        else
            id = va_arg (ap, U32);
    }
    va_end (ap);

    /*  Create a buffer for the PDU.  */
    if ((result = Net_Get_Buffer (&nb, sizeof (PDU) + length, NETOPT_NONBLOCK | NETOPT_PRIORITY)) != NETOK)
    {
        ReportPDUError (PDU_MEM_ERROR, result, (S32) 308);
        return PDU_MEM_ERROR;
    }

    /*  Initialize the pdu data fields. */
    pdu = nb;
    nb->len = sizeof (EventReportPDU);
    p = (EventReportPDU *) ((PDU *) & nb->data)->data;
    p->receivingID.site = PDU_ALL_ENTITIES;
    p->receivingID.entity = entID;
    SetPDUEntityID (&p->originID, gThisEntity.entity);
    p->eventType = event_type;
    p->numFixedRecs = oitems;
    p->numVarRecs = 0;

    /*  Copy the list of data items to the PDU record. */
    va_start (ap, num_items);
    nb->offset = NET_PKT_HEADER_SIZE;
    bytes = nb->size - nb->len - nb->offset;
    iPtr = (PDUDataItem *) & p->data;
    for (i = 0; i < num_items; ++i, ++iPtr)
    {
        /*  Determine size of the item */
        id = va_arg (ap, EventReportItem);
        chunk = sizeof (PDUDataItem);

        /*  Make sure it fits. */
        if (chunk > bytes)
        {
            if (!nb->next)
                if ((result = Net_Get_Buffer (&nb->next, chunk, NETOPT_NONBLOCK | NETOPT_PRIORITY)) != NETOK)
                {
                    ReportPDUError (PDU_MEM_ERROR, result, (S32) 309);
                    return PDU_MEM_ERROR;
                }
            nb = nb->next;
            iPtr = (PDUDataItem *) nb->data;
            nb->offset = 0;
            nb->len = 0;
            bytes = nb->size - nb->len - nb->offset;
        }

        /*  Copy the arguments */
        bytes -= chunk;
        nb->len += chunk;
        iPtr->datumID = id;
        iPtr->datum = va_arg (ap, U32);
    }
    va_end (ap);

    if (event_type == Event_Coin ||
        event_type == Event_Tourney || event_type == Event_TourneyStart ||
        (event_type == Event_Heartbeat && demo_game))
        return SendPDU ((PDU *) pdu, length, kEventReportPDU, gBroadcast, false);

    /* if multicast sync message, always use broadcast channel */
    if (event_type == Message_MCast_Channel || event_type == Message_BCast_Countdown)
        return SendPDU ((PDU *) pdu, length, kMessagePDU, gBroadcast, false);
    else
        return SendPDU ((PDU *) pdu, length, kEventReportPDU, gComAddress, false);

}                               /*  end PutEventReportPDU */


/*//////////////////////////////////////////////////////////////////////////////
   //  This PDU reports significant events to this node.
   //
   // pdu          - This is the pdu to parse.
   // entID        - Returns the ID for the entity this data refers to.
   // event_type   - Identifies type of event that occurred. [EventTypeID]
   // num_items    - On entry, maximum number of items that item_list can hold.
   //                On return, holds number of entries actually in item_list.
   // item_list    - List of data ID items being reported.
   ////////////////////////////////////////////////////////////////////////////// */
S16
ParseEventReportPDU (PDU * pdu, U16 * entID, EventTypeID * event_type,
    U32 * num_items, PDUDataItem * item_list)
{
    EventReportPDU *p = (EventReportPDU *) & pdu->data;
    S16 i, k, j, len, id, ok, node;
    PDUDataItem *iPtr = (PDUDataItem *) & p->data;

    /*  Parse packet into individual fields and check them if possible. */
    if (!CompareEntityID (&p->receivingID, &gThisEntity, false))
        return PDU_PARAM_ERROR;

    *entID = p->receivingID.entity;
    *event_type = (EventTypeID) p->eventType;
    *num_items = p->numFixedRecs;
    node = p->originID.entity;

    /*  Error check the data. */
    for (len = i = 0; i < *num_items; ++i, ++iPtr)
    {
        ok = 0;
        switch (id = iPtr->datumID)
        {
            case Report_Node:
                ok = iPtr->datum <= 7;
                break;          /*  1.Node number (0-7) */
            case Report_Track:
                ok = iPtr->datum < NTRACKS;
                break;          /*  2.Track number (0-2) */
            case Report_Channel:
                ok = iPtr->datum <= 63;
                break;          /*  3.Mcast address (0-63) */
            case Report_CarType:
                ok = iPtr->datum < NCARS;
                break;          /*  4.Car Type (0-7) */
            case Report_RaceFlags:
                ok = true;
                break;          /*  5.Race flag settings */
            case Report_Count:
                ok = iPtr->datum <= 20;
                break;          /*  6.Countdown Secs (0-10?) */
            case Report_Highlight:
                ok = iPtr->datum <= 1;
                break;          /*  7.Highlight (Boolean)  */
            case Report_Score:
                ok = iPtr->datum <= 400L * 60 * ONE_SEC;
                break;          /* 8.Score value (0-?) */
            case Report_ScoreFlags:
                ok = iPtr->datum <= 2;
                break;          /*  9.Score flags (0=new score, 1=synch) */
            case Report_CheckPoint:
                ok = true;
                break;          /* 10.Checkpoint number (0-?) */
            case Report_Flags:
                ok = true;
                break;          /* 11.Checkpoint flags (various) */
            case Report_SimID:
                ok = iPtr->datum <= 0x208;
                break;          /* 12.Simulation ID (U32) */
            case Report_Drones:
                ok = true;
                break;          /* 13.Drone responsibilites */
            case Report_PolePos:
                ok = true;
                break;          /* 14.Pole position (0-7) */
            case Report_Time:
                ok = in (-2000, (S32) iPtr->datum, 20000);
                break;          /* 15.time value. (0-?) */
            case Report_Slot:
                ok = iPtr->datum <= 7;
                break;          /* 16.slot number. (0-8) */
            case Report_Difficulty:
                ok = iPtr->datum <= 7;
                break;          /* 17.Difficulty factor (0-3) */
            case Report_TourneyFlags:
                ok = true;
                break;          /* 18.tourney flags. (various) */
            case Report_TourneyLaps:
                ok = iPtr->datum <= 99;
                break;          /* 19.tourney laps. (0-99). */
            case Report_State:
                ok = iPtr->datum < NUM_NODESTATES;
                break;          /* 20. Node states. (0-4). */
            case Report_MarkerFlag:
                ok = true;
                break;          /* 21.Marker flag (0-1) */
            case Report_NumberLaps:
                ok = (iPtr->datum >= 1 && iPtr->datum <= 33) || iPtr->datum == 0;
                break;          /* 22.Number of laps (2-3 or 0) */
            default:
                ok = false;
                break;
        }
        if (!ok)
        {
            MSG ("Received bad data[%ld] from:%ld-%s type:%ld data:%ld",
                (long) i, (long) node, NodeName (node), (long) id, (U32) iPtr->datum);
            return PDU_PARAM_ERROR;
        }
    }

    /*  Copy the list of data items from the PDU record. */
    iPtr = (PDUDataItem *) & p->data;
    for (len = i = 0; i < *num_items; ++i, ++iPtr, ++item_list)
    {
        item_list->datumID = iPtr->datumID;
        item_list->datum = iPtr->datum;
    }

    return PDU_NO_ERROR;

}                               /*  end ParseEventReportPDU */


/* ----------------------------------------------------------------------------- */

/*//////////////////////////////////////////////////////////////////////////////
   //  Sets the values of an EntityID record.
   ////////////////////////////////////////////////////////////////////////////// */
static void
SetPDUEntityID (EntityID * entity, U16 id)
{
    entity->site = gThisEntity.site;
    entity->entity = id;

}                               /*  end SetPDUEntityID */


/*//////////////////////////////////////////////////////////////////////////////
   //  Compares two EntityID records and returns true if they are identical.
   ////////////////////////////////////////////////////////////////////////////// */
static BOOL
CompareEntityID (EntityID * a, EntityID * b, BOOL useEntity)
{
#if 1
    return true;
#else
    return (a->site == b->site && (!useEntity || a->entity == b->entity));
#endif
}                               /*  end CompareEntityID */


/*//////////////////////////////////////////////////////////////////////////////
   //  Reports PDU Errors.
   ////////////////////////////////////////////////////////////////////////////// */
void
ReportPDUError (S16 PDUerr,...)
{
    va_list nextArg;
    S32 NetErr, i, arg;
    S32 nucErr;

    switch (PDUerr)
    {
        case PDU_TIMEOUT:
            EMSG ("PDU TIMEOUT ERROR");
            break;
        case PDU_PARAM_ERROR:   /*EMSG("PDU PARAM ERROR"); */
            break;
        case PDU_TRUNC_ERROR:
            EMSG ("PDU TRUNC ERROR");
            break;
        case PDU_NO_PDU:
            EMSG ("PDU NO PDU ERROR");
            break;
        case PDU_CANT_INIT:
            EMSG ("PDU CANT INIT ERROR");
            break;
        case PDU_ASSERT_FAILURE:
            EMSG ("PDU ASSERT FAILURE");
            break;

        case PDU_NUCLEUS_ERROR:
            va_start (nextArg, PDUerr);
            nucErr = va_arg (nextArg, S32);
            i = va_arg (nextArg, S16);
            va_end (nextArg);
            EMSG ("PDU NUCLEUS ERROR %ld, location %ld", (S32) nucErr, i);
            break;

        case PDU_SEND_ERROR:
            va_start (nextArg, PDUerr);
            NetErr = va_arg (nextArg, S32);
            i = (S32) va_arg (nextArg, S16);
            va_end (nextArg);
            SMSG (1, 41, "SEND ERR%5ld: %lx-%ld", ++totalErrors, NetErr, i);
            break;

        case PDU_MEM_ERROR:
            va_start (nextArg, PDUerr);
            NetErr = va_arg (nextArg, S32);
            i = va_arg (nextArg, S32);
            va_end (nextArg);
            SMSG (2, 41, "MEM  ERR%5ld: %lx-%ld", ++totalErrors, NetErr, i);
            break;
    }

    return;

}                               /*  end ReportPDUError */



/*//////////////////////////////////////////////////////////////////////////////
   //  Returns the current simulation time for use in a timestamp.
   ///////////////////////////////////////////////////////////////////////////// */
U32
GetTimeStamp (void)
{
    return GetElapsedTime ();

}                               /*  end GetTimeStamp */
