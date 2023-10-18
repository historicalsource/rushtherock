/*
 *	stats.c  non-guts statistics
 *
 *	Copyright 1996 Time Warner Interactive
 *	Unauthorized reproduction, adaptation, distribution, performance or 
 *	display of this computer program or the associated audiovisual work
 *	is strictly prohibited.
 *
 *	======================================================================
 *	$Author: grjost $						$Date: 1997/10/03 01:40:50 $
 *	$Revision: 3.13 $						$Locker:  $
 *	======================================================================
 *	Change Log:
 *
 *	$Log: stats.c,v $
 * Revision 3.13  1997/10/03  01:40:50  grjost
 * Fixed game logging
 *
 * Revision 3.12  1997/09/19  01:58:05  gray
 * Updated button instructions.
 *
 * Revision 3.11  1997/09/18  23:51:28  gray
 * Use GUTS function to get time/date from calender chip.
 *
 * Revision 3.10  1997/09/16  03:44:03  gray
 * Update for 8 tracks and limit to 4 car difficulties.
 *
 * Revision 3.9  1997/06/28  01:42:52  gray
 * Extended to 4 tracks.
 *
 * Revision 3.8  1996/11/28  04:17:17  petrick
 * Fixed free game counting bug.
 *
 * Revision 3.7  1996/11/23  23:10:07  gray
 * Update for free and continued games.
 *
 * Revision 3.6  1996/11/15  03:41:22  gray
 * Made game stats reset with guts stats. Force black background. Clean entry-exit.
 *
 * Revision 3.5  1996/11/14  13:23:51  petrick
 * Made stat a static so's not to interfere with stat() func.
 *
 * Revision 3.4  1996/10/23  00:14:33  petrick
 * Made track_no a global.
 *
 * Revision 3.3  1996/10/10  05:12:22  petrick
 * Made irqtime a true global in globals_h.
 *
 * Revision 3.2  1996/09/26  02:00:11  gray
 * Fixed the no-stats-for-10-seconds bug.
 *
 * Revision 3.1  1996/09/09  02:24:52  gray
 * JAMMA version
 *
 * Revision 1.7  1996/08/31  23:54:47  gray
 * Select option with percentages.
 *
 * Revision 1.6  1996/08/31  20:38:28  gray
 * rearrange order of stat pages.
 *
 * Revision 1.5  1996/08/31  05:57:42  gray
 * got number linked, finishes, hiscores, etc in.
 *
 * Revision 1.4  1996/08/30  19:48:42  gray
 * Select stats.
 *
 * Revision 1.3  1996/08/30  01:03:18  gray
 * Added time-of-day histogram, calender chip init, more run-time stats.
 *
 * Revision 1.2  1996/08/28  06:28:06  gray
 * NVRAM loading and saving. Record track, car, and transmission select times.
 *
 * Revision 1.1  1996/08/27  08:22:30  gray
 * Initial revision
 *
 *
 *	======================================================================
 */

#include <config.h>
#include <eer_defs.h>
#include <os_proto.h>
#include "attract.h"
#include "controls.h"
#include "debug.h"
#include "globals.h"
#include "stats.h"

extern S16 gThisCar;
extern BOOL gCarLocked;
extern S16 autotrans;
extern BOOL solo_flag;
extern int GetPICDateTime (U8 *DateTimeBuffer);

/*running stats on current game*/
GAMESTAT curstat;
/*structure that get read from and written to NVRAM*/
static STAT stat;

S32 gGameStats;
static S32 stat_track;
static S32 stat_scrn;
static BOOL stat_blank_screen;
char stat_str[80];

extern BOOL		gPlayingFree;	/*      Set if player has played a free game. */
extern BOOL     gContinuedGame;	/*      Set if player is playing a continued game. */

#define STAT_COMMON 0
#define STAT_TRACK1 1
#define STAT_TRACK2 2
#define STAT_TRACK3 3

#define STAT_COM_SCRNS 4
#define STAT_TRK_SCRNS 2

void init_game_stats(void)
{
	/* Reset game stats if GUTS stats have been reset. */
	if(eer_gets(EER_NEWCOINS) == 0)
		reset_gamestats();

	/*load STAT from NVRAM*/
	load_gamestats();

	/* tell GUTS about idle time */
	eer_play(0);
	/*init game stats state*/
	curstat.attract_mstime = IRQTIME;
	curstat.game_in_progress = 0;
	curstat.next_gstate = TRKSEL;
}

void game_stats(void)
{
	switch(gstate)
		{
		case ATTRACT:	stat_attract();	break;
		case TRKSEL:	stat_trksel();	break;
		case CARSEL:	stat_carsel();	break;
		case PREPLAY:	stat_preplay();	break;
		case PLAYGAME:	stat_playgame();break;
		case GAMEOVER:	stat_gameover();break;
		default:			break;
		}
}

void stat_attract(void)
{
	U32 save_mstime;

	curstat.game_in_progress = 0;
	curstat.next_gstate = TRKSEL;

	/*update GUTS and game stats every 10 seconds*/
	if((IRQTIME - curstat.attract_mstime) > 10000)
		{
		/* tell GUTS about idle time */
		eer_play(0);
		/*game stats*/
		curstat.attract_mstime = IRQTIME;
		stat.attract_secs += 10;	/*bump attract time by 10 seconds*/
		save_gamestats();			/*write STAT to NVRAM*/
		}
}

void stat_trksel(void)
{
	if(curstat.next_gstate == TRKSEL)
		{
		/* tell GUTS player 1 is starting a game */
		eer_play(1);
		/*game stats*/
		curstat.attract_mstime = IRQTIME - curstat.attract_mstime;	/*save delta ms*/
		curstat.trksel_mstime = IRQTIME;
		curstat.game_in_progress = 1;
		curstat.next_gstate = CARSEL;
		}
}

#define TRANSEL 123

void stat_carsel(void)
{
	if(curstat.next_gstate == CARSEL)
		{
		curstat.trk_num = (trackno < STAT_NUM_TRACKS) ? trackno : STAT_NUM_TRACKS - 1;
		curstat.trksel_mstime = IRQTIME - curstat.trksel_mstime;      /*save delta ms*/
		curstat.carsel_mstime = IRQTIME;	/*save start car select mstime*/
		curstat.next_gstate = TRANSEL;
		}
	else if((curstat.next_gstate == TRANSEL) && gCarLocked)
		{
		curstat.car_num = gThisCar % STAT_NUM_CARS;	/* 4 car difficulties. */
		curstat.carsel_mstime = IRQTIME - curstat.carsel_mstime;      /*save delta ms*/
		curstat.transel_mstime = IRQTIME;	/*save start transmission select mstime*/
		curstat.next_gstate = PREPLAY;
		}
}

void stat_preplay(void)
{
	if((curstat.next_gstate == PREPLAY) && curstat.game_in_progress)
		{
		curstat.autotrans = autotrans;
		curstat.transel_mstime = IRQTIME - curstat.transel_mstime;      /*save delta ms*/
		curstat.race_mstime = IRQTIME;	/*save race start mstime*/
		curstat.next_gstate = PLAYGAME;
		}
}

void stat_playgame(void)
{
	if((curstat.next_gstate == PLAYGAME) && curstat.game_in_progress)
		{
		curstat.next_gstate = GAMEOVER;
		}
	/*grab running stats (music,views,aborts,etc.)*/
}

