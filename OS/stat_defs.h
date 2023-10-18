#define EER_ERRORS (19)	/* count of EEPROM errors */
#define NEWGAME_BROTHERS (0x1)	/* Default to "only child" */
#define CONTGAME_BROTHERS (0x2)	/* Default to "only child" */
#define FREEGAME_BROTHERS (0x4)	/* Default to "only child" */
#define CAR_BROTHERS (0x8)	/* Default to "only child" */
#define TRK_BROTHERS (0x10)	/* Default to "only child" */
#define N_TRIES (4)	/* number of retries on write */
#define EEPROM_SIZE (2048)	/* in bytes */
#define STRIDE (2)	/* shift amount eeprom addressing */
#define SIGNATURE (0x0FBE5A0E2)	/* "unique" signature for this game */
#define MAX_STAT (70)	/* Highest legit stat number */
extern int eer_gets(/* unsigned int stat_num */); /* ret: val or -1 = error*/
extern int eer_puts(/* unsigned int stat_num,val*/);/* ret: 0 = ok, -1 = error*/
extern int eer_incs(/* unsigned int stat_num,val*/);/* ret: val or -1 = error */
extern int eer_init();	/* returns total errors, if EER_ERROR defined */
extern int eer_busy();	/* returns !0 if some write still in progress */
/* You have one "orphan" nybble */
#define BUCKET_NUM (99)
#define MAX_RUN (18)	/* in bytes */
#define RUN_BYTES (19)
#define RUN_LEN (25)
#define RUN_REC (0)
#define SIGNAT_POSN (100)
#define MAX_HOME (207)	/* in NYBBLES */
#define HOME_BYTES (104)
#define MAX_DATA (121)	/* Largest record, in bytes */
#define ALT_OFFS (112)
#define HOME_REC (1)
#define ALT_REC (2)
#define SIZ_MASK (7)
#define RUN_MASK (31)
#define HOME_MASK (255)
#define RUN_POSN (0)
#define HOME_POSN (5)
#define SIZ_POSN (13)
#define INC_ABLE (65536)
#define MAP_TYPE unsigned long
#define DEF_CTR(HSZ,HP,RP,LABEL) (INC_ABLE|(HSZ<<SIZ_POSN)|(HP<<HOME_POSN)|(RP)),
#define DEF_OPT(HSIZE,HPOS,RPOS,LABEL) ((HSIZE<<SIZ_POSN)|(HPOS<<HOME_POSN)|(RPOS)),
#define FIRST_HISTOGRAM_REC (3)	/* entry in master index */
#define FIRST_HISTOGRAM_OFFSET (224)	/* eeprom offset */
#define HISTOGRAM_LEN (109)	/* eeprom bytes for histograms */
#define HISTOGRAM_BYTES (80)	/* ram bytes for histograms */
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
#define HST_INITIAL_COMPRESS (0)
#define HST_TIE_TO_NEWER (1)
#define FIRST_HST_REC (8)	/* entry in master index */
#define FIRST_HST_OFFSET (333)	/* start of h.s. table eeprom */
#define HST_BYTES (960)	/* #bytes in high-score table shadow */
#define HST_TABLES (8)	/* number of high-score tables */
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
#define FIRST_BUCKET (1357)	/* start of "running" area */
#define MAX_CHECK (8)
#define LAST_BUCKET (26)
