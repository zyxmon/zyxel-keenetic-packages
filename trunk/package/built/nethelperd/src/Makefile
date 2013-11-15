PROG        = nethelperd
CC          = gcc
RM          = /bin/rm -f
PREFIX	    = $(DESTDIR)/usr
SBINDIR	    = $(PREFIX)/sbin

.PHONY:     clean
all:        $(PROG)
$(PROG):
	      $(CC) -o $@ $@.c

clean:
	      $(RM) ./nethelperd

install: all
	mkdir -p $(PREFIX)
	install ./nethelperd $(PREFIX)

uninstall: clean
	$(RM) $(PREFIX)/nethelperd 

.PHONY:     clean install uninstall
