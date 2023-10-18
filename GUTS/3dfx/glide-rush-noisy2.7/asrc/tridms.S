/****************************************************************************/
/*                                                                          */
/* trirush.S - Source for _grDrawTriangleNonAA glide library function.       */
/*		Lifted some stuff from trinew.S written by Mike Lynch	    */
/*		at Williams Entertainment, Inc.				    */
/*                                                                          */
/* Written by:  David Shepperd						    */
/* Copyright (c) 1997 by Atari Games, Corp.				    */
/* All rights Reserved                                                      */
/*                                                                          */
/* Use, duplication, or disclosure is strictly forbidden unless approved,   */
/* in writing by Atari Games, Corp.					    */
/*                                                                          */
/****************************************************************************/

/****************************************************************************
 * If the CPP variable TIME_IT is non-zero, some timing code will be included
 * in the function (timing it will, of course, have an adverse affect on
 * performance). 6 global variables are updated:
 *
 * dms_t_min - minimum time
 * dms_t_max - maximum time
 * dms_t_avg - accumulation (average computed by sombody else)
 * dms_t_count - number of drawn triangles
 * dms_t_culled - number of triangles dismissed due to "area" below limit
 * dms_t_reject - number of rejected triangles due to bad input paramters
 *
 * The times are in processor counts (1/2 CPU clock) as obtained from the
 * CPU's count register (the count is accurate regardless of pipline or
 * other CPU stalls).
 *
 * NOTE: This code is not likely to work worth a damn on a 4k. It hasn't
 * been tested on a 4k and I do not recommend anyone even try it.
 *
 * Theory of operations:
 *
 * My observation is that it takes about ~300 nanoseconds to write any size
 * parameter, up to 16 bytes, to the PCI bus (~600ns for a cache line write).
 * In our case, START, DX and DY are written for Alpha, S, T and W for a
 * total of 18 paramters plus the triangle start command. The goal was to
 * reduce the number of writes to the PCI.
 *
 * The CACHE flavor is stright line coded. What I do is disable interrupts,
 * turn on all 32 FP regs, load up all parameters into the regs (in case the
 * cache line we want overlaps one or more of the input parameter structs)
 * and claim a cache line (flush, mark dirty exclusive) that lies on top of
 * the last 8 parameter registers of the 3dfx. After doing this, I do no
 * data accesses to DRAM which will prevent the cache line that was just
 * claimed from unexpectedly getting swiped out from under me. I write the
 * X, Y and the first 4 parameters via sd's as they are computed then write
 * the remaining parameters as sw's (store words; cheap 1 cycle ops since
 * they are simply putting a value into cached memory). At the end of the
 * procedure, I flush the cache line marking it invalid in the process,
 * write the start triangle command as a simple sw and restore the SR to
 * what it was at entry. There a total of 8 writes (5 doubles, one cache
 * write and one word) to the PCI bus in this function. Claiming the
 * cache line could cost as much as 500ns if it was dirty. Some time was
 * spent reordering the function to reduce pipline stalls. I left the
 * untangled version in this file as a reference. There probably are more
 * pipeline stalls that could be removed. Finding those "Waldo's" is left
 * as an exercise to the reader.
 ***************************************************************************************/

/****************************************************************************************
 *                                                                          		*
 * User Adjustable CPP variables. These values are expected to be set at the command 	*
 * line.										*
 *                                                                          		*
 * USE_ATARI_VERTEX - set non-zero to use Atari Vertex offsets. Defaults to 0.		*
 * USE_OLD_VERTEX - set non-zero to use Glide 1.0 GrVertex offsets. Defaults to 0.	*
 * USE_OOW - set non-zero if to use GrVertex OOW instead of the TMU OOW. 		*
 * 		(this is only relavent if using Glide's GrVertex, Defaults to 0)	*
 * DRAW_FUNCTION_NAME - set to name to be assigned to this function. Defaults to:	*
 *	agcDrawTriangle if USE_ATARI_VERTEX is set					*
 *	oldDrawTriangle if USE_OLD_VERTEX is set					*
 *	_grDrawTriangleNonAA if neither USE_xxx_VERTEX is set. This is the default.	*
 * DRAW_BLEND_NAME - set to name to assign to blended draw routine. Defaults to		*
 *	agcDrawTriBlend									*
 * TIME_IT - set to non-zero to enable insertion of timer code. Defaults to 0		*
 * TIME_BLEND - set to non-zero to enable insertion of timer code. Defaults to 0	*
 * DRAW_NO_CULL - set to non-zero to disable cull test. Moves "store" of X and Y params	*
 *		very early in the sequence. Default is 0.				*
 * DRAW_MIN_AREA - set to signed floating point value. Triangles having an area less	*
 *		than this value will be culled. Default is 1.0				*
 * MIPS4 - set to non-zero if mips IV instruction set is available. Default is 0.	*
 * VALIDATE_PARAMS - set to non-zero to enable insertion of parameter validation code.	*
 *		Defaults to 0.								*
 * VALIDATE_LOWER_X - set to floating point value of mininum allowable X. Default 0.0	*
 * VALIDATE_UPPER_X - set to floating point value of maximum allowable X. Default 513.0 *
 * VALIDATE_LOWER_Y - set to floating point value of mininum allowable Y. Default 0.0	*
 * VALIDATE_UPPER_Y - set to floating point value of maximum allowable Y. Default 257.0 *
 *		(above 4 parameters are only relevant if VALIDATE_PARAMS is non-zero)	*
 *											*
 ****************************************************************************************/

#if MIPS4
	.set mips4
#endif

#ifndef DRAW_MIN_AREA
# define DRAW_MIN_AREA	1.0	/* assign default area minimum */
#endif

#if 0
#ifndef DRAW_BLEND_NAME
# define DRAW_BLEND_NAME agcDrawTriBlend
#endif
#endif

/****************************************************************************/
/*                                                                          */
/* Offsets to fields of a vertex struct used by the game.		    */
/*                                                                          */
/****************************************************************************/
#if USE_ATARI_VERTEX
#define	X_OFFSET	( 0*4)	/* This is the format of Atari's vertex */
#define	Y_OFFSET	( 1*4)
#define	ALPHA_OFFSET	( 2*4)
#define	TMU0_S_OFFSET	( 3*4)
#define	TMU0_T_OFFSET	( 4*4)
#define	TMU0_W_OFFSET	( 5*4)
# if USE_ATARI_RGB
#  define RED_OFFSET	( 6*4)
#  define GREEN_OFFSET	( 7*4)
#  define BLUE_OFFSET	( 8*4)
# else
#define TMU1_S_OFFSET	( 6*4)
#define TMU1_T_OFFSET	( 7*4)
# endif
# undef USE_OOW			/* can't have this and Atari vertex too */
# ifndef DRAW_FUNCTION_NAME
#  define DRAW_FUNCTION_NAME agcDrawTriangle
# endif
#endif

#if USE_OLD_VERTEX
#define	X_OFFSET	( 0*4)	/* This is the format of a Glide 1.0 GrVertex */
#define	Y_OFFSET	( 1*4)
#define	Z_OFFSET	( 2*4)
#define	OOZ_OFFSET	( 3*4)
#define	OOW_OFFSET	( 4*4)
#define	RED_OFFSET	( 5*4)
#define	GREEN_OFFSET	( 6*4)
#define	BLUE_OFFSET	( 7*4)
#define	ALPHA_OFFSET	( 8*4)
#define	TMU0_W_OFFSET	(10*4)
#define	TMU0_S_OFFSET	(11*4)
#define	TMU0_T_OFFSET	(12*4)
#define	TMU1_W_OFFSET	(13*4)
#define	TMU1_S_OFFSET	(14*4)
#define	TMU1_T_OFFSET	(15*4)
# ifndef DRAW_FUNCTION_NAME
#  define DRAW_FUNCTION_NAME oldDrawTriangle
# endif
#endif

#if !USE_ATARI_VERTEX && !USE_OLD_VERTEX
#define	X_OFFSET	( 0*4)	/* This is the format of a Glide 2.x GrVertex */
#define	Y_OFFSET	( 1*4)
#define	Z_OFFSET	( 2*4)
#define	RED_OFFSET	( 3*4)
#define	GREEN_OFFSET	( 4*4)
#define	BLUE_OFFSET	( 5*4)
#define	OOZ_OFFSET	( 6*4)
#define	ALPHA_OFFSET	( 7*4)
#define	OOW_OFFSET	( 8*4)
#define	TMU0_S_OFFSET	( 9*4)
#define	TMU0_T_OFFSET	(10*4)
#define	TMU0_W_OFFSET	(11*4)
#define	TMU1_S_OFFSET	(12*4)
#define	TMU1_T_OFFSET	(13*4)
#define	TMU1_W_OFFSET	(14*4)
# ifndef DRAW_FUNCTION_NAME
#  define DRAW_FUNCTION_NAME _grDrawTriangleNonAA
# endif
#endif

/****************************************************************************/
/*                                                                          */
/* Offsets to the hardware registers. 					    */
/* (NOTE: These are the alternate register mappings)			    */
/*                                                                          */
/****************************************************************************/
#define FTRIANGLE_START ( 0*4+0x80)
#define	VAX_OFFSET	( 2*4+0x80)
#define	VAY_OFFSET	( 3*4+0x80)
#define	VBX_OFFSET	( 4*4+0x80)
#define	VBY_OFFSET	( 5*4+0x80)
#define	VCX_OFFSET	( 6*4+0x80)
#define	VCY_OFFSET	( 7*4+0x80)

#define	START_R_OFFSET	( 8*4+0x80)
#define	DRDX_OFFSET	( 9*4+0x80)
#define	DRDY_OFFSET	(10*4+0x80)
#define	START_G_OFFSET	(11*4+0x80)
#define	DGDX_OFFSET	(12*4+0x80)
#define	DGDY_OFFSET	(13*4+0x80)
#define	START_B_OFFSET	(14*4+0x80)
#define	DBDX_OFFSET	(15*4+0x80)

#define	DBDY_OFFSET	(16*4+0x80)
#define	START_Z_OFFSET	(17*4+0x80)
#define	DZDX_OFFSET	(18*4+0x80)
#define	DZDY_OFFSET	(19*4+0x80)
#define	START_A_OFFSET	(20*4+0x80)
#define	DADX_OFFSET	(21*4+0x80)
#define	DADY_OFFSET	(22*4+0x80)
#define	START_S_OFFSET	(23*4+0x80)

#define	DSDX_OFFSET	(24*4+0x80)
#define	DSDY_OFFSET	(25*4+0x80)
#define	START_T_OFFSET	(26*4+0x80)
#define	DTDX_OFFSET	(27*4+0x80)
#define	DTDY_OFFSET	(28*4+0x80)
#define	START_W_OFFSET	(29*4+0x80)
#define	DWDX_OFFSET	(30*4+0x80)
#define	DWDY_OFFSET	(31*4+0x80)

#define FTRIANGLE_STARTa (32*4+0x80)	/* alternate triangle start command */

#define NOP_COMMAND	0x120		/* FBI/TMU NOP */

#define	FBI		0x0400
#define	TMU0		0x0800
#define	TMU1		0x1000
#define	TMU2		0x2000

#ifndef SST_BASE
# define SST_BASE	0xA8200000	/* pointer to start of SST registers non-cached */
#endif
#ifndef SST_BASEc
# define SST_BASEc	(SST_BASE&~0x20000000)	/* pointer to start of SST registers cached */
#endif

/* Floating point register definitions */

#define tay	$f0		/* temporary A Y value */
#define tby	$f1		/* temporary B Y value */
#define tcy	$f2		/* temporary C Y value */

#define dxBC	$f0
#define dyAB	$f1
#define dxAB	$f2
#define dyBC	$f3
#define farea	$f4
#define FTMP1	$f5
#define FTMP2	$f6

#define F_A_A	$f5		/* A alpha */
#define F_B_A	$f6		/* B alpha */
#define F_C_A	$f7		/* C alpha */
#define F_A_S	$f8		/* A S */
#define F_B_S	$f9		/* B S */
#define F_C_S	$f10		/* C S */
#define F_A_T	$f11		/* A T */
#define F_B_T	$f12		/* B T */
#define F_C_T	$f13		/* C T */
#define F_A_W	$f14		/* A W */
#define F_B_W	$f15		/* B W */
#define F_C_W	$f16		/* C W */
#define F_A_AmB $f17		/* Alpha A - B */
#define F_A_BmC $f18		/* Alpha B - C */
#define F_S_AmB $f19		/* S A - B */
#define F_S_BmC $f20		/* S B - C */
#define F_T_AmB $f21		/* T A - B */
#define F_T_BmC $f22		/* T B - C */
#define F_W_AmB $f23		/* W A - B */
#define F_W_BmC $f24		/* W B - C */
#define F_DnDX	$f25
#define F_DnDY	$f26
#define F_AB_BCx $f27
#define F_BC_ABx $f28
#define F_BC_ABy $f29
#define F_AB_BCy $f30

