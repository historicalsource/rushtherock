#ifndef _OS_PROTO_H_
#define _OS_PROTO_H_

/* $Id: os_proto.h,v 1.22 1997/08/29 17:57:33 shepperd Exp $ */

/*	Transitional os_proto.h. Prototypes for new (post-"great rename")
 *	O.S. routines. If "OBSOLETE" is #define'd, also includes macros
 *	to convert old names and calling conventions to new. In the few
 *	cases where this is not possible, a prototype for a "shim"
 *	routine is included instead. The shim routine will arrange the
 *	parameters and call the new routine. This will add a layer of
 *	subroutine call, and require that the relevant .c file also be
 *	compiled with "OBSOLETE" #define'd
 *
 *	The routines declared in this file are in alphabetical order. Because
 *	the Great Rename resulted in unique two- or three- letter prefixes
 *	for each general class of functions, this means they will also tend
 *	to be grouped by functional class. Each comment header also indicates
 *	the source-file name for a function. In some cases, this will be one
 *	of several files. for example, "*_txt.c" refers to the set of an_txt.c
 *	(vanilla Alphanumeric plane) and bm_txt.c (Bit-map text) so far,
 *	some future possiblilities of pf_txt (Rasteroids-style playfield text)
 *	and mo_txt.c (MMII-style motion-object text.)
 */

#ifndef GREAT_RENAME
#define OBSOLETE (1)
#else
#define OBSOLETE (0)
#endif

#ifndef __STDC__
#define __STDC__ (0)
#endif

#if __STDC__
#define PARMS(x) x
#else
#define PARMS(x) ()
#define const
#define volatile
#endif

/*		aud_excw(code,buf,cnt)
 *	Handle an exception-mode transfer, in the "one byte out, 'cnt' bytes in"
 *	way.
 *	Returns:
 *		0 if exception in progress or sound processor is in reset
 *		1 if exception complete or under way
 */
extern int aud_excw
PARMS((
    int	__code,		/* one of S_* sound codes gotten from Audio group */
    void *__buf,	/* return value buffer pointer			*/
    int	__cnt		/* # of expected return values			*/
));
#if OBSOLETE
#define com_exc(code,buf,count) aud_excw((code),(buf),(count))
#endif

/*		aud_f_put(code)					(audio.c)
 *	Attempts to send <code> to the sound processor (or internal sound
 *	process.) This routine is intended for internal diagnostic use, or
 *	as the assembly language "hook" by which other parts of audio.c
 *	access particularly wierd hardware. Its use by a game progam is
 *	deprecated, and it may be deleted in future GUTS.
 *	Returns:
 *		0 if sound process (from reset) or port is currently busy
 *		!=0 if code sent
 */
extern int aud_f_put
PARMS((
    int __code		/* one of S_* sound codes gotten from Audio group */
));
#if OBSOLETE
#define com_fwrt(code) aud_f_put(code)
#endif

/*		aud_init(level)					(audio.c)
 *	Resets audio processor (or process) and varying amounts of
 *	local state, depending on <level>.
 *
 *		If level is 0, reset the sound process[or] and any crucial
 *	state (e.g. coin variables) but do not wait for the sound processor
 *	to "come up". 
 *	Returns: 0x10000 (1 sound, all status bits OK.)
 *
 *		If level is < 0, reset as for 0, but leave the sound
 *	process[or] "dead" if possible. Not all configurations can do this,
 *	but it is provided as sort of a "finished with sounds" call for
 *	those that can, and would benefit in some way from having the sounds
 *	processor stopped. 
 *	Returns: 0x10000 = 1 sound and all status bits OK.
 *
 *		If level > 0, reset as for 0, but also wait for the sound
 *	process[or] to "come up", then return a "status" and the "number of 
 *	sounds".  
 *	Returns:
 *		-1 = sound processor did not respond or more than 32768 sounds
 *		0 = user aborted (there are always 1 sound STOP, NOISY, ...)
 *		0xnnnnssss = nnnn sounds and status of ssss.
 */
extern long aud_init
PARMS((
    int __level		/* see above */
));

/*		aud_irq()					(audio.c)
 *	Interrupt service routine for sound process or processor.
 *	this is typically called from the assembly "root" after registers
 *	have been saved. It is only included in this file for the bizarre
 *	cases where a game may need to "hijack" a shared interrupt.
 */
extern void aud_irq();

/*		aud_mput(buff,cnt)
 *	Adds <cnt> unsigned shorts (aud_t's) in buffer pointed to by <buff>
 *	to queue of "sounds" to play. <buff> is assumed to contain _one_
 *	complete command, so the first aud_t will have D15 set, and the
 *	remainder will have D15 cleared.
 *	Also, this routine will place the whole buffer, or none of it, on the
 *	queue.
 *		Note that the <param> are masked with 0x7FFF
 *	to insure that they are not treated as commands.
 *	Returns:
 *		0 if no room (i.e. not sent)
 *		<cnt> if queued
 */
extern int aud_mput
PARMS((
    unsigned short *buff,
    int cnt
));

/*		aud_poll(usec)
 *	Check on sound processor status, using usec as the amount of time 
 *	since last check.  For queued sounds without CAGE style interrupts
 *	this routine will send a sound too.
 *	Return:
 *		1 if a sound has been sent
 *		0 if nothing to output 
 *		-1 if sound system should not be running from last reset
 *		-2 if sound subsystem has just required reset 
 */
extern int aud_poll
PARMS((
    unsigned long __usec	/* # of micro seconds since last call	*/
));

/*		aud_adj_volume( display, attract )
 *	This is the hook provided to allow a game to use the
 *	Williams volume-control buttons during game play. In
 *	a less non-ideal world, this would be automatically
 *	called from aud_poll(), but many sound subsystems don't
 *	provide a particularly useful aud_poll(), and most games
 *	don't call it anyway. Since we are making this a separate
 *	routine, we give it two parameters:
 *
 *	<display> a pointer to a display function which will be
 *		 "back-called" to do whatever this month's bizarre
 *		 video hardware requires to display a visual
 *		 indication of the volume. It takes two parameters:
 *	  <volume> in whatever odd units the sound subsystem uses.
 *	  <action> to indicate what to do:
 *		AAV_HIDE -> remove display from screen
 *		AAV_UPDATE -> update existing display
 *		AAV_SHOW -> add display to screen (informational)
 *
 *		 If <display> is zero, volume will still be
 *		 fiddled, but nothing will show on the screen.
 *
 *	<attract> An indication of whether we are setting the
 *		  attract (!=0) or game (==0) volume. This could
 *		  in principle be pried out of the internals of
 *		  the sound subsystem, but in practice letting
 *		  the game specify it is a lot easier.
 */
enum aud_adjv_opt { AAV_HIDE = 0, AAV_UPDATE = 1, AAV_SHOW = 2 };

extern void aud_adj_volume
PARMS((
    void (*__display)(int __volume, int __action),
    int __attract
));

/*		aud_put(code)
 *	Mainly a new name for com_wrt(). Tries up to MAX_AUD_TRIES
 *	times to write <code> to sound port. In newer versions of GUTS,
 *	this is simply a synonym for aud_q_put(), as all sound commands
 *	are queued.
 *	Returns:
 *		0 if failed
 *		1 if sent
 *		2 if on queue (if MAX_AUD_TRIES == 0)
 */
extern int aud_put
PARMS((
    int __code		/* one of S_* sound codes gotten from Audio group */
));

/*		aud_q_get()
 *	Called to receive asyncronous input from audio process (e.g. reports
 *	of position in a tune)
 *	Returns:
 *		-1 if no input available
 *		else data from port
 */
