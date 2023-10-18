
/*
 *	controls.h	-- Definitions for control layout of various stacks.
 *
 *	======================================================================
 *	$Author: gray $						$Date: 1997/05/31 02:23:34 $
 *	$Revision: 3.4 $						$Locker:  $
 *	======================================================================
 *
 *		Copyright 1996 Atari Games.
 *	Unauthorized reproduction, adaptation, distribution, performance or 
 *	display of this computer program or the associated audiovisual work
 *	is strictly prohibited.
 *
 *	======================================================================
 *
 *	Change Log:
 *	$Log: controls.h,v $
 * Revision 3.4  1997/05/31  02:23:34  gray
 * Removed old force wheel hw addresses.
 *
 * Revision 3.3  1996/11/14  12:51:08  geraci
 * raw wheel range is now defined here
 *
 * Revision 3.2  1996/11/11  09:18:05  geraci
 * new a-to-d method
 *
 * Revision 3.1  1996/09/09  02:23:24  gray
 * JAMMA version
 *
 * Revision 2.4  1996/08/18  19:35:00  gray
 * Make PHOENIX_PORT only, removed flag.
 *
 * Revision 2.3  1996/06/14  23:22:17  gray
 * Fix address of SWFORCE port for phoenix.
 *
 * Revision 2.2  1996/06/03  20:54:05  gray
 * Phoenix port.
 *
 * Revision 2.1  1996/05/04  02:56:18  petrick
 * Focus 2 release
 *
 * Revision 1.11  1996/03/25  19:04:17  petrick
 * Moved WHEEL_RANGE here.
 *
 * Revision 1.10  1996/03/14  17:48:13  gray
 * Switch definitions here now (were in zglue.h ?).
 *
 * Revision 1.9  1996/02/01  03:57:54  petrick
 * ADC_xxx definitions moved here so the are more global.
 *
 * Revision 1.8  1996/01/20  03:40:12  geraci
 * Different stacks are controlled with debug switches
 *
 * Revision 1.7  1996/01/19  19:48:03  gray
 * Update settings for ALANSTACK.
 *
 * Revision 1.6  1996/01/19  02:40:28  geraci
 * got rid of SHIFTER and enable XIO for JOHNSTACK
 *
 * Revision 1.5  1996/01/11  22:02:53  geraci
 * BETSY is gone
 *
 * Revision 1.4  1996/01/10  20:02:19  gray
 * Update SPOOSTACK and ALANSTACK to XIO.
 *
 * Revision 1.3  1996/01/09  01:53:02  petrick
 * Alan's stack moved to XIO.
 *
 * Revision 1.2  1996/01/06  01:50:54  gray
 * Add SPOOSTACK flags. Change OPTORD to non-kludge address.
 *
 * Revision 1.1  1995/12/23  01:17:22  petrick
 * Initial revision
 *
 * Revision 1.1  1995/12/23  01:17:22  petrick
 * Initial revision
 *
 *
 *	======================================================================
 */

/*	These define the A/D converter used by the various analog controls. */
typedef enum
{
	ADC_ACCEL,
	ADC_BRAKE,
	ADC_CLUTCH,
	ADC_WHEEL,
	NUM_POTS
}	POT_TYPES;

#define WHEEL_RANGE		(0xc00 >> 2)	/* Range of steering wheel choices */
#define RAW_WHEEL_RANGE 2048

#define SW_EDGES 		(0x0FFFFFFFF)
#define SW_LEVELS 		(0x0)

#ifndef SW_RADIO		/* Also defined in config.h */
#define SW_RADIO 		(0x8)	/*Radio button*/
#endif

#ifndef SW_REVERSE	
#define SW_REVERSE 		(0x00000100)	/*Radio button*/
#endif

#ifndef SW_FIRST	
#define SW_FIRST		0x00000200			/* Shifter gear bitmask defines */
#endif
#ifndef SW_SECOND	
#define SW_SECOND		0x00000400
#endif
#ifndef SW_THIRD	
#define SW_THIRD		0x00000800
#endif
#ifndef SW_FOURTH	
#define SW_FOURTH		0x00001000
#endif

#ifndef SW_ABORT	
#define SW_ABORT		SW_START
#endif


