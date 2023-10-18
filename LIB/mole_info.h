#ifndef _MOLE_INFO_H
#define _MOLE_INFO_H

#include "icelesspkt.h"

#define MAXFNAMELEN			20 /* divisible by 4 */
#define MFL					MAXFNAMELEN
#define MAXPATHLEN			4  /* doesn't include rootlsn and filename */
#define MAXPATHNAMELEN		(MAXFNAMELEN*MAXPATHLEN)		/* path only */
#define MAXPATHFNAMELEN		(MAXFNAMELEN*(MAXPATHLEN+1)+4) /* with rootlsn & fname */
#define OLD_HD_CMDS 	1
#define MAXNUMSEC		63
#define MAXNUMDIRSEC	4 /* ThisNum*DIRENTRIESPERSEC-1(now 63) is the max # files or*/
/*                           subdirs in a dir (if you don't overflow your partition).*/
/*							 See hdrive.h for definition of DIRENTRIESPERSEC(now 16).*/
#define BYTESPERSEC		512
#define MAXTRACKSIZE	(BYTESPERSEC * MAXNUMSEC)
#define LONGSPERSEC		(BYTESPERSEC/4) /* 128 */
#define DEFROOTDIR		0
#define DIRFLAG 0xFFFFFFFF
static const U32 stack_trackbuf = 0x0UL;
#define STACKTRACKBUFSIZE (MAXTRACKSIZE*2)

#define IDSECTORS 		1

#define ERRORSECTOFF 	(2*MAXNUMSEC)
#define ERRORSECTNUM 	(ERRORSECTOFF - IDSECTORS)
#define ESECTFLAG1		0xFEEB
#define ESECTFIRSTSECT	0x0
#define ESECTNUMSECTUSED 0x1
#define ESECTLASTSECTBYTES 0x0
#define ESECTFLAG2		0xFACE
#define ESECTFLAG3		0xC0EDBEEF

typedef struct {
	U16 eflag1;
	U8  firstsect;   /* in queue */
	U8  numsectused; /* in queue */
	U16 lastsectbytes;
	U16 eflag2;
	U32 eflag3;
} ESectHdr;

#define ESECTMAXBYTES	(ERRORSECTNUM * BYTESPERSEC - sizeof(ESectHdr))
#define ESECT0MAXBYTES	(BYTESPERSEC - sizeof(ESectHdr))

typedef struct {
	U32 rootlsn;
	U8  path_tbl[MAXPATHLEN][MAXFNAMELEN];
	S32 path_len; /* -1 => undef */
} DirName;

typedef struct {
	U8	    fname[MAXFNAMELEN];
	DirName dname;
} FilePath;

enum {
	/* acks */
	NACK, 				ACK, 			ACK_DISK_INF,
	ACK_DIR_INF, 		ACK_DISK_INFO,	ACK_FILE_INF,
	ACK_LIST_DIR,		ACK_READ,		ACK_RD_FILE,		LAST_ACK = ACK_RD_FILE,
	/* cmds that don't need an ack */
	CMD_DISK_WRITE_FAST, CMD_WRITE_FAST,           LAST_CMD_NO_ACK=CMD_WRITE_FAST,
	/* cmds that do need an ack */
	CMD_DIR_INF,
	CMD_DISK_INF,
	CMD_DISK_INFO,
	CMD_DISK_READ,
	CMD_DISK_RD,
	CMD_DISK_WR,
	CMD_DISK_WRITE,
	CMD_FILE_INF,
	CMD_GET_STATUS,
	CMD_LIST_DIR,
	CMD_MK_DIR,
	CMD_MK_FILE,
	CMD_MK_ROOT,
	CMD_MULTI,
	CMD_RD_FILE,
	CMD_READ,
	CMD_RESET_STATUS,
	CMD_RESET_STATUS_SYNC,
	CMD_RM_FILE,
	CMD_RM_DIR,
	CMD_SYNC,
	CMD_WR_FILE,
	CMD_WRITE,
	CMD_CLOSE_CONN,										NUM_CMDS
};

#define ASC_THREAD 2
#define UNIX_THREAD 2
#define USER_FLAG1_FROM_UNIX 0xAA
#define USER_FLAG2_FROM_UNIX 0x55
#define USER_FLAG1_FROM_ASC 0x55
#define USER_FLAG2_FROM_ASC 0xAA
#define ASC_PORT  230
#define ASC_BOARD 1
#define ASC_RR	  1

#define PKT_USERSTART PKTuser
#define PKT_U( user_flag )  (PKT_USERSTART + (user_flag))