/* Integer register definitions */

#define r0	$0
#define at	$1
#define v0	$2
#define v1	$3
#define a0	$4
#define a1	$5
#define a2	$6
#define a3	$7
#define t0	$8
#define t1	$9
#define t2	$10
#define t3	$11
#define t4	$12
#define t5	$13
#define t6	$14
#define t7	$15
#define s0	$16
#define s1	$17
#define s2	$18
#define s3	$19
#define s4	$20
#define s5	$21
#define s6	$22
#define s7	$23
#define t8	$24
#define t9	$25
#define k0	$26
#define	k1	$27
#define gp	$28
#define	sp	$29
#define	fp	$30
#define ra	$31

#define SST	v0	/* holds pointer to 3dfx chips in uncached memory */
#define SSTc	v1	/* holds pointer to 3dfx chips in cached memory */
#define A	a0	/* holds pointer to A vertex struct */
#define B	a1	/* holds pointer to B vertex struct */
#define C	a2	/* holds pointer to C vertex struct */
#define SAVSR	a3	/* holds old SR contents */
#define	ITMP1	t0	/* scratch integer registers */
#define	ITMP2	t1
#define	ITMP3	t2
#define NEGATE	t2	/* holds flag indicating area needs negating */

#define SAV_F20	t3	/* Saved lower 32 bits of F20 and F21 */
#define SAV_F22	t4	/* Saved lower 32 bits of F22 and F23 */
#define SAV_F24	t5	/* Saved lower 32 bits of F24 and F25 */
#define SAV_F26	t6	/* Saved lower 32 bits of F26 and F27 */
#define SAV_F28	t7	/* Saved lower 32 bits of F28 and F29 */
#define SAV_F30	t8	/* Saved lower 32 bits of F30 */

#if TIME_IT || TIME_BLEND
#define TIME	t9	/* holds start time */
#endif

/* Misc definitions */

#define C0_COUNT   $9	/* CP0 count register */
#define C0_COMPARE $11	/* CP0 compare register (can be used as a scratch reg) */
#define C0_SR	   $12	/* CP0 status register */
#define C0_LLREG   $17	/* CP0 register LLREG (can used as a scratch reg) */
#define SR_IE	1	/* Interrupt enable bit */
#define SR_FR	(1<<26)	/* Enable 32 FP regs */
#define DCACHE_CLAIM   ((3<<2)|1)
#define DCACHE_DISCARD ((4<<2)|1)
#define DCACHE_FLUSH   ((5<<2)|1)

#define W_OFFSET TMU0_W_OFFSET

#define PARAMETER_LIST ((W_OFFSET<<24)|(TMU0_T_OFFSET<<16)|(TMU0_S_OFFSET<<8)|ALPHA_OFFSET)

	.bss
#if TIME_IT || TIME_BLEND
	.globl dms_t_min
	.globl dms_t_max
	.globl dms_t_avg
	.globl dms_t_count
dms_t_min: .space 4		;# min time
dms_t_max: .space 4		;# max time
dms_t_avg: .space 4		;# accumulated time
dms_t_count: .space 4		;# number of triangles
#define COMPUTE_TIMES()			\
	mfc0	ITMP1, C0_COUNT;	\
	lw	ITMP2, dms_t_min;	\
	lw	ITMP3, dms_t_max;	\
	subu	TIME, ITMP1, TIME;	\
	beq	ITMP2, r0, 10f;		\
	nop;				\
	bgt	TIME, ITMP2, 20f;	\
	nop;				\
10:	sw	TIME, dms_t_min;	\
20:	blt	TIME, ITMP3, 30f;	\
	nop		;		\
	sw	TIME, dms_t_max;	\
30:	lw	ITMP1, dms_t_avg;	\
	lw	ITMP2, dms_t_count;	\
	addu	ITMP1, TIME;		\
	sw	ITMP1, dms_t_avg;	\
	addu	ITMP2, 1;		\
	sw	ITMP2, dms_t_count;
#else
#define COMPUTE_TIMES() ;
#endif
#if VALIDATE_PARAMS || TIME_IT || TIME_BLEND
	.globl dms_t_reject
	.globl dms_t_culled
	.globl dms_t_lastarea
dms_t_culled: .space 4		;# triangles dismissed due to area too small
dms_t_reject: .space 4		;# triangles dismissed due to bad params
dms_t_lastarea: .space 4	;# save last area computed
#endif
	.text

	.ent	DRAW_FUNCTION_NAME
	.frame	$29, 0, $31
	.mask	0, 0
	.fmask	0, 0
	.globl	DRAW_FUNCTION_NAME
DRAW_FUNCTION_NAME:
	.set noreorder
	mfc0	SAVSR, C0_SR			;# get current SR
	li	SSTc, SST_BASEc			;# pointer to SST cached
	and	ITMP1, SAVSR, ~SR_IE		;# disable interrupts
#if !MIPS4
	or	ITMP1, SR_FR			;# enable all FP regs
#endif
	mtc0	ITMP1, C0_SR
#if TIME_IT
	mfc0	TIME, C0_COUNT			;# record start time
#else
	nop
#endif
	lwc1	tay, Y_OFFSET(A)		;# pickup the three Y values
	lwc1	tby, Y_OFFSET(B)
	lwc1	tcy, Y_OFFSET(C)

#if VALIDATE_PARAMS
# ifndef VALIDATE_LOWER_Y
#  define VALIDATE_LOWER_Y 0.0
# endif
# ifndef VALIDATE_LOWER_X
#  define VALIDATE_LOWER_X 0.0
# endif
# ifndef VALIDATE_UPPER_Y
#  define VALIDATE_UPPER_Y 257.0		/* assume standard res */
# endif
# ifndef VALIDATE_UPPER_X
#  define VALIDATE_UPPER_X 513.0
# endif
	li.s	FTMP1, VALIDATE_LOWER_Y		;# for debugging, check params are in limits
	c.lt.s	tay, FTMP1
	bc1t	reject				;# and ignore triangle if not
	c.lt.s	tby, FTMP1
	bc1t	reject
	c.lt.s	tcy, FTMP1
	bc1t	reject
	li.s	FTMP1, VALIDATE_UPPER_Y
	c.lt.s	FTMP1, tay
	bc1t	reject
	c.lt.s	FTMP1, tby
	bc1t	reject
	c.lt.s	FTMP1, tcy
	bc1t	reject
#endif
	move	NEGATE, r0			;# assume not to negate
	c.lt.s	tay, tby			;# if (va->y < vb->y)
	bc1f	20f				;# 
	c.le.s	tby, tcy			;#   if (vb->y > vc->y)
	bc1t	sort_done			;#            (else it is already sorted)
	c.lt.s	tay, tcy			;#     if (va->y < vc->y)
	bc1f	10f				;# 
	move	ITMP1, B			;# 	  a=a, b=c, c=b
	move	B, C				;#	  negate = 1
	move	C, ITMP1
	b	sort_done
	or	NEGATE, r0, 1

10:	move	ITMP1, A			;#      else 
	move	ITMP2, B			;#	  a=c, b=a, c=b
	move	A, C
	move	B, ITMP1
	b	sort_done
	move	C, ITMP2

20:	c.lt.s	tby, tcy			;#   else if (vb-Y < vc->y)
	bc1f	40f				;# 
	c.lt.s	tay, tcy			;#      if (va->y < vc->y)
	bc1f	30f				;#
	move	ITMP1, B			;#        a=b, b=a, c=c
	move	B, A				;#	  negate = 1
	move	A, ITMP1
	b	sort_done
	or	NEGATE, r0, 1

30:	move	ITMP1, A			;#      else 
	move	A, B				;#	  a=b, b=c, c=a
	move	B, C
	b	sort_done
	move	C, ITMP1

40:	move	ITMP1, A			;#   else 
	move	A, C				;#	a=c, b=b, c=a
	move	C, ITMP1			;#	negate = 1
;#	b	sort_done			;# fall through to sort done
	or	NEGATE, r0, 1

	.align	3
sort_done:

	lwc1	dxAB, X_OFFSET(A)
	lwc1	dxBC, X_OFFSET(B)
	lwc1	FTMP1, X_OFFSET(C)
#if VALIDATE_PARAMS
	li.s	FTMP2, VALIDATE_LOWER_X
	c.lt.s	dxAB, FTMP2
	bc1t	reject
	c.lt.s	dxBC, FTMP2
	bc1t	reject
	c.lt.s	FTMP1, FTMP2
	bc1t	reject
	li.s	FTMP2, VALIDATE_UPPER_X
	c.lt.s	FTMP2, dxAB
	bc1t	reject
	c.lt.s	FTMP2, dxBC
	bc1t	reject
	c.lt.s	FTMP2, FTMP1
	bc1t	reject
#endif
	sub.s	dxAB, dxBC			;# dxAB = A(x) - B(x)
	sub.s	dxBC, FTMP1			;# dxBC = B(x) - C(x)
	lwc1	dyAB, Y_OFFSET(A)
	lwc1	dyBC, Y_OFFSET(B)
	lwc1	FTMP1, Y_OFFSET(C)
	sub.s	dyAB, dyBC			;# dyAB = A(y) - B(y)
	sub.s	dyBC, FTMP1			;# dyBC = B(y) - C(y)
	mul.s	farea, dxAB, dyBC
	mul.s	FTMP1, dxBC, dyAB
	sub.s	farea, FTMP1			;# farea = dxAB*dyBC - dxBC*dyAB
	beq	NEGATE, r0, 50f			;# not to negate
	mov.s	FTMP1, farea

	neg.s	FTMP1
50:	li.s	FTMP2, DRAW_MIN_AREA		;# get arbitrary area limit (NOTE: 3dfx crashes
						;#  if area > 0.0 but less than something less
						;#   than 1.0. By default, we toss triangles
						;#     with areas of one pixel or less).
	c.le.s	FTMP1, FTMP2
	bc1t	culled				;# dismiss triangles with areas too small
#if VALIDATE_PARAMS
	swc1	farea, dms_t_lastarea		;# remember this so when we crash we can tell why
#endif

	li	SST, SST_BASE			;# pointer to SST non-cached
	ld	ITMP1, X_OFFSET(A)		;# copy A, B, Cs X and Y params to output
	ld	ITMP2, X_OFFSET(B)
	sd	ITMP1, VAX_OFFSET(SST)
	ld	ITMP1, X_OFFSET(C)
	sd	ITMP2, VBX_OFFSET(SST)
	sd	ITMP1, VCX_OFFSET(SST)

#undef FTMP1				/* can't use these for anything anymore */
#undef FTMP2
	lwc1	F_A_A, ALPHA_OFFSET(A)		;# get all the params in regs so we can...
	lwc1	F_B_A, ALPHA_OFFSET(B)		;# ...safely claim cache lines all to ourself
	lwc1	F_C_A, ALPHA_OFFSET(C)
	lwc1	F_A_S, TMU0_S_OFFSET(A)
	lwc1	F_B_S, TMU0_S_OFFSET(B)
	lwc1	F_C_S, TMU0_S_OFFSET(C)
	lwc1	F_A_T, TMU0_T_OFFSET(A)
	lwc1	F_B_T, TMU0_T_OFFSET(B)
	lwc1	F_C_T, TMU0_T_OFFSET(C)
	lwc1	F_A_W, W_OFFSET(A)
	lwc1	F_B_W, W_OFFSET(B)
	lwc1	F_C_W, W_OFFSET(C)

/* WARNINIG DANGER WARNING DANGER 
 * The following only saves the lower 32 bits of the 11 floating registers $f20-$f30.
 * ALL Your C code needs to be compiled -fshort-double for this to work correctly.
 * If it is used with higher level code that expects to use 64 bit doubles, it will fail
 * that code. Since we (AGC) only use short-doubles (32 bit) and there aren't enough I
 * registers to save all 11 of the needed double registers, we do a big cheat here.
 */
	and	ITMP1, SAVSR, SR_FR
	beq	ITMP1, r0, 10f
	addu	ITMP3, r0, -1		/* li ITMP3, -1 without using a macro */

	dsll32	ITMP3, 0
	nor	ITMP3, r0, ITMP3
	mfc1	ITMP1, $f20
	mfc1	ITMP2, $f21
	and	ITMP1, ITMP3
	dsll32	ITMP2, 0
	or	SAV_F20, ITMP1, ITMP2
	mfc1	ITMP1, $f22
	mfc1	ITMP2, $f23
	and	ITMP1, ITMP3
	dsll32	ITMP2, 0
	or	SAV_F22, ITMP1, ITMP2
	mfc1	ITMP1, $f24
	mfc1	ITMP2, $f25
	and	ITMP1, ITMP3
	dsll32	ITMP2, 0
	or	SAV_F24, ITMP1, ITMP2
	mfc1	ITMP1, $f26
	mfc1	ITMP2, $f27
	and	ITMP1, ITMP3
	dsll32	ITMP2, 0
	or	SAV_F26, ITMP1, ITMP2
	mfc1	ITMP1, $f28
	mfc1	ITMP2, $f29
	and	ITMP1, ITMP3
	dsll32	ITMP2, 0
	or	SAV_F28, ITMP1, ITMP2
	b	20f
	mfc1	SAV_F30, $f30

