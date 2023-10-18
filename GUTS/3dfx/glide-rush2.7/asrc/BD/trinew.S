/****************************************************************************/
/*                                                                          */
/* trisetup.S - Source for _grDrawTriangleNonAA glide library function.     */
/*                                                                          */
/* Written by:  Michael J. Lynch                                            */
/* Copyright (c) 1996 by Williams Electronics Games Inc.                    */
/* All rights Reserved                                                      */
/*                                                                          */
/* Use, duplication, or disclosure is strictly forbidden unless approved,   */
/* in writing by Williams Electronics Games Inc.                            */
/*                                                                          */
/****************************************************************************/

/****************************************************************************/
/*                                                                          */
/* These definitions are linked to structures defined in glide.h.  If any   */
/* of those structure definitions changed, these offset values must also    */
/* be changed to match the new field offsets.                               */
/*                                                                          */
/****************************************************************************/
#define	LIGHTING_SUPPORT	0
#define	DMA_SUPPORT		0


/****************************************************************************/
/*                                                                          */
/* Offsets to fields of the _SST structure.                                 */
/*                                                                          */
/****************************************************************************/
#define	STATE_PARAMINDEX_OFFSET			0x114


/****************************************************************************/
/*                                                                          */
/* Offsets to fields of the GrVertex structure.                             */
/*                                                                          */
/****************************************************************************/
#define	X_OFFSET	0
#define	Y_OFFSET	4
#define	Z_OFFSET	8
#define	OOZ_OFFSET	12
#define	OOW_OFFSET	16
#define	RED_OFFSET	20
#define	GREEN_OFFSET	24
#define	BLUE_OFFSET	28
#define	ALPHA_OFFSET	32

#define	GLIDE_NUM_TMU	2

#if (GLIDE_NUM_TMU != 2)
#err TMUS
#define	TMU0_W_OFFSET	36
#define	TMU0_S_OFFSET	40
#define	TMU0_T_OFFSET	44
#if (GLIDE_NUM_TMU > 2)
#define	TMU1_W_OFFSET	48
#define	TMU1_S_OFFSET	52
#define	TMU1_T_OFFSET	56
#define	TMU2_W_OFFSET	60
#define	TMU2_S_OFFSET	64
#define	TMU2_T_OFFSET	68
#endif
#else
#define	TMU0_W_OFFSET	40
#define	TMU0_S_OFFSET	44
#define	TMU0_T_OFFSET	48
#define	TMU1_W_OFFSET	52
#define	TMU1_S_OFFSET	56
#define	TMU1_T_OFFSET	60
#endif

/****************************************************************************/
/*                                                                          */
/* Offsets to the hardware registers.                                       */
/*                                                                          */
/****************************************************************************/
#define	VAX_OFFSET	0x88
#define	VBX_OFFSET	0x90
#define	VCX_OFFSET	0x98

#define	START_R_OFFSET	0xa0
#define	DRDX_OFFSET	0xa4
#define	DRDY_OFFSET	0xa8
#define	START_G_OFFSET	0xac
#define	DGDX_OFFSET	0xb0
#define	DGDY_OFFSET	0xb4
#define	START_B_OFFSET	0xb8
#define	DBDX_OFFSET	0xbc

#define	DBDY_OFFSET	0xc0
#define	START_Z_OFFSET	0xc4
#define	DZDX_OFFSET	0xc8
#define	DZDY_OFFSET	0xcc
#define	START_A_OFFSET	0xd0
#define	DADX_OFFSET	0xd4
#define	DADY_OFFSET	0xd8
#define	START_S_OFFSET	0xdc

#define	DSDX_OFFSET	0xe0
#define	DSDY_OFFSET	0xe4
#define	START_T_OFFSET	0xe8
#define	DTDX_OFFSET	0xec
#define	DTDY_OFFSET	0xf0
#define	START_W_OFFSET	0xf4
#define	DWDX_OFFSET	0xf8
#define	DWDY_OFFSET	0xfc

#define	FBI		0x400
#define	TMU0		0x800
#define	TMU1		0x1000
#define	TMU2		0x2000

#define	TRIANGLECMD_OFFSET	0x100


