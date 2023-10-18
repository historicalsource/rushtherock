/*
 *	init.c	-- initialization routines
 *
 *	Copyright 1996 Time Warner Interactive.
 *	Unauthorized reproduction, adaptation, distribution, performance or 
 *	display of this computer program or the associated audiovisual work
 *	is strictly prohibited.
 *
 *	======================================================================
 *	$Author: gray $						$Date: 1997/10/09 19:13:15 $
 *	$Revision: 3.18 $						$Locker:  $
 *	======================================================================
 *	Change Log:
 *
 *	$Log: init.c,v $
 * Revision 3.18  1997/10/09  19:13:15  gray
 * New way to allocate GUTS memory pool.
 *
 * Revision 3.17  1997/10/02  00:54:17  gray
 * Added seconds-since-poweron variable (gUpSeconds).
 *
 * Revision 3.16  1997/06/23  22:29:14  gray
 * made GameExecTime use IRQTIME not ZCLOCK.
 *
 * Revision 3.15  1997/04/07  22:03:31  gray
 * Start of init restructure.
 *
 * Revision 3.14  1997/03/31  19:52:56  gray
 * app_init removed - initialization stuff moved here.
 *
 * Revision 3.13  1997/03/31  02:48:16  gray
 * Changing game entry point since nucleus gone.
 *
 * Revision 3.12  1996/12/05  10:20:21  geraci
 * house cleaning
 *
 * Revision 3.11  1996/11/23  01:59:16  gray
 * Reduced GUTS/Nucleus stack to 16K.
 *
 * Revision 3.10  1996/11/15  09:52:08  petrick
 * Changes for the new driver.
 *
 * Revision 3.9  1996/11/07  08:18:24  gray
 * Car shading and reflection stuff.
 *
 * Revision 3.8  1996/10/10  05:23:28  petrick
 * Changed around order of some init stuff so disk callback would work.
 *
 * Revision 3.7  1996/09/27  03:26:51  gray
 * Made camera control local. Eliminated CamFunc.
 *
 * Revision 3.6  1996/09/25  02:02:21  petrick
 * Got rid of include file.
 *
 * Revision 3.5  1996/09/25  01:25:59  gray
 * romaddr not used now.
 *
 * Revision 3.4  1996/09/16  03:46:38  petrick
 * Changed back to 90 degree view angle.
 *
 * Revision 3.3  1996/09/15  22:18:53  gray
 * Made background screen clear color gray (not black).
 *
 * Revision 3.2  1996/09/14  09:48:58  petrick
 * Made view angle defaulted to 75 degrees.
 *
 * Revision 3.1  1996/09/09  02:24:52  gray
 * JAMMA version
 *
 * Revision 2.16  1996/08/28  05:45:29  gray
 * Fixed game stat warning.
 *
 * Revision 2.15  1996/08/28  05:41:07  gray
 * init game stats.
 *
 * Revision 2.14  1996/08/18  21:25:20  gray
 * Removing zoid stuff. No more zoid.h.
 *
 * Revision 2.13  1996/08/18  20:53:09  gray
 * Made Nucleus default, removed flag.
 *
 * Revision 2.12  1996/08/18  03:30:29  gray
 * Removing ZOID stuff.
 *
 * Revision 2.11  1996/07/21  23:20:21  gray
 * Cleaned up initialization sequence.
 *
 * Revision 2.10  1996/07/11  01:48:05  gray
 * Set vid_frames to 2. Call MB for 3Dfx init.
 *
 * Revision 2.9  1996/06/28  05:10:08  geraci
 * init hint table here instead of game
 *
 * Revision 2.8  1996/06/21  03:10:18  gray
 * Cleaned up alternate world loading.
 *
 * Revision 2.7  1996/06/20  22:38:53  gray
 * Made hard drive loading the default.
 *
 * Revision 2.6  1996/06/15  00:28:08  gray
 * Set up ambient level and view angle for phoenix.
 *
 * Revision 2.5  1996/06/03  20:57:35  gray
 * Phoenix port.
 *
 * Revision 2.4  1996/05/29  22:22:15  geraci
 * calls init moobs routine to initialized stuff for sounds
 *
 * Revision 2.3  1996/05/28  21:45:02  geraci
 * added last track loaded index to indicate current track in memory
 *
 * Revision 2.2  1996/05/25  02:11:29  gray
 * moved download stuff to dnl.c
 *
 * Revision 2.1  1996/05/04  02:56:18  petrick
 * Focus 2 release
 *
 * Revision 1.42  1996/03/28  12:34:30  gray
 * Added DBG_init_debug(). Disabled alternate view angles (for now).
 *
 * Revision 1.41  1996/03/27  05:40:57  petrick
 * Changed attract .pro to .h
 *
 * Revision 1.40  1996/03/25  19:24:28  petrick
 * hanged loading to loaded.
 *
 * Revision 1.39  1996/03/14  18:59:14  gray
 * Made alternate view angle more hidden.
 *
 * Revision 1.38  1996/03/14  17:52:48  gray
 * moved download stuff from zglue.c to here.
 *
 * Revision 1.37  1996/03/08  19:51:38  gray
 * Got alpha palettes limping along (need GUTS work).
 *
 * Revision 1.36  1996/03/08  10:17:09  gray
 * Stuff white on transparent for stamps till Dave fixes.
 *
 * Revision 1.35  1996/03/08  08:39:12  gray
 * Attempt to get new alpha palettes working. Still fried.
 *
 * Revision 1.34  1996/03/01  18:30:30  gray
 * Made variable view angle on SW_DEBUG9 and SW_DEBUG10.
 *
 * Revision 1.33  1996/02/28  21:36:34  geraci
 * added compile flag to menu initialization call
 *
 * Revision 1.32  1996/02/27  14:00:04  gray
 * Include cleanup, prune crap.
 *
 * Revision 1.31  1996/02/24  21:51:37  gray
 * New mod to get stamps working again.
 *
 * Revision 1.30  1996/02/23  05:14:14  gray
 * got texture palette stuff working and faked _PALB palette.
 *
 * Revision 1.29  1996/02/22  01:04:51  gray
 * Cleaned init(). Added reinit_zoid to set viewang and ambient. 45 degree view
 * angle on SW_DEBUG9. Move OS palette. Prune old junk.
 *
 * Revision 1.28  1996/02/14  02:16:22  geraci
 * moved init stuff from game.c to here
 *
 * Revision 1.27  1996/02/07  02:45:18  geraci
 * put defines from gstate.h into game.h; made PLAYGAME state
 *
 *
 *	======================================================================
 */

