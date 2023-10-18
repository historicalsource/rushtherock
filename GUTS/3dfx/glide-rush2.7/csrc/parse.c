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
** $Revision: 2 $ 
** $Date: 6/13/96 7:58a $ 
**
** Parsing code for grabbing information from "voodoo.ini" initialization file
**
*/
#pragma optimize ("",off)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <malloc.h>
#include <3dfx.h>
#define FX_DLL_DEFINITION
#include <fxdll.h>
#include <sst1vid.h>
#include <sst1init.h>

static int sst1InitFgets(char *, FILE *);
static int sst1InitFgetc(FILE *);
static int sst1InitParseFieldDac(char *);
static int sst1InitParseFieldCfg(char *);
static int sst1InitParseDacRdWrString(char *, sst1InitDacStruct *);
static int sst1InitParseDacRdWr(char *, sst1InitDacRdWrStruct *);
static int sst1InitParseSetVideoString(char *, sst1InitDacStruct *);
static int sst1InitParseSetMemClkString(char *, sst1InitDacStruct *);
static int sst1InitParseSetVideoModeString(char *, sst1InitDacStruct *);
static void sst1InitPrintDacRdWr(sst1InitDacRdWrStruct *, char *);
static void sst1InitToLower(char *string);
static void sst1InitFixFilename(char *dst, char *src);

/*
** sst1InitVoodooFile():
**  Find and setup "voodoo.ini" file if possible
**
**    Returns:
**      FxTRUE if "voodoo.ini" file is found and opened with no errors
**      FXFALSE if cannot find file or error opening file
**
*/
FX_ENTRY FxBool FX_CALL sst1InitVoodooFile()
{
#ifndef INIT_DOS
    return(FXFALSE);
#else
    char filename[256], fixedFilename[512], *tmpPtr;
    char path[512];
    FILE *file = (FILE *) NULL;
    char buffer[1024];
    int inCfg, inDac;
    int i;
    int helper = (GETENV(("SST_DEBUGDAC"))) ? 1 : 0;

    if(getenv("VOODOO_FILE")) {
        /* Override voodoo.ini name */
        strcpy(filename, getenv("VOODOO_FILE"));
        if(!(file = fopen(filename, "r")))
            return(FXFALSE);
    } else {
        /* Override path setting */
        if(getenv("VOODOO_PATH"))
            strcpy(path, getenv("VOODOO_PATH"));
        else if(getenv("PATH")) {
            strcpy(path, ".;");
            strcat(path, getenv("PATH"));
        } else
            strcpy(path, ".;");

        i = 0;
        while(1) {
            if(!i) {
                if((tmpPtr = strtok(path, ";")) == NULL)
                    break;
            } else {
                if((tmpPtr = strtok(NULL, ";")) == NULL)
                    break;
            }
            strcpy(filename, tmpPtr);
            sst1InitFixFilename(fixedFilename, filename);
            sprintf(filename, "%s\\voodoo.ini", filename);
            i++;
            if((file = fopen(filename, "r")))
                break;
        }
    }
    if(file == (FILE *) NULL)
        return(FXFALSE);

    inCfg = inDac = 0;
    while(sst1InitFgets(buffer, file)) {
        buffer[strlen(buffer)-1] = (char) NULL;
        if(!strcmp(buffer, "[CFG]")) {
            inCfg = 1; inDac = 0;
            continue;
        } else if (!strcmp(buffer, "[DAC]")) {
            inCfg = 0; inDac = 1;
            continue;
        } else if(buffer[0] == '[') {
            inCfg = 0; inDac = 0;
            continue;
        }
        if(inCfg) {
            if(!sst1InitParseFieldCfg(buffer)) {
                if(helper)
                    INIT_PRINTF(("sst1InitVoodooFile(): ERROR in [CFG] section of .ini file...\n"));
                return(FXFALSE);
            }
        } else if(inDac) {
            if(!sst1InitParseFieldDac(buffer)) {
                if(helper)
                    INIT_PRINTF(("sst1InitVoodooFile(): ERROR in [DAC] section of .ini file...\n"));
                return(FXFALSE);
            }
        }
    }
    fclose(file);
    INIT_PRINTF(("sst1Init Routines(): Using Initialization file '%s'\n", filename));

    return(FXTRUE);
#endif
}

#ifdef INIT_DOS

