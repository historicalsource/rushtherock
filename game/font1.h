/*
 *	font1.h	-- Definitions of font1 for font driver.
 *
 *	Copyright 1996 Time Warner Interactive.
 *	Unauthorized reproduction, adaptation, distribution, performance or 
 *	display of this computer program or the associated audiovisual work
 *	is strictly prohibited.
 *
 *	======================================================================
 *	$Author: geraci $						$Date: 1996/11/13 14:52:38 $
 *	$Revision: 3.6 $						$Locker:  $
 *	======================================================================
 *	Change Log:
 *
 *	$Log: font1.h,v $
 * Revision 3.6  1996/11/13  14:52:38  geraci
 * fixed some fonts
 *
 * Revision 3.5  1996/10/31  08:43:39  petrick
 * Made 'end' the proper size.
 *
 * Revision 3.4  1996/10/31  04:45:33  petrick
 * Added (+) chars.
 * .
 *
 * Revision 3.3  1996/10/03  04:39:55  petrick
 * Fixed size and location of 'END' character.
 *
 * Revision 3.2  1996/09/24  01:08:27  petrick
 * Adjusted for new font.
 *
 * Revision 3.1  1996/09/09  02:23:24  gray
 * JAMMA version
 *
 * Revision 2.3  1996/08/28  02:49:28  petrick
 * Fixed sizes of a couple of characters.
 *
 * Revision 2.2  1996/08/21  06:49:49  petrick
 * Added other color font1's/
 *
 * Revision 2.1  1996/05/04  02:56:18  petrick
 * Focus 2 release
 *
 * Revision 1.4  1996/03/21  06:32:26  petrick
 * Added 'end' char.
 *
 * Revision 1.3  1996/03/12  07:02:16  petrick
 * Added small letters.
 *
 * Revision 1.2  1996/03/06  10:37:30  petrick
 * Made digits non-proportional.
 *
 *
 *	======================================================================
 */


#ifndef _font1_h_
#define _font1_h_

#include "font.h"

/* ASCII sort the following table: try unix cmd (man ascii) to see ordering */

/* ASCII order: !"#$%&'()*+,-./0123456789:;<=>?@A-Z[\]^_`a-z{|}~ */
/*        space^       minus^                thru^    thru^		 */


const FONTTYPE font1[] = 
{
	/*{ch, W,    X,  Y} => LETTER COORDINANTS */

	{'!',  9, 101,  32 },
	{'(',  5, 214,  32 },
	{')',  5, 230,  32 },
	{'+', 11, 195,  32 },
	{'-', 16,  80,  32 },
	{'.',  9,  69,  32 },

	{'0', 16,   0,   0 },
	{'1', 16,  16,   0 },
	{'2', 16,  32,   0 },
	{'3', 16,  48,   0 },
	{'4', 16,  64,   0 },
	{'5', 16,  80,   0 },
	{'6', 16,  96,   0 },
	{'7', 16, 112,   0 },
	{'8', 16, 128,   0 },
	{'9', 15, 144,   0 },

	{':',  9, 164,  32 },
	{'?', 15, 112,  32 },
	{'@', 16, 176,  32 },

	{'A', 15, 160,   0 },
	{'B', 16, 176,   0 },
	{'C', 15, 192,   0 },
	{'D', 16, 208,   0 },
	{'E', 16, 224,   0 },
	{'F', 16, 240,   0 },

	{'G', 16,   0,  16 },
	{'H', 16,  16,  16 },
	{'I',  9,  36,  16 },
	{'J', 15,  48,  16 },
	{'K', 16,  64,  16 },
	{'L', 15,  80,  16 },
	{'M', 16,  96,  16 },
	{'N', 16, 112,  16 },
	{'O', 15, 128,  16 },
	{'P', 15, 144,  16 },
	{'Q', 15, 160,  16 },
	{'R', 15, 176,  16 },
	{'S', 16, 192,  16 },
	{'T', 15, 208,  16 },
	{'U', 15, 224,  16 },
	{'V', 15, 240,  16 },
	{'W', 16,   0,  32 },
	{'X', 15,  17,  32 },
	{'Y', 16,  32,  32 },
	{'Z', 16,  48,  32 },

	/*{'\\', 16,  55 , 181},*/ /* '\' is a special char and has to be doubled to '\\' */

	/* lower-case letters would go here */
	{'a', 16, 160,   0 },
	{'b', 16, 176,   0 },
	{'c', 15, 192,   0 },
	{'d', 16, 208,   0 },
	{'e', 16, 224,   0 },
	{'f', 16, 240,   0 },

	{'g', 16,   0,  16 },
	{'h', 16,  16,  16 },
	{'i',  9,  36,  16 },
	{'j', 16,  48,  16 },
	{'k', 16,  64,  16 },
	{'l', 15,  80,  16 },
	{'m', 16,  96,  16 },
	{'n', 16, 112,  16 },
	{'o', 16, 128,  16 },
	{'p', 15, 144,  16 },
	{'q', 15, 160,  16 },
	{'r', 15, 176,  16 },
	{'s', 16, 192,  16 },
	{'t', 15, 208,  16 },
	{'u', 16, 224,  16 },
	{'v', 15, 240,  16 },
	{'w', 16,   0,  32 },
	{'x', 16,  16,  32 },
	{'y', 16,  32,  32 },
	{'z', 16,  48,  32 },

	{'{', 16, 128,  32 },		/*	Use this for delete char/backspace. */
	{'}', 47,   0,  48 },		/*	Use for 'End'. */

};


const FONTINFO font1info = 
{ { "FONT1"/*Name (a.k.a. font1.rgb)*/,  256/*Width*/,  64/*Height*/ },
  16 /*letter height*/,
  16 /* spacewidth */,
  sizeof(font1)/sizeof(FONTTYPE),
  font1
};

const FONTINFO font1yinfo = 
{ { "FONT1YEL"/*Name (a.k.a. font1.rgb)*/,  256/*Width*/,  64/*Height*/ },
  16 /*letter height*/,
  16 /* spacewidth */,
  sizeof(font1)/sizeof(FONTTYPE),
  font1
};

const FONTINFO font1rinfo = 
{ { "FONT1RED"/*Name (a.k.a. font1.rgb)*/,  256/*Width*/,  64/*Height*/ },
  16 /*letter height*/,
  16 /* spacewidth */,
  sizeof(font1)/sizeof(FONTTYPE),
  font1
};

#endif /* _font1_h_ */
