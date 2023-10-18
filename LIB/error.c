#include <config.h>
#include <eer_defs.h>
#include <os_proto.h>
#include <string.h>
#include "stdtypes.h"
#include "error.h"
#include "io.h"
#include "hdrive.h"
#include "mole_info.h"
#include "mole.h"
#include "ctype.h"
#if HOCKEY
#include "rom.h"
#endif

#include "glide.h"
extern void sst_text2fb(int swap);

#define MAXERRLEN 129 /* 2 lines + null */
#define ERRORLINE ((VIS_V_PIX/8)-2)

const char * const ErrorTable[]={
	"GENERIC ERR: ",
	"MBOX T-OUT: ",
	"ANIM ERR: ",
	"MATH ERR: ",
	"",
	"MBOX ERR: ",
	"",
	""
};
#define NERRORS	(sizeof(ErrorTable)/sizeof(char *))

#define NUMSCROLLINGERRS 6
static U8 last_error;
static char scrollinglist[NUMSCROLLINGERRS][65/*line size + null*/];
static U8  errorcolor[NUMSCROLLINGERRS];
static U8  errorcolorflag;
static U8  error_with_this_color;
static U8  ESectorCode;
enum {ESECT_UNINITED, ESECT_OK, ESECT_INUSE};

int running_standalone;		/* If running stand-alone, don't print warnings */

#if USER_RECS
void wrt_err_msg(char *msg) {
	U8 *rcd;
	int indx;
	indx = eer_gets(EER_UREC_IDX);	/* get index to next record to write */
	if (indx < USER_RECS) {
		rcd = eer_user_rd(indx, 0);	/* get ptr to next record to write */
		if (rcd) strncpy((char *)rcd, msg, 64);	/* if there is one, copy error msg */
		eer_user_wrt(indx);		/* write message */
		indx += 1;			/* advance index */
#if 0
		if (indx >= USER_RECS) indx = 0; /* wrap it */
#endif
		eer_puts(EER_UREC_IDX, indx);
	}

}
#endif

#define DOMAIN 1
#define SING 2
#define OVERFLOW 3
#define UNDERFLOW 4
#define TLOSS 5
#define PLOSS 6

/*** this routine is needed to force math errors out our error handling code ***/
double __matherror (void *ptr, char *name, double arg1, double arg2, int type, double ret) {
	char err[MAXERRLEN];

	switch(type) {
		case SING: 	nsprintf(err,MAXERRLEN,"%s(%g): SING error",name,arg1); break;
		case DOMAIN:nsprintf(err,MAXERRLEN,"%s(%g): DOMAIN error",name,arg1); break;
		default:	nsprintf(err,MAXERRLEN,"%s(%g)",name,arg1); break;
	}
	Error(MATHERROR,err);
	return 0;
}

void Error(S16 errid, const char *msg) {
	char err[MAXERRLEN];
	int len, i, j;
	char * errp;
	U32 red_palb, grn_palb;

	if (running_standalone) return;
	if (0) {
		red_palb = RED_PALB; /* _PALB's not avail yet of MB txt */
		grn_palb = GRN_PALB;
	} else {
		red_palb = RED_PAL;
		grn_palb = GRN_PAL;
	}
    if (errid >= NERRORS)
		len = nsprintf(err, MAXERRLEN, "UNKNOWN ERR %d: ", errid);
	else
	    len = nsprintf(err, MAXERRLEN, "%s", ErrorTable[errid]);
#if USER_RECS
	if(1 || errid==CRITICALERR) {
		char err[64];
		int idx = eer_gets(EER_CC0)+eer_gets(EER_CC1);
		nsprintf(err,64,"%-2d %-60.60s",idx,msg);
		err[63] = 0;
		wrt_err_msg(err);
	}
#endif	
	if (len<0)
		len = 0; /* for some reason sprintf was(still?) returning -1 */
    len += nsprintf(err+len, MAXERRLEN-len, "%.*s", MAXERRLEN-1-len, msg);

	last_error = (last_error+1)%NUMSCROLLINGERRS;
	errorcolor[last_error] = errorcolorflag;
	errp = scrollinglist[last_error];
	nsprintf(errp, 65, "%-64.64s", err);
	if (len>64) {
		last_error = (last_error+1)%NUMSCROLLINGERRS;
		errorcolor[last_error] = errorcolorflag;
		errp = scrollinglist[last_error];
		nsprintf(errp, 65, "%-64.64s", err + 64);
	}
	error_with_this_color = 1;
	
	if (errid!=QUIETERROR) {
		for (i=0; i<NUMSCROLLINGERRS; i++) {
			j = (last_error+NUMSCROLLINGERRS-i)%NUMSCROLLINGERRS;
			txt_str(0,ERRORLINE-i, scrollinglist[j], errorcolor[j]?red_palb:grn_palb);
		}
	}
	esectprint(err); /* put here in case write to HD gives an error */
#if 0
	grBufferClear(0x00300000, 0, 0);
	sst_text2fb(0);
	grBufferSwap(0);
#endif
}

