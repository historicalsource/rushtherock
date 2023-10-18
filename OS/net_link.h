#ifndef net_link_defs
#define net_link_defs
/**********************************************************************/
/*File:           Net_Link.h                                          */
/*Author:         Mark C van der Pol                                  */
/*Organisation:   Technology Group, Atari Games Corporation.          */
/*Contents:       Function Prototypes for the Network Link driver     */
/*                                                                    */
/*(C) Copyright Atari Games Corporation 1996                          */
/*                                                                    */
/**********************************************************************/

#ifndef __U32_TYPE_DEFINED
#include <config.h>
#endif

typedef U32 network_descriptor;
typedef U32 packet_number;
typedef U8 netadd[6];

/*****************************************************************************/
/*  The buffer structure.                                                    */
/*  Applications can request any number of buffers, and link them into       */
/*  any length packets. The driver will follow all links until the last      */
/*  one, marked with a NULL pointer.                                         */
/*  Note that the size indicates the 'size' of the buffer, whilest 'len'     */
/*  is the amount of data in this buffer, and could be 0 (silly) upto len.   */
/*****************************************************************************/
typedef struct NET_BUFFER {
   struct NET_BUFFER *next; /* Pointer to the next buffer in the list.       */
   struct NET_BUFFER *self; /* Pointer to self, for error trapping           */
   U16    size;             /* Size of this buffer -normally NET_BUFFER_SIZE */
   U16    len;              /* Length of used space in this buffer.          */
   U16    offset;           /* Start of space to use. Len is counted from    */
                            /* this offset, so buffer space is reduced if    */
                            /* offset is signifcant. That is, space avail    */
                            /* equals NET_BUFFER_SIZE - offset               */
   U16    flags;            /* Flags used by the allocate funcs.             */

   U8     data[256];        /* The actual data.                              */
} Net_Buffer;

/**********************************************************************/
/* The following defines the capabilities of applications' callback   */
/* functions. Only if a capability has been set will the callback be  */
/* generated for that function.                                       */
/*                                                                    */
/**********************************************************************/
typedef U32 Net_CB_Caps;
#define CB_Will_Filter (1)
#define CB_Will_Receive (CB_Will_Filter << 1)
#define CB_Will_TXResult (CB_Will_Receive <<1)
#define CB_Will_Relinquish (CB_Will_Receive <<1)
/*	The call back function can do something with
	busy wait cycles, if given the oportunity.     ---------------------*/
#define CB_Non_Block (CB_Will_Relinquish <<1)

#define NET_NON_BLOCK CB_Non_Block
/*	Overrides the per-call ability to block. The
	driver will never block, and use busy waits
	where unavoidable. ( very few cases )		---------------------*/
#define NET_SEND_RAW (NET_NON_BLOCK << 1)
/*	Turns of driver pre-pended packet formating
	In most cases, will result in an illegal
	packet unless the caller is very intimate 
	with Ethernet packet protocols...             	---------------------*/

/******************************************************************************/
/*    The following enumeration identifies what type of callback is being     */
/*    made, and what sort of parameters can be expected in the function       */
/*    call.                                                                   */
/******************************************************************************/

typedef enum NET_CB_FUNC {
   CB_Filter,   /* Filter the incoming packet.                                */
                /*   Extra Function Parameters:                               */
                /*   int signature_len,        Length of signature (bytes)    */
                /*   int *signature_buf        Buffer containing the sig.     */
                /*   buffer **copy_to          filled in by called function   */
                /*                             with pointer to buffer to fill */
                /*                             in with packet, if it is wanted*/
   CB_Receive,  /* Receive a packet ( presumably, a previous filter call      */
                /* indicated a desire to do so.)                              */
                /*   Extra Function Parameters:                               */
                /*   int len,                  Number of bytes copied into    */
                /*                             buffer.                        */
                /*   buffer *packet,           Pointer to the buffer used,    */
                /*                             presumably the same one as     */
                /*                             was indicated in 'copy_to'     */
   CB_TXResult, /* A previous Transmit has some results to report.           */
                /*   Extra function Parameters:                              */
                /*   Net_Result result,         Enumeration of result code.  */
                /*   buffer *i_am_free          If not NULL, buffer being    */
                /*                              released back to the app.    */
   CB_Relinquish,   /*  This CB is called in busy wait loops to allow        */
                    /*  game or application specific rescheduling to occur.  */
                    /*  If none is defined/desired or possible, then         */
                    /*  return, or don't declare it as a cabability.         */
                    /*  Otherwise, do whatever background activity is        */
                    /*  feasable, then return. For example, if Nucleus       */
                    /*  present, a call to NU_Reschedule is the ticket.      */
                    /*  Return CB_OK if all went well, or CB_FAILED if       */
                    /*  something prevented proper relinquishing.            */
   CB_BfrRlse,      /*  Not yet implemented, won't be called               */
   CB_BfrAquire     /*  Not yet implemented, won't be called               */
} Net_CB_Func;

