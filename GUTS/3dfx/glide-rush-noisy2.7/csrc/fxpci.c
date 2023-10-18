/*
** Copyright (c) 1995, 3Dfx Interactive, Inc.
** All Rights Reserved.
**
** This is UNPUBLISHED PROPRIETARY SOURCE CODE of 3Dfx Interactive, Inc.;
** the contents of this file may not be disclosed to third parties, copied or
** duplicated in any form, in whole or in part, without the prior written
** permission of 3Dfx Interactive, Inc.
**
** RESTRICTED RIGHTS LEGEND:
** Use, duplication or disclosure by the Government is subject to restrictions
** as set forth in subdivision (c)(1)(ii) of the Rights in Technical Data
** and Computer Software clause at DFARS 252.227-7013, and/or in similar or
** successor clauses in the FAR, DOD or NASA FAR Supplement. Unpublished  -
** rights reserved under the Copyright Laws of the United States.
**
**
** $Revision: 25 $ 
** $Date: 6/22/96 6:51a $ 
**
*/

#include <stdlib.h>
#include <stdio.h>
#include <3dfx.h>
#define FX_DLL_DEFINITION
#include <fxdll.h>
#include "fxpci.h"

#define CONFIG_ADDRESS_PORT 0xCF8
#define CONFIG_DATA_PORT    0xCFC
#define CONFIG_ADDRESS_ENABLE_BIT 0x80000000l

#define CONFIG_MAPPING_ENABLE_BYTE 0x80
#define CONFIG_MAPPING_DISABLE_BYTE 0x00

#define CONFIG_SPACE_ENABLE_PORT 0xCF8
#define FORWARD_REGISTER_PORT    0xCFA
#define CONFIG_MAPPING_OFFSET    0xC000

#define PCI_INTERRUPT  0x1A
#define PCI_FUNC_CODE  0xB1
#define PCI_PHYS_ADDR  0x000FFE6E

/* Configuration */

/* xxx these should be encapsulated as separate files so
       the ifdefs aren't required here */
/* xxx these are now defined in the makefile */

/* PORTIO_              selects how port I/O is accomplished
     DIRECT               use runtime library (DOS, WIN95)
     GPD                  use genport miniport driver (NT)
     WINRT                use WinRT */

/* MAPPL_               selects how physical address space
                          gets mapped into local (virtual) memory
     DPMI                 use DPMI map physical (DOS)
     MEMMAP_VXD           use memmap VXD (WIN95)
     MAPMEM_MINIPORT      use mapmem miniport driver (NT)
     WINRT                use WinRT */

/*#define PORTIO_DIRECT*/
/*#define PORTIO_GPD*/
/*#define PORTIO_WINRT*/

/*#define MAPPL_DPMI*/
/*#define MAPPL_PHARLAP*/
/*#define MAPPL_MEMMAP_VXD*/
/*#define MAPPL_MAPMEM_MINIPORT*/
/*#define MAPPL_WINRT*/

#if defined( PORTIO_WINRT ) || defined( MAPPL_WINRT )
#define OPEN_WINRT
#define CLOSE_WINRT

#include <windows.h>
#include <winioctl.h>
#include <winrtctl.h>

HANDLE hWinRT;

#endif

/* -------------------------------------------------- */
/* Direct port I/O                                    */

#if defined( PORTIO_DIRECT )
#   if defined( __WATCOMC__ )
#       include <conio.h>
        /* xxx how do you find the version # ? */
#       if 1
#           define pioInByte(port) inp(port)
#           define pioInWord(port) inpw(port)
#           define pioInLong(port) inpd(port)
#           define pioOutByte(port,data) outp(port, data)
#           define pioOutWord(port,data) outpw(port, data)
#           define pioOutLong(port,data) outpd(port, data)
#       else
#           error Unknown Watcom C version
#       endif /* WATCOM versions */
#   elif defined( __MSC__ )
#       include <conio.h>
#       if _MSC_VER == 900         /* MS VC 2.0 */
#           define pioInByte(port) inp(port)
#           define pioInWord(port) inpw(port)
#           define pioInLong(port) _inpd(port)
#           define pioOutByte(port,data) _outp(port, data)
#           define pioOutWord(port,data) outpw(port, data)
#           define pioOutLong(port,data) _outpd(port, data)
#       elif _MSC_VER == 1000      /* MS VC 4.0 */
            /* xxx check this (the underscores) out to make sure */
#           define pioInByte(port) _inp(port)
#           define pioInWord(port) _inpw(port)
#           define pioInLong(port) _inpd(port)
#           define pioOutByte(port,data) _outp(port, data)
#           define pioOutWord(port,data) _outpw(port, data)
#           define pioOutLong(port,data) _outpd(port, data)
#       elif _MSC_VER == 1010      /* MS VC 4.1 */
            /* xxx check this (the underscores) out to make sure */
#           define pioInByte(port) _inp(port)
#           define pioInWord(port) _inpw(port)
#           define pioInLong(port) _inpd(port)
#           define pioOutByte(port,data) _outp(port, data)
#           define pioOutWord(port,data) _outpw(port, data)
#           define pioOutLong(port,data) _outpd(port, data)
#       else
#           error Unknown Microsoft VC version
#       endif /* MSC versions */
#   elif defined( __DJGPP__ )
#       if ( __DJGPP__ == 2 ) && ( __DJGPP_MINOR__ == 0 )
            /* DJGPP 2.0 beta */