void IncErrorColor(void) {
	if (error_with_this_color) {
		error_with_this_color=0;
		errorcolorflag^=1;
	}
}

void FatalError(S16 errid, const char *msg) {
	int i=0;
#if USER_RECS
	char err[64];
	int idx = eer_gets(EER_CC0)+eer_gets(EER_CC1);
	nsprintf(err,64,"%-2d %-60.60s",idx,msg);
	err[63] = 0;
	wrt_err_msg(err);
#endif	

	running_standalone = 0;
	prc_delay(30);
	Error(errid,msg);
	sst_text2fb(0);
	grBufferSwap(0);

#if !ROM
	/*ForceMoleToStop();*/
#endif

	while(1)
	{
		prc_delay(1);
#if !ROM
		*(VU32 *)WATCHDOG = 1;
#endif
	}
	prc_reboot();
}

void Errorf(S16 errid, char const *fmt,...) {
	char err[MAXERRLEN];
	va_list ap;

	va_start(ap, fmt);
	nvfprintf(err, MAXERRLEN, fmt, ap);
	va_end(ap);

	Error(errid, err);
}

void FatalErrorf(S16 errid, char const *fmt,...) {
	char err[MAXERRLEN];
	va_list ap;

	va_start(ap, fmt);
	nvfprintf(err, MAXERRLEN, fmt, ap);
	va_end(ap);
	
	FatalError(errid,err);
}

void WarnMsg(char const *fmt,...) {
	char str[MAXERRLEN];
	va_list ap;

	if (running_standalone) return;
	va_start(ap, fmt);
	nvfprintf(str, MAXERRLEN, fmt, ap);
	va_end(ap);

#if HOCKEY
	Error(ERR, str);
#else
	txt_str(0,ERRORLINE,str,RED_PAL);
#endif
}

void FatalMsg(char const *fmt, ...) {
	char err[MAXERRLEN];
	va_list ap;

	va_start(ap, fmt);
	nvfprintf(err, MAXERRLEN, fmt, ap);
	va_end(ap);

#if HOCKEY
	FatalError(ERR, err);
#else
	txt_str(0,ERRORLINE,err,RED_PAL);
	sst_text2fb(0);
	grBufferSwap(0);
	ForceMoleToStop();
	while(1)
		*(VU32 *)WATCHDOG = 1;		
#endif
}

void InitError(void) {
	int i;
	for (i=0; i<NUMSCROLLINGERRS; i++) {
		scrollinglist[i][0]='\0';
		errorcolor[i]=0;
	}
	last_error=0;
	errorcolorflag=0;
	error_with_this_color=0;
	ESectorCode = ESECT_UNINITED;
}

