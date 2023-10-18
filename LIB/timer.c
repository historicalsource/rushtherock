/*****************************************************************************************
3D Hockey :

Copyright 1994,1995 Time Warner Interactive.  Unauthorized reproduction, adaptation,
distribution, performance or display of this computer program or the associated
audiovisual work is strictly prohibited.
*****************************************************************************************/

#include "stdtypes.h"
#include <os_proto.h>
#include <config.h>
#include <intvecs.h>
#include "timer.h"
#include "controls.h"
#include "error.h"
#if HOCKEY
#include "main.h"
#else
#define TestMessage(x)
#endif

extern U32 prc_get_count(void);

#if MACE
extern ControlIRQ(void);
#endif

static struct tq timerq;
TimerInfo timer_list;

VF32 ZCLOCK;		/* Time in seconds, updated in 1ms interrupt */
VF32 TIME;			/* Time in seconds, updated once per FRAME */
VU32 ITIME;			/* Integer TIME in ms, updated once per FRAME */
VF32 SPF;			/* Seconds per frame, updated once per FRAME */

static F32 oldtime;
extern F32 ATime;


#if HOST_IS_MATHBOX

#if DO_TIMERS
TimerT timers[MAX_TIMERS];

const struct {
	char name[16];
	int level;
} timer_descs[MAX_TIMERS] = {
	{"TOTAL          ", 0},
	{"MATHBOX        ", 1},
	{"DrawObjects    ", 2},
	{"DoPolys        ", 3},
	{"DoSimplePoly   ", 4},
	{"Trapify        ", 4},
	{"PreloadEdges   ", 5},
	{"Xform Verts    ", 3},
	{"Xform Norms    ", 3},
	{"DrawBlits      ", 2},
	{"DrawPolys      ", 2},
	{"VBLANK         ", 1},
	{"HOST           ", 1},
#if HOCKEY
	{"Drone Teamwork ", 2},
	{"Goalies        ", 2},
	{"Players        ", 2},
	{"Kid Controls   ", 3},
	{"Drone Controls ", 3},
	{"Interp         ", 3},
	{"Physics        ", 3},
	{"Physics1       ", -4},
	{"Physics2       ", -4},
	{"Physics3       ", -4},
	{"Physics4       ", -4},
	{"Physics5       ", -4},
	{"Collision      ", 2},
	{"Collision1     ", 3},
	{"Collision2     ", 3},
	{"Collision3     ", 3},
	{"Anim           ", 2},
	{"Anim1          ", -3},
	{"Anim2          ", -4},
	{"Anim3          ", -5}
#endif
};

void start_timer_func(int x) {

}

void end_timer_func(int x) {

}


void TimerInit()
{
	int i;
	for (i=0; i<MAX_TIMERS; i++)
	{
		timers[i].frame = 0;
		timers[i].total = 0;
		timers[i].count = 0;
		timers[i].current = 0;
	}
}

static void ReduceTimerTotals()
{
	int i;
	
	for (i=0; i<MAX_TIMERS; i++)
	{
		timers[i].total >>= 4;
	}
}
#define MAX_TIMER_LEVEL 8
static const TimerColor[MAX_TIMER_LEVEL] = {
	GRN_PAL, VIO_PAL, GRN_PAL, VIO_PAL, GRN_PAL, VIO_PAL, GRN_PAL, VIO_PAL
};

void PrintTimers()
{
	int i;
	int x, y;
	F32 frac,tot;
	int pcent;
	int reduce = 0;
	int level;
	int parent[MAX_TIMER_LEVEL];

	y = 4;
	for (i=0; i<MAX_TIMERS; i++)
	{
		if (timer_descs[i].level < 0)
			continue;
		
		level = timer_descs[i].level;
		parent[level] = i;

		if (timers[i].count == 0)
			continue;
		
		x = 2;
		txt_str(x,y,"     ",TimerColor[level]);
		txt_str(x+level,y,timer_descs[i].name,TimerColor[level]);
		x += 18;
		txt_decnum(x,y,timers[i].frame,12,RJ_BF,TimerColor[level]);
		txt_decnum(0,-1,timers[i].count,8,RJ_BF,TimerColor[level]);

		if (level > 0) {
			tot = (F32)timers[parent[level-1]].frame;
			frac = (tot != 0) ? (((F32)timers[i].frame) / tot ) : 1.0;
		} else
			frac = 1.0;
		pcent = (S32)(frac * 1000.0);
		txt_decnum(0,-1,pcent,5,RJ_BF,TimerColor[level]);
		
		txt_decnum(0,-1,timers[i].total>>10,12,RJ_BF,TimerColor[level]);
		if (level > 0) {
			tot = (F32)timers[parent[level-1]].total;
			frac = (tot != 0) ? (((F32)timers[i].total) / tot ) : 1.0;
		} else
			frac = 1.0;
		pcent = (S32)(frac * 1000.0);
		txt_decnum(0,-1,pcent,5,RJ_BF,TimerColor[level]);
		
		if (timers[i].total >= (1<<30))
			reduce = 1;
		y++;
	}
	if (reduce)
		ReduceTimerTotals();
}

