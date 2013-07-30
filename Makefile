ifneq ($(origin APPEND_KERNEL), undefined)
INPUT_OBJS=zimage.o
CFLAGS+=-DAPPEND_KERNEL="$(APPEND_KERNEL)"
endif

ifneq ($(origin APPEND_DTBS), undefined)
CFLAGS+=-DAPPEND_DTBS="$(APPEND_DTBS)"
endif

CFLAGS+=-Wall -ffreestanding
LDFLAGS=-static -nostdlib
GCC=$(CROSS_COMPILE)gcc
OBJCOPY=$(CROSS_COMPILE)objcopy
LD=$(CROSS_COMPILE)ld
LOADADDR=0x10008000
BINFMT=elf32-littlearm

MFG=raumfeld
UART=8250
UART_BASE=0xf1012000

CFLAGS+=-DUART_BASE=$(UART_BASE)

BOARD_OBJ = board-$(MFG).o
UART_OBJ = serial-$(UART).o

COMMON_OBJS = \
	main.o \
	print.o \
	register.o \
	string.o

all: uImage

zimage.o: $(APPEND_KERNEL)
	$(OBJCOPY) -I binary -O $(BINFMT) -B arm $^ $@

%.o: %.c
	$(GCC) $(CFLAGS) -c $^

matcher: $(COMMON_OBJS) $(BOARD_OBJ) $(UART_OBJ) $(INPUT_OBJS)
	$(LD) $(LDFLAGS) -T matcher.lds -Ttext $(LOADADDR) -o $@ $^

matcher.bin: matcher
	$(OBJCOPY) -O binary --set-section-flags .bss=alloc,load,contents $^ $@
ifneq ($(origin APPEND_DTBS), undefined)
	./append_dtbs.sh $@ $(APPEND_DTBS)
endif

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