static int CheckESectorsHdr(U32 * buf) {
	ESectHdr hdr;
	int lastsect, status;

	status = Read_Sectors(buf, NumLSN-ERRORSECTOFF, 1/*num_sectors*/);
	if(status)
		FatalMsg("CheckESectorsHdr: Read_Sectors status:%d", status);

	memcpy(&hdr, buf, sizeof(ESectHdr));

	if(hdr.eflag1!=ESECTFLAG1) {
		WarnMsg("CheckESectorsHdr: (Re)Init'ing ErrorSectors: eflag1 %4lu",
				(U32)hdr.eflag1);
		return 1;
	}
	if(hdr.eflag2!=ESECTFLAG2) {
		WarnMsg("CheckESectorsHdr: (Re)Init'ing ErrorSectors: eflag2 %4lu",
				(U32)hdr.eflag2);
		return 1;
	}
	if(hdr.eflag3!=ESECTFLAG3) {
		WarnMsg("CheckESectorsHdr: (Re)Init'ing ErrorSectors: eflag3 %4lu",
				(U32)hdr.eflag3);
		return 1;
	}
	if(hdr.firstsect>=ERRORSECTNUM) {
		WarnMsg("CheckESectorsHdr: (Re)Init'ing ErrorSectors: firstsect %lu",
				(U32)hdr.firstsect);
		return 1;
	}
	if(hdr.numsectused>ERRORSECTNUM || hdr.numsectused==0) {
		WarnMsg("CheckESectorsHdr: (Re)Init'ing ErrorSectors: numsectused %lu",
				(U32)hdr.numsectused);
		return 1;
	}
	lastsect = (hdr.firstsect + hdr.numsectused - 1) % ERRORSECTNUM;
	if(hdr.lastsectbytes>BYTESPERSEC
	   || (lastsect==0 && hdr.lastsectbytes>ESECT0MAXBYTES)) {
		WarnMsg("CheckESectorsHdr: (Re)Init'ing ErrorSectors: numsectused %lu",
				(U32)hdr.numsectused);
		return 1;
	}
	return 0;
}

static void ReInitErrorSectors(U32 * buf) {
	int status, i, j;
	ESectHdr * hdrp = (ESectHdr *)buf;
	int code = ESectorCode;
	hdrp->eflag1 = ESECTFLAG1;
	hdrp->firstsect = ESECTFIRSTSECT;
	hdrp->numsectused = ESECTNUMSECTUSED;
	hdrp->lastsectbytes = ESECTLASTSECTBYTES;
	hdrp->eflag2 = ESECTFLAG2;
	hdrp->eflag3 = ESECTFLAG3;
	status = Write_Sectors(buf, NumLSN-ERRORSECTOFF, 1/*num_sectors*/);
	if(status)
		FatalMsg("ReInitErrorSectors: Write_Sectors status:%d", status);
	/* get errors from scrolling error list */
	for (i=1; i<=NUMSCROLLINGERRS; i++) {
		j = (last_error+i)%NUMSCROLLINGERRS;
		if(scrollinglist[j][0]) {
			ESectorCode = ESECT_OK;
			esectprint(scrollinglist[j]);
			ESectorCode = code;
		}
	}
}

void InitErrorSectors(void) {
	ESectHdr hdr;
	U32  buf[LONGSPERSEC];
	U8 * bufp;
	int  reinit, bytetotal, cursect, lastsect, status, sect;

	if(ESectorCode!=ESECT_UNINITED)
		WarnMsg("InitErrorSectors: call with bad ESectorCode (%d)", ESectorCode);
	ESectorCode = ESECT_INUSE;

	reinit = CheckESectorsHdr(buf);
	memcpy(&hdr, buf, sizeof(ESectHdr));

	lastsect = (hdr.firstsect + hdr.numsectused - 1) % ERRORSECTNUM;
	for(sect=0,cursect=hdr.firstsect; !reinit && sect<ERRORSECTNUM; sect++,cursect++) {
		if(hdr.firstsect<=lastsect) {
			if(sect<hdr.firstsect||sect>lastsect)
				continue;
		} else {
			if(sect>lastsect && sect<hdr.firstsect)
				continue;
		}
		bufp = (U8*)buf;
		bytetotal = BYTESPERSEC;
		if(sect==0) {
			bufp += sizeof(ESectHdr);
			bytetotal -= sizeof(ESectHdr);
			if(sect==lastsect)
				bytetotal = hdr.lastsectbytes;
		} else {
			if(sect==lastsect)
				bytetotal = hdr.lastsectbytes;
			if(bytetotal>0) {
				status = Read_Sectors(buf, NumLSN-ERRORSECTOFF+sect, 1/*num_sectors*/);
				if(status)
					FatalMsg("InitErrorSectors: Read_Sectors status:%d", status);
			}
		}
		for( ;  bytetotal>0;  bytetotal--,bufp++) {
			if(!isprint(*bufp) && *bufp!='\n') {
				reinit = 1;
				WarnMsg("InitErrorSectors: (Re)Init'ing ErrorSectors: bad char(%d)",
						(int)*bufp);
				break;
			}
		}
	}
	if(reinit)
		ReInitErrorSectors(buf);

	ESectorCode = ESECT_OK;
}

