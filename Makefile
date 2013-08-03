CFLAGS+=-Wall -ffreestanding
LDFLAGS=-static -nostdlib
GCC=$(CROSS_COMPILE)gcc
OBJCOPY=$(CROSS_COMPILE)objcopy
LD=$(CROSS_COMPILE)ld

include Makefile.config

CFLAGS+=-DUART_BASE=$(UART_BASE)

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
CFLAGS+=-DAPPEND_KERNEL="$(APPEND_KERNEL)"
INPUT_OBJS=zimage.o
endif

ifneq ($(origin APPEND_DTBS), undefined)
CFLAGS+=-DAPPEND_DTBS="$(APPEND_DTBS)"
BINARY_OBJS+=dtbs-bin.o
endif

ALL_OBJS=$(COMMON_OBJS) $(BOARD_OBJ) $(UART_OBJ) $(INPUT_OBJS) $(BINARY_OBJS)

all: uImage

version.h:
	./genver.sh >version.h

zimage.o: $(APPEND_KERNEL)
	$(OBJCOPY) -I binary -O $(BINFMT) -B arm $^ $@

dtbs-bin.o: $(APPEND_DTBS)
	./append_dtbs.sh dtbs.bin $^
	$(OBJCOPY) -I binary -O $(BINFMT) -B arm dtbs.bin $@

%.o: %.c
	$(GCC) $(CFLAGS) -c $^

matcher: version.h $(ALL_OBJS)
	$(LD) $(LDFLAGS) -T matcher.lds -Ttext $(LOADADDR) -o $@ $(ALL_OBJS)

matcher.bin: matcher
	$(OBJCOPY) -O binary --set-section-flags .bss=alloc,load,contents $^ $@

uImage: matcher.bin
	mkimage -A arm -O linux -C none -T kernel \
		-a $(LOADADDR) -e $(LOADADDR) \
		-n "ImpedanceMatcher (3rd stage)" -d $^ $@

clean:
	rm -fr matcher.bin matcher *.o uImage

distclean: clean
	rm -fr cscope.*

cscope:
	-find . -name "*.[chS]" >cscope.files
	-cscope -b -R
