/*****************************************************************************************
3D Hockey :

Copyright 1994,1995 Time Warner Interactive.  Unauthorized reproduction, adaptation,
distribution, performance or display of this computer program or the associated
audiovisual work is strictly prohibited.
*****************************************************************************************/
#include "strings.h"
#include <config.h>
#include <stdarg.h>
#include <stdtypes.h>
#include <os_proto.h>
#include "io.h"
#if HOCKEY
#include "debug.h"
#include "rom.h"
#endif
#if HOCKEY&&MB_HOST&&!ROM&&0
#include <iio_proto.h>
#define IIO_DEFINED 1
#else
#define IIO_DEFINED 0
#endif

#ifndef SJB
#define SJB 0
#endif

#ifndef TLF
#define TLF 0
#endif




int sprintf(char *str, char const *fmt, ...) {
	int ret;
	va_list ap;
	
	va_start(ap, fmt);
	ret = nvfprintf(str, 256, fmt, ap);
	va_end(ap);
	return (ret);
}


int vsprintf(char *fp, const char *fmt0, va_list ap) {
	return nvfprintf(fp, 256, fmt0, ap);
}






S32 ClearScreen;

void ClearOverlay(S16 sX,S16 sY,S16 eX, S16 eY) {
	if (DOPRINT) {
#if 0
		if (sX==0 && sY==0 && eX==64 && eY==48)
		{
			vid_clear();
			ClearScreen = 1;
		}
		else
#endif
		{
			S16 j;
			for(j=sY;j<eY;j++)
				txt_clr_wid(sX,j,(eX-sX));
			ClearScreen = 1;
		}
	}
}

void DoClearScreen()
{
#if 0
	vid_clear();		/* clear text in os */
#else
	ClearOverlay(0,0,64,50);
#endif
	ClearScreen = 1;	/* clear trap text */
}


static S16 clrovl;
static void VScreenInit(void);

void InitStrings( void ) {
	clrovl = 0;
}

void xyprint(S16 x, S16 y, const char *string) {
	if (DOPRINT && (DODEBUG || clrovl))
		xyprintc(RED_PAL,x,y,string);
}

void xyprintc(U32 color, S16 x, S16 y, const char *string) {
	if (DOPRINT) {
		if(DODEBUG) {
			txt_str(x,y,string,color);
			clrovl = 1;
		} else if(clrovl) {
			DoClearScreen();
			clrovl = 0;
		}
	}
}

extern int (vsprintf)(char *fp, const char *fmt0, va_list ap);

void xyprintf(S16 x, S16 y, char const *fmt,...) {
	if (DOPRINT && (DODEBUG || clrovl)) {
		char str[70];
		va_list ap;

		va_start(ap, fmt);
		vsprintf(str, fmt, ap);
		va_end(ap);
	
		xyprintc(RED_PAL,x,y,str);
	}
	if (!DOPRINT && clrovl) {
		DoClearScreen();
		clrovl = 0;
	}
}

void xyprintfc(U32 color,S16 x, S16 y, char const *fmt,...) {
	if (DOPRINT && (DODEBUG || clrovl)) {
		char str[70];
		va_list ap;

		va_start(ap, fmt);
		vsprintf(str, fmt, ap);
		va_end(ap);
	
		xyprintc(color,x,y,str);
	}
}

void xyecho(S16 x, S16 y, char const *str) {
	if (DOPRINT)
		txt_str(x,y,str,RED_PAL);
}

void xyechoc(U32 color, S16 x, S16 y, char const *str) {
	if (DOPRINT)
		txt_str(x,y,str,color);
}

void xyechobigc(U32 color, S16 x, S16 y, char const *str) {
	if (DOPRINT)
		txt_str(x,y,str,color | 0 /*AN_BIG_SET*/);
}

void xyechof(S16 x, S16 y, char const *fmt,...) {
	if (DOPRINT) {
		char str[70];
		va_list ap;

		va_start(ap, fmt);
		vsprintf(str, fmt, ap);
		va_end(ap);
		
		txt_str(x,y,str,RED_PAL);
	}
}

void xyechofc(U32 color,S16 x, S16 y, char const *fmt,...) {
	if (DOPRINT) {
		char str[70];
		va_list ap;

		va_start(ap, fmt);
		vsprintf(str, fmt, ap);
		va_end(ap);
	
		txt_str(x,y,str,color);
	}
}

void debug(int db,U32 color,S16 x, S16 y, char const *fmt,...) {
	if (DOPRINT && (DODEBUG || clrovl)) {
		char str[70];
		va_list ap;

		if (!((db & SJB) && (DEBUG4)) && !((db & TLF) && (DEBUG5)))
			return;
	
		va_start(ap, fmt);
		vsprintf(str, fmt, ap);
		va_end(ap);
	
		xyprintc(color,x,y,str);
	}
}

void printUVS(U32 color,S16 x, S16 y, char *desc, F32 uvs[][3]) {
	if (DOPRINT && (DODEBUG || clrovl)) {
		xyprintfc(color,x,y,"%s: %5f %5f %5f/ %5f %5f %5f/ %5f %5f %5f",desc,
				  uvs[0][0],uvs[0][1],uvs[0][2],
				  uvs[1][0],uvs[1][1],uvs[1][2],
				  uvs[2][0],uvs[2][1],uvs[2][2]);
	}
}

void gprintf(S16 x, S16 y, char const *fmt,...) {
	char str[170];
	va_list ap;
	va_start(ap, fmt);
	vsprintf(str, fmt, ap);
	va_end(ap);
	xyechoc(GRN_PAL,x,y,str);
}
