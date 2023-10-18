/*
 *	debug.c  debug stuff
 *
 *	Copyright 1996 Time Warner Interactive
 *	Unauthorized reproduction, adaptation, distribution, performance or 
 *	display of this computer program or the associated audiovisual work
 *	is strictly prohibited.
 *
 *	======================================================================
 *	$Author: gray $						$Date: 1997/10/13 03:57:32 $
 *	$Revision: 3.37 $						$Locker:  $
 *	======================================================================
 *	Change Log:
 *
 *	$Log: debug.c,v $
 * Revision 3.37  1997/10/13  03:57:32  gray
 * Old-style target fog restored, no default fog for T1-3.
 *
 * Revision 3.36  1997/10/06  07:47:58  gray
 * Reenable fog tweaker.
 *
 * Revision 3.35  1997/09/16  22:02:33  grjost
 * Changed switch 7 back to EngineTweak
 *
 * Revision 3.34  1997/09/15  22:27:35  gray
 * Removed trackset hack - trackno now 0 to NTRACKS.
 *
 * Revision 3.33  1997/08/22  01:32:52  gray
 * Fog tweaker, engine tweaker, and gamenet debug.
 *
 * Revision 3.32  1997/06/04  02:44:44  gray
 * Zapped ithrottle.
 *
 * Revision 3.31  1997/05/30  07:53:15  gray
 * fastout is history.
 *
 * Revision 3.30  1997/05/21  00:10:16  gray
 * caruvs finally exterminated.
 *
 * Revision 3.29  1997/05/11  03:37:10  gray
 * Cleaning model interface.
 *
 * Revision 3.28  1997/05/08  16:26:19  gray
 * Removed obsolete steering wheel debug.
 *
 * Revision 3.27  1997/04/07  22:02:37  gray
 * Cleaning out old nuclues debug.
 *
 * Revision 3.26  1997/03/31  03:16:50  gray
 * Model AST period debug.
 *
 * Revision 3.25  1997/03/31  03:10:37  gray
 * Nucleus gone - got rid of include and nucleus types.
 *
 * Revision 3.24  1997/03/28  19:31:16  gray
 * Removed nucleus memory debug.
 *
 * Revision 3.23  1997/03/28  00:15:09  gray
 * Removed profile stuff.
 *
 * Revision 3.22  1997/01/28  05:44:40  geraci
 * fixed vfprintf stuff
 *
 * Revision 3.21  1996/11/27  12:56:10  gray
 * Changed NetTime from F32 to 10microsec S32.
 *
 * Revision 3.20  1996/11/27  09:23:30  petrick
 * Made one type of Error message work even for production.
 *
 * Revision 3.19  1996/11/27  08:34:04  petrick
 * Put all my messages back on switches.
 *
 * Revision 3.18  1996/11/27  01:48:12  petrick
 * Temporary change to let certain debug messages cut through debug6 turn off.
 *
 * Revision 3.17  1996/11/23  02:03:59  gray
 * Look at Nucleus memory usage.
 *
 * Revision 3.16  1996/11/22  19:13:18  petrick
 * Grrr
 *
 * Revision 3.15  1996/11/22  18:43:40  petrick
 * Made 2nd & 4th raise/lower slower for fine control.
 *
 * Revision 3.14  1996/11/20  12:27:47  gray
 * dynamic shadow tweaker.
 *
 * Revision 3.13  1996/11/19  18:48:25  gray
 * Different size collision boxes.
 *
 * Revision 3.12  1996/11/16  13:02:26  geraci
 * removed some unused game states
 *
 * Revision 3.11  1996/11/16  10:09:22  gray
 * Display MemFree on switch8.
 *
 * Revision 3.10  1996/11/14  13:04:24  gray
 * Added TexHoles to switch 10 display.
 *
 * Revision 3.9  1996/11/07  07:27:37  gray
 * Display remaining texture memory on switch 8.
 *
 * Revision 3.8  1996/11/03  05:58:29  geraci
 * split up debug switches and regular game switches
 *
 * Revision 3.7  1996/10/31  06:55:47  geraci
 * car data is in upper case now
 *
 * Revision 3.6  1996/10/30  00:24:21  gray
 * New suspension tires and roads.
 *
 * Revision 3.5  1996/10/17  08:20:20  petrick
 * Gave less room for the MSG routines.
 *
 * Revision 3.4  1996/09/24  23:37:03  gray
 * ZGLU purge.
 *
 * Revision 3.3  1996/09/17  11:14:22  gray
 * Testing new road surface data.
 *
 * Revision 3.2  1996/09/16  05:18:20  gray
 * Fly mode up down now shifter (not coins).
 *
 * Revision 3.1  1996/09/09  02:24:52  gray
 * JAMMA version
 *
 * Revision 2.39  1996/09/05  05:34:06  geraci
 * fixed fly around mode
 *
 * Revision 2.38  1996/09/04  20:34:54  gray
 * Adding dead reckon data to game_car. Killing mainout.
 *
 * Revision 2.37  1996/09/04  09:37:12  petrick
 * Made orange car slot wok.
 *
 * Revision 2.36  1996/09/02  09:40:20  gray
 * Make net time stuff global.
 *
 * Revision 2.35  1996/09/02  01:23:20  gray
 * Moved initialization of CPU count vars to globals.
 *
 * Revision 2.34  1996/08/30  14:43:55  petrick
 * Made NodeNames upper case.
 *
 * Revision 2.33  1996/08/30  14:41:47  geraci
 * added global inverse to processor time factor
 *
 * Revision 2.32  1996/08/22  05:00:51  petrick
 * Added GameState func.
 *
 * Revision 2.31  1996/08/21  06:00:56  petrick
 * Got rid of stack-specific stuff.
 *
 * Revision 2.30  1996/08/21  05:54:57  gray
 * r5k 200 MHz benchmark stuff.
 *
 * Revision 2.29  1996/08/18  21:24:01  gray
 * Removing zoid stuff. No more zoid.h.
 *
 * Revision 2.28  1996/08/18  03:29:33  gray
 * Removing ZOID stuff.
 *
 * Revision 2.27  1996/08/17  18:27:59  gray
 * Testing 200MHz r5k.
 *
 * Revision 2.26  1996/08/02  08:02:00  gray
 * steering debug.
 *
 * Revision 2.25  1996/08/01  05:05:03  gray
 * steering wheel debug
 *
 * Revision 2.24  1996/07/30  09:15:08  petrick
 * Added SEMSG
 *
 * Revision 2.23  1996/07/30  00:13:52  gray
 * model and tire debug stuff.
 *
 * Revision 2.22  1996/07/23  20:45:00  gray
 * Removed some collision debug routines.
 *
 * Revision 2.21  1996/07/21  21:59:46  gray
 * Added NucTimerTime.
 *
 * Revision 2.20  1996/07/20  00:18:51  gray
 * Slimmed timer interrrupt routine. wheelpos() and forcewheel() moved.
 *
 * Revision 2.19  1996/07/19  09:03:17  petrick
 * Got rid of PALBs in MSG.
 *
 * Revision 2.18  1996/07/19  08:50:36  gray
 * Added mathbox debug numbers to switch 8.
 *
 * Revision 2.17  1996/07/18  06:26:17  gray
 * Fixed some perf text.
 *
 * Revision 2.16  1996/07/18  02:13:57  gray
 * Add HISR execution time to performance measure.
 *
 * Revision 2.15  1996/07/16  02:06:19  gray
 * Exception time measure stuff.
 *
 * Revision 2.14  1996/07/11  01:47:16  gray
 * Added timebar graphic for debug.
 *
 * Revision 2.13  1996/07/07  01:34:00  petrick
 * Made network messages wrap after 16 lines instead of entire screen.
 *
 * Revision 2.12  1996/06/25  19:53:11  gray
 * Print fltinfo on debug switch 11.
 *
 * Revision 2.11  1996/06/21  20:28:35  geraci
 * using model version of level for fly around
 *
 * Revision 2.10  1996/06/20  21:19:22  gray
 * New profile stuff. Missing get_uptime() workaround.
 *
 * Revision 2.9  1996/06/18  19:48:55  gray
 * Put Flyaround mode on Debug Switch 9.
 *
 * Revision 2.8  1996/06/18  04:55:17  geraci
 * flyaround now drops you from where you are to back on the track
 *
 * Revision 2.7  1996/06/18  03:27:13  gray
 * First pass at flyaround.
 *
 * Revision 2.6  1996/06/14  23:23:39  gray
 * Transpose matrix for phoenix in ZGLU_DePosor for UNITVEC.
 *
 * Revision 2.5  1996/06/03  20:54:30  gray
 * Phoenix port.
 *
 * Revision 2.4  1996/05/24  21:23:10  petrick
 * Made MSG && SMSG more efficient.
 *
 * Revision 2.3  1996/05/17  21:27:00  petrick
 * Changed order of SMSG params, added CMSG.
 *
 * Revision 2.2  1996/05/16  19:03:38  petrick
 * Added EMSG
 *
 * Revision 2.1  1996/05/04  02:56:18  petrick
 * Focus 2 release
 *
 * Revision 1.14  1996/04/25  09:24:39  gray
 * Orbit mode debug.
 *
 * Revision 1.13  1996/04/23  22:54:03  gray
 * Removed the FocusI defeeb debug.
 *
 * Revision 1.12  1996/04/10  00:03:28  petrick
 * Error messages override switch settings.
 *
 * Revision 1.11  1996/04/01  08:53:48  gray
 * More steering debug and feeb assist debug.
 *
 * Revision 1.10  1996/03/31  11:05:14  gray
 * Steering force debug.
 *
 * Revision 1.9  1996/03/29  01:50:12  gray
 * Changed profile setup from number count to msec for avg.
 *
 * Revision 1.8  1996/03/28  12:27:08  gray
 * Made generic debug print after ZOID_EndFrame, Overhaul of AG debug.
 *
 * Revision 1.7  1996/03/16  02:02:44  petrick
 * Moved NodeName func here.
 *
 * Revision 1.6  1996/03/16  02:01:40  gray
 * collision debug.
 *
 * Revision 1.5  1996/03/14  17:49:33  gray
 * Temporarily put ZGLU stuff here.
 *
 * Revision 1.4  1996/03/12  12:34:51  gray
 * Collision debug stuff.
 *
 * Revision 1.3  1996/03/08  01:10:43  petrick
 * Moved Jim's debug stuff here.
 *
 * Revision 1.2  1996/03/07  05:38:00  gray
 * Add DBG_ prefix, empty routines of DO_PROFILE = 0, add DBG_print_model_info.
 *
 * Revision 1.1  1996/03/07  01:42:15  gray
 * Initial revision
 *
 *
 *	======================================================================
 */