#include "net_link.h"
#include "blit.h"
#include "globals.h"
#include "game.h"
#include "os_proto.h"
#include "phx_proto.h"
#include "intvecs.h"
#include "maxpath.h"
#include "mb.h"
#include "world.h"
#include "modeldat.h"
#include "mdrive.h"
#include "drivsym.h"
#include "checkpoint.h"
#include "menus.h"
#include "stats.h"
#include "debug.h"
#include "attract.h"

#include "Pro/timer.pro"
#include "Pro/init.pro"
#include "Pro/fswheel.pro"
#include "Pro/game.pro"
#include "Pro/globals.pro"
#include "Pro/pots.pro"
#include "Pro/world.pro"
#include "Pro/camera.pro"
#include "Pro/timint.pro"
#include "Pro/initiali.pro"
#include "Pro/select.pro"
#include "Pro/strings.pro"
#include "Pro/checkpoint.pro"
#include "Pro/menus.pro"
#include "Pro/moobs.pro"
#include "Pro/sounds.pro"
#include "Pro/dnl.pro"

#include "glide.h"
#include "mem.h"
#include "hdrive.h"
#include "mole.h"
#include "timer.h"

#include "Pro/error.pro"


#define NET_MEM_BYTES (30*1024)
#define GUTS_STACK_BYTES (32*1024)


/* Define prototypes for function references.  */
void game_loop(void);
static void model_ast(void);
static void model_act_start(S32 time, void *task_param);
static void frame_done(void);


F32	gCurGameTime,gLastGameTime,gGameLoopTime,gGameExecTime;
S32	game_block;
U32	mem_pool_start;
U32	net_mem_start;
VU32 	frames;
VU32 	gotvb;


extern S32 HDStart(void); 
extern F32 ZCLOCK;



/* ----------------------------------------------------------------------------- */
/* Game entry point (reset vector) */
void start(void)
{
	extern U16		modelrun;

	vid_clear();	/* clear any GUTS text. */
	vid_fields(2);	/*min 30 fps*/

	net_mem_start = (U32)prc_extend_bss(0);
	mem_pool_start = net_mem_start + NET_MEM_BYTES;

	/***************** was in app_init. **********************/
	InitSW();/*!!!Gotta do this first!!! Will figure out why later. _AG_ */
	Init_TimInt();

	frames = 0;
	gotvb = 0;

	Net_Initialize( (void *)net_mem_start, NET_MEM_BYTES, (void(*))0, (void *)0 );

	/* This call may bend any time-driven interrupt vector. */
	prc_set_vec(TIMER_INTVEC, TimInt);

	prc_set_vec(GAMEFRAME_INTVEC, frame_done);

	/* Wait 50 msec for 1st iteration of model task. */
	model_act_start( 50000, (void *)0 );
	/***************** was in app_init. **********************/

	/******************** was model task init. ***************/
	init();
	modelrun = 0;
	init_model_task();
	/******************** was model task init. ***************/


	game_init();
	game_loop();
}

#define NOZCLK 1

