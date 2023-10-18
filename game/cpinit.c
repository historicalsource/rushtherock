/*
 *	cpinit.c - initial placement points and checkpoint data
 *
 *	Copyright 1996 Time Warner Interactive
 *	Unauthorized reproduction, adaptation, distribution, performance or 
 *	display of this computer program or the associated audiovisual work
 *	is strictly prohibited.
 *
 *	======================================================================
 *	$Author: grjost $						$Date: 1997/10/15 21:50:53 $
 *	$Revision: 3.38 $						$Locker:  $
 *	======================================================================
 *	Change Log:
 *
 *	$Log: cpinit.c,v $
 * Revision 3.38  1997/10/15  21:50:53  grjost
 * Scaled track1 again
 *
 * Revision 3.37  1997/10/15  21:15:45  grjost
 * Changed track 1 finish line.
 *
 * Revision 3.36  1997/10/15  18:56:25  grjost
 * Fixed track 1 checkpoints
 *
 * Revision 3.35  1997/10/14  21:54:43  grjost
 * Shaved checkpoint time off of track 2.
 *
 * Revision 3.34  1997/10/14  20:04:59  grjost
 * Changed track 4 difficulty times
 *
 * Revision 3.33  1997/10/14  19:18:22  grjost
 * Changed easy times
 *
 * Revision 3.32  1997/10/14  18:20:29  grjost
 * Changed checkpoint times
 *
 * Revision 3.31  1997/09/27  01:00:41  grjost
 * Fixed track 7 checkpoints
 *
 * Revision 3.30  1997/09/18  22:36:20  grjost
 * Fixed CP times for track 7
 *
 * Revision 3.29  1997/09/17  19:35:33  grjost
 * Changed checkpoint lap times for T5 & T6
 *
 * Revision 3.28  1997/09/16  22:02:01  grjost
 * Fixed track 6.
 *
 * Revision 3.27  1997/09/15  22:27:35  gray
 * Removed trackset hack - trackno now 0 to NTRACKS.
 *
 * Revision 3.26  1997/09/03  00:57:21  gray
 * Fixed Alcatraz cps so finish is lowest logical cp #.
 *
 * Revision 3.25  1997/09/02  00:16:57  gray
 * Extra checkpoint and start 'tail' for Alcatraz. Better T4 cp times.
 *
 * Revision 3.24  1997/08/29  02:57:26  gray
 * Different finish line on Alcatraz.
 *
 * Revision 3.23  1997/08/23  19:55:43  gray
 * Fixed T8 (consumer alcatraz) checkpoints.
 *
 * Revision 3.22  1997/08/20  01:46:02  gray
 * Put modified alcatraz (plus backward) in track 8 for consumer.
 *
 * Revision 3.21  1997/08/18  20:15:14  gray
 * New Alcatraz checkpoints.
 *
 * Revision 3.20  1997/07/29  02:40:10  gray
 * More consumer stuff.
 *
 * Revision 3.19  1997/07/27  22:29:14  gray
 * Checkpoint set select for consumer track dev.
 *
 * Revision 3.18  1997/07/23  02:36:20  gray
 * New cps for t567 and 8.
 *
 * Revision 3.17  1997/07/02  18:21:01  gray
 * Made t7 and t8 recrunched alcatraz (as placeholders).
 *
 * Revision 3.16  1997/07/01  18:58:32  gray
 * Made alternate tracks (additional 4 tracks) view 1 selectable.
 *
 * Revision 3.15  1997/06/28  01:40:05  gray
 * Extended to 4 tracks.
 *
 * Revision 3.14  1997/01/30  20:32:39  geraci
 * four more difficulty settings
 *
 * Revision 3.13  1997/01/25  01:24:20  geraci
 * new checkpoint scheme
 *
 * Revision 3.12  1996/11/18  10:21:52  geraci
 * tournemant finish line checkpoint now has time
 *
 * Revision 3.11  1996/11/14  12:54:36  geraci
 * checkpoint 3 and 4 moved on track 3
 *
 * Revision 3.10  1996/11/11  22:07:05  geraci
 * checkpoint 3 on track 1 moved to new spot
 *
 * Revision 3.9  1996/11/08  07:15:38  geraci
 * new checkpoint 3 on track 1
 *
 * Revision 3.8  1996/11/07  14:33:56  geraci
 * latest and greatest (cp 1 on track 1 moved)
 *
 * Revision 3.7  1996/10/24  11:41:19  geraci
 * fixed track 1's number of laps
 *
 * Revision 3.6  1996/10/18  08:38:41  geraci
 * track 2 checkpoint on top of hills coords changed
 *
 * Revision 3.5  1996/10/16  09:40:23  geraci
 * moved last checkpoint in track 2 to next path point
 *
 * Revision 3.4  1996/10/04  07:43:26  geraci
 * tweaked
 *
 * Revision 3.3  1996/10/03  06:36:26  geraci
 * checkpoints are tweaked some more
 *
 * Revision 3.1  1996/09/09  02:24:52  gray
 * JAMMA version
 *
 * Revision 2.7  1996/08/31  22:37:36  geraci
 * new track 1 times
 *
 * Revision 2.6  1996/08/30  16:25:52  geraci
 * track 1 reduced to 2 laps
 *
 * Revision 2.5  1996/08/23  01:42:50  geraci
 * number of laps for race in data
 *
 * Revision 2.4  1996/08/21  09:13:31  geraci
 * new track tuning
 *
 * Revision 2.3  1996/08/20  05:14:53  geraci
 * new track times
 *
 * Revision 2.2  1996/05/16  01:21:09  geraci
 * extended radius for checkpoint 8 on track 3
 *
 * Revision 2.1  1996/05/04  02:56:18  petrick
 * Focus 2 release
 *
 * Revision 1.18  1996/04/12  01:05:41  geraci
 * made last checkpoint time 5 secs instead of zero
 *
 * Revision 1.17  1996/04/05  03:44:02  geraci
 * tweak track 3 checkpoint times
 *
 * Revision 1.16  1996/04/02  16:21:05  geraci
 * changed lap definition back again but added lap to flag field
 *
 * Revision 1.15  1996/04/01  13:54:31  geraci
 * changed lap count method
 *
 * Revision 1.14  1996/03/28  02:15:55  geraci
 * moved checkpoint 3 on expert track
 *
 * Revision 1.13  1996/03/25  23:43:40  geraci
 * tuned more checkpoints
 *
 * Revision 1.12  1996/03/21  03:45:20  geraci
 * more tuning
 *
 * Revision 1.11  1996/03/20  03:47:27  geraci
 * supports two checkpoint bonus times and initial time for track
 *
 * Revision 1.10  1996/03/13  18:36:01  gray
 * Track times too short with catchup. Added 5 sec to each cp.
 *
 * Revision 1.9  1996/03/12  10:01:06  geraci
 * moved checkpoint 5 on track 1
 *
 * Revision 1.8  1996/03/12  07:41:35  geraci
 * fixed track 1 finish line checkpoint
 *
 * Revision 1.7  1996/03/09  08:05:15  geraci
 * moved checkpoint 5 on track 1
 *
 * Revision 1.6  1996/03/08  07:48:30  geraci
 * new checkpoint moob
 *
 * Revision 1.5  1996/03/01  21:20:13  geraci
 * tuned checkpoints
 *
 * Revision 1.4  1996/03/01  02:10:08  geraci
 * player_model and drone_model are now model
 *
 * Revision 1.3  1996/02/28  21:33:00  geraci
 * fixed radius on track 1 shortcut
 *
 * Revision 1.2  1996/02/22  03:38:52  geraci
 * deleted invalid checkpoint from track 1
 *
 * Revision 1.1  1996/02/02  05:33:36  geraci
 * Initial revision
 *
 *
 *	======================================================================
 */

