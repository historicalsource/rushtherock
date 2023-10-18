#include "font.h"
#include "error.h"
#include "stdlib.h"

/* ----------------------------------------------------------------------------- */
static void ConfirmTexDef(const TEXDEF * des_texdef, const TEXDEF * act_texdef) {
	if(strcmp(des_texdef->Name, act_texdef->Name))
		WarnMsg("ConfirmTexDef: bad name found des:%s act:%s",
				 des_texdef->Name, act_texdef->Name);
	if(des_texdef->Width != act_texdef->Width)
		WarnMsg("ConfirmTexDef: bad Width found des:%d act:%d for %s",
				 des_texdef->Width, act_texdef->Width, act_texdef->Name);
	if(des_texdef->Height != act_texdef->Height)
		WarnMsg("ConfirmTexDef: bad Height found des:%d act:%d for %s",
				 des_texdef->Height, act_texdef->Height, act_texdef->Name);
}

/* ----------------------------------------------------------------------------- */
BLIT * NewFontBlit(const FONTTYPE ** opt_fontpp/*pass 0 to ignore*/,
				   const SFONTINFO * sfinfop,  int x, int y, char ch) {
	BLIT * blit;
	const FONTTYPE * fontp;
	const FONTINFO * finfop = sfinfop->finfop;

	blit = NewBlit(finfop->texdef.Name, x, y);
	ConfirmTexDef(&(finfop->texdef), blit->Info);
	fontp = RenameFontBlitChar(blit, sfinfop, ch);
	if(opt_fontpp)
		*opt_fontpp = fontp;
	return blit;
}

/* ----------------------------------------------------------------------------- */
const FONTTYPE * RenameFontBlit(BLIT * blit, int preserve, const SFONTINFO * sfinfop,
								char ch) {
	const FONTINFO * finfop = sfinfop->finfop;

	RenameBlit(blit, finfop->texdef.Name, preserve);
	ConfirmTexDef(&(finfop->texdef), blit->Info);
	return RenameFontBlitChar(blit, sfinfop, ch);
}

/* ----------------------------------------------------------------------------- */
static int fontcmp(const FONTTYPE * font1p, const FONTTYPE * font2p) {
	if(font1p->ch < font2p->ch)
		return -1;
	return (font1p->ch > font2p->ch);
}

/* ----------------------------------------------------------------------------- */
const FONTTYPE * RenameFontBlitChar(BLIT * blit,
									const SFONTINFO * sfinfop/*same as last time*/,
									char ch) {
	FONTTYPE 		tmpfont;
	const FONTTYPE 	*fontp;
	const FONTINFO 	*finfop = sfinfop->finfop;

	tmpfont.ch = ch;
	fontp = bsearch(&tmpfont, finfop->font, finfop->total, sizeof(FONTTYPE),
					(int (*) (const void *, const void *))fontcmp);
	if(!fontp) {
		WarnMsg("RenameFontBlitChar(%s,%c) FAILED", finfop->texdef.Name, ch);
		fontp = finfop->font + 0;
	}
	blit->Width  = fontp->width * sfinfop->scale_x + 0.5;    /* eg 0->255 (except width is a U8) */
	blit->Height = finfop->lheight * sfinfop->scale_y - 0.5; /* eg 0->255 (except lheight is a U8) */
	blit->Right  = fontp->x + fontp->width;                  /* eg 255->0 */
	blit->Left   = fontp->x; 						         /* eg 0->255 */
	blit->Top    = finfop->texdef.Height-1 - fontp->y;	     /* eg 0->255 */
	blit->Bot    = finfop->texdef.Height-1 - (fontp->y + finfop->lheight - 1);/*eg 255->0*/
#if 0
	/* KLUDGE */
	blit->Width ++;
	blit->Height ++;
#endif
	UpdateBlit(blit);
	return fontp;
}

/* ----------------------------------------------------------------------------- */
void InitSFontInfo(SFONTINFO * sfinfop, const FONTINFO * finfop) {
	sfinfop->finfop = finfop;
	sfinfop->next_x = 0;
	sfinfop->last_x = 0;
	sfinfop->last_y = 0;
	sfinfop->x_inc = 0; /* extra incrementing */
	sfinfop->y_inc = 0;
	sfinfop->start_x = 0;
	sfinfop->scale_x = 1.0;
	sfinfop->scale_y = 1.0;
	sfinfop->color = SFONTDEFCOLOR;
}

/* ----------------------------------------------------------------------------- */
static int calc_start_x(SFONTINFO * sfinfop,
						int x/*or use INCX,LASTX, or STARTX*/,
						int y/*or use INCY or LASTY*/, char ch) {
	int next_x;
	
	if(x==STARTX || ch=='\n')
		next_x = sfinfop->start_x;
	else if(x>=0) 
		next_x = x;
	else if (x==INCX)
		next_x = sfinfop->next_x + sfinfop->x_inc;
	else /* x==LASTX */
		next_x = sfinfop->last_x;
	return next_x;
}

