#
# Copyright (C) 2012 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
include $(TOPDIR)/rules.mk

PKG_NAME:=aria2
PKG_VERSION:=1.17.0
PKG_RELEASE:=1a

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.bz2
PKG_SOURCE_URL:=@SF/aria2
PKG_MD5SUM:=9e2b4f1103a05c6b087b8f380a82df5b
PKG_INSTALL:=1
PKG_BUILD_DEPENDS:= zlib

include $(INCLUDE_DIR)/package.mk

define Package/aria2
  SECTION:=net
  CATEGORY:=Network
  SUBMENU:=File Transfer
  TITLE:=lightweight download utility
  URL:=http://aria2.sourceforge.net/
  DEPENDS:=+libopenssl +libxml2 +libstdcpp \
    $(INTL_DEPENDS) $(ICONV_DEPENDS)
endef

define Package/aria2/description
 aria2 is a lightweight multi-protocol & multi-source command-line download
 utility
endef

define Package/aria2/conffiles
/etc/aria2/aria2.conf
endef

CONFIGURE_ARGS += \
	--disable-nls \
	--without-gnutls \
	--without-libnettle \
	--without-libgmp \
	--without-libgcrypt \
	--without-libexpat \
	--without-libcares \
	--without-sqlite3 \
	--with-openssl \
	--with-libxml2 \
	--with-libz

CONFIGURE_VARS += \
	ZLIB_CFLAGS="-I$(STAGING_DIR)/usr/include" \
	ZLIB_LIBS="-L$(STAGING_DIR)/usr/lib"

define Package/aria2/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/src/aria2c $(1)/usr/bin/
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/K87aria2c $(1)/etc/init.d/K87aria2c
	$(INSTALL_DIR) $(1)/etc/aria2
	$(CP) ./files/aria2.conf $(1)/etc/aria2/aria2.conf
endef

$(eval $(call BuildPackage,aria2))