#define CHKPOINT_DATA

#include "globals.h"
#include "checkpoint.h"

/* track 1 checkpoint list */
const InitCheckPoint InitTrack0CPs[] =
{
	{	/* Starting position */
		{-1630, 276, 5972},			/* pos[3] */
		250000,						/* radius (in sq. feet) */
		0,							/* logical checkpoint */
		{0, 0}						/* best times */
	},
	{	/* checkpoint 1 */
		{-1621, 78, 2640},			/* pos[3] */
		250000,						/* radius (in sq. feet) */
		1,							/* logical checkpoint */
		{20, 0}						/* best times */
	},	
	{	/* checkpoint 2 */
		{-1988, 83, -1037},			/* pos[3] */
		250000,						/* radius (in sq. feet) */
		2,							/* logical checkpoint */
		{17, 17}					/* best times */
	},
	{	/* checkpoint 3 */
		{-1679, 12, -2655},			/* pos[3] */
		250000,						/* radius (in sq. feet) */
		3,							/* logical checkpoint */
		{17, 17}					/* best times */
	},
	{	/* checkpoint 4 (finish line) */
		{1389, -116, 670},			/* pos[3] */
		250000,						/* radius (in sq. feet) */
		0,							/* logical checkpoint */
		{19, 19}   					/* best times */
	},
	{	/* checkpoint 5 */
		{-491, -14, 1514},			/* pos[3] */
		250000,						/* radius (in sq. feet) */
		1,							/* logical checkpoint */
		{0, 17}	    				/* best times */
	},
	{	/* null checkpoint */
		{0, 0, 0}, 0, -1, {0, 0}	/* -1 = end of checkpoint list */
	}
	
};