#           define pioInByte(port) inportb(port)
#           define pioInWord(port) inportw(port)
#           define pioInLong(port) inportl(port)
#           define pioOutByte(port,data) outportb(port, data)
#           define pioOutWord(port,data) outportw(port, data)
#           define pioOutLong(port,data) outportl(port, data)
#       else
#           error Unknown DJGPP version
#       endif /* DJGPP versions */
#   else
#       error Unknown compiler
#   endif /* compilers */

/* -------------------------------------------------- */
/* Port I/O through genport miniport driver           */

#elif defined( PORTIO_GPD )

/* xxx _really_ should be a separte file */

#include <windows.h>
#include <stddef.h>
#include <winioctl.h>
#include <gpioctl.h>

FxU8  pioInByte ( unsigned short port );                /* inp */
FxU16 pioInWord ( unsigned short port );                /* inpw */
FxU32 pioInLong ( unsigned short port );                /* inpd */

FxBool  pioOutByte ( unsigned short port, FxU8 data );  /* outp */
FxBool  pioOutWord ( unsigned short port, FxU16 data ); /* outpw */
FxBool  pioOutLong ( unsigned short port, FxU32 data ); /* outpd */

HANDLE hGpdFile;

FxU8 pioInByte ( unsigned short port )
{
  BOOL   IoctlResult;
  LONG   IoctlCode;
  ULONG  PortNumber;
  UCHAR  DataBuffer;
  ULONG  DataLength;
  ULONG  ReturnedLength;

  IoctlCode = IOCTL_GPD_READ_PORT_UCHAR;
  PortNumber = port;
  DataLength = sizeof(DataBuffer);

  IoctlResult = DeviceIoControl( hGpdFile, IoctlCode, &PortNumber, sizeof(PortNumber),
                                 &DataBuffer, DataLength, &ReturnedLength, NULL );

  if ( IoctlResult && ReturnedLength == DataLength )
    return DataBuffer;

  else
    return FXFALSE;
}

FxU16 pioInWord ( unsigned short port )
{
  BOOL   IoctlResult;
  LONG   IoctlCode;
  ULONG  PortNumber;
  USHORT DataBuffer;
  ULONG  DataLength;
  ULONG  ReturnedLength;

  IoctlCode = IOCTL_GPD_READ_PORT_USHORT;
  PortNumber = port;
  DataLength = sizeof(DataBuffer);

  IoctlResult = DeviceIoControl( hGpdFile, IoctlCode, &PortNumber, sizeof(PortNumber),
                                 &DataBuffer, DataLength, &ReturnedLength, NULL );

  if ( IoctlResult && ReturnedLength == DataLength )
    return DataBuffer;

  else
    return FXFALSE;
}

FxU32 pioInLong ( unsigned short port )
{
  BOOL   IoctlResult;
  LONG   IoctlCode;
  ULONG  PortNumber;
  ULONG  DataBuffer;
  ULONG  DataLength;
  ULONG  ReturnedLength;

  IoctlCode = IOCTL_GPD_READ_PORT_ULONG;
  PortNumber = port;
  DataLength = sizeof(DataBuffer);

  IoctlResult = DeviceIoControl( hGpdFile, IoctlCode, &PortNumber, sizeof(PortNumber),
                                 &DataBuffer, DataLength, &ReturnedLength, NULL );

  if ( IoctlResult && ReturnedLength == DataLength )
    return DataBuffer;

  else
    return FXFALSE;
}

FxBool pioOutByte ( unsigned short port, FxU8 data )
{
  BOOL                IoctlResult;
  LONG                IoctlCode;
  GENPORT_WRITE_INPUT InputBuffer;
  ULONG               DataLength;
  ULONG               ReturnedLength;

  IoctlCode = IOCTL_GPD_WRITE_PORT_UCHAR;
  InputBuffer.PortNumber = port;
  InputBuffer.CharData = (UCHAR) data;
  DataLength = offsetof(GENPORT_WRITE_INPUT, CharData) +
               sizeof(InputBuffer.CharData);

  IoctlResult = DeviceIoControl( hGpdFile, IoctlCode, &InputBuffer, DataLength,
                                 NULL, 0, &ReturnedLength, NULL );

  if ( IoctlResult )
    return FXTRUE;
  else
    return FXFALSE;
}

FxBool pioOutWord ( unsigned short port, FxU16 data )
{
  BOOL                IoctlResult;
  LONG                IoctlCode;
  GENPORT_WRITE_INPUT InputBuffer;
  ULONG               DataLength;
  ULONG               ReturnedLength;

  IoctlCode = IOCTL_GPD_WRITE_PORT_USHORT;
  InputBuffer.PortNumber = port;
  InputBuffer.ShortData = (USHORT) data;
  DataLength = offsetof(GENPORT_WRITE_INPUT, ShortData) +
               sizeof(InputBuffer.ShortData);

  IoctlResult = DeviceIoControl( hGpdFile, IoctlCode, &InputBuffer, DataLength,
                                 NULL, 0, &ReturnedLength, NULL );

  if ( IoctlResult )
    return FXTRUE;
  else
    return FXFALSE;
}

