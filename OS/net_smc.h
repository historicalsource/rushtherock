#ifndef net_smc_defs
#define net_smc_defs
/**********************************************************************/
/*File:           Net_smc.h                                           */
/*Author:         Mark C van der Pol                                  */
/*Organisation:   Technology Group, Time Warner Interactive.          */
/*Contents:       Constants specific to a given implementation or     */
/*                build that uses a network link.                     */
/*(C) Copyright Time Warner Interactive 1995                          */
/*                                                                    */
/**********************************************************************/


#define SMC_CHIP_TOTAL (1)

        /* Chunks that buffers are allocated in. This can change
           from system to system, and compile to compile, so code
           must *never* assume this value - always use the define..
           The value may be changed to tune memory use vs. speed, or
           any other reason.                                          */
#define NET_BUFFER_SIZE   (256)


#define MAX_ENET_PACKET_SIZE (1518)
	/* The maximum legal number of bytes in an ethernet packet
	 */

        /* This constant is the overhead at the front of each
           packet, which deals with stuff like source & dest.
           address,packet types, etc.
           Incoming packets have this number of bytes at the
           head BEFORE the user's PDU data.
           Outgoing packets have this number of bytes pre-
           pended and filled in before transmission.
           Note that the user's data is ensured of being
           U64 aligned.
        */
#define NET_PKT_HEADER_SIZE ( 2 * sizeof(netadd) + 2 + 2 )

        /* Stack size for the high level interrupt routine */
#define SMC_HISR_STACK_SIZE 0x6000

#define SMC_XBUS_INTERRUPT (0x40)

        /* Total number of network buffers available. This number
           can change to tune an implementation, so applications
           better use NET_BUFFER_SIZE whenever they use this sort of
           buffer                                                     */
#define NET_BUFFER_TOTAL  (100)

        /* Number of network descriptors that can be open at any time */
        /* on any one device.                                         */
#define NET_DESCRIPTORS_TOTAL (15)

        /* The number of pending transmit packets that are waiting  */
        /* for resources on the SMC                                 */
#define TX_QUEUE_SIZE  (18)

        /* Number of cycles through busy wait loops before alternative */
        /* is tried. DON'T MAKE SMALLER THAN 1 !!!                     */
#define SMC_BUSY_LOOP (10)



/*/////////////////////////////////////////////////////////////////////////// */

#ifdef PC_TEST
#define SMC_BASE_ADDRESS0 0x0320
        /* address that first SMC is at */
#define SMC_READ_REG(port_no,reg_no) (_inp16(port_no + SMC_REG(reg_no)))
#define SMC_WRITE_REG(port_no,reg_no,value) (_outp16(port_no+SMC_REG(reg_no),value))
#define SMC_WRITE_B_REG(port_no,reg_no,value) (_outp8(port_no+SMC_REG(reg_no),value))
#define SMC_READ_B_REG(port_no,reg_no) (_inp8(port_no + SMC_REG(reg_no)))
#define SMC_SET_BANK(port_no,reg_no) (_outp8(port_no+SMC_REG(SMC_BSR),SMC_BANK(reg_no)))



/*/////////////////////////////////////////////////////////////////////////// */

#elif HOST_BOARD == IDT4xEVAL && defined(HOST_BOARD)
extern void wbflush(void);
/* Code to read the SMC Register on RISQ Boards*/
#define SMC_BASE_ADDRESS0 (XBUS_SLOT76 + 0x100)
#define SMC_INTERRUPT0 (XBUS0_LVL)
#define SMC_READ_REG(port_no,reg_no) \
        ((U16)*((VU32 *)(port_no+(( reg_no & 0xe )<<1))))
#define SMC_WRITE_REG(port_no,reg_no,value) \
        { (*((U32 *)(port_no+((reg_no & 0xe )<<1))) = value) ;\
          wbflush();\
        }
#define SMC_WRITE_DATA(port_no,reg_no,value) \
          (*((U32 *)(port_no+((reg_no & 0xe )<<1))) = value) 
/* For byte write to the Stream I/O board SMC chip, the next bank of
   addresses is used - so add 0x20 to the base address. Note that
   for instance, 124 will get register 3, and 12a will get reg 5...
*/
#define SMC_WRITE_B_REG(port_no,reg_no,value) \
        { *((U32 *)(port_no+ 0x020 +((reg_no & 0xe)<<1))) =value&0xff ; wbflush();}
