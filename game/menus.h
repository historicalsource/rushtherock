/*
 *	menus.h -- defines for menus.c
 *
 *	Copyright 1996 Time Warner Interactive
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
 *	$Log: menus.h,v $
 * Revision 3.1  1996/09/09  02:23:24  gray
 * JAMMA version
 *
 * Revision 2.1  1996/05/04  02:56:18  petrick
 * Focus 2 release
 *
 * Revision 1.2  1996/02/14  02:29:23  geraci
 * main_menu is now the external menu referenced not chkpoint_menu
 *
 * Revision 1.1  1996/02/02  05:33:22  geraci
 * Initial revision
 *
 *
 *	======================================================================
 */

#ifndef _MENU_H
#define _MENU_H

typedef struct
{
    char *label_txt;
    char *help_txt;
    void (*rout)(S16);
    void *child_menu;
} MENU;

#include "Pro/menus.pro"

/* only define MENU_DATA in module declaring chkpoint_menu (menu_data.c) */
#ifndef MENU_DATA
extern MENU main_menu[];
#endif

#endif	/* _MENU_H */

