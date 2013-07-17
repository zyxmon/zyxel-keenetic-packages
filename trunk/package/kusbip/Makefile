#
# Copyright (C) 2008-2010 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=usbip
PKG_VERSION:=1
PKG_RELEASE:=1

include $(INCLUDE_DIR)/package.mk

define KernelPackage/usbip
  SUBMENU:=USB Support
  TITLE:=USB/IP kernel support
  FILES:= \
	$(PKG_BUILD_DIR)/usbip_common_mod.ko \
	$(PKG_BUILD_DIR)/usbip.ko \
	$(PKG_BUILD_DIR)/vhci-hcd.ko
endef

define KernelPackage/usbip/description
  Kernel module for USB/IP support
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) src/* $(PKG_BUILD_DIR)/
ifeq ($(CONFIG_TARGET_keeneticII),y)
	$(CP) files/hcd22.h $(PKG_BUILD_DIR)/hcd.h
	$(CP) files/hub22.h $(PKG_BUILD_DIR)/hub.h
else
	$(CP) files/hcd23.h $(PKG_BUILD_DIR)/hcd.h
	$(CP) files/hub23.h $(PKG_BUILD_DIR)/hub.h
endif
endef

define Build/Compile
	$(MAKE) -C $(LINUX_DIR) \
		CROSS_COMPILE="$(KERNEL_CROSS)" \
		ARCH="$(LINUX_KARCH)" \
		SUBDIRS="$(PKG_BUILD_DIR)" \
		KERNELDIR=$(LINUX_DIR) \
		CC="$(TARGET_CC)" \
		EXTRA_CFLAGS="-I$(PKG_BUILD_DIR)" \
		modules
endef

$(eval $(call KernelPackage,usbip))