void stat_gameover(void)
{
	S32 score,rank;

	if((curstat.next_gstate == GAMEOVER) && curstat.game_in_progress)
		{
		/* tell GUTS player 1 is ending a game */
		if(gPlayingFree)
			eer_end(0,2);	/*end player 1 free game time*/
		else if(gContinuedGame)
			eer_end(0,1);   /*end player 1 continued game time*/
		else
			eer_end(0,0);	/*end player 1 normal game time*/
		/*game stats*/
		curstat.race_mstime = IRQTIME - curstat.race_mstime;      /*save delta ms*/
		curstat.attract_mstime = IRQTIME;	/*save attract start mstime*/
		curstat.hiscore_mstime = 0; /*fill later*/
		score = game_car[gThisNode].score;
		rank = HiScoreRank( score, curstat.trk_num );
		curstat.finish = (score > 0) ? 1 : 0;
		curstat.hiscore = (rank > 0) ? 1 : 0;
		curstat.linked = (solo_flag == 0) ? 1: 0;
		curstat.next_gstate = ATTRACT;
		curstat.game_in_progress = 0;
		stat_update();
		}
}

void stat_update(void)
{
	TRKSTAT *tp;
	U32 time_bin;
	U16 day,hour;

	stat.attract_secs += ((curstat.attract_mstime + 500)/1000);
	stat_get_dayhour(&day,&hour);
	stat.tod_hist[hour][day]++;

	tp = &stat.trk[curstat.trk_num];
	tp->games++;
	tp->trksel_secs += ((curstat.trksel_mstime + 500)/1000);;
	tp->carsel_secs += ((curstat.carsel_mstime + 500)/1000);
	tp->transsel_secs += ((curstat.transel_mstime + 500)/1000);
	tp->race_secs += ((curstat.race_mstime + 500)/1000);;
	tp->hiscore_secs += ((curstat.hiscore_mstime + 500)/1000);;
	if(curstat.autotrans)
		tp->autotrans++;
	if(curstat.finish)
		tp->finish++;
	if(curstat.hiscore)
		tp->hiscore++;
	if(curstat.linked)
		tp->linked++;

	tp->car[curstat.car_num]++;
	time_bin = (curstat.race_mstime + 500)/(1000 * STAT_BIN_SECS);
	if(time_bin >= STAT_NUM_TIMES)
		time_bin = STAT_NUM_TIMES - 1;
	tp->tc_hist[time_bin][curstat.car_num]++;

	save_gamestats();
}

void reset_gamestats(void)
{
	TRKSTAT *tsp;
	S32 i,j,k;

	stat.attract_secs = 0;
	for(i=0;i<STAT_NUM_HOURS;i++)
		for(j=0;j<STAT_NUM_DAYS;j++)
			stat.tod_hist[i][j] = 0;
	for(i=0;i<STAT_NUM_TRACKS;i++)
		{
		tsp = &stat.trk[i];
		tsp->games = 0;

		tsp->trksel_secs = 0;
		tsp->carsel_secs = 0;
		tsp->transsel_secs = 0;
		tsp->race_secs = 0;
		tsp->hiscore_secs = 0;

		tsp->finish = 0;
		tsp->hiscore = 0;
		tsp->linked = 0;
		tsp->autotrans = 0;
		tsp->reverse = 0;
		tsp->abort = 0;
		tsp->crash = 0;
		for(j=0;j<STAT_NUM_CARS;j++)
			tsp->car[j] = 0;
		for(j=0;j<STAT_NUM_TUNES;j++)
			tsp->music_secs[j] = 0;
		for(j=0;j<STAT_NUM_VIEWS;j++)
			tsp->view_secs[j] = 0;
		for(j=0;j<STAT_NUM_DRONE_OPTIONS;j++)
			tsp->drone_opt[j] = 0;
		for(j=0;j<STAT_NUM_FORCE_OPTIONS;j++)
			tsp->force_opt[j] = 0;
		for(j=0;j<STAT_NUM_KETCHUP_OPTIONS;j++)
			tsp->ketchup_opt[j] = 0;
		for(j=0;j<STAT_NUM_TIMES;j++)
			for(k=0;k<STAT_NUM_CARS;k++)
				tsp->tc_hist[j][k] = 0;
		}
	save_gamestats();
}

/* offset into nvram (KB) */
#define STAT_BRAM_OFFSET 16
extern void eer_write_byte(U8 *where,U8 *unlock,int what);

void save_gamestats(void)
{
	U32 *nvp,i;
	U08 *cp;

	/*write STAT to NVRAM*/
	nvp = (U32 *)(BRAM_BASE + (1024 * STAT_BRAM_OFFSET * 4));
	cp = (U08 *)&stat;
	for(i=0;i<sizeof(STAT);i++)
		eer_write_byte( (U8 *)nvp++, (U8 *)BRAM_UNLK, *cp++ & 0xFF);
}

void load_gamestats(void)
{
	U32 *nvp,i;
	U08 *cp;

	/*read STAT from NVRAM*/
	nvp = (U32 *)(BRAM_BASE + (1024 * STAT_BRAM_OFFSET * 4));
	cp = (U08 *)&stat;
	for(i=0;i<sizeof(STAT);i++)
		*cp++ = *nvp++;
}

S32 ChkGameStats(void)
{
	S32 prev_track = stat_track;
	S32 prev_scrn = stat_scrn;
	static S32 lasttime;

	if((IRQTIME - lasttime) > 1000)
		gGameStats = 0;
	lasttime = IRQTIME;

	if(!gGameStats)
		{
		if(game_stat_seq())
			{
			gGameStats = 1;
			stat_track = STAT_COMMON;
			stat_scrn = 0;
			stat_blank_screen = 1;
			}
		}
	else if(edges & SW_ABORT)
		{
		gGameStats = 0;
		edges &= ~SW_ABORT;
		levels &= ~SW_ABORT;
		}
	else if (edges & SW_RADIO)
		{
		stat_scrn = (stat_track == STAT_COMMON) ? (stat_scrn + 1) % STAT_COM_SCRNS : 0;
		stat_track = STAT_COMMON;
		}
	else if (edges & (SW_VIEW1|SW_VIEW2|SW_VIEW3))
		{
#if 1
		if(edges & SW_VIEW1)
			stat_track--;
		else if(edges & SW_VIEW3)
			stat_track++;
		if(stat_track < STAT_TRACK1)
			stat_track = STAT_NUM_TRACKS;
		else if(stat_track > STAT_NUM_TRACKS)
			stat_track = STAT_TRACK1;
#else
		if(edges & SW_VIEW1) stat_track = STAT_TRACK1;
		else if(edges & SW_VIEW2) stat_track = STAT_TRACK2;
		else stat_track = STAT_TRACK3;

		if(prev_track == STAT_COMMON) stat_scrn = 0;
		else if(stat_track == prev_track) stat_scrn = (stat_scrn + 1) % STAT_TRK_SCRNS;
#endif
		}

	if((stat_track != prev_track) || (stat_scrn != prev_scrn))
		{
		stat_erase();
		stat_disp();
		}

	if(gGameStats || (levels & SW_RADIO))
		{
		/* if in game stats, clear bits so free game wont start. */
		edges &= ~(SW_VIEW1|SW_VIEW2|SW_VIEW3);
		levels &= ~(SW_VIEW1|SW_VIEW2|SW_VIEW3);
		}

	return(gGameStats);
}