#include <config.h>
#include <string.h>

#include "globals.h"
#include "os_proto.h"
#include "dis.h"
#include "blit.h"
#include "game.h"
#include "gamenet.h"
#include "debug.h"
#include "drivsym.h"
#include "mem.h"

#include "Pro/math.pro"
#include "Pro/mathsubs.pro"
#include "Pro/unitvecs.pro"
#include "Pro/vecmath.pro"

char 		dbgstr[80];
S32 DBG_AG_magic;
/*DBG_PROFILE DBG_profile[DBG_MAXPROFILES];*/
F32 		DBG_msbtime,DBG_lsbtime;
U32 DBG_last_count;	/*Temp workaround until get_uptime works for Phoenix.*/
U32 DBG_count_roll;     /*Temp workaround until get_uptime works for Phoenix.*/

S32 		gLine, gPal, gErrnum;				/*	Jim's debugging stuff. */

extern U32 prc_get_count(void);

void DBG_init_debug(void)
{
/*	DBG_init_profile();*/
	DBG_init_AG_debug();
}

void DBG_print_debug(void)
{
	DBG_print_AG_debug();
}

void DBG_txt_clr(S32 row,S32 cnt)
{
	S32 i;

	for(i=0;i<cnt;i++)
		txt_clr_wid(0,row+i,64);
}

void DBG_printf(S32 col,S32 row,S32 pal, char *fmt,... )
{
	va_list 	ap;

	va_start(ap,fmt);
	nvfprintf(dbgstr,256,fmt,ap);
	va_end(ap);

	txt_str(col,row,dbgstr,pal);

}

/***************** AG debug stuff ******************/

char DBG_fltinfo[200];

#define DBG_AG_DBGMAX 3
S32 DBG_AG_debug_num;
S32 DBG_offcnt8;

S32 DBG_timebar_blits_setup;


void DBG_init_AG_debug(void)
{
	DBG_AG_magic = 0;
	DBG_fltinfo[0] = 0;

	DBG_init_perf();
	DBG_initFogger();
}


U32 dbgFogColor[MAX_TRACKS];
F32 dbgFogMinDist[MAX_TRACKS];
F32 dbgFogMaxDist[MAX_TRACKS];
F32 dbgFogMaxLevel[MAX_TRACKS];

const U32 defFogColors[MAX_TRACKS] =
{ 0x000000, 0x000000, 0x000000, 0xFFE3C4, 0xDCFFFF, 0xDCFFFF, 0xDCFFFF, 0xDCFFFF };
const F32 defFogMinDist[MAX_TRACKS] =
{ 200.0, 200.0, 200.0, 200.0, 200.0, 200.0, 200.0, 200.0 };
const F32 defFogMaxDist[MAX_TRACKS] =
{ 2000.0, 2000.0, 2000.0, 2000.0, 2000.0, 2000.0, 2000.0, 2000.0 };
const F32 defFogMaxLevel[MAX_TRACKS] =
{ 0.0, 0.0, 0.0, 0.2, 1.0, 1.0, 1.0, 1.0 };

void DBG_initFogger(void)
{
	S32 i;

	for(i=0;i<MAX_TRACKS;i++)
		{
		dbgFogColor[i] = defFogColors[i];
		dbgFogMinDist[i] = defFogMinDist[i];
		dbgFogMaxDist[i] = defFogMaxDist[i];
		dbgFogMaxLevel[i] = defFogMaxLevel[i];
		}
}

F32 xMgUvPos[3],xMgUv[3][3];
S32 xMgUvObj;

void DBG_AddMgUv(void)
{
	S32 i,j;
	extern F32 xUv[3][3];

	for(i=0;i<3;i++)
		{
		xMgUvPos[i] = 0.0;
		for(j=0;j<3;j++)
			{
			xMgUv[i][j] = (i == j) ? 1.0 : 0.0;
			xUv[i][j] = xMgUv[i][j];
			}
		}


	i = MBOX_FindObject("MGUVARROWS");
	xMgUvObj = MBOX_NewObject(i, xMgUvPos, xMgUv, -1, 0);
}

#define TWEAK_SHADOW 0
#if TWEAK_SHADOW
S32 gMod;
F32 gScale[3];
#endif

extern void fmodtodispuv(F32 moduv[3][3], F32 camuv[3][3]);