#define USER_FLAG1	0
#define USER_FLAG2 	1
#define USER_CSUM   2 /* For MIN(15, user_info_len-USER_CSUM-1) next bytes.*/
                      /* If a DATA_??_TOTLEN > (15-(USER_DATASTART-USER_CSUM-1), */
                      /* then it should contain a csum. */
#define USER_FSN   	3
#define USER_CMD   	4

#define USER_DATASTART 5
#define USER_D( data_flag )	(USER_DATASTART + (data_flag))
#define PKT_D( data_flag )	(PKT_U(USER_DATASTART) + (data_flag))


/** all acks **/
#define DATA_AA_STATUS 0

/* nack */
#define DATA_N_TOTLEN 1
/* ack */
#define DATA_A_TOTLEN 1
/* ack disk info */
#define DATA_ADI_NHEADS 1 /* U16 */
#define DATA_ADI_NCYLS  3 /* U16 */
#define DATA_ADI_NSECS  5 /* U16 */
#define DATA_ADI_TOTLEN 7
/* ack disk inf */
#define DATA_ADINF_NLSN   1 /* U32 */
#define DATA_ADINF_NSECS  5 /* U16 */ /*used for efficiency reasons*/
#define DATA_ADINF_TOTLEN 7
/* ack dir inf */
#define DATA_ADIRI_DIR_NUM_SECT     1  /* U32 */
#define DATA_ADIRI_PART_NUM_SECT    5  /* U32 */
#define DATA_ADIRI_NUM_FILES		9  /* U16 */
#define DATA_ADIRI_MAX_FSECTSIZE	11 /* U32 */
#define DATA_ADIRI_FREE_SECT		15 /* U32 */
#define DATA_ADIRI_TOTLEN    		19  
/* ack file inf */
#define DATA_AF_NUM_SECT	1  /* U32 */
#define DATA_AF_NUM_BYTES	5  /* U32 */
#define DATA_AF_TOTLEN    	9 
/* ack list dir */
#define DATA_ALDIR_EXDATA_LEN	1  /* U32 */
#define DATA_ALDIR_FIRSTF	    5  /* U32 */ /* 0 if empty, o/w 1->num of files */
#define DATA_ALDIR_LASTF		9  /* U32 */ /* 0 if empty, o/w 1->num of files */
#define DATA_ALDIR_AT_END		13 /* U8 */
#define DATA_ALDIR_MINTOTLEN    14
#define EXDATA_ALDIR_NUM_SECT	0  /* U32 */
#define EXDATA_ALDIR_NUM_BYTES	4  /* U32 */
#define EXDATA_ALDIR_MINTOTLEN	8  /* fname follows */
/* ack read */           	    /*!!!!!!! assumes USER_DATASTART==5 !!!!!!!*/	
#define DATA_AR_LEN 1 /* U16 */ /*!! Want (USER_DATASTART+DATA_AR_MINTOTLEN)%4==0 !!*/
#define DATA_AR_MINTOTLEN 3	  /*!! This makes host buffer quiker (long word alligned)*/
/* ack rd file */
#define DATA_ARF_LEN       1 /* U32 */
#define DATA_ARF_AT_END    5 /* U8 */
#define DATA_ARF_TOTLEN    6


