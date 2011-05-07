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