/* track 1 backward checkpoint list */
const InitCheckPoint InitTrack0BCPs[] =
{
	{	/* checkpoint 0 (finish line) */
		{1389, -116, 670},				/* pos[3] */
		10000,						/* radius (in sq. feet) */
		0,							/* logical checkpoint */
		{16, 16}					/* best times */
	},
	{	/* checkpoint  1 */
		{-1679, 12, -2655},			/* pos[3] */
		10000,						/* radius (in sq. feet) */
		1,							/* logical checkpoint */
		{17, 12}					/* best times */
	},
	{	/* checkpoint  2 */
		{-1988, 83, -1037},			/* pos[3] */
		10000,						/* radius (in sq. feet) */
		2,								/* logical checkpoint */
		{20, 20}					/* best times */
	},
	{	/* checkpoint 3 */
		{-491, -14, 1514},			/* pos[3] */
		10000,						/* radius (in sq. feet) */
		3,							/* logical checkpoint */
		{11, 11}					/* best times */
	},
	{	/* null checkpoint */
		{0, 0, 0}, 0, -1, {0, 0}	/* -1 = end of checkpoint list */
	}
};

/* track 2 checkpoint list */
const InitCheckPoint InitTrack1CPs[] =
{
	{	/* checkpoint 0 (finish line) */
		{-420, 0, -300},			/* pos[3] */
		250000,						/* radius (in sq. feet) */
		0,							/* logical checkpoint */
		{18, 18}					/* best times */
	},
	{	/* checkpoint 1 */
		{1140, 143, 2240},			/* pos[3] */
		250000,						/* radius (in sq. feet) */
		1,							/* logical checkpoint */
		{22, 17}					/* best times */
	},
	{	/* checkpoint 2 */
		{720, 0, 4320},				/* pos[3] */
		250000,						/* radius (in sq. feet) */
		2,							/* logical checkpoint */
		{11, 11}					/* best times */
	},
	{	/* checkpoint 3 */
		{-3183, 50, 2899},			/* pos[3] */
		250000,						/* radius (in sq. feet) */
		3,							/* logical checkpoint */
		{18, 18}					/* best times */
	},
	{	/* checkpoint 4 */
		{-5605, 1, 1690},			/* pos[3] */
		250000,						/* radius (in sq. feet) */
		4,							/* logical checkpoint */
		{17, 17}					/* best times */
	},
	{	/* checkpoint 5 */
		{-4176, 0, -197},			/* pos[3] */
		810000,						/* radius (in sq. feet) */
		5,							/* logical checkpoint */
		{16, 16}					/* best times */
	},
	{	/* null checkpoint */
		{0, 0, 0}, 0, -1, {0, 0}	/* -1 = end of checkpoint list */
	}
};

/* track 2 backward checkpoint list */
const InitCheckPoint InitTrack1BCPs[] =
{
	{	/* checkpoint 0 (finish line) */
		{-420, 0, -300},			/* pos[3] */
		250000,						/* radius (in sq. feet) */
		0,							/* logical checkpoint */
		{19, 19}					/* best times */
	},
	{	/* checkpoint 1 */
		{-4176, 0, -197},			/* pos[3] */
		810000,						/* radius (in sq. feet) */
		1,							/* logical checkpoint */
		{16, 16}					/* best times */
	},
	{	/* checkpoint 2 */
		{-5605, 1, 1690},			/* pos[3] */
		250000,						/* radius (in sq. feet) */
		2,							/* logical checkpoint */
		{17, 17}					/* best times */
	},
	{	/* checkpoint 3 */
		{-3183, 50, 2899},			/* pos[3] */
		250000,						/* radius (in sq. feet) */
		3,							/* logical checkpoint */
		{19, 19}					/* best times */
	},
	{	/* checkpoint 4 */
		{720, 0, 4320},				/* pos[3] */
		250000,						/* radius (in sq. feet) */
		4,							/* logical checkpoint */
		{11, 11}					/* best times */
	},
	{	/* checkpoint 5 */
		{1140, 143, 2240},			/* pos[3] */
		250000,						/* radius (in sq. feet) */
		5,							/* logical checkpoint */
		{23, 17}					/* best times */
	},
	{	/* null checkpoint */
		{0, 0, 0}, 0, -1, {0, 0}	/* -1 = end of checkpoint list */
	}
};