extern long aud_q_get
PARMS((
void
));
#if OBSOLETE
#define com_qrd() aud_q_get()
#endif

/*		aud_qlen()
 *	Returns the length of the output queue. This is scalled so that
 *	0xFFFF is completely full, 0x8000 is half-full, etc, so that the
 *	game programmer does not have to know what the max queue size is.
 */
extern int aud_qlen
PARMS((
void
));

/*		aud_q_mput(num,code,param,...)			(audio.c)
 *	Adds <code> to queue of "sounds" to play and adds num-1 parameters
 *	to the queue as well.  Note that the <param> are masked with 0x7FFF
 *	to insure that they are not treated as commands. This was an
 *	experimental addition by Ed Logg, and may or may not continue to
 *	be supported. As such, its use is deprecated in favor of aud_mput()
 *	which guarantees atomicity and does not require a (possibly buggy)
 *	stdarg implementation to work.
 *	Returns:
 *		0 if no room (i.e. something was not sent)
 *		1 if sent 
 *		2 if on queue
 */
extern int aud_q_mput
PARMS((
    int __num,		/* number of sounds and/or arguments to send */
    ...			/* one of S_* sound codes gotten from Audio group */
    			/* num-1 parameters to pass after masking by 0x7FFF */
));


/*		aud_q_put(code)		 			(audio.c)
 *	Adds <code> to queue of "sounds" to play. On a system with
 *	external Audio, <code> will then be written to the port communicating
 *	with the sound processor. On a host-based audio system, <code> will
 *	be placed directly in the "action" queue of the sound process. 
 *	Returns:
 *		0 if no room
 *		1 if sent 
 *		2 if on queue.
 */
extern int aud_q_put
PARMS((
    int __code		/* one of S_* sound codes gotten from Audio group */
));
#if OBSOLETE
#define SoundCall(__code) aud_q_put(__code)
#define com_wrt(__code) aud_q_put((__code))
#endif

/*		aud_reset()					(audio.c)
 *	Does whatever needs to be done to reset the sound processor (or
 *	process) and initialize the minimum state need to prevent errors
 *	such as spurious coins.
 */
extern void aud_reset();
#if OBSOLETE
#define res_6502() aud_reset()
#define ResetSoundProcess() aud_reset()
#endif

/*		aud_setvol(volume)				(audio.c)
 *	Set the volume of sound.  A parameter of -1 will set the
 *	volume to the value stored in EEPROM. 
 *	Returns:
 *		-1 for an error
 *		<volume> that was set
 */
extern int aud_setvol
PARMS((
    int __volume	/* volume index: MIN_VOLUME <= __volume <= MAX_VOLUME */
));
#if OBSOLETE
#define SetVolume(level) aud_setvol(level)
#endif

/*		cn_bong()					(coin.c)
 *	returns (and clears) number of pending "bongs", that is, the number
 *	of coins that have arrived since last called. This routine calls 
 *	cn_check() to update pending_bongs (and other coin-related stuff),
 *	then returns/clears pending_bongs.
 */
extern int cn_bong();

/*		cn_check()					(coin.c)
*	Checks for any change in the number of coins seen, and propagates
*	it to credits, coins, bonus coins, etc.  Returns non-zero if any
*	new coins were processed. It _also_ increments pending_bongs if
*	so. The routine cn_bong() returns the number of pending bongs
*	(New coins) and clears it.
*
*	This routine _MUST_NOT_ be called from interrupt level (you know
*	who you are!) as it must not be re-entered. it is the single point
*	where consistency is maintained.
*
*	Since there is no real reason for a game program to call this, its
*	use is deprecated in favor of cn_bong().
*/
extern int cn_check();

/*		cn_chkcoin()					(coin.c)
 *	Essentially duplicates the checking of cn_dbcoin() (below), without
 *	doing the decrement. Returns 0 if insufficient coins, 1 if sufficient.
 */
extern int cn_chkcoin
PARMS((
    int __number		/* number of coins needed */
));


/* 		cn_click_vendor()				(coin.c)
*	This routine drives a coin counter like vendor as defined by the 
*	definition VEND_M if there is one.
*/
#ifdef	VEND_M
extern void cn_click_vendor();
#endif


/*		cn_coin_counters()				(coin.c)
*	The following stuff added to deal with "host"-driven electromechanical
*	counters. This was first done on BATMAN. The user needs to define
*	two macros: COUNTER_ON and COUNTER_OFF that do the appropriate
*	magic to turn the specified counter on or off respectively. The
*	array cctim contains "timers" for up to four mechs. Any
*	mapping from mech number to electro-mechanical counter is up to
*	the aforemention macros. 
*
*	This routine must be called every 16ms.  
*
*	Coin counts may be lost if the number of coins exceeds 16 while the
*	counter is busy counting earlier counts.
*/
extern void cn_coin_counters();


/* 		cn_coins()					(coin.c)
*   	Returns -1 if called before cn_init();
*   	Returns 0 if called in free play;
*	else returns the number of credits and bonus credits.
*/
extern int cn_coins();


/* 		cn_continues()					(coin.c)
 *	Return the current credit "balance" (see cn_credits()) as used for
 *	continues rather than whole games.
 *	Returns -1 (0xFFFFFFFF) if called before cn_init();
 */
extern long cn_continues();


/* 		cn_credits()					(coin.c)
 *	Return the current "balance", Because we cannot expect Game programmers
 *	to properly declare this one, and GreenHills is iffy on structure-valued
 *	functions. This returns a signed long which is packed as follows:
 *
 *	D31-16	Whole credits
 *	D15-8	Numerator of partial credits
 *	D7-0	Denominator of partial credits
 *
 *	Returns -1 (0xFFFFFFFF) if called before cn_init();
 */
extern long cn_credits();


/* 		cn_cvt_crdt(value)				(coin.c)
*	Convert coin-inputs to credits. Called with U32 <value> containing 
*	2-bit switch bits. Actually, they are counter "deltas", i.e. 
*	differences between old and new values for each mech.
*	The coin counters appear as 2-bit groups (modulo 4). They are
*	_supposed_ to be in the order:
*
*	D1,0 -	Right-most ([n])
*	D3,2 -
*	D5,4 -  (Right coin in a two mech door)
*	D7,6 -	Left-most ([0])
*
*	But this can be changed by re-mapping via MECH_ORDER.
*
*	This routine now only bumps the counters in irq_cnt, but the name is
*	kept to simplify external interface. It is the _ONLY_ routine in this
*	package which may be called from the IRQ routine. It must _not_ be
*	re-entered.
*
*	It supports an arbitrary number of 'fields' by using MECH_ORDER but
*	currently only allows EXACTLY four distinct counter/switches.
*
*	As a vestige of early host-read coin-switch routines, the use of
*	this routine is deprecated in favor of cn_add_coin(), which is
*	not in this file either because neither routine should be randomly
*	called by game software. Those who really need to muck with coins
*	at this level are encouraged to read gel_coin.c, coin94.c, and coin.h
*/
#if (HAS_TWO)
extern unsigned long cn_cvt_crdt
#else
extern void cn_cvt_crdt
#endif
PARMS((
    unsigned long __delta			/* change in coins */
));


/* 		cn_dbcoin(number)				(coin.c)
 *	Subtract (if possible) <number> coins. This routine is provided so
 *	games which give a discount for "continue" can deduct less than one
 *	whole "credit". The recomended number of coins for such a case can
 *	be gotten by calling cn_discount(), which returns 0 to mean:
 *	"no discount, charge a whole credit". Like cn_debit(), returns 1
 *	for success, 0 for failure.
 */
