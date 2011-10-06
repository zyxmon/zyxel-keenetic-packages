#
# Copyright (C) 2011 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=xupnpd
PKG_REV:=213
PKG_VERSION:=$(PKG_REV)
PKG_RELEASE:=1

PKG_SOURCE_PROTO:=svn
PKG_SOURCE_VERSION:=$(PKG_REV)
PKG_SOURCE_SUBDIR:=xupnpd-$(PKG_VERSION)
PKG_SOURCE_URL:=http://tsdemuxer.googlecode.com/svn/trunk/xupnpd/src/
PKG_SOURCE:=$(PKG_SOURCE_SUBDIR).tar.gz


include $(INCLUDE_DIR)/package.mk


TARGET_CFLAGS += -fno-exceptions -fno-rtti -DWITH_URANDOM

define Build/Compile
	$(TARGET_CC) $(TARGET_CFLAGS) -c -I$(PKG_BUILD_DIR) -I$(STAGING_DIR)/usr/include -o $(PKG_BUILD_DIR)/luaxcore.o $(PKG_BUILD_DIR)/luaxcore.cpp
	$(TARGET_CC) $(TARGET_CFLAGS) -c -I$(PKG_BUILD_DIR) -I$(STAGING_DIR)/usr/include -o $(PKG_BUILD_DIR)/luaxlib.o $(PKG_BUILD_DIR)/luaxlib.cpp
	$(TARGET_CC) $(TARGET_CFLAGS) -c -I$(PKG_BUILD_DIR) -I$(STAGING_DIR)/usr/include -o $(PKG_BUILD_DIR)/main.o $(PKG_BUILD_DIR)/main.cpp
	$(TARGET_CC) $(TARGET_CFLAGS) -c -I$(PKG_BUILD_DIR) -I$(STAGING_DIR)/usr/include -o $(PKG_BUILD_DIR)/mem.o $(PKG_BUILD_DIR)/mem.cpp
	$(TARGET_CC) $(TARGET_CFLAGS) -c -I$(PKG_BUILD_DIR) -I$(STAGING_DIR)/usr/include -o $(PKG_BUILD_DIR)/soap.o $(PKG_BUILD_DIR)/soap.cpp
	$(TARGET_CC) $(TARGET_CFLAGS) -c -I$(PKG_BUILD_DIR) -I$(STAGING_DIR)/usr/include -o $(PKG_BUILD_DIR)/mcast.o $(PKG_BUILD_DIR)/mcast.cpp
	$(TARGET_CC) $(TARGET_CFLAGS) -o $(PKG_BUILD_DIR)/xupnpd -L$(STAGING_DIR)/usr/lib \
		$(PKG_BUILD_DIR)/main.o $(PKG_BUILD_DIR)/soap.o $(PKG_BUILD_DIR)/luaxcore.o \
		$(PKG_BUILD_DIR)/mem.o $(PKG_BUILD_DIR)/luaxlib.o $(PKG_BUILD_DIR)/mcast.o -llua -lm -ldl
endef

define Package/xupnpd
  SECTION:=multimedia
  CATEGORY:=Multimedia
  DEPENDS:=+liblua
  TITLE:=eXtensible UPnP agent
  URL:=http://xupnpd.org/
endef

define Package/xupnpd/conffiles
/usr/share/xupnpd.lua
endef


define Package/xupnpd/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/xupnpd $(1)/usr/bin
	$(INSTALL_DIR) $(1)/usr/share/xupnpd
	$(CP) $(PKG_BUILD_DIR)/*.lua $(1)/usr/share/xupnpd
	$(INSTALL_DIR) $(1)/usr/share/xupnpd/ui
	$(CP) $(PKG_BUILD_DIR)/ui/* $(1)/usr/share/xupnpd/ui
	$(INSTALL_DIR) $(1)/usr/share/xupnpd/www
	$(CP) $(PKG_BUILD_DIR)/www/* $(1)/usr/share/xupnpd/www
	$(INSTALL_DIR) $(1)/usr/share/xupnpd/playlists
	$(CP) $(PKG_BUILD_DIR)/playlists/* $(1)/usr/share/xupnpd/playlists
	$(INSTALL_DIR) $(1)/usr/share/xupnpd/playlists/example
	$(CP) $(PKG_BUILD_DIR)/playlists/example/* $(1)/usr/share/xupnpd/playlists/example
endef

$(eval $(call BuildPackage,xupnpd))
