# build executable on typing make
all: ldd

%.o: %.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -c -I. -Iinclude -o $@ $^

ldd-k: ldd.o
	$(CC) -o $@ $^ -L.

clean:
	rm -f *.o ldd