/****************************************************************************/
/*                                                                          */
/* void _grDrawTriangleNonAA(GrVertex *, GrVertex *, GrVertex *)            */
/*                                                                          */
/****************************************************************************/
/* Floating point register definitions */
#define	DYAB		$f0
#define	DXAB		$f2
#define	DYBC		$f4
#define	DXBC		$f6
#define	OOA		$f8
#define	AREA		$f10
#define	AY		$f12
#define	BY		$f14
#define	FTMP1		$f16
#define	FTMP2		$f18

#define	CY		FTMP2

#define	DPDX		OOA
#define	DPDY		FTMP1

#define	DA		AREA
#define	DB		AY
#define	DC		BY

#define	DXAB_HOLD	$2
#define	DYAB_HOLD	$3

#define	VTXA_PTR	$4
#define	VTXB_PTR	$5
#define	VTXC_PTR	$6

#define	TMP		$7
#define	VTXA_HOLD	$8
#define	VTXB_HOLD	$9
#define	VTXC_HOLD	$10
#define	NEG		$11
#define	SAVED_AREA	$12
#define	ITMP1		$13
#define	ITMP2		$14
#define	ITMP3		$15
#define	ITMP4		VTXA_HOLD
#define	ITMP5		VTXB_HOLD
#define	ITMP6		VTXC_HOLD

#define	DXAC_HOLD	$24
#define	DYAC_HOLD	$25

#define FIFO_LIMIT	0xA000

	.bss
	.globl	sst_memfifo_min

	.text
	.ent	_grDrawTriangleNonAANoCull
	.globl	_grDrawTriangleNonAANoCull
_grDrawTriangleNonAANoCull:
	.frame	$29, 0, $31
	.mask	1<<28, 0

	sw	$28, ($29)		# save gp
	li	$28, 0xa8200000		# ptr to SST1 in non-cached memory

5:	lw	ITMP1, ($28)		# get status
	lw	ITMP1, ($28)		# get status
	lw	ITMP2, sst_memfifo_min
	srl	ITMP1, 12		# get FIFO value
	and	ITMP1, 0xFFFF		# zap all unwanted bits
	bgt	ITMP1, ITMP2, 10f	# gt than min
	sw	ITMP1, sst_memfifo_min	# a new min
10:	li	ITMP2, FIFO_LIMIT	# minimum acceptable fifo size
	bltu	ITMP1, ITMP2, 5b	# wait until there is room
	
	mfc0	ITMP1, $12		# get C0_SR
	li	$28, 0x88200000		# ptr to SST1 in cached memory
	sw	ITMP1, 4($29)		# save SR
	and	ITMP1, -2		# disable interrupts

	mtc0	ITMP1, $12
	cache	(3<<2)|1, START_R_OFFSET($28)	# claim 3 cache lines for our exclusive use
	cache	(3<<2)|1, DBDY_OFFSET($28)
	cache	(3<<2)|1, DSDX_OFFSET($28)
	li	$28, 0xa8200000		# ptr to SST1 in non-cached memory

	.set	noreorder
	# First read the vertices into double gp registers */
	ld	VTXA_HOLD,X_OFFSET(VTXA_PTR)	# Get ax, ay
	ld	VTXB_HOLD,X_OFFSET(VTXB_PTR)	# Get bx, by
	mtc1	VTXA_HOLD,DXAB			# Get ax
	ld	VTXC_HOLD,X_OFFSET(VTXC_PTR)	# Get cx, cy

	# Now move the data to fp registers for calculation of
	# dxAB, dyAB, dxBC, dyBC, and triangle area
	mtc1	VTXB_HOLD,FTMP1			# Get bx
	lwc1	DYAB,Y_OFFSET(VTXA_PTR)		# Get ay
	sub.s	DXAB,DXAB,FTMP1			# Calc dxAB
	mov.s	AY,DYAB				# Save ay for sort
	lwc1	DYBC,Y_OFFSET(VTXB_PTR)		# Get by
	mtc1	VTXC_HOLD,DXBC			# Get cx
	sub.s	DYAB,DYAB,DYBC			# Calc dyAB
	mov.s	BY,DYBC				# Save by for sort
	lwc1	FTMP2,Y_OFFSET(VTXC_PTR)	# Get cy
	sub.s	DXBC,FTMP1,DXBC			# Calc dxBC
	li.s	OOA,1.0				# Set up for 1/area
	sub.s	DYBC,DYBC,FTMP2			# Calc dyBC

	# Save dxAB and dyAB
	mfc1	DXAB_HOLD,DXAB			# Save dxAB
	mfc1	DYAB_HOLD,DYAB			# Save dyAB

	# FTMP2 contains cy for sort

	# Calculate area
	mul.s	FTMP1,DYAB,DXBC			# Calc dxBC * dyAB
	j	_grDrawTriangle
	mul.s	AREA,DXAB,DYBC			# Calc dxAB * dyBC
	.set	reorder
	.end	_grDrawTriangleNonAANoCull


	.ent	_grDrawTriangleNonAA
	.globl	_grDrawTriangleNonAA