void DBG_print_AG_debug(void)
{
        S32 i,j;

	if((dlevels & SW_DEBUG8) && (gstate == PREPLAY2))
		DBG_AddMgUv();

/*xxx*/

#if 1
{
extern S32 dbgBCastCnt;
extern S32 nettime_synced;
	DBG_printf(24,41,YEL_PALB,"BCastMsgCnt   :%ld    ",dbgBCastCnt);
	DBG_printf(24,42,YEL_PALB,"nettime_synced:%ld    ",nettime_synced);
}
#endif

#if 0
{
	DBG_printf(24,41,YEL_PALB,"suscomp0:%0.3f    ",model[gThisNode].suscomp[0]);
	DBG_printf(24,42,YEL_PALB,"suscomp1:%0.3f    ",model[gThisNode].suscomp[1]);
	DBG_printf(24,43,YEL_PALB,"suscomp2:%0.3f    ",model[gThisNode].suscomp[2]);
	DBG_printf(24,44,YEL_PALB,"suscomp3:%0.3f    ",model[gThisNode].suscomp[3]);
}
#endif

#if 0
{
extern S32 upsec_inc;
	DBG_printf(24,41,YEL_PALB,"IRQTIME  :%ld    ",IRQTIME);
	DBG_printf(24,42,YEL_PALB,"UpSeconds:%ld    ",gUpSeconds);
	DBG_printf(24,43,YEL_PALB,"upset_inc:%ld    ",upsec_inc);
}
#endif

#if 0
{
	extern F32 dbgZf;
	extern F32 dbgYf;
	extern S32 gTireCnt;
	extern F32 gSusAvg;
	DBG_printf(30,41,YEL_PALB,"BackF:%0.3f    ",dbgZf);
	DBG_printf(30,42,YEL_PALB,"upF  :%0.3f    ",dbgYf);
}
#endif

#if 0
{
	for(i=0;i<MAX_LINKS;i++)
		{
		DBG_printf(10,38+i,YEL_PALB,"%ld gc.mph:%0.2f gc.ap:%08lX   ",i,
			game_car[i].mph,
			game_car[i].appearance);
		}
	DBG_printf(10,38+gThisNode,RED_PALB,"%ld",(S32)gThisNode);
}
#endif


#if 0
{
	F32 uvs[3][3], quat[4];
	fmatcopy(model[gThisNode].reckon.base_UV[0], uvs[0]);
	make_quat_from_uvs (uvs, quat);
	DBG_printf(20,44,YEL_PALB,"q0:%0.3f    ",quat[0]);
	DBG_printf(30,44,YEL_PALB,"q1:%0.3f    ",quat[1]);
	DBG_printf(40,44,YEL_PALB,"q2:%0.3f    ",quat[2]);
	DBG_printf(50,44,YEL_PALB,"q3:%0.3f    ",quat[3]);
	uvs[1][0] = -uvs[1][0];
	uvs[1][2] = -uvs[1][2];
	uvs[0][1] = -uvs[0][1];
	uvs[2][1] = -uvs[2][1];
	make_quat_from_uvs (uvs, quat);
	DBG_printf(20,45,YEL_PALB,"q0:%0.3f    ",quat[0]);
	DBG_printf(30,45,YEL_PALB,"q1:%0.3f    ",quat[1]);
	DBG_printf(40,45,YEL_PALB,"q2:%0.3f    ",quat[2]);
	DBG_printf(50,45,YEL_PALB,"q3:%0.3f    ",quat[3]);
}
#endif

#if 0
{
	static F32 dbgCUcnt;
	static F32 dbgCUsum;
	static F32 dbgCUavg;
	if(gstate == COUNTDOWN)
		{
		dbgCUcnt = 0.0;
		dbgCUsum = 0.0;
		dbgCUavg = 0.0;
		}
	else if(gstate == PLAYGAME)
		{
		dbgCUcnt += 1.0;
		dbgCUsum += model[gThisNode].catchup;
		dbgCUavg = dbgCUsum/dbgCUcnt;
		}
	DBG_printf(30,43,RED_PALB,"CUcur:%0.3f    ",model[gThisNode].catchup);
	DBG_printf(30,44,RED_PALB,"CUcnt:%ld    ",(S32)dbgCUcnt);
	DBG_printf(30,45,RED_PALB,"CUavg:%0.3f    ",dbgCUavg);
}
#endif

#if 0
/*Gamenet debug*/
{
extern U32 gLSfiltcalls;
extern U32 gLSfiltaccs;
extern U32 gLSrcvcalls;
extern S32 gLSpktno;
extern S32 gLSplen;
extern U32 gLSp1;
extern U32 gLSp2;
extern U32 gLSp3;
extern U32 gLSdat[8];

        extern S32 LS_rcvcnt[MAX_LINKS];
        extern S32 LS_rcverr[MAX_LINKS];
        extern S32 LS_sndcnt;

	for(i=0;i<HIGHEST_GNERR;i++)
		DBG_printf(2,10+i,RED_PALB,"GNerr%ld:%ld    ",i,GNerrcnt[i]);

	DBG_printf(20,10,RED_PALB,"gLSfiltcnt:%ld    ",gLSfiltcalls);
	DBG_printf(20,11,RED_PALB,"gLSrcvcnt:%ld    ",gLSrcvcalls);
	DBG_printf(20,12,RED_PALB,"gLSpktno:%ld    ",gLSpktno);
	DBG_printf(20,13,RED_PALB,"gLSplen:%ld    ",gLSplen);
	DBG_printf(20,14,RED_PALB,"gLSp1:%ld    ",gLSp1);
	DBG_printf(20,15,RED_PALB,"gLSp2:%ld    ",gLSp2);
	DBG_printf(20,16,RED_PALB,"gLSp3:%ld    ",gLSp3);

	for(i=0;i<8;i++)
		DBG_printf(40,10+i,RED_PALB,"gLSdat%ld:0x%08lX    ",i,gLSdat[i]);

	DBG_printf(20,18,RED_PALB,"gThisNode=%ld:%ld sndcnt:%ld    ",
			gThisNode, (S32)linkStat[gThisNode].state, LS_sndcnt);
	for(i=0;i<MAX_LINKS;i++)
		{
		DBG_printf(20,20+i,YEL_PALB,"state%ld:%ld  rcvcnt:%ld     ",
				i, (S32)linkStat[i].state, linkStat[i].rcvcnt);
		}


}
#endif

#if 0
	DBG_printf(20,20,YEL_PALB,"dedges :0x%08lX    ",dedges);
	DBG_printf(20,21,YEL_PALB,"dlevels:0x%08lX    ",dlevels);
#endif

#if 0
{
	static F32 maxperiod,minperiod;
	extern U32 mast_period;
	extern S32 game_block;
	F32 period;

	if((levels & SW_REVERSE) || (maxperiod == 0.0))
		{
		maxperiod = 0.0;
		minperiod = 1000.0;
		game_block = 0;
		}

	period = (F32)mast_period*gMsecPerCount;
	if(period > maxperiod)
		maxperiod = period;
	if(period < minperiod)
		minperiod = period;
	DBG_printf(30,19,RED_PALB,"CPU_SPEED   :%ld    ",CPU_SPEED);
	DBG_printf(30,20,YEL_PALB,"model_period:%0.3f    ",period);
	DBG_printf(30,21,YEL_PALB,"max_period  :%0.3f    ",maxperiod);
	DBG_printf(30,22,YEL_PALB,"min_period  :%0.3f    ",minperiod);
	DBG_printf(30,23,RED_PALB,"game_block  :%ld    ",game_block);
}
#endif

#if TWEAK_SHADOW
	if(edges & SW_REVERSE)
		gMod++;
	if((gMod < 0) || (gMod > 2))
		gMod = 0;

	if(levels & SW_COINL)
		gScale[gMod] -= 0.01;
	else if(levels & SW_COINR)
		gScale[gMod] += 0.01;

	if(gScale[gMod] < 0.01)
		gScale[gMod] = 0.01;

	if(gMod == 0)
		{
		DBG_printf(10,32,RED_PAL,"wscale:%0.2f  ",gScale[0]);
		DBG_printf(10,33,WHT_PAL,"fscale:%0.2f  ",gScale[1]);
		DBG_printf(10,34,WHT_PAL,"bscale:%0.2f  ",gScale[2]);
		}
	else if(gMod == 1)
		{
		DBG_printf(10,32,WHT_PAL,"wscale:%0.2f  ",gScale[0]);
		DBG_printf(10,33,RED_PAL,"fscale:%0.2f  ",gScale[1]);
		DBG_printf(10,34,WHT_PAL,"bscale:%0.2f  ",gScale[2]);
		}
	else
		{
		DBG_printf(10,32,WHT_PAL,"wscale:%0.2f  ",gScale[0]);
		DBG_printf(10,33,WHT_PAL,"fscale:%0.2f  ",gScale[1]);
		DBG_printf(10,34,RED_PAL,"bscale:%0.2f  ",gScale[2]);
		}
#endif

#if 1
{
extern S16 	rawswvel,swvel;
extern U32 gMBavg;
extern U32 tex_holes;
	if(dlevels & SW_DEBUG8)
		{
		DBG_printf(1,42,WHT_PAL,"MemFree :%ldK     ",(S32)BytesFree()/1024);
		DBG_printf(1,43,WHT_PAL,"TexFree :%ldK     ",((S32)0x800000 - (S32)MBOX_GetTexBase())/1024);
		DBG_printf(1,44,WHT_PAL,"TexHoles:%ldK     ",((S32)tex_holes)/1024);
#if 0
		DBG_printf(20,14,WHT_PALB,"rawswvel:%d    ",swvel);
		DBG_printf(20,15,WHT_PALB,"rawswvel:%d    ",rawswvel);
		DBG_printf(20,16,WHT_PALB,"fastin.wheel:%d    ",model[gThisNode].fastin.wheel);
		DBG_printf(20,17,WHT_PALB,"fastin.swvel:%d    ",model[gThisNode].fastin.swvel);
#elif 0
		DBG_printf(20,14,WHT_PALB,"roadboost[0]:%d    ",model[gThisNode].roadboost[0]);
		DBG_printf(20,15,WHT_PALB,"sound_flags[0]:%d    ",model[gThisNode].sound_flags[0]);
		DBG_printf(20,16,WHT_PALB,"shortcut_flags[0]:%d    ",model[gThisNode].shortcut_flags[0]);
#elif 0
{		
	S32 i,j;
	F32 tf[4];
	MODELDAT *m = &model[gThisNode];
		for(i=0;i<4;i++)
			for(tf[i]=0.0,j=0;j<3;j++)
				tf[i] += (m->TIREFORCE[i][j] < 0.0) ? -m->TIREFORCE[i][j] : m->TIREFORCE[i][j];
		DBG_printf(30,40,WHT_PALB,"tireforce(0):%0.2f    ",tf[0]);
		DBG_printf(30,41,WHT_PALB,"tireforce(1):%0.2f    ",tf[1]);
		DBG_printf(30,42,WHT_PALB,"tireforce(2):%0.2f    ",tf[2]);
		DBG_printf(30,43,WHT_PALB,"tireforce(3):%0.2f    ",tf[3]);

		DBG_printf(10,40,WHT_PALB,"suscomp(0):%0.2f    ",model[gThisNode].suscomp[0]);
		DBG_printf(10,41,WHT_PALB,"suscomp(1):%0.2f    ",model[gThisNode].suscomp[1]);
		DBG_printf(10,42,WHT_PALB,"suscomp(2):%0.2f    ",model[gThisNode].suscomp[2]);
		DBG_printf(10,43,WHT_PALB,"suscomp(3):%0.2f    ",model[gThisNode].suscomp[3]);
}
#elif 0
{
		extern U16 xxxNumSurfs;
		extern S32 xxxSurfDist;
		DBG_printf(20,40,WHT_PALB,"NumSurfs:%d    ",xxxNumSurfs);
		DBG_printf(20,41,WHT_PALB,"SurfDist:%ld    ",xxxSurfDist);
}
#elif 0
{
		extern F32 xxxColDist;
		extern F32 xxxColForce;
		DBG_printf(20,40,WHT_PALB,"ColDist :%0.2f    ",xxxColDist);
		DBG_printf(20,41,WHT_PALB,"ColForce:%0.2f    ",xxxColForce);
}
#elif 0
{
extern F32 xFRpos[3];
extern F32 Xscale[6];
extern F32 xDel1,xDel2,xInterp;
extern F32 xUv[3][3];
static F32 scmax[4];
static F32 scale[6];
static S32 scalei;
MODELDAT *m = &model[gThisNode];
		if(gstate != PLAYGAME)
			{
			for(i=0;i<4;scmax[i]=0.0,i++);
			for(i=0;i<6;scale[i]=2.2,i++);
			for(i=0;i<6;Xscale[i]=2.2,i++);
			scalei = 0;
			}
		else
			{
			if(edges & SW_REVERSE)
				{
				for(i=0;i<4;scmax[i]=0.0,i++);
				for(i=0;i<6;Xscale[i]=scale[i],i++);
				scalei = (scalei < 5) ? scalei+1 : 0;
/*				new_sus(scale);*/
				}

			if((levels & SW_VIEW1) && (edges & SW_VIEW2) && (scale[scalei] > 0.0))
				scale[scalei] -= 0.1;
				
			if((levels & SW_VIEW1) && (edges & SW_VIEW3) && (scale[scalei] < 9.9))
				scale[scalei] += 0.1;

/*			if((levels & SW_MUSIC) && ((edges & SW_VIEW2) || (edges & SW_VIEW3)))
				mod_susval(scalei);*/

			for(i=0;i<4;i++)
				if(m->suscomp[i] > scmax[i])
					scmax[i] = m->suscomp[i];


			DBG_printf(20,30,WHT_PAL ,          "FRONT  REAR");
			DBG_printf(10,31,WHT_PAL ,"spring:");
			DBG_printf(10,32,WHT_PAL ,"Cdamp :");
			DBG_printf(10,33,WHT_PAL ,"Rdamp :");

			DBG_printf(21,31,(scalei==0) ? YEL_PALB : WHT_PAL,"%0.1f",scale[0]);
			DBG_printf(28,31,(scalei==1) ? YEL_PALB : WHT_PAL,"%0.1f",scale[1]);
			DBG_printf(21,32,(scalei==2) ? YEL_PALB : WHT_PAL,"%0.1f",scale[2]);
			DBG_printf(28,32,(scalei==3) ? YEL_PALB : WHT_PAL,"%0.1f",scale[3]);
			DBG_printf(21,33,(scalei==4) ? YEL_PALB : WHT_PAL,"%0.1f",scale[4]);
			DBG_printf(28,33,(scalei==5) ? YEL_PALB : WHT_PAL,"%0.1f",scale[5]);


			DBG_printf(15,38,WHT_PALB,"FL :%0.2f    ",m->suscomp[1]);
			DBG_printf(15,39,WHT_PALB,"max:%0.2f    ",scmax[1]);
			DBG_printf(30,38,WHT_PALB,"FR :%0.2f    ",m->suscomp[0]);
			DBG_printf(30,39,WHT_PALB,"max:%0.2f    ",scmax[0]);

			DBG_printf(15,42,WHT_PALB,"RL :%0.2f    ",m->suscomp[3]);
			DBG_printf(15,43,WHT_PALB,"max:%0.2f    ",scmax[3]);
			DBG_printf(30,42,WHT_PALB,"RR :%0.2f    ",m->suscomp[2]);
			DBG_printf(30,43,WHT_PALB,"max:%0.2f    ",scmax[2]);

			DBG_printf( 4,45,WHT_PALB,"TIREV X:%0.2f    ",m->TIREV[0][0]);
			DBG_printf(20,45,WHT_PALB,      "Y:%0.2f    ",m->TIREV[0][1]);
			DBG_printf(30,45,WHT_PALB,      "Z:%0.2f    ",m->TIREV[0][2]);

			DBG_printf(42,30,WHT_PALB,"del1  :%0.6f    ",xDel1);
			DBG_printf(42,31,WHT_PALB,"del2  :%0.6f    ",xDel2);
			DBG_printf(42,32,WHT_PALB,"interp:%0.6f    ",xInterp);

			DBG_printf(25,34,WHT_PALB,"x0:%0.4f ",xUv[0][0]);
			DBG_printf(25,35,WHT_PALB,"x1:%0.4f ",xUv[1][0]);
			DBG_printf(25,36,WHT_PALB,"x2:%0.4f ",xUv[2][0]);

			DBG_printf(35,34,WHT_PALB,"y0:%0.4f ",xUv[0][1]);
			DBG_printf(35,35,WHT_PALB,"y1:%0.4f ",xUv[1][1]);
			DBG_printf(35,36,WHT_PALB,"y2:%0.4f ",xUv[2][1]);

			DBG_printf(45,34,WHT_PALB,"z0:%0.4f ",xUv[0][2]);
			DBG_printf(45,35,WHT_PALB,"z1:%0.4f ",xUv[1][2]);
			DBG_printf(45,36,WHT_PALB,"z2:%0.4f ",xUv[2][2]);

			xMgUvPos[0] = xFRpos[1];
			xMgUvPos[1] = -xFRpos[2];
			xMgUvPos[2] = xFRpos[0];
			fmodtodispuv(xUv,xMgUv);
			Transpose3x3(xMgUv);
			ZOID_UpdateObject(xMgUvObj,xMgUvPos,xMgUv);
			}
}
#endif
		}
	else if(trailing_dedges & SW_DEBUG8)
		DBG_txt_clr(14,4);
}
#endif

#if 0
	/*model debug*/
	if(dlevels & SW_DEBUG8)
		DBG_print_model_info(0);
	else if(trailing_dedges & SW_DEBUG8)
		DBG_print_model_info(1);
#endif

	/************ DEBUG SWITCH 7 ***************/
	if(dlevels & SW_DEBUG7)
/*		DBG_doEngineTweak();*/
		DBG_doFogger();


	/************ DEBUG SWITCH 8 ***************/
	/*flag when blits are zapped*/
	if(gstate == ENDGAME)
		{
		DBG_timebar_blits_setup = 0;
		DBG_txt_clr(46,1);	/*clear timebar labels*/
		}

	if(dedges & SW_DEBUG8)
		{
		DBG_AG_debug_num = (DBG_offcnt8 > 30) ? 0 : DBG_AG_debug_num + 1;
		DBG_AG_debug_num = (DBG_AG_debug_num > DBG_AG_DBGMAX) ? 0 : DBG_AG_debug_num;
		DBG_offcnt8 = 0;
		}

	if(dlevels & SW_DEBUG8)
		DBG_disp_perf(0);		/*display perf*/
	else if(trailing_dedges & SW_DEBUG8)
		DBG_disp_perf(1);		/*erase perf*/
	else
		DBG_offcnt8++;			/*count off time*/


	/************ DEBUG SWITCH 10 ***************/
	if(dlevels & SW_DEBUG10)
		{
		txt_str(1,40,"COLLISION:OFF ",YEL_PALB);
		txt_str(1,41,"CATCHUP:FULL ",YEL_PALB);		
		}
	else if(trailing_dedges & SW_DEBUG10)
		{
		DBG_txt_clr(40,2);
		}


	/************ DEBUG SWITCH 11 ***************/
	if(dlevels & SW_DEBUG11)
		{
		DBG_print_fps(0);
/*		txt_str(2,1,DBG_fltinfo,YEL_PALB);*/
/*		ZOID_PrintStats();*/
		}
	else if(trailing_dedges & SW_DEBUG11)
		{
		DBG_print_fps(1);
/*		txt_clr_wid(0,1,64);*/
		}
}

