/*
 *  sounds.c -- Audio Motion Object handlers:
 *
 *  Copyright 1996 Time Warner Interactive
 *  Unauthorized reproduction, adaptation, distribution, performance or
 *  display of this computer program or the associated audiovisual work
 *  is strictly prohibited.
 *
 *  ======================================================================
 *  $Author: hightower $                     $Date: 1997/10/07 20:01:50 $
 *  $Revision: 3.17 $                       $Locker:  $
 *  ======================================================================
 *  Change Log:
 *
 *  $Log: sounds.c,v $
 * Revision 3.17  1997/10/07  20:01:50  hightower
 * Now clearing IRQs and bypassing local queue when HW is ready
 *
 * Revision 3.16  1997/10/07  19:08:09  hightower
 * Fixed problem with missing sounds
 *
 * Revision 3.15  1997/10/07  08:51:01  hightower
 * rushified and removed audio hang bug
 *
 * Revision 3.14  1997/08/22  00:53:00  gray
 * Four engine types now.
 *
 * Revision 3.13  1997/08/13  21:07:34  gray
 * Added 7th radio station.
 *
 * Revision 3.12  1997/06/13  18:44:10  gray
 * Audio mods to be compatible with old and new sounds.
 *
 * Revision 3.11  1996/12/11  10:59:09  geraci
 * doesn't keep trying if sounds can't get through
 *
 * Revision 3.10  1996/12/04  01:59:16  geraci
 * made sound calls more robust
 *
 * Revision 3.9  1996/11/25  16:01:50  geraci
 * stop all sounds is even more robust than before
 *
 * Revision 3.8  1996/11/20  21:37:20  geraci
 * doesn't hang on waiting for audqlen to clear
 *
 * Revision 3.7  1996/11/15  08:05:31  geraci
 * tests for existence of sound
 *
 * Revision 3.6  1996/11/14  12:35:14  geraci
 * handles stop all sounds much better (it works finally)
 *
 * Revision 3.5  1996/10/09  02:22:00  geraci
 * new driving through water sound
 *
 * Revision 3.4  1996/10/04  07:36:03  geraci
 * single parameter sound call returns return code
 *
 * Revision 3.3  1996/10/02  02:57:16  geraci
 * removed old music references
 *
 * Revision 3.2  1996/09/18  10:14:03  geraci
 * using attract music during regular game
 *
 * Revision 3.1  1996/09/09  02:24:52  gray
 * JAMMA version
 *
 * Revision 2.9  1996/08/21  07:07:17  geraci
 * new attract music
 *
 * Revision 2.8  1996/08/18  19:36:33  gray
 * Make PHOENIX_PORT only, removed flag.
 *
 * Revision 2.7  1996/08/17  07:09:16  geraci
 * new music score
 *
 * Revision 2.6  1996/08/02  19:00:11  geraci
 * music light goes off when changing volume
 *
 * Revision 2.5  1996/07/27  08:18:06  geraci
 * more precision send for listener and drone positions
 *
 * Revision 2.4  1996/07/25  09:18:43  geraci
 * shows music light when music is playing
 *
 * Revision 2.3  1996/05/24  22:16:29  geraci
 * assigned TPM engine to NSX
 *
 * Revision 2.2  1996/05/11  01:16:16  geraci
 * fixed sound handle bug and fixed some debug display stuff
 *
 * Revision 2.1  1996/05/04  02:56:18  petrick
 * Focus 2 release
 *
 * Revision 1.22  1996/05/04  02:22:26  geraci
 * deleted old squeal code
 *
 * Revision 1.21  1996/05/03  22:28:35  geraci
 * better tire squeals
 *
 * Revision 1.20  1996/05/02  01:07:03  geraci
 * eliminated two other music selections
 *
 * Revision 1.19  1996/04/30  01:27:33  geraci
 * added command to send multiple parameter commands using audmput
 *
 * Revision 1.18  1996/04/29  22:12:02  geraci
 * new music selection
 *
 * Revision 1.17  1996/04/26  01:20:26  geraci
 * new music selections
 *
 * Revision 1.16  1996/04/20  07:14:55  geraci
 * deleted unused engine models
 *
 * Revision 1.15  1996/04/12  00:23:10  geraci
 * another new engine
 *
 * Revision 1.14  1996/04/11  01:51:43  geraci
 * new engine
 *
 * Revision 1.13  1996/04/03  04:16:48  geraci
 * fixed angle conversion in positional sound call
 *
 * Revision 1.12  1996/02/06  03:38:46  geraci
 * put num engines in carsnd.h not sound.h
 *
 * Revision 1.11  1996/02/02  05:30:30  geraci
 * temp fix for locked up tire squeals
 *
 * Revision 1.10  1996/01/24  22:44:20  geraci
 * move music call to here
 *
 * Revision 1.9  1996/01/19  03:04:08  geraci
 * added SOUND routine to handle all other miscellaneous sounds
 *
 * Revision 1.8  1996/01/11  23:44:40  geraci
 * replaced ints with S32s...
 *
 * Revision 1.7  1996/01/11  20:58:09  geraci
 * changed handle passing to aud_mput rountine for sound effects
 * changed own engine call so that passed parameters come from table
 *
 *
 *    Rev 0.33:  CJP    6/26/94:  shift handle in kill_moob
 *    Rev 0.32:  CJP    6/15/94
 *    Rev 0.40:  CJP    2/10/95:  added manual engine
 *
 *  ======================================================================
 */

