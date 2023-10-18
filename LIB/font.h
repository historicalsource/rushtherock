#ifndef _FONT_H
#define _FONT_H

#include "mb.h"
#include "blit.h"

typedef struct {
	char ch;
	U8	 width;
	U8	 x;
	U8	 y;
} FONTTYPE;

typedef struct {
	TEXDEF texdef;
	U8     lheight;
	U8	   spacewidth;
	U16	   total;
	const FONTTYPE * font;
} FONTINFO;

#define SFONTDEFCOLOR 0x7FFF

typedef struct {
	const FONTINFO * finfop;
	S16 next_x; /* except for x_inc added later */
	S16 last_x;
	S16 last_y;
	S16 x_inc; /* extra incrementing: value used when placing next char */
	S16 y_inc;
	S16 start_x;
	S16 color; /* use RGB555(,,): SFONTDEFCOLOR implies don't color */
	S16 dummy;
	F32	scale_x, scale_y;
} SFONTINFO;

#define INCX   (-1)
#define LASTX  (-2)
#define STARTX (-3)
#define INCY   (-1)
#define LASTY  (-2)

#include "Pro/font.pro"

#endif