#define DBG_MAXFOGFIELD 5
void DBG_doFogger(void)
{
	static S32 curfield;
	static U32 presstime;
	F32 dir,step,del;
	S32 val;
	U32 holdtime = 0;
	S32 trk = trackno;

	/* change field. */
	if(edges & SW_MUSIC)
		curfield--;
	else if(edges & SW_VIEW2)
		curfield++;
	
	/* keep field in range. */
	if(curfield < 0)
		curfield = DBG_MAXFOGFIELD;
	else if(curfield > DBG_MAXFOGFIELD)
		curfield = 0;

	/* determine whether to increment or decrement. */
	if(levels & SW_VIEW1)
		dir = -1.0;
	else
		dir = 1.0;

	/* determine amount to inc/dec. */
	if((edges & SW_VIEW1)||(edges & SW_VIEW3))
		{
		presstime = IRQTIME;
		step = 1.0;
		}
	else if((levels & SW_VIEW1)||(levels & SW_VIEW3))
		{
		holdtime = IRQTIME - presstime;
		if(holdtime > 2000)
			step = 4.0;
		else if(holdtime > 250)
			step = 1.0;
		else
			step = 0.0;
		}
	else
		step = 0.0;

	switch(curfield)
		{
		case 0:	/* Red */
			del = dir * step;
			val = (dbgFogColor[trk] >> 16) & 0xFF;
			val += del;
			if(val > 0xFF) val = 0xFF;
			else if(val < 0) val = 0;
			dbgFogColor[trk] = (dbgFogColor[trk] & 0xFFFF) | (val << 16);
			break;
		case 1:	/* Green */
			del = dir * step;
			val = (dbgFogColor[trk] >> 8) & 0xFF;
			val += del;
			if(val > 0xFF) val = 0xFF;
			else if(val < 0) val = 0;
			dbgFogColor[trk] = (dbgFogColor[trk] & 0xFF00FF) | (val << 8);
			break;
		case 2:	/* Blue */
			del = dir * step;
			val = (dbgFogColor[trk] >> 0) & 0xFF;
			val += del;
			if(val > 0xFF) val = 0xFF;
			else if(val < 0) val = 0;
			dbgFogColor[trk] = (dbgFogColor[trk] & 0xFFFF00) | (val << 0);
			break;
		case 3:	/* FogMinDist */
			del = dir * step;
			dbgFogMinDist[trk] += del;
			if(dbgFogMinDist[trk] < 0.0)
				dbgFogMinDist[trk] = 0.0;
			if(dbgFogMaxDist[trk] < dbgFogMinDist[trk])
				dbgFogMaxDist[trk] = dbgFogMinDist[trk];
			break;
		case 4:	/* FogMaxDist */
			del = dir * step;
			dbgFogMaxDist[trk] += del;
			if(dbgFogMaxDist[trk] < 0.0)
				dbgFogMaxDist[trk] = 0.0;
			if(dbgFogMaxDist[trk] < dbgFogMinDist[trk])
				dbgFogMinDist[trk] = dbgFogMaxDist[trk];
			break;
		case 5:	/* FogMaxLevel */
			del = dir * step * 0.001;
			dbgFogMaxLevel[trk] += del;
			if(dbgFogMaxLevel[trk] < 0.0)
				dbgFogMaxLevel[trk] = 0.0;
			else if(dbgFogMaxLevel[trk] > 1.0)
				dbgFogMaxLevel[trk] = 1.0;
			break;
		default:
			break;
		}

	DBG_drawFogger(curfield);
}

void DBG_drawFogger( S32 field )
{
	S32 toprow = 40;
	S32 col = 20;
	S32 row,i,j;
	U32 color;
	S32 trk = trackno;

	row = toprow;
	DBG_printf(col,row++,RED_PALB,"FogColorR  :");
	DBG_printf(col,row++,RED_PALB,"FogColorG  :");
	DBG_printf(col,row++,RED_PALB,"FogColorB  :");
	DBG_printf(col,row++,RED_PALB,"FogMinDist :");
	DBG_printf(col,row++,RED_PALB,"FogMaxDist :");
	DBG_printf(col,row++,RED_PALB,"FogMaxLevel:");

	row = toprow;
	col += 12;
	i = 0;
	j = field + 1;
	color = dbgFogColor[trk];
	DBG_printf(col,row+i++,(i==j)?YEL_PALB:RED_PALB,"%3ld  ", ( color >> 16) & 0xFF );
	DBG_printf(col,row+i++,(i==j)?YEL_PALB:RED_PALB,"%3ld  ", ( color >>  8) & 0xFF );
	DBG_printf(col,row+i++,(i==j)?YEL_PALB:RED_PALB,"%3ld  ", ( color >>  0) & 0xFF );
	DBG_printf(col,row+i++,(i==j)?YEL_PALB:RED_PALB,"%0.2f  ", dbgFogMinDist[trk] );
	DBG_printf(col,row+i++,(i==j)?YEL_PALB:RED_PALB,"%0.2f  ", dbgFogMaxDist[trk] );
	DBG_printf(col,row+i++,(i==j)?YEL_PALB:RED_PALB,"%0.4f  ", dbgFogMaxLevel[trk] );
}

extern F32 dbgRPMbias;
extern F32 dbgRPMscale;
extern S32 dbgRPMval;
extern F32 dbgTORbias;
extern F32 dbgTORscale;
extern S32 dbgTORval;


#define DBG_MAXENGFIELD 3
void DBG_doEngineTweak(void)
{
	static S32 curfield;
	static U32 presstime;
	F32 dir,step,del;
	S32 val;
	U32 holdtime = 0;

	/* change field. */
	if(edges & SW_MUSIC)
		curfield--;
	else if(edges & SW_VIEW2)
		curfield++;
	
	/* keep field in range. */
	if(curfield < 0)
		curfield = DBG_MAXENGFIELD;
	else if(curfield > DBG_MAXENGFIELD)
		curfield = 0;

	/* determine whether to increment or decrement. */
	if(levels & SW_VIEW1)
		dir = -1.0;
	else
		dir = 1.0;

	/* determine amount to inc/dec. */
	if((edges & SW_VIEW1)||(edges & SW_VIEW3))
		{
		presstime = IRQTIME;
		step = 1.0;
		}
	else if((levels & SW_VIEW1)||(levels & SW_VIEW3))
		{
		holdtime = IRQTIME - presstime;
		if(holdtime > 2000)
			step = 10.0;
		else if(holdtime > 250)
			step = 1.0;
		else
			step = 0.0;
		}
	else
		step = 0.0;

	switch(curfield)
		{
		case 0:	/* RPMbias */
			del = dir * step * 10.0;
			dbgRPMbias += del;
			if(dbgRPMbias < -10000.0)
				dbgRPMbias = -10000.0;
			else if(dbgRPMbias > 10000.0)
				dbgRPMbias = 10000.0;
			break;
		case 1:	/* RPMscale */
			del = dir * step * 0.001;
			dbgRPMscale += del;
			if(dbgRPMscale < 0.0)
				dbgRPMscale = 0.0;
			else if(dbgRPMscale > 10.0)
				dbgRPMscale = 10.0;
			break;
		case 2:	/* TORbias */
			del = dir * step * 10.0;
			dbgTORbias += del;
			if(dbgTORbias < -10000.0)
				dbgTORbias = -10000.0;
			else if(dbgTORbias > 10000.0)
				dbgTORbias = 10000.0;
			break;
		case 3:	/* TORscale */
			del = dir * step * 0.001;
			dbgTORscale += del;
			if(dbgTORscale < 0.0)
				dbgTORscale = 0.0;
			else if(dbgTORscale > 10.0)
				dbgTORscale = 10.0;
			break;
		default:
			break;
		}

	DBG_drawEngTweak(curfield);
}

