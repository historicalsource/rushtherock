#ifndef net_data_defs
#define net_data_defs
/**********************************************************************/
/*File:           Net_Data.h                                          */
/*Author:         Mark C van der Pol                                  */
/*Organisation:   Technology Group, Time Warner Interactive.          */
/*Contents:       Data definitions for the Network driver.            */
/*                                                                    */
/*(C) Copyright Time Warner Interactive 1995                          */
/*                                                                    */
/**********************************************************************/

#ifndef __U32_TYPE_DEFINED
#include <config.h>
#endif
#include <net_link.h>

/*   typedef U32 network_descriptor;     [ defined in net_link.h ]           */
/*   Opaque type for network descriptors. One of these is assigned for each  */
/*   open of the driver. They actually have an internal strcuture, as        */
/*   follows:                                                                */
/*   upper 4 bits: identify the ~ Instance of the SMC driver/chip to use.    */
/*                 On a system with only one Chip/driver, will be ~0.        */
/*   bits 10-4:    monatomically increasing number bumped for each open on   */
/*                  a given driver/chip. (6 bits)                            */
/*   lower 4 bits:  index into the net-descriptor array for each device.     */
/*                  If more netdecriptors are required, more bits will be    */
/*                  needed.                                                  */
#define SMC_IDX_FROM_ND(nd) ((~(nd & 0xf0000000)>>28))
#define SMC_ND_IDX_FROM_ND(nd) ( nd & 0x000f )

/*   typedef U32 packet_number;          [ defined in net_link.h ]           */
/*   Opaque type for packet numbers. Each of these refers uniquely to a      */
/*   packet currently in the driver's domain.                                */
/*   It is based on the network descriptor, with two more fields added:      */
/*   bits 11-16:   SMC internal packet number (6 bits)                       */
/*   bits 17-28:   monatomically increaseing for each packet handled on this */
/*                 interface. (12 bits)                                      */

/*****************************************************************************/
/*Note:   So from a given packet number, you can detirmine which interface   */
/*handled it, and which application is responsible for it.                   */
/*****************************************************************************/
/* Macro to turn a driver pkt num into an SMC pkt num. */
#define SMC_DRV_PKT(packet_number) ((packet_number >> 11 ) &  0x01f)
/* macro to turn RX FIFO pkt num to driver pkt num */
#define SMC_RXPKT_DRV(packet_number) ((packet_number << 3) & 0x0001f800)
/* Macro to turn general SMC pkt num into driver pkt num */
#define SMC_PKT_DRV(packet_number) ((packet_number << 11) & 0x0001f800)
/* macro to get the ND from a packet Number */
#define SMC_ND_FROM_PKT_NUM(pkt_no) ((network_descriptor)pkt_no & 0xf00003ff)

/*  typedef U8 netadd[6];          [ defined in net_link.h ]           */
/*  A simple way to enforce network address types.  */

/*****************************************************************************/
/*  The buffer structure.                                                    */
/*  Applications can request any number of buffers, and link them into       */
/*  any length packets. The driver will follow all links until the last      */
/*  one, marked with a NULL pointer.                                         */
/*  Note that the size indicates the 'size' of the buffer, whilest 'len'     */
/*  is the amount of data in this buffer, and could be 0 (silly) upto len.   */
/*****************************************************************************/
/*  typedef struct NET_BUFFER {          [ defined in net_link.h ]          */
/*   struct NET_BUFFER *next; * Pointer to the next buffer in the list.       */
/*   U16    size;             * Size of this buffer -normally NET_BUFFER_SIZE */
/*   U16    len;              * Length of used space in this buffer.          */
/*   U16    offset;           * Start of space to use. Len is counted from    */
/*                            * this offset, so buffer space is reduced if    */
/*                            * offset is signifcant. That is, space avail    */
/*                            * equals NET_BUFFER_SIZE - offset               */
/*   U16    flags;            * Flags used by the allocate funcs.             */
/*   char   data[0];          * The actual data.                              */
/*} Net_Buffer;                                                              */