#include <config.h>
#include "carsnd.h"
#include "dis.h"
#include "game.h"
#include "globals.h"
#include "os_proto.h"
#include "modeldat.h"
#include "sounds.h"

#include "Pro/sounds.pro"
#include "Pro/moobs.pro"

/* These coodinate transforms are provided as macros so that each game can
 * easily convert their own native coordinates to CAGE format coordinates.
 * x and y coords must end up as 15 bit signed data.
 * Angles must end up as 9 bit data with zero facing into the screen.
 * Moob velocity is measured in mm/60hz frame.
 * Engine velocity is measured in 1/8 mm per 60 hz frame.
 */
/* these macros convert TANK coordinates to something that will
   make CAGE happy */

#define X_COORD_TRANS(x)          ((S16)((x*3)/10))     /* Convert from feet to meters */
#define Y_COORD_TRANS(y)          ((S16)((y*3)/10))     /* Convert from feet to meters */
#define LX_COORD_TRANS(x)         ((S32)((x*3)/10))     /* Convert from feet to meters */
#define LY_COORD_TRANS(y)         ((S32)((y*3)/10))     /* Convert from feet to meters */
#define ANGLE_COORD_TRANS(t)      (128 - (0x1ff & (t >> 7)))    /* NOT currently used */
#define MOOB_VELOCITY_XFORM(v)    ((S16)((v*447)/120))  /* Convert from mph to (mm/16.67 ms) */
#define ENGINE_VELOCITY_XFORM(v)  (v<<1)

#define show_parms   0

#define MAX_RADIO    7
const S16 max_radio = MAX_RADIO;

const S16 demo_radio_off[MAX_RADIO] =
{S_KPULPATT, S_KAUG8ATT, S_KZETHNOATT, S_KTHEROCK,
 S_KSTLATT, S_KRAVERUSHATT, S_KBLUESATT};

const S16 demo_radio_on[MAX_RADIO] =
{S_PULPATT, S_AUG8ATT, S_ZETHNOATT, S_THEROCK,
 S_STLATT, S_RAVERUSHATT, S_BLUESATT};

#define NUM_ENGINES  4
const S16 num_engines = NUM_ENGINES;

const U16 own_engine[NUM_ENGINES][2] =
{
    {S_AUDREY, 0},              /* v1.06 vw */
    {S_GHR, 0},                 /* v1.06 std */
    {S_NSX, 0},
    {S_TPM, 0},
};

const U16 other_engine[NUM_ENGINES][2] =
{
    {S_M_MULTIIDLE, 0},
    {S_M_MULTIIDLE, 0},
    {S_M_MULTIIDLE, 0},
    {S_M_MULTIIDLE, 0},
};

const U16 tire_squeal[4] =
{S_SQUEAL_LEFT,
 S_SQUEAL_RIGHT,
 S_SKID_CUSTOM1,
 S_PEELOUT};

U8 car_sound_handle[9];
S16 sound_index;
char snd_buf[10];
S32 sounds_are_present;

extern S16 which_engine;

extern U32 ctl_mod_latch (int x);

#define Q_SIZE 4096
#define Q_MASK (Q_SIZE-1)
#if (((Q_SIZE)&(Q_MASK))!=0)
Compile ERROR:line __LINE__ in file __FILE__-- q size must be a power of 2 !
#endif

typedef struct sndEl
{
   S16 d[16];
   U8  cnt;
}
sndEl;

typedef struct sndQueue
{
    U32 head;
    U32 tail;
   sndEl  q[Q_SIZE];
}
sndQueue;

static sndQueue q;

static int
sndQEmpty (void)
{
    return q.head == q.tail;
}

#define AUDIO_HW_READY ( (aud_qlen()) < (0xd000) )
#define AUDIO_HW_SEND(ptr,cnt) \
{ if(cnt == 1) aud_put(*(ptr)); else aud_mput(ptr,cnt); }

static void
sndQXfer (void)
{
    while (q.tail != q.head && AUDIO_HW_READY )
    {
        sndEl *el = &q.q[q.tail];

        AUDIO_HW_SEND( el->d, el->cnt );

#if 0
        {
            static S32 row;

            DBG_printf( 1, row++, GRN_PAL,
              "sndQXfer: %d words (%d)", el->cnt, (q.tail-q.head)&Q_MASK );

            if( row > 40 ) row = 0;
                txt_clr_wid (1, row, 80);
        }
#endif
        q.tail++;
        if (q.tail >= Q_SIZE)
            q.tail = 0;
    }
}

static void
sndQStore (S16 * parms, S32 cnt)
{
    int i;
    sndEl *el;
    S32 state;

    if (((q.head + 1) & Q_MASK) == q.tail)
        return; /* full */

    if( cnt > sizeof( q.q[0].d )/sizeof( q.q[0].d[0] ) )
    {
        char b[80];
        sprintf( b, "sndQStore: Look at the size of that mama: %-5d",
          (int)cnt );
        txt_str( 5, 20, b, GRN_PAL );
        return;
    }

    state = prc_set_ipl( INTS_OFF );

    el = &q.q[q.head];

    el->cnt = cnt;

    for (i = 0; i < cnt; i++)
    {
        el->d[i] = parms[i];
    }

    q.head++;
    if (q.head >= Q_SIZE)
        q.head = 0;

    prc_set_ipl( state );
}