/* track 3 checkpoint list */
const InitCheckPoint InitTrack2CPs[] =
{
	{	/* checkpoint 0 (finish line) */
		{-3067, 0, 0},				/* pos[3] */
		250000,						/* radius (in sq. feet) */
		0,							/* logical checkpoint */
		{16, 16}					/* best times */
	},
	{	/* checkpoint  1 */
		{-5419, -31, 67},			/* pos[3] */
		250000,						/* radius (in sq. feet) */
		1,							/* logical checkpoint */
		{17, 12}					/* best times */
	},
	{	/* checkpoint  2 */
		{-7111, 198, 2918},			/* pos[3] */
		250000,						/* radius (in sq. feet) */
		2,								/* logical checkpoint */
		{20, 20}					/* best times */
	},
	{	/* checkpoint 3 */
		{-7924, 173, 2962},			/* pos[3] */
		250000,						/* radius (in sq. feet) */
		3,							/* logical checkpoint */
		{11, 11}					/* best times */
	},
	{	/* checkpoint 4 */
		{-6376, 140, 575},			/* pos[3] */
		250000,						/* radius (in sq. feet) */
		4,							/* logical checkpoint */
		{24, 24}					/* best times */
	},
	{	/* checkpoint 5 */
		{-4936, 61, 524},			/* pos[3] */
		250000,						/* radius (in sq. feet) */
		5,							/* logical checkpoint */
		{21, 21}					/* best times */
	},
	{	/* checkpoint 6 */
		{-3888, 227, 2680},			/* pos[3] */
		250000,						/* radius (in sq. feet) */
		6,							/* logical checkpoint */
		{13, 13}					/* best times */
	},
	{	/* checkpoint 7 */
		{-1661, 258, 3104},			/* pos[3] */
		1000000,					/* radius (in sq. feet) */
		7,							/* logical checkpoint */
		{19, 19}					/* best times */
	},
	{	/* checkpoint 8 */
		{-2527, 290, 1359},			/* pos[3] */
		250000,						/* radius (in sq. feet) */
		8,							/* logical checkpoint */
		{13, 13}					/* best times */
	},
	{	/* null checkpoint */
		{0, 0, 0}, 0, -1, {0, 0}	/* -1 = end of checkpoint list */
	}
};

/* track 3 backward checkpoint list */
const InitCheckPoint InitTrack2BCPs[] =
{
	{	/* checkpoint 0 (finish line) */
		{-3067, 0, 0},				/* pos[3] */
		250000,						/* radius (in sq. feet) */
		0,							/* logical checkpoint */
		{16, 16}					/* best times */
	},
	{	/* checkpoint 1 */
		{-2527, 290, 1359},			/* pos[3] */
		250000,						/* radius (in sq. feet) */
		1,							/* logical checkpoint */
		{13, 13}					/* best times */
	},
	{	/* checkpoint 2 */
		{-1661, 258, 3104},			/* pos[3] */
		1000000,					/* radius (in sq. feet) */
		2,							/* logical checkpoint */
		{19, 19}					/* best times */
	},
	{	/* checkpoint 3 */
		{-3888, 227, 2680},			/* pos[3] */
		250000,						/* radius (in sq. feet) */
		3,							/* logical checkpoint */
		{13, 13}					/* best times */
	},
	{	/* checkpoint 4 */
		{-4936, 61, 524},			/* pos[3] */
		250000,						/* radius (in sq. feet) */
		4,							/* logical checkpoint */
		{21, 21}					/* best times */
	},
	{	/* checkpoint 5 */
		{-6376, 140, 575},			/* pos[3] */
		250000,						/* radius (in sq. feet) */
		5,							/* logical checkpoint */
		{24, 24}					/* best times */
	},
	{	/* checkpoint 6 */
		{-7924, 173, 2962},			/* pos[3] */
		250000,						/* radius (in sq. feet) */
		6,							/* logical checkpoint */
		{11, 11}					/* best times */
	},
	{	/* checkpoint  7 */
		{-7111, 198, 2918},			/* pos[3] */
		250000,						/* radius (in sq. feet) */
		7,								/* logical checkpoint */
		{20, 20}					/* best times */
	},
	{	/* checkpoint  8 */
		{-5419, -31, 67},			/* pos[3] */
		250000,						/* radius (in sq. feet) */
		8,							/* logical checkpoint */
		{17, 12}					/* best times */
	},
	{	/* null checkpoint */
		{0, 0, 0}, 0, -1, {0, 0}	/* -1 = end of checkpoint list */
	}
};

