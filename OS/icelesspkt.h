#if !defined(_ICELESSPKT_H_)
#define _ICELESSPKT_H_
/***********************************************************************
 * The ICELESS Frame format is as follows:
 *
 * All packets have the following format:
 *
 * Byte  0:	(PKTlen) msb count of user bytes in record (0xFF=EOP mark)
 *       1:	(PKTlen) lsb count of user bytes in record
 *       2:	TO: identifer of target channel (pipe) to which packet is directed
 *	 3:     TO THREAD: identifier of target within application
 *       4:	FROM: identifer of target channel from which packet came
 *	 5:	FROM THREAD: identifer of target within application
 *	 6-n:	PKTlen bytes of optional data
 *
 * Channel 0 is special in that the ASCLEAP will assume the packet is meant
 * for it and will perform the task spelled out by the data in the packet.
 * The format of the command frame (the data in the "optional data" area of
 * the packet is as follows:
 *
 * Byte	0:	packet number (for reference purposes)
 *	1:	command byte
 *	2-n:	command data (filename typically)
 *
 * The following commands are available to channel 0:
 *
 *	ICEopenp - opens a pipe to an ICELESS/CD-SIM board. It returns an 8
 *		   bit "channel" which can be used in subsequent I/O
 *		   operations. The flags byte is a don't care and there needs
 *		   to be 3 bytes of "address": Ramrom Port, board address and
 *		   thread.
 *
 *	ICEopenn - opens a pipe to a network host:port. It returns an 8
 *		   bit "channel" which can be used in subsequent I/O
 *		   operations. If the flags byte is 0, the pipe will be
 *		   established as a TCP connection, otherwise it will be a
 *		   UDP (datagram) connection. The data following the flags
 *		   byte must be a null terminated ASCII string of the form
 *		   host:port where the host is either a host name or an IP
 *		   address in dot notation (eg. x.x.x.x) and the port is a
 *		   decimal TCP or UDP port number.
 *
 *	ICEclose - closes the channel specified by the sole byte in the 
 *		   in the command data.
 *
 *	ICEopenf - opens a Xinu file using Xinu's RFS. The command data consists
 *		   of a flags byte containing the mode (bit0=1 means READ, bit1=1
 *		   means WRITE) followed by a null terminated text string
 *		   containing the path and filename. Returns a channel number.
 *
 *	ICEreset - sets then clears the reset line on the ICELESS/CD-SIM board.
 *		   It reqires 2 bytes of command data: Ramrom Port (0/1) and board
 *		   address (0-3).
 *
 *      ICEholdreset - sets and holds the reset line on the ICELESS/CD-SIM board.
 *		   It reqires 2 bytes of command data: Ramrom Port (0/1) and board
 *		   address (0-3). The reset line can be "unset" with an ICEreset
 *		   command.
 *
 *	ICEattn  - sets then clears the attention flag on the target processor.
 *		   It reqires 2 bytes of command data: Ramrom Port (0/1) and board
 *		   address (0-3). This transaction is automatically performed after
 *		   the last byte of a complete packet has been loaded into the FIFO.
 *
 *	ICEsetsize - sets the size of the downlink FIFO's. There are two bytes
 *		   of data in this frame, msb first, indicating the FIFO size.
 *
 * Packets sent to channels that have not been "ICEopenp"'d will result in an error
 * being returned.
 *
 */

#if HOST_BOARD_CLASS && ((HOST_BOARD&HOST_BOARD_CLASS) == PHOENIX)
# define HB_PHOENIX 1
#else
# define HB_PHOENIX 0
#endif

#ifdef SUN
# define AIO_PARAMS(x) ()
#else
# define AIO_PARAMS(x) x
#endif
  
/* Commands from ICELESS to game and from game to ICELESS */