static void
sndCmds( S16 *parms, S32 cnt )
{
    S32 state;

    state = prc_set_ipl( INTS_OFF );

    if( q.head == q.tail && AUDIO_HW_READY )
    {
        AUDIO_HW_SEND( parms, cnt );
    }
    else
    {
        sndQStore(parms,cnt);
    }

    prc_set_ipl( state );
}


/* This should be called once per frame from the model task -- don't call
 * from the game task since the model task should be the only task modifying
 * the q.tail variable
 */
void
sndUpdate (void)
{
    sndQXfer ();
}

/*************************************************************************************
 *         sndListenerUpdate()
 *
 */

S32
sndListenerUpdate (S32 x, S32 y, U16 velocity, U16 vel_angle, U16 facing_angle)
{
    U16 cmdBuf[7];
    S32 x_pos, y_pos;

    x_pos = LX_COORD_TRANS (x);
    y_pos = LY_COORD_TRANS (y);
    cmdBuf[0] = S_MOOB_LISTENER;
    cmdBuf[1] = x_pos >> 7;
    cmdBuf[2] = y_pos >> 7;
    cmdBuf[3] = ((x_pos << 8) | (y_pos & 0x7f)) & 0x7f7f;
    cmdBuf[4] = MOOB_VELOCITY_XFORM (velocity);
    cmdBuf[5] = vel_angle;
    cmdBuf[6] = facing_angle;

#if show_parms
    {
        S16 i;

        if (++sound_index > 999)
            sound_index = 0;
        sprintf (snd_buf, "%3dlu", sound_index);
        txt_str (0, 14, snd_buf, WHT_PALB);

        for (i = 0; i < sizeof (cmdBuf) / sizeof (U16); i++)
            txt_hexnum (7 + i * 5, 14, cmdBuf[i], 4, 2, RED_PAL);
    }
#endif

    sndCmds (cmdBuf, 7);

    return 0;
}

/*************************************************************************************
 *
 *
 */

S32
sndStartStaticUnpitched (U16 objID, U8 handle, U8 priority, S16 x, S16 y)
{
    U16 cmdBuf[4];

    handle &= 0xFF;
    priority &= 0x7F;
    cmdBuf[0] = objID;
    cmdBuf[1] = (handle << 7) + priority;
    cmdBuf[2] = X_COORD_TRANS (x);
    cmdBuf[3] = Y_COORD_TRANS (y);

#if show_parms
    {
        S16 i;

        if (++sound_index > 999)
            sound_index = 0;
        sprintf (snd_buf, "%3dssu", sound_index);
        txt_str (0, 15, snd_buf, WHT_PALB);

        for (i = 0; i < sizeof (cmdBuf) / sizeof (U16); i++)
            txt_hexnum (7 + i * 5, 15, cmdBuf[i], 4, 2, RED_PAL);
    }
#endif

    sndCmds (cmdBuf, 4);

    return 0;
}

/*************************************************************************************
 *
 *
 */

S32
sndStartStaticPitched (U16 objID, U8 handle, U8 priority,
                       S32 x, S32 y, U16 pitch, U8 filter, U8 Q)
{
    U16 cmdBuf[6];

    handle &= 0xFF;
    priority &= 0x7F;
    cmdBuf[0] = objID;
    cmdBuf[1] = (handle << 7) + priority;
    cmdBuf[2] = X_COORD_TRANS (x);
    cmdBuf[3] = Y_COORD_TRANS (y);
    cmdBuf[4] = pitch;
    cmdBuf[5] = (Q << 8) | (filter & 0xFF);     /*Q in hi byte, filter frequency in low */

#if show_parms
    {
        S16 i;

        if (++sound_index > 999)
            sound_index = 0;
        sprintf (snd_buf, "%3dssp", sound_index);
        txt_str (0, 16, snd_buf, WHT_PALB);

        for (i = 0; i < sizeof (cmdBuf) / sizeof (U16); i++)
            txt_hexnum (7 + i * 5, 16, cmdBuf[i], 4, 2, RED_PAL);
    }
#endif

    sndCmds (cmdBuf, 6);

    return 0;
}

/*******************************************************************************
 *
 *
 */

S32
sndStartDopplerUnpitched (U16 objID, U8 handle, U8 priority,
                          S32 x, S32 y, U16 velocity, U16 vel_angle)
{
    U16 cmdBuf[6];

    handle &= 0xFF;
    priority &= 0x7F;
    cmdBuf[0] = objID;
    cmdBuf[1] = (handle << 7) + priority;
    cmdBuf[2] = X_COORD_TRANS (x);
    cmdBuf[3] = Y_COORD_TRANS (y);
    cmdBuf[4] = MOOB_VELOCITY_XFORM (velocity);
    cmdBuf[5] = vel_angle;

#if show_parms
    {
        S16 i;

        if (++sound_index > 999)
            sound_index = 0;
        sprintf (snd_buf, "%3dsdu", sound_index);
        txt_str (0, 17, snd_buf, WHT_PALB);

        for (i = 0; i < sizeof (cmdBuf) / sizeof (U16); i++)
            txt_hexnum (7 + i * 5, 17, cmdBuf[i], 4, 2, RED_PAL);
    }
#endif

    sndCmds (cmdBuf, 6);

    return 0;
}

