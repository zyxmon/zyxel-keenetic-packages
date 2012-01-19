#
# Copyright (C) 2006-2010 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=portmap
PKG_VERSION:=6.0
PKG_RELEASE:=3

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tgz
PKG_SOURCE_URL:=http://neil.brown.name/portmap/
PKG_MD5SUM:=ac108ab68bf0f34477f8317791aaf1ff

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)_$(PKG_VERSION)

PKG_BUILD_DEPENDS:=libwrap

include $(INCLUDE_DIR)/package.mk

define Package/portmap
  SECTION:=net
  CATEGORY:=Network
  TITLE:=The RPC Portmapper
  URL:=http://neil.brown.name/portmap/
endef

define Package/portmap/description
 Portmap is a server that converts RPC (Remote Procedure Call) program
 numbers into DARPA protocol port numbers.
endef

MAKE_FLAGS += \
	CFLAGS="$(TARGET_CFLAGS) $(TARGET_CPPFLAGS) -DFACILITY=LOG_DAEMON -DIGNORE_SIGCHLD" \
	RPCUSER="nobody" \
	NO_TCP_WRAPPER="yes" \
	all

define Package/portmap/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/portmap $(1)/usr/sbin/
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/S19portmap $(1)/etc/init.d/S19portmap
endef

$(eval $(call BuildPackage,portmap))