#define SMC_READ_B_REG(port_no,reg_no) \
        ((U8)*((VU32 *)(port_no+ 0x020 + (( reg_no & 0xe )<<1))))
#define SMC_SET_BANK(port_no,bank_no) \
        { (*((U32 *)(port_no+(SMC_REG(SMC_BSR)<<1))) = (bank_no & 0x03));\
          wbflush();\
        }
#define SMC_READ_BANK(port_no) \
        ((U16)(*((VU32 *)(port_no+(SMC_REG(SMC_BSR)<<1))) & 0x03 ))


/*/////////////////////////////////////////////////////////////////////////// */

#elif HOST_BOARD == PHOENIX && defined(HOST_BOARD)

#include <phx_proto.h>

U32 smc_last_access;	/* Contains the prc_get_count value of last access */

/* Code to read the SMC Register on PHOENIX*/
#define SMC_BASE_ADDRESS0 (XBUS_SLOT3 + 0x100)
#define SMC_INTERRUPT0 (XBUS0_LVL)

#define SMC_WRITE_REG_DELAY(port_no,reg_no,value) \
        { \
	  while(prc_get_count()-smc_last_access< (CPU_SPEED/2000000)) { ; }\
	  (*((VU32 *)(port_no+((reg_no & 0xe )<<1))) = value) ;\
	  smc_last_access = prc_get_count();\
	  FLUSH_WB();\
        }
#define SMC_WRITE_REG(port_no,reg_no,value) \
        { \
	  (*((VU32 *)(port_no+((reg_no & 0xe )<<1))) = value) ;\
	  FLUSH_WB();\
        }

#define SMC_WRITE_DATA_DELAY(port_no,reg_no,value) \
        { while(prc_get_count()-smc_last_access< (CPU_SPEED/2000000)) { ; }\
          (*((VU32 *)(port_no+((reg_no & 0xe )<<1))) = value) ;\
	  smc_last_access = prc_get_count();\
        }

#define SMC_WRITE_DATA(port_no,reg_no,value) \
        { \
          (*((VU32 *)(port_no+((reg_no & 0xe )<<1))) = value) ;\
        }

/* For byte write to the Stream I/O board SMC chip, the next bank of
   addresses is used - so add 0x20 to the base address. Note that
   for instance, 124 will get register 3, and 12a will get reg 5...
*/
#define SMC_WRITE_B_REG_DELAY(port_no,reg_no,value) \
        { while(prc_get_count()-smc_last_access< (CPU_SPEED/2000000)) { ; }\
	  *((VU32 *)(port_no+ 0x020 +((reg_no & 0xe)<<1))) =value&0xff ; \
	  smc_last_access = prc_get_count();\
	  FLUSH_WB();\
	}

#define SMC_WRITE_B_REG(port_no,reg_no,value) \
        { \
	  *((VU32 *)(port_no+ 0x020 +((reg_no & 0xe)<<1))) =value&0xff ; \
	  FLUSH_WB();\
	}

/* To use the following, change the definition to be a function decl. */
#define SMC_READ_REG_DELAY(port_no,reg_no)  \
	{  U16 res;\
	   while(prc_get_count()-smc_last_access< (CPU_SPEED/2000000)) { ; }\
           res = ((U16)*((VU32 *)(port_no+(( reg_no & 0xe )<<1))));\
	  smc_last_access = prc_get_count();\
	  return res;\
        }

#define SMC_READ_REG(port_no,reg_no)  \
           ((U16)*((VU32 *)(port_no+(( reg_no & 0xe )<<1))))

/* To use the following, change the definition to be a function decl. */
#define SMC_READ_B_REG_DELAY(port_no,reg_no) \
	{  U8 res;\
	   while(prc_get_count()-smc_last_access< (CPU_SPEED/2000000)) { ; }\
           res = ((U8)*((VU32 *)(port_no+ 0x020 + (( reg_no & 0xe )<<1))));\
	  smc_last_access = prc_get_count();\
	  return res;\
        }
        
#define SMC_READ_B_REG(port_no,reg_no) \
           ((U8)*((VU32 *)(port_no+ 0x020 + (( reg_no & 0xe )<<1))))
        
