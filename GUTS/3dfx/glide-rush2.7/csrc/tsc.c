/*
** Copyright (c) 1995, 3Dfx Interactive, Inc.
** All Rights Reserved.
**
** This is UNPUBLISHED PROPRIETARY SOURCE CODE of 3Dfx Interactive, Inc.;
** the contents of this file may not be disclosed to third parties, copied or
** duplicated in any form, in whole or in part, without the prior written
** permission of 3Dfx Interactive, Inc.
**
** RESTRICTED RIGHTS LEGEND:
** Use, duplication or disclosure by the Government is subject to restrictions
** as set forth in subdivision (c)(1)(ii) of the Rights in Technical Data
** and Computer Software clause at DFARS 252.227-7013, and/or in similar or
** successor clauses in the FAR, DOD or NASA FAR Supplement. Unpublished -
** rights reserved under the Copyright Laws of the United States.
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <3dfx.h>
#include <tsc.h>

static FxU32 tsc_histogram[TSC_MAX_CLOCKS];

FxU32        tsc_begin, tsc_end;
static FxU32 tsc_bias;

void 
tscInit( void ) {
   FxU32 test = 100;
   int   i;
   

   for ( i = 0; i < 10; i++ )
   {
     tscBegin();
     tsc_end = tscReadTSC();
     if ( ( tsc_end - tsc_begin ) < test )
        test = tsc_end - tsc_begin;
   }
   tsc_bias = test;
}

/*
** timing routines
*/
void tscStoreDelta( void )
{
   FxU32 value = tsc_end - tsc_begin;

   if ( value < TSC_MAX_CLOCKS && value > tsc_bias )
      tsc_histogram[value-tsc_bias]++;
}

void tscPrintHistogram( void )
{
   float total = 0.0F;
   int   i;

   for ( i = 0; i < TSC_MAX_CLOCKS; i++ )
      total += tsc_histogram[i];

   for ( i = 0; i < TSC_MAX_CLOCKS; i++ )
   {
      float percent = ( tsc_histogram[i] * 100.0F ) / total;

      if ( percent > 1.0 )
      {
         printf( "%04d: %3.2f\n", i, percent );
      }
   }
}

FxU32 
tscGetMostFrequent(void) {
  float total = 0.0F;
  int   i;
  float
    maxPercent = 0.f;
  FxU32
    mostFrequentClockCount;
  
  for ( i = 0; i < TSC_MAX_CLOCKS; i++ )
    total += tsc_histogram[i];
  
  for ( i = 0; i < TSC_MAX_CLOCKS; i++ ) {
    float percent = ( tsc_histogram[i] * 100.0F ) / total;
    
    if ( percent > maxPercent ) {
      maxPercent = percent;
      mostFrequentClockCount = i;
    }
  }
  return mostFrequentClockCount;
}

FxU32 *tscGetHistogram( void )
{
   return tsc_histogram;
}

void tscResetHistogram(void)
{
  int i;
  
  for (i = 0; i < TSC_MAX_CLOCKS; i++)
    tsc_histogram[i] = 0;
}
 
