/*
 *	menu_data.c -- menus used in menus.c
 *
 *	Copyright 1996 Time Warner Interactive
 *	Unauthorized reproduction, adaptation, distribution, performance or 
 *	display of this computer program or the associated audiovisual work
 *	is strictly prohibited.
 *
 *	======================================================================
 *	$Author: geraci $						$Date: 1996/11/11 22:13:18 $
 *	$Revision: 3.5 $						$Locker:  $
 *	======================================================================
 *	Change Log:
 *
 *	$Log: menu_data.c,v $
 * Revision 3.5  1996/11/11  22:13:18  geraci
 * linked priority and solo priority replaced by priority
 *
 * Revision 3.4  1996/11/07  14:36:37  geraci
 * lots of new maxpath tools
 *
 * Revision 3.3  1996/10/31  07:03:46  geraci
 * new max path tools
 *
 * Revision 3.2  1996/10/25  09:58:25  geraci
 * multi maxpath support, removed time stamp stuff
 *
 * Revision 3.1  1996/09/09  02:24:52  gray
 * JAMMA version
 *
 * Revision 2.8  1996/09/06  11:02:54  geraci
 * new menus for track hints
 *
 * Revision 2.7  1996/08/21  16:37:51  geraci
 * can save alternate max path to drone test directory
 *
 * Revision 2.6  1996/07/31  15:58:19  geraci
 * new tools for maxpath
 *
 * Revision 2.5  1996/06/28  05:01:10  geraci
 * max path edit mode
 *
 * Revision 2.4  1996/06/17  23:13:53  geraci
 * write hint file from maxpath menu
 *
 * Revision 2.3  1996/05/11  01:38:24  geraci
 * typo
 *
 * Revision 2.2  1996/05/11  01:15:05  geraci
 * added time stamp menus routines
 *
 * Revision 2.1  1996/05/04  02:56:18  petrick
 * Focus 2 release
 *
 * Revision 1.3  1996/04/27  02:19:52  geraci
 * displays maxpath data
 *
 * Revision 1.2  1996/02/14  02:17:06  geraci
 * new path point and sound menus
 *
 * Revision 1.1  1996/02/02  05:32:21  geraci
 * Initial revision
 *
 *
 *	======================================================================
 */

/* set MENU_DATA to suppress external defines for menus defined in this module */
#define MENU_DATA

#include "stdtypes.h"
#include "checkpoint.h"
#include "maxpath.h"
#include "menus.h"
#include "modeldat.h"
#include "Pro/timint.pro"

/*---------------------------------------------------------------------------*/

const MENU chkpoint_menu[] =
{
    { "Check point menu", NULL, display_chkpnts, NULL },
    { "Forward", "Move checkpoint to next path point", fwd_chkpnt, NULL },
    { "Backward", "Move checkpoint to previous path point", back_chkpnt, NULL },
    { NULL, NULL, NULL, NULL }
};

/*---------------------------------------------------------------------------*/

const MENU path_menu[] =
{
    { "Path point menu", NULL, display_path_points, NULL },
    { "Forward", "Move up path points list", fwd_path_points, NULL },
    { "Backward", "Move down path points list", back_path_points, NULL },
    { NULL, NULL, NULL, NULL }
};

/*---------------------------------------------------------------------------*/

const MENU set_maxpath_menu[] =
{
    { "Set maxpath data set to use", NULL, edit_hint_data, NULL },
	{ "Base_set", "Select maxpath base set", select_mpath_base, NULL },
	{ "Set 1", "Select maxpath set 1", select_mpath_set1, NULL },
	{ "Set 2", "Select maxpath set 2", select_mpath_set2, NULL },
	{ "Set 3", "Select maxpath set 3", select_mpath_set3, NULL },
	{ "Set 4", "Select maxpath set 4", select_mpath_set4, NULL },
	{ "Set 5", "Select maxpath set 5", select_mpath_set5, NULL },
	{ "Set 6", "Select maxpath set 6", select_mpath_set6, NULL },
	{ "Set 7", "Select maxpath set 7", select_mpath_set7, NULL },
    { NULL, NULL, NULL, NULL }
};

