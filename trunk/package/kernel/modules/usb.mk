# $Id$

USB_MENU:=USB Support

define KernelPackage/ralink_dwc_otg
  SUBMENU:=$(USB_MENU)
  TITLE:=Ralink High Speed USB 2.0 DWC OTG driver
  KCONFIG:= \
	CONFIG_DWC_OTG \
	CONFIG_DWC_OTG_HOST_ONLY=y
  FILES:= \
	$(LINUX_DIR)/arch/mips/rt2880/lm.$(LINUX_KMOD_SUFFIX) \
	$(LINUX_DIR)/drivers/usb/dwc_otg/dwc_otg.$(LINUX_KMOD_SUFFIX)
  AUTOLOAD:=$(call AutoLoad,10,ralink_dwc_otg)
endef

define KernelPackage/ralink_dwc_otg/description
  Ralink High Speed USB 2.0 DWC OTG Driver Module.
endef

$(eval $(call KernelPackage,ralink_dwc_otg))

define KernelPackage/usb-audio
  TITLE:=Support for USB audio devices
  KCONFIG:= \
	CONFIG_USB_AUDIO \
	CONFIG_SND_USB_AUDIO
  $(call AddDepends/usb)
  $(call AddDepends/sound)
# For Linux 2.6.35+
ifneq ($(wildcard $(LINUX_DIR)/sound/usb/snd-usbmidi-lib.ko),)
  FILES:= \
	$(LINUX_DIR)/sound/usb/snd-usbmidi-lib.ko \
	$(LINUX_DIR)/sound/usb/snd-usb-audio.ko
else
  FILES:= \
	$(LINUX_DIR)/sound/usb/snd-usb-lib.ko \
	$(LINUX_DIR)/sound/usb/snd-usb-audio.ko
endif
endef

define KernelPackage/usb-audio/description
 Kernel support for USB audio devices
endef

$(eval $(call KernelPackage,usb-audio))

define KernelPackage/usb-serial-cp2101
  TITLE:=Support for Silicon Labs cp210x devices
  KCONFIG:=CONFIG_USB_SERIAL_CP2101
  FILES:=$(LINUX_DIR)/drivers/usb/serial/cp2101.ko
endef

define KernelPackage/usb-serial-cp210x/description
 Kernel support for Silicon Labs cp210x USB-to-Serial converters
endef

$(eval $(call KernelPackage,usb-serial-cp2101))