/* Magic Game Stats Button Sequence: */
/* Hold music button for at least 1 second, */
/* While holding music button, press and hold view1. */
/* While holding music button, press and hold view2. */
/* While holding music button, press and hold view3. */
BOOL game_stat_seq(void)
{
	static S32 stat_button_state;
	static U32 button_time;
	S32 stat_levels,stat_edges;

	stat_levels = levels & (SW_RADIO|SW_VIEW1|SW_VIEW2|SW_VIEW3);
	stat_edges  = edges  & (SW_RADIO|SW_VIEW1|SW_VIEW2|SW_VIEW3);
	switch(stat_button_state)
		{
		case 0:	/* start sequence */
			if (stat_edges == SW_RADIO)
				{
				stat_button_state = 1;
				button_time = IRQTIME;
				}
			break;
		case 1:	/* press view1 */
			if (stat_levels  == SW_RADIO)
				stat_button_state = 1;
			else if((stat_levels == (SW_RADIO|SW_VIEW1)) && ((IRQTIME - button_time) > 1000))
				stat_button_state = 2;
			else
				stat_button_state = 0;
			break;
		case 2:	/* press view2 */
			if (stat_levels  == (SW_RADIO|SW_VIEW1))
				stat_button_state = 2;
			else if (stat_levels == (SW_RADIO|SW_VIEW1|SW_VIEW2))
				stat_button_state = 3;
			else
				stat_button_state = 0;
			break;
		case 3:	/* press view3 */
			if (stat_levels  == (SW_RADIO|SW_VIEW1|SW_VIEW2))
				stat_button_state = 3;
			else if (stat_levels == (SW_RADIO|SW_VIEW1|SW_VIEW2|SW_VIEW3))
				stat_button_state = 4;
			else
				stat_button_state = 0;
			break;
		case 4:	/* release view3*/
			stat_button_state = 0;
		}
	return(stat_button_state == 4);
}

void ShowGameStats(BOOL show)
{
	if(!show)
		{
		stat_erase();
		ZOID_EndFrame();
		return;
		}

	if(stat_blank_screen)
		ZOID_Reset();
	stat_blank_screen = 0;

	stat_disp();
	
}

void stat_erase(void)
{
	DBG_txt_clr(1,47);
}

void stat_disp(void)
{
	DBG_printf(24,1,RED_PAL,"GAME STATISTICS");
	DBG_printf( 0,46,RED_PAL," MUSIC BUTTON:GAMESxTIME-OF-DAY  VIEW BUTTONS:CYCLE TRACK STATS");
/*	DBG_printf( 0,46,RED_PAL,"MUSIC BUTTON CYCLES COMMON STATS, VIEW BUTTONS CYCLE TRACK STATS");*/
	DBG_printf(23,47,RED_PAL,"PRESS ABORT TO EXIT");
	if(stat_track == STAT_COMMON)
		{
#if 1
		HOURxDAY_hist();
#else
		if(stat_scrn == 0)	TIMExTRK_hist();
		else if(stat_scrn == 1)	HOURxDAY_hist();
		else if(stat_scrn == 2) TOO_MUCH_nums();
		else			SELECT_numbers();
#endif
		}
	else
		{
#if 1
		TIMExCAR_hist();
#else
		if(stat_scrn == 0)	TIMExCAR_hist();
		else if(stat_scrn == 0)	stat_track_numbers();
#endif
		}
}


#define COM_HIST_ROW 3
#define COM_HIST_COL 3

/* 0123456789112345678921234567893123456789412345678951234567896123 */
/*    01234567891123456789212345678931234567894123456789512345678   */
/*    -----------------------------------------------------------   */
/*    !    TIME   !   TOTAL  !   TRACK1     TRACK2     TRACK3   !   */
/*    -----------------------------------------------------------   */
/*    ! 0:00-0:00 !  123456  !   123456     123456     123456   !   */
/*    -----------------------------------------------------------   */
/*                  1234567      123456     123456     123456       */
/*                               100.0%     100.0%     100.0%       */
void TIMExTRK_hist(void)
{
	S32 i,j,k,txt_color;
	U32 num,grand_total,time_total,trk_total[STAT_NUM_CARS];
	U16 pct;

	DBG_printf(22,2,YEL_PALB,"ALL TRACKS - HISTOGRAM");
	TIMExTRK_template();

	for(grand_total=0,i=0;i<STAT_NUM_TIMES;i++)
		{
		txt_color = (i & 1) ? YEL_PALB : YEL_PAL;
		for(time_total=0,j=0;j<STAT_NUM_TRACKS;j++)
			{
			for(num=0,k=0;k<STAT_NUM_CARS;k++)
				num += stat.trk[j].tc_hist[i][k];
			trk_total[j] = (i == 0) ? num : trk_total[j] + num;
			time_total += num;
			grand_total += num;
			if(num > 0)
				DBG_printf(COM_HIST_COL+27+(j*11),COM_HIST_ROW+3+i,txt_color,"%6d",num);
			}
		if(time_total > 0)
			DBG_printf(COM_HIST_COL+15,COM_HIST_ROW+3+i,txt_color,"%6ld",time_total);
		}
	DBG_printf(COM_HIST_COL+14,COM_HIST_ROW+3+STAT_NUM_TIMES+1,YEL_PAL,"%7ld",grand_total);
	for(i=0;i<STAT_NUM_TRACKS;i++)
		{
		DBG_printf(COM_HIST_COL+27+(i*11),COM_HIST_ROW+3+STAT_NUM_TIMES+1,YEL_PAL,"%6ld",trk_total[i]);
		pct = (grand_total == 0) ? 0 : ((U32)trk_total[i] * 100)/grand_total;
		DBG_printf(COM_HIST_COL+29+(i*11),COM_HIST_ROW+3+STAT_NUM_TIMES+2,YEL_PAL,"%3d%%",pct);
		}
}

void TIMExTRK_template(void)
{
	S16 i,bseconds,eseconds,bmin,bsec,emin,esec;
	S32 txt_color;
	S16 row = COM_HIST_ROW;

	DBG_printf(COM_HIST_COL,row++,RED_PAL,"-----------------------------------------------------------");
	DBG_printf(COM_HIST_COL,row++,RED_PAL,"!    TIME   !   TOTAL  !   TRACK1     TRACK2     TRACK3   !");
	DBG_printf(COM_HIST_COL,row++,RED_PAL,"-----------------------------------------------------------");
	for(i=0;i<STAT_NUM_TIMES;i++)
		{
		bseconds = i * 15;
		eseconds = bseconds + 14;
		bmin = bseconds/60;
		bsec = bseconds - (bmin * 60);
		emin = eseconds/60;
		esec = eseconds - (emin * 60);
		txt_color = (i & 1) ? RED_PALB : RED_PAL;
		DBG_printf(COM_HIST_COL,row,txt_color,"!           !          !                                  !");
		txt_color = (i & 1) ? YEL_PALB : YEL_PAL;
		DBG_printf(COM_HIST_COL+2,row,txt_color,"%01d:%02d-%01d:%02d",bmin,bsec,emin,esec);
		DBG_printf(COM_HIST_COL,row,RED_PAL,"!");
		DBG_printf(COM_HIST_COL+58,row,RED_PAL,"!");
		row++;
		}
	DBG_printf(COM_HIST_COL,row++,RED_PAL,"-----------------------------------------------------------");
}

#define TOD_HIST_ROW 3
#define TOD_HIST_COL 3

/* 0123456789112345678921234567893123456789412345678951234567896123 */
/*    01234567891123456789212345678931234567894123456789512345678   */
/*    -----------------------------------------------------------   */
/*    ! TIME OF DAY !  MON   TUE   WED   THU   FRI   SAT   SUN  !   */
/*    -----------------------------------------------------------   */
/*    ! 00:00-00:59 ! 12345 12345 12345 12345 12345 12345 12345 !   */
/*    ! 01:00-01:59 ! 12345 12345 12345 12345 12345 12345 12345 !   */
/*    ! 23:00-23:59 ! 12345 12345 12345 12345 12345 12345 12345 !   */
/*    -----------------------------------------------------------   */
/*         123456     12345 12345 12345 12345 12345 12345 12345     */
/*                     100%  100%  100%  100%  100%  100%  100%     */

