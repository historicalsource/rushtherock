#ifndef __TSC_H__
#define __TSC_H__

#ifndef __3DFX_H__
#  include <3dfx.h>
#endif

/*
** timing routines
*/
#define TSC_MAX_CLOCKS 5000

extern FxU32 tsc_begin, tsc_end;

FxU32 *tscGetHistogram( void );
FxU32  tscGetMostFrequent(void);
void   tscInit( void );
FxU32  tscReadTSC( void );
void   tscPrintHistogram( void );
void   tscStoreDelta();
void   tscResetHistogram(void);

#define tscBegin() \
          tsc_begin = tscReadTSC();
#define tscEnd() \
          tsc_end = tscReadTSC(); \
          tscStoreDelta();

#if defined(__WATCOMC__)
#pragma aux tscReadTSC = \
   ".586" \
   "rdtsc" \
   modify [edx eax] \
   value [eax];

#elif defined(__MSC__) || defined(__GNUC__)
FxU32 tscReadTSC(void);
#else
#error "TSC:  unsupported compiler"
#endif

#endif