void ResetTimers()
{
	int i;

	for (i=0; i<MAX_TIMERS; i++)
	{
		timers[i].frame = 0;
		timers[i].count = 0;
	}
}

#else
void TimerInit() {}
void PrintTimers() {}
void ResetTimers() {}
void start_timer_func(int x) {}
void end_timer_func(int x) {}

#endif /* DO_TIMERS */
#endif /* HOST_IS_MATHBOX */


int timerCount;
static U32 last_timer_count;

int AlmostTimerOverflow(void) {
	return (ZCLOCK>(16384.0f/*4.55hrs*/-1800.0f/*.5hr*/));
}

void TimerResetNearOverflow(void) {
	if (AlmostTimerOverflow()) {
		ResetTimer();
#if HOCKEY
		InitControls();
#endif
	}
}

static void WaitOnTimer(void) { /* similar to wait() */
	int x = 20000; /*just like in main */
	volatile int i;
	volatile int q;
	for (i=0; i<x; i++) {
		q = i;
		if (oldtime != ZCLOCK)
			return;
	}
	Error(ERR,"WaitOnTimer: Timed Out!");
}

void TimerCalc(int print) {
#if 1
	if (oldtime == ZCLOCK)
		WaitOnTimer();
#endif
	TIME = ZCLOCK;
	ITIME = (VU32)(TIME * 1000.0);
	SPF = (F32)(TIME - oldtime);
	if(SPF==0) { /* Reset Timer */
		ZCLOCK = 1.0f/1000.0f;
		TIME = ZCLOCK;
		ITIME = (VU32)(TIME * 1000.0);
		SPF = 1.0/300.0;
		last_timer_count = prc_get_count();
		Error(ERR,"TimerCalc: SPF unchanged. Reset Timer.");
	}
	oldtime = TIME;
	ATime = TIME;	/* Animation */

	if (print)
		xyprintf(2,46,"TIME=%.2f SPF=%.4f FPS=%.2f",(double)TIME,(double)SPF,(double)1.0/SPF);
}

void PunchTimer(F32 interval) {
	static F32 r_oldtime;
	F32			r_spf;

	r_spf = ZCLOCK - r_oldtime;
	r_oldtime = ZCLOCK;

	SPF = interval;
	TIME += interval;
	ITIME = (VU32)(TIME * 1000.0);
	ATime = TIME;
	oldtime = TIME;

	ZCLOCK = TIME;	/* Syncronize timer clock with TIME */
	
#if 0
	xyprintf(0,46,"TIME=%.2f  SPF=%.2f  FPS=%2.1f",TIME,SPF,1.0F/*/r_spf*/);
#endif
}


static F32 saveTIME;
static F32 saveZCLOCK;
static F32 saveoldtime;
void SaveTimer(void) {
	saveZCLOCK 	= ZCLOCK;
	saveTIME 	= TIME;
	saveoldtime = oldtime;
}


void RestoreTimer(void) {
 	ZCLOCK 	= saveZCLOCK;
	TIME 	= saveTIME;
	oldtime = saveoldtime;

	ITIME = (VU32)(TIME * 1000.0);
	SPF = (F32)(TIME - oldtime);
	ATime = TIME;	/* Animation */
}


void (*OSTimerFunc)();

void TimerIRQ1ms(void * arg) {
	U32 count;
	F32 dtime;
	
	timerq.delta = 1000/*how_many_usecs_until_you_want_to_do_it_again*/;
	tq_ins(&timerq);

	count = prc_get_count();
	dtime = (count - last_timer_count)*(2.0/CPU_SPEED);
	if (dtime > (2.0/30.0)) /* LONG Interruption... lets assume its the debugger */
		dtime = 0.0;
	last_timer_count = count;
#if 0
	OSTimerFunc();
#endif
	ZCLOCK += dtime;

	timerCount++;
	if (!(timerCount & 0xF))
	{
#if (HOCKEY || MACE)
		ControlIRQ(); /* keep after ZCLOCK update */
#else
		ctl_upd_sw();
#endif
	}

#if 0
	OS_ctl_upd_sw();
#endif
}

void ResetTimer(void) {
	oldtime = 0.0f;
	ZCLOCK = 1.0f/1000.0f;
	TimerCalc(0);
	last_timer_count = prc_get_count();
}

void InitializeTimerIRQ(void) {
	TestMessage("ResetTimer()");
	ResetTimer();
	timerCount = 0;
#if 1
    timerq.func = TimerIRQ1ms;
    timerq.vars = (void *)0/*whatever_parameter_you_want_passed_to_timeout*/;
    timerq.delta = 1000/*how_many_usecs_until_you_want_to_timeout*/;
	TestMessage("tq_ins()");
    tq_ins(&timerq);
#else
	OSTimerFunc = prc_set_vec (TIMER_INTVEC, TimerIRQ1ms );
#endif
	TestMessage("end InitializeTimerIRQ()");
}

