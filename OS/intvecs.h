/*		intvecs.h
 *
 *		Copyright 1996 Atari Games Corporation
 *	Unauthorized reproduction, adaptation, distribution, performance or 
 *	display of this computer program or the associated audiovisual work
 *	is strictly prohibited.
 *
 * ++++++ WARNING WARNING WARNING WARNING WARNING +++++
 * This file is machine generated. Any changes you put in here will be lost
 * the next time phx_vecs.mac is touched. You need to make your changes to
 * phx_vecs.mac instead of this file.
 */
#if !defined(_INTVECS_H_)
#define _INTVECS_H_

struct irq_tab {
	void 	(**softvec)();
	void    (*(*installer)(const struct irq_tab *, void(*new)()))();
	int	notes;
};
extern  void (*gameint)();	/*  post-wierdness Vblank  */
extern  void (*ms4vec)();	/*  4 Millisecond timer  */
extern  void (*prc_delay_vec)();	/*  prc_delay soft vector  */
extern  void (*set_ipl_vec)();	/*  prc_set_ipl soft vector  */
extern  void (*savintp)();	/*  context save soft vector  */
extern  void (*restintp)();	/*  context restore soft vector  */
extern  void (*timervec)();	/*  Soft Timer interrupt  */
extern	void (* volatile tpllvec)();	/* Adjust 1MS timer */
extern  void (*framevec)();	/*  Frame complete  */
extern  void (*ethintp)();	/*  ethernet interrupt  */
extern  void (*cagevec)();	/*  CAGE interrupt  */
extern  void (*dbgintp)();	/*  Debug switch interrupt  */
extern  void (*vsyintp)();	/*  VSYNC interrupt  */
extern  void (*dm0intp)();	/*  Galileo DMA0Comp  */
extern  void (*dm1intp)();	/*  Galileo DMA1Comp  */
extern  void (*dm2intp)();	/*  Galileo DMA2Comp  */
extern  void (*dm3intp)();	/*  Galileo DMA3Comp  */
extern  void (*tm3intp)();	/*  Galileo T3EXP timer interrupt  */
extern  void (*uartintp)();	/*  I/O ASIC UART interrupts  */

enum vecs {
    GAMEVB_INTVEC,
    MS4_INTVEC,
    PRC_DELAY_INTVEC,
    PRC_SET_IPL_INTVEC,
    SAVE_INTVEC,
    RESTORE_INTVEC,
    SW0_INTVEC,
    SW1_INTVEC,
    INT0_INTVEC,
    INT1_INTVEC,
    INT3_INTVEC,
    INT4_INTVEC,
    INT5_INTVEC,
    TLBMOD_INTVEC,
    TLBLOAD_INTVEC,
    TLBSTORE_INTVEC,
    ADDRERRL_INTVEC,
    ADDRERRS_INTVEC,
    BUSERRI_INTVEC,
    BUSERRD_INTVEC,
    SYSCALL_INTVEC,
    BREAK_INTVEC,
    RESERV_INTVEC,
    COPROC_INTVEC,
    OVRFLOW_INTVEC,
    TRAP_INTVEC,
    FLOAT_INTVEC,
    TIMER_INTVEC,
    TIMERPLL_INTVEC,
    GAMEFRAME_INTVEC,
    IDE_INTVEC,
    ETH_INTVEC,
    CAGE_INTVEC,
    DBG_INTVEC,
    VSY_INTVEC,
    DM0_INTVEC,
    DM1_INTVEC,
    DM2_INTVEC,
    DM3_INTVEC,
    TM3_INTVEC,
    UART_INTVEC,
    N_INTVECS
};

void (*prc_set_vec (unsigned int vecnum, void (*routine)() ))();
void prc_init_vecs(void);		/* set the default vectors */
#endif		/* _INTVECS_H_ */