FxBool pioOutLong ( unsigned short port, FxU32 data )
{
  BOOL                IoctlResult;
  LONG                IoctlCode;
  GENPORT_WRITE_INPUT InputBuffer;
  ULONG               DataLength;
  ULONG               ReturnedLength;

  IoctlCode = IOCTL_GPD_WRITE_PORT_ULONG;
  InputBuffer.PortNumber = port;
  InputBuffer.LongData = (ULONG) data;
  DataLength = offsetof(GENPORT_WRITE_INPUT, LongData) +
               sizeof(InputBuffer.LongData);

  IoctlResult = DeviceIoControl( hGpdFile, IoctlCode, &InputBuffer, DataLength,
                                 NULL, 0, &ReturnedLength, NULL );

  if ( IoctlResult )
    return FXTRUE;
  else
    return FXFALSE;
}

/* -------------------------------------------------- */
/* Port I/O through WinRT                             */

#elif defined( PORTIO_WINRT )

unsigned int   pciOutp( int port, int value );
unsigned int   pciOutpw( int port, unsigned short value );
unsigned long  pciOutpd( int port, unsigned long value );
unsigned int   pciInp( int port );
unsigned short pciInpw( int port );
unsigned long  pciInpd( int port );

#define pioInByte(port) pciInp(port)
#define pioInWord(port) pciInpw(port)
#define pioInLong(port) pciInpd(port)
#define pioOutByte(port,data) pciOutp(port, data)
#define pioOutWord(port,data) pciOutpw(port, data)
#define pioOutLong(port,data) pciOutpd(port, data)

#else

/* -------------------------------------------------- */
/* No Port I/O selected                               */

#error Unknown Port I/O type

#endif /* Port I/O */

/* -------------------------------------------------- */
/* Memmap through DPMI                                */

#if defined( MAPPL_DPMI )

#include <fxdpmi.h>

/* -------------------------------------------------- */
/* Memmap through Phar Lap system calls               */

#elif defined( MAPPL_PHARLAP )

#include <pharlap.h>
#include <hw386.h>
#include "fxphrlap.h"

/* -------------------------------------------------- */
/* Memmap through Conrad's memmap vxd                 */

#elif defined( MAPPL_MEMMAP_VXD )

#include <windows.h>
#include <fxmemmap.h>

HANDLE hMemmapFile;

/* -------------------------------------------------- */
/* Memmap through Microsoft's sample mapmem miniport  */

#elif defined( MAPPL_MAPMEM_MINIPORT )

#include <windows.h>
#include <winioctl.h>
/*#include <ioaccess.h> */

/* xxx The following two typedefs are here to avoid including
   miniport.h and ntddk.h from the NT DDK */

typedef enum _INTERFACE_TYPE
{
    Internal,
    Isa,
    Eisa,
    MicroChannel,
    TurboChannel,
    PCIBus,
    MaximumInterfaceType
} INTERFACE_TYPE, *PINTERFACE_TYPE;

typedef LARGE_INTEGER PHYSICAL_ADDRESS;

#include <mapmem.h>

HANDLE hMapmemFile;

/* -------------------------------------------------- */
/* Memmap through WinRT                               */

#elif defined( MAPPL_WINRT )

#else

#error Unknown memory map type

#endif

/* PRIVATE DATA */
#define MAX_PCI_DEVICES 32

static const char          *pciErrorString      = "No errors.\n";
static const FxBool         deviceExists[MAX_PCI_DEVICES];
static const FxBool         libraryInitialized  = FXFALSE;
static const FxU32          configMechanism     = 0;
static const FxBool         busDetected         = FXFALSE;

/* PRIVATE FUNCTIONS */

FxU32 _pciCreateConfigAddress( FxU32 bus_number, FxU32 device_number, 
                               FxU32 function_number, FxU32 register_offset ) {
    FxU32 retval = CONFIG_ADDRESS_ENABLE_BIT;

    retval |= ( bus_number & 0xFF ) << 16;
    retval |= ( device_number & 0x1F ) << 11;
    retval |= ( function_number & 0x7 ) << 8;
    retval |= ( register_offset & 0xFC );
    return retval;
}

FxU16 _pciCreateConfigSpaceMapping( FxU32 device_number, FxU32 register_offset ) {

    FxU16 retval = 0;
    retval |= ( device_number & 0xFF ) << 8;
    retval |= ( register_offset & 0xFC );
    retval += CONFIG_MAPPING_OFFSET;
    return retval;
}

FxU32 _pciFetchRegister( FxU32 offset, FxU32 size_in_bytes, 
              FxU32 device_number, FxU32 config_mechanism  ) {
    FxU32 retval;
    if ( config_mechanism == 1 ) {
        pioOutLong( CONFIG_ADDRESS_PORT, _pciCreateConfigAddress( 0, device_number, 0, offset ) );
    retval = pioInLong( CONFIG_DATA_PORT );
    retval >>= 8 * ( offset & 0x3 );
    } else { /* config mechanism 2 */
    pioOutByte( CONFIG_ADDRESS_PORT, CONFIG_MAPPING_ENABLE_BYTE );
    retval = pioInLong( _pciCreateConfigSpaceMapping( device_number, offset ) ); 
    retval >>= 8 * ( offset & 0x3 );
    pioOutByte( CONFIG_ADDRESS_PORT, CONFIG_MAPPING_DISABLE_BYTE );
    }

    switch( size_in_bytes ) {
    case 1:
        retval &= 0xFF;
        break;
    case 2:
        retval &= 0xFFFF;
        break;
        default: /* 4 bytes */
        break;
    }

    return retval;
}