10:
	mfc1	SAV_F20, $f20		/* NOTE: Only saves the even regs (dmfc1 doesn't work) */
	mfc1	SAV_F22, $f22
	mfc1	SAV_F24, $f24
	mfc1	SAV_F26, $f26
	mfc1	SAV_F28, $f28
	mfc1	SAV_F30, $f30
20:
	cache	DCACHE_CLAIM, DSDX_OFFSET(SSTc)	;# claim 1 cache line for our exclusive use

#define FTMP1 F_A_AmB
#if MIPS4
	recip.s	FTMP1, farea			;# FTMP1 = ooa = 1.0/area
#else
	li.s	FTMP1, 1.0
	div.s	FTMP1, farea
#endif
	mul.s	dxBC, FTMP1			;# dxBC *= ooa;
	mul.s	dyAB, FTMP1			;# dyAB *= ooa;
	mul.s	dxAB, FTMP1			;# dxAB *= ooa;
	mul.s	dyBC, FTMP1			;# dyBC *= ooa;
#undef FTMP1				/* cant use this for anything anymore */

#if VALIDATE_PARAMS
# define F_MIN_ALP	F_A_AmB
# define F_MAX_ALP	F_A_BmC
	li.s	F_MAX_ALP, 255.0		;# max alpha value 
	li.s	F_MIN_ALP, 0.0			;# min alpha value
	c.lt.s	F_A_A, F_MIN_ALP
	bc1t	cache_reject
	c.lt.s	F_MAX_ALP, F_A_A
	bc1t	cache_reject
	c.lt.s	F_B_A, F_MIN_ALP 
	bc1t	cache_reject
	c.lt.s	F_MAX_ALP, F_B_A
	bc1t	cache_reject
	c.lt.s	F_C_A, F_MIN_ALP
	bc1t	cache_reject
	c.lt.s	F_MAX_ALP, F_C_A
	bc1t	cache_reject
# undef F_MIN_ALP
# undef F_MAX_ALP
#endif

#if 0

/* This is the untangled version of the CACHE code listed on the other side of
 * the #else below. It is here just for reference purposes since the tangled
 * version is "write only" source code. The tangled version is simply this
 * code rearranged to fill as many of the pipline slots as possible as well
 * as to take advantage of the 5k's dual instruction issue.
 */
	sub.s	F_A_AmB, F_A_A, F_B_A		;# daAB = A - B
	sub.s	F_A_BmC, F_B_A, F_C_A		;# daBC = B - C
	mul.s	F_AB_BCx, F_A_AmB, dyBC		;# daAB*dyBC
	mul.s	F_BC_ABx, F_A_BmC, dyAB		;# daBC*dyAB 
	sub.s	F_DnDX, F_AB_BCx, F_BC_ABx	;# dadx = daAB*dyBC - daBC*dyAB
	mul.s	F_BC_ABy, F_A_BmC, dxAB		;# daBC*dxAB
	mul.s	F_AB_BCy, F_A_AmB, dxBC		;# daAB*dxBC
	sub.s	F_DnDY, F_BC_ABy, F_AB_BCy	;# dady = daBC*dxAB - daAB*dxBC

	mfc1	ITMP1, F_A_A			;# put A alpha into GPR (note: sign extends)
	dsll32	ITMP1, 0			;# undo sign extend from mfc1 above, part 1
	dsrl32	ITMP1, 0			;# undo sign extend from mfc1 above, part 2
	mfc1	ITMP2, F_DnDX			;# get dadx
	dsll32	ITMP2, 0			;# put dadx in bits 63-32
	or	ITMP2, ITMP1			;# mix in A alpha_start
	sd	ITMP2, START_A_OFFSET(SST)	;# write A and dadx to 3dfx
	mfc1	ITMP3, F_DnDY			;# get dady
	dsll32	ITMP3, 0			;# undo sign extend of dady, part 1
	dsrl32	ITMP3, 0			;# undo sign extend of dady, part 2
	mfc1	ITMP1, F_A_S			;# get A S
	dsll32	ITMP1, 0			;# put S into bits 63-32
	or	ITMP1, ITMP3			;# mix dady and S
	sd	ITMP1, DADY_OFFSET(SST)		;# send to 3dfx

	sub.s	F_S_AmB, F_A_S, F_B_S		;# dsAB = A - B
	sub.s	F_S_BmC, F_B_S, F_C_S		;# dsBC = B - C
	mul.s	F_AB_BCx, F_S_AmB, dyBC		;# dsAB*dyBC
	mul.s	F_BC_ABx, F_S_BmC, dyAB		;# dsBC*dyAB 
	mul.s	F_BC_ABy, F_S_BmC, dxAB		;# dsBC*dxAB
	mul.s	F_AB_BCy, F_S_AmB, dxBC		;# dsAB*dxBC
	sub.s	F_DnDX, F_AB_BCx, F_BC_ABx	;# dsdx = dsAB*dyBC - dsBC*dyAB
	sub.s	F_DnDY, F_BC_ABy, F_AB_BCy	;# dsdy = dsBC*dxAB - dsAB*dxBC
	swc1	F_DnDX, DSDX_OFFSET(SSTc)	;# send dsdx to cache
	swc1	F_DnDY, DSDY_OFFSET(SSTc)	;# send dsdy to cache

	sub.s	F_T_AmB, F_A_T, F_B_T		;# dtAB = A - B
	sub.s	F_T_BmC, F_B_T, F_C_T		;# dtBC = B - C
	mul.s	F_AB_BCx, F_T_AmB, dyBC		;# dtAB*dyBC
	mul.s	F_BC_ABx, F_T_BmC, dyAB		;# dtBC*dyAB 
	mul.s	F_BC_ABy, F_T_BmC, dxAB		;# dtBC*dxAB
	mul.s	F_AB_BCy, F_T_AmB, dxBC		;# dtAB*dxBC
	sub.s	F_DnDX, F_AB_BCx, F_BC_ABx	;# dtdx = dtAB*dyBC - dtBC*dyAB
	sub.s	F_DnDY, F_BC_ABy, F_AB_BCy	;# dtdy = dtBC*dxAB - dtAB*dxBC
	swc1	F_A_T, START_T_OFFSET(SSTc)	;# sent T to cache
	swc1	F_DnDX, DTDX_OFFSET(SSTc)	;# sent dtdx to cache
	swc1	F_DnDY, DTDY_OFFSET(SSTc)	;# send dtdy to cache

	sub.s	F_W_AmB, F_A_W, F_B_W		;# dwAB = A - B
	sub.s	F_W_BmC, F_B_W, F_C_W		;# dwBC = B - C
	mul.s	F_AB_BCx, F_W_AmB, dyBC		;# dwAB*dyBC
	mul.s	F_BC_ABx, F_W_BmC, dyAB		;# dwBC*dyAB 
	mul.s	F_BC_ABy, F_W_BmC, dxAB		;# dwBC*dxAB
	mul.s	F_AB_BCy, F_W_AmB, dxBC		;# dwAB*dxBC
	sub.s	F_DnDX, F_AB_BCx, F_BC_ABx	;# dwdx = dwAB*dyBC - dwBC*dyAB
	sub.s	F_DnDY, F_BC_ABy, F_AB_BCy	;# dwdy = dwBC*dxAB - dwAB*dxBC
	swc1	F_A_W, START_W_OFFSET(SSTc)	;# sent W to cache
	swc1	F_DnDX, DWDX_OFFSET(SSTc)	;# sent dwdx to cache
	swc1	F_DnDY, DWDY_OFFSET(SSTc)	;# send dwdy to cache

#else

/* This is the _real_ code. Good luck trying to follow it. */

	sub.s	F_A_AmB, F_A_A, F_B_A		;# daAB = A - B
	sub.s	F_A_BmC, F_B_A, F_C_A		;# daBC = B - C
	sub.s	F_S_AmB, F_A_S, F_B_S		;# dsAB = A - B
	sub.s	F_S_BmC, F_B_S, F_C_S		;# dsBC = B - C
	sub.s	F_T_AmB, F_A_T, F_B_T		;# dtAB = A - B
	mul.s	F_AB_BCx, F_A_AmB, dyBC		;# daAB*dyBC
	mul.s	F_BC_ABx, F_A_BmC, dyAB		;# daBC*dyAB 
	mul.s	F_BC_ABy, F_A_BmC, dxAB		;# daBC*dxAB
	mul.s	F_AB_BCy, F_A_AmB, dxBC		;# daAB*dxBC
	sub.s	F_T_BmC, F_B_T, F_C_T		;# dtBC = B - C
	sub.s	F_W_AmB, F_A_W, F_B_W		;# dwAB = A - B
	sub.s	F_DnDX, F_AB_BCx, F_BC_ABx	;# dadx = daAB*dyBC - daBC*dyAB
	mul.s	F_AB_BCx, F_S_AmB, dyBC		;# dsAB*dyBC
	mul.s	F_BC_ABx, F_S_BmC, dyAB		;# dsBC*dyAB 
	sub.s	F_DnDY, F_BC_ABy, F_AB_BCy	;# dady = daBC*dxAB - daAB*dxBC
	mul.s	F_AB_BCy, F_S_AmB, dxBC		;# dsAB*dxBC
	mfc1	ITMP2, F_DnDX			;# get dadx
	mul.s	F_BC_ABy, F_S_BmC, dxAB		;# dsBC*dxAB
	mfc1	ITMP1, F_A_A			;# put A alpha into GPR (note: sign extends)
	mfc1	ITMP3, F_DnDY			;# get dady
	sub.s	F_W_BmC, F_B_W, F_C_W		;# dwBC = B - C
	dsll32	ITMP1, 0			;# undo sign extend from mfc1 above, part 1
	sub.s	F_DnDX, F_AB_BCx, F_BC_ABx	;# dsdx = dsAB*dyBC - dsBC*dyAB
	dsrl32	ITMP1, 0			;# undo sign extend from mfc1 above, part 2
	sub.s	F_DnDY, F_BC_ABy, F_AB_BCy	;# dsdy = dsBC*dxAB - dsAB*dxBC
	dsll32	ITMP2, 0			;# put dadx in bits 63-32
	mul.s	F_AB_BCx, F_T_AmB, dyBC		;# dtAB*dyBC
	or	ITMP2, ITMP1			;# mix in A alpha_start
	mul.s	F_BC_ABx, F_T_BmC, dyAB		;# dtBC*dyAB 
	sd	ITMP2, START_A_OFFSET(SST)	;# write A and dadx to 3dfx
	mfc1	ITMP1, F_A_S			;# get A S
	swc1	F_DnDX, DSDX_OFFSET(SSTc)	;# send dsdx to cache
	dsll32	ITMP1, 0			;# put S into bits 63-32
	swc1	F_DnDY, DSDY_OFFSET(SSTc)	;# send dsdy to cache
	dsll32	ITMP3, 0			;# undo sign extend of dady, part 1
	mul.s	F_BC_ABy, F_T_BmC, dxAB		;# dtBC*dxAB
	dsrl32	ITMP3, 0			;# undo sign extend of dady, part 2
	mul.s	F_AB_BCy, F_T_AmB, dxBC		;# dtAB*dxBC
	or	ITMP1, ITMP3			;# mix dady and S
	sub.s	F_DnDX, F_AB_BCx, F_BC_ABx	;# dtdx = dtAB*dyBC - dtBC*dyAB
	mul.s	F_AB_BCx, F_W_AmB, dyBC		;# dwAB*dyBC
	sd	ITMP1, DADY_OFFSET(SST)		;# send to 3dfx
	swc1	F_A_T, START_T_OFFSET(SSTc)	;# sent T to cache
	sub.s	F_DnDY, F_BC_ABy, F_AB_BCy	;# dtdy = dtBC*dxAB - dtAB*dxBC
	mul.s	F_BC_ABx, F_W_BmC, dyAB		;# dwBC*dyAB 
	mul.s	F_BC_ABy, F_W_BmC, dxAB		;# dwBC*dxAB
	mul.s	F_AB_BCy, F_W_AmB, dxBC		;# dwAB*dxBC
	swc1	F_DnDX, DTDX_OFFSET(SSTc)	;# sent dtdx to cache
	swc1	F_DnDY, DTDY_OFFSET(SSTc)	;# send dtdy to cache
	sub.s	F_DnDX, F_AB_BCx, F_BC_ABx	;# dwdx = dwAB*dyBC - dwBC*dyAB
	swc1	F_A_W, START_W_OFFSET(SSTc)	;# sent W to cache
	sub.s	F_DnDY, F_BC_ABy, F_AB_BCy	;# dwdy = dwBC*dxAB - dwAB*dxBC
	swc1	F_DnDX, DWDX_OFFSET(SSTc)	;# sent dwdx to cache
	swc1	F_DnDY, DWDY_OFFSET(SSTc)	;# send dwdy to cache