extern int cn_dbcoin
PARMS((
    int __number		/* number of coins to subtract */
));


/*		cn_debit(number)				(coin.c)
 *	Subtract <number> of credits. Returns 1 if there were enough credits
 *	(and removes them), else returns 0 and leaves credits alone
 */
extern int cn_debit
PARMS((
    unsigned int __number	/* number of credits to subtract */
));


/*		cn_discount() 					(coin.c)
 *	Returns 0 if there is no discount for a continued game, else returns
 *	The default price of such a game ((new_game_price + 1)/2).
 *	(returns -1 if called before cn_init() )
 */
extern int cn_discount();

/*		cn_init()					(coin.c)
 *	Initialize the coin_handling software state. Must be called
 *	_after_ eer_init(), as it needs to get the coin option "switches".
 *
 *	As this routine is called by the power up code before calling the
 *	game, its use by random game programs is deprecated. It is included
 *	in this file for historical reasons and may soon disappear.
 */
extern int cn_init
PARMS((
));


/*		cn_irq()					(coin.c)
*
*	This routine looks at the coin inputs every 4mSec, and does the
*  	basic validation of coin pulses. The criteria are the same as the 6502
*  	coin routine.  A valid coin is a 16-800 mSec contact closure preceded
*  	and followed by at least 33 mSec of contact open. Bounce at the edges
*  	of these periods is ignored, but the 16mSec minimum must be 4 solid 
*	samples.  This routine replaces the assembly program and cn_cvt_crdt().
*
*	A private variable set by cn_init() is tested to insure that no coins
*	count while in self test. This routine is only present for hardware that
*	uses the host to read coin inputs. Such hardware places more stringent
*	than usual demands on the game programmer _NOT_ to turn interrupts
*	off for long periods of time. In this case "long" is anything over
*	two milliseconds.
*
*	In the absence of really wierd hardware, this routine should not need
*	to be called by the game. It is included in this file to allow for
*	those cases where a game program has to "hijack" a shared interrupt.
*	
*/
extern void cn_irq();


/*		cn_next(contin)					(coin.c)
 *	Returns the number of COINS needed to reach one full credit.
 *	Returns zero if a full credit is already available. If <contin>
 *	is non-zero, uses discount price (if any) rather than full price.
 */
extern int cn_next
PARMS((
    int __contin	/* non zero if asking about coins to continue */
));


/* 		cn_price()					(coin.c)
 *	Return the price of a credit - 0-8 coins, ( or -1 if called before
 *	cn_init()
 */
extern int cn_price();


/*		cn_total_coins()				(coin.c)
*	Returns total _value_ of coins recorded in EEPROM. This _assumes_
*	that the mech-multipliers have not been changed since the stats
*	were cleared.
*/
extern unsigned long cn_total_coins();


/* 		cn_total_creds()				(coin.c)
*	Returns total credits, which is to say, total coins divided by
*	current price of a game. Needless to say, this also depends on the
*	price not having changed since the stats were cleared
*/
extern unsigned long cn_total_creds();


/*		cn_zap_bonus()					(coin.c)
 *	Clear all partial or full bonus coins. This is _supposed_ to be
 *	called at the start (or end) of a game, because mktg lives in fear
 *	that someone, somewhere, will get the benefit of someone elses
 *	partial bonus...
 */
extern void cn_zap_bonus();

/*		ctl_autorepeat()				(ctl_sw.c)
 *	Set one or more control switches to fake edges periodically.
 */
extern void ctl_autorepeat
PARMS((
    unsigned long __switches, /* Bit mask of switches to autorepeat */
    int 	__initial,   /* Number of frames before first auto-closure */
    int 	__later	/* Number of frames between subsequent auto-closures */
));

/*		ctl_read_debug(edges)				      (joy_sw.c)
 *	Functionally equivalent to ctl_read_sw() (below), but refers to the
 *	debug switches, rather than player controls. Newer versions of GUTS
 *	will automatically suppress the reading of these switches and return
 *	0 under the control of an EEPROM (or other) option.
 */
#define GUTS_OPT_DBG_SW (4)	/* nominal bit for "debug switches active"	../os/st_proto.h */

extern unsigned long 
ctl_read_debug
PARMS((
    unsigned long __edges	/* Mask of which switches show only edges */
));
#if OBSOLETE
#define debug_sw(edges) ctl_read_debug((edges))
#endif

/*		ctl_read_sw(edges)				(ctl_sw.c)
 *	Returns the state of the game controls as switches. A binary 1
 *	corresponds to an asserted switch (or newly asserted switch).
 *
 *	If the bit corresponding to a switch is set in <edges>, it is
 *	interpreted as a request for an "edge detect" read, rather than a
 *	stable debounced value. Such switches will be read as asserted only
 *	if they have been released since the last such "edge detect" reading.
 *
 *	If the bit corresponding to a switch is clear in <edges> the current
 *	debounced value is returned. A non-edge read of a switch has no effect
 *	on the state of edge detection for that switch.
 *
 *	All debouncing and edge detection depends on the routine ctl_upd_sw()
 *	being called periodically, most usually from the VBlank routine.
 *
 *	Games which do not have the canonical "joystick and buttons" user
 *	interface nonetheless have this routine, which synthesizes a set
 *	of switches from whatever controls (Analog joystick, Trackball,...)
 *	do exist.
 */
extern unsigned long ctl_read_sw
PARMS((
    unsigned long __edges	/* Mask of which switches show only edges */
));

/*		ctl_upd_sw()
 *	Polls switches and whatever controls is used for "synthetic joystick".
 *	Returns debounced switches (equivalent to ctl_read_sw(0)). Should
 *	be called about every 4-20 milliseconds. The self-test vblank
 *	routine calls it once a frame.
 */
extern unsigned long ctl_upd_sw();
#if OBSOLETE
#define sf_swtch() ctl_upd_sw()
#endif

/*		ctl_read_quad(index)				(swt_test.c)
 *	Returns the number of quadrature ticks for the appropriate
 *	quadrature port since the last time the function was called.
 *
 *	valid values for 'index' are 0 through QUAD_CNT-1.
 */
extern int ctl_read_quad
PARMS((
    int __index 		/* index of quadrature to read */
));

/*		ctl_upd_quad()
 *	Polls quardature ports and accumulates ticks. Sould be called
 *	at least once per field unless the quardature h/w is higher resolution
 *	in which case should be called more often (perhaps as often as once
 *	per millisecond).
 */
extern int ctl_upd_quad
PARMS((
    void
));

/*		eer_bumph(table,bin,bump)			(eedrive.c)
 *	Older "hook" to allow game-maintained histograms. Adds <bump> to
 *	indicated <bin> of histogram <table>. If the result is > 255, scales
 *	all "sibling" tables and the current table by shifting all bins in
 *	these tables down one bit.
 *
 *	NOTE: A previous version of this routine used to get "brothers" as its
 *	last parameter. This one uses the information from TIE_HISTOGRAMS
 *	entry in stat_map.mac.
 *
 *	OBSOLETE! For any histogram which actually represents a monotonic
 *	one-dimensional quantity (e.g. Game time) You should be using the
 *	routine eer_tally_hist()
 */
extern int eer_bumph
PARMS((
    int	__table,	/* which histogram table to modify */
    int __bin,		/* which bin to increment */
    int __bump		/* how much to increment by */
));