enum ice {
   ICEnop,		/* NOP */
   ICEexamine,		/* Examine memory or registers */
   ICEdeposit,		/* Deposit memory or registers */
   ICEcontinue,		/* Continue program execution */
   ICEstep,		/* Step one instruction */
   ICEgetuser,		/* Get data from user */
   ICEuser,		/* User data follows */
   ICEreset,		/* reset the processor and FIFO's */
   ICEattn,		/* signal attention to target */
   ICEopenp, 		/* opens a pipe to an ICELESS/CD-SIM board */
   ICEreply,		/* reply from a previous command */
   ICEclose, 		/* closes the channel specified by the byte in the data field */
   ICEopenf, 		/* opens a Xinu file using Xinu's RFS. */
   ICEsetsize, 		/* sets the size of the downlink FIFO's. */
   ICEsettimeout,	/* sets the timeout value */
   ICEholdreset,	/* drop and hold reset */
   
/* Response codes */

   ICEack,		/* Command acknowledge */
   ICEnak,		/* Command failure, reason code follows */

   NAKunsupport=0,	/* command is unsupported */
   NAKinsufarg,		/* insufficient arguments */
   NAKtimeout,		/* command timed out */
   NAKnoqueue,		/* ran out of write queues to target */
   NAKnochans,		/* ran out of channels */
   NAKillcmd,		/* illegal command */
   NAKillparam,		/* illegal parameter */
   NAKbusy,		/* channel busy */
   NAKtoobig,		/* input record too big */
   NAKnotopen,		/* channel not opened */
   NAKfatal,		/* internal fatal error */
               
/* All commands are followed by a byte that identifies which target (thread)
 * the command is directed to. The following values are allowed:
 */

#if 0
   THREADhost=0,   	/* host CPU */
   THREADgdb,		/* pre-assigned thread to GDB communications */
   THREADgpu,		/* GPU (specific to Jaguar systems) */
   THREADdsp,		/* DSP (specific to Jaguar systems) */
#endif

/* The examine and deposit commands have a byte that indentifies an
 * address space with which to operate. That byte can be one of the
 * following values:
 */
 
   ASmemory=0,		/* Memory local to the processor */
   ASregister,		/* Processor's registers */

   PKTmsl=0,		/* packet byte offsets. MSB of len */
   PKTlsl,		/* LSB of len */
   PKTto,		/* to target */
   PKTtothrd,		/* to target thread */
   PKTfrom,		/* from target */
   PKTfromthrd,		/* from target thread */
   PKTuser,		/* user data */
   PKTpktn=0,		/* packet number */
   PKTcmd,		/* command */
   PKTreason,		/* NAK reason */
   PKTport=PKTreason,	/* port number */
   PKTchan=PKTreason,	/* channel opened */
   PKTboard,		/* board number */
   PKTthread		/* thread */
};

# if !defined(XSTAT_SYNC)
#  define XSTAT_SYNC (0x01)	/* sync bit */
#  define XSTAT_XFF  (0x04)	/* XBUS FIFO full */
#  define XSTAT_XHF  (0x08)	/* XBUS FIFO half full */
#  define XSTAT_XEF  (0x10)	/* XBUS FIFO empty */
#  define XSTAT_SFF  (0x20)	/* SERIAL FIFO full */
#  define XSTAT_SHF  (0x40)	/* SERIAL FIFO half full */
#  define XSTAT_SEF  (0x80)	/* SERIAL FIFO empty */
# endif

#if !defined(ICELESS_ASCLEAP)
typedef enum {
   STATE_getmlen,
   STATE_getllen,
   STATE_getto,
   STATE_gettothrd,
   STATE_getfrom,
   STATE_getfromthrd,
   STATE_getdata,
   STATE_send,
   STATE_eatit,
   CMD_data=0,
   CMD_NAK,
   CMD_OK
} Estate;


