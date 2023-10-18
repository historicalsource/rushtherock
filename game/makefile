###############################################################################
#
# gnu makefile for RUSH (R4K host)
#
#   IMPORTANT NOTE: This makefile assumes you have an environment variable GAME
#                   defined.  It should be set to the directory where your code
#                   resides, and be the full path name, not an abbreviated
#                   version.  For instance, mine is
#                   '/home/gizmo/usr/people/rush'.  Note the lack of a '/' at
#                   the end of this definition.
#
#   ===========================================================================
#   $Author: gray $                          $Date: 1997/10/06 21:42:19 $
#   $Revision: 2.47 $                             $Locker:  $
#   ===========================================================================
#   Change Log:
#
#   $Log: makefile,v $
# Revision 2.47  1997/10/06  21:42:19  gray
# Point to saved GUTS
#
# Revision 2.46  1997/09/29  07:28:05  hightower
# Extracted checksum funcs from gamenet.c to cksum.c
#
# Revision 2.45  1997/09/26  06:09:02  hightower
# Changed color off code to turn color to off instead of to blue
#
# Revision 2.44  1997/09/05  23:17:24  gray
# New guts pointer for rush 1.5.
#
# Revision 2.43  1997/09/05  01:22:25  gray
# Point to saved GUTS.
#
# Revision 2.42  1997/08/27  22:44:27  grjost
# Changed GUTS path
#
# Revision 2.41  1997/08/22  01:36:46  gray
# Added gamenet module.
#
# Revision 2.40  1997/07/27  23:02:08  gray
# New gnu 2.7 toolchain.
#
# Revision 2.39  1997/04/14  20:17:12  gray
# New GUTS path.
#
# Revision 2.38  1997/03/31  19:51:58  gray
# app_init is gone.
#
# Revision 2.37  1997/03/31  03:22:35  gray
# smc_demo.o and nucleus.a are history.
#
# Revision 2.36  1997/03/26  00:45:53  gray
# GNU includes now in GUTS, removed old phoenix stuff.
#
# Revision 2.35  1996/12/08  01:06:41  geraci
# no production line for convenience
#
# Revision 2.34  1996/12/02  04:54:59  gray
# define PRODUCTION_RELEASE=1 to disable cheat modes and debug switches.
#
# Revision 2.33  1996/11/27  03:30:18  geraci
# Only one glide library is needed
#
# Revision 2.32  1996/11/25  10:48:16  gray
# Use standard glide lib.
#
# Revision 2.31  1996/11/24  20:20:01  geraci
# old and new phoenix lib stuff is the same
#
# Revision 2.30  1996/11/23  07:07:17  petrick
# Made fdnl target use fd to download instead of fdnl.
#
# Revision 2.29  1996/11/19  17:51:59  petrick
# Changed default GUTS to FLOS.
#
# Revision 2.28  1996/11/16  08:34:24  geraci
# added link to rel version of guts
#
# Revision 2.27  1996/11/16  07:52:22  gray
# Setup for Flagstaff, New Disk Driver.
#
# Revision 2.26  1996/11/15  20:10:03  petrick
# New disk driver changes.
#
# Revision 2.25  1996/11/13  01:20:55  geraci
# added flagstaff guts pointer
#
# Revision 2.24  1996/10/31  00:59:28  gray
# Temp test glide lib.
#
# Revision 2.23  1996/10/16  09:40:01  petrick
# Added tourney.c
#
# Revision 2.22  1996/10/08  05:20:58  gray
# Flagstaff port.
#
# Revision 2.21  1996/09/28  01:45:51  petrick
# Added hiscore.c, got rid of some old comments.
#
# Revision 2.20  1996/09/05  21:45:08  gray
# New PXLIB and PXMB to support reflection maps.
#
# Revision 2.19  1996/09/05  08:22:11  petrick
# Temporary change to Jim's MB and LIB to install Bruce's latest changes.
#
# Revision 2.18  1996/09/01  09:14:05  gray
# Added reckon.c
#
# Revision 2.17  1996/08/27  08:23:51  gray
# Added stats.c, removed reent.c.
#
# Revision 2.16  1996/08/18  19:37:03  gray
# Made code PHOENIX_PORT only, removed flag from makefile.
#
# Revision 2.15  1996/08/18  03:32:06  gray
# Can kill sprintf.c now that zoid is gone.
#
# Revision 2.14  1996/08/15  07:49:36  petrick
# Added game options menu stuff.
#
# Revision 2.13  1996/07/28  23:33:46  petrick
# Pointed LIBS back to RUSH standard locations.
#
# Revision 2.12  1996/07/26  10:27:05  petrick
# Changes for 32 bit mb indices.
#
# Revision 2.11  1996/07/24  11:06:58  petrick
# Points to Forrest's 'new' OS.
#
# Revision 2.10  1996/07/16  02:05:14  gray
# New glide2 stuff.
#
# Revision 2.9  1996/06/27  22:08:31  petrick
# Added pdnl target to download to host defined by environ variable PHXHOST
#
# Revision 2.8  1996/06/20  21:20:11  gray
# vfprintf.c removed from phoenix LIB. Move __MIPSEL__ define to makedepend only.
#
# Revision 2.7  1996/06/19  20:42:50  petrick
# Explicitly defined __MIPSEL__ so we can compile HACK.
#
# Revision 2.6  1996/06/17  17:53:00  gray
# fixed LINKF dependancy. Use local net_link.a.
#
# Revision 2.5  1996/06/15  05:01:06  petrick
# Added targets.c to list of files.
#
# Revision 2.4  1996/06/15  00:57:47  gray
# ___PHOENIX VERSION___
#
# Revision 2.3  1996/05/25  02:18:16  gray
# Added dnl.c and dnl.h for hard drive download.
#
# Revision 2.2  1996/05/08  01:24:23  gray
# Made common OS and LIB dirs in /home/boffo/usr7/rush/game.
#
# Revision 2.1  1996/05/04  02:56:18  petrick
# Focus 2 release
#
###############################################################################