/* track 4 checkpoint list */
const InitCheckPoint InitTrack3CPs[] =
{
	{	/* checkpoint 0 (starting position) */
		{701, 112, -838},				/* pos[3] */
		76000,						/* radius (in sq. feet) */
		6,							/* logical checkpoint */
		{0, 0}					/* best times */
	},
	{	/* checkpoint  1 (finish line) */
		{517, 160, 446},			/* pos[3] */
		22500,						/* radius (in sq. feet) */
		0,							/* logical checkpoint */
		{12, 7}					/* best times */
	},
	{	/* checkpoint  2 */
		{-367, 350, 1705},			/* pos[3] */
		10000,						/* radius (in sq. feet) */
		1,							/* logical checkpoint */
		{9, 9}					/* best times */
	},
	{	/* checkpoint  3 */
		{-653, 276, -347},			/* pos[3] */
		12000,						/* radius (in sq. feet) */
		2,								/* logical checkpoint */
		{11, 11}					/* best times */
	},
	{	/* checkpoint 4 */
		{240, 373, -1487},			/* pos[3] */
		62000,						/* radius (in sq. feet) */
		3,							/* logical checkpoint */
		{11, 11}					/* best times */
	},
	{	/* checkpoint 5 */
		{-664, 48, 518},			/* pos[3] */
		16000,						/* radius (in sq. feet) */
		4,							/* logical checkpoint */
		{18, 18}					/* best times */
	},
	{	/* checkpoint 6 */
		{158, -17, -2163},			/* pos[3] */
		30000,						/* radius (in sq. feet) */
		5,							/* logical checkpoint */
		{13, 13}					/* best times */
	},
	{	/* checkpoint 7 */
		{701, 112, -838},				/* pos[3] */
		76000,						/* radius (in sq. feet) */
		6,							/* logical checkpoint */
		{9, 9}					/* best times */
	},
	{	/* null checkpoint */
		{0, 0, 0}, 0, -1, {0, 0}	/* -1 = end of checkpoint list */
	}
};

/* track 4 backward checkpoint list */
const InitCheckPoint InitTrack3BCPs[] =
{
	{	/* checkpoint 0 (finish line) */
		{701, 105, -838},				/* pos[3] */
		10000,						/* radius (in sq. feet) */
		0,							/* logical checkpoint */
		{16, 16}					/* best times */
	},
	{	/* checkpoint 1 */
		{211, -15, -2150},			/* pos[3] */
		10000,						/* radius (in sq. feet) */
		1,							/* logical checkpoint */
		{11, 11}					/* best times */
	},
	{	/* checkpoint 2 */
		{-726, 53, 328},			/* pos[3] */
		10000,						/* radius (in sq. feet) */
		2,							/* logical checkpoint */
		{11, 11}					/* best times */
	},
	{	/* checkpoint  3 */
		{194, 624, -826},			/* pos[3] */
		10000,						/* radius (in sq. feet) */
		3,								/* logical checkpoint */
		{20, 20}					/* best times */
	},
	{	/* checkpoint  4 */
		{-591, 332, 422},			/* pos[3] */
		10000,						/* radius (in sq. feet) */
		4,							/* logical checkpoint */
		{17, 12}					/* best times */
	},
	{	/* null checkpoint */
		{0, 0, 0}, 0, -1, {0, 0}	/* -1 = end of checkpoint list */
	}
};

/* track 5 checkpoint list */
const InitCheckPoint InitTrack4CPs[] =
{
	{	/* Starting position */
		{3690, 145, 258},			/* pos[3] */
		250000,						/* radius (in sq. feet) */
		0,							/* logical checkpoint */
		{18, 18}						/* best times */
	},
	{	/* checkpoint 1 */
		{2957, 253, 3340},			/* pos[3] */
		250000,						/* radius (in sq. feet) */
		1,							/* logical checkpoint */
		{22, 18}						/* best times */
	},	
	{	/* checkpoint 2 */
		{50, 235, 2578},			/* pos[3] */
		250000,						/* radius (in sq. feet) */
		2,							/* logical checkpoint */
		{22, 22}					/* best times */
	},
	{	/* checkpoint 3 */
		{ 200, 145, 258},			/* pos[3] */
		250000,						/* radius (in sq. feet) */
		3,							/* logical checkpoint */
		{23, 23}					/* best times */
	},
	{	/* null checkpoint */
		{0, 0, 0}, 0, -1, {0, 0}	/* -1 = end of checkpoint list */
	}
	
};