void _pciUpdateRegister( FxU32 offset, FxU32 data, FxU32 size_in_bytes, 
             FxU32 device_number, FxU32 config_mechanism  ) {

    FxU32 regval =  _pciFetchRegister( offset & ( ~0x3 ), 4, device_number, config_mechanism );
    FxU32 mask = (FxU32) ~0l;


    switch( size_in_bytes ) {
    case 1:
        mask &= 0xFF;
        data &= 0xFF;
        break;
    case 2:
        mask &= 0xFFFF;
        data &= 0xFFFF;
        break;
    case 4:
    default:
        break;
    }

    data <<= 8 * ( offset & 0x03 );
    mask <<= 8 * ( offset & 0x03 );

    regval = ( regval & ~mask ) | data;

    if ( config_mechanism == 1 ) {
        pioOutLong( CONFIG_ADDRESS_PORT, _pciCreateConfigAddress( 0, device_number, 0, offset ) );
        pioOutLong( CONFIG_DATA_PORT, regval );
    } else { /* config mechanism 2 */
    pioOutByte( CONFIG_ADDRESS_PORT, CONFIG_MAPPING_ENABLE_BYTE );
        pioOutLong( _pciCreateConfigSpaceMapping( device_number, offset ), regval );
    pioOutByte( CONFIG_ADDRESS_PORT, CONFIG_MAPPING_DISABLE_BYTE );
    }

    return;
}

/* PUBLIC DATA  */

const PciRegister PCI_VENDOR_ID       = { 0x0,  2, READ_ONLY };
const PciRegister PCI_DEVICE_ID       = { 0x2,  2, READ_ONLY };
const PciRegister PCI_COMMAND         = { 0x4,  2, READ_WRITE };
const PciRegister PCI_STATUS          = { 0x6,  2, READ_WRITE };
const PciRegister PCI_REVISION_ID     = { 0x8,  1, READ_ONLY };
const PciRegister PCI_CLASS_CODE      = { 0x9,  3, READ_ONLY };
const PciRegister PCI_CACHE_LINE_SIZE = { 0xC,  1, READ_WRITE };
const PciRegister PCI_LATENCY_TIMER   = { 0xD,  1, READ_WRITE };
const PciRegister PCI_HEADER_TYPE     = { 0xE,  1, READ_ONLY };
const PciRegister PCI_BIST            = { 0xF,  1, READ_WRITE };
const PciRegister PCI_BASE_ADDRESS_0  = { 0x10, 4, READ_WRITE };
const PciRegister PCI_BASE_ADDRESS_1  = { 0x14, 4, READ_WRITE };
const PciRegister PCI_BASE_ADDRESS_2  = { 0x18, 4, READ_WRITE };
const PciRegister PCI_BASE_ADDRESS_3  = { 0x1C, 4, READ_WRITE };
const PciRegister PCI_BASE_ADDRESS_4  = { 0x20, 4, READ_WRITE };
const PciRegister PCI_BASE_ADDRESS_5  = { 0x24, 4, READ_WRITE };
    /* 0x28->3B - Reserved */
const PciRegister PCI_INTERRUPT_LINE  = { 0x3C, 1, READ_WRITE };
const PciRegister PCI_INTERRUPT_PIN   = { 0x3D, 1, READ_ONLY };
const PciRegister PCI_MIN_GNT         = { 0x3E, 1, READ_ONLY };
const PciRegister PCI_MAX_LAT         = { 0x3F, 1, READ_ONLY };


/* PUBLIC FUNCTIONS */

const char *pciGetErrorString( void ) {
    return pciErrorString;
}