default: game        # Default target to make.  Don't move it.
.SILENT:             # Makes 'make' not echo commands when executed.
.MAKEOPTS:  -k       # Default make flags.

###############################################################################
###	VARIABLES	#######################################################
###############################################################################

###	PATHS	###############################################################

GAMELIB     = /home/boffo/usr7/rush/game/LIB

GUTS       = /home/boffo/usr7/rush/game/OS
#GUTS       = /usr/local/GUTS/3dfx/rush/flr5k/rel
#GUTS       = /usr/local/GUTS/3dfx/rush/flr5k/new
#GUTS        = /usr/local/GUTS/3dfx/rush_1.5/flr5k/new

MB      = /home/boffo/usr7/rush/game/MB
#MB     = /home/fred/usr/people/gray/rush/game/code/MB

GLIDET_INCLUDE_PATH = /usr/local/GUTS/3dfx/glide-rush2.7/include
GLIDET_LIBRARY_PATH = /usr/local/GUTS/3dfx/glide-rush2.7

ERR         = err/
PRO         = $(GAME)/Pro
OBJ         = obj/
LOC_INCS  = .:$(MB):$(GUTS):$(GAMELIB):./inc
SYS_INCS1 = $(R4KL_INCLUDE_PATH):$(R4KL_INCLUDE_PATH_HW):$(GLIDET_INCLUDE_PATH)

# Set the following variable to a colon separated list of directories in which
# to find .h, .i and .inc files. This list will be converted to -I directives
# and assigned to the variable GNU_INCS.
HPATH       = $(SYS_INCS1):$(LOC_INCS):$(PRO)
GNU_INCS    = -I$(subst :, -I,$(HPATH))
VPATH       = $(HPATH)       # Used by make to look up include files

LINKF       = game.lnk       # Linker control file.

### FLAGS   #########################################################################
SHOWERR     = t              # display errors on screen while building.
SHOWWARN    = t              # display warnings while building.
# If PRODUCTION_RELEASE=1, all cheat modes and debug switched are disabled.
DEFS        = -DDEBUG -DPRINTF_EMBEDDED -DMB_HOST=1 -DPRODUCTION_RELEASE=1
DEPFLAGS    = $(GNU_INCS) $(DEFS) # global #defines to use while compiling C
ASM_PPFLAGS = -line          # Add -lis for listings of MACPP'd sources, \
                             # Leave the -line for asxx to report line nums

