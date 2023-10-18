/*
 *	font2.h	-- Definitions of font2 for font driver.
 *
 *	Copyright 1996 Time Warner Interactive.
 *	Unauthorized reproduction, adaptation, distribution, performance or 
 *	display of this computer program or the associated audiovisual work
 *	is strictly prohibited.
 *
 *	======================================================================
 *	$Author: gray $						$Date: 1996/09/09 02:23:24 $
 *	$Revision: 3.1 $						$Locker:  $
 *	======================================================================
 *	Change Log:
 *
 *	$Log: font2.h,v $
 * Revision 3.1  1996/09/09  02:23:24  gray
 * JAMMA version
 *
 * Revision 2.3  1996/08/14  05:29:03  petrick
 * Offset of 'L' was wrong.
 *
 * Revision 2.2  1996/06/07  02:07:28  petrick
 * Changed font to be less than 8:1 texture ratio.
 *
 * Revision 2.1  1996/05/04  02:56:18  petrick
 * Focus 2 release
 *
 * Revision 1.1  1996/03/27  05:36:46  petrick
 * Initial revision
 *
 *
 *	======================================================================
 */


#ifndef _font2_h_
#define _font2_h_

#include "font.h"

/* ASCII sort the following table: try unix cmd (man ascii) to see ordering */

/* ASCII order: !"#$%&'()*+,-./0123456789:;<=>?@A-Z[\]^_`a-z{|}~ */
/*        space^       minus^                thru^    thru^		 */


const FONTTYPE font2[] = 
{
	/*{ch, W,    X,  Y} => LETTER COORDINANTS */

	{'A', 11,   0,   0 },
	{'B',  9,  11,   0 },
	{'C', 10,  20,   0 },
	{'D', 10,  30,   0 },
	{'E',  8,  40,   0 },
	{'F',  8,  48,   0 },
	{'G', 10,  56,   0 },
	{'H', 10,  66,   0 },
	{'I',  5, 123,  16 }, /* out of order in file */
	{'J',  9,  76,   0 },
	{'K', 11,  85,   0 },
	{'L',  8,  95,   0 },
	{'M', 12, 104,   0 },
	{'N', 10, 116,   0 },

	{'O', 10,   0,  16 },
	{'P', 10,  10,  16 },
	{'Q', 10,  20,  16 },
	{'R', 10,  30,  16 },
	{'S',  9,  40,  16 },
	{'T',  9,  49,  16 },
	{'U', 10,  58,  16 },
	{'V', 11,  68,  16 },
	{'W', 12,  79,  16 },
	{'X', 11,  91,  16 },
	{'Y', 11, 102,  16 },
	{'Z', 11, 113,  16 },
};



const FONTINFO font2info = 
{ { "FONT2"/*Name (a.k.a. font2.rgb)*/,  128/*Width*/,  32/*Height*/ },
  16 /*letter height*/,
  12 /* spacewidth */,
  sizeof(font2)/sizeof(FONTTYPE),
  font2
};

#endif /* _font2_h_ */
