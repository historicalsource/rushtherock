/*
 *	font3.h	-- Definitions of font1 for font driver.
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
 *	$Log: font3.h,v $
 * Revision 3.1  1996/09/09  02:23:24  gray
 * JAMMA version
 *
 * Revision 1.3  1996/09/04  09:34:12  petrick
 * Changes lower row start point.
 *
 * Revision 1.2  1996/08/30  16:01:23  petrick
 * Fixed some mistakes.
 *
 * Revision 1.1  1996/08/30  12:28:45  petrick
 * Initial revision
 *
 *	8/29/96 New Font File: Tom Capizzi
 *
 *	======================================================================
 */


#ifndef _font3_h_
#define _font3_h_

#include "font.h"

/* ASCII sort the following table: try unix cmd (man ascii) to see ordering */

/* ASCII order: !"#$%&'()*+,-./0123456789:;<=>?@A-Z[\]^_`a-z{|}~ */
/*        space^       minus^                thru^    thru^		 */


const FONTTYPE font3[] = 
{
	/*{ch, W,    X,  Y} => LETTER COORDINATES */

	{'A', 33,   1,   0 },
	{'B', 26,  34,   0 },
	{'C', 25,  60,   0 },
	{'E', 22,  85,   0 },
	{'F', 23, 107,   0 },
	{'G', 27, 130,   0 },
	{'H', 28, 157,   0 },
	{'I', 13, 185,   0 },
	{'J', 17, 198,   0 },
	{'M', 37, 215,   0 },

	{'N', 27,   0,  34 },
	{'O', 31,  27,  34 },
	{'P', 26,  58,  34 },
	{'R', 26,  84,  34 },
	{'S', 23, 110,  34 },
	{'T', 24, 135,  34 },
	{'U', 26, 159,  34 },
	{'V', 29, 186,  34 },
	{'W', 40, 215,  34 },

	/*{'\\', 16,  55 , 181},*/ /* '\' is a special char and has to be doubled to '\\' */

	/* lower-case letters would go here */

};


const FONTINFO font3info = 
{ { "FONT3"/*Name (a.k.a. font1.rgb)*/,  256/*Width*/,  64/*Height*/ },
  32 /*letter height*/,
  20 /* spacewidth */,
  sizeof(font3)/sizeof(FONTTYPE),
  font3
};


#endif /* _font3_h_ */