ASMFLAGS    = -mips3 -32 -G 0 -g
LIBDIRS     = -L$(GUTS) -L${R4KL_LIBRARY_PATH} -L${GLIDET_LIBRARY_PATH}
LIBS        =  $(MB)/zmb.a -los -lglide -lnsprintff -lmf -lcf -lgcc
CFLAGS      = $(GNU_INCS) $(DEFS) -ansi -mhard-float -fshort-double -G 0 -Wall -Wno-unused \
                -mips3 -mgp32 -mfp64 -Wa,-32 \
                -Wno-cast-qual -Wno-return-type \
#               -Wredundant-decls -pedantic

### TOOLS   ###################################################################
PROCESSOR   = r4kl                  # idt R4600
CCO         = gcc$(PROCESSOR) $(CFLAGS) -O              # Set -O0 for no optimize and -g3 for max debug info
CPROTO_CPP 	= gcc$(PROCESSOR) -E $(GNU_INCS) $(DEFS)	# Used for creating dependencies
PP			= macpp					# assembelr pre-processor
ASM 		= as$(PROCESSOR)		# assembler
LN 			= ld$(PROCESSOR)		# linker
GENPROTO 	= cproto				# Prototype generator program.
MIXIT   	= mixit
RM 			= /sbin/rm -f			# Use rm with flags preset.
ECHO 		= /bin/echo
SHELL		= /bin/sh				# Used by make
CMP 		= cmp -s				# File comparison.

###	SHORTCUTS	#######################################################
BELL		= $(ECHO) "\007"
USECOLOR	= t
ERR_ON   	= if [ $(USECOLOR) = "t" ]; then ($(ECHO) -n '\033[31m'); fi
WARN_ON   	= if [ $(USECOLOR) = "t" ]; then ($(ECHO) -n '\033[33m'); fi
MSG_ON   	= if [ $(USECOLOR) = "t" ]; then ($(ECHO) -n '\033[32m'); fi
COLOR_OFF   = if [ $(USECOLOR) = "t" ]; then ($(ECHO) -n '\033[0m'); fi


###############################################################################
###	OBJECTS	###############################################################
###############################################################################

###	GAMELIB	###############################################################
LIBCODEC1	= anim.c balloon.c blit.c error.c fmath.c font.c
LIBCODEC2	= hdrive.c mem.c mole.c mole_info.c
LIBCODEC3	= timer.c strings.c
LIBCODEC 	= $(LIBCODEC1) $(LIBCODEC2) $(LIBCODEC3)
LIBCODEO	= $(addprefix obj/,$(subst .c,.o,$(LIBCODEC)))
ODIR_LIBC 	= $(addprefix $(GAMELIB)/,$(LIBCODEC))

###	GAME CODE	#######################################################
C1	= attract.c camera.c carsnd.c checkpoint.c cksum.c
C2	= collision.c cpinit.c debug.c dis.c dnl.c drones.c
C3	= fswheel.c game.c gamenet.c globals.c hiscore.c hud.c init.c
C4	= math.c mathsubs.c maxpath.c mdrive.c menu_data.c menus.c
C5	= model.c money.c moobs.c pdu.c pots.c 
C6	= resurrect.c select.c sounds.c stats.c targets.c timint.c tourney.c visuals.c

###	CAR MODEL	#######################################################
C8	= cars.c communic.c controls.c d3math.c drivetra.c drivsym.c 
C9	= initiali.c reckon.c road.c stree.c tires.c unitvecs.c
C10 = vecmath.c world.c 

###	MISC	###############################################################
C12	= gameblk.c			# These files cause problems for the prototyper.

PHX_ROOT = $(GUTS)/phx_root.o
OSINTFC = $(PHX_ROOT) $(addprefix $(OBJ), root.o )