const char *pciGetClassName( FxU32 class_code ) {
    switch( class_code ) {
        /* Pre-Class Code Devices        */
        case 0x000000:  return "old style non-VGA device";
        case 0x000100:  return "old style VGA-compatible device";
        /* Mass Storage Controllers */
        case 0x010000:  return "SCSI bus controller";
        case 0x010100:  return "IDE Controller";
        case 0x010200:  return "floppy Controller";
        case 0x010300:  return "IPI bus controller";
        case 0x010400:  return "RAID controller";
        case 0x018000:  return "mass storage controller";
        /* Network Controllers */
        case 0x020000:  return "ethernet controller";
        case 0x020100:  return "token ring controller";
        case 0x020200:  return "FDDI controller";
        case 0x020300:  return "ATM controller";
        case 0x028000:  return "network controller";
        /* Display Adapters */
        case 0x030000:  return "VGA-compatible display controller";
        case 0x030001:  return "8514-compatible display controller";
        case 0x030101:  return "XGA-compatible display controller";
        case 0x038000:  return "display controller";
        /* Multimedia Adapters */
        case 0x040000:  return "video multimedia adapter";
        case 0x040100:  return "audio multimedia adapter";
        case 0x048000:  return "multimedia adapter";
        /* Memory Controllers */
        case 0x050000:  return "RAM controller";
        case 0x050100:  return "Flash controller";
        case 0x058000:  return "memory controller";
        /* Bridges */
        case 0x060000:  return "host bridge";
        case 0x060100:  return "ISA bridge";
        case 0x060200:  return "EISA bridge";
        case 0x060300:  return "MCA bridge";
        case 0x060400:  return "PCI-toPCI bridge";
        case 0x060500:  return "PCMCIA bridge";
        case 0x060600:  return "NuBus bridge";
        case 0x060700:  return "CardBus bridge";
        case 0x068000:  return "bridge";
        /* Communications Devices */
        case 0x070000:  return "XT-compatible serial controller";
        case 0x070001:  return "16450-compatible serial controller";
        case 0x070002:  return "16550-compatible serial controller";
        case 0x070100:  return "parallel port";
        case 0x070101:  return "bidirectional parallel port";
        case 0x070102:  return "ECP 1.X compliant parallel port";
        case 0x078000:  return "communications device";
        /* Generic System Peripherals */
        case 0x080000:  return "8259 PIC";
        case 0x080001:  return "ISA PIC";
        case 0x080002:  return "EISA PIC";
        case 0x080100:  return "8237 DMA controller";
        case 0x080101:  return "ISA DMA controller";
        case 0x080102:  return "EISA DMA controller";
        case 0x080200:  return "8254 system timer";
        case 0x080201:  return "ISA system timer";
        case 0x080202:  return "EISA system timer";
        case 0x080300:  return "generic RTC controller";
        case 0x080301:  return "ISA RTC controller";
        case 0x088000:  return "system peripheral";
        /* Input Devices */
        case 0x090000:  return "keyboard controller";
        case 0x090100:  return "pen digitizer";
        case 0x090200:  return "mouse controller";
        case 0x098000:  return "input device";
        /* Docking Stations */
        case 0x0A0000:  return "generic docking station";
        case 0x0A8000:  return "docking station";
        /* Processors */
        case 0x0B0000:  return "386";
        case 0x0B0100:  return "486";
        case 0x0B0200:  return "Pentium";
        case 0x0B1000:  return "Alpha";
        case 0x0B2000:  return "PowerPC";
        case 0x0B4000:  return "co-processor";
        /* Serial Bus Controllers */
        case 0x0C0000:  return "FireWire( IEEE 1394 )";
        case 0x0C0100:  return "ACCESS.bus";
        case 0x0C0200:  return "SSA";
        case 0x0C0300:  return "USB";
        case 0x0C0400:  return "FibreChannel";
        default:                return "???";
    }
}

const char *pciGetVendorName( FxU16 vendor_id ) {
    switch( vendor_id ) {
        case 0x1002:    return "ATI";
        case 0x1003:    return "ULSI";
        case 0x1004:    return "VLSI";
        case 0x1005:    return "Avance";
        case 0x100A:    return "Phoenix";
        case 0x100B:    return "National";
        case 0x100C:    return "Tseng";
        case 0x100D:    return "AST";
        case 0x100E:    return "Weitek";
        case 0x1013:    return "Cirrus";
        case 0x1014:    return "IBM";
        case 0x101C:    return "WD";
        case 0x101E:    return "AMI";
        case 0x1022:    return "AMD";
        case 0x1023:    return "Trident";
        case 0x1025:    return "Acer";
        case 0x102B:    return "Matrox";
        case 0x102C:    return "C & T";
        case 0x103C:    return "HP";
        case 0x1040:    return "Kubota";
        case 0x1047:    return "Genoa";
        case 0x104B:    return "BusLogic";
        case 0x104C:    return "TI";
        case 0x104D:    return "Sony";
        case 0x104E:    return "Oak";
        case 0x105D:    return "Number 9";
        case 0x106B:    return "Apple";
        case 0x1073:    return "Yamaha";
        case 0x1078:    return "Cyrix";
        case 0x1091:    return "Intergraph";
        case 0x1092:    return "Diamond";
        case 0x10A9:    return "SGI";
        case 0x10B7:    return "3Com";
        case 0x10B9:    return "Acer";
        case 0x10BC:    return "ALR";
        case 0x10BE:    return "Tseng";
        case 0x10c7:    return "MediaVision";
        case 0x10CE:    return "Radius";
        case 0x10CA:
        case 0x10D0:    return "Fujitsu";
        case 0x10DD:    return "E & S";
        case 0x10DE:    return "NVidia";
        case 0x1102:    return "Creative";
        case 0x121A:    return "3Dfx";
        case 0x5333:    return "S3";
        /* I/O Board Manufacturers */
        case 0x9004:    return "Adaptec";
        case 0x8086:    return "Intel";
        default:                return "???";
    }
}


