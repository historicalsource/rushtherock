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
** $Revision: 12 $ 
** $Date: 6/21/96 7:29p $ 
**
*/

#ifndef _SST1TEST_H_
#define _SST1TEST_H_


#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
# if defined(__MSC__)
#include <conio.h>
#include <io.h>
#include <dos.h>
# endif

#include <3dfx.h>
#include <sst.h>
#include <fxpci.h>
#define GLIDE_HARDWARE
#include <glide.h>
#include <sst1init.h>
#include <fxos.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    FxU32 width;
    FxU32 height;
    void  *data; /* all data rgb565 */
} Buffer;

typedef struct {
    GrMipMapId_t mmid;
    Gu3dfInfo    info;
} Texture;

typedef struct {
    FxU32  fbiRev;
    FxU32  tmuRev;
    FxU32  nTMUs;
    FxU32  tmuMem;
    FxU32  fbiMem;
    FxBool sli;
} TestConfig;


typedef struct {
    FxU32 x;
    FxU32 y;
    FxU32 width;
    FxU32 height;
} TestWindow;

typedef FxU32 TestOutput;
#define OUTPUT_CONSOLE  0x1
#define OUTPUT_MONO     0x4

typedef struct {
    FxU32      outputDevices;
    FILE       *console;
    TestWindow monoWindow;
} TestIO;

#define MONO_WIDTH    80
#define MONO_SCANLINE 160
#define MONO_BASE     ((char*)0xB0000)

typedef FxI32 TestCode;
#define EXCEPTION -1
#define FAIL       0
#define PASS       1
#define NOT_TESTED 2

typedef struct {
    FxU32            tmu;
    FxBool           reference;
    FxBool           initHardware;
    FxBool           shutdownHardware;
    TestConfig       *config;
    TestCode         result;
} TestPacket;

typedef void (*TestFP)(TestPacket *);

typedef struct {
    char       *name;
    TestFP      test;
    TestPacket packet;
} TestFunction;



#define MAX_NUM_TESTS 100

#define APP_FAIL    (-1)
#define APP_SUCCEED   0

/* UTIL.C */
void   testOutputInit( TestIO *io );
void   testOutput( char *fmt, ... );
FxBool testAskBegin( void );
int    testResultMatrix( FxU32 nTests, TestFunction testArray[] );
void   testPrintConfig( TestConfig *config );
void   testPrintArray( FxU32 nTests, TestFunction testArray[] );
void   testContinue( void );
FxBool testYesNo( void );
FxBool testSetBaseAddress( FxU32 baseAddress );
FxBool testReadConfigInt( char *file, char *tag, FxU32 *value );
FxBool testWriteConfigInt( char *file, char *tag, FxU32 value );
void       testSetException( const char *eString );
const char *testGetException( void );
Texture    *loadTexture( char *file );
void       dlTexture( Texture *t, 
                      FxU32 tmu, 
                      FxU32 mmLevel, 
                      FxU32 trilinear );


/* TESTS.C */
void testInit();
void testCreateTestArray( FxU32              *nTests, 
                          TestFunction       *testArray[],
                          TestConfig         *config,
                          FxBool             reference );

typedef void TestF( TestPacket * );

TestF testPOWERUP;
TestF testPASSTHRU;
TestF testPCI;
TestF testCONFIG;
TestF testFBIMEM;
TestF testTMUMEM;
#if 0
TestF testDAC640;
TestF testCHK640;
TestF testDAC800;
TestF testCHK800;
#else
TestF testDAC512;
TestF testCHK512;
#endif
TestF testGLIDE00;
TestF testGLIDE01;
TestF testGLIDE02;
TestF testGLIDE03;
TestF testGLIDE04;
TestF testGLIDE05;
TestF testGLIDE06;
TestF testGLIDE07;
TestF testGLIDE08;
TestF testGLIDE09;
TestF testGLIDE10;



#ifdef __cplusplus
}
#endif

#endif

