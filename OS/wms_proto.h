#if !defined(_WMS_PROTO_H_)
#define _WMS_PROTO_H_

/* $Id: wms_proto.h,v 1.5 1997/07/01 20:40:44 shepperd Exp $ */

extern char *UARTputs(int, const char *);
extern void UARTInit(int, int);
extern int UARTputchar(int, int);
#define ASIC_UART_INIT  (0x0633)
extern void UnLock(void);

extern void gt64010_init(void);
extern int get_device_number(int);
extern int get_sst_device_number(void);
extern int get_pci_config_reg(int dev_num, int reg_num);
extern void put_pci_config_reg(int dev_num, int reg_num, int data);
extern void gr_test(void);

/*
 * Routines found in picfuncs.c
 */
extern int SetPICDateTime (U8 *);	/* Sets RTC from 7 bytes of data    */
extern int GetPICDateTime (U8 *);	/* Read Date/Time from RTC          */
extern int ReadPICNVRAM (U8, U8 *);	/* Read location, display value for
					   RTC NVRAM (0 <= register <= 63)  */
extern int WritePICNVRAM (U8, U8);	/* Write a location with a value    */
extern char **GetMFG( void );		/* Get Manufacturing Information    */

/*
 * Indices of string pointers returned by GetMFG.
 */
#define MFG_SERIAL 0
#define MFG_TYPE   1
#define MFG_DATE   2

#endif