void HOURxDAY_hist(void)
{
	S32 i,j,txt_color;
	U32 num,grand_total,day_total[STAT_NUM_DAYS];
	U16 pct;

	DBG_printf(22,2,YEL_PALB,"TIMExDAY HISTOGRAM");
	HOURxDAY_template();

	for(grand_total=0,i=0;i<STAT_NUM_HOURS;i++)
		{
		txt_color = (i & 1) ? YEL_PALB : YEL_PAL;
		for(j=0;j<STAT_NUM_DAYS;j++)
			{
			num = stat.tod_hist[i][j];
			day_total[j] = (i == 0) ? num : day_total[j] + num;
			grand_total += num;
			if(num > 0)
				DBG_printf(TOD_HIST_COL+16+(j*6),TOD_HIST_ROW+3+i,txt_color,"%5ld",num);
			}
		}
	DBG_printf(TOD_HIST_COL+5,TOD_HIST_ROW+3+STAT_NUM_HOURS+1,YEL_PAL,"%6ld",grand_total);
	for(i=0;i<STAT_NUM_DAYS;i++)
		{
		DBG_printf(TOD_HIST_COL+16+(i*6),TOD_HIST_ROW+3+STAT_NUM_HOURS+1,YEL_PAL,"%5ld",day_total[i]);
		pct = (grand_total == 0) ? 0 : ((U32)day_total[i] * 100)/grand_total;
		DBG_printf(TOD_HIST_COL+17+(i*6),TOD_HIST_ROW+3+STAT_NUM_HOURS+2,YEL_PAL,"%3d%%",pct);
		}
}

void HOURxDAY_template(void)
{
	S16 i,bseconds,eseconds,bmin,bsec,emin,esec;
	S32 txt_color;
	S16 row = TOD_HIST_ROW;

	DBG_printf(TOD_HIST_COL,row++,RED_PAL,"-----------------------------------------------------------");
	DBG_printf(TOD_HIST_COL,row++,RED_PAL,"! TIME OF DAY !  MON   TUE   WED   THU   FRI   SAT   SUN  !");
	DBG_printf(TOD_HIST_COL,row++,RED_PAL,"-----------------------------------------------------------");
	for(i=0;i<STAT_NUM_HOURS;i++)
		{
		bseconds = i * 60;
		eseconds = bseconds + 59;
		bmin = bseconds/60;
		bsec = bseconds - (bmin * 60);
		emin = eseconds/60;
		esec = eseconds - (emin * 60);
		txt_color = (i & 1) ? RED_PALB : RED_PAL;
		DBG_printf(TOD_HIST_COL,row,txt_color,"!             !                                           !");
		txt_color = (i & 1) ? YEL_PALB : YEL_PAL;
		DBG_printf(TOD_HIST_COL+2,row,txt_color,"%02d:%02d-%02d:%02d",bmin,bsec,emin,esec);
		DBG_printf(TOD_HIST_COL,row,RED_PAL,"!");
		DBG_printf(TOD_HIST_COL+58,row,RED_PAL,"!");
		row++;
		}
	DBG_printf(TOD_HIST_COL,row++,RED_PAL,"-----------------------------------------------------------");
}

#define TRK_HIST_ROW 3
#define TRK_HIST_COL 3

/* 0123456789112345678921234567893123456789412345678951234567896123 */
/*    01234567891123456789212345678931234567894123456789512345678   */
/*    -----------------------------------------------------------   */
/*    !    TIME   ! TOTAL !  CAR1  CAR2  CAR3  CAR4  CAR5  CAR6 !   */
/*    -----------------------------------------------------------   */
/*    ! 0:00-0:00 ! 12345 ! 12345 12345 12345 12345 12345 12345 !   */
/*    -----------------------------------------------------------   */
/*                 123456   12345 12345 12345 12345 12345 12345     */
/*                           100%  100%  100%  100%  100%  100%     */

/* 0123456789112345678921234567893123456789412345678951234567896123 */
/*    01234567891123456789212345678931234567894123456789512345678   */
/*    -----------------------------------------------------------   */
/*    !    TIME   ! TOTAL ! CAR:BEG  CAR:ADV  CAR:EXP  CAR:EXT  !   */
/*    -----------------------------------------------------------   */
/*    ! 0:00-0:00 ! 12345 !  12345    12345    12345    12345   !   */
/*    -----------------------------------------------------------   */
/*                 123456   123456   123456   123456   123456       */
/*                            100%     100%     100%     100%       */
void TIMExCAR_hist(void)
{
	S32 i,j,txt_color;
	U32 grand_total,time_total,car_total[STAT_NUM_CARS];
	U16 num,pct;

	DBG_printf(22,2,YEL_PALB,"TRACK %ld - HISTOGRAM",stat_track);
	TIMExCAR_template();

	for(grand_total=0,i=0;i<STAT_NUM_TIMES;i++)
		{
		txt_color = (i & 1) ? YEL_PALB : YEL_PAL;
		for(time_total=0,j=0;j<STAT_NUM_CARS;j++)
			{
			num = stat.trk[stat_track-1].tc_hist[i][j];
			car_total[j] = (i == 0) ? num : car_total[j] + num;
			time_total += num;
			grand_total += num;
			if(num > 0)
				DBG_printf(TRK_HIST_COL+23+(j*9),TRK_HIST_ROW+3+i,txt_color,"%5d",num);
			}
		if(time_total > 0)
			DBG_printf(TRK_HIST_COL+14,TRK_HIST_ROW+3+i,txt_color,"%5ld",time_total);
		}
	DBG_printf(TRK_HIST_COL+13,TRK_HIST_ROW+3+STAT_NUM_TIMES+1,YEL_PAL,"%6ld",grand_total);
	for(i=0;i<STAT_NUM_CARS;i++)
		{
		DBG_printf(TRK_HIST_COL+22+(i*9),TRK_HIST_ROW+3+STAT_NUM_TIMES+1,YEL_PAL,"%6ld",car_total[i]);
		pct = (grand_total == 0) ? 0 : ((U32)car_total[i] * 100)/grand_total;
		DBG_printf(TRK_HIST_COL+24+(i*9),TRK_HIST_ROW+3+STAT_NUM_TIMES+2,YEL_PAL,"%3d%%",pct);
		}
}


void TIMExCAR_template(void)
{
	S16 i,bseconds,eseconds,bmin,bsec,emin,esec;
	S32 txt_color;
	S16 row = TRK_HIST_ROW;

	DBG_printf(TRK_HIST_COL,row++,RED_PAL,"-----------------------------------------------------------");
	DBG_printf(TRK_HIST_COL,row++,RED_PAL,"!    TIME   ! TOTAL ! CAR:BEG  CAR:ADV  CAR:EXP  CAR:EXT  !");
	DBG_printf(TRK_HIST_COL,row++,RED_PAL,"-----------------------------------------------------------");
	for(i=0;i<STAT_NUM_TIMES;i++)
		{
		bseconds = i * 15;
		eseconds = bseconds + 14;
		bmin = bseconds/60;
		bsec = bseconds - (bmin * 60);
		emin = eseconds/60;
		esec = eseconds - (emin * 60);
		txt_color = (i & 1) ? RED_PALB : RED_PAL;
		DBG_printf(TRK_HIST_COL,row,txt_color,"!           !       !                                     !");
		txt_color = (i & 1) ? YEL_PALB : YEL_PAL;
		DBG_printf(TRK_HIST_COL+2,row,txt_color,"%01d:%02d-%01d:%02d",bmin,bsec,emin,esec);
		DBG_printf(TRK_HIST_COL,row,RED_PAL,"!");
		DBG_printf(TRK_HIST_COL+58,row,RED_PAL,"!");
		row++;
		}
	DBG_printf(TRK_HIST_COL,row++,RED_PAL,"-----------------------------------------------------------");
}




