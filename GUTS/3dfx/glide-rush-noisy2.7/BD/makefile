# board specific defines
MACH=-mcpu=r4600 -mips2 -fshort-double #-EL 

GCC=		gccr4kl
#GCC=		g++r4kl
SIZE=		sizer4kl
NM=		nmr4kl
CONVERT=	objcopyr4kl
AR=		arr4kl
RANLIB=		ranlibr4kl

PROFILE=	NO
KERNEL=		NO

ifeq ($(PROFILE),YES)
	PROFILE=	-DPROFILE
else
	PROFILE=
endif

ifeq ($(KERNEL),YES)
	KERNEL=		-DKERNEL
else
	KERNEL=
endif

OBJDIR=		obj
CSRCDIR=	csrc
ASRCDIR=	asrc
INCDIR=		include
MISC=		-c #-lang-c-c++-comments 
BUILDDIR=	.

DEFINES=	$(PROFILE) $(KERNEL) -DGLIDE_LIB -DDMA_REGISTERS -DSAVE_GP=1 -DFAKE_STORES=1 -DGLIDE_SHAME
DEFINES +=	-DMAX_NUM_SST=1 -DGLIDE_USE_C_TRISETUP -DGLIDE_NO_SPLASH -DFAKE_IO -DNOISY
DEFINES +=	#-DFIFO_CHECKING -DTIME_FIFO_WAITS -DUSE_CACHED_SST 
DEFINES +=	-DSDMS_TIMING

HEADERS	=-I. -I$(INCDIR)

AFLAGS = $(MISC) $(DEFINES) -g -DCLANGUAGE $(HEADERS) -G 0 $(MACH) -mips3 
CFLAGS = $(MISC) $(DEFINES) -g -fomit-frame-pointer -c $(HEADERS) -G 0 $(MACH)
AS2= $(GCC) $(AFLAGS) -c 
CCX= $(GCC) $(CFLAGS)
CC2= $(CCX) -O2
CC1= $(CCX) -O1
CC0= $(CCX) -O0
CC = $(CC2)
AS = $(AS2)

#INCS=	$(INCDIR)/3dfx.h $(INCDIR)/fx64.h $(INCDIR)/fxglide.h \
#	$(INCDIR)/fxpci.h $(INCDIR)/gdebug.h $(INCDIR)/glide.h \
#	$(INCDIR)/gsstdef.h $(INCDIR)/ncc.h $(INCDIR)/sst.h
#
#OBJS=	$(OBJDIR)/g3df.o $(OBJDIR)/gamma.o \
#	$(OBJDIR)/gdraw.o $(OBJDIR)/tridms.o $(OBJDIR)/triagc.o \
#	$(OBJDIR)/gerror.o $(OBJDIR)/gglide.o $(OBJDIR)/glfb.o \
#	$(OBJDIR)/gsst.o $(OBJDIR)/gtexdl.o $(OBJDIR)/gu.o \
#	$(OBJDIR)/guclip.o $(OBJDIR)/gtex.o \
#	$(OBJDIR)/tritex.o $(OBJDIR)/sst1init.o $(OBJDIR)/sprite.o 

INCS =  $(INCDIR)/3dfx.h $(INCDIR)/files $(INCDIR)/fx64.h $(INCDIR)/fxdll.h $(INCDIR)/fxglide.h \
	$(INCDIR)/fxglob.h $(INCDIR)/fximg.h $(INCDIR)/fxos.h $(INCDIR)/fxpci.h $(INCDIR)/glide.h \
	$(INCDIR)/glidesys.h $(INCDIR)/glideutl.h $(INCDIR)/glob.h $(INCDIR)/gsstdef.h \
	$(INCDIR)/gump.h $(INCDIR)/sst1_pci.h $(INCDIR)/tsc.h

OBJS = 	$(OBJDIR)/dac.o $(OBJDIR)/fx64.o $(OBJDIR)/fxglob.o \
	$(OBJDIR)/gaa.o $(OBJDIR)/gamma.o $(OBJDIR)/gbanner.o \
	$(OBJDIR)/gdraw.o $(OBJDIR)/gerror.o $(OBJDIR)/gglide.o $(OBJDIR)/glfb.o \
	$(OBJDIR)/gmovie.o $(OBJDIR)/gpci.o $(OBJDIR)/gsplash.o $(OBJDIR)/gsst.o $(OBJDIR)/gtex.o \
	$(OBJDIR)/gtexdl.o $(OBJDIR)/gu.o $(OBJDIR)/guclip.o $(OBJDIR)/gump.o $(OBJDIR)/gxdraw.o \
	$(OBJDIR)/info.o $(OBJDIR)/misc.o $(OBJDIR)/print.o $(OBJDIR)/sli.o \
	$(OBJDIR)/sst1_pci.o $(OBJDIR)/sst1init.o $(OBJDIR)/tsc.o  \
	$(OBJDIR)/tridms.o  \
	$(OBJDIR)/util.o $(OBJDIR)/video.o $(OBJDIR)/g3df.o 