/*		eer_busy()					(eedrive.c)
 *	Returns 0 if all EEPROM writes are complete, else non-zero.
 *	Typically called in a loop with a prc_delay0() to wait until it is
 *	safe to switch from game to self-test or vice-versa. If any EEPROM
 *	"user records" are "checked out" at the time this is done, the
 *	game will wait forever. Judicious use of the routines eer_user_wrt(),
 *	eer_user_free(), eer_user_flush(), and eer_user_purge() is highly
 *	recommended.
 */
extern int eer_busy();


/*		eer_clrhist(table)				(eedrive.c)
 *	Clear histogram table <table>
 *	Returns -1 for bogus table number, else 0;
 */
extern int eer_clrhist
PARMS((
    unsigned int __table	/* which histogram to clear (HIST_*) */
));

/*		eer_end(player, contin)				(stats.c)
 *	Logs the end of a new (contin = 0) or continued (contin = 1)
 *	game for the specified player.
 */
extern void eer_end
PARMS((
    unsigned int __player,
    unsigned int __contin
));


/*		eer_geth(table,bin)				(eedrive.c)
 *	Return the value of the specified bin of the specified table.
 *	All valid values will be positive. A negative return value
 *	indicates an error (non-existant table or bin)
 */
extern int eer_geth
PARMS((
    unsigned int __table,		/* one of HIST_* */
    unsigned int __bin		/* a valid bin number in table */
));


/*		eer_gets(stat_num)				(eedrive.c)
 *	Returns value of statistic identified by <stat_num>. Valid
 *	statistic numbers are of the form "EER_*", and are defined
 *	in the file eer_defs.h, which is automatically generated from
 *	stat_map.mac. And invalid <stat_num> will cause -1 to be returned.
 */
extern int eer_gets
PARMS((
    unsigned int __stat_num	/* one of EER_* */
));


/*		eer_hist_line(bin, buff, buflen)		(eedrive.c)
 *	Format a histogram line into caller's <buffer>, of maximum length
 *	<buflen>. This routine uses static values saved by a previous call to
 *	eer_hist_title() (below.). It returns the value of the bin, or -1
 *	in case of error.
 */
extern int eer_hist_line
PARMS((
    unsigned int __bin,		/* which bin of current histogram */
    char *	 __buff,	/* user's buffer, to store text */
    int		 __buflen	/* maximum size of caller's buffer */
));


/*		eer_hist_title(table, buf, buflen, maxpt, medpt, bar)(eedrive.c)
 *	Formats title of histogram <table> in caller's buffer <buf>, whose
 *	maximum length is <buflen>. If <maxpt> is non-null, the maximum bin
 *	value with be stored at *maxpt. Similarly, if <medpt> is non-null, the
 *	bin number containing the median value will be stored at *medpt.
 *
 *	The histogram bar will be painted with the characters from <bar> if
 *	If <bar> is non-null, it will be saved for subsequent use in painting
 *	the histogram bars by eer_hist_line().
 *
 *	Returns length of title, or -1 = error.
 *
 *	This routine is intended to be used in conjunction with eer_hist_line
 *	and must be called before any call to eer_hist_line. It sets up the
 *	"state" needed for formatting the individual lines.
 */
extern int eer_hist_title
PARMS((
  unsigned int	__table,	/* which histogram (HIST_*) */
  char 		*__buf,		/* buffer to receive title string */
  int 		__buflen,	/* length of buf */
  int 		*__maxpt,	/* pointer for optional return of maximum */
  int 		*__medpt,	/* pointer for optional return of median */
  const char	*__bar		/* characters to use in painting bar graph */
));


/*		eer_hstr(rank,table)				(eedrive.c)
 *	Returns a pointer to a static struct containing the re-constituted
 *	entry for entry <rank> in High Score Table <table>
 *	Returns the null pointer if given bad params.
 */
extern struct hst_ent *eer_hstr
PARMS((
    unsigned int __rank,	/* Which rank (0 is highest) to read */
    unsigned int __table	/* Which High Score Table (HST_*) */
));


/*		eer_hstw(rank,entptr,table)			(eedrive.c)
 *	Packs and writes data from struct hst_ent at <entptr> in High Score
 *	Table <table> at <rank>. Returns 0 if bad params. Otherwise inserts
 *	entry, pushing any higher numbered (lower scores) entries down and
 *	returns non-zero.
 */
extern int eer_hstw
PARMS((
    unsigned int 	__rank,		/* rank of entry to insert */
    struct hst_ent 	*__entptr,	/* un-packed form of entry to insert */
    unsigned int 	__table		/* Which High Score Table (HST_*) */
));


/*		eer_hwt()					(eedrive.c)
 *	The "handler" for EEPROM. Will probably be called during VBlank.
 *	Must not be called more frequently than	once every 10 milliseconds.
 *	Lots of places in SYSTEM_1 stat routines assume	that a "tick" of
 *	eer_rtc (maintained by this routine) is exactly 1/60th of a second,
 *	so BEWARE. In most cases, this will be called from erc_vbex().
 *
 *	This routine will in most cases be called by the GUTS root code,
 *	of possibly as part of erc_vbex(). It is included in this file for
 *	historical reasons, but should not usually be called by a game
 *	program, especially more than once a frame!
 */
extern void eer_hwt();


/*		eer_incs(stat_num,value)			(eedrive.c)
 *	Adds <value> to statistic counter at position <stat_num> (one of
 *	EER_*) Returns -1 for bogus params. Otherwise returns resulting sum.
 */
extern int eer_incs
PARMS((
    unsigned int __stat_num,	/* which counter to increment (EER_*) */
    unsigned int __value	/* Amount to add to selected counter */
));


/*		eer_p_incs(stat_num,value)			(eedrive.c)
 *	Adds <value> to statistic counter at position <stat_num> (one of
 *	EER_*) Returns -1 for bogus params. Otherwise returns resulting sum.
 *	Pins incremented value at 255.
 */
extern int eer_p_incs
PARMS((
    unsigned int __stat_num,	/* which counter to increment (EER_*) */
    int		 __value	/* Amount to add to selected counter */
));


/*		eer_init()					(eedrive.c)
 *	Called to initialize the shadows from EEPROM, and also to re-write
 *	them if any errors were detected. If the user has defined a stat
 *	named EER_ERRORS, the value is returned. Otherwise the return is 0.
 *
 *	Called by the power up code before calling the game. Game programmers
 *	should never need to call this routine. Its inclusion in this file
 *	is for historical reasons.
 *	
 */
extern int eer_init();


/*		eer_opt(menu)					(stats.c)
 *	Set game options using a menu as described in stats.c
 *	This routine is primarily included for "side door" access for
 *	games which want to "take over" the self-test operator screens.
 *	It is not particularly useful otherwise and its use by game
 *	programs is deprecated.
 */
extern void eer_opt
PARMS((
    const unsigned char *menu
));


/*		eer_play(who)					(stats.c)
*	Starts and stops various game timers. <who> contains a bit for
*   	each active player (or other timer). D0 means the first player is
*	active, D1 the second, etc. By convention, (1<<N_PLAYERS) is a timer
*	for a "session", which is the time from the entry of the first player
*	to the exit of the last, for games in which players may enter and exit
*	at will. eer_play(0) should be called periodically during attract mode
*	to accurately accumulate "time powered on". Once each pass through the
*	attract sequence should suffice. As stats.c is a real game-programmer
*	playpen, your particular version of eer_play() may or may not conform
*	to this spec. You have been warned.
*
*	This routine actually returns a pointer to an array of unsigned longs
*	named timers[]. For an explanation of this array, see the #defines 
*	in stats.c.  Just to get you started, the first (T_0PTIME) is time
*	for 0 players (idle).  These timers are used (and sometimes modified)
*	by other routines in stats.c, but should not be lightly messed with.
*
*/
extern unsigned long *eer_play
PARMS((
    unsigned int __player_mask
));