#define NUM_ROW 3
#define NUM_COL 1

#define STAT_COM_NUM_ENTRIES 39
#define COM_LABEL_COL (NUM_COL+2)
#define COM_TOTAL_COL (NUM_COL+28)
#define COM_TRK1_COL (NUM_COL+36)
#define COM_TRK2_COL (NUM_COL+44)
#define COM_TRK3_COL (NUM_COL+52)

/* 0123456789112345678921234567893123456789412345678951234567896123 */
/*  012345678911234567892123456789312345678941234567895123456789612 */
/*  --------------------------------------------------------------  */
/*  !                          ! TOTAL ! TRACK1  TRACK2  TRACK3  !  */
/*  --------------------------------------------------------------  */
/*  ! Total Games Played       !123456 !123456  123456  123456   !  */
/*  !                          !       !                         !  */
/*  ! Total Minutes Played     !123456 !123456  123456  123456   !  */
/*  !                          !       !                         !  */
/*  ! Race Finishes            !123456 !123456  123456  123456   !  */
/*  ! Hi Score Enter           !123456 !123456  123456  123456   !  */
/*  ! Linked Games             !123456 !123456  123456  123456   !  */
/*  !                          !       !                         !  */
/*  ! Car Select: Car1         !123456 !123456  123456  123456   !  */
/*  ! Car Select: Car2         !123456 !123456  123456  123456   !  */
/*  ! Car Select: Car3         !123456 !123456  123456  123456   !  */
/*  ! Car Select: Car4         !123456 !123456  123456  123456   !  */
/*  ! Car Select: Car5         !123456 !123456  123456  123456   !  */
/*  ! Car Select: Car6         !123456 !123456  123456  123456   !  */
/*  !                          !       !                         !  */
/*  ! Trans. Select: Auto      !123456 !123456  123456  123456   !  */
/*  ! Trans. Select: Manual    !123456 !123456  123456  123456   !  */
/*  !                          !       !                         !  */
/*  ! Drone Option: On         !123456 !123456  123456  123456   !  */
/*  ! Drone Option: Off        !123456 !123456  123456  123456   !  */
/*  ! Drone Option: RacerX     !123456 !123456  123456  123456   !  */
/*  !                          !       !                         !  */
/*  ! Force Option: Regular    !123456 !123456  123456  123456   !  */
/*  ! Force Option: More       !123456 !123456  123456  123456   !  */
/*  ! Force Option: Full       !123456 !123456  123456  123456   !  */
/*  !                          !       !                         !  */
/*  ! Ketchup Option: On       !123456 !123456  123456  123456   !  */
/*  ! Ketchup Option: Off      !123456 !123456  123456  123456   !  */
/*  !                          !       !                         !  */
/*  ! Avg. Track Select (sec)  !  12.1 !  12.1    12.9    12.9   !  */
/*  ! Avg. Car   Select (sec)  !  12.1 !  12.1    12.9    12.9   !  */
/*  ! Avg. Trans Select (sec)  !  12.1 !  12.1    12.9    12.9   !  */
/*  ! Avg. Race Time    (sec)  ! 123.1 ! 123.1   123.9   123.9   !  */
/*  ! Avg. HiScore Time (sec)  !  12.1 !  12.1    12.9    12.9   !  */
/*  ! Avg. Total Time   (sec)  ! 123.1 ! 123.1   123.9   123.9   !  */
/*  !                          !       !                         !  */
/*  ! Fatal Crashes Per Game   !123456 !123456  123456  123456   !  */
/*  !                          !       !                         !  */
/*  ! Aborts Per Game          !123456 !123456  123456  123456   !  */
/*  --------------------------------------------------------------  */
void TOO_MUCH_nums(void)
{
	TRKSTAT *t1 = &stat.trk[0];
	TRKSTAT *t2 = &stat.trk[1];
	TRKSTAT *t3 = &stat.trk[2];
	S32 txt_color;
	U32 trk1,trk2,trk3;
	S16 i,j,row = NUM_ROW + 3;
	F32 invcnt,invt1cnt,invt2cnt,invt3cnt;
	F32 avg,t1avg,t2avg,t3avg;

	DBG_printf(22,2,YEL_PALB,"COMMON - SELECT STATS",stat_scrn);
	TOO_MUCH_template();

	stat_num_line(row++, "Total Games Played", t1->games, t2->games, t3->games);
	row++;

	stat_num_line(row++, "Total Minutes Played", t1->race_secs/60, t2->race_secs/60, t3->race_secs/60);
	row++;

	stat_num_line(row++, "Race Finishes", t1->finish, t2->finish, t3->finish);
	stat_num_line(row++, "Hi Scores Enter", t1->hiscore, t2->hiscore, t3->hiscore);
	stat_num_line(row, "Linked Games", t1->linked, t2->linked, t3->linked);
	row += 2;

	stat_num_line(row++, "Car Select: Car1", t1->car[0], t2->car[0], t3->car[0]);
	stat_num_line(row++, "Car Select: Car2", t1->car[1], t2->car[1], t3->car[1]);
	stat_num_line(row++, "Car Select: Car3", t1->car[2], t2->car[2], t3->car[2]);
	stat_num_line(row++, "Car Select: Car4", t1->car[3], t2->car[3], t3->car[3]);
	stat_num_line(row++, "Car Select: Car5", t1->car[4], t2->car[4], t3->car[4]);
	stat_num_line(row++, "Car Select: Car6", t1->car[5], t2->car[5], t3->car[5]);
	row++;

	trk1 = t1->autotrans; trk2 = t2->autotrans; trk3 = t3->autotrans;
	stat_num_line(row++, "Trans. Select: Auto", trk1, trk2, trk3);
	trk1 = t1->games - trk1; trk2 = t2->games - trk2; trk3 = t3->games - trk3;
	stat_num_line(row++, "Trans. Select: Manual", trk1, trk2, trk3);
	row++;

	stat_num_line(row++, "Drone Option: On", t1->drone_opt[0], t2->drone_opt[0], t3->drone_opt[0]);
	stat_num_line(row++, "Drone Option: Off", t1->drone_opt[1], t2->drone_opt[1], t3->drone_opt[1]);
	stat_num_line(row++, "Drone Option: RacerX", t1->drone_opt[2], t2->drone_opt[2], t3->drone_opt[2]);
	row++;

	stat_num_line(row++, "Force Option: Regular", t1->force_opt[0], t2->force_opt[0], t3->force_opt[0]);
	stat_num_line(row++, "Force Option: More", t1->force_opt[1], t2->force_opt[1], t3->force_opt[1]);
	stat_num_line(row++, "Force Option: Full", t1->force_opt[2], t2->force_opt[2], t3->force_opt[2]);
	row++;

	stat_num_line(row++, "Ketchup Option: On", t1->ketchup_opt[0], t2->ketchup_opt[0], t3->ketchup_opt[0]);
	stat_num_line(row++, "Ketchup Option: Off", t1->ketchup_opt[1], t2->ketchup_opt[1], t3->ketchup_opt[1]);
	row++;

	i = t1->games + t2->games + t3->games;
	invcnt = (i == 0) ? 1.0 : 1.0/(F32)i;
	invt1cnt = (t1->games == 0) ? 1.0 : 1.0/(F32)t1->games;
	invt2cnt = (t2->games == 0) ? 1.0 : 1.0/(F32)t2->games;
	invt3cnt = (t3->games == 0) ? 1.0 : 1.0/(F32)t3->games;

	avg = (F32)(t1->trksel_secs + t2->trksel_secs + t3->trksel_secs) * invcnt;
	t1avg = (F32)t1->trksel_secs * invt1cnt;
	t2avg = (F32)t2->trksel_secs * invt2cnt;
	t3avg = (F32)t3->trksel_secs * invt3cnt;
	stat_avg_line(row++, "Avg. Track Select (sec)", avg, t1avg, t2avg, t3avg);

	avg = (F32)(t1->carsel_secs + t2->carsel_secs + t3->carsel_secs) * invcnt;
	t1avg = (F32)t1->carsel_secs * invt1cnt;
	t2avg = (F32)t2->carsel_secs * invt2cnt;
	t3avg = (F32)t3->carsel_secs * invt3cnt;
	stat_avg_line(row++, "Avg. Car   Select (sec)", avg, t1avg, t2avg, t3avg);

	avg = (F32)(t1->transsel_secs + t2->transsel_secs + t3->transsel_secs) * invcnt;
	t1avg = (F32)t1->transsel_secs * invt1cnt;
	t2avg = (F32)t2->transsel_secs * invt2cnt;
	t3avg = (F32)t3->transsel_secs * invt3cnt;
	stat_avg_line(row++, "Avg. Trans Select (sec)", avg, t1avg, t2avg, t3avg);

	avg = (F32)(t1->race_secs + t2->race_secs + t3->race_secs) * invcnt;
	t1avg = (F32)t1->race_secs * invt1cnt;
	t2avg = (F32)t2->race_secs * invt2cnt;
	t3avg = (F32)t3->race_secs * invt3cnt;
	stat_avg_line(row++, "Avg. Race Time    (sec)", avg, t1avg, t2avg, t3avg);

	avg = (F32)(t1->hiscore_secs + t2->hiscore_secs + t3->hiscore_secs) * invcnt;
	t1avg = (F32)t1->hiscore_secs * invt1cnt;
	t2avg = (F32)t2->hiscore_secs * invt2cnt;
	t3avg = (F32)t3->hiscore_secs * invt3cnt;
	stat_avg_line(row++, "Avg. HiScore Time (sec)", avg, t1avg, t2avg, t3avg);

	t1avg = t1->trksel_secs + t1->carsel_secs + t1->transsel_secs + t1->race_secs + t1->hiscore_secs;
	t2avg = t2->trksel_secs + t2->carsel_secs + t2->transsel_secs + t2->race_secs + t2->hiscore_secs;
	t3avg = t3->trksel_secs + t3->carsel_secs + t3->transsel_secs + t3->race_secs + t3->hiscore_secs;
	avg = (t1avg + t2avg + t3avg) * invcnt;
	t1avg *= invt1cnt;
	t2avg *= invt2cnt;
	t3avg *= invt3cnt;
	stat_avg_line(row++, "Avg. Total Time   (sec)", avg, t1avg, t2avg, t3avg);
	row++;

	stat_num_line(row++, "Fatal Crashes Per Game", t1->crash, t2->crash, t3->crash);
	row++;

	stat_num_line(row++, "Aborts Per Game", t1->abort, t2->abort, t3->abort);
	row++;
}

