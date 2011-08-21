# build executable on typing make
all: sd-idle-2.6

%.o: %.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -c -I. -Iinclude -o $@ $^

#simple: sd-idle-2.6.o
#	$(CC) -o $@ $^ -L.

#clean:
#	rm -f *.o simple