#endif
	cache	DCACHE_FLUSH, DSDX_OFFSET(SSTc)	;# flush cache line to write buffer
	swc1	farea, FTRIANGLE_START(SST)	;# start the triangle

	and	ITMP1, SAVSR, SR_FR
	beq	ITMP1, r0, 10f
	mtc1	SAV_F20, $f20

	dsrl32	SAV_F20, 0
	mtc1	SAV_F20, $f21
	mtc1	SAV_F22, $f22
	dsrl32	SAV_F22, 0
	mtc1	SAV_F22, $f23
	mtc1	SAV_F24, $f24
	dsrl32	SAV_F24, 0
	mtc1	SAV_F24, $f25
	mtc1	SAV_F26, $f26
	dsrl32	SAV_F26, 0
	mtc1	SAV_F26, $f27
	mtc1	SAV_F28, $f28
	dsrl32	SAV_F28, 0
	mtc1	SAV_F28, $f29
	b	20f
	mtc1	SAV_F30, $f30

10:	mtc1	SAV_F22, $f22
	mtc1	SAV_F24, $f24
	mtc1	SAV_F26, $f26
	mtc1	SAV_F28, $f28
	mtc1	SAV_F30, $f30
20:
#if TIME_IT
	COMPUTE_TIMES()
#endif

	j	ra
	mtc0	SAVSR, C0_SR

#if VALIDATE_PARAMS
cache_reject:
	cache	(4<<2)|1, DSDX_OFFSET(SSTc)	;# just discard any cache contents
	lw	ITMP1, dms_t_reject		;# bump reject count
	addu	ITMP1, 1
	sw	ITMP1, dms_t_reject
	j	ra
	mtc0	SAVSR, C0_SR			;# re-enable interrupts

reject:
	lw	ITMP1, dms_t_reject		;# increment the reject counter
	addu	ITMP1, 1
	sw	ITMP1, dms_t_reject
	j	ra
	mtc0	SAVSR, C0_SR
#endif

culled:
#if VALIDATE_PARAMS || TIME_IT || TIME_BLEND
	lw	ITMP1, dms_t_culled		;# increment the culled counter
	addu	ITMP1, 1
	sw	ITMP1, dms_t_culled
#endif
	j	ra
	mtc0	SAVSR, C0_SR
	.set reorder
	.end	DRAW_FUNCTION_NAME

#ifdef DRAW_BLEND_NAME
/*****************************************************************************
 *
 * DrawTriBlend - This function is used by S.F. Rush to have its two TMU's do
 * something different on the triangles they are rendering.
 * TMU1's S is in RED_OFFSET and TMU1's T is in GREEN_OFFSET.
 *
 *****************************************************************************/

#undef FTMP1
#undef FTMP2
#define FTMP1	$f5
#define FTMP2	$f6
#if !USE_ATARI_VERTEX
# undef TMU1_S_OFFSET
# undef TMU1_T_OFFSET
# define TMU1_S_OFFSET RED_OFFSET
# define TMU1_T_OFFSET GREEN_OFFSET
#endif

	.ent	DRAW_BLEND_NAME
	.frame	$29, 0, $31
	.mask	0, 0
	.fmask	0, 0
	.globl	DRAW_BLEND_NAME
DRAW_BLEND_NAME:
	.set noreorder
	mfc0	SAVSR, C0_SR			;# get current SR
	li	SSTc, SST_BASEc			;# pointer to SST cached
	and	ITMP1, SAVSR, ~SR_IE		;# disable interrupts
	or	ITMP1, SR_FR			;# enable all FP regs
	mtc0	ITMP1, C0_SR
#if TIME_BLEND
	mfc0	TIME, C0_COUNT			;# record start time
#else
	nop
#endif
	lwc1	tay, Y_OFFSET(A)		;# pickup the three Y values
	lwc1	tby, Y_OFFSET(B)
	lwc1	tcy, Y_OFFSET(C)

	move	NEGATE, r0			;# assume not to negate
	c.lt.s	tay, tby			;# if (va->y < vb->y)
	bc1f	20f				;# 
	c.le.s	tby, tcy			;#   if (vb->y > vc->y)
	bc1t	tbsort_done			;#            (else it is already sorted)
	c.lt.s	tay, tcy			;#     if (va->y < vc->y)
	bc1f	10f				;# 
	move	ITMP1, B			;# 	  a=a, b=c, c=b
	move	B, C				;#	  negate = 1
	move	C, ITMP1
	b	tbsort_done
	or	NEGATE, r0, 1

10:	move	ITMP1, A			;#      else 
	move	ITMP2, B			;#	  a=c, b=a, c=b
	move	A, C
	move	B, ITMP1
	b	tbsort_done
	move	C, ITMP2

20:	c.lt.s	tby, tcy			;#   else if (vb-Y < vc->y)
	bc1f	40f				;# 
	c.lt.s	tay, tcy			;#      if (va->y < vc->y)
	bc1f	30f				;#
	move	ITMP1, B			;#        a=b, b=a, c=c
	move	B, A				;#	  negate = 1
	move	A, ITMP1
	b	tbsort_done
	or	NEGATE, r0, 1

30:	move	ITMP1, A			;#      else 
	move	A, B				;#	  a=b, b=c, c=a
	move	B, C
	b	tbsort_done
	move	C, ITMP1

40:	move	ITMP1, A			;#   else 
	move	A, C				;#	a=c, b=b, c=a
	move	C, ITMP1			;#	negate = 1
;#	b	tbsort_done			;# fall through to sort done
	or	NEGATE, r0, 1

	.align	3
tbsort_done:

	lwc1	dxAB, X_OFFSET(A)
	lwc1	dxBC, X_OFFSET(B)
	lwc1	FTMP1, X_OFFSET(C)
	sub.s	dxAB, dxBC			;# dxAB = A(x) - B(x)
	sub.s	dxBC, FTMP1			;# dxBC = B(x) - C(x)
	lwc1	dyAB, Y_OFFSET(A)
	lwc1	dyBC, Y_OFFSET(B)
	lwc1	FTMP1, Y_OFFSET(C)
	sub.s	dyAB, dyBC			;# dyAB = A(y) - B(y)
	sub.s	dyBC, FTMP1			;# dyBC = B(y) - C(y)
	mul.s	farea, dxAB, dyBC
	mul.s	FTMP1, dxBC, dyAB
	sub.s	farea, FTMP1			;# farea = dxAB*dyBC - dxBC*dyAB
	beq	NEGATE, r0, 50f			;# not to negate
	mov.s	FTMP1, farea

	neg.s	FTMP1
50:	li.s	FTMP2, DRAW_MIN_AREA		;# get arbitrary area limit (NOTE: 3dfx crashes
						;#  if area > 0.0 but less than something less
						;#   than 1.0. By default, we toss triangles
						;#     with areas of one pixel or less).
	c.le.s	FTMP1, FTMP2
	bc1t	culled				;# dismiss triangles with areas too small

	li	SST, SST_BASE			;# pointer to SST non-cached
	ld	ITMP1, X_OFFSET(A)		;# copy A, B, Cs X and Y params to output
	ld	ITMP2, X_OFFSET(B)
	sd	ITMP1, VAX_OFFSET(SST)
	ld	ITMP1, X_OFFSET(C)
	sd	ITMP2, VBX_OFFSET(SST)
	sd	ITMP1, VCX_OFFSET(SST)

#undef FTMP1				/* can't use these for anything anymore */
#undef FTMP2
	lwc1	F_A_A, ALPHA_OFFSET(A)		;# get all the params in regs so we can
	lwc1	F_B_A, ALPHA_OFFSET(B)
	lwc1	F_C_A, ALPHA_OFFSET(C)
	lwc1	F_A_S, TMU0_S_OFFSET(A)		;#   safely claim a cache line all to ourself
	lwc1	F_B_S, TMU0_S_OFFSET(B)
	lwc1	F_C_S, TMU0_S_OFFSET(C)
	lwc1	F_A_T, TMU0_T_OFFSET(A)
	lwc1	F_B_T, TMU0_T_OFFSET(B)
	lwc1	F_C_T, TMU0_T_OFFSET(C)
	lwc1	F_A_W, W_OFFSET(A)
	lwc1	F_B_W, W_OFFSET(B)
	lwc1	F_C_W, W_OFFSET(C)

/* WARNINIG DANGER WARNING DANGER 
 * The following only saves the lower 32 bits of the 12 floating registers $f20-$f31.
 * If it is used with higher level code that expects to use 64 bit doubles, it will fail
 * that code. Since we only use short-doubles (32 bit) and there aren't enough I registers
 * to save all 12 of the needed floating registers, we do a big cheat here.
 */
	and	ITMP1, SAVSR, SR_FR
	beq	ITMP1, r0, 10f
	addu	ITMP3, r0, -1		/* li ITMP3, -1 without using a macro */

	dsll32	ITMP3, 0
	dsrl32	ITMP3, 0
	mfc1	ITMP1, $f20
	mfc1	ITMP2, $f21
	and	ITMP1, ITMP3
	dsll32	ITMP2, 0
	or	SAV_F20, ITMP1, ITMP2
	mfc1	ITMP1, $f22
	mfc1	ITMP2, $f23
	and	ITMP1, ITMP3
	dsll32	ITMP2, 0
	or	SAV_F22, ITMP1, ITMP2
	mfc1	ITMP1, $f24
	mfc1	ITMP2, $f25
	and	ITMP1, ITMP3
	dsll32	ITMP2, 0
	or	SAV_F24, ITMP1, ITMP2
	mfc1	ITMP1, $f26
	mfc1	ITMP2, $f27
	and	ITMP1, ITMP3
	dsll32	ITMP2, 0
	or	SAV_F26, ITMP1, ITMP2
	mfc1	ITMP1, $f28
	mfc1	ITMP2, $f29
	and	ITMP1, ITMP3
	dsll32	ITMP2, 0
	or	SAV_F28, ITMP1, ITMP2
	b	20f
	mfc1	SAV_F30, $f30

10:
	mfc1	SAV_F20, $f20		/* NOTE: Only saves the even regs (dmfc1 doesn't work) */
	mfc1	SAV_F22, $f22
	mfc1	SAV_F24, $f24
	mfc1	SAV_F26, $f26
	mfc1	SAV_F28, $f28
	mfc1	SAV_F30, $f30
20:
	cache	DCACHE_CLAIM, DSDX_OFFSET(SSTc)	;# claim 1 cache line for our exclusive use

#define FTMP1 F_A_AmB
#if MIPS4
	recip.s	FTMP1, farea			;# FTMP1 = ooa = 1.0/area
#else
	li.s	FTMP1, 1.0
	div.s	FTMP1, farea
#endif
	mul.s	dxBC, FTMP1			;# dxBC *= ooa;
	mul.s	dyAB, FTMP1			;# dyAB *= ooa;
	mul.s	dxAB, FTMP1			;# dxAB *= ooa;
	mul.s	dyBC, FTMP1			;# dyBC *= ooa;

#undef FTMP1				/* can't use these for anything anymore */

#if 0

