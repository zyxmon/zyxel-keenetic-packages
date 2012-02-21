#
# Copyright (C) 2010,2011 NDM Systems
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.

include $(TOPDIR)/rules.mk

PKG_NAME:=in.www
PKG_VERSION:=1.0.0
PKG_RELEASE:=1

include $(INCLUDE_DIR)/package.mk

define Package/in.www
  SUBMENU:=Web Servers/Proxies
  SECTION:=net
  CATEGORY:=Network
  TITLE:=Inetd based web server
endef

define Package/in.www/description
  Inetd based web server
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) -r src/* $(PKG_BUILD_DIR)
endef

define Build/Compile
	"$(TARGET_CC)" $(TARGET_CFLAGS) -I $(STAGING_DIR)/include/ \
		-I $(STAGING_DIR)/usr/include/ -L $(STAGING_DIR)/lib/ \
		-s -o $(PKG_BUILD_DIR)/in.www \
		-DCGI \
				$(PKG_BUILD_DIR)/in.www.c
endef

define Package/in.www/install	
	$(INSTALL_DIR) $(1)/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/in.www $(1)/bin/in.www
endef

$(eval $(call BuildPackage,in.www))
