/*
 *  select.h -- defines for select.c
 *
 *  Copyright 1996 Time Warner Interactive
 *  Unauthorized reproduction, adaptation, distribution, performance or
 *  display of this computer program or the associated audiovisual work
 *  is strictly prohibited.
 *
 *  ======================================================================
 *  $Author: hightower $                       $Date: 1997/10/03 13:52:45 $
 *  $Revision: 1.2 $                        $Locker:  $
 *  ======================================================================
 *  Change Log:
 *
 *  $Log: select.h,v $
 * Revision 1.2  1997/10/03  13:52:45  hightower
 * Fix music play bug and allow punch down if no slowpokes
 *
 * Revision 1.1  1996/09/19  06:23:20  geraci
 * Initial revision
 *
 *
 *  ======================================================================
 */

#ifndef _SELECT_H
#define _SELECT_H

#include "blit.h"
#include "Pro/select.pro"

/*==============================  PROTOTYPES  ===============================*/

extern BOOL gWait4TrkPlaying;    /* "waiting for drivers on track n" playing */

#endif  /* _SELECT_H */