/*void main(int argc, char **argv) */
void foo(int argc, char **argv)
{
    char buffer[2048]; /* buffer for command line inputs */
    int inCfg, inDac;
    sst1InitEnvVarStruct *envVarsPtr;
    sst1InitDacStruct *dacPtr;
    sst1InitDacSetVideoStruct *setVideo;
    sst1InitDacSetMemClkStruct *setMemClk;
    FILE *file = fopen(argv[1], "r");

    inCfg = inDac = 0;
    while(sst1InitFgets(buffer, file)) {
        buffer[strlen(buffer)-1] = (char) NULL;
        if(!strcmp(buffer, "[CFG]")) {
            inCfg = 1; inDac = 0;
            continue;
        } else if (!strcmp(buffer, "[DAC]")) {
            inCfg = 0; inDac = 1;
            continue;
        } else if(buffer[0] == '[') {
            inCfg = 0; inDac = 0;
            continue;
        }
        if(inCfg) {
            if(!sst1InitParseFieldCfg(buffer))
                /* Error processing .ini file */
                exit(1);
        } else if(inDac) {
            if(!sst1InitParseFieldDac(buffer))
                /* Error processing .ini file */
                exit(1);
        }
    }

    /* Dump CFG Data... */
    envVarsPtr = envVarsBase;
    while(envVarsPtr) {
        printf("ENV VAR:%s  VALUE:%s\n", envVarsPtr->envVariable,
            envVarsPtr->envValue);
        envVarsPtr = (sst1InitEnvVarStruct *) envVarsPtr->nextVar;
    }

    /* Dump Dac Data... */
    dacPtr = dacStructBase;
    while(dacPtr) {
        printf("DAC MANU:%s  DEVICE:%s\n", dacPtr->dacManufacturer,
            dacPtr->dacDevice);
        if(dacPtr->detect) {
            printf("\tDetect:\n");
            sst1InitPrintDacRdWr(dacPtr->detect, "\t\t");
        }
        if(dacPtr->setVideo) {
            setVideo = dacPtr->setVideo;
            while(1) {
                printf("\tsetVideo (%dx%d @ %d Hz)\n",
                  setVideo->width, setVideo->height, setVideo->refresh);
                sst1InitPrintDacRdWr(setVideo->setVideoRdWr, "\t\t");
                if(!setVideo->nextSetVideo)
                    break;
                else
                    setVideo = setVideo->nextSetVideo;
            }
        }
        if(dacPtr->setMemClk) {
            setMemClk = dacPtr->setMemClk;
            while(1) {
                printf("\tsetMemClk (%d MHz)\n", setMemClk->frequency);
                sst1InitPrintDacRdWr(setMemClk->setMemClkRdWr, "\t\t");
                if(!setMemClk->nextSetMemClk)
                    break;
                else
                    setMemClk = setMemClk->nextSetMemClk;
            }
        }
        dacPtr = dacPtr->nextDac;
    }
    fclose(file);
}

static void sst1InitFixFilename(char *dst, char *src)
{
    while(*src) {
        *dst++ = *src;
        if(*src == '\\')
            *dst++ = *src;
        src++;
    }
    *dst = (char) NULL;
}


static int sst1InitFgets(char *string, FILE *stream)
{
    int validChars = 0;
    char *ptr = string;
    int charRead;

    while((charRead = sst1InitFgetc(stream))) {
        *ptr++ = (char) charRead;
        validChars++;
        if(charRead == '\n') {
            *ptr++ = (char) NULL;
            break;
        }
    }
    return(validChars);
}

static int sst1InitFgetc(FILE *stream)
{
    static int column = 0;
    static int validChars = 0;
    int charRead, charReadL;
    int inComment;

    inComment = 0;
    while(1) {
        charRead = fgetc(stream);
        if(inComment == 1) {
            if(charRead <= 0)
                return(0);
            else if(charRead == '\n')
                inComment = 0;
            column = 0;
            validChars = 0;
            continue;
        } else if(column == 0 && charRead == '#') {
            /* Comment line */
            inComment = 1;
            column = 0;
            validChars = 0;
        } else if(charRead <= 0) {
                return(0);
        } else {
            if(charRead == '\n') {
                if(validChars > 0) {
                    validChars = 0;
                    column = 0;
                    return(charRead);
                } else
                    continue;
            } else {
                if(isspace(charRead))
                    continue;
                validChars++;
                column++;
                charReadL = (islower(charRead)) ? toupper(charRead) : charRead;
                return(charReadL);
            }
        }
    }
}

