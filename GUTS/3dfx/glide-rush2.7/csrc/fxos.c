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
** successor clauses in the FAR, DOD or NASA FAR Supplement. Unpublished  -
** rights reserved under the Copyright Laws of the United States.
**
** $Revision: 5 $
** $Date: 3/21/96 11:45a $
*/

#include <stdio.h>
#include <stdlib.h>

#ifdef WIN32
#include <windows.h>
#include <fxos.h>

void sleep(int secs)
{
    GdiFlush();
    Sleep(secs * 1000);
}

int fxGethostname(char *name, int n)
{
    return (int)GetComputerName((LPTSTR)name,(LPDWORD)&n);
}
#else
#include <fxos.h>
#endif

/* return current time in seconds (floating point)	*/
float fxTime(void)
{
#ifdef __sparc__
/* times returns 0 in BSD Unix, so we use ftime instead	*/
#   include <sys/timeb.h>
    struct timeb tb;
    static time_t once;			/* saves first time value */

    ftime(&tb);
    if (once == 0)			/* stash away first call */
	once = tb.time;			/* as float is not big enough */
    return (tb.time - once) + tb.millitm * .001;
    
#else
#if defined ( WIN32 ) || ( __DOS__ )
#   include <time.h>
#   define times(a) clock()
#   define HZ	CLOCKS_PER_SEC
#else
#   include <sys/types.h>
#   include <sys/times.h>
#   include <sys/param.h>
    struct tms foo;
#endif
    return times(&foo)/(float)HZ;
#endif
}

/* returns elapsed time in seconds */
float timer(int flag)
{
    static float starttime,endtime;

    if (flag == 0)
	starttime = endtime = fxTime();
    else
	endtime = fxTime();
    return endtime - starttime;
}

/*---------------------------------------------------------------------- */
/* open a file thru a search path, first search in the current directory */
/* and then go thru the semicolon separated search path */
/* returns the file handle and the path prefix that was used (if one) */
/*---------------------------------------------------------------------- */
FILE *fxFopenPath(const char *filename, const char *mode, const char *path, const char **pprefix)
{
    char *psemi;
    FILE *file;

    /* first try and open up the file in the current directory */
    if (pprefix) *pprefix = NULL;
    if (file = fopen(filename,mode))
	return file;
    if (path == NULL)
	return NULL;

    while (*path) {		/* for each chunk of the search path */
	char nameWithPath[1024];
	psemi = strchr(path,';');		/* find the next semicolon */
	if (psemi) {				/* if one, copy up to it */
	    strncpy(nameWithPath,path,psemi-path);
	    nameWithPath[psemi-path] = '\0';
	}
	else					/* else copy entire path */
	    strcpy(nameWithPath,path);
	strcat(nameWithPath,"/");		/* add directory separator */
	strcat(nameWithPath,filename);		/* add filename */
	if (pprefix) *pprefix = path;		/* save the prefix */
	if (file = fopen(nameWithPath,mode))
	    return file;
	path = psemi;				/* advance to next path element */
	if (path)
	    path++;				/* if one, then skip semicolon */
	else
	    break;				/* else all done */
    }
    return file;
}