/* track 5 backward checkpoint list */
const InitCheckPoint InitTrack4BCPs[] =
{
	{	/* Starting position */
		{3690, 145, 258},			/* pos[3] */
		250000,						/* radius (in sq. feet) */
		0,							/* logical checkpoint */
		{40, 40}						/* best times */
	},
	{	/* checkpoint 1 */
		{ 200, 145, 258},			/* pos[3] */
		250000,						/* radius (in sq. feet) */
		1,							/* logical checkpoint */
		{40, 40}					/* best times */
	},
	{	/* checkpoint 2 */
		{50, 235, 2578},			/* pos[3] */
		250000,						/* radius (in sq. feet) */
		2,							/* logical checkpoint */
		{40, 40}					/* best times */
	},
	{	/* checkpoint 3 */
		{2957, 253, 3340},			/* pos[3] */
		250000,						/* radius (in sq. feet) */
		3,							/* logical checkpoint */
		{40, 40}						/* best times */
	},	
	{	/* null checkpoint */
		{0, 0, 0}, 0, -1, {0, 0}	/* -1 = end of checkpoint list */
	}
	
};

/* track 6 checkpoint list */
const InitCheckPoint InitTrack5CPs[] =
{
	{	/* Starting position */
		{2960, 65, -20},			/* pos[3] */
		250000,						/* radius (in sq. feet) */
		0,							/* logical checkpoint */
		{27, 27}						/* best times */
	},
	{	/* checkpoint 1 */
		{4591, 20, 2581},			/* pos[3] */
		250000,						/* radius (in sq. feet) */
		1,							/* logical checkpoint */
		{25, 20}						/* best times */
	},	
	{	/* checkpoint 2 */
		{1400, 350, 4458},			/* pos[3] */
		250000,						/* radius (in sq. feet) */
		2,							/* logical checkpoint */
		{30, 30}					/* best times */
	},
	{	/* checkpoint 3 */
		{ 0, 320, 1638},			/* pos[3] */
		250000,						/* radius (in sq. feet) */
		3,							/* logical checkpoint */
		{24, 24}					/* best times */
	},
	{	/* null checkpoint */
		{0, 0, 0}, 0, -1, {0, 0}	/* -1 = end of checkpoint list */
	}
	
};

/* track 6 backward checkpoint list */
const InitCheckPoint InitTrack5BCPs[] =
{
	{	/* Starting position */
		{2800, 65, -20},			/* pos[3] */
		250000,						/* radius (in sq. feet) */
		0,							/* logical checkpoint */
		{40, 40}						/* best times */
	},
	{	/* checkpoint 1 */
		{ 0, 320, 1638},			/* pos[3] */
		250000,						/* radius (in sq. feet) */
		1,							/* logical checkpoint */
		{40, 40}					/* best times */
	},
	{	/* checkpoint 2 */
		{1400, 350, 4458},			/* pos[3] */
		250000,						/* radius (in sq. feet) */
		2,							/* logical checkpoint */
		{40, 40}					/* best times */
	},
	{	/* checkpoint 3 */
		{5238, 30, 3098},			/* pos[3] */
		250000,						/* radius (in sq. feet) */
		3,							/* logical checkpoint */
		{40, 40}						/* best times */
	},	
	{	/* null checkpoint */
		{0, 0, 0}, 0, -1, {0, 0}	/* -1 = end of checkpoint list */
	}
	
};

/* track 7 checkpoint list */
const InitCheckPoint InitTrack6CPs[] =
{
	{	/* checkpoint 0 (finish line) */
		{0, 40, 0},					/* pos[3] */
		10000,						/* radius (in sq. feet) */
		0,							/* logical checkpoint */
		{20, 20}					/* best times */
	},
	{	/* checkpoint  1 */
		{0, 40, -2000},				/* pos[3] */
		10000,						/* radius (in sq. feet) */
		1,							/* logical checkpoint */
		{14, 10}					/* best times */
	},
	{	/* checkpoint  2 */
		{2458, 111, -5126},			/* pos[3] */
		10000,						/* radius (in sq. feet) */
		2,								/* logical checkpoint */
		{34, 34}					/* best times */
	},
	{	/* checkpoint 3 */
		{60, 40, -2000},			/* pos[3] */
		10000,						/* radius (in sq. feet) */
		3,							/* logical checkpoint */
		{25, 25}					/* best times */
	},
	{	/* checkpoint 4 */
		{60, 40, 0},			/* pos[3] */
		10000,						/* radius (in sq. feet) */
		4,							/* logical checkpoint */
		{12, 12}					/* best times */
	},
	{	/* checkpoint 5 */
		{1565, 380, 2201},			/* pos[3] */
		10000,						/* radius (in sq. feet) */
		5,							/* logical checkpoint */
		{31, 31}					/* best times */
	},
	{	/* null checkpoint */
		{0, 0, 0}, 0, -1, {0, 0}	/* -1 = end of checkpoint list */
	}
};