FxBool FX_EXPORT FX_CSTYLE pciOpen( void ) {
    int deviceNumber;

    if ( libraryInitialized ) return FXTRUE;

    configMechanism = 1;
    busDetected = FXFALSE;  

#if defined( OPEN_WINRT )
    if ( ( hWinRT = WinRTOpenDevice( 0, FALSE ) ) == INVALID_HANDLE_VALUE ) {
        pciErrorString = "WinRT initialization failure.\n"  
                         "Make sure the driver is properly installed.\n";
        return FXFALSE;
    }
#endif
#if defined( PORTIO_GPD )
    hGpdFile = CreateFile( "\\\\.\\GpdDev", GENERIC_READ | GENERIC_WRITE,
                           FILE_SHARE_READ | FILE_SHARE_WRITE,
                           NULL, OPEN_EXISTING, 0, NULL );
    if ( hGpdFile == INVALID_HANDLE_VALUE ) {
        pciErrorString = "Genport initialization failure.\n"  
                         "Make sure the Genport I/O driver is properly installed.\n";
        return FXFALSE;
    }
#endif
#if defined( MAPPL_MEMMAP_VXD )
    hMemmapFile = CreateFile("\\\\.\\FXMEMMAP.VXD", 0, 0, NULL, 0,
                             FILE_FLAG_DELETE_ON_CLOSE, NULL);
    if ( hMemmapFile == INVALID_HANDLE_VALUE ) {
        pciErrorString = "Memmap initialization failure.\n"  
                         "Make sure the Memmap VxD is properly installed.\n";
        return FXFALSE;
    }
#endif
#if defined( MAPPL_MAPMEM_MINIPORT )
    hMapmemFile = CreateFile("\\\\.\\MAPMEM", GENERIC_READ | GENERIC_WRITE,
                             0, NULL, OPEN_EXISTING,
                             FILE_ATTRIBUTE_NORMAL, NULL);
    if ( hMapmemFile == INVALID_HANDLE_VALUE ) {
        pciErrorString = "Mapmem initialization failure.\n"  
                         "Make sure the Mapmem driver is properly installed.\n";
        return FXFALSE;
    }    
#endif

    /*
    **      Scan All PCI device numbers
    */ 

    for ( deviceNumber = 0; deviceNumber < 32; deviceNumber++ ) {
        FxU32 regVal;

        pioOutLong( CONFIG_ADDRESS_PORT, _pciCreateConfigAddress( 0, deviceNumber, 0, 0x0 ) );
        regVal = pioInLong( CONFIG_DATA_PORT );
/* fprintf( stderr, "1: slot %d returned: %04x\n", deviceNumber,regVal); */

    if ( ( regVal & 0xFFFF ) != 0xFFFF ) {
        busDetected = FXTRUE;
        configMechanism = 1;
        deviceExists[deviceNumber] = FXTRUE;
    } else {
        deviceExists[deviceNumber] = FXFALSE;
    }
    }

    if ( !busDetected )  { /* Try Configuration Mechanism 2 (only 16 devices) */
        pioOutByte( CONFIG_SPACE_ENABLE_PORT, 0xF0 );
        pioOutByte( FORWARD_REGISTER_PORT, 0x00);

        for ( deviceNumber = 0; deviceNumber < 16; deviceNumber++ ) {
            FxU32 regVal;

            regVal = pioInLong( _pciCreateConfigSpaceMapping( deviceNumber, 0x0 ) );
/* fprintf( stderr, "2: slot %d returned: %04x\n", deviceNumber,regVal); */
        if ( ( regVal & 0xFFFF ) != 0xFFFF ) {
        busDetected = FXTRUE;
        configMechanism = 2;
        deviceExists[deviceNumber] = FXTRUE;
        } else {
        deviceExists[deviceNumber] = FXFALSE;
        }
    }
    pioOutByte( CONFIG_SPACE_ENABLE_PORT, 0x00 );
    }

    if ( busDetected ) {
        libraryInitialized = FXTRUE;
    } else {
    pciErrorString = "No PCI Bus detected.\n";
    return FXFALSE;
    }    

    return FXTRUE;
}

FxBool FX_EXPORT FX_CSTYLE pciClose( void ) {
    if ( !libraryInitialized ) {
        pciErrorString = "You must open the PCI library before it may be closed.\n";
        return FXFALSE;
    }

#if defined( MAPPL_DPMI )
    DpmiUnmapMemory();
#endif
#if defined( CLOSE_WINRT )
    WinRTCloseDevice( hWinRT );
#endif
#if defined( PORTIO_GPD )
    CloseHandle( hGpdFile );
#endif
#if defined( MAPPL_MEMMAP_VXD )
    CloseHandle( hMemmapFile );
#endif
#if defined( MAPPL_MAPMEM_MINIPORT )
    CloseHandle( hMapmemFile );
#endif
    libraryInitialized = FXFALSE;
    return FXTRUE;
}

FxBool FX_EXPORT FX_CSTYLE pciDeviceExists( FxU32 device_number ) {
    if ( !libraryInitialized ) return FXFALSE;
    if ( device_number > MAX_PCI_DEVICES ) return FXFALSE;
    return deviceExists[device_number];
}

FxBool FX_EXPORT FX_CSTYLE pciGetConfigData( PciRegister reg, FxU32 device_number, FxU32 *data ) {

    if ( !libraryInitialized ) {
        pciErrorString = "You must open the PCI library before you may call pciGetConfigData().\n";
        return FXFALSE;
    }
    if ( device_number > MAX_PCI_DEVICES ) {
        pciErrorString = "Device_number is out of range.\n";
        return FXFALSE;
    }

    if ( !deviceExists[device_number] ) {
    pciErrorString = "Cannot read from a non-existant device.\n";
    return FXFALSE;
    }
    
    if ( reg.rwFlag == WRITE_ONLY ) {
    pciErrorString = "Cannot read a WRITE_ONLY register.\n";
    return FXFALSE;
    }

    *data = _pciFetchRegister( reg.regAddress, reg.sizeInBytes, device_number, configMechanism );

    return FXTRUE;
}

FxBool FX_EXPORT FX_CSTYLE pciSetConfigData( PciRegister reg, FxU32 device_number, FxU32 *data ) {

    if ( !libraryInitialized ) {
        pciErrorString = "You must open the PCI library before you may call pciSetConfigData().\n";
        return FXFALSE;
    }
    if ( device_number > MAX_PCI_DEVICES ) {
        pciErrorString = "Device_number is out of range.\n";
        return FXFALSE;
    }

    if ( !deviceExists[device_number] ) {
        pciErrorString = "Cannot update config registers in a non-existant device.\n";
        return FXFALSE;
    }       
    

    if ( reg.rwFlag == READ_ONLY ) {
    pciErrorString = "Cannot write to a READ_ONLY register.\n";
    return FXFALSE;
    }

    _pciUpdateRegister( reg.regAddress, *data, reg.sizeInBytes, device_number, configMechanism );

    return FXTRUE;
}