_grDrawTriangleNonAA:
	.frame	$29, 0, $31
	.mask	1<<28, 0
	sw	$28, ($29)			# save gp
	li	$28, 0xa8200000		# ptr to SST1

5:	lw	ITMP1, ($28)		# get status
	lw	ITMP1, ($28)		# get status
	lw	ITMP2, sst_memfifo_min
	srl	ITMP1, 12		# get FIFO value
	and	ITMP1, 0xFFFF		# zap all unwanted bits
	bgt	ITMP1, ITMP2, 10f	# gt than min
	sw	ITMP1, sst_memfifo_min	# a new min
10:	li	ITMP2, FIFO_LIMIT	# minimum acceptable fifo size
	blt	ITMP1, ITMP2, 5b	# wait until there is room

	mfc0	ITMP1, $12		# get C0_SR
	li	$28, 0x88200000		# ptr to SST1 in cached memory
	sw	ITMP1, 4($29)		# save SR
	and	ITMP1, -2		# disable interrupts

	mtc0	ITMP1, $12
	cache	(3<<2)|1, START_R_OFFSET($28)	# claim 3 cache lines for our exclusive use
	cache	(3<<2)|1, DBDY_OFFSET($28)
	cache	(3<<2)|1, DSDX_OFFSET($28)
	li	$28, 0xa8200000		# ptr to SST1 in non-cached memory

	.set	noreorder
	# First read the vertices into double gp registers */
	ld	VTXA_HOLD,X_OFFSET(VTXA_PTR)	# Get ax, ay
	ld	VTXB_HOLD,X_OFFSET(VTXB_PTR)	# Get bx, by
	mtc1	VTXA_HOLD,DXAB			# Get ax
	ld	VTXC_HOLD,X_OFFSET(VTXC_PTR)	# Get cx, cy

	# Now move the data to fp registers for calculation of
	# dxAB, dyAB, dxBC, dyBC, and triangle area
	mtc1	VTXB_HOLD,FTMP1			# Get bx
	lwc1	DYAB,Y_OFFSET(VTXA_PTR)		# Get ay
	sub.s	DXAB,DXAB,FTMP1			# Calc dxAB
	mov.s	AY,DYAB				# Save ay for sort
	lwc1	DYBC,Y_OFFSET(VTXB_PTR)		# Get by
	mtc1	VTXC_HOLD,DXBC			# Get cx
	sub.s	DYAB,DYAB,DYBC			# Calc dyAB
	mov.s	BY,DYBC				# Save by for sort
	lwc1	FTMP2,Y_OFFSET(VTXC_PTR)	# Get cy
	sub.s	DXBC,FTMP1,DXBC			# Calc dxBC
	li.s	OOA,1.0				# Set up for 1/area
	sub.s	DYBC,DYBC,FTMP2			# Calc dyBC

	# Save dxAB and dyAB
	mfc1	DXAB_HOLD,DXAB			# Save dxAB
	mfc1	DYAB_HOLD,DYAB			# Save dyAB

	# FTMP2 contains cy for sort

	# Calculate area
	mul.s	FTMP1,DYAB,DXBC			# Calc dxBC * dyAB
	mul.s	AREA,DXAB,DYBC			# Calc dxAB * dyBC

	c.le.s	AREA,FTMP1
	bc1t	triangle_culled
_grDrawTriangle:
	sub.s	AREA,AREA,FTMP1			# finish area calc (BDSLOT)

	# Calc dxAC for sorts
	mtc1	VTXA_HOLD,DXAB			# Get ax
	mtc1	VTXC_HOLD,FTMP1			# Get cx
	mfc1	SAVED_AREA,AREA
	div.s	OOA,OOA,AREA			# Start 1 over area divide

	sub.s	DXAB,DXAB,FTMP1			# Calc dxAC

#if (DMA_SUPPORT==1)
	xori	$28,0x1000			# Set the buffer base