/* track 7 backward checkpoint list */
const InitCheckPoint InitTrack6BCPs[] =
{
	{	/* checkpoint 0 (finish line) */
		{0, 40, 0},				/* pos[3] */
		10000,						/* radius (in sq. feet) */
		0,							/* logical checkpoint */
		{36, 16}					/* best times */
	},
	{	/* checkpoint 1 */
		{1565, 380, 2201},			/* pos[3] */
		10000,						/* radius (in sq. feet) */
		1,							/* logical checkpoint */
		{31, 11}					/* best times */
	},
	{	/* checkpoint 2 */
		{60, 40, 0},			/* pos[3] */
		10000,						/* radius (in sq. feet) */
		2,							/* logical checkpoint */
		{31, 11}					/* best times */
	},
	{	/* checkpoint 3 */
		{60, 40, -3500},			/* pos[3] */
		10000,						/* radius (in sq. feet) */
		3,							/* logical checkpoint */
		{31, 11}					/* best times */
	},
	{	/* checkpoint  4 */
		{2135, 72, -7777},			/* pos[3] */
		10000,						/* radius (in sq. feet) */
		4,								/* logical checkpoint */
		{30, 20}					/* best times */
	},
	{	/* checkpoint  5 */
		{0, 40, -3500},			/* pos[3] */
		10000,						/* radius (in sq. feet) */
		5,							/* logical checkpoint */
		{37, 12}					/* best times */
	},
	{	/* null checkpoint */
		{0, 0, 0}, 0, -1, {0, 0}	/* -1 = end of checkpoint list */
	}
};

/* track 8 checkpoint list */
const InitCheckPoint InitTrack7CPs[] =
{
	{	/* checkpoint 0 (finish line) */
		{700, 215, -835},			/* pos[3] */
		76000,						/* radius (in sq. feet) */
		0,							/* logical checkpoint */
		{11, 11}					/* best times */
	},
	{	/* checkpoint 1 */
		{-368, 380, 1705},				/* pos[3] */
		10000,						/* radius (in sq. feet) */
		5,							/* logical checkpoint */
		{16, 16}					/* best times */
	},
	{	/* checkpoint 2 */
		{-653, 306, -347},			/* pos[3] */
		12000,						/* radius (in sq. feet) */
		4,							/* logical checkpoint */
		{17, 12}					/* best times */
	},
	{	/* checkpoint 3 */
		{240, 405, -1487},			/* pos[3] */
		62000,						/* radius (in sq. feet) */
		3,								/* logical checkpoint */
		{20, 20}					/* best times */
	},
	{	/* checkpoint 4 */
		{-664, 78, 518},			/* pos[3] */
		16000,						/* radius (in sq. feet) */
		2,							/* logical checkpoint */
		{11, 11}					/* best times */
	},
	{	/* checkpoint 5 */
		{159, 13, -2163},			/* pos[3] */
		30000,						/* radius (in sq. feet) */
		1,							/* logical checkpoint */
		{11, 11}					/* best times */
	},
	{	/* null checkpoint */
		{0, 0, 0}, 0, -1, {0, 0}	/* -1 = end of checkpoint list */
	}
};

/* track 8 backward checkpoint list */
const InitCheckPoint InitTrack7BCPs[] =
{
	{	/* checkpoint 0 (finish line) */
		{700, 215, -835},			/* pos[3] */
		76000,						/* radius (in sq. feet) */
		0,							/* logical checkpoint */
		{11, 11}					/* best times */
	},
	{	/* checkpoint 1 */
		{159, 13, -2163},			/* pos[3] */
		30000,						/* radius (in sq. feet) */
		1,							/* logical checkpoint */
		{11, 11}					/* best times */
	},
	{	/* checkpoint 2 */
		{-664, 78, 518},			/* pos[3] */
		16000,						/* radius (in sq. feet) */
		2,							/* logical checkpoint */
		{11, 11}					/* best times */
	},
	{	/* checkpoint 3 */
		{240, 405, -1487},			/* pos[3] */
		62000,						/* radius (in sq. feet) */
		3,								/* logical checkpoint */
		{20, 20}					/* best times */
	},
	{	/* checkpoint 4 */
		{-653, 306, -347},			/* pos[3] */
		12000,						/* radius (in sq. feet) */
		4,							/* logical checkpoint */
		{17, 12}					/* best times */
	},
	{	/* checkpoint 5 */
		{-368, 380, 1705},				/* pos[3] */
		10000,						/* radius (in sq. feet) */
		5,							/* logical checkpoint */
		{16, 16}					/* best times */
	},
	{	/* null checkpoint */
		{0, 0, 0}, 0, -1, {0, 0}	/* -1 = end of checkpoint list */
	}
};