const MENU activate_maxpath_menu[] =
{
    { "Activate maxpath set previously de-activated", NULL, edit_hint_data, NULL },
	{ "Set 1", "Activate maxpath set 1", activate_mpath_set1, NULL },
	{ "Set 2", "Activate maxpath set 2", activate_mpath_set2, NULL },
	{ "Set 3", "Activate maxpath set 3", activate_mpath_set3, NULL },
	{ "Set 4", "Activate maxpath set 4", activate_mpath_set4, NULL },
	{ "Set 5", "Activate maxpath set 5", activate_mpath_set5, NULL },
	{ "Set 6", "Activate maxpath set 6", activate_mpath_set6, NULL },
	{ "Set 7", "Activate maxpath set 7", activate_mpath_set7, NULL },
    { NULL, NULL, NULL, NULL }
};

const MENU priority_menu[] =
{
    { "Set path priorites", NULL, edit_hint_data, NULL },
	{ "Inc_Priority", "Add 1 to solo priority (0=lowest)", inc_path_priority, NULL },
	{ "Dec_Priority", "Subtract 1 from path priority (0=lowest)", dec_path_priority, NULL },
    { NULL, NULL, NULL, NULL }
};

const MENU copy_maxpath_menu[] =
{
    { "Copy to current path from selected set", NULL, edit_hint_data, NULL },
	{ "Base_set", "Select maxpath base set", copy_from_base, NULL },
	{ "Set 1", "Select maxpath set 1", copy_from_set1, NULL },
	{ "Set 2", "Select maxpath set 2", copy_from_set2, NULL },
	{ "Set 3", "Select maxpath set 3", copy_from_set3, NULL },
	{ "Set 4", "Select maxpath set 4", copy_from_set4, NULL },
	{ "Set 5", "Select maxpath set 5", copy_from_set5, NULL },
	{ "Set 6", "Select maxpath set 6", copy_from_set6, NULL },
	{ "Set 7", "Select maxpath set 7", copy_from_set7, NULL },
    { NULL, NULL, NULL, NULL }
};

const MENU set_drone_menu[] =
{
    { "Set drone number for weight table", NULL, edit_weight_table, NULL },
	{ "Drone0", "Select drone 0", select_drone0, NULL },
	{ "Drone1", "Select drone 1", select_drone1, NULL },
	{ "Drone2", "Select drone 2", select_drone2, NULL },
	{ "Drone3", "Select drone 3", select_drone3, NULL },
	{ "Drone4", "Select drone 4", select_drone4, NULL },
	{ "Drone5", "Select drone 5", select_drone5, NULL },
	{ "Drone6", "Select drone 6", select_drone6, NULL },
	{ "Drone7", "Select drone 7", select_drone7, NULL },
    { NULL, NULL, NULL, NULL }
};

const MENU weight_menu[] =
{
	{ "Update path weight menu", NULL, edit_weight_table, NULL },
	{ "Set_drone", "Select drone to update", NULL, (MENU *)set_drone_menu },
	{ "Dist", "Weigh distance of drone to path", set_dist_weight, NULL },
	{ "Dir", "Weigh equivalence of drone and path direction", set_dir_weight, NULL },
	{ "Speed", "Weigh equivalence of drone and path speed", set_speed_weight, NULL },
	{ "Own_path", "Weigh if path is drone default path", set_own_weight, NULL },
	{ "Priority", "Weigh according to path priority", set_priority_weight, NULL },
	{ "W_Hint", "Weigh if current path is on priority hint", set_hint_weight, NULL },
	{ "Save", "Save weight file to hard drive", save_weight_table, NULL },
	{ NULL, NULL, NULL, NULL }
};

const MENU single_hint_menu[] =
{
    { "Edit hint on current point", NULL, edit_hint_data, NULL },
	{ "Width", "Stay within maxpath width", stay_within_width1, NULL },
	{ "Get_on", "Try to get back to maxpath", get_on_maxpath1, NULL },
	{ "Ease_on", "Ease back onto maxpath", ease_on_maxpath1, NULL },
	{ "Stay_on", "Stay on maxpath", stay_on_maxpath1, NULL },
	{ "Parallel", "Stay parallel to maxpath", stay_parallel_to_maxpath1, NULL },
	{ "Reset_Par", "Pick new parallel line", reset_parallel1, NULL },
    { NULL, NULL, NULL, NULL }
};