/*		eer_puth(table,bin,value)			(eedrive.c)
 *	Store <value> in selected <bin> of histogram <table> (one of
 *	HIST_*). Returns -1 for parameter error, else <value>.
 *	OBSOLETE, most histograms should use eer_tally_hist() and eer_clrhist()
 */
extern int eer_puth
PARMS((
    unsigned int __table,	/* which histogram (HIST_*)	*/
    unsigned int __bin,		/* which bin			*/
    int 	 __value	/* New value			*/
));


/*		eer_puts(stat_num,value)
 *	Write <value> at stat position selected by <stat_num> (one of the
 *	constants EER_* defined in eer_defs.h (generated from stat_map.mac.)
 *	Returns -1 for erroneous input, else 0;
 */
extern int eer_puts
PARMS((
    unsigned int __stat_num,		/* One of "EER_*" */
    unsigned int __value		/* Value to put in statistic */
));


/*		eer_rank(score,table)				(eedrive.c)
 *	Returns the rank that <score> "deserves" in High Score Table <table>.
 *	Returns -1 if score does not qualify. This will be interpreted as a
 *	very big unsigned number if passed to eer_hstw(), so it will be safe.
 *	Read the source if you want ties to go to the new guy.
 */
extern int eer_rank
PARMS((
    unsigned long __score,		/* higher is better */
    unsigned int __table		/* one of HST_* */
));


/* 		eer_rtc
 *	This is incremented every time eer_hwt() is called. It is used by
 *	various routines in stats.c to time various events.
 */
extern volatile unsigned long eer_rtc;

/*		eer_start(player), eer_stop(player)
 *	Simply calls eer_play() with the appropriate playmask to add/drop
 *	<player> to the set of active players. Note in particular that
 *	eer_stop() (or eer_play() with a zero in the appropriate bit) is
 *	_not_ the same as eer_end(). These routines simply stop a player's
 *	timer, e.g. while paused waiting for coins to continue. They do
 *	not "end the game."
 */
extern unsigned long *eer_start
PARMS((
    unsigned int __player
));
extern unsigned long *eer_stop
PARMS((
    unsigned int __player
));

/*		eer_stats(alter)				(stats.c)
 *	Display games statistics. If <alter> is non-zero, allow clearing.
 *	This is provided so a game can include a "secret handshake" to
 *	display the stats without going into selftest. Programmers who allow
 *	alteration when called via secret handsahke will be shot and asked
 *	to leave the company.
 */
extern void eer_stats
PARMS((
    int alter
));


/*		eer_tally_hist(table,value)			(eedrive.c)
 *	Increment appropriate bin of <table> for <value>. This routine "knows"
 *	all about max and min values and offset and siblings ads nauseum, so
 *	should be used for any histogram that actually represents a range of
 *	monotonically increasing values. NOTE: "siblings" may get scaled.
 */
extern int eer_tally_hist
PARMS((
    unsigned int __table,		/* which histogram (HIST_*) */
    unsigned int __value		/* value for this event */
));

/*		erc_vbex()
 *	Does a variety of tasks nominally done every vblank, although
 *	they do not strictly need to be done _during_ vblank. Just pretty
 *	nearly every 60th of a second (for the game timers) and no two
 *	calls less than 10 milliseconds apart (for the EEPROM). The
 *	somewhat arcane name comes from EeRom and Coin Vertical Blank Exit.
 */
extern void erc_vbex();
 

/*		net_poll()
 *	Check, and possibly log or display, various net statistics
 *	such as rec'd checksum errors. The parameter is an estimate
 *	of the number of microseconds since the previous call. For
 *	example, a game running at a 30Hz frame rate would call as:
 *	net_poll(33333);
 *
 *	Return:
 *		0 if net OK
 *		<0 for fatal errors
 *		>0 for number of accumulated errors since last call
 *		(not yet implemented)
 */
extern int net_poll
PARMS((
    int usec	/* how long (in microseconds) since last call */
));

/*		pot_age()
 *	Age pots to reduce limits.
 */
extern void pot_age
PARMS((
));

/*		pot_init(<level>)
 *	Primarily called by selftest at power-on. If <level> is 0, reset
 *	pot limits to those in EEPROM (if available). If <level> != 0,
 *	reset limits to "nothing known".
 */
extern void pot_init
PARMS((
	int __level
));

/*		pot_linear(potnum,play,width,fuzz)
 *	Returns a scaled pot value in the range -width to +width, using
 *	linear (as opposed to parabolic) scaling.  The pot to read is
 *	selected by <potnum>.  <play> specifies the number of values
 *	around zero to ignore (count as zero).  <fuzz> specifies the
 *	number of values at the max or min to ignore (treat as +/- width.)
 *
 *	Modified 2APR96 to accept a _negative_ width, and to interpret
 *	that as a request to return an unsigned valuefrom 0 to |width|,
 *	with <play> indicating the number of values to ignore at the
 *	low end, and <fuzz> indicating the number of values to ignore
 *	at the high end.  Note that this interpretation will return a
 *	total of (|width|+1) values, while the original routine returned
 *	(2*width) values.  I didn't write it :-) MEA
 *
 *	Although the use of a _signed_ width to indicate an _unsigned_
 *	return value is strange, it was done in the name of backward
 *	compatibility.  All previous games which used this code (or
 *	its ancestors) had centered controls.  Most previous driving
 *	games came out of Applied Research, so they did all pot-
 *	massaging in the game code.
 */
extern int pot_linear
PARMS((
    int __potnum,	/* Which pot */
    int __play,		/* Ignore this many "counts" around zero */
    int __width,	/* Scale value to +/- width */
    int __fuzz		/* ignore this many counts at outer edge */
));

/*		pot_parabolic()
 *	A value is returned from -width to +width.  The play is the
 *	number of values around the middle to ignore (ie. treat as
 *	if the middle).
 */
extern int pot_parabolic
PARMS((
    int __potnum,	/* Which pot */
    int __play,		/* Ignore this many "counts" around zero */
    int __width		/* Scale value to +/- width */
));

/*		pot_raw()
 *	An unscaled value is returned for the pot specified by <potnum>.
 */
extern unsigned short pot_raw
PARMS((
    int __potnum	/* Which pot */
));

/*		pot_save()
 *	Stores current filter limits for A->D conversion in EEPROM.
 *	This should be called periodically to track long-term drift.
 *	When we get reliable hardware/software, this routine and pot_age()
 *	will remain as stubs for a year or so. Their use is currently,
 *	unfortunately, required.
 */
extern void pot_save
PARMS((
));

/*		prc_cache_init(cache_on)
 *	Initializes cache (in processor and board specific way), and
 *	conditionally leaves it enabled.  If <cache_on> is non-zero,
 *	cache will be enabled after call. If zero, it will be disabled.
 *
 *	There _should_ be no reason for a game program to call this
 *	routine. The power-up code will initailize the cache and
 *	enable it prior to calling the game.
 */
extern void prc_cache_init
PARMS((
    int __cache_on		/* != 0 -> leave cache on, ==0 -> turn off */
));

/*		prc_delay(fields)				(os_root.asm)
 *	Wait until at least <fields> Vblanks have occured. In particular,
 *	delay(0) will return as soon after VBlank as practical. Care is
 *	usually taken to have prc_delay() function correctly even if interrupts
 *	are masked (possibly by polling.) A video field is usually 16.66...
 *	milliseconds in duration (60Hz).
 */
