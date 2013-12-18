CFLAGS+=-I..

libfdt: atags_to_fdt.o fdt_rw.o fdt_ro.o fdt.o fdt_wip.o

atags_to_fdt.o: atags_to_fdt.c
	$(GCC) $(CFLAGS) -c $^

fdt_rw.o: fdt_rw.c
	$(GCC) $(CFLAGS) -c $^

fdt_ro.o: fdt_ro.c
	$(GCC) $(CFLAGS) -c $^

fdt.o: fdt.c
	$(GCC) $(CFLAGS) -c $^

fdt_wip.o: fdt_wip.c
	$(GCC) $(CFLAGS) -c $^

	ar rcs libfdt.a atags_to_fdt.o fdt_rw.o fdt_ro.o fdt.o fdt_wip.o

clean:
	rm -fr *.o *.a
