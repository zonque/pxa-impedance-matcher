CFLAGS=-Wall -ffreestanding
LDFLAGS=-static -nostdlib
GCC=$(CROSS_COMPILE)gcc
OBJCOPY=$(CROSS_COMPILE)objcopy
LD=$(CROSS_COMPILE)ld
LOADADDR=0xa0008000
BINFMT=elf32-littlearm

INPUT_OBJS = \
	zimage.o		\
	dtb-raumfeld-controller-0.o	\
	dtb-raumfeld-controller-1.o	\
	dtb-raumfeld-controller-2.o	\
	dtb-raumfeld-connector-0.o	\
	dtb-raumfeld-connector-1.o	\
	dtb-raumfeld-connector-2.o	\
	dtb-raumfeld-speaker-0.o	\
	dtb-raumfeld-speaker-1.o	\
	dtb-raumfeld-speaker-2.o

all: matcher.bin matcher.image

dtb-%.o: input/%.dtb
	$(OBJCOPY) -I binary -O $(BINFMT) -B arm $^ $@

zimage.o: input/zImage
	$(OBJCOPY) -I binary -O $(BINFMT) -B arm $^ $@

%.o: %.c
	$(GCC) $(CFLAGS) -c $^

matcher: main.o print.o board.o led.o $(INPUT_OBJS)
	$(LD) $(LDFLAGS) -T matcher.lds -o matcher $^

matcher.bin: matcher
	$(OBJCOPY) -O binary matcher matcher.bin

matcher.image: matcher.bin
	mkimage -A arm -O linux -C none -T kernel \
		-a $(LOADADDR) -e $(LOADADDR) \
		-n "ImpedanceMatcher (3rd stage)" -d $^ uImage

clean:
	rm -fr matcher.bin matcher *.o uImage

