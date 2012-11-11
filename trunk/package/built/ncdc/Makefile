#
# Copyright (C) 2011-2012 Entware
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=ncdc
PKG_VERSION:=1.14
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=http://dev.yorhel.nl/download
PKG_MD5SUM:=c7a9f4ae51a561c1196456ae0b5718c3

PKG_FIXUP:=autoreconf
PKG_INSTALL:=1

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/nls.mk

define Package/ncdc
	SECTION:=net
	CATEGORY:=Network
	SUBMENU:=P2P
	TITLE:=ncdc is a modern and lightweight DC client
	DEPENDS:=+libncursesw +bzip2 +libgnutls +libsqlite3 +glib2 $(ICONV_DEPENDS) +libgmp
	URL:=http://dev.yorhel.nl/ncdc
	MAINTAINER:=Entware team, wl500g-repo.googlecode.com
endef

define Package/ncdc/description
 ncdc is a modern and lightweight direct connect client with a friendly
 ncurses interface.
endef

define Build/Compile
	$(HOSTCC) $(PKG_BUILD_DIR)/deps/makeheaders.c -o $(PKG_BUILD_DIR)/makeheaders
	$(call Build/Compile/Default)
endef

TARGET_CFLAGS += $(FPIC) -I$(PKG_BUILD_DIR)/lib/includes
TARGET_LDFLAGS += -Wl,-rpath-link=$(STAGING_DIR)/usr/lib,-rpath-link=$(STAGING_DIR)/usr/lib/libintl-full/lib

define Package/ncdc/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/ncdc $(1)/usr/bin/
endef

$(eval $(call BuildPackage,ncdc))
