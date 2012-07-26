#
# Copyright (C) 2006-2012 Zyxmon
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=fuse-convmvfs
PKG_VERSION:=0.2.6
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=@SF/fuse-convmvfs
#PKG_MD5SUM:=d52caf26159c905574c2ce4bd792b9ee

PKG_FIXUP:=libtool
PKG_INSTALL:=1

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/nls.mk

define Package/fuse-convmvfs
  SECTION:=utils
  CATEGORY:=Utilities
  DEPENDS:=$(ICONV_DEPENDS) +libfuse +libstdcpp
  TITLE:=fuse-convmvfs
  URL:=http://fuse-convmvfs.sourceforge.net/
endef

define Package/fuse-convmvfs/description
This is a filesystem converting file name from one character set to another. It's implemented by using FUSE.
endef

EXTRA_CFLAGS += \
	-I$(ICONV_PREFIX)/include \
	-I$(STAGING_DIR)/usr/include/fuse \
	-DICONV_CONST=const

EXTRA_LDFLAGS += \
	-L$(INTL_PREFIX)/lib \
	-L$(ICONV_PREFIX)/lib \
	-liconv

define Package/fuse-convmvfs/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/convmvfs $(1)/usr/bin/
endef

$(eval $(call BuildPackage,fuse-convmvfs))
