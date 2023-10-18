#if !defined(_PHX_PROTO_H_)
# define _PHX_PROTO_H_ 1

/* $Id: phx_proto.h,v 1.39 1997/10/06 17:21:49 shepperd Exp $ */

/* The following functions can be found in phx_root.mac */

extern void startup(void);		/* reset vector */
extern void boot_entry(void);		/* entry to reset code */
extern void except(int);		/* exception handler entry */
extern U32 prc_enable_cp1(void);	/* turns on the FPU. Returns old SR */
extern U32 prc_set_fpcsr(U32);		/* set the FPU CSR to value. Returns old value */
extern U32 prc_get_fpcsr(void);		/* get the current FPU CSR */
extern U32 prc_get_cause(void);		/* get the current cause register */
extern U32 prc_set_cause(U32);		/* set the cause register (returns old value) */
extern U32 prc_get_ipl(void);		/* get the current SR register */
extern int prc_set_ipl(int);		/* set the IMASK and IE bits in the SR register */
extern int prc_set_sr(int);		/* set the processor status register */
extern U32 prc_timer_rate(U32);		/* set the processor COMPARE register. Returns old value */
extern U32 prc_get_count(void);		/* get the counter register */
extern U32 prc_set_count(U32);		/* get the counter register */
extern U32 prc_get_compare(void);	/* get the compare register */
extern U32 prc_set_compare(U32);	/* get the compare register */
extern void prc_reset_hardware(void);	/* reset all the h/w on the board */
extern void prc_wait_n_usecs(int);	/* wait at least specified microseconds */
extern void prc_blink_led(int led, int time); /* Blink leds */
extern void prc_flush_pdcache(const char *buff, int cnt); /* flush partial data cache */
extern void prc_inv_pdcache(const char *buff, int cnt); /* writeback and invalidate partial data cache */
extern int prc_isie(void);		/* returns 1 if interrupts are enabled, else returns 0 */
#if INCLUDE_SYSCALL
extern int prc_syscall(int which, ...);	/* executes a syscall instruction */
#endif

/* The following function can be found in phx_audio.c */

extern int aud_dyn_adjvol(int which_volume);

/* The following functions can be found in phx_shims.c (among those already prototyped in os_proto) */

extern U32 *prc_timer_ptr(U32 *);	/* set pointer to counter variable */
extern void stub_start(void);		/* restarts GUTS */
extern U32 prc_timer_ints(void);	/* returns number of timer interrupts that have occured */
extern const char *get_pm_msg(int cause);	/* returns string ID'ing the cause for exception */
extern U32 prc_get_exception_time( int );	/* returns accumulated interrupt counts and... */
						/* ...clears counts if parameter is non-zero */
extern void prc_timer_jobs(int);	/* called by Vsync IRQ to fake a timer interrupt */
extern struct act_q *prc_add_q(struct act_q *head, struct act_q *new);
extern void prc_swint0(void);		/* issue software interrupt 0 */
extern void prc_swint1(void);		/* issue software interrupt 1 */
extern int prc_get_actlvl(void);	/* get the current 'action' level (0=not at action level) */
extern VU32 pc_at_interrupt;		/* saved PC at first level interrupt */
#if INCLUDE_SYSCALL
# ifndef MAX_SYSCALLS
#  define MAX_SYSCALLS	16
# endif
extern int (*prc_set_syscall(int which, int (*func)(void *a, ...)))(void *a, ...);
#endif

#ifndef MAX_AST_QUEUES
# define MAX_AST_QUEUES 1
#endif

#if MAX_AST_QUEUES
extern int prc_get_astlvl(void);	/* get the current AST level (-1 = not at ast level) */

/*		prc_q_ast(level, new)
 *	Adds an element to the "AST queue" and launches an AST immediately if
 *	not already executing at AST level. 'level' is the AST level at which
 *	to run (from 0 to MAX_AST_QUEUES). The higher the level, the lower
 *	the priority. Level 0 is the highest priority just below 'actions'.
 *	AST functions will never interrupt one another and, as with actions,
 *	are executed in the order they appear in the queue. The queues
 *	are serviced according to the level. Higher level AST's will not be
 *	executed until all the lower level queues are empty. An AST may
 *	queue another AST, but the new one won't begin executing until the
 *	current AST exits (priority permitting).
 *
 *	IRQ action functions (those queued by prc_q_action()) will interrupt
 *	and execute ahead of an AST at any level.
 *
 *	The parameter 'new' is a pointer to a struct act_q which must be
 *	allocated by the user and "live" from the time of its creation to
 *	the time the specified action is called. If the element is already on
 *	some other queue, it will not be added to the AST queue. Returns 0
 *	if action successfully queued, 1 if level is out of range (too small),
 *	2 if level is out of range (too large) or 3 if 'new' is already
 *	present on some other queue.
 *	
 */
extern int prc_q_ast(int level, struct act_q *new);
#endif

/* The following six variables are reserved for use by Xinu.
 */
extern struct act_q *(*prc_q_action_vec)(struct act_q *new);
extern void (*process_action_q_vec)(int, int);
extern int (*prc_q_ast_vec)(int level, struct act_q *new);
extern void (*process_ast_q_vec)(int, int);
extern volatile int _guts_inest, _guts_astlvl;

/* The following functions can be found in phx_stubs */

extern int phx_puts(const char *);
extern int phx_putc(int);

/* The following function can be found in phx_ide.c */

extern int ide_init(void);

/* The following functions can be found in idtc4000 */

extern void flush_dcache(void);
extern void flush_cache(void);

/* The following functions can be found in mallocr */

