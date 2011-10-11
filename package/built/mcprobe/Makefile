# 
# Copyright (C) 2010-2011 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
#
#
#
#

include $(TOPDIR)/rules.mk

PKG_NAME:=mcprobe
PKG_VERSION:=0
PKG_RELEASE:=1

#PKG_BUILD_PARALLEL:=0

include $(INCLUDE_DIR)/package.mk

define Package/mcprobe
  SECTION:=net
  CATEGORY:=Network
  TITLE:=Multicast test utility
  URL:= URL:=http://sourceforge.net/projects/udpxy
endef

define Package/mcprobe/description
  mcprobe is a test utility for multicast.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)
endef


define Package/mcprobe/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/mcprobe $(1)/usr/bin
endef

$(eval $(call BuildPackage,mcprobe))
