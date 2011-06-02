#
# Copyright (C) 2011 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
# vovic67, mediatomb for Keenetic
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/nls.mk

PKG_NAME:=mediatomb
PKG_VERSION:=0.12.1
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=@SF/mediatomb
PKG_MD5SUM:=e927dd5dc52d3cfcebd8ca1af6f0d3c2

PKG_BUILD_PARALLEL:=0

include $(INCLUDE_DIR)/package.mk

define Package/mediatomb
  SECTION:=multimedia
  CATEGORY:=Multimedia
  TITLE:=A powerfull UPNP media server
  URL:=http://www.mediatomb.cc/
  DEPENDS:=+libexif +libsqlite3 +id3lib +file \
        +libexpat +libffmpeg +libstdcpp
endef

define Package/mediatomb/description
  MediaTomb is an open source (GPL) UPnP MediaServer with a web user interface and
  with the aim of being fully compliant with DLNA/UPnP-AV clients.
endef

CONFIGURE_ARGS += \
	--prefix=/usr \
	--enable-pthread-lib \
	--enable-inotify \
	--enable-db-autocreate \
	--enable-id3lib \
	--disable-taglib \
	--enable-libexif \
	--enable-libmagic \
	--enable-protocolinfo-extension \
	--disable-libextractor \
	--enable-ffmpeg \
	--disable-libjs \
	--disable-tombdebug \
	--disable-upnpdebug \
	--disable-sopcast \
	--disable-youtube \
	--disable-weborama \
	--disable-atrailers \
	--disable-curl \
	--enable-sqlite3 \
	--enable-sqlite-backup-defaults \
	--disable-mysql \
	--disable-external-transcoding \
	--disable-ffmpegthumbnailer \
	--disable-libmp4v2 \
	--disable-lastfmlib \
	--disable-ImageMagick \
	--enable-libexif \
	--with-iconv-libs=$(ICONV_PREFIX)/lib

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		HOSTCC="$(HOSTCC)" \
		DESTDIR="$(PKG_INSTALL_DIR)" \
		all install
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_BUILD_DIR)/build/libmediatomb.a $(1)/usr/lib/
endef

define Package/mediatomb/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_BUILD_DIR)/build/libmediatomb.a $(1)/usr/lib/
	$(INSTALL_DIR) $(1)/etc/mediatomb
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_DIR) $(1)/usr/share
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/mediatomb $(1)/usr/bin/
	$(CP)  $(PKG_INSTALL_DIR)/usr/share/* $(1)/usr/share/
	$(INSTALL_DATA) ./files/config.xml $(1)/etc/mediatomb/
	$(INSTALL_BIN) ./files/mediatomb.init $(1)/etc/init.d/mediatomb
endef

$(eval $(call BuildPackage,mediatomb))
