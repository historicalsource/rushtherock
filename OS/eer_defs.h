#define EER_AUD_VOL (0)	/* audio volume level */
#define EER_GUTS_OPT (1)	/* GUTS options, incl. attract mute */
#define EER_CC0 (2)	/* Coin mech 1 */
#define EER_CC1 (3)	/* Coin mech 2 */
#define EER_CC2 (4)	/* Coin mech 3 */
#define EER_CC3 (5)	/* Coin mech 4 */
#define EER_CC4 (6)	/* Bill acceptor */
#define EER_CCX (7)	/* Service credit switch */
#define EER_0PTIME (8)	/* idle time */
#define EER_1PTIME (9)	/* 1 player time in play */
#define EER_NEWCOINS (10)	/* new games */
#define EER_CONTCOINS (11)	/* cont games */
#define EER_FREEGMS (12)	/* earned games */
#define EER_GMS (13)	/* total games */
#define EER_NEWTIM (14)	/* new game time */
#define EER_CONTTIM (15)	/* continued game time */
#define EER_FREETIM (16)	/* earned game time */
#define EER_MIRROR (17)	/* number of mirror games */
#define EER_LINKED (18)	/* number of linked games */
#define EER_ERRORS (19)	/* count of EEPROM errors */
#define EER_HI_CNT (20)	/* hi score games since hi-score reset */
#define EER_GAME_CNT (21)	/* games since hi-score reset */
#define EER_CNOPT (22)	/* Coin mech options */
#define EER_CNVAL (23)	/* Coin price options */
#define EER_GMOPT (24)	/* game options */
#define EER_TRKOPT (25)	/* Track options */
#define EER_LAPOPT (26)	/* Number of laps options */
#define EER_TRNOPT (27)	/* Tournament options */
#define EER_RESET (28)	/* PROC non-powerup RESETS */
#define EER_WR_RO (29)	/* PROC write to read only mem */
#define EER_RD_NEM (30)	/* PROC read from non-existant mem */
#define EER_WR_NEM (31)	/* PROC write to non-existant mem */
#define EER_ADRERR_R (32)	/* PROC address alignment error on read */
#define EER_ADRERR_W (33)	/* PROC address alignment error on write */
#define EER_BUSERR_I (34)	/* PROC bus error on I fetch */
#define EER_BUSERR_D (35)	/* PROC bus error on D fetch */
#define EER_SYSCALL (36)	/* PROC SYSCALLS */
#define EER_BREAKP (37)	/* PROC Breakpoints */
#define EER_ILGINS (38)	/* PROC reserved instructions */
#define EER_COPROC (39)	/* PROC coprocessor unusables */
#define EER_ARITH (40)	/* PROC arithmetic overflows */
#define EER_TRAP (41)	/* PROC trap exceptions */
#define EER_RESERV (42)	/* PROC reserved exceptions */
#define EER_FLOAT (43)	/* PROC floating point exceptions */
#define EER_UNDEF (44)	/* PROC undefined exceptions */
#define EER_OVERFL (45)	/* PROC overflow checks */
#define EER_DVDBY0 (46)	/* PROC divide by 0 */
#define EER_RANGE (47)	/* PROC range checks */
#define EER_UHINT (48)	/* PROC unhandled interrupt */
#define EER_MOVERFL (49)	/* PROC multiply overflow */
#define EER_DATALST (50)	/* SOFT sound data lost */
#define EER_AUDRESET (51)	/* SOFT sound proc resets */
#define EER_LINKFAIL (52)	/* SOFT LINK failures */
#define EER_DSK_ERR (53)	/* DISK any disk error */
#define EER_DSK_AMNF (54)	/* DISK address not found */
#define EER_DSK_TK0NF (55)	/* DISK trk 0 not found */
#define EER_DSK_ABORT (56)	/* DISK command abort */
#define EER_DSK_IDNF (57)	/* DISK ID not found */
#define EER_DSK_UNCDTA (58)	/* DISK uncorr data error */
#define EER_DSK_TIMOUT (59)	/* DISK device timeout */
#define EER_DSK_WERR (60)	/* DISK write fault */
#define EER_DSK_CORR (61)	/* DISK correctable data err */
#define EER_FSYS_USEALT (62)	/* FSYS Used alternate file */
#define EER_POT0L (63)	/* Brake  - low limit */
#define EER_POT1L (64)	/* Gas    - low limit */
#define EER_POT2L (65)	/* Clutch - low limit */
#define EER_POT3L (66)	/* Wheel  - low limit */
#define EER_POT0H (67)	/* Brake  - high limit */
#define EER_POT1H (68)	/* Gas    - high limit */
#define EER_POT2H (69)	/* Clutch - high limit */
#define EER_POT3H (70)	/* Wheel  - high limit */
#define HIST_NEWGAME (0)
#define HIST_CONTGAME (1)
#define HIST_FREEGAME (2)
#define HIST_CAR (3)
#define HIST_TRK (4)
#define HST_TRACK1_TOP (0)
#define HST_TRACK2_TOP (1)
#define HST_TRACK3_TOP (2)
#define HST_TRACK4_TOP (3)
#define HST_TRACK5_TOP (4)
#define HST_TRACK6_TOP (5)
#define HST_TRACK7_TOP (6)
#define HST_TRACK8_TOP (7)
#define N_TRIES (4)	/* number of retries on write */
#define EEPROM_SIZE (2048)	/* in bytes */
#define STRIDE (2)	/* shift amount eeprom addressing */
#define MAX_STAT (70)	/* Highest legit stat number */
extern int eer_gets(/* unsigned int stat_num */); /* ret: val or -1 = error*/
extern int eer_puts(/* unsigned int stat_num,val*/);/* ret: 0 = ok, -1 = error*/
extern int eer_incs(/* unsigned int stat_num,val*/);/* ret: val or -1 = error */
extern int eer_init();	/* returns total errors, if EER_ERROR defined */
extern int eer_busy();	/* returns !0 if some write still in progress */
/* You have one "orphan" nybble */
#define HIST_TABLES (5)	/* number of histogram tables */
extern int eer_geth(/* unsigned int table,bin */); /* ret: val or -1 = error */
extern int eer_puth(/* unsigned int table,bin,val*/);/* ret: val or -1 = error*/
extern int eer_bumph(/* unsigned int table,bin,bump*/);
    /* bump bin of table, scale all "tied" histograms on > 255. */
    /*  ret: val or -1 = error */
