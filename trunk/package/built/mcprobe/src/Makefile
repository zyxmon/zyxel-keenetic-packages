# build executable on typing make
all: mcprobe

%.o: %.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -c -I. -Iinclude -o $@ $^

mcprobe: mcprobe.o
	$(CC) -o $@ $^ -L.

clean:
	rm -f *.o mcprobe
