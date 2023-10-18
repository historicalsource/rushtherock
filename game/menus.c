/*
 *  menus.c -- General purpose debugging menus
 *
 *	Copyright 1996 Time Warner Interactive
 *	Unauthorized reproduction, adaptation, distribution, performance or 
 *	display of this computer program or the associated audiovisual work
 *	is strictly prohibited.
 *
 *	======================================================================
 *	$Author: gray $						$Date: 1997/09/05 22:38:22 $
 *	$Revision: 3.8 $						$Locker:  $
 *	======================================================================
 *	Change Log:
 *
 *	$Log: menus.c,v $
 * Revision 3.8  1997/09/05  22:38:22  gray
 * debug switch 0 is now debug switch 15.
 *
 * Revision 3.7  1997/05/30  19:02:58  gray
 * Got rid of FLAGSTAFF conditionals.
 *
 * Revision 3.6  1996/11/03  05:57:09  geraci
 * split up debug switches and regular game switches
 *
 * Revision 3.5  1996/10/29  05:06:55  geraci
 * fixed menu selection buttons to make them conform
 *
 * Revision 3.4  1996/10/10  05:19:24  petrick
 * Cleaned out some unused globals.
 *
 * Revision 3.3  1996/10/08  05:20:58  gray
 * Flagstaff port.
 *
 * Revision 3.2  1996/10/03  05:16:11  geraci
 * took in tunnel off of debug switch
 *
 * Revision 3.1  1996/09/09  02:24:52  gray
 * JAMMA version
 *
 * Revision 2.4  1996/07/10  22:42:14  geraci
 * put reverb switch on switch 2 instead of switch 1
 *
 * Revision 2.3  1996/05/16  18:12:59  geraci
 * exchanged view2 and view3 for coin keys
 *
 * Revision 2.2  1996/05/08  01:25:25  gray
 * COIN1 and COIN2 became COINL and COINR in new OS.
 *
 * Revision 2.1  1996/05/04  02:56:18  petrick
 * Focus 2 release
 *
 * Revision 1.5  1996/04/30  01:26:48  geraci
 * added tunnel test code
 *
 * Revision 1.4  1996/04/17  20:24:20  geraci
 * enables new steering wheel display
 *
 * Revision 1.3  1996/03/20  03:50:44  geraci
 * allowed debug switches to be used during model task
 *
 * Revision 1.2  1996/02/14  02:25:56  geraci
 * handled menu selection items better also handle menu depth better
 *
 * Revision 1.1  1996/02/02  05:32:42  geraci
 * Initial revision
 *
 *
 *	======================================================================
 */

#include "debug.h"       /* debug switches */
#include "config.h"      /* colors and switches etc. */
#include "game.h"
#include "globals.h"
#include "menus.h"
#include "model.h"       /* IRQTIME */
#include "os_proto.h"    /* txt_str */

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/

#define MENU_KEYS		(SW_ABORT | SW_REVERSE | SW_MUSIC | SW_VIEW1 | SW_VIEW2 | SW_VIEW3)

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/

extern	U32		last_levels;

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/

S16     cp_edit, cur_selection, num_selections, menu_depth;
S16		menu_txt_index[10], display_pos_data, clear_pos_data;
S16		disp_sw_bar;
MENU    *cur_menu;
MENU	*menu_tab[10];     /* 10 levels deep max. */
char    buf[66];

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Enable/Disable menu and if enabled check keystroke and update menu
*         Upon entry: mode -> Initialize = init important menu data
*                             Do_it = check keystroke and act on key         */