/* This is the untangled version of the CACHE code listed on the other side of
 * the #else below. It is here just for reference purposes since the tangled
 * version is "write only" source code. The tangled version is simply this
 * code rearranged to fill as many of the pipline slots as possible as well
 * as to take advantage of the 5k's dual instruction issue.
 */
	sub.s	F_A_AmB, F_A_A, F_B_A		;# daAB = A - B
	sub.s	F_A_BmC, F_B_A, F_C_A		;# daBC = B - C
	mul.s	F_AB_BCx, F_A_AmB, dyBC		;# daAB*dyBC
	mul.s	F_BC_ABx, F_A_BmC, dyAB		;# daBC*dyAB 
	sub.s	F_DnDX, F_AB_BCx, F_BC_ABx	;# dadx = daAB*dyBC - daBC*dyAB
	mul.s	F_BC_ABy, F_A_BmC, dxAB		;# daBC*dxAB
	mul.s	F_AB_BCy, F_A_AmB, dxBC		;# daAB*dxBC
	sub.s	F_DnDY, F_BC_ABy, F_AB_BCy	;# dady = daBC*dxAB - daAB*dxBC

	mfc1	ITMP1, F_A_A			;# put A alpha into GPR (note: sign extends)
	dsll32	ITMP1, 0			;# undo sign extend from mfc1 above, part 1
	dsrl32	ITMP1, 0			;# undo sign extend from mfc1 above, part 2
	mfc1	ITMP2, F_DnDX			;# get dadx
	dsll32	ITMP2, 0			;# put dadx in bits 63-32
	or	ITMP2, ITMP1			;# mix in A alpha_start
	sd	ITMP2, START_A_OFFSET(SST)	;# write A and dadx to 3dfx
	mfc1	ITMP3, F_DnDY			;# get dady
	dsll32	ITMP3, 0			;# undo sign extend of dady, part 1
	dsrl32	ITMP3, 0			;# undo sign extend of dady, part 2
	mfc1	ITMP1, F_A_S			;# get A S
	dsll32	ITMP1, 0			;# put S into bits 63-32
	or	ITMP1, ITMP3			;# mix dady and S
	sd	ITMP1, DADY_OFFSET(SST)		;# send to 3dfx

	sub.s	F_S_AmB, F_A_S, F_B_S		;# dsAB = A - B
	sub.s	F_S_BmC, F_B_S, F_C_S		;# dsBC = B - C
	mul.s	F_AB_BCx, F_S_AmB, dyBC		;# dsAB*dyBC
	mul.s	F_BC_ABx, F_S_BmC, dyAB		;# dsBC*dyAB 
	mul.s	F_BC_ABy, F_S_BmC, dxAB		;# dsBC*dxAB
	mul.s	F_AB_BCy, F_S_AmB, dxBC		;# dsAB*dxBC
	sub.s	F_DnDX, F_AB_BCx, F_BC_ABx	;# dsdx = dsAB*dyBC - dsBC*dyAB
	sub.s	F_DnDY, F_BC_ABy, F_AB_BCy	;# dsdy = dsBC*dxAB - dsAB*dxBC
	swc1	F_DnDX, DSDX_OFFSET(SSTc)	;# send dsdx to cache
	swc1	F_DnDY, DSDY_OFFSET(SSTc)	;# send dsdy to cache

	sub.s	F_T_AmB, F_A_T, F_B_T		;# dtAB = A - B
	sub.s	F_T_BmC, F_B_T, F_C_T		;# dtBC = B - C
	mul.s	F_AB_BCx, F_T_AmB, dyBC		;# dtAB*dyBC
	mul.s	F_BC_ABx, F_T_BmC, dyAB		;# dtBC*dyAB 
	mul.s	F_BC_ABy, F_T_BmC, dxAB		;# dtBC*dxAB
	mul.s	F_AB_BCy, F_T_AmB, dxBC		;# dtAB*dxBC
	sub.s	F_DnDX, F_AB_BCx, F_BC_ABx	;# dtdx = dtAB*dyBC - dtBC*dyAB
	sub.s	F_DnDY, F_BC_ABy, F_AB_BCy	;# dtdy = dtBC*dxAB - dtAB*dxBC
	swc1	F_A_T, START_T_OFFSET(SSTc)	;# sent T to cache
	swc1	F_DnDX, DTDX_OFFSET(SSTc)	;# sent dtdx to cache
	swc1	F_DnDY, DTDY_OFFSET(SSTc)	;# send dtdy to cache

	sub.s	F_W_AmB, F_A_W, F_B_W		;# dwAB = A - B
	sub.s	F_W_BmC, F_B_W, F_C_W		;# dwBC = B - C
	mul.s	F_AB_BCx, F_W_AmB, dyBC		;# dwAB*dyBC
	mul.s	F_BC_ABx, F_W_BmC, dyAB		;# dwBC*dyAB 
	mul.s	F_BC_ABy, F_W_BmC, dxAB		;# dwBC*dxAB
	mul.s	F_AB_BCy, F_W_AmB, dxBC		;# dwAB*dxBC
	sub.s	F_DnDX, F_AB_BCx, F_BC_ABx	;# dwdx = dwAB*dyBC - dwBC*dyAB
	sub.s	F_DnDY, F_BC_ABy, F_AB_BCy	;# dwdy = dwBC*dxAB - dwAB*dxBC
	swc1	F_A_W, START_W_OFFSET(SSTc)	;# sent W to cache
	swc1	F_DnDX, DWDX_OFFSET(SSTc)	;# sent dwdx to cache
	swc1	F_DnDY, DWDY_OFFSET(SSTc)	;# send dwdy to cache
	cache	DCACHE_FLUSH, DSDX_OFFSET(SSTc)	;# flush cache line to PCI write buffer
	lwc1	F_A_S, TMU1_S_OFFSET(A)		;# load up TMU 1 params
	lwc1	F_B_S, TMU1_S_OFFSET(B)
	lwc1	F_C_S, TMU1_S_OFFSET(C)
	lwc1	F_A_T, TMU1_T_OFFSET(A)
	lwc1	F_B_T, TMU1_T_OFFSET(B)
	lwc1	F_C_T, TMU1_T_OFFSET(C)
	cache	DCACHE_CLAIM, DSDX_OFFSET+TMU1(SSTc) ;# claim a cache line on TMU1 params
	sw	r0, NOP_COMMAND(SST)		;# send a NOP to PCI compensate for packer bug
	swc1	F_A_W, START_W_OFFSET+TMU1(SSTc) ;# re-write W to cache
	swc1	F_DnDX, DWDX_OFFSET+TMU1(SSTc)	;# re-write dwdx to cache
	swc1	F_DnDY, DWDY_OFFSET+TMU1(SSTc)	;# re-write dwdy to cache
	swc1	F_A_S, START_S_OFFSET+TMU1(SST)	;# send S param to PCI

	sub.s	F_S_AmB, F_A_S, F_B_S		;# dsAB = A - B
	sub.s	F_S_BmC, F_B_S, F_C_S		;# dsBC = B - C
	mul.s	F_AB_BCx, F_S_AmB, dyBC		;# dsAB*dyBC
	mul.s	F_BC_ABx, F_S_BmC, dyAB		;# dsBC*dyAB 
	mul.s	F_BC_ABy, F_S_BmC, dxAB		;# dsBC*dxAB
	mul.s	F_AB_BCy, F_S_AmB, dxBC		;# dsAB*dxBC
	sub.s	F_DnDX, F_AB_BCx, F_BC_ABx	;# dsdx = dsAB*dyBC - dsBC*dyAB
	sub.s	F_DnDY, F_BC_ABy, F_AB_BCy	;# dsdy = dsBC*dxAB - dsAB*dxBC
	swc1	F_DnDX, DSDX_OFFSET+TMU1(SSTc)	;# send dsdx to cache
	swc1	F_DnDY, DSDY_OFFSET+TMU1(SSTc)	;# send dsdy to cache

	sub.s	F_T_AmB, F_A_T, F_B_T		;# dtAB = A - B
	sub.s	F_T_BmC, F_B_T, F_C_T		;# dtBC = B - C
	mul.s	F_AB_BCx, F_T_AmB, dyBC		;# dtAB*dyBC
	mul.s	F_BC_ABx, F_T_BmC, dyAB		;# dtBC*dyAB 
	mul.s	F_BC_ABy, F_T_BmC, dxAB		;# dtBC*dxAB
	mul.s	F_AB_BCy, F_T_AmB, dxBC		;# dtAB*dxBC
	sub.s	F_DnDX, F_AB_BCx, F_BC_ABx	;# dtdx = dtAB*dyBC - dtBC*dyAB
	sub.s	F_DnDY, F_BC_ABy, F_AB_BCy	;# dtdy = dtBC*dxAB - dtAB*dxBC
	swc1	F_A_T, START_T_OFFSET+TMU1(SSTc) ;# sent T to cache
	swc1	F_DnDX, DTDX_OFFSET+TMU1(SSTc)	;# sent dtdx to cache
	swc1	F_DnDY, DTDY_OFFSET+TMU1(SSTc)	;# send dtdy to cache
	cache	DCACHE_FLUSH, DSDX_OFFSET+TMU1(SSTc) ;# flush cache line to PCI write buffer
	sw	r0, NOP_COMMAND(SST)		;# send a NOP to PCI compensate for packer bug
	swc1	farea, FTRIANGLE_START(SST)	;# start the triangle

#else

/* This is the _real_ code. Good luck trying to follow it. */

	sub.s	F_A_AmB, F_A_A, F_B_A		;# daAB = A - B
	sub.s	F_A_BmC, F_B_A, F_C_A		;# daBC = B - C
	sub.s	F_S_AmB, F_A_S, F_B_S		;# dsAB = A - B
	sub.s	F_S_BmC, F_B_S, F_C_S		;# dsBC = B - C
	sub.s	F_T_AmB, F_A_T, F_B_T		;# dtAB = A - B
	mul.s	F_AB_BCx, F_A_AmB, dyBC		;# daAB*dyBC
	mul.s	F_BC_ABx, F_A_BmC, dyAB		;# daBC*dyAB 
	mul.s	F_BC_ABy, F_A_BmC, dxAB		;# daBC*dxAB
	mul.s	F_AB_BCy, F_A_AmB, dxBC		;# daAB*dxBC
	sub.s	F_T_BmC, F_B_T, F_C_T		;# dtBC = B - C
	sub.s	F_W_AmB, F_A_W, F_B_W		;# dwAB = A - B
	sub.s	F_DnDX, F_AB_BCx, F_BC_ABx	;# dadx = daAB*dyBC - daBC*dyAB
	mul.s	F_AB_BCx, F_S_AmB, dyBC		;# dsAB*dyBC
	mul.s	F_BC_ABx, F_S_BmC, dyAB		;# dsBC*dyAB 
	sub.s	F_DnDY, F_BC_ABy, F_AB_BCy	;# dady = daBC*dxAB - daAB*dxBC
	mul.s	F_AB_BCy, F_S_AmB, dxBC		;# dsAB*dxBC
	mfc1	ITMP2, F_DnDX			;# get dadx
	mul.s	F_BC_ABy, F_S_BmC, dxAB		;# dsBC*dxAB
	mfc1	ITMP1, F_A_A			;# put A alpha into GPR (note: sign extends)
	mfc1	ITMP3, F_DnDY			;# get dady
	sub.s	F_W_BmC, F_B_W, F_C_W		;# dwBC = B - C
	dsll32	ITMP1, 0			;# undo sign extend from mfc1 above, part 1
	sub.s	F_DnDX, F_AB_BCx, F_BC_ABx	;# dsdx = dsAB*dyBC - dsBC*dyAB
	dsrl32	ITMP1, 0			;# undo sign extend from mfc1 above, part 2
	sub.s	F_DnDY, F_BC_ABy, F_AB_BCy	;# dsdy = dsBC*dxAB - dsAB*dxBC
	dsll32	ITMP2, 0			;# put dadx in bits 63-32
	mul.s	F_AB_BCx, F_T_AmB, dyBC		;# dtAB*dyBC
	or	ITMP2, ITMP1			;# mix in A alpha_start
	mul.s	F_BC_ABx, F_T_BmC, dyAB		;# dtBC*dyAB 
	sd	ITMP2, START_A_OFFSET(SST)	;# write A and dadx to 3dfx
	mfc1	ITMP1, F_A_S			;# get A S
	swc1	F_DnDX, DSDX_OFFSET(SSTc)	;# send dsdx to cache
	dsll32	ITMP1, 0			;# put S into bits 63-32
	swc1	F_DnDY, DSDY_OFFSET(SSTc)	;# send dsdy to cache
	dsll32	ITMP3, 0			;# undo sign extend of dady, part 1
	mul.s	F_BC_ABy, F_T_BmC, dxAB		;# dtBC*dxAB
	dsrl32	ITMP3, 0			;# undo sign extend of dady, part 2
	mul.s	F_AB_BCy, F_T_AmB, dxBC		;# dtAB*dxBC
	or	ITMP1, ITMP3			;# mix dady and S
	sub.s	F_DnDX, F_AB_BCx, F_BC_ABx	;# dtdx = dtAB*dyBC - dtBC*dyAB
	mul.s	F_AB_BCx, F_W_AmB, dyBC		;# dwAB*dyBC
	sd	ITMP1, DADY_OFFSET(SST)		;# send to 3dfx
	swc1	F_A_T, START_T_OFFSET(SSTc)	;# sent T to cache
	sub.s	F_DnDY, F_BC_ABy, F_AB_BCy	;# dtdy = dtBC*dxAB - dtAB*dxBC
	mul.s	F_BC_ABx, F_W_BmC, dyAB		;# dwBC*dyAB 
	mul.s	F_BC_ABy, F_W_BmC, dxAB		;# dwBC*dxAB
	mul.s	F_AB_BCy, F_W_AmB, dxBC		;# dwAB*dxBC
	swc1	F_DnDX, DTDX_OFFSET(SSTc)	;# sent dtdx to cache
	swc1	F_DnDY, DTDY_OFFSET(SSTc)	;# send dtdy to cache
	sub.s	F_DnDX, F_AB_BCx, F_BC_ABx	;# dwdx = dwAB*dyBC - dwBC*dyAB
	swc1	F_A_W, START_W_OFFSET(SSTc)	;# sent W to cache
	sub.s	F_DnDY, F_BC_ABy, F_AB_BCy	;# dwdy = dwBC*dxAB - dwAB*dxBC
	swc1	F_DnDX, DWDX_OFFSET(SSTc)	;# sent dwdx to cache
	swc1	F_DnDY, DWDY_OFFSET(SSTc)	;# send dwdy to cache
	cache	DCACHE_FLUSH, DSDX_OFFSET(SSTc)	;# flush cache line to write buffer

	lwc1	F_A_S, TMU1_S_OFFSET(A)		;# load up TMU 1 params
	lwc1	F_B_S, TMU1_S_OFFSET(B)
	lwc1	F_C_S, TMU1_S_OFFSET(C)
	lwc1	F_A_T, TMU1_T_OFFSET(A)
	lwc1	F_B_T, TMU1_T_OFFSET(B)
	lwc1	F_C_T, TMU1_T_OFFSET(C)
	cache	DCACHE_CLAIM, DSDX_OFFSET+TMU1(SSTc) ;# claim a cache line over TMU1 params
	sw	r0, NOP_COMMAND(SST)		;# send a NOP to PCI compensate for packer bug
	sub.s	F_S_AmB, F_A_S, F_B_S		;# dsAB = A - B
	sub.s	F_S_BmC, F_B_S, F_C_S		;# dsBC = B - C
	swc1	F_A_W, START_W_OFFSET+TMU1(SSTc) ;# re-write W to cache
	swc1	F_DnDX, DWDX_OFFSET+TMU1(SSTc)	;# re-write dwdx to cache
	swc1	F_DnDY, DWDY_OFFSET+TMU1(SSTc)	;# re-write dwdy to cache
	mul.s	F_AB_BCx, F_S_AmB, dyBC		;# dsAB*dyBC
	mul.s	F_BC_ABx, F_S_BmC, dyAB		;# dsBC*dyAB 
	swc1	F_A_S, START_S_OFFSET+TMU1(SST)	;# send S param to PCI
	mul.s	F_BC_ABy, F_S_BmC, dxAB		;# dsBC*dxAB
	mul.s	F_AB_BCy, F_S_AmB, dxBC		;# dsAB*dxBC
	sub.s	F_T_AmB, F_A_T, F_B_T		;# dtAB = A - B
	sub.s	F_DnDX, F_AB_BCx, F_BC_ABx	;# dsdx = dsAB*dyBC - dsBC*dyAB
	mul.s	F_AB_BCx, F_T_AmB, dyBC		;# dtAB*dyBC
	mul.s	F_BC_ABx, F_T_BmC, dyAB		;# dtBC*dyAB 
	sub.s	F_DnDY, F_BC_ABy, F_AB_BCy	;# dsdy = dsBC*dxAB - dsAB*dxBC
	mul.s	F_AB_BCy, F_T_AmB, dxBC		;# dtAB*dxBC
	mul.s	F_BC_ABy, F_T_BmC, dxAB		;# dtBC*dxAB
	swc1	F_DnDX, DSDX_OFFSET+TMU1(SSTc)	;# send dsdx to cache
	sub.s	F_T_BmC, F_B_T, F_C_T		;# dtBC = B - C
	sub.s	F_DnDX, F_AB_BCx, F_BC_ABx	;# dtdx = dtAB*dyBC - dtBC*dyAB
	swc1	F_DnDY, DSDY_OFFSET+TMU1(SSTc)	;# send dsdy to cache
	sub.s	F_DnDY, F_BC_ABy, F_AB_BCy	;# dtdy = dtBC*dxAB - dtAB*dxBC

	swc1	F_A_T, START_T_OFFSET+TMU1(SSTc) ;# sent T to cache
	swc1	F_DnDX, DTDX_OFFSET+TMU1(SSTc)	;# sent dtdx to cache
	swc1	F_DnDY, DTDY_OFFSET+TMU1(SSTc)	;# send dtdy to cache
	cache	DCACHE_FLUSH, DSDX_OFFSET+TMU1(SSTc) ;# flush cache line to PCI write buffer
	sw	r0, NOP_COMMAND(SST)		;# send a NOP to PCI compensate for packer bug
	swc1	farea, FTRIANGLE_START(SST)	;# start the triangle
