# 
# Copyright (C) 2010-2011 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
#
#
#
#

include $(TOPDIR)/rules.mk

PKG_NAME:=ldd
PKG_VERSION:=0.9.30.3
PKG_RELEASE:=61

#PKG_BUILD_PARALLEL:=0

include $(INCLUDE_DIR)/package.mk

define Package/ldd
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=LDD trace utility
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)
endef

#TARGET_CFLAGS += $(FPIC)

define Package/ldd/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/ldd $(1)/usr/bin
endef

$(eval $(call BuildPackage,ldd))