/*************************************************************************************
 *
 *
 */

S32
sndStartDopplerPitched (U16 objID, U8 handle, U8 priority,
                        S32 x, S32 y,
                        U16 velocity, U16 vel_angle,
                        U16 pitch, U8 filter, U8 Q)
{
    U16 cmdBuf[8];

    handle &= 0xFF;
    priority &= 0x7F;
    cmdBuf[0] = objID;
    cmdBuf[1] = (handle << 7) + priority;
    cmdBuf[2] = X_COORD_TRANS (x);
    cmdBuf[3] = Y_COORD_TRANS (y);
    cmdBuf[4] = MOOB_VELOCITY_XFORM (velocity);
    cmdBuf[5] = ANGLE_COORD_TRANS (vel_angle);
    cmdBuf[6] = pitch;

    /*Q in hi byte, filter frequency in low */
    cmdBuf[7] = (Q << 8) | (filter & 0xFF);

#if show_parms
    {
        S16 i;

        if (++sound_index > 999)
            sound_index = 0;
        sprintf (snd_buf, "%3dsdp", sound_index);
        txt_str (0, 18, snd_buf, WHT_PALB);

        for (i = 0; i < sizeof (cmdBuf) / sizeof (U16); i++)
            txt_hexnum (7 + i * 5, 18, cmdBuf[i], 4, 2, RED_PAL);
    }
#endif

    sndCmds (cmdBuf, 8);

    return 0;
}


/*************************************************************************************
 *
 *
 */

S32
sndPositionSound (U16 objId, U16 angle, U16 volume)
{
    U16 cmdBuf[3];

    cmdBuf[0] = objId;
    cmdBuf[1] = volume;
    cmdBuf[2] = angle;
#if 0
    cmdBuf[2] = ANGLE_COORD_TRANS (angle);
#endif

#if show_parms
    {
        S16 i;

        if (++sound_index > 999)
            sound_index = 0;
        sprintf (snd_buf, "%3dps", sound_index);
        txt_str (0, 19, snd_buf, WHT_PALB);

        for (i = 0; i < sizeof (cmdBuf) / sizeof (U16); i++)
            txt_hexnum (7 + i * 5, 19, cmdBuf[i], 4, 2, RED_PAL);
    }
#endif

    sndCmds (cmdBuf, 3);

    return 0;
}


/*************************************************************************************
 *
 *
 */

S32
sndChangePosition (U8 handle, S16 x, S16 y)
{
    U16 cmdBuf[4];

#if 0
    handle &= 0xFF;
    cmdBuf[0] = S_MOOB_CHANGE_POSITION;
    cmdBuf[1] = handle << 7;
    cmdBuf[2] = x;
    cmdBuf[3] = y;

    sndCmds (cmdBuf, 4);

    return 0;
#endif
}

/*************************************************************************************
 *
 *
 */

S32
sndChangeVelocity (U8 handle, U16 velocity, U16 vel_angle)
{
    U16 cmdBuf[4];

#if 0
    handle &= 0xFF;
    cmdBuf[0] = S_MOOB_CHANGE_VELOCITY;
    cmdBuf[1] = handle << 7;
    cmdBuf[2] = velocity;
    cmdBuf[3] = vel_angle;

    sndCmds (cmdBuf, 4);

    return 0;
#endif
}

/*************************************************************************************
 *
 *
 */

S32
sndChangePitch (U8 handle, U16 pitch)
{
    U16 cmdBuf[3];

#if 0
    handle &= 0xFF;
    cmdBuf[0] = S_MOOB_CHANGE_PITCH;
    cmdBuf[1] = handle << 7;
    cmdBuf[2] = pitch;

    sndCmds (cmdBuf, 3);
    return 0;
#endif
}

/*************************************************************************************
 *
 *
 */

S32
sndChangeFilter (U8 handle, U8 filter, U8 Q)
{
    U16 cmdBuf[3];

#if 0
    handle &= 0xFF;
    cmdBuf[0] = S_MOOB_CHANGE_FILTER;
    cmdBuf[1] = handle << 7;
    cmdBuf[2] = (Q << 8) | (filter & 0xFF);     /*Q in hi byte, filter frequency in low */

    sndCmds (cmdBuf, 3);

    return 0;
#endif
}


/*************************************************************************************
 *
 *
 */

S32
sndStartEngine (U16 engineID, U16 pitch, U8 filter_frequency, U8 filter_Q)
{
    U16 cmdBuf[3];

    cmdBuf[0] = engineID;
    cmdBuf[1] = pitch;
    cmdBuf[2] = (filter_Q << 8) | (filter_frequency & 0xFF);    /*Q in hi byte, filter frequency in low */

#if show_parms
    {
        S16 i;

        if (++sound_index > 999)
            sound_index = 0;
        sprintf (snd_buf, "%3dstae", sound_index);
        txt_str (0, 20, snd_buf, WHT_PALB);

        for (i = 0; i < sizeof (cmdBuf) / sizeof (U16); i++)
            txt_hexnum (7 + i * 5, 20, cmdBuf[i], 4, 2, RED_PAL);
    }
#endif

    sndCmds (cmdBuf, 3);

    return 0;
}


/*************************************************************************************
 *        Engine_PF
 *
 */