void stat_num_line(S16 row, char *label, U32 t1val, U32 t2val, U32 t3val)
{
	S32 txt_color;
	U32 total;

	txt_color = (row & 1) ? YEL_PALB : YEL_PAL;
	total = t1val + t2val + t3val;
	DBG_printf(COM_LABEL_COL,row,txt_color,"%s",label);
	DBG_printf(COM_TOTAL_COL,row,txt_color,"%6ld",total);
	DBG_printf(COM_TRK1_COL,row,txt_color,"%6ld",t1val);
	DBG_printf(COM_TRK2_COL,row,txt_color,"%6ld",t2val);
	DBG_printf(COM_TRK3_COL,row,txt_color,"%6ld",t3val);
}

void stat_avg_line(S16 row, char *label, F32 avg, F32 t1avg, F32 t2avg, F32 t3avg)
{
	S32 txt_color;

	txt_color = (row & 1) ? YEL_PALB : YEL_PAL;
	DBG_printf(COM_LABEL_COL,row,txt_color,"%s",label);
	DBG_printf(COM_TOTAL_COL,row,txt_color,"%6.1f",avg);
	DBG_printf(COM_TRK1_COL,row,txt_color,"%6.1f",t1avg);
	DBG_printf(COM_TRK2_COL,row,txt_color,"%6.1f",t2avg);
	DBG_printf(COM_TRK3_COL,row,txt_color,"%6.1f",t3avg);
}

void TOO_MUCH_template(void)
{
	S32 txt_color;
	S16 i,row = NUM_ROW;

	DBG_printf(NUM_COL,row++,RED_PAL,"--------------------------------------------------------------");
	DBG_printf(NUM_COL,row++,RED_PAL,"!                          ! TOTAL ! TRACK1  TRACK2  TRACK3  !");
	DBG_printf(NUM_COL,row++,RED_PAL,"--------------------------------------------------------------");
	for(i=0;i<STAT_COM_NUM_ENTRIES;i++)
		{
		txt_color = (i & 1) ? RED_PALB : RED_PAL;
		DBG_printf(NUM_COL,row,txt_color,"!                          !       !                         !");
		DBG_printf(NUM_COL,row,RED_PAL,"!");
		DBG_printf(NUM_COL+61,row,RED_PAL,"!");
		row++;
		}
	DBG_printf(NUM_COL,row++,RED_PAL,"--------------------------------------------------------------");
}



U32 STAT_games,STAT_t1games,STAT_t2games,STAT_t3games;

#define SEL_NUM_COL 1
#define SEL_NUM_ROW 3
#define SEL_NUM_ENTRIES 29
#define SEL_NUM_LABEL_COL (SEL_NUM_COL+2)
#define SEL_NUM_TOTAL_COL (SEL_NUM_COL+15)
#define SEL_NUM_TRK1_COL (SEL_NUM_COL+27)
#define SEL_NUM_TRK2_COL (SEL_NUM_COL+38)
#define SEL_NUM_TRK3_COL (SEL_NUM_COL+49)

