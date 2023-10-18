#ifndef _ERROR_H
#define _ERROR_H

#include "strings.h"
#include "Pro/error.pro"

#define GENERIC			0
#define MBOXTIMEOUT		1
#define ANIMERROR		2
#define MATHERROR		3
#define ERR				4
#define MBOXERROR		5
#define QUIETERROR		6
#define CRITICALERR		7

extern int running_standalone;

#endif