extern void prc_delay
PARMS((
    int	__fields	/* how many video fields to wait */
));

/*		prc_delay0()					(os_root.asm)
 *	Wait until the next Vblank occurs.  Care is
 *	usually taken to have prc_delay0() function correctly even if interrupts
 *	are masked (possibly by polling.) A video field is usually 16.66...
 *	milliseconds in duration (60Hz).
 */
extern void prc_delay0
PARMS((
));

#if (VIDEO_BOARD == SST_V)
/*		prc_delay_options(flags)			(phx_shims.c)
 *	Sets option bits for the default prc_delay() function to use.
 *	Returns previous options flags.
 */
extern unsigned long prc_delay_options
PARMS((
    unsigned long opts 	/* an 'or' of the following desired options */
));

#define PRC_DELAY_OPT_TEXT2FB	1	/* Write text in fake screen to frame */
#define PRC_DELAY_OPT_SWAP	2	/* swap the buffer and wait */
#define PRC_DELAY_OPT_CLEAR	4	/* clear the buffer _after_ swapping */
#endif

/*		prc_mod_latch(change)
 *	"Safely" modifies the multi-bit latch found on many game boards.
 *	if the MSBit of <change> is cleared, any '1' bits in <change> will
 *	be set in the latch (and shadow). If the MSBit is cleared, any '0'
 *	bits in <change> will be cleared in the latch. This is to allow
 *	prc_mod_latch(~BITS_TO_CHANGE) to easily specify bits to clear.
 *	any non-specified bits are left alone, or rather set to their value
 *	in the internally maintained shadow.
 */
extern int prc_mod_latch
PARMS((
    int __change		/* bits to set/clear per MSBit */
));
#if OBSOLETE
#define mod_latch(changes) prc_mod_latch(changes)
#endif

/*		prc_panic(msg)
 *	Called when something that "can't happen" does. <msg> should be
 *	a pointer to a string explaining just what happened. Many games
 *	will attempt to display the message, as well as possibly a stack
 *	dump. During development it is more common to halt the processor,
 *	so as to keep as much traceback in the analyzer as possible.
 */
extern void prc_panic
PARMS((
    const char *__msg		/* explanatory message to possibly display */
));
#if OBSOLETE
#define panic(msg) prc_panic(msg)
#endif
struct act_q {
    struct act_q *next;
    struct act_q *que;
    void (*action)();
    void *param;
};
/*		prc_q_action()
 *	Adds an element to the "action queue". This is how a hardware
 *	interrupt causes some "software interrupt" to be executed. The
 *	parameter is a pointer to a struct act_q which must be allocated
 *	by the user and "live" from the time of its creation to the time
 *	the specified action is called. If the element is already on some
 *	other queue, it will not be added to the action queue, and a pointer
 *	to the head of the other queue will be returned. Otherwise returns 0.
 *	
 */
struct act_q *prc_q_action
PARMS((
    struct act_q *__q_elt
));

/*		prc_reboot()
 *	Initializes processor/game-board state as if a RESET had occured.
 *	The simplest impleenation will just shut off interrupts (via
 *	prc_set_ipl(INTS_OFF)) and wait for Watchdog to hit. Games which
 *	log watchdog resets will probably want to do something more elaborate
 *	to prevent spurious counts. This routine is intended to be the means
 *	for switching from game to selftest and vice-versa.
 */
extern void prc_reboot();

/*		prc_set_ipl(new_ipl)				(os_root.asm)
 *	Sets processor Interrupt Priority Level. Returns previous value,
 *	so it can be used as:
 *
 *	int old_ipl;
 *	...
 *	old_ipl = prc_set_ipl(INTS_OFF);
 *	...				( code that must not be interrupted )
 *	(void) prc_set_ipl(old_ipl);
 *
 *	Appropriate constants for INTS_ON and INTS_OFF should be defined in
 *	config.mac (for config.h). Any additional intermediate values should
 *	also be defined there as symbolic constants.
 */
extern int prc_set_ipl
PARMS((
    int __new_ipl	/* new level (or mask) */
));
#if OBSOLETE
#define SetIPL(level) prc_set_ipl(level)
#endif

/*
 *		utl_rand(in)					(utilasm.asm)
 *
 *	Returns a random number between 0 and in. seed is	
 *	kept locally, in a static variable, so need not be 	
 *	provided. On the other hand, multiple uses of utl_rand(in)	
 *	will not be truly independent, nor can the sequence	
 *	be restarted. 						
 *
*/
extern unsigned long seed;
extern int 	utl_rand
PARMS((
    int		__in	/* return range				*/
));


/*
 *	Basic GUTS timer queue...				(time.c)
 */

/*	structure used for simple timer queue, consists of a queue
 * element for the action queue, plus a delta time from the previous
 * timer queue entry (if any) to this one.
 */
struct tq {
    struct tq *next;
    struct tq *que;		/* should point to __time_q if on timer queue */
    void (*func)(void *);
    void *vars;
    unsigned long delta;
};

/*		tq_maint
 *	Simple timer "delta Q" hook. The user must call this periodically,
 *	with the parameter usec indicating a "best guess" at how many
 *	microseconds have elapsed since the last call. This routine will
 *	be called from the hardware timer interrupt routine _unless_ that
 *	vector has been "hijacked", so it should not be of interest to anyone
 *	not doing so.
 *		This code locks out interrupts, so that no timer events can
 *	be added or deleted by any code running at a higher hardware priority
 *	than itself. If this becomes an issue due to the inevitable increase
 *	in latency, it could be avoided if we can guarantee that no timer
 *	events will be posted by any code that could execute at a higher
 *	hardware interrupt level at all.
 */
void
tq_maint
PARMS((
    unsigned long usec
));

/* 		tq_ins()
 *	Insert an entry on the timer queue, which will cause an "action"
 *	to occur "delta" microseconds in the future. The caller must
 *	allocate the queue entry (in memory that will "live" until it
 *	times out), and fill in the delta, func, and vars fields.  The
 *	last two need only be filled in once, if the same element will
 *	be used for the same action repeatedly. This routine will not
 *	insert the same entry twice.  The delta field will be adjusted
 *	by tq_ins() to account for any earlier events in the queue, so
 *	should not be "watched" by user code.
 *
 *	Returns 0 if successful, -1 if unsuccessful.
 */
int
tq_ins
PARMS((
    struct tq *tqp
));

/*		tq_del()
 *	Delete an un-expired entry from the timer queue.
 *
 *	Returns 0 if successful, -1 if unsuccessful (entry not found
 *	on queue.)
 */
int
tq_del
PARMS((
    struct tq *tqp
));


/*
 *	Character output stuff...				(text.c)
 */

/*		txt_width(txt,set)
 *	Return width of string in <col> units for string to
 *	be displayed in 'set'.
 */
extern int	txt_width
PARMS((
    const char	*__txt,	/* text of message to output			*/
    int		__set	/* Character Set selection			*/
));
#if OBSOLETE
#define bstrlen(text) txt_width((text),BIG_SET))
#endif


/*		txt_height(txt,set)
 *	Return height of string in <row> units for string to
 *	be displayed in 'set'.
 */
extern int	txt_height
PARMS((
    const char	*__txt,	/* text of message to output			*/
    int		__set	/* Character Set selection			*/
));


/*		txt_stamp(col,row,stamp,pal)
 *	Output a single alpha "stamp" at (col,row). No character set
 *	translation is performed, and no indication of error is returned.
 */
