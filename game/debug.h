
#ifndef _DEBUG_H
#define _DEBUG_H

#include <config.h>
#include "blit.h"

#if 0
#define SW_DEBUG0    0x00010000
#define SW_DEBUG1    0x00020000
#define SW_DEBUG2    0x00040000
#define SW_DEBUG3    0x00080000
#define SW_DEBUG4    0x00100000
#define SW_DEBUG5    0x00200000
#define SW_DEBUG6    0x00400000
#define SW_DEBUG7    0x00800000
#define SW_DEBUG8    0x01000000
#define SW_DEBUG9    0x02000000
#define SW_DEBUG10   0x04000000
#define SW_DEBUG11   0x08000000
#define SW_DEBUG12   0x10000000
#define SW_DEBUG13   0x20000000
#define SW_DEBUG14   0x40000000
#define SW_DEBUG_ALL 0x7FFF0000
#else
#define SW_DEBUG0	(SW_OPT0)
#define SW_DEBUG1	(SW_OPT1)
#define SW_DEBUG2	(SW_OPT2)
#define SW_DEBUG3	(SW_OPT3)
#define SW_DEBUG4	(SW_OPT4)
#define SW_DEBUG5	(SW_OPT5)
#define SW_DEBUG6	(SW_OPT6)
#define SW_DEBUG7	(SW_OPT7)
#define SW_DEBUG8	(SW_OPT8)
#define SW_DEBUG9	(SW_OPT9)
#define SW_DEBUG10	(SW_OPT10)
#define SW_DEBUG11	(SW_OPT11)
#define SW_DEBUG12	(SW_OPT12)
#define SW_DEBUG13	(SW_OPT13)
#define SW_DEBUG14	(SW_OPT14)
#define SW_DEBUG15	(SW_OPT15)
#define SW_DEBUG_ALL	(SW_OPT0|SW_OPT1|SW_OPT2|SW_OPT3|SW_OPT4|SW_OPT5|SW_OPT6|SW_OPT7| \
			 SW_OPT8|SW_OPT9|SW_OPT10|SW_OPT11|SW_OPT12|SW_OPT13|SW_OPT14|SW_OPT15)
#endif

#define DO_PERF 1

#define PERF_STATE_SPIN 0
#define PERF_STATE_GAME 1
#define PERF_STATE_MBOX 2

typedef struct BDG_perftyp {
	S32 State;
	U32 FrameTime;
	U32 GameTime;
	U32 NetTime;
	U32 IntTime;
	U32 ModelTime[3];
} DBG_PERF;

extern DBG_PERF DBG_perf;


typedef struct time64 {
	U32	msb;
	U32	lsb;
} TIME64;

extern S32	gLine, gPal, gErrnum;
extern S32 DBG_AG_magic;
extern S32 DBG_timebar_blits_setup;	/*set to 0 on MBOX_ResetModels()*/
extern U32 prc_get_count(void);

#include "Pro/debug.pro"

#endif