static int sst1InitParseFieldCfg(char *string)
{
    char *envName, *envVal;
    sst1InitEnvVarStruct *envVarsPtr;

    if((envName = strtok(string, "=")) == NULL)
        return(0);
    if((envVal = strtok((char *) NULL, "=")) == NULL)
        /* Valid environment variable, NULL value */
        return(1);
    sst1InitToLower(envVal);

    if(envVarsBase == (sst1InitEnvVarStruct *) NULL) {
        if((envVarsPtr = malloc(sizeof(sst1InitEnvVarStruct))) ==
          (sst1InitEnvVarStruct *) NULL)
            return(0);
        envVarsBase = envVarsPtr;
    } else {
        envVarsPtr = envVarsBase;
        while(1) {
            if(envVarsPtr->nextVar == (sst1InitEnvVarStruct *) NULL)
                break;
            else
               envVarsPtr = envVarsPtr->nextVar;
        }
        if((envVarsPtr->nextVar = malloc(sizeof(sst1InitEnvVarStruct))) ==
           (sst1InitEnvVarStruct *) NULL)
            return(0);
        envVarsPtr = envVarsPtr->nextVar;
    }
    envVarsPtr->nextVar = (sst1InitEnvVarStruct *) NULL;
    strcpy(envVarsPtr->envVariable, envName);
    strcpy(envVarsPtr->envValue, envVal);

    return(1);
}

static int sst1InitParseFieldDac(char *string)
{
    char *dacFieldReference, *dacFieldValue;
    static sst1InitDacStruct *dacPtr = (sst1InitDacStruct *) NULL;

    if((dacFieldReference = strtok(string, "=")) == NULL)
        return(0);
    if(!strcmp(dacFieldReference, "MANUFACTURER")) {
        /* Add new dac device */
        if(dacStructBase == (sst1InitDacStruct *) NULL) {
            if((dacPtr = malloc(sizeof(sst1InitDacStruct))) ==
              (sst1InitDacStruct *) NULL)
                return(0);
            dacStructBase = dacPtr;
        } else {
            dacPtr = dacStructBase;
            while(1) {
                if(dacPtr->nextDac == (sst1InitDacStruct *) NULL)
                    break;
                else
                   dacPtr = dacPtr->nextDac;
            }
                if((dacPtr->nextDac = malloc(sizeof(sst1InitDacStruct))) ==
               (sst1InitDacStruct *) NULL)
                return(0);
            dacPtr = dacPtr->nextDac;
        }
        dacPtr->nextDac = (sst1InitDacStruct *) NULL;
        dacPtr->dacManufacturer[0] = (char) NULL;
        dacPtr->dacDevice[0] = (char) NULL;
        dacPtr->detect = (sst1InitDacRdWrStruct *) NULL;
        dacPtr->setVideo = (sst1InitDacSetVideoStruct *) NULL;
        dacPtr->setMemClk = (sst1InitDacSetMemClkStruct *) NULL;
        dacPtr->setVideoMode = (sst1InitDacSetVideoModeStruct *) NULL;
        if((dacFieldValue = strtok((char *) NULL, "=")) == NULL)
            return(0);
        strcpy(dacPtr->dacManufacturer, dacFieldValue);
    } else if(!strcmp(dacFieldReference, "DEVICE")) {
        if((dacFieldValue = strtok((char *) NULL, "=")) == NULL)
            return(0);
        strcpy(dacPtr->dacDevice, dacFieldValue);
    } else if(!strcmp(dacFieldReference, "DETECT")) {
        if((dacFieldValue = strtok((char *) NULL, "=")) == NULL)
            return(0);
        sst1InitToLower(dacFieldValue);
        if(!sst1InitParseDacRdWrString(dacFieldValue, dacPtr))
            return(0);
    } else if(!strcmp(dacFieldReference, "SETVIDEO")) {
        if((dacFieldValue = strtok((char *) NULL, "=")) == NULL)
            return(0);
        sst1InitToLower(dacFieldValue);
        if(!sst1InitParseSetVideoString(dacFieldValue, dacPtr))
            return(0);
    } else if(!strcmp(dacFieldReference, "SETMEMCLK")) {
        if((dacFieldValue = strtok((char *) NULL, "=")) == NULL)
            return(0);
        sst1InitToLower(dacFieldValue);
        if(!sst1InitParseSetMemClkString(dacFieldValue, dacPtr))
            return(0);
    } else if(!strcmp(dacFieldReference, "SETVIDEOMODE")) {
        if((dacFieldValue = strtok((char *) NULL, "=")) == NULL)
            return(0);
        sst1InitToLower(dacFieldValue);
        if(!sst1InitParseSetVideoModeString(dacFieldValue, dacPtr))
            return(0);
    } else
        return(0);

    return(1);
}