void DBG_drawEngTweak( S32 field )
{
	S32 toprow = 40;
	S32 col = 20;
	S32 row,i,j;
	U32 color;

	row = toprow;
	DBG_printf(col,row++,RED_PALB,"RPM Bias   :");
	DBG_printf(col,row++,RED_PALB,"RPM Scale  :");
	DBG_printf(col,row++,RED_PALB,"TorqueBias :");
	DBG_printf(col,row++,RED_PALB,"TorqueScale:");

	row = toprow;
	col += 12;
	i = 0;
	j = field;
	DBG_printf(col,row+i,(i++ ==j)?YEL_PALB:RED_PALB,"%0.1f  ", dbgRPMbias );
	DBG_printf(col,row+i,(i++ ==j)?YEL_PALB:RED_PALB,"%0.4f  ", dbgRPMscale );
	DBG_printf(col,row+i,(i++ ==j)?YEL_PALB:RED_PALB,"%0.1f  ", dbgTORbias );
	DBG_printf(col,row+i,(i++ ==j)?YEL_PALB:RED_PALB,"%0.4f  ", dbgTORscale );


	col += 12;
	i = 0;
	DBG_printf(col,row+i,RED_PALB,"CurRPM:");
	DBG_printf(col+7,row+i,YEL_PALB,"%ld      ",dbgRPMval);
	i+=2;
	DBG_printf(col,row+i,RED_PALB,"CurTOR:");
	DBG_printf(col+7,row+i,YEL_PALB,"%ld      ",dbgTORval);
}

#if 1
void new_sus(F32 scale[6])
{
	MODELDAT *m;
	S32 i;
	F32 fspring,rspring,fcdamp,rcdamp,frdamp,rrdamp,ftirez,rtirez;

	fspring = -1500.0 * scale[0];
	rspring = -1750.0 * scale[1];
	fcdamp =  -400.0  * scale[2];
	rcdamp =  -500.0  * scale[3];
	frdamp =  -200.0  * scale[4];
	rrdamp =  -250.0  * scale[5];
	ftirez = -881.25/fspring + 0.5;
	rtirez = -881.25/rspring + 0.5;

	for(i=0;i<MAX_LINKS;i++)
		{
		m = &model[i];
		m->springrate[0] = m->springrate[1] = fspring;
		m->springrate[2] = m->springrate[3] = rspring;
		m->cdamping[0] = m->cdamping[1] = fcdamp;
		m->cdamping[2] = m->cdamping[3] = rcdamp;
		m->rdamping[0] = m->rdamping[1] = frdamp;
		m->rdamping[2] = m->rdamping[3] = rrdamp;
		m->TIRER[0][2] = m->TIRER[1][2] = ftirez;
		m->TIRER[1][2] = m->TIRER[2][2] = rtirez;
		}
}
#endif

F32 Xscale[6];

#if 1
DBG_tweak_sus(S32 madd)
{
	MODELDAT *m = (MODELDAT *)madd;
	S32 i;
	F32 fspring,rspring,fcdamp,rcdamp,frdamp,rrdamp,ftirez,rtirez;

	for(i=0;i<6;i++)
		if((Xscale[i] < 0.1) || (Xscale[i] > 9.9))
			Xscale[i] = 2.2;

#if 1
	fspring = -1500.0 * Xscale[0];
	rspring = -1750.0 * Xscale[1];
	fcdamp =  -400.0  * Xscale[2];
	rcdamp =  -500.0  * Xscale[3];
	frdamp =  -200.0  * Xscale[4];
	rrdamp =  -250.0  * Xscale[5];
#else
	fspring = -4500.0 * Xscale[0];
	rspring = -4500.0 * Xscale[1];
	fcdamp =  -200.0  * Xscale[2];
	rcdamp =  -200.0  * Xscale[3];
	frdamp =  -600.0  * Xscale[4];
	rrdamp =  -600.0  * Xscale[5];
#endif
	ftirez = -881.25/fspring + 0.5;
	rtirez = -881.25/rspring + 0.5;

	for(i=0;i<MAX_LINKS;i++)
		{
		m = &model[i];
		m->springrate[0] = m->springrate[1] = fspring;
		m->springrate[2] = m->springrate[3] = rspring;
#if 0
		m->farspringrate = fspring;
		m->rarspringrate = rspring;
#endif
		m->cdamping[0] = m->cdamping[1] = fcdamp;
		m->cdamping[2] = m->cdamping[3] = rcdamp;
		m->rdamping[0] = m->rdamping[1] = frdamp;
		m->rdamping[2] = m->rdamping[3] = rrdamp;
		m->TIRER[0][2] = m->TIRER[1][2] = ftirez;
		m->TIRER[2][2] = m->TIRER[3][2] = rtirez;
		}
}
#endif

void DBG_print_fps(S32 erase)
{
	S32 fps;
	S32 col = 1;
	S32 row = 46;
	extern F32 gGameExecTime;


	DBG_printf(col,row,GRN_PAL,"  ");

	if(!erase)
		{
		fps = (gGameExecTime < 0.0101) ? 99 : 1.0/gGameExecTime;
		if(fps >= 30)
			DBG_printf(col,row,WHT_PALB,"%ld",fps);
		else
			DBG_printf(col,row,YEL_PALB,"%ld",fps);
		}
}

#define DBG_MAX_FRAME_BLITS 8
#define DBG_MAX_BAR_BLITS 8

S32 DBG_num_frame_blits;
F32 DBG_bar_scale;
Blit *DBG_frame_blit[DBG_MAX_FRAME_BLITS];
Blit *DBG_bar_blit[DBG_MAX_BAR_BLITS];

DBG_PERF DBG_perf;

F32 DBG_mbscale;

void DBG_init_perf(void)
{
	/*always make 0 display first.*/
	DBG_AG_debug_num = 0;
	DBG_offcnt8 = 100;

	DBG_timebar_blits_setup = 0;
}

void DBG_disp_perf(S32 erase)
{

/*	DBG_printf(col,row,YEL_PALB,"PerfNum:%ld",DBG_AG_debug_num);*/

	/*always display timebar (and #s if DBG_AG_debug_num == 1)*/
	DBG_disp_timebar(erase);	/*generates DBG_mbscale used by DBG_mbox_times*/

	switch(DBG_AG_debug_num)
		{
		case 0: /*timebar only*/		break;
		case 1: /*timebar and #s */		break;
		case 2:	DBG_mbox_numbers(erase);	break;
		case 3:	DBG_mbox_times(erase);		break;
		}


}

void DBG_mbox_numbers(S32 erase)
{
	S32 total_polys;
	S32 row = 36;
	S32 col = 1;

	DBG_txt_clr(row,9);

	if(erase)
		return;

	DBG_printf(col,row++,WHT_PALB,"NumObjects:%ld",mb_global.NumObjects);
	DBG_printf(col,row++,YEL_PALB,"NumVisObjs:%ld",mb_global.NumVisObjs);
	DBG_printf(col,row++,WHT_PALB,"SimpleObjs:%ld",mb_global.NumSimpleObjs);
/*	DBG_printf(col,row++,WHT_PALB,"SortObjs  :%ld",mb_global.NumSortObjs);*/
	row++;
	total_polys = mb_global.NumPolys + mb_global.NumSimpleClipPolys + mb_global.NumClipPolys;
	DBG_printf(col,row++,YEL_PALB,"TotalNumPolys:%ld",total_polys);
	DBG_printf(col,row++,WHT_PALB,"SimplePolys  :%ld",mb_global.NumPolys);
	DBG_printf(col,row++,WHT_PALB,"ClipPolysIN  :%ld",mb_global.NumSimpleClipPolys);
	DBG_printf(col,row++,WHT_PALB,"ClipPolysCLIP:%ld",mb_global.NumClipPolys);
}

void DBG_mbox_times(S32 erase)
{
	F32 total_time;
	F32 xfrm_time,clip_time,draw_time,text_time,xtra_time;
	F32 xfrm_pcnt,clip_pcnt,draw_pcnt,text_pcnt,xtra_pcnt;
	S32 row = 40;
	S32 col = 1;

	DBG_txt_clr(row,5);

	if(erase)
		return;


	total_time = DBG_LimitFtime(mb_global.MBTime * DBG_mbscale * gMsecPerCount, 0.1, 99.9);	

	xfrm_time = DBG_LimitFtime(mb_global.VertXformTime * DBG_mbscale * gMsecPerCount, 0.0, 99.9);
	clip_time = DBG_LimitFtime(mb_global.ClipTime      * DBG_mbscale * gMsecPerCount, 0.0, 99.9);
	draw_time = DBG_LimitFtime(mb_global.LynchDrawTime * DBG_mbscale * gMsecPerCount, 0.0, 99.9);
	text_time = DBG_LimitFtime(mb_global.TextTime      * DBG_mbscale * gMsecPerCount, 0.0, 99.9);
	xtra_time = total_time - xfrm_time - clip_time - draw_time - text_time;

	xfrm_pcnt = xfrm_time / total_time;
	clip_pcnt = clip_time / total_time;
	draw_pcnt = draw_time / total_time;
	text_pcnt = text_time / total_time;
	xtra_pcnt = xtra_time / total_time;

#if 0
/*				       123456789112345678921234567893123456789*/
	DBG_printf(col,row++,WHT_PALB,"     XFRM    CLIP    DRAW    TEXT    OVHD");
	DBG_printf(col,row++,WHT_PALB,"ms   99.9    99.9    DRAW    TEXT    OVHD");
	DBG_printf(col,row++,WHT_PALB,"pcnt (99.9%) (99.9%) TEXT    OVHD");
#endif


	DBG_printf(col,row++,WHT_PALB,"TotalMBTime:%0.1f ",total_time);
	row++;

	DBG_printf(col+ 5,row,WHT_PALB,"XFRM");
	DBG_printf(col+13,row,WHT_PALB,"CLIP");
	DBG_printf(col+21,row,WHT_PALB,"DRAW");
	DBG_printf(col+29,row,WHT_PALB,"TEXT");
	DBG_printf(col+37,row,WHT_PALB,"OVHD");
	row++;

	DBG_printf(col,row,WHT_PALB,"ms");
	DBG_printf(col+ 5,row,WHT_PALB,"%0.1f ",xfrm_time);
	DBG_printf(col+13,row,WHT_PALB,"%0.1f ",clip_time);
	DBG_printf(col+21,row,WHT_PALB,"%0.1f ",draw_time);
	DBG_printf(col+29,row,WHT_PALB,"%0.1f ",text_time);
	DBG_printf(col+37,row,WHT_PALB,"%0.1f ",xtra_time);
	row++;

	DBG_printf(col,row,WHT_PALB,"pcnt");
	DBG_printf(col+ 5,row,WHT_PALB,"(%0.1f%%) ",xfrm_pcnt * 100.0);
	DBG_printf(col+13,row,WHT_PALB,"(%0.1f%%) ",clip_pcnt * 100.0);
	DBG_printf(col+21,row,WHT_PALB,"(%0.1f%%) ",draw_pcnt * 100.0);
	DBG_printf(col+29,row,WHT_PALB,"(%0.1f%%) ",text_pcnt * 100.0);
	DBG_printf(col+37,row,WHT_PALB,"(%0.1f%%) ",xtra_pcnt * 100.0);
}

