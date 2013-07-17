DEBUG ?= n
KBUILD_VERBOSE:=1

usbip_common_mod-objs := usbip_common.o usbip_event.o
usbip-objs 	  := stub_dev.o stub_main.o stub_rx.o stub_tx.o
vhci-hcd-objs	  := vhci_sysfs.o vhci_tx.o vhci_rx.o vhci_hcd.o 

obj-m += usbip_common_mod.o
obj-m += usbip.o
obj-m += vhci-hcd.o

ifeq ($(DEBUG),y)
	EXTRA_CFLAGS += -DCONFIG_USB_DEBUG
endif

ifeq ($(MAKING_MODULES),1)
-include $(TOPDIR)/Rules.make
endif
