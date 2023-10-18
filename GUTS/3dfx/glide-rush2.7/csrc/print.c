/*
** Copyright (c) 1996, 3Dfx Interactive, Inc.
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
** successor clauses in the FAR, DOD or NASA FAR Supplement. Unpublished  -
** rights reserved under the Copyright Laws of the United States.
**
**
** $Revision: 4 $ 
** $Date: 6/13/96 7:45p $ 
**
** Print functions for SST-1 Initialization routines
*/
#pragma optimize ("",off)
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <sst.h>
#include <sst1vid.h>
#define FX_DLL_DEFINITION
#include <fxdll.h>
#include <sst1init.h>

#ifdef INIT_OUTPUT

FX_ENTRY void FX_CALL sst1InitVPrintf(const char *, va_list);

FX_ENTRY void FX_CALL sst1InitPrintf(const char *format, ...)
{
    va_list args;
    static FxBool firstPass = FXTRUE;
    static FxBool printIt = FXFALSE;

    if(firstPass == FXTRUE) {
        firstPass = FXFALSE;
        if(GETENV(("SST_INITDEBUG")))
            printIt = FXTRUE;
        if(GETENV(("SST_INITDEBUG_FILE"))) {
            if((sst1InitMsgFile = fopen(GETENV(("SST_INITDEBUG_FILE")), "w")))
                printIt = FXTRUE;
            else {
                fprintf(stderr, "sst1InitPrintf(): Could not open file '%s' for logging...\n", GETENV(("SST_INITDEBUG_FILE")));
                printIt = FXFALSE;
            }
        }
    }

    if(printIt == FXFALSE)
        return;

    va_start(args, format);
    sst1InitVPrintf(format, args);
    va_end(args);
}

FX_ENTRY void FX_CALL sst1InitVPrintf(const char *format, va_list args)
{
    vfprintf(sst1InitMsgFile, format, args);
    fflush(sst1InitMsgFile);
}

#endif

#pragma optimize ("",on)