static int sst1InitParseDacRdWrString(char *string, sst1InitDacStruct *dacBase)
{
    int cntr = 0;
    int i;
    sst1InitDacRdWrStruct *dacRdWrPtr;
    char *dacRdWrCmd;
    char stringCpy[2048];

    while(1) {
        strcpy(stringCpy, string);
        if((dacRdWrCmd = strtok(stringCpy, ";")) == NULL)
            return(0);
        if(cntr == 0) {
            if(!(dacBase->detect = malloc(sizeof(sst1InitDacRdWrStruct))))
                return(0);
            dacRdWrPtr = dacBase->detect;
        } else {
            for(i=0; i<(cntr-1); i++) {
                if((dacRdWrCmd = strtok(NULL, ";")) == NULL)
                    return(0);
            }
            if((dacRdWrCmd = strtok(NULL, ";")) == NULL) {
                break;
            }
            if(!(dacRdWrPtr->nextRdWr = malloc(sizeof(sst1InitDacRdWrStruct)))) 
                return(0);

            dacRdWrPtr = dacRdWrPtr->nextRdWr;
        }
        dacRdWrPtr->nextRdWr = (sst1InitDacRdWrStruct *) NULL;
        if(!sst1InitParseDacRdWr(dacRdWrCmd, dacRdWrPtr))
            return(0);
        cntr++;
    }
    return(1);
}

static int sst1InitParseDacRdWr(char *string, sst1InitDacRdWrStruct *dacRdWrPtr)
{
    char *addr, *data, *mask, *addrDataCmd;
    char stringCpy[2048];

    strcpy(stringCpy, string);

    if(stringCpy[5] == '(') {
        stringCpy[5] = (char) NULL;
        addrDataCmd = &stringCpy[6];
    } else if(stringCpy[7] == '(') {
        stringCpy[7] = (char) NULL;
        addrDataCmd = &stringCpy[8];
    } else if(stringCpy[8] == '(') {
        stringCpy[8] = (char) NULL;
        addrDataCmd = &stringCpy[9];
    } else if(stringCpy[9] == '(') {
        stringCpy[9] = (char) NULL;
        addrDataCmd = &stringCpy[10];
    } else
        return(0);

    if(!strcmp(stringCpy, "dacwr")) {
        if((addr = strtok(addrDataCmd, ",")) == NULL)
            return(0);
        if((data = strtok(NULL, ",")) == NULL)
            return(0);
        if(data[strlen(data)-1] != ')')
            return(0);
        data[strlen(data)-1] = (char) NULL;
        dacRdWrPtr->type = DACRDWR_TYPE_WR;
        SSCANF(addr, "%i", &dacRdWrPtr->addr);
        SSCANF(data, "%i", &dacRdWrPtr->data);
    } else if(!strcmp(stringCpy, "dacwrpop")) {
        if((addr = strtok(addrDataCmd, ",")) == NULL)
            return(0);
        if((mask = strtok(NULL, ",")) == NULL)
            return(0);
        if((data = strtok(NULL, ",")) == NULL)
            return(0);
        if(data[strlen(data)-1] != ')')
            return(0);
        data[strlen(data)-1] = (char) NULL;
        dacRdWrPtr->type = DACRDWR_TYPE_WRMOD_POP;
        SSCANF(addr, "%i", &dacRdWrPtr->addr);
        SSCANF(mask, "%i", &dacRdWrPtr->mask);
        SSCANF(data, "%i", &dacRdWrPtr->data);
    } else if(!strcmp(stringCpy, "dacrdwr")) {
        if((addr = strtok(addrDataCmd, ",")) == NULL)
            return(0);
        if((mask = strtok(NULL, ",")) == NULL)
            return(0);
        if((data = strtok(NULL, ",")) == NULL)
            return(0);
        if(data[strlen(data)-1] != ')')
            return(0);
        data[strlen(data)-1] = (char) NULL;
        dacRdWrPtr->type = DACRDWR_TYPE_RDMODWR;
        SSCANF(addr, "%i", &dacRdWrPtr->addr);
        SSCANF(mask, "%i", &dacRdWrPtr->mask);
        SSCANF(data, "%i", &dacRdWrPtr->data);
    } else if(!strcmp(stringCpy, "dacrd")) {
        if((addr = strtok(addrDataCmd, ",")) == NULL)
            return(0);
        if(addr[strlen(addr)-1] == ')') {
            dacRdWrPtr->type = DACRDWR_TYPE_RDNOCHECK;
            SSCANF(addr, "%i", &dacRdWrPtr->addr);
        } else {
            dacRdWrPtr->type = DACRDWR_TYPE_RDCHECK;
            if((data = strtok(NULL, ",")) == NULL)
                return(0);
            if(data[strlen(data)-1] != ')')
                return(0);
            data[strlen(data)-1] = (char) NULL;
            SSCANF(addr, "%i", &dacRdWrPtr->addr);
            SSCANF(data, "%i", &dacRdWrPtr->data);
        }
    } else if(!strcmp(stringCpy, "dacrdpush")) {
        if((addr = strtok(addrDataCmd, ",")) == NULL)
            return(0);
        if(addr[strlen(addr)-1] == ')') {
            dacRdWrPtr->type = DACRDWR_TYPE_RDPUSH;
            SSCANF(addr, "%i", &dacRdWrPtr->addr);
        } else
            return(0);
    } else {
        return(0);
    }

    return(1);
}