#define SMC_SET_BANK_DELAY(port_no,bank_no) \
        { while(prc_get_count()-smc_last_access< (CPU_SPEED/2000000)) { ; }\
	  *((VU32 *)(port_no+(SMC_REG(SMC_BSR)<<1))) = (bank_no & 0x03);\
	  smc_last_access = prc_get_count();\
          FLUSH_WB();\
        } 
        
#define SMC_SET_BANK(port_no,bank_no) \
        { \
	  *((VU32 *)(port_no+(SMC_REG(SMC_BSR)<<1))) = (bank_no & 0x03);\
          FLUSH_WB();\
        }

#define SMC_READ_BANK_DELAY(port_no) \
	  smc_last_access = prc_get_count(),\
          ((U16)(*((VU32 *)(port_no+(SMC_REG(SMC_BSR)<<1))) & 0x03 ))

#define SMC_READ_BANK(port_no) \
          ((U16)(*((VU32 *)(port_no+(SMC_REG(SMC_BSR)<<1))) & 0x03 ))

/*/////////////////////////////////////////////////////////////////////////// */


#elif HOST_BOARD == FLAGSTAFF && defined(HOST_BOARD)

#include <phx_proto.h>

/* Code to read the SMC Register on PHOENIX*/
#define SMC_BASE_ADDRESS0 (ENET_BASE)
#define SMC_INTERRUPT0 (NET_LVL)

#define SMC_WRITE_REG(port_no,reg_no,value) \
        { \
	  (*((VU32 *)(port_no+((reg_no & 0xe )<<1))) = value) ;\
	  FLUSH_WB();\
        }

#define SMC_WRITE_DATA(port_no,reg_no,value) \
        { \
          (*((VU32 *)(port_no+((reg_no & 0xe )<<1))) = value) ;\
        }

/* For byte write to the Stream I/O board SMC chip, the next bank of
   addresses is used - so add 0x20 to the base address. Note that
   for instance, 124 will get register 3, and 12a will get reg 5...
*/
#define SMC_WRITE_B_REG(port_no,reg_no,value) \
        { \
	  *((VU32 *)(port_no+ 0x020 +((reg_no & 0xe)<<1))) =value&0xff ; \
	  FLUSH_WB();\
	}

#define SMC_READ_REG(port_no,reg_no)  \
           ((U16)*((VU32 *)(port_no+(( reg_no & 0xe )<<1))))

#define SMC_READ_B_REG(port_no,reg_no) \
           ((U8)*((VU32 *)(port_no+ 0x020 + (( reg_no & 0xe )<<1))))
        
#define SMC_SET_BANK(port_no,bank_no) \
        { \
	  *((VU32 *)(port_no+(SMC_REG(SMC_BSR)<<1))) = (bank_no & 0x03);\
          FLUSH_WB();\
        }

#define SMC_READ_BANK(port_no) \
          ((U16)(*((VU32 *)(port_no+(SMC_REG(SMC_BSR)<<1))) & 0x03 ))

/*/////////////////////////////////////////////////////////////////////////// */

#elif HOST_BOARD == CHAMELEON && defined(HOST_BOARD)

#include <phx_proto.h>

/* Code to read the SMC Register on PHOENIX*/
#define SMC_BASE_ADDRESS0 (SMC_BASE)
#define SMC_INTERRUPT0 (NET_LVL)

#define SMC_WRITE_REG(port_no,reg_no,value) \
        { \
	  *(VU16 *)(port_no+(reg_no & 0xe)) = (value) ;\
        }

#define SMC_WRITE_DATA(port_no,reg_no,value) \
        { \
          *(VU16 *)(port_no+(reg_no & 0xe)) = (value) ;\
        }

#define SMC_WRITE_B_REG(port_no,reg_no,value) \
        { \
	  *(VU8 *)(port_no+reg_no) = (value) ; \
	}

#define SMC_READ_REG(port_no,reg_no)  \
           *(VU16 *)(port_no+(reg_no & 0xe))

#define SMC_READ_B_REG(port_no,reg_no) \
           *(VU8 *)(port_no + reg_no)
        
#define SMC_SET_BANK(port_no,bank_no) \
        { \
	  *(VU16 *)(port_no+SMC_REG(SMC_BSR)) = (bank_no & 0x03);\
        }

#define SMC_READ_BANK(port_no) \
          (*(VU16 *)(port_no+SMC_REG(SMC_BSR)) & 0x03 )

/*/////////////////////////////////////////////////////////////////////////// */

#elif HOST_BOARD == SEATTLE && defined(HOST_BOARD)