/****************************************************************************/
/*  The following are constants used in the flags field of a Net_Buffer.    */
/*  Primarily used internally by the driver.                                */
/****************************************************************************/
#define NB_HASH       (0x3f)         /* The hash value of the destination address */
#define NB_INUSE      (1<<7)         /* Buffer is allocated */
#define NB_FREE       (1<<8)         /* Buffer has been freed */
#define NB_TXPND      (1<<9)         /* In use by the send process */
#define NB_TXDONE     (1<<10)        /* Send completed, buffer back to app */
#define NB_RX         (1<<11)        /* Handling an incoming packet */
#define NB_MCAST      (1<<12)        /* If RX, packet is a Multicast */
#define NB_BCAST      (1<<13)        /* If RX, packet is broadcast */

typedef struct NET_BUFFER_STATS {
    U32 freed_buffers;
    U32 free_q_size;
    U32 free_q_high;
    U32 pool_emptieds;
    U32 got_buffers;
    U32 free_calls;
    U32 bad_selfs;
    U32 bad_frees;
    U32 nul_frees;
    U32 get_calls;
} Net_Buffer_Stats;

/* We receive a chunk of memory from the app to use as our
   buffer pool, which we initialise and maintain.
   This way, the app can 'tune' the ammount of memory to
   allocate to use for network buffers.
*/
typedef struct NET_BUFFER_POOL {
   Net_Buffer *free_buffer;
   Net_Buffer *last_buffer;
   U8  *pool_address;
   U32 pool_chunks;
   U32 pool_size;
   Net_Buffer_Stats stats;
} Net_Buffer_Pool;

/*************************************************************************/
/* The following structure defines the signature (portion of an incoming */
/* packet) which the filter is interested in. Currently, only one filter */
/* per open is supported. ( But many opens are possible...)              */
/*************************************************************************/
typedef struct Pkt_SIGS {
   U16 sig_len;
   U16 sig_offset;
} Pkt_Sigs;

/*  This constant is used to create a local signature buffer on the */
/*  driver's stack should the mode of the driver be such that sigs  */
/*  are not pre-fetched, or if no memory is available for the pre-  */
/*  fetch buffer. If a given network_descriptor requires a bigger   */
/*  signature than this, under some conditions packets will NOT be  */
/*  passed up. One such condition is lack of network buffers, and   */
/*  an other is bloody-mindedness of the programmer. Initial        */
/*  implementation is such a condition, too. ( it's a matter of     */
/*  time... ).                                                      */
/*  Note: Altough some problems could be resolved by increasing     */
/*  this constant, it is a highly inefficient way of using the      */
/*  driver, and is only brought out as some conditions caught by it */
/*  could occur legitimately (out of network buffers)               */
/*  Also note: Normally, signatures can be bigger, upto             */
/*  NET_BUFFER_SIZE, so DON'T PANIC                                 */
#define NET_FIXED_SIG_SIZE 32

struct Net_Stats {
   U32  sends;                  /* The count of calls to Net_Send           */
   U32  pendings;               /* Count of pending packets                 */
   U32  blocks;                 /* Blocked sends                            */
   U32  filter;                 /* The number of filter calls made          */
   U32  receive;                /* The number of receive upcalLs made       */
   U32  txresult;               /* The number of txresult calls made        */
   U32  bfrrlse;                /* The number of bfr rlse calls made        */
   U32  bfraquire;              /* The number of bfr aquire calls made      */
   U32  filt_pass;              /* Number of filter calls that passed       */
   U32  filt_drop;              /* Number of filter calls that dropped      */
   U32  no_resrc;               /* calls abandandond due to lack of resource*/
   U32  errors;                 /* number of error conditions detected...   */
};

#define SMC_ND_Stats_Inc(nd,var) (++smc_data[SMC_IDX_FROM_ND(nd)].descriptors[SMC_ND_IDX_FROM_ND(nd)].n_stats. ## var )

/******************************************************************************/
/* The following holds information for each open made on the driver, for all  */
/* supported net devices.                                                     */
/* This implies that for each open, you have one network descriptor,          */
/* one call back function, and one signature.                                 */
/* Of course, multiple opens are supported, so these limits are not crippling.*/
/******************************************************************************/

typedef struct NET_DESC {
        network_descriptor    nd_id;
        net_callback_fn *Net_Callback;
        Net_CB_Caps capabilities;
        Pkt_Sigs signature;           /* Signature for this nd          */
        struct Net_Stats n_stats;
	Net_Buffer *rx_buff,*tx_buff; /* Current buffers for receive and send */
} Net_Desc;

#endif