#endif

	and	ITMP1, SAVSR, SR_FR
	beq	ITMP1, r0, 10f
	mtc1	SAV_F20, $f20

	dsrl32	SAV_F20, 0
	mtc1	SAV_F20, $f21
	mtc1	SAV_F22, $f22
	dsrl32	SAV_F22, 0
	mtc1	SAV_F22, $f23
	mtc1	SAV_F24, $f24
	dsrl32	SAV_F24, 0
	mtc1	SAV_F24, $f25
	mtc1	SAV_F26, $f26
	dsrl32	SAV_F26, 0
	mtc1	SAV_F26, $f27
	mtc1	SAV_F28, $f28
	dsrl32	SAV_F28, 0
	mtc1	SAV_F28, $f29
	b	20f
	mtc1	SAV_F30, $f30

10:	mtc1	SAV_F22, $f22
	mtc1	SAV_F24, $f24
	mtc1	SAV_F26, $f26
	mtc1	SAV_F28, $f28
	mtc1	SAV_F30, $f30
20:

#if TIME_BLEND
	COMPUTE_TIMES()
#endif

#if MIPS4
	mtc0	SAVSR, C0_SR		;# NEC CPU has FP hazard requiring 2 insns
	j	ra
	nop
#else
	j	ra
	mtc0	SAVSR, C0_SR
#endif

	.set reorder
	.end	DRAW_BLEND_NAME
#endif					/* DRAW_BLEND_NAME */

/* _grVerticesToSST writes the X and Y vertices to the
 * 3dfx chip 64 bits at a time. This function is a support
 * routine for Rush's modified 2 TMU drawtriangle primative.
 * 
 * At entry:
 *	A0 - pointer to GrVertex A
 *	A1 - pointer to GrVertex B
 *	A2 - pointer to GrVertex C
 * At exit:
 *	v1 clobbered.
 *	Returns pointer to SST.
 */

	.ent	_grVerticesToSST
	.frame	$29, 0, $31
	.mask	0, 0
	.fmask	0, 0
	.globl	_grVerticesToSST
_grVerticesToSST:
	.set noreorder
	la	v0, SST_BASE
	ld	v1, X_OFFSET(a0)
	sd	v1, VAX_OFFSET(v0)
	ld	v1, X_OFFSET(a1)
	sd	v1, VBX_OFFSET(v0)
	ld	v1, X_OFFSET(a2)
	j	ra
	sd	v1, VCX_OFFSET(v0)
	.set reorder
	.end	_grVerticesToSST

#ifdef DRAW_GT_NAME
/* ----------------------------------------------------------//BF-7/28/97 */
#define F_A_R	F_A_A		/* A red */
#define F_B_R	F_B_A		/* B red */
#define F_C_R	F_C_A		/* C red */
#define F_A_G	F_A_S		/* A green */
#define F_B_G	F_B_S		/* B green */
#define F_C_G	F_C_S		/* C green */
#define F_A_B	F_A_T		/* A blue */
#define F_B_B	F_B_T		/* B blue */
#define F_C_B	F_C_T		/* C blue */
#define F_R_AmB F_A_AmB		/* red A - B */
#define F_R_BmC F_A_BmC		/* red B - C */
#define F_G_AmB F_S_AmB		/* green A - B */
#define F_G_BmC F_S_BmC		/* green B - C */
#define F_B_AmB F_T_AmB		/* blue A - B */
#define F_B_BmC F_T_BmC		/* blue B - C */
/*****************************************************************************
 *
 * DrawTriGT - This function is used by AirRace to do ASTW+RGB polys
 *
 *****************************************************************************/

#undef FTMP1
#undef FTMP2
#define FTMP1	$f5
#define FTMP2	$f6
	.ent	DRAW_GT_NAME
	.frame	$29, 0, $31
	.mask	0, 0
	.fmask	0, 0
	.globl	DRAW_GT_NAME
DRAW_GT_NAME:
	.set noreorder
	mfc0	SAVSR, C0_SR			;# get current SR
	li	SSTc, SST_BASEc			;# pointer to SST cached
	and	ITMP1, SAVSR, ~SR_IE		;# disable interrupts
#if !MIPS4
	or	ITMP1, SR_FR			;# enable all FP regs
#endif
	mtc0	ITMP1, C0_SR
#if TIME_IT
	mfc0	TIME, C0_COUNT			;# record start time
#else
	nop
#endif
	lwc1	tay, Y_OFFSET(A)		;# pickup the three Y values
	lwc1	tby, Y_OFFSET(B)
	lwc1	tcy, Y_OFFSET(C)

#if VALIDATE_PARAMS
	li.s	FTMP1, VALIDATE_LOWER_Y		;# for debugging, check params are in 
limits
	c.lt.s	tay, FTMP1
	bc1t	reject				;# and ignore triangle if not
	c.lt.s	tby, FTMP1
	bc1t	reject
	c.lt.s	tcy, FTMP1
	bc1t	reject
	li.s	FTMP1, VALIDATE_UPPER_Y
	c.lt.s	FTMP1, tay
	bc1t	reject
	c.lt.s	FTMP1, tby
	bc1t	reject
	c.lt.s	FTMP1, tcy
	bc1t	reject
#endif
	move	NEGATE, r0			;# assume not to negate
	c.lt.s	tay, tby			;# if (va->y < vb->y)
	bc1f	20f				;# 
	c.le.s	tby, tcy			;#   if (vb->y > vc->y)
	bc1t	gtsort_done			;#            (else it is already sorted)
	c.lt.s	tay, tcy			;#     if (va->y < vc->y)
	bc1f	10f				;# 
	move	ITMP1, B			;# 	  a=a, b=c, c=b
	move	B, C				;#	  negate = 1
	move	C, ITMP1
	b	gtsort_done
	or	NEGATE, r0, 1

10:	move	ITMP1, A			;#      else 
	move	ITMP2, B			;#	  a=c, b=a, c=b
	move	A, C
	move	B, ITMP1
	b	gtsort_done
	move	C, ITMP2

20:	c.lt.s	tby, tcy			;#   else if (vb-Y < vc->y)
	bc1f	40f				;# 
	c.lt.s	tay, tcy			;#      if (va->y < vc->y)
	bc1f	30f				;#
	move	ITMP1, B			;#        a=b, b=a, c=c
	move	B, A				;#	  negate = 1
	move	A, ITMP1
	b	gtsort_done
	or	NEGATE, r0, 1

30:	move	ITMP1, A			;#      else 
	move	A, B				;#	  a=b, b=c, c=a
	move	B, C
	b	gtsort_done
	move	C, ITMP1

40:	move	ITMP1, A			;#   else 
	move	A, C				;#	a=c, b=b, c=a
	move	C, ITMP1			;#	negate = 1
;#	b	gtsort_done			;# fall through to sort done
	or	NEGATE, r0, 1

	.align	3
gtsort_done:

	lwc1	dxAB, X_OFFSET(A)
	lwc1	dxBC, X_OFFSET(B)
	lwc1	FTMP1, X_OFFSET(C)
#if VALIDATE_PARAMS
	li.s	FTMP2, VALIDATE_LOWER_X
	c.lt.s	dxAB, FTMP2
	bc1t	reject
	c.lt.s	dxBC, FTMP2
	bc1t	reject
	c.lt.s	FTMP1, FTMP2
	bc1t	reject
	li.s	FTMP2, VALIDATE_UPPER_X
	c.lt.s	FTMP2, dxAB
	bc1t	reject
	c.lt.s	FTMP2, dxBC
	bc1t	reject
	c.lt.s	FTMP2, FTMP1
	bc1t	reject
#endif
	sub.s	dxAB, dxBC			;# dxAB = A(x) - B(x)
	sub.s	dxBC, FTMP1			;# dxBC = B(x) - C(x)
	lwc1	dyAB, Y_OFFSET(A)
	lwc1	dyBC, Y_OFFSET(B)
	lwc1	FTMP1, Y_OFFSET(C)
	sub.s	dyAB, dyBC			;# dyAB = A(y) - B(y)
	sub.s	dyBC, FTMP1			;# dyBC = B(y) - C(y)
	mul.s	farea, dxAB, dyBC
	mul.s	FTMP1, dxBC, dyAB
	sub.s	farea, FTMP1			;# farea = dxAB*dyBC - dxBC*dyAB
	beq	NEGATE, r0, 50f			;# not to negate
	mov.s	FTMP1, farea

	neg.s	FTMP1
50:	li.s	FTMP2, DRAW_MIN_AREA		;# get arbitrary area limit (NOTE: 3dfx crashes
						;#  if area > 0.0 but less than something less
						;#   than 1.0. By default, we toss triangles
						;#     with areas of one pixel or less).
	c.le.s	FTMP1, FTMP2
	bc1t	culled				;# dismiss triangles with areas too small
#if VALIDATE_PARAMS
	swc1	farea, dms_t_lastarea		;# remember this so when we crash we can tell why
