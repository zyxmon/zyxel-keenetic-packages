# 
# Copyright (C) 2006 OpenWrt.org
# Copyright (C) 2010,2011 NDM Systems
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=dnrd
PKG_VERSION:=2.20.3
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=@SF/dnrd
PKG_MD5SUM:=41c9b070aae8ed403fc8c2aac7ab157c

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)

include $(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)
  SECTION:=net
  CATEGORY:=Base system
  TITLE:=DNS caching daemon
  URL:=http://dnrd.sourceforge.net
endef

MAKE_FLAGS += CFLAGS="$(TARGET_CPPFLAGS) $(TARGET_CFLAGS) $(TARGET_LDFLAGS)"

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/src/dnrd $(1)/usr/sbin/
	$(INSTALL_DIR) $(1)/bin
	$(INSTALL_BIN) ./files/dns.sh $(1)/bin/
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