CCOMMON		= $(C1) $(C2) $(C3) $(C4) $(C5) $(C6) $(C7) $(C8) $(C9) $(C10)
CDEPEND		= $(C12) $(CCOMMON)
CFILESPRO 	= $(CCOMMON) $(LIBCODEC)
COBJS		= $(CDEPEND:%.c=$(OBJ)%.o)
TMPDEPEND_C = $(addprefix $(OBJ),$(LIBCODEC))
LINKFILES	= $(OSINTFC) $(COBJS) $(LIBCODEO)

###	ASM FILES	#######################################################
ASM1    	= root.asm
ASMOBJS 	= $(ASM1:%.asm=$(OBJ)%.o)

###	PROTOTYPES	#######################################################
PROTOS    	= $(addprefix $(PRO)/,$(CFILESPRO:.c=.pro))
NEWPROTOS 	= $(subst .pro,.pro_new,$(PROTOS))


###############################################################################
###	TARGETS	###############################################################
###############################################################################

game:	options game.elf game.sym
	$(MSG_ON)
	$(ECHO) 'Everything is up to date.'
	$(COLOR_OFF)

all:	options protos depend game 
	$(MSG_ON)
	$(ECHO) 'Made all'
	$(COLOR_OFF)

fromscratch : realclean all
	$(MSG_ON)
	$(ECHO) 'Brand spanking new copy built'
	$(COLOR_OFF)

dnl : game
	$(MSG_ON)
	$(ECHO) 'Downloading to $(DNLHOST)'
	$(COLOR_OFF)
	dnl $(DNLHOST)
	$(BEEP)
	$(BEEP)

pdnl : game
	$(MSG_ON)
	$(ECHO) 'Downloading to $(PHXHOST)'
	$(COLOR_OFF)
	pdnl $(PHXHOST)
	$(BEEP)
	$(BEEP)

fdnl : game
	$(MSG_ON)
	$(ECHO) 'Downloading to $(FLGHOST)'
	$(COLOR_OFF)
	fd $(FLGHOST)
	$(BEEP)
	$(BEEP)

game.elf : $(LINKFILES) $(LINKF)
	if [ ! -r *.err ]; then\
	  $(ECHO) 'LDR4KL in progress...';\
	  $(LN) -T$(LINKF) -Map game.map -o game.elf $(LIBDIRS) $(LIBS);\
	  $(BELL);\
	else\
	  $(ECHO) 'Fix the ".err" files before linking!';\
	  $(BELL);\
	  exit 1;\
	fi

game.sym: game.elf
	$(ECHO) 'Making symbol table...'
	nm$(PROCESSOR) -n game.elf > game.sym

link_files: $(LINKFILES) $(LINKF)


protos: pre_protos $(PROTOS)
newprotos: $(NEWPROTOS)
$(NEWPROTOS):
$(PROTOS):

$(PRO)/%.pro_new : %.c
	CPROTO_CPP="$(CPROTO_CPP)";\
	export CPROTO_CPP;\
	$(GENPROTO) $(DEPFLAGS) $< > $(PRO)/$*.pro_new 2> /dev/null

$(PRO)/%.pro : $(PRO)/%.pro_new
	`$(CMP) $(PRO)/$*.pro $<` || (cp $< $(PRO)/$*.pro; $(ECHO) 'Updated $(PRO)/$*.pro ...')

depend:
	$(ECHO) ===== MAKING DEPENDENCIES =====
	-$(RM) -f $(OBJ)*.c makefile.dep?*
	touch makefile.dep2
	cd $(OBJ) && ln -s $(ODIR_LIBC) ./
	makedepend -p$(OBJ) -fmakefile.dep2 -- $(DEPFLAGS) -D__MIPSEL__ -- -- $(CDEPEND) $(TMPDEPEND_C)
	sed -e s.$(OBJ)$(OBJ).$(OBJ). makefile.dep2 > makefile.dep
	-$(RM) -f $(OBJ)*.c makefile.dep?*

pre_protos:
	$(ECHO) ===== MAKING PROTOTYPES =====


#	ASM dependancies
$(OBJ)root.o	: root.asm
$(OBJ)os_vec.o	: os_vec.asm

#	The Link File
CACHE=8
RR_SIZE=8