#endif

	li	ITMP2,0x80000000		# Cheap * -1.0

	# Sort the vertices into ascending y order
	c.lt.s	AY,BY			# ay < by
	bc1f	_ay_gt_by		# br = no

	# (A B C), (A C B), (C A B)	(BDSLOT)
	c.le.s	BY,CY			# by > cy
	bc1t	_abc			# br = no (A B C)

	# (A C B), (C A B)		(BDSLOT)
	c.lt.s	AY,FTMP2		# ay < cy
	bc1f	_cab
	move	TMP,VTXB_PTR		# save b (BDSLOT)

	# (A C B)
	sd	VTXA_HOLD,VAX_OFFSET($28)
	xor	SAVED_AREA,SAVED_AREA,ITMP2
	move	VTXB_PTR,VTXC_PTR	# move c to b
	sub.s	DYAB,AY,FTMP2
	mfc1	ITMP1,DXBC
	xor	ITMP1,ITMP1,ITMP2
	mtc1	ITMP1,DXBC
	mfc1	ITMP1,DYBC
	xor	ITMP1,ITMP1,ITMP2
	mtc1	ITMP1,DYBC
	move	VTXC_PTR,TMP
	sd	VTXB_HOLD,VCX_OFFSET($28)
	j	_vsort_done
	sd	VTXC_HOLD,VBX_OFFSET($28)

_cab:	# (C A B)
	sd	VTXC_HOLD,VAX_OFFSET($28)
	move	VTXB_PTR,VTXA_PTR	# move a to b
	move	VTXA_PTR,VTXC_PTR	# move c to a
	mfc1	ITMP1,DXAB
	xor	ITMP1,ITMP1,ITMP2
	mtc1	ITMP1,DXAB
	mov.s	DYBC,DYAB
	sub.s	DYAB,FTMP2,AY
	mtc1	DXAB_HOLD,DXBC
	move	VTXC_PTR,TMP
	sd	VTXA_HOLD,VBX_OFFSET($28)
	j	_vsort_done_noneg
	sd	VTXB_HOLD,VCX_OFFSET($28)

_ay_gt_by:	# (B A C), (B C A), (C B A)
	c.lt.s	BY,FTMP2
	bc1f	_cba			# br = yes
	move	TMP,VTXA_PTR		# save a (BDSLOT)

	# (B A C), (B C A)
	c.lt.s	AY,FTMP2		# ay < cy
	bc1t	_bac			# br = yes
	
	# (B C A)
	sd	VTXB_HOLD,VAX_OFFSET($28)
	move	VTXA_PTR,VTXB_PTR	# move b to a
	move	VTXB_PTR,VTXC_PTR	# move c to b
	mfc1	ITMP1,DXAB
	xor	ITMP1,ITMP1,ITMP2
	sub.s	FTMP2,FTMP2,AY
	mov.s	DXAB,DXBC
	mov.s	DYAB,DYBC
	mtc1	ITMP1,DXBC
	mov.s	DYBC,FTMP2
	move	VTXC_PTR,TMP
	sd	VTXC_HOLD,VBX_OFFSET($28)
	j	_vsort_done_noneg
	sd	VTXA_HOLD,VCX_OFFSET($28)

_bac:	# (B A C)
	sd	VTXB_HOLD,VAX_OFFSET($28)
	xor	SAVED_AREA,SAVED_AREA,ITMP2
	move	VTXA_PTR,VTXB_PTR	# move b to a
	mov.s	DXBC,DXAB
	sub.s	DYBC,AY,FTMP2
	xor	DXAB_HOLD,DXAB_HOLD,ITMP2
	mtc1	DXAB_HOLD,DXAB
	xor	DYAB_HOLD,DYAB_HOLD,ITMP2
	mtc1	DYAB_HOLD,DYAB
	move	VTXB_PTR,TMP
	sd	VTXA_HOLD,VBX_OFFSET($28)
	j	_vsort_done
	sd	VTXC_HOLD,VCX_OFFSET($28)