/* ----------------------------------------------------------------------------- */
void    game_loop(void)
{
#if NOZCLK
	gLastGameTime = (F32)IRQTIME * 0.001;
#else
	gLastGameTime = ZCLOCK;
#endif
	gGameLoopTime = gGameExecTime = 0.0;

	while(1)
		{
		/*pet dog*/
		WDOG = 1;

#if NOZCLK
		gCurGameTime = (F32)IRQTIME * 0.001;
#else
		gCurGameTime = ZCLOCK;
#endif
		gGameLoopTime = gCurGameTime - gLastGameTime;
		gLastGameTime = gCurGameTime;
		game();
#if NOZCLK
		gGameExecTime = ((F32)IRQTIME * 0.001) - gLastGameTime;
#else
		gGameExecTime = ZCLOCK - gLastGameTime;
#endif

		while(!gotvb);
		gotvb = 0;
		frames++;
		}
}


/* ----------------------------------------------------------------------------- */
/* frame swap interrupt. */
static void frame_done(void)
{
	gotvb = 1;
}


/* ----------------------------------------------------------------------------- */
/*Powerup Initialization*/
void init(void)
{

        InitMemHandler2(mem_pool_start, GUTS_STACK_BYTES);
	InitializeTimerIRQ();
	InitError();
	InitStrings();
	HDStart();
	ZOID_Reset();
	MBOX_SetBGColor(0 /*bogus wndx*/, 0.5 /*red*/, 0.5 /*blue*/, 0.5 /*green*/);

	/* Initialize Steering Wheel Stuff */
	swforce = 0; /* !! do this before interrupts turned on !! */
	InitSW();
	freewheel();
	frictwheel();

	PotsInit();

	/* Initialize global vars */
	init_glob();

	/* Load initial stuff from HD. */
	loadInit();

	/*reset zoid with rush setup*/
	reinit_zoid();

	/* Initialize model vars (once at powerup) */
	modelinit();	/* do model powerup init */

	InitTrkSel();
	ZeroCamera();

	/* Set up the blit structures */
	InitBlits();
	InitAttract();

#if CP_EDITOR
	/* initialize menu stuff */
	check_menu (Initialize, NULL);
#endif

	InitMoobs();

	/* initialize sound handles */
	init_drone_handles();
	init_car_sound_handles();

	DBG_init_debug();

	gstate = ATTRACT;

	last_track_loaded = -1;

	init_hint_table();
	init_game_stats();
}


/*Reset Zoid, setup Rush Viewing Angle and Ambient Level.*/
void reinit_zoid(void)
{
	WINDOW *win;
	F32 ambientlvl;
	F32 lightdir[3];

	win = MBOX_SetWindow(90.0,0);
	win->ambientlevel = 0.0;
	win->ambientrgb[0] = 0.0;
	win->ambientrgb[1] = 0.0;
	win->ambientrgb[2] = 0.0;

	ambientlvl = 0.3;
	ZOID_SetAmbient(0,ambientlvl,0);
#if 1
	lightdir[0] = 0.577;
	lightdir[1] = -0.577;
	lightdir[2] = 0.577;
	ZOID_SetLightDir(lightdir,1);
#else
	lightdir[0] = -0.707;
	lightdir[1] = -0.707;
	lightdir[2] = 0.0;
	ZOID_SetLightDir(lightdir,0);
#endif
}



#define MODEL_AST_LEVEL (MAX_AST_QUEUES-1)
static struct tq    timer_act;
static struct act_q timer_ast;

U32 mast_period,last_mast_time;

/* Model action routine. */
static void model_act(void *param)
{

	timer_ast.param = param;
	prc_q_ast(MODEL_AST_LEVEL, &timer_ast);
}

/* Model task action queue kickoff. */
static void model_act_start(S32 time, void *task_param)
{
	timer_ast.action = model_ast;

	timer_act.func = model_act;
	timer_act.vars = task_param;
	timer_act.delta = time;
	tq_ins(&timer_act);
}

static void model_ast(void)
{
	U32 delta_time;
	S32 act_delta;
	F32 gUsecPerCount;

	/* if model data structure locked by game task - reschedule in 1/2 msec. */
	if(gModeldatLock)
		{
		game_block++;
		timer_act.delta = 500;
		tq_ins(&timer_act);
		return;
		}

	gUsecPerCount    = (2.0 * 1000000.0)/(F32)CPU_SPEED;

#if DO_PERF
	delta_time = prc_get_count();
#endif
	mast_period = (last_mast_time) ? delta_time - last_mast_time : 0;
	last_mast_time = delta_time;


	model_iteration();



#if DO_PERF
	DBG_perf.ModelTime[DBG_perf.State] += prc_get_count() - delta_time;
#endif

	/* requeue for 5ms from entry time. */
	delta_time = prc_get_count() - delta_time;
	act_delta = 5000 - (delta_time * gUsecPerCount);
	if(act_delta < 500)
		act_delta = 500;
	timer_act.delta = act_delta;
	tq_ins(&timer_act);


}


