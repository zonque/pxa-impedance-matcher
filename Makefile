CFLAGS += -Wall -ffreestanding -marm -fpic -mno-single-pic-base -fno-builtin -fno-stack-protector -fomit-frame-pointer
LDFLAGS = -static -nostdlib
GCC = $(CROSS_COMPILE)gcc
OBJCOPY = $(CROSS_COMPILE)objcopy
LD = $(CROSS_COMPILE)ld
MKIMAGE ?= mkimage

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

ifneq ($(origin LIBFDT), undefined)
CFLAGS += -DLIBFDT="$(LIBFDT)" -I./libfdt
LDFLAGS += -L./libfdt -lfdt
endif

ifneq ($(origin RELOCATE_DTB), undefined)
CFLAGS += -DRELOCATE_DTB=$(RELOCATE_DTB)
endif

ALL_OBJS = $(COMMON_OBJS) $(BOARD_OBJ) $(UART_OBJ) $(INPUT_OBJS) $(BINARY_OBJS)

export CFLAGS GCC CROSS_COMPILE

all: uImage

version.h:
	./genver.sh >version.h

zimage.o: $(APPEND_KERNEL)
	cp $(APPEND_KERNEL) input/zImage
	$(OBJCOPY) -I binary -O $(BINFMT) -B arm --prefix-sections zImage input/zImage $@
	$(MAKE) -C libfdt

dtbs-bin.o: $(APPEND_DTBS)
	./append_dtbs.sh dtbs.bin $^
	$(OBJCOPY) -I binary -O $(BINFMT) -B arm dtbs.bin $@

%.o: %.c
	$(GCC) $(CFLAGS) -c $^

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
	$(MAKE) -C libfdt clean

distclean: clean
	rm -fr cscope.*

cscope:
	-find . -name "*.[chS]" >cscope.files
	-cscope -b -R