_cba:	# (C B A)
	sd	VTXC_HOLD,VAX_OFFSET($28)
	xor	SAVED_AREA,SAVED_AREA,ITMP2
	move	VTXA_PTR,VTXC_PTR	# move a to c
	mfc1	ITMP1,DYBC
	xor	ITMP1,ITMP1,ITMP2
	mtc1	ITMP1,DYAB
	mfc1	ITMP1,DXBC
	xor	ITMP1,ITMP1,ITMP2
	mtc1	ITMP1,DXAB
	xor	DXAB_HOLD,DXAB_HOLD,ITMP2
	mtc1	DXAB_HOLD,DXBC
	xor	DYAB_HOLD,DYAB_HOLD,ITMP2
	sd	VTXB_HOLD,VBX_OFFSET($28)
	mtc1	DYAB_HOLD,DYBC
	move	VTXC_PTR,TMP
	j	_vsort_done
	sd	VTXA_HOLD,VCX_OFFSET($28)

_abc:
	sd	VTXA_HOLD,VAX_OFFSET($28)
	mtc1	DXAB_HOLD,DXAB
	mfc1	SAVED_AREA,AREA
	sd	VTXB_HOLD,VBX_OFFSET($28)
	j	_vsort_done_noneg
	sd	VTXC_HOLD,VCX_OFFSET($28)

_vsort_done:

	# Also need to negate OOA
	mfc1	TMP,OOA				# Save OOA
	xor	TMP,TMP,ITMP2
	mtc1	TMP,OOA

	# NOTE - dxAB, dyAB, dxBC, and dyBC have been adjusted by the
	# sorting algorithm above such that they are correct for the
	# sorted order vertices.
	# So the following are needed (dont trash them)
	# DXAB, DYAB, DXBC, DYBC, OOA
	# VTXA_PTR, VTXB_PTR, VTXC_PTR
	# We also need a couple of temporaries (thats all of the fps)
	# DA, DB, DC, FTMP1, FTMP2

	# Now we need to multiply each of dxAB, dyAB, dxBC, and dyBC by
	# (1.0 / area)

_vsort_done_noneg:
	li	$28, 0x88200000			# Point to SST1 in cached memory
	mul.s	DXAB,DXAB,OOA			# dxAB * (1.0/area)

	# OK - since we know we are always depth buffering we can go
	# ahead and calculate the deltas for depth buffering
	# We read the data needed for these delta calcs to take
	# advantage of the cycles from the multiply above.
	lwc1	DA,OOW_OFFSET(VTXA_PTR)		# get vtxa oow value
	lwc1	DB,OOW_OFFSET(VTXB_PTR)		# get vtxb oow value
	lwc1	DC,OOW_OFFSET(VTXC_PTR)		# get vtxc oow value

	mul.s	DYAB,DYAB,OOA			# dyAB * (1.0/area)

	sub.s	DC,DB,DC			# Calc db - dc
	swc1	DA,START_W_OFFSET($28)		# write the startw to hdw
	
	# While we are sitting here waiting for the above multiply
	# lets get the start R value
#if (LIGHTING_SUPPORT==1)
	lw	ITMP1,RED_OFFSET(VTXA_PTR)	# get vtxa red value
#endif

	mul.s	DXBC,DXBC,OOA			# dxBC * (1.0/area)
	sub.s	DA,DA,DB			# Calc da - db

	# Take advantage of extra multiply cycles to write the start red
	# value and read the start green values.
#if (LIGHTING_SUPPORT==1)
	lw	ITMP2,GREEN_OFFSET(VTXA_PTR)	# get vtxa green value
	sw	ITMP1,START_R_OFFSET($28)	# write startr to hdw
#endif

	mul.s	DYBC,DYBC,OOA			# dyBC * (1.0/area)

	# Take advantage of a few multiply cycle to write the start g
	# get the start b and write the start b values.
#if (LIGHTING_SUPPORT==1)
	sw	ITMP2,START_G_OFFSET($28)	# write startg to hdw
	lw	ITMP3,BLUE_OFFSET(VTXA_PTR)	# get vtxa blue value
#endif

	# OOA is NOT needed past this point

	# Now we calculate dwdx and dwdy
	mul.s	DPDX,DA,DYBC			# (da - db) * dyBC
#if (LIGHTING_SUPPORT==1)
	sw	ITMP3,START_B_OFFSET($28)	# write startb to hdw