static void sst1InitPrintDacRdWr(sst1InitDacRdWrStruct *dacRdWrBase,
  char *prefix)
{
    sst1InitDacRdWrStruct *dacRdWrPtr = dacRdWrBase;

    while(dacRdWrPtr) {
        if(dacRdWrPtr->type == DACRDWR_TYPE_WR) {
            printf("%sDacWR", prefix);
            printf("(0x%x,0x%x)\n", dacRdWrPtr->addr, dacRdWrPtr->data);
        } else if(dacRdWrPtr->type == DACRDWR_TYPE_RDMODWR) {
            printf("%sDacRD-MOD-WR", prefix);
            printf("(0x%x,0x%x,0x%x)\n", dacRdWrPtr->addr, dacRdWrPtr->mask,
              dacRdWrPtr->data);
        } else if(dacRdWrPtr->type == DACRDWR_TYPE_RDNOCHECK) {
            printf("%sDacRD-NOCHECK", prefix);
            printf("(0x%x)\n", dacRdWrPtr->addr);
        } else if(dacRdWrPtr->type == DACRDWR_TYPE_RDCHECK) {
            printf("%sDacRD-CHECK", prefix);
            printf("(0x%x,0x%x)\n", dacRdWrPtr->addr, dacRdWrPtr->data);
        } else
            printf("%sDAC???", prefix);
        dacRdWrPtr = dacRdWrPtr->nextRdWr;
    }
}