extern int eer_clrhist(/* unsigned int table */); /* ret: 0 ok, -1 = error */
extern int eer_hist_title
  (/* int table, char *buff, int len, int *max, int *med, char *bar */);
    /* returns title of histogram table in caller's buff, max length len */
    /* optionally stores max bin value and median bin number. */
    /* saves pointer to bar characters */
    /* returns length of title, or -1 = error */
extern int eer_hist_line(/* unsigned int bin, char *buff, int buflen */);
    /* format a line into caller's buff, max length buflen */
    /* ret: bin value or -1 = error */
    /* uses table provided by eer_hist_title() */
extern int eer_tally_hist(/* int table, int value */);
    /* increment the appropriate bin of table, based on value. */
    /* returns current value of bin or -1 = error. May scale histogram and */
    /* any "tied" to it */
#define HST_TIE_TO_NEWER (1)
#define MAX_INITS (8)	/* Max # of initials in any H.S.T */
#ifndef __HST_ENT_TYPE_DEFINED
#define __HST_ENT_TYPE_DEFINED
extern struct hst_ent {
    unsigned long score;
    char initials[MAX_INITS+1];
    } *eer_hstr(/*unsigned int rank,table */); /* ret: 0 if given bad param */
#endif
extern int
eer_hstw(/* unsigned int rank, struct hst_ent *entptr, unsigned int table */);
/* writes entry at "rank" in "table", rippling lower scores down */
extern int eer_rank(/* unsigned int score,table */); /* ret: rank (0 is top) */
#define LAST_BUCKET (26)