/* sync's, & status's & disk_info */
#define DATA_CMD_TOTLEN 0
/* disk read AND disk write */
#define DATA_D_ADDR  0  /* U32 */
#define DATA_D_CYL   4  /* U16 */
#define DATA_D_HEAD  6  /* U16 */
#define DATA_D_SECT  8  /* U16 */
#define DATA_D_NSECT 10 /* U16 */
#define DATA_D_TOTLEN 12
/* dir inf & list dir */
#define DATA_DIRI_ROOT_DIR   0  /* U32 */
#define DATA_DIRI_PATHCLEN   4  /* U16 */ /* non-zero */
#define DATA_DIRI_MINTOTLEN  6  /* a path MUST follow */
/* file inf & rm file */
#define DATA_F_ROOT_DIR     0  /* U32 */
#define DATA_F_FPATHCLEN    4  /* U16 */ /* non-zero */
#define DATA_F_MINTOTLEN    6  /* a fpath MUST follow */
/* multi */
#define DATA_M_COUNT 0  /* U8 */
#define DATA_M_CMD 1  /* U8 */
#define DATA_M_MINTOTLEN 2
/* mk dir */
#define DATA_MD_DIR_NUM_SEC  0  /* U32 */
#define DATA_MD_PART_NUM_SEC 4  /* U32 */
#define DATA_MD_ROOT_DIR     8  /* U32 */
#define DATA_MD_PATHCLEN     12 /* U16 */ /* non-zero */
#define DATA_MD_MINTOTLEN    14 /* a path MUST follow */
/* mk file */
#define DATA_MF_NUM_SEC  	 0  /* U32 */
#define DATA_MF_ROOT_DIR     4  /* U32 */
#define DATA_MF_FPATHCLEN    8 /* U16 */ /* non-zero */
#define DATA_MF_MINTOTLEN    10 /* a fpath MUST follow */
/* mk root */
#define DATA_MR_NEW_DIR      0  /* U32 */
#define DATA_MR_DIR_NUM_SEC  4  /* U32 */
#define DATA_MR_PART_NUM_SEC 8  /* U32 */
#define DATA_MR_TOTLEN   	 12
/* read */
#define DATA_R_SRC 0  /* U32 */
#define DATA_R_DLEN 4 /* U16 */
#define DATA_R_TOTLEN 6
/* rm dir */
#define DATA_RD_FORCE		 0  /* U8 */
#define DATA_RD_ROOT_DIR     1  /* U32 */
#define DATA_RD_PATHCLEN     5  /* U16 */ /* non-zero */
#define DATA_RD_MINTOTLEN    7  /* a path MUST follow */
/* rd file */
#define DATA_RF_START_BYTE 	 0  /* U32 */
#define DATA_RF_LEN 	 	 4  /* U32 */
#define DATA_RF_DEST  		 8  /* U32 */
#define DATA_RF_EX_SPACE     12 /* U16 */
#define DATA_RF_ROOT_DIR     14 /* U32 */
#define DATA_RF_FPATHCLEN    18 /* U16 */ /* non-zero */
#define DATA_RF_MINTOTLEN    20 /* a fpath MUST follow */
/* write AND writefast */
#define DATA_W_DEST 0 /* U32 */
#define DATA_W_LEN  4 /* U16 */ /*!!!!!!! assumes USER_DATASTART==5 !!!!!!!*/	
#define DATA_W_DUMMY1 6         /*!! Want (USER_DATASTART+DATA_W_MINTOTLEN)%4==0 !!*/
#define DATA_W_MINTOTLEN 7	  /*!! This makes host buffer quiker (long word alligned)*/
/* wr file */
#define DATA_WF_START_BYTE 	 0  /* U32 */
#define DATA_WF_LEN 	 	 4  /* U32 */
#define DATA_WF_SRC	  		 8  /* U32 */
#define DATA_WF_AT_END       12 /* U8  */
#define DATA_WF_ROOT_DIR     13 /* U32 */
#define DATA_WF_FPATHCLEN    17 /* U16 */ /* non-zero */
#define DATA_WF_MINTOTLEN    19 /* a fpath MUST follow */


#if 1
#define MAX_USER_SIZELONG 123
#define MAX_USER_SIZE	  (MAX_USER_SIZELONG*sizeof(U32))  /*492*/
#define MAX_PKT_SIZE      (MAX_USER_SIZE + PKT_USERSTART)  /*492+6==498*/
#define MAX_DATA_SIZE	  (MAX_USER_SIZE - USER_DATASTART) /*492-5==487*/
#define MAX_RW_DATA_SIZE  (MAX_DATA_SIZE - MAX(DATA_W_MINTOTLEN,DATA_AR_MINTOTLEN))
                                                         /*487-7==480*/
#define MAX_RW_DATA_SIZELONG	  (MAX_RW_DATA_SIZE/4*4) /*now480*/
#else
#define MAX_USER_SIZELONG 259
#define MAX_USER_SIZE	  (MAX_USER_SIZELONG*sizeof(U32))  /*1036*/
#define MAX_PKT_SIZE      (MAX_USER_SIZE + PKT_USERSTART)  /*1036+6==1042*/
#define MAX_DATA_SIZE	  (MAX_USER_SIZE - USER_DATASTART) /*1036-5==1031*/
#define MAX_RW_DATA_SIZE  (MAX_DATA_SIZE - MAX(DATA_W_MINTOTLEN,DATA_AR_MINTOTLEN))
                                                         /*1031-7==1024*/
#define MAX_RW_DATA_SIZELONG	  (MAX_RW_DATA_SIZE/4*4) /*now1024*/
#endif
#define STATUS_FSNERR   (1<<0)
#define STATUS_REBOOTED (1<<1)
#define STATUS_DISKERR  (1<<2)
#define STATUS_CMDERR   (1<<3)

U32 get_U32(const U8 * stack);
U16 get_U16(const U8 * stack);
void set_U32(U8 * stack, U32 src);
void set_U16(U8 * stack, U16 src);
int strnlen(const char * str, unsigned int max_len);
int strnlen2(const char * str, unsigned int max_len);
int parity8(U8 val);
U8 check_sum(U8 * buf, unsigned int len/*max 15*/);

#endif