S32
sndChangeEngine_PF (U16 pitch, U8 filter_frequency, U8 filter_Q)
{
    U16 cmdBuf[3];

    cmdBuf[0] = S_ENGINE_CHANGE_PF;
    cmdBuf[1] = pitch;
    cmdBuf[2] = (filter_Q << 8) | (filter_frequency & 0xFF);    /*Q in hi byte, filter frequency in low */

#if show_parms
    {
        S16 i;

        if (++sound_index > 999)
            sound_index = 0;
        sprintf (snd_buf, "%3dcepf", sound_index);
        txt_str (0, 21, snd_buf, WHT_PALB);

        for (i = 0; i < sizeof (cmdBuf) / sizeof (U16); i++)
            txt_hexnum (7 + i * 5, 21, cmdBuf[i], 4, 2, RED_PAL);
    }
#endif

    sndCmds (cmdBuf, 3);

    return 0;
}

/*************************************************************************************
 *
 *
 */

S32
sndStartEngineWithVolume (U16 engineID, U16 pitch,
                          U8 filter_frequency, U8 filter_Q, U8 volume)
{
    U16 cmdBuf[4];

    cmdBuf[0] = engineID;
    cmdBuf[1] = pitch;
    cmdBuf[2] = (filter_Q << 8) | (filter_frequency & 0xFF);    /*Q in hi byte, filter frequency in low */
    cmdBuf[3] = volume;

#if show_parms
    {
        S16 i;

        if (++sound_index > 999)
            sound_index = 0;
        sprintf (snd_buf, "%3dsewv", sound_index);
        txt_str (0, 22, snd_buf, WHT_PALB);

        for (i = 0; i < sizeof (cmdBuf) / sizeof (U16); i++)
            txt_hexnum (7 + i * 5, 22, cmdBuf[i], 4, 2, RED_PAL);
    }
#endif

    sndCmds (cmdBuf, 4);

    return 0;
}


/*************************************************************************************
 *         Engine object control (Pitch, Filter, Volume)
 *
 */

S32
sndChangeEngineWithVolume (U16 pitch, U8 filter_frequency, U8 filter_Q, U8 volume)
{
    U16 cmdBuf[4];

    cmdBuf[0] = S_ENGINE_CHANGE_PFV;
    cmdBuf[1] = pitch;
    cmdBuf[2] = (filter_Q << 8) | (filter_frequency & 0xFF);    /*Q in hi byte, filter frequency in low */
    cmdBuf[3] = volume;

#if show_parms
    {
        S16 i;

        if (++sound_index > 999)
            sound_index = 0;
        sprintf (snd_buf, "%3dcewv", sound_index);
        txt_str (0, 23, snd_buf, WHT_PALB);

        for (i = 0; i < sizeof (cmdBuf) / sizeof (U16); i++)
            txt_hexnum (7 + i * 5, 23, cmdBuf[i], 4, 2, RED_PAL);
    }
#endif

    sndCmds (cmdBuf, 4);
}

/*************************************************************************************
 *         Automatic Engine model (speed)
 *
 */

S32
sndUpdateAutoEngine (U16 speed)
{
    U16 cmdBuf[2];

    cmdBuf[0] = S_ENGINE_CHANGE_AUTO;
    cmdBuf[1] = ENGINE_VELOCITY_XFORM (speed);

#if show_parms
    {
        S16 i;

        if (++sound_index > 999)
            sound_index = 0;
        sprintf (snd_buf, "%3duae", sound_index);
        txt_str (0, 24, snd_buf, WHT_PALB);

        for (i = 0; i < sizeof (cmdBuf) / sizeof (U16); i++)
            txt_hexnum (7 + i * 5, 24, cmdBuf[i], 4, 2, RED_PAL);
    }
#endif

    sndCmds (cmdBuf, 2);
}

/*************************************************************************************
 *         Start Manual Engine model (rpm and torque)
 *
 */

S32
sndStartManualEngine (U16 engineID, U16 rpm, U16 etorque)
{
    U16 cmdBuf[3];

    if (which_engine >= num_engines)
        which_engine = 0;

    cmdBuf[0] = own_engine[which_engine][0];

    /* range to 16k */
    cmdBuf[1] = (rpm >> own_engine[which_engine][1]) & 0x7FFF;

    if (etorque == 0xffff)
        etorque = 0;

    cmdBuf[2] = etorque & 0x7FFF;       /* +/- 8k */

#if show_parms
    {
        S16 i;

        if (++sound_index > 999)
            sound_index = 0;
        sprintf (snd_buf, "%3dsme", sound_index);
        txt_str (0, 25, snd_buf, WHT_PALB);

        for (i = 0; i < sizeof (cmdBuf) / sizeof (U16); i++)
            txt_hexnum (7 + i * 5, 25, cmdBuf[i], 4, 2, RED_PAL);
    }
#endif

    sndCmds (cmdBuf, 3);

    return 0;
}

/*************************************************************************************
 *         Update Manual Engine model (rpm and torque)
 *
 */