#include <reent.h>

int _heap_remaining_r(struct _reent *rep); /* figure out how much free memory is available */
int _heap_size_r(struct _reent *rep);	   /* return size of sbrk'able area */
int heap_remaining(void);		   /* return size of free memory from _impure_ptr */
void _reset_malloc_r(struct _reent *rep);  /* reset malloc area to initial conditions */
void reset_malloc(void);		   /* reset malloc area (use _impure_ptr) */
unsigned char *prc_extend_bss(int amt);	   /* extend bss by 'amt'. Returns pointer to extended area */
extern struct _reent *mainline_reent;	   /* Pointers to various reent structs */
extern struct _reent *interrupt_reent;
extern struct _reent *ast_reent;
extern struct _reent *action_reent;
extern struct _reent *qio_reent;
extern struct _reent *xinu_reent;

/* The following functions can be found in sst_vid and/or sst_text */

typedef volatile struct timer_pll {
    int prev_scan;		/* scan line at last timer interrupt */
    int ticks;			/* ticks since last VB */
    U32 frame;			/* current frame */
    U32 field;			/* current field */
    U32 field_last;		/* field # of the last frame interrupt */
    int fields_frame;		/* number of fields/frame */
    int frame_overrun;		/* number of times buffer swaps came too late */
    int swap_in;		/* swap requested */
    int swap_out;		/* swap performed */
    int nxt_upd;		/* ints to next call to ctl_upd_sw */
    int ints_upd;		/* interrupts per call to ctl_upd_sw */
    int no_vsync;		/* != 0 if no functioning vsync interrupts */
    int ticks_line;		/* timer ticks per line */
    int lines;			/* lines per field */
    int ticks_field;		/* timer ticks per field */
    int fields_sec;		/* fields per second */
    int vlines;			/* visible lines */
} TimerPll;

extern TimerPll *get_timer_pll(void);

extern void sst_bufswap(void);		/* does a grBufferSwap() and also lets interrupt routine know */

extern int vid_field_rate(void);	/* returns number of fields per second */

/*		txt_select(which_method)
 *	Use txt_select() to select the method of getting text
 *	to the screen. Returns previous setting.
 */
extern int txt_select( int );
#define TXT_NONE (0)	/* Ignore text output */
#define TXT_HOST (1)	/* Draw directly to screen */
#define TXT_VSCREEN (2)	/* Draw only to the virtual screen */

typedef struct txt_alpha_ptr {
    U16 *screen;	/* pointer to array of U16's containing the chars to display */
    U32 *dirty;		/* pointer to array of dirty bits */
    U32 texture_offset;	/* offset into texture memory for alpha's */
    U32 color_offset;	/* offset into the color RAM for start of alpha palette */
} TxtAlphaPtr;

/*		txt_vsptr(TxtAlphaPtr *new)
 *	3DFX GUTS has a Virtual Screen into which the txt functions
 *	write their data. The contents of the virtual screen are converted
 *	to bitmaps which are written directly to the frame buffer either
 *	a pixel at a time or blocks of pixels at a time depending on the
 *	color palette.
 *
 *	The parameter to this function is a pointer to an AlphaPtr
 *	structure that contains a pointer to the screen (an array of U32's
 *	AN_VIS_ROW*AN_VIS_COL/2, 2 characters per entry due to a hardware
 *	restriction in the mathbox where this entry is very likely to point),
 *	a pointer to the array of dirty bits (an array of U32's each bit
 *	representing a character on the virtual screen and each U32 containing
 *	one half of a row of characters), an offset into the texture memory
 *	to the start of the alpha textures and an offset into the coloram to
 *	the start of the alpha palette.
 *
 *	txt_vsptr returns a pointer to the previously set value or 0 if
 *	nothing previously set. If the new parameter is 0, it only returns
 *	the previous value.
 */
extern TxtAlphaPtr *txt_vsptr(TxtAlphaPtr *new);

/*		sst_text2fb(flag)
 *	Unpacks the converts the contents of the virtual screen (VS, as
 *	pointed to by the value set in a previously called txt_vsptr())
 *	to bitmap data and writes it into the frame buffer. The 
 *	flag, contains an 'or' of one or more of the T2FB_FLAG_xxx bits.
 *
 */
#define T2FB_FLAG_SWAP	(1<<0)	/* swap buffers after rendering (not used) */
#define T2FB_FLAG_CLEAR (1<<1)	/* erase character from VS after rendering */
#define T2FB_FLAG_FIFO  (1<<2)	/* check for room in FIFO before writing */
extern void sst_text2fb(int flag);

/*		vid_clr_alphas()
 *	This function simply erases the contents of the virtual
 *	screen if there is one.
 */
extern void vid_clr_alphas(void);

/*		sst_reset()
 *	This function resets the 3dfx hardware and restarts it.
 *	It will take as much as 0.5 seconds to complete and the
 *	sync signals to the monitor will be turned off for a
 *	short period of time. The effect is to completely reset
 *	the 3dfx chip set. The monitor picture may blink and roll
 *	due to the interruption of sync.
 */
extern void sst_reset(void);

/*		sst_text_init()
 *	Establishes a virtual text screen for the 3dfx. The
 *	parameters texture_off and color_off are unused at this
 *	time and should be set to 0. Eventually they may be the
 *	offsets into the texture memory for the alphanumeric textures
 *	should someday they be written using triangles instead of
 *	direct writes into the bitmap. This function is automatically
 *	called at boot.
 */
extern int sst_text_init(unsigned int texture_off, unsigned int color_off);

#endif		/* _PHX_PROTO_H_ */