static int sst1InitParseSetVideoString(char *string, sst1InitDacStruct *dacBase)
{
    int cntr = 0;
    int i;
    sst1InitDacRdWrStruct *dacRdWrPtr;
    sst1InitDacSetVideoStruct *dacSetVideoPtr;
    char *dacRdWrCmd;
    char stringCpy[2048];

    while(1) {
        strcpy(stringCpy, string);
        if((dacRdWrCmd = strtok(stringCpy, ";")) == NULL)
            return(0);
        if(cntr == 0) {
            if(!dacBase->setVideo) {
                if(!(dacBase->setVideo =
                  malloc(sizeof(sst1InitDacSetVideoStruct))))
                    return(0);
                dacSetVideoPtr = dacBase->setVideo;
            } else {
                dacSetVideoPtr = dacBase->setVideo;
                while(1) {
                    if(!dacSetVideoPtr->nextSetVideo)
                        break;
                    dacSetVideoPtr = dacSetVideoPtr->nextSetVideo;
                }
                if(!(dacSetVideoPtr->nextSetVideo =
                   malloc(sizeof(sst1InitDacSetVideoStruct))))
                    return(0);
                dacSetVideoPtr = dacSetVideoPtr->nextSetVideo;
            }
            dacSetVideoPtr->nextSetVideo = (sst1InitDacSetVideoStruct *) NULL;
            /* Width */
            SSCANF(dacRdWrCmd, "%i", &dacSetVideoPtr->width);
            /* Height */
            if((dacRdWrCmd = strtok(NULL, ";")) == NULL)
                return(0);
            SSCANF(dacRdWrCmd, "%i", &dacSetVideoPtr->height);
            /* Refresh */
            if((dacRdWrCmd = strtok(NULL, ";")) == NULL)
                return(0);
            SSCANF(dacRdWrCmd, "%i", &dacSetVideoPtr->refresh);
            /* video16BPP */
            if((dacRdWrCmd = strtok(NULL, ";")) == NULL)
                return(0);
            SSCANF(dacRdWrCmd, "%i", &dacSetVideoPtr->video16BPP);
            /* First DacRdWr */
            if((dacRdWrCmd = strtok(NULL, ";")) == NULL)
                return(0);
            if(!(dacSetVideoPtr->setVideoRdWr =
               malloc(sizeof(sst1InitDacRdWrStruct))))
                return(0);
            dacRdWrPtr = dacSetVideoPtr->setVideoRdWr;
        } else {
            for(i=0; i<(cntr+3); i++) {
                if((dacRdWrCmd = strtok(NULL, ";")) == NULL)
                    return(0);
            }
            if((dacRdWrCmd = strtok(NULL, ";")) == NULL) {
                break;
            }
            if(!(dacRdWrPtr->nextRdWr = malloc(sizeof(sst1InitDacRdWrStruct))))
                return(0);
            dacRdWrPtr = dacRdWrPtr->nextRdWr;
        }
        dacRdWrPtr->nextRdWr = (sst1InitDacRdWrStruct *) NULL;
        if(!sst1InitParseDacRdWr(dacRdWrCmd, dacRdWrPtr))
            return(0);
        cntr++;
    }
    return(1);
}

static int sst1InitParseSetMemClkString(char *string,
  sst1InitDacStruct *dacBase)
{
    int cntr = 0;
    int i;
    sst1InitDacRdWrStruct *dacRdWrPtr;
    sst1InitDacSetMemClkStruct *dacSetMemClkPtr;
    char *dacRdWrCmd;
    char stringCpy[2048];

    while(1) {
        strcpy(stringCpy, string);
        if((dacRdWrCmd = strtok(stringCpy, ";")) == NULL)
            return(0);
        if(cntr == 0) {
            if(!dacBase->setMemClk) {
                if(!(dacBase->setMemClk =
                  malloc(sizeof(sst1InitDacSetMemClkStruct))))
                    return(0);
                dacSetMemClkPtr = dacBase->setMemClk;
            } else {
                dacSetMemClkPtr = dacBase->setMemClk;
                while(1) {
                    if(!dacSetMemClkPtr->nextSetMemClk)
                        break;
                    dacSetMemClkPtr = dacSetMemClkPtr->nextSetMemClk;
                }
                if(!(dacSetMemClkPtr->nextSetMemClk =
                   malloc(sizeof(sst1InitDacSetMemClkStruct))))
                    return(0);
                dacSetMemClkPtr = dacSetMemClkPtr->nextSetMemClk;
            }
            dacSetMemClkPtr->nextSetMemClk = (sst1InitDacSetMemClkStruct *)
              NULL;
            /* Frequency */
            SSCANF(dacRdWrCmd, "%i", &dacSetMemClkPtr->frequency);

            /* First DacRdWr */
            if((dacRdWrCmd = strtok(NULL, ";")) == NULL)
                return(0);
            if(!(dacSetMemClkPtr->setMemClkRdWr =
               malloc(sizeof(sst1InitDacRdWrStruct))))
                return(0);
            dacRdWrPtr = dacSetMemClkPtr->setMemClkRdWr;
        } else {
            for(i=0; i<(cntr); i++) {
                if((dacRdWrCmd = strtok(NULL, ";")) == NULL)
                    return(0);
            }
            if((dacRdWrCmd = strtok(NULL, ";")) == NULL) {
                break;
            }
            if(!(dacRdWrPtr->nextRdWr = malloc(sizeof(sst1InitDacRdWrStruct))))
                return(0);
            dacRdWrPtr = dacRdWrPtr->nextRdWr;
        }
        dacRdWrPtr->nextRdWr = (sst1InitDacRdWrStruct *) NULL;
        if(!sst1InitParseDacRdWr(dacRdWrCmd, dacRdWrPtr))
            return(0);
        cntr++;
    }
    return(1);
}