/* ----------------------------------------------------------------------------- */
/* Works for font & for ' ' & '\n' */
/* Can use RenameFontBlit{Char}() on returned blits. */
/* Or just RemoveBlit & recall this function */
/* Placement of ch is at sfinfop->[last_x,last_y] (after call) */
/* return value of 0 if ch==' ' */
/* presently doesn't allow ' ' to be in the font, except via spacewidth */
BLIT * NewSFontBlit(SFONTINFO * sfinfop,
					int x/*or use INCX,LASTX, or STARTX*/,
					int y/*or use INCY or LASTY*/, char ch) {
	BLIT * blit;
	const FONTTYPE * fontp;
	
	
	sfinfop->last_x = sfinfop->next_x = calc_start_x(sfinfop, x, y, ch);
	
	if(y>=0 && ch=='\n')
		sfinfop->last_y = y + sfinfop->y_inc + sfinfop->finfop->lheight * sfinfop->scale_y + 0.5;
	else if (y==INCY || ch=='\n')
		sfinfop->last_y += sfinfop->y_inc + sfinfop->finfop->lheight * sfinfop->scale_y + 0.5;
	else if(y>=0)
		sfinfop->last_y = y;
	/* else y==LASTY */

	if(ch==' ') {
		blit = 0;
		sfinfop->next_x += sfinfop->finfop->spacewidth * sfinfop->scale_x + 0.5;
	} else if(ch=='\n') {
		blit = 0;
	} else {
		blit = NewFontBlit(&fontp, sfinfop, sfinfop->next_x, sfinfop->last_y, ch);
		if(sfinfop->color != SFONTDEFCOLOR) {
			blit->color = sfinfop->color;
			UpdateBlit(blit);
		}
		sfinfop->next_x += fontp->width * sfinfop->scale_x + 0.5;
	}
	return blit;
}

/* ----------------------------------------------------------------------------- */
/* Works for font & for ' ' & '\n' */
int NewSFontBlitPrint(BLIT ** opt_blitarrayp, int array_len, SFONTINFO * sfinfop,
					  int x/*or use INCX,LASTX, or STARTX*/,
					  int y/*or use INCY or LASTY*/,
					  const char * str) {
	int len = strlen(str);
	int cnt;
	BLIT * blit;

	if(!len)
		return len;
	blit = NewSFontBlit(sfinfop, x, y, *(str++));
	if(opt_blitarrayp && (array_len--)>0)
		*(opt_blitarrayp++) = blit;
	
	for (cnt=1;  cnt<len;  cnt++) {
		blit = NewSFontBlit(sfinfop, INCX, LASTY, *(str++));
		if(opt_blitarrayp && (array_len--)>0)
			*(opt_blitarrayp++) = blit;
	}
	return len;
}

/* ----------------------------------------------------------------------------- */
int NewSFontBlitPrintf(BLIT ** opt_blitarrayp, int array_len, SFONTINFO * sfinfop,
					   int x/*or use INCX,LASTX, or STARTX*/,
					   int y/*or use INCY or LASTY*/,
					   char const *fmt, ...) {
	char str[70];
	va_list ap;

	va_start(ap, fmt);
	nvfprintf(str, 70, fmt, ap);
	va_end(ap);

	return NewSFontBlitPrint(opt_blitarrayp, array_len, sfinfop, x, y, str);
}

/* ----------------------------------------------------------------------------- */
/* ignores Y value changes, centers based on start of 1st and end of last blit/char */
int NewSFontBlitCenterPrint(BLIT ** blitarrayp, int array_len, SFONTINFO * sfinfop,
							int x/*or use INCX,LASTX, or STARTX*/,
							int y/*or use INCY or LASTY*/,
							const char * str) {
	int start_x = calc_start_x(sfinfop, x, y, *str);
	int len = NewSFontBlitPrint(blitarrayp, array_len, sfinfop, x, y, str);
	int width = sfinfop->next_x - start_x;
	int i;
	BLIT ** blitp = blitarrayp;

	if(width<0)
		width = 0;
	else
		width /= 2;
	if(!blitarrayp)
		len = 0;
	if(len>array_len)
		len = array_len;
	for( i=len; i>0;  i--,blitarrayp++) {
		if(*blitarrayp) {
			(*blitarrayp)->X -= width;
			UpdateBlit(*blitarrayp);
		}
	}
	return len;
}

/* ----------------------------------------------------------------------------- */
int NewSFontBlitCenterPrintf(BLIT ** blitarrayp, int array_len,
							 SFONTINFO * sfinfop,
							 int x/*or use INCX,LASTX, or STARTX*/,
							 int y/*or use INCY or LASTY*/,
							 char const *fmt, ...) {
	char str[70];
	va_list ap;

	va_start(ap, fmt);
	nvfprintf(str, 70, fmt, ap);
	va_end(ap);

	return NewSFontBlitCenterPrint(blitarrayp, array_len, sfinfop, x, y, str);
}

/* ----------------------------------------------------------------------------- */
/* ignores Y value changes, justifies based on start of 1st and end of last blit/char*/
int NewSFontBlitRightJPrint(BLIT ** blitarrayp, int array_len, SFONTINFO * sfinfop,
							int x/*or use INCX,LASTX, or STARTX*/,
							int y/*or use INCY or LASTY*/,
							const char * str) {
	int start_x = calc_start_x(sfinfop, x, y, *str);
	int len = NewSFontBlitPrint(blitarrayp, array_len, sfinfop, x, y, str);
	int width = sfinfop->next_x - start_x;
	int i;
	BLIT ** blitp = blitarrayp;

	if(width<0)
		width = 0;

	if(!blitarrayp)
		len = 0;
	if(len>array_len)
		len = array_len;
	for( i=len; i>0;  i--,blitarrayp++) {
		if(*blitarrayp) {
			(*blitarrayp)->X -= width;
			UpdateBlit(*blitarrayp);
		}
	}
	return len;
}

/* ----------------------------------------------------------------------------- */
int NewSFontBlitRightJPrintf(BLIT ** blitarrayp, int array_len,
							 SFONTINFO * sfinfop,
							 int x/*or use INCX,LASTX, or STARTX*/,
							 int y/*or use INCY or LASTY*/,
							 char const *fmt, ...) {
	char str[70];
	va_list ap;

	va_start(ap, fmt);
	nvfprintf(str, 70, fmt, ap);
	va_end(ap);

	return NewSFontBlitRightJPrint(blitarrayp, array_len, sfinfop, x, y, str);
}