/* SELECT STATS */
/* 0123456789112345678921234567893123456789412345678951234567896123 */
/*  012345678911234567892123456789312345678941234567895123456789612 */
/*  --------------------------------------------------------------  */
/*  ! SELECT OPT. !   TOTAL   !   TRACK1     TRACK2     TRACK3   !  */
/*  --------------------------------------------------------------  */
/*  ! #Games      !  12345    !  12345      12345      12345     !  */
/*  !             !           !                                  !  */
/*  ! #Track1     !12345(100%)!12345(100%)12345(100%)12345(100%) !  */
/*  ! #Track2     !12345(100%)!12345(100%)12345(100%)12345(100%) !  */
/*  ! #Track3     !12345(100%)!12345(100%)12345(100%)12345(100%) !  */
/*  !             !           !                                  !  */
/*  ! #Linked     !12345(100%)!12345(100%)12345(100%)12345(100%) !  */
/*  ! #Solo       !12345(100%)!12345(100%)12345(100%)12345(100%) !  */
/*  !             !           !                                  !  */
/*  ! #Car1       !12345(100%)!12345(100%)12345(100%)12345(100%) !  */
/*  ! #Car2       !12345(100%)!12345(100%)12345(100%)12345(100%) !  */
/*  ! #Car3       !12345(100%)!12345(100%)12345(100%)12345(100%) !  */
/*  ! #Car4       !12345(100%)!12345(100%)12345(100%)12345(100%) !  */
/*  ! #Car5       !12345(100%)!12345(100%)12345(100%)12345(100%) !  */
/*  ! #Car6       !12345(100%)!12345(100%)12345(100%)12345(100%) !  */
/*  !             !           !                                  !  */
/*  ! #DronesOn   !12345(100%)!12345(100%)12345(100%)12345(100%) !  */
/*  ! #DronesOff  !12345(100%)!12345(100%)12345(100%)12345(100%) !  */
/*  ! #RacerX     !12345(100%)!12345(100%)12345(100%)12345(100%) !  */
/*  !             !           !                                  !  */
/*  ! #ForceReg   !12345(100%)!12345(100%)12345(100%)12345(100%) !  */
/*  ! #ForceMore  !12345(100%)!12345(100%)12345(100%)12345(100%) !  */
/*  ! #ForceFull  !12345(100%)!12345(100%)12345(100%)12345(100%) !  */
/*  !             !           !                                  !  */
/*  ! #CatchupOn  !12345(100%)!12345(100%)12345(100%)12345(100%) !  */
/*  ! #CatchupOff !12345(100%)!12345(100%)12345(100%)12345(100%) !  */
/*  !             !           !                                  !  */
/*  ! #Auto       !12345(100%)!12345(100%)12345(100%)12345(100%) !  */
/*  ! #Manual     !12345(100%)!12345(100%)12345(100%)12345(100%) !  */
/*  --------------------------------------------------------------  */
void SELECT_numbers(void)
{
	TRKSTAT *t1 = &stat.trk[0];
	TRKSTAT *t2 = &stat.trk[1];
	TRKSTAT *t3 = &stat.trk[2];
	S16 row = SEL_NUM_ROW + 5;
	U32 t1g,t2g,t3g;

	DBG_printf(22,2,YEL_PALB,"SELECT OPTION STATS",stat_scrn);
	SELECT_numbers_template();

	t1g = t1->games; t2g = t2->games; t3g = t3->games;
	SELECT_numbers_line(row++,"#Track1",t1g,0,0);
	SELECT_numbers_line(row++,"#Track2",0,t2g,0);
	SELECT_numbers_line(row++,"#Track3",0,0,t3g);
	row++;
	SELECT_numbers_line(row++,"#Linked",t1->linked,t2->linked,t3->linked);
	SELECT_numbers_line(row++,"#Solo"  ,t1g - t1->linked,t2g - t2->linked,t3g - t3->linked);
	row++;
	SELECT_numbers_line(row++,"#Car1",t1->car[0],t2->car[0],t3->car[0]);
	SELECT_numbers_line(row++,"#Car2",t1->car[1],t2->car[1],t3->car[1]);
	SELECT_numbers_line(row++,"#Car3",t1->car[2],t2->car[2],t3->car[2]);
	SELECT_numbers_line(row++,"#Car4",t1->car[3],t2->car[3],t3->car[3]);
	SELECT_numbers_line(row++,"#Car5",t1->car[4],t2->car[4],t3->car[4]);
	SELECT_numbers_line(row++,"#Car6",t1->car[5],t2->car[5],t3->car[5]);
	row++;
	SELECT_numbers_line(row++,"#DronesOn" ,t1->drone_opt[0],t2->drone_opt[0],t3->drone_opt[0]);
	SELECT_numbers_line(row++,"#DronesOff",t1->drone_opt[1],t2->drone_opt[1],t3->drone_opt[1]);
	SELECT_numbers_line(row++,"#DronesOn" ,t1->drone_opt[2],t2->drone_opt[2],t3->drone_opt[2]);
	row++;
	SELECT_numbers_line(row++,"#ForceReg" ,t1->force_opt[0],t2->force_opt[0],t3->force_opt[0]);
	SELECT_numbers_line(row++,"#ForceMore",t1->force_opt[1],t2->force_opt[1],t3->force_opt[1]);
	SELECT_numbers_line(row++,"#ForceFull",t1->force_opt[2],t2->force_opt[2],t3->force_opt[2]);
	row++;
	SELECT_numbers_line(row++,"#CatchupOn" ,t1->ketchup_opt[0],t2->ketchup_opt[0],t3->ketchup_opt[0]);
	SELECT_numbers_line(row++,"#CatchupOff",t1->ketchup_opt[1],t2->ketchup_opt[1],t3->ketchup_opt[1]);
	row++;
	SELECT_numbers_line(row++,"#Auto"  ,t1->autotrans,t2->autotrans,t3->autotrans);
	SELECT_numbers_line(row++,"#Manual",t1g - t1->autotrans,t2g - t2->autotrans,t3g - t3->autotrans);
}


void SELECT_numbers_line(S16 row, char *label, U32 t1val, U32 t2val, U32 t3val)
{
	S32 txt_color;
	U32 total,pct,t1pct,t2pct,t3pct;

	txt_color = (row & 1) ? YEL_PALB : YEL_PAL;
	total = t1val + t2val + t3val;
	pct = (STAT_games) ? (total * 100)/STAT_games : 0;
	t1pct = (STAT_t1games) ? (t1val * 100)/STAT_t1games : 0;
	t2pct = (STAT_t2games) ? (t2val * 100)/STAT_t2games : 0;
	t3pct = (STAT_t3games) ? (t3val * 100)/STAT_t3games : 0;
	DBG_printf(SEL_NUM_LABEL_COL,row,txt_color,"%s",label);
	DBG_printf(SEL_NUM_TOTAL_COL,row,txt_color,"%5ld(%ld%%)",total,pct);
	DBG_printf(SEL_NUM_TRK1_COL,row,txt_color,"%5ld(%ld%%)",t1val,t1pct);
	DBG_printf(SEL_NUM_TRK2_COL,row,txt_color,"%5ld(%ld%%)",t2val,t2pct);
	DBG_printf(SEL_NUM_TRK3_COL,row,txt_color,"%5ld(%ld%%)",t3val,t3pct);
}

void SELECT_numbers_template(void)
{
	S32 txt_color;
	S16 i,row = SEL_NUM_ROW;

	DBG_printf(SEL_NUM_COL,row++,RED_PAL,"--------------------------------------------------------------");
	DBG_printf(SEL_NUM_COL,row++,RED_PAL,"! SELECT OPT. !   TOTAL   !   TRACK1     TRACK2     TRACK3   !");
	DBG_printf(SEL_NUM_COL,row++,RED_PAL,"--------------------------------------------------------------");
	for(i=0;i<SEL_NUM_ENTRIES;i++)
		{
		txt_color = (i & 1) ? RED_PALB : RED_PAL;
		DBG_printf(SEL_NUM_COL,row,txt_color,"!             !           !                                  !");
		DBG_printf(SEL_NUM_COL,row,RED_PAL,"!");
		DBG_printf(SEL_NUM_COL+61,row,RED_PAL,"!");
		row++;
		}
	DBG_printf(SEL_NUM_COL,row++,RED_PAL,"--------------------------------------------------------------");

	STAT_t1games = stat.trk[0].games;
	STAT_t2games = stat.trk[1].games;
	STAT_t3games = stat.trk[2].games;
	STAT_games = STAT_t1games + STAT_t2games + STAT_t3games;

	DBG_printf(SEL_NUM_LABEL_COL,SEL_NUM_ROW+3,YEL_PAL,"#Games");
	DBG_printf(SEL_NUM_TOTAL_COL+2,SEL_NUM_ROW+3,YEL_PAL,"%5ld",STAT_games);
	DBG_printf(SEL_NUM_TRK1_COL+2,SEL_NUM_ROW+3,YEL_PAL,"%5ld",STAT_t1games);
	DBG_printf(SEL_NUM_TRK2_COL+2,SEL_NUM_ROW+3,YEL_PAL,"%5ld",STAT_t2games);
	DBG_printf(SEL_NUM_TRK3_COL+2,SEL_NUM_ROW+3,YEL_PAL,"%5ld",STAT_t3games);
}