FxBool FX_EXPORT FX_CSTYLE pciMapPhysicalToLinear( FxU32 *linear_addr, FxU32 physical_addr, FxU32 *length ) {
    if ( !libraryInitialized ) {
        pciErrorString = "You must call pciOpen() before you use any other library entry points.\n";
        return FXFALSE;
    }
#if defined( MAPPL_DPMI )
    *linear_addr = DpmiMapPhysicalToLinear( physical_addr, *length );
#elif defined( MAPPL_PHARLAP )
    {
        CONFIG_INF config;  /* information about Phar Lap environment */
        ULONG DevPageCount; /* page count of device memory */
        ULONG LinBase;      /* linear base address of ds segment */
        ULONG pDev;         /* linear ptr where device is mapped */
        UINT DevSize;       /* page-aligned size of device memory */
        ULONG pDevPage;     /* linear scan ptr thru device pages */
        ULONG pte;          /* page table entry */
        ULONG pti;          /* page table info */
        
        pciPhrlapGetConfigInfo( &config, (UCHAR*) &config );
        DevPageCount = ((ULONG) *length) >> 12;  /* 4 KB pages */
        if ( pciPhrlapMapPhysMemEndSegment( config.c_ds_sel, physical_addr, DevPageCount, (ULONG*) linear_addr ) != 0 )
        {
            pciErrorString = "Phar Lap returned an error trying to map physical to flat memory.\n";
            return FXFALSE;
        }
        /* This is per TNT DOS-Extender SDK 8.0 Release Notes */
        /* pciPhrlapReadPageTableEntry & pciPhrlapWritePageTableEntry are not
           supported under DPMI. */
        /* xxx PE_PCD is supposed to be set by the DPMI map device in memory call--
           do we need to use DPMI's versions of these or are we ok?
           There doesn't seem to be corresponding calls for DPMI */
        if ( !config.c_dpmif )
        {
            if ( pciPhrlapGetSegmentLinearBase( config.c_ds_sel, &LinBase ) != 0 )
            {
                pciErrorString = "Phar Lap returned an error trying to map physical to flat memory.\n";
                return FXFALSE;
            }
            pDev = *linear_addr + LinBase;
            DevSize = (UINT) *length;
            /* Touch every page 0x1000 = 4 KB pages */
            for ( pDevPage = pDev; pDevPage - pDev < DevSize; pDevPage += 0x1000 )
            {
                if ( pciPhrlapReadPageTableEntry( pDevPage, &pte, &pti ) != 0 )
                {
                    pciErrorString = "Phar Lap returned an error trying to map physical to flat memory.\n";
                    return FXFALSE;
                }
                pte |= PE_PCD;
                if ( pciPhrlapWritePageTableEntry( pDevPage, pte, pti ) != 0 )
                {
                    pciErrorString = "Phar Lap returned an error trying to map physical to flat memory.\n";
                    return FXFALSE;
                }
            }
        }
    }
#elif defined( MAPPL_MEMMAP_VXD )
    {
        DWORD nret;
        DWORD version;
        DWORD Physical [2];       /* Physical address[0] & size[1] */
        DWORD Linear [2];         /* Linear address[0] & size[1] */
        LPDWORD pPhysical = Physical;
        LPDWORD pLinear = Linear;

        Physical[0] = physical_addr;
        Physical[1] = *length;

        /* Check version */
        /* xxx - should check for features */
        /* xxx - should check version is actually compatible */
        DeviceIoControl(hMemmapFile, GETAPPVERSIONDWORD, NULL, 0, &version, 
                        sizeof(version), &nret, NULL);

        /* Map physical to linear */
        /* xxx - returns 0 in Linear if fails, but really should
                 check return value, 0 is suceess, -1 is failure. */

        /* Stuff added to auto-switch passthru using fxmemmap */
        if (getenv("SST_DUALHEAD") == NULL) {
	    DeviceIoControl(hMemmapFile, GETLINEARADDR_AUTO, 
                        &pPhysical, sizeof(pPhysical), 
                        &pLinear, sizeof(pLinear), 
                        &nret, NULL);
        } else {
	    DeviceIoControl(hMemmapFile, GETLINEARADDR, 
                        &pPhysical, sizeof(pPhysical), 
                        &pLinear, sizeof(pLinear), 
                        &nret, NULL);
        }

        *linear_addr = Linear[0];

        if ( nret == 0 ) {
            pciErrorString = "Memmap returned an error trying to map physical to linear memory.\n";
            return FXFALSE;
        }
    }
#elif defined( MAPPL_MAPMEM_MINIPORT )
    {
        PHYSICAL_MEMORY_INFO pmi;
        DWORD                cbReturned;

        pmi.InterfaceType       = PCIBus;
        pmi.BusNumber           = 0;
        pmi.BusAddress.HighPart = 0x00000000;
        pmi.BusAddress.LowPart  = physical_addr;
        pmi.AddressSpace        = 0;
        pmi.Length              = *length;

        if ( !DeviceIoControl( hMapmemFile,
                              (DWORD) IOCTL_MAPMEM_MAP_USER_PHYSICAL_MEMORY,
                              &pmi, sizeof(PHYSICAL_MEMORY_INFO),
                              linear_addr, sizeof(PVOID),
                              &cbReturned, NULL ) ) {
            pciErrorString = "Mapmem returned an error trying to map physical to linear memory.\n";
            return FXFALSE;
        }
    }
#elif defined( MAPPL_WINRT )
    {
        WINRT_MEMORY_MAP map;
        map.address = (PVOID)physical_addr;
        map.length = *length;
        if ( !WinRTMapMemory( hWinRT, &map, linear_addr, length ) ) {
            pciErrorString = "WinRT was returned an error trying to map physical to linear memory.\n";
            return FXFALSE;
        }
    }
#else
#error You need to map the physical address somehow.
#endif
    return FXTRUE;
}