#endif

	# Take advantage of a few multiply cycles to the get and write
	# the start t and start s values.
	lw	ITMP4,TMU0_T_OFFSET(VTXA_PTR)	# get vtxa tmu0 tow value
	lw	ITMP5,TMU0_S_OFFSET(VTXA_PTR)	# get vtxa tmu0 sow value
	sw	ITMP4,START_T_OFFSET($28)	# write start tow value

	mul.s	FTMP1,DC,DYAB			# (db - dc) * dyAB
	sub.s	DPDX,DPDX,FTMP1			# FBI dWdx

	# Take advantage of another cycle to write the s value
	sw	ITMP5,START_S_OFFSET($28)	# write start sow value

	mul.s	DPDY,DA,DXBC			# (da - db) * dxBC
	mul.s	FTMP2,DC,DXAB			# (db - dc) * dxAB

	sub.s	DPDY,FTMP2,DPDY			# FBI dWdy

	# Now we calculate the drdx, drdy, dgdx, dgdy, dbdx, and dbdy
	# values and send them off to the hardware.
	# NOTE - we already have vtxa r,g,b,tmu0 tow, and tum0 sow
	# values in ITMP1, ITMP2, ITMP3, ITMP4, and ITMP5 respectively.

	# TMU0 TOW
	mtc1	ITMP4,DA			# Get vtxa tmu0 tow
	lwc1	DB,TMU0_T_OFFSET(VTXB_PTR)	# Get vtxb tmu0 tow
	lwc1	DC,TMU0_T_OFFSET(VTXC_PTR)	# Get vtxc tmu0 tow

	sub.s	DA,DA,DB			# Calc towa - towb
	swc1	DPDX,DWDX_OFFSET($28)		# write FBI dWdx
	sub.s	DB,DB,DC			# Calc towb - towc

	mul.s	DPDX,DA,DYBC			# (towa - towb) * dyBC
	swc1	DPDY,DWDY_OFFSET($28)		# write FBI dWdy
	mul.s	FTMP1,DB,DYAB			# (towb - towc) * dyAB
	sub.s	DPDX,DPDX,FTMP1			# tmu0 dTdx

	mul.s	DPDY,DA,DXBC			# (towa - towb) * dxBC
	mul.s	FTMP2,DB,DXAB			# (towb - towc) * dxAB
	lwc1	DC,TMU0_S_OFFSET(VTXC_PTR)	# Get vtxc sow
	sub.s	DPDY,FTMP2,DPDY			# tmu0 dTdy

	# TMU0 SOW
	mtc1	ITMP5,DA			# Get vtxa sow
	lwc1	DB,TMU0_S_OFFSET(VTXB_PTR)	# Get vtxb sow
	swc1	DPDX,DTDX_OFFSET($28)		# write tmu0 dTdx

	sub.s	DA,DA,DB			# Calc sowa - sowb
	sub.s	DB,DB,DC			# Calc sowb - sowc

	swc1	DPDY,DTDY_OFFSET($28)		# write tmu0 dTdy
	mul.s	DPDX,DA,DYBC			# (sowa - sowb) * dyBC
	mul.s	FTMP1,DB,DYAB			# (sowb - sowc) * dyAB
	sub.s	DPDX,DPDX,FTMP1			# tmu0 dSdx

	mul.s	DPDY,DA,DXBC			# (sowa - sowb) * dxBC
	mul.s	FTMP2,DB,DXAB			# (sowb - sowc) * dxAB
	sub.s	DPDY,FTMP2,DPDY			# tmu0 dSdy

#if (LIGHTING_SUPPORT == 0)
	swc1	DPDX,DSDX_OFFSET($28)		# write tmu0 dSdx
	swc1	DPDY,DSDY_OFFSET($28)		# write tmu0 dSdy