/* doesn't save current sectnum being used (or sectnum==0) */
static void esectputc(char c, U32 * buf, int * sectnump, U32 * buf0) {
	int status;
	char * bufp;
	ESectHdr * hdr = (ESectHdr *)buf0;
	int lastsect = (hdr->firstsect + hdr->numsectused - 1) % ERRORSECTNUM;

	if(hdr->lastsectbytes>=BYTESPERSEC
	   || (lastsect==0 && hdr->lastsectbytes>=ESECT0MAXBYTES)) {
		if(lastsect!=0 && *sectnump==lastsect) {
			status = Write_Sectors(buf, NumLSN-ERRORSECTOFF+lastsect,
								   1/*num_sectors*/);
			if(status)
				FatalMsg("esectputc: Write_Sectors status:%d", status);
		}
		lastsect = (lastsect+1) % ERRORSECTNUM;
		if(hdr->numsectused==ERRORSECTNUM)
			hdr->firstsect = (hdr->firstsect+1) % ERRORSECTNUM;
		else
			hdr->numsectused++;
		hdr->lastsectbytes = 0;
	}
	if(lastsect==0)
		bufp = (char*)buf0 + sizeof(ESectHdr) + hdr->lastsectbytes;
	else {
		bufp = (char*)buf + hdr->lastsectbytes;
		if(lastsect!=*sectnump) {
			status = Read_Sectors(buf, NumLSN-ERRORSECTOFF+lastsect, 1/*num_sectors*/);
			if(status)
				FatalMsg("esectputc: Read_Sectors status:%d", status);
			*sectnump = lastsect;
		}
	}
	*bufp = c;
	hdr->lastsectbytes++;
}

/* doesn't save current sectnum being used (or sectnum==0) */
static void esectputstr(const char * str, U32 * buf, int * sectnump, U32 * buf0) {
	char c;

	for( ; (c=*str)!=0; str++) {
		if(isprint(c) || c=='\n')
			esectputc(c, buf, sectnump, buf0);
		else {
			char tmp[24];

			nsprintf(tmp,24,"<<BAD CHAR:%3d>>", (int)c); /* must all pass isprint() */
			esectputstr(tmp, buf, sectnump, buf0); /* allows 1 level of RECURSION */
		}
	}
}

void esectprint(const char * str) {
	U32  buf0[LONGSPERSEC];
	U32  buf[LONGSPERSEC];
	U8 * bufp;
	int  status;
	char badchar = 0;
	int badcnt = 0;
	int sectnum = -1;

	/* turn interrupts off */
	if(ESectorCode!=ESECT_OK)
		return;
	ESectorCode = ESECT_INUSE;
	/* turn interrupts on */

	if(CheckESectorsHdr(buf0))
		ReInitErrorSectors(buf0);

	esectputstr(str, buf, &sectnum, buf0);
	esectputc('\n', buf, &sectnum, buf0);

	/* save current sectnum and buf0 */
	if(sectnum>0) {
		status = Write_Sectors(buf, NumLSN-ERRORSECTOFF+sectnum, 1/*num_sectors*/);
		if(status)
			FatalMsg("esectprint: Write_S(0x%x) status:%d devstat:%d (NumLSN:0x%lx) ",
					 NumLSN-ERRORSECTOFF+sectnum, status, ide_check_devstat(), NumLSN);
	}
	status = Write_Sectors(buf0, NumLSN-ERRORSECTOFF+0, 1/*num_sectors*/);
	if(status)
		FatalMsg("esectprint: Write_Sectors status:%d devstat:%d", status, ide_check_devstat());
	ESectorCode = ESECT_OK;
}