extern void txt_stamp
PARMS((
    int		__col,	/* coordinates to place stamp */
    int 	__row,	/* (0,0) is upper left corner of screen		*/
    const int	__stamp,/* stamp number (AN_*) to output		*/
    int		__pal	/* Palette (NOT Character Set) selection	*/
));
#if OBSOLETE
#define cwrite(col,row,stamp,pal) txt_stamp((col),(row),(stamp),(pal))
#endif


/*		txt_str(col,row,txt,pal)			(text.c)
 *	Position "cursor" and output string. Sets internal variables
 *	<col>,<row> to position after output, and returns number of
 *	characters output.
 *	If <row> == -1, continue at stored row and column. If <col> = -1,
 *	center text on specified row.
 */
extern int	txt_str	
PARMS((
    int		__col,	/* coord. of UL corner of first character of txt */
    int 	__row,	/* (0,0) is upper left corner of screen		*/
    const char	*__txt,	/* text of message to output			*/
    int		__pal	/* Palette and Character Set selection		*/
));
#if OBSOLETE
#define pmess(col,row,txt,pal) txt_str((col),(row),(txt),(pal))
#define pbmess(col,row,txt,pal) txt_str((col),(row),(txt),(pal)|AN_BIG_SET)

/*	The obsolete routine pwrite() is similar, except that there is
 *	no "cursor" and no "magic" value for row or col.
 */
extern int	pwrite
PARMS((
    int		__col,	/* coord. of UL corner of first character of txt */
    int 	__row,	/* (0,0) is upper left corner of screen		*/
    const char	*__txt,	/* text of message to output			*/
    int		__pal	/* Palette and Character Set selection		*/
));
#endif


/*		txt_clr_str(col,row,txt,pal)			(text.c)
 *	Clears the string as drawn above.
 */
extern void	txt_clr_str
PARMS((
    int		__col,	/* coord. of UL corner of first character of txt */
    int 	__row,	/* (0,0) is upper left corner of screen		*/
    const char	*__txt,	/* text of message to output			*/
    int		__pal	/* Palette and Character Set selection		*/
));
#if OBSOLETE
#define erase_text(col,row,txt) txt_clr_str((col),(row),(txt),0)
#endif


/*		txt_clr_wid(col,row,wid)
 *	Clears the screen area at (row,col) for <wid> width.
 *	Row and col are interpreted as if by txt_str()
 */
extern void txt_clr_wid
PARMS((
    int __column,	/* horiz posn, or -1 to center */
    int __row,		/* vert posn */
    int __width		/* stamps to clear */
));
#if OBSOLETE
#define clear_text(col,row,width) txt_clr_wid((col),(row),(width))
#endif


/*		txt_cstr(txt,pal)
 *	Output string at present <col>,<row>. Sets internal variables
 *	<col>,<row> to position after output.
 */
extern int	txt_cstr
PARMS((
    const char	*__txt,	/* text of message to output			*/
    int		__pal	/* Palette and Character Set selection		*/
));
#if OBSOLETE
#define mess(txt,pal) txt_cstr((txt),(pal))
#endif


/*		txt_decnum(col,row,val,wid,for,pal)
 *	Position "cursor" and output decimal number. Sets internal variables
 *	<col>,<row> to position after output, and returns number of characters
 *	output.
 *	If <row> == -1, continue at stored row and column. If <col> = -1,
 *	center text on specified row.
 */
extern int	txt_decnum
PARMS((
    int		__col,	/* coord. of UL corner of first character of txt */
    int 	__row,	/* (0,0) is upper left corner of screen		*/
    unsigned long __val, /* value to output				*/
    int		__wid,	/* Size of output field				*/
    int		__for,	/* one of:
    			 * RJ_ZF (0) Right Justify with ZERO Fill
    			 * RJ_BF (1) Right Justify with BLANK Fill
    			 * LJ_BF (2) Left Justify with BLANK Fill
    			 * LJ_NF (3) Left Justify with NO Fill
    			 *						*/
    int		__pal	/* Palette and Character Set selection		*/
));
/*
 *	FORMAT parameters to txt_[dec/hex]num routines
 */
#define	RJ_ZF	(0)	/* Right Justify with ZERO fill		*/
#define	RJ_BF	(1)	/* Right Justify with BLANK fill	*/
#define	LJ_BF	(2)	/* Left Justify with BLANK fill		*/
#define	LJ_NF	(3)	/* Left Justify with NO fill		*/


#if OBSOLETE
extern int	pdec
PARMS((
    int		__col,	/* coord. of UL corner of first character of txt */
    int 	__row,	/* (0,0) is upper left corner of screen		*/
    unsigned long __val, /* value to output				*/
    int		__wid,	/* Size of output field				*/
    int		__for,	/* one of:
    			 * RJ_ZF (0) Right Justify with ZERO Fill
    			 * RJ_BF (1) Right Justify with BLANK Fill
    			 * LJ_BF (2) Left Justify with BLANK Fill
    			 * LJ_NF (3) Left Justify with NO Fill
    			 *						*/
    int		__pal	/* Palette and Character Set selection		*/
));
#define pdecnum(col,row,val,wid,form,pal) \
    txt_decnum((col),(row),(val),(wid),(form),(pal))
#define decnum(val,wid,form,pal) \
    txt_decnum(0,-1,(val),(wid),(form),(pal))
#endif


/*		txt_cdecnum(val,wid,for,pal)
 *	Output decimal number at present <col>,<row>. Sets internal variables
 *	<col>,<row> to position after output.
 */
extern void	txt_cdecnum
PARMS((
    unsigned long __val, /* value to output				*/
    int		__wid,	/* Size of output field				*/
    int		__for,	/* one of:
    			 * RJ_ZF (0) Right Justify with ZERO Fill
    			 * RJ_BF (1) Right Justify with BLANK Fill
    			 * LJ_BF (2) Left Justify with BLANK Fill
    			 * LJ_NF (3) Left Justify with NO Fill
    			 *						*/
    int		__pal	/* Palette and Character Set selection		*/
));


/*		txt_hexnum(col,row,val,wid,for,pal)
 *	Position "cursor" and output hexadecimal number. Sets internal
 *	variables <col>,<row> to position after output, and returns number
 *	of characters output.
 *	If <row> == -1, continue at stored row and column. If <col> = -1,
 *	center text on specified row.
 */
extern int	txt_hexnum
PARMS((
    int		__col,	/* coord. of UL corner of first character of txt */
    int 	__row,	/* (0,0) is upper left corner of screen		*/
    unsigned long __val, /* value to output				*/
    int		__wid,	/* Size of output field				*/
    int		__for,	/* one of:
    			 * RJ_ZF (0) Right Justify with ZERO Fill
    			 * RJ_BF (1) Right Justify with BLANK Fill
    			 * LJ_BF (2) Left Justify with BLANK Fill
    			 * LJ_NF (3) Left Justify with NO Fill
    			 *						*/
    int		__pal	/* Palette and Character Set selection		*/
));
#if OBSOLETE
extern int	phex
PARMS((
    int		__col,	/* coord. of UL corner of first character of txt */
    int 	__row,	/* (0,0) is upper left corner of screen		*/
    unsigned long __val, /* value to output				*/
    int		__wid,	/* Size of output field				*/
    int		__for,	/* one of:
    			 * RJ_ZF (0) Right Justify with ZERO Fill
    			 * RJ_BF (1) Right Justify with BLANK Fill
    			 * LJ_BF (2) Left Justify with BLANK Fill
    			 * LJ_NF (3) Left Justify with NO Fill
    			 *						*/
    int		__pal	/* Palette and Character Set selection		*/
));
#define phexnum(col,row,val,wid,form,pal) \
    txt_hexnum((col),(row),(val),(wid),(form),(pal))
