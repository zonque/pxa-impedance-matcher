CFLAGS += -Wall -ffreestanding -marm -fpic -mno-single-pic-base -fno-builtin -fno-stack-protector -fomit-frame-pointer
LDFLAGS = -static -nostdlib
CC = $(CROSS_COMPILE)gcc
OBJCOPY = $(CROSS_COMPILE)objcopy
LD = $(CROSS_COMPILE)ld
AR = $(CROSS_COMPILE)ar
MKIMAGE ?= mkimage

VECHO = :

ifeq ($(CFG_FILE),)
include Makefile.config
else
include $(CFG_FILE)
endif

CFLAGS+=-DUART_BASE=$(UART_BASE)
CFLAGS+=-DNR_BANKS=$(NR_BANKS)
CFLAGS+=-DUART_PORT=$(UART_PORT)

BOARD_OBJ = board-$(MFG).o
UART_OBJ = serial-$(UART).o
BINARY_OBJS =
STD_CLEANFILES = *~ *.o *.d *.a *.i *.s a.out

COMMON_OBJS = \
	dtbs.o \
	main.o \
	print.o \
	register.o \
	string.o

ifneq ($(origin APPEND_KERNEL), undefined)
CFLAGS += -DAPPEND_KERNEL="$(APPEND_KERNEL)"
INPUT_OBJS = zimage.o
endif

ifneq ($(origin APPEND_DTBS), undefined)
CFLAGS += -DAPPEND_DTBS="$(APPEND_DTBS)"
BINARY_OBJS += dtbs-bin.o
endif

all: uImage

#
# Generic compile rules
#
%: %.o
	@$(VECHO) LD $@
	$(LINK.c) -o $@ $^ $(LDLIBS)

%.o: %.c
	@$(VECHO) CC $@
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ -c $<

%.o: %.S
	@$(VECHO) AS $@
	$(CC) $(CPPFLAGS) $(AFLAGS) -D__ASSEMBLY__ -o $@ -c $<

%.d: %.c
	@$(VECHO) DEP $<
	$(CC) $(CPPFLAGS) -MM -MG -MT "$*.o $@" $< > $@

%.d: %.S
	@$(VECHO) DEP $<
	$(CC) $(CPPFLAGS) -MM -MG -MT "$*.o $@" $< > $@

%.i:    %.c
	@$(VECHO) CPP $@
	$(CC) $(CPPFLAGS) -E $< > $@

%.s:    %.c
	@$(VECHO) CC -S $@
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ -S $<

%.a:
	@$(VECHO) AR $@
	$(AR) $(ARFLAGS) $@ $^

#
# Rules for libfdt
#
LIBFDT_dir = libfdt
LIBFDT_archive = $(LIBFDT_dir)/libfdt.a
LIBFDT_lib = $(LIBFDT_dir)/libfdt-$(DTC_VERSION).$(SHAREDLIB_EXT)
LIBFDT_include = $(addprefix $(LIBFDT_dir)/,$(LIBFDT_INCLUDES))
LIBFDT_version = $(addprefix $(LIBFDT_dir)/,$(LIBFDT_VERSION))

include $(LIBFDT_dir)/Makefile.libfdt

.PHONY: libfdt
libfdt: $(LIBFDT_archive) $(LIBFDT_lib)

$(LIBFDT_archive): $(addprefix $(LIBFDT_dir)/,$(LIBFDT_OBJS))
$(LIBFDT_lib): $(addprefix $(LIBFDT_dir)/,$(LIBFDT_OBJS))

ifneq ($(DEPTARGETS),)
-include $(LIBFDT_OBJS:%.o=$(LIBFDT_dir)/%.d)
endif
ifneq ($(origin LIBFDT), undefined)
CFLAGS += -DLIBFDT="$(LIBFDT)" -I./libfdt
#LDFLAGS += -L./libfdt -lfdt
endif

ifneq ($(origin RELOCATE_DTB), undefined)
CFLAGS += -DRELOCATE_DTB=$(RELOCATE_DTB)
endif

ALL_OBJS = $(COMMON_OBJS) $(BOARD_OBJ) $(UART_OBJ) $(INPUT_OBJS) $(BINARY_OBJS) $(LIBFDT_archive)

export CFLAGS CC CROSS_COMPILE

version.h:
	./genver.sh >version.h

main.o: version.h

zimage.o: $(APPEND_KERNEL)
	cp $(APPEND_KERNEL) input/zImage
	$(OBJCOPY) -I binary -O $(BINFMT) -B arm --prefix-sections zImage input/zImage $@

dtbs-bin.o: $(APPEND_DTBS)
	./append_dtbs.sh dtbs.bin $^
	$(OBJCOPY) -I binary -O $(BINFMT) -B arm dtbs.bin $@

matcher: version.h $(ALL_OBJS)
	$(LD) -T matcher.lds -Ttext $(LOADADDR) -o $@ $(ALL_OBJS) $(LDFLAGS)

matcher.bin: matcher
	$(OBJCOPY) -O binary --set-section-flags .bss=alloc,load,contents $^ $@

uImage: matcher.bin
	$(MKIMAGE) -A arm -O linux -C none -T kernel \
		-a $(LOADADDR) -e $(LOADADDR) \
		-n "ImpedanceMatcher (3rd stage)" -d $^ $@

clean:
	rm -fr *.bin matcher *.o uImage version.h
	$(MAKE) libfdt_clean

distclean: clean
	rm -fr cscope.*

cscope:
	-find . -name "*.[chS]" >cscope.files
	-cscope -b -R
