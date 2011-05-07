#
# Copyright (C) 2010,2011 NDM Systems
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=wimax-drivers
PKG_RELEASE:=1

include $(INCLUDE_DIR)/package.mk

define KernelPackage/wimax-drivers
  SUBMENU:=Other modules
  TITLE:=Drivers for WiMAX
  FILES:= \
	$(PKG_BUILD_DIR)/samsung_u200/u200.$(LINUX_KMOD_SUFFIX)
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)/samsung_u200
	$(CP) ./samsung_u200/* $(PKG_BUILD_DIR)/samsung_u200
endef


define Build/Compile
	$(MAKE) -C "$(LINUX_DIR)" \
		CROSS_COMPILE="$(TARGET_CROSS)" \
	        ARCH="$(LINUX_KARCH)" \
	        SUBDIRS="$(PKG_BUILD_DIR)/samsung_u200" \
		EXTRA_CFLAGS="$(BUILDFLAGS)" \
	        modules
endef

$(eval $(call KernelPackage,wimax-drivers))