typedef enum NET_CB_RESULT {
   CB_OK = 1,         /*  A callback which succeeded without needing more    */
                      /*  attention, or actions.                             */
   CB_FAILED,         /*  Some error was detected in the callback routine.   */

                      /* The following are specific to filter callbacks.     */
   CB_Drop_Packet,    /*  The packet is of no interest to this application   */
   CB_Copy_Packet,    /*  The filter function has detirmined it wants the    */
                      /*  entire packet. The buffer pointer 'copy_to' has    */
                      /*  been initialised to point to a buffer of suf-      */
                      /*  ficient size to hold the packet.                   */
   CB_Copy_Part,      /*  The filter function has detirmined it wants a      */
                      /*  portion of the packet. The buffer pointer          */
                      /*  'copy_to' has been initialised to point to a       */
                      /*  buffer of sufficient size to hold the part.        */
                      /*  The first U16  in the buffer contains an offset    */
                      /*  to the start of the part that is required, and     */
                      /*  the second U16  the length of the part required.   */
   CB_Extract_Part,   /*  Not yet implemented                                */
   CB_Take_Ownership  /*  Not yet implemented                                */
} Net_CB_Result;

/********************************************************************************/
/*  Net_Control command constants						*/

typedef enum NET_CONTROL_CMD {
					/*	Params		Range/Meaning */
    NETCTL_CB_CAPABILITY = 1,		/*	Net_CB_Caps	Capability to change
    ====================			U32	   0 = Capability off
							  ~0 = Capability supported
	This call permits the changing of the 
	call-back capabilites that are defined
	when the descriptor is opened. The same
	values and ranges as in the Net_Open_Interface
	call are supported, but only one capability
	can be changed per call.                        ---------------------*/

    NETCTL_MC_CHANNEL,			/*	U32	  Channel number 0-63
    =================				U32	  0 = turn of listening to MC
							 ~0 = turn on receiving on MC
	Turns on/off one of the Multi-Cast channels
	on the chip. All MC's received on all ON 
	channels will be filtered to ALL open 
	descriptors. (for now).
	There is no easy way to discover which 
	channels are already on, nor an easy way
	to turn all off.				---------------------*/

    NETCTL_MC_PROMISCOUS,		/*	U32	 0 = MC Promiscous off
    ====================				~0 = MC Promiscous mode
	A seperate mode from the MC Channels is
	Multi-Cast Promiscous : this receives
	all MC's (wheter a channel for it is
	on or not) and filters it to ALL open
	descriptors. (for now.) This mode 
	operates independently from the 
	MC_CHANNEL - any ON channels stay in
	effect, and will 'reappear' when 
	MC_PROMISCOUS is turned off.			--------------------*/

    NETCTL_PROMISCOUS,			/*	U32	 0 = Promiscous off
    ====================				~0 = Promiscous mode
	This mode receives ALL packets, irrespective
	of the address in the packet.
	Each packet will go to all open interfaces.	--------------------*/

    NETCTL_LOOPBACK,			/*	U32	 NET_LOGICAL_LOOPBACK
    ====================				 NET_PHYSICAL_LOOPBACK
	Control over the chip's path to the wire.	 NET_FULLDUPLEX
	The LOOPBACK modes do not transmit packets 	 NET_HALFDUPLEX
	to the wire. The DUPLEX modes transmit to
	the wire, and either see our own packets
	(FULL) or ignore our own packets (HALF).
	Anything not one of the defines turns off
	any loopback mode, and reverts to the 
	previous DUPLEX mode, whatever it was.		--------------------*/
#	define	NET_LOGICAL_LOOPBACK (1)
#	define	NET_PHYSICAL_LOOPBACK (2)
#	define	NET_FULLDUPLEX (3)
#	define	NET_HALFDUPLEX (4)

    NETCTL_NULL                	       /* NONE
	END of LIST marker.				===================*/
} Net_Control_Cmd;

/* Function prototype for callback routines.
   The execution context of these could be interrupt time, so they should be
   snappy.
   Also, if any services are required beyond GUTS, make sure that the correct
   handler is passed to Net_Initialize. For Nucleus, this should be
   NU_Activate_HISR with a registered HISR structure pointer as the parameter.
   See the template App_Initialize for an implementation.
*/
typedef Net_CB_Result net_callback_fn(
               Net_CB_Func func,      /*  Callback type indicator          */
               network_descriptor nd, /*  The nd originating this callback */
               packet_number pkt_no,  /*  Driver Internal Packet number    */
               U32 len,               /*  Total length of packet (bytes)   */
               U32 param1,            /*  Param 1                          */
               U32 param2,            /*  Param 2                          */
               U32 param3             /*  Param 3                          */
);


