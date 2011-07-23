#
# Copyright (C) 2011 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=pshare
PKG_VERSION:=0.0.2rc2
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)_$(PKG_VERSION)_src.tar.gz
PKG_SOURCE_URL:=http://tsdemuxer.googlecode.com/files/

include $(INCLUDE_DIR)/package.mk

PKG_UNPACK:=gzip -dc $(DL_DIR)/$(PKG_SOURCE) | $(TAR) -C $(PKG_BUILD_DIR) --strip-components=1 -xf -

TARGET_CFLAGS += -fno-exceptions -fno-rtti -DWITH_URANDOM

define Build/Compile
	$(TARGET_CC) $(TARGET_CFLAGS) -c -I$(PKG_BUILD_DIR) -I$(STAGING_DIR)/usr/include -o $(PKG_BUILD_DIR)/main.o $(PKG_BUILD_DIR)/main.cpp
	$(TARGET_CC) $(TARGET_CFLAGS) -c -I$(PKG_BUILD_DIR) -I$(STAGING_DIR)/usr/include -o $(PKG_BUILD_DIR)/upnp.o $(PKG_BUILD_DIR)/upnp.cpp
	$(TARGET_CC) $(TARGET_CFLAGS) -c -I$(PKG_BUILD_DIR) -I$(STAGING_DIR)/usr/include -o $(PKG_BUILD_DIR)/soap.o $(PKG_BUILD_DIR)/soap.cpp
	$(TARGET_CC) $(TARGET_CFLAGS) -c -I$(PKG_BUILD_DIR) -I$(STAGING_DIR)/usr/include -o $(PKG_BUILD_DIR)/tmpl.o $(PKG_BUILD_DIR)/tmpl.cpp
	$(TARGET_CC) $(TARGET_CFLAGS) -c -I$(PKG_BUILD_DIR) -I$(STAGING_DIR)/usr/include -o $(PKG_BUILD_DIR)/mem.o $(PKG_BUILD_DIR)/mem.cpp
	$(TARGET_CC) $(TARGET_CFLAGS) -c -I$(PKG_BUILD_DIR) -I$(STAGING_DIR)/usr/include -o $(PKG_BUILD_DIR)/proxy.o $(PKG_BUILD_DIR)/proxy.cpp
	$(TARGET_CC) $(TARGET_CFLAGS) -c -I$(PKG_BUILD_DIR) -I$(STAGING_DIR)/usr/include -o $(PKG_BUILD_DIR)/common.o $(PKG_BUILD_DIR)/common.cpp	
	$(TARGET_CC) $(TARGET_CFLAGS) -o $(PKG_BUILD_DIR)/pshare -L$(STAGING_DIR)/usr/lib $(PKG_BUILD_DIR)/main.o $(PKG_BUILD_DIR)/upnp.o $(PKG_BUILD_DIR)/soap.o $(PKG_BUILD_DIR)/tmpl.o $(PKG_BUILD_DIR)/mem.o $(PKG_BUILD_DIR)/proxy.o $(PKG_BUILD_DIR)/common.o
endef

define Package/pshare
  SECTION:=multimedia
  CATEGORY:=Multimedia
  TITLE:=UPnP Playlist Browser http://ps3muxer.org/pshare.html
  URL:=http://code.google.com/p/tsdemuxer/
endef

define Package/pshare/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/pshare $(1)/usr/bin
	$(INSTALL_DIR) $(1)/usr/share/pshare/playlists
	$(CP) $(PKG_BUILD_DIR)/playlists/* $(1)/usr/share/pshare/playlists/
	$(INSTALL_DIR) $(1)/usr/share/pshare/www
	$(CP) $(PKG_BUILD_DIR)/www/* $(1)/usr/share/pshare/www/
endef

$(eval $(call BuildPackage,pshare))