void FX_EXPORT FX_CSTYLE pciUnmapPhysical( FxU32 linear_addr, FxU32 length ) {
#if defined( MAPPL_DPMI )
    /* Do nothing */
    /* xxx - we should call Free Physical Mapping */
#elif defined( MAPPL_PHARLAP )
    /* Do nothing */
#elif defined( MAPPL_MEMMAP_VXD )
    /* Do nothing */
#elif defined( MAPPL_MAPMEM_MINIPORT )
    DWORD                cbReturned;

    DeviceIoControl( hMapmemFile,
                     (DWORD) IOCTL_MAPMEM_UNMAP_USER_PHYSICAL_MEMORY,
                     &linear_addr, sizeof(PVOID),
                     NULL, 0,
                     &cbReturned, NULL );
#elif defined ( MAPPL_WINRT )
    WinRTUnMapMemory( hWinRT, linear_addr, &length );
#else
#error You need to unmap the physical address somehow.
#endif
    return;
}

FxBool FX_EXPORT FX_CSTYLE pciFindCardMulti(FxU32 vendorID, FxU32 deviceID, FxU32 *devNum, FxU32 cardNum)
{
    FxU32 deviceNumber;

    /*      1) open the PCI device and scan it for devices */
    if (!pciOpen()) {
        fprintf(stderr, pciGetErrorString());
#if defined( PORTIO_WINRT ) || defined( MAPPL_WINRT )
        fprintf(stderr,"Use \"net start winrt\" to start the driver.\n");
#endif
#if defined( PORTIO_GPD )
        fprintf(stderr,"Use \"net start genport\" to start the driver.\n");
#endif
#if defined( MAPPL_MAPMEM_MINIPORT )
        fprintf(stderr,"Use \"net start mapmem\" to start the driver.\n");
#endif
        exit(100);
    }

    /*      2) scan the existing devices for a match */
    for ( deviceNumber = 0; deviceNumber < MAX_PCI_DEVICES; deviceNumber++ ) {
        if ( pciDeviceExists( deviceNumber ) ) {
            FxU32 vID, dID;

            pciGetConfigData( PCI_VENDOR_ID, deviceNumber, &vID );
            pciGetConfigData( PCI_DEVICE_ID, deviceNumber, &dID );
            if ((vID == vendorID) && ((dID == deviceID) || deviceID==0xFFFF)) {
                if (cardNum == 0) {
                    *devNum = deviceNumber;
                    return FXTRUE;
                }
                cardNum--;
            }
        }
    }
    return FXFALSE;         /* didn't find the card, return false */
}

FxBool FX_EXPORT FX_CSTYLE pciFindCard(FxU32 vendorID, FxU32 deviceID, FxU32 *devNum)
{
    return pciFindCardMulti(vendorID, deviceID, devNum, 0);
}

/*---------------------------------------------------------------------- */
/* find and map a PCI card into virtual memory using the following 4 steps: */
/*      1) open the PCI device and scan it for devices */
/*      2) scan the existing devices for a vendorId, deviceId, cardNum match */
/*      3) find the current physcial address of the card */
/*      4) map the physical memory to virtual memory */
/*---------------------------------------------------------------------- */
FxU32 FX_EXPORT * FX_CSTYLE pciMapCardMulti(FxU32 vendorID, FxU32 deviceID, FxI32 length, FxU32 *devNum, FxU32 cardNum)
{
    FxU32 physAddress, virtAddress;

    /*      1) open the PCI device and scan it for devices */
    /*      2) scan the existing devices for a match */
    if (!pciFindCardMulti(vendorID, deviceID, devNum, cardNum))
        return NULL;
    /*      3) find the current physcial address of the card */
    pciGetConfigData( PCI_BASE_ADDRESS_0, *devNum, &physAddress );
    if (length <= 0) return (FxU32 *)length;
    physAddress &= ~0xF;
    /*      4) map the physical memory to virtual memory */
    if (!pciMapPhysicalToLinear(&virtAddress, physAddress, (FxU32 *) &length)) {
        fprintf(stderr, pciGetErrorString());
        fprintf(stderr,"Is Memory Address & Count setup properly in the Registry?\n");
        exit(101);
    }
    return (FxU32 *)virtAddress;
}

FxU32 FX_EXPORT * FX_CSTYLE pciMapCard(FxU32 vendorID, FxU32 deviceID, FxI32 length, FxU32 *devNum)
{
    return pciMapCardMulti(vendorID, deviceID, length, devNum, 0);
}