S32
sndUpdateManualEngine (U16 rpm, S16 etorque)
{
    S16 cmdBuf[3];

    if (which_engine >= num_engines)
        which_engine = 0;

    cmdBuf[0] = own_engine[which_engine][0];

    /* range to 16k */
    cmdBuf[1] = (rpm >> own_engine[which_engine][1]) & 0x7FFF;

    cmdBuf[2] = etorque & 0x7FFF;       /* +/- 8k */

#if show_parms
    {
        S16 i;

        if (++sound_index > 999)
            sound_index = 0;
        sprintf (snd_buf, "%3dumae", sound_index);
        txt_str (0, 26, snd_buf, WHT_PALB);

        for (i = 0; i < sizeof (cmdBuf) / sizeof (U16); i++)
            txt_hexnum (7 + i * 5, 26, cmdBuf[i], 4, 2, RED_PAL);
    }
#endif

    sndCmds (cmdBuf, 3);

    return 0;
}

/*************************************************************************************
 *         Automatic Engine model, CRASH call:  Reduce speed as requested
 *
 */

S32
sndEngineCrash (U16 speed)
{
    U16 cmdBuf[2];

    cmdBuf[0] = S_ENGINE_CRASH;
    cmdBuf[1] = ENGINE_VELOCITY_XFORM (speed);

#if show_parms
    {
        S16 i;

        if (++sound_index > 999)
            sound_index = 0;
        sprintf (snd_buf, "%3dec", sound_index);
        txt_str (0, 27, snd_buf, WHT_PALB);

        for (i = 0; i < sizeof (cmdBuf) / sizeof (U16); i++)
            txt_hexnum (7 + i * 5, 27, cmdBuf[i], 4, 2, RED_PAL);
    }
#endif

    sndCmds (cmdBuf, 2);

    return 0;
}


/*************************************************************************************
 *
 *
 */

S32
sndStopEngine (void)
{
#if show_parms
    {
        S16 i;

        if (++sound_index > 999)
            sound_index = 0;
        sprintf (snd_buf, "%3dstoe", sound_index);
        txt_str (0, 28, snd_buf, WHT_PALB);

    }
#endif

    SOUND (S_ENGINE_STOP);
}

/*************************************************************************************
 *
 *
 */

S32
sndKillMoob (U16 handle)
{
    U16 cmdBuf[2];

    cmdBuf[0] = S_MOOB_KILL_OBJECT;
    cmdBuf[1] = handle << 7;

#if show_parms
    {
        S16 i;

        if (++sound_index > 999)
            sound_index = 0;
        sprintf (snd_buf, "%3dkm", sound_index);
        txt_str (0, 29, snd_buf, WHT_PALB);

        for (i = 0; i < sizeof (cmdBuf) / sizeof (U16); i++)
            txt_hexnum (7 + i * 5, 29, cmdBuf[i], 4, 2, RED_PAL);
    }
#endif

    sndCmds (cmdBuf, 2);

    return 0;
}

/*************************************************************************************
 *
 *
 */

