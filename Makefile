CFLAGS=-Wall
LDFLAGS=-static -nostdlib
GCC=$(CROSS_COMPILE)gcc
OBJCOPY=$(CROSS_COMPILE)objcopy
LD=$(CROSS_COMPILE)ld
LOADADDR=0xa0008000
BINFMT=elf32-littlearm

all: matcher.bin matcher.image

start.o: start.S
	$(GCC) -c $^

main.o: main.c
	$(GCC) $(CFLAGS) -c $^

print.o: print.c
	$(GCC) $(CFLAGS) -c $^

zimage-in.o: $(ZIMAGE_IN)
	$(OBJCOPY) -I binary -O $(BINFMT) -B arm $(ZIMAGE_IN) $@

dtb-in.o: $(DTB_IN)
	$(OBJCOPY) -I binary -O $(BINFMT) -B arm $(DTB_IN) $@

matcher: start.o main.o print.o zimage-in.o dtb-in.o
	$(LD) $(LDFLAGS) -T matcher.lds -o matcher $^

matcher.bin: matcher
	$(OBJCOPY) -O binary matcher matcher.bin

matcher.image: matcher.bin
	mkimage -A arm -O linux -C none -T kernel -a $(LOADADDR) -e $(LOADADDR) -n "ImpedanceMatcher (3rd stage)" -d matcher.bin uImage

clean:
	rm -fr matcher.bin matcher *.o uImage

