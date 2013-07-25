LDFLAGS=-static -nostdlib
GCC=$(CROSS_COMPILE)gcc
OBJCOPY=$(CROSS_COMPILE)objcopy
LOADADDR=0xa0008000

all: matcher.bin matcher.image

start.o: start.S
	$(GCC) -c $^

main.o: main.c
	$(GCC) -c $^

matcher: start.o main.o
	$(GCC) $(LDFLAGS) -o matcher $^

matcher.bin: matcher
	$(OBJCOPY) -O binary matcher matcher.bin

matcher.image: matcher.bin
	#$(OBJCOPY) -O binary --pad-to 0x1ffff --set-start 0xa0008000 matcher matcher.image
	mkimage -A arm -O linux -C none -T kernel -a $(LOADADDR) -e $(LOADADDR) -n "ImpedanceMatcher (3rd stage)" -d matcher.bin uImage

clean:
	rm -fr matcher.bin matcher *.o uImage