static int sst1InitParseSetVideoModeString(char *string,
  sst1InitDacStruct *dacBase)
{
    int cntr = 0;
    int i;
    sst1InitDacRdWrStruct *dacRdWrPtr;
    sst1InitDacSetVideoModeStruct *dacSetVideoModePtr;
    char *dacRdWrCmd;
    char stringCpy[2048];

    while(1) {
        strcpy(stringCpy, string);
        if((dacRdWrCmd = strtok(stringCpy, ";")) == NULL)
            return(0);
        if(cntr == 0) {
            if(!dacBase->setVideoMode) {
                if(!(dacBase->setVideoMode =
                  malloc(sizeof(sst1InitDacSetVideoModeStruct))))
                    return(0);
                dacSetVideoModePtr = dacBase->setVideoMode;
            } else {
                dacSetVideoModePtr = dacBase->setVideoMode;
                while(1) {
                    if(!dacSetVideoModePtr->nextSetVideoMode)
                        break;
                    dacSetVideoModePtr = dacSetVideoModePtr->nextSetVideoMode;
                }
                if(!(dacSetVideoModePtr->nextSetVideoMode =
                   malloc(sizeof(sst1InitDacSetVideoModeStruct))))
                    return(0);
                dacSetVideoModePtr = dacSetVideoModePtr->nextSetVideoMode;
            }
            dacSetVideoModePtr->nextSetVideoMode =
              (sst1InitDacSetVideoModeStruct *) NULL;
            /* video16BPP */
            SSCANF(dacRdWrCmd, "%i", &dacSetVideoModePtr->video16BPP);

            /* First DacRdWr */
            if((dacRdWrCmd = strtok(NULL, ";")) == NULL)
                return(0);
            if(!(dacSetVideoModePtr->setVideoModeRdWr =
               malloc(sizeof(sst1InitDacRdWrStruct))))
                return(0);
            dacRdWrPtr = dacSetVideoModePtr->setVideoModeRdWr;
        } else {
            for(i=0; i<(cntr); i++) {
                if((dacRdWrCmd = strtok(NULL, ";")) == NULL)
                    return(0);
            }
            if((dacRdWrCmd = strtok(NULL, ";")) == NULL) {
                break;
            }
            if(!(dacRdWrPtr->nextRdWr = malloc(sizeof(sst1InitDacRdWrStruct))))
                return(0);
            dacRdWrPtr = dacRdWrPtr->nextRdWr;
        }
        dacRdWrPtr->nextRdWr = (sst1InitDacRdWrStruct *) NULL;
        if(!sst1InitParseDacRdWr(dacRdWrCmd, dacRdWrPtr))
            return(0);
        cntr++;
    }
    return(1);
}

static void sst1InitToLower(char *string)
{
    char *ptr = string;

    while(*ptr) {
        *ptr = (isupper(*ptr)) ? tolower(*ptr) : *ptr;
        ptr++;
    }
}

/*
** sst1InitGetenv():
**  Getenv() for INIT routines.
**
**  If the actual environment variable exists (determined by a call to
**  the system getenv() routine), then that pointer is returned.  Otherwise,
**  if the variable is defined in the [CFG] section of "voodoo.ini", then
**  a pointer to the value defined in "voodoo.ini" is returned.  Otherwise,
**  NULL is returned
**
*/
FX_ENTRY char * FX_CALL sst1InitGetenv(char *string)
{
    sst1InitEnvVarStruct *envVarsPtr;
    char *retVal = (char *) NULL;

    /* Does the real environment variable exist? */
    if(getenv(string))
        return(getenv(string));

    /* Does the requested environment variable exist in "voodoo.ini"? */
    /* Dump CFG Data... */
    envVarsPtr = envVarsBase;
    while(envVarsPtr) {
        if(!strcmp(string, envVarsPtr->envVariable)) {
            retVal = envVarsPtr->envValue;
            break;
        }
        envVarsPtr = (sst1InitEnvVarStruct *) envVarsPtr->nextVar;
    }
    return(retVal);
}
#endif  /* INIT_DOS */

#pragma optimize ("",on)