/* called once at the beginning of each frame to reset times.*/
void DBG_perf_frame_init(void)
{
	/* Model time added to DBG_perf.ModelTime[PERF_STATE_SPIN] */
	DBG_perf.State = PERF_STATE_SPIN;

	DBG_perf.GameTime = 0.0;
	DBG_perf.NetTime = 0.0;
	DBG_perf.IntTime = 0.0;
	DBG_perf.ModelTime[PERF_STATE_SPIN] = 0.0;
	DBG_perf.ModelTime[PERF_STATE_GAME] = 0.0;
	DBG_perf.ModelTime[PERF_STATE_MBOX] = 0.0;
}

void DBG_init_timebar_blits(void)
{
	S32 i;
	Blit *blit;

	for(i=0;i<DBG_MAX_FRAME_BLITS;i++)
		{
		blit = NewBlit("TIMEBAR",   6, 360);
		blit->Hide = 1;
		UpdateBlit(blit);
		DBG_frame_blit[i] = blit;
		}
	DBG_num_frame_blits = 0;

	for(i=0;i<DBG_MAX_BAR_BLITS;i++)
		{
		blit = NewBlit("COLORTEX",   6, 364);
		blit->Hide = 1;
		blit->Height = 8;
		UpdateBlit(blit);
		DBG_bar_blit[i] = blit;
		DBG_set_bar_color(i, i);
		}

	DBG_timebar_blits_setup = 1;
}

void DBG_hide_timebar(void)
{
	S32 i;

	DBG_txt_clr(46,1);

	if(!DBG_timebar_blits_setup)
		return;

	for(i=0;i<DBG_MAX_FRAME_BLITS;i++)
		{
		DBG_frame_blit[i]->Hide = 1;
		UpdateBlit(DBG_frame_blit[i]);
		}

	for(i=0;i<DBG_MAX_BAR_BLITS;i++)
		{
		DBG_bar_blit[i]->Hide = 1;
		UpdateBlit(DBG_bar_blit[i]);
		}

	DBG_num_frame_blits = 0;
}

#define DBG_BAR_LTCYN  0
#define DBG_BAR_LTMAG  1
#define DBG_BAR_LTYEL  2
#define DBG_BAR_WHT    3
#define DBG_BAR_LTRED  4
#define DBG_BAR_LTGRN  5
#define DBG_BAR_LTBLU  6
#define DBG_BAR_LTGRY  7
#define DBG_BAR_CYN    8
#define DBG_BAR_MAG    9
#define DBG_BAR_YEL   10
#define DBG_BAR_DKGRY 11
#define DBG_BAR_RED   12
#define DBG_BAR_GRN   13
#define DBG_BAR_BLU   14
#define DBG_BAR_BLK   15

void DBG_set_bar_color(S32 bliti, S32 colnum)
{
	Blit *blit = DBG_bar_blit[bliti];

	blit->Left = 1 + ((colnum & 0x3) * 4);
	blit->Right = blit->Left + 1;
	blit->Bot = 1 + (((colnum >> 2) & 0x3) * 4);
	blit->Top = blit->Bot + 1;

#if 0
	/*fix later*/
	blit->Top = 14;
	blit->Bot = 15;
	blit->Left = 1 + ((colnum & 0x3) * 4);
	blit->Right = blit->Left + 1;
#endif
}

void DBG_update_frame_blits(F32 totaltime)
{
	S32 i,fps;
	Blit *blit;
	F32 frame_width,fnum_frame_blits;
	S32 col,row=46;

	DBG_num_frame_blits = (S32)(totaltime / 16.667) + 1;
	if(DBG_num_frame_blits < 2)
		DBG_num_frame_blits = 2;
	else if(DBG_num_frame_blits > DBG_MAX_FRAME_BLITS)
		DBG_num_frame_blits = DBG_MAX_FRAME_BLITS;
	fnum_frame_blits = DBG_num_frame_blits;

	frame_width = 500.0 / fnum_frame_blits;
	DBG_bar_scale = 500.0 / (fnum_frame_blits * 16.667);

	for(i=0;i<DBG_MAX_FRAME_BLITS;i++)
		{
		blit = DBG_frame_blit[i];
		if(i < DBG_num_frame_blits)
			{
			blit->Hide = 0;
			blit->X = 6 + (i * frame_width);
			blit->Width = frame_width;
			}
		else
			blit->Hide = 1;
		UpdateBlit(blit);
		}

	DBG_txt_clr(row,1);
	for(i=0;i<DBG_num_frame_blits;i++)
		{
		fps = 60.0/(F32)(i + 1);
		col = 6.0 + frame_width + (frame_width * i) - 8.0;
		col = col >> 3;
		DBG_printf(col,row,YEL_PALB,"%ld",fps);
		}
}

void DBG_update_timebar(S32 numbars, F32 *timelist)
{
	S32 i;
	Blit *blit;
	F32 tottime;

	for(tottime=0.0,i=0;i<numbars;i++)
		tottime += timelist[i];

	if(tottime > (DBG_num_frame_blits * 16.667))
		DBG_update_frame_blits(tottime);

	for(i=0;i<numbars;i++)
		{
		blit = DBG_bar_blit[i];
		if(i < numbars)
			{
			blit->Hide = 0;
			blit->X = (i == 0) ? 6 : (DBG_bar_blit[i-1]->X + DBG_bar_blit[i-1]->Width);
			blit->Width = timelist[i] * DBG_bar_scale;
			}
		else
			blit->Hide = 1;
		UpdateBlit(blit);
		}
}

F32 DBG_LimitFtime(F32 time, F32 min, F32 max)
{
	if(time < min)
		return(min);
	else if(time > max)
		return(max);
	else
		return(time);
}


void DBG_disp_timebar(S32 erase)
{
	U32 game_model_count;
	U32 mbox_model_count;
	U32 mbox_count;
	U32 game_count;
	U32 model_count;
	F32 total_time,meas_mbtime;
	F32 mbox_time,model_time,game_time,int_time,sys_time;
	F32 mbox_pcnt,model_pcnt,game_pcnt,int_pcnt;
	F32 mstime[8];
	S32 row = 40;
	S32 col = 1;

	if(erase)
		{
		DBG_txt_clr(row,5);
		DBG_hide_timebar();
		return;
		}


	mbox_model_count = DBG_perf.ModelTime[PERF_STATE_MBOX];
	game_model_count = DBG_perf.ModelTime[PERF_STATE_GAME];

	mbox_count = mb_global.MBTime - mbox_model_count;
	game_count = DBG_perf.FrameTime - mb_global.MBTime - game_model_count;
	model_count = game_model_count + mbox_model_count;

	total_time = DBG_LimitFtime((mbox_count + model_count + game_count) * gMsecPerCount, 1.0, 100.0);

	/*get measureable game times*/
	mbox_time = DBG_LimitFtime(mbox_count * gMsecPerCount, 0.1, 99.9);
	mbox_pcnt = mbox_time / total_time;

	model_time = DBG_LimitFtime(model_count * gMsecPerCount, 0.1, 99.9);
	model_pcnt = model_time / total_time;

	game_time = DBG_LimitFtime(game_count * gMsecPerCount, 0.1, 99.9);
	game_pcnt = game_time / total_time;

	/*get system times*/
	int_time = DBG_LimitFtime(DBG_perf.IntTime * gMsecPerCount, 0.1, 99.9);
	sys_time = int_time;


	/*proportionally remove sys_time from game,mode, and mbox times*/
	mbox_time -= (sys_time * mbox_pcnt);
	model_time -= (sys_time * model_pcnt);
	game_time -= (sys_time * game_pcnt);

	/*recalculate percentages*/
	total_time = mbox_time + model_time + game_time + sys_time;
	mbox_pcnt = mbox_time / total_time;
	model_pcnt = model_time / total_time;
	game_pcnt = game_time / total_time;
	int_pcnt = int_time / total_time;

	/*calc mathbox time scale*/
	meas_mbtime = DBG_LimitFtime(mb_global.MBTime * gMsecPerCount, 0.1, 99.9);
	DBG_mbscale = mbox_time / meas_mbtime;


	if(DBG_AG_debug_num == 1)
		{
		DBG_printf(1 ,40,RED_PALB,"MBOX:%0.1f ",mbox_time);
		DBG_printf(11,40,RED_PALB,"(%0.1f%%) ",mbox_pcnt * 100.0);

		DBG_printf(1 ,41,GRN_PALB,"MODL:%0.1f ",model_time);
		DBG_printf(11,41,GRN_PALB,"(%0.1f%%) ",model_pcnt * 100.0);

		DBG_printf(1 ,42,BLU_PALB,"GAME:%0.1f ",game_time);
		DBG_printf(11,42,BLU_PALB,"(%0.1f%%) ",game_pcnt * 100.0);

		DBG_printf(1 ,43,GRY_PALB,"XCEP:%0.1f ",int_time);
		DBG_printf(11,43,GRY_PALB,"(%0.1f%%) ",int_pcnt * 100.0);
		}

	if(!DBG_timebar_blits_setup && (gstate == PLAYGAME))
		{
		DBG_init_timebar_blits();
		DBG_set_bar_color(0,DBG_BAR_RED);
		DBG_set_bar_color(1,DBG_BAR_GRN);
		DBG_set_bar_color(2,DBG_BAR_BLU);
		DBG_set_bar_color(3,DBG_BAR_BLK);
		}

	if(DBG_timebar_blits_setup)
		{
		mstime[0] = mbox_time;
		mstime[1] = model_time;
		mstime[2] = game_time;
		mstime[3] = int_time;
		DBG_update_timebar(4, mstime);
		}
}