/* pointer to checkpoints for different tracks */
/*InitCheckPoint * const InitCP_Track[MAX_TRACKS] =*/
InitCheckPoint * const InitCP_Track[MAX_TRACKS*2] =
{
/* normal set 1 (T1 - T4) */
	(InitCheckPoint * const) &InitTrack0CPs,
	(InitCheckPoint * const) &InitTrack1CPs,
	(InitCheckPoint * const) &InitTrack2CPs,
	(InitCheckPoint * const) &InitTrack3CPs,
/* normal set 2 (T5 - T8) */
	(InitCheckPoint * const) &InitTrack4CPs,
	(InitCheckPoint * const) &InitTrack5CPs,
	(InitCheckPoint * const) &InitTrack6CPs,
	(InitCheckPoint * const) &InitTrack7CPs,
/* backward set 2 (for consumer tracks only) */
	(InitCheckPoint * const) &InitTrack4BCPs,
	(InitCheckPoint * const) &InitTrack5BCPs,
	(InitCheckPoint * const) &InitTrack6BCPs,
	(InitCheckPoint * const) &InitTrack7BCPs,
/* backward set 1 (for consumer tracks only) */
	(InitCheckPoint * const) &InitTrack0BCPs,
	(InitCheckPoint * const) &InitTrack1BCPs,
	(InitCheckPoint * const) &InitTrack2BCPs,
	(InitCheckPoint * const) &InitTrack3BCPs
};

/*const Init_Track_Data init_track_data[MAX_TRACKS] =*/
const Init_Track_Data init_track_data[MAX_TRACKS*2] =
{
/* normal set 1 (T1 - T4) */
/*	 |------ start_time[diff] ------|,|-------- end_time[diff] ------|,loop, fin, b4fin */
	{{60, 60, 60, 60, 60, 60, 60, 60},{70, 50, 40, 30, 20, 15, 10,  5},   2,   4,   3},
	{{60, 60, 60, 60, 60, 60, 60, 60},{70, 50, 40, 30, 20, 15, 10,  5},   0,   0,   5},
	{{60, 60, 60, 60, 60, 60, 60, 60},{70, 50, 40, 30, 20, 15, 10,  5},   0,   0,   8},
	{{60, 60, 60, 60, 60, 60, 60, 60},{60, 40, 30, 20, 16, 12,  9,  5},   1,   1,   7},
/* normal set 2 (T5 - T8) */
	{{60, 60, 60, 60, 60, 60, 60, 60},{70, 50, 40, 30, 20, 15, 10,  5},   0,   0,   3},
	{{60, 60, 60, 60, 60, 60, 60, 60},{70, 50, 40, 30, 20, 15, 10,  5},   0,   0,   3},
	{{60, 60, 60, 60, 60, 60, 60, 60},{70, 50, 40, 30, 20, 15, 10,  5},   0,   0,   5},
	{{60, 60, 60, 60, 60, 60, 60, 60},{70, 50, 40, 30, 20, 15, 10,  5},   0,   0,   5},
/*backward set 2 (for consumer tracks only) */
	{{60, 60, 60, 60, 60, 60, 60, 60},{70, 50, 40, 30, 20, 15, 10,  5},   0,   0,   3},
	{{60, 60, 60, 60, 60, 60, 60, 60},{70, 50, 40, 30, 20, 15, 10,  5},   0,   0,   3},
	{{60, 60, 60, 60, 60, 60, 60, 60},{70, 50, 40, 30, 20, 15, 10,  5},   0,   0,   5},
	{{60, 60, 60, 60, 60, 60, 60, 60},{70, 50, 40, 30, 20, 15, 10,  5},   0,   0,   5},
/*backward set 1 (for consumer tracks only) */
	{{60, 60, 60, 60, 60, 60, 60, 60},{70, 50, 40, 30, 20, 15, 10,  5},   0,   0,   3},
	{{60, 60, 60, 60, 60, 60, 60, 60},{70, 50, 40, 30, 20, 15, 10,  5},   0,   0,   5},
	{{60, 60, 60, 60, 60, 60, 60, 60},{70, 50, 40, 30, 20, 15, 10,  5},   0,   0,   8},
	{{60, 60, 60, 60, 60, 60, 60, 60},{70, 50, 40, 30, 20, 15, 10,  5},   0,   0,   4},

};