typedef struct pktio {
   unsigned char *buf;	/* receive buffer into which to place incoming packets or to send */
   void *useri;		/* user defined parameter (used by iceless_io) */
   void (*func)AIO_PARAMS((struct pktio *)); /* pointer to function to call when packet arrives */
   void *user;		/* user defined parameter */
   struct pktio *next;	/* link to next one to use when this one is spent */
   int board;		/* Xbus board number (0-3) */
   int to;		/* channel upon which to expect data or to which to send data */
   int tothread;	/* thread upon which to expect data or to which to send data */
   int from;		/* channel from which this packet originated */
   int frmthread;	/* thread from which this packet originated */
   int size;		/* size of receive buffer or amount of data to send */
   int len;		/* amount of data in receive buffer or amount remaining to send */
   Estate state;	/* state reciever or sender is presently in */
   int polling;		/* flag indicating we are presently polling (interrupt semaphore) */
   int jj,kk;		/* local variables used by the packet reader/writer */   
} PktIOStruct;

# if !HB_PHOENIX
extern int 		(*pktQueRecv)AIO_PARAMS((PktIOStruct *pkt)); /* queue up a read request */
extern int 		(*pktQueSend)AIO_PARAMS((PktIOStruct *pkt, int flag));
extern void 	       *(*pktInit)();
extern PktIOStruct	*(*pktPoll)AIO_PARAMS((int board, int flag, int channel, int thread));
# else
extern int 		pktQueRecv AIO_PARAMS((PktIOStruct *pkt)); /* queue up a read request */
extern int 		pktQueSend AIO_PARAMS((PktIOStruct *pkt, int flag));
extern void 	        *pktInit();
extern PktIOStruct	*pktPoll AIO_PARAMS((int board, int flag, int channel, int thread));
#  if ICELESS_STUB
#    define pktQueRecv_p	pktQueRecv
#    define pktQueSend_p	pktQueSend
#    define pktInit_p 		pktInit
#    define pktPoll_p		pktPoll
#  endif
# endif
#endif

#define PKT_CHK_CHANNEL 1
#define PKT_CHK_THREAD  2

#define BOARDS 1		/* assume only 1 board */
#define GDB_THREAD (0xFF)	/* gdb traffic is on thread FF */
#define GDB_CHANNEL (0xFF)	/* gdb traffic is passed on channel FF */
#define TWI_THREAD (0x00)	/* twi's gdb traffic is on thread 0 */

#if defined HOST_BOARD
/*****************************************************************
 * The following is the interface to the ICELESS board
 */
 
# if HOST_BOARD == HCR4K
#   if ICELESS_BASE < XBUS_SLOT0 || ICELESS_BASE >= (XBUS_SLOT77 + (XBUS_SLOT77-XBUS_SLOT76))
#    define S_FILLER 7
#   endif
# endif

# if HOST_BOARD == MB4600
#  if !defined(F_FILLER)
#   define F_FILLER (7-4)
#  endif
#  if !defined(S_FILLER)
#   define S_FILLER 7
#  endif
# endif

# if !defined(S_FILLER)
#  define S_FILLER 3
# endif

# if !defined(F_FILLER)
#  define F_FILLER S_FILLER
# endif

typedef volatile struct {	/* Processor specific */
# if HB_PHOENIX
   unsigned long data;
   unsigned long sync;
   unsigned long stat;
# else
#  if BYTE0_OFFS == 0		/* Little endian */
   unsigned char data;		/* the data comes in on bits 7-0 */
   unsigned char fill0[S_FILLER];
   unsigned char sync;
   unsigned char fill1[S_FILLER];
   unsigned char stat;
   unsigned char fill2[S_FILLER];
#  else				/* Big endian */
   unsigned char fill0[F_FILLER];
   unsigned char data;		/* the data comes in on bits 7-0 */
   unsigned char fill1[S_FILLER];
   unsigned char sync;
   unsigned char fill2[S_FILLER];
   unsigned char stat;
#  endif
# endif
} IcelessBoard;
#endif

#endif