void DBG_AG_print_steering(S32 erase)
{
	extern S16 oldrawpos;
	extern S16 gSWtorque,gSWdamp,gSWfrict;	/*from model*/
	extern S16 tottorque,scaletorque,last_swforce;
	extern U8 gFeedbackLevel;
	extern S16 gFscale;
	S16 col = 12;
	S16 row = 4;

	if(erase) { DBG_txt_clr(row,15); return; }

	DBG_printf(col,row++,YEL_PALB,"STEERING DEBUG");
	DBG_printf(col,row++,RED_PALB,"CLIPPOS:0x%04x   ",oldrawpos);
	DBG_printf(col,row++,RED_PALB,"SWPOS  :0x%04x   ",gSWpos);
	row++;
	DBG_printf(col,row++,RED_PALB,"SWTORQUE :%d   ",gSWtorque);
	DBG_printf(col,row++,RED_PALB,"SWDAMPK  :%d   ",gSWdamp);
	DBG_printf(col,row++,RED_PALB,"SWFRICT  :%d   ",gSWfrict);
	DBG_printf(col,row++,RED_PALB,"FBLEVEL  :%d   ",gFeedbackLevel);
	DBG_printf(col,row++,RED_PALB,"FSCALE   :%d   ",gFscale);
	DBG_printf(col,row++,RED_PALB,"TOTTORQUE:%d   ",tottorque);
	DBG_printf(col,row++,RED_PALB,"SCLFORCE :%d   ",scaletorque);
	DBG_printf(col,row++,RED_PALB,"AMPFORCE :%d   ",last_swforce);
}

void DBG_AG_magic_switch(S32 erase)
{
	if(erase)
		{
		txt_clr_wid(0,4,64);
		DBG_AG_magic = 0;
		return;
		}

	txt_str(8,4,"MAGIC SWITCH MODE",YEL_PALB);
	DBG_AG_magic = 1;
}

void DBG_print_catchup_info(S32 erase)
{
	S16 i,j,k,nin,inlist[MAX_LINKS];
	F32 fdist = 0.0;

	if(erase)
		{
		for(i=0;i<(1+MAX_LINKS);i++)
			txt_clr_wid(0,4+i,64);
		return;
		}

	/*count number in game */
	for(nin=0,i=0;i<MAX_LINKS;i++)
		if(model[i].in_game)
			{
			inlist[nin++] = i;
			if(game_car[i].place == 0)
				fdist = game_car[i].distance;
			}

	/*print em in sorted order*/
	txt_str(8,4,"CATCHUP DEBUG",YEL_PALB);
	for(i=0;i<nin;i++)
		for(j=0;j<nin;j++)
			{
			k = inlist[j];
			if(game_car[k].place == i)
				{
				sprintf(dbgstr,"place:%d catchup:%0.4f lag:%f     ",
					game_car[k].place,
					(F64)model[k].catchup,
					(F64)(fdist - game_car[k].distance));
				txt_str(8,5+k,dbgstr,RED_PALB);
				}
			}
}

void DBG_print_model_info(S32 erase)
{
	MODELDAT *m = &model[gThisNode];
	S16 col = 30;
	S16 row = 16;

	print_tire_info(erase,0,42,10);
	print_tire_info(erase,1,10,10);
	print_tire_info(erase,2,42,30);
	print_tire_info(erase,3,10,30);


	if(erase) { DBG_txt_clr(row,2); return; }


	DBG_printf(col,row++,YEL_PALB,"GEAR:%d",m->gear);
	DBG_printf(col,row++,YEL_PALB,"RPM :%0.1f",m->engangvel*rdpstorpm);
}

void print_tire_info(S32 erase,S32 tirenum,S16 col,S16 row)
{
        MODELDAT *m = &model[gThisNode];
	F32 xf,yf,zf,xpct,ypct,zpct;
	F32 xfa,yfa,zfa,ztot;
	S32 slipflag;
	S32 i;

	for(i=0;i<7;i++)
		txt_clr_wid(col,row+i,22);

	if(erase) return;

	slipflag = (U32)(m->tires[tirenum].slipflag) & 0xFF;

	xf = m->TIREFORCE[tirenum][0];
	yf = m->TIREFORCE[tirenum][1];
	zf = m->TIREFORCE[tirenum][2];
	for(ztot = 0.0,i=0;i<4;i++)
		ztot += m->TIREFORCE[i][2];

	xfa = (xf < 0.0) ? -xf : xf;
	yfa = (yf < 0.0) ? -yf : yf;
	zfa = (zf < 0.0) ? -zf : zf;
	ztot = (ztot < 0.0) ? -ztot : ztot;

	xpct = (zfa != 0) ? (xfa * 100.0)/zfa : 0.0;
	ypct = (zfa != 0) ? (yfa * 100.0)/zfa : 0.0;
	zpct = (ztot != 0.0) ? (zfa * 100.0)/ztot : 0.0;

	DBG_printf(col,   row++,RED_PALB,"TIRE %ld",tirenum);
	DBG_printf(col,   row++,YEL_PAL, "---FORCE---");

	DBG_printf(col,   row,  YEL_PALB,"X: (%0.1f%%)",xpct);
	DBG_printf(col+13,row++,YEL_PALB,"%0.1f",xf);

	DBG_printf(col,   row,  YEL_PALB,"Y: (%0.1f%%)",ypct);
	DBG_printf(col+13,row++,YEL_PALB,"%0.1f",yf);

	DBG_printf(col,   row,  YEL_PALB,"Z: (%0.1f%%)",zpct);
	DBG_printf(col+13,row++,YEL_PALB,"%0.1f",zf);

	DBG_printf(col,   row++,YEL_PAL, "--SLIPFLAG-");
	DBG_printf(col,   row,  YEL_PALB,"%d",slipflag);
}


/************** end AG debug stuff ******************/


#if 0
/********************** profile stuff ****************************/

#if DO_PROFILE

void DBG_init_profile(void)
{
	DBG_PROFILE *pro;
	S32 i;

	extern U32 prc_get_count(void);
	/*This is a hack until get_uptime() works for Phoenix.*/
	/*See DBG_get_ftime() for details.*/

	DBG_lsbtime = (2.0 * 1000.0)/(F32)CPU_SPEED;	/*msec per count*/
	DBG_msbtime = (F32)0x10000 * (F32)0x10000 * DBG_lsbtime;
	DBG_count_roll = 0;
	DBG_last_count = prc_get_count();
	for(pro=&DBG_profile[0],i=0;i<DBG_MAXPROFILES;i++,pro++)
		DBG_setup_profile(i,"Unused.........",1000);
}

void DBG_setup_profile(S32 ndx,char *name,F32 ms4avg)
{
	DBG_PROFILE *pro = &DBG_profile[ndx];

	strncpy(pro->title,name,15);
	pro->Avg = pro->Min = pro->Max = 0.0;
	pro->sum = pro->min = pro->max = 0.0;
	pro->ms4avg = ms4avg;
	pro->starttime.msb = pro->starttime.lsb = 0;
	pro->lasttime.msb = pro->lasttime.lsb = 0;
	pro->cnt = 0;
}

void DBG_start_profile(S32 ndx)
{
	DBG_get_time(&(DBG_profile[ndx].lasttime));
}

void DBG_end_profile(S32 ndx)
{
	DBG_PROFILE *pro;
	TIME64 curtime;
	F32 deltime;

	pro = &DBG_profile[ndx];

	if((pro->lasttime.msb == 0) && (pro->lasttime.lsb == 0))	/*init flag*/
		{
		DBG_get_time(&(pro->starttime));
		pro->lasttime = pro->starttime;
		}
	else
		{
		DBG_get_time(&curtime);
		deltime = DBG_del_time(&curtime, &(pro->lasttime));
		pro->lasttime = curtime;
		pro->sum += deltime;
		if(deltime < pro->min) pro->min = deltime;
		if(deltime > pro->max) pro->max = deltime;
		pro->cnt++;

		deltime = DBG_del_time(&curtime, &(pro->starttime));
		if(deltime >= pro->ms4avg)
			{
			pro->Avg = pro->sum/(F32)pro->cnt;
			pro->Min = pro->min;
			pro->Max = pro->max;
			pro->starttime = curtime;
			DBG_reset_tavg(pro);
			}
		}
}

void DBG_print_profile(S32 erase)
{
	DBG_PROFILE *pro;
	S32 i;

	if(erase)
		{
		for(i=0;i<(1+DBG_MAXPROFILES);i++)
			txt_clr_wid(0,4+i,64);
		return;
		}

	txt_str(8,4,"PROFILE INFO",YEL_PALB);
	for(pro=&DBG_profile[0],i=0;i<DBG_MAXPROFILES;i++,pro++)
		{
		sprintf(dbgstr,"%ld)%s",i,pro->title);
		txt_str(1,5+i,dbgstr,RED_PALB);
		sprintf(dbgstr,"Min:%f Avg:%f Max:%f   ",
				(F64)pro->Min,
				(F64)pro->Avg,
				(F64)pro->Max);
		txt_str(3+16,5+i,dbgstr,RED_PALB);
		}
}

/*This is a hack until get_uptime() works for Phoenix.*/
U32 DBG_get_uptime(U32 *ptr)
{
	U32 count;

	count = prc_get_count();
	if(count < DBG_last_count)
		DBG_count_roll++;
	DBG_last_count = count;
	*ptr = count;
	return(DBG_count_roll);
}