/* ////////////////////////////////////////////////////////////////////////// */
/* //////////////   F U N C T I O N    P R O T O T Y P E S    //////////////  */

U32 Net_Initialize(
        U8 *nb_pool,               /* Pointer to memory that net_buffers    */
                                   /* get allocated from                    */
        U32 nb_size,               /* the number of bytes in said memory    */
        void (*handler)(),         /* The handler that can guarantee a safe */
                                   /* context for the call-backs that get   */
                                   /* registered when net interfaces are    */
                                   /* opened. If 0, a default context is    */
                                   /* used which is GUTS only.              */
        void *func                 /* Reference to function to be executed  */
);

void Net_Interrupt_Handler(void);

network_descriptor Net_Open_Interface(
        char *device,             /* Upto 8 chars - name of device to open   */
        net_callback_fn *cb,      /* address of the callback function        */
        Net_CB_Caps caps,         /* Or'd number indicating the functions    */
                                  /* the cb supports.                        */
        int sig_len,              /* the number of bytes needed in the sig.  */
        int sig_offset            /* the offset from the start of the packet */
                                  /* to the signature bytes.                 */
);

U32 Net_Send_Packet(
        network_descriptor nd,
        const netadd dest,
        Net_Buffer *pkt,
        int opts
);

U32 Net_Close_Interface(
        network_descriptor nd_id);

U32 Net_Get_Buffer(
        Net_Buffer **return_pointer, /* This location will be updated with a */
                                     /* pointer to the assigned buffer, if   */
                                     /* one is available.                    */
        int size,                    /* the total size required. As many     */
                                     /* buffers as are needed are allocated  */
                                     /* & chained, with the last buffer link */
                                     /* set to NULL, and it may not be       */
                                     /* entirley used.                       */
        int opts                     /* Blocking or Non-blocking             */
);

U32 Net_Free_Buffer(
        Net_Buffer *buffer_pointer, /* This is the buffer to be freed.       */
                                    /* An error will be posted if it was     */
                                    /* already freed, or not a 'real' buffer.*/
                                    /* All buffers in the chain are free'd   */
        int opts                    /* Blocking or Non-blocking              */
);

U32 Net_Control( 
	network_descriptor nd,	    /* Network Descriptor of already opened  */
				    /* interface which needs an out-of-band  */
				    /* command or state change.		     */
	Net_Control_Cmd command,    /* Command to be performed.		     */	
	...			    /* variable numbers of parameters to     */
				    /* satisfy command.			     */	
);

void Net_Aquire_MC(const  network_descriptor nd, const U32 node_id, netadd *aquired_add);
U32 Net_Release_MC(const network_descriptor nd,const U32 node_id , const netadd mc_add);

U32 Net_Relinquish(const network_descriptor nd_id);

/* net_dbug.c */
void Net_Print_Buff(const Net_Buffer *buff, int verbose);
void Net_Print_Buff_Stats( const void *bp);

/* //////////////   f u n c t i o n    p r o t o t y p e s    /////////////// */
/* ////////////////////////////////////////////////////////////////////////// */


/*************************************************************************/
/*  Various Contstants that can be used as options to various functions. */
/*  Some are specific to a single function, and other to a family of     */
/*  functions.                                                           */
/*************************************************************************/

#define NETOPT_SEND_IMMEDIATE   (1)
#define NETOPT_NONBLOCK         (2)
#define NETOPT_BLOCKING         (4)
#define NETOPT_PRIORITY         (8)		/* USED WHEN SENDING A PACKET */


/**************************************************************************/
/*  Result indicators.                                                    */
/**************************************************************************/

#define NETOK                           (0x001)
#define NETOK_CLOSE_COMPLETE            (0x002)
#define NETOK_SEND_COMPLETE             (0x003)
#define NETOK_SEND_PENDING              (0x004)

#define NETERR_BADPARAMETER             (0x100)
#define NETERR_SEND_NORESOURCE          (0x200)
#define NETERR_SEND_BADPARAMETER        (0x300)
#define NETERR_SEND_TIMEOUT             (0x400)
#define NETERR_CLOSE_BADPARAMETER       (0x500)
#define NETERR_NOMEMORY                 (0x600)
#define NETERR_FREE_ERROR               (0x700)
#define NETERR_BAD_BUFFER               (0x800)
#define NETERR_SEND_BAD_HEADER          (0x900)
#define NETERR_MMU_NORESOURCE           (0xA00)
#define NETERR_SMC_BLOCKED              (0xB00)
#define NETERR_DMA_ERROR                (0xC00)

/**************************************************************************/
/*  Result Masks                                                          */
/**************************************************************************/

#define NETERR_MASK     (0xff00)
#define NETOK_MASK      (0x00ff)



#endif
