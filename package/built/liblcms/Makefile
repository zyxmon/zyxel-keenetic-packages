#
# Copyright (C) 2006-2009 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=lcms
PKG_VERSION:=1.19
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=@SF/lcms
PKG_MD5SUM:=
PKG_INSTALL:=1

PKG_FIXUP:=libtool

PKG_BUILD_DEPENDS:=zlib

include $(INCLUDE_DIR)/package.mk

define Package/liblcms
  SECTION:=libs
  CATEGORY:=Libraries
  DEPENDS:= +libjpeg +libtiff
  TITLE:=A small-footprint, speed optimized color management engine.
  URL:=http://www.littlecms.com/
endef

TARGET_CFLAGS += $(FPIC)

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include
	$(CP) $(PKG_INSTALL_DIR)/usr/include/*.h $(1)/usr/include/
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/liblcms.{a,la,so*} $(1)/usr/lib/
endef

define Package/liblcms/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(CP) $(PKG_INSTALL_DIR)/usr/bin/* $(1)/usr/bin
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/liblcms.so* $(1)/usr/lib/
endef

$(eval $(call BuildPackage,liblcms))