#endif

	li	SST, SST_BASE			;# pointer to SST non-cached
	ld	ITMP1, X_OFFSET(A)		;# copy A, B, Cs X and Y params to output
	ld	ITMP2, X_OFFSET(B)
	sd	ITMP1, VAX_OFFSET(SST)
	ld	ITMP1, X_OFFSET(C)
	sd	ITMP2, VBX_OFFSET(SST)
	sd	ITMP1, VCX_OFFSET(SST)

#undef FTMP1				/* can't use these for anything anymore */
#undef FTMP2
	lwc1	F_A_A, ALPHA_OFFSET(A)		;# get all the params in regs so we can
	lwc1	F_B_A, ALPHA_OFFSET(B)
	lwc1	F_C_A, ALPHA_OFFSET(C)
	lwc1	F_A_S, TMU0_S_OFFSET(A)		;#   safely claim a cache line all to ourself
	lwc1	F_B_S, TMU0_S_OFFSET(B)
	lwc1	F_C_S, TMU0_S_OFFSET(C)
	lwc1	F_A_T, TMU0_T_OFFSET(A)
	lwc1	F_B_T, TMU0_T_OFFSET(B)
	lwc1	F_C_T, TMU0_T_OFFSET(C)
	lwc1	F_A_W, W_OFFSET(A)
	lwc1	F_B_W, W_OFFSET(B)
	lwc1	F_C_W, W_OFFSET(C)

/* WARNINIG DANGER WARNING DANGER 
 * The following only saves the lower 32 bits of the 11 floating registers $f20-$f30.
 * ALL Your C code needs to be compiled -fshort-double for this to work correctly.
 * If it is used with higher level code that expects to use 64 bit doubles, it will fail
 * that code. Since we (AGC) only use short-doubles (32 bit) and there aren't enough I
 * registers to save all 11 of the needed double registers, we do a big cheat here.
 */
	and	ITMP1, SAVSR, SR_FR
	beq	ITMP1, r0, 10f
	addu	ITMP3, r0, -1		/* li ITMP3, -1 without using a macro */

	dsll32	ITMP3, 0
	nor	ITMP3, r0, ITMP3
	mfc1	ITMP1, $f20
	mfc1	ITMP2, $f21
	and	ITMP1, ITMP3
	dsll32	ITMP2, 0
	or	SAV_F20, ITMP1, ITMP2
	mfc1	ITMP1, $f22
	mfc1	ITMP2, $f23
	and	ITMP1, ITMP3
	dsll32	ITMP2, 0
	or	SAV_F22, ITMP1, ITMP2
	mfc1	ITMP1, $f24
	mfc1	ITMP2, $f25
	and	ITMP1, ITMP3
	dsll32	ITMP2, 0
	or	SAV_F24, ITMP1, ITMP2
	mfc1	ITMP1, $f26
	mfc1	ITMP2, $f27
	and	ITMP1, ITMP3
	dsll32	ITMP2, 0
	or	SAV_F26, ITMP1, ITMP2
	mfc1	ITMP1, $f28
	mfc1	ITMP2, $f29
	and	ITMP1, ITMP3
	dsll32	ITMP2, 0
	or	SAV_F28, ITMP1, ITMP2
	b	20f
	mfc1	SAV_F30, $f30

10:
	mfc1	SAV_F20, $f20		/* NOTE: Only saves the even regs (dmfc1 doesn't work) */
	mfc1	SAV_F22, $f22
	mfc1	SAV_F24, $f24
	mfc1	SAV_F26, $f26
	mfc1	SAV_F28, $f28
	mfc1	SAV_F30, $f30
20:
	cache	DCACHE_CLAIM, DSDX_OFFSET(SSTc)	;# claim 1 cache line for our exclusive use

#define FTMP1 F_A_AmB
#if MIPS4
	recip.s	FTMP1, farea			;# FTMP1 = ooa = 1.0/area
#else
	li.s	FTMP1, 1.0
	div.s	FTMP1, farea
#endif
	mul.s	dxBC, FTMP1			;# dxBC *= ooa;
	mul.s	dyAB, FTMP1			;# dyAB *= ooa;
	mul.s	dxAB, FTMP1			;# dxAB *= ooa;
	mul.s	dyBC, FTMP1			;# dyBC *= ooa;
#undef FTMP1				/* can't use these for anything anymore */

#if VALIDATE_PARAMS
# define F_MIN_ALP	F_A_AmB
# define F_MAX_ALP	F_A_BmC
	li.s	F_MAX_ALP, 255.0		;# max alpha value 
	li.s	F_MIN_ALP, 0.0			;# min alpha value
	c.lt.s	F_A_A, F_MIN_ALP
	bc1t	cache_reject
	c.lt.s	F_MAX_ALP, F_A_A
	bc1t	cache_reject
	c.lt.s	F_B_A, F_MIN_ALP 
	bc1t	cache_reject
	c.lt.s	F_MAX_ALP, F_B_A
	bc1t	cache_reject
	c.lt.s	F_C_A, F_MIN_ALP
	bc1t	cache_reject
	c.lt.s	F_MAX_ALP, F_C_A
	bc1t	cache_reject
# undef F_MIN_ALP
# undef F_MAX_ALP
#endif

#if 0

/* This is the untangled version of the CACHE code listed on the other side of
 * the #else below. It is here just for reference purposes since the tangled
 * version is "write only" source code. The tangled version is simply this
 * code rearranged to fill as many of the pipline slots as possible as well
 * as to take advantage of the 5k's dual instruction issue.
 */
	sub.s	F_A_AmB, F_A_A, F_B_A		;# daAB = A - B
	sub.s	F_A_BmC, F_B_A, F_C_A		;# daBC = B - C
	mul.s	F_AB_BCx, F_A_AmB, dyBC		;# daAB*dyBC
	mul.s	F_BC_ABx, F_A_BmC, dyAB		;# daBC*dyAB 
	sub.s	F_DnDX, F_AB_BCx, F_BC_ABx	;# dadx = daAB*dyBC - daBC*dyAB
	mul.s	F_BC_ABy, F_A_BmC, dxAB		;# daBC*dxAB
	mul.s	F_AB_BCy, F_A_AmB, dxBC		;# daAB*dxBC
	sub.s	F_DnDY, F_BC_ABy, F_AB_BCy	;# dady = daBC*dxAB - daAB*dxBC

	mfc1	ITMP1, F_A_A			;# put A alpha into GPR (note: sign extends)
	dsll32	ITMP1, 0			;# undo sign extend from mfc1 above, part 1
	dsrl32	ITMP1, 0			;# undo sign extend from mfc1 above, part 2
	mfc1	ITMP2, F_DnDX			;# get dadx
	dsll32	ITMP2, 0			;# put dadx in bits 63-32
	or	ITMP2, ITMP1			;# mix in A alpha_start
	sd	ITMP2, START_A_OFFSET(SST)	;# write A and dadx to 3dfx
	mfc1	ITMP3, F_DnDY			;# get dady
	dsll32	ITMP3, 0			;# undo sign extend of dady, part 1
	dsrl32	ITMP3, 0			;# undo sign extend of dady, part 2
	mfc1	ITMP1, F_A_S			;# get A S
	dsll32	ITMP1, 0			;# put S into bits 63-32
	or	ITMP1, ITMP3			;# mix dady and S
	sd	ITMP1, DADY_OFFSET(SST)		;# send to 3dfx

	sub.s	F_S_AmB, F_A_S, F_B_S		;# dsAB = A - B
	sub.s	F_S_BmC, F_B_S, F_C_S		;# dsBC = B - C
	mul.s	F_AB_BCx, F_S_AmB, dyBC		;# dsAB*dyBC
	mul.s	F_BC_ABx, F_S_BmC, dyAB		;# dsBC*dyAB 
	mul.s	F_BC_ABy, F_S_BmC, dxAB		;# dsBC*dxAB
	mul.s	F_AB_BCy, F_S_AmB, dxBC		;# dsAB*dxBC
	sub.s	F_DnDX, F_AB_BCx, F_BC_ABx	;# dsdx = dsAB*dyBC - dsBC*dyAB
	sub.s	F_DnDY, F_BC_ABy, F_AB_BCy	;# dsdy = dsBC*dxAB - dsAB*dxBC
	swc1	F_DnDX, DSDX_OFFSET(SSTc)	;# send dsdx to cache
	swc1	F_DnDY, DSDY_OFFSET(SSTc)	;# send dsdy to cache

	sub.s	F_T_AmB, F_A_T, F_B_T		;# dtAB = A - B
	sub.s	F_T_BmC, F_B_T, F_C_T		;# dtBC = B - C
	mul.s	F_AB_BCx, F_T_AmB, dyBC		;# dtAB*dyBC
	mul.s	F_BC_ABx, F_T_BmC, dyAB		;# dtBC*dyAB 
	mul.s	F_BC_ABy, F_T_BmC, dxAB		;# dtBC*dxAB
	mul.s	F_AB_BCy, F_T_AmB, dxBC		;# dtAB*dxBC
	sub.s	F_DnDX, F_AB_BCx, F_BC_ABx	;# dtdx = dtAB*dyBC - dtBC*dyAB
	sub.s	F_DnDY, F_BC_ABy, F_AB_BCy	;# dtdy = dtBC*dxAB - dtAB*dxBC
	swc1	F_A_T, START_T_OFFSET(SSTc)	;# sent T to cache
	swc1	F_DnDX, DTDX_OFFSET(SSTc)	;# sent dtdx to cache
	swc1	F_DnDY, DTDY_OFFSET(SSTc)	;# send dtdy to cache

	sub.s	F_W_AmB, F_A_W, F_B_W		;# dwAB = A - B
	sub.s	F_W_BmC, F_B_W, F_C_W		;# dwBC = B - C
	mul.s	F_AB_BCx, F_W_AmB, dyBC		;# dwAB*dyBC
	mul.s	F_BC_ABx, F_W_BmC, dyAB		;# dwBC*dyAB 
	mul.s	F_BC_ABy, F_W_BmC, dxAB		;# dwBC*dxAB
	mul.s	F_AB_BCy, F_W_AmB, dxBC		;# dwAB*dxBC
	sub.s	F_DnDX, F_AB_BCx, F_BC_ABx	;# dwdx = dwAB*dyBC - dwBC*dyAB
	sub.s	F_DnDY, F_BC_ABy, F_AB_BCy	;# dwdy = dwBC*dxAB - dwAB*dxBC
	swc1	F_A_W, START_W_OFFSET(SSTc)	;# sent W to cache
	swc1	F_DnDX, DWDX_OFFSET(SSTc)	;# sent dwdx to cache
	swc1	F_DnDY, DWDY_OFFSET(SSTc)	;# send dwdy to cache

	cache	DCACHE_FLUSH, DSDX_OFFSET(SSTc)	;# flush cache line to PCI write buffer

	lwc1	F_A_R, RED_OFFSET(A)		;# load up RGB params
	lwc1	F_B_R, RED_OFFSET(B)
	lwc1	F_C_R, RED_OFFSET(C)
	lwc1	F_A_G, GREEN_OFFSET(A)
	lwc1	F_B_G, GREEN_OFFSET(B)
	lwc1	F_C_G, GREEN_OFFSET(C)
	lwc1	F_A_B, BLUE_OFFSET(A)
	lwc1	F_B_B, BLUE_OFFSET(B)
	lwc1	F_C_B, BLUE_OFFSET(C)

	cache	DCACHE_CLAIM, START_R_OFFSET(SSTc) ;# claim a cache line on RGB params

	sub.s	F_R_AmB, F_A_R, F_B_R		;# drAB = A - B
	sub.s	F_R_BmC, F_B_R, F_C_R		;# drBC = B - C
	mul.s	F_AB_BCx, F_R_AmB, dyBC		;# drAB*dyBC
	mul.s	F_BC_ABx, F_R_BmC, dyAB		;# drBC*dyAB 
	mul.s	F_BC_ABy, F_R_BmC, dxAB		;# drBC*dxAB
	mul.s	F_AB_BCy, F_R_AmB, dxBC		;# drAB*dxBC
	sub.s	F_DnDX, F_AB_BCx, F_BC_ABx	;# drdx = drAB*dyBC - drBC*dyAB
	sub.s	F_DnDY, F_BC_ABy, F_AB_BCy	;# drdy = drBC*dxAB - drAB*dxBC

	swc1	F_A_R, START_R_OFFSET(SSTc)	;# put R into cache
	swc1	F_DnDX, DRDX_OFFSET(SSTc)	;# put drdx into cache
	swc1	F_DnDY, DRDY_OFFSET(SSTc)	;# put drdy into cache

	sub.s	F_G_AmB, F_A_G, F_B_G		;# dgAB = A - B
	sub.s	F_G_BmC, F_B_G, F_C_G		;# dgBC = B - C
	mul.s	F_AB_BCx, F_G_AmB, dyBC		;# dgAB*dyBC
	mul.s	F_BC_ABx, F_G_BmC, dyAB		;# dgBC*dyAB 
	mul.s	F_BC_ABy, F_G_BmC, dxAB		;# dgBC*dxAB
	mul.s	F_AB_BCy, F_G_AmB, dxBC		;# dgAB*dxBC
	sub.s	F_DnDX, F_AB_BCx, F_BC_ABx	;# dgdx = dgAB*dyBC - dgBC*dyAB
	sub.s	F_DnDY, F_BC_ABy, F_AB_BCy	;# dgdy = dgBC*dxAB - dgAB*dxBC

	swc1	F_A_G, START_G_OFFSET(SSTc)	;# put G into cache
	swc1	F_DnDX, DGDX_OFFSET(SSTc)	;# put dgdx into cache
	swc1	F_DnDY, DGDY_OFFSET(SSTc)	;# put dgdy into cache

	sub.s	F_B_AmB, F_A_B, F_B_B		;# dbAB = A - B
	sub.s	F_B_BmC, F_B_B, F_C_B		;# dbBC = B - C
	mul.s	F_AB_BCx, F_B_AmB, dyBC		;# dbAB*dyBC
	mul.s	F_BC_ABx, F_B_BmC, dyAB		;# dbBC*dyAB 
	mul.s	F_BC_ABy, F_B_BmC, dxAB		;# dbBC*dxAB
	mul.s	F_AB_BCy, F_B_AmB, dxBC		;# dbAB*dxBC
	sub.s	F_DnDX, F_AB_BCx, F_BC_ABx	;# dbdx = dbAB*dyBC - dbBC*dyAB
	sub.s	F_DnDY, F_BC_ABy, F_AB_BCy	;# dbdy = dbBC*dxAB - dbAB*dxBC

	swc1	F_A_B, START_B_OFFSET(SSTc)	;# put B into cache
	swc1	F_DnDX, DBDX_OFFSET(SSTc)	;# put dbdx into cache
	swc1	F_DnDY, DBDY_OFFSET(SST)	;# put dbdy into non-cache

	cache	DCACHE_FLUSH, START_R_OFFSET(SSTc) ;# flush cache line to PCI write buffer

