#ifndef __GDEBUG_H__
#define __GDEBUG_H__

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
** $Revision: 3 $
** $Date: 5/05/96 9:57a $
*/

#include <stdarg.h>

#define GDBG_MAX_LEVELS 512

/* if debug info turned on then GDBG_INFO does something */
#ifdef GDBG_INFO_ON

#define GDBG_INFO(a) gdbg_info a
#define GDBG_INFO_MORE(a) gdbg_info_more a
#define GDBG_PRINTF(a) gdbg_printf a

/* otherwise GDBG_INFO does nothing */
#else

#define GDBG_INFO(a)
#define GDBG_INFO_MORE(a)
#define GDBG_PRINTF(a)

#endif

#define GDBG_INIT		gdbg_init
#define GDBG_ERROR		gdbg_error
#define GDBG_GET_ERRORS		gdbg_get_errors
#define GDBG_GET_DEBUGLEVEL	gdbg_get_debuglevel
#define GDBG_SET_DEBUGLEVEL	gdbg_set_debuglevel
#define GDBG_SET_FILE		gdbg_set_file

extern void	gdbg_init(void);
extern void	gdbg_vprintf(const char *format, va_list);
extern void	gdbg_printf(const char *format, ...);
extern int	gdbg_info(const int level, const char *format, ...);
extern int	gdbg_info_more(const int level, const char *format, ...);
extern void	gdbg_error(const char *name, const char *format, ...);
extern int	gdbg_get_errors(void);
extern int	gdbg_get_debuglevel(const int level);
extern void	gdbg_set_debuglevel(const int level, const int value);
extern int	gdbg_set_file(const char *name);

#endif /* !__GDEBUG_H__ */
