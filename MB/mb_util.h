#ifndef _MB_UTIL_H
#define _MB_UTIL_H

#include "fmath.h"
#include "mb.h"

/* Error codes for mathbox functions */
enum {
	MBOX_NOERR = 0,
	MBOX_WARN,
	MBOX_FATAL
};

enum {
	SHOW_ALL = 0,		/* show the root - don't bother with children */
	SHOW_EACHCHILD,		/* show each child individually */
	SHOW_RECURSIVE		/* used only by MBOX_ShowObject routine */
};

enum {
	XLU_ONLY = 0,		/* change translucency on root object only */
	XLU_CHILDREN,		/* change translucency on root and all of it's children */
	XLU_RECURSIVE		/* used only by MBOX_ChangeXlu */
};

enum {
	HIDE_ALL = 0,		/* hide the root - don't display children */
	HIDE_ONLY,			/* hide the root - display children */
	HIDE_EACHCHILD,		/* hide each child individually */
	HIDE_RECURSIVE		/* used only by the MBOX_HideObject routine */
};


#include "Pro/mb_util.pro"

#endif
