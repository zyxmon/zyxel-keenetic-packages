obj-m := asix.o
asix-objs   :=

# dependencies
${asix-objs}: asix.h axusbnet.c axusbnet.h

ifeq ($(MAKING_MODULES),1)

-include $(TOPDIR)/Rules.make
endif