#else
	mul.s	DPDY,DA,DXBC			# (sowa - sowb) * dxBC
	mul.s	FTMP2,DB,DXAB			# (sowb - sowc) * dxAB
	lwc1	DC,RED_OFFSET(VTXC_PTR)		# Get vtxc red
	sub.s	DPDY,FTMP2,DPDY			# tmu0 dSdy

	# Red
	mtc1	ITMP1,DA			# Get vtxa red
	lwc1	DB,RED_OFFSET(VTXB_PTR)		# Get vtxb red
	swc1	DPDX,DSDX_OFFSET($28)		# write tmu0 dSdx

	sub.s	DA,DA,DB			# Calc ra - rb
	sub.s	DB,DB,DC			# Calc rb - rc

	swc1	DPDY,DSDY_OFFSET($28)		# write tmu0 dSdy
	mul.s	DPDX,DA,DYBC			# (ra - rb) * dyBC
	mul.s	FTMP1,DB,DYAB			# (rb - rc) * dyAB
	sub.s	DPDX,DPDX,FTMP1			# dRdx

	mul.s	DPDY,DA,DXBC			# (ra - rb) * dxBC
	mul.s	FTMP2,DB,DXAB			# (rb - rc) * dxAB
	sub.s	DPDY,FTMP2,DPDY			# dRdy

	# Green
	mtc1	ITMP2,DA			# Get vtxa green
	lwc1	DB,GREEN_OFFSET(VTXB_PTR)	# Get vtxb green
	lwc1	DC,GREEN_OFFSET(VTXC_PTR)	# Get vtxc green

	sub.s	DA,DA,DB			# Calc ga - gb
	swc1	DPDX,DRDX_OFFSET($28)		# write dRdx
	sub.s	DB,DB,DC			# Calc gb - gc

	mul.s	DPDX,DA,DYBC			# (ga - gb) * dyBC
	swc1	DPDY,DRDY_OFFSET($28)		# write dRdy
	mul.s	FTMP1,DB,DYAB			# (gb - gc) * dyAB
	sub.s	DPDX,DPDX,FTMP1			# dGdx

	mul.s	DPDY,DA,DXBC			# (ga - gb) * dxBC
	mul.s	FTMP2,DB,DXAB			# (gb - gc) * dxAB
	sub.s	DPDY,FTMP2,DPDY			# dGdy

	# Blue
	mtc1	ITMP3,DA			# Get vtxa blue
	lwc1	DB,BLUE_OFFSET(VTXB_PTR)	# Get vtxb blue
	lwc1	DC,BLUE_OFFSET(VTXC_PTR)	# Get vtxc blue

	sub.s	DA,DA,DB			# Calc ba - bb
	swc1	DPDX,DGDX_OFFSET($28)		# write dGdx
	sub.s	DB,DB,DC			# Calc bb - bc
	mul.s	DPDX,DA,DYBC			# (ba - bb) * dyBC
	swc1	DPDY,DGDY_OFFSET($28)		# write dGdy
	mul.s	FTMP1,DB,DYAB			# (bb - bc) * dyAB
	sub.s	DPDX,DPDX,FTMP1			# dBdx

	mul.s	DPDY,DA,DXBC			# (ba - bb) * dxBC
	swc1	DPDX,DBDX_OFFSET($28)		# dBdx
	mul.s	FTMP2,DB,DXAB			# (bb - bc) * dxAB
	sub.s	DPDY,FTMP2,DPDY			# dBdy
	swc1	DPDY,DBDY_OFFSET($28)		# dBdy
#endif

#if (DMA_SUPPORT==0)
	/* Return to caller when all done */
	cache	(5<<2)|1, START_R_OFFSET($28)	# write the 3 cache lines to h/w
	cache	(5<<2)|1, DBDY_OFFSET($28)
	cache	(5<<2)|1, DSDX_OFFSET($28)
	li	$28, 0xa8200000			# ptr to SST1 in non-cached memory
	.set	noat
	sw	SAVED_AREA,TRIANGLECMD_OFFSET($28)	# Start rendering
	.set	at
	lw	ITMP1, 4($29)			# restore interrupts
	mtc0	ITMP1, $12
	jr	$31
	lw	$28, ($29)
#else
	j	_start_dma
	sw	SAVED_AREA,TRIANGLECMD_OFFSET($28)
#endif

triangle_culled:
	lw	ITMP1, 4($29)			# restore interrupts
	mtc0	ITMP1, $12
	jr	$31				# Rejected tris only
	lw	$28, ($29)
	.set	reorder
	.end	_grDrawTriangleNonAA

#if (DMA_SUPPORT==1)
	.ent	_start_dma
	.set	noreorder
	.frame	$29,0,$31
	.mask	1<<28, 0
_start_dma:
	# Get address of DMA Controller 0
	la	$4,0xac000000

	andi	$5,$28,0x1000
	srl	$5,10
	or	$4,$4,$5

	# Disable DMA
	li	$7,0xdc0
	sw	$7,0x840($4)

	# Send the next record address
	li	$6,0x00100000
	sll	$5,4
	or	$6,$6,$5
	sw	$6,0x830($4)

	# Start the DMA
	ori	$7,0x3000
	lw	$28, ($29)
	jr	$31
	sw	$7,0x840($4)
	
	.set	reorder
	.end	_start_dma
#endif