/*  --------------------------------------------------------------  */
/*  !                          ! TOTAL ! TRACK1  TRACK2  TRACK3  !  */
/*  --------------------------------------------------------------  */
/*  ! Total Games Played       !123456 ! 100.0%  100.0%  100.0%  !  */
/*  !                          !       !                         !  */
/*  ! Total Minutes Played     !100.0% ! 100.0%  100.0%  100.0%  !  */
/*  !                          !       !                         !  */
/*  ! Race Finishes            !100.0% ! 100.0%  100.0%  100.0%  !  */
/*  !                          !       !                         !  */
/*  ! Linked Games             !100.0% ! 100.0%  100.0%  100.0%  !  */
/*  !                          !       !                         !  */
/*  ! Car Select: Car1         !100.0% ! 100.0%  100.0%  100.0%  !  */
/*  ! Car Select: Car2         !100.0% ! 100.0%  100.0%  100.0%  !  */
/*  ! Car Select: Car3         !100.0% ! 100.0%  100.0%  100.0%  !  */
/*  ! Car Select: Car4         !100.0% ! 100.0%  100.0%  100.0%  !  */
/*  ! Car Select: Car5         !100.0% ! 100.0%  100.0%  100.0%  !  */
/*  ! Car Select: Car6         !100.0% ! 100.0%  100.0%  100.0%  !  */
/*  !                          !       !                         !  */
/*  ! Trans. Select: Auto      !100.0% ! 100.0%  100.0%  100.0%  !  */
/*  ! Trans. Select: Manual    !100.0% ! 100.0%  100.0%  100.0%  !  */
/*  !                          !       !                         !  */
/*  ! Drone Option: On         !100.0% ! 100.0%  100.0%  100.0%  !  */
/*  ! Drone Option: Off        !100.0% ! 100.0%  100.0%  100.0%  !  */
/*  ! Drone Option: Racer X    !100.0% ! 100.0%  100.0%  100.0%  !  */
/*  !                          !       !                         !  */
/*  ! Force Option: Regular    !100.0% ! 100.0%  100.0%  100.0%  !  */
/*  ! Force Option: More       !100.0% ! 100.0%  100.0%  100.0%  !  */
/*  ! Force Option: Full       !100.0% ! 100.0%  100.0%  100.0%  !  */
/*  !                          !       !                         !  */
/*  ! Ketchup Option: On       !100.0% ! 100.0%  100.0%  100.0%  !  */
/*  ! Ketchup Option: Off      !100.0% ! 100.0%  100.0%  100.0%  !  */
/*  !                          !       !                         !  */
/*  ! View Time: View1         !100.0% ! 100.0%  100.0%  100.0%  !  */
/*  ! View Time: View2         !100.0% ! 100.0%  100.0%  100.0%  !  */
/*  ! View Time: View3         !100.0% ! 100.0%  100.0%  100.0%  !  */
/*  !                          !       !                         !  */
/*  ! Music Time: Off          !100.0% ! 100.0%  100.0%  100.0%  !  */
/*  ! Music Time: Track1       !100.0% ! 100.0%  100.0%  100.0%  !  */
/*  ! Music Time: Track2       !100.0% ! 100.0%  100.0%  100.0%  !  */
/*  ! Music Time: Track3       !100.0% ! 100.0%  100.0%  100.0%  !  */
/*  ! Music Time: Track4       !100.0% ! 100.0%  100.0%  100.0%  !  */
/*  ! Music Time: Track5       !100.0% ! 100.0%  100.0%  100.0%  !  */
/*  ! Music Time: Track6       !100.0% ! 100.0%  100.0%  100.0%  !  */
void stat_common_percents(void)
{
}

void stat_common_pct_template(void)
{
}

void stat_track_numbers(void)
{
	U16 day,hour;

	DBG_printf(22,2,YEL_PALB,"TRACK %ld NUMBERS",stat_track);

	stat_get_dayhour(&day,&hour);
	DBG_printf(12,12,YEL_PALB,"DAY:%d  HOUR:%d",day,hour);
}

#define IO_MIC_REQ (0x10)
#define IO_MIC_ACK (0x100)

#define MIC_NOP0	(0x0)
#define MIC_GET_TIME	(0x2)
#define MIC_RETURN_TIME	(0x3)
#define MIC_SET_TIME	(0x4)

void stat_get_dayhour(U16 *day, U16 *hour)
{
	U08 time[7];
	U16 i;

#if 0
	/*set time - hack until in GUTS */
	MIC_chat(MIC_SET_TIME);	/*set time in calender chip*/
	/*set time/date in BCD*/
	MIC_chat(0x0);	/*sec   lsn*/
	MIC_chat(0x1);	/*sec   msn*/
	MIC_chat(0x0);	/*min   lsn*/
	MIC_chat(0x2);	/*min   msn*/
	MIC_chat(0x1);	/*hour  lsn*/
	MIC_chat(0x2);	/*hour  msn*/
	MIC_chat(0x2);	/*day   lsn*/
	MIC_chat(0x0);	/*day   msn (always 0)*/
	MIC_chat(0x7);	/*date  lsn*/
	MIC_chat(0x2);	/*date  msn*/
	MIC_chat(0x8);	/*month lsn*/
	MIC_chat(0x0);	/*month msn*/
	MIC_chat(0x6);	/*year  lsn*/
	MIC_chat(0x9);	/*year  msn*/
#endif

#if 1
	GetPICDateTime(time);
#else
	MIC_chat(MIC_GET_TIME);		/*read time from calender chip into MIC registers*/
	MIC_chat(MIC_RETURN_TIME);	/*read time/date from calender chip into MIC registers*/
	for(i=0;i<7;i++)
		time[i] = MIC_chat(MIC_NOP0);
#endif

	/* convert BCD day to int and limit */
	*day = (time[3] & 0xF) - 1;
	if(*day >= STAT_NUM_DAYS)
		*day = STAT_NUM_DAYS - 1;

	/* convert BCD hours to int and limit */
	*hour = (((time[2] & 0xF0) >> 4) * 10) + (time[2] & 0xF);
	if(*hour >= STAT_NUM_HOURS)
		*hour = STAT_NUM_HOURS - 1;

}

void stat_save_daytime(U08 day, U08 hour, U08 min)
{
	U08 lsn,msn;

	MIC_chat(MIC_SET_TIME);	/*set time in calender chip*/
	/*set time/date in BCD*/
	MIC_chat(0x0);	/*sec   lsn*/
	MIC_chat(0x0);	/*sec   msn*/

	msn = min/10;
	lsn = min - (msn*10);
	MIC_chat(lsn);	/*min   lsn*/
	MIC_chat(msn);	/*min   msn*/

	msn = hour/10;
	lsn = hour - (msn*10);
	MIC_chat(lsn);	/*hour  lsn*/
	MIC_chat(msn);	/*hour  msn*/

	MIC_chat(day+1);	/*day (1-7)  lsn*/
	MIC_chat(0x0);	/*day   msn (always 0)*/

	MIC_chat(0x7);	/*date  lsn*/
	MIC_chat(0x2);	/*date  msn*/
	MIC_chat(0x8);	/*month lsn*/
	MIC_chat(0x0);	/*month msn*/
	MIC_chat(0x6);	/*year  lsn*/
	MIC_chat(0x9);	/*year  msn*/
}

U08 MIC_chat(U08 cmd)
{
	VU32 *cp = (VU32 *)IO_H2MIC_CMD;
	VU32 *dp = (VU32 *)IO_MIC2H_DTA;
	U08 dat;

	/*write cmd and REQ*/
	*cp = IO_MIC_REQ | (cmd & 0xF);

	/*wait for ACK*/
	while((*dp & IO_MIC_ACK) == 0);

	/*read returned data*/
	dat = *dp & 0xFF;
	
	/*clear REQ*/
	*cp = 0;

	/*wait for ACK clear*/
	while((*dp & IO_MIC_ACK) != 0);

	return(dat);
}