EXTRAS  =  $(OBJDIR)/fxpci.o $(OBJDIR)/glob.o $(OBJDIR)/parse.o $(OBJDIR)/fximg.o $(OBJDIR)/fxos.o 
#EXTRAS +=  

all:	 $(BUILDDIR)/libglide.a

$(OBJDIR)/%.o : $(CSRCDIR)/%.c
	$(CC) -o $@ $<

$(OBJDIR)/%.o : $(ASRCDIR)/%.S
	$(AS) -o $@ $<

%.E : $(CSRCDIR)/%.c
	$(CC) -E $< > $@

%.E : $(ASRCDIR)/%.S
	$(AS) -E $< > $@

$(BUILDDIR)/libglide.a:	$(OBJS)
	-rm -f $(BUILDDIR)/libglide.a
	$(AR) -cqv $(BUILDDIR)/libglide.a $(OBJS)
	$(RANLIB) $(BUILDDIR)/libglide.a

clean:	clobber
	-rm -f $(OBJDIR)/*.o *.E

clobber:
	-rm -f $(BUILDDIR)/libglide*

distribute:	clean
	tar -zcvf /home/ftp/pub/glide.tgz .

ALLD0   =       $(patsubst $(OBJDIR)/%.o,$(CSRCDIR)/%.c,$(OBJS))
ALLD1   =       $(patsubst $(CSRCDIR)/tridms.c,$(ASRCDIR)/tridms.S,$(ALLD0))
ALLDEPENDS=     $(ALLD1)

depends : $(ALLDEPENDS)
	@echo "\tMaking dependencies ..."
	@rm -f $@
	@$(GCC) $(CFLAGS) -M $^ >> $@
	@echo "Done"

$(OBJDIR)/dac.o: $(CSRCDIR)/dac.c \
  $(INCDIR)/sst.h $(INCDIR)/ncc.h $(INCDIR)/fx64.h $(INCDIR)/3dfx.h $(INCDIR)/fxdll.h \
  $(INCDIR)/sst1vid.h $(INCDIR)/sst1init.h $(INCDIR)/fxpci.h
$(OBJDIR)/fx64.o: $(CSRCDIR)/fx64.c \
  $(INCDIR)/fx64.h $(INCDIR)/3dfx.h
$(OBJDIR)/fxglob.o: $(CSRCDIR)/fxglob.c
$(OBJDIR)/gaa.o: $(CSRCDIR)/gaa.c \
  $(INCDIR)/3dfx.h $(INCDIR)/fxdll.h $(INCDIR)/glide.h $(INCDIR)/sst1vid.h \
  $(INCDIR)/glidesys.h $(INCDIR)/glideutl.h $(INCDIR)/fxglide.h $(INCDIR)/gsstdef.h \
  $(INCDIR)/sst.h $(INCDIR)/ncc.h $(INCDIR)/fx64.h
$(OBJDIR)/gamma.o: $(CSRCDIR)/gamma.c \
  $(INCDIR)/sst.h $(INCDIR)/ncc.h $(INCDIR)/fx64.h $(INCDIR)/3dfx.h $(INCDIR)/fxdll.h \
  $(INCDIR)/sst1vid.h $(INCDIR)/sst1init.h $(INCDIR)/fxpci.h
$(OBJDIR)/gbanner.o: $(CSRCDIR)/gbanner.c $(INCDIR)/glide.h $(INCDIR)/3dfx.h $(INCDIR)/sst1vid.h \
  $(INCDIR)/glidesys.h $(INCDIR)/glideutl.h $(INCDIR)/fxglide.h $(INCDIR)/gsstdef.h \
  $(INCDIR)/sst.h $(INCDIR)/ncc.h $(INCDIR)/fx64.h $(CSRCDIR)/banner.inc
$(OBJDIR)/gdraw.o: $(CSRCDIR)/gdraw.c \
  $(INCDIR)/3dfx.h $(INCDIR)/fxdll.h $(INCDIR)/glide.h $(INCDIR)/sst1vid.h \
  $(INCDIR)/glidesys.h $(INCDIR)/glideutl.h $(INCDIR)/fxglide.h $(INCDIR)/gsstdef.h \
  $(INCDIR)/sst.h $(INCDIR)/ncc.h $(INCDIR)/fx64.h
$(OBJDIR)/gerror.o: $(CSRCDIR)/gerror.c \
  $(INCDIR)/3dfx.h $(INCDIR)/fxdll.h $(INCDIR)/glide.h $(INCDIR)/sst1vid.h \
  $(INCDIR)/glidesys.h $(INCDIR)/glideutl.h $(INCDIR)/fxglide.h $(INCDIR)/gsstdef.h \
  $(INCDIR)/sst.h $(INCDIR)/ncc.h $(INCDIR)/fx64.h
$(OBJDIR)/gglide.o: $(CSRCDIR)/gglide.c \
  $(INCDIR)/3dfx.h $(INCDIR)/glidesys.h $(INCDIR)/sst.h $(INCDIR)/ncc.h $(INCDIR)/fx64.h \
  $(INCDIR)/sst1vid.h $(INCDIR)/sst1init.h $(INCDIR)/fxpci.h $(INCDIR)/fxdll.h \
  $(INCDIR)/glide.h $(INCDIR)/glideutl.h $(INCDIR)/fxglide.h $(INCDIR)/gsstdef.h
$(OBJDIR)/glfb.o: $(CSRCDIR)/glfb.c $(INCDIR)/3dfx.h $(INCDIR)/fxdll.h $(INCDIR)/glide.h \
  $(INCDIR)/sst1vid.h $(INCDIR)/glidesys.h $(INCDIR)/glideutl.h $(INCDIR)/fxglide.h \
  $(INCDIR)/gsstdef.h $(INCDIR)/sst.h $(INCDIR)/ncc.h $(INCDIR)/fx64.h
$(OBJDIR)/gmovie.o: $(CSRCDIR)/gmovie.c \
  $(INCDIR)/3dfx.h $(INCDIR)/fxdll.h $(INCDIR)/glide.h $(INCDIR)/sst1vid.h \
  $(INCDIR)/glidesys.h $(INCDIR)/glideutl.h $(INCDIR)/fxglide.h $(INCDIR)/gsstdef.h \
  $(INCDIR)/sst.h $(INCDIR)/ncc.h $(INCDIR)/fx64.h
$(OBJDIR)/gpci.o: $(CSRCDIR)/gpci.c \
  $(INCDIR)/3dfx.h $(INCDIR)/glide.h $(INCDIR)/sst1vid.h $(INCDIR)/glidesys.h \
  $(INCDIR)/glideutl.h $(INCDIR)/fxglide.h $(INCDIR)/gsstdef.h $(INCDIR)/sst.h \
  $(INCDIR)/ncc.h $(INCDIR)/fx64.h
$(OBJDIR)/gsplash.o: $(CSRCDIR)/gsplash.c $(INCDIR)/glide.h $(INCDIR)/3dfx.h $(INCDIR)/sst1vid.h \
  $(INCDIR)/glidesys.h $(INCDIR)/glideutl.h \
  $(CSRCDIR)/splshdat.c
$(OBJDIR)/gsst.o: $(CSRCDIR)/gsst.c \
  $(INCDIR)/3dfx.h $(INCDIR)/glidesys.h $(INCDIR)/sst1vid.h $(INCDIR)/sst1init.h \
  $(INCDIR)/fxpci.h $(INCDIR)/fxdll.h $(INCDIR)/glide.h $(INCDIR)/glideutl.h \
  $(INCDIR)/fxglide.h $(INCDIR)/gsstdef.h $(INCDIR)/sst.h $(INCDIR)/ncc.h \
  $(INCDIR)/fx64.h
$(OBJDIR)/gtex.o: $(CSRCDIR)/gtex.c \
  $(INCDIR)/3dfx.h $(INCDIR)/fxdll.h $(INCDIR)/glide.h $(INCDIR)/sst1vid.h \
  $(INCDIR)/glidesys.h $(INCDIR)/glideutl.h $(INCDIR)/fxglide.h $(INCDIR)/gsstdef.h \
  $(INCDIR)/sst.h $(INCDIR)/ncc.h $(INCDIR)/fx64.h
$(OBJDIR)/gtexdl.o: $(CSRCDIR)/gtexdl.c \
  $(INCDIR)/3dfx.h $(INCDIR)/fxdll.h $(INCDIR)/glide.h $(INCDIR)/sst1vid.h \
  $(INCDIR)/glidesys.h $(INCDIR)/glideutl.h $(INCDIR)/fxglide.h $(INCDIR)/gsstdef.h \
  $(INCDIR)/sst.h $(INCDIR)/ncc.h $(INCDIR)/fx64.h
$(OBJDIR)/gu.o: $(CSRCDIR)/gu.c \
  $(INCDIR)/3dfx.h $(INCDIR)/fxdll.h $(INCDIR)/glide.h $(INCDIR)/sst1vid.h \
  $(INCDIR)/glidesys.h $(INCDIR)/glideutl.h $(INCDIR)/fxglide.h $(INCDIR)/gsstdef.h \
  $(INCDIR)/sst.h $(INCDIR)/ncc.h $(INCDIR)/fx64.h $(INCDIR)/sst1init.h \
  $(INCDIR)/fxpci.h
$(OBJDIR)/guclip.o: $(CSRCDIR)/guclip.c \
  $(INCDIR)/3dfx.h $(INCDIR)/fxdll.h $(INCDIR)/glide.h $(INCDIR)/sst1vid.h \
  $(INCDIR)/glidesys.h $(INCDIR)/glideutl.h $(INCDIR)/fxglide.h $(INCDIR)/gsstdef.h \
  $(INCDIR)/sst.h $(INCDIR)/ncc.h $(INCDIR)/fx64.h
$(OBJDIR)/gump.o: $(CSRCDIR)/gump.c \
  $(INCDIR)/3dfx.h $(INCDIR)/fxdll.h $(INCDIR)/glide.h $(INCDIR)/sst1vid.h \
  $(INCDIR)/glidesys.h $(INCDIR)/glideutl.h $(INCDIR)/fxglide.h $(INCDIR)/gsstdef.h \
  $(INCDIR)/sst.h $(INCDIR)/ncc.h $(INCDIR)/fx64.h $(INCDIR)/gump.h
$(OBJDIR)/gxdraw.o: $(CSRCDIR)/gxdraw.c \
  $(INCDIR)/3dfx.h $(INCDIR)/glide.h $(INCDIR)/sst1vid.h $(INCDIR)/glidesys.h \
  $(INCDIR)/glideutl.h $(INCDIR)/fxglide.h $(INCDIR)/gsstdef.h $(INCDIR)/sst.h \
  $(INCDIR)/ncc.h $(INCDIR)/fx64.h
$(OBJDIR)/info.o: $(CSRCDIR)/info.c \
  $(INCDIR)/sst.h $(INCDIR)/ncc.h $(INCDIR)/fx64.h $(INCDIR)/3dfx.h $(INCDIR)/fxdll.h \
  $(INCDIR)/sst1vid.h $(INCDIR)/sst1init.h $(INCDIR)/fxpci.h
$(OBJDIR)/misc.o: $(CSRCDIR)/misc.c $(INCDIR)/3dfx.h $(INCDIR)/fxpci.h
$(OBJDIR)/print.o: $(CSRCDIR)/print.c \
  $(INCDIR)/sst.h $(INCDIR)/ncc.h $(INCDIR)/fx64.h $(INCDIR)/3dfx.h $(INCDIR)/sst1vid.h \
  $(INCDIR)/fxdll.h $(INCDIR)/sst1init.h $(INCDIR)/fxpci.h
$(OBJDIR)/sli.o: $(CSRCDIR)/sli.c \
  $(INCDIR)/sst.h $(INCDIR)/ncc.h $(INCDIR)/fx64.h $(INCDIR)/3dfx.h $(INCDIR)/fxdll.h \
  $(INCDIR)/sst1vid.h $(INCDIR)/sst1init.h $(INCDIR)/fxpci.h
$(OBJDIR)/sst1_pci.o: $(CSRCDIR)/sst1_pci.c $(INCDIR)/3dfx.h $(INCDIR)/fxpci.h $(INCDIR)/sst1_pci.h
$(OBJDIR)/sst1init.o: $(CSRCDIR)/sst1init.c \
  $(INCDIR)/sst.h $(INCDIR)/ncc.h $(INCDIR)/fx64.h $(INCDIR)/3dfx.h $(INCDIR)/fxpci.h \
  $(INCDIR)/fxdll.h $(INCDIR)/sst1vid.h $(INCDIR)/sst1init.h
$(OBJDIR)/tsc.o: $(CSRCDIR)/tsc.c \
  $(INCDIR)/3dfx.h $(INCDIR)/tsc.h
$(OBJDIR)/tridms.o: $(ASRCDIR)/tridms.S
$(OBJDIR)/util.o: $(CSRCDIR)/util.c \
  $(INCDIR)/sst.h $(INCDIR)/ncc.h $(INCDIR)/fx64.h $(INCDIR)/3dfx.h $(INCDIR)/fxdll.h \
  $(INCDIR)/sst1vid.h $(INCDIR)/sst1init.h $(INCDIR)/fxpci.h
$(OBJDIR)/video.o: $(CSRCDIR)/video.c \
  $(INCDIR)/sst.h $(INCDIR)/ncc.h $(INCDIR)/fx64.h $(INCDIR)/3dfx.h $(INCDIR)/fxdll.h \
  $(INCDIR)/sst1vid.h $(INCDIR)/sst1init.h $(INCDIR)/fxpci.h