#endif


/*		txt_chexnum(val,wid,for,pal)
 *	Output hexadecimal number at present <col>,<row>. Sets internal
 *	variables <col>,<row> to position after output.
 */
extern void	txt_chexnum
PARMS((
    unsigned long __val, /* value to output				*/
    int		__wid,	/* Size of output field				*/
    int		__for,	/* one of:
    			 * RJ_ZF (0) Right Justify with ZERO Fill
    			 * RJ_BF (1) Right Justify with BLANK Fill
    			 * LJ_BF (2) Left Justify with BLANK Fill
    			 * LJ_NF (3) Left Justify with NO Fill
    			 *						*/
    int		__pal	/* Palette and Character Set selection		*/
));
#if OBSOLETE
#define hexnum(val,wid,form,pal) txt_chexnum((val),(wid),(form),(pal))
#endif

#if (PROCESSOR == M68000 || PROCESSOR == M68010)
extern int ul_over_us
PARMS((
    unsigned long __ul,		/* Numerator */
    unsigned int __us		/* Denominator, must come from unsigned short */
));
#else
#define ul_over_us(num,den) (((unsigned long)(num))/((unsigned short)(den)))
#endif
/*		txt_setpos()
 *	Used to save/restore the "cursor position" for
 *	text output (used by txt_c*(), or txt_*() with row < 0)
 *	Takes a "magic cookie" of zero (default postion) or
 *	the value returned by a previous call. Returns a
 *	"magic cookie" encoding the current position.
 */
extern unsigned long txt_setpos
PARMS((
    unsigned long newpos	/* Magic cookie, do not open */
));

/*		utl_cdec(val,buff,wid,form)
 *	Convert a decimal number to a text string. <wid> specifies the visible
 *	width. <buff> must be at least <wid>+1 long accomodate the '\0' that
 *	will be stored. <form> is a format code, as defined above at the
 *	description for txt_decnum(). Returns the number of _actual_ digits.
 *	This may be larger than <wid>, although in this case only the least
 *	significant digits will be in <buff>.
 */
extern int	utl_cdec
PARMS((
    unsigned long __val, /* value to convert				*/
    char	*__buff, /* where to place text				*/
    int		__wid,	/* Size of output field				*/
    int		__form	/* RJ_ZF, et al. See above			*/
));
#if OBSOLETE
#define cdec(val,buff,wid,form) utl_cdec((val),(buff),(wid),(form))
#endif


/*		utl_chex(val,buff,wid,form)
 *	Convert a hex number to a text string. <wid> specifies the visible
 *	width. <buff> must be at least <wid>+1 long accomodate the '\0' that
 *	will be stored. <form> is a format code, although only RJ_ZF and RJ_BF
 *	work. That is, if <form> is RJ_ZF (0), the result will be zero-filled,
 *	otherwise it will be blank-filled. Returns the number of digits stored.
 *	This is unlike utl_cdec() although for both routines only the
 *	least significant digits will be in <buff>.
 */
extern int	utl_chex
PARMS((
    unsigned long __val, /* value to convert				*/
    char	*__buff, /* where to place text				*/
    int		__wid,	/* Size of output field				*/
    int		__form	/* 0 (RJ_ZF) for zero-fill, != 0 for blank fill	*/
));
#if OBSOLETE
#define chex(val,buff,wid,form) utl_chex((val),(buff),(wid),(form))
#endif


/*		vcr(on_off)
 *	on_off != 0 for RECORD
 *	on_off == 0 for STOP
 */
extern void vcr
PARMS((
    int		__on_off
));


/*		vid_clear()
 *	Clears the screen.
 */
extern void vid_clear();
#if OBSOLETE
#define ClearScreen() vid_clear()
#endif

/*		vid_fields( frame_rate )
 *	Establish the frame rate, for such hardware as needs special
 *	treatment to switch buffers, show the current screen, etc.
 *	if <frame_rate> == 0, that part of the video (e.g. GXn video RAM)
 *	will be turned off. Otherwise, <frame_rate> specifies the number
 *	of fields (vertical traces of the CRT screen) to display for each
 *	frame (unique visual image) of video. <frame_rate>s from 0..4
 *	should be supported on any hardware. Video hardwares that do not
 *	have such a concept can simply return 0.
 */
extern int vid_fields
PARMS(( int frame_rate		/* fields per nominal frame (0 means off) */
));

/*		vid_init()
 *	Initializes video circuitry, including clocks, color RAM,
 *	Clearing the "primary" screen, and setting up VAD, MOB, etc.
 */
extern void vid_init();
#if OBSOLETE
#define init_video() vid_init()
#endif

/*		vid_master()
 *	On games with more than one video board and no auto-genlock,
 *	such as GT, we need to elect one board as "master". This routine
 *	is the hardware-independant way of doing so. If its parameter is
 *	non-zero, this board will assert its "master status". In any case
 *	it returns the previous status. All that is guaranteed is zero or
 *	non-zero. It is almost certainly not a good idea for game code
 *	to be mucking with this, but TMEK does already :-(
 */
extern int vid_master PARMS((
int me		/* non-zero to assert mastertude */
));

/*		vid_set_bg()
 *	Set the background color, if possible. Returns the previous Background
 *	color. This is a routine because some hardware systems have write-only
 *	registers for the background, and we need to know the previous value
 *	to flash the screen for, e.g. guns.
 */
extern int vid_set_bg
PARMS((
    int	_bgcolor	/* New color for background */
));

/*		vid_set_pal()
 *	Set a range of entire in the hardware color loop-up table.
 */
extern void vid_set_pal
PARMS((
   void *palptr,		/* pointer to color information to set */
   int start_color,	/* starting entry in hardware CLUT to set */
   int num_colors	/* number of entries in hardware CLUT to set */
));

/*		vid_set_vmode()
 *	This routine is (so far) specific to CoJag hardware, although
 *	the name would fit several different possibilities on other
 *	hardware. In any case, it is included because the VMODE register
 *	on the Jaguar is write-only, and several pieces of the video
 *	code need to know what mode is being used. Returns previous mode.
 *	If <new_mode> is zero, it is ignored, so one can simply "get"
 *	the current mode. The _usefull_ non-zero values are:
 *
 *	VIDEO_MODE_CRY, 	Jaguar's unique contribution to colorimetry
 *	VIDEO_MODE_RBG5,	Jaguar's unique order Red, Blue, Green
 *	VIDEO_MODE_DEFAULT,	Should be one of the above.
 */
extern int vid_set_vmode
PARMS((
    int new_mode
));

/*		vid_set_vb()
 *	establish a routine to be called at the beginning of every vertical
 *	blanking interval. returns a pointer to the previous routine.
 */
extern void (*vid_set_vb
PARMS((
    void (*new_vblank)()
))
)();
#if OBSOLETE
#define set_vblank(rtn) vid_set_vb((rtn))
#endif

/*		vid_waitvb(edge)
 *	Waits for edge of Vertical Blanking signal and returns.
 *	if parameter <edge> is zero, waits for leading edge, else
 *	waits for trailing edge. In any case, does whatever is needed
 *	to keep the vertical blank signal recurring. If no vertical blank
 *	is sensed within approximately 20 Milliseconds, returns 0, else 1.
 */
extern int vid_waitvb
PARMS((
    int __edge		/* zero for leading edge, else await trailing edge */
));

#endif			/* _OS_PROTO_H_ */