const MENU multi_hint_menu[] =
{
    { "Edit hint from marker A to B", NULL, edit_hint_data, NULL },
	{ "Width", "Stay within maxpath width", stay_within_width2, NULL },
	{ "Get_on", "Try to get back to maxpath", get_on_maxpath2, NULL },
	{ "Ease_on", "Ease back onto maxpath", ease_on_maxpath2, NULL },
	{ "Stay_on", "Stay on maxpath", stay_on_maxpath2, NULL },
	{ "Parallel", "Stay parallel to maxpath", stay_parallel_to_maxpath2, NULL },
	{ "Reset_Par", "Pick new parallel line", reset_parallel2, NULL },
    { NULL, NULL, NULL, NULL }
};

const MENU edit_hint_menu[] =
{
    { "Edit hint menu", NULL, edit_hint_data, NULL },
	{ "Single", "Edit hint on current point", NULL, (MENU *)single_hint_menu },
	{ "Multi", "Edit hints from marker A to B", NULL, (MENU *)multi_hint_menu },
    { NULL, NULL, NULL, NULL }
};

const MENU more_options_menu[] =
{
    { "Edit hint data menu / Select mpath set", NULL, edit_hint_data, NULL },
	{ "MP_set", "Select maxpath point set to edit", NULL, (MENU *)set_maxpath_menu },
	{ "De-act", "De-activate current set", deactivate_mpath_set, NULL },
	{ "Activ", "Activate any set currently de-activated", NULL, (MENU *)activate_maxpath_menu },
	{ "Loop", "Set loop data (Mrk_A=start, Mrk_B=end)", set_loop_data, NULL },
	{ "P_Copy", "Copy full path from another mpath set", NULL, (MENU *)copy_maxpath_menu },
	{ "Priority", "Set track priorities", NULL, (MENU *)priority_menu },
	{ "Weights", "Update weights for each drone", NULL, (MENU *)weight_menu },
	{ "Hints", "Edit hint data", NULL, (MENU *)edit_hint_menu },
    { NULL, NULL, NULL, NULL }
};

const MENU edit_maxpath_menu[] =
{
    { "Edit maxpath data menu", NULL, edit_maxpath_points, NULL },
    { "Fwrd", "Move to next maxpath point", next_maxpath_point, NULL },
    { "Bkwd", "Move to previous maxpath point", prev_maxpath_point, NULL },
	{ "Curv", "Curve marked maxpath points (toggle)", curve_maxpath_point, NULL },
	{ "Line", "Make marker A to B into a straight line", straighten_maxpath, NULL },
	{ "Move", "Move marked maxpath points (toggle)", move_maxpath_point, NULL },
	{ "MrkA", "Set marker A to current maxpath point", set_marker_a, NULL },
	{ "MrkB", "Set marker B to current maxpath point", set_marker_b, NULL },
	{ "Copy", "Copy from marker_A:B to current mpath point", copy_mpath_section, NULL },
	{ "Ins", "Insert from marker_A:B to current mpath point", insert_mpath_section, NULL },
	{ "Del", "Delete from marker_A to marker_B", delete_mpath_section, NULL },
	{ "Cancl", "Cancel current move/curve", cancel_move_curve, NULL },
	{ "More", "Edit maxpath hint data / Select mpath set", NULL, (MENU *)more_options_menu },
    { NULL, NULL, NULL, NULL }
};

const MENU maxpath_menu[] =
{
    { "Max Path menu", NULL, display_maxpath_points, NULL },
    { "Forward", "Move up maxpath points list", fwd_maxpath_points, NULL },
    { "Backward", "Move down maxpath points list", back_maxpath_points, NULL },
	{ "Edit_path", "Edit maxpath data", NULL, (MENU *)edit_maxpath_menu },
	{ "Save_Path", "Save path file to hard drive", write_cur_path_to_hd, NULL },
	{ "Save_Rec", "Save recorded path file to hard drive", write_drone_path_to_hd, NULL },
	{ "Link", "Link active paths to base path", create_maxpath_links, NULL },
    { NULL, NULL, NULL, NULL }
};

/*---------------------------------------------------------------------------*/

const MENU main_menu[] =
{
	{ "Main Menu", NULL, NULL, NULL },
	{ "Check points", "Add/delete/edit checkpoints", NULL, (MENU *)chkpoint_menu },
	{ "Path points", "View path points", NULL, (MENU *)path_menu },
	{ "Max Path", "View maxpath data", NULL, (MENU *)maxpath_menu },
    { NULL, NULL, NULL, NULL }
};
