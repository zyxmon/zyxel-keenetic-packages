PROG        = usb_modeswitch old_usb_modeswitch
CC          = gcc
CCFLAGS     = -l usb
RM          = /bin/rm -f
PREFIX	    = $(DESTDIR)/usr
SBINDIR	    = $(PREFIX)/sbin

.PHONY:     clean
all:        $(PROG)
$(PROG):
	      $(CC) $(CCFLAGS) -o $@ $@.c

clean:
	      $(RM) ./usb_modeswitch ./old_usb_modeswitch

install: all
	mkdir -p $(SBINDIR)
	install ./usb_modeswitch $(SBINDIR)
	install ./old_usb_modeswitch $(SBINDIR)

uninstall: clean
	$(RM) /usr/sbin/usb_modeswitch /usr/sbin/old_usb_modeswitch

.PHONY:     clean install uninstall
