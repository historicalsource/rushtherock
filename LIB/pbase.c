/*****************************************************************************************
3D Hockey :

Copyright 1994,1995 Time Warner Interactive.  Unauthorized reproduction, adaptation,
distribution, performance or display of this computer program or the associated
audiovisual work is strictly prohibited.
*****************************************************************************************/

#include <config.h>
#include <os_proto.h>
#if HOCKEY
#include "rom.h"	/* for dependencies */
#endif
void main(void);

const char date[]=__DATE__ " " __TIME__;

#if (HOCKEY || MACE)
extern const unsigned char game_optmenu[];
extern const unsigned char game_coinmenu[];
#endif

const struct pconfigb def_pbase =  {
	main,									/* RESET vector */
#if MACE
	game_optmenu,							/* Def option menu  */
	0,
#elif HOCKEY
	game_optmenu,							/* Def option menu  */
	0,/*game_coinmenu,*/							/* Def coin menu    */
#else
	0,										/* Def option menu  */
	0,										/* Def coin menu    */
#endif
	date,									/* MAIN linktime    */
	0,										/* trap type 0:STOP +:68K -:68010  */
	NO_ROM_TEST+NO_RAM_TEST+NO_LOG_RESET
#ifdef GAME_SPECIFIC
	,(int(*)())no_op
#endif
};
				