SCACHE=$(strip $(CACHE))

$(LINKF) : makelink makefile 
	@echo "\tMaking $@..."
	PGM_BASE=0x${SCACHE}0000000; export PGM_BASE;\
	 PGM_LEN=0x00280000; export PGM_LEN;\
	   PBASE=0x${SCACHE}0060000; export PBASE;\
	 UNLK_EP=0xB5500000; export UNLK_EP;\
	 RR_SIZE=${RR_SIZE}; export RR_SIZE;\
	 ROOT=${PHX_ROOT}; export ROOT;\
	 INIT_SP=0x${SCACHE}07FFFFC; export INIT_SP;\
	sh $< > $@
	echo >> $@ "INPUT (" $(PHX_ROOT) $(filter-out $(PHX_ROOT),$(LINKFILES)) " )"

#
#  maintain options.h
#
options		: inc/options.h inc/optmenu.h

inc/options.h	: option_pp.mac option_map.mac
	@$(ECHO) ===== MAKING options.h =====
	$(PP) -out=inc/options.h option_pp.mac option_map.mac

inc/optmenu.h	: option_pp.mac option_map.mac
	@$(ECHO) ===== MAKING optmenu.h =====
	-$(RM) foobie
	@echo > foobie "MAKE_MENU == 1"
	$(PP) -out=inc/optmenu.h foobie option_pp.mac option_map.mac
	-$(RM) foobie

#	Cleanup
clean	:
	-$(RM) $(OBJ)*.o
	-$(RM) inc/*.h
	-$(RM) game.map game.sym game.elf
	-$(RM) *.lnk
	-$(RM) $(ERR)*.err $(ERR)*.warn
	-$(RM) *~

realclean: clean
	-$(RM) $(PRO)/*

###############################################################################
###	RULES	###############################################################
###############################################################################

$(OBJ)%.o : %.asm
	$(RM) $(basename $(notdir $@)).lis
	@$(ECHO) "\tMACPP'ing $<..."
	- $(PP) $(ASM_PPFLAGS) -out=$(basename $(@F)).pasm \
		-assem=".include $(GUTS)/def_pp.mac" $< > $(basename $(@F)).err 2>&1; \
	if [ ! -s $(basename $(@F)).err ]; then\
		$(ECHO) "\tAssembling $(basename $(@F)).pasm...";\
		as$(PROCESSOR) $(ASMFLAGS) -o $(OBJ)$(basename $(@F)).o $(basename $(@F)).pasm \
			> $(basename $(@F)).err 2>&1;\
		if [ ! -s $(basename $(@F)).err ]; then\
			$(RM) $(basename $(@F)).err;\
		else\
			$(ECHO) 'Errors in "$(basename $(@F)).pasm"';\
		fi\
	else\
		$(ECHO) 'Errors in "$(basename $(@F)).asm"';\
	fi

# MRI _or_ cygnus GCC toolchain
$(OBJ)%.o : %.c
	echo 'Compiling "$<"...';\
	$(RM) $(OBJ)$*.o;\
	$(CCO) -o $(OBJ)$*.o -c -g $< > $(ERR)$*.err 2>&1;\
	if [ ! -s $(ERR)$*.err ]; then\
		$(RM) $(ERR)$*.err $(ERR)$*.warn;\
	elif [ -f $(OBJ)$*.o ];	then\
		mv $(ERR)$*.err $(ERR)$*.warn;\
		$(WARN_ON);\
		$(ECHO) '    Warnings for "$*.c" in "$(ERR)$*.warn"';\
		if [ $(SHOWWARN) = "t" ]; then\
			cat $(ERR)$*.warn;\
		fi;\
		$(COLOR_OFF);\
	else\
		$(ERR_ON);\
		$(ECHO) '    Errors in "$*.c"';\
		$(BELL);\
		if [ $(SHOWERR) = "t" ]; then\
			cat $(ERR)$*.err;\
		fi;\
		$(COLOR_OFF);\
	fi

###############################################################################
###	DEPENDENCIES	#######################################################
###############################################################################

include makefile.dep 

# DO NOT DELETE THIS LINE -- make depend depends on it.
