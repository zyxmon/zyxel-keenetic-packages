#
# Copyright (C) 2011-2012 Entware
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=lynx
PKG_VERSION:=2.8.7rel.2
PKG_RELEASE:=1a

PKG_SOURCE:=$(PKG_NAME)$(PKG_VERSION).tar.bz2
PKG_SOURCE_URL:=http://lynx.isc.org/current
PKG_MD5SUM:=cb936aef812e4e463ab86cbbe14d4db9

PKG_INSTALL:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)2-8-7
PKG_BUILD_DEPENDS:=zlib

include $(INCLUDE_DIR)/package.mk

define Package/lynx
	SECTION:=net
	CATEGORY:=Network
	DEPENDS:=+libopenssl +libbz2 +libslang2
	TITLE:=a text web browser
	URL:=http://lynx.isc.org/
	MAINTAINER:=Entware team, wl500g-repo.googlecode.com
endef

define Package/lynx/description
 Lynx is the text web browser
endef

CONFIGURE_ARGS += \
	--sysconfdir="$(ROOT_ZYX)/etc" \
	--with-ssl \
	--disable-ipv6 \
	--with-screen=slang \
	--enable-widec \
	--with-bzlib \
	--with-zlib \

define Package/lynx/conffiles
/etc/lynx.cfg
endef

define Package/lynx/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/lynx $(1)/usr/bin
	$(INSTALL_DIR) $(1)/etc
	$(INSTALL_CONF) $(PKG_INSTALL_DIR)$(ROOT_ZYX)/etc/lynx.cfg $(1)/etc
endef

$(eval $(call BuildPackage,lynx))
