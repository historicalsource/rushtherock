#ifndef _TIMER_H
#define _TIMER_H

#include "stdtypes.h"
#include "Pro/timer.pro"

#ifdef HOCKEY
#define DO_TIMERS  1
#else
#define DO_TIMERS  0	/* define to 1 to include R4K MBOX timer instrumentation */
#endif

#if DO_TIMERS
#  define START_TIMER(x) start_timer_func(x)
#  define END_TIMER(x)   end_timer_func(x)
#else
#  define START_TIMER(x)
#  define END_TIMER(x)
#endif

typedef struct {
	unsigned int frame;		/* sum measured time for a frame */
	unsigned int total;		/* total measured time */
	unsigned int count;		/* number of times through this function */
	unsigned int current;	/* measured time most recent instance took */
} TimerT;

enum timers_enum {
	timer_total,
	
	timer_mb,
	 timer_drawobj,
	  timer_dopolys,
	   timer_dosimple,
	   timer_trapify,
	    timer_edges,
	  timer_verts,
	  timer_norms,
	 timer_drawblits,
	 timer_drawpolys,
	
	timer_vblank,

	timer_host,
#if HOCKEY
	 timer_drones,
	 timer_goalies,
	 timer_players,
	  timer_kidcontrols,
	  timer_dronecontrols,
	  timer_interp,
	  timer_physics,
	   timer_physics1,
	   timer_physics2,
	   timer_physics3,
	   timer_physics4,
	   timer_physics5,	
	 timer_collision,
	  timer_collision1,
	  timer_collision2,
	  timer_collision3,
	 timer_anim,
	  timer_anim1,
	  timer_anim2,
	  timer_anim3,
#endif
	MAX_TIMERS
};

extern VF32 ZCLOCK;		/* Time in seconds, updated in 1ms interrupt */
extern VF32 TIME;			/* Time in seconds, updated once per FRAME */
extern VU32 ITIME;			/* Integer TIME in ms, updated once per FRAME */
extern VF32 SPF;			/* Seconds per frame, updated once per FRAME */

typedef struct
{
	U32		Animation;
	U32		CollPoly;
	U32		CollBody;
	U32		MBox;
	U32		Inner;
	U32		Outer;
} TimerInfo;

extern TimerInfo timer_list;

#endif