S32
sndKillAllMoobs (void)
{
#if show_parms
    {
        S16 i;

        if (++sound_index > 999)
            sound_index = 0;
        sprintf (snd_buf, "%3dkam", sound_index);
        txt_str (30, 29, snd_buf, WHT_PALB);
    }
#endif

    SOUND (S_MOOB_KILL_ALL);
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
   *
   *      Send skid command to DSP
   *        Upon entry: tire -> 0=left, 1=right, 2=front, 3=back
   *                    pitch -> pitch of squeal
   *                    volume -> volume of squeal                              */

S32
sndDoSkid (S16 tire, S16 pitch, S16 volume)
{
    U16 cmdBuf[4], handle;

    /* handle 0-3 */
    handle = car_sound_handle[tire] & 0xFF;

    if (volume == 0)
    {
        sndKillMoob (handle);
        return (4);
    }
    else
    {
        cmdBuf[0] = tire_squeal[tire];
        cmdBuf[1] = handle << 7;
        cmdBuf[2] = pitch;
        cmdBuf[3] = ((volume > MAX_VOLUME) ? MAX_VOLUME : volume);
    }
#if show_parms
    {
        S16 i;

        if (++sound_index > 999)
            sound_index = 0;
        sprintf (snd_buf, "%3dds", sound_index);
        txt_str (0, 30, snd_buf, WHT_PALB);

        for (i = 0; i < sizeof (cmdBuf) / sizeof (U16); i++)
            txt_hexnum (7 + i * 5, 30, cmdBuf[i], 4, 2, RED_PAL);
    }
#endif

    sndCmds (cmdBuf, 4);

    return 0;
}

/*************************************************************************************
 *
 *
 */

S32
sndRoadNoise (S16 pitch, S16 volume)
{
    U16 cmdBuf[4], handle;

    handle = car_sound_handle[4] & 0xFF;

    if (volume == 0)
    {
        sndKillMoob (handle);
        return (4);
    }
    else
    {
        cmdBuf[0] = S_ROADNOISE;
        cmdBuf[1] = handle << 7;
        cmdBuf[2] = pitch;
        cmdBuf[3] = ((volume > MAX_VOLUME) ? MAX_VOLUME : volume);
    }

#if show_parms
    {
        S16 i;

        if (++sound_index > 999)
            sound_index = 0;
        sprintf (snd_buf, "%3drn", sound_index);
        txt_str (0, 31, snd_buf, WHT_PALB);

        for (i = 0; i < sizeof (cmdBuf) / sizeof (U16); i++)
            txt_hexnum (7 + i * 5, 31, cmdBuf[i], 4, 2, RED_PAL);
    }
#endif

    sndCmds (cmdBuf, 4);

    return 0;
}

/*************************************************************************************
 *
 *
 */

S32
sndGravelNoise (S16 pitch, S16 volume)
{
    U16 cmdBuf[4], handle;

    handle = car_sound_handle[5] & 0xFF;

    if (volume == 0)
    {
        sndKillMoob (handle);
        return (4);
    }
    else
    {
        cmdBuf[0] = S_GRAVELNOISE;
        cmdBuf[1] = handle << 7;
        cmdBuf[2] = pitch;
        cmdBuf[3] = ((volume > MAX_VOLUME) ? MAX_VOLUME : volume);
    }

#if show_parms
    {
        S16 i;

        if (++sound_index > 999)
            sound_index = 0;
        sprintf (snd_buf, "%3dgn", sound_index);
        txt_str (0, 32, snd_buf, WHT_PALB);

        for (i = 0; i < sizeof (cmdBuf) / sizeof (U16); i++)
            txt_hexnum (7 + i * 5, 32, cmdBuf[i], 4, 2, RED_PAL);
    }
#endif

    sndCmds (cmdBuf, 4);

    return 0;
}

/*************************************************************************************
 *
 *
 */

S32
sndWindNoise (S16 pitch, S16 volume)
{
    U16 cmdBuf[4], handle;

    handle = car_sound_handle[6] & 0xFF;

    if (volume == 0)
    {
        sndKillMoob (handle);
        return (4);
    }
    else
    {
        cmdBuf[0] = S_WINDNOISE;
        cmdBuf[1] = handle << 7;
        cmdBuf[2] = pitch;
        cmdBuf[3] = ((volume > MAX_VOLUME) ? MAX_VOLUME : volume);
    }

#if show_parms
    {
        S16 i;

        if (++sound_index > 999)
            sound_index = 0;
        sprintf (snd_buf, "%3dwn", sound_index);
        txt_str (0, 33, snd_buf, WHT_PALB);

        for (i = 0; i < sizeof (cmdBuf) / sizeof (U16); i++)
            txt_hexnum (7 + i * 5, 33, cmdBuf[i], 4, 2, RED_PAL);
    }
#endif

    sndCmds (cmdBuf, 4);

    return 0;
}

/*************************************************************************************
 *
 *
 */

S32
sndSplashNoise (S16 pitch, S16 volume)
{
    U16 cmdBuf[4], handle;

    handle = car_sound_handle[7] & 0xFF;

    if (volume == 0)
    {
        sndKillMoob (handle);
        return (4);
    }
    else
    {
        cmdBuf[0] = S_WATERROAD;
        cmdBuf[1] = handle << 7;
        cmdBuf[2] = pitch;
        cmdBuf[3] = ((volume > MAX_VOLUME) ? MAX_VOLUME : volume);
    }

#if show_parms
    {
        S16 i;

        if (++sound_index > 999)
            sound_index = 0;
        sprintf (snd_buf, "%3dspn", sound_index);
        txt_str (0, 34, snd_buf, WHT_PALB);

        for (i = 0; i < sizeof (cmdBuf) / sizeof (U16); i++)
            txt_hexnum (7 + i * 5, 33, cmdBuf[i], 4, 2, RED_PAL);
    }
#endif

    sndCmds (cmdBuf, 4);

    return 0;
}

/*************************************************************************************
 *
 *
 */

S32
doSndTest (U16 snd_id, U16 pitch, U16 volume)
{
    U16 cmdBuf[4], handle;

    handle = car_sound_handle[8] & 0xFF;

    if (volume == 0)
    {
        sndKillMoob (handle);
        return (4);
    }
    else
    {
        cmdBuf[0] = snd_id;
        cmdBuf[1] = handle << 7;
        cmdBuf[2] = pitch;
        cmdBuf[3] = ((volume > MAX_VOLUME) ? MAX_VOLUME : volume);
    }

#if show_parms
    {
        S16 i;

        if (++sound_index > 999)
            sound_index = 0;
        sprintf (snd_buf, "%3ddst", sound_index);
        txt_str (0, 34, snd_buf, WHT_PALB);

        for (i = 0; i < sizeof (cmdBuf) / sizeof (U16); i++)
            txt_hexnum (7 + i * 5, 34, cmdBuf[i], 4, 2, RED_PAL);
    }
#endif

    sndCmds (cmdBuf, 4);

    return 0;
}

/**************************************************************************************/

S32
sndUpdateMoobEngine (U16 objID, U8 handle, U8 priority,
                     S32 x, S32 y,
                     U16 velocity, U16 vel_angle, U16 rpm, U16 etorque)
{
    S16 cmdBuf[9];
    S32 x_pos, y_pos;

    x_pos = LX_COORD_TRANS (x);
    y_pos = LY_COORD_TRANS (y);

    handle &= 0xFF;
    priority &= 0x7F;

    if (objID >= (U16) num_engines)
        objID = 0;

    cmdBuf[0] = other_engine[objID][0];

    /* range to 16k */
    cmdBuf[1] = (rpm >> other_engine[objID][1]) & 0x7FFF;

    if (etorque == 0xffff)
        etorque = 0;

    cmdBuf[2] = etorque & 0x7FFF;       /* +/- 8k */
    cmdBuf[3] = (handle << 7) + priority;
    cmdBuf[4] = x_pos >> 7;
    cmdBuf[5] = y_pos >> 7;
    cmdBuf[6] = ((x_pos << 8) | (y_pos & 0x7f)) & 0x7f7f;
    cmdBuf[7] = MOOB_VELOCITY_XFORM (velocity);
    cmdBuf[8] = vel_angle;
/*
   cmdBuf[8] = ANGLE_COORD_TRANS(vel_angle);
 */

#if show_parms
    {
        S16 i;

        if (++sound_index > 999)
            sound_index = 0;
        sprintf (snd_buf, "%3dumoe", sound_index);
        txt_str (0, 35, snd_buf, WHT_PALB);

        for (i = 0; i < sizeof (cmdBuf) / sizeof (U16); i++)
            txt_hexnum (7 + i * 5, 35, cmdBuf[i], 4, 2, RED_PAL);
    }
#endif


    sndCmds (cmdBuf, 9);

    return 0;
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
   *
   *       Activate miscellaneous sound
   *         Upon entry: sound_command -> command to execute                    */

S32
SOUND (S32 sound_command)
{
    S32 ret_val;
    static S32 last;

    if (!sounds_are_present)
        return 1;

#if show_parms
    {
        if (++sound_index > 999)
            sound_index = 0;
        sprintf (snd_buf, "%3dsnd", sound_index);
        txt_str (0, 36, snd_buf, WHT_PALB);

        txt_hexnum (7, 36, sound_command, 4, 2, RED_PAL);
    }
#endif

    if (sound_command == S_STOP_ALL_SNDS)
    {
        S32 time;

        wait_for_sounds ();
        sndKillAllMoobs ();

        ret_val = aud_put (S_STOP_ALL_SNDS);
        wait_for_sounds ();

        time = IRQTIME;
        while (IRQTIME - time < ONE_SEC / 10);
    }
    else
    {
        S16 cmd = sound_command;

        sndCmds (&cmd, 1);
        ret_val = 0;
    }

    return (ret_val);
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
   *
   *       Activate miscellaneous sound with parameter list
   *         Upon entry: sound_command -> command to execute
   *                     num_parms -> number of 16 bit entries to send
   *                     parm_table -> pointer to array of 16 bit words to send */

S32
sound_wparms (S32 sound_command, S16 num_parms, S16 * parm_table)
{
    S16 i, cmdBuf[20];

    cmdBuf[0] = sound_command;

    for (i = 0; i < num_parms; i++)
        cmdBuf[i + 1] = parm_table[i];

#if show_parms
    {
        if (++sound_index > 999)
            sound_index = 0;
        sprintf (snd_buf, "%3dswp", sound_index);
        txt_str (0, 37, snd_buf, WHT_PALB);

        txt_hexnum (7, 37, sound_command, 4, 2, RED_PAL);
    }
#endif

    sndCmds (cmdBuf, num_parms + 1);
    return 0;
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
   *
   *       Activate music
   *         Upon entry: mode -> Do_it = start playing music
   *                             Undo_it = stop playing music
   *                     radio_station -> music selection to play               */

void
music (S16 mode, U8 radio_station)
{
    S16 sound_command = S_KSTLATT;
    S16 flag;
    S16 *radio_sound[2];

    flag = true;

    if (radio_station > max_radio)
    {
        radio_station -= max_radio + 1;
        radio_sound[0] = (S16 *) & demo_radio_off[0];
        radio_sound[1] = (S16 *) & demo_radio_on[0];
    }
    else
    {
        radio_sound[0] = (S16 *) & demo_radio_off[0];
        radio_sound[1] = (S16 *) & demo_radio_on[0];
    }

    switch (mode)
    {
        case Undo_it:
            if (radio_station >= max_radio)
                radio_station = 0;

            sound_command = radio_sound[0][radio_station];
            flag = false;

            break;

        case Do_it:
            if (radio_station >= max_radio)
                flag = false;
            else
                sound_command = radio_sound[1][radio_station];
            break;
    }

    ctl_mod_latch (flag ? LA_MUSIC : ~LA_MUSIC);

#if show_parms
    {
        if (++sound_index > 999)
            sound_index = 0;
        sprintf (snd_buf, "%3dmus", sound_index);
        txt_str (0, 38, snd_buf, WHT_PALB);

        txt_hexnum (7, 38, sound_command, 4, 2, RED_PAL);
    }
#endif

    aud_put (sound_command);
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
   *
   *       Initialize handles for car sounds                                    */

void
init_car_sound_handles ()
{
    S16 i;

    for (i = 0; i < 9; i++)
        car_sound_handle[i] = get_next_handle ();
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
   *
   *       Wait till sound queue is empty                                       */

void
wait_for_sounds (void)
{
    S32 timeout = IRQTIME;

    while (aud_qlen () != 0 && IRQTIME - timeout < ONE_SEC / 2);
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
   *
   *      Checks if sound are there or not                                      */

void
test_sounds (void)
{
    S32 i, sound_timeout;

    sounds_are_present = true;

    /* flood the cage board */
    for (i = 0; i < 100; i++)
        aud_put (S_GAME_MODE);

    sound_timeout = IRQTIME;

    /* now see if cage sucks them in */
    while (aud_qlen () != 0)
    {
        if (IRQTIME - sound_timeout > ONE_SEC)
            /* nope, cage is fried */
        {
            sounds_are_present = false;
            break;
        }
    }
}