/* Code to read the SMC Register on SEATTLE*/
#define SMC_BASE_ADDRESS0 (WB_ETH)
#define SMC_INTERRUPT0 (NET_LVL)


#define SMC_WRITE_REG(port_no, reg_no, value) do { \
	*(VU32 *)WB_ADR = reg_no >> 1; \
	*(VU32 *)port_no = value; } while(0)

#define SMC_WRITE_DATA(port_no, reg_no, value) do { \
	*(VU32 *)WB_ADR = reg_no >> 1; \
	*(VU32 *)port_no = value; } while(0)

#define inline __inline__

static inline U16 smc_read_reg(int port_no, int reg_no)
{
 *(VU32 *)WB_ADR = reg_no >> 1;
 return( (U16) *(VU32 *)port_no );
}

#define SMC_READ_B_REG(port_no,reg_no) ((U8)smc_read_reg(port_no, reg_no))

#define SMC_READ_REG(port_no, reg_no) smc_read_reg(port_no, reg_no)

#define SMC_SET_BANK(port_no, bank_no) do { \
	*(VU32 *)WB_ADR = SMC_REG(SMC_BSR) >> 1; \
	*(VU32 *)port_no = bank_no & 3; } while(0)

#define SMC_READ_BANK(port_no) smc_read_reg(port_no, SMC_REG(SMC_BSR))


/*/////////////////////////////////////////////////////////////////////////// */

#elif HOST_BOARD == HCR4K && defined(HOST_BOARD)

/* Code to read the SMC Register on ZOID 20 RUSH boards*/
#define SMC_BASE_ADDRESS0 (XBUS_SLOT76 + 0x100)
#define SMC_INTERRUPT0 (XBUS0_LVL)
#define SMC_READ_REG(port_no,reg_no) \
        ((U16)*((VU32 *)(port_no+(( reg_no & 0xe )<<1))))
#define SMC_WRITE_REG(port_no,reg_no,value) \
        { int tmp; \
          (*((U32 *)(port_no+((reg_no & 0xe )<<1))) = value) ;\
          tmp = *(volatile int *)CTRL_BASE;\
        }
#define SMC_WRITE_DATA(port_no,reg_no,value) \
          (*((U32 *)(port_no+((reg_no & 0xe )<<1))) = value) 
/* For byte write to the Stream I/O board SMC chip, the next bank of
   addresses is used - so add 0x20 to the base address. Note that
   for instance, 124 will get register 3, and 12a will get reg 5...
*/
#define SMC_WRITE_B_REG(port_no,reg_no,value) \
        { int tmp; *((U32 *)(port_no+ 0x020 +((reg_no & 0xe)<<1))) =value&0xff ; tmp =*(volatile int *)CTRL_BASE;}
#define SMC_READ_B_REG(port_no,reg_no) \
        ((U8)*((VU32 *)(port_no+ 0x020 + (( reg_no & 0xe )<<1))))
#define SMC_SET_BANK(port_no,bank_no) \
        { int tmp; \
          *((U32 *)(port_no+(SMC_REG(SMC_BSR)<<1))) = (bank_no & 0x03);\
          tmp = *(volatile int *)CTRL_BASE;\
        }
#define SMC_READ_BANK(port_no) \
        ((U16)(*((VU32 *)(port_no+(SMC_REG(SMC_BSR)<<1))) & 0x03 ))
#else


/*/////////////////////////////////////////////////////////////////////////// */

#error Unknown host/target system.
#endif


/*/////////////////////////////////////////////////////////////////////////// */


#define SMC_BASE_ADDRESS SMC_BASE_ADDRESS0
        /* We currently support only a single chip. */

#ifdef PC_TEST
#define EnterCritSec()  { DosEnterCritSec()
#define ExitCritSec()   DosExitCritSec(); }
#elif defined CALL_NUCLEUS
#define EnterCritSec() \
{ \
   int old_ipl; \
   old_ipl = NU_Control_Interrupts(NU_DISABLE_INTERRUPTS);
#define ExitCritSec() \
   NU_Control_Interrupts(old_ipl); \
}

#else
#define EnterCritSec() \
{ \
   int old_ipl; \
   old_ipl = prc_set_ipl(INTS_OFF);
#define ExitCritSec() \
   prc_set_ipl(old_ipl); \
}
#endif /* PC_TEST */

#endif /* net_smc_defs */
