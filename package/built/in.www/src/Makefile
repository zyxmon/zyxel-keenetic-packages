
all:	in.www

in.www:
	$(CC) $(CFLAGS) -o in.www in.www.c
	strip in.www
	cp in.www ../bin

clean:
	rm -f in.www