#else

/* This is the _real_ code. Good luck trying to follow it. */

	sub.s	F_A_AmB, F_A_A, F_B_A		;# daAB = A - B
	sub.s	F_A_BmC, F_B_A, F_C_A		;# daBC = B - C
	sub.s	F_S_AmB, F_A_S, F_B_S		;# dsAB = A - B
	sub.s	F_S_BmC, F_B_S, F_C_S		;# dsBC = B - C
	sub.s	F_T_AmB, F_A_T, F_B_T		;# dtAB = A - B
	mul.s	F_AB_BCx, F_A_AmB, dyBC		;# daAB*dyBC
	mul.s	F_BC_ABx, F_A_BmC, dyAB		;# daBC*dyAB 
	mul.s	F_BC_ABy, F_A_BmC, dxAB		;# daBC*dxAB
	mul.s	F_AB_BCy, F_A_AmB, dxBC		;# daAB*dxBC
	sub.s	F_T_BmC, F_B_T, F_C_T		;# dtBC = B - C
	sub.s	F_W_AmB, F_A_W, F_B_W		;# dwAB = A - B
	sub.s	F_DnDX, F_AB_BCx, F_BC_ABx	;# dadx = daAB*dyBC - daBC*dyAB
	mul.s	F_AB_BCx, F_S_AmB, dyBC		;# dsAB*dyBC
	mul.s	F_BC_ABx, F_S_BmC, dyAB		;# dsBC*dyAB 
	sub.s	F_DnDY, F_BC_ABy, F_AB_BCy	;# dady = daBC*dxAB - daAB*dxBC
	mul.s	F_AB_BCy, F_S_AmB, dxBC		;# dsAB*dxBC
	mfc1	ITMP2, F_DnDX			;# get dadx
	mul.s	F_BC_ABy, F_S_BmC, dxAB		;# dsBC*dxAB
	mfc1	ITMP1, F_A_A			;# put A alpha into GPR (note: sign extends)
	mfc1	ITMP3, F_DnDY			;# get dady
	sub.s	F_W_BmC, F_B_W, F_C_W		;# dwBC = B - C
	dsll32	ITMP1, 0			;# undo sign extend from mfc1 above, part 1
	sub.s	F_DnDX, F_AB_BCx, F_BC_ABx	;# dsdx = dsAB*dyBC - dsBC*dyAB
	dsrl32	ITMP1, 0			;# undo sign extend from mfc1 above, part 2
	sub.s	F_DnDY, F_BC_ABy, F_AB_BCy	;# dsdy = dsBC*dxAB - dsAB*dxBC
	dsll32	ITMP2, 0			;# put dadx in bits 63-32
	mul.s	F_AB_BCx, F_T_AmB, dyBC		;# dtAB*dyBC
	or	ITMP2, ITMP1			;# mix in A alpha_start
	mul.s	F_BC_ABx, F_T_BmC, dyAB		;# dtBC*dyAB 
	sd	ITMP2, START_A_OFFSET(SST)	;# write A and dadx to 3dfx
	mfc1	ITMP1, F_A_S			;# get A S
	swc1	F_DnDX, DSDX_OFFSET(SSTc)	;# send dsdx to cache
	dsll32	ITMP1, 0			;# put S into bits 63-32
	swc1	F_DnDY, DSDY_OFFSET(SSTc)	;# send dsdy to cache
	dsll32	ITMP3, 0			;# undo sign extend of dady, part 1
	mul.s	F_BC_ABy, F_T_BmC, dxAB		;# dtBC*dxAB
	dsrl32	ITMP3, 0			;# undo sign extend of dady, part 2
	mul.s	F_AB_BCy, F_T_AmB, dxBC		;# dtAB*dxBC
	or	ITMP1, ITMP3			;# mix dady and S
	sub.s	F_DnDX, F_AB_BCx, F_BC_ABx	;# dtdx = dtAB*dyBC - dtBC*dyAB
	mul.s	F_AB_BCx, F_W_AmB, dyBC		;# dwAB*dyBC
	sd	ITMP1, DADY_OFFSET(SST)		;# send to 3dfx
	swc1	F_A_T, START_T_OFFSET(SSTc)	;# sent T to cache
	sub.s	F_DnDY, F_BC_ABy, F_AB_BCy	;# dtdy = dtBC*dxAB - dtAB*dxBC
	mul.s	F_BC_ABx, F_W_BmC, dyAB		;# dwBC*dyAB 
	mul.s	F_BC_ABy, F_W_BmC, dxAB		;# dwBC*dxAB
	mul.s	F_AB_BCy, F_W_AmB, dxBC		;# dwAB*dxBC
	swc1	F_DnDX, DTDX_OFFSET(SSTc)	;# sent dtdx to cache
	swc1	F_DnDY, DTDY_OFFSET(SSTc)	;# send dtdy to cache
	sub.s	F_DnDX, F_AB_BCx, F_BC_ABx	;# dwdx = dwAB*dyBC - dwBC*dyAB
	swc1	F_A_W, START_W_OFFSET(SSTc)	;# sent W to cache
	sub.s	F_DnDY, F_BC_ABy, F_AB_BCy	;# dwdy = dwBC*dxAB - dwAB*dxBC
	swc1	F_DnDX, DWDX_OFFSET(SSTc)	;# sent dwdx to cache
	swc1	F_DnDY, DWDY_OFFSET(SSTc)	;# send dwdy to cache

	cache	DCACHE_FLUSH, DSDX_OFFSET(SSTc)	;# flush cache line to write buffer

	lwc1	F_A_R, RED_OFFSET(A)		;# load up RGB params
	lwc1	F_B_R, RED_OFFSET(B)
	lwc1	F_C_R, RED_OFFSET(C)
	lwc1	F_A_G, GREEN_OFFSET(A)
	lwc1	F_B_G, GREEN_OFFSET(B)
	lwc1	F_C_G, GREEN_OFFSET(C)
	lwc1	F_A_B, BLUE_OFFSET(A)
	lwc1	F_B_B, BLUE_OFFSET(B)
	lwc1	F_C_B, BLUE_OFFSET(C)

	cache	DCACHE_CLAIM, START_R_OFFSET(SSTc) ;# claim a cache line over RGB params

	sub.s	F_R_AmB, F_A_R, F_B_R		;# drAB = A - B
	swc1	F_A_R, START_R_OFFSET(SSTc)	;# put R into cache
	swc1	F_A_G, START_G_OFFSET(SSTc)	;# put G into cache
	swc1	F_A_B, START_B_OFFSET(SSTc)	;# put B into cache
	sub.s	F_R_BmC, F_B_R, F_C_R		;# drBC = B - C
	mul.s	F_AB_BCx, F_R_AmB, dyBC		;# drAB*dyBC
	mul.s	F_AB_BCy, F_R_AmB, dxBC		;# drAB*dxBC
	mul.s	F_BC_ABx, F_R_BmC, dyAB		;# drBC*dyAB 
	mul.s	F_BC_ABy, F_R_BmC, dxAB		;# drBC*dxAB
	sub.s	F_DnDX, F_AB_BCx, F_BC_ABx	;# drdx = drAB*dyBC - drBC*dyAB
	sub.s	F_DnDY, F_BC_ABy, F_AB_BCy	;# drdy = drBC*dxAB - drAB*dxBC

	sub.s	F_G_AmB, F_A_G, F_B_G		;# dgAB = A - B
	sub.s	F_G_BmC, F_B_G, F_C_G		;# dgBC = B - C
	swc1	F_DnDX, DRDX_OFFSET(SSTc)	;# put drdx into cache
	swc1	F_DnDY, DRDY_OFFSET(SSTc)	;# put drdy into cache
	mul.s	F_AB_BCx, F_G_AmB, dyBC		;# dgAB*dyBC
	mul.s	F_AB_BCy, F_G_AmB, dxBC		;# dgAB*dxBC
	mul.s	F_BC_ABx, F_G_BmC, dyAB		;# dgBC*dyAB 
	mul.s	F_BC_ABy, F_G_BmC, dxAB		;# dgBC*dxAB
	sub.s	F_DnDX, F_AB_BCx, F_BC_ABx	;# dgdx = dgAB*dyBC - dgBC*dyAB
	sub.s	F_DnDY, F_BC_ABy, F_AB_BCy	;# dgdy = dgBC*dxAB - dgAB*dxBC

	sub.s	F_B_AmB, F_A_B, F_B_B		;# dbAB = A - B
	sub.s	F_B_BmC, F_B_B, F_C_B		;# dbBC = B - C
	swc1	F_DnDX, DGDX_OFFSET(SSTc)	;# put dgdx into cache
	swc1	F_DnDY, DGDY_OFFSET(SSTc)	;# put dgdy into cache
	mul.s	F_AB_BCx, F_B_AmB, dyBC		;# dbAB*dyBC
	mul.s	F_AB_BCy, F_B_AmB, dxBC		;# dbAB*dxBC
	mul.s	F_BC_ABx, F_B_BmC, dyAB		;# dbBC*dyAB 
	mul.s	F_BC_ABy, F_B_BmC, dxAB		;# dbBC*dxAB
	sub.s	F_DnDX, F_AB_BCx, F_BC_ABx	;# dbdx = dbAB*dyBC - dbBC*dyAB
	sub.s	F_DnDY, F_BC_ABy, F_AB_BCy	;# dbdy = dbBC*dxAB - dbAB*dxBC

	swc1	F_DnDX, DBDX_OFFSET(SSTc)	;# put dbdx into cache
	swc1	F_DnDY, DBDY_OFFSET(SST)	;# put dbdy into non-cache

	cache	DCACHE_FLUSH, START_R_OFFSET(SSTc) ;# flush cache line to PCI write buffer
#endif

	swc1	farea, FTRIANGLE_START(SST)	;# start the triangle

	and	ITMP1, SAVSR, SR_FR
	beq	ITMP1, r0, 10f
	mtc1	SAV_F20, $f20

	dsrl32	SAV_F20, 0
	mtc1	SAV_F20, $f21
	mtc1	SAV_F22, $f22
	dsrl32	SAV_F22, 0
	mtc1	SAV_F22, $f23
	mtc1	SAV_F24, $f24
	dsrl32	SAV_F24, 0
	mtc1	SAV_F24, $f25
	mtc1	SAV_F26, $f26
	dsrl32	SAV_F26, 0
	mtc1	SAV_F26, $f27
	mtc1	SAV_F28, $f28
	dsrl32	SAV_F28, 0
	mtc1	SAV_F28, $f29
	b	20f
	mtc1	SAV_F30, $f30

10:	mtc1	SAV_F22, $f22
	mtc1	SAV_F24, $f24
	mtc1	SAV_F26, $f26
	mtc1	SAV_F28, $f28
	mtc1	SAV_F30, $f30
20:
#if TIME_IT
	COMPUTE_TIMES()
#endif

	j	ra
	mtc0	SAVSR, C0_SR

	.set reorder
	.end	DRAW_GT_NAME
#endif			/* DRAW_GT_NAME */
/* //BF-7/28/97 */