void DBG_get_time(TIME64 *tp)
{
	tp->msb = DBG_get_uptime(&(tp->lsb));
}

F32 DBG_del_time(TIME64 *cur,TIME64 *last)
{
	if(cur->lsb > last->lsb)
		return(((F32)(cur->msb - last->msb))*DBG_msbtime + ((F32)(cur->lsb - last->lsb))*DBG_lsbtime);
	else
		return(((F32)(cur->msb - last->msb))*DBG_msbtime - ((F32)(last->lsb - cur->lsb))*DBG_lsbtime);
}


F32 DBG_get_ftime(void)
{
	U32 count;
	extern U32 prc_get_count(void);

	/*This is a hack until get_uptime() works for Phoenix.*/
	/*prc_get_count() returns the processors internal cycle count.*/
	/*It counts at half the processor input clock rate.*/
	/*At 150 MHz, the count will roll over in about 53 seconds.*/
	/*This hack will work as long as DBG_get_ftime() is called more than */
	/* once per 53 seconds, */

	count = prc_get_count();
	if(count < DBG_last_count)
		DBG_count_roll++;
	DBG_last_count = count;
	return(( (F32)DBG_count_roll * DBG_msbtime ) + ( (F32)count * DBG_lsbtime));
}

void DBG_init_profile_data(DBG_PROFILE *pro)
{
	strncpy(pro->title,"Unused.........",15);
	pro->Avg = pro->Min = pro->Max = 0.0;
	pro->sum = pro->min = pro->max = 0.0;
	pro->cnt = 0;
	pro->ms4avg = 1000.0;	/*default to 1 second*/
	pro->starttime.msb = pro->starttime.lsb = 0;
	pro->lasttime.msb = pro->lasttime.lsb = 0;
}

void DBG_reset_tavg(DBG_PROFILE *pro)
{
	pro->cnt =  0;
	pro->sum =  0.0;
	pro->min =  1000000000.0;
	pro->max = -1000000000.0;
}

#else /* !DO_PROFILE */

/*setup empty routines if !DO_PROFILE*/
void DBG_init_profile(void) {}
void DBG_setup_profile(S32 ndx,char *name,F32 ms4avg) {}
void DBG_start_profile(S32 ndx) {}
void DBG_end_profile(S32 ndx) {}
void DBG_print_profile(S32 erase) {}

#endif 

/******************* end profile stuff *******************************/
#endif


/*////////////////////////////////////////////////////////////////////////// */
void MSG( char *s,... )
{
	va_list 	ap;
	char 		str[80], fmt[80];

	if ((dlevels & SW_DEBUG7) /*|| gErrOverride*/)
		{
		sprintf( fmt, "%4d) %s", (int)gErrnum++, s );

		va_start( ap, s );
		nvfprintf( str, 256, fmt, ap );
		va_end( ap );

		ClearLine( gLine );
		txt_str( 1,gLine,str,gPal ); 
		if (++gLine > 40) 
			{
			gLine = 9; 
			gPal = (gPal == RED_PAL) ? GRN_PAL : RED_PAL;
			}
		}

} /* end MSG */


/*////////////////////////////////////////////////////////////////////////// */
void EMSG( char *s,... )
{
	va_list 	ap;
	char 		str[80], fmt[80];

	if (!(dlevels & SW_DEBUG6))
		return;

	sprintf( fmt, "%4d) %s", (int)gErrnum++, s );

	va_start( ap, s );
	nvfprintf( str, 256, fmt, ap );
	va_end( ap );

	ClearLine( gLine );
	txt_str( 1,gLine,str,YEL_PALB ); 
	if (++gLine > 45) 
		gLine = 9; 

} /* end EMSG */


/*////////////////////////////////////////////////////////////////////////// */
void SEMSG( S32 r, S32 c, S32 pal, char *fmt,... )
{
	va_list 	ap;
	char 		str[80];

	va_start( ap, fmt );
	nvfprintf( str, 256, fmt, ap );
	va_end( ap );

	txt_str( c,r,str,pal );

} /* end SEMSG */


/*////////////////////////////////////////////////////////////////////////// */
void SMSG( S32 r, S32 c, char *fmt,... )
{
	va_list 	ap;
	char 		str[80];

	if (!(dlevels & SW_DEBUG6) && !(trailing_dedges & SW_DEBUG6))
		return;

	va_start( ap, fmt );
	nvfprintf( str, 256, fmt, ap );
	va_end( ap );

	if (dlevels & SW_DEBUG6)
		txt_str( c,r,str,YEL_PAL );
	else if (trailing_dedges & SW_DEBUG6)
		txt_clr_str( c,r,str,0 );

} /* end SMSG */


/*////////////////////////////////////////////////////////////////////////// */
void CMSG( S32 r, S32 c, S32 pal, char *fmt,... )
{
	va_list 	ap;
	char 		str[80];

	if (!(dlevels & SW_DEBUG6) && !(trailing_dedges & SW_DEBUG6))
		return;

	va_start( ap, fmt );
	nvfprintf( str, 256, fmt, ap );
	va_end( ap );

	if (dlevels & SW_DEBUG6)
		txt_str( c,r,str,pal );
	else if (trailing_dedges & SW_DEBUG6)
		txt_clr_str( c,r,str,0 );

} /* end CMSG */


/*////////////////////////////////////////////////////////////////////////// */
void ClearText( void )
{
	S32 i;

	for (i=0; i<47; ++i)
		ClearLine(i);
	gLine = 9;
	gErrnum = 1;

}	/* End ClearText */


/*////////////////////////////////////////////////////////////////////////// */
void ClearLine( S32 i )
{
	txt_clr_str( 0,i,"                                                               ",0);
}	/* End ClearLine */


/*//////////////////////////////////////////////////////////////////////////////
//	Returns the name assicoiated with this stack (debug only).
////////////////////////////////////////////////////////////////////////////// */
char *NodeName( U8 node )
{
	switch (node)
		{
		case 0:	return "RED";
		case 1:	return "GREEN";
		case 2:	return "BLUE";
		case 3:	return "PURPLE";
		case 4:	return "YELLOW";
		case 5:	return "TEAL";
		case 6:	return "BLACK";
		case 7:	return "ORANGE";
		case 8:	return "WHITE";
		}
	return "????";

}	/* end NodeName */


/*//////////////////////////////////////////////////////////////////////////////
//	Returns the name assicoiated with this stack (debug only).
////////////////////////////////////////////////////////////////////////////// */
char *GameState( S16 state )
{
	switch (state)
		{
		case ATTRACT:		return "ATTRACT";
		case TRKSEL:		return "TRKSEL";
		case CARSEL:		return "CARSEL";
		case PLAYGAME:		return "PLAYGAME";
		case ENDGAME:		return "ENDGAME";
		case GAMEOVER:		return "GAMEOVER";
		case HISCORE:		return "HISCORE";
		case PREPLAY:		return "PREPLAY";
		case PREPLAY2:		return "PREPLAY2";
		case COUNTDOWN:		return "COUNTDOWN";
		}
	return "????";

}	/* end GameState */


extern F32 ZCLOCK;
extern S16 brake;

F32 gFlyTime,gFlySpeed,gFlyYaw;
F32 gFlyUvs[3][3],gFlyPos[3];

void flyaround_init(void)
{
	S32 i,j;
	MODELDAT  *m = &model[gThisNode];

	txt_str(28,4,"FLY MODE",YEL_PALB);

	gFlyMode = 1;
	gFlyTime = ZCLOCK;
	gFlySpeed = 0.0;

	for(i=0;i<3;i++)
		for(j=0;j<3;j++)
			gFlyUvs[i][j] = game_car[gThisNode].dr_uvs[i][j];

	veccopy(game_car[gThisNode].dr_pos, gFlyPos);

	model[gThisNode].resurrect.moving_state = 0;
}

void flyaround_cleanup(void)
{
	MODELDAT  *m = &model[gThisNode];
	CAR_DATA *gc = &game_car[gThisNode];

	txt_str(28,4,"        ",YEL_PAL);

	MOVMEM((char *)(m->UV.fpuvs), (char *)(m->resurrect.uvs), sizeof(F32)*3*3);

	m->resurrect.pos[0] = gc->dr_pos[2];
	m->resurrect.pos[1] = gc->dr_pos[0];
	m->resurrect.pos[2] = -gc->dr_pos[1];

	model[gThisNode].resurrect.moving_state = -2;

	gFlyMode = 0;
}


void flyaround(S16 mode)
{
	MODELDAT  *m = &model[gThisNode];
	F32 curflytime,delflytime;
	S32 i,j;

	if(mode == Initialize)
		{
		flyaround_init();
		return;
		}
	else if(mode == Cleanup)
		{
		flyaround_cleanup();
		return;
		}
	/* else Doit */

	curflytime = ZCLOCK;
	delflytime = curflytime - gFlyTime;	/*seconds*/
	gFlyTime = curflytime;

	/*Get iteration time to nearest msec*/
	i = (S32)((delflytime*1000.0) + 0.5);
	delflytime = (F32)i * 0.001;

	/*update uvs*/
	if((gSWpos < -0x4) || (gSWpos > 0x4))
		gFlyYaw = 6.2830 * delflytime * (F32)gSWpos * 0.000244;
	else
		gFlyYaw = 0.0;
	WYawUV(gFlyYaw, gFlyUvs);

	/*update speed pos*/
	if(gGasVal > 0.01)
		gFlySpeed = 400.0 * delflytime * gGasVal;
	else if(brake > 0x8)
		gFlySpeed = -400.0 * delflytime * (F32)brake * 0.000244;
	else gFlySpeed = 0.0;
	for(i=0;i<3;i++)
		gFlyPos[i] += (gFlySpeed * gFlyUvs[i][2]);

	/*update altitude pos*/
	if (m_levels & SW_FIRST)
		gFlyPos[1] += 1.0;
	else if (m_levels & SW_THIRD)
		gFlyPos[1] += 0.1;
	else if (m_levels & SW_SECOND)
		gFlyPos[1] -= 1.0;
	else if (m_levels & SW_FOURTH)
		gFlyPos[1] -= 0.1;

	/* fix */
	matcopyD2M(gFlyUvs, m->UV.fpuvs);
	makesuvs(&m->UV);

	m->RWR[YCOMP] =  gFlyPos[0];
	m->RWR[ZCOMP] = -gFlyPos[1];
	m->RWR[XCOMP] =  gFlyPos[2];
}


