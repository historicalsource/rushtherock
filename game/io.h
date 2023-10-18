#ifndef _IO_H
#define _IO_H

/*several of the LIB modules include io.h and expects stuff from config.h*/ 
#include <config.h>
#include "stdtypes.h"

/* querry-replace-regexp
  #define[ 	]*\([A-Z0-9]*\)\(.*\)

  #ifndef \1
#define \1 \2
#endif

*/

#define DOMOLE 1


#define BIT0		0x001L
#define BIT1		0x002L
#define BIT2		0x004L
#define BIT3		0x008L
#define BIT4		0x010L
#define BIT5		0x020L
#define BIT6		0x040L
#define BIT7		0x080L


#define SW0BIT0		(!(*IOSWITCH0 & BIT0))
#define SW0BIT1		(!(*IOSWITCH0 & BIT1))
#define SW0BIT2		(!(*IOSWITCH0 & BIT2))
#define SW0BIT3		(!(*IOSWITCH0 & BIT3))
#define SW0BIT4		(!(*IOSWITCH0 & BIT4))
#define SW0BIT5		(!(*IOSWITCH0 & BIT5))
#define SW0BIT6		(!(*IOSWITCH0 & BIT6))
#define SW0BIT7		(!(*IOSWITCH0 & BIT7))

#define SW1BIT0		(!(*IOSWITCH1 & BIT0))
#define SW1BIT1		(!(*IOSWITCH1 & BIT1))
#define SW1BIT2		(!(*IOSWITCH1 & BIT2))
#define SW1BIT3		(!(*IOSWITCH1 & BIT3))
#define SW1BIT4		(!(*IOSWITCH1 & BIT4))
#define SW1BIT5		(!(*IOSWITCH1 & BIT5))
#define SW1BIT6		(!(*IOSWITCH1 & BIT6))
#define SW1BIT7		(!(*IOSWITCH1 & BIT7))

#define SW2BIT0		(!(*IOSWITCH2 & BIT0))
#define SW2BIT1		(!(*IOSWITCH2 & BIT1))
#define SW2BIT2		(!(*IOSWITCH2 & BIT2))
#define SW2BIT3		(!(*IOSWITCH2 & BIT3))
#define SW2BIT4		(!(*IOSWITCH2 & BIT4))
#define SW2BIT5		(!(*IOSWITCH2 & BIT5))
#define SW2BIT6		(!(*IOSWITCH2 & BIT6))
#define SW2BIT7		(!(*IOSWITCH2 & BIT7))

#define SW3BIT0		(!(*IOSWITCH3 & BIT0))
#define SW3BIT1		(!(*IOSWITCH3 & BIT1))
#define SW3BIT2		(!(*IOSWITCH3 & BIT2))
#define SW3BIT3		(!(*IOSWITCH3 & BIT3))
#define SW3BIT4		(!(*IOSWITCH3 & BIT4))
#define SW3BIT5		(!(*IOSWITCH3 & BIT5))
#define SW3BIT6		(!(*IOSWITCH3 & BIT6))
#define SW3BIT7		(!(*IOSWITCH3 & BIT7))

#define AUX0BIT0	(!(*AUXSWITCH0 & BIT1))
#define AUX0BIT1	(!(*AUXSWITCH0 & BIT2))

#define AUX1BIT0	(!(*AUXSWITCH1 & BIT1))
#define AUX1BIT1	(!(*AUXSWITCH1 & BIT2))

#define DODEBUG		1
#define SELFTEST	SW0BIT6

#define JOYRIGHT	SW0BIT4
#define JOYLEFT		SW0BIT2
#define JOYUP		SW0BIT3
#define JOYDOWN		SW0BIT5
#ifndef BLUEBUTTON
#define BLUEBUTTON SW0BIT1
#endif
#ifndef BLACKBUTTON
#define BLACKBUTTON SW0BIT0
#endif
#ifndef STARTBUTTON
#define STARTBUTTON SW1BIT0
#endif

#define JOY0RT		SW0BIT4
#define JOY0LT		SW0BIT2
#define JOY0UP		SW0BIT3
#define JOY0DN		SW0BIT5
#define JOY0B0		SW0BIT0
#define JOY0B1		SW0BIT1

#define JOY1RT		SW1BIT4
#define JOY1LT		SW1BIT2
#define JOY1UP		SW1BIT3
#define JOY1DN		SW1BIT5
#define JOY1B0		SW1BIT0
#define JOY1B1		SW1BIT1

#define JOY2RT		SW2BIT4
#define JOY2LT		SW2BIT2
#define JOY2UP		SW2BIT3
#define JOY2DN		SW2BIT5
#define JOY2B0		SW2BIT0
#define JOY2B1		SW2BIT1

#define JOY3RT		SW3BIT4
#define JOY3LT		SW3BIT2
#define JOY3UP		SW3BIT3
#define JOY3DN		SW3BIT5
#define JOY3B0		SW3BIT0
#define JOY3B1		SW3BIT1


#define DEBUG0		0
#define DEBUG1		0
#define DEBUG2		0
#define DEBUG3		0
#define DEBUG4		0
#define DEBUG5		0

#define BLACKBTN	0x01
#define BLUEBTN		0x02
#define LEFT 		0x04
#define UP			0x08
#define RIGHT		0x10
#define DOWN		0x20
#define BIGBTN		0x40
#define STARTBTN	0x100

#define DEBUGCOLOR	RED_PAL

#define DOPRINT		1

#endif