void check_menu (S16 mode, MENU *menu_pntr)
{
	if (mode == Initialize)
		{
		cp_edit = 0;
		menu_depth = 0;
		display_pos_data = clear_pos_data = disp_sw_bar = false;
        cur_menu = NULL;
		return;
		}

	if (dlevels & SW_DEBUG3)
		{
		display_pos_data = true;

		if (dlevels & SW_DEBUG15)
			{
			disp_sw_bar = true;
			dedges &= ~SW_DEBUG15;
			dlevels &= ~SW_DEBUG15;
			}
		else
			disp_sw_bar = false;
		}
	else
		display_pos_data = false;

	if (trailing_dedges & SW_DEBUG3)
		clear_pos_data = true;

    /* just turned menu switch on */
	if (dedges & SW_DEBUG15)
		{
		cp_edit = true;
		if (cur_menu == NULL)
			{
			cur_menu = menu_pntr;
			cur_selection = 1;
			}

		/* initialize menu display and run root menu's routine */
		display_menu (Initialize, cur_menu);

		if (cur_menu == NULL)
			{
			cp_edit = false;
			txt_str (2, 37, "Bad menu data!  No biscuit! ", RED_PAL);
			}
		}
	else
        /* just turned menu switch off */
		if (trailing_dedges & SW_DEBUG15)
		{
			cp_edit = false;
			display_menu (Undo_it, cur_menu);
		}		    

    /* are menus on? */
	if (cp_edit)
		{
        /* yes, update current selection */
		sprintf (buf, " %s", cur_menu[cur_selection].label_txt);
		txt_str (menu_txt_index[cur_selection], 38, buf, (IRQTIME & 0x100) ? YEL_PAL : RED_PAL);

        /* update root menu's routine */
        if (cur_menu[0].rout != NULL)
			(cur_menu[0].rout)(Do_it);

        /* hilight menu item to the left */
		if (edges & SW_VIEW1)
			{
			if (--cur_selection < 1)
				cur_selection = num_selections;

			display_menu (Do_it, cur_menu);
			}

		/* highlight menu item to the right */
        if (edges & SW_VIEW3)
			{
			if (++cur_selection > num_selections)
				cur_selection = 1;

			display_menu (Do_it, cur_menu);
			}

		/* select current menu item */
		if (edges & (SW_ABORT | SW_VIEW2))
			{
			/* if sub-menu exists, go into it */
			if (cur_menu[cur_selection].child_menu != NULL)
				{
				clear_display();

				menu_tab[menu_depth++] = (MENU *)cur_menu;

				cur_menu = (MENU *)cur_menu[cur_selection].child_menu;
				cur_selection = 1;

				display_menu (Initialize, cur_menu);

				if (cur_menu == NULL)
					{
					cp_edit = false;
					txt_str (2, 37, "Bad menu data!  ", RED_PAL);
					}
				}
			else
				/* if no sub-menu, just run routine (if any) */
				if (cur_menu[cur_selection].rout != NULL)
					(cur_menu[cur_selection].rout)(Initialize);

			}

		/* go back one menu level */
		if (edges & (SW_REVERSE | SW_MUSIC))
			{
			/* at root? */
			if (menu_depth != 0)
				/* no, back up one level */
				{
				clear_display();

				cur_menu = (MENU *)menu_tab[--menu_depth];
				cur_selection = 1;

				display_menu (Initialize, cur_menu);

				if (cur_menu == NULL)
					{
					cp_edit = false;
					txt_str (2, 37, "Bad menu data!  ", RED_PAL);
					}
				}
			else
				/* if no previos menu, just run routine (if any) */
				if (cur_menu[cur_selection].rout != NULL)
					(cur_menu[cur_selection].rout)(Initialize);

			}

		levels			&= ~MENU_KEYS;
		last_levels		&= ~MENU_KEYS;
		edges			&= ~MENU_KEYS;
		trailing_edges	&= ~MENU_KEYS;
		}
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Update current menu
*         Upon entry: mode -> Do_it = display menu
*                             Undo_it = clear menu area                      */

void display_menu (S16 mode, MENU *cur_menu)
{
	S16		index;

	switch (mode)
		{
		case Initialize:
            if (cur_menu[0].rout != NULL)
				(cur_menu[0].rout)(Initialize);

		case Do_it:
			index = 0;
			clear_menu();

			sprintf (buf, "%-60s", cur_menu[0].label_txt);
			txt_str (2, 37, buf, CYN_PAL);

            if (cur_menu[0].label_txt == NULL)
				{
				cur_menu = NULL;
				break;
				}

            num_selections = 0;
			menu_txt_index[1] = 1;

			while (cur_menu[++index].label_txt != NULL)
				{
                num_selections++;

				menu_txt_index[index+1] = menu_txt_index[index] + strlen(cur_menu[index].label_txt) + 1;

				sprintf (buf, " %s", cur_menu[index].label_txt);
				txt_str (menu_txt_index[index], 38, buf, YEL_PAL);
				}

			if (num_selections == 0)
				{
				cur_menu = NULL;
				break;
				}

			sprintf (buf, "%-60s", cur_menu[cur_selection].help_txt);
			txt_str (2, 39, buf, GRN_PAL);

			break;


		case Undo_it:
			/* clear_display will set up buf with all spaces */
			clear_display();
			clear_menu();
			break;
		}
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Clear display area (15-36)                                           */

void clear_display()
{
	S16 i;

	sprintf (buf, "%-60s", "");
	for (i=15; i<37; i++)
		txt_str (2, i, buf, WHT_PAL);
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Clear menu area (37-39)                                              */

void clear_menu()
{
	S16 i;

	sprintf (buf, "%-60s", "");
	for (i=37; i<40; i++)
		txt_str (2, i, buf, WHT_PAL);
}

